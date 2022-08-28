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



## 值类别 Value categories

​		The **value category** of an expression indicates whether an expression resolves to a value, a function, or an object of some kind.

Prior to C++11, there were only two possible value categories: `lvalue` and `rvalue`.In C++11, three additional value categories (`glvalue`, `prvalue`, and `xvalue`) were added to support a new feature called `move semantics`.



### Lvalue

​		An **lvalue** (pronounced “ell-value”, short for “left value” or “locator value”, and sometimes written as “l-value”) is an expression that evaluates to a function or object that has an identity.

> An object or function has an identity if it has an identifier (such as a variable or named function) or an identifiable memory address (one that can be retrieved using operator&)

​		 Identifiable objects persist beyond the scope of the expression.

```c++
int x{};
std::cout << x << '\n'; // x is an lvalue expression
```

​		Since the introduction of constants into the language, lvalues come in two subtypes:

- a **modifiable lvalue** is an lvalue whose value can be modified. 
- A **non-modifiable lvalue** is an lvalue whose value can’t be modified (because the lvalue is const or constexpr).

```c++
int x{};
const double d{};
```



### rvalue

​		An **rvalue** (pronounced “arr-value”, short for “right value”, and sometimes written as `r-value`) is an expression that is not an l-value. 

> Commonly seen rvalues include literals (except string literals, which are lvalues) and the return value of functions or operators. 

​		Rvalues only exist within the scope of the expression in which they are used.

```c++
int return5()
{
    return 5;
}


int x{ 5 }; // 5 is an rvalue expression
const double d{ 1.2 }; // 1.2 is an rvalue expression

std::cout << x << '\n'; // x is a modifiable lvalue expression
std::cout << d << '\n'; // d is a non-modifiable lvalue expression

std::cout << return5(); // return5() is an rvalue expression (since the result is returned by value)
std::cout << x + 1 << '\n'; // x + 1 is a rvalue
std::cout << static_cast<int>(d) << '\n'; // the result of static casting d to an int is an rvalue
/*  
	these expressions produce values that must be used immediately (within the scope of the expression) or they are discarded.
*/
```

​		In C++11, rvalues are broken into two subtypes: prvalues and xvalues

​		 lvalues can implicitly convert to rvalues, so an lvalue can be used wherever an rvalue is required.

```c++
int x { 2 };
x = x + 1;
```



> lvalues expressions are those that evaluate to variables or other identifiable objects that persist beyond the end of the expression.
> rvalues expressions are those that evaluate to literals or the returned value of functions and operators that are discarded at the end of the expression.

## 求值顺序



