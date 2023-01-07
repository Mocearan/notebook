#    第一部分  c++基础




编程语言都具备一组公共的语法特征，不同语言仅在特征的细节上有所区别。学习编程语言，理解语法特征的实现细节是第一步。

最基本的特征包括：

- 内置数据类型
- 变量，用来为对象命名
- 表达式和语句，用于操纵上述数据类型的具体值
- 流程控制语句，这些结构允许我们有选择的执行一些语句或者重复的执行一些语句
- 函数，用于定义可供随时调用的计算单元

大多数编程语言通过两种方式来进一步扩充基本特征：

- 程序员自定义数据类型
- 将一些有用的功能封装成库函数提供给程序员

运行时检查数据类型的语言称为动态数据类型语言，c++是一种静态数据类型语言，类型检查发生在编译时，编译器必须知道程序中每一个变量对应的数据类型。

c++中最重要的语法特征都表现在类上。通过它程序员可以定义自己的数据类型。



---

# < 2 >  变量和基本类型

数据是程序的基础，体现数据的意义以及能在数据上执行什么操作。

c++支持广泛的数据类型，定义了内置数据类型也为程序员提供了自定义数据类型的机制。因此c++标准库定义了一些更加复杂的数据类型，比如可变长字符串和向量等。

https://zh.cppreference.com/w/cpp/language/type C++ 类型系统

**类型的一个重要作用就是：对于不同的数据类型，应用不同的数据解译方案。**，这就使得同样的数据（纯粹的二进制数据），可以表达多种多样的含义，从而构建起了计算机抽象现实世界的基础。

![image-20200919192807335](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200919192807335.png)

## 2.0 变量基础

关于变量有几个简单的要义：

-   存储什么数据（数据类型）
-   存储在哪里（内存分配方式）
-   如何使用（引用存储的数据）

### 变量的命名规则

-   大小写的英文字母
-   阿拉伯数字（不能做首字符）
-   下划线

编译器的字符解析支持Unicode之后，实质上已经可以包含世界上所有的自然语言了。唯一一条保留的规定是：阿拉伯数字不能做首字符。

但是，作为语言的保留关键字和运算符不能作为变量名。

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



## 2.1 基本内置类型

cc++基础内置类型包括：
	算数类型：字符、整数、布尔值、和浮点数。
	空类型：不对应具体的值，仅用于一些特殊的场合，最常见的是，当函数不返回任何值时使用空类型作为返回类型。

### 算数类型

分为两类：

整形（integral type， 包括字符和布尔类型在内）和 浮点型

算数类型的尺寸（也就是该类型数据所占的比特数）在不同机器上有所差别。

c++标准规定了尺寸的最小值。同时允许编译器赋予这些类型更大的尺寸。

c++的字符和布尔类型其实也都属于整数类型范畴。只不过字符和布尔并不表示整数，而使用其各自的字符集查找方案或者标记替换一定尺寸的整数。

