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



### 函数名

### 参数列表

#### 形参（parameters）& 实参（arguments）

​		A **function parameter** is a **variable** used in a function. 

​		Function parameters work almost identically to variables defined inside the function, but with one difference: they are always initialized with a value provided by the caller of the function.

​		An **argument** is a **value** that is passed *from* the caller *to* the function when a function call is made



#### 参数列表

​		Function parameters are defined in the function declaration by placing them in between the parenthesis after the function identifier, with multiple parameters being separated by commas.

​		Note that multiple arguments are also separated by commas.





### 函数体



## 递归

​		如果一个函数调用了它自身，不管这种调用是直接的还是间接的，都称该函数为递归函数（recursivefunction）。

​		在递归函数中，一定有某条路径是不包含递归调用的；否则，函数将“永远”递归下去，换句话说，函数将不断地调用它自身直到程序栈空间耗尽为止。

```c++
int f(int val)
{
    if(val > 1)
        return f(val - 1) * val;
    return 1;
}
```

