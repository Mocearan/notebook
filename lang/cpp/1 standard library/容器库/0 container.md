# STL container

---

​		STL包含三种基本的容器类型：顺序容器、关联容器和容器适配器。

​		容器在动态内存中存放资源并以RAII的方式进行资源管理。

​		容器内的空间分配策略由`allocator`决定，由容器的抽象统一访问机制由`iterator`提供。

## 容器分类

### Sequence Container 顺序容器

​		顺序容器用于存储有顺序的元素，因此能够以位置来选择插入的元素。

> 所谓顺序，是指元素插入的位置。

​		STL在c++11包含6个顺序容器：`std::array` / `std::vector` / `std::deque` / `std::list` / `std::forward_list` / `std::basic_stirng`

#### std::array

​		array是最典型的顺序容器。

#### std::vector

​		`std::vector`是一个能从`0`位置增长容量的动态数组，因此叫向量。

- 通过`operator[]`随机访问元素
- 尾部的增删操作高效

```c++
std::vector<int> vect;
for (int count=0; count < 6; ++count)
    vect.push_back(10 - count); // insert at end of array

for (int index=0; index < vect.size(); ++index)
    std::cout << vect[index] << ' ';
```



#### std::deque

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



#### std::list

​		`std::list` 是特殊的序列容器，是一种双向链表，每个元素除数据项外还有分别指向前后元素的指针。

​		`std::list`只提供队头元素和队尾元素的访问权，即不能进行随机访问。要访问中部的元素，只能通过队头元素或队尾元素的迭代器逐次遍历到达。

​		队列能够高效的在任何位置进行插入或删除操作。

#### std::string

​		虽然STL string( wstring)不是通常意义上的顺序容器，但它们属于特殊的具有`char / wchar`类型元素的向量。



### Associative Container 关联容器

​		关联容器是会自动将插入的元素排序的容器。

> 默认通过元素的`operator<`进行排序。

​		关联容器包括：`std::set` / `std::multiset` / `std::map` / `std::multimap` .

​		同时为了特殊的需要，STL提供了无序关联容器版本：`std::unorderd_set` / `std::unorderd_multiset` / `std::unorderd_map` / `std::unordered_multimap`



#### std::set

​		存储单一元素，不允许重复元素。元素按值排序。

#### std::multiset

​		在`std::set`的基础上允许重复元素

#### std::map

​		也称为关联数组（`associative array`），是存储元素为键值对的集合（a set where each element is a key/ value pair)。

​		用key来进行排序，并索引value，并且key不允许重复。

#### std::multimap

​		也称为字典（dictionary），在`std::map`基础上允许重复key。key默认升序排列。

> Real-life dictionaries are multimaps: the key is the word, and the value is the meaning of the word. 



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

