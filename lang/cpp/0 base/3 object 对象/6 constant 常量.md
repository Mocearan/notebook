# constant

​		常量具有不能改变的固定值。

​		从形式上说，c++有两种常量：字面常量和符号常量。

​		

## literal constant

​		A **literal** (also known as a **literal constant**) is a fixed value that has been inserted directly into the source code.

> Literals and variables both have a value (and a type). However, the value of a literal is fixed and can’t be changed (hence it being called a constant), whereas the value of a variable can be changed through initialization and assignment.

​		**Literal constants** (usually just called **literals**) are unnamed values inserted directly into the code.

​		They are constants because their values can not be changed dynamically (you have to change them, and then recompile for the change to take effect).

​		不要滥用含义不明显的字符常量，最好只在给`const / constexpr / enum`赋初值时使用。

​		每个字面值常量都对应一种数据类型。

### letaral type

​		The type of a literal is assumed from the value and format of the literal itself.

![image-20230312150458247](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230312150458247.png)

​		If the default type of a literal is not as desired, you can change the type of a literal by adding a suffix:

![image-20230312150528601](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230312150528601.png)

- ```整型```：十进制10，八进制012， 十六进制0xA
- ```无符号整型```：U、u后缀。

```c++
std::cout << 5; // 5 (no suffix) is type int (by default)
std::cout << 5u; // 5u is type unsigned int
std::cout << 5L; // 5L is type long

std::cout << 5.0; // 5.0 (no suffix) is type double (by default)
std::cout << 5.0f; // 5.0f is type float
```



### charactor literals

​		字符字面量是指单引号内的一个字符，数据类型是`char`，可以隐式的转换当前机器所用字符集中对应的整型值。

​		如``ASCII``字符集，``utf-8``字符集等。

​		字符集中的字符可以用任意进制的数字的整型值来表示，常用十进制数组来表示ASCII字符集的值。也可以用1~3位的八进制数`\num`或十六进制数`\xNum`。序列里十六进制数字的数量没有限制，遇到第一个不为八进制或十六进制的字符，表明当前8/16进制序列结束。

​		这使得我们可以表示出字符集中的每一个字符，还可以在长字符串中嵌入它们。

​		宽字符字面量`L'ab'`，数据类型是`wchar_t`，单引号内字符的数量及其含义依赖于具体实现。

​		c++程序可以操作``Unicode``等其他字符集，这些字符集的规模远超``ASCII``，通常表示成4个或8个十六进制数字

- 字面量形如`u'\uABCD'` 、`u'\xDEAD'`、`U'\UABCDECBA`。
- 对任意十六进制数字X而言，较短的`U'\UXXXX'`和`U\U0000XXXX`等价，但是字面量长度只能是4个或8个，称为通用字符名字。

> 如`u'0430'`是斯拉夫语小写字母`a`，utf-8中是两字节的十六进制值`D0B0`，utf-16中是2字节的十六进制值`0430`，utf-32中是四字节的十六进制值`00000430`

![image-20230316215839727](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230316215839727.png)

- `''`，ascii字符，`‘m’`
- `u8''`，UTF-8字符，`u8'm'`，可变宽字符编码，常用字符包括ascii字符为1字节，不常用字符占据2字节，特别罕见的字符3或4字节。
- `u''`，UTF-16字符，`u'm'`
- `U''`，UTF-32字符，`U'm'`
- `L''`，wchar_t字符，`L'm'`

### string literals

![image-20230312155024656](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230312155024656.png)

- `“”`，ascii字符串，`“mass”`
- `R""`，原始字符串，`“R(\b)”`
  - `R""`可以与其他的字符串前缀组合表达对应编码的原始字符串，但顺序敏感，R必须紧邻`“”`

- `u8""`，UTF-8字符串，`u8"hello"`
- `u""`，UTF-16字符串，`u"foo"`
- `U""`，UTF-32字符串，`U"foo"`
- `L""`，wchar_t字符串，`L"foo"`

#### C-style Null-terminated strings

​		a string as a collection of sequential characters. 

​		The sequence of character constants inside double quotes represents a string constant.

​		C风格字符串是以``‘\0’``作为空终止符的`char`数组，字符串字面量是静态分配的以`‘\0’`为空终止符的`char`常量数组。