![image-20200915225845274](https://gitee.com/masstsing/picgo-picserver/raw/master/20200915225846.png)



#### 整数类型

整数用于表示不同尺寸的整数。

![image-20200919193329888](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200919193329888.png)

c++语言规定：

-   int至少和short一样大

-   long至少和int一样大 (32位系统上，ling == int 32 bit, 64位系统上, 64bit)

    所以long的位数和系统位数，一般来说是一样的。在查看指针地址等有关于操作系统字长的数据时，应该使用long作为数据类型。

    [字长]([https://baike.baidu.com/item/%E5%AD%97%E9%95%BF/97660?fr=aladdin](https://baike.baidu.com/item/字长/97660?fr=aladdin))

-   long long至少和long一样大

其中数据类型long long是在c++11 中新定义的。

![image-20200915232056919](https://gitee.com/masstsing/picgo-picserver/raw/master/20200915232057.png)



> 内置类型的机器实现：
>
> 计算机以2的整数次幂个bit为单位划块来处理内存数据，可寻址的最小内存块称为“字节”（byte）。存储的基本单元称为“字”（word），通常由几个字节组成，视不同机器有所不同。
>
> c++中，一个字节要至少能容纳机器基本字符集中的字符。大多数机器的字节由8bit构成，字则由32或64bit构成，也就是4或8字节。
>
> 计算机将内存中的每个字节与一个数字（address，地址）关联，在一个字节为8bit、字为32bit的机器上，我们可能看待一个字的内存区如图：
>
> ![image-20200915232204084](https://gitee.com/masstsing/picgo-picserver/raw/master/20200915232204.png)

>
>左侧地址，右侧是每个字节的8bit。
>
>可以使用某个地址来表示从这个地址开始的大小不同的比特串。为了赋予内存中某个地址明确地含义，必须首先知道存储在该地址的数据的类型。类型决定了数据所占的比特数以及该如何解释这些比特的内容。
>
>即，内存只是简单的存放了二进制数据，对这些二进制数据的定义与解释依赖于类型，不同类型有对这个数据的不同的解释方式。导致同一数据在不同的类型上有不同的表现。

#### 字符类型

![image-20200919193133109](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200919193133109.png)

c++提供了几种字符类型，其中多数支持国际化。（以国际化的字符集表示法去解析该类型存储的数据）。

基本的字符类型是**char**

>   				一个char的空间应确保可以存放机器基本字符集几种任意字符对应的数字值。也就是说，**一个char的大小和一个机器字节一样**。使用ASCII编码。
>   			
>   			`‘0’`,  48
>   			
>   			`‘A’`, 65
>   			
>   			`‘a’`, 97
>   			
>   			`‘\0’`, 0
>   			
>   			c++中并没有定义byte字节类型，（数据存储单位，并非表示字符，只用于存储一个字节大小的数据）。
>   			
>   			所以在c++中，char的首要作用是表示0-127的标准ASCII码的字符集，表示字符数据；另一个次要作用是，作为非常规的有符号字节存储单位来使用。但是更为准确的做法是，使用unsigned char来存储byte数据。

```c++
// 在c++中对于，ASCII码值和字符之间的转换使用
//

#include <iostream>

using namespace std;

int main(void)
{
    // 将字符转换为ASCII码值，将其转换为整数类型即可。
    char c = '?';
    cout << (short)c << endl;
    cout << (int)';' << endl;

    // 将ASCII码值转换为字符，将其转换为字符类型即可。
    char cc = 60;
    cout << cc << endl;
    cout << (char)61 << endl;

    return 0;
}
```

>   **转义字符**
>
>   		由于有些字符不能直接使用或有歧义，在表达该字符时需要使用‘\’进行换行。
>   	
>   	`\n`: new line ，换行符
>   	
>   	`\t`: table , 水平制表。一般情况下是8个字符。
>   	
>   	`\v`：vertical table，垂直制表符。
>   	
>   	`\b`: back， 退格符， 光标往前退格
>   	
>   	`\a`: alert， 发出一个声音提示
>   	
>   	`\r`: 回车
>   	
>   	`\\`: \
>   	
>   	`\'`: ‘
>   	
>   	`\"`: “





其他的字符类型用于扩展字符集。

`char8_t`: c++20

```char16_t```：为Unicode， 使用时需要加前缀u

```char32_t```：为Unicode， 使用时需要加前缀U

```wchar_t```：32bit，用于确保可以存放机器最大扩展字符集中的任意一个字符。使用时需要加前缀L。

```c++

// 宽字符类型的使用
//

#include <iostream>
#include <string>
using namespace std;


int main(void)
{
    wchar_t c1 = L'中';

    char16_t c2 = u'龙';

    char32_t c3 = U'化';

    char c = 'a';
    char16_t ccc = u'a';
    char32_t cccc = U'a';
    wchar_t wc = L'a';

    cout << "char size " << sizeof(c) << endl;
    cout << "char16 size " << sizeof(ccc) << endl;
    cout << "char32 size " << sizeof(cccc) << endl;
    cout << "wchar size " << sizeof(wc) << endl;

    wcout << wc << endl;

    wchar_t e = L'我';
    cout << e << endl; // 得到unicode的编码值
    wcout << e << endl; // 输出中文, 这种方式还是有问题
    wcout << (wchar_t)e << endl;// 输出中文，这种方式也有问题
    cout << "\u4E2D" << endl; // 通过Unicode码使用字符，需要加\u进行编码指定

    return 0;
}

```



#### 布尔类型

`bool`， 标准化之前的C++和c一样没有bool类型，在ANSI/ISO c++ standard后加入了bool。

```c++
bool flag1 = true;
bool flag2 = false;

if(flag1 == true && flag2 == false)
    cout << "condition right" << endl;
```



在c++中，布尔类型作为数值类型实现，true为1，false为0.可以与其他的算术类型进行运算。

当非零值（无论正负）赋给bool时，认为是真，且都成为true（1），0则为假。

其大小为1字节。（sizeof(bool))的值由实现定义，而且可以不为1.

```c++
#include <iostream>

using namespace std;
int main(void)
{
    bool b = true;
    cout << sizeof(b) <<endl; //1

    bool bb = 10000000000;
    cout << sizeof(bb) << endl;//1
    cout << bb << endl;//1
    return 0;
}
```





#### 浮点型

浮点类型由三个类型：

```float```：单精度(6-7)，1个机器字长（32bit / 64bit）。通常4字节。

```double```：双精度(15-16)， 2个机器字长。通常8字节。

```long double```：拓展精度(18-19)， 3 或4个机器字长。通常16字节。

一般来说，类型 float 和 double 分别有 7 和 16个有效位。long double则通常被用于有特殊浮点需求的硬件，它的具体实现不同，精度也各不相同。

**这里说的精度是以科学计数法表示的小数部分精度，而非实数意义上的小数精度。**

![image-20200919193401973](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200919193401973.png)

```c++
#include <iostream>
#include <cfloat>

int main(void)
{
    float value1 = 234.56f; // 浮点型字面量默认是double类型，使用f后缀指定为float型
    auto value2 = 123.11; // double
    long double value3 = 132.341l; // long doueble

    // 科学计数法
    // 1. 尾数
    // 2. E/e
    // 3. 指数
    double value4 = 1.23E15;

    // 浮点数的极限不是以精确的数值定义的，而是通过指数的大小来确定的。
    // float: +38 ~ -37
    // double: +308 ~ -307
    // long double: +4932 ~ -4931
    int f_max = FLT_MAX_10_EXP; // 其值是指数的极限
    int d_max = DBL_MAX_10_EXP;
    int ld_max = LDBL_MAX_10_EXP;

    std::cout << f_max << std::endl; // 38
    std::cout << d_max << std::endl; // e08
    std::cout << ld_max << std::endl;// 4932

    // 精度
    int f_dig = FLT_DIG;    //6
    int d_dig = DBL;		//15
    int ld_dig = LDBL_DIG; 	//18

    return 0;
}
```



> **浮点数的精度**
>
> ![image-20200919204243774](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200919204243774.png)
>
> 由于浮点数的小数计算机表示，是对二进制整数的小数化解译，即``` sum(2*-bit)```（整数的二进制表示是``` sum(2*bit)```），所以要精确表示小数位的浮点数，需要该小数位后的若干位来进行补足（比如0.6，需要 1/2 + 1/8 来表示，即 0.5 + 0.125)，所以需要注意浮点数并非是完全精确是十进制小数+。 
>
> ![image-20200920233033901](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920233033901.png)
>
> 
>
> **浮点型的机器表示** : 浮点数的比较，从浮点数的存储方式理解
>
> ```c++
> // 浮点数的比较：浮点数的存储方式
> float f_v1 = 20;
> float f_v2 = 20.3;
> float f_v3 = 20.5;
> double d_v1 = 20;
> double d_v2 = 20.3;
> double d_v3 = 20.5;
> 
> std::cout << ((f_v1 == d_v1) ? "true":"false") << std::endl; // true
> std::cout << ((f_v2 == d_v2) ? "true":"false") << std::endl; // false
> std::cout << ((f_v2 == (float)d_v2) ? "true":"false") << std::endl; // true
> std::cout << ((f_v3 == d_v3) ? "true":"false") << std::endl; // true
> 
> ```
>
> ![image-20200920105436849](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920105436849.png)
>
> ![image-20200920105637164](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920105637164.png)
>
> 由于浮点数的小数计算机表示，是对二进制整数的小数化解译，即``` sum(2*-bit)```（整数的二进制表示是``` sum(2*bit)```），所以要精确表示小数位的浮点数，需要该小数位后的若干位来进行补足（比如0.6，需要 1/2 + 1/8 来表示，即 0.5 + 0.125)，所以需要注意浮点数并非是完全精确是十进制小数+。 
>
> 20.3 = 16+4+0.25 + 0.03125 + ….
>
> 累加法：
>
> ![image-20200920111618675](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920111618675.png)
>
> ![image-20200920111658387](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920111658387.png)
>
> 二进制存储表示： 1符号 + 指数的二进制移位码 + 尾数的二进制表示
>
> 20.3 ： 1.0100010110011001…E100（2^4)
>
> 符号：0 ， 1bit
>
> 指数：
>
> 		float， 8bit，指数二进制+127移码， 0000 0100 + 0111 1111， 1000 0011
> 	
> 		double, 11bit, 指数二进制+1023移码，0000 0100 + 0011 1111 1111， 0100 0000 0011
>
> 尾数：10100010110011001…
>
> (float)1-1000 0011-10100010110011001…
>
> (double)1-0100 0000 0011-10100010110011001…
>
> 但是对于float和double之间的比较，指数部分的不同并不是造成差异的原因，因为在比较时，指数部分还是会减去各自的移码，回到0000 0100去比较。造成差异的根本原因，是二进制表示的浮点数，无法精确的表示每一个十进制的实数小数。只能通过逼近极限的方式求得某个精度下的近似值。(1/2, 1/4, 1/8…组合)。所以在float和double比较时，由于尾数精度的不同，造成了近似值的差异。
>
> ![image-20200920120554366](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920120554366.png)
>
> ![image-20200920120726577](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200920120726577.png)

#### 有符号和无符号类型

除布尔型和扩展的字符型外，其他的整型都可以划分为**带符号（signed）和无符号的（unsigned）**。

带符号类型可以表示正数、负数或0，无符号类型则仅能表示大于等于0的值。

类型int、short、long和long long都是带符号的。加前缀unsigned可以得到无符号的。

字符型被分为了三种：char、signed char、unsigned char。

> **注意**：
>
> 	char和signed char并不一样。
>
> 尽管字符型有三种，但是字符的表现形式却只有两种：带符号和无符号的。char实际上会表现为有无符号两种形式中的一种，具体是哪种由编译器来决定。(char 最终的编译是按照有符号还是无符号解析，是由编译器来决定的。)

无符号类型中的所有bit都用来存储值。

c++标准并没有规定带符号类型应如何表示，但是约定了在表示范围内正值和负值的量应该平衡。

因此，8bit的signed char 理论上应该可以表示-127值127区间内的值，大多数现代计算机将实际的表示范围定位-128到127.（因为存在正负0，所以当值为0时，将符号位0/1中的某一个与后续的所有`0`组合为-128（补码）。

> 选择类型：
>
> **与c一样，c++的设计准则之一也是尽可能地接近硬件。c++的算数类型必须满足各种硬件特质，所以它们常常显得繁杂而令人不知所措**
>
> 实施上，大多数程序员能够（也应该）对数类型的使用作出限定从而简化选择的过程。
>
> - 当明确知晓数值不可能为负时，选用无符号类型
>
> - 使用int执行整数运算。
>
>   在实际应用中，short常常显得太小而long一般和int有一样的尺寸。
>
>   如果数值超过了int的表示是范围，选择用long long
>
> - 在算数表达式中不要使用char 和 bool，只有才存放字符或者布尔值时才使用它们。
>
>   因为类型char在不同机器上可能是有符号和无符号的，使用char进行运算可能会出现问题。如果需要一个不大的整数，明确指出是signed char还是unsigned char。
>
> - 执行浮点运算选用double。
>
>   这是因为float通常精度不够而且双精度浮点数和运算代价基本相同甚至更低。long double提供的精度一般情况下是没必要的。

### 类型宽度

```sizeof(TYPE)```

```c++
// centos 7  64bit 

#include <iostream>

using namespace std;

int main(void)
{
    cout << "short: " << sizeof(short) <<endl; //2
    cout << "int: " << sizeof(int) <<endl; //4
    cout << "long: " << sizeof(long) <<endl;//8
    cout << "long long: " << sizeof(long long) <<endl; //8

    return 0;
}
```



### 类型的极限（limit）

![image-20200919193044259](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20200919193044259.png)

```c++
 // <climits>
 //  https://zh.cppreference.com/w/cpp/header/climits

#define CHAR_BIT /* 见定义 */
#define SCHAR_MIN /* 见定义 */
#define SCHAR_MAX /* 见定义 */
#define UCHAR_MAX /* 见定义 */
#define CHAR_MIN /* 见定义 */
#define CHAR_MAX /* 见定义 */
#define MB_LEN_MAX /* 见定义 */
#define SHRT_MIN /* 见定义 */
#define SHRT_MAX /* 见定义 */
#define USHRT_MAX /* 见定义 */
#define INT_MIN /* 见定义 */
#define INT_MAX /* 见定义 */
#define UINT_MAX /* 见定义 */
#define LONG_MIN /* 见定义 */
#define LONG_MAX /* 见定义 */
#define ULONG_MAX /* 见定义 */
#define LLONG_MIN /* 见定义 */
#define LLONG_MAX /* 见定义 */
#define ULLONG_MAX /* 见定义 */
```

c++与C的类型系统并非完全相同，但可兼容。为此，c++有着更严格的类型检查。

```c++
// <limits>
//  https://zh.cppreference.com/w/cpp/header/limits

namespace std {
    template<class T> class numeric_limits;
 
    enum float_round_style {
        round_indeterminate         = -1,
        round_toward_zero           =  0,
        round_to_nearest            =  1,
        round_toward_infinity       =  2,
        round_toward_neg_infinity   =  3,
    };
 
    enum float_denorm_style {
        denorm_indeterminate = -1,
        denorm_absent        =  0,
        denorm_present       =  1
    };
 
 
    template<> class numeric_limits<bool>;
 
    template<> class numeric_limits<char>;
    template<> class numeric_limits<signed char>;
    template<> class numeric_limits<unsigned char>;
    template<> class numeric_limits<char8_t>;
    template<> class numeric_limits<char16_t>;
    template<> class numeric_limits<char32_t>;
    template<> class numeric_limits<wchar_t>;
 
    template<> class numeric_limits<short>;
    template<> class numeric_limits<int>;
    template<> class numeric_limits<long>;
    template<> class numeric_limits<long long>;
    template<> class numeric_limits<unsigned short>;
    template<> class numeric_limits<unsigned int>;
    template<> class numeric_limits<unsigned long>;
    template<> class numeric_limits<unsigned long long>;
 
    template<> class numeric_limits<float>;
    template<> class numeric_limits<double>;
    template<> class numeric_limits<long double>;
}
```

```c++

```





虽然定义了类型的极限值，但是在c++仍然允许类型数据溢出。但是由于计算机最终处理数据是以补码的形式处理，所以在极限值的溢出处理上遵循补码表示原则：[机器数编码](https://blog.csdn.net/dai_ma_dong/article/details/88853006)

```c++
#include <iostream>
#include <climits>

using namespace std;

int main(void)
{
    cout << "int max : " << INT_MAX  <<endl;
    cout << "uint max : " << UINT_MAX    <<endl;
    cout << "ull max : " <<  ULONG_LONG_MAX  <<endl;

    int val1 = INT_MAX + 1;
    cout << "int max + 1 :" << val1 << endl;
    val1 = INT_MAX + 2;
    cout << "int max + 2 :" << val1 << endl;
    unsigned int val2 = UINT_MAX + 1;
    cout << " uint max + 1 :" << val2 << endl;
    val2 = UINT_MAX + 2;
    cout << " uint max + 2 :" << val2 << endl;
    unsigned int val3 = -1;
    cout << " uint -1 :" << val3 << endl;


    return 0;
}



int max : 2147483647
uint max : 4294967295
ull max : 18446744073709551615
int max + 1 :-2147483648
int max + 2 :-2147483647
 uint max + 1 :0
 uint max + 2 :1
 uint -1 :4294967295
```



### 类型转换

对象的类型定义了数据和运算两个概念。而在运算中有一种操作被普遍支持，就是进行类型之间的转换（convert）。

#### 自动隐式类型转换

使用一种类型来取用另一种对象。类型所能表示的值的范围决定了转换的过程。

> 转为bool：0为false， 其他包括负数为正。
>
> bool转为其他：false为0， true 为1.
>
> 浮点数转为整数：舍弃小数部分。
>
> 整数转为浮点数：浮点部分为0，整数超过浮点数表示范围时会有精度损失。
>
> 给无符号类型超出表示范围的值：初始值对无符号类型表示的数值总数取模后的余数。
>
> 给有符号类型一个超出表示范围的值时，结果是未定义的（undefined）。程序可能继续工作、崩溃，或者该内存成为垃圾数据。

>
>
>​	无法预知的行为源于编译器无须（有时是不能）检测的错误。即使代码编译通过了，如果程序执行了一条未定义的表达式，仍有可能产生错误。
>
>不幸的是，在某些情况和/或某些编译器下，含有无须预知行为的程序也能正确执行。但是我们却无法保证同样一个程序在别的编译器下能正常工作，甚至已经编译通过的的代码再次执行也可能会出错。此外，也不能认为这样的程序对一组输入有效，对另一组输入就一定有效。
>
>程序也应该尽量避免依赖于实现环境的行为。如果我们把int的尺寸看成是一个确定不变的已知值，那么这样的程序就称作不可移植的（nonportable）。当程序移植到别的机器上后，依赖于实现环境的程序就可能发生错误。要从过去的代码中定位这类错误可不是一件轻松愉快的工作。
>
>当从无符号数中减去一个值时，不管这个值是不是无符号数，我们都必须确保结果不能是一个负值。
>
>notice:
>	切勿混用带符号类型和无符号类型

#### 强制类型转换



### 字面值常量

每个字面值常量都对应一种数据类型。

#### 整型和浮点型字面值

```整型```：十进制10，八进制012， 十六进制0xA，长整型l

```无符号整型```：U、u后缀。

```浮点型```：浮点型3.124 F/f ，双精度浮点型 3.144/ 3.144E0， 长双精度 2.134L

#### 字符和字符串字面值

```字符```：'a’

```字符串```："abc”

字符串字面值实质上是一个const char []类型，编译器在每个字符串的结尾处加上一个'\0’来标识结束。

如果两个字符串字面值位置紧邻，仅有空格、缩进或者换行符分隔，实质上是同一个字符串的自由书写格式。

#### 转义序列

有两类字符程序员不能直接使用：一类是不可打印的字符，如推个或其他控制字符，因为没有可视的图符。另一类是在c++语言中有特殊含义的字符（单引号、双引号、问号、反斜线）。

![eb0125e78246fce9beb94f8253d78ab1.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C1000:1)


![f41c03d61f19ae5d7a7927c1de1277d6.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C1001:1)


#### 指定字面值的类型

![7cbca797e2fc56eeba091a54a4c5b2ee.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C997:1)


可以组合部分标识字来使用，如UL。

#### 布尔字面值和指针字面值

```布尔```：true真，false假

```指针```：NULL，nullptr

## 2.2 变量

变量提供一个具名的、可供程序操作的存储空间。

c++每个变量都有其数据类型，数据类型决定变量的空间大小、布局方式、空间能存储的值的范围，以及变量能参与的运算。

对c++来说，变量和对象可以互换使用。

### 2.2.1 变量定义

> 变量定义：类型说明符（type specifier）+ 变量名列表（逗号分隔，分号结束，可初始化）

对象是指一块能存储数据并具有某种类型的内存空间。

#### 变量初始化 （c 语言兼容的初始化）

当对象在创建时获得了一个特定的值，称对象被**初始化（initialized）**了。在同一条语句中可以使用已经初始化的变量去初始化后定义的变量。

c++语言中，赋值和初始化是两个完全不同的操作。

> 初始化的含义是创建变量时赋予其一个初始值。
>
> 赋值的含义是把对象当前值擦掉，而以一个新的值来替代。

```c++
int n = 30;
int k = n - 50;
int x;
x = n - m;
```



#### 初始化列表 （c++中不同于C语言的初始化方式）

- 列表初始化的四种形式

```c
int units_sold1 = 0; // c兼容的初始化方式

int units_sold4(0);  // C++中基于对象构造衍生的初始化方式
// 由于是基于构造函数的初始化，所以不能像c兼容初始化那样延后初始化，除非是默认初始化后重新赋值。
// int count;
// count(2); //error.

int units_sold2 = { 0 };// 基于数组列表初始化衍生的单值初始化，一般仅用于“值列表”语义（c++98）
// 由于是基于初始化列表的初始化，只要相应的赋值重载支持了初始化列表对象参数，就可以延后赋值时使用列表初始化
// int count = {20};
// count = {400};

int units_sold3{ 0 }; // 基于对象构造和数组列表初始化衍生的初始化方式。（C++11）
// 同上
// int count{20};
// count{400};


// 默认初始化
int day = {};
int day{};
int day();
int day;
```

```列表初始化```：作为c++11新标准的一部分，用花括号来初始化变量的到了全面应用，而在此之前，这种初始化的形式仅在某些受限的场合下才能使用。

**无论是初始化对象还是某些时候为对象赋新值都可以使用这样一组由花括号括起来的初始值了。**

如果使用列表初始化且初始值存在丢失信息的风险，则编译器报错。

```c
	long double ld = 3.1415926536;
	//int a{ ld }, b = { ld };//转换未执行，存在丢失信息的风险  从“long double”转换到“int”需要收缩转换
	int c(ld), d = ld;	//转化执行，确实丢失了部分值
```





#### 默认初始化

如果变量定义时没有指定初始值，则变量被默认初始化（default initialized），此时变量被赋予默认值。

- ```内置类型```

  函数体之外（全局作用域）的变量被初始化为0.

  函数体之内（局部作用域）的变量不被初始化，值是未定义的，拷贝或其他形式的访问将引发错误。

- ```类```

  类的初始化方式由类的各个构造函数来定义。是否允许不经初始化就定义对象也由类自己决定（默认构造函数，类提供一个合适的默认值）。

> **未初始化变量引发运行时故障**
> 		未初始化变量含有一个不确定的值，使用未初始化变量的值时一种错误的编程行为并且很难调试。编译器都能对一部分使用未初始化变量的行为提出警告，但编译器并未被要求检查此类错误。
> 		使用未初始化的变量将带来无法预计的后果。
> 		如果幸运程序会崩溃，此时只要找到崩溃的位置就很容易发现变量没被初始化的问题。另外一些时候，程序会错误执行。
> 		更糟糕的情况是，程序结果过时对时错、无法把握。而且，往无关的位置添加代码还会导致我们误认为程序对了，其实结果仍旧有错。
>
> 	advice：
> 		建议初始化每一个内置类型的变量。虽然并非必须这么做，但如果我们不能确保默认初始化后程序安全，那么这么做不失为一种简单可靠的方法。

### 2.2.2 变量声明和定义的关系

c++语言支持分离式编译（separate compilation）机制，为此c++将声明和定义区分开来。

> 分离式编译：允许将程序分割为若干个文件，每个文件可被独立编译。
>
> 如果要在多个文件中使用同一个变量，就必须将声明和定义分离。此时，变量的定义必须出现在且只能出现在一个文件中，而其他用到该变量的文件必须对其进行声明，却绝对不能重复定义。

```声明（declaration）```：使名字在当前文件可用。一个文件如果想使用别处定义的名字则必须包含对那个名字的声明。

```定义（definition）```：负责在某个文件中唯一创建于名字关联的对象实体。定义会申请对象的存储空间，无论堆栈。

如果想声明一个变量而非定义它，就在变量名前加extern关键字，且不要显式初始化变量。

> 任何包含了显式初始化的声明即成为定义。
> 		我们能给由extern关键标记的变量赋新值，但是这么做也就抵消了extern的作用。extern语句如果包含初始化就不再是声明，而变成定义了；

在函数体内部初始化一个由extern关键字表示的变量，将引发错误。（局部作用域只能向外看到本文件的全局作用域）。

变量能且智能被定义一次，但是可以被多次声明。

	静态类型：
		c++是一种静态类型（statically typed）语言，其含义是在编译阶段检查类型。其中，检查类型的过程称为类型检查（type checking)
		我们已经知道，对象的类型决定了对象所能参与的运算。c++语言中，编译器负责检查数据类型是否支持要执行的运算，如果试图执行类型不支持的运算，编译器将报错并且不会生成可执行文件。
		程序越复杂，静态类型检查越有助于发现问题。然而，前提是编译器必须知道每一个实体对象的类型，这就要求我们在使用某个变量之前必须声明其类型。

### 标识符

由字母、数字、下划线组成，	必须以字母或下划线开头。长度不限，大小写敏感。

c++语言的保留字不能被用作标识符。
		c++也为标准库保留了一些名字。用户自定义的标识符中不能连续出现两个下划线，也不能以下划线紧连大写字母开头。
		此外，定义在函数体外的标识符不能以下划线开头

		变量命名规范：
			· 标识符要能体现实际含义
			· 变量名一般用小写字母，如index，不要使用Index或INDEX
			· 用户自定义的类名一般以代谢字母开头，如Sales_item
			· 如果标识符由多个单词组成，则单词间应有明显区分，如student_loan或studentLoan，不要使用studentloan
	
			命名规范，若能坚持，必将有效

![d9e2054f123d6bf97a81d46eb4921175.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C1004:1)


### 名字的作用域

作用域（scope）是程序的一部分，在其中名字有其特定的含义。c++中大多数作用域都以花括号分隔。

同一名字在不同作用域下指向不同的实体。名字的有效区域为当前作用域。

main定义于所有花括号之外，它和其他大多数定义在函数体之外的名字一样拥有**全局作用域（global scope）**
一旦声明之后，全局作用域内的名字在整个文件的范围内都可使用。**局部变量拥有块作用域（block scope）**

	一般来说，在对象第一次被使用的地方附近定义它是一种好的选择，因为这样有助于更容易地找到变量的定义。
	更重要的是，当变量的定义与它的第一次被使用的地方很近时，我们也会给它赋值一个比较合理的初始值。




	内层作用域（inner scope）
	外层作用域（outer scope）
	在作用域内（in scope)
	
	如果某函数有可能用到某全局变量，则不宜再定义一个同名的局部变量。

