# function declaration

函数声明引入函数名和它的类型。函数定义将函数名/类型与函数体关联。

---





## parameter list

### default arguments

A **default argument** is a default value provided for a function parameter.

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

