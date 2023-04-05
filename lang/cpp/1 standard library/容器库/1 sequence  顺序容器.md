# 顺序容器

​		顺序容器以线性序列的方式存储元素，不对元素进行排序，元素顺序与插入他们的顺序相同。

​		有五种标准顺序容器：

- `std::array<T, N>`， 数组容器，长度固定，不能增删元素
- `std::vector<T>`， 向量容器，长度单向可变，自动扩容
- `std::deque<T>`，双端队列容器，长度双向可变，自动扩容

- `std::list<T>`， 双向链表容器，长度可变，高效增删，低效随机访问
- `std::forward_list<T>`，单向链表容器，长度可变，单向查找



# std::vector

​		`<vector>`

---

​		单向增长的动态数组，因此又称向量，数据结构中称为`arraylist`。

- 元素超出`vector`当前容量就会进行扩容
  - 代价是小长度时频繁重建内存和拷贝元素
  - 因为连续，扩容意味着重新分配内存，会拷贝原来的数据到新的内存地址上，除非元素支持移动
- 数组意味着连续存储，随机访问
- 可以指针偏移
- 为保持连续，在非头尾的元素进行插入和删除时会有移动开销



## `std::vector`的内存管理与效率

- 当添加元素时，如果空间不足会进行扩容
- 为保持连续和随机访问特性，扩容会以类似`realloc`的方式分配
- 基于`std::allocator`提供内存分配策略，大概在`1.5-2`倍之间
- `capacity()`获取存储空间容量
- `size()`获取当前已用的存储单元个数
- `resize()`检查已用存储单元个数，并重置为指定个数，在此过程中，添加默认元素会造成扩容，删减元素不会缩容
- `reverse()`检查存储空间容量，并重置为容纳不小于指定个数元素的大小



### 扩容测试

```c++

void test_expand()
{
    std::vector<int> iv;
    std::cout << "capa: " << iv.capacity() << "  " << "size: " << iv.size() << '\n';

    for ( int i = 0; i < 1000000; ++i ) {
        if (iv.capacity() == iv.size())
            std::cout << "capa: " << iv.capacity() << "  " << "size: " << iv.size() << '\n';
        iv.push_back(1);
    }
}
// 在linux上， 始终是以*2的方式进行扩容的
```



### 通过`reverse()`避免过频繁的扩容

```c++
std::vector<int> iv;
iv.reserve(2048);

for ( int i = 0; i < 10000000; ++i ) {
    iv.push_back(1);
    if (iv.capacity() == iv.size())
        std::cout << "capa: " << iv.capacity() << "  " << "size: " << iv.size() << '\n';
}
```



### 使用`swap()`进行缩容

- shrink to fit

  ```c++
  vector<int>(ivec).swap(ivec); 
  ivec.shrink_to_fit();
  ```

- clear

  ```c++
  template<class T>
      void clearVec(vector<T>& v)
  {
  	vector<T> vt;
      vt.swap(v);
      
      //iv.clear();
      //iv.shrink_to_fit();
  }
  ```



## implement

