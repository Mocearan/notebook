# function declaration

​		函数声明引入函数名和它的类型。函数定义将函数名/类型与函数体关联。

---

A function declaration may contain argument names. This can be a help to the reader of a program, but unless the declaration is also a function definition, the compiler simply ignores such names.

```c++
double sqrt(double d);
```

The type of a function consists of its return type followed by the sequence of its argument types in parentheses.

For such a *member function*, the name of its class is also part of the function type.



## parameter

​		Therefore, we can conclude that C++ really passes everything by value! The properties of pass by address (and reference) come solely from the fact that we can dereference the passed address to change the argument, which we can not do with a normal value parameter!

​		参数传递的本质是声明与初始化，使用调用函数时传入的值作为初始化器，初始化一个函数栈帧上局部作用域的变量。

### pass by value

​		When a function is called, all of the parameters of the function are created as variables, and the value of each of the arguments is *copied* into the matching parameter. This process is called **pass by value**.

### pass by address

​	 With **pass by address**, instead of providing an object as an argument, the caller provides an object’s *address* (via a pointer). 

```c++
void printByValue(std::string val) // The function parameter is a copy of str
{
    std::cout << val << '\n'; // print the value via the copy
}

void printByReference(const std::string& ref) // The function parameter is a reference that binds to str
{
    std::cout << ref << '\n'; // print the value via the reference
}

void printByAddress(const std::string* ptr) // The function parameter is a pointer that holds the address of str
{
    std::cout << *ptr << '\n'; // print the value via the dereferenced pointer
}

printByValue(str); // pass str by value, makes a copy of str
printByReference(str); // pass str by reference, does not make a copy of str
printByAddress(&str); // pass str by address, does not make a copy of str

std::string* ptr { &str }; // define a pointer variable holding the address of str
printByAddress(ptr); // pass str by address, does not make a copy of str
```

​		just copying the pointer (holding the address of the object) from the caller to the called function. Since an address is typically only 4 or 8 bytes, a pointer is only 4 or 8 bytes, so copying a pointer is always fast.

​		Because this is the address of the actual argument object being passed (not a copy), if the function parameter is a pointer to non-const, the function can modify the argument via the pointer parameter.

​		If a function is not supposed to modify the object being passed in, the function parameter can be made a pointer to const.

​		When passing a parameter by address, care should be taken to ensure the pointer is not a null pointer before you dereference the value.

```c++
void print(int* ptr)
{
  if(!ptr) return; // if ptr is a null pointer, early return back to the caller
  ...
}
```

​		If a null pointer should never be passed to the function, an `assert` can be used instead (or also) (as asserts are intended to document things that should never happen):

```c++
void print(const int* ptr) // now a pointer to a const int
{
	assert(ptr); // fail the program in debug mode if a null pointer is passed (since this should never happen)

	// (optionally) handle this as an error case in production mode so we don't crash if it does happen
	if (!ptr)
		return;

	std::cout << *ptr;
}
```

​		传递数组时，数组名退化，成为按地址传递的指针。

```c++
// even though prime is the actual array, within this function it should be treated as a constant
void passArray(const int prime[5])
{
    // so each of these lines will cause a compile error!
    prime[0] = 11;
    prime[1] = 7;
    prime[2] = 5;
    prime[3] = 3;
    prime[4] = 2;
}

void printSize(int array[])
{
    std::cout << std::size(array) << '\n'; // Error
    std::cout << sizeof(array) / sizeof(array[0]) << '\n'; // warning, sizeof(array) get pointer size
}

void printSize(int array[5])
{
    std::cout << std::size(array) << '\n'; // Error
    std::cout << sizeof(array) / sizeof(array[0]) << '\n'; // warning, sizeof(array) get pointer size
}
```

> 可见，虽然定长数组带上了数组长度，但是并不能从数组名计算到数组的长度。该数组长度只作为标识以供使用。
>
> 并且`sizeof`数组名，只得到指针类型的尺寸。



###  pass by reference

​		When using **pass by reference**, we declare a function parameter as a reference type (or const reference type) rather than as a normal type.

> When the function is called, each reference parameter is bound to the appropriate argument. Because the reference acts as an alias for the argument, no copy of the argument is made.

​	**Pass by reference allows us to change the value of an argument**



​		Pass fundamental types by value, and class (or struct) types by const reference.

> ​		Because class types can be expensive to copy (sometimes significantly so), class types are usually passed by const reference instead of by value to avoid making an expensive copy of the argument.

​		the cost of copying an object is generally proportional to two things:

- The size of the object. 
- Any additional setup costs.

>  				Some class types do additional setup when they are instantiated (e.g. such as opening a file or database, or allocating a certain amount of dynamic memory to hold an object of a variable size). 
>  				
>  				It’s best to assume that most standard library classes have setup costs, unless you know otherwise that they don’t.

​		accessing an object through a reference is slightly more expensive than accessing an object through a normal variable identifier. 

