# 标识符

​		标识符是一个字符序列，由数字字符、下划线、大小写拉丁字母和大多数Unicode字符进行任意长度的组合。长度不限，大小写敏感。

---



## 标识符命名规则

​		有效的标识符必须以一个非数字字符（拉丁字母、下划线、Unicode非数字字符）开头。标识符区分大小写，每个字符都起作用。

>   **note：**
>
>   ​	C++ 的文法中形式上要求 Unicode 字符以 `\u` 或 `\U` 进行**转义**，但根据**翻译阶段 1**，这只不过是将源代码中的原始 Unicode 字符呈现给编译器的方式。还要注意的是，对这个功能特性的支持可能是有限的，比如 [gcc](https://gcc.gnu.org/wiki/FAQ#What_is_the_status_of_adding_the_UTF-8_support_for_identifier_names_in_GCC.3F).
>
>   **转义：**
>
>   ​	转义序列用于在**字符串字面量**和**字符字面量**中表示某些特殊字符。
>
>   ​	ps：c++/基本语言/表达式/初等表达式/字面量/字符字面量/转义序列

​		c++语言的保留字不能被用作标识符。
​		c++也为标准库保留了一些名字。

> 用户自定义的标识符中不能连续出现两个下划线，也不能以下划线紧连大写字母开头。
> 此外，定义在函数体外的标识符不能以下划线开头



### 命名规则

-   大小写的英文字母
-   阿拉伯数字（不能做首字符）
-   下划线

​		编译器的字符解析支持Unicode之后，实质上已经可以包含世界上所有的自然语言了。唯一一条保留的规定是：阿拉伯数字不能做首字符。

​		但是，作为语言的保留关键字和运算符不能作为变量名。



### 常用的几种命名规范（变量、函数）

-   大驼峰， Pascal命名法

    UploadFile，通常用来命名类。结构体等复合数据类型

-   小驼峰

    uploadFIle, 通常用来命名函数名、局部变量名

-   匈牙利命名法

    属性+类型+描述 ， 局部变量不需要属性

    属性：

    -   全局变量g_
    -   成员变量m_
    -   常量：c_
    -   静态变量：s_

    类型：

    -   指针 p
    -   函数 fn  （pfnBuy, 函数指针变量）
    -   句柄 h
    -   整型 i/n
    -   长整型 l
    -   布尔 b
    -   浮点 f 
    -   文件 f

-   蛇形命名法(upload_file)

Sometimes you’ll see a mix of the two: underscores used for variables and intercaps used for functions.

When working in an existing program, use the conventions of that program (even if they don’t conform to modern best practices). Use modern best practices when you’re writing new programs.

In any case, avoid abbreviations.

​		不建议将类型信息加到名字中

- 降低了抽象水平，尤其不利于泛型编程
- 编译器更加擅长追踪类型信息
- 改变名的类型时，需要修改所有该名字
- 随着用到的类型越来越多，设计的缩写集会越来越大，有些含糊不清，有些过于啰嗦
- 名字应当



## 声明中的名字与标识符

### 功能

​	有效的标识符必须以一个非数字字符（拉丁字母、下划线、Unicode非数字字符）开头。标识符区分大小写，每个字符都起作用。

>   **note：**
>
>   ​	C++ 的文法中形式上要求 Unicode 字符以 `\u` 或 `\U` 进行[转义](https://zh.cppreference.com/w/cpp/language/escape)，但根据[翻译阶段 1](https://zh.cppreference.com/w/cpp/language/translation_phases) ，这只不过是将源代码中的原始 Unicode 字符呈现给编译器的方式。还要注意的是，对这个功能特性的支持可能是有限的，比如 [gcc](https://gcc.gnu.org/wiki/FAQ#What_is_the_status_of_adding_the_UTF-8_support_for_identifier_names_in_GCC.3F).
>
>   **转义：**
>
>   ​	转义序列用于在**字符串字面量**和**字符字面量**中表示某些特殊字符。
>
>   ​	ps：c++/基本语言/表达式/初等表达式/字面量/字符字面量/转义序列

### 僵尸标识符

​	某些标识符在历次c++标准的迭代中逐渐被移除，但是在特定的语境中，它们为兼容早起标准而被保留。

![image-20210401145101585](https://gitee.com/masstsing/picgo-picserver/raw/master/20210401145101.png)



## 表达式中的名字与标识符

​	命名某个变量、函数、概念的特化或枚举项的标识符可以作为表达式使用。仅由这个标识符组成的表达式的结果，是该标识符所命名的实体。

### 表达式的类型

​	若该标识符命名的是某个函数、变量、模板形参对象或数据成员，则表达式的**值类别**为左值，否则为**纯右值**（例如枚举项是纯右值表达式，概念的特殊是bool纯右值）。

该表达式类型的确定：

-   如果这个（无限定的）标识符所命名的实体是局部实体，并且被`lambda表达式`以复制捕获时，该表达式的类型是 嵌套的最内层`lambda`表达式的闭包对象中 被这种俘获所声明的非静态数据成员的**类成员访问表达式**的类型。

    >   ```c++
    >   void f() {
    >    float x, &r = x;
    >    [=] {
    >      decltype(x) y1;             // y1 拥有 float 类型
    >      decltype((x)) y2 = y1;      // y2 拥有 float const& 类型
    >                                  // 因为此 lambda 非 mutable 而 x 是左值
    >      decltype(r) r1 = y1;        // r1 拥有 float& 类型
    >      decltype((r)) r2 = y2;      // r2 拥有 float const& 类型
    >    };
    >   }
    >   ```
    >
    >   ps：c++语言/基本语法/表达式/初等表达式/lambda表达式/lambda捕获
    >
    >   **类成员访问表达式：**
    >
    >   ​		访问其操作数的一个成员。
    >
    >   ps：c++语言/基本语法/表达式/运算符/成员访问运算符

-   若命名的实体是某个T类型的**模板形参对象**，则表达式的类型为`const T`

    >   **模板形参对象：**
    >
    >   ​	每个模板都是有一个或多个模板形参所参数化的，它们在模板声明语法中的形参列表中指定。
    >
    >   ​	非类型的模板形参是一个模板形参对象，如
    >
    >   >   ```c++
    >   >   struct A { friend bool operator==(const A&, const A&) = default; };
    >   >   template<A a> void f() {
    >   >      &a; // OK
    >   >      const A& ra = a, &rb = a; // 都绑定到同一模板形参对象
    >   >      assert(&ra == &rb); // 通过
    >   >   }
    >   >   ```
    >
    >   ps：c++语言/基础语法/模板/形参与实参/模板形参/非类型模板形参

-   除此之外，表达式的类型与被命名的实体的类型相同。

>   在非静态成员函数内，每个非静态成员的标识符`member_name`都隐式变换成一个类成员访问表达式`this->member_name



### 表达式中的无限定标识符

除了声明了的标识符之外，下列项也可以当做声明了的标识符用在表达式中

-   函数写法的**重载运算符**名，比如 `operator+` 或 `operator new`；

    >   **重载运算符：**
    >
    >   ​		为用户定义类型的操作数定制c++运算符。
    >
    >   ps：c++语言/基础语法/表达式/运算符/运算符重载

-   **用户定义转换函数**的名字，比如 operator bool；

    >   **用户定义转换函数：**
    >
    >   ​		允许从类类型到其他类型的隐式转换或显式转换。
    >
    >   ps：c++语言/基础语法/表达式/类型转换/用户定义转换

-   **用户定义字面量运算符**的名字，比如 operator "" _km；

    >   **用户定义字面量运算符：**
    >
    >   ​		用户定义的字面量所调用的函数被称为字面量运算符。
    >
    >   ​	ps：c++ 语言/基本语法/表达式/初等表达式/字面量/用户定义字面量

-   模板的名后随实参列表，比如 `MyTemplate<int>`；

-   ``~ ``字符后随类名，比如 ``~MyClass`；

-   ``~`` 字符后随 **`decltype`**说明符，比如 ``~decltype(str)``。

    >   **decltype说明符：**
    >
    >   ​		检查实体的声明类型，或表达式的类型和值类别。
    >
    >   ps：c++语言/基础语法/声明/说明符/简单类型说明符/decltype说明符

这些和标识符一起，被称作*无限定的标识表达式*。



### 表达式中的有限定标识符

​		有限定的标识表达式是在无限定的标识表达式前面带上**作用域解析运算符 ``::``**，并且可以选择性的带上一系列以作用域解析运算符分割的枚举、 类或命名空间的名字，或者 decltype 表达式 。

>   ​	如：`std::string::npos`，是命名在命名空间 std 内的类 string 中的静态成员 npos 的表达式。
>
>   ​	表达式` ::tolower `命名的是全局命名空间内的函数` tolower` 。
>
>   ​	表达式` ::std::cout `指名 std 命名空间（顶层命名空间）中的全局变量 cout 。
>
>   ​	表达式 `boost::signals2::connection` 指名的是声明于 `signals2` 命名空间中的类型 `connection` ，前者则声明于命名空间 `boost`。

​		有限定标识符中，可能会需要以关键词 `template`消除模板待决名的歧义。



# 名字

​		名字用来代表某个实体或者某个符号，它可以是以下之一：

-   标识符
-   函数写法的重载运算符的名字（`operator+ 、operator new`)
-   用户定义的转换函数的名字（`operator bool`）
-   用户定义的字面量运算符的名字（`operator "" _km`）
-   后随实参列表的模板名字(``TempName<int>``)



## 名字的功能

​		每个代表实体的名字都是由声明引入到程序（或者说翻译单元）中来的。代表标号的名字则既可以通过 goto 语句，也可以通过**标签语句**引入到程序中来。

> 同一个实体，可以由不同的名字来表示。

​		在多个翻译单元中使用的相同名字，可以根据其**链接性**代表相同或者不同的实体。

​		每当编译器在程序中遇到一个未知的名字时，它就会通过进行**名字查找**来将其与引入这个名字的声明联系起来，但对模板的声明和定义中的**待决名**不会这样做。（对于这些名字，编译器需要确定它们命名的是类型、模板还是某些其他实体，这可能需要**显式消歧义**

​		在名字的作用域结束时，结束名字对应实体的生存期。但引用引入的名字例外。

>   **待决名：**
>
>   ​		在模板定义中，某些构造的含义可以在不同的实例化间有所不同。
>
>   ​	ps：c++语言/基本语法/模板/待决名
>
>   **显式消歧义：**
>
>   ​		在模板的声明或定义中，部分情况下模板形参的名字具有歧义不能当做类型或值使用，需要显式使用`typename`或`template`显式的消除歧义。
>
>   ​	ps：c++语言/基本语法/模板/待决名/消歧义符



## 名字的作用域

​		作用域（scope）是程序的一部分，在其中名字有其特定的含义。换言之，名字只能在程序文本的某个特定区域使用。

> ​		大多数时候，程序中同一实体只被一个名字所表示，所以往往会讲“变量的作用域”，“对象的作用域”。
>
> ​		但实际上，在c++中，作用域是针对于名字的，因为c++引入了引用这一概念，使得变量能够被多个名字指代。

​		An identifier’s `scope` determines where an identifier can be accessed within the source code. 

​		Scope is a compile-time property, and trying to use an identifier when it is out of scope will result in a compile error.

​		c++中大多数作用域都以花括号分隔。

​		同一名字在不同作用域下指向不同的实体。名字的有效区域为当前作用域。

> main定义于所有花括号之外，它和其他大多数定义在函数体之外的名字一样拥有**全局作用域（global scope）** 一旦声明之后，全局作用域内的名字在整个文件的范围内都可使用。
>
> **NOTE：**
>
> ​		始终牢记，c++的编译单元是文件，生成不同的二进制文件，通过头文件引入名字，通过链接实现不同编译单元的交互。所谓全局，是指编译单元中的全局。

### 块作用域 block scope

local variable 拥有块作用域。将复合语句声明的代码块称为块作用域。由花括号指明块的范围。

> ​		一般来说，在对象第一次被使用的附近定义它是一种好的选择，因为这样有助于更容易地找到变量的定义。
> 更重要的是，当变量的定义与它的第一次被使用的地方很近时，我们也会给它赋值一个比较合理的初始值。

​		在一个嵌套的作用域中，拥有如下概念：

> 内层作用域（inner scope）
> 		外层作用域（outer scope）
> 		在作用域内（in scope)
>
> 如果内层作用域中有可能用到外层作用域或**全局作用域**的名字，则不宜再定义一个同名的局部对象。会因为产生冲突而造成名字遮蔽。
>

- 函数或lambda表达式引起的`{}`中的块作用域称为局部作用域

  ​	函数中的标签具有局部作用域

- `for/while/if/switch`语句中的`()`和`{}`构成的块作用域称为语句作用域

​		Local variables are destroyed in the opposite order of creation at the end of the set of curly braces in which it is defined (or for a function parameter, at the end of the function).



### 函数形参作用域

​		

### 命名空间作用域

A **namespace** is a region that allows you to declare names inside of it for the purpose of disambiguation. 



```c++
#include <iostream>

void print() // this print lives in the global namespace
{
	std::cout << " there\n";
}

namespace foo
{
	void print() // this print lives in the foo namespace
	{
		std::cout << "Hello";
	}

	void printHelloThere()
	{
		print(); // calls print() in foo namespace
		::print(); // calls print() in global namespace
	}
}

int main()
{
	foo::printHelloThere();

	return 0;
}
```



### 类作用域



### 枚举作用域



### 模板形参作用域



### 文件作用域（全局作用域）

所谓文件作用域，即一个编译单元内都可见的最外层域，也叫全局作用域。





## 声明和定义

**声明（declaration）**将名字引入（或再引入）到 C++ 程序中。

> 使名字在当前文件可用。一个文件如果想使用别处定义的名字则必须包含对那个名字的声明。

**定义（definition）**是完全定义了声明中所引入的实体的声明，足以使该名字所标识的实体被使用。

> 负责在某个文件中唯一创建于名字关联的对象实体。定义会申请对象的存储空间，无论堆栈。

c++语言支持分离式编译（separate compilation）机制，为此c++将声明和定义区分开来。

**分离式编译**：

允许将程序分割为若干个文件，每个文件可被独立编译。

如果要在多个文件中使用同一个变量，就必须将声明和定义分离。此时，变量的定义必须出现在且只能出现在一个文件中，而其他用到该变量的文件必须对其进行声明，却绝对不能重复定义。





## 名字查找 name lookup

​		名字查找，是当程序中出现一个名字时，将它和引入它的声明联系起来的过程。以便使用其对应定义的实体。

>   例如，为编译 ``std::cout << std::endl;``，编译器进行了：
>
>   -   名字 `std` 的无限定的名字查找，找到了头文件 `<iostream>` 中的命名空间 std 的声明
>
>   -   名字 `cout` 的有限定的名字查找，找到了命名空间 `std` 中的一个变量声明
>
>   -   名字 `endl` 的有限定的名字查找，找到了命名空间 `std` 中的一个函数模板声明
>
>   -   名字 `operator <<` 的**实参依赖查找**，找到了命名空间 `std` 中的多个函数模板声明
>
>   -   名字 `std::ostream::operator<<` 的有限定名字查找，找到了 `std::ostream` 类中的多个成员函数声明
>
>       >   **实参依赖查找：**
>       >
>       >   ​	又称ADL（argument-dependent lookup)或koenig查找，是一组对函数调用表达式中无限定的函数名进行查找的规则。
>       >
>       >   ps：c++语言/基本语法/函数/函数调用/实参依赖查找

