# Variable

​		variables are names for a piece of memory that can be used to store information. 

---

​		从c++声明语义上说，已经声明的，除静态数据成员之外的，对象或引用是变量。

​		变量提供一个具名的、可供程序操作的存储空间。C++中的每个变量都有数据类型，数据类型决定

- 变量所占内存空间的大小
- 布局方式
- 该空间能存储的值的范围
- 以及变量能参与的运算

## local variables

​		variables that are defined inside a function (including function parameters).

> Although function parameters are not defined inside the function body, for typical functions they can be considered to be part of the scope of the function body block.

​		local variables have several different properties that differentiate how local variables behave from other kinds of (non-local) variables. 

- Local variables have **block scope**.

  ```c++
  int max(int x, int y) // x and y enter scope here
  {
      // assign the greater of x or y to max
      int max{ (x > y) ? x : y }; // max enters scope here
  
      return max;
  } // x, y, and max leave scope here
  ```

- **All variable names within a scope must be unique**

- **Local variables have automatic storage duration**

  - Local variables can be defined inside nested blocks.
  - Its scope is limited from its point of definition to the end of the nested block, and its lifetime is the same. 
  - Note that nested blocks are considered part of the scope of the outer block in which they are defined.

  ```c++
  #include <iostream>
  
  int main()
  { // outer block
  
      int x { 5 }; // x enters scope and is created here
  
      { // nested block
          int y { 7 }; // y enters scope and is created here
  
          // x and y are both in scope here
          std::cout << x << " + " << y << " = " << x + y << '\n';
      } // y goes out of scope and is destroyed here
  
      // y can not be used here because it is out of scope in this block
  
      return 0;
  } // x goes out of scope and is destroyed here
  ```

- **Local variables have no linkage**

  ​	Local variables have `no linkage`, which means that each declaration refers to a unique object

  ```c++
  int main()
  {
      int x { 2 }; // local variable, no linkage
  
      {
          int x { 3 }; // this identifier x refers to a different object than the previous x
      }
  
      return 0;
  }
  ```

- **Variables should be defined in the most limited scope**

  ​	Variables should be defined in the most limited scope

  ```c++
  #include <iostream>
  
  int main()
  {
      // do not define y here
  
      {
          // y is only used inside this block, so define it here
          int y { 5 };
          std::cout << y << '\n';
      }
  
      // otherwise y could still be used here, where it's not needed
  
      return 0;
  }
  ```

  > ​	Define variables in the most limited existing scope. Avoid creating new blocks whose only purpose is to limit the scope of variables.
  >
  > ​	If creating a nested block seems useful to intentionally limit the scope of a chunk of code, that code might be better to put in a separate function instead.

### local static variable 静态局部变量

块作用域中以`static / thread_local`说明的变量拥有静态或线程存储期。

- 静态存储期

  局部变量的自动存储期被提升为静态存储期。

  > 声明在首次执行时初始化（零初始化或常量初始化例外，进入块前就初始化了）一次，后续跳过。
  >
  > 如初始化异常，则初始化失败，下次对该静态局部变量声明的执行会再次尝试初始化。
  >
  > 如果初始化过程递归，行为未定义。
  >
  > 在初始化成功的情况下，静态局部变量在程序退出时析构。

- 同一内联函数(可隐式内联)的所有定义中的局部静态对象指向同一翻译单元中定义的相同对象，只要函数具有外部链接。

  > 同一内联函数的所有定义指的是，同一内联函数在多处展开，形成多处定义。

- 多线程并发初始化同一静态局部变量，初始化只会严格的发生一次。

  > 与对任意函数进行`std::call_once`等效。
  >
  > 在单例模式中典型应用。
  >
  > 以前，这样的保证通过double-check lock来实现。相对于double-check，对于已经初始化过的静态变量的检查开销减少为一次非原子布尔值比较。

 it’s common to use “s_” to prefix static (static duration) local variables.

One of the most common uses for static duration local variables is for unique ID generators.

```c++
int generateID()
{
    static int s_itemID{0};
    return s_itemID++; // makes copy of s_itemID, increments the 
}
```

One good use for a **const static local variable** is when you have a function that needs to use a const value, but creating or initializing the object is expensive (e.g. you need to read the value from a database). 

Avoid `static` local variables unless the variable never needs to be reset.

> 局部静态变量使得函数有了状态，成为不可重入函数。

## global variable

In C++, variables can also be declared *outside* of a function. Such variables are called **global variables**.

By convention, global variables are declared at the top of a file, below the includes, but above any code. many developers prefix global variable identifiers with “g” or “g_” to indicate that they are global

```c++
#include <iostream>

// Variables declared outside of a function are global variables
int g_x {}; // global variable g_x

void doSomething()
{
    // global variables can be seen and used everywhere in the file
    g_x = 3;
    std::cout << g_x << '\n';
}

int main()
{
    doSomething();
    std::cout << g_x << '\n';

    // global variables can be seen and used everywhere in the file
    g_x = 5;
    std::cout << g_x << '\n';

    return 0;
}
// g_x goes out of scope here
```