## 2.3 复合类型

是基于其他类型定义的类型。c++ 语言有几种符合类型，比如指针和引用。

一条声明语句由一个基本数据类型（base type）和紧随其后的一个声明符（declarator)列表组成。每个声明符命名了一个变量并指定该变量为与基本数据类型有关的某种类型。

复合类型声明符含义：&取地址，*解引用，[]数组，()函数或名称分隔

```c++
[!] 【变量】 以可引用地址，不可寻址的 方式解释变量， 其本身可引用，可寻址
	使用变量实质上是使用其地址，并且从内存的地址上取值
	int a = 1;
	int* p = &a;
	int b = *a; //error

* 【解引用】以可寻址，可引用 的方式解释变量，其本身可引用，可寻址 比如
		int a = 1; //  使用a，实质上是使用地址取值
		int* p = &a;//  直接使用p， 实质上是使用p的地址取值，拿到a的地址
						// 要通过p得到对a取值，需要以从p的地址拿到p的值并以寻址的方式解释p的值（a的地址），
						// 寻到a的地址后，又会使用其地址取值

& 【引用】 以可引用，不可寻址地址的方式解释变量， 其本身不可引用，不可寻址
		int* p = &a; //a会直接对地址取值，而&a是使用其地址不取值
					//将a的地址值，赋给一个以可寻址的类型为其值

		int& ba = *p; // 声明到ba是一个可引用地址的变量，其值为一个解过引用的变量地址，
						//	即不可寻址的地址，直接取值。

		int& b = *new int;
		delete &b;
```

