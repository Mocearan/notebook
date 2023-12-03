## object 对象

​		存储数据的空间以及类型解译对该空间的制约性说明，称为对象。

---

​	

​		In some older programming languages (like Apple Basic), you could directly access memory. In C++, direct memory access is not allowed. Instead, we access memory indirectly through an object.

​		An **object** is a region of storage (usually memory) that has a value and other associated properties.

​		This means we can focus on using objects to store and retrieve values, and not have to worry about where in memory they’re actually being placed.



​	c++程序可以创建、销毁、引用、访问并操作一个对象。

> ​		理解对象要有简单的内存和CPU 知识。
>
> ​		数据以字节为基本单位存储在内存上，CPU的控制单元（指令寄存器，指令译码器，操作控制器等）指挥ALU（算术逻辑单元， 分为算术运算和逻辑运算<移位、逻辑测试或两个值比较>）通过地址总线寻址到内存上的某个空间，按照数据总线宽度，一次或多次读取数据，加载到CPU的存储单元（CPU片缓存和寄存器组），然后再根据控制单元的指令对这些数据进行运算或操作。
>
> ![image-20230312121025105](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230312121025105.png)
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
> ​		对于操作属性，拷贝和移动是通用且重要的属性。
>
> c++语义上的移动语义，是持有值属性，将空间属性与地址属性与新的名字建立映射的操作。
>
> c++语义上的拷贝语义，即是通过名字与空间属性的映射，对值属性进行操作。



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

​		对象在创建时获得了一个特定的值，我们说这个对象被初始化（initialized）了。

​		在C++语言中，初始化是一个异常复杂的问题：`base/initalization`

​		c++语言中，赋值和初始化是两个完全不同的操作：

- 初始化的含义是创建变量时赋予其一个初始值。
- 赋值的含义是把对象当前值以一个新的值来覆盖。

> 在很多编程语言中二者的区别几乎可以忽略不计，即使在C++语言中有时这种区别也无关紧要，所以人们特别容易把二者混为一谈。

#### 初始值

​		用于初始化变量的值可以是任意复杂的表达式。

​		在同一条语句中可以使用已经初始化的对象去初始化后定义的变量。

#### 初始化的形式



##### 列表初始化

- 列表初始化的四种形式

```c
int units_sold1 = 0;
int units_sold2 = { 0 };
int units_sold3{ 0 };
int units_sold4(0);
```

​		作为c++11新标准的一部分，用花括号来初始化变量的到了全面应用，而在此之前，这种初始化的形式仅在某些受限的场合下才能使用。

​		无论是初始化对象还是某些时候为对象赋新值都可以使用这样一组由花括号括起来的初始值了。

​		如果使用列表初始化且初始值存在丢失信息的风险，则编译器报错。

```c
long double ld = 3.1415926536;
//int a{ ld }, b = { ld };//转换未执行，存在丢失信息的风险  从“long double”转换到“int”需要收缩转换
int c(ld), d = ld;	//转化执行，确实丢失了部分值
```

##### 默认初始化

​		如果对象定义时没有指定初始值，则对象被默认初始化（default initialized），此时对象被赋予默认值。

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
> **advice：**
> 	建议初始化每一个内置类型的对象。虽然并非必须这么做，但如果我们不能确保默认初始化后程序安全，那么这么做不失为一种简单可靠的方法。



#### 声明点

[The New C++ -- 变量 （6. 变量名的声明点和作用域）_heartszhang的专栏-CSDN博客](https://blog.csdn.net/heartszhang/article/details/7248237?locationNum=6)

[声明点 - 游戏蛮牛 - C++中文翻译用户手册 (manew.com)](http://docs.manew.com/cpp/43.html)

[C中的声明点 - 程序园 (voidcn.com)](http://www.voidcn.com/article/p-dhelexhw-bsr.html)

