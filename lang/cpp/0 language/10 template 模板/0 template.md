# template

​		the template system was designed to simplify the process of creating functions (or classes) that are able to work with different data types.

---

​		Just like a normal definition, a **template** describes what a function or class looks like. Unlike a normal definition (where all types must be specified), in a template we can use one or more placeholder types. 

​		模板不是类或函数，而是生成类或函数的规则。

​		Templates have one job: to generate functions / class (that are compiled and executed).

​		Templates are a compile-time mechanism, so their use incurs no run-time overhead compared to hand-crafted code.

​		And because templates can be written agnostically of specific types, programming with templates is sometimes called **generic programming**.

​		templates do have a few drawbacks:

- expand into a crazy amount of code, which can lead to code bloat and slow compile times

- produce crazy-looking, borderline unreadable error messages

  > These error messages can be quite intimidating, but once you understand what they are trying to tell you, the problems they are pinpointing are often quite straightforward to resolve.

​		A good rule of thumb is to create normal at first, and then convert them into templates if you find you need an overload for different parameter/member types.



## syntax

```c++
template < 形参列表 > requires子句(可选) 	// 声明	(1)	
export template < 形参列表 > 				// 声明	(2)	(C++11 前)
template < 形参列表 > concept 概念名 = 约束表达式 ;	// (3)	(C++20 起)
```

​		the primary difference is the addition of the type in angled brackets (called a **template argument**), which specifies the actual type that will be used in place of template type `T`.



## parameterized 参数化

​		The **template<typename T>** prefix makes **T** a type parameter of the declaration it prefixes.

​		 It is C++’s version of the mathematical “for all T” or more precisely “for all types T.” If you want the mathematical “for all T, such that P(T),” you use concepts。

> 这是一种无约束的对所有类型都不做检查的抽象。
>
> 而概念就是对模板参数的一种约束。

​		通常，模板只对满足特定条件的模板参数有意义。

> 如容器提供拷贝操作，那么容器要求元素类型是可拷贝的。

​		This **template<Element T>** prefix is C++’s version of mathematic’s “for all T such that **Element(T)**”; that is, **Element** is a predicate that checks whether **T** has all the properties that a **Vector** requires. Such a predicate is called a *concept*。

​		A template argument for which a concept is specified is called a *constrained argument* and a template for which an argument is constrained is called a *constrained template*.

### parameterized type 参数化类型

### parameterized operations 参数化操作



## template instantiation 模板实例化

​		A template plus a set of template arguments is called an *instantiation* or a *specialization*.

​		The process of creating functions / class (with specific types) from templates (with template types) is called **template instantiation** (or **instantiation** for short).

​		The process for instantiating a function is simple: the compiler essentially clones the template and replaces the template type (`T`) with the actual type we’ve specified (`int`).

​		当模板实参在模板中的应用不能满足模板的要求时，实例化会在编译会失败。

> 如模板实参不支持模板所要求的的操作，不能成功调用。

​		Template definitions are not subject to the one-definition rule, and functions instantiated from function templates are implicitly inline, so they are exempt from the one-definition rule.

> ```c++
> // Max.h
> #ifndef MAX_H
> #define MAX_H
> template <typename T>
> T max(T x, T y){return (x > y) ? x : y;}
> #endif
> 
> // Max.cpp
> #include "Max.h" // import template definition for max<T, T>()
> void foo(){std::cout << max(3, 2);}
> 
> // main.cpp
> #include "Max.h" // import template definition for max<T, T>()
> int main(void)
> {
>     std::cout << max(3, 5);
>     foo();
> }
> ```
>
> ​		both main.cpp and foo.cpp `#include "Max.h"` 



### Template argument deduction

​		In cases where the type of the arguments match the actual type we want, we do not need to specify the actual type -- instead, we can use **template argument deduction** to have the compiler deduce the actual type that should be used from the argument types in the function call.

