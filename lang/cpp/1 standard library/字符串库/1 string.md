

# basic_string

​		`basic_string`是对各种类型和编码的空终止字符串的抽象封装。

```c++
namespace std
{
    template<class charT, class traits = char_traits<charT>, class Allocator = allocator<charT>>
        class basic_string;
}

// CharT 字符类型
// Traits 指定字符类型上操作的特性类
// Allocator 用于分配内部存储的分配器类型
```

​		这个模板基类不能直接使用，需要以特定类型字符实例化为特定类型字符串：

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



## 成员类型

- `size_type`

  ​	`size_t`，实际的`size_t`大小依赖于环境。



## 初始化、反初始化

### 构造

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

### 析构

- `~basic_string()`

  ​	销毁字符串，释放内存



## 接口

### allocator

- `get_allocator()`

  ​	获取关联的分配器

### compare

​		lexicographical ordering  字典序

​		**string**s can be compared against each other, against C-style strings §[1.7.1](ch01.xhtml#sec1_7_1)), and against string literals.

- `operator== / operator!=`

- `operator< / operator<= / operator> / operator>=`

- `compare()`

  ​	 returns -1, 0, or 1

### search

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

### substr

​		 截取子串, 返回一个字符串，该字符串是由参数指定的字符串的副本

- The first argument is an index into the **string** (a position)
- and the second is the length of the desired substring.

```c++
string name = "Niels Stroustrup";
string s = name.substr(6,10);
```





### iterator

- `begin() / end()` 获取起始/尾后迭代器
- `cbegin() / cend()` 获取常起始/常尾后迭代器
- `rbegin() / rend()` 获取反向起始/反向尾后迭代器
- `crbegin() / crend()` 获取反向常起始/反向常尾后迭代器



### `operator=`

- `basic_string& operator= (const basic_string& str)`
- ``basic_string& operator= (const charT* str)``
- ``basic_string& operator= (charT c)``

```c++
std::string sString;

// Assign a string value
sString = std::string("One");
std::cout << sString << '\n';

// Assign a C-style string
sString = "Three";
std::cout << sString << '\n';

// Assign a char
sString = '5';
std::cout << sString << '\n';

// Chain assignment
std::string sOther;
sString = sOther = "Six";
std::cout << sString << ' ' << sOther << '\n';
```





### `assign()`

- ``basic_string& assign (const basic_string& str)``

- ``basic_string& assign (const charT* str)``

  ```c++
  std::string sString;
  const std::string sTwo("Two");
  
  sString.assign(sTwo);
  std::cout << sString << '\n';
  
  sString.assign("Four");
  std::cout << sString << '\n';
  ```

  

- `basic_string& assign(const basic_string& str, size_type index, size_type len);`

  ```c++
  const std::string sSource("abcdefg");
  std::string sDest;
  
  sDest.assign(sSource, 2, 4); // assign a substring of source from index 2 of length 4
  std::cout << sDest << '\n';
  ```

- `basic_string& assign(const charT* s, size_type len)`

  ```c++
  std::string sDest;
  sDest.assign("abcdefg", 4);
  ```

- `basic_string& assign(size_type len, charT c)`

  ```c++
  std::string sDest;
  sDest.assign(4, 'g');
  ```





### `copy`

​		复制子串 `[pos, pos+count)` 到 `dest` 所指向的字符串。

- 若请求的子串超过过 `std::string`结尾，或若 `count == npos` ，则复制的子串为 `[pos, size())` 
- 若 pos > size() ，则抛出 [std::out_of_range](https://zh.cppreference.com/w/cpp/error/out_of_range) 

​		`copy`产生的子串不是空中止的。

```c++
size_type copy( CharT* dest, size_type count, size_type pos = 0 ) const; // (C++20 前)
constexpr size_type copy( CharT* dest, size_type count, size_type pos = 0 ) const; // (C++20 起)
```

  ```c++
  size_t length;
  char buffer[8];
  std::string str{"Test string......"};
  
  length = str.copy(buffer, 7, 5);
  buffer[length] = '\0';
  std::cout << buffer << '\n';
  
  length = str.copy(buffer, str.size(), 5);
  buffer[length] = '\0';
  std::cout << buffer << '\n';
  
  length = str.copy(buffer, 7, 0);
  buffer[length] = '\0';
  std::cout << buffer << '\n';
  
  length = str.copy(buffer, 7);
  buffer[length] = '\0';
  std::cout << buffer << '\n';
  
  length = str.copy(buffer, std::string::npos, 5);
  buffer[length] = '\0';
  std::cout << buffer << '\n';
  
  length = str.copy(buffer, std::string::npos);
  buffer[length] = '\0';
  std::cout << buffer << '\n';
  ```

### `insert`

- `basic_string& insert(size_type index, const basic_string& str);`

  ```c++
  std::string sString("aaaa");
  sString.insert(2, std::string("bbbb"));
  ```

- `basic_string& insert(size_type index, const charT* str);`

  ```c++
  std::string sString("aaaa");
  sString.insert(4, "cccc");
  ```

- `basic_string& insert(size_type index, const basic_string& s, size_type start, size_type num);`

  ```c++
  std::string sString("aaaa");
  const string sInsert("01234567");
  
  sString.insert(2, sInsert, 3, 4); // inser [3, 7) into sString at index 2
  ```

- `basic_string& insert(size_type index, const charT* s, size_type len);`

  ```c++
  string sString("aaaa");
  sString.insert(2, "bcdef", 3); // aabcdaa
  ```

- `basic_string& insert(size_type index, size_type num, charT c)`

  ```c++
  string sString("aaaa");
  sString.insert(2, 4, 'c'); // aaccccaa
  ```

- `void insert(iterator it, size_type num, charT c);`

- `iterator insert(iterator it, charT c)`

- `void insert(iterator it, InputIt begin, InputIt end)`




### `push_back`

```c++
void push_back(charT c)
```

```c++
std::string sString{"one"};
sString.push_back('2');
```

### `operator+=`

- `basic_string& operator+=(const basic_string& s)`

  ```c++
  std::string sString{"one"};
  sString += std::string{" two"};
  std::cout << sString << '\n';
  ```

- `basic_string& operator+=(const charT* str)`

  ```c++
  std::string sString{"one"};
  sString += " two";
  ```

- `basic_string& operator+=(charT c);`

  ```c++
  std::string sString{"one"};
  sString += ' ';
  ```

  

```c++
void m2(std::string& s1, std::string& s2)
{
    s1 = s1 + '\n';   // append newline
    s2 += '\n';        // append newline
}
```

### `append`

- `basic_string& append(const basic_string& s)`

  ```c++
  std::string sString{"one"};
  std::string sThree{" three"};
  sString.append(sThree);
  
  std::cout << sString << '\n';
  ```

- `basic_string& append(const basid_string& s, size_type index, size_type num);`

  ```c++
  std::string sString{"one "};
  
  const std::string sTemp{"twothreefour"};
  sString.append(sTemp, 3, 5); // append substring of sTemp starting at index 3 of length 5
  std::cout << sString << '\n';
  ```

- `basic_string& append(const charT* s);`

  ```c++
  std::string sString{"one"};
  sString.append(" three");
  ```

- `basic_string& append(const charT* s, size_type len)`

  ```c++
  std::string sString{"one "};
  sString.append("threefour", 5); // one three
  ```

- `basic_string& append(size_type num, charT c)`

  ```c++
  std::string s{"aaa"};
  s.append(4, 'b');
  ```

- `template<class InputIt> basic_string& append(InputIt start, InputIt end);`

  - Appends all characters from the range [start, end) 



### `erase`

​	移除字符

### `clear`

​	清空字符串





### `replace`

​	 替换字符串的指定部分

​		replaces a substring with a value. I

```C++
string name = "Niels Stroustrup";
name.replace(0,5,"nicholas");          // name becomes "nicholas Stroustrup"
```



### `resize`

​	展开或缩小字符串(在字符串末尾截断或添加字符)



### `swap`

```c++
void swap(basic_string& str)
boid std::swap(basic_string& s1, basic_string& s2)`
```

​	交换两个字符串的值

```c++
std::string sStr1("red");
std::string sStr2("blue");

std::cout << sStr1 << ' ' << sStr2 << '\n';
std::swap(sStr1, sStr2);
std::cout << sStr1 << ' ' << sStr2 << '\n';
sStr1.swap(sStr2);
std::cout << sStr1 << ' ' << sStr2 << '\n';
```



### `at`

​	访问指定字符，有边界检查

```c++
charT& at(size_type index);
const charT& at(size_type index) const;
```

​		进行边界检查，非法访问时抛出 [std::out_of_range](https://zh.cppreference.com/w/cpp/error/out_of_range) 类型的异常

```c++
std::string sSource{ "abcdefg" };
std::cout << sSource.at(5) << '\n';
sSource.at(5) = 'X';
```



### `operator[]`

​		访问指定字符

```c++
charT& operator[](size_type index);
const charT& operator[](size_type index) const;
```

​		如果 index > size()，那么行为未定义。

```c++
std::string sSource{ "abcdefg" };
std::cout << sSource[5] << '\n';
sSource[5] = 'X';
```



### `c_str / data`

​		A null terminator is appended. 

​		c++11前，`data()`的返回底层数组的指针，不满足末尾的`\0`可用，即在`[data(), data() + size())`范围内可用。

​		c++11后，`data()`与`c_str()`相同，包含了末尾的`\0`，范围为`[data(), data() + size()]`

```c++
const charT* c_str() const;
const charT* data() const;
```

​		The C-style string is owned by the std::basic_string and should not be deleted

```c++
std::string sSource{ "abcdefg" };
std::cout << std::strlen(sSource.c_str()); // 7

std::string sSource{ "abcdefg" };
const char* szString{ "abcdefg" };
// memcmp compares the first n characters of two C-style strings and returns 0 if they are equal
if (std::memcmp(sSource.data(), szString, sSource.length()) == 0)
    std::cout << "The strings are equal";
else
    std::cout << "The strings are not equal";
```

​		因为二者返回指向底层存储的指针，所以不能确定`std::string`对象生命周期的时候，要么现用现取，要么拷贝使用。

```c++
char* p;
{
    std::string s{"hello world"};
    //p = (char*)s.data();
	p = new char[s.length() + 1];
    ::strcpy(p, s.data());
}	
p[2] = 'C';
```



### `capacity`

​	返回当前对象分配的存储空间能保存的字符数量

字符串的容量反映了该字符串分配多少内存来保存其内容。该值以字符串字符计算，不包括NULL结束符。

```c++
size_type capacity() const;
```

```c++
std::string s { "01234567" };
std::cout << "Length: " << s.length() << '\n'; // 8
std::cout << "Capacity: " << s.capacity() << '\n'; // 15
```



### `empty`

​		检查字符串是否为空

```c++
bool empty() const;
```

```c++
std::string string1 { "Not Empty" };
std::cout << (string1.empty() ? "true" : "false") << '\n';
std::string string2; // empty
std::cout << (string2.empty() ? "true" : "false")  << '\n';
```



### `length / size`

​		返回字符数

​		返回字符串中`CharT`元素数

```c++
size_type size() const;
size_type length() const;
```

```c++
std::string s { "012345678" };
std::cout << s.length() << '\n'; // 9
std::cout << s.size() << '\n'; // 9
```





### `max_size`

​		返回可分配的最大字符数

​		返回 string 由于保有系统或库实现限制所能保有的最大元素数

```c++
size_type max_size() const;
```

```c++
std::string s { "MyString" };
std::cout << s.max_size() << '\n'; // 4294967294
```

​		容量比实际需要的空间大，是由``allocator``的分配策略决定的，避免频繁的内存分配和数据拷贝。

​		其次，当字符串重新分配时，字符串的实际内容被移动到新的内存地址中，这意味着对字符串原地址的所有引用、指针和迭代器都会失效。



### `reserve`

​		扩展或缩小字符串的容量

```c++
void reserve(); // 非强制的收缩到适合请求
void reserve(size_type size);
```

```c++
std::string s { "01234567" };
std::cout << "Length: " << s.length() << '\n';
std::cout << "Capacity: " << s.capacity() << '\n';

s.reserve(200);
std::cout << "Length: " << s.length() << '\n';
std::cout << "Capacity: " << s.capacity() << '\n';

s.reserve();
std::cout << "Length: " << s.length() << '\n';
std::cout << "Capacity: " << s.capacity() << '\n';
```

- 通过`reserve()`来预分配字符串大小，避免字符串的重新分配。

  ```c++
  #include <iostream>
  #include <string>
  #include <cstdlib> // for rand() and srand()
  #include <ctime> // for time()
  
  int main()
  {
      std::srand(std::time(nullptr)); // seed random number generator
  
      std::string s{}; // length 0
      s.reserve(64); // reserve 64 characters
  
      // Fill string up with random lower case characters
      for (int count{ 0 }; count < 64; ++count)
          s += 'a' + std::rand() % 26;
  
      std::cout << s;
  }
  ```

- 另一种避免连接多个字符串避免分配的方式是使用`std::ostringstream`.



### `operator>>`

### `getline`

​		`getline` 从输入流读取字符并将它们放进字符串：

```c++
std::string strBuf;
std::getline(std::cin, strBuf);
```

### `operator<<`

