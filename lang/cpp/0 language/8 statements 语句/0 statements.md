# statements 语句

​		A computer program is a sequence of instructions that tell the computer what to do.

---

​		A **statement** is a type of instruction that causes the program to *perform some action*.

- smallest independent unit of computation in the C++ language

- end in a semicolon

  ​	a line that ends in a semicolon, it’s probably a statement.

​		不同于表达式的目的是求值，语句本身没有值，语句的作用是指定执行的顺序。

​		为了提高程序的性能，编译器可能会在确保执行结果不变的前提下调整代码的顺序。



## classification 分类

​		C++ includes the following types of statements：

- labeled statements;

- expression statements;

  ​	An **expression statement** is a statement that consists of an expression followed by a semicolon.

  ​	When the statement is executed, the expression will be evaluated (and the result of the expression will be discarded).

- compound statements;

- conditional statements;

- iteration statements（loop）;

- jump statements;

- declaration statements;

- try blocks;

- atomic and synchronized blocks (TM TS).



## 表达式语句

​		表达式的末尾加上分号就称为一条语句。

> 赋值、函数调用本身是表达式的一部分。

### 声明语句

​		一个声明就是一条语句。

​		除`static`变量外，当控制线程到达当前声明语句的时候，会同时执行初始化器。这样做的目的是为了减少由未初始化变量造成的程序错误，并且让代码的局部性更好。

> 在绝大多数情况下，如果没有为变量找到一个合适的值，就暂时不要声明它。

​		对于用户自定义的数据类型，先确定一个合适的初始化器再定义变量能获得更好的性能：

```c++
std::string s1;
s1 = "hello world";

std:;string s2{"hello world"};
```

​		缺少初始化器的变量声明常常是因为，需要后续一条专门的语句给变量赋值。



## compound statements 复合语句

​		A **compound statement** (also called a **block**, or **block statement**) is a group of *zero or more statements* that is treated by the compiler as if it were a single statement.

`{...}`

> Blocks begin with a `{` symbol, end with a `}` symbol, with the statements to be executed being placed in between. Blocks can be used anywhere a single statement is allowed. No semicolon is needed at the end of a block.

### function body is a block

```c++
int add(int x, int y)
{
    return x + y;
}
```

### conditional statement blocks

One of the most common use cases for blocks is in conjunction with `if statements`.

```c++
if(val >= 0 ) {
    std::cout << value ,<< "is a positive integer (or zero)\n";
    std::cout << "Double this number is " << value * 2 << '\n';
} else {
    std::cout << value << " is a negative integer\n";
    std::cout << "The positive od this number is " << -value << 
        '\n';
}
```

### nested blocks

blocks can be nested inside other blocks.

```c++
int main()
{
    {
        auto val = add(3,4);
    }
    
    return 0;
}
```

Despite what C++ technically supports, it’s a good idea to keep your nesting level to 3 or less. 

​	

## conditional statements 条件语句

- if-else statement
- switch statement



## jump statements 跳转语句





## **flow control** 控制流

 allow the programmer to change the normal path of execution through the program.

![image-20220526112139550](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220526112139550.png)



Halts

​		A **halt** is a flow control statement that terminates the program. In C++, halts are implemented as functions (rather than keywords), so our halt statements will be function calls.

- `std::exit / std::atexit`
- `std::quick_exit / std::at_quick_exit`
- `std::abort`
- `std::terminate`

​		Destroying local objects is an important part of C++ (particularly when we get into classes), and none of the above-mentioned functions clean up local variables. Exceptions are a better and safer mechanism for handling error cases.

​		Only use a halt if there is no safe way to return normally from the main function. If you haven’t disabled exceptions, prefer using exceptions for handling errors safely.





















































