```c++
max<int>(1,2); // specifying we want to call max<int>
max<>(1,2);
max(1,2);

// the compiler will consider both max<int> template function overloads and max non-template function overloads.

template <typename T>
T max(T x, T y)
{
    std::cout << "called max<int>(int, int)\n";
    return (x > y) ? x : y;
}

int max(int x, int y)
{
    std::cout << "called max(int, int)\n";
    return (x > y) ? x : y;
}
```

​		Note how the syntax in the bottom case looks identical to a normal function call! This is usually the preferred syntax used when invoking function templates.

> 当在模板实例化时，在实参中传递中包含模板参数时，就可以使用模板实参推导。



## non-type template parameters 非类型模板参数

​		非类型模板参数是一种模板形参，其中形参的类型是预定义的，并被替换为传入的实参的`constexpr`值。

> 非类型模板形参一般在模板类中作为明确的值来使用，且在编译阶段就要使用，如果不是`constexpr`就会产生编译错误。

​		费类型模板参数可以是一下任意一种类型：

- integral type 整型
- enumeration type 枚举类型
- pointer or reference to a class object 类对象的指针或引用
- pointer or reference to a function 函数指针或引用
- pointer or reference to a class member function 成员函数指针或引用
- std::nullptr_t 空指针类型
- floating point type 浮点类型



## 模板的分离编程

​		模板作为产生类和函数的规则，在生成类或函数时，需要具体的类型信息进行填充。如果将模板的实现定义分离在单独的翻译单元中，该翻译单元作为独立的编译产物会单独进行编译，从而没有足够的类型信息支撑。

​		最常用的分离方法是，以头文件文本扩展的方式，将实现分离在文本中，实际在编译期看来，它们仍然是同一头文件。分离的文本文件通常以`.inl(inline)`命名

> `.hpp`的实现文件通常以`.ipp / .ixx`命名

```c++
// Array.h
template<typename T>
class Array<T>
{
    ...
};

#include "Array.inl" // implement
```

> 这种方法的缺点是，随着`Array.h`被不同的翻译单元包含，导致不同翻译单元中包含众多的模板类副本（指模板类本身，而不是生成的类），可能会增加编译和链接事件（链接器应该删除重复的定义）
>
> 除非编译时间或链接时间过长成为一个重要问题，否则这个方法就是首选的。

​		另一种方法是通过混合翻译单元，即包含翻译单元来实现的。

```c++
// main.cpp

#include "Array.h"
#include "Array.cpp"

Array<int> a;
```

> 向用户暴露`.cpp`是糟糕行为，会造成很多难以预料的依赖问题。
>
> 这里实际上是将实现文件当成一种文本文件来使用。

​		还有一种方式是，以`three-file`的方式，向实现文件中引入类型信息。这种方法不向用户暴露`.cpp`，只是在内部使用时需要向每个模板类实现文件增加一个类型信息声明的`.cpp`。

```c++
// Ensure the full Array template definition can be seen
#include "Array.h"
#include "Array.cpp" // we're breaking best practices here, but only in this one place

// #include other .h and .cpp template definitions you need here

template class Array<int>; // Explicitly instantiate template Array<int>
template class Array<double>; // Explicitly instantiate template Array<double>

// instantiate other templates here
```

> This method may be more efficient (depending on how your compiler and linker handle templates and duplicate definitions), but requires maintaining the templates.cpp file for each program.



## function template

​		A **function template** is a function-like definition that is used to generate one or more overloaded functions, each with a different set of actual types.

```c++
template <typename T> // this is the template paremeter declaration
T max(T x, T y) // this is the function template definition for max<T>
{
    return (x > y) ? x : y;
}

max<int>(1, 2);
```

> ​		There is no difference between the `typename` and `class` keywords in this context.
>
> ​		prefer the newer `typename` keyword, because it makes it clearer that the template type can be replaced by any type (such as a fundamental type), not just class types.

​		When this process happens due to a function call, it’s called **implicit instantiation**. An instantiated function is often called a **function instance** (**instance** for short) or a **template function**. 



