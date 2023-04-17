# STL container

---

​		很多操作是基于值的集合。保存这些值的集合，并提供操作接口的类称为容器。编程中很重要的部分是基于容器及其基本操作来构建的。

​		STL包含三种基本的容器类型：顺序容器、关联容器和容器适配器。

​		容器在动态内存中存放资源并以RAII的方式进行资源管理。

​		容器内的空间分配策略由`allocator`决定，由容器的抽象统一访问机制由`iterator`提供。

## 元素

​		几乎任何类型都有资格成为元素类型:内置数字类型(如``char、int和double``)、用户定义类型(如``string、Entry、list<int>和Matrix<double,2>``)和指针(如``const char*、Shape*和double*``)。

- 元素不是一个指向对象的引用或指针。

  ​	这使得容器美观、紧凑、访问速度快。对于关心内存大小和运行时性能的人来说，这是至关重要的。

- 如果你有一个依赖虚函数来获得多态行为的类层次结构，不要直接将对象存储在容器中。

```c++
vector<Shape> vs;                                  // No, don't - there is no room for a Circle or a Smiley (§5.5)
vector<Shape*> vps;                              // better, but see §5.5.3 (don't leak)
vector<unique_ptr<Shape>> vups;      // OK
```



## 边界检查

```c++
void silly(vector<Entry>& book)
{
     int i = book[book.size()].number;            // book.size() is out of range
     // ...
}
```

> 初始化可能会在i中放置一些随机值，而不是给出一个错误。这是不可取的，超出范围的错误是一个常见的问题。

​		经常使用vector的一个简单的范围检查自适应:

```c++
template<typename T>
struct Vec : std::vector<T> {
     using vector<T>::vector;                          // use the constructors from vector (under the name Vec)

     T& operator[](int i) { return vector<T>::at(i); }               // range check
     const T& operator[](int i) const { return vector<T>::at(i); }  // range check const objects;

     auto begin() { return Checked_iter<vector<T>>{*this}; }          
     auto end() { return Checked_iter<vector<T>>{*this, vector<T>::end()};}
};


//////////////////////////////////

try {
  	book[book.size()] = {"Joe",999999};       // will throw an exception
  	// ...
}
catch (out_of_range&) {
 	cerr << "range error\n";
}
```

> 直接使用vector::at()，不需要Vec解决方案。

​		为什么标准不保证范围检查?许多性能关键型应用程序使用向量，检查所有下标意味着大约10%的开销。显然，根据硬件、优化器和应用程序对下标的使用，成本可能会有很大差异。然而，经验表明，这样的开销会导致人们更倾向于更不安全的内置数组。即使仅仅是对这种开销的恐惧也会导致废弃。至少vector在调试时可以很容易地进行范围检查，我们可以在未检查的默认值之上构建经过检查的版本。

​		range-for通过隐式访问范围内的所有元素来避免范围错误。只要它们的参数有效，标准库算法也会这样做，以确保没有范围错误。



## allocator 分配器

​		默认情况下，标准库容器使用``new``来分配元素存储空间。标准库容器还提供了自定义特定语义分配器的接口，用于处理下列问题：

- 性能
  - 池分配器：池分配器是一种分配器，它是一个分配管理器，一次分配多个对象的存储空间
- 安全性（作为删除的一部分清理内存的分配器）
- 每线程分配和非均匀内存体系结构（在具有要匹配的指针类型的特定内存中进行分配）

​		比如一个重要的、长期运行的系统使用了一个事件队列，使用向量存储以``shared_ptr``传递的事件，事件的最后一个用户隐式删除它：

```c++
struct Event {
  	vector<int> data = vector<int>(512);
};

list<shared_ptr<Event>> q;

void producer()
{
    for (int n = 0; n!=LOTS; ++n) {
         lock_guard lk {m};            // m is a mutex; see §18.3
         q.push_back(make_shared<Event>());
         cv.notify_one();                // cv is a condition_variable;
    }
}
```

​		这段代码在逻辑上简单有效，健壮且可维护。但是它造成了大量的碎片，假使拥有16个生产者个4个消费者，事件队列处理了100000个时间后，无论当前实际使用的内存时多少，因为内存碎片，实际消耗了6GB的物理内存。

​		碎片问题的传统解决方案是使用池分配器。C++对此提供了直接支持。池分配器是在``std``的``pmr``（“多态内存资源”）子空间中定义的：

```c++
pmr::synchronized_pool_resource pool;                  // make a pool

struct Event {
      vector<int> data = vector<int>{512,&pool};    // let Events use the pool
};

list<shared_ptr<Event>> q {&pool};                         // let q use the pool

void producer()
{
    for (int n = 0; n!=LOTS; ++n) {
         scoped_lock lk {m};          // m is a mutex (§18.3)
         q.push_back(allocate_shared<Event,pmr::polymorphic_allocator<Event>>{&pool});
         cv.notify_one();
    }
}
```

