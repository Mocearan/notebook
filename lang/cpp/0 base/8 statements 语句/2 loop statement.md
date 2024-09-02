# loop statement

​		Loops are control flow constructs that allow a piece of code to execute repeatedly until some condition is met.

---



## `while`

​		A `while statement` is declared using the **while** keyword. When a `while statement` is executed, the `condition` is evaluated. If the condition evaluates to `true`, the associated statement executes.

​		`while statement` will keep looping for as long as the condition evaluates to `true`

```c++
while (condition)
    statement;
```

​		Often, we want a loop to execute a certain number of times. To do this, it is common to use a **loop variable**, often called a **counter**. Loop variables should be of type (signed) int.



## `do-while`

​		A **do while statement** is a looping construct that works just like a while loop, except the statement always executes at least once. 

```c++
do
    statement; // can be a single statement or a compound statement
while (condition);
```

​		Favor while loops over do-while when given an equal choice.



### `do-while(0)`

​		`do-while(0)`的作用是使得多条语句成块。

- 一个应用是在编写宏函数时，使得多条语句成块避免编译错误。

  ```c++
  #define F(x) do { \
  	statement_one; \
  	statement_two; \
  }while(0)
  
  
  if(cond) 
      Foo(x);
  else
      ...
  ```

- 另一个应用是构成局部作用域，用以跳过代码，避免goto的使用

  ```c++
  void func()
  {
  	do {
          if(cond)
              break; // goto error handle
          
          // some work
      } while(0);
      
      
      // error handle
  }
  ```

  







## `for`

```c++
for (init-statement; condition; end-expression)
   statement
```

> ​		Defining multiple variables (in the init-statement) and using the comma operator (in the end-expression) is acceptable inside a `for statement`.

​		The easiest way to initially understand how a `for statement` works is to convert it into an equivalent `while statement`:

```c++
{ // note the block here
    init-statement; // used to define variables used in the loop
    while(condition) {
        statement;
        end-expression; // used to modify the loop variable prior to reassessement of the condition
    }
} // variables defined inside the loop go out of scope here
```

- First, the init-statement is executed. This only happens once when the loop is initiated.

  The init-statement is typically used for variable definition and initialization.

- Second, for each loop iteration, the condition is evaluated.

- Finally, after the statement is executed, the end-expression is evaluated. 

  Typically, this expression is used to increment or decrement the loop variables defined in the init-statement.

- After the end-expression has been evaluated, execution returns to the second step (and the condition is evaluated again).

```c++
#include <iostream>

int main()
{
    for (int count{ 1 }; count <= 10; ++count)
        std::cout << count << ' ';

    return 0;
}
```

> Prefer `for loops` over `while loops` when there is an obvious loop variable.
> Prefer `while loops` over `for loops` when there is no obvious loop variable.

## `range-for （for-each loops)`

```c++
for(element_declaration : container)
    statement;
```

​		`element_declaration` 具有相同的类型，因此，通常使用`auto`来进行类型推断。对于只读的复合类型，往往使用`const auto &`来进行访问。

​		`range-for`是来自于`begin()/end()`的语法糖，其底层实现为：

```c++
for(auto beg = container.begin(), end = container.end(); beg not_eq end; ++beg) {
    auto& r = *beg;
	statement;
}
```

​		所以支持`range-for`需要为被迭代的类中实现`begin() / end()`函数，或者允许通过`begin(x) / end(x)`得到相应的迭代器。

> 因此，使用范围for语句体内不应改变其所遍历序列的大小。



- 编译器首先查找可用的成员`begin / end`
  - 找到但不可用则`range-for`错误
- 否则，则外层作用域继续寻找`begin / end`
  - 如果找不到或找到的不能用，则`range-for`错误。

```c++
template<typename T>
T* begin(Container<T>& x) { return &x[0];}

template<typename T>
T* end(Container<T>& x) { return x.begin() + x.size(); }
```

​		对于内置数组，编译器用`arr / arr+N`来代替`begin / end`。】=、

```c++
constexpr int fibonacci[]{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
for (auto number : fibonacci)  { // iterate over array fibonacci
    std::cout << number << ' '; // we access the array element for this iteration through variable number
}
```

​		可以使用`auto &`来更易元素，但在`range-for`中避免进行更易容器的操作。因为对容器进行了增加或删除，`end()`的值可能就无效了。

​		对于非标准库定义的容器（无`std::initializer_list`)，仅支持`fixed array`，对于退化的数组和变长数组都是不支持的。

```c++
int sumArray(const int array[]) // err, array is a pointer
```

​		大多数容器不支持索引访问，因此，c++没有提供获取当前元素索引的方法。在`c++20`中，`range-for`可以定义初始化语句，从而较为内聚的自定义索引：

```c++
for(ini-statement; element_declaration : container)
    statement;
```

```c++
 for (int i{ 0 }; auto score : scores) // i is the 
     ...; ++i;
```

​		控制语句对于`range-for`来说也是可用的。





##` infinite loops `(死循环)

​		if the expression always evaluates to true, the while loop will execute forever.

​		The only way to exit an infinite loop is through a return statement, a break statement, an exit statement, a goto statement, an exception being thrown, or the user killing the program.

- while

  ```c++
  while(true) {...}
  ```

- do-while

  ```c++
  do {...} while(true);
  ```

- for

  ```c++
  for(;;) {...}
  ```

  

### `intentional infinite loops`

​		Favor `while(true)` for intentional infinite loops.

​		显式表征一个死循环，使程序不退出的循环执行某一流程。

​		It is common to see this kind of loop in web server applications that run continuously and service web requests.





## `no raw loop`

[algorithm - C++ "No raw loops" without losing perfomance - Stack Overflow](https://stackoverflow.com/questions/58530330/c-no-raw-loops-without-losing-perfomance)
