

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