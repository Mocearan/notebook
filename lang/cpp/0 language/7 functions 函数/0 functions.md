# functions

​		A **function** is a collection of statements that executes sequentially

---

​		Functions are C++ entities that associate a sequence of [statements](https://en.cppreference.com/w/cpp/language/statements) (a *function body*) with a *name* and a list of zero or more *function parameters*.

​		A **function** is a reusable sequence of statements designed to do a particular job.

​		Functions provide a way for us to split our programs into small, modular chunks that are easier to organize, test, and use. 

​		A **function call** is an expression that tells the CPU to interrupt the current function and execute another function. 

```c++
return-type identifier() // identifier replaced with the name of your function
{
// Your code here
}
```



## 信息传递

​		程序中语言本身提供的信息传递机制有三种：函数调用，类对象中成员共享，全局变量。

​		在程序之外，通过操作系统提供的多种机制也可以进行信息传递：文件，socket，管道，共享内存等本地IPC或网络。



### 信息传递的方式

​		信息传递可以根据需要选择不同的方式，关注的重点在于：

- 传递是拷贝、共享还是留空移动的

- 共享的传递是否可变

  



## 函数四元素

​		The basic semantics of argument passing and function value return are that of initialization (§[3.4](ch03.xhtml 

### 返回值

​		Return values provide a way for functions to return a single value back to the function’s caller. a function can only return a single value back to the caller each time it is called. 

​		When the return statement is executed, the return value is copied from the function back to the caller. This process is called **return by value**.

​		 **Return by reference** returns a reference that is bound to the object being returned, which avoids making a copy of the return value. 

> ​		*must* be sure that the object being referenced outlives the function returning the reference. Otherwise, the reference being returned will be left dangling (referencing an object that has been destroyed), and use of that reference will result in undefined behavior.
>
> ```c++
> const std::string& getProgramName() // returns a const reference
> {
>     static const std::string s_programName { "Calculator" }; // has static duration, destroyed at end of program
> 
>     return s_programName;
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
>     return (x > y) ? x : y;
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



#### void return values

​		Functions are not required to return a value. 



#### The debate over use of early returns

​		There’s a similar argument to be made for return statements. A return statement that is not the last statement in a function is called an **early return**. Many programmers believe early returns should be avoided. A function that only has one return statement at the bottom of the function has a simplicity to it -- you can assume the function will take its arguments, do whatever logic it has implemented, and return a result without deviation. Having extra returns complicates the logic.

​		The counter-argument is that using early returns allows your function to exit as soon as it is done, which reduces having to read through unnecessary logic and minimizes the need for conditional nested blocks, which makes your code more readable.

​		**Some developers take a middle ground, and only use early returns at the top of a function to do parameter validation (catch bad arguments passed in), and then a single return thereafter.**



#### return aggregate type

​		Structs are usually returned by value, so as not to return a dangling reference.

​		也可以返回一个临时对象。

```c++
return Point3d { 0.0, 0.0, 0.0 }; // return an unnamed Point3d
```

​		函数返回类型显式声明，而不是使用类型推断(`auto return`)的情况下，甚至可以在return语句中省略该类型：

```c++
 return { 0.0, 0.0, 0.0 }; // return an unnamed Point3d
```



#### return reference

​		函数返回值可以是引用类型，但返回引用时要注意所返回的对象的生命周期，避免造成空悬引用。

​		一般用于类返回类的成员引用。

​		可以使用`const &`返回来避免不经类接口的对类成员的修改。



### 函数名

### 参数列表

#### 形参（parameters）& 实参（arguments）

​		A **function parameter** is a **variable** used in a function. 

​		Function parameters work almost identically to variables defined inside the function, but with one difference: they are always initialized with a value provided by the caller of the function.

​		An **argument** is a **value** that is passed *from* the caller *to* the function when a function call is made



#### 参数列表

​		Function parameters are defined in the function declaration by placing them in between the parenthesis after the function identifier, with multiple parameters being separated by commas.

​		Note that multiple arguments are also separated by commas.



#### 机理

​		Therefore, we can conclude that C++ really passes everything by value! The properties of pass by address (and reference) come solely from the fact that we can dereference the passed address to change the argument, which we can not do with a normal value parameter!

​		参数传递的本质是声明与初始化，使用调用函数时传入的值作为初始化器，初始化一个函数栈帧上局部作用域的变量。

##### pass by value

​		When a function is called, all of the parameters of the function are created as variables, and the value of each of the arguments is *copied* into the matching parameter. This process is called **pass by value**.

##### pass by address

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



#####  pass by reference

​		When using **pass by reference**, we declare a function parameter as a reference type (or const reference type) rather than as a normal type.

> When the function is called, each reference parameter is bound to the appropriate argument. Because the reference acts as an alias for the argument, no copy of the argument is made.

​	**Pass by reference allows us to change the value of an argument**



​		Pass fundamental types by value, and class (or struct) types by const reference.

> ​		Because class types can be expensive to copy (sometimes significantly so), class types are usually passed by const reference instead of by value to avoid making an expensive copy of the argument.

​		the cost of copying an object is generally proportional to two things:

- The size of the object. 
- Any additional setup costs.

>  		Some class types do additional setup when they are instantiated (e.g. such as opening a file or database, or allocating a certain amount of dynamic memory to hold an object of a variable size). 
>						
>  		It’s best to assume that most standard library classes have setup costs, unless you know otherwise that they don’t.

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



### 函数体