> 字符串字面值实质上是一个const char []类型，编译器在每个字符串的结尾处加上一个'\0’来标识结束。
>
> 两个完全一样的字符串字面值是否同一存储依赖于实现。

`operator""`

```cpp
"Hello, world!"; // "Hello, world!" is a C-style string literal, static const char[]
```

- C++ will concatenate sequential string literals

  ```C++
  std::cout << "Hello," " world!";
  ```

> C++ also has literals for std::string and std::string_view. In most cases these won’t be needed, but they may occasionally come in handy when using type deduction, either via the `auto` keyword, or class template argument deduction.
>
> ​	如果两个字符串字面值位置紧邻，仅有空格、缩进或者换行符分隔，实质上是同一个字符串的自由书写格式。

​		字符串字面值中的空白符会截断字符串。

#### 转义序列

​		有两类字符程序员不能直接使用：

- 一类是不可打印的字符，如推个或其他控制字符，因为没有可视的图符。
- 另一类是在c++语言中有特殊含义的字符（单引号、双引号、问号、反斜线）。

#### raw strings

​		ASCII编码的字符串字面量，有部分字符需要以``‘\’``来进行转义才能使用。在正则表达式或其他一些复杂字符串中会变得相当复杂难懂，并造成潜在的错误。

​		为此c++提供了原始字符串字面量（raw string literal），在其中所有的字符都按所见表达，反斜线不会进行转义。

​		原始字符串字面量以`R"(chars)"`的形式表示`chars`字符序列，`()`是想要以裸字符串字面量表现的序列，对于字符串需要加上`“”`.

```c++
"\\w\\\\W";
R"("\w\\W")";
```

​		在两侧括号外与`“”`之间还可以插入对称的任意符号序列，与`()`共同组成对`()`的转义，使得在`chars`序列中能够使用`()`字符。

​		允许在`chars`序列中以换行进行美化而不会被视作换行符。

```c++
R"***("quoted string 
containing 
the usual terminator ("))")***";
// "quoted string containing the usual terminator ("))"
```



#### C++-style strings

`operator“”s`

#### C++-style string view

`“operator”sv`

```c++
#include <iostream>
#include <string>      // for std::string
#include <string_view> // for std::string_view

int main()
{
 using namespace std::literals; // easiest way to access the s and sv suffixes

 std::cout << "foo\n";   // no suffix is a C-style string literal
 std::cout << "goo\n"s;  // s suffix is a std::string literal
 std::cout << "moo\n"sv; // sv suffix is a std::string_view literal

 return 0;
};
```





### integer literals

​		整型字面量的类型由它的进制、取值和后缀共同决定。

#### 进制

- 整型字面量默认是十进制的
- 以`0b`为前缀的是二进制字面量
- 以`0`为前缀的是八进制字面量
- 以`0x`为前缀的是十六进制字面量

​		可以使用`‘`来分隔字面量，拥有以下形式:

- *decimal-literal*	 *integer-suffix*(optional)(1)

  ​	是一个非零十进制位（`1`、`2`、`3`、`4`、`5`、`6`、`7`、`8`、`9`）后随零或多个十进制位（`0`、`1`、`2`、`3`、`4`、`5`、`6`、`7`、`8`、`9`）

- *octal-literal*         *integer-suffix*(optional)(2)

  ​	是数位零（`0`）后随零或多个八进制位（`0`、`1`、`2`、`3`、`4`、`5`、`6`、`7`）

- *hex-literal*          *integer-suffix*(optional)(3)

  ​	是字符序列 `0x` 或字符序列 `0X` 后随一或多个十六进制数位（`0`、`1`、`2`、`3`、`4`、`5`、`6`、`7`、`8`、`9`、`a`、`A`、`b`、`B`、`c`、`C`、`d`、`D`、`e`、`E`、`f`、`F`）

- *binary-literal*     *integer-suffix*(optional)(4)    (since C++14)

  ​	 是字符序列 `0b` 或字符序列 `0B` 后随一或多个二进制位（`0`、`1`）

  > hexadecimal values useful as a concise way to represent a value in memory. For this reason, hexadecimal values are often used to represent memory addresses or raw values in memory.

Optional single quotes (`'`) may be inserted between the digits as a separator. They are ignored by the compiler.

