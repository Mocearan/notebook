# 类型说明符

​		声明说明符，是以空白分隔的说明符的序列

---



## 声明说明符序列

​		以各种说明符，加以空白符分隔，组成的序列。是声明语句中对类型部分的说明。

​		声明符包括：

- `typdef`说明符
- 函数说明符
  - `inline`
  - `virtual`
  - `explicit`
- `inline`说明符
- `friend`说明符
- `constexpr`说明符
- `consteval`说明符
- `constinit`说明符
- 存储类说明符
  - `register`
  - `static`
  - `thread_local`
  - `extern`
  - `mutable`
- 类型说明符
  - 类说明符
  - 枚举说明符
  - 简单类型说明符
    - `char` / `char8_t` / `char16_t` / `char32_t` / `wchar_t` / `bool` / `short` / `int` / `long` / `signed` / `unsigned` / `float` / `double` / `void`
    - `auto`
    - `decltype`
    - 先前声明过的类名
    - 先前声明过的枚举名
    - 先前声明㢧的`typedef`名或类型别名
    - 带模板实参的模板名
    - 无模板实参的模板名
  - 详述类型说明符
    - `class / struct / union`，后随已定义的`class / struct / union`标识符
    - `class / struct / union`， 后随已定义带模板实参的类模板名
    - `enum` 后随已定义为枚举名的标识符
  - `typename`说明符
  - `cv`限定符



## typedef说明符

## 函数说明符

### inline

#### inline 变量说明符

### virtual 函数说明符

### explicit

## friend



## 存储类型说明符

### auto（已废除）

### register

### thread_local

### extern

### mutable



## 类型说明符

### 简单类型说明符

#### 基础类型 

>   char / char8_t / char16_t / char32_t / wchar_t / bool / short / int / long / signed / unsgined / float / double / void
>
>   ps：基本概念/实体/类型/基础类型/

#### auto

​		占位类型说明符。

- 对于变量，指定要从其初始化器自动推导出其类型。
- 对于函数，指定要从其return语句推导出其返回类型。
- 对于非类型模板形参，指定要从实参推导出其类型。



#### decltype

​		如果表达式的求值结果是左值，decltype作用于该表达式（不是变量）得到一个引用类型。

​		举个例子，假定p的类型是int＊

- 因为解引用运算符生成左值，所以`decltype(*p)`的结果是`int&`。
- 因为取地址运算符生成右值，所以`decltype(&p)`的结果是`int＊＊`，也就是说，结果是一个指向整型指针的指针。

#### 先前声明过的类名

#### 先前声明过的枚举名

### 带模板实参的模板名

#### 无模板实参的模板名



### 类说明符

#### class

#### struct

#### union

### 枚举说明符

#### 无作用域枚举

#### 有作用域枚举

#### using enum 声明

### 详述类型说明符

### typename说明符



### cv限定符（const/volatile）

>   CV限定实际上是从类型的可写角度，来限定对象从外部可写的权限。
>
>   而对象内部限定的可写性，是对象自身性质的一部分，依赖类的访问控制机制（常函数接口，常成员等，公有私有等）
>
>   >   ​		在本节，所谓访问权限为：可读或可写。即从外部观察变量时的权限，因已经向外部暴露，则认为都可读。

-   指明 被声明对象 或 被命名类型 的常变性（常量性 或 易变性）。

-   可以在任意类型说明符或声明说明符序列中

-   除函数类型 / 引用类型 外都可用

    >   包括不完整类型
    >
    >   数组类型的cv限定，同元素的cv限定

-   由对象被创建时所使用的cv限定来决定常变性

    -   声明语句中，声明说明符序列，或声明符的一部分
    -   new 表达式中，类型表示的一部分

#### const

​		限定对象外部访问权限为不可写，除非绕过对象的直接访问。

-   必须进行初始化
-   不能进行赋值

>   通过非const类型的引用或指针，间接访问并修改const对象是可行的，但结果UB。

​		根据子对象组成对象的性质，const对象的子对象（数据成员）也不能被外部修改。

>   ​		在未声明为 `extern` 的非局部非 volatile 非模板(C++14 起)非 inline(C++17 起)变量声明上使用 `const` 限定符，会给予该变量内部连接。
>
>   ​		这有别于 C，const 文件作用域对象拥有外部连接。

##### mutable

​		允许子对象被修改，即便子对象所属的对象声明为const。

​		可出现在非引用非 const 非静态数据成员声明中

