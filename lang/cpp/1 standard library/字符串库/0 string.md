# 空终止字符串（C-Style字符串）

​		尾随`\0`（称为空终止符）的非零字节序列。

> A C-style string is called a **null-terminated string** 
>
> A **null terminator** is a special character (‘\0’, ascii code 0) used to indicate the end of the string.

​		编译器自动为字符串添加`\0`，因而需要注意的是空终止字符串与字符数组的区别。

- 空终止字符串一般不需要注意`\0`

- 而当空终止字符串转换为字符数组时，应当注意`\0`也需要提供存储空间，导致字符数组的长度多一个

  ```c++
  char myString[]{ "string" };
  const int length{ static_cast<int>(std::size(myString)) };
  //  const int length{ sizeof(myString) / sizeof(myString[0]) }; // use instead if not C++17 capable
  std::cout << myString << " has " << length << " characters.\n";
  ```

  实际上空终止字符串是一个字符指针，以`\0`作为字面意义的结束。因为，其可以用指针或数组的方式进行使用。



## symbolic constants 符号常量

```c++
const char* name{"Mass"};
```

​		字符串值`Mass`存储在进程常量区，栈上只有一个常量指针指向该地址。多个指向相同值的指针具有相同的地址。

> 常量区具有静态存储期。

```c++
int nArray[5]{ 9, 7, 5, 3, 1 };
char cArray[]{ "Hello!" };
const char* name{ "Alex" };

std::cout << nArray << '\n'; // nArray will decay to type int*
std::cout << cArray << '\n'; // cArray will decay to type char*
std::cout << name << '\n'; // name is already type char*

/*
003AF738
Hello!
Alex
*/
```

> `std::cout`在处理不同类型的指针上有一些不同
>
> - non-char pointer, it will simply print the contents of that pointer (the address that the pointer is holding).
>
> - `char*` or `const char*`, it will assume you’re intending to print a string.
>
>   > 这也许会导致一些错误：
>   >
>   > ```c++
>   > char c{'a'};
>   > std::cout<< &c;
>   > ```
>   >
>   > 会将`&c`视为`char*`，从而打印字符串，因为没有空终止符，所以会将后续内存中的垃圾值也视为字符串输出。

## 字节字符串

​		

## 多字节字符串



## 宽字节字符串

# string class 字符串类

---

​		使用C-style strings的缺点之一是需要字符串的用户进行所有的内存管理。

```c++
char* strHello{new char[7]}; // 需要小心的处理字符串的边界
strcpy(strHello, "hello!");
delete[] strHello;
```

​		因为C-style strings基于指针，因此也不能使用符合直觉的操作符来进行字符串操作，如赋值和比较。

​		因此C++标准库以类的形式提供了`std::string`和`std::wstring`提供了更好的处理字符串的方式。

## text manipulation

- `std::string`，使大多数用户不必通过指针对C风格的字符数组进行操作
- `std::string_view`，允许对存储在别处（`std::string / char[]`）的字符序列进行操作
- `std::regex`，提供了文本查找模式的正则表达式

> `std::string`和`std::regex`对象都可以使用各种字符类型(例如，Unicode)。

## basic_string

​		字符串类拥有共同的模板基类`basic_string`:

```c++
namespace std
{
    template<class charT, class traits = char_traits<charT>, class Allocator = allocator<charT>>
        class basic_string;
}
```

​		这个模板基类不能直接使用，`basic_string`的两个派生类提供两种类型的字符串：

```c++
namespace std
{
	typedef basic_stirng<char> string;	// ascii and utf-8 stirngs, 单/多字节字符串
    typedef basic_string<wchar_t> wstring; // wide-character / unicode(utf-16) strings, 宽字节字符串
	typedef std::basic_string<char8_t> std::u8string;  // (C++20)
	typedef std::basic_string<char16_t> std::u16string; // (C++11)
	typedef std::basic_string<char32_t> std::u32string; // (C++11)
}
```

### 模板形参

- CharT 字符类型
- Traits 指定字符类型上操作的特性类
- Allocator 用于分配内部存储的分配器类型

### 成员类型

- `size_type`

  ​	`size_t`，实际的`size_t`大小依赖于环境。

### 成员函数

#### 构造

- `basic_string()`

  ```c++
  std::string s;
  ```

