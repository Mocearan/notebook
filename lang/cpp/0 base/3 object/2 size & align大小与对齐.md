# object size & align

---

​		memory on modern machines is typically organized into byte-sized units, with each byte of memory having a unique address. 

​		most objects actually take up more than 1 byte of memory. A single object may use 2, 4, 8, or even more consecutive memory addresses. The amount of memory that an object uses is based on its data type.

​		we typically access memory through variable names (and not directly via memory addresses), the compiler is able to hide the details of how many bytes a given object uses from us. 

## object size

​		the more memory an object uses, the more information it can hold，an object with *n* bits can hold 2n unique values.

​		C++ only guarantees that each fundamental data types will have a minimum size:

![image-20230312121144984](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230312121144984.png)

​		 the actual size of the variables may be different on your machine。

​		`1 <=> sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)`

​		`1 <= sizeof(bool) <= sizeof(long)`

​		`sizeof(char) <= sizeof(wchar_t) <= sizeof(long)`

​		`sizeof(float) < sizeof(double) < sizeof(long double)`

​		`sizeof(N) <=> sizoef(signed N) <=> sizeof(unsigned N)`

- char 是机器上最适合保存和操作字符的类型，能够保存机器字符集中的任意字符。通常占一个8位的字节。
- int 是机器上最适合保存和操作整型的类型，通常4字节。不能假定Int和指针尺寸一样大，因为很多64位机器上，指针的尺寸比整数大（8字节）

### sizeof

​		The **sizeof operator** is a unary operator that takes either a type or a variable, and returns its size in bytes. 

```c++
#include <iostream>

int main()
{
    std::cout << "bool:\t\t" << sizeof(bool) << " bytes\n";
    std::cout << "char:\t\t" << sizeof(char) << " bytes\n";
    std::cout << "wchar_t:\t" << sizeof(wchar_t) << " bytes\n";
    std::cout << "char16_t:\t" << sizeof(char16_t) << " bytes\n";
    std::cout << "char32_t:\t" << sizeof(char32_t) << " bytes\n";
    std::cout << "short:\t\t" << sizeof(short) << " bytes\n";
    std::cout << "int:\t\t" << sizeof(int) << " bytes\n";
    std::cout << "long:\t\t" << sizeof(long) << " bytes\n";
    std::cout << "long long:\t" << sizeof(long long) << " bytes\n";
    std::cout << "float:\t\t" << sizeof(float) << " bytes\n";
    std::cout << "double:\t\t" << sizeof(double) << " bytes\n";
    std::cout << "long double:\t" << sizeof(long double) << " bytes\n";

    return 0;
}

// Your results may vary if you are using a different type of machine, or a different compiler.
```

​		can also use the *sizeof* operator on a variable name:

```c
#include <iostream>

int main()
{
    int x{};
    std::cout << "x is " << sizeof(x) << " bytes\n";

    return 0;
}
```

​		On modern machines, objects of the fundamental data types are fast, so performance while using these types should generally not be a concern.

## 对齐

​		You might assume that types that use less memory would be faster than types that use more memory. This is not always true. CPUs are often optimized to process data of a certain size (e.g. 32 bits), and types that match that size may be processed quicker. On such a machine, a 32-bit *int* could be faster than a 16-bit *short* or an 8-bit *char*.

​		每个对象类型都有对齐要求（alignment requirement），是一个`std::size_t`类型，2的n次幂的整数。

​		表示这个类型对象所占连续内存的大小。为了满足对齐要求的整数性质，某些成员后面会被插入填充空间。

>   在对齐的地址上能够更快的访问数据。
>
>   在不对齐的地址上，可能需要更多的总线周期，才能将包含该数据的全部地址加载到寄存器中进行运算。而对齐后只需要一次。
>
>   当某类型不足对齐要求，或超过对齐要求分割后部分不足对齐要求，会进行填充以满足一个对齐要求。以使得经过一个总线周期，总能开始读取下一个数据。

```c++
#include <iostream>
 
// S 类型的对象可以在任何地址上分配
// 因为 S.a 和 S.b 都可以在任何地址上分配
struct S {
  char a; // 大小：1，对齐：1
  char b; // 大小：1，对齐：1
}; // 大小：2，对齐：1
 
// X 类型的对象只能在 4 字节边界上分配
// 因为 X.n 必须在 4 字节边界上分配
// 因为 int 的对齐要求（通常）就是 4
struct X {
  int n;  // 大小：4，对齐：4
  char c; // 大小：1，对齐：1
  // 三个填充字节
}; // 大小：8，对齐：4
 
int main()
{
    std::cout << "sizeof(S) = " << sizeof(S)
              << " alignof(S) = " << alignof(S) << '\n';
    std::cout << "sizeof(X) = " << sizeof(X)
              << " alignof(X) = " << alignof(X) << '\n';
}
```

-   最弱的对齐（最小的对齐要求）是`char、signed char`和`unsigned char`的对齐，等于1 ；

-   所有类型中最大的基础对齐（fundamental alignment）是实现定义的，并等于 `std::max_align_t` 的对齐。

-   `alignof / std::alignment_of`，查询对齐要求

-   `std::align`，获取某存储中经过适当对齐的地址

-   `std::aligned_storage`，获取经过适当对齐的存储

-   `alignas`，要求更严格的对齐（更大的对齐要求）

    ```c++
    constexpr int bufmax = 1024;
    alignas(X) buffer[bufmax];
    ```
    
    
    
-   `#pragma pack(n)`

    >   ​		当使用 `alignas`使某个类型的对齐比 `std::max_align_t`的更严格（更大）时，称其为具有扩展对齐（extended alignment）要求的类型.
    >
    >   ​		具有扩展对齐的类型或包含具有扩展对齐的非静态成员的类类型称为过对齐（over-aligned）类型。
    >
    >   >   ​		`new `表达式、 `std::allocator::allocate`和 `std::get_temporary_buffer`是否支持过对齐类型是由实现定义的。
    >   >
    >   >   ​		以过对齐类型实例化的分配器(Allocator)允许在编译期发生实例化失败，在运行时抛出 `std::bad_alloc`异常，默认忽略不支持的对齐要求，也允许正确地处理它们。

