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

​		传统c/c++程序中严重错误主要来源于范围错误，会导致 错误的结果、崩溃和安全问题。容器、算法和``range-for``的使用极大缓解了这个问题。

​		范围错误的主要原因是认为传递指针，依靠约定确定指向的元素数量。标准库``string_view``提供针对字符只读的字符串视图。``<span>``中的``span``基本上是一个 `(pointer，length)` 对，表示一个元素序列:

```c++
span<int>:  {begin(), size()}
			 ↓->
intergers:   | 1 | 2 | 3 | 4 | 5 |....| 23 |
```

​		`span` 访问连续的元素序列。

- 元素可以以多种方式存储，包括在向量和内置数组中。
- 与指针一样，``span``不实际拥有所指向的字符。类似于``string_view``和STL迭代器对。

```c++
void fpn(int* p, int n) 
{
    for (int i = 0; i<n; ++i) // n个整数是假设，只是一种约定，不能用来做range-for，编译器也无法实现廉价而有效的范围检查。
        p[i] = 0;
}

int a[100];
fpn(a,100);               // OK
fpn(a,1000);             // oops, my finger slipped! (range error in fpn)
```

​		可以使用`span`来改进：

```c++
void fs(span<int> p)
{
	for (int& x : p)
    	x = 0;
}

 int a[100];
fs(a);                   // implicitly creates a span<int>{a,100}
fs(a,1000);              // error: span expected
fs({a+10,100});          // a range error in fs
fs({a,x});               // obviously suspect
```

​		直接从数组创建``span`是安全的(编译器会计算元素数量)。在函数间传递`span`比`(pointer,count)`接口简单，不需要额外的检查

```c++
void f1(span<int> p);
void f2(span<int> p){f1(p);}
```

​		对``span``使用下标时不要求进行范围检查，越界访问是未定义的行为。核心指南支持库[CG]中的原始``gsl::span``做范围检查。

​		在底层软件中写入和读取缓冲区时，很难在保持高性能的同时避免缓冲区溢出。



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



# facilities container

​		标准提供了一些不完全适合STL框架的容器（如内置数组、``array``和``string``），也称为“拟容器”。

- 保存元素，是容器
- 但都有不同的限制或特别功能，使它们在STL语境中显得笨拙

| **Containers**      |                                                              |
| :------------------ | ------------------------------------------------------------ |
| **T[N]**            | Built-in array: a fixed-size contiguously allocated sequence of **N** |
|                     | elements of type **T**; implicitly converts to a **T***      |
| **array<T,N>**      | A fixed-size contiguously allocated sequence of **N** elements |
|                     | of type **T**; like the built-in array, but with most problems solved |
| **bitset<N>**       | A fixed-size sequence of **N** bits                          |
| **vector<bool>**    | A sequence of bits compactly stored in a specialization of **vector** |
| **pair<T,U>**       | Two elements of types **T** and **U**                        |
| **tuple<T...>**     | A sequence of an arbitrary number of elements of arbitrary types |
| **basic_string<C>** | A sequence of characters of type **C**; provides string operations |
| **valarray<T>**     | An array of numeric values of type **T**; provides numeric operations |

- `std::pair`和`std::tuple`是异构数据结构

  ​	其他容器都是同构的(所有元素都是相同的类型)。

- `std::array`和`std::tuple`的元素是连续的顺序结构，``std::list``和``std::map``是链式结构。

- `std::bitset`和`std::vector<bool>`保存比特位，并通过代理对象访问它们

  ​	其他标准库容器都可以容纳各种类型并直接访问元素

- `std::basic_string`要求其元素是某种形式的字符，并提供字符串操作，例如连接和敏感于地区的操作。

- `std::valarray`要求元素都是数值，并提供数值运算。



## `std::array<T, N>`

​		``<array>``中定义的`std::array<T,N>`是

- 给定类型`T`的固定长度`N`的元素序列

- 元素数量`N`在编译时指定，因此`std::array`中元素的存储可以分配到栈中、对象中或静态存储中。

- 元素的存储分配在定义`std::array<T,N>`的作用域中

  - 最好将数组理解为与大小紧密相连的内置数组

  - 没有隐式的、可能令人惊讶的指针类型转换，并提供了一些方便的函数

  - 与使用内置数组相比，使用数组没有任何开销(时间或空间)

  - 数组并不遵循STL容器的“处理元素”模型。相反，数组直接包含它的元素

  - 它只不过是内置数组的一个更安全的版本

    - 数据维度与长度维度绑定在一个对象中，不会发生退化
    - 可以使用`operator=`来拷贝数组

  - 必要时，可以将数组显式传递给需要指针的c风格函数

    ```c++
     f(a,a.size());                    // error: no conversion
    f(a.data(),a.size());        	 // C-style use
    ```



​		相比于`std::vector`，`std::array`虽然不够灵活，但是更简单。

- 直接访问栈上分配的元素，相对于通过`std::vector`的资源句柄在堆上分配释放元素有更显著的性能优势
- 栈是一种有限的资源，要注意栈溢出
- 一些关键的安全软件、实时软件禁止动态内存分配，使用`delete`可能造成内存碎片或内存耗尽



## `std::bitset<T>`

​		系统的状态，通常表示为一组标志，表示二元条件，如好/坏、真/假和开/关。

- c++通过对整数的位操作有效地支持小标志集的概念。
- 类`std::bitset<N>`通过提供对N位序列``[0:N)``的操作来推广这种概念
  - N在编译时已知
  - ``long long``整型无法容纳的位集合，使用``std::bitset``比直接使用integer方便得多
  - 对于较小的集合，``std::bitset``通常是优化过的。
  - 如果想给这些位命名，而不是使用index，可以使用集合或枚举



### 初始化

```c++
// bitset可以用整数或字符串初始化:
bitset<9> bs1 {"110001111"};
bitset<9> bs2 {0b1'1000'1111};       // binary literal using digit separators (§1.4)
```



### 操作

```c++
// 位操作符和左移和右移操作符可以应用:
bitset<9> bs3 = ~bs1;               // complement: bs3=="001110000"
bitset<9> bs4 = bs1&bs3;        // all zeros
bitset<9> bs5 = bs1<<2;          // shift left: bs5 = "000111100"