- `basic_string(const CharT* szCString)`

  ​	从c-style字符串构建`std::basic_string`，不包括`NULL`终止符

  ​	如果c风格字符串大小超过string允许的最大值，会抛出`length_error`异常

  ```c++
  const char* str{"my string"};
  std::string s{str};
  ```

- `basic_string(const CharT* str, size_type length)`

  ```c++
  const char* str{"my string"};
  std::string s(str, 4); // my s
  ```

- `basic_string(const std::string& str)`

  ```c++
  std::string s{std::string{"mystring"}};
  ```

- `basic_string(const std::string& str, size_type pos)`

- `basic_string(const std::string& str, size_type pos, size_type count)`

  - 从`str`的`pos`开始拷贝最多`count`个字符
  - 没有`count`限制的版本将拷贝`str`从`pos`开始的所有字符
  - `pos`超出`str`的范围，将会抛出`out_of_range`异常

- `basic_string(size_type count, CharT ch)`

  ​	以`count`个`ch`创建字符串

  ```c++
  std::string s(4, 'q');
  ```

- `template <class InputIt> basic_string(InputIt first, InputIt last)`

  ​	以字符范围[first, last]创建字符串


#### 析构

- `~basic_string()`

  ​	销毁字符串，释放内存

#### 分配器

- `get_allocator()`

  ​	获取关联的分配器

#### 容量

- `capacity()`  

  ​	返回当前对象分配的存储空间能保存的字符数量

- `empty()`

  ​	检查字符串是否为空

- `length() / size()`

  ​	返回字符数

- `max_size()` 

  ​	返回可分配的最大字符数

- `reserve()`

  ​	扩展或缩小字符串的容量

#### 元素访问

- `at`

  ​	访问指定字符，有边界检查

- `operator[]`

  ​	访问指定字符

- `c_str() / data()`

  ```c++
  printf("For people who like printf: %s\n",s.c_str());           // s.c_str() returns a pointer to s' characters
  cout << "For people who like streams: " << s << '\n';
  ```

  

#### 迭代器

- `begin() / end()` 获取起始/尾后迭代器
- `cbegin() / cend()` 获取常起始/常尾后迭代器
- `rbegin() / rend()` 获取反向起始/反向尾后迭代器
- `crbegin() / crend()` 获取反向常起始/反向常尾后迭代器

#### 修改

##### 赋值

- `operator=`
- `assign()`

##### 拷贝

- `copy()`

  ​	将内容(不以null结尾)复制到字符数组

##### 尾插

- `operator+=`

  ```c++
  void m2(std::string& s1, std::string& s2)
  {
      s1 = s1 + '\n';   // append newline
      s2 += '\n';        // append newline
  }
  ```

- `append()`

- `push_back()`

##### 插入

- `insert()`

##### 删除

- `erase()`

  ​	移除字符

- `clear()`

  ​	清空字符串

##### 更改

- `replace()` 替换字符串的指定部分

  ​	replaces a substring with a value. I

  ```C++
  string name = "Niels Stroustrup";
  name.replace(0,5,"nicholas");          // name becomes "nicholas Stroustrup"
  ```

  

- `resize()`

  ​	展开或缩小字符串(在字符串末尾截断或添加字符)

- `swap()`

  ​	交换两个字符串的值

- `operator[]` 修改指定字符的值

  ```c++
  string name = "Niels Stroustrup";
  name[0] = toupper(name[0]);            // name becomes "Nicholas Stroustrup"
  ```

  

#### 比较

​		lexicographical ordering  字典序