```c++
template<typename T>
class sequence_list 
{
public:
    explicit sequence_list(std::ssize_t count = 1);
    ~sequence_list();

    // copy sementic
    sequence_list(const sequence_list &oth);
    sequence_list &operator=(const sequence_list &oth);

    // move sementic
    sequence_list(sequence_list &&oth);
    sequence_list &operator=(sequence_list &&oth);

public:
    //* interface

    void clear();
    bool empty() const;
    
    std::ssize_t length() const;
    std::ssize_t size() const;
    std::ssize_t capacity() const;

    T &operator[](std::ssize_t index);
    const T &operator[](std::ssize_t index) const;

    T &at(std::ssize_t index);
    const T &at(std::ssize_t index) const;

    std::ssize_t find(const T &value
        , std::function<bool(const T &, const T &)> &&eq = [](const T &t1, const T &t2) {return t1 == t2;}) const;

    T &prior(std::ssize_t index);
    T &next(std::ssize_t index);

    void insert(std::ssize_t index, T &&t);
    
    //T&& remove(std::size_t index); 
    T erase(std::ssize_t index);
    // size_t implicit convert cause ambiguity problem. 
    // 1. use grammer, eg meta programming  
    // 2. use technique, Abstract elements and iterators, not use base type for position
    bool remove(const T & value);

    void travel(std::function<void(const T &)>&& visitor) const;

private:
    T *             elems_{ nullptr };
    std::ssize_t     size_{};
    std::ssize_t     capacity_{};
};


template<typename T>
sequence_list<T>::sequence_list(std::ssize_t count)
{
    elems_ = new T[count];
    assert(elems_ and "bad allocate");
    size_ = 0;
    capacity_ = count;
}

template<typename T>
sequence_list<T>::sequence_list(const sequence_list &oth)
{
    delete [] elems_;
    
    elems_ = new T[oth.capacity_];
    assert(elems_ and "bad allocate");
    size_ = oth.size_;
    capacity_ = oth.capacity_;

    for ( std::ssize_t i{ 0 }; i < oth.size_; ++i )
        elems_[i] = oth.elems_[i];
}

template<typename T>
sequence_list<T>& sequence_list<T>::operator=(const sequence_list<T>& oth)
{
    if ( this == &oth )
        return *this;

    delete [] elems_;

    elems_ = new T[oth.capacity_];
    assert(elems_ and "bad allocate");
    size_ = oth.size_;
    capacity_ = oth.capacity_;

    for ( std::ssize_t i{ 0 }; i < oth.size_; ++i )
        elems_[i] = oth.elems_[i];

    return *this;
}

template<typename T>
sequence_list<T>::~sequence_list()
{
    delete [] elems_;
}

//* interface
template<typename T> inline
std::ssize_t sequence_list<T>::size() const
{
    return size_;
}

template<typename T> inline
std::ssize_t sequence_list<T>::length() const
{
    return size_;
}

template<typename T> inline
std::ssize_t sequence_list<T>::capacity() const
{
    return capacity_;
}


template<typename T> inline
void sequence_list<T>::clear() 
{
    size_ = 0;
}

template<typename T> inline
bool sequence_list<T>::empty() const 
{
    return size_ == 0;
}

template<typename T> inline
T &sequence_list<T>::operator[](std::ssize_t index)
{
    assert(index < size_ and "bad index");
    return elems_[index];
}

template<typename T> inline
const T &sequence_list<T>::operator[](std::ssize_t index) const
{
    assert(index < size_ and "bad index");
    return elems_[index];
}

template<typename T> inline
T &sequence_list<T>::at(std::ssize_t index)
{
    assert(index < size_ and "bad index");
    return elems_[index];
}

template<typename T> inline
const T &sequence_list<T>::at(std::ssize_t index) const
{
    assert(index < size_ and "bad index");
    return elems_[index];
}

template<typename T>
std::ssize_t sequence_list<T>::find(const T &value, std::function<bool(const T&, const T&)>&& eq) const
{
    for ( std::ssize_t i{ 0 }; i < size_; ++i )
        if ( eq(elems_[i], value) )
            return i;
    return -1;
}

template<typename T> inline
T &sequence_list<T>::prior(std::ssize_t index)
{
    return at(index - 1);
}

template<typename T> inline
T &sequence_list<T>::next(std::ssize_t index)
{
    return at(index + 1);
}

template<typename T>
void sequence_list<T>::insert(std::ssize_t index, T &&t)
{
    assert((index >= 0 or index <= size_) and "bad index");

    // check capacity
    if ( size_ == capacity_ ) {
        T *new_elems = new T[size_ * 2];
        assert(new_elems and "bad allocate");
        for ( std::ssize_t i{ 0 }; i < index; ++i )
            new_elems[i] = elems_[i];

        new_elems[index] = std::move(t);

        for ( std::ssize_t i{ index }; i < size_; ++i )
            new_elems[i + 1] = elems_[i];

        delete [] elems_;
        elems_ = new_elems;
        capacity_ *= 2;
    } else {
        for ( std::ssize_t i{ size_ }; i > index; --i ) 
            elems_[i] = elems_[i - 1];
        
        elems_[index] = std::move(t);
    }
    
    size_++;
}

template<typename T>
// T &&sequence_list<T>::remove(typename std::enable_if<std::is_same<T, std::size_t>::type>::type* = 0)
T sequence_list<T>::erase(std::ssize_t index)
{
    assert((index >= 0 or index <= size_) and "bad index");

    if ( auto after_size{ size_ - 1 }; after_size <= capacity_ / 4 ) {
        auto new_elems = new T[capacity_ / 2];
        assert(new_elems and "bad allocate");
        for ( std::ssize_t i{ 0 }; i < index; ++i )
            new_elems[i] = elems_[i];
        capacity_ /= 2;

        auto value = elems_[index];

        for ( std::ssize_t i{ index }; i < size_; ++i )
            new_elems[i] = elems_[i + 1];
        delete elems_;
        elems_ = new_elems;

        --size_;
        return value;
    } else {
        auto &&value = elems_[index];

        for ( std::ssize_t i{ index }; i < size_; ++i )
            elems_[i] = elems_[i + 1];
        --size_;

        return value;
    }
}

template<typename T>
bool sequence_list<T>::remove(const T &value)
{
    if ( auto index{ find(value) }; index not_eq -1 ) {
        erase(index);
        return true;
    } else {
        return false;
    }
}

template<typename T>
void sequence_list<T>::travel(std::function<void(const T &)>&& visitor) const
{
    for ( std::ssize_t i{0}; i < size_; ++i )
        visitor(elems_[i]);
}
```



## 基本使用

### 声明/初始化

```c++
// defualt
std::vector<double> values;
std::vector<double> values2{};

// single parameter init
std::vector<double> values(20);	// {...}[20]
std::vector<double> values2{20}; // {20}[1]

// list init
std::vector<std::string> words{"one", "two", "three", "four", "five"};
std::vector<int> array2 = { 9, 7, 5, 3, 1 }; // use initializer list to initialize array (before C++11)

// copy init
std::vector<std::string> words{"one", "two", "three", "four", "five"};
auto words_copy = words;
auto words_copy{ std::begin(words), std::end(words) };

// move init
std::vector<std::string> words{"one", "two", "three", "four", "five"};
auto words_copy{ std::make_move_iterator(std::begin(words)), std::make_move_iterator(std::end(words)) };
```