```c++
class A
{

public:
    int a;
    mutable int b;
    //mutable int *const c;// ERR 顶层const，常指针不能修改指向
    mutable const int *d; // OK，底层const，可修改指向
};

int main( void )
{
    const A a {};
    // a.a = 1;
    a.b = 1;
}
```

​		mutable 用于指定不影响类的外部可观察状态的成员：通常用于互斥体、记忆缓存、惰性求值和访问指令等。

```c++
#include <mutex>
class ThreadsafeCounter
{
    mutable std::mutex m; // “M&M 规则”：mutable 与 mutex 一起出现
    int data = 0;
public:
    int get() const
    {
        std::lock_guard<std::mutex> lk( m );
        return data;
    }
    void inc()
    {
        std::lock_guard<std::mutex> lk( m );
        ++data;
    }
};

```

>   `mutable` 被当做存储类说明符而非类型限定符，但它不影响存储类或连接



##### 顶层const / 底层const

​		对于指针而言，const限定对const本身的访问权限 还是对指针间接访问对象的权限，由const与`*`的相对位置来决定。

​		`const *`，称为底层const，限定通过指针进行间接访问的权限。

​		`* const`，称为顶层const，限定对指针变量本身进行访问的权限。

​		`const * const`，同时限定两者。

>   ps：声明/声明符/指针声明符/常量性



##### 常量的定义方式

-   通过#define声明常量宏

    预处理器处理，不申请内存，不进行类型检查

-   通过const声明常变量

-   通过enum引入枚举常量

>   c++建议通过后两种方式来声明常量。
>
>   这种建议限于面向应用的编程，对框架以下的底层编程来说，#define能灵活处理大量的泛化问题。
>
>   >   不要陷入某种教条之中，要寻找问题的本质，和最佳解法。
>
>   如：
>
>   ```c++
>   #define STR(a) #a
>   #define CAT(a, b) a ##b
>   ```
>
>   



#### volatile

​		`volatile`声明易变性，是声明给硬件/编译器的。

​		通常，编译器会在单个线程进行指令重排，加载某个值到寄存器绕过内存来加速程序，但如果对象跨线程使用，这种优化可能导致异常的错误。

​		所以，``volatile``声明的对象在多线程场景下（具有被多个线程读写的性质），向编译器声明该对象，相对于某个想要持有它的线程来说，是易变，禁止在这个对象上进行指令重排，或将其加载到寄存器。

> 对于`volatile`声明的变量，cpu不将其加载到寄存器，即每次访问时都从其内存上读取数据，而不是cpu缓存数据。

​		c++中的`volatile`不同于其他语言中具有同步语义，仅用于声明变量在多线程环境中的易变性。

​		这种易变性声明可声明为：volatile对象，volatile的子对象，const volatile对象的mutable子对象。

>   **可能的优化：**
>
>   ​	ps：基本概念/as-if规则
>
>   **求值顺序**
>
>   ​	ps：表达式/求值顺序
>
>   **memory order**
>
>   ​	ps：标准库/原子操作库/内存顺序/memory_order

`volatile `的一些用法被弃用：

-   volatile 类型的左值作为内建自增自减运算符的操作数；
-   volatile 类型的左值作为内建赋值、复合赋值运算符的左操作数
-   除非该直接赋值表达式出现于不求值语境或是弃值表达式
-   volatile 对象类型作为函数参数类型或返回类型；volatile 限定符在结构化绑定声明中。

#### const-volatile

​		const-volatile / const volatile 对象的非mutable子对象 / volatile的const子对象 / const对象的非mutable的volatile子对象。

​		兼具常变性。

#### cv限定引用和指针的限定性转换

​		`无限定 < const / volatile < const-volatile`

依此次序，左边的引用/指针能够隐式转换到右边。

右边的引用/指针需要使用`const_cast`来降级使用。

>   多级指针的限定性转换：
>
>    ps：基本概念/表达式/类型转换/隐式转换/限定性转换



## 常量性说明符

### constexpr说明符

> 指定变量或函数的值可在常量表达式中出现。

​	constexpr说明符 声明在**编译时**求 函数 或变量的值。然后这些变量或函数（带上了合适的实参）就可以用在只允许编译时常量表达式中。

​	用于对象声明的constexpr说明符蕴含const含义，即声明常量。

> c++14前，对非静态成员函数，也可使用constexpr来声明编译时的常量性。

​	用于函数声明的constexpr说明符蕴含inline含义，即声明内联。