```c++
unsigned long long l1 = 18446744073709550592ull; // C++11
unsigned long long l2 = 18'446'744'073'709'550'592llu; // C++14
unsigned long long l3 = 1844'6744'0737'0955'0592uLL; // C++14
unsigned long long l4 = 184467'440737'0'95505'92LLU; // C++14
int bin { 0b1011'0010 };  // assign binary 1011 0010 to the variable, c++14
// Also note that the separator can not occur before the first digit of the value:
int bin { 0b'1011'0010 };  // error: ' used before first digit of value
```





####  *integer-suffix*

 if provided, may contain one or both of the following (if both are provided, they may appear in any order:

- unsigned-suffix(`u` /``U``)
- one of
  - long-suffix(`l/L`)
  - lonog-long-suffix(`ll/LL`) （c++11)
  - size-suffix(`z/Z`)  (c++23)

![image-20230312153109983](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230312153109983.png)

#### 整型选择

整数字面量的类型，是依赖于所用数字基底和 *整数后缀* 的列表中，首个能适合其值的类型。



```c++
#include <cstddef>
#include <iostream>
#include <type_traits>
 
int main()
{
std::cout << 123    << '\n'
          << 0123   << '\n'
          << 0x123  << '\n'
          << 0b10   << '\n'
          << 12345678901234567890ull << '\n'
          << 12345678901234567890u   << '\n'; // 类型是 unsigned long long
                                              // 即使无 long long 后缀
 
//   std::cout << -9223372036854775808 << '\n'; // 错误：值 9223372036854775808
                // 不吻合 signed long long，这是无后缀整数字面量允许的最大类型
     std::cout << -9223372036854775808u << '\n'; // 应用于无符号值的一元减
                // 从 2^64 减去该值，给出 9223372036854775808
     std::cout << -9223372036854775807 - 1 << '\n'; // 计算值 -9223372036854775808
                                                    // 的正确方式
 
#if __cpp_size_t_suffix >= 202011L // C++23
     static_assert(std::is_same_v<decltype(0UZ), std::size_t>);
     static_assert(std::is_same_v<decltype(0Z), std::make_signed_t<std::size_t>>);
#endif
}
```



#### converting

- **Converting binary to decimal** (uint)

  ​	We multiply each binary digit by its digit value (determined by its position). Summing up all these values gives us the total: Consider the 8 bit (1 byte) binary number 0101 1110. Binary 0101 1110 means (0 * 128) + (1 * 64) + (0 * 32) + (1 * 16) + (1 * 8) + (1 * 4) + (1 * 2) + (0 * 1). If we sum up all of these parts, we get the decimal number 64 + 16 + 8 + 4 + 2 = 94.

- **converting decimal to binary**(uint)

  ​	The first method involves continually dividing by 2, and writing down the remainders. The binary number is constructed at the end from the remainders, from the bottom up.

  ​	The second method involves working backwards to figure out what each of the bits must be. This method can be easier with small binary numbers.

  > 148 = (1 * 128) + (0 * 64) + (0 * 32) + (1 * 16) + (0 * 8) + (1 * 4) + (0 * 2) + (0 * 1) = 1001 0100

  

Signed integers are typically stored using a method known as **two’s complement**.

> 0 would have two representations: 0000 0000 (positive zero) and 1111 1111 (negative zero). By adding 1, 1111 1111 intentionally overflows and becomes 0000 0000.        

In two’s complement, the leftmost (most significant) bit is used as the sign bit. A 0 sign bit means the number is positive, and a 1 sign bit means the number is negative.     

- **Converting binary to decimal** (int)

  ​	If the sign bit is 0, just convert the number as shown for unsigned numbers above.

  ​	If the sign bit is 1, then we invert the bits, add 1, then convert to decimal, then make that decimal number negative (because the sign bit was originally negative).

- **converting decimal to binary**(int)         

  ​	Positive signed numbers are represented in binary just like positive unsigned numbers (with the sign bit set to 0).

  ​	Negative signed numbers are represented in binary as the bitwise inverse of the positive number, plus 1.

  

#### printng decimal / octal / hexadecimal / binary number

​	Printing in decimal, octal, or hex is easy via use of std::dec, std::oct, and std::hex:

```c++
#include <iostream>

int main()
{
    int x { 12 };
    std::cout << x << '\n'; // decimal (by default)
    std::cout << std::hex << x << '\n'; // hexadecimal
    std::cout << x << '\n'; // now hexadecimal
    std::cout << std::oct << x << '\n'; // octal
    std::cout << std::dec << x << '\n'; // return to decimal
    std::cout << x << '\n'; // decimal

    return 0;
}
```

