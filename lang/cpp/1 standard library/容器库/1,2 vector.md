# `std::vector<T>`

​		`std::vector<T>` 是提供了内存管理的动态数组。

​		 `<vector>`

---

​		`std::vector<T>`是动态数组，可以自动的进行扩容

- 元素超出`vector`当前容量就会进行扩容
- 数组意味着连续存储
- 因为连续，扩容意味着重新分配内存，会拷贝原来的数据到新的内存地址上，除非元素支持移动
- 为保持连续，在非头尾的元素进行插入和删除时会有移动开销

## 概览

```c++
#include <compare>
#include <initializer_list>
 
namespace std {
  // 类模板 vector
  template<class T, class Allocator = allocator<T>> class vector;
 
  template<class T, class Allocator>
    constexpr bool operator==(const vector<T, Allocator>& x,
                              const vector<T, Allocator>& y);
  template<class T, class Allocator>
    constexpr /*synth-three-way-result*/<T> operator<=>(const vector<T, Allocator>& x,
                                                        const vector<T, Allocator>& y);
 
  template<class T, class Allocator>
    constexpr void swap(vector<T, Allocator>& x, vector<T, Allocator>& y)
      noexcept(noexcept(x.swap(y)));
 
  template<class T, class Allocator, class U>
    constexpr typename vector<T, Allocator>::size_type
      erase(vector<T, Allocator>& c, const U& value);
  template<class T, class Allocator, class Predicate>
    constexpr typename vector<T, Allocator>::size_type
      erase_if(vector<T, Allocator>& c, Predicate pred);
 
  // 类 vector<bool>
  template<class Allocator> class vector<bool, Allocator>;
 
  // 散列支持
  template<class T> struct hash;
  template<class Allocator> struct hash<vector<bool, Allocator>>;
 
  namespace pmr {
    template<class T>
      using vector = std::vector<T, polymorphic_allocator<T>>;
  }
}
```

```c++
namespace std {
  template<class T, class Allocator = allocator<T>>
  class vector {
  public:
    // 类型
    using value_type             = T;
    using allocator_type         = Allocator;
    using pointer                = typename allocator_traits<Allocator>::pointer;
    using const_pointer          = typename allocator_traits<Allocator>::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = /* 由实现定义 */;
    using difference_type        = /* 由实现定义 */;
    using iterator               = /* 由实现定义 */;
    using const_iterator         = /* 由实现定义 */;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 
    // 构造/复制/销毁
    constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) { }
    constexpr explicit vector(const Allocator&) noexcept;
    constexpr explicit vector(size_type n, const Allocator& = Allocator());
    constexpr vector(size_type n, const T& value, const Allocator& = Allocator());
    template<class InputIt>
      constexpr vector(InputIt first, InputIt last, const Allocator& = Allocator());
    constexpr vector(const vector& x);
    constexpr vector(vector&&) noexcept;
    constexpr vector(const vector&, const Allocator&);
    constexpr vector(vector&&, const Allocator&);
    constexpr vector(initializer_list<T>, const Allocator& = Allocator());
    constexpr ~vector();
    constexpr vector& operator=(const vector& x);
    constexpr vector& operator=(vector&& x)
      noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
               allocator_traits<Allocator>::is_always_equal::value);
    constexpr vector& operator=(initializer_list<T>);
    template<class InputIt>
      constexpr void assign(InputIt first, InputIt last);
    constexpr void assign(size_type n, const T& u);
    constexpr void assign(initializer_list<T>);
    constexpr allocator_type get_allocator() const noexcept;
 
    // 迭代器
    constexpr iterator               begin() noexcept;
    constexpr const_iterator         begin() const noexcept;
    constexpr iterator               end() noexcept;
    constexpr const_iterator         end() const noexcept;
    constexpr reverse_iterator       rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator       rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
 
    constexpr const_iterator         cbegin() const noexcept;
    constexpr const_iterator         cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;
 
    // 容量
    [[nodiscard]] constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;
    constexpr void      resize(size_type sz);
    constexpr void      resize(size_type sz, const T& c);
    constexpr void      reserve(size_type n);
    constexpr void      shrink_to_fit();
 
    // 元素访问
    constexpr reference       operator[](size_type n);
    constexpr const_reference operator[](size_type n) const;
    constexpr const_reference at(size_type n) const;
    constexpr reference       at(size_type n);
    constexpr reference       front();
    constexpr const_reference front() const;
    constexpr reference       back();
    constexpr const_reference back() const;
 
    // 数据访问
    constexpr T*       data() noexcept;
    constexpr const T* data() const noexcept;
 
    // 修改器
    template<class... Args> constexpr reference emplace_back(Args&&... args);
    constexpr void push_back(const T& x);
    constexpr void push_back(T&& x);
    constexpr void pop_back();
 
    template<class... Args>
      constexpr iterator emplace(const_iterator position, Args&&... args);
    constexpr iterator insert(const_iterator position, const T& x);
    constexpr iterator insert(const_iterator position, T&& x);
    constexpr iterator insert(const_iterator position, size_type n, const T& x);
    template<class InputIt>
      constexpr iterator insert(const_iterator position,
                                InputIt first, InputIt last);
    constexpr iterator insert(const_iterator position, initializer_list<T> il);
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    constexpr void     swap(vector&)
      noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
               allocator_traits<Allocator>::is_always_equal::value);
    constexpr void     clear() noexcept;
  };
 
  template<class InputIt, class Allocator = allocator</*iter-value-type*/<InputIt>>>
    vector(InputIt, InputIt, Allocator = Allocator())
      -> vector</*iter-value-type*/<InputIt>, Allocator>;
 
  // 交换
  template<class T, class Allocator>
    constexpr void swap(vector<T, Allocator>& x, vector<T, Allocator>& y)
      noexcept(noexcept(x.swap(y)));
}
```

