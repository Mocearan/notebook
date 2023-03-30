# type conversion

​		The process of converting a value from one data type to another data type is called a **type conversion**.

---



​		Type conversion can be invoked in one of two ways: either implicitly (as needed by the compiler), or explicitly (when requested by the programmer). 

## The standard conversions 标准转换

​		The C++ language standard defines how different fundamental types (and in some cases, compound types) can be converted to other types.These conversion rules are called the **standard conversions**.

​		The standard conversions can be broadly divided into 4 categories, each covering different types of conversions:

- Numeric promotions 数值提升
- Numeric conversions 数值转换
- Arithmetic conversions 算数转换
- Other conversions (which includes various pointer and reference conversions)

​		When a type conversion is needed, the compiler will see if there are standard conversions that it can use to convert the value to the desired type. The compiler may apply zero, one, or more than one standard conversions in the conversion process.

> How do you have a type conversion with zero conversions?
>
> As an example, on architectures where `int` and `long` both have the same size and range, the same sequence of bits is used to represent values of both types. 



### Numeric promotions

> ​		A wider data type is one that uses more bits, and a narrower data type is one that uses less bits.
>
> - Some 32-bit processors (such as the x86 series) can manipulate 8-bit or 16-bit values directly. 
> - Other 32-bit CPUs (like the PowerPC), can only operate on 32-bit values, and additional tricks must be employed to manipulate narrower values.
>
> ​		C++ is designed to be portable and performant across a wide range of architectures,C++ defines a category of type conversions informally called the `numeric promotions`.

​		保护值不被改变类型转换成为提升（promotion）。执行算数运算之前，通常把较短的整数类型通过整型提升（integral promotion）成`int`。提升的结果一般不会是`long`（除非运算对象的类型是`char16_t / char32_t / wchar_t`或者本身比`int`大的一个普通枚举类型）或`long double`。这反映了C语言中类型提升的本质：把算数对象变得复合算数运算的“自然尺寸”。

​		整型提升的规则：

- 如果`int`能表示类型为`char / signed char / unsigned char / short char / short int / unsigned short int`的值
  - 则将该数据转换为`int`
  - 否则转化`unsigned int`
- `char16_t / char32_t / wchar_t`或普通枚举类型的数据转换成下列类型中的第一个能够表示其全部值的类型：`int / usigned int / long / unsigned long / unsigned long long`
- 如果位域的全局值能用`int`表示，则它转换为`int`；否则如果全部值能用`unsigned int`表示，则它转换为`unsignedint``
  - 如果`int`和`unsigned int`都不行，则不执行任何整数提升
- `bool`值转换成`int`，其中`false -> 0 / true -> 1`

​		A **numeric promotion** is the type conversion of a narrower numeric type (such as a `char`) to a wider numeric type (typically `int` or `double`) that can be processed efficiently and is less likely to have a result that overflows.

​		All numeric promotions are **value-preserving**, which means that all values in the original type are representable without loss of data or precision in the new type.

> Because such promotions are safe, the compiler will freely use numeric promotion as needed, and will not issue a warning when doing so.

​		The numeric promotion rules are divided into two subcategories: `integral promotions` and `floating point promotions`.



#### Floating point promotions

​		Using the **floating point promotion** rules, a value of type `float` can be converted to a value of type `double`.

​		This means we can write a function that takes a `double` and then call it with either a `double` or a `float` value:

```c++
#include <iostream>

void printDouble(double d)
{
    std::cout << d;
}

int main()
{
    printDouble(5.0); // no conversion necessary
    printDouble(4.0f); // numeric promotion of float to double

    return 0;
}
```



#### Integral promotions

​		Using the **integral promotion** rules, the following conversions can be made:

- `signed char` or `signed short` can be converted to `int` 

  signed range conversion, 1-2 byte to 2 byte, ok

- `unsigned char`, `char8_t`, and `unsigned short` can be converted to `int` if` int` can hold the entire range of the type, or `unsigned int` other wise

  unsigned range convert to signed range, 1-2 byte to 2 byte, first int, overflow to unsigned int

- if `char` is `signed` by default, it follows the `signed char` conversion rules above. if it is `unsigned` by default, it follows the `unsigned char` conversion rules above. 

- `bool` can be converted to `int`, with false becoming 0 and true becoming 1.

​	In most cases, this lets us write a function taking an `int` parameter, and then use it with a wide variety of other integral types. 

```c++
#include <iostream>

void printInt(int x)
{
    std::cout << x;
}

