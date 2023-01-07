# Smart Pointer 智能指针

​		A Smart pointer is a composition class that is designed to manage dynamically allocated memory and ensure that memory gets deleted when the smart pointer object goes out of scope.

---

​		自动管理对象生存期，异常安全保证。

> Although smart pointers can offer other features, the defining characteristic of a smart pointer is that it manages a dynamically allocated resource, and ensures the dynamically allocated object is properly cleaned up at the appropriate time (usually when the smart pointer goes out of scope).
>
> Because of this, smart pointers should never be dynamically allocated themselves (otherwise, there is the risk that the smart pointer may not be properly deallocated, which means the object it owns would not be deallocated, causing a memory leak). 

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



### moveable auto_ptr

```c++
#include <iostream>
#include <utility> // std::swap

template<typename T>
class Auto_ptr
{
    T *ptr_;

public:
    Auto_ptr( T *ptr = nullptr ): ptr_ { ptr } { }
    ~Auto_ptr() { delete ptr_; }

    Auto_ptr( const Auto_ptr & ) = delete;
    Auto_ptr &operator=( const Auto_ptr & ) = delete;

    // Move constructor
    // Transfer ownership of a.ptr_ to ptr_
    Auto_ptr( Auto_ptr &&a ) noexcept
    {
        a.swap(*this);
    }

    // Move assignment
    // Transfer ownership of a.ptr_ to ptr_
    Auto_ptr &operator=( Auto_ptr &&a ) noexcept
    {
        a.swap(*this);
        return *this;
    }

    void swap(Auto_ptr& oth)
    {
        std::swap( oth.ptr_, ptr_ ); 
        // 不能使用std::swap直接交换两个Auto_ptr对象，因为calls both the move constructor and move assignment on move-capable objects。造成递归。
    }

    T &operator*() const { return *ptr_; }
    T *operator->() const { return ptr_; }
    bool isNull() const { return ptr_ == nullptr; }
};

class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

Auto_ptr<Resource> generateResource()
{
    return Auto_ptr<Resource> { new Resource };
}

int main()
{
    auto mainres = generateResource(); // this assignment will invoke the move assignment

    return 0;
}
```

## std::unique_ptr

​		`std::unique_ptr`是c++11中使用完备移动语义基础来替换`std::auto_ptr`的新moveable smart pointer。用来管理任何不被多个对象共享的动态分配的对象。

```c++
#include <iostream>
#include <memory> // for std::unique_ptr
#include <utility> // for std::move

class Resource
{
public:
    Resource() {std::cout << "Resource acquired\n";}
	~Resource() {std::cout <<"Resource destroyed\n";}    	
};

int main()
{
	// allocate a Resource object and have it owned by std::unique_ptr
    std::unique_ptr<Resource> res{new Resource()};
    
    // copy deleted
    // auto res2 = res1;
    auto res2 = std::move(res1);  // converts res1 into an r-value, which triggers a move assignment instead of a copy assignment
    
    
    return 0;
} // res goes out of scope here, and the allocated Resource is destroyed
```



### operator* / operator->

​	Operator* returns a reference to the managed resource, and operator-> returns a pointer.

```c++
std::unique_ptr<Resource> res{ new Resource{} };

if (res) // use implicit cast to bool to ensure res contains a Resource
    std::cout << *res << '\n'; // print the Resource that res is owning
```

### 数组支持

​		不同于`std::auto_ptr`，`std::unique_ptr`可以支持对标量和数组的`delete`，因此`std::unique_ptr`支持数组作为对象。

​		Favor std::array, std::vector, or std::string over a smart pointer managing a fixed array, dynamic array, or C-style string.

​		提供`operator[]`重载来以数组索引的方式使用智能指针访问对象。

### std::make_unique

​		c++11没有提供`std::make_unique`，在c++14中才加入。

​		This templated function constructs an object of the template type and initializes it with the arguments passed into the function.

```c++
#include <memory> // for std::unique_ptr and std::make_unique
#include <iostream>

class Fraction
{
private:
	int m_numerator{ 0 };
	int m_denominator{ 1 };

public:
	Fraction(int numerator = 0, int denominator = 1) :
		m_numerator{ numerator }, m_denominator{ denominator }
	{
	}

	friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
	{
		out << f1.m_numerator << '/' << f1.m_denominator;
		return out;
	}
};


int main()
{
	// Create a single dynamically allocated Fraction with numerator 3 and denominator 5
	// We can also use automatic type deduction to good effect here
	auto f1{ std::make_unique<Fraction>(3, 5) };
	std::cout << *f1 << '\n';

	// Create a dynamically allocated array of Fractions of length 4
	auto f2{ std::make_unique<Fraction[]>(4) };
	std::cout << f2[0] << '\n';

	return 0;
}
```

