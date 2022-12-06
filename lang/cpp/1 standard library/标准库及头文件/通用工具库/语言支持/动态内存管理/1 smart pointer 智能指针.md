# Smart Pointer 智能指针

​		A Smart pointer is a composition class that is designed to manage dynamically allocated memory and ensure that memory gets deleted when the smart pointer object goes out of scope.

---

​		自动管理对象生存期，异常安全保证。

​		内置的裸指针被称为`dumb pointers` 哑指针，因为不能进行自身的资源管理。

> 裸指针除了用作函数接口外，永远不应使用裸指针。
>
> 首选unique_lock，知道编译不过，再考虑有必要时换用shared_ptr。

​		智能指针对资源生命周期管理有两个部分。一个是独立的，不涉及其他指针对资源引用的管理。另一个是交互的，涉及与其他指针对资源引用时的管理，即指针赋值的场景。

​		智能指针独立引用资源的管理，使用RAII就能实现。但指针赋值的场景却相当复杂。

## 指针赋值的常用场景

​		作为指针赋值，其本意不是进行资源的拷贝，而是以传递引用的方式方便的使用动态分配的资源。

- 要么是将一个指针对资源的引用出让给其他指针，本身放弃引用资源
- 要么是将一个指针对资源的引用共享给其他指针，本身仍旧引用资源

​		那么对于一个智能指针，应该具有移动语义，或共享拷贝语义。即，指针的赋值要么意味着前一个指针失去引用资源的能力，要么意味着两者以共享的方式操作同一资源。

```c++
// 需要移动
Resource* generateResource()
{
     Resource* r{ new Resource() };
     return r;;
}

// 需要共享
Resource* res1(new Resource());
Resource* res2(res1); // Alternatively, don't initialize res2 and then assign res2 = res1;

void passByValue(Resource* res)
{
}
```



## RAII for raw pointer

​		最简单的智能指针是以RAII进行管理的裸指针。

```c++
#include <iostream>

template <typename T>
class Auto_ptr1
{
	T* m_ptr;
public:
	// Pass in a pointer to "own" via the constructor
	Auto_ptr1(T* ptr=nullptr)
		:m_ptr(ptr)
	{
	}

	// The destructor will make sure it gets deallocated
	~Auto_ptr1()
	{
		delete m_ptr;
	}

	// Overload dereference and operator-> so we can use Auto_ptr1 like m_ptr.
	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
};

// A sample class to prove the above works
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
    void sayHi() { std::cout << "Hi!\n"; }
};

void someFunction()
{
    Auto_ptr1<Resource> ptr(new Resource()); // ptr now owns the Resource

    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;

    if (x == 0)
        return; // the function returns early

    // do stuff with ptr here
    ptr->sayHi();
}

int main()
{
    someFunction();

    return 0;
}
```

​		这个简单的RAII 智能指针有一些问题：

- 默认的拷贝赋值不能提供深拷贝，造成double free
- 作为函数返回值时，同样因为不能深拷贝，造成空悬指针

```c++
// 需要移动
??? generateResource()
{
     Resource* r{ new Resource() };
     return Auto_ptr1(r);
}

// 需要共享
Auto_ptr1<Resource> res1(new Resource());
Auto_ptr1<Resource> res2(res1); // Alternatively, don't initialize res2 and then assign res2 = res1;

void passByValue(Auto_ptr1<Resource> res)
{
}
```



### std::auto_ptr

​		`std::auto_ptr`尝试在现有引用机制的基础上进行移动语义的实现。

​		其实现基本如下：

```c++
#include <iostream>

template <typename T>
class Auto_ptr2
{
	T* m_ptr;
public:
	Auto_ptr2(T* ptr=nullptr)
		:m_ptr(ptr)
	{
	}

	~Auto_ptr2()
	{
		delete m_ptr;
	}

	// A copy constructor that implements move semantics
	Auto_ptr2(Auto_ptr2& a) // note: not const
	{
		m_ptr = a.m_ptr; // transfer our dumb pointer from the source to our local object
		a.m_ptr = nullptr; // make sure the source no longer owns the pointer
	}

	// An assignment operator that implements move semantics
	Auto_ptr2& operator=(Auto_ptr2& a) // note: not const
	{
		if (&a == this)
			return *this;

		delete m_ptr; // make sure we deallocate any pointer the destination is already holding first
		m_ptr = a.m_ptr; // then transfer our dumb pointer from the source to the local object
		a.m_ptr = nullptr; // make sure the source no longer owns the pointer
		return *this;
	}

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
	bool isNull() const { return m_ptr == nullptr; }
};

class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};

int main()
{
	Auto_ptr2<Resource> res1(new Resource());
	Auto_ptr2<Resource> res2; // Start as nullptr

	std::cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
	std::cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");

	res2 = res1; // res2 assumes ownership, res1 is set to null

	std::cout << "Ownership transferred\n";

	std::cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
	std::cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");

	return 0;
}
```

