

# 对象的生存期

​		每个对象和引用都有生存期（lifetime），这是一项运行时性质。

---

​		每个对象或引用在程序执行时都存在一个时刻，开始其生存期，也都存在一个时刻，结束其生存期。

​		对象的生存期要么和它的存储空间的生存期相同，或者内嵌于其中。

>   内嵌其中：即对象的生存期可能是他的存储空间的生存期的一个子集。
>
>   ​	如该存储空间没有被释放，但在该存储地址上构造了新对象，使得旧对象生存期结束。

​		一般而言，对象的生存期从对象的构造函数完成的那一刻开始，直到析构函数执行完位置。

## 对象的存储类别

​		不同存储期的对象具有不同的生存期：

- 自动存储期对象，除非特别说明，函数中声明的对象在定义处被创建，在超出作用域时被销毁。大多数实现中，自动对象被创建在栈帧上。

- 静态存储期对象，全局作用域、名字空间作用域和其他作用域中声明为`static`的变量，在程序运行时创建并初始化一次，程序结束运行时销毁。大多数实现中，静态存储期对象存储在静态内存中。

  > 地址唯一，多线程共享，需要加锁保护。

- 动态存储期对象，`new / delete`来自由控制生存期。

- 线程存储期对象，`thread_local`，与线程的生命周期相同。

  ​	另外还有一种临时存储期对象，通常也是自动存储期对象的一种，是计算的中间结果或用于存放`const`引用实参的值对象，不具名没有身份不可索引。生命周期由用法决定：

  - 临时对象绑定到引用，则由引用接管声明周期
  - 否则，临时对象的声明周期与它所处的完整表达式一致。



- `auto` 或 无说明符

  ​	具有自动存储期（c++11前）。 是存储类别的默认项，含义在c++11后变更。

  ​	只能用于块作用域或函数形参作用域中声明的对象。

- `register` 

  ​	具有自动存储期

  ​	另外提示编译器将此对象优化存储到CPU的寄存器中存储。(弃用)  (C++17 前)。

  ​	只能用于块作用域或函数形参作用域中声明的对象。

- `thread_local` 

  ​	线程存储期（c++11 起）。

  ​	只能用于命名空间作用域声明的对象、在块作用域声明的对象及静态数据成员。

  ​	指示对象具有线程存储期。它能与 `static` 或 `extern` 结合，以分别指定内部或外部链接（但静态数据成员始终拥有外部链接），此时 `static` 不影响存储期，只说明链接性。

  > ​		从不同作用域指代的且带内部或外部链接的 `thread_local` 变量的名字可能指代相同或不同的实例，这取决于代码是否线程执行。
  >
  > ```c++
  > #include <iostream>
  > #include <string>
  > #include <thread>
  > #include <mutex>
  > 
  > std::mutex cout_mutex;
  > 
  > class TL
  > {
  >  public:
  >  TL()
  >  {
  >      std::cout << std::this_thread::get_id() << " TL()" << std::endl;
  >  }
  >  ~TL()
  >  {
  >      std::cout << std::this_thread::get_id() << " ~TL()" << std::endl;
  >  }
  > 
  >  unsigned int rage = 1;
  > };
  > 
  > thread_local TL tl;
  > // thread_local unsigned int rage = 1;
  > 
  > void increase_rage( const std::string &thread_name )
  > {
  >  ++tl.rage;
  >  std::lock_guard<std::mutex> lock { cout_mutex };
  >  std::cout << thread_name << " rage count: " << tl.rage << '\n';
  > }
  > 
  > int main()
  > {
  >  std::thread a { increase_rage, "a" }, b { increase_rage, "b" };
  >  {
  >      std::lock_guard<std::mutex> lock { cout_mutex };
  >      std::cout << "main's rage count: " << tl.rage << '\n';
  >  }    
  >  a.join();
  >  b.join();
  > }
  > ```

- `static` 

  ​	静态或线程存储期，内部连接。

  ​	可用于对象声明（除函数形参作用域外）、函数声明（除块作用域外）、匿名联合体声明上。

  - 用于声明类成员时，它会声明一个静态成员
  - 用于声明对象时，它指定静态存储期（除非与 `thread_local` 协同出现）
  - 命名空间作用域内声明时，它指定内部链接。

