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

​	c++程序可以创建、销毁、引用、访问并操作一个对象。

> 理解对象要有简单的内存和CPU 知识。
>
> ​		数据以字节为基本单位存储在内存上，CPU的控制单元（指令寄存器，指令译码器，操作控制器等）指挥ALU（算术逻辑单元， 分为算术运算和逻辑运算<移位、逻辑测试或两个值比较>）通过地址总线寻址到内存上的某个空间，按照数据总线宽度，一次或多次读取数据，加载到CPU的存储单元（CPU片缓存和寄存器组），然后再根据控制单元的指令对这些数据进行运算或操作。
>
> ![image-20210621232539533](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210621232539533.png)
>
> 对象是将CPU和内存分离模型在逻辑抽象上进行合并，将数据存储和对数据的复制、移动、计算打包成一个整体进行表现。
>
> > [处理器结构_百度百科 (baidu.com)](https://baike.baidu.com/item/处理器结构/14724193?fr=aladdin)
> >
> > [地址总线和数据总线的关系、作用和宽度 - 接口/总线/驱动 - 电子发烧友网 (elecfans.com)](http://www.elecfans.com/emb/jiekou/20171113578362.html)



### 对象的要素

​		对对象进行抽象， 能够的到组成对象的基本属性：空间属性，地址（寻址）属性，值属性，操作属性。

​		无论是标量类型还是复合类型、类类型，任何对象都拥有这四个基本属性。

- **空间属性：**即代码段指令明确说明的，对象在内存上占用的空间大小，与类型相关即容纳多少数量数据的属性，
- **地址（寻址）属性：**内存上每个基本单位字节，都按照一定的规则进行了编码，对内存的使用，首先要通过该编码找到该空间，所以称为地址。
- **值属性：**在某个或某连续的内存空间上存储的数据，称为该对象的值。
- **操作属性：**服从于代码段指令对该对象制约下的可以进行的运算，称为对象的操作属性。具体的操作约束依赖于类型。

> 不管是对于匿名对象还是声明了名字后的对象或引用，这四个属性都存在。
>
> 基于程序 = 数据结构 + 算法的基本认知，空间属性、地址属性、值属性是对象的数据结构存储属性，操作属性是对象的算法操作属性。
>
> 其中，左值属性能够将地址属性和空间属性与名字建立映射。右值属性是值属性，
>
> c++语义上的移动语义，是持有值属性，将空间属性与地址属性与新的名字建立映射的操作。
>
> c++语义上的拷贝赋值语义，即是通过名字与空间属性的映射，对值属性进行操作。



### c++对象的性质

-   大小（内存占用），可以使用`sizeof`获取

-   对齐要求，可以使用`alignof`获取

-   存储期

-   生存期

    > object’s **lifetime** is defined to be the time between its creation and destruction.
    >
    > lifetime is a runtime property.

-   类型

    > c++每个对象都有其类型，类型决定变量的空间大小、布局方式、空间能存储的值的范围，以及变量能参与的运算。

-   值

-   名字（可选）

    > 具名的对象即变量。
    >
    > > ​		Objects can be named or unnamed (anonymous). A named object is called a **variable**, and the name of the object is called an **identifier**. In our programs, most of the objects we create and use will be variables.



### 对象创建

​		对象能由**定义**、**new表达式**、throw表达式、更改联合体的活跃成员和求值要求临时对象的表达式显式创建。

> When the program is run (called **runtime**), the object will be instantiated. **Instantiation** is a fancy word that means the object will be created and assigned a memory address. 

​		**显式对象创建**中创建的对象是**唯一定义**的。

> **唯一定义：**
>
> ​	任何变量、函数、类类型、枚举类型、概念或模板，在每个翻译单元中都只允许有一个定义。
>
> ps: c++语言/基础语法/基本概念/定义与ODR

​		**隐式生存期类型**的对象可以由下列操作**隐式创建**：

- 开始 char、unsigned char 或 std::byte (C++17 起)数组生存期的操作

  该情况下在该数组中创建这种对象

- 调用下列分配函数

  - operator new
  - operator new[]
  - std::malloc
  - std::calloc
  - std::realloc
  - std::aligned_alloc(C++17 起)

  该情况下在分配的存储中创建这种对象

- 调用下列对象表示复制函数

  - std::memcpy
  - std::memmove
  - std::bit_cast(C++20 起)

  该情况下在目标存储区域或结果中创建这种对象

```c++
#include <cstdlib>
#include <cstddef>

class Test
{
public:
   Test() = default;
   ~Test() = default;
   Test(const Test&) = default;

};

int main(void)
{
// 显式创建
    int a = int();
    Test t = Test();
// 隐式创建
    int aa;
    Test tt;

    int* aaa = new int;
    Test* ttt = static_cast<Test*>(std::malloc(sizeof(Test)));

    std::byte buf[8];
}
```

**隐式创建，即通过在某个既定存储上再建构某个类型的解译方式，来将对象绑定在存储上。可能存在存储上原类型（可能是原始字节）和再建构的类型发生冲突。**

> ​		只要在能给予程序有定义的行为时，同一存储区域中可创建零或多个对象。
>
> ​		如果无法这样创建，例如操作冲突，则程序行为未定义。
>
> ​		若多个这种隐式创建的对象的集合会给予程序有定义行为，则不指定这些集合中的哪一个被创建。换言之，不要求隐式创建的对象是唯一定义的。
>
> > ​	以字节形式创建的数组，作为申请的存储使用，可能在运行时转换为其他类型的对象。这样的程序在编译期间是非良构的，不要求编译器进行类型诊断。
> >
> > ​	详见  c++语言/基础语法/基本概念/定义与ODR

**显式创建，即通过显式初始化的方式，直接建构类型解译， 无存储本身的类型干扰，即唯一定义。**

> 指定的存储区域内隐式创建对象后，一些操作会生成指向 适合的已创建对象 的指针。
>
> 适合的已创建对象 与 存储区域拥有相同地址。
>
> 类似地，当且仅当不存在能给予程序有定义行为的指针值时，行为才未定义；而若有多个给予程序有定义行为的值，则不指定产生哪个值。
>
> > 当类对象隐式创建

**调用 std::allocator::allocate类型隐式定义的 复制/移动 特殊成员函数 亦能创建对象。**

> 也是通过复制，进行再建构。



### 对象的初始化

`关于初始化：C++/基本语法/初始化`

#### 初始值

当对象在创建时获得了一个特定的值，称对象被**初始化（initialized）**了。在同一条语句中可以使用已经初始化的对象去初始化后定义的变量。

c++语言中，赋值和初始化是两个完全不同的操作。

> 初始化的含义是创建变量时赋予其一个初始值。
>
> 赋值的含义是把对象当前值擦掉，而以一个新的值来替代。

#### 列表初始化

- 列表初始化的四种形式

```c
int units_sold1 = 0;
int units_sold2 = { 0 };
int units_sold3{ 0 };
int units_sold4(0);
```

> **列表初始化**：
>
> ​		作为c++11新标准的一部分，用花括号来初始化变量的到了全面应用，而在此之前，这种初始化的形式仅在某些受限的场合下才能使用。

无论是初始化对象还是某些时候为对象赋新值都可以使用这样一组由花括号括起来的初始值了。

如果使用列表初始化且初始值存在丢失信息的风险，则编译器报错。

```c
long double ld = 3.1415926536;
//int a{ ld }, b = { ld };//转换未执行，存在丢失信息的风险  从“long double”转换到“int”需要收缩转换
int c(ld), d = ld;	//转化执行，确实丢失了部分值
```

#### 默认初始化

如果对象定义时没有指定初始值，则对象被默认初始化（default initialized），此时对象被赋予默认值。

- ```内置类型```

  函数体之外（全局作用域）的变量被初始化为0.

  函数体之内（局部作用域）的对象不被初始化，值是未定义的，拷贝或其他形式的访问将引发错误。

- ```类```

  类的初始化方式由类的各个构造函数来定义。是否允许不经初始化就定义对象也由类自己决定（默认构造函数，类提供一个合适的默认值）。

> **未初始化对象引发运行时故障**
> 		未初始化对象含有一个不确定的值，使用未初始化变量的值时一种错误的编程行为并且很难调试。编译器都能对一部分使用未初始化对象的行为提出警告，但编译器并未被要求检查此类错误。
> 		使用未初始化的对象将带来无法预计的后果。
> 		如果幸运程序会崩溃，此时只要找到崩溃的位置就很容易发现对象没被初始化的问题。另外一些时候，程序会错误执行。
> 		更糟糕的情况是，程序结果过时对时错、无法把握。而且，往无关的位置添加代码还会导致我们误认为程序对了，其实结果仍旧有错。
>
> 	advice：
> 		建议初始化每一个内置类型的对象。虽然并非必须这么做，但如果我们不能确保默认初始化后程序安全，那么这么做不失为一种简单可靠的方法。



#### 声明点

[The New C++ -- 变量 （6. 变量名的声明点和作用域）_heartszhang的专栏-CSDN博客](https://blog.csdn.net/heartszhang/article/details/7248237?locationNum=6)

[声明点 - 游戏蛮牛 - C++中文翻译用户手册 (manew.com)](http://docs.manew.com/cpp/43.html)

[C中的声明点 - 程序园 (voidcn.com)](http://www.voidcn.com/article/p-dhelexhw-bsr.html)



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