### return deduction

​		当使用模板参数类型与其他类型进行运算时，结果类型不能很在编码阶段很好的进行预估，那么可以使用返回值类型推导来让编译器来在实例化时决定返回值的类型。

```c++
#include <iostream>

template <typename T, typename U> // We're using two template type parameters named T and U
// T max(T x, U y) // x can resolve to type T, and y can resolve to type U
auto max(T x, U y)
{
    return (x > y) ? x : y; // uh oh, we have a narrowing conversion problem here
}

int main()
{
    std::cout << max(2, 3.5) << '\n';

    return 0;
}
```

​		This version of `max` now works fine with operands of different types

> This is a good use for an `auto` return type -- we’ll let the compiler deduce what the return type should be from the return statement



### Abbreviated function templates 函数模板缩写

​		C++20 introduces a new use of the `auto` keyword: When the `auto` keyword is used as a parameter type in a normal function, the compiler will automatically convert the function into a function template with each auto parameter becoming an independent template type parameter.

​		This method for creating a function template is called an **abbreviated function template**.

```c++
auto max(auto x, auto y)
{
	return (x > y) ? x : y;
}

// is shorthand in c++20 for the following:
template<typename T, typename U>
auto max(T x, U y)
{
	return ( x > y) ? x : y;
}
```

​		In cases where you want each template type parameter to be an independent type, this form is preferred as the removal of the template parameter declaration line makes your code more concise and readable.



## class template

​		a **class template** is a template definition for instantiating class types.

```c++
template <typename T, typename U>
struct Pair
{
    T first{};
    U second{};
};

Pair<int, double> p1{ 1, 2.3 }; // a pair holding an int and a double
Pair<double, int> p2{ 4.5, 6 }; // a pair holding a double and an int
Pair<int, int> p3{ 7, 8 };      // a pair holding two ints

template <typename T, typename U>
void print(Pair<T, U> p)
{
    std::cout << '[' << p.first << ", " << p.second << ']';
}

print(p2);
```

​		就像函数模板一样，类模板通常是在头文件中定义的，因此它们可以包含到任何需要它们的代码文件中。 

```c++
/*
 * @Author: Mass
 * @Date: 2022-11-22 11:28:03
 * @LastEditors: Mass
 * @LastEditTime: 2022-11-22 11:38:13
 * @FilePath: /cpp/lang/template/basic/Array.h
 */
#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>

template<typename T> // added
class Array
{
private:
    int length_ {};
    T *data_ {}; 

public:
    Array( int len )
    {
        assert( len, 0 );
        data_ = new T[ len ] {};// allocated an array of objects of type T
        length_ = len;
    }

    Array( const Array & ) = delete;
    Array &operator=( const Array & ) = delete;

    ~Array()
    {
        delete [] data_;
    }

    void erase()
    {
        delete [] data_;
        // We need to make sure we set data_ to 0 here, otherwise it will
        // be left pointing at deallocated memory!
        data_ = nullptr;
        length_ = 0;
    }

    T &operator[]( int index ) // now return a T&
    {
        assert( index >= 0 and index < length_ );
        return data_[ index ];
    }

    // templated getLength() function defined below
    int getLength() const;
};

// member functions defined outside the class need therir own template declaration
template<typename T>
int Array<T>::getLength() const // note class name is Array<T>, not Array
{
    return length_;
}

#endif //  ARRAY_H
```

- 在类声明之外定义的每个模板成员函数都需要自己的模板声明

- 模板类名时`Array<T>`，而不是`Array`，除非在类内部使用

  ```c++
  Array(const Array&) = delete;
  Array& operator=(const Array&) = delete;
  ```

  ​	当类内部使用没有模板参数的类名时，模板参数隐含的与当前实例化的类型相同。