// to_ullong()和to_string()操作提供了构造函数的反向操作
bitset<8*sizeof(int)> b = i;             // assume 8-bit byte (see also §17.7)
cout << b.to_string() << '\n';          // write out the bits of i

//  bitset output operator
 bitset<8*sizeof(int)> b = i;      // assume 8-bit byte (see also §17.7)
cout << b << \'n';                 // write out the bits of i

// bitset提供了许多用于使用和操作位集合的函数，如all()、any()、none()、count()、flip()。
```



## `std::pair<K,V>`

​		编程中将两个不同类型的作为一个逻辑整体来使用的场景非常常见，如映射容器中的键值元素、函数参数或返回值的多元逻辑整体（``array{data, length}``)。

​		传统最常用的方式是通过`struct`来将不同类型的类型组织在一起，函数返回值可以借此返回一个错误说明和函数结果。当代码不需要泛型时，使用具有命名成员的简单结构通常会使代码更易于维护。

```c++
struct My_res {
 	Entry* ptr;
 	Error_code err;
};

My_res complex_search(vector<Entry>& v, const string& s)
{
     Entry* found = nullptr;
     Error_code err = Error_code::found;
     // ... search for s in v ...
     return {found,err};
}

 My_res r = complex_search(entry_table,s);      // search entry_table
 if (r.err != Error_code::good) {
          // ... handle error ...
 }
 //... use r.ptr ....
```

​		`c++11`通过`std::pair<K,V>`和`std::tuple`提供了组合两个或多个不同类型值作为一个整体的能力。

​		`std::pair`有两个成员``first / second``。实现角度来看，以朴素的序号来标定元素足够通用。在应用时往往希望通过以命名的方式来使用，结构化绑定提供了为结构化数据命名的能力。

```c++
auto [ptr,success] = complex_search(entry_table,s);      // search entry_table
if (success != Error_code::good)
     // ... handle error ...
}
// ... use r.ptr ....
```

​		标准库经常使用`std::pair`来传递函数结果。

```c++
template<typename Forward_iterator, typename T, typename Compare>
 pair<Forward_iterator,Forward_iterator>
 equal_range(Forward_iterator first, Forward_iterator last, const T& val, Compare cmp);