#### 2.3.1 引用

```& 引用（reference）```：为对象起了另外一个名字，引用类型对象引用（refers to）另外一个对象。

实质上，引用即封装了指针操作。其过程为，通过一个变量名找到地址，&取地址，将该地址赋予一个新
	的变量，相当于一个地址被多个指针指向，然后通过*解引用返回值。


```c++
		引用定义 ：type& ba = a;
		过程：声明可取址为type类型的变量ba，为其初始化为引用（&）a变量（[!])
		使用：ba
			表现为使用新变量([!])，通过寄存器变量映射到a变量地址
		图解：
								  eax
								   |
						= a		   ↓     
				     --------- {0xfabc}<-----
				     |	                     |
		 -------------|-----------------------|-----------------CPU---------------------------------
		 -------------|-----------------------|----------------Memory-------------------------------
            
		              |                       | type& ba
					 ↓  type* pA    	     |
			     0xfdef<----------type<-------------ba //fabc//【0xacac】
			     ↗	|              |				     |
			    /	↓              |                     |
			   pA {0xfabc}      0xfabc<-----a          {0xfdef} 【pA】
				|   ↓ 		     ↙|                  
				|    = 	   &a[fabc]↓
				------------------>1
				     *pA
		
	故指针的复合类型为：
		type* pA = &a;
	引用的复合类型为：
		type& BA = a;
```