​		**string**s can be compared against each other, against C-style strings §[1.7.1](ch01.xhtml#sec1_7_1)), and against string literals.

- `operator== / operator!=`

- `operator< / operator<= / operator> / operator>=`

- `compare()`

  ​	 returns -1, 0, or 1

#### 查找

- `find()`

  ​	查找首个字符或子串的索引

- `find_first_of()`

  ​	查找给定集合中的字符的首个索引

- `find_first_not_of()`

  ​	查找非给定集合中的字符的首个索引

- `find_last_of()`

  ​	查找给定集合中的字符的最后一个索引

- `find_last_not_of()`

  ​	查找非给定集合中的字符的最后一个索引

- `rfind()`

  ​	查找最后一个字符或子串的索引

#### 子串

- `operator+`

  ​	合并子串

- `substr()` 截取子串

  返回一个字符串，该字符串是由参数指定的字符串的副本
  
  - The first argument is an index into the **string** (a position)
  - and the second is the length of the desired substring.
  
  ```c++
  string name = "Niels Stroustrup";
  string s = name.substr(6,10);

#### IO

- `operator>>`
- `getline()`
- `operator<<`

### 功能遗漏

​		虽然`basic_string`支持众多的功能，但是有一些需要注意的遗漏。

- 正则表达式支持
- 从数字构造字符串
- 大小写转换函数
- 不区分大小写的比较
- 按词切分（Tokenization）或按字符拆分
- 获取字符串左侧或右侧部分的便利函数
- 空白符去除
- 按`sprintf`风格格式化字符串
- utf-8和utf-16间的转换（`std::string`和`std::wstring`的转换）

​		需要自行拓展实现这些功能，或将字符串转换为C-style字符串(``c_str()``)后使用C库函数提供的功能。



### `std::string` literal

​		一个常规的字符串字面量是`const char*`类型的字符串常量，要得到一个`std::string`类型的字符串常量需要在字符串字面量后面加上`s`后缀。

​		 use the namespace **std::literals::string_literals** 

```c++
using std::literals::string_literals;
auto cat = "Cat"s;    // a std::string
auto dog = "Dog";   // a C-style string: a const char*
```



### implement

​		现在，字符串通常使用短字符串优化实现。也就是说，短字符串值保存在字符串对象本身中，只有较长的字符串被放置在自由存储区中。

```c++
string s1 {"Annemarie"};                          // short string
string s2 {"Annemarie Stroustrup"};       // long string
```

​		当字符串的值从短字符串变为长字符串时(反之亦然)，它的表示也会相应调整。

​		一个“短”字符串可以有多少个字符是实现定义的，通常是14个字符。

> ​		字符串的实际性能可能严重依赖于运行时环境。特别是，在多线程实现中，内存分配的成本可能相对较高。此外，当使用大量不同长度的字符串时，可能会导致内存碎片。这些是短串优化变得无处不在的主要原因。

​		为了处理多个字符集，string实际上是字符类型为char的通用模板basic_string的别名：

```c++
template<typename Char>
class basic_string {
        // ... string of Char ...
};
using string = basic_string<char>;
```

​		用户可以定义任意字符类型的字符串。例如，假设我们有一个日文字符类型Jchar，我们可以这样写:

```c++
using Jstring = basic_string<Jchar>;
```



## basic_string_view

​		字符序列上进行的最常见的操作是作为参数传递给一个函数去读，可以是pass by value，pass by reference 或者传递C风格的字符串指针。

​		在许多系统中，还提供其他标准库没有的字符串类型。

​		拷贝或初始化一个`std::string`是比较慢的，因为涉及到独立内存空间的构建。

​		在所有这些情况下，传递子字符串时，会有额外的复杂性。为此标准库提供了``string_view``，表示字符序列的一对指针和长度。

```c++
std::string_view => {std::stirng::begin(), std::string::size()}
							↓
                              |H|e|l|l|o| |w|o|r|l|d|\0|
```

​		`std::string_view`提供了对已经分配存储的字符串(包括 C风格字符串字面值， `std::string`，或``char``数组)的只读访问，而无需复制。它不拥有它所指向的字符，是`std::string`的观察者模式。

>  ​		可以将``std::string_view``视为改进的``const std::string&``：
>
>  - 它可以用于以多种不同方式存储的字符序列
>  - 可以很容易地传递一个子字符串
>  - 不需要创建一个字符串来传递c风格的字符串参数
>
>  ​        `std::string_view`的一个重要限制是它是某个字符序列的只读视图。
>
>  ​		例如，不能使用``string_view``将字符序列传递给将参数修改为小写的函数。为此，可以考虑使用``std::span``

```c++
std::string_view text{ "hello" }; // view the text "hello", which is stored in the binary
std::string_view str{ text }; // view of the same "hello"
std::string_view more{ str }; // view of the same "hello"

/////////////////////////////////////////////////////////////////////

string cat(string_view sv1, string_view sv2)
{
     string res {sv1};           // initialize from sv1
     return res += sv2;        // append from sv2 and return
}

string king = "Harold";
auto s1 = cat(king,"William");                          // HaroldWilliam: string and const char*
auto s2 = cat(king,king);                                   // HaroldHarold: string and string
auto s3 = cat("Edward","Stephen"sv);           // EdwardStephen: const char * and string_view
auto s4 = cat("Canute"sv,king);                      // CanuteHarold
auto s5 = cat({&king[0],2},"Henry"sv);           // HaHenry
auto s6 = cat({&king[0],2},{&king[2],4});         // Harold
```

> no more copies of the string “hello” are created. The string “hello” is stored in the binary and is not allocated at run-time.

​		`string_view`定义了一个范围，因此我们可以遍历它的字符。

```c++
void print_lower(string_view sv1)
{
    for (char ch : sv1)
        cout << tolower(ch);
}
```



​		When we copy a `std::string_view`, the new `std::string_view` observes the same string as the copied-from `std::string_view` is observing. 

```c++
#include <iostream>
#include <string>
#include <string_view>

void print(std::stirng_view str) // access “Hello, world!” through str, without making a copy of the string.
{
    std::cout << str << '\n';
}

int main()
{
    std::string_view s{"Hello, world!"}; // initialize std::string_view s with C-style string literal, provides read-only access, without making a copy of the string. 
    print(s);
}
```

> no copies of the string “Hello, world!” are made.

​		Prefer `std::string_view` over `std::string` when you need a read-only string, especially for function parameters.

​		Unlike `std::string`, `std::string_view` has full support for constexpr:

```c++
constexpr std::string_view s{ "Hello, world!" };
```

​		A `std::string_view` can be created using a `std::string` initializer, and a `std::string` will implicitly convert to a `std::string_view`:

```c++
std::string s{ "Hello, world" };
std::string_view sv{ s }; // Initialize a std::string_view from a std::string

printSV(s); // implicitly convert a std::string to std::string_view
```

​		`std::string_view` won’t allow implicit convert to a `std::string`, but explicit create a `std::string` with a `std::stirng_view` initializer is allowed, or we can convert an existing `std::string_view` to a `std::string` using `static_cast`

```c++
std::string_view sv{ "balloon" };

std::string str{ sv }; // okay, we can create std::string using std::string_view initializer

void printString(std::string str) { std::cout << str << '\n'; }
// printString(sv);   // compile error: won't implicitly convert std::string_view to a std::string

printString(static_cast<std::string>(sv)); // okay, we can explicitly cast a std::string_view to a std::string
```

​		可以把``string_view``看作是一种指针，要被使用，它必须指向某物。因此函数返回``std::string_view`` 常常是危险的。返回的字符串在`string_view`被使用之前就销毁了。

```c++
string_view bad()
{
    string s = "Once upon a time";
    return {&s[5],4};                // bad: returning a pointer to a local
}
```

 		对``std::string_view``的范围外访问行为是未定义的。使用``at()``进行范围检查的访问，会抛出``std::out_of_range``来尝试超出范围的访问，或者使用``gsl::string_span``.

​		change the viewed string, the changes are reflected in the `std::string_view`

```c++
char arr[]{"Gold"};
std::string_view str{arr};
arr[3] = 'f';

std::cout << str<< '\n'; // Golf
```

> Use `std::string_view` instead of C-style strings.
>
> Prefer `std::string_view` over `std::string` for read-only strings, unless you already have a `std::string`.

​		`std::stirng_view`可以通过遮蔽的方式，在不改变底层字符串的基础上，得到变更的结果。

```c++
std::stirng_view str{"Peach"};
str.remvoe_prefix(1); // each
str.remove_suffix(2); // ea
```

> 一旦进行了遮蔽，无法进行恢复，除非通过原字符串重置`std::stirng_view`。

​		`std::stirng_view`能够从非空终止字符数组进行构建。

```c++
char vowels[] { 'a', 'e', 'i', 'o', 'u'};
// vowels isn't null-terminated. We need to pass the length manually.
// Because vowels is an array, we can use std::size to get its length.
std::string_view str{vowels, std::size(vowels)};
std::cout << str << '\n'; // This is safe. std::cout knows how to print std::string_view.
```

​		`std::string_view`可能并非构建自空终止字符串，因此不能获取其底层字符串，需要先转换为`std::string`，再获取。

```c++
std::string_view sv{"balloon"};
sv.remove_suffix(3);
std::string str{sv};
auto szNullTerminated{str.c_str()};
```

​		making the parameter a `std::string_view` is the most flexible choice, because it can work efficiently with C-style string arguments (including string literals), `std::string` arguments (which will implicitly convert to `std::string_view`), and `std::string_view` arguments:

```c++
void printSV(std::string_view sv)
{
    std::cout << sv << '\n';
}

std::string s{ "Hello, world" };
std::string_view sv { s };

printSV(s);              // ok: pass std::string
printSV(sv);             // ok: pass std::string_view
printSV("Hello, world"); // ok: pass C-style string literal

return 0;
```

> ​		Note that we pass `std::string_view` by value instead of by const reference. This is because `std::string_view` is typically fast to copy, and pass by value is optimal for cheap to copy types.
>
> ​		if your function needs to call some other function that takes a C-style string or `std::string` parameter, then `const std::string&` may be a better choice, as `std::string_view` is not guaranteed to be null-terminated (something that C-style string functions expect) and does not efficiently convert back to a std::string.

​		`std::string_view`并不维持字符串的生命周期，对于字符串字面值来说，其存储在常量区，与程序的生命周期一致，不会造成问题；对于C-style字符串变量，`std::string`来说，`std::stirng_view`的生命周期可能长过它们。这会造成`UB`。

```c++
std::string_view askForName()
{
  std::cout << "What's your name?\n";

  // Use a std::string, because std::cin needs to modify it.
  std::string name{};
  std::cin >> name;

  // We're switching to std::string_view for demonstrative purposes only.
  // If you already have a std::string, there's no reason to switch to
  // a std::string_view.
  std::string_view view{ name };

  std::cout << "Hello " << view << '\n';

  return view;
} // name dies, and so does the string that name created.

std::string_view view{ askForName() };
// view is observing a string that already died.
std::cout << "Your name is " << view << '\n'; // Undefined behavior
```

​		Using `std::string_view::data()` of a non-null-terminated string can cause undefined behavior.Only use `std::string_view::data()` if the `std::string_view`‘s view hasn’t been modified and the string being viewed is null-terminated. 

```c++
 std::string_view str{ "balloon" };

  // Remove the "b"
  str.remove_prefix(1);
  // remove the "oon"
  str.remove_suffix(3);
  // Remember that the above doesn't modify the string, it only changes
  // the region that str is observing.

  std::cout << str << " has " << std::strlen(str.data()) << " letter(s)\n"; // all 6
  std::cout << "str.data() is " << str.data() << '\n';		// alloon
  std::cout << "str is " << str << '\n';					// all
```

> `std::string_view::data()`返回底层字符串的`const char*`，`cout / strlen`会以空终止字符串的逻辑查找字符串结尾。

​		`std::string_view`的实现不完整，在未来的版本中可能会进行完善

```c++
std::string s{ "hello" };
std::string_view v{ "world" };

// Doesn't work
std::cout << (s + v) << '\n';
std::cout << (v + s) << '\n';

// Potentially unsafe, or not what we want, because we're treating
// the std::string_view as a C-style string.
std::cout << (s + v.data()) << '\n';
std::cout << (v.data() + s) << '\n';

// Ok, but ugly and wasteful because we have to construct a new std::string.
std::cout << (s + std::string{ v }) << '\n';
std::cout << (std::string{ v } + s) << '\n';
std::cout << (s + static_cast<std::string>(v)) << '\n';
std::cout << (static_cast<std::string>(v) + s) << '\n';
```



### `std::string_view` literal

​		We can create string literals with type `std::string_view` by using a `sv` suffix after the double-quoted string literal.

```c++
std::cout << "foo\n";   // no suffix is a C-style string literal
std::cout << "goo\n"s;  // s suffix is a std::string literal
std::cout << "moo\n"sv; // sv suffix is a std::string_view literal
```

​		The “sv” suffix lives in the namespace `std::literals::string_view_literals`. The easiest way to access the literal suffixes is via using directive `using namespace std::literals`.

​		当从``const char*``构造一个``string_view``，这需要计算字符数。对于``"Stephen"sv``，长度在编译时计算。

## char_traits



