# std::initializer_list

​		`<initializer_list> `

​		When a compiler sees an initializer list, it automatically converts it into an object of type std::initializer_list. 

---

- `std::initializer_list<T>`
- `std::initalizer_list`
  - unless you initialize the std::initializer_list right away



​		类似于`std::string_view`，`std::initializer_list`是initializer list的轻量级`header`结构，无须使用`const std::initlizer_list&`来优化传递。

​		`std::initializer_list`提供一个`size()`来提供初始化列表提供的元素个数，仍未支持使用`operator[]`来进行索引访问。

​		初始化列表总是倾向于优先匹配`std::initializer_list`构造函数，而不是其他备选项。

```c++
std::vector<int> array{ 5 }; // Calls std::vector::vector(std::initializer_list<int>), 1 element: 5
std::vector<int> array(5); // Calls std::vector::vector(std::vector::size_type), 5 value-initialized elements: 0 0 0 0 0
```

> ​		对于同时定义了`cons(std::initliazer_list<T>)`和``cons(T)``的类，对`con(T)`的匹配应该使用拷贝初始化或直接初始化。
>
> ​		The same happens to std::vector and other container classes that have both a list constructor and a constructor with a similar type of parameter

​		如果实现了`cons(std::initializer_list)`，为防止列表元素的实际存储不支持深拷贝，应该确保至少为此做以下其中一项保护措施。

1. 重载 list assignment operator
2. 重载合适的 copy assignment operator
3. 禁用 copy assignment operator

```c++
#include <cassert> // for assert()
#include <initializer_list> // for std::initializer_list
#include <iostream>

class IntArray
{
private:
	int m_length{};
	int* m_data{};

public:
	IntArray() = default;

	IntArray(int length)
		: m_length{ length }
		, m_data{ new int[length] {} }
	{

	}

	IntArray(std::initializer_list<int> list) // allow IntArray to be initialized via list initialization
		: IntArray(static_cast<int>(list.size())) // use delegating constructor to set up initial array
	{
		// Now initialize our array from the list
		int count{ 0 };
		for (auto element : list)
		{
			m_data[count] = element;
			++count;
		}
	}

	~IntArray()
	{
		delete[] m_data;
	}

//	IntArray(const IntArray&) = delete; // to avoid shallow copies
//	IntArray& operator=(const IntArray& list) = delete; // to avoid shallow copies

	int& operator[](int index)
	{
		assert(index >= 0 && index < m_length);
		return m_data[index];
	}

	int getLength() const { return m_length; }
};

int main()
{
	IntArray array{};
	array = { 1, 3, 5, 7, 9, 11 }; // Here's our list assignment statement

	for (int count{ 0 }; count < array.getLength(); ++count)
		std::cout << array[count] << ' ';

	return 0;
}
```