引用并非对象，它只是一个已经存在的对象起的另外一个名字。（不可寻址因为名字本身相当于只是一个声明而不是定义，本身不可引用）。

引用必须绑定在一个已存在的对象上（初始化），初始化完成后，引用将和它的初始值对象一直绑定在一起。

引用只能绑定在对象上，而不能与字面值或者某个表达式的计算结果绑定在一起。

**实际应用中，引用一般作参数传递与返回值。（使用引用的效率实际上是和指针一样的，4bytes（32bit），8bytes（64bit））**

- 返回引用时，需要注意局部变量的生命周期

### 2.3.2 指针

指针本身是一个对象，允许指针赋值、拷贝，生命周期内可以指向不同的对象。指针无须在定义时赋初值，块作用域内不初始化会有不确定值。

指针可以在定义时初始化，但其将拥有一个不确定的值（野指针，空悬指针）。

编译器并不负责检查空悬指针（无效指针），试图拷贝或者以其他方式访问无效指针的值都将引发错误。
这一点和试图使用未经初始化的变量是一样的。（指针解释本变量可引用可寻址，但其值是否可引用，是否可寻址并不一定）访问无效指针的后果无法预计，因此程序员必须清楚任意给定的指针是否有效。

```c++
指针定义 : type* pA = &a;（*在左值左结合）
	过程：声明可寻址（*）到type类型的变量pA，为其初始化为引用（&）a变量的地址。
	使用：*pA，（pA{0xfdef}->*->a{1}）
		表现为使用新变量（[!]），对其值寻址（*），得到变量a([1])
	图解：

	-------------------------Memory---------------------------
			 type
			   |
			   ↓
		   {0xfdef}
			   |type* pA
			   ↓		          
	    pA---->0xfdef           a---> 0xfabc
			  ↙|					 ↙|
  &pA[0xfdef]	|  =         &a[0xfabc]|  
				↓↗                    ↓
		      {0xfabc}---------------->1
						  *pA
```

**指针传递本质上也是值传递，即指针是间接级，无法通过指针自身改变自身。当需要修改一个指针本身时，需要通过更高级间接级的指针**

#### 空指针

	不指向任何对象，在试图使用一个指针之前代码可以首先检查是否为空。
	实质上是在指针变量的值域放上一个0值。

```c++
int* p1 = nullptr;
int* p2 = 0;
int* p3 = NULL; //#include <cstdlib>
```


	新标准下，现在的c++程序最好使用nullptr，同时尽量避免使用NULL。
	advice： 初始化所有指针
		使用未经初始化的指针是引发运行时错误的一大原因。
		和其他变量一样，访问未经初始化的指针所引发的后果也是无法预计的。通常这一行为将造成程序崩溃，
	而且一旦崩溃，要想定位到出错位置将是特别棘手的问题。
		在大多数编译器环境下，如果使用了未经初始化的指针，则该指针所占内存空间的当前内容将被看做一个地址值。
	访问该指针，相当于去访问一个本不存在的位置上的本不存在的对象。（对不可寻址的值寻址）糟糕的是，如果指针所占内存空间中恰好有内容，而这些内容又被当做了某个地址，我们就很难分清它到底是合法还是非法了，
	
		因此建议初始化所有的指针，并且在可能得情况下，尽量等定义了对象之后再定义指向它的指针。如果实在不清楚指针
	应该指向何处，就把它出事啊为nullptr或者0，这样程序就能检测并指导它没有指向任何具体的对象了。

#### 指针的操作

拷贝、取值、赋值、合法的条件表达式运算、合法的逻辑判断（==，！=）

#### void* 指针

是一种泛型指针，可用于存放任意对象的地址。一个void* 指针能够做的事比较少：与别的指针比较、函数的输入输出、赋给另外一个void*指针。

不能直接操作void*指针指向的独享，因为我们并不知道这个对象到底是什么类型，在使用时需要转化成具体的类型。

在void*的视角来看内存空间也就是单纯的内存空间，没办法访问内存空间中存的对象数据（没有类型，无法解释数据）。

### 2.3.3 理解复合类型的声明

声明符是修饰符。

#### 指向指针的指针

	指向指针的指针
		一般来说，声明符中修饰符的个数并没有限制。当有多个修饰符连写在一起时，按照其逻辑关系详加解释即可。
		以指针为例，指针是内存中的内存，像其他对象一样也有自己的地址，因此允许把指针的地址再存放到另一个指针当中。
		通过*的个数可以区分指针的级别。也就是**表示指向指针的指针，***表示指向指针的指针的指针。
		此时为了取到某个层级的值，需要对应多次的解引用。

#### 引用指针的引用

对指针进行地址传递。


```c++
	引用本身不是一个对象，因此不能定义指向引用的指针，但指针是对象，所以存在对指针的引用。
	
void page52(void) {
	int i = 42;
	int* p;
	int*& r = p; //int*类型的引用。要理解r的类型，最简单的办法就是从右向左阅读r的定义。是个引用，引用指针。

	r = &i;
	std::cout << *r << std::endl;
}
```

对复合声明的复杂类型，从名字开始，从右向左，从内到外的阅读定义。

## 2.4 常量与const 限定符

关键字const 对变量的类型加以限定，使得变量的值不能被改变， 即声明为常量。

const 对象一旦创建后其值就不能再改变，所以const对象必须初始化。

在C语言中，常量除字面值外，常用宏来表示。（C语言的const变量，因为未对通过指针访问const修饰的变量做处理，尤其是通过强制类型转换后进行非常规访问，是可以修改其值的）。c++中通过

```c
#define ABC "abc"
```

c++中通过编译器对const变量做出了限制，是通过生成临时变量来实现的。

https://blog.csdn.net/k346k346/article/details/47960539

