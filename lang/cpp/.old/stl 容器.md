# STL容器

## class array<>

```include <array>```

```c++
namespace std{
    template<typename T, size_t N>
    class array;
}

// T 指定数组元素类型。
// N 指定数组的长度容量
```



array是一系列元素的排列，有着固定的大小。无法通过增加或移除元素来改变array的大小或容量。只能替换元素值。

因此array是static（这里并非存储期的静态概念，而是与dynamic相对的编译期静态概念，即array的内存模型在编译期确定）的，是一个c风格静态数组的封装，它比c风格的字符串更安全，且效率并未因此损失。

因为array是一个静态数组，其实现很大可能将内存分配在栈上，且绝不会重新分配。



### array的声明与定义

```array<T, N> ARRAY_NAME{...};``` 

```c++
std::array<int, 3> int3arr;
```



### array的初始化

```c++
// 默认初始化
std::array<int, 3> int3arr;
// 部分初始化
std::array<const char*, 5> names{"mass", "tsing"};
// 同上
std::array<const char*, 5> names = {"mass", "tsing"};
```

> 需要注意的是，array<> 的N参数需要一个常量属性的值。否则会报错。
>
> ```c++
> int size = 3;
> int carray[size] = {1, 2, 3};
> //std::array<int, size> array= {1, 2, 3}; // ‘size’ is not usable in a constant expression
> const int csize = 3;
> std::array<int, csize> array = {1, 2, 3};
> ```

### array的操作

#### 数组拷贝

是数组元素值的拷贝，接收的array拥有自己独立的存储空间。并非是简单的句柄指针的拷贝。

```c++
int3arr = {2,3,4};
std::array<int, 3> copy = int3arr;
// C风格的数组不能支持数组的拷贝，如果需要对数组进行批量的拷贝，只能通过循环来逐个元素赋值
```



#### 数组索引 [ ]

虽然array提供了```[]```运算符重载来提供数组的索引特性，但是作为纯粹数组概念的封装，将数组作为一个整体概念进行封装，而不再是传统C风格数组的 指针+长度 两个维度的结合体了。

因此array<>不提供 ```*```运算符的重载。

```c++
std::cout << carray[0] << std::endl;
std::cout << *carray << std::endl;
std::cout << array[0] << std::endl;
//std::cout << *array << std::endl;
```

> array<>的索引有如下的问题：
>
> ```c++
> array[10] = 4;
> std::cout << array[10] << std::endl; //4
> std::cout << array.size() << std::endl;//3
> std::cout << array.max_size() << std::endl;//3
> ```
>
> 所以还是需要注意索引是否越界。

#### 数组大小

##### size

array已经容纳的元素数

```c++
std::cout << array.size() << std::endl;  //3
std::cout << sizeof(carray) << std::endl; // 12 (bytes)
std::cout << sizeof(carray) / sizeof(carray[0])<< std::endl; //3
```

##### max_size

array最大可以容纳的元素数

#### 遍历

array除了使用传统的for循环迭代索引的方式外，还支持基于```std::initializer_list<>```的域迭代。

```c++
for(int& i : array)
    std::cout << i << " ";
std::cout << std::endl;
```

> 实际上，传统的数组也能够支持域迭代。甚至临时的值列表也能够支持域迭代。
>
> ```c++
> for (int n : {0, 1, 2, 3, 4, 5}) // 初始化器可以是花括号初始化器列表
>     std::cout << n << ' ';
> std::cout << '\n';
> 
> int a[] = {0, 1, 2, 3, 4, 5};
> for (int n : a) // 初始化器可以是数组
>     std::cout << n << ' ';
> std::cout << '\n';
> ```
>
> 



## class vector<>

```#include <vector>```

vector是一个向量，即其内存可自由的向单向增长。

在使用上，基本可以替代传统的c风格数组，并有一定的增强。



### vector 对象的定义

```std::vector<type> v_name;```

```c++
#include <vector>

std::vector<int> values;
```

>   type可以是任意类型，包括标准库所有模板类进行嵌套定义。

### vector 对象的初始化



### vector对象的操作

#### 数据访问

##### at

对c风格的数组索引增加了越界检查。

```c++
values.at[0];
```

##### operator[]

给与vector索引访问的支持，使得vector拥有和数组相似的能力。

```c++
values[1] =1;
```



#### 大小容量

##### vector大小

获取vector中现存元素数

```size()```

##### vector长度

获取vector的长度，实质上和size一样，只是在vector的向量语义上，给与向量长度的含义。

```length()```

##### vector容量

获取vector的现有内存容量能够容纳对象的个数。并非内存字节大小，而是以对象内存为单元计数。

```capacity()```



#### 尾后添加

```push_back(val)```

```c++
values.push_back(10);
```

#### 尾后移除

```pop_back(val)```

```c++
values.pop_back();
```

#### 依值移除

```erase(vector<type>::iterator)```

```c++
values.erase(values.begin());
```

