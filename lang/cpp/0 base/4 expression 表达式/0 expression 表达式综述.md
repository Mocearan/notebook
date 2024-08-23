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

​		表达式的类型等同于由求值表达式产生的值、对象或函数的类型。

​		请注意，表达式的类型必须在编译时确定（否则类型检查和类型推断将不起作用）——但是，表达式的值可以在编译时（如果表达式是constexpr）或运行时（如果该表达式不是constexpr的话）确定。



## 复合表达式

​		复合表达式（compound expression）是指含有两个或多个运算符的表达式。

​		求复合表达式的值需要首先将运算符和运算对象根据运算符的优先级和结合性合理地组合在一起，优先级与结合律决定了运算对象组合的方式。

​		

## 求值顺序

​		c++没有明确规定表达式中的子表达式的求值顺序，特别是不能假定表达式是从左到右求值的。

- 括号打破优先级和结合律

- 不能假定表达式是从左到右求值

  ```cpp
  int i = f1() * f2();
  ```

  - 不能假定表达式是从左到右求值
  - 无法知道到底f1在f2之前调用还是f2在f1之前调用

- 避免在同一条表达式中使用超过一个运算符读写一个对象

  ```c++
  v[i] = i++;
  
  // 除非只用了一个运算符
  i++;
  i+=1;
  ```

- 逻辑与，逻辑非三个运算符左侧运算对象先被求值

  - 逻辑与，逻辑非进行短路求值

- 三目运算符先求左侧运算对象

- 逗号运算符先求左侧运算对象

  - 区分形参列表和逗号表达式

    - 避免使用逗号表达式给单一参数赋值

      ```c++
      f(v[i], i++); // 语法错误
      f((v[i], i++)); // 具有迷惑性
      ```