对于函数和函数模板的名字，名字查找可以将同一个名字和多个声明联系起来，而且可能从实参依赖查找中得到额外的声明。还会进行**模板实参推导**，并将声明的集合交给**重载决议**，由它选择所要使用的那个声明。

>   **模板实参推导：**
>
>   ​		为实例化一个函数模板，每个模板实参都必为已知的，但并非必须指定每个模板实参。只要可能，编译器都会从函数实参推导缺失的模板实参。
>
>   ps：c++语言/基本语法/模板/函数模板/模板实参推导
>
>   **重载决议：**
>
>   ​		为了编译函数调用，编译器必须首先进行名字查找，对于函数可能涉及实参依赖查找，而对于函数模板可能后随模板实参推导。若这些步骤产生多于一个*候选函数*，则进行*重载决议*，选择将要实际调用的函数。
>
>   ps：c++语言/基本语法/函数/重载/重载决议

对所有其他的名字（变量，命名空间，类等等）的名字查找，程序只有在只产生单个声明的情况下才能编译（不能进行二义性声明）。

对某个作用域中的名字进行查找 会找到该名字的所有声明，

### 名字遮蔽

​		但有一种例外，被称作**“ struct hack ”或“类型/非类型名字隐藏”**：

​		同一作用域中，某个名字的一些出现可以引用非 typedef声明的 `class/struct/union/enum` ，而其他相同的名称要么全都引用同一个变量、非静态数据成员 或者枚举项，要么全都是指可能的重载函数或函数模板名。

