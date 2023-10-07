# type

​		**data type** tell compiler how to interpret the contents of memory in some meaningful way. 

​		Every name and every expression has a type that determines the operations that may be performed on it.

​		给原本无意义的**位序列**提供了语义含义。

---

​		c++是一种静态类型语言，类型系统是基础。编译器在处理任何实体（对象、值、引用、函数、名字和表达式）时，必须清楚的确定其类型。

​		类型与实现它们的机器硬件密切相关。c++语言本身提供的类型都是计算机硬件能直接处理的基础类型，没有内置的高级数据类型。作为补充，通过标准库来提供许多高级数据类型。

​		类型决定了实体能进行哪些操作，这些操作给原本无意义的**位序列**提供了语义含义。

​		**类型对于不同的数据类型，应用不同的数据解译方案**，这就使得同样的数据（纯粹的二进制数据，即**位序列**），可以表达多种多样的含义，从而构建起了计算机抽象现实世界的基础。

- A *type* defines a set of possible values and a set of operations (for an object).
- An *object* is some memory that holds a value of some type.
- A *value* is a set of bits interpreted according to a type.
- A *variable* is a named object.



## 类型检查

​		c++通过类型系统和一些辅助的特性，帮助程序员表达设计。

​		静态类型和编译时的类型检查，对内置类型和用户自定义类型提供同等的支持。

​		静态类型系统的目标是支持基于对象的编程风格而：将不同的想法表示为不同的类型，如整型、浮点型、字符串、原始内存和对象，而不是到处使用泛型。

​		c++保留了大多数C语言的类型特性，保留了语言特性到硬件的直接映射。

​		c++致力于完整的类型安全，但同C一样，是“强类型、弱检查”的。

## memory and type

​		The smallest unit of memory is a **binary digit** (also called a **bit**), which can hold a value of 0 or 1. 

​		Memory is organized into sequential units called **memory addresses** (or **addresses** for short). the memory address allows us to find and access the contents of memory at a particular location.

​		 the memory address allows us to find and access the contents of memory at a particular location.each memory address holds 1 byte of data.

​		We use a **data type** (often called a “type” for short) to tell the compiler how to interpret the contents of memory in some meaningful way. 

> When you give an object a value, the compiler and CPU take care of encoding your value into the appropriate sequence of bits for that data type, which are then stored in memory (remember: memory can only store bits). 

​		most objects actually take up more than 1 byte of memory. A single object may use 2, 4, 8, or even more consecutive memory addresses. The amount of memory that an object uses is based on its data type.

​		we typically access memory through variable names (and not directly via memory addresses), the compiler is able to hide the details of how many bytes a given object uses from us. 



​		计算机以2的整数次幂个bit为单位划块来处理内存数据，可寻址的最小内存块称为“字节”（byte）。存储的基本单元称为“字”（word），通常由几个字节组成，视不同机器有所不同。

​		c++中，一个字节要至少能容纳机器基本字符集中的字符。大多数机器的字节由8bit构成，字则由32或64bit构成，也就是4或8字节。

​		计算机将内存中的每个字节与一个数字（address，地址）关联，在一个字节为8bit、字为32bit的机器上，我们可能看待一个字的内存区如图：

![image-20220608063536846](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220608063536846.png)

​		左侧地址，右侧是每个字节的8bit。

​		可以使用某个地址来表示从这个地址开始的大小不同的比特串。为了赋予内存中某个地址明确地含义，必须首先知道存储在该地址的数据的类型。类型决定了数据所占的比特数以及该如何解释这些比特的内容。

​		即，**内存只是简单的存放了二进制数据，对这些二进制数据的定义与解释依赖于类型，不同类型有对这个数据的不同的解释方式。导致同一数据在不同的类型上有不同的表现。**



## 类型的分类

### 按照声明分类