```c++
#include <iostream>
#include "Array.h"

int main()
{
	Array<int> intArray { 12 };
	Array<double> doubleArray { 12 };

	for (int count{ 0 }; count < intArray.getLength(); ++count)
	{
		intArray[count] = count;
		doubleArray[count] = count + 0.5;
	}

	for (int count{ intArray.getLength() - 1 }; count >= 0; --count)
		std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';

	return 0;
}
```

​		模板类的实例化方式与模板函数相同：编译器根据需要生成一个副本，用用户实际的数据类型替换模板形参，然后编译副本。

​		模板类本身不会被编译。



### CTAD: Class template argument deduction

​		Starting in C++17, when instantiating an object from a class template, the compiler can deduce the template types from the types of the object’s initializer (this is called **class template argument deduction** or **CTAD** for short).

```c++
std::pair<int, int> p1{1,2}; // explicitly specify class template std::pair<int, int>(c++11 onward)
std::pair p2{1, 2}; // CTAD used to deduce std::pair<int, int> from the initializers (c++17)
```

- CTAD is only performed if no template argument list is present.

  ```c++
  std::pair<> p1 { 1, 2 };    // error: too few template arguments, both arguments not deduced
  std::pair<int> p2 { 3, 4 }; // error: too few template arguments, second argument not deduced
  ```

​		

​		**deduction guide**, which tells the compiler how to deduce the template arguments for a given class template.

```c++
template <typename T, typename U>
struct pair
{
    T first{};
    U second{};
};

// Here's a deduction guide for our pair class
// pair objects initialized with arguments of type T and U should deduce to pair<T, U>
template <typename T, typename U>
pair(T, U) -> pair<T, U>;

pair p2{ 1, 2 };     // CTAD used to deduce pair<int, int> from the initializers (C++17)
```

​		 C++20 added the ability for the compiler to automatically generate deduction guides for aggregate class types, so the version of `pair` without the deduction guides should compile in C++20.

> This assumes your compiler supports feature P1816



## 模板特化

​		针对特定类型，对模板的实现进行特殊的实现。



### explicit（full）template specialization

​		在主模板之后，使用特定的类型，重新定义模板。

> 当编译器尝试实例化对应的模板时，搜索到已经显式定义的针对该类型的实现，就会使用该实现替换泛型的模板实现。

- `template<>`全特化声明，声明此版本将所有模板参数特化

- 模板原型声明，以特定的类型，声明模板类或模板函数的原型

  

#### class template explicit specialization 类模板显式特化

​		类模板显式特化被视为完全独立的类，即使的他们的创建方式与模板类相同。因此，可以更改关于特化类的任何东西，包括实现方式、接口函数等。

```cpp
template <typename T>
class Storage8
{
private:
    T m_array[8];

public:
    void set(int index, const T& value)
    {
        m_array[index] = value;
    }

    const T& get(int index) const
    {
        return m_array[index];
    }
};

///////////// bool类型存储优化问题
// Requires the Storage8 type definition from above

template <> // the following is a template class with no templated parameters
class Storage8<bool> // we're specializing Storage8 for bool
{
// What follows is just standard class implementation details
private:
    unsigned char m_data{};

public:
    void set(int index, bool value)
    {
        // Figure out which bit we're setting/unsetting
        // This will put a 1 in the bit we're interested in turning on/off
        auto mask{ 1 << index };

        if (value)  // If we're setting a bit
            m_data |= mask;   // use bitwise-or to turn that bit on
        else  // if we're turning a bit off
            m_data &= ~mask;  // bitwise-and the inverse mask to turn that bit off
	}

    bool get(int index)
    {
        // Figure out which bit we're getting
        auto mask{ 1 << index };
        // bitwise-and to get the value of the bit we're interested in
        // Then implicit cast to boolean
        return (m_data & mask);
    }
};
```

​		While C++ gives us free reign to add, remove, or change functions of specialied class as we see fit, keeping a consistent interface means the programmer can use either class in exactly the same manner.