​		此情况下无错误，但类型名在查找中被隐藏（代码必须用**详述类型说明符**来访问它）。

>   **详述类型说明符：**
>
>   ​		详述类型说明符可用于指代先前声明的类名（class、struct 或 union）或先前声明的 enum 名，即使该名字被非类型声明所隐藏。它们亦可声明新的类名。
>
>   ps：c++语言/基本语法/声明/说明符/类型说明符/详述类型说明符

​		当同名变量定义在有重合的作用域下时，较大范围的作用域中的变量名被较小范围的作用域中的变量名屏蔽（name hiding / shadowing)。

```c++
#include <iostream>
int value { 5 }; // global variable

void foo()
{
    std::cout << "global variable value: " << value << '\n'; // value is not shadowed here, so this refers to the global value
}

int main()
{
    int value { 7 }; // hides the global variable value until the end of this block
  {
        { // nested block
        // value refers to outer block apples here
        std::cout << value << '\n'; // print value of outer block apples

        int value{ 0 }; // define apples in the scope of the nested block

        value = 10; // this applies to outer block apples

        std::cout << value << '\n'; // print value of outer block apples
    } // outer block apples retains its value even after we leave the nested block

  }
    ++value; // increments local value, not global value
    std::cout << "local variable value: " << value << '\n';
    foo();
    return 0;
} // local value is destroyed
```



