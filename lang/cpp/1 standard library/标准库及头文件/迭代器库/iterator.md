# iterator

​		An **iterator** is an object designed to traverse through a container (e.g. the values in an array, or the characters in a string), providing access to each element along the way.

---

​		A container may provide different kinds of iterators. For example, an array container might offer a forwards iterator that walks through the array in forward order, and a reverse iterator that walks through the array in reverse order.

​		Once the appropriate type of iterator is created, the programmer can then use the interface provided by the iterator to traverse and access elements without having to worry about what kind of traversal is being done or how the data is being stored in the container. And because C++ iterators typically use the same interface for traversal (operator++ to move to the next element) and access (operator* to access the current element), we can iterate through a wide variety of different container types using a consistent method.

​		指针和迭代器有相似的地方，因为迭代器建立在指针访问顺序容器的方式之上，进行了推广和标准化。其次，指针并非专用做对数据结构的访问，而是一种语言的机制，是一种实现迭代访问的手段，而非迭代概念本身。

​		迭代器是对数据结构访问的逻辑抽象，并进行接口的统一设计。



## Iterator invalidation 迭代器失效 (dangling iterators)

​		like pointers and references, iterators can be left “dangling” if the elements being iterated over change address or are destroyed. When this happens, we say the iterator has been **invalidated**. Accessing an invalidated iterator produces undefined behavior.

​		Some operations that modify containers (such as adding an element to a `std::vector`) can have the side effect of causing the elements in the container to change addresses. When this happens, existing iterators to those elements will be invalidated.

> https://en.cppreference.com/w/cpp/container/vector#Iterator_invalidation

```c++
#include <iostream>
#include <vector>

int main()
{
	std::vector v{ 1, 2, 3, 4, 5, 6, 7 };

	auto it{ v.begin() };

	++it; // move to second element
	std::cout << *it << '\n'; // ok: prints 2

	v.erase(it); // erase the element currently being iterated over

	// erase() invalidates iterators to the erased element (and subsequent elements)
	// so iterator "it" is now invalidated

	++it; // undefined behavior
	std::cout << *it << '\n'; // undefined behavior

	return 0;
}
```