- 基础类型：`std::is_fundamental`
  - `void`类型：`std::is_void`
  - `std::nullptr_t`：`std::is_null_pointer`
  - 算数类型：`std::is_arithmetic`
    - 浮点类型（float、double、long double）：`std::is_floating_point`
    - 整数类型：`std::is_integral`
      - `bool`类型
      - 字符类型
        - 窄字符类型
          - 通常字符类型：`char、signed char、unsigned char`
          - `char8_t`
        - 宽字符类型：`char16_t、char32_t、wchar_t`
      - 有符号整数类型：`short int、long int、long long int`
      - 无符号整数类型：`unsigned short int、unsigned int、unsigned long int、unsigned long long int`
- 复合类型：`std::is_compound`
  - 引用类型：`std::is_reference`
    - 左值引用：`std::is_lvalue_reference`
      - 到对象的左值引用类型+
      - 到函数的左值引用类型
    - 右值引用：`std::is_rvalue_reference`
      - 到对象的右值引用类型
      - 到函数的右值引用类型
  - 指针类型：`std::is_pointer`
    - 指向对象的指针（对象指针）类型
    - 指向函数的指针（函数指针）类型
  - 指向成员的指针（成员指针）：`st d::is_member_pointer`
    - 指向数据成员的指针类型：`std::is_member_object_pointer`
    - 指向成员函数的指针类型：`std::is_member_function_pointer`
  - 数组类型：`std::is_array`
  - 函数类型：`std::is_function`
  - 枚举类型：`std::is_enum`
  - 类类型：
    - 非联合体类型：`std::is_class`、`struct`
    - 联合体类型：`std::is_union`

**对于除了引用和函数外的每个类型，类型系统还支持该类型的三个附加cv限定版本（const、volatile、const volatile）**

> **cv限定：**
>
> ​	类型说明符序列中，用以指定被声明对象的常量性或易变性。
>
> ps：c++/基础语言/声明/说明符/cv类型限定符

​		枚举类型和类类型也常被称为用户自定义类型（user-defined type）。基础类型和除用户自定义类型外的其他复合类型称为内置类型。

### 按照性质分类

​		根据类型的各项性质，将它们分组到不同的类别之中：

- 对象类型

  是非函数类型、非饮用类型且非`void`类型的（可有cv限定的）类型：`std::is_object`

- 标量类型

  可有cv限定的算数、指针、成员指针、枚举和`std::nullptr_t`类型：`std::is_scalar`

- 类型特征库中的具名要求类型

  - 平凡类型：`std::is_trivial`

  - POD类型：`std::is_pod`

  - 字面类型：`std::is_literal_type`

  - 其他类别

    > **类型支持：**
    >
    > ps：c++语言/标准库/语言支持库/类型支持
    >
    > **具名要求：**
    >
    > ​		C++标准的规范性文本中使用的具名要求，用以定义标准库的期待。
    >
    > ps：c++语言/标准库/具名要求

### 按照结构分类

> **Types in C++ are：**
>
> **Short version:** 
>
> -   Object types: scalars, arrays, classes, unions
> -   Reference types
> -   Function types
> -   (Member types) [see below]
> -   void
>
> ------
>
> **Long version**
>
> -   Object types
>     -   Scalars
>         1.  arithmetic (integral, float)
>         2.  pointers: T * for any type T
>         3.  enum
>         4.  pointer-to-member
>         5.  nullptr_t
>     -   Arrays: T[] or T[N] for any complete, non-reference type T
>     -   Classes: class Foo or struct Bar
>         1.  Trivial classes
>         2.  Aggregates
>         3.  POD classes
>         4.  (etc. etc.)
>     -   Unions: union Zip
> -   References types: T &, T && for any object or free-function type T
> -   Function types
>     -   Free functions: R foo(Arg1, Arg2, ...)
>     -   Member functions: R T::foo(Arg1, Arg2, ...)
> -   void
>
> Member types work like this. A member type is of the form T::U, but you can't have objects or variables of member type. You can only have member *pointers*. A member pointer has type T::* U, and it is a pointer-to-member-object if U is a (free) object type, and a pointer-to-member-function if U is a (free) function type.
>
> All types are complete except void, unsized arrays and declared-but-not-defined classes and unions. All incomplete types except void can be completed.
>
> All types can be const/volatile qualified.
>
> The <type_traits> header provides trait classes to check for each of these type characteristics.

