# entities 实体

​		实体即脱离设计模型、理论建构之外的，实际用以组织程序代码的基本构件。

---

​		C++ 程序中的*实体*包括值、[对象](https://zh.cppreference.com/w/cpp/language/object)、[引用](https://zh.cppreference.com/w/cpp/language/reference)、 [结构化绑定](https://zh.cppreference.com/w/cpp/language/structured_binding) (C++17 起)、[函数](https://zh.cppreference.com/w/cpp/language/functions)、[枚举项](https://zh.cppreference.com/w/cpp/language/enum)、[类型](https://zh.cppreference.com/w/cpp/language/type)、类成员、[模板](https://zh.cppreference.com/w/cpp/language/templates)、[模板特化](https://zh.cppreference.com/w/cpp/language/template_specialization)、[命名空间](https://zh.cppreference.com/w/cpp/language/namespace)和[形参包](https://zh.cppreference.com/w/cpp/language/parameter_pack)。

预处理器**宏**不是 C++ 实体。

> ### 不是对象的实体
>
> - 值
> - 引用
> - 函数
> - 枚举项
> - 类型
> - 类的非静态成员
> - 模板
> - 类或函数 模板的特化
> - 命名空间
> - 形参包
> - this

- 类型：定义一组可能的值以及一组对象上的操作
- 对象：存放某类型值的内存空间
- 值：一组二进制位，具体的含义由其类型决定
- 变量：命名的对象

## data 数据

​		**data** is any information that can be moved, processed, or stored by a computer.

​		A single piece of data, stored in memory somewhere, is called a **value**.



## object 对象

​	存储数据的空间以及类型解译对该空间的制约性说明，称为对象。

> In some older programming languages (like Apple Basic), you could directly access memory. In C++, direct memory access is not allowed. Instead, we access memory indirectly through an object.
>
> An **object** is a region of storage (usually memory) that has a value and other associated properties.
>
> This means we can focus on using objects to store and retrieve values, and not have to worry about where in memory they’re actually being placed.



## 变量

> variables are names for a piece of memory that can be used to store information. 

​		对c++来说，变量和对象基本上可以互换使用，只不过具名的对象称为变量。

​		变量提供一个具名的、可供程序操作的内存空间。

> 我们需要使用一个名字来访问一个对象，一个命名后的对象被称为一个变量，它有特定的类型，类型决定我们可以对其做什么样的操作。		
>
> 声明是将名字引入或再引入到c++程序中的过程，即将一个对象的名字和一个对象的地址属性绑定。
>
> **非静态数据成员：**
>
> 非静态数据成员是在类的成员说明中声明的数据成员。
>
> 是组成类内存模型的一部分。
>
> 类的内存结构狭义上只与类的非静态数据成员有关，但实际上，与数据成员之间的结构化组织（内存对齐），类与类之间关系的组织（虚基类表与虚基类指针，虚函数表与虚函数指针）有关。但与静态成员无关，静态成员作为实例化对象的抽象层次，向所有实例化对象递达数据的一种半抽象手段，其内存结构处于静态存储区。
>
> ps：c++语言/类/成员/数据成员/非静态数据成员
>
> **引用：**
>
> 通过某名字寻址到某对象后，建立新名字与该对象的映射关系即为引用。
>
> ps：c++语言/基本概念/实体/引用



### 变量名

从本质上说，变量名是对一段某一类型的内存空间的**具名声明**。也就是内存模型和操作模型映射到一个别名。

> 所谓编程，就是在不同层次上，将某一组行为抽象起来包装在一个别名里。所以编程其实就是一种整理工作。
>
> 汇编语言对机器语言的封装，高级语言对汇编语言的包装，类对数据和行为的包装，更高层次逻辑对低级逻辑的包装，都应如是。
>
> **声明：**
>
> ​		声明将名字引入c++程序中。也即使建立名字与对象左值属性的映射。
>
> ​		ps：c++语言/基础语法/声明

变量名在形式上是一个**标识符**

> **标识符：**
>
> 标识符是一个字符序列，由数字字符、下划线、大小写拉丁字母和大多数Unicode字符进行任意长度的组合。长度不限，大小写敏感。
>
> 由字母、数字、下划线组成，必须以字母或下划线开头。
>
> ​		ps：c++语言/基础语法/基本概念/名字与标识符



### 定义形式

`类型说明符（type specifier）+ 变量名列表（逗号分隔，分号结束，可初始化）`

**如果想声明一个变量而非定义它，就在变量名前加``extern``关键字，且不要显式初始化变量。**

> 任何包含了显式初始化的声明即成为定义。
> 		我们能给由extern关键标记的变量赋新值，但是这么做也就抵消了``extern``的作用。``extern``语句如果包含初始化就不再是声明，而变成定义了；
>
> 在函数体内部初始化一个由``extern``关键字表示的变量，将引发错误。（局部作用域只能向外看到本文件的全局作用域）。
>
> 变量能且只能被定义一次，但是可以被多次声明
>



## 值

​		在c++语义分析中，值即对于某块内存地址上持有的数据属性。

​		值可能是不确定的，例如默认初始化的非结构化类型（非类类型-标量类型）。

### 变量值和常量值

​		值按照可变性（variability），可以分为变量值和常量值。

​		变量值是指，变量在定义后，其对象模型空间属性上在不同阶段（初始化时，初始化后）所容纳的数据。



## 引用

​		引用的概念见本节 类型/复杂类型/引用

​		引用的相关细节，见 c++语言/基础语言/声明/声明符/引用声明

## 结构化绑定



## 函数



## 枚举项







## 类成员