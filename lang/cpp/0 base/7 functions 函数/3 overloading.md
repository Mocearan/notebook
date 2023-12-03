# Overloading 重载

---

​		同一作用域中，多个函数

-   拥有相同名称

-   不同形参列表

    -   形参数量
    -   形参对应位置上的类型

-   或非静态成员内函数不同cv限定版本

    则各个函数实现之间，称为函数重载（overload）。

    > `返回值类型`和``nocexcept``不作为函数重载的条件。

​	重载函数的地址有特定的确定方法。（重载集地址）

​	函数重载也称为函数多态性（静态多态）。

>   静态多态：编译期确定，编译期绑定。
>
>   动态多态：运行期确定，运行期绑定。如通过虚函数实现的重写。



​		**Function overloading** allows us to create multiple functions with the same name, so long as each identically named function has different parameter types (or the functions can be otherwise differentiated).

​		Each function sharing a name (in the same scope) is called an **overloaded function** (sometimes called an **overload** for short).

​		Each function sharing a name (in the same scope) is called an **overloaded function** (sometimes called an **overload** for short).

​		Function overloading provides a great way to reduce the complexity of your program by reducing the number of function names you need to remember. It can and should be used liberally.

> overload based on name mangling



## overloading deferentiate

​		区别重载的条件：

- Number of parameters

- Type of parameters

  > parameters passed by value, the const qualifier is also not considered.
  >
  > ```c++
  > void print(int);
  > void print(const int); // not differentiated from print(int)
  > ```

  - ellipsis parameters are considered to be a unique type

    ```c++
    void foo(int x, int y);
    void foo(int x, ...); // differentiated from foo(int)
    ```

- member functions

  - const or volatile qualifier
  - ref-qualifiers

>  type aliases are not distinct from overloads using the aliased type.

> return type is not used to differentiate overloaded functions. 



### name managling

​		为实现重载，编译期实际上会对函数签名进行`name managling`，将名字按照编译期的规则进行重新编排，一般会将参数类型按照位置顺序带入名称，这样就能做到各个重载版本之间实际的名字不同。

​		`extern "C"`里的函数声明，将不会再编译时进行`name managling`.