[https://blog.csdn.net/weixin_34194702/article/details/86291334?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~all~baidu_landing_v2~default-1-86291334.nonecase&utm_term=c++%20%E4%BC%A0%E5%8F%82%20%E5%B8%B8%E9%87%8F%E8%A1%A8](https://blog.csdn.net/weixin_34194702/article/details/86291334?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~all~baidu_landing_v2~default-1-86291334.nonecase&utm_term=c++ 传参 常量表)

https://blog.csdn.net/gsyzhu/article/details/8243230



### 初始化和const

只能在const类型的对象上执行不改变其内容的操作。

初始化是不改变const对象的操作中的一种，如果利用一个对象去初始化另外一个对象，则它们是不是const都无关紧要。

```c++
int i = 42;
const int ci = i; // 常量
int j = ci;
//无须在意ci是不是一个常量。拷贝一个对象的值并不会改变它，一旦拷贝完成，新的对象就和原来的对象没什么关系了。
```



### const常量的生命周期和作用域

默认状态下，const对象仅在文件内有效

对于一个const变量，编译器将在编译过程中把用到该变量的地方都替换成对应的值。也就是说，编译器会找到代码中所有用到该变量的地方，用const变量的值替换。

为了执行上述操作，编译器必须知道变量的初始值。**默认情况下，const对象被设定为仅在文件内有效**，多个文件中出现了同名的const变量时，等同于在不同的全局作用域中分别定义了独立的变量。（c++的编译单元是文件，但是如果重复定义的变量，在编译好的文件在进行链接时会出现重定义。）

当需要在多个文件中共享const变量，需要在一个文件中定义const变量，而在其他的多个文件中声明并使用它。解决方法是，对于第一个文件共享的const变量，不管是声明还是定义都添加extern关键字，这样只需定义一次就可以了。**多个文件之间共享const对象，必须在变量的定义之前添加extern关键字**

```c++
//file1.cc
extern const int bufSize = fcn(); // 定义
//file2.cc
extern const int bufSize; //引用到了file1.cc中的bufSize， 引用性声明

// file1.h头文件中的声明也由extern做了限定，其作用是指明bufSize并非本文件独有，它的定义将在别处。
//声明： 想声明一个变量而非定义它，就在变量名前添加extern，而且不要显式地初始化变量
// 声明可以多次，但是只能定义一次。（定义即确定内存模型）
```

### 2.4.1 const限定的引用（常量引用）

即以const的方式引用一个变量。（reference to const）。对常量的引用不能用作修改它所绑定的对象。

因为c++中const变量是真常量，将其看为字面值。所以对于一个const变量的引用也只能声明为const的。

引用的对象是常量还是变量可以决定其所能参与的操作，却无论如何都不会影响到引用和对象的绑定关系本身。

```c++
const int &i = 42;
const int c1 = 1024;
const int &r1 = c1;

r1=42;//error
int &r2 = c1;// 类型不匹配。const。
```

#### 初始化和常量引用

引用类型必须与所引用对象的类型一致，但有两个例外：

- 在初始化常量引用时允许用任意表达式作为初始值，只要表达式的结果能够被引用即可。尤其允许为一个常量引用绑定非常量的对象、字面值，甚至一个一般表达式。（**限定引用的访问权限，即顶层const**）

  ```c++
  double dval = 3.14;
  const int &ri = dval;
  //编译器的工作模式是如下
  const int temp = dval;
  const int &ri = temp;
  
  // 实际上ri绑定了一个临时量（temporary）对象。所谓临时量对象就是当编译器需要一个空间来暂存表达式的求值结果时临时创建的一个未命名的对象。c++程序员们常常把临时对象简称为临时量。
  ```

  常量引用仅对引用参与的操作做出了限定，对于引用的目标对象本身是不是一个常量未作限定。对象可能是非常量，所以允许通过其他途径改变它的值。

  ```c++
  int i = 42, & r1 = i, const& r2 = i;
  r1 = 43;
  cout << r2;
  ```

- 

### 2.4.2 指针和const（常量指针）

可以令指针指向常量或非常量，与常量引用类似。

指向常量的指针（pointer to const )不能用于改变其所指对象的值。要想存放常量对象的地址，只能使用指向常量的指针。

```c++
const double pi = 3.14;
double *ptr = &pi; // err
const double* ptr = &pi; 
```

指针的类型必须与其所指对象的类型一致，但是有两个例外：

- 允许令一个常量的指针指向一个非常量对象

  ```c
  double dval = 3.14;
  const double* ptr = &dval; 
  // 这和上述常量引用是一致的。
  ```

  这种是指针或引用自身对所引用对象访问权限的限制。

#### const指针（指针常量）

指针本身是常量属性，其存储的地址值和其类型信息不会改变。但其对所指向对象的访问权限是不受限的。常量必须进行初始化，指针常量也一样。

```c
int number = 0;
int * const pnum = &number;
```

##### 指向常量的const指针（常量指针常量）

本身被访问属性和访问属性都受限。

```c
const double i = 2.31;
const int * const pi = &i;
```

### 2.4.3 顶层const 和 底层const

顶层const（top-level const）表示常量对象，即对象本身的属性为const

底层const（low-level const）表示常量指针或引用。指针、引用的访问属性为const方式。

需要注意的是，引用不存在顶层const，因为引用本身不是对象，本身不可寻址没有实体，没有本身被访问属性。

```c++
constexpr int *np = nullptr; //顶层const，指向常量
int j = 0;
constexpr int i = 42;  //常量i
// i j必须在函数体外
constexpr const int *p = &i; //const*  底层const，constexpr底层const
constexpr int *p1 = &j; //顶层const，指向变量
```



### 2.4.4 constexpr 和 常量表达式

```常量表达式```：是指值不会改变并且在编译过程就能得到计算结果的表达式。

字面值术语常量表达式，常量表达式初始化的const对象也是常量表达式。

c++中有几种情况下是需要用到常量表达式的。（见后）

一个对象（或表达式）是不是常量表达式由它的数据类型和初始值共同决定。

```c++
const int max_files = 20;
const int limit = max_files + 1;
int statff_size = 27; //variable
const int sz = get_size(); // no, get_size()是函数调用，需要运行期堆栈运行后得到值。
```

#### constexpr 变量

c++11新标准规定，允许将变量声明为constexpr类型，**用以让编译器来验证变量的值是否是一个常量表达式**。声明为constexpr的变量一定是一个常量，而且必须用常量表达式初始化。

```c++
	constexpr int mf = 20;
	constexpr int limit = mf + 1;
	constexpr int sz = size();// 函数调用在常量表达式中必须具有常量值
```

一般来说，如果你认定变量是一个常量表达式，那就把它声明成constexpr类型。

`const`只确保值在初始化后不能更改不能确保在编译期确定，其中`constexpr`(在C++11中引入)确保变量是编译时间常数。

![813d6db5f6c63b6f9991a5843e4bb33f.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C1002:1)


![f3c83e1ebcae8eafcc229c87987217cc.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C1005:1)


![b2ba90562ca3ba91a28e6a7851516637.png](C:%5CUsers%5C52648%5COneDrive%5C%E6%96%87%E6%A1%A3%5Cen-resource:%5Cdatabase%5C1003:1)

https://cloud.tencent.com/developer/ask/37526

#### 字面值类型

对声明constexpr时用到的类型必须有所限制，这些类型一般简单而易得，称之为字面值类型（literal type）。

算数类型、引用和指针都属于字面值类型。自定义类、库中的类和模板类都不属于字面值类型，也就不能定义成constexpr。

算数类型都属于字面值类型。
		nullptr或者0的指针是constexpr指针，或者是存储于某个固定地址中的对象。
			（函数体内定义的变量一般来说并非存放在固定地址中，函数体外的对象（全局作用域）地址固定不变，能用来初始化constexpr指针）
			（函数体内定义的全局范围（static，extern）变量，有固定地址。）

#### 指针和constexpr

			（明确constexpr限定符修饰变量，还是指针）

**constexpr声明中如果定义了一个指针，限定符constexpr仅对指针有效**，与指针所指的对象无关。

```c++
const int *p = nullptr; // 底层const，指向整型常量的指针
constexpr int *q = nullptr; // 顶层const，constexpr修饰指针称为常量指针
// 类类型及其引用、指针不属于字面值类型。不能被定义成constexpr。
```

与其他常量指针基本一致。

```c++
constexpr int* constexpr q = nullptr; //err
constexpr int* const q = nullptr;  // right
```

### 2.4.5 const与#define

const定义的常量与#define定义的符号常量的区别

- const定义的常量有类型，而#define定义的没有类型，编译可以对前者进行类型安全检查，而后者仅仅只是简单的替换。
- const定义的常量在编译时分配内存，而#define定义的常量是在预编译时由预编译器进行替换，不分配内存。
- 作用域不同，const定义的常变量的作用域是local 变量的作用域。而#define定义的常量作用域为它的定一点到程序结束，当然也可以在某个地方使用#undef取消。

定义常量还可以使用enum，尽量用const、enum替换#define定义常量。

iniline可以替换宏函数。

在基础设施的编程中，#define的用于还是很灵活的。（#和##的预处理字符替换和连接）。

```c++
#define STR(a) #a
#define CAT(a,b) a##b

int main(void)
{
    int xy = 100;
    cout << STR(ABCD) << endl;
    cout << CAT(x,y) << endl;
    return 0;
}
```

- ```#define```定义的常量，容易产生副作用

  ```c++
  // Effective c++ 3rd的一个例子
  #define CALL_WITH_MAX(x,y) f((x) > (y) ? (x) : (y))
  
  int a = 5;
  int b = 0;
  CALL_WITH_MAX(++a, b);
  CALL_WITH_MAX(++a, b+ 10);
  
  // 在这里，调用f之前，a的递增次数竟然取决于“它被拿来和谁比较”
  ```

  

## 2.5 处理类型

程序的复杂性一方面来自于类型的复杂。

- 一些类型难于拼写，它们的名字难记又容易写错，且无法明确体现其真实的目的和含义
- 有时候根本搞不清到底需要的类型是什么，不得不回过头去从程序的上下文中寻求帮助

### 2.5.1 类型别名

类型别名（type alias）：是某种类型的同义词。

使用类型别名可以让复杂的名字变得简单明了，易于使用，有助于程序员清楚地知道使用该类型的真实目的。

