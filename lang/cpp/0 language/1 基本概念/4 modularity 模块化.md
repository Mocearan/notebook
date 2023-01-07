# Modularity 模块化

---

​		c++程序是由众多独立开发的部分组成的，如函数，用户自定义类型，类层次，模板等实体。管理众多部件的关键是明确定义这些部件之间的交互方式，首要的问题是如何区分各个部分的接口和实现。

​		在语言层面，c++通过声明来表达接口，定义来完成实现。声明指定使用函数或类型所需的所有接口内容。

```c++
double sqrt(double);        // the square root function takes a double and returns a double

class Vector {                   // what is needed to use a Vector
public:
        Vector(int s);
        double& operator[](int i);
        int size();

private:
        double* elem; // elem points to an array of sz doubles
        int sz;
};
```

​		这里的关键在于函数和类的定义，即其实现可以在其他地方。而使用者仅通过这个声明，就能获取使用函数或类型的所有信息。

```c++
double sqrt(double d)              // definition of sqrt()
{
           // ... algorithm as found in math textbook ...
}

Vector::Vector(int s)                                // definition of the constructor
        :elem{new double[s]}, sz{s}           // initialize members
{
}

double& Vector::operator[](int i)           // definition of subscripting
{
        return elem[i];
}

int Vector::size()                                     // definition of size()
{
        return sz;
}
```

​		一个实体可以声明多次，但只能定义一次。

## 分离编程

​		标准库和其他库，和程序本身的分离式编程本身没有本质的区别，库是我们恰巧使用的相同语言写成的其他代码。

​		c++支持分离编译（独立编译）的概念，即使用方只通过函数或类型的声明来获取编译所需的符号信息。

​		实现分离编译可以通过两种方式：

- 头文件原型声明

  ​	将声明放入单独的头文件中，并在需要获取相关符号信息的地方通过`#include`来展开头文件。

- 模块

  ​	定义单独编译的模块文件，然后在需要获取相关符号信息的地方通过`import`来导入符号。显式导出的声明才能被其他代码导入。

​		两者都可以用来将程序组织成一组半独立的代码片段。这种分离可用于最小化编译时间，并强制分离程序逻辑上不同的部分（从而最小化错误的可能性）。

​		库通常是单独编译的代码片段（例如函数）的集合。

​		用于组织代码的头文件技术可以追溯到C语言的早期，至今仍是最常见的。模块的使用在C++20中是新的，在代码卫生和编译时间方面提供了巨大的优势。

### 头文件

​		传统的方式是，将一个模块名作为一个文件名，将该模块的符号声明放入该文件形成符号头文件，指定了一个模块的接口信息。

```c++
// Vector.h:

class Vector {
public:
        Vector(int s);
        double& operator[](int i);
        int size();
private:
        double* elem;          // elem points to an array of sz doubles
        int sz;
};
```

​		用户通过`#include`来展开该模块所有的符号信息。

```c++
// user.cpp:

#include "Vector.h"       // get Vector's interface
#include <cmath>         // get the standard-library math function interface including sqrt()

double sqrt_sum(const Vector& v)
{
        double sum = 0;
        for (int i=0; i!=v.size(); ++i)
                sum+=std::sqrt(v[i]);                      // sum of square roots
        return sum;
}
```

​		为了帮助编译器确保一致性，提供实现的.cpp文件还将包括提供其接口的.h文件来进行符号校验

```c++
// Vector.cpp:

#include "Vector.h"          // get Vector's interface

Vector::Vector(int s)
        :elem{new double[s]}, sz{s}               // initialize members
{
}

//...
```

​		.cpp文件可以独立编译称为翻译单元。一个程序可以由数千个翻译单元组成。

​		user.cpp和Vector.cpp中的代码共享Vector.h中提供的Vector接口信息，但这两个文件在其他方面是独立的，可以单独编译。

​		头文件通过文件表示模块化，然后通过单独编译利用模块化。

​		程序组织的最佳方法是将程序视为一组具有明确定义的依赖关系的模块。

### 缺点

​		使用头文件和`#include`来进行模块化是一种古老的方式，具有一些缺点：

- 编译时长

  ​	头文件中所有的符号信息都会在被`#include`处完全展开，插入符号信息。也就是说头文件中的符号信息，在不同的翻译单元中实际上被重复的处理。

- 顺序相关性

  ​	头文件包含的顺序会造成影响

- 重复包含

  ​	在复杂的模块化组织中，可能隐式的造成在一个翻译单元中对同一头文件重复展开

- 循环引用

  ​	相互包含

- 不一致性

  ​	两个文件中对同一实体的声明有所不同，可能会导致崩溃或细微错误。

  ​	如果我们无意或有意地在两个源文件中单独声明一个实体，而不是将其放在一个头文件中，或者通过不同头文件之间的顺序依赖关系，就会发生这种情况。

- 传递性

  ​	在头文件中表达声明所需的所有代码都必须存在于该头文件中。这会导致大量代码膨胀，因为头文件#包含其他头文件，这会导致头文件的用户（意外或故意）依赖于此类实现细节。

### 模块

## 命名空间

## 函数