​		优先使用`std::make_unique`，它需要更少的输入，同时解决了一个因c++遗留的关于函数参数求值顺序未定而导致的异常安全问题。

> https://www.learncpp.com/cpp-tutorial/stdunique_ptr/#smartpointerexceptionsafety
>
> ```cpp
> some_function(std::unique_ptr<T>(new T), function_that_can_throw_exception());
> ```
>
> ​		The compiler is given a lot of flexibility in terms of how it handles this call. It could create a new T, then call function_that_can_throw_exception(), then create the std::unique_ptr that manages the dynamically allocated T. If function_that_can_throw_exception() throws an exception, then the T that was allocated will not be deallocated, because the smart pointer to do the deallocation hasn’t been created yet. This leads to T being leaked.
>
> ​		std::make_unique() doesn’t suffer from this problem because the creation of the object T and the creation of the std::unique_ptr happen inside the std::make_unique() function, where there’s no ambiguity about order of execution.

### return std::unique_ptr

```c++
#include <memory> // for std::unique_ptr

std::unique_ptr<Resource> createResource()
{
     return std::make_unique<Resource>();
}

int main()
{
    auto ptr{ createResource() };

    // do whatever

    return 0;
}
```

​		If this value is not assigned to anything, the temporary return value will go out of scope and the Resource will be cleaned up. 

​		If it is assigned (as shown in main())

- in C++14 or earlier, move semantics will be employed to transfer the Resource from the return value to the object assigned to (in the above example, ptr),
- and in C++17 or newer, the return will be elided. This makes returning a resource by std::unique_ptr much safer than returning raw pointers!

​	In general, you should not return std::unique_ptr by pointer (ever) or reference (unless you have a specific compelling reason to).

### pass std::unique_ptr

- take ownership

  ```c++
  void takeOwnership(std::unique_ptr<Resource> res)
  {
       if (res)
            std::cout << *res << '\n';
  } // the Resource is destroyed here
  
  auto ptr{ std::make_unique<Resource>() };
  
  // takeOwnership(ptr); // This doesn't work, need to use move semantics
  takeOwnership(std::move(ptr)); // ok: use move semantics
  ```

- won’t take ownership

  - pass a std::unique_ptr by reference

    ​	Although you can pass a std::unique_ptr by reference (which will allow the function to use the object without assuming ownership), you should only do so when the called function might alter or change the object being managed.

  - Instead, it’s better to just pass the resource itself 

    ​	by pointer or reference, depending on whether null is a valid argument

    ```c++
    // The function only uses the resource, so we'll accept a pointer to the resource, not a reference to the whole std::unique_ptr<Resource>
    void useResource(Resource* res)
    {
    	if (res)
    		std::cout << *res << '\n';
    	else
    		std::cout << "No resource\n";
    }
    
    int main()
    {
    	auto ptr{ std::make_unique<Resource>() };
    
    	useResource(ptr.get()); // note: get() used here to get a pointer to the Resource
    
    	std::cout << "Ending program\n";
    
    	return 0;
    } // The Resource is destroyed here
    ```

### 管理类资源

​		use std::unique_ptr as a composition member of your class.  This way, you don’t have to worry about ensuring your class destructor deletes the dynamic memory, as the std::unique_ptr will be automatically destroyed when the class object is destroyed.

​		 if the class object is not destroyed properly (e.g. it is dynamically allocated and not deallocated properly), then the std::unique_ptr member will not be destroyed either, and the object being managed by the std::unique_ptr will not be deallocated.

### 错误使用

​		Note that std::make_unique() prevents both of the cases from happening inadvertently.

- multiple classes manage the same resource. 

  ```c++
  Resource* res{ new Resource() };
  std::unique_ptr<Resource> res1{ res };
  std::unique_ptr<Resource> res2{ res };
  ```

  both res1 and res2 will try to delete the Resource, which will lead to undefined behavior.

- manually delete the resource out from underneath the std::unique_ptr.

  ```c++
  Resource* res{ new Resource() };
  std::unique_ptr<Resource> res1{ res };
  delete res;
  ```

  the std::unique_ptr will try to delete an already deleted resource, again leading to undefined behavior.

## std::shared_ptr

​		std::shared_ptr is meant to solve the case where you need multiple smart pointers co-owning a resource.