​	To use std::bitset, we can define a std::bitset variable and tell std::bitset how many bits we want to store. The number of bits must be a compile time constant. std::bitset can be initialized with an unsigned integral value (in any format, including decimal, octal, hex, or binary).

```c++
#include <bitset> // for std::bitset
#include <iostream>

int main()
{
	// std::bitset<8> means we want to store 8 bits
	std::bitset<8> bin1{ 0b1100'0101 }; // binary literal for binary 1100 0101
	std::bitset<8> bin2{ 0xC5 }; // hexadecimal literal for binary 1100 0101

	std::cout << bin1 << ' ' << bin2 << '\n';
	std::cout << std::bitset<4>{ 0b1010 } << '\n'; // create a temporary std::bitset and print it

	return 0;
}
```



### floating point literals

​		There are two different ways to declare floating-point literals:

- standard notation
- scientific notation

[Floating Point | Thomas Finley (tfinley.net)](https://tfinley.net/csarch-notes/2000/floating)

#### 科学计数法

​		Numbers in scientific notation take the following form: 

![](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220417120356817.png)

​		Because it can be hard to type or display exponents in C++, we use the letter ‘e’ (or sometimes ‘E’) to represent the “times 10 to the power of” part of the equation. 

​		For example, `1.2 x 10⁴` would be written as `1.2e4`

​		Here’s the most important thing to understand: The digits in the significand (the part before the ‘e’) are called the **significant digits**. The number of significant digits defines a number’s **precision**. The more digits in the significand, the more precise a number is.

##### How to convert numbers to scientific notation

​		Use the following procedure:

- Your exponent starts at zero.
- Slide the decimal so there is only one non-zero digit to the left of the decimal.
  - Each place you slide the decimal to the left increases the exponent by 1.
  - Each place you slide the decimal to the right decreases the exponent by 1.
- Trim off any leading zeros (on the left end of the significand)
- Trim off any trailing zeros (on the right end of the significand) only if the original number had no decimal point. We’re assuming they’re not significant unless otherwise specified.

![image-20230318154929611](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230318154929611.png)

​		```浮点型```：浮点型3.124 F/f ，双精度浮点型 3.144/ 3.144E0， 长双精度 2.134LD

#### pointer/reference

​		`std::nullptr_t`

### user-defined lieraral type

​		含有`constexpr`构造函数的类称为字面值常量类型。

​		构造函数必须足够简单才能声明成`constexpr`：

- 函数体必须为空
- 所有成全都是用潜在的常量表达式初始化的

```c++
constexpr Point origo{0,0};
constexpr int z = origo.x;

constexpr Point a[] = {
    origo, Point{1,1}, Point{2,2}, origo.move{3,3};
}
```





## symbolic constant

​		c++中有四种符号，它们又归属两类符号常量：

- compile-time constants 编译时常量：表达式在编译时求值

  ​	**Runtime constants** are constants whose initialization values can only be resolved at runtime (when your program is running). 

  ​	here are a few odd cases where C++ requires a compile-time constant instead of a run-time constant .

  - constexpr / enum / macro

- Runtime constants

  ​	**Compile-time constants** are constants whose initialization values can be determined at compile-time (when your program is compiling).

  ​	Compile-time constants enable the compiler to perform optimizations that aren’t available with runtime constants.

  - const variables

​		

​		对于`constexpr`和`const`的区别，`constexpr`表征一个编译期计算为常量的表达式，而`const`声明常变量。

​		人们更倾向于使用命名的符号常量而非字面量，因为：

- 命名常量使得代码易于理解和维护
- 某些语境下要求必须使用常量表达式（数组尺寸、case标签、template中的常量模板参数
- 嵌入式系统中，不可修改的数据往往置于只读内存，因为与动态内存想必，只读内存更廉价（价格、功耗）、空间也更大。系统崩溃时只读内存的数据也基本上不受影响。
- 如果在编译时完成了初始化操作，即使在多线程系统中也不会发生针对该对象的数据竞争。
- 编译时求值一次，把你在运行时多次求值的效率高很多。



### compile-time constants

#### 常量表达式

​		是指值不会改变并且在编译过程就能得到计算结果的表达式。

​		字面值术语常量表达式，常量表达式初始化的const对象也是常量表达式。

#### constexpr

​		ensures that a constant must be a compile-time constant.

​		`constexpr`含义是常量表达式求值，即在编译时求值，用于说明常量，允许将数据放置在只读内存中以及提升性能。

```c++
constexpr double gravity { 9.8 }; // ok, the value of 9.8 can be resolved at compile-time
constexpr int sum { 4 + 5 }; // ok, the value of 4 + 5 can be resolved at compile-time


int age{};
std::cin >> age;
constexpr int myAge { age }; // compile error: age is a runtime constant, not a compile-time constant
```

​		Note that literals are also implicitly constexpr, as the value of a literal is known at compile-time.

​		Constexpr variables can also be used in constant expressions：

```c++
constexpr int x { 3 };
constexpr int y { 4 };
std::cout << x + y; // x + y evaluated at compile-time
//Similar to the literal case, the compiler can substitute in the value 7.
```

​		如果某个函数参与常量表达式求值，则函数需要声明为`constexpr`：`constexpr double func(double p1);`

- 定义为`constexpr`的函数必须非常简单，只能有一条用于计算某个值的`return`语句。

- 当接受常量（字面值、符号常量中的编译时常量、被常量表达式初始化的常变量）时，结果就是一个常量表达式。

- 接受非常量实参的`constexpr`函数其结果就不会是一个常量表达式。

  > 即`constexpr`函数可以兼容常量表达式求值，和非常量表达式求值两种上下文环境，不必进行常量性重载。
  
- `constexpr`的基底含义仍旧是只读，因此声明为`constexpr`的函数不必限定为`const`函数

  ```c++
  class Point{
     constexpr Point move(int dx, intdy) /* const */ {return {x+dx, y + dy};} 
  };
  
  ```

- `constexpr`函数能够接收`const&`参数，因为`const&`引用的是值。

  ```c++
  template<>
  class complex<double>
  {
  public:
      constexpr complex(double re = 0.0, double im = 0.0);
      constexpr complex(const complex<float>&);
      explicit constexpr complex(const complex<long double>&);
      
      constexpr double real();
      void real(double);
      constexpr double image();
      void image(double);
  
  	complex<double>& operator=(double);
      complex<double>& operator+=(double);
  };
  
  /////
  constexpr complex<float> z1{1,2};
  constexpr double re = z1.real();
  constexpr double im = z1.image();
  
  constexpr complex<double> z2{re, im};
  constexpr complex<double> z3{z1}; // 编译器识别出 z1是一个字面值常量类型，而const complex<double>&引用的是一个常量值（不是指针或引用等其他东西）
  ```

  

#### macro

​		Avoid using #define to create symbolic constants macros. Use const or constexpr variables instead.

- because these #defined values aren’t variables, you can’t add a watch in the debugger to see their values. 
- macros can have naming conflicts with normal code
- macros don’t follow normal scoping rules



### const variables

​		任何语义上一经初始化就不应该再改变，并且初始化器在编译期不能确定的变量都应当声明为常变量。

> ​		Any variable that should not be modifiable after initialization and whose initializer is not known at compile-time should be declared as const.

​		`const`的含义是”承诺不改变对象的值”，其说明的主体是对象的访问形式，主要用于接口说明。即“对象接口、或对对象的访问操作承诺不改变对象的值”。

> 需要注意的是直接使用对象也是一种对象访问的接口形式。
>
> 在声明符一节详细说明了对象访问。
>
> 一个指针或引用涉及两个对象，关于它们的常量性讨论在指针或引用小节进行说明。这里只讨论一般的常变量性质。对于指针或引用，其本质上也是对这种常变量的应用。

​		声明常变量就不必担心变量会在操作内部被改变，编译器负责确认并执行`const`的承诺。

#### 常变量的性质

> CV限定实际上是从类型的可写角度，来限定对象从外部可写的权限。

>   而对象内部限定的可写性，是对象自身性质的一部分，依赖类的访问控制机制（常函数接口，常成员等，公有私有等）

- 常变量只能初始化，不能被赋值
  - 常变量可以被常量表达式初始化，这种常变量可以用在常量表达式中
  - 常变量可以被非常量表达式初始化，这种常变量不可以用在常量表达式中
  - 通常，定义概念上的常量时，指符号化的字面量，应该使用`constexpr`
    - 不能使用c++11时，常用枚举值类代替`constexpr`的功能，来替换`const`

- 常变量可以赋值变量，常变量不能被赋值
- 指针可以赋值给常指针，表示以常量访问的形式访问原指针索引的对象
- 常指针不能赋值给指针，常指针的常量性限定丢失

> 常变量可以初始化自字面量、常变量或变量，因为常变量的创建是独立变量的拷贝。
>
> 常指针间接访问对象，这里的常是限定指针对索引对象的访问形式，而非指针本身。除非将指针本身声明为常变量（常指针常变量）

#### 常变量的声明

##### 常变量替换字面量

​		simply put the `const` keyword either before or after the variable type, like so:

```c++
const double gravity { 9.8 };  // preferred use of const before type
int const sidesInSquare { 4 }; // "east const" style, okay, but not preferred
const int something { 1 + 2 }; // the compiler can resolve this at compiler time
```

​		Const variables *must* be initialized when you define them, and then that value can not be changed via assignment:

```c++
const double gravity; // error: const variables must be initialized
gravity = 9.9; // error: const variables can not be changed
```

###### magic numbers

​		**Avoid magic numbers, use symbolic constants instead**

​		A **magic number** is a literal (usually a number) that either has an unclear meaning or is used multiple times.

​		Using magic numbers is generally considered bad practice because, in addition to not providing context as to what they are being used for, they pose problems if the value needs to change.

​		Magic numbers aren’t always numbers -- they can also be strings or other types.

```c++
const int maxStudentsPerSchool{ numClassrooms * 30 };

const int maxStudentsPerClass { 30 }; // now obvious what 30 is
const int maxStudentsPerSchool{ numClassrooms * maxStudentsPerClass };
```



​		Note that literals used only once, and in obvious contexts, are not considered “magic”. The values -1, 0, 0.0, and 1 are often used in contexts that are obvious:

> ```cpp
> int idGenerator { 0 };         // fine: we're starting our id generator with value 0
> idGenerator = idGenerator + 1; // fine: we're just incrementing our generator
> int kmtoM(int km) { return km * 1000; } // fine: it's obvious 1000 is a conversion factor
> ```



##### 变量的常变量副本

​		Any variable that should not be modifiable after initialization and whose initializer is known at compile-time should be declared as constexpr.

​		const variables can be initialized from other variables (including non-const ones):

```c++
const int usersAge { age };

void printInt(const int x) // x is a runtime constant because the value isn't known until the program is run
{
    std::cout << x;
}
```







#### 操作的常量性

​		Const function parameters and return values。

​		函数的参数列表，通过不同的声明符，说明了对实参对象的访问方式。

​		函数的返回值，通过不同的声明符，说明了操作限定的对返回值的访问方式。

- 首先说明，`const &`能够延长对象的生命周期。

```cpp
void printInt(const int x)
{
    std::cout << x;
}
```

​		Making a function parameter const enlists the compiler’s help to ensure that the parameter’s value is not changed inside the function. Note that we did not provide an explicit initializer for our const parameter -- the value of the argument in the function call will be used as the initializer in this case.

​		Function parameters for arguments passed by value should not be made const.

> we generally don’t care if the function changes the value of the parameter (since it’s just a copy that will be destroyed at the end of the function anyway)

​		不要在`return by value`的函数返回值上使用`const`，`return by value`往往有用。但`return by pointer`往往比较危险。



#### 常量性操作

​		用在类成员中，说明成员函数的操作对对象是常量性的，不会改变对象的状态。
```c++
void A::getX() const;
```





#### const variable name

- Some programmers prefer to use all upper-case names for const variables. 

- Others use normal variable names with a ‘k’ prefix. 

- However, we will use normal variable naming conventions, which is more common. 

  > ​		Const variables act exactly like normal variables in every case except that they can not be assigned to, so there’s no particular reason they need to be denoted as special.



### symbolic constants in multi-file program

​		In many applications, a given symbolic constant needs to be used throughout your code (not just in one location). These can include physics or mathematical constants that don’t change (e.g. pi or Avogadro’s number), or application-specific “tuning” values (e.g. friction or gravity coefficients). Instead of redefining these every time they are needed, it’s better to declare them once in a central location and use them wherever needed. That way, if you ever need to change them, you only need to change them in one place.