```c++
int main()
{
    // Define a Storage8 for integers (instantiates Storage8<T>, where T = int)
    Storage8<int> intStorage;

    for (int count{ 0 }; count < 8; ++count)
    {
        intStorage.set(count, count);
	}

    for (int count{ 0 }; count < 8; ++count)
    {
        std::cout << intStorage.get(count) << '\n';
    }

    // Define a Storage8 for bool  (instantiates Storage8<bool> specialization)
    Storage8<bool> boolStorage;

    for (int count{ 0 }; count < 8; ++count)
    {
        boolStorage.set(count, count & 3);
    }

	std::cout << std::boolalpha;

    for (int count{ 0 }; count < 8; ++count)
    {
        std::cout << boolStorage.get(count) << '\n';
    }

    return 0;
}
```



#### function template explicit specialization 函数模板显式特化

```c++
#include <iostream>
#include <string>

template <typename T>
class Storage
{
private:
    T m_value{};
public:
    Storage(T value)
        : m_value{ value }
    {
    }
    ~Storage() {}; // need an explicitly defined destructor to specialize

    void print()
    {
        std::cout << m_value << '\n';
    }
};

template <>
Storage<char*>::Storage(char* const value)
{
    if (!value)
        return;

    // Figure out how long the string in value is
    int length{ 0 };
    while (value[length] != '\0')
        ++length;
    ++length; // +1 to account for null terminator

    // Allocate memory to hold the value string
    m_value = new char[length];

    // Copy the actual value string into the m_value memory we just allocated
    for (int count = 0; count < length; ++count)
        m_value[count] = value[count];
}

template <>
Storage<char*>::~Storage()
{
    delete[] m_value;
}


template<> // 向编译器声明，这是一个模板，但是没有模板参数，已经显式指定了所有的模板参数
void Storage<double>::print()
{
    std::cout << std::scientific << m_value << '\n';
}

int main()
{
    // Define some storage units
    Storage<int> nValue { 5 };
    Storage<double> dValue { 6.7 };

    // Print out some values
    nValue.print();
    dValue.print();
    
    // Dynamically allocate a temporary string
    std::string s;

    // Ask user for their name
    std::cout << "Enter your name: ";
    std::cin >> s;

    // Store the name
    Storage<char*> storage(s.data());

    storage.print(); // Prints our name

    s.clear(); // clear the std::string

    storage.print(); // Prints our name
}
```





### template partial specialization 模板偏特化

​		模板偏特化是指针对部分模板参数进行特化。

​		模板偏特化只能对模板类进行，不能对模板函数进行。

```c++
#include <iostream>
#include <cstring>

template <typename T, int size> // size is the expression parameter
class StaticArray
{
private:
	// The expression parameter controls the size of the array
	T m_array[size]{};

public:
	T* getArray() { return m_array; }

	T& operator[](int index)
	{
		return m_array[index];
	}
};

template <typename T, int size>
void print(StaticArray<T, size>& array)
{
	for (int count{ 0 }; count < size; ++count)
		std::cout << array[count] << ' ';
}

// overload of print() function for partially specialized StaticArray<char, size>
template <int size>
void print(StaticArray<char, size>& array)
{
	for (int count{ 0 }; count < size; ++count)
		std::cout << array[count];
}

int main()
{
	// Declare an char array of size 14
	StaticArray<char, 14> char14{};

	std::strcpy(char14.getArray(), "Hello, world!");

	// Print the array
	print(char14);

	std::cout << ' ';

	// Now declare an char array of size 12
	StaticArray<char, 12> char12{};

	std::strcpy(char12.getArray(), "Hello, mom!");

	// Print the array
	print(char12);

	return 0;
}
```

> 这里不是对print()进行偏特化，而是使用StaticArray的不同偏特化对print进行重载。



#### 模板类成员偏特化

​		对模板类的成员函数也不能进行偏特化。解法是对模板类进行偏特化，改写想要特化的函数。

​		但需要将其他部分原样不动的拷贝，因为特化都是独立的，不会自动获得同名模板的功能。