### 访问

```c++

```



### 删除

```c++
for(auto it = vec.begin(); it not_eq vec.end(); it++)
    if(*it == 3)
        vec.erase(it);
```

​		在循环中使用迭代器改变容器的存储结构，会导致迭代器失效。其本质是迭代器的底层指针成为野指针。

> MSDN中讲`erase`的返回值是，“An iterator that designates the first element remaining beyond any elements removed, or a pointer to the end of the vector if no such element exists”
>
> > 一个迭代器，用于指定除已移除元素之后的第一个元素，或者一个指向``vector``对象末尾的指针(如果不存在该元素)

​		为避免迭代器失效，代码应改为

```c++
for(auto it = vec.begin(); it not_eq vec.end(); )
    if(*it == 3)
        it = vec.erase(it);
	else
        ++it;
```





### 比较

```c++
std::vector<int> alice{1, 2, 3};
std::vector<int> bob{7, 8, 9, 10};
std::vector<int> eve{1, 2, 3};

std::cout << std::boolalpha;

// 比较不相等的容器
std::cout << "alice == bob returns " << (alice == bob) << '\n';
std::cout << "alice != bob returns " << (alice != bob) << '\n';
std::cout << "alice <  bob returns " << (alice < bob) << '\n';
std::cout << "alice <= bob returns " << (alice <= bob) << '\n';
std::cout << "alice >  bob returns " << (alice > bob) << '\n';
std::cout << "alice >= bob returns " << (alice >= bob) << '\n';

std::cout << '\n';

// 比较相等的容器
std::cout << "alice == eve returns " << (alice == eve) << '\n';
std::cout << "alice != eve returns " << (alice != eve) << '\n';
std::cout << "alice <  eve returns " << (alice < eve) << '\n';
std::cout << "alice <= eve returns " << (alice <= eve) << '\n';
std::cout << "alice >  eve returns " << (alice > eve) << '\n';
std::cout << "alice >= eve returns " << (alice >= eve) << '\n';
```



### 排序

```c++
typedef struct Rect
{
    int id, length, width;

    bool operator<(const Rect &a) const
    {
        if ( id not_eq a.id )
            return id < a.id;
        else if ( length not_eq a.length )
            return length < a.length;
        else
            return width < a.width;
    }
} rect_t;

std::vector<Rect> vec {
    {2, 3, 4},
    { 1, 2,3 }
};

for ( const auto &rec : vec )
    std::cout << rec.id << ' ' << rec.length << ' ' << rec.width << '\n';

// std::sort(vec.begin(), vec.end());
std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b) {
    if ( b.id not_eq a.id )
        return b.id < a.id;
    else if ( b.length not_eq a.length )
        return b.length < a.length;
    else
        return b.width < a.width;
});

for ( const auto &rec : vec )
    std::cout << rec.id << ' ' << rec.length << ' ' << rec.width << '\n';
```



# std::deque

## 操作

### 迭代器

- `begin() / end()`
- `rbegin() / rend()`
- `cbegin() / cend()`
- `crbegin() / crend()`

### 属性

- `size()`
- `max_size()`
- `empty()`
- `capacity()`
- `resize()`

### 访问

- `at()`
- `[]`
- `front()/back()`

### 赋值

- `={}`
- `[]=`
- `assign()`

### 增删

- `push_back()/pop_back`
- `insert`
- `emplace() / emplace_back()`
- `erase()`
- `clear()`
- `swap()`



# std::list

## 操作

### 迭代器

- `begin() / end()`
- `rbegin() / rend()`
- `cbegin() / cend()`
- `crbegin() / crend()`

### 属性

- `size()`
- `max_size()`
- `empty()`
- `resize()`
- `reverse`

### 访问

- `front()/back()`

### 赋值

- `={}`
- `assign()`

### 增删

- `swap()`
- `push_back/push_front`
- `emplace`
- `emplace_back/emplace_front`
- `insert`
- `pop_front/pop_back`
- `erase`
- `remove()`
- `remove_if`()
- `unique()`
- `clear()`
- `sort()`
- `merge()`
- `splice()`

# std::forward_list

## 操作

### 迭代器

- `begin() / end()`

- `cbegin() / cend()`

- `before_begin / cbefore_begin`

  

### 属性

- `max_size()`
- `empty()`
- `resize()`
- `reverse()`

### 访问

- `front()`

### 赋值

- `={}`
- `assign()`

### 增删

- `swap()`
- `push_front`
- `emplace_after`
- `emplace_front`
- `insert_after`
- `pop_front`
- `erase_after`
- `remove()`
- `remove_if`()
- `unique()`
- `clear()`
- `sort()`
- `merge()`
- `splice_after()`