> 从c++17开始，对static成员变量，也可以使用constexpr来声明内联。

​	函数或函数模板的任何声明使用了constexpr说明符，则每个声明必须都含有该说明符。

#### constexpr常量

constexpr变量在编译期一经初始化就不再改变，即成为符号常量。

```c++
constexpr double pi = 3.1415926;
constexpr int max = 17;
```

constexpr必须给定一个编译时已知的值，若某个变量初始化时的值在编译时位置，但初始化后也绝不改变，为解决这种情况，c++提供了第二种形式的常量：const。

```c++
constexpr int max = 100;
void use(int n)
{
	constexpr int c1 = max + 7;
   	const int c2 = n + 7; // ok, 但是c2的值依然不能改变。
    //c2 = 7; //err
}

// const变量常见的原因是，c++98不支持constexpr，所以以const替代。另外，不是常量表达式（编译时未知）但初始化后不允许改变的“变量”本身就非常有用。
```



constexpr变量必须满足：

- 类型必须是**字面类型（LiteralType）**

    > **字面类型：**
    >
    > ​	指明一个类型为字面类型。是核心语言所定义的一种类型类别。
    >
    > ps：c++语言/标准库/具名要求/其他/字面类型

- 必须在声明时立即初始化

- 初始化的**全表达式**，必须是常量表达式，包括所有隐式转换、构造函数调用等。

    > **全表达式：**
    >
    > ![image-20210524232855210](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210524232855210.png)
    >
    > ps：c++原因/基础语法/表达式/求值顺序/顺序规则/全表达式

- （c++20起）必须拥有常量析构，即

    - 既不是类类型，也不是数组（包括多维）
    - 或者，是类类型或者数组（包括多维），该类类型拥有constexpr析构函数。对于作用仅为销毁该对象的虚设表达式 e ，假如认为该对象与其非 mutable 子对象（但并非其 mutable 子对象）的生存期始于 e 内，则 e 会是核心常量表达式。

如果constexpr变量并非在翻译单元局部内定义，而是从外部引用，那么不应该用翻译单元局部内

​	指代或指代可用于常量表达式的实体

   或指向或指代这种实体的子对象（可能递归的）	

来初始化该constexpr变量。这种初始化在**模块接口单元**或模块划分中被禁止，而在任何其他语境中被弃用。

> **模块接口单元：**
>
> ​	模块有助于将大量代码分拆为各个逻辑部分。
>
> ps：c++语言/基础语法/基本概念/模块



#### constexpr函数

constexpr函数必须满足：

- 必须非虚函数（c++20前）

    > **虚函数**
    >
    > 虚函数是可在派生类中覆盖其行为的成员函数，支持动态调用派发。
    >
    > ps：c++语言/基础语法/声明/函数声明/virtual函数说明符

- 必须不是协程（c++20起）

    > **协程：**
    >
    > 协程是能暂停执行以在之后恢复的函数。
    >
    > ps：c++语言/标准库/ 标准库及头文件/通用工具库/语言支持/协程支持

- 如果存在返回类型，必须是字面类型

- 每个参数都必须是字面类型

- 对于构造函数，该类必须无虚基类

    > c++20起，对于析构函数也要求该类必须无虚基类

- 至少存在一组实参值，使得函数的一个调用，是核心常量表达式的被被求值的子表达式。

    > c++14开始，对于构造函数，该实参值则是足以用于常量初始化器。

    这一条不要求诊断。

- （c++20前）函数体必须不是函数try块

    > **函数try块：**
    >
    > ​	建立围绕整个函数体的异常处理块
    >
    > ps：c++语言/基础语法/异常/函数try块

- （c++14前）函数体必须被弃置或预置，或只含有下列内容：

    - 空语句
    - static_assert声明
    - 不定义类或枚举的typedef声明及别名声明
    - using声明
    - using指令
    - 恰好一条return语句（不是构造函数）

- (c++14起)函数体必须不含：

    - goto语句
    - 除case和default之外的标号的语句
    - （c++20前）try块
    - （c++20前）asm声明
    - （c++20前）不进行初始化的变量定义
    - 非字面类型的变量定义
    - 静态或县城存储期变量的定义



**=default;或=delete;的函数体不含任何上述内容**。