- With a variable identifier, the compiler can just go to the memory address assigned to that variable and access the value. 
- With a reference, there usually is an extra step: the compiler must first determine which object is being referenced, and only then can it go to that memory address for that object and access the value.
- The compiler can also sometimes optimize code using objects passed by value more highly than code using objects passed by reference. This means code generated for objects passed by reference is typically slower than the code generated for objects passed by value.

​		 why we don’t pass everything by reference:

- For objects that are cheap to copy, the cost of copying is similar to the cost of binding, so we favor pass by value so the code generated will be faster.
- For objects that are expensive to copy, the cost of the copy dominates, so we favor pass by (const) reference to avoid making a copy.
- If you’re not sure whether an object is cheap or expensive to copy, favor pass by const reference.

> An object is cheap to copy if uses 2 or fewer “words” of memory (where a “word” is approximated by the size of a memory address) and it has no setup costs.
>
> ```c++
> // Evaluates to true if the type (or object) uses 2 or fewer memory addresses worth of memory
> #define isSmall(T) (sizeof(T) <= 2 * sizeof(void*))
> ```

​		Common types that are cheap to copy include all of the fundamental types, enumerated types, and std::string_view.
​		

​		Common types that are expensive to copy include std::array, std::string, std::vector, and std::ostream.

​		 Pass by reference has the same benefits as pass by address without the risk of inadvertently dereferencing a null pointer.

​		Pass by const reference has a few other advantages over pass by address.

- because an object being passed by address must have an address, only lvalues can be passed by address (as rvalues don’t have addresses). Pass by const reference is more flexible, as it can accept lvalues and rvalues.

  ```c++
  void printByReference(const int& ref) // The function parameter is a reference that binds to the argument
  {
      std::cout << ref << '\n'; // print the value via the reference
  }
  printByReference(5); // valid (because the parameter is a const reference)
  ```

- the syntax for pass by reference is natural, as we can just pass in literals or objects. 

> In modern C++, most things that can be done with pass by address are better accomplished through other methods. Follow this common maxim: “Pass by reference when you can, pass by address when you must”.

​	

​		One of the more common uses for pass by address is to allow a function to accept an “optional” argument.However, in many cases, function overloading is a better alternative to achieve the same result。

```c++
void greet(std::string* name=nullptr)
{
    std::cout << "Hello ";
    std::cout << (name ? *name : "guest") << '\n';
}

//////////////////////////

void greet()
{
    std::cout << "Hello guest\n";
}

void greet(const std::string& name) // name now passed by const reference
{
    std::cout << "Hello " << name << '\n';
}
```

​		 Just like we can pass a normal variable by reference, we can also pass pointers by reference  to allow a function to change what a pointer argument points to.

```c++
void nullify(int*& refptr) // refptr is now a reference to a pointer
{
    refptr = nullptr; // Make the function parameter a null pointer
}
```

> 等同于使用二级指针。
>
> `int&*` 是错误的语法，因为不能有指向引用的指针(因为指针必须保存对象的地址，而引用不是对象)。

​		If they can’t be optimized away entirely, references are normally implemented by the compiler using pointers. This means that behind the scenes, pass by reference is essentially just a pass by address (with access to the reference doing an implicit dereference).

​		不同于使用指针，使用左值引用的参数传递，在函数调用时，不能显式的以语法形式表征传入的参数会被函数内部所改变。

```c++
void increment(int& a)
{
    ++a;
}
int x = 10;
increment(x);

void increment(int* p)
{
	++(*p);	
}
int x= 10;
increment(&x);
```

​		从调用形式上无法判断传入的实参是否被修改，除非函数名能明确的表示对实参的修改，否则不要使用左值引用作为函数的形参。



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

  



### 默认参数

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

    

## return type

​		Return values provide a way for functions to return a single value back to the function’s caller. a function can only return a single value back to the caller each time it is called. 

​		When the return statement is executed, the return value is copied from the function back to the caller. This process is called **return by value**.

​		 **Return by reference** returns a reference that is bound to the object being returned, which avoids making a copy of the return value. 

> ​		*must* be sure that the object being referenced outlives the function returning the reference. Otherwise, the reference being returned will be left dangling (referencing an object that has been destroyed), and use of that reference will result in undefined behavior.
>
> ```c++
> const std::string& getProgramName() // returns a const reference
> {
>  static const std::string s_programName { "Calculator" }; // has static duration, destroyed at end of program
> 
>  return s_programName;
> }
> ```
>
> ​		Modern compilers will produce a warning or error if you try to return a local variable by reference (so the above program may not even compile), but compilers sometimes have trouble detecting more complicated cases.
>
> ​		 returning non-const static variables by reference is fairly non-idiomatic, and should generally be avoided.
>
> ​		Avoid returning references to non-const local static variables.
>
> ​		**It’s okay to return reference parameters by reference**
>
> ```c++
> // Takes two std::string objects, returns the one that comes first alphabetically
> const std::string& firstAlphabetical(const std::string& a, const std::string& b)
> {
> 	return (a < b) ? a : b; // We can use operator< on std::string to determine which comes first alphabetically
> }
> ```
>
> ​		If we had used pass by value and return by value, we would have made up to 3 copies of std::string (one for each parameter, one for the return value). By using pass by reference/return by reference, we can avoid those copies.
>
> ​		when a non-const reference is returned from a function, the caller can use the reference to modify the value being returned.
>
> ```c++
> // takes two integers by non-const reference, and returns the greater by reference
> int& max(int& x, int& y)
> {
>  return (x > y) ? x : y;
> }
> max(x, y) = 7; // sets the greater of x or y to 7
> ```
>
> 