​		使用池分配器，同样的条件下，消耗的物理内存不到3MB。实际使用的内存量不变。在消除碎片后，物理内存使用随着时间的推移是稳定的，因此系统可以运行数月。

- `polymorphic_resource`多线程池化分配器
  - 多态资源必须从``memory_resources`派生，并定义成员`allocate()、deallocate() 和 is_equal()``
  - 这个想法是让用户构建自己的资源来调优代码
- `unsynchronized_polymorphic_resource`类似于``polymorphic_resource``，但只能由一个线程使用。
- `monotonic_polymorphic_resource`快速分配器，它只在销毁内存时释放内存，并且只能由一个线程



## 容器分类

> ​		请注意，对于小元素的短序列（甚至对于insert（）和erase（）），向量通常比列表更有效。我建议将标准库向量作为元素序列的默认类型：您需要有理由选择另一个。

### Sequence Container 顺序容器

​		顺序容器用于存储有顺序的元素，因此能够以位置来选择插入的元素。

> 所谓顺序，是指元素插入的位置。

​		STL在c++11包含6个顺序容器：`std::array` / `std::vector` / `std::deque` / `std::list` / `std::forward_list` / `std::basic_stirng`

#### `std::array<T,N>`

​		array是最典型的顺序容器。

#### `std::vector<T>`

​		`std::vector`是一个能从`0`位置增长容量的动态数组，因此叫向量。

- 元素连续存储
- 通过`operator[]`随机访问元素
- 尾部的增删操作高效

```c++
std::vector<int> vect;
for (int count=0; count < 6; ++count)
    vect.push_back(10 - count); // insert at end of array

for (int index=0; index < vect.size(); ++index)
    std::cout << vect[index] << ' ';
```



##### 实现方式

```c++
template<typename T>
class Vector {
   allocator<T> alloc;    // standard-library allocator of space for Ts
   T* elem;                     // pointer to first element
   T* space;                   // pointer to first unused (and uninitialized) slot
   T* last;                       // pointer to last slot
public:
   // ...
   int size() const { return space-elem; }            // number of elements
   int capacity() const { return last-elem; }        // number of slots available for elements
   // ...
   void reserve(int newsz);                         // increase capacity() to newsz
   // ...
   void push_back(const T& t);                 // copy t into Vector
   void push_back(T&& t);                         // move t into Vector
};
```

- 

#### `std::deque<T>`

​		`std::deque`是一个双端队列，实现为可以两端增长的动态数组。

```c++
for (int count=0; count < 3; ++count)
{
    deq.push_back(count); // insert at end of array
    deq.push_front(10 - count); // insert at front of array
}

for (int index=0; index < deq.size(); ++index)
    std::cout << deq[index] << ' ';
```



#### `std::list<T>`

​		`std::list` 是特殊的序列容器，是一种双向链表，每个元素除数据项外还有分别指向前后元素的指针。

​		`std::list`只提供队头元素和队尾元素的访问权，即不能进行随机访问。要访问中部的元素，只能通过队头元素或队尾元素的迭代器逐次遍历到达。

​		队列能够高效的在任何位置进行插入或删除操作。

#### `std::forward_list<T>`

​		是为空序列优化的容器。一个空的``forward_lilist``只占用一个单词，而一个空向量占用三个单词。



#### `std::string`

​		虽然STL string( wstring)不是通常意义上的顺序容器，但它们属于特殊的具有`char / wchar`类型元素的向量。

#### `std::bitset<N>`



### Associative Container 关联容器

​		关联容器会自动将插入的元素排序。

> 默认通过元素的`operator<`进行排序。

​		关联容器包括：`std::set` / `std::multiset` / `std::map` / `std::multimap` .

​		同时为了特殊的需要，STL提供了无序关联容器版本：`std::unorderd_set` / `std::unorderd_multiset` / `std::unorderd_map` / `std::unordered_multimap`



#### ``std::set<T>``

​		存储单一元素，不允许重复元素。元素按值排序。

#### ``std::multiset<T>``

​		在`std::set`的基础上允许重复元素

#### `std::unordered_set<T>`

​		A set using a hashed lookup

#### `std::unordered_multiset<T>`

​		A multiset using a hashed lookup



#### `std::map<K,V>`

​		也称为关联数组（`associative array`），是存储元素为键值对的集合（a set where each element is a key/ value pair)。

​		用key来进行排序，并索引value，并且key不允许重复。

#### `std::unordered_map<K,V>`

​		A map using a hashed lookup

