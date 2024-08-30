# 运算符

​		The specific operation to be performed is denoted by a construct (typically a symbol or pair of symbols) called an **operator**.

​		表示要执行的具体操作的结构（一个或一对符号）称为运算符。

---

​		运算符作为输入的操作数的数量称为运算符的元（arity）。

​		C++中的运算符有三种不同的变体：

- **一元**运算符作用于一个操作数。
- **二元**运算符作用于两个操作数（称为*left*和*right*）。
- **三元**运算符作用于三个操作数。

> 根据使用方式，同一运算符可能具有多种含义。



##  常见运算符

![image-20220814192726068](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192726068.png)

- 算数运算符的结果类型通常经由算数类型转换决定，目的是产生一个最宽泛的运算对象类型。

- 对于接受左值运算对象的运算符来说，其结果是一个表示该左值运算对象的左值：

  ```c++
  int j = x = y;
  int *p = &++x;
  int *q = *(x++); // err, 不同于前置++,(x++)的结果是一个右值
  int* p2 = &(x>y?x:y); // 只要x y都是左值且类型相同，运算符的结果是一个同类型的左值
  int& r = (x<y)?x:1; // err, x 1类型不同，1不是左值
  ```

  - 指针相减的结果是`ptrdiff_t`

- 对于运算结果上溢、下溢，除零这类运算异常，不会抛出标准异常

​		还有一种特殊的字符——空白符（空格、制表符、换行等），常用来分隔单词，形式表意。除在文本中作为分隔外，其他情况下直接被编译器忽略。

## 特殊运算符

![image-20220814192745527](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192745527.png)

- `sizeof`的结果是`size_t`

  

### 类型转换运算符

- C风格转型
- `static_cast`
- `dynamic_cast`
- `const_cast`
- `reinterpret_cast`



## Table of operators

![image-20220814192805286](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192805286.png)

![image-20220814192825267](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192825267.png)

![image-20220814192843069](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220814192843069.png)

​		you can explicitly parenthesize the expression to make it evaluate the way you want. This works in C++ because parentheses have one of the highest precedence levels, so parentheses generally evaluate before whatever is inside them.



## 运算符优先级

​		defines the order in which operators are bound to their arguments

​		The order in which operators are evaluated in a compound expression is determined by an operator’s **precedence**. 

​		In C++, when the compiler encounters an expression, it must similarly analyze the expression and determine how it should be evaluated. To assist with this, all operators are assigned a level of precedence. Operators with the highest level of precedence are evaluated first.

​		

## 运算符结合性

​		如果两个具有相同优先级的操作符在表达式中彼此相邻，操作符的**结合性**告诉编译器是从左到右求值，还是从右到左求值。

​		复杂运算符构词应用最长匹配规则：`a++++1 => (a++)+(+1)`

## 替代表示

​		某些布局的键盘导致一些字符输入不便，还有的程序员不喜欢使用`&& / ~`等逻辑运算符。因此设计了一组关键词作为等效替代：

| ``and`` | ``and_eq`` | ``bitand`` | ``bitor`` | ``compl`` | ``not`` | ``not_eq`` | ``or`` | ``or_eq`` | ``xor`` | ``xor_eq`` |
| :-----: | :--------: | :--------: | :-------: | :-------: | :-----: | :--------: | :----: | :-------: | :-----: | :--------: |
|  `&&`   |   ``&=``   |    `&`     |    `|`    |    `~`    |   `!`   |    `!=`    |  `||`  |   `|=`    |   `^`   |    `^=`    |



## 运算对象转换

​		在表达式求值的过程中，运算对象常常由一种类型转换成另外一种类型。





## 运算符重载

​		makes it possible to specify the behavior of the operators with user-defined classes.

​		当运算符作用于类类型的运算对象时，用户可以自行定义其含义。因为这种自定义的过程事实上是为已存在的运算符赋予了另外一层含义，所以称之为重载运算符（overloaded operator）。
