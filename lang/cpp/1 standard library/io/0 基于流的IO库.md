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

​		I/O流类都有析构函数，释放所有拥有的资源(比如缓冲区和文件句柄)。也就是说，它们是“资源获取即初始化”（RAII）

> 一个流就是一个字符序列，是从IO设备读出或写入IO设备的。

![image-20221209104100446](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20221209104100446.png)

- `basic_ios`类虽然不是最基类，但是`basic_ios`是直接使用的最基类
- `basic_ios`定义了一组公用的输入输出流设施
- `basic_istream` 是用于处理输入流的主要类.
  -  在输入流上，``operator>>`` 用来从流中提取出数据 
- `basic_ostream` 是用于处理输出流的主要类
  - 在输出流上，`operator<<`用来向流中添加数据。



## 流 stream

​		标准库直接支持：

- 标准流:附加到系统标准I/O流的流
- 文件流:附加到文件的流
- 字符串流:附加到字符串的流
- 内存流:连接到特定内存区域的流
- 同步流:可以从多个线程使用的流，没有数据竞争

​		可以定义自己的流，例如，附加到通信通道。

​		流不能被复制，始终通过引用传递它们。

​		所有标准库流都是以字符类型为参数的模板。例如，``ostream为basic_ostream<char>``。对于每一个这样的流，标准库也为``wchar_t``提供了一个版本。例如，``wostream``为``basic_ostream<wchar_t>``。

​		宽字符流可用于``unicode``字符。

### 标准流

- `std::cin / wcin`

  ​	`istream`类型的对象，处理输入，这个对象也被成为标准输入。

  > reads input from keyboard using the **extraction operator**

  从键盘读取数据时，对于如何指出文件结束，不同操作系统有不同的约定。在Windows中，输入
  文件结束符的方法是Ctrl+Z。Unix系统中，包括MacOS系统中，文件结束符输入时Ctrl+D

- `cout / wcout``

  ​	`ostream`类型的对象，为标准输出。

  ​	allows us to send data to the console to be printed as text。

  ​	*cout* stands for “character output”.

- `cerr / wcerr`

  ​	`ostream`对象，输出警告和错误消息， 标准错误。

  ​	提供无缓冲的输出方式。

- `clog / wclog`

  ​	`ostream`对象，我们通常用`cer`r来，输出程序运行时的一般性信息。提供缓冲的输出方式。、

  ​	因为clog不经常使用，它经常从标准流列表中被省略。



### 文件流

​	`<fstream>`提供了读写文件的流:

- `ifstreams`用于从文件中读取
- `ofstreams`用于写入文件
- `ftream` 用于读取和写入文件

​		

```c++
// 测试文件流是否已正确打开通常是通过检查其状态来完成的。

ofstream ofs {"target"};                   // “o” for “output”
if (!ofs)
 	error("couldn't open 'target' for writing");

ifstream ifs {"source"};                     // “i” for “input”
if (!ifs)
         error("couldn't open 'source' for reading");
```

​		成功打开后，``ofs``像普通的``ostream``一样（`cout`），``ifs``像普通的``istream``(``cin``)。

- 提供文件定位和文件打开方式的详细控制
- 提供关于文件名组成和文件系统的操作

### 字符串流

​		`<sstream>`提供读写字符串的流:

- `istringstreams` 用于从字符串中读取
- `ostringstreams` 用于写入字符串
- 用于读写字符串的``stringstreams``

```c++
ostringstream oss;
oss << "{temperature," << scientific << 123.4567890 << "}";
cout << oss.view() << '\n';
```

​		`ostringstream`的内容可以使用``str() / view()`` 来获取`std::string`拷贝或`std::string_view`视图。

​		``ostringstream/istringstream``的一个常见用途是在将在字符串和GUI之间进行格式化。

​		`stringstream`既可以用于读，也可以用于写。

> ​		例如，定义一个操作，它可以将任何具有字符串表示形式的类型转换为另一种也可以表示为字符串的类型:
>
> ```c++
> template<typename Target =string, typename Source = string>
> Target to(Source arg)                // convert Source to Target
> {
>     stringstream buf;
>     Target result;
> 
>     if (!(buf << arg)                                // write arg into stream
>          || !(buf >> result)                      // read result from stream
>          || !(buf >> std::ws).eof())         // is anything left in stream?
>          throw runtime_error{"to<>() failed"};
> 
>     return result;
> }
> // 函数模板参数只有在无法推导或没有默认值时才需要显式地提到，因此我们可以这样写:
> auto x1 = to<string,double>(1.2); // very explicit (and verbose)
> auto x2 = to<string>(1.2);  // Source is deduced to double
> auto x3 = to<>(1.2);   // Target is defaulted to string; Source is deduced to double
> auto x4 = to(1.2); // the <> is redundant;
>                    // Target is defaulted to string; Source is deduced to double
> ```
>
> ​		如果所有函数模板参数都是默认的，<>可以省略。

### 内存流

​		从c++的早期开始，就有了附加到用户指定的内存区域的流，这样我们就可以直接读写它们。

- 最古老的``strstream``已经被弃用了几十年
- 替代品``spanstream``、``ispanstream``和``osspanstream``要在c++ 23之后才会正式加入标准
- 然而，它们已经被广泛使用;尝试你的实现或搜索GitHub

​		他们的行为类似于字符串流的使用，只是它们以``span``而不是字符串作为参数。

```c++
array<char,128> buf;
ospanstream ss(buf);
ss << "write " << arg << " to memory\n";
```

​		出目标缓冲区溢出，流状态将被设置为失败。

### 同步流

​		在多线程系统中，I/O会变得混乱，需要保证：

- 一个流始终只被一个线程使用
- 对流的访问是同步的，因此一次只有一个线程获得访问权

​		`osyncstream`保证输出操作序列将与预期缓冲区的结果一致，即使有其他线程尝试写入。

```c++
osyncstream oss(cout);
// thread 1
oss << x;
// thread 2
oss << s;
```

​		混用``osyncstreams``和``cout``，线程间可能会产生干扰。

## 用户自定义类型的IO

```c++
struct Entry {
     string name;
     int number;
};