int main()
{
    printInt(2);

    short s{ 3 }; // there is no short literal suffix, so we'll use a variable for this one
    printInt(s); // numeric promotion of short to int

    printInt('a'); // numeric promotion of char to int
    printInt(true); // numeric promotion of bool to int

    return 0;
}
```



​		There are two things worth noting here.:

- First, on some systems, some of the integral types may be converted to `unsigned int` rather than `int`.
-  Second, some narrower unsigned types (such as `unsigned char`) will be converted to larger signed types (such as `int`).

​		So while integral promotion is value-preserving, it is not necessarily sign-preserving.



#### **Not all value-preserving conversions are numeric promotions**

​		Some value-preserving type conversions (such as `char` to `short`, `int` to `long`, or `int` to `double`) are not considered to be numeric promotions in C++ (they are `numeric conversions`)

​		This is because such conversions do not assist in the goal of converting smaller types to larger types that can be processed more efficiently.

​		The distinction is mostly academic. However, in certain cases, the compiler will favor numeric promotions over numeric conversions. 



### Numeric conversions

​		**numeric conversions**, that cover additional type conversions not covered by the numeric promotion rules.

​		五中基础类型的数值转换：

1. 整数类型转换：整数类型能被转换为其他整数类型(excluding integral promotions):

   ```c++
   short s = 3; // convert int to short
   long l = 3; // convert int to long
   char ch = s; // convert short to char
   // 普通枚举类型值也能被转换成整数类型
   // 如果目标类型是unsigned的，结果值所占的二进制位数以目标类型为准，如果有必要会丢掉前部的二进制位
   	// 确切的说，转换前的整数值对2的n次幂取模后所得的结果值就是转换结果，其中n是目标类型所占的位数
   	unsigned char uc = 1023;  // 1023->001111111111   uc->11111111->255
   // 如果目标类型是signed，则当前原值能用目标类型表示时，不发生改变；反之结果毅力阿玉实现
   	signed char sc = 1023; // 依赖于实现
   ```

2. 浮点类型转换：浮点数类型能被转换为其他浮点数类型(excluding point promotions):

   ```c++
   // 如果原值能用目标类型完整地表示，则所得的结果与原值相等
   float f = 3.0; // convert double to float
   long double ld = 3.0; // convert fouble to long double
   
   float f = FLT_MAX;
   double d = f;
   // 如果原值结语两个相邻的目标值之间，则结果取他们中的一个
   double d2 = DBL_MAX;
   long double ld = d2;
   // 其他情况下结果是未定义的
   double d2 = DBL_MAX;
   float f2 = d2;  // 如果FLT_MAX < DBL_MAC，UB
   
   long double ld2 = numeric_cast<long double>::max();
   double d3 = ld2; // 如果sizeof(long double) > sizeof(double)， UB
   ```

3. 浮点向整数转换:

   ​	浮点值的小数部分被忽略。

   ```c++
   int i = 3.5; // convert double to int
   
   char b = 2000.7; // 2000.7 -> 2000 -> 整形转换
   ```

4. Converting an integral type to any floating point type:

   ```c++
   double d = 3; // convert int to double
   ```

5. 布尔类型转换：整数、浮点数、指针都能隐式地转换成`bool`类型:

   ​	`!0 == true / 0 == false`

   ```c++
   bool b1 = 3; // convert int to bool
   bool b2 = 3.0; // convert double to bool
   ```




​	`{}`禁止一些窄化类型转换，如果潜在的窄化类型住哪换确实无法避免，则应该考虑使用一些在运行时执行检查的类型转换函数（如`narrow_cast<>`



​		指针和引用类型转换：

- 任何指向对象类型的指针都能隐式地转换成`void*`

- 指向派生类的指针或引用，能隐式地转换成其可访问的且明确无二义的基类指针或引用

- 指向函数的指针和指向成员的指针不能隐式的转换成`void*`

- 求值结果为0的常量表达式能隐式的转换成

  - 任意指针类型的空指针

    ```c++
    double* p = (1+2)*(2*(1-1)); // 最好直接使用 nullptr
    ```

  - 指向成员的指针类型

- `T*` 可以隐式转换成`const T*`。类似地，`T&`能隐式的转换成`const T&`

> 指向成员的指针或引用的类型在类中介绍。



#### Narrowing conversions

​		窄化类型转换是结果可能会损失数据的数值转换。

> 把某个类型的值转换到另一个类型，如果再转换回原类型并且初始值不变，该转换就成为值保护的转换，否则就是窄化类型转化。

​		In general, narrowing conversions should be avoided. If you do need to perform one, use explicit conversion, Such as `static_cast` 

​		Such narrowing conversions include:

- floating point type to integral type

- wider floating point type to a narrower floating point type

  - unless value being converted 

    - is `constexpr` 

    - and is in range of the destination type

      > even if the narrower type doesn’t have the precision to store the whole number

- integral to a floating point type

  - unless the value being converted :
    - is `constexpr` 
    - and is in range of the destination type 
    - and can be converted back into the original type without data lost

- wider integral type to a narrower integral type

  - unless the value being converted
    - is `constexpr`
    - and after integral promotion will fit into the destination type



​		compiler will usually issue a warning(or error) when it determine that an implicit narrowing conversion is required.

> Compilers will often *not* warn when converting a signed int to an unsigned int, or vice-versa, even though these are narrowing conversions. 
>
> Be extra careful of inadvertent conversions between these types (particularly when passing an argument to a function taking a parameter of the opposite sign).



##### `{}` disallows narrowing conversions

​		Narrowing conversions are strictly disallowed when using brace initialization(which is one of the primary reasons this initialization form is preferred).

```c++
char c{2.3};
```



### Arithmetic conversion

​		c++会在基础类型的算数运算和赋值运算中进行有意义的类型转换，使得他们可以自由的组合。目的是保证表达式计算的结果具有更高的精度。

​		If one of these operators is invoked with operands of different types, one or both of the operands will be implicitly converted to matching types using a set of rules called the **usual arithmetic conversions**.

​		require operands to be of the same type:

- The binary arithmetic operators: +, -, *, /, %
- The binary relational operators: <, >, <=, >=, ==, !=
- The binary bitwise arithmetic operators: &, ^, |
- The conditional operator ?: (excluding the condition, which is expected to be of type `bool`)

​		The usual arithmetic conversion rules are pretty simple.

- If the type of at least one of the operands is on the priority list, the operand with lower priority is converted to the type of the operand with higher priority.
- Otherwise (the type of neither operand is on the list), both operands are numerically promoted

> prioritized list：
>
> - long double (highest)
> - double
> - float
> - unsigned long long
> - long long
> - unsigned long
> - long
> - unsigned int
> - int (lowest)



​		适用于二元运算符运算对象的算数类型转换，用于把他们转换成一种常见的类型，并且该类型作为运算结果的类型：

> 这个规则使得转换后的结果要么是无符号整型，要么是在实现中尺寸更大的带符号的整型。这也是我么你要求避免在同一条表达式中混用有无符号整型的原因之一。

- 如果一个运算对象的类型是`long double`，则另一个也转换成`long double`
  - 否则，如果一个运算对象的类型是`double`，则另一个也转换成`double`
  - 否则，若谷一个运算对象的类型是`float`，则另一个也转换`float`
  - 否则，两个运算对象都执行整数提升
- 否则，如果一个运算对象的类型是`unsigned long long`，则另一个也转化成`unsigned long long`
  - 否则，如果一个运算对象的类型是`long long int`，而另一个运算对象的类型是`unsigned long int`
    - 则当`long long int`能表示所有`unsigned long int`的值时，该`unsigned long int`转换成`long long int`
    - 否则，两个运算对象都转换成`unsigned long long int`
  - 否则，如果一个运算对象的类型是`long int`，而另一个运算对象的类型是`unsigned int`
    - 则当`long int`能表示所有`unsigned int`的值时，该`unsigned int`转化成`long int`
    - 否则，两个运算对象都转换成`unsigned long int`
  - 否则，如果一个运算对象的类型是`long`，则另一个对象也转化成`long`
  - 否则，如果一个运算对象的类型是`unsigned`，则另一个也转化成`unsigned`
  - 否则，两个运算对象都转换成`int`



## Implicit type conversion

​		**Implicit type conversion** (also called **automatic type conversion** or **coercion**) is performed automatically by the compiler when one data type is required, but a different data type is supplied. 

​		When a type conversion is invoked (whether implicitly or explicitly), the compiler will determine whether it can convert the value from the current type to the desired type. If a valid conversion can be found, then the compiler will produce a new value of the desired type. 

> Note that type conversions don’t change the value or type of the value or object being converted.

​		If the compiler can’t find an acceptable conversion, then the compilation will fail with a compile error. 

- the compiler might not know how to convert a value between the original type and the desired type

- statements may disallow certain types of conversions. 

  ```c++
  int x { 3.5 }; // brace-initialization disallows conversions that result in data loss
  ```

  

### cases

​		implicit type conversion happens in all of the following cases:

- When initializing (or assigning a value to) a variable with a value of a different data type

  ```c++
  double d{ 3 }; // int value 3 implicitly converted to type double
  d = 6; // int value 6 implicitly converted to type double
  ```

- When the type of a return value is different from the function’s declared return type:

  ```c++
  float doSomething()
  {
      return 3.0; // double value 3.0 implicitly converted to type float
  }
  ```

- When using certain binary operators with operands of different types:

  ```c++
  double division{ 4.0 / 3 }; // int value 3 implicitly converted to type double
  ```

- When using a non-Boolean value in an if-statement

  ```c++
  if (5) {} // int value 5 implicitly converted to type bool
  
  ```
  
- When an argument passed to a function is  a different type than the function parameter:

  ```c++
  void doSomething(long l){}
  
  doSomething(3); // int value 3 implicitly converted to type long
  ```
  



### 显式化隐式转换

```c++
// Taken from google-protobuf stubs/common.h
//
// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda) and others
//
// Contains basic types and utilities used by the rest of the library.