/////////////////////////////////
auto less = [](const Record& r1, const Record& r2) { return r1.name<r2.name;};// compare names
auto [first,last] = equal_range(v.begin(),v.end(),Record{"Reg"},less);
for (auto p = first; p!=last; ++p)  // print all equal records
         cout << *p;         // assume that << is defined for Record
```

​		`std::pair`提供了操作符，``= / == / <``等，如果它的元素是这样的话。创建``std::pair``时因类型推导无需显式地说明其类型。

```c++
vector<string> v{...};
pair p1 {v.begin(),2};               // one way
auto p2 = make_pair(v.begin(),2);    // another way

// std::pair<vector<string>::iterator,int>
```

​		

## `std::tuple`

​		`std::tuple`是一个异构容器，是`std::pair`的泛化形式，可以包含零个或多个元素。

```c++
tuple t0 {};                                                                      // empty
tuple<string,int,double> t1 {"Shark",123,3.14};         // the type is explicitly specified
auto t2 = make_tuple(string{"Herring"},10,1.23);      // the type is deduced to tuple<string,int,double>
tuple t3 {"Cod"s,20,9.99};                                            // the type is deduced to tuple<string,int,double>
```

​		`std::tuple`中的元素独立的，他们之间的结构并非不变。如果需要不变式结构，必须将元组封装在一个类中制约其结构。

​		对于单一的、特定的用途，通常理想的是简单的结构体。但`std::tuple`的灵活性在许多通用场景中，不必定义大量结构体结构体，作为灵活性的代价同`std::pair`一样是需要使用序号来标定元素，`std::tuple`通过`std::get<N>`函数模板访问特定序号的元素。

```c++
2string fish = get<0>(t1);              // get the first element: "Shark"
int count = get<1>(t1);                // get the second element: 123
double price = get<2>(t1);         // get the third element: 3.14
```

- 序号从``0``开始，`std::get()`方法的``index``参数必须是一个常量

- `std::get`模板函数以索引作为模板值参数

- 索引访问`std::tuple`元素常见而丑陋，且易出错

  - `std::tuple`中类型唯一元素可以按其类型命名

    ```c++
    auto fish = get<string>(t1);                // get the string: "Shark"
    auto count = get<int>(t1);                  // get the int: 123
    auto price = get<double>(t1);           	// get the double: 3.14
    
    get<string>(t1) = "Tuna";       // write to the string
    get<int>(t1) = 7;                      // write to the int
    get<double>(t1) = 312;          // write to the doubl
    ```

  - `std::tuple`的用法往往结合其他更高层次的结构化数据结构设计，如结构化绑定

    ```c++
    auto [fish, count, price] = t1;
    cout << fish << ' ' << count << ' ' << price << '\n';      // read
    fish = "Sea Bass"; 
    
    auto [fish, count, price] = todays_catch();
    cout << fish << ' ' << count << ' ' << price << '\n';
    ```



​		`std::tuple`的优势在于，存储或传递数量未知、类型未知的元素。

​		遍历`std::tuple`的元素有点麻烦，需要递归，并在编译时对函数体求值:

```c++
template <size_t N = 0, typename... Ts> 
constexpr void print(tuple<Ts...> tup)
{					// sizeof…(Ts) 给出了Ts中的元素个数。
    if constexpr (N<sizeof...(Ts)) {         // not yet at the end?
       cout << get<N>(tup) << ' ';         	// print the Nth element
   	print<N+1>(tup);                        // print the next element
    }
}

print(t0);           // no output
print(t2);           // Herring 10 1.23
print(tuple{ "Norah", 17, "Gavin", 14, "Anya", 9, "Courtney", 9, "Ada", 0 });
```

​		`std::tuple`也提供了操作符，`= / == / <`等。还有二元组和二元组之间的转换。



# Alternatives container

​		有一些场景中，逻辑对象在不同的场景下以不同类型的表达方式互相指代。容纳这些在不同场景下，对同一值进行不同类型表达的类型称为替代容器。

​		标准库基于C语言`union`扩展了三种替代容器：

| **union**         | A built-in type that holds one of a set of alternatives (§[2.5](ch02.xhtml#sec2_5)) |
| ----------------- | ------------------------------------------------------------ |
| **variant<T...>** | One of a specified set of alternatives (in **<variant>**)    |
| **optional<T>**   | A value of type **T** or no value (in **<optional>**)        |
| **any**           | A value one of an unbounded set of alternative types (in **<any>**) |



## `std::variant`

​		与直接使用``union``相比，`std::variant<A,B,C>`通常更安全、更方便。

```c++
std::variant<std::string,Error_code> compose_message(istream& s)
{
    std::string mess;
    // ... read from s and compose message ...
    if (no_problems)
        return mess;              // return a string
    else
        return Error_code{some_problem};        // return an Error_code
}
```

​		用一个值赋值或初始化`std::variant`时，可以查询`std::variant`的类型并提取值。

```c++
auto m = compose_message(cin);