ostream& operator<<(ostream& os, const Entry& e)
{
   return os << "{\"" << e.name << "\", " << e.number << "}";
}

istream &operator>>(istream &is, Entry &e)
         // read { "name" , number } pair. Note: formatted with { " " , and }
{
    char c, c2;
    if ( is >> c && c == '{' && is >> c2 && c2 == "\"" ) { // start with a { followed by a "
        string name;                                // the default value of a string is the empty string: ""
        while ( is.get(c) && c != " )           // anything before a " is part of the name
                 name += c;

        if ( is >> c && c == ',' ) {
            int number = 0;
            if ( is >> number >> c && c == '}' ) { // read the number and a }
                e = { name,number };            // assign to the entry
                return is;
            }
        }
    }

    is.setstate(ios_base::failbit);            // register the failure in the stream
    return is;
}
```



## 格式化IO

​		`iostream`和``format``库提供了用于控制输入和输出格式的操作。输出格式也提供了对``unicode``的支持

- `iostream`是老式c++格式化工具，主要用于格式化数字流。
- `format`是最新的格式化工具，重点是``printf()``风格规范的值组合格式。

​		

### I/O 操纵符

​		最简单的格式化控件称为操纵符，可以在``<ios>``、``<istream>``、``<ostream>``和``<iomanip>``(用于接受参数的操纵符)中找到。

- 将整数输出为十进制(默认值)、八进制或十六进制:

  ```c++
  cout << 1234 <<' ' << hex << 1234 <<' ' << oct << 1234 << dec << 1234 <<'\n';  // 1234 4d2 2322 1234
  ```

- 显式地设置浮点数的输出格式

  ```c++
  constexpr double d = 123.456;
  
  cout << d << "; "                                   // use the default format for d, 123.456; 
          << scientific << d << "; "             // use 1.123e2 style format for d, 1.234560e+002; 
          << hexfloat << d << "; "               // use hexadecimal notation for d, 0x1.edd2f2p+6; 
          << fixed << d << "; "                    // use 123.456 style format for d, 123.456000; 
          << defaultfloat << d <<'\n';         // use the default format for d, 123.456
  ```

- Precision是一个整数，它决定用于显示浮点数的位数

  - 通用格式(defaultfloat)，允许实现选择一种最能在可用空间中保留值样式的格式表示值。精度指定最大位数。
  - 科学格式(scientific)。表示小数点前有一位数字和指数的值。精度指定小数点后的最大位数。
  - 固定格式(fixed)，将值表示为一个整数部分，后面跟着一个小数点和一个小数部分。精度指定小数点后的最大位数

  ```c++
  // 浮点值被四舍五入，而不仅仅是截断，而且precision()不会影响整型输出
  cout.precision(8);
  cout << "precision(8):" << 1234.56789 <<' '<< 1234.56789 <<' '<< 123456 <<'\n'; // precision(8): 1234.5679 1234.5679 123456
  
  cout.precision(4);
  cout << "precision(4): " << 1234.56789 <<' '<< 1234.56789 <<' '<< 123456 <<'\n'; // precision(4): 1235 1235 123456
  cout << 1234.56789 <<'\n'; // 1235
  ```

- 浮点操作符是“粘性的(sticky)”；

  - 也就是说，它们的影响将持续到后续的浮点操作。
  - 也就是说，它们主要是为格式化值流而设计的。

- 还可以指定数字要放入的字段的大小及其在该字段中的对齐方式。

- 除了基本的数字，``<<``还可以处理时间和日期:``duration、time_point year_month_date、weekday、month和zoned_time``

  ```c++
  cout << "birthday: " << November/28/2021 << '\n'; // birthday: 2021-11-28
  cout << << "zt: " << zoned_time{current_zone(), system_clock::now()} <<'\n'; // zt: 2021-12-05 11:03:13.5945638 EST
  ```

- 该标准还为`complex`、``bitset``、错误码和指针定义了``<<``。流I/O是可扩展的，可以为用户定义的类型定义``<<``



#### ``std::setprecision()``

​		覆盖``std::cout``当前的显示精度。**输出操纵符**改变数据输出的方式，在``iomanip``报头中定义。

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



#### std::setw

​		限制流操作的字符最大数量。

```c++
#include <iomanip>
char buf[10]{};
std::cin >> std::setw(10) >> buf; // 只会从流中读出9个字符，因为会给terminator留下一个字符，'\0'
```



#### std::endl

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



### `printf` 风格格式化

​		printf()一直是c++标准库的一部分，但它缺乏类型安全性和处理用户定义类型的可扩展性。

​		在``<format>``中，标准库提供了类型安全但不可扩展的``printf()``样式格式化机制。

- 基本函数``format()``生成一个字符串:

  ```c++
  string s = format("Hello, {}\n", val);
  cout << format("Hello, {}\n", val);
  ```

- 格式化指令前面有一个冒号

  - 数格式的替代方案是，x表示十六进制，o表示八进制，d表示十进制，b表示二进制。

  ```c++
  cout << format("{} {:x} {:o} {:d} {:b}\n", 1234,1234,1234,1234,1234);
  ```

- 默认情况下，format()按顺序接受参数。也可以指定任意的顺序。

  ​	冒号前的数字是要格式化的参数的数字。

  ```c++
  cout << format("{3:} {1:x} {2:o} {0:b}\n", 000, 111, 222, 333);
  ```

  ​	在最好的风格中，编号从0开始。这允许多次格式化一个参数

  ```c++
  cout << format("{0:} {0:x} {0:o} {0:d} {0:b}\n", 1234); // default, hexadecimal, octal, decimal, binary
  ```

- 浮点格式与ostream相同:

  - e代表scientific, a代表hexfloat, f代表fixed, g代表default

    ```c++
    cout << format("{0:}; {0:e}; {0:a}; {0:f}; {0:g}\n",123.456);         // default, scientific, hexfloat, fixed, default
    // 结果与ostream的结果相同，只是十六进制数前面没有0x
    // 123.456; 1.234560e+002; 1.edd2f2p+6; 123.456000; 123.456
    ```

  - 点位于精度说明符之前

    ```c++
    cout << format("precision(8): {:.8} {} {}\n", 1234.56789, 1234.56789, 123456); // precision(8): 1234.5679 1234.56789 123456
    cout << format("precision(4): {:.4} {} {}\n", 1234.56789, 1234.56789, 123456); // precision(4): 1235 1234.56789 123456
    cout << format("{}\n", 1234.56789); // 1234.56789
    ```

  - 与流格式化器一样，还可以指定将数字放入其中的字段的大小及其在该字段中的对齐方式。

  - 与流格式化器一样，format()也可以处理时间和日期

    ```c++
    cout << format("birthday: {}\n",November/28/2021);
    cout << format("zt: {}", zoned_time{current_zone(), system_clock::now()});
    ```

    - 通常，值的默认格式与默认流输出格式相同

    - 然而，``format()``提供了一种包含大约60个格式说明符的迷你语言，允许对数字和日期的格式进行非常详细的控制

      -  所有时间和日期格式字符串都以%开头

      ```c++
      auto ymd = 2021y/March/30 ;
      cout << format("ymd: {3:%A},{1:} {2:%B},{0:}\n", ymd.year(), ymd.month(), ymd.day(), weekday(ymd)); // ymd: Tuesday, March 30, 2021
      ```

- 格式说明符提供灵活性的同事也有很多出错的机会

  - 一些说明符带有可选的或区域相关的语义
  - 如果在运行时捕获格式化错误，则抛出``format_error``异常
  - ``vformat()``将变量作为格式，增加灵活性的同时减少出错的机会

  ```c++
  string ss = format("{:%F}", 2);           // error: mismatched argument; potentially caught at compile time
  string sss = format("{%F}", 2);          // error: bad format; potentially caught at compile time
  
  
  string fmt = "{}";
  cout << vformat(fmt, make_format_args(2));   // OK
  fmt = "{:%F}";
  cout << vformat(fmt, make_format_args(2));   // error: format and argument mismatch; caught at run time
  ```

- 格式化器可以直接写入迭代器定义的缓冲区

  ```c++
  string buf;
  format_to(back_inserter(buf), "iterator: {} {}\n", "Hi! ", 2022);
  cout << buf;       // iterator: Hi! 2022
  ```

### C格式化

​		c++标准库也支持C标准库I/O，包括printf()和scanf()。从类型和安全性的角度来看，这个库的许多使用都是不安全的，因此我不建议使用它。

​		特别是，为了安全方便地输入，它可能很难使用。不支持用户定义类型。如果您不使用c风格的I/O，但关心I/O性能，请调用：

```c++
ios_base::sync_with_stdio(false);                  // avoid significant overhead
```

​		如果没有这个调用，标准的``iostream``(例如cin和cout)与C风格I/O相比显著变慢。

​		如果喜欢``printf()``风格的格式化输出，使用``format``(§11.它是类型安全的，更容易使用，灵活，快速。