//
// Use implicit_cast as a safe version of static_cast or const_cast
// for upcasting in the type hierarchy (i.e. casting a pointer to Foo
// to a pointer to SuperclassOfFoo or casting a pointer to Foo to
// a const pointer to Foo).
// When you use implicit_cast, the compiler checks that the cast is safe.
// Such explicit implicit_casts are necessary in surprisingly many
// situations where C++ demands an exact type match instead of an
// argument type convertable to a target type.
//
// The From type can be inferred, so the preferred syntax for using
// implicit_cast is the same as for static_cast etc.:
//
//   implicit_cast<ToType>(expr)
//
// implicit_cast would have been part of the C++ standard library,
// but the proposal was submitted too late.  It will probably make
// its way into the language in the future.
template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}

// When you upcast (that is, cast a pointer from type Foo to type
// SuperclassOfFoo), it's fine to use implicit_cast<>, since upcasts
// always succeed.  When you downcast (that is, cast a pointer from
// type Foo to type SubclassOfFoo), static_cast<> isn't safe, because
// how do you know the pointer is really of type SubclassOfFoo?  It
// could be a bare Foo, or of type DifferentSubclassOfFoo.  Thus,
// when you downcast, you should use this macro.  In debug mode, we
// use dynamic_cast<> to double-check the downcast is legal (we die
// if it's not).  In normal mode, we do the efficient static_cast<>
// instead.  Thus, it's important to test in debug mode to make sure
// the cast is legal!
//    This is the only place in the code we should use dynamic_cast<>.
// In particular, you SHOULDN'T be using dynamic_cast<> in order to
// do RTTI (eg code like this:
//    if (dynamic_cast<Subclass1>(foo)) HandleASubclass1Object(foo);
//    if (dynamic_cast<Subclass2>(foo)) HandleASubclass2Object(foo);
// You should design the code some other way not to need this.

