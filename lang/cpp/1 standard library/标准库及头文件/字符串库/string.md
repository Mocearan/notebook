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

​		`#include <string>`

---

​		使用C-style strings的缺点之一是需要字符串的用户进行所有的内存管理。

```c++
char* strHello{new char[7]}; // 需要小心的处理字符串的边界
strcpy(strHello, "hello!");
delete[] strHello;
```

​		因为C-style strings基于指针，因此也不能使用符合直觉的操作符来进行字符串操作，如赋值和比较。

​		因此C++标准库以类的形式提供了`std::string`和`std::wstring`提供了更好的处理字符串的方式。



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

### 成员函数

#### 构造 / 析构

- 

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

- `c_str()`

- `data()`

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

- `replace()`

  ​	替换字符串的指定部分

- `resize()`

  ​	展开或缩小字符串(在字符串末尾截断或添加字符)

- `swap()`

  ​	交换两个字符串的值

#### 比较

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

- `substr()`

  ​	截取子串

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

## basic_string_view

​		initializing (or copying) a `std::string` is slow.To address the issue with `std::string` being expensive to initialize (or copy), C++17 introduced `std::string_view`。

​		`std::string_view` provides read-only access to an *existing* string (a C-style string literal, a `std::string`, or a char array) without making a copy.

>  could view `std::string_view` as an improved `const reference to string`.

```c++
std::string_view text{ "hello" }; // view the text "hello", which is stored in the binary
std::string_view str{ text }; // view of the same "hello"
std::string_view more{ str }; // view of the same "hello"
```

> no more copies of the string “hello” are created. The string “hello” is stored in the binary and is not allocated at run-time.

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

​		We can create string literals with type `std::string_view` by using a `sv` suffix after the double-quoted string literal.

```c++
std::cout << "foo\n";   // no suffix is a C-style string literal
std::cout << "goo\n"s;  // s suffix is a std::string literal
std::cout << "moo\n"sv; // sv suffix is a std::string_view literal
```

> The “sv” suffix lives in the namespace `std::literals::string_view_literals`. The easiest way to access the literal suffixes is via using directive `using namespace std::literals`.

​		Returning a std::string_view from a function is usually a bad idea. 

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



## char_traits