> 函数体非 =delete; 的 **constexpr 构造函数**必须满足下列额外要求：
>
> - （c++20前）对于**class 或 struct**的构造函数，每个子对象和每个**union**非 static 数据成员必须被初始化。若类是联合体式的类，对于其每个非空匿名联合体成员，必须恰好有一个变体成员被初始化
> - （c++20前）对于非空 union的构造函数，恰好有一个非静态数据成员被初始化
> - 每个被选用于初始化非静态成员和基类的构造函数必须是 constexpr 构造函数。
>
> 
>
> （c++20前）析构函数不能为 constexpr，但能在常量表达式中隐式调用平凡析构函数。
>
> 函数体非 =delete; 的 **constexpr 析构函数**必须满足下列额外要求：
>
> - 每个用于销毁非静态数据成员与基类的析构函数必须为 constexpr 析构函数。

**对于 constexpr 函数模板和类模板的 constexpr 函数成员，必须至少有一个特化满足上述要求。其他特化仍被认为是 constexpr，尽管常量表达式中不能出现这种函数的调用。**

```c++
#include <iostream>
#include <stdexcept>
 
// C++11 constexpr 函数使用递归而非迭代
// （C++14 constexpr 函数可使用局部变量和循环）
constexpr int factorial(int n)
{
    return n <= 1? 1 : (n * factorial(n - 1));
}
 
// 字面类
class conststr {
    const char* p;
    std::size_t sz;
public:
    template<std::size_t N>
    constexpr conststr(const char(&a)[N]): p(a), sz(N - 1) {}
 
    // constexpr 函数通过抛异常来提示错误
    // C++11 中，它们必须用条件运算符 ?: 这么做
    constexpr char operator[](std::size_t n) const
    {
        return n < sz ? p[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() const { return sz; }
};
 
// C++11 constexpr 函数必须把一切放在单条 return 语句中
// （C++14 无此要求）
constexpr std::size_t countlower(conststr s, std::size_t n = 0,
                                             std::size_t c = 0)
{
    return n == s.size() ? c :
           'a' <= s[n] && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
                                        countlower(s, n + 1, c);
}
 
// 输出要求编译时常量的函数，用于测试
template<int n>
struct constN
{
    constN() { std::cout << n << '\n'; }
};
 
int main()
{
    std::cout << "4! = " ;
    constN<factorial(4)> out1; // 在编译时计算
 
    volatile int k = 8; // 不允许使用 volatile 者优化
    std::cout << k << "! = " << factorial(k) << '\n'; // 运行时计算
 
    std::cout << "the number of lowercase letters in \"Hello, world!\" is ";
    constN<countlower("Hello, world!")> out2; // 隐式转换到 conststr
}
```





#### 注解

​	(c++17前)因为 noexcept运算符始终对常量表达式返回 `true`，故它可用于检查具体特定的 constexpr 函数调用是否采用常量表达式分支：

```c++
constexpr int f(); 
constexpr bool b1 = noexcept(f()); // false，constexpr 函数未定义
constexpr int f() { return 0; }
constexpr bool b2 = noexcept(f()); // true，f() 是常量表达式
```

​	constexpr 构造函数允许用于非字面类型的类。例如，`std::unique_ptr` 的默认构造函数是 constexpr，允许**常量初始化**。

引用变量可声明为 constexpr（其初始化器必须是**引用常量表达式**）：

> **常量初始化：**
>
> ​	设置静态变量的初值为编译时常量。
>
> ps：c++语言/基础语法/初始化/静态初始化/常量初始化
>
> **引用常量表达式：**
>
> 是指代具有静态存储期的对象或指代函数的左值核心常量表达式。c++14开始不再于标准中使用。
>
> ps：c++语言/基础语法/表达式/常量表达式/历史类别

```c++
static constexpr int const& x = 42; // 到 const int 对象的 constexpr 引用
                                    // （该对象拥有静态存储期，因为 static 引用延续生命周期）
```

​	尽管在 constexpr 函数中允许 try 块与内联汇编，常量表达式中仍然不允许抛异常或执行汇编。

​	若变量拥有常量析构，则无需为调用其析构函数而生成机器码，即使其析构函数非平凡。

### consteval说明符

### constinit说明符



## 属性

​		为类型、对象、代码等引入由实现定义的属性。

```c++
[[ 属性列表 ]]		// C++11 起
[[ using 属性命名空间 : 属性列表 ]]	// C++17 起

//  属性列表 是由逗号分隔的零或更多个 属性 的序列（可以以指示包展开的省略号 ... 结束）
//  using: namespace 在属性列表开头， using 所指定的命名空间会作用于所有属性
```