#### 两种用于定义类型别名的方法

- typedef

  ```c++
  typedef double wages;
  typedef wages base, *p;//*p 是double* 的同义词
  ```

  		关键字typedef作为**声明语句中的基本数据类型的一部分**出现。含有typedef的声明语句定义的不再是变量而是类型别名。这里的声明符也可以包含类型修饰，从而能由基本数据类型构造出复合类型来。

- 别名声明（alias declaration）  using

  ```c++
  using SI = Sales_item;
  SI item;
  ```

  		这种方法用关键字using作为别名的开始，其后紧跟别名和等号，其作用是把等号左侧的名字规定成等号右侧类型的别名。
		
  		类型别名和类型的名字等价，只要是类型的名字能出现的地方，就能使用类型别名。

#### 指针、常量和类型别名

如果某个类型别名指代的是复合类型或常量，那么把它用到声明语句里就会产生意想不到到的后果。

使用了类型别名的声明语句，人们会错误的将原来的类型替换进别名语句来理解。这是错误的。

```c++
typedef char* pstring;
const pstring cstr = 0; // cstr 指向char 的常量指针。
		// 不等同于 const char* pstring;（一个以只读权限指向char类型的指针，本身可写）
const pstring *ps; // ps 是一个二级指针，存储了一个指向char类型的常量指针。

// const pstring 是声明语句中的基本数据类型。
// 如果简单替换后，const char *cstr = 0; 基本数据成为const char，*成为了声明符的一部分，说明声明了一个const char类型的指针。
// 也就是说，typedef之后，会将复合类型说明符当做基本数据类型的一部分。using也一样。
```

### 2.5.2 auto 类型说明符

c++11新标准引入auto类型说明符，让编译通过初始值来推算变量的类型。因此，auto定义的变量必须有初始值。

使用auto也能在一条语句中声明多个变量。因为一条声明语句智能有一个基本数据类型。

```c++
Sales_item val1, val2;
auto item = val1 + val2;
std::cout << item << std::endl;

auto i = 0u, *p = &i;
```

#### 复合类型、常量和auto

编译器推断出的auto类型有时候和初始值的类型并不完全一样，编译器会适当地改变结果类型使其更符合初始化规则。

```c++
int i = 0, &r = i;
const int ci = i, & cr = ci;
```

- 引用的auto以引用对象的类型为准

  ```c++
  /*
  	使用引用其实是使用引用的对象，特别是当引用被用作初始值时，真正参与初始化的其实是引用对象的值。
  	此时编译器以引用对象的类型作为auto的类型。
  */
  auto a = r;
  std::cout << typeid(a).name(); // int
  ```

- auto会忽略掉顶层const，同时底层const则会保留下来

  ```c++
  /*
  	auto一般会忽略掉顶层const，同时底层const则会保留下来
  */
  auto b = ci;
  std::cout << typeid(b).name()<<std::endl;//int, ci的顶层const特性被忽略掉了
  			// 对于ci来说，是对i进行了拷贝，并对自身的内存限制了被访问权限
  		// 以auto推断也是一样，既然创建了新的内存空间，内存空间的访问权限就使用默认权限而不沿用推断
  
  auto c = cr;
  std::cout << typeid(c).name() << std::endl;//int， 兼具上一条和 引用的推断
  
  auto d = &i;
  std::cout << typeid(d).name() << std::endl;//int *， 对i取地址得到一个整数的指针
  
  auto e = &ci;
  std::cout << typeid(e).name() << std::endl;//const int *，ci的内存地址是受限访问的，推导出一个对该内存的间接访问，当然这种访问方式的底层访问权限也必须是受限的。
  ```

- 如果希望推断得到一个顶层const，需要附加指出

  ```c++
  const auto f = ci;
  std::cout << typeid(f).name() << std::endl; // const int
  
  const auto f = &ci;	// const int * const
  std::cout << typeid(f).name() << std::endl; // 这个破玩意儿也会把顶层const去掉。。。int const *
  ```

- 希望推导得到一个引用，需要附加指出。

  ```c++
  /*
  	设置一个类型为auto的引用时，初始值中的顶层常量属性仍然保留到引用中，
  	如果我们给初始值绑定一个引用，则此时的常量就不是顶层常量了，而是底层常量，所以引用中保留的是底层const。
  		（第二句解释， 就是比如g给个ci绑定了一个引用，这时ci的引用关系上就有了一个来自顶层的引用g，所以ci就不再是独立的顶层常量了，故不违背一般去掉顶层const的说法。引用也没有顶层const。
  		也就是通过底层的被访问权限受限推导到顶层的访问权限受限。）
  */
  
  auto& g = ci; //const int& 
  std::cout << typeid(g).name() << std::endl;// int
  
  //auto& h = 42; error， 不能为非常量引用绑定字面值
  const auto& h = 42;//const int& 
  std::cout << typeid(h).name() << std::endl;//int
  ```

- 同一条语句中定义多个变量的推导

  ```c++
  /*
          在同一条语句中定义多个变量，切记，符号&和*值从属于某个声明符，
          而非基本数据类型的一部分，因此初始值必须是同一种类型
      */
  auto k = ci, & l = i;
  auto& m = ci, * p = &ci;
  auto& n = i /*, * p2 = &ci*/; //ci是const int
  const auto* p2 = &ci;
  ```

### 2.5.3 decltype 类型指示符

c++11新保准引入第二种类型说明符decltype， 选择并返回操作数的数据类型。

再此过程中，编译器分析表达式并得到它的类型，却不实际计算表达式的值。

```c++
decltype(f()) sum = x;
// 编译器不实际调用f()，而是从f()的函数签名中推导返回值类型作为sum的类型。
```

**decltype处理顶层const和引用的方式与auto有些不同**：

- 如果decltype使用的表达式是一个变量，则decltype返回该变量的类型（包括顶层const和引用在内）

  ```c++
  void page63(const Sales_item si, const Sales_item& ss) 
  {
      decltype(si) x("xxxxxxxxx"); // const Sales_item x
  	decltype(ss) y{ x }; // const Sales_item &y
      		//ss是个引用，decltype(ss)返回的结果就是引用类型，因此作为引用的y必须初始化；
  	// 引用从来都作为其所指对象的同义词出现，只有用在decltype处是一个例外。（ss得到一个引用类型)
  }
  ```

- decltype 和 引用

  ```c++
  /*
  	如果decltype使用的表达式不是一个变量，则decltype返回表达式结果对应的类型。
  	有些表达式将向decltype返回一个引用类型。一般意味着该表达式的结果对象能作为一条赋值语句的左值。
  
  	如果表达式的内容是解引用操作，则decltype将得到引用类型。
  	解引用指针可以得到指针所指的对象，而且还能给这个对象赋值，因此decltype（*z）的类型是引用类型
  */
  void page63(const Sales_item si, const Sales_item& ss) 
  {
      decltype(si) x("xxxxxxxxx");
  	decltype(ss) y{ x };
      
      const decltype(ss + si) *z;//const Sales_item *z
      //ss + si使整体结果是一个具体的对象而非一个引用,而且得到的新的z也不再是const，因为相加之后，已经得到了一个具有新值的对象，而非原来某个对象的常值
      
      decltype(*z) a{ y }; // /const Sales_item &a
      //x也可以，变量不影响给常量赋初值
  }
  ```

- decltype的结果类型与表达式形式密切相关

  ```c++
  void page63(const Sales_item si, const Sales_item& ss) 
  {
      decltype(*z) a{ y };//const Sales_item &a
      
  /*
  	前提： 有些表达式将向decltype返回一个引用类型。一般意味着该表达式的结果对象能作为一条赋值语句的左值。
  */
      
  /*
      decltype的另外一个与auto的重要区别是：
         decltype的结果类型与表达式形式密切相关。对于decltype表达式来说，如果变量名加上了一对括号，则得到的类型与不加括号会有不同。
  */
      
  /*
  	结论：
         不加括号得到的就是该变量的类型，加上括号，相当于进行了运算.
         而且因为其是变量，变量是一种可以作为赋值语句左值的特殊表达式，所以这样的decltype就会得到引用类型
  */
      decltype((si)) b{ a };//const Sales_item &b
      decltype(si) c;// const Sales_item c , 如果是内置类型，未初始化，const要求初始化，这里默认初始化。
      int i = 0;
      decltype(i) d; // int d
      decltype((i)) e{ d }; //int &e, 这里加const为啥会失效？？？
  }
  ```

  	注意：decltype((variable)) 的结果永远是引用，而decltype(varaible)结果只有当variable本身就是一个引用时才是引用。

  ```c++
  void page63() 
  {
  	//解释上文  变量作为赋值语句参与表达式得到引用
  	int in1 = 3, in2 = 4;
  	decltype(in1) in3 = in1;
  	decltype(in1 = in2) in4 = in1;// in1 = in2 表达式生存期结束，产生匿名对象引用该表达式结果的地址，从而产生引用类型
  }
  ```

