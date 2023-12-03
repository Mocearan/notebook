# 表达式



------------

​		An **expression** is a combination of literals, variables, operators, and function calls that can be executed to produce a singular value. 

​		The process of executing an expression is called **evaluation**, and the single value produced is called the **result** of the expression.

​		When an expression is evaluated, each of the terms inside the expression are evaluated, until a single value remains. 

```cpp
type identifier { expression };
```

​		Values calculated in an expression are discarded at the end of the expression.

​		All expressions in C++ have two properties: a type and a value category.

## 表达式类型 expression type

​		The type of an expression is equivalent to the type of the value, object, or function that results from the evaluated expression.

​		Note that the type of an expression must be determinable at compile time (otherwise type checking and type deduction wouldn’t work) -- however, the value of an expression may be determined at either compile time (if the expression is constexpr) or runtime (if the expression is not constexpr).





## 求值顺序

​		c++没有明确规定表达式中的子表达式的求值顺序，特别是不能假定表达式是从左到右求值的。

- 避免在同一条表达式中使用超过一个运算符读写一个对象

  ```c++
  v[i] = i++;
  ```

  - 除非只用了一个运算符

    ```c++
    i++;
    i+=1;
    ```

- 逗号，逻辑与，逻辑非三个运算符左侧运算符先被求值

  - 逻辑与，逻辑非进行短路求值

  - 区分形参列表和逗号表达式

    - 避免使用逗号表达式给单一参数赋值

      ```c++
      f(v[i], i++); // 语法错误
      f((v[i], i++)); // 具有迷惑性
      ```























