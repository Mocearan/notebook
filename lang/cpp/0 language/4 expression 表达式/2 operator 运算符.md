# 运算符

The specific operation to be performed is denoted by a construct (typically a symbol or pair of symbols) called an **operator**.

---

The number of operands that an operator takes as input is called the operator’s *arity*.

Operators in C++ come in three different *arities*:

**Unary** operators act on one operand. 

**Binary** operators act on two operands (known as *left* and *right*). 

**Ternary** operators act on three operands.

Note that some operators have more than one meaning depending on how they are used.



##  常见运算符

![image-20220814192726068](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192726068.png)



## 特殊运算符

![image-20220814192745527](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192745527.png)

### 类型转换运算符

类型转换运算符包括：

- C风格转型
- `static_cast`
- `dynamic_cast`
- `const_cast`
- `reinterpret_cast`

引起类型转换。

> **类型转换：**
>
> ​		转换位序列的类型语义。
>
> ps：c++/基本概念/实体/类型

## Table of operators

![image-20220814192805286](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192805286.png)

![image-20220814192825267](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192825267.png)

![image-20220814192843069](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192843069.png)

you can explicitly parenthesize the expression to make it evaluate the way you want. This works in C++ because parentheses have one of the highest precedence levels, so parentheses generally evaluate before whatever is inside them.

## 运算符优先级

defines the order in which operators are bound to their arguments

The order in which operators are evaluated in a compound expression is determined by an operator’s **precedence**. 

In C++, when the compiler encounters an expression, it must similarly analyze the expression and determine how it should be evaluated. To assist with this, all operators are assigned a level of precedence. Operators with the highest level of precedence are evaluated first.



## 运算符结合性

If two operators with the same precedence level are adjacent to each other in an expression, the operator’s **associativity** tells the compiler whether to evaluate the operators from left to right or from right to left. 



## 替代表示

are alternative spellings for some operators

## 运算符重载

makes it possible to specify the behavior of the operators with user-defined classes.