​		为了复用这些需要拷贝的部分，通常在偏特化模板类成员时通过继承来复用代码。

```c++
#include <iostream>

template <typename T, int size> // size is the expression parameter
class StaticArray_Base
{
protected:
	// The expression parameter controls the size of the array
	T m_array[size]{};

public:
	T* getArray() { return m_array; }

	T& operator[](int index)
	{
		return m_array[index];
	}

	void print()
	{
		for (int i{ 0 }; i < size; ++i)
			std::cout << m_array[i] << ' ';
		std::cout << '\n';
	}

	virtual ~StaticArray_Base() = default;
};

template <typename T, int size> // size is the expression parameter
class StaticArray: public StaticArray_Base<T, size>
{
};

template <int size> // size is the expression parameter
class StaticArray<double, size>: public StaticArray_Base<double, size>
{
public:

	void print()
	{
		for (int i{ 0 }; i < size; ++i)
			std::cout << std::scientific << this->m_array[i] << ' ';
// note: The this-> prefix in the above line is needed.
// See https://stackoverflow.com/a/6592617 or https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members for more info on why.
		std::cout << '\n';
	}
};

int main()
{
	// declare an integer array with room for 6 integers
	StaticArray<int, 6> intArray{};

	// Fill it up in order, then print it
	for (int count{ 0 }; count < 6; ++count)
		intArray[count] = count;

	intArray.print();

	// declare a double buffer with room for 4 doubles
	StaticArray<double, 4> doubleArray{};

	for (int count{ 0 }; count < 4; ++count)
		doubleArray[count] = (4.0 + 0.1 * count);

	doubleArray.print();

	return 0;
}
```

#### 针对指针的偏特化

​		全特化需要完全解析模板类型，所以需要对每个指针类型都写一个单独的全特化。

​		为了将类型特化和指针特化分离，使用偏特化不完全解析类型信息，构建模板的指针版本。

```c++
#include <iostream>
#include <cstring>

// Our Storage class for non-pointers
template <typename T>
class Storage
{
private:
	T m_value;
public:
	Storage(T value)
        : m_value { value }
	{
	}

	~Storage()
	{
	}

	void print() const
	{
		std::cout << m_value << '\n';
	}
};

// Partial-specialization of Storage class for pointers
template <typename T>
class Storage<T*>
{
private:
	T* m_value;
public:
	Storage(T* value)
            : m_value { new T { *value } } // this copies a single value, not an array
	{
	}

	~Storage()
	{
		delete m_value;
	}

	void print() const
	{
		std::cout << *m_value << '\n';
	}
};

// Full specialization of constructor for type char*
template <>
Storage<char*>::Storage(char* value)
{
	// Figure out how long the string in value is
	int length { 0 };
	while (value[length] != '\0')
		++length;
	++length; // +1 to account for null terminator

	// Allocate memory to hold the value string
	m_value = new char[length];

	// Copy the actual value string into the m_value memory we just allocated
	for (int count = 0; count < length; ++count)
		m_value[count] = value[count];
}

// Full specialization of destructor for type char*
template<>
Storage<char*>::~Storage()
{
	delete[] m_value;
}

// Full specialization of print function for type char*
// Without this, printing a Storage<char*> would call Storage<T*>::print(), which only prints the first char
template<>
void Storage<char*>::print() const
{
	std::cout << m_value;
}

int main()
{
	// Declare a non-pointer Storage to show it works
	Storage<int> myint { 5 };
	myint.print();

	// Declare a pointer Storage to show it works
	int x { 7 };
	Storage<int*> myintptr { &x };

	// If myintptr did a pointer assignment on x,
	// then changing x will change myintptr too
	x = 9;
	myintptr.print();

	// Dynamically allocate a temporary string
	char* name { new char[40]{ "Alex" } };

	// Store the name
	Storage<char*> myname { name };

	// Delete the temporary string
	delete[] name;

	// Print out our name to prove we made a copy
	myname.print();
}
```

​		