### 限定名和非限定名 qualified & unqualified names

A **qualified name** is a name that includes an associated scope.Most often, names are qualified with a namespace using the scope resolution operator (::).

```c++
std::cout // identifier cout is qualified by namespace std
::foo			// identifier foo is qualified by the global namespace
```

> A name can also be qualified by a class name using the scope resolution operator (::), or by a class object using the member selection operators (. or ->).
>
> 成员访问运算符并非严格意义上的名字限定符，但某种意义上也是通过某一限定类型的对象来限定访问。
>
> ```c++
> class C; // some class
> C::s_member; // s_member is qualified by class C
> obj.x; // x is qualified by class object obj
> ptr->y; // y is qualified by pointer to class object ptr
> ```

An unqualified name is a name that does not include a scoping qualifer.





### 有限定名字查找 qualified name lookup

出现在作用域解析操作符`::`右边，或`::`后随消歧义符`template`右边 的名字是有限定标识符，会进行有限定的名字查找。

有限定标识符可能指代：

-   类成员（包括静态和非静态函数、类型和模板等）
-   命名空间的成员（包括其他的命名空间）
-   枚举项

#### 规则

1.  若`::`左侧为空，只会在全局命名空间作用域做出的声明（或通过**using声明**引入到全局命名空间的声明）中查找。

    被局部声明屏蔽的名字可以通过这种限定查找来访问。

    ```c++
    #include <iostream>
    
    int main()
    {
    	struct std{};
        std::cout << "fail\n"; // 错误。标准库命名空间std被局部定义的std结构体屏蔽
        ::std::cout << "ok\n"; // ::std通过在全局命名空间中限定查找，访问到标准库命名空间std
    }
    ```