​		**Return by address** works almost identically to return by reference, except a pointer to an object is returned instead of a reference to an object. 

> ​		The major advantage of return by address over return by reference is that we can have the function return `nullptr` if there is no valid object to return. 
>
> ​		The major disadvantage of return by address is that the caller has to remember to do a `nullptr` check before dereferencing the return value, otherwise a null pointer dereference may occur and undefined behavior will result.
>
> ​		Prefer return by reference over return by address unless the ability to return “no object” (using `nullptr`) is important.



​		document function with a comment indicating what the return values mean.

```c++
// Function asks user to enter a value
// Return value is the integer entered by the user from the keyboard
int getValueFromUser();
```



### void return values

​		Functions are not required to return a value. 



### return aggregate type

​		结构体通常按值返回，这样就不会返回悬空引用。C++11新标准规定，函数可以返回花括号包围的值的列表。

```c++
// 返回一个临时对象
return Point3d { 0.0, 0.0, 0.0 }; // return an unnamed Point3d
```

```c++
// 可以在return语句中省略该类型 
return { 0.0, 0.0, 0.0 }; // return an unnamed Point3d
```





### return reference

​		函数返回值可以是引用类型，但返回引用时要注意所返回的对象的生命周期，避免造成空悬引用。

- 一般用于类返回类的成员引用，避免返回局部对象的引用或指针
- 可以使用`const &`返回来避免不经类接口的对类成员的修改



### return array pointer

​		数组不能被拷贝，所以函数不能返回数组。不过，函数可以返回数组的指针或引用。

```c++
int (*func(int i))[10];
// func(int i) 函数
// *func(int i) 函数调用的结果可以解引用
// (*func(int i))[10] 函数调用的结果执行解引用操作
// int (*func(int i))[10] 表示数组中的元素是int类型
```

​		从语法上来说，要想定义一个返回数组的指针或引用的函数比较繁琐，但可以用以下方法来简化：

```c++
typedef int arrT[10];
using arrT = int[10];

arrT* func(int i);
```

​		还可以使用尾置返回和`decltype`来简化：

```c++
auto func(int i) -> int(*)[10];

int arr[10];
decltype(arr)* func(int i);
```





### early returns

​		There’s a similar argument to be made for return statements. A return statement that is not the last statement in a function is called an **early return**. Many programmers believe early returns should be avoided. A function that only has one return statement at the bottom of the function has a simplicity to it -- you can assume the function will take its arguments, do whatever logic it has implemented, and return a result without deviation. Having extra returns complicates the logic.

​		The counter-argument is that using early returns allows your function to exit as soon as it is done, which reduces having to read through unnecessary logic and minimizes the need for conditional nested blocks, which makes your code more readable.

​		**Some developers take a middle ground, and only use early returns at the top of a function to do parameter validation (catch bad arguments passed in), and then a single return thereafter.**



### trailing return

​		C++11新标准中使用尾置返回类型（trailing return type）可以简化返回复杂复合类型的函数声明。

```c++
auto func(int i) -> int(*)[10]; // 返回一个数组指针
```





## 函数指针

​		函数指针指向的是函数而非对象。

- 和其他指针一样，函数指针指向某种特定类型。
- 函数的类型由它的返回类型和形参类型共同决定，与函数名无关。

​		要想声明一个可以指向该函数的指针，只需要用指针替换函数名即可：

```c++
return_type (*fname)(params_list);
```

- pf前面有个＊，因此pf是指针；右侧是形参列表，表示pf指向的是函数

```c++
pf = lcompare;
pf = &lcompare;
// 二者等价，因为c语言的设计原因
// 因此能直接使用指向函数的指针调用该函数，无须提前解引用指针
pf("hello");
(*pf)("hello");
```

​		常作为函数形参来使用：

```c
void ub(const std::string& s1, bool (*pf)(const std::string& x));
```

​		还可以使用尾置返回类型的方式声明一个返回函数指针的函数：

```c++
auto f1(int) -> int (*)(int*, int);
```

​		或者使用`decltype`从一个已知的函数上获取函数类型

```c++
std::string::size_type largetLength(const std::string&, const std::string&);
decltype(largetLength)* getF(const std::string&);
```

