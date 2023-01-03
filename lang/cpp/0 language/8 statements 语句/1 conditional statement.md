# conditional statement

A **conditional statement** is a statement that specifies whether some associated statement(s) should be executed or not.

---



## if

```c++
if (condition)
    ctrue_statement;
else if(confition2)
    cond2_true_statement;
...
else
  	false_statuement;
```

​		each branch may be a  compound statement(block).

​		an `else statement` is paired up with the last unmatched `if statement` in the same block.

```c++
if (x >= 0) // outer if statement
        // it is bad coding style to nest if statements this way
        if (x <= 20) // inner if statement
            std::cout << x << " is between 0 and 20\n";

    // which if statement does this else belong to?
    else
        std::cout << x << " is negative\n";

///////////////////////////////////////////////////////

if (x >= 0) // outer if statement
{
    if (x <= 20) // inner if statement
        std::cout << x << " is between 0 and 20\n";
    else // attached to inner if statement
        std::cout << x << " is negative\n";
}
```

Nested `if statements` can often be flattened by either restructuring the logic or by using logical operators  

```c++
if (x < 0)
    std::cout << x << " is negative\n";
else if (x <= 20) // only executes if x >= 0
    std::cout << x << " is between 0 and 20\n";
else // only executes if x > 20
    std::cout << x << " is greater than 20\n";

```

```c++
    if (x > 0 && y > 0) // && is logical and -- checks if both conditions are true
        std::cout << "Both numbers are positive\n";
    else if (x > 0 || y > 0) // || is logical or -- checks if either condition is true
        std::cout << "One of the numbers is positive\n";
    else
        std::cout << "Neither number is positive\n";
```

​		The most common case is testing a variable against **0** (or the **nullptr**). To do that, simply leave out the explicit mention of the condition. 

### if with init-statement

​		与for语句一样，在if语句的条件下声明名称的目的是限制变量的范围，以提高可读性并最小化错误。

```c++
if ( init-statement condition ) statement-true
if ( init-statement condition ) statement-true else statement-false
```





```c++
// std::lock_guard, we don't know that, but it's some type. We know what types are!
if (std::lock_guard lock(mx); shared_flag) { unsafe_ping(); shared_flag = false; }

// Whew, no thanks!
if (auto keywords = {"if", "for", "while"};
    std::any_of(keywords.begin(), keywords.end(),
                [&s](const char* kw) { return s == kw; })) 
```





## switch

chain many if-else statements together is both difficult to read and inefficient.

testing a variable or expression for equality against a set of different values is common, **switch statement** that is specialized for this purpose.

```c++
#include <iostream>

void printDigitName(int x)
{
    switch(x) {
        case 1 : 
            std::cout <<"one";
            return ;
        case 2:
            std::cout <<"two";
            return ;
        case 3:
            std::cout << "three";
            return ;
        default:
            std::cout <<"Unknown";
            return;
    }
}
```

Often the expression is just a single variable, but it can be any valid expression.The one restriction is that the condition must evaluate to an integral type or an enumerated type or be convertible to one.

> because switch statements are designed to be highly optimized.
>
> Historically, the most common way for compilers to implement switch statements is via [Jump tables](https://en.wikipedia.org/wiki/Branch_table) -- and jump tables only work with integral values.
>
> a jump table works much like an array, an integral value is used as the array index to “jump” directly to a result. This can be much more efficient than doing a bunch of sequential comparisons.
>
> There is technically no reason that C++ couldn’t relax the restriction so that other types could be used as well, they just haven’t done so yet (as of C++20).

`case` keyword and followed by a constant expression. The constant expression must either match the type of the condition or must be convertible to that type.

If the conditional expression does not match any case label and a default label exists, execution begins at the first statement after the default label.

Each set of statements underneath a label should end in a `break statement` or a `return statement`.



### fallthrough

When execution flows from a statement underneath a label into statements underneath a subsequent label, this is called **fallthrough**.

Once the statements underneath a case or default label have started executing, they will overflow (fallthrough) into subsequent cases. `Break` or `return` statements are typically used to prevent this.

Since fallthrough is rarely desired or intentional, many compilers and code analysis tools will flag fallthrough as a warning. Sometime  fallthrough is intended,  the compiler and code analysis tools don’t know. To help address this, C++17 adds a new attribute called `[[fallthrough]]`.

> **Attributes** are a modern C++ feature that allows the programmer to provide the compiler with some additional data about the code. 

The `[[fallthrough]]` attribute modifies a `null statement` to indicate that fallthrough is intentional (and no warnings should be triggered):

```c++
int main()
{
    switch(2)
    {
        case 1:
            std::cout << 1 << '\n';
            break;
        case 2:
            std::cout << 2 << '\n';
        	[[fallthrough]]; // intentional fallthrough -- note the semicolon to indicate the null statement
        case 3:
            std::cout <<3 <<'\n';
            break;
    }
}
```



### scope

`case`是一个标签语句，所以`case`并不会隐式的划分块作用域。默认的switch语句内部是一个整体的块，除非显式的使用`{}`来创建一个内部的块。

```c++
switch (1)
{
    int a; // okay: declaration is allowed before the case labels
    int b{ 5 }; // illegal: initialization is not allowed before the case labels

    case 1:
        int y; // okay but bad practice: declaration is allowed within a case
        y = 4; // okay: assignment is allowed
        break;

    case 2:
        int z{ 4 }; // illegal: initialization is not allowed if subsequent cases exist
        y = 5; // okay: y was declared above, so we can use it here too
        break;

    case 3:
        break;
}
```

> 因为声明是在编译期执行，而初始化是在运行期进行的。
>
> 依据`switch`的判定，执行流必然会从某个`case`（包括`default`)开始执行，在所有`case`之前的语句，都不能在运行期得到执行。所以`case`之前只能进行声明，而不能进行初始化。（`a`, `b`）

If defining variables used in a case statement, do so in a block inside the case.

```c++
switch(1)
{
    case 1:
    {
        // note addition of explicit block here
        int x{4}; // okay, variables can be initialized inside a block inside a case
        std::cout << x;
        break;
    }
    default:
        std::cout << "default case\n";
        break;
}
```