​		但这样实现的移动语义有一些用起来非常危险的缺点：

- 基于拷贝语义实现的移动语义，没有任何形式上的移动说明

  ​		对于赋值和参数传递等情景下，拷贝或共享是更符合直觉的行为。因此，对于被拷贝的对象可能没有任何提示性的语法说明其被移动，失去了所有权。

  ​		在被移动后，继续使用原对象，相当危险，容易造成崩溃。

- 总是使用非数组删除来删除其内容。

  ​	对于`T`是数组的智能指针，不能正确的释放资源。它也不能阻止传递一个动态数组。因此造成内存泄漏。

- 不能与STL中的容器和算法兼容，这些容器和算法是以拷贝或共享元素来设计的。

​		基于上述原因，`std::auto_ptr`已经在c++11中弃用，并在c++17中被移除。

​		`std::auto_ptr`其本质还是在拷贝语义的基础上实现移动语义。以重写拷贝语义的方式实现移动语义导致，两者语义的边界不清造成问题，如`p1 = p2`。基于此，从c++11起，c++为移动语义配套了完整的，独立于拷贝语义的一整套语言设施（值类别-右值，引用-右值引用，移动构造，移动赋值）。

​		也因此，智能指针也分化为了两种语义性质，所有权转移的移动语义指针，和所有权共享的拷贝语义指针。



### copyable auto_ptr

​		a version that implements a copy constructor and copy assignment operator that do deep copies.

```c++
#include <iostream>

template<typename T>
class Auto_ptr3
{
	T* m_ptr;
public:
	Auto_ptr3(T* ptr = nullptr)
		:m_ptr(ptr)
	{
	}

	~Auto_ptr3()
	{
		delete m_ptr;
	}

	// Copy constructor
	// Do deep copy of a.m_ptr to m_ptr
	Auto_ptr3(const Auto_ptr3& a)
	{
		m_ptr = new T;
		*m_ptr = *a.m_ptr;
	}

	// Copy assignment
	// Do deep copy of a.m_ptr to m_ptr
	Auto_ptr3& operator=(const Auto_ptr3& a)
	{
		// Self-assignment detection
		if (&a == this)
			return *this;

		// Release any resource we're holding
		delete m_ptr;

		// Copy the resource
		m_ptr = new T;
		*m_ptr = *a.m_ptr;

		return *this;
	}

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
	bool isNull() const { return m_ptr == nullptr; }
};

class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};

Auto_ptr3<Resource> generateResource()
{
	Auto_ptr3<Resource> res{new Resource};
	return res; // this return value will invoke the copy constructor
}

int main()
{
	Auto_ptr3<Resource> mainres;
	mainres = generateResource(); // this assignment will invoke the copy assignment

	return 0;
}
```

​		在没有ROV的场景下， 进行了三次构造三次析构，ROV的场景下返回值的中间临时变量被消除，进行了两次构造两次析构。

​		对于大多数场景，这种指针的深拷贝方式虽然安全，但不是指针赋值本身的含义。指针赋值的含义，要么是对资源的转移，要么是对资源的共享。

## std::unique_ptr

## std::shared_ptr

### 实现原理

### 线程安全性

### make_shared

> ```c++
> std::make_shared<A>(params); // 效率更高,一次new，空间上更少
> std::shared_ptr(new A(params)); // 两次new ，空间更多
> ```
>
> make_shared在某些情况下是不能使用的，比如当构造函数私有时。

## enable_shared_from_this

​		在构造函数和析构函数的函数栈上不能进行`shared_from_this()`的调用，会抛出`bad_weak_ptr`的运行异常。因为在构造和析构的函数栈上，对象语义都是不完整的，没有完整的内存模型，因而不能由此来获取智能指针。