2.  只有完成对`::`左边名字的查找后，才能对`::`右边的名字进行名字查找。

    1.  如果`::`左边名字是`decltype`表达式或空，不会进行名字查找
    2.  `::`左侧的名字查找可以是有限定也可以是无限定的，取决于当前`::`左边的名字组合中是否还包含一个`::`。且左侧名字只查找命名空间、类类型、枚举和能特化为类型的模板。

    ```c++
    struct A {
        static int n;
    };
    
    int main()
    {
        int A;
        A::n = 42; // 左边的A进行无限定查找，且只查找命名空间、类类型、枚举和能特化为类型的模板，忽略变量A。
        A b;   // 错误，对A的无限定查找，按照就近原则查找到了变量A。
    }
    ```

3.  当限定名为**声明符**时，对该声明符中，在限定名之后出现的名字，在限定符指明的类或命名空间作用于中进行无限定查找。

    ```c++
    class X{};
    
    constexpr int number = 100;
    
    struct C {
        class X {};
        static const int number = 50;
        static X arr[number];
    };
    
    
    X C::arr[number], brr[number]; // C::arr[number]声明错误，X的查找是无限定查找，查找到::X，而不是C::X，造成C::arr二义性声明。
    C::X C::arr[number], brr[number]; // C::arr[number]声明正确，X有限定查找到C::X，进而与C::arr形成匹配的多次声明（声明可以多次，定义只能一次)。
    
    // brr属于另一个声明符，对number的查找都是非限定查找。
    // 这里，空格之前的X 和 C::X都是 说明符（声明说明符）
    // 空格之后的是声明符序列
    ```

    >   **声明符：**
    >
    >   ​		声明符是将名字引入或再引入c++程序的标识符。
    >
    >   ​		初始化声明符列表是一个或多个初始化声明符的逗号分隔列表。
    >
    >   ps：c++语言/基础语法/声明/声明符

