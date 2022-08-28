# 基于流的I/O库

---

​		c++语言并未定义任何输入输出（IO）语句，取而代之，包含了一个全面的标准库来提供IO机制。
（以及其他很多设施）。对于很多用途，我们只需要了解IO库中一部分基本概念和操作。	

​		`iostream`库包含两个基础类型，``istream``和``ostream``，分别表示输入流和输出流。一个流就是一个字符序列，是从IO设备读出或写入IO设备的。

> **“流”想要表达的是，随着时间的推移，字符是顺序生成或消耗的。** 
>
> 
>
> **键盘输入特殊字符**
>
> 从键盘读取数据时，对于如何指出文件结束，不同操作系统有不同的约定。在Windows中，输入
> 	文件结束符的方法是Ctrl+Z。Unix系统中，包括MacOS系统中，文件结束符输入时Ctrl+D

```c++
#include <iostream>

// rest of code that uses iostream functionality here
```



## I/O 操作符



## std::cin



### std::endl

Using std::endl can be a bit inefficient, as it actually does two jobs: it moves the cursor to the next line, and it “flushes” the output (makes sure that it shows up on the screen immediately). 

```c++
 /**
   *  @brief  Write a newline and flush the stream.
   *
   *  This manipulator is often mistakenly used when a simple newline is
   *  desired, leading to poor buffering performance.
  */

template<typename _CharT, typename _Traits>
inline basic_ostream<_CharT, _Traits>&
endl(basic_ostream<_CharT, _Traits>& __os)
{ return flush(__os.put(__os.widen('\n'))); }
```



## 缓冲

​		A **buffer** (also called a data buffer) is simply a piece of memory set aside for storing data temporarily while it’s moved from one place to another. 

## 流



## 类型



## 错误类别接口



## 预定义标准流对象

​		标准库定义了4个IO对象。

**`cin`** ： `istream`类型的对象，处理输入，这个对象也被成为标准输入。

> reads input from keyboard using the **extraction operator**

**`cout`** ：`ostream`类型的对象，为标准输出。

> allows us to send data to the console to be printed as text。
>
> *cout* stands for “character output”.

**`cerr`**：`ostream`对象，输出警告和错误消息， 标准错误。

**`clog`** ：`ostream`对象，我们通常用`cer`r来，输出程序运行时的一般性信息



We can override the default precision that std::cout shows by using an `output manipulator` function named `std::setprecision()`. **Output manipulators** alter how data is output, and are defined in the *iomanip* header.

```c++
#include <iostream>
#include <iomanip> // for output manipulator std::setprecision()

int main()
{
    std::cout << std::setprecision(16); // show 16 digits of precision
    std::cout << 3.33333333333333333333333333333333333333f <<'\n'; // f suffix means float
    std::cout << 3.33333333333333333333333333333333333333 << '\n'; // no suffix means double

    return 0;
}
```



## std::getline

```c++
#include <string> // For std::string and std::getline
#include <iostream>

int main()
{
    std::cout << "Enter your full name: ";
    std::string name{};
    std::getline(std::cin >> std::ws, name); // read a full line of text into name

    std::cout << "Enter your age: ";
    std::string age{};
    std::getline(std::cin >> std::ws, age); // read a full line of text into age

    std::cout << "Your name is " << name << " and your age is " << age << '\n';

    return 0;
}
```

If using `std::getline` to read strings, use the `std::ws` `input manipulator` to ignore leading whitespace.