template<typename To, typename From>     // use like this: down_cast<T*>(foo);
inline To down_cast(From* f)                     // so we only accept pointers
{
  // Ensures that To is a sub-type of From *.  This test is here only
  // for compile-time type checking, and has no overhead in an
  // optimized build at run-time, as it will be optimized away
  // completely.
  if (false)
  {
    implicit_cast<From*, To>(0);
  }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
  assert(f == NULL || dynamic_cast<To>(f) != NULL);  // RTTI: debug mode only!
#endif
  return static_cast<To>(f);
}
```





## explicit type conversion

​		explicit requests by the programmer, this form of type conversion is often called an **explicit type conversion** (as opposed to implicit type conversion, where the compiler performs a type conversion automatically).

​		C++ supports 5 different types of casts: `C-style casts`, `static casts`, `const casts`, `dynamic casts`, and `reinterpret casts`. The latter four are sometimes referred to as **named casts**.

> Avoid const casts and reinterpret casts unless you have a very good reason to use them.



### C-style casts

​		In standard C programming, casts are done via the () operator, with the name of the type to convert the value placed inside the parenthesis.

​		`(type)value`

​		C++ will also let you use a `C-style cast` with a more function-call like syntax:

​		`type(value)`

​		在语义上，C-style cast包含了`static cast` / `const cast`和`reinterpret cast`，这是一个充满了误用风险的转换形式，使用c++ `named casts`替换很容易避免这些误用。

> [C++ background: Static, reinterpret and C-Style casts | Anteru's Blog](https://anteru.net/blog/2007/c-background-static-reinterpret-and-c-style-casts/)



### static_cast 

​		**static_cast**, which can be used to convert a value of one type to a value of another type.

​		The `static_cast` operator takes an expression as input, and returns the evaluated value converted to the type specified inside the angled brackets.

​		`static_cast` is best used to convert one fundamental type into another.

​		advantage of `static_cast`：

- provides compile-time type checking
- can’t inadvertently remove `const`



### dynamic_cast 

​		在处理多态的场景下，遇到基类提供的虚函数接口不能表达派生类特有的功能接口时，需要将一个基类引用（指针或引用）的对象转化为实际的派生类对象来进行访问。

​		将派生类对象转换为一个基类对象，因为派生类拥有基类部分，进行对象切割，丢弃派生类拓展的部分就能得到完整的基类对象。这种转型是安全的，称为向上转型（upcasting）。

​		使用引用（指针，引用）时，依据类型信息确定的指针可访问的宽度，和直接使用对象转型是同理的。也是向上转型。

​		而将一个基类对象转型为其派生类对象是不安全的，因为派生类拓展部分的信息无法由基类对象提供。称为downcasting。

​		但使用引用（指针，引用）时，进行downcasting就不一定不安全。因为基类可能引用基类对象、期望转型到的派生类对象、非期望转型到的七类派生类对象。此时，需要提供类型信息来进行转型的验证是否为期望转型到的派生类对象。通过验证的转换，即为安全的downcasting。

​		`dynamic_casting`，通过RTTI提供的动态类型信息，进行转换时的对象信息验证来进行转换，存在转换失败的可能，即转换的对象实际类型与期望转换到的类型不同。

​		转换失败时，返回空指针。

​		`dynamic_cast`在一些情况下不能使用：

- `protected / private`继承
- 没声明或继承任何虚函数的继承链（因此没有虚函数表来进行动态类型信息验证）
- 涉及虚基类的某些情况（[dynamic_cast 运算符 | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/cpp/dynamic-cast-operator?redirectedfrom=MSDN&view=msvc-170)

```c++
#include <iostream>
#include <string>