## 2.6 自定义数据结构

数据结构是把一组相关的数据元素组织起来然后使用它们的策略和方法。

c++允许用户以类的形式自定义数据类型。

### 2.6.1 定义Sales_data类型

#### 结构体

struct 结构体（也是一种类）
	一般以数据的组织为目的，使用struct。默认为public。
	而以实现围绕数据的功能为目的,使用class。默认为private

- 类内初始值

  c++11规定，可以为护具成员提供一个类内初始值。没有初始值的成员被默认初始化。

  可以放在赋值语句中或者{}初始化列表中，但是不能使用()圆括号。如bookNo使用圆括号报错。

```c++
struct Sales_data {
	std::string bookNo{ "sssss" };
	unsigned uints_sold = 0; //c++11规定，可以为数据成员提供一个类内初始值（in-class Initializer）
	double revenue = 0.0;//类内初始值将用于初始化数据成员。没有初始值的成员将被默认初始化。
	/*
		类内初始值的限制和之前2.2.1介绍的类似，
		
	*/

	Sales_data() {};
} accum, * salesprtr, & ref{ accum };/*  */
//必须写封号，因为类体后面可以紧跟变量名以示定义该类型的对象.但是一般会分开。
```

#### 结构体内存对齐

编译器为每个“数据单元”安排在某个合适的位置上。

c++/c 语言非常灵活，允许语言的用户干涉“内存对齐“。

- 为什么要对齐

  性能原因：在对齐的地址上访问数据快。与CPU数据总线的寻址宽度与周期有关。

  比如一个非对齐的数据，CPU会从默认的以对齐的寻址方式，从前一个数据块中取出部分数据，然后再从后一个数据块中取出剩下的数据，然后进行移位拼装成一个完整的数据块。

- 对齐的规则

  第一个成员与结构体变量的地址偏移为0。

  ```c++
  #include <iostream>
  
  struct test {
  	char a;
  	double b;
  };
  
  int main() {
      //std::cout << "Hello, World!" << std::endl;
  	std::cout << sizeof(test) << std::endl;
  
  	test t;
  	printf("%p\n", &t);
  	printf("%p\n", &t.a);
      return 0;
  }
  ```

  其他成员要对齐到对齐数的整数倍的地址

  - **对齐数**，取编译器预设的一个对其整数与该结构体成员大小的较小值。
  - vs默认为8，g++默认为4

  结构体总大小为为最大对齐数的整数倍，g++只能取1,2，4

  ```c++
  #include <iostream>
  
  struct test {
  	char a;
  	double  b;
  	char c;
  };
  
  int main() {
      //std::cout << "Hello, World!" << std::endl;
  	std::cout << sizeof(test) << std::endl;
  
  	test t;
      return 0;
  }
   
  ```

  

  因为上述对齐原因，成员的排布会对结构体的大小有影响

  ```c++
  #include <iostream>
  
  struct test {
  	char a;
      char c;
  	double  b;
  };
  
  int main() {
      //std::cout << "Hello, World!" << std::endl;
  	std::cout << sizeof(test) << std::endl;
  
  	test t;
      return 0;
  }
  ```

  可以通过预编译指令来调整编译器的预设的对齐整数

  ```c++
  #include <iostream>
  
  #pragma pack(4)
  struct test {
  	char a;
  	double  b;
  	char c;
  };
  #pragma pack()
  
  int main() {
      //std::cout << "Hello, World!" << std::endl;
  	std::cout << sizeof(test) << std::endl;
  
  	test t;
      return 0;
  }
  ```

### 2.6.2 使用Sales_data类

各类本身没有提供任何操作，对于该类的输入、输出、运算等功能，需要手工编码实现。

假设该类定义与头文件中。

```c++
#include "Sales_data.h"

#include <iostream>
#include <string>

void page66(void) {
	Sales_data data1, data2;
	double price = 0.0;
	std::cin >> data1.bookNo >> data1.uints_sold >> price;
	data1.revenue = data1.uints_sold * price;

	std::cin >> data2.bookNo >> data2.uints_sold >> price;
	data2.revenue = data2.uints_sold * price;

	if (data1.bookNo == data2.bookNo) {
		unsigned totalCnt = data1.uints_sold + data2.uints_sold;
		double totalRevenue = data1.revenue + data2.revenue;
		std::cout << data1.bookNo << " " << totalCnt << " " << totalRevenue << " ";
		if (totalCnt != 0)
			std::cout << totalRevenue / totalCnt << std::endl;
		else
			std::cout << "(no sales)" << std::endl;
		return;
	}
	else {
		std::cerr << "Data must refer to the same ISBN" << std::endl;
	}
	return;
}
 
```

### 2.6.3 编写自己的头文件

尽管可以在函数体内定义类，但这样的类受到了很多限制。

类一般不定义在函数体内，函数体外定义类时，各个指定的源文件中可能只有一处该类的定义，在不同文件中使用同一个类，类的定义就必须保持一致。

所以，为了确保各个文件中类的定义一致，类通常被定义在头文件中，而且类所在头文件的名字应与类的名字一样。如果同一个文件中有多个类，该文件应有一个主要的功能和对外提供的类，以此命名文件。

**头文件通常包含那些只能被定义一次的实体，如类，const 和 constexpr变量等。**

头文件也常用到其他头文件的功能，为了保证多次包含的情况下也能安全正常工作，需要进行一些特殊的头文件处理。

		但是头文件定义时，可能需要引用其他对象的定义，这样在头文件被引用时，可能隐式的引用
	了其他头文件，如果再显式地引用相同的头文件。会造成依赖冲突。
		所以必须在书写头文件时做防卫性声明。
			#ifndef MASSPP_OSTYPE_H
			#define MASSPP_OSTYPE_H
			...
			#endif
		这样使得多次包含时，相同变量只被定义一次。

**头文件一旦改变，相关的源文件必须重新编译以获取更新过的声明**

#### 预处理器概述

防卫性声明通过预处理指令控制宏来保证头文件多次包含后安全工作。

预处理器（preprocessor）是从C语言继承过来的。

预处理器是在编译之前执行的一段程序，可以部分的改变我们所写的程序。

```c++
#include
	是一项预处理功能。用指定的头文件内容替换#include指令

#define、#ifndef、#endif
        组合起来用于检查某个指定的预处理变量是否已经定义。来达到头文件防卫性声明的目的。
/*
	第一次包含头文件是，#ifndef的结果为真，预处理器将顺序执行后面的操作直至遇到#endif为止。
此时，预处理宏的值将被#define定义。头文件被拷贝到.cpp的实现文件中。再多次包含头文件时，
#ifndef的检查结果将为假。编译器将忽略#ifndef到#endif之间的部分。
*/


```

预处理宏无视c++语言中关于作用域的规则，由预处理器管理。

整个程序中的预处理宏包括头文件防卫性声明必须唯一。通常的做法是基于头文件中类的名字（即文件名）
来构建保护符的名字，以确保其唯一性。（Google规范，项目名+路径+文件）。为了避免与程序中的其他实体发生名字冲突，一般把预处理变量的名字全部大写。

头文件即使（目前还）没有被包含在任何其他头文件中，也应该设置保护符。头文件保护符很简单，程序员只要习惯性地加上就可以了，没必要太在乎程序需不需要。



## 自动类型推导 auto



## 小结

	类型系统是编程语言的基础。
	类型规定了其对象的存储要求和所能执行的操作。c++语言提供了一套基础内置类型，如int和char等，
	这些类型与实现它们的机器硬件密切相关。此外，还可以定义复合类型，如指针和引用等。复合类型的定义以
	其他类型为基础。
	c++语言允许用户以类的形式自定义类型。c++库通过类提供了一套高级抽象类型，如输入输出和string等。

## 术语