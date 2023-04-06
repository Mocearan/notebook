# 基于流的I/O库

---

​		c++包含以 I/O流库的形式 提供了文本和数值的格式化和非格式化缓冲I/O。它是可扩展的，可以完全像内置类型和类型安全一样支持用户定义的类型。

​		文件系统库提供了操作文件和目录的基本工具。

## I/O的基本概念

### 缓冲

​		A **buffer** (also called a data buffer) is simply a piece of memory set aside for storing data temporarily while it’s moved from one place to another. 

​		未缓冲的输出通常立即处理，而缓冲的输出通常作为块存储和写入。

​		

### 流

​		Abstractly, a **stream** is just a sequence of bytes that can be accessed sequentially. Over time, a stream may produce or consume potentially unlimited amounts of data.

> “流”想要表达的是，随着时间的推移，字符是顺序生成或消耗的。*

​		Typically we deal with two different types of streams：

- **Input streams** are used to hold input from a data producer, such as a keyboard, a file, or a network. 
- **output streams** are used to hold output for a particular data consumer, such as a monitor, a file, or a printer. 

​		当用户按下键盘上的一个键时，键代码就被放入输入流中。然后，程序从流中提取值，以便使用它。您将值插入到流中，数据消费者(例如监视器)使用它们。

​		使用格式化的I/O操作通常比逐个操作字符更不容易出错，效率更高，代码也更少。特别是，istreams负责内存管理和范围检查。我们可以使用stringstreams或memory streams对内存进行格式化。

- ostream将类型化对象转换为字符流(字节):
  - obj -> ostream -> stream buffer -> byte sequences
- istream将字符流(字节)转换为类型化对象:
  - byte sequences -> stream buffer -> ostream -> obj

### std::getline

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

​	默认情况下，一个空白字符，如空格或换行符，会终止读取,可以使用getline()函数读取整行。

### 类型敏感

​		字符常量是用单引号括起来的字符。请注意，字符是作为字符而不是作为数值输出的。

```c++
int b = 'b';         //  note: char implicitly converted to int
char c = 'c';
cout << 'a' << b << c;

// a98c
```



## IO state

​		iostream有一个状态，我们可以检查它来确定操作是否成功。最常见的用途是读取一个值序列:

```c++
vector<int> read_ints(istream& is)
{
    vector<int> res;
    for (int i; is>>i; )
         res.push_back(i);
    return res;
}
```

​		一般来说，I/O状态保存了读或写所需的所有信息，例如格式化信息，错误状态，以及使用哪种类型的缓冲。特别是，用户可以设置状态来反映错误发生，并在错误不严重时清除状态。

```c++
vector<int> read_ints(istream& is, const string& terminator)
{
     vector<int> res;
     for (int i; is >> i; )
      	res.push_back(i);

     if (is.eof())                            // fine: end of file
		return res;
     if (is.fail()) {                         // we failed to read an int; was it the terminator?
        is.clear();          // reset the state to good()
        string s;
        if (is>>s && s==terminator)
            return res;
        is.setstate(ios_base::failbit);           // add fail() to is's state
     }
     return res;
}

auto v = read_ints(cin,"stop");
```



## 类层次图

​		按照流的类型，`iostream`库包含两个基础类型，``istream``和``ostream``，分别表示输入流和输出流。

​		I/O流类都有析构函数，释放所有拥有的资源(比如缓冲区和文件句柄)。也就是说，它们是“资源获取即初始化”(RAII

> 一个流就是一个字符序列，是从IO设备读出或写入IO设备的。

![image-20221209104100446](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20221209104100446.png)

- `basic_ios`类虽然不是最基类，但是`basic_ios`是直接使用的最基类
- `basic_ios`定义了一组公用的输入输出流设施
- `basic_istream` 是用于处理输入流的主要类.
  -  在输入流上，``operator>>`` 用来从流中提取出数据 
- `basic_ostream` 是用于处理输出流的主要类
  - 在输出流上，`operator<<`用来向流中添加数据。

## 预定义标准流对象

### std::cin / wcin

> 键盘输入特殊字符
>
> 	从键盘读取数据时，对于如何指出文件结束，不同操作系统有不同的约定。在Windows中，输入
> 	文件结束符的方法是Ctrl+Z。Unix系统中，包括MacOS系统中，文件结束符输入时Ctrl+D

### cout / wcout

### cerr / wcerr

​		提供无缓冲的输出方式。

### clog / wclog

​		提供缓冲的输出方式。

​		因为clog不经常使用，它经常从标准流列表中被省略。

## I/O 操纵符

​		i/O操纵符是一个可以用来对流输入输出进行限定和修改的对象。

`<iomanip>`

---

### std::setw

​		限制流操作的字符最大数量。

```c++
#include <iomanip>
char buf[10]{};
std::cin >> std::setw(10) >> buf; // 只会从流中读出9个字符，因为会给terminator留下一个字符，'\0'
```



### std::endl

​		Using std::endl can be a bit inefficient, as it actually does two jobs: it moves the cursor to the next line, and it “flushes” the output (makes sure that it shows up on the screen immediately). 

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