- **Global variables have file scope and static duration**

  - **file scope** (also informally called **global scope** or **global namespace scope**)

  - Global variables are created when the program starts, and destroyed when it ends. This is called **static duration**.

  - Unlike local variables, which are uninitialized by default, static variables are zero-initialized by default.

    - Non-constant global variables can be optionally initialized:

      ```c++
      int g_x; // no explicit initializer (zero-initialized by default)
      int g_y {}; // zero initialized
      int g_z { 1 }; // initialized with value
      ```

    - As with all constants, constant global variables must be initialized.

      ```c++
      #include <iostream>
      
      const int g_x; // error: constant variables must be initialized
      constexpr int g_w; // error: constexpr variables must be initialized
      
      const int g_y { 1 };  // const global variable g_y, initialized with a value
      constexpr int g_z { 2 }; // constexpr global variable g_z, initialized with a value
      
      void doSomething()
      {
          // global variables can be seen and used everywhere in the file
          std::cout << g_y << '\n';
          std::cout << g_z << '\n';
      }
      
      int main()
      {
          doSomething();
      
          // global variables can be seen and used everywhere in the file
          std::cout << g_y << '\n';
          std::cout << g_z << '\n';
      
          return 0;
      }
      // g_y and g_z goes out of scope here
      ```


### Avoid global variables

- global variables make the program’s state unpredictable. 

-  Global variables are at the opposite end of the spectrum -- because they can be accessed anywhere, you might have to look through the entire program to understand their usage. 

- Global variables also make your program less modular and less flexible.

- Initialization of static variables (which includes global variables) happens as part of program startup, before execution of the `main` function. 

  - The first phase is called `static initialization`.

    ​	global variables with constexpr initializers (including literals) are initialized to those values. Also, global variables without initializers are zero-initialized.

  - The second phase is called `dynamic initialization`.

    ​	 global variables with non-constexpr initializers are initialized.

  Within a single file, global variables are generally initialized in order of definition (there are a few exceptions to this rule). Given this, you need to be careful not to have variables dependent on the initialization value of other variables that won’t be initialized until later.

  ```c++
  #include <iostream>
  
  int initx();  // forward declaration
  int inity();  // forward declaration
  
  int g_x{ initx() }; // g_x is initialized first
  int g_y{ inity() };
  
  int initx()
  {
      return g_y; // g_y isn't initialized when this is called
  }
  
  int inity()
  {
      return 5;
  }
  
  int main()
  {
      std::cout << g_x << ' ' << g_y << '\n';
  }
  ```

#### **when use non-const global variable**:

- A good example is a log file.

   It probably makes sense to define this as a global, because you’re likely to only have one log in a program and it will likely be used everywhere in your program.

-  the std::cout and std::cin objects are implemented as global variables

As a rule of thumb, any use of a global variable should meet at least the following two criteria: There should only ever be one of the thing the variable represents in your program, and its use should be ubiquitous throughout your program.

> singleton is a non-const global variable!.

#### **how to avoid**

If you do find a good use for a non-const global variable, a few useful bits of advice will minimize the amount of trouble you can get into. This advice isn’t only for non-const global variables, but can help with all global variables.

-  prefix all non-namespaced global variables with “g” or “g_”

- put them in a namespace

  ```c++
  // constexpr double gravity { 9.8 }; // unclear if this is a local or global variable from the name
  namespace constants
  {
      constexpr double gravity { 9.8 };
  }
  
  int main()
  {
      return 0;
  }
  ```

- instead of allowing direct access to the global variable, it’s a better practice to “encapsulate” the variable. 

  ```c++
  namespace constants
  {
      constexpr double gravity { 9.8 }; // has internal linkage, is accessible only by this file
  }
  
  double getGravity() // this function can be exported to other files to access the global outside of this file
  {
      // We could add logic here if needed later
      // or change the implementation transparently to the callers
      return constants::gravity;
  }
  ```

- don’t use the variable directly in your function body. Pass it in as an argument instead. 

  ```c++
  #include <iostream>
  
  namespace constants
  {
      constexpr double gravity { 9.8 };
  }
  
  // This function is only useful for calculating your instant velocity based on the global gravity
  double instantVelocity(int time)
  {
      return constants::gravity * time;
  }
  
  // This function can calculate the instant velocity for any gravity value (more useful)
  double instantVelocity(int time, double gravity)
  {
      return gravity * time;
  }
  
  int main()
  {
      std::cout << instantVelocity(5);
    	std::cout << instantVelocity(5, constants::gravity); // pass our constant to the function as a parameter
  }
  ```

  

### sharing global constant

nstead of redefining these constants in every file that needs them (a violation of the “Don’t Repeat Yourself” rule), it’s better to declare them once in a central location and use them wherever needed. 

#### Internal: Prior to C++17, the following is the easiest and most common solution:

1. Create a header file to hold these constants
2. Inside this header file, define a namespace
3. Add all your constants inside the namespace (make sure they’re *constexpr*)
4. \#include the header file wherever you need it

```c++
#ifndef CONSTANTS_H
#define CONSTANTS_H

// define your own namespace to hold constants
namespace constants
{
    // constants have internal linkage by default
    constexpr double pi { 3.14159 };
    constexpr double avogadro { 6.0221413e23 };
    constexpr double myGravity { 9.2 }; // m/s^2 -- gravity is light on this planet
    // ... other related constants
}
#endif


// main.cpp
#include "constants.h" // include a copy of each constant in this file

#include <iostream>

int main()
{
    std::cout << "Enter a radius: ";
    int radius{};
    std::cin >> radius;

    std::cout << "The circumference is: " << 2.0 * radius * constants::pi << '\n';

    return 0;
}
```

Because const globals have internal linkage, each .cpp file gets an independent version of the global variable that the linker can’t see. In most cases, because these are const, the compiler will simply optimize the variables away.

> 1. Changing a single constant value would require recompiling every file that includes the constants header, which can lead to lengthy rebuild times for larger projects.
> 2. If the constants are large in size and can’t be optimized away, this can use a lot of memory.

#### External:shared across all files

define the constants in a .cpp file (to ensure the definitions only exist in one place), and put forward declarations in the header (which will be included by other files).

> use const instead of constexpr in this method because constexpr variables can’t be forward declared

```c++
// constants.h
#ifndef 	CONSTANTS_H
#define		CONSTATNS_H

namespace cnostants
{
  // since the actual variables are inside a namespace, the forward declarations need to be inside a namespace as well
  extern const double pi;
  extern const double avofadro;
  extern const double myGravity;
}

#ENDIF //CONSTANTS_H


// constants.cpp
#include "constants.h"

namespace constants
{
  // actual global variables
  extern const double pi{3.14156};
  extern const double avogadro {6.0221413e23};
  extern const double myGravity{9.2};// m/s^2 -- gravity is light on this planet
}



/// main.cpp

#include "constants.h" // include all the forward declarations

#include <iostream>

int main()
{
  std::cout << "Enter a radius: ";
  int radius{};
  std::cin >> radius;
  
  std::cout << "The circumference is: " << 2.0 * radius * constants::pi << '\n';
}
```

> Because global symbolic constants should be namespaced (to avoid naming conflicts with other identifiers in the global namespace), the use of a “g_” naming prefix is not necessary.

这种方法也有一些缺点：

- 只在定义的翻译单元是编译时变量，而通过头文件引入的前置声明会被视作运行时常量，因此在某些需要编译时常量的场景下不能使用。

  > such as array sizes

- 编译时常量能够进行更彻底的无副作用优化，而运行时常量能做的优化要更少一些。



> 综合上述两种方法：
>
> Given the above downsides, prefer defining your constants in the header file. If you find that for some reason those constants are causing trouble, you can move some or all of them into a .cpp file as needed.



#### inline global constants

C++17 introduced a new concept called `inline variables`。an **inline variable** is one that is allowed to be defined in multiple files without violating the one definition rule

> In C++, the term `inline` has evolved to mean “multiple definitions are allowed”

Inline global variables have external linkage by default.

The linker will consolidate all inline definitions of a variable into a single variable definition (thus meeting the one definition rule).**This allows us to define variables in a header file and have them treated as if there was only one definition in a .cpp file somewhere. **

> Let’s say you have a normal constant that you’re #including into 10 code files. Without inline, you get 10 definitions. With inline, the compiler picks 1 definition to be the canonical definition, so you only get 1 definition. This means you save 9 constants worth of memory.

These variables will also retain their constexpr-ness in all files in which they are included, so they can be used anywhere a constexpr value is required. Constexpr values can also be more highly optimized by the compiler than runtime-const (or non-const) variables.

**Inline variables have two primary restrictions that must be obeyed:****

1. All definitions of the inline variable must be identical (otherwise, undefined behavior will result).
2. The inline variable definition (not a forward declaration) must be present in any file that uses the variable.

```c++
// constants.j
#ifndef CONSTANTS_H
#define CONSTANTS_H

//define your own namespace to hold constants
namespace constants {
  inline constexpr double pi{3.14159}; // note: now inline constexpr
  inline constexpr double avogadro{6.0221413e23};
  inline constexpr double myGravity{9.2}; // m/s^2 -- gravity is a light on this planet
}

#endif
```

```c++
// main.cpp
#include "constants.h"

#include <iostream>

int main()
{
    std::cout << "Enter a radius: ";
    int radius{};
    std::cin >> radius;

    std::cout << "The circumference is: " << 2.0 * radius * constants::pi << '\n';

    return 0;
}
```

> We can include `constants.h` into as many code files as we want, but these variables will only be instantiated once and shared across all code files.

This method does retain the downside of requiring every file that includes the constants header to be recompiled if any constant value is changed. 

If you need global constants and your compiler is C++17 capable, prefer defining inline constexpr global variables in a header file.
