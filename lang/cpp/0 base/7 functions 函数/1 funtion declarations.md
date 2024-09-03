# function declaration

​		函数声明引入函数名和它的类型。函数定义将函数名/类型与函数体关联。

---

A function declaration may contain argument names. This can be a help to the reader of a program, but unless the declaration is also a function definition, the compiler simply ignores such names.

```c++
double sqrt(double d);
```

The type of a function consists of its return type followed by the sequence of its argument types in parentheses.

For such a *member function*, the name of its class is also part of the function type.

## parameter list

### const形参和实参

​		当用实参初始化形参时会忽略掉顶层const。

```c++
void f(const int i);

// 调用fcn函数时，既可以传入const int也可以传入int。
// 反之，用const int和int调用 void f(const int i) 和 void f(int i)的调用形式是一样的，所以这两个函数原型不构成重载，会产生重定义错误
```



### 数组形参

- 不允许拷贝数组
- 使用数组时会将其转换成指针
  - 失去对数组元素个数的维度

```c++
void p(const int *);
void p(const int[]);
void p(const int[10]); // 形参在函数内部使用含有10个元素的数组，但实际传入的不一定

// 尽管表现形式不同，但上面的三个函数是等价的：每个函数的唯一形参都是const int＊类型的。

void p(const int(&arr)[10]); // 引用含有10个元素的数组
```

​		有三种方式来安全的传递数组：

- 增加尾标识
  - 如c风格字符串结尾加`\0`
- 标准库头尾指针方式
  - `void p(const int *begin, const int* end);`
- c风格 指针加 元素个数
  - `void p(const int* arr, size_t size);`



​		在C++语言中实际上没有真正的多维数组，所谓多维数组其实是数组的数组。

​		当将多维数组传递给函数时，真正传递的是指向数组首元素的指针。

- 因为我们处理的是数组的数组，所以首元素本身就是一个数组，指针就是一个指向数组的指针

- 数组第二维的大小都是数组类型的一部分，不能省略
  ```c++
  int *mareix[10]; // [*p, *p, ...]
  int (*matrix)[10]; // p->[10]
  
  // matrix[][10] == (*matrix)[10] == p->[10]
  void p(int matrix[][10], int rowsize);
  ```

  



### default arguments

​		A **default argument** is a default value provided for a function parameter.

> If the caller provides an argument, the value of the argument in the function call is used. If the caller does not provide an argument, the value of the default argument is used.

```c++
void print(int x, int y = 4)  {...}
print(1,2);
print(3);
```

Note that you must use the equals sign to specify a default argument. Using parenthesis or brace initialization won’t work:

```c++
void foo(int x = 5);   // ok
void goo(int x ( 5 )); // compile error
void boo(int x { 5 }); // compile error
```



#### **Multiple default arguments**

由于c++不支持指定参数赋值，所以多默认参数的函数实参传递时按顺序传递的，因此也不支持默认参数排在非默认参数之前。

> Default arguments can only be supplied for the rightmost parameters. 

```c++
#include <iostream>
void print(int x=10, int y=20, int z=30){}
void print(int x=10, int y); // not allowed

print(1, 2, 3); // all explicit arguments
print(1, 2); // rightmost argument defaulted
print(1); // two rightmost arguments defaulted
print(); // all arguments defaulted
```



#### **Default arguments can not be redeclared**

 That means for a function with a forward declaration and a function definition, the default argument can be declared in either the forward declaration or the function definition, but not both.

Best practice is to declare the default argument in the forward declaration and not in the function definition, as the forward declaration is more likely to be seen by other files (particularly if it’s in a header file).



#### **Default arguments and function overloading**

Functions with default arguments may be overloaded.

```c++
void print(std::string string){}
void print(char ch=' '){}
print("Hello, world"); // resolves to print(std::string)
print('a'); // resolves to print(char)
print(); // resolves to print(char)
```

>  `print()` acts as if the user had explicitly called `print(' ')`

但是默认参数的实参省略，可能会导致重载二义性，因为带有默认参数的函数被视为缺省该参数的重载复合。

```c++
void print(int x);
void print(int x, int y = 10); // print(x)
void print(int x, double y = 20.5); // print(x)
print(1, 2); // will resolve to print(int, int)
print(1, 2.5); // will resolve to print(int, double)
print(1); // ambiguous function call
```



### 可变形参

​		无法提前预知应该向函数传递几个实参，C++11新标准提供了两种主要的方法：

- 如果所有的实参类型相同，可以传递一个名为`std::initializer_list`的标准库类型

  - 用于表示某种特定类型的值的数组

  - 定义`initializer_list`对象时，必须说明列表中所含元素的类型

  - `initializer_list`的元素永远是常量值

    ```c++
    std::initializer_list<T> lst;
    std::initializer_list<T> lst{a,b,c,...};
    lst2(lst);
    lst2 = lst;
    lst.size();
    lst.begin();
    lst.end();
    ```

    

- 如果实参的类型不同，我们可以编写一种特殊的函数，也就是所谓的可变参数模板

- 除此之外，使用名为varargs的C标准库功能的省略符形参

  - 大多数类类型的对象在传递给省略符形参时都无法正确拷贝

  - 省略符形参只能出现在形参列表的最后一个位置

    ```c++
    void f(param_list, ...);
    void f(...);
    ```

    
