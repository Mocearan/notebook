# size



## 字符串长度

​		返回字符串中`CharT`元素数

- `size_type size() const;`
- `size_type length() const;`

```c++
std::string s { "012345678" };
std::cout << s.length() << '\n'; // 9
std::cout << s.size() << '\n'; // 9
```



## 字符串容量

​		字符串的容量反映了该字符串分配多少内存来保存其内容。该值以字符串字符计算，不包括NULL结束符。

- `size_type capacity() const;`

```c++
std::string s { "01234567" };
std::cout << "Length: " << s.length() << '\n'; // 8
std::cout << "Capacity: " << s.capacity() << '\n'; // 15
```





## 判空

- `bool empty() const;`

```c++
std::string string1 { "Not Empty" };
std::cout << (string1.empty() ? "true" : "false") << '\n';
std::string string2; // empty
std::cout << (string2.empty() ? "true" : "false")  << '\n';
```



## 字符串最大值

​		返回 string 由于保有系统或库实现限制所能保有的最大元素数

- `size_type max_size() const;`

```c++
std::string s { "MyString" };
std::cout << s.max_size() << '\n'; // 4294967294
```

​		容量比实际需要的空间大，是由allocator的分配策略决定的，避免频繁的内存分配和数据拷贝。

​		其次，当字符串重新分配时，字符串的实际内容被移动到新的内存地址中，这意味着对字符串原地址的所有引用、指针和迭代器都会失效。



## 预分配

- `void reserve();`

  ​	非强制的收缩到适合请求

- `void reserve(size_type size);`

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

​		通过`reserve()`来预分配字符串大小，避免字符串的重新分配。

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

​		另一种避免连接多个字符串避免分配的方式是使用`std::ostringstream`.