4.  `::~`后随标识符（即指定析构函数或伪析构函数），标识符的查找将在`::`左边名字上下文作用域中查找。

    ```c++
    
    ```




#### reduce qualifier

##### using declarations

 A **using declaration** allows us to use an unqualified name (with no scope) as an alias for a qualified name.

```c++
using std::cout;
cout << "Hello World!"; //  // so no std:: prefix is needed here!
```

The `using declaration` (`using std::cout;`) tells the compiler that we’re going to be using the object (`cout`) from the specified namespace( `std namespace`).

> So whenever it sees `cout`, it will assume that we mean `std::cout`. If there’s a naming conflict between `std::cout` and some other use of `cout`, `std::cout` will be preferred. 

##### using directives

a **using directive** imports all of the identifiers from a namespace into the scope of the `using directive`.

> For technical reasons, using directives do not actually import names into the current scope -- instead they import the names into an outer scope (more details about which outer scope is picked can be found [here](https://quuxplusone.github.io/blog/2020/12/21/using-directive/). However, these names are not accessible from the outer scope -- they are *only* accessible via unqualified (non-prefixed) lookup from the scope of the using directive (or a nested scope).
>
> The practical effect is that (outside of some weird edge cases involving multiple using directives inside nested namespaces), using directives behave as if the names had been imported into the current scope. To keep things simple, we will proceed under the simplification that the names are imported into the current scope.

```c++
using namespace std; // this using directive tells the compiler to import all names from namespace std into the current namespace without qualification

cout << "Hello world!";
```

using directives import *all* of the names from a namespace (potentially including lots of names you’ll never use), the possibility for naming collisions to occur increases significantly (especially if you import the `std` namespace).

Even if a `using directive` does not cause naming collisions today, it makes your code more vulnerable to future collisions. 

##### how to use

If a `using declaration` or `using directive` is used within a block, the names are applicable to just that block (it follows normal block scoping rules).

If a `using declaration` or `using directive` is used in the global namespace, the names are applicable to the entire rest of the file (they have file scope).

Once a `using statement` has been declared, there’s no way to cancel or replace it with a different `using statement` within the scope in which it was declared.Unless into a block scoping:

```c++
int main()
{
    {
        using namespace foo;
        // calls to foo:: stuff here
    } // using namespace foo expires

    {
        using namespace Goo;
        // calls to Goo:: stuff here
    } // using namespace Goo expires

    return 0;
}
```

**Avoid `using directives` (particularly `using namespace std;`), except in specific circumstances. `Using declarations` are generally considered safe to use inside blocks.**

Limit their use in the global namespace of a code file, and never use them in the global namespace of a header file.



### 无限定名字查找





### lookup  & using statement

In 1995, namespaces were standardized, and all of the functionality from the standard library was moved out of the global namespace and into namespace `std`.Updating every name that was now moved into the `std` namespace to use the `std::` prefix was a massive risk. A solution was requested.

typing `std::` before everything you use from the standard library can become repetitive, and in some cases, can make your code harder to read.C++ provides some solutions to both of these problems, in the form of `using statements`.

`Using directives` are the solution that was provided for old pre-namespace codebases that used unqualified names for standard library functionality. Rather than having to manually update every unqualified name to a qualified name (which was risky), a single `using directive` (of `using namespace std;`) could be placed at the top of the each file, and all of the names that had been moved to the `std` namespace could still be used unqualified.



### 名字重整

​		When the compiler compiles a function, it performs **name mangling**, which means the compiled name of the function is altered (“mangled”) based on various criteria, such as the number and type of parameters, so that the linker has unique names to work with.

> for example:
>
> `int fcn()` might compile to name `__fcn_v`
>
> `int fcn(int)` might compile to  name `__fcn_i`
>
> So while in the source code, two overloaded functions share a name, in compiled code, the names are actually unique.

​		There is no standardization on how names should be mangled, so different compilers will produce different mangled names.



## 匿名对象

​		A **temporary object** (also sometimes called an **anonymous object**) is an object that is created for temporary use (and then destroyed) within a single expression. Temporary objects have no scope at all (this makes sense, since scope is a property of an identifier, and temporary objects have no identifier). This means a temporary object can only be used directly at the point where it is created, since there is no way to refer to it beyond that point.