if (holds_alternative<string>(m)) {
   cout << get<string>(m);
} else {
   auto err = get<Error_code>(m);
   // ... handle error ...
}
```

​		这种风格排除了异常，还有其他一些用途：一个简单的编译器可能需要区分具有不同表示的不同类型的节点：

```c++
using Node = variant<Expression,Statement,Declaration,Type>;

void check(Node* p)
{
    if (holds_alternative<Expression>(*p)) {
        Expression& e = get<Expression>(*p);
        // ...
     }
     else if (holds_alternative<Statement>(*p)) {
        Statement& s = get<Statement>(*p);
        // ...
     }
     // ... Declaration and Type ...
}
```

​		这种通过检查其他选项来决定适当操作的模式非常常见，但效率相对较低，因此值得直接支持:

```c++
void check(Node* p)
{
    visit(overloaded {
         [](Expression& e) { /* ... */ },
         [](Statement& s) { /* ... */ },
         // ... Declaration and Type ...
    }, *p);
}
```

​		这基本上等同于虚函数调用，但可能更快。与所有关于性能的声明一样，在性能至关重要时，这种“可能更快”应该通过测量来验证。在大多数情况下，性能上的差异是微不足道的。

​		重载类是必要的，奇怪的是，它不是标准的。它是一个“魔法”，可以从一组参数(通常是lambda)构建一个重载集:

```c++
template<class... Ts>
struct overloaded : Ts... {            // variadic template (§8.4)
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;    // deduction guide
```

​		然后，“visitor”访问将()应用于重载对象，该对象根据重载规则选择最合适的lambda来调用。

​		deduction guide是一种解决微妙歧义的机制，主要用于基础库中的类模板构造器(§7.2.3)。

​		如果我们试图访问与预期类型不同的变量，则会抛出bad_variant_access。



## `std::optional`

​		`std::optional<A>`可以看作是一种特殊的变体(比如``std::variant<A,nothing>``)，也可以看作是``A*``指向对象或为``nullptr``这一思想的泛化。

​		对于可能返回或不返回对象的函数，``optional``很有用:

```c++
optional<string> compose_message(istream& s)
{
    string mess;

    // ... read from s and compose message ...
    if (no_problems)
            return mess;
    return {};          // the empty optional
}

if (auto m = compose_message(cin))
        cout << *m;               // note the dereference (*)
else {
        // ... handle error ...
}
```

​		它排除了异常，注意``*``的奇怪用法。``std::optional``对象被视为指向其对象的指针，而不是对象本身。与``nullptr``等价的``std::optional``对象是空对象``{}``。

```c++
int sum(optional<int> a, optional<int> b)
{
    int res = 0;
    if (a) res+=*a;
    if (b) res+=*b;
    return res;
}

int x = sum(17,19);         // 36
int y = sum(17,{});          // 17
int z = sum({},{});            // 0
```

​		如果我们尝试访问一个没有值的``optional``对象，结果是``undefined``而不抛出异常。因此，``std::optional``不能保证类型安全。不要尝试:

```c++
nt sum2(optional<int> a, optional<int> b)
{
     return *a+*b;     // asking for trouble
}
```



## `std::any`

​		`std::any`对象可以保存任意类型，并且知道它保存的是哪种类型。它基本上是一个无约束版本的``std::variant``

```c++
std::any compose_message(std::istream& s)
{
    std::string mess;
    // ... read from s and compose message ...
    if (no_problems)
        return mess;                        // return a string
    else
        return error_number;          // return an int
}
```

​		当你用值赋值或初始化any时，它会记住该值的类型。稍后，我们可以通过断言值的预期类型来提取any所持有的值。

```c++
auto m = compose_message(cin);
string& s = any_cast<string>(m); 
cout << s;
```

​		如果我们试图访问类型与预期不同的any对象，则会抛出bad_any_access。
