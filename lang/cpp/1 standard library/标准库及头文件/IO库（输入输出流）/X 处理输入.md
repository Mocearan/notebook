# 处理输入的错误

---



​		When the extraction operator is used, the following procedure happens:

- If there is data already in the input buffer, that data is used for extraction.
- If the input buffer contains no data, the user is asked to input data for extraction (this is the case most of the time). When the user hits enter, a ‘\n’ character will be placed in the input buffer.
- operator>> extracts as much data from the input buffer as it can into the variable (ignoring any leading whitespace characters, such as spaces, tabs, or ‘\n’).
- Any data that can not be extracted is left in the input buffer for the next extraction.



​		We can generally separate input text errors into four types:

- Input extraction succeeds but the input is meaningless to the program (e.g. entering ‘k’ as your mathematical operator).
- Input extraction succeeds but the user enters additional input (e.g. entering ‘*q hello’ as your mathematical operator).
- Input extraction fails (e.g. trying to enter ‘q’ into a numeric input).
- Input extraction succeeds but the user overflows a numeric value.



## **Error case 1: Extraction succeeds but input is meaningless**

​		The solution here is simple: do input validation. This usually consists of 3 steps:

1. Check whether the user’s input was what you were expecting.
2. If so, return the value to the caller.
3. If not, tell the user something went wrong and have them try again.



## **Error case 2: Extraction succeeds but with extraneous input**

​		从输入中读取到了预期的数据，但还有一部分多余的数据也被输入并留在缓冲区中。

> when the extraction operator was called, it sees “*7\n” waiting in the buffer to be extracted, so it uses that instead of asking the user for more input. 

​		It would be better if any extraneous characters entered were simply ignored.To ignore everything up to and including the next ‘\n’ character, we call

```c++
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```

```c++
#include <limits> // for std::numeric_limits

void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


std::cin >> x;
ignoreLine();
```



## **Error case 3: Extraction fails**

​		输入与预期的数据不符。

​		Two things happen at this point: data is left in the buffer, and std::cin goes into “failure mode”.

```c++
if (std::cin.fail()) // has a previous extraction failed?
{
    // yep, so let's handle the failure
    std::cin.clear(); // put us back in 'normal' operation mode
    ignoreLine(); // and remove the bad input
}
```

`std::cin` has a Boolean conversion indicating whether the last input succeeded

```c++
if (!std::cin) // has a previous extraction failed?
```

eg:

```c++
double getDouble()
{
    while (true) // Loop until user enters a valid input
    {
        std::cout << "Enter a double value: ";
        double x{};
        std::cin >> x;

        if (!std::cin) // has a previous extraction failed?
        {
            // yep, so let's handle the failure
            std::cin.clear(); // put us back in 'normal' operation mode
            ignoreLine(); // and remove the bad input
        }
        else // else our extraction succeeded
        {
            ignoreLine();
            return x; // so return the value we extracted
        }
    }
}
```



## Error case 4: Extraction succeeds but the user overflows a numeric value

​		数据范围溢出。

​		Additional inputs are skipped, leaving y with the initialized value of 0. We can handle this kind of error in the same way as a failed extraction.

​		A failed extraction due to invalid input will cause the variable to be zero-initialized. Zero initialization means the variable is set to 0, 0.0, “”, or whatever value 0 converts to for that type.





## eg

```c++
#include <iostream>
#include <limits>

void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double getDouble()
{
    while (true) // Loop until user enters a valid input
    {
        std::cout << "Enter a double value: ";
        double x{};
        std::cin >> x;

        // Check for failed extraction
        if (!std::cin) // has a previous extraction failed?
        {
            // yep, so let's handle the failure
            std::cin.clear(); // put us back in 'normal' operation mode
            ignoreLine(); // and remove the bad input
            std::cerr << "Oops, that input is invalid.  Please try again.\n";
        }
        else
        {
            ignoreLine(); // remove any extraneous input
            return x;
        }
    }
}

char getOperator()
{
    while (true) // Loop until user enters a valid input
    {
        std::cout << "Enter one of the following: +, -, *, or /: ";
        char operation{};
        std::cin >> operation;
        ignoreLine(); // // remove any extraneous input

        // Check whether the user entered meaningful input
        switch (operation)
        {
        case '+':
        case '-':
        case '*':
        case '/':
            return operation; // return it to the caller
        default: // otherwise tell the user what went wrong
            std::cerr << "Oops, that input is invalid.  Please try again.\n";
        }
    } // and try again
}

void printResult(double x, char operation, double y)
{
    switch (operation)
    {
    case '+':
        std::cout << x << " + " << y << " is " << x + y << '\n';
        break;
    case '-':
        std::cout << x << " - " << y << " is " << x - y << '\n';
        break;
    case '*':
        std::cout << x << " * " << y << " is " << x * y << '\n';
        break;
    case '/':
        std::cout << x << " / " << y << " is " << x / y << '\n';
        break;
    default: // Being robust means handling unexpected parameters as well, even though getOperator() guarantees operation is valid in this particular program
        std::cerr << "Something went wrong: printResult() got an invalid operator.\n";
    }
}

int main()
{
    double x{ getDouble() };
    char operation{ getOperator() };
    double y{ getDouble() };

    printResult(x, operation, y);

    return 0;
}
```





# input validation 输入验证

​		输入验证是验证用户输入是否满足预期的设定。

---

​		输入验证通常分为两种：字符串和数值。

### 字符分类

​		字符串库在字节字符串库的`<cctype>`中提供了一组用于字符检查的函数。

```c++
namespace std {
  int isalnum(int c);
  int isalpha(int c);
  int isblank(int c);
  int iscntrl(int c);
  int isdigit(int c);
  int isgraph(int c);
  int islower(int c);
  int isprint(int c);
  int ispunct(int c);
  int isspace(int c);
  int isupper(int c);
  int isxdigit(int c);
  int tolower(int c);
  int toupper(int c);
}
```

​		可以用来检查指定的字符是数字还是字母。

## 字符串验证

​		所有用户输入都将作为字符串接收，然后根据格式是否恰当来接受或者拒绝。

​		大多数语言中，这是通过正则表达式来完成的，但是正则表达式比手动书写字符串验证条件性能要低，所以只有在不考虑性能影响，或手动验证条件太多太繁琐的情况下才使用正则表达式验证。





## 数值验证

​		数值验证通常考虑用户输入是否在一个给定的范围内。还需要处理用户输入的非数值内容。-