#### `std::multimap<K,V>`

​		A map in which a key can occur many times

​		也称为字典（dictionary），在`std::map`基础上允许重复key。key默认升序排列。

> Real-life dictionaries are multimaps: the key is the word, and the value is the meaning of the word. 

#### `std::unordered_multimap<K,V>`

​		A multimap using a hashed lookup



### Container Adapters 容器适配器

​		容器适配器是特殊的预定义容器，对特殊的使用方式进行了适配。容器适配器可以定制它所使用的顺序容器。

#### std::stack

​		元素操作遵循LIFO（后进先出），即只能操作栈顶元素。`std::stack`默认使用`std::deque`作为实现容器，也可以使用`std::vector / std::list`。

#### std::queue

​		是元素操作遵循FIFO（先进先出）的单端队列容器，即智能在队尾插入元素，队头移除元素。`std::queue`默认使用`std::deque`作为底层实现容器，也可以使用`std::list`

#### std::priority_queue

​		优先级队列是元素被排序的队列（通过`operator<`），元素被插入到队尾，元素会进行排序，即队尾元素不一定是刚插入的元素。从队头移除的元素，是该有序队列中优先级最高的项。

### span 视图



## 容器接口

​		从符号的角度来看，标准容器及其基本操作是相似的。操作的含义对于各种容器是等效的。基本操作适用于每种有意义且可以有效实施的容器：

| **Standard Container Operations (partial)** |                                                              |
| :------------------------------------------ | ------------------------------------------------------------ |
| **value_type**                              | The type of an element                                       |
| **p=c.begin()**                             | **p** points to first element of **c**; also **cbegin()** for an iterator to **const** |
| **p=c.end()**                               | **p** points to one-past-the-last element of **c**;also **cend()** for an iterator to **const** |
| **k=c.size()**                              | **k** is the number of elements in **c**                     |
| **c.empty()**                               | Is **c** empty?                                              |
| **k=c.capacity()**                          | **k** is the number of elements that **c** can hold without a new allocation |
| **c.reserve(k)**                            | Increase the capacity to **k**; if **k<=c.capacity()**, **c.reserve(k)** does nothing |
| **c.resize(k)**                             | Make the number of elements **k**;added elements have the default value **value_type{}** |
| **c[k]**                                    | The **k**th element of **c**; zero-based; no range guaranteed checking |
| **c.at(k)**                                 | The **k**th element of **c**; if out of range, throw **out_of_range** |
| **c.push_back(x)**                          | Add **x** at the end of **c**; increase the size of **c** by one |
| **c.emplace_back(a)**                       | Add **value_type{a}** at the end of **c**; increase the size of **c** by one |
| **q=c.insert(p,x)**                         | Add **x** before **p** in **c**                              |
| **q=c.erase(p)**                            | Remove element at **p** from **c**                           |
| **c=c2**                                    | Assignment: copy all elements from **c2** to get **c==c2**   |
| **b=(c==c2)**                               | Equality of all elements of **c** and **c2**; **b==true** if equal |
| **x=(c<=>c2)**                              | Lexicographical order of **c** and **c2**:**x<0** if **c** is less than **c2**, **x==0** if equal, and **0<x** if greater than.**!=**, **<**, **<=**, **>**, and **>=** are generated from **<=>** |

​		符号和语义的一致性使得能够容器的扩展新类型，以标准容器相似的方式使用。

​		`emplace`操作（如template_back（））接受元素构造函数的参数，并在容器中新分配的空间中构建对象，而不是将对象复制到容器中。

```c++
v.push_back(pair{1,"copy or move"});      // make a pair and move it into v
v.emplace_back(1,"build in place");          // build a pair in v
```

​		对于简单的实例，二者可能会被优化的性能一致。

### 迭代器

​		容器几乎都拥有如下的获取头尾迭代器的接口，但针对于特定的容器，可能只有其中的一部分。

- `begin()`	头
- `end()`        尾后
- `cbegin()`   常头
- `cend() `       常尾后
- `rbegin()`    尾后
- `rend()`        头
- `crbegin()`   常尾后
- `crend()`      常头

 他们通过返回`container::iterator` / `container::const_iterator` / `container::reverse_iterator` / `container::const_reverse_iterator`定义的迭代器类型，来提供对元素的限定访问。

```c++
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vect;
    for (int count=0; count < 6; ++count)
        vect.push_back(count);

    std::vector<int>::const_iterator it; // declare a read-only iterator
    it = vect.cbegin(); // assign it to the start of the vector
    while (it != vect.cend()) // while it hasn't reach the end
        {
        std::cout << *it << ' '; // print the value of the element it points to
        ++it; // and iterate to the next element
        }

    std::cout << '\n';
}
```

