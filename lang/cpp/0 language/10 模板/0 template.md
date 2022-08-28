# template

​		the template system was designed to simplify the process of creating functions (or classes) that are able to work with different data types.

---

​		Just like a normal definition, a **template** describes what a function or class looks like. Unlike a normal definition (where all types must be specified), in a template we can use one or more placeholder types. 

​		Templates have one job: to generate functions / class (that are compiled and executed).

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



## template instantiation

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



### **Abbreviated function templates**

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

​		Just like function templates, class templates are typically defined in header files so they can be included into any code file that needs them. 



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