​		Internally, std::shared_ptr keeps track of how many std::shared_ptr are sharing the resource. As long as at least one std::shared_ptr is pointing to the resource, the resource will not be deallocated, even if individual std::shared_ptr are destroyed. As soon as the last std::shared_ptr managing the resource goes out of scope (or is reassigned to point at something else), the resource will be deallocated.

```cpp
#include <iostream>
#include <memory> // for std::shared_ptr

class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};

int main()
{
	// allocate a Resource object and have it owned by std::shared_ptr
	Resource* res { new Resource };
	std::shared_ptr<Resource> ptr1{ res };
	{
		std::shared_ptr<Resource> ptr2 { ptr1 }; // make another std::shared_ptr pointing to the same thing
         // std::shared_ptr<Resource> ptr2 { res }; // create ptr2 directly from res (instead of ptr1)


		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, but nothing happens

	std::cout << "Killing another shared pointer\n";

	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed
```

> 如果不是通过拷贝语义从一个`std::shared_ptr`进行拷贝，而是从资源直接构造另一个`std::shared_ptr`，两个`std::shared_ptr`不能建立联系，因而互为独立的对资源的引用。
>
> 但是当其中一个释放了资源时，也无法通知另一个，因而构成悬垂指针。

​		Always make a copy of an existing std::shared_ptr if you need more than one std::shared_ptr pointing to the same resource.

​		`std::uniquq_pt`可以通过`std::shared_ptr`安全的转换为一个`std::shared_ptr`，反之不行。

### 实现原理

​		std::shared_ptr uses two pointers internally. One pointer points at the resource being managed. The other points at a “control block”, which is a dynamically allocated object that tracks of a bunch of stuff, including how many std::shared_ptr are pointing at the resource. When a std::shared_ptr is created via a std::shared_ptr constructor, the memory for the managed object (which is usually passed in) and control block (which the constructor creates) are allocated separately. However, when using std::make_shared(), this can be optimized into a single memory allocation, which leads to better performance。

> Each std::shared_ptr will have one pointer pointing at the resource. However, each std::shared_ptr will independently allocate its own control block, which will indicate that it is the only pointer owning that resource. Thus, when that std::shared_ptr goes out of scope, it will deallocate the resource, not realizing there are other std::shared_ptr also trying to manage that resource.

### 数组支持

​		In C++17 and earlier, std::shared_ptr does not have proper support for managing arrays, and should not be used to manage a C-style array. As of C++20, std::shared_ptr does have support for arrays. 	

### 线程安全性

### make_shared

```c++
std::make_shared<A>(params); // 效率更高,一次new，空间上更少
std::shared_ptr(new A(params)); // 两次new ，空间更多
```

​		make_shared在某些情况下是不能使用的，比如当构造函数私有时。

### enable_shared_from_this

​		在构造函数和析构函数的函数栈上不能进行`shared_from_this()`的调用，会抛出`bad_weak_ptr`的运行异常。因为在构造和析构的函数栈上，对象语义都是不完整的，没有完整的内存模型，因而不能由此来获取智能指针。



## std::weak_ptr

### 循环引用

​		循环引用(也称为循环引用或循环)是一系列引用，其中每个对象引用下一个对象，最后一个对象引用回第一个对象，导致引用循环。

> 引用不需要是实际的c++引用——它们可以是指针、惟一id或任何其他标识特定对象的方法。

​		以循环形式使用`std::shared_ptr`的实际效果是，每个对象最终都使下一个对象保持激活状态：最后一个对象使第一个对象保持活动状态。因此，系列中的任何对象都不能被释放，因为它们都认为其他对象仍然需要它!

```c++
#include <iostream>
#include <memory> // for std::shared_ptr
#include <string>

class Person
{
	std::string m_name;
	std::shared_ptr<Person> m_partner; // initially created empty

public:

	Person(const std::string &name): m_name(name)
	{
		std::cout << m_name << " created\n";
	}
	~Person()
	{
		std::cout << m_name << " destroyed\n";
	}

	friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
	{
		if (!p1 || !p2)
			return false;

		p1->m_partner = p2;
		p2->m_partner = p1;

		std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

		return true;
	}
};

int main()
{
	auto lucy { std::make_shared<Person>("Lucy") }; // create a Person named "Lucy"
	auto ricky { std::make_shared<Person>("Ricky") }; // create a Person named "Ricky"

	partnerUp(lucy, ricky); // Make "Lucy" point to "Ricky" and vice-versa

	return 0;
}
```