```c++
// std::vector<bool> 特化
namespace std {
  template<class Allocator>
  class vector<bool, Allocator> {
  public:
    // 类型
    using value_type             = bool;
    using allocator_type         = Allocator;
    using pointer                = /* 由实现定义 */;
    using const_pointer          = /* 由实现定义 */;
    using const_reference        = bool;
    using size_type              = /* 由实现定义 */;
    using difference_type        = /* 由实现定义 */;
    using iterator               = /* 由实现定义 */;
    using const_iterator         = /* 由实现定义 */;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 
    // 位引用
    class reference {
      friend class vector;
      constexpr reference() noexcept;
    public:
      constexpr reference(const reference&) = default;
      constexpr ~reference();
      constexpr operator bool() const noexcept;
      constexpr reference& operator=(const bool x) noexcept;
      constexpr reference& operator=(const reference& x) noexcept;
      constexpr void flip() noexcept;   // 翻转位
    };
 
    // 构造/复制/销毁
    constexpr vector() : vector(Allocator()) { }
    constexpr explicit vector(const Allocator&);
    constexpr explicit vector(size_type n, const Allocator& = Allocator());
    constexpr vector(size_type n, const bool& value, const Allocator& = Allocator());
    template<class InputIt>
      constexpr vector(InputIt first, InputIt last, const Allocator& = Allocator());
    constexpr vector(const vector& x);
    constexpr vector(vector&& x);
    constexpr vector(const vector&, const Allocator&);
    constexpr vector(vector&&, const Allocator&);
    constexpr vector(initializer_list<bool>, const Allocator& = Allocator()));
    constexpr ~vector();
    constexpr vector& operator=(const vector& x);
    constexpr vector& operator=(vector&& x);
    constexpr vector& operator=(initializer_list<bool>);
    template<class InputIt>
      constexpr void assign(InputIt first, InputIt last);
    constexpr void assign(size_type n, const bool& t);
    constexpr void assign(initializer_list<bool>);
    constexpr allocator_type get_allocator() const noexcept;
 
    // 迭代器
    constexpr iterator               begin() noexcept;
    constexpr const_iterator         begin() const noexcept;
    constexpr iterator               end() noexcept;
    constexpr const_iterator         end() const noexcept;
    constexpr reverse_iterator       rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator       rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
 
    constexpr const_iterator         cbegin() const noexcept;
    constexpr const_iterator         cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;
 
    // 容量
    [[nodiscard]] constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr size_type capacity() const noexcept;
    constexpr void      resize(size_type sz, bool c = false);
    constexpr void      reserve(size_type n);
    constexpr void      shrink_to_fit();
 
    // 元素访问
    constexpr reference       operator[](size_type n);
    constexpr const_reference operator[](size_type n) const;
    constexpr const_reference at(size_type n) const;
    constexpr reference       at(size_type n);
    constexpr reference       front();
    constexpr const_reference front() const;
    constexpr reference       back();
    constexpr const_reference back() const;
 
    // 修改器
    template<class... Args> constexpr reference emplace_back(Args&&... args);
    constexpr void push_back(const bool& x);
    constexpr void pop_back();
    template<class... Args>
      constexpr iterator emplace(const_iterator position, Args&&... args);
    constexpr iterator insert(const_iterator position, const bool& x);
    constexpr iterator insert(const_iterator position, size_type n, const bool& x);
    template<class InputIt>
      constexpr iterator insert(const_iterator position,
                                InputIt first, InputIt last);
    constexpr iterator insert(const_iterator position, initializer_list<bool> il);
 
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    constexpr void swap(vector&);
    constexpr static void swap(reference x, reference y) noexcept;
    constexpr void flip() noexcept;     // 翻转所有位
    constexpr void clear() noexcept;
  };
}
```