> ​		让程序员来手动指示编译器这里该如何做、那里该如何做，就有可能会生成更高效的代码
>
> ​		 C++11 之前，标准里没有规定这样的东西，但 GCC、VC 等编译器发现这样做确实很有用，于是就实现出了自己“编译指令”，在 GCC 里是``__ attribute __``，在 VC 里是``__declspec``. 编译器拓展属性也可用在C中。

### 用法

- 简单属性

  `[[attribute]]`

  ```c++
  [[noreturn]]
  ```

- 命名空间属性

  `[[attribute_name::attribute]]`

  ```c++
  [[gnu::unused]]
  ```

- 实参属性

  `[[attribute(parameter)]]`

  ```c++
  [[deprecated("because")]]
  ```

- 命名空间实参属性

  ``[[attribute_name::attribute(parameter)]]``

  ```c++
  [[cats::meow([[]])]]
  ```

### 标准属性

C++ 标准仅定义下列属性。

```c++

[[noreturn]]	// (C++11)	指示函数不返回
[[carries_dependency]]	// (C++11)	指示释放消费 std::memory_order 中的依赖链传入和传出该函数。
[[deprecated]]	// (C++14)
[[deprecated("原因")]]	// (C++14)	指示允许使用声明有此属性的名称或实体，但因 原因 而不鼓励使用。
[[fallthrough]]	// (C++17)	指示从前一 case 标号直落是有意的，而在发生直落时给出警告的编译器不应该为此诊断。
[[nodiscard]]	// (C++17)
[[nodiscard("原因")]]	// (C++20)	鼓励编译器在返回值被舍弃时发布警告。
[[maybe_unused]]	//	(C++17)	压制编译器在未使用实体上的警告，若存在。
[[likely]]	// (C++20)
[[unlikely]] 	// (C++20)	指示编译器应该针对通过某语句的执行路径比任何其他执行路径更可能或更不可能的情况进行优化。
[[no_unique_address]]	// (C++20)	指示非静态数据成员不需要拥有不同于其类的所有其他非静态数据成员的地址。
[[optimize_for_synchronized]]	// (TM TS)	指示应该针对来自 synchronized 语句的调用来优化该函数定义
```

- ``[[noreturn]]``	

  ​		c++11, 指示函数不返回

  ```c++
  
  [[noreturn]]              // 属性标签
  int func(bool flag)       // 函数绝不会返回任何值
  {
      throw std::runtime_error("XXX");
  }
  ```

  

- `[[carries_dependency]]`

  ​		(C++11)，指示释放消费 std::memory_order 中的依赖链传入和传出该函数。

- `[[deprecated]]`

  ​		(C++14)

- `[[deprecated("原因")]]`

  ​		(C++14)	指示允许使用声明有此属性的名称或实体，但因 原因 而不鼓励使用。

- `[[fallthrough]]`

  ​		(C++17)	指示从前一 case 标号直落是有意的，而在发生直落时给出警告的编译器不应该为此诊断。

- `[[nodiscard]]`

  ​		(C++17)

- `[[nodiscard("原因")]]`

  ​		(C++20)	鼓励编译器在返回值被舍弃时发布警告。

- `[[maybe_unused]]`

  ​		(C++17)	压制编译器在未使用实体上的警告，若存在。

- `[[likely]]`

  ​		(C++20)

- `[[unlikely]]`

  ​		(C++20)	指示编译器应该针对通过某语句的执行路径比任何其他执行路径更可能或更不可能的情况进行优化。

- `[[no_unique_address]]`

  ​		(C++20)	指示非静态数据成员不需要拥有不同于其类的所有其他非静态数据成员的地址。

- `[[optimize_for_synchronized]]`

  ​		(TM TS)	指示应该针对来自 synchronized 语句的调用来优化该函数定义

### GCC拓展属性

​		GCC 的属性都在“gnu::”里

> [Attribute Syntax (Using the GNU Compiler Collection (GCC))](https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html)

常用的`gnu::`拓展属性：

- deprecated：与 C++14 相同，但可以用在 C++11 里。
- unused：用于变量、类型、函数等，表示虽然暂时不用，但最好保留着，因为将来可能会用。
- constructor：函数会在 main() 函数之前执行，效果有点像是全局对象的构造函数。
- destructor：函数会在 main() 函数结束之后执行，有点像是全局对象的析构函数。
- always_inline：要求编译器强制内联函数，作用比 inline 关键字更强。
- hot：标记“热点”函数，要求编译器更积极地优化。