class Base
{
protected:
	int m_value{};

public:
	Base(int value)
		: m_value{value}
	{
	}

	virtual ~Base() = default;
};

class Derived : public Base
{
protected:
	std::string m_name{};

public:
	Derived(int value, const std::string& name)
		: Base{value}, m_name{name}
	{
	}

	const std::string& getName() const { return m_name; }
};

Base* getObject(bool returnDerived)
{
	if (returnDerived)
		return new Derived{1, "Apple"};
	else
		return new Base{2};
}

int main()
{
	Base* b{ getObject(true) };

	Derived* d{ dynamic_cast<Derived*>(b) }; // use dynamic cast to convert Base pointer into Derived pointer
    
    if (d) // make sure d is non-null
		std::cout << "The name of the Derived is: " << d->getName() << '\n';
	delete b;

	return 0;
}


//////////  可以是引用的
int main()
{
	Derived apple{1, "Apple"}; // create an apple
	Base& b{ apple }; // set base reference to object
	Derived& d{ dynamic_cast<Derived&>(b) }; // dynamic cast using a reference instead of a pointer

	std::cout << "The name of the Derived is: " << d.getName() << '\n'; // we can access Derived::getName through d

	return 0;
}
// Because C++ does not have a “null reference”, dynamic_cast can’t return a null reference upon failure. Instead, if the dynamic_cast of a reference fails, an exception of type std::bad_cast is thrown.
```

​		这种动态的类型一致性检查，因为使用RTTI提供的动态类型信息，具有运行时的性能损耗，因此很多人认为`dynamic_cast`是糟糕的类设计实践，建议通过编译器选项禁用RTTI。

> RTTI，Run-time type information(RTTI)，是c++的特性之一，在运行时公开关于对象数据类型的信息，`dynamic_cast`使用了该功能。
>
> RTTI有相当大的空间性能成本，一些编译器允许关闭RTTI进行优化，使得`dynamic_cast`不能使用。

​		作为替代，应该尽量使用虚函数来完成所有的动态抽象。通常，也应该优先使用虚函数而不是downcasting，但有时downcasting是更好的选择：

- 当不能在基类中添加虚函数来实现派生类的功能时（如基类时标准库，或某些不易修改的三方库提供）
- 当需要访问特定于派生类而不能添加到基类的接口时（如派生类的access funciton)
- 向基类添加虚函数没有意义时（如基类没有要返回的恰当类型信息或值信息，此时如果户需要实例化虚函数，可以将基类的虚函数改为纯虚函数，派生类中通过改变返回值来完成该功能）



​		`static_cast`也能进行downcasting，类似于静态的强制转换，不验证期望的派生类类型信息，因而不安全。

> use static_cast unless you’re downcasting, in which case dynamic_cast is usually a better choice. However, you should also consider avoiding casting altogether and just use virtual functions.