c++支持广泛的数据类型，定义了内置数据类型也为程序员提供了自定义数据类型的机制。因此c++标准库定义了一些更加复杂的数据类型，比如可变长字符串和向量等。

> c++类型系统由语言支持的类型和标准库提供的类型支持共同构建。



## type aliases



### using

​		In C++, **using** is a keyword that creates an alias for an existing data type.

`using type_name_t = type;`

> Many type alias names use a “_t” or “_type” suffix to help decrease the chance of naming collisions with other identifiers.

```c++
using distance_t = double; // define distance_t as an alias for type double
distance_t milesToDestination{ 3.4 }; // defines a variable of type double
```

​		An alias does not actually define a new type -- it just introduces a new identifier for an existing type. An alias is completely interchangeable with the aliased type.

> This allows us to do things that are syntactically valid but semantically meaningless. For example:
>
> ```c++
> using miles_t = long; 
> using speed_t = long;
> 
> miles_t distance{5};
> speed_t mhz{3200};
> // both long
> 
> // The following is syntactically valid (but semantically meaningless)
> distance = mhz;
> ```
>
> ​		Because the compiler does not prevent these kinds of semantic errors for type aliases, we say that aliases are not **type safe**. In spite of that, they are still useful.
>
> > ​		Some languages support the concept of a **strong typedef** (or strong type alias). A strong typedef actually creates a new type that has all the original properties of the original type, but the compiler will throw an error if you try to mix values of the aliased type and the strong typedef. As of C++20, C++ does not directly support strong typedefs (though enum classes are similar), but there are quite a few 3rd party C++ libraries that implement strong typedef-like behavior.

​		Because scope is a property of an identifier, type alias identifiers follow the same scoping rules as variable identifiers: a type alias defined inside a block has block scope and is usable only within that block, whereas a type alias defined in the global namespace has file scope and is usable to the end of the file.

​		If you need to use one or more type aliases across multiple files, they can be defined in a header file and #included into any code files that needs to use the definition:

```c++
// mytypes.h
#ifndef MYTYPES
#define MYTYPES

	using miles_t = long;
	using speed_t = long;

#endif // MYTYPES
```

​		

### typedef

​		**typedef** (which is short for “type definition”) is a keyword with the same semantics as “using”, but reversed syntax.

`typedef type type_name_t;`

​		Typedefs are still in C++ for historical reasons, but their use is discouraged.

​		the syntax for typedefs can get ugly with more complex types. 

> ```c++
> typedef int (*fcn_t)(double, char); // fcn_t hard to find
> using fcn_t = int(*)(double, char); // fcn_t easier to find
> ```



### using type aliases for platform independent coding

​		One of the uses for type aliases is that they can be used to hide platform specific details. 

​		In order to make sure each aliased type resolves to a type of the right size, type aliases of this kind are typically used in conjunction with preprocessor directives:

```c++
#ifdef INT_2_BYTES
using int8_t = char;
using int16_t = int;
using int32_t = long;
#else
using int8_t = char;
using int16_t = short;
using int32_t = int;
#endif
```

> ​		The fixed-width integers (such as `std::int_fast16_t` and `std::int_least32_t`) and `size_t` type are actually just type aliases to various fundamental types.

### using type aliases to make complex types simple

```c++
using pairlist_t = std::vector<std::pair<std::string, int>>; // make pairlist_t an alias for this crazy type

bool hasDuplicates(pairlist_t pairlist) // use pairlist_t in a function parameter
{
    // some code here
    return false;
}

int main()
{
     pairlist_t pairlist; // instantiate a pairlist_t variable

     return 0;
}
```



### using type aliases for legibility

​		Type aliases can also help with code documentation and comprehension.

```c++
using testScore_t = int;
testScore_t gradeTest();
```



### using type aliases for easier code maintenance

​		However, if you use type aliases, then changing types becomes as simple as updating the type alias (e.g. from `using studentID_t = short;` to `using studentID_t = long;`).