​		也可以通过自限引用的方式形成最简单的循环引用:

```c++
#include <iostream>
#include <memory> // for std::shared_ptr

class Resource
{
public:
	std::shared_ptr<Resource> m_ptr {}; // initially created empty

	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};

int main()
{
	auto ptr1 { std::make_shared<Resource>() };

	ptr1->m_ptr = ptr1; // m_ptr is now sharing the Resource that contains it

	return 0;
} 
```



### 打破循环引用

​		`std::weak_ptr`被设计用来解决上面描述的“循环引用所有权”问题。

​		`std::weak_ptr`是一个观察者，可以用`std::shared_ptr`的方式观察并访问同一对象，但它不属于对资源的一个共享者。

```c++
#include <iostream>
#include <memory> // for std::shared_ptr and std::weak_ptr
#include <string>

class Person
{
	std::string m_name;
	std::weak_ptr<Person> m_partner; // note: This is now a std::weak_ptr

public:

	Person(const std::string &name): m_name(name)
	{
		std::cout << m_name << " created\n";
	}
	~Person()
	{
		std::cout << m_name << " destroyed\n";
	}

	friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
	{
		if (!p1 || !p2)
			return false;

		p1->m_partner = p2;
		p2->m_partner = p1;

		std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

		return true;
	}
};

int main()
{
	auto lucy { std::make_shared<Person>("Lucy") };
	auto ricky { std::make_shared<Person>("Ricky") };

	partnerUp(lucy, ricky);

	return 0;
}
```

​		但是不能通过`std::weak_ptr`来直接访问资源，它没有`operator->`。需要先将`std::weak_ptr`转换为`std::shared_ptr`再使用。

```c++
class Person
{
public:
    ...
        
	const std::shared_ptr<Person> getPartner() const { return m_partner.lock(); } // use lock() to convert weak_ptr to shared_ptr
	const std::string& getName() const { return m_name; }
};

int main()
{
	auto lucy { std::make_shared<Person>("Lucy") };
	auto ricky { std::make_shared<Person>("Ricky") };

	partnerUp(lucy, ricky);

	auto partner = ricky->getPartner(); // get shared_ptr to Ricky's partner
	std::cout << ricky->getName() << "'s partner is: " << partner->getName() << '\n';

	return 0;
}
```

​		这个从`std::weak_ptr`获取的`std::shared_ptr`，成为一个局部作用域的变量，不会像类的成员一样保有更长的生命周期。它在局部作用域超出时，就会销毁，对资源的共享引用随之减少一个。

### `std::weak_ptr`的悬垂

​		裸指针在释放后不置空时，无法探测其是否悬垂。这是裸指针最大的问题之一。

​		因为`std::weak_ptr`不实际管理资源的生命周期，所以`std::weak_ptr`同裸指针一样有可能指向一个已经被`std::shared_ptr`释放的资源。但是`std::weak_ptr`虽然不增加对资源的引用计数，但是它观察了资源的引用计数，可以由此判断对象是否还有效。

​		The easiest way to test whether a std::weak_ptr is valid is to use the `expired()` member function, which returns `true` if the std::weak_ptr is pointing to an invalid object, and `false` otherwise.

```c++
// h/t to reader Waldo for an early version of this example
#include <iostream>
#include <memory>

class Resource
{
public:
	Resource() { std::cerr << "Resource acquired\n"; }
	~Resource() { std::cerr << "Resource destroyed\n"; }
};

// Returns a std::weak_ptr to an invalid object
std::weak_ptr<Resource> getWeakPtr()
{
	auto ptr{ std::make_shared<Resource>() };
	return std::weak_ptr{ ptr };
} // ptr goes out of scope, Resource destroyed

// Returns a dumb pointer to an invalid object
Resource* getDumbPtr()
{
	auto ptr{ std::make_unique<Resource>() };
	return ptr.get();
} // ptr goes out of scope, Resource destroyed

int main()
{
	auto dumb{ getDumbPtr() };
	std::cout << "Our dumb ptr is: " << ((dumb == nullptr) ? "nullptr\n" : "non-null\n");

	auto weak{ getWeakPtr() };
	std::cout << "Our weak ptr is: " << ((weak.expired()) ? "expired\n" : "valid\n");

	return 0;
}
```

​		Note that if a std::weak_ptr is expired, then we shouldn’t call `lock()` on it, because the object being pointed to has already been destroyed, so there is no object to share. If you do call `lock()` on an expired std::weak_ptr, it will return a std::shared_ptr to `nullptr`.