- `extern` 

  ​	静态或线程存储期， 外部连接。

  ​	可用于变量声明和函数声明（除了类成员或函数形参）

  ​	指定外部链接，技术上不影响存储期。但不能用来定义自动存储期的对象，故所有 `extern` 对象都具有静态或线程存储期。

  ​	使用 `extern` 且没有初始化器的声明不是定义。

  > For variables, creating a forward declaration is also done via the `extern` keyword (with no initialization value).
  >
  > ```c++
  > //a.cpp
  > // global variable definitions
  > int g_x { 2 }; // non-constant globals have external linkage by default
  > extern const int g_y { 3 }; // this extern gives g_y external linkage
  > 
  > //main.cpp
  > #include <iostream>
  > 
  > extern int g_x; // this extern is a forward declaration of a variable named g_x that is defined somewhere else
  > extern const int g_y; // this extern is a forward declaration of a const variable named g_y that is defined somewhere else
  > 
  > int main()
  > {
  >  std::cout << g_x; // prints 2
  > 
  >  return 0;
  > }
  > ```
  >
  > Note that function forward declarations don’t need the `extern` keyword -- the compiler is able to tell whether you’re defining a new function or making a forward declaration based on whether you supply a function body or not.
  >
  > ```c++
  > // External global variable definitions:
  > int g_x;                       // defines non-initialized external global variable (zero initialized by default)
  > extern const int g_x{ 1 };     // defines initialized const external global variable
  > extern constexpr int g_x{ 2 }; // defines initialized constexpr external global variable
  > 
  > // Forward declarations
  > extern int g_y;                // forward declaration for non-constant global variable
  > extern const int g_y;          // forward declaration for const global variable
  > extern constexpr int g_y;      // not allowed: constexpr variables can't be forward declared
  > ```
  >
  > > 因为翻译单元隔离的性质，多个翻译单元之间定义不可见，所以`constexpr`变量不能分离在头文件和源文件中，必须定义在头文件中。

- `mutable`

  ​	不影响存储期和链接性。

  > 见c++语言/基本语法/声明/说明符/CV限定符

声明中只能出现一个存储类别说明符，但`thread_local`和`static / extern`可以结合使用。

> 在 C 中，不能取 `register` 变量的地址，但在 C++ 中，声明为 `register` 的对象与声明不带任何存储类说明符的变量在语义上没有区别。

除了`thread_local`以外的存储类别说明符都不能在[显式特化](https://zh.cppreference.com/w/cpp/language/template_specialization)及[显式实例化](https://zh.cppreference.com/w/cpp/language/class_template#.E6.98.BE.E5.BC.8F.E5.AE.9E.E4.BE.8B.E5.8C.96)中使用：

```c++
template<class T>
  struct S {
    thread_local static int tlm;
  };

template<>
thread_local int S<float>::tlm = 0; // 这里没有"static"
```



## 对象的生存期

​		程序中所有的对象都具有下列存储期：

- 自动存储期 automatic

  ​	对象的存储在外围代码块开始时分配在栈上，在代码块结束时解分配。

  ​	未声明为`static / extern / thread_local`的所有局部对象均拥有此存储期

- 线程存储期 thread

  ​	对象在线程开始时分配，在线程结束时解分配。

  ​	每个线程拥有自身的对象实例。

  ​	声明为`thread_local`的对象拥有此存储期。`thread_local`能与`static / extern`一同出现，用于调整链接性。

- 静态存储期 static

  ​	对象的存储在程序开始时分配在初始化数据段、bss段，在程序结束时解分配。

  ​	静态存储期的对象只存在一个实例。

  ​	命名空间作用域下的对象、声明为`static / extern`的对象均拥有此存储期。

- 动态存储期

  ​	对象的存储通过动态内存分配函数来按请求进行分配和解分配。

![image-20220514111642125](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220514111642125.png)

## 对象生存期的开始

-   将获得与其类型对应的正确对齐方式和大小的存储

-   完成其初始化（若存在）（包括不经由构造函数或经由**平凡默认构造函数**的默认初始化），除非：

    -   对象是联合体成员或其子对象，生存期的开始决定于联合体成员是否在联合体中初始化，或者联合体是否被激活

    -   若对象内嵌与联合体对象，则其生存期开始于**平凡特殊成员函数**赋值或构造含有它的联合体对象

    -   如果数组对象是通过``std::allocator::allocate`分配的，它的生命周期也可以开始。

        >   **平凡默认构造函数：**	
        >
        >   ​		不进行任何动作的构造函数，所有与C语言兼容的数据类型（POD类型）都是可平凡默认构造的。
        >
        >   ps：c++语言/基础语法/类/特殊成员函数/默认构造函数/平凡默认构造函数
        >
        >   **平凡特殊成员函数：**
        >
        >   ​		trivial special member function.
        >
        >   ​		即特殊成员函数的每个不执行任何特殊操作的版本。
        >
        >   ps：c++语言/基础语法/类/特殊成员函数/*/平凡成员函数



​		某些操作会在给定的存储区域中**隐式创建对象**，并开始其生存期，这些对象拥有**隐式生存期类型**。

## 对象生存期的结束

-   如果是非类类型，则在销毁该对象时结束（从20起，可能会过**伪析构函数**调用结束）

    >   **伪析构函数调用：**
    >
    >   ​		唯一作用在于计算点号或箭头前面的*后缀表达式*
    >
    >   ps: c++语言/基础语法/类/特殊成员函数/析构函数/伪析构函数调用
    >
    >   https://blog.csdn.net/weixin_30642267/article/details/97374403

-   如果是类类型，则是**析构函数**调用开始时

    >   **析构函数：**
    >
    >   ​		释放对象可能在其生存期间获得的资源。
    >
    >   ps: c++语言/基本语法/类/特殊成员函数/析构函数

-   如果对象占据的存储空间被释放，或者被不内嵌在该对象中的对象所重用。

>   





## 静态变量的生存期

- 所有静态变量（静态存储期）的生命周期一直驻留到进程退出为止
- 但是全局静态变量的创建时机与进程一致，分布在各个编译单元的全局静态静态变量作为程序执行的上下文信息而先于main的载入而创建。
- 局部静态变量，当程序流程第一次到达该处时才创建且无论流程经历多少次都只创建一次，并驻留到程序结束为止。
- 全局作用域下的变量，默认都是静态存储期的外部变量，加上static声明为静态存储期的内部变量。

## 引用的生存期

​		引用严格意义上说不具有生存期，因为引用只是一个名字，不具有对所引用对象生存期的管理：引用引入的名字在作用域后，不会结束名字对应实体的生存期。

​		但引用所引入的名字，从初始化完成之时开始，在作用域后结束。

>   **NOTE：**
>
>   ​	被引用对象的生生存期可能在引用的生存期结束之前就结束，这会造成**悬垂引用**。
>
>   ​	这类似于主线程创建的子线程，主线程可能在子线程之前结束。但引用没有`join`这样机制，需要在编码阶段就考虑好



## 隐式生存期类型

​		隐式生存期类型：

-   标量类型

-   数组类型

-   聚合类型

    ​	通过花括号初始化器列表初始化聚合体。

-   至少一个平凡的合格构造函数 或 一个平凡且未被删除的析构函数 的类类型

-   或以上类型的的**cv限定**版本

    ​	即**const和volatile**类型限定符，可出现于任何类型说明符（包括声明说明符序列）中，以指定被声明对象或被命名类型的常量性（constness）或易变性（volatile）。

​	若隐式创建的对象有不是隐式生存期类型的子对象，则其生存期不会隐式开始。



# 对象的链接性

​		An identifier’s **linkage** determines whether other declarations of that name refer to the same object or not.

> scope defines where a single declaration can be seen and used.
>
> Linkage defines whether multiple declarations refer to the same object or not.

​		指代对象、引用、函数、类型、模板、命名空间或值的名字，具有链接性。

​		如果某个名字具有链接性，那么它所指代的实体与另一作用域中的声明所引入的相同名字指代的是相同的实体。如果有变量、函数或其他实体在数个作用域声明但没有足够的链接，那么就会生成该实体的多个实例。

- `static` 内部链接
- `extern` 外部链接
- 无链接
- 模块链接

> https://www.cnblogs.com/chenhuan001/p/7504365.html
>
> https://blog.csdn.net/qq_15698613/article/details/96315170

Global variable and functions identifiers can have either `internal linkage` or `external linkage`. 



## 内部链接

名字在当前翻译单元中生效。

An identifier with **internal linkage** can be seen and used within a single file, but it is not accessible from other files (that is, it is not exposed to the linker（链接器）). 

> This means that if two files have identically named identifiers with internal linkage, those identifiers will be treated as independent.

内部链接在多个翻译单元中各自生成不在翻译单元之间链接的实体，因而在多翻译单元之间不违反ODR。

- 命名空间作用域下声明的任何名字都默认具有内部链接。

  > 所有在无名命名空间及其内嵌命名空间中声明的名字，即便显式声明为`extern`，也均有内部链接

- 声明为 `static` 的变量、变量模板 (C++14 起)、函数或函数模板具有内部链接；

  - use the `static` keyword make non-constant global variable  have internal linkage 

  - Const and constexpr global variables have internal linkage by default 

    > don’t need the `static` keyword -- if it is used, it will be ignored
    >
    > > global: 全局命名空间作用域（C 中的文件作用域）
    >
    > 在 C 中具有外部链接，但在 C++ 中具有内部链接。

  > ```c++
  > static int g_x; // non-constant globals have external linkage by default, but can be given internal linkage via the static keyword
  > 
  > const int g_y { 1 }; // const globals have internal linkage by default
  > constexpr int g_z { 2 }; // constexpr globals have internal linkage by default
  > 
  > // This function is declared as static, and can now be used only within this file
  > // Attempts to access it from another file via a function forward declaration will fail
  > static int add(int x, int y)
  > {
  >  return x + y;
  > }
  > 
  > int main()
  > {
  >  return 0;
  > }
  > ```
  >
  > 

- 非`extern`声明的`const/constexpr`限定的变量，且未被声明为具有外部链接，非`valatile`，非模板，非`inline`，且未被`export`，具有内部链接。

- 匿名联合体的数据成员具有内部

## 外部链接

可以从其他翻译单元中的作用域引用该名称。

An identifier with **external linkage** can be seen and used both from the file in which it is defined, and from other code files (via a forward declaration). 

> Variables and functions with external linkage also have [language linkage](https://en.cppreference.com/w/cpp/language/language_linkage), which makes it possible to link translation units written in different programming languages.

> The `extern` keyword can also be used to specify [language linkage](https://en.cppreference.com/w/cpp/language/language_linkage) and [explicit template instantiation declarations](https://en.cppreference.com/w/cpp/language/class_template), but it's not a storage class specifier in those cases (except when a declaration is directly contained in a language linkage specification, in which case the declaration is treated as if it contains the `extern` specifier).
>
> **language linkage:**
>
> ​		Provides for linkage between modules written in different programming languages.
>
> c++/语言/声明/**linkage specification**

非匿名命名空间中声明的任意名字默认具有外部链接性：

- 未声明为 `static` 的函数、命名空间作用域内未声明为 `static` 的非 const 变量，和所有声明为 `extern` 的变量
- 枚举
- 类及其成员函数、静态数据成员（无论const）、嵌套类及枚举，在类体中首次使用友元声明引入的函数;
- 未声明为 `static` 的函数模板

具名模块中可导出的名字具有外部链接性

块作用域中首次声明为`extern`的变量名、首次声明的函数名，都具有外部链接。

全局作用域下名字默认具有外部链接。

> 可被其他编译单元使用extern进行引用。（引用性声明，不能extern定义，重定义）。

> To make a global variable external (and thus accessible by other files), we can use the `extern` keyword
>
> ```C++
> int g_x { 2 }; // non-constant globals are external by default
> 
> extern const int g_y { 3 }; // const globals can be defined as extern, making them external
> extern constexpr int g_z { 3 }; // constexpr globals can be defined as extern, making them external (but this is useless, see the note in the next section)
> 
> int main()
> {
>  return 0;
> }
> ```
>
> 

## 无链接

名字只在声明它的作用域内有效。

- 块作用域下声明的任何名字默认均无链接：

- 非`extern`声明的变量，无论是否`static`修饰

  > 块作用域下，可以使用extern引用编译单元全局作用域下外部链接性的变量。

- 块作用域下的局部类声明，及其成员函数

- 块作用域下声明的其他名字，如`typedef`、枚举及枚举项

任何作用域下，没有指定为外部链接、模块链接或内部链接的名字都是无链接的。

## 模块链接

名字只能从同一模块单元或其他翻译单元中同名模块引用。

在命名空间作用域中声明的名字，如果它们的声明附加到一个命名的模块上，并且没有导出，并且没有内部链接，那么它们就具有模块链接。

命名空间作用域中声明的，附加在一个具名模块上且不导出的具有内部链接的名字，具有模块链接。

## Linkage summary

An identifier’s *linkage* determines whether multiple declarations of an identifier refer to the same identifier or not.

- An identifier with

   

  no linkage

   

  means the identifier only refers to itself. This includes:

  - Local variables
  - User-defined type definitions (such as enums and classes) declared inside a block

- An identifier with

   

  internal linkage

   

  can be accessed anywhere within the file it is declared. This includes:

  - Static global variables (initialized or uninitialized)
  - Static functions
  - Const global variables
  - Functions declared inside an unnamed namespace
  - User-defined type definitions (such as enums and classes) declared inside an unnamed namespace

- An identifier with

   

  external linkage

   

  can be accessed anywhere within the file it is declared, or other files (via a forward declaration). This includes:

  - Functions
  - Non-const global variables (initialized or uninitialized)
  - Extern const global variables
  - Inline const global variables
  - User-defined type definitions (such as enums and classes) declared inside a namespace or in the global scope