## 初始化

### 默认初始化

```c++
// no need to specify length at the declaration
std::vector<double> values;
std::vector<double> values2{};
```

- 不同于`std::array<T,N>`，动态数组无须指定长度
- 容量为0，没有实际分配元素的存储空间
- 添加第一个元素时会按照`alloctor`的策略自动分配内存



### 列表初始化

```c++
std::vector<int> array2 = { 9, 7, 5, 3, 1 }; // use initializer list to initialize array (before C++11)
std::vector<int> array3 { 9, 7, 5, 3, 1 }; // use uniform initialization to initialize array

// as with std::array, the type can be omitted since C++17
std::vector array4 { 9, 7, 5, 3, 1 }; // deduced to std::vector<int>
```

### 实参初始化

``` c++
std::vector<double> values(20); // {...}[20]
std::vector<double> values2{20}; // {20}[1]
```

- 虽然`{}`也能进行实参初始化，但在具有二义性时，优先进行列表初始化

- 当`{}`能够决议到实参初始化时也应当尽量使用`()`来进行实参初始化

  ```c++
  //std::vector<std::string> words4{5, "Mo"};   
  std::vector<std::string> words4(5, "Mo");
  std::cout << "words4: " << words4 << '\n';
  ```

- 在创建`std::vector`时指定一个合适的初始大小，能够有效避免在后续插入时，前若干次插入导致的频繁扩容

  > 标准库的分配算法通常是一个常对数，较小的值计算得到的值仍然较小。随着值变大，增量比例虽然减小，但增量值仍在变大。



### 拷贝初始化

​		可以用元素相同的容器来初始化`std::vector<T>`。

```c++
vector( const vector& other ); // (C++20 前)
constexpr vector( const vector& other ); // (C++20 起)

vector( const vector& other, const Allocator& alloc ); // (C++11 起) (C++20 前)
constexpr vector( const vector& other, const Allocator& alloc ); //  (C++20 起)

// std::vector<std::string> words{"one", "two", "three", "four", "five"};
// auto words_copy { words };
```

​		或通过一堆迭代器来指定拷贝初始化的范围：

```c++
template< class InputIt >
vector( InputIt first, InputIt last,
	const Allocator& alloc = Allocator() );// (C++20 前)
template< class InputIt >
constexpr vector( InputIt first, InputIt last,
	const Allocator& alloc = Allocator() ); // (C++20 起)

// std::vector<std::string> words{"one", "two", "three", "four", "five"};
// auto words_copy{ std::begin(words), std::end(words) };
```



### 移动初始化

```c++
vector( vector&& other );// (C++11 起) (C++17 前)
vector( vector&& other ) noexcept;//(C++17 起)  (C++20 前)
constexpr vector( vector&& other ) noexcept; // (C++20 起)

vector( vector&& other, const Allocator& alloc );// (C++11 起) (C++20 前)
constexpr vector( vector&& other, const Allocator& alloc ); // (C++20 起)
```

```c++
std::vector<std::string> words{"one", "two", "three", "four", "five"};
auto words_copy{ std::make_move_iterator(std::begin(words)), std::make_move_iterator(std::end(words)) };
// 此时 words不应当再
```





### `std::vector<bool>`

​		`std::vector<bool>`在内部实现上，会将原本一个字节的`bool`类型，压缩用单个``bit``来存储，每8个值占用一个字节。

```c++
std::vector<bool> array { true, false, false, true, true };
std::cout << "The length is: " << array.size() << '\n';

for (int i : array)
  std::cout << i << ' ';
```

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

- `reserve()`

  ```c++
  std::vector<double> values2{};
  values2.reverse(20);
  ```

  


### 访问

- `operator[]`
- `at()`

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

### 整体操作

- `swap()`
- ``data()`

