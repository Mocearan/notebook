# 错误处理方法论

---



## 测试

​		**Software verification** (a.k.a. **software testing**) is the process of determining whether or not the software works as expected in all cases.

​		测试的原则：Write your program in small, well defined units (functions or classes), compile often, and test your code as you go.



### informal testing

​		 write some code to test the unit that was just added, and then erase the test once the test passes. 

```c++
#include <iostream>

// We want to test the following function
bool isLowerVowel(char c)
{
    switch (c)
    {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
    default:
        return false;
    }
}

int main()
{
    // So here's our temporary tests to validate it works
    std::cout << isLowerVowel('a'); // temporary test code, should produce 1
    std::cout << isLowerVowel('q'); // temporary test code, should produce 0

    return 0;
}
```



		### **Preserving tests**

​		it can make more sense to preserve your tests so they can be run again in the future.

```c++
#include <iostream>

// Not called from anywhere right now
// But here if you want to retest things later
void testVowel()
{
    std::cout << isLowerVowel('a'); // temporary test code, should produce 1
    std::cout << isLowerVowel('q'); // temporary test code, should produce 0
}

int main()
{
    return 0;
}
```



**Automating test functions**

​		writing a test function that contains both the tests AND the expected answers and compares them

```c++
// returns the number of the test that failed, or 0 if all tests passed
int testVowel()
{
    if (isLowerVowel('a') != true) return 1;
    if (isLowerVowel('q') != false) return 2;

    return 0;
}
```





### unit testing framworks



### **Integration testing**

​		Once each of your units has been tested in isolation, they can be integrated into your program and retested to make sure they were integrated properly. This is called an **integration test**.



## 测试覆盖率

​		The term **code coverage** is used to describe how much of the source code of a program is executed while testing.



### **Statement coverage**

​		The term **statement coverage** refers to the percentage of statements in your code that have been exercised by your testing routines.

```c++
bool isLowerVowel(char c)
{
    switch (c) // statement 1
    {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true; // statement 2
    default:
        return false; // statement 3
    }
}
```

> This function will require two calls to test all of the statements, as there is no way to reach statement 2 and 3 in the same function call.



### **Branch coverage**

​		**Branch coverage** refers to the percentage of branches that have been executed, each possible branch counted separately.

```c++
bool isLowerVowel(char c)
{
    switch (c)
    {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
    default:
        return false;
    }
}
```

>  two calls will be needed to give you 100% branch coverage: one (such as `isLowerVowel('a')`) to test the first cases, and another (such as `isLowerVowel('q')`) to test the default case. 

```c++
void compare(int x, int y)
{
	if (x > y)
		std::cout << x << " is greater than " << y << '\n'; // case 1
	else if (x < y)
		std::cout << x << " is less than " << y << '\n'; // case 2
	else
		std::cout << x << " is equal to " << y << '\n'; // case 3
}
```

> 3 calls are needed to get 100% branch coverage here



### **Loop coverage**

​		**Loop coverage** (informally called **the 0, 1, 2 test**) says that if you have a loop in your code, you should ensure it works properly when it iterates 0 times, 1 time, and 2 times. 

```c++
#include <iostream>

void spam(int timesToPrint)
{
    for (int count{ 0 }; count < timesToPrint; ++count)
         std::cout << "Spam! ";
}
```

> To test the loop within this function properly, you should call it three times: `spam(0)` to test the zero-iteration case, `spam(1)` to test the one-iteration case, and `spam(2)` to test the two-iteration case. If `spam(2)` works, then `spam(n)` should work, where `n > 2`.



### **Testing different categories of input**

​		Here are some basic guidelines for category testing:

- For integers
  - how your function handles negative values, zero, and positive values
  -  should also check for overflow if that’s relevant.
- For floating point numbers,
  - how your function handles values that have precision issues (values that are slightly larger or smaller than expected).
  -  Good `double` type values to test with are `0.1` and `-0.1` (to test numbers that are slightly larger than expected) and `0.6` and `-0.6` (to test numbers that are slightly smaller than expected).
- For strings,
  - how your function handles an empty string (just a null terminator), normal valid strings, strings that have whitespace, and strings that are all whitespace.
- If your function takes a pointer
  - test `nullptr` as well 



## semantic errors 语义错误

​		`syntax errors`, which occur when you write code that is not valid according to the grammar of the C++ language. The compiler will notify you of these types of errors, so they are trivial to catch, and usually straightforward to fix.

​		`semantic errors`, which occur when you write code that does not do what you intended. The compiler generally will not catch semantic errors (though in some cases, smart compilers may be able to generate a warning).

> ​		Semantic errors can cause most of the same symptoms of `undefined behavior`, such as causing the program to produce the wrong results, causing erratic behavior, corrupting program data, causing the program to crash -- or they may not have any impact at all.

### Conditional logic errors

​		A **conditional logic error** occurs when the programmer incorrectly codes the logic of a conditional statement or loop condition.

```c++
#include <iostream>

int main()
{
    std::cout << "Enter an integer: ";
    int x{};
    std::cin >> x;

    if (x >= 5) // oops, we used operator>= instead of operator>
        std::cout << x << " is greater than 5";

    return 0;
}
```

```c++
#include <iostream>

int main()
{
    std::cout << "Enter an integer: ";
    int x{};
    std::cin >> x;

    // oops, we used operator> instead of operator<
    for (int count{ 1 }; count > x; ++count)
    {
        std::cout << count << ' ';
    }

    return 0;
}
```



### **Infinite loops**

```c++
#include <iostream>

int main()
{
    int count{ 1 };
    while (count <= 10) // this condition will never be false
    {
        std::cout << count << ' '; // so this line will repeatedly execute
    }

    return 0; // this line will never execute
}
```

```c++
#include <iostream>

int main()
{
    for (unsigned int count{ 5 }; count >= 0; --count)
    {
        if (count == 0)
            std::cout << "blastoff! ";
        else
          std::cout << count << ' ';
    }

    return 0;
}
```



### **Off-by-one errors**

​		An **off-by-one** error is an error that occurs when a loop executes one too many or one too few times.

```c++
for (unsigned int count{ 1 }; count < 5; ++count)
{
    std::cout << count << ' ';
}
```

> This code is supposed to print `1 2 3 4 5`, but it only prints `1 2 3 4`



### **Incorrect operator precedence**

```cpp
int x{ 5 };
int y{ 7 };

if (!x > y) // oops: operator precedence issue
    std::cout << x << " is not greater than " << y << '\n';
else
    std::cout << x << " is greater than " << y << '\n';
```

> ​		This can also happen when mixing Logical OR and Logical AND in the same expression (Logical AND takes precedence over Logical OR). Use explicit parenthesization to avoid these kinds of errors.



### **Precision issues with floating point types**

```c++
float f{ 0.123456789f };
    std::cout << f;
```

​		`operator==` and `operator!=` can be problematic with floating point numbers due to small rounding errors

```c++
double d{ 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 }; // should sum to 1.0

if (d == 1.0)
    std::cout << "equal";
else
    std::cout << "not equal";
```



### **Integer division**

​		both operands are integers, we end up doing an integer division instead:

```c++
#include <iostream>

int main()
{
    int x{ 5 };
    int y{ 3 };

    std::cout << x << " divided by " << y << " is: " << x / y; // integer division

    return 0;
}
```



### **Accidental null statements**

```c++
if (c=='y'); // accidental null statement here
        blowUpWorld(); // so this will always execute since it's not part of the if-statement
```



### **Not using a compound statement when one is required**

```c++
if (c=='y')
    std::cout << "Okay, here we go...\n";
	blowUpWorld(); // oops, will always execute.  Should be inside compound statement.
```

> dangling else



## 错误处理方法

​		There are three key places where assumption errors typically occur:

- When a function returns, the programmer may have assumed the called function was successful when it was not.
- When a program receives input (either from the user, or a file), the programmer may have assumed the input was in the correct format and semantically valid when it was not.
- When a function has been called, the programmer may have assumed the arguments would be semantically valid when they were not.

​		we defined **defensive programming** as the practice of trying to anticipate all of the ways software can be misused, either by end-users, or by developers (either the programmer themselves, or others). Once you’ve anticipated (or discovered) some misuse, the next thing to do is handle it.



​		There are 4 general strategies that can be used to **Handling errors in functions**:

- Handle the error within the function
- Pass the error back to the caller to deal with
- Halt the program
- Throw an exception

​		由于模块化和抽象机制的应用，运行时的错误捕获与处理往往是分离的。即，在抽象的组件中往往不能应付具体使用者面临的问题，能做的处理很有限；而使用者不能保证不出现问题。

​		因此，对于运行时错误的处理往往是由实现者通知使用者进行处理。



**Handling the error within the function**

```c++
void printDivision(int x, int y)
{
    if (y != 0)
        std::cout << static_cast<double>(x) / y;
    else
        std::cerr << "Error: Could not divide by zero\n";
}
```



### **Passing errors back to the caller**

​		In many cases, the error can’t reasonably be handled in the function that detects the error. 

```c++
bool printDivision(int x, int y)
{
    if (y == 0)
    {
        std::cerr << "Error: could not divide by zero\n";
        return false;
    }

    std::cout << static_cast<double>(x) / y;

    return true;
}
```

​		the caller can check the return value to see if the function failed for some reason.

​		In some cases, the full range of return values isn’t used. In such cases, we can use a return value that wouldn’t otherwise be possible to occur normally to indicate an error. 

```c++
// The reciprocal of x is 1/x, returns 0.0 if x=0
double reciprocal(double x)
{
    if (x == 0.0)
       return 0.0;

    return 1.0 / x;
}
```

​		if the full range of return values are needed, then using the return value to indicate an error will not be possible (because the caller would not be able to tell whether the return value is a valid value or an error value). In such a case, an `out parameter` might be a viable choice.



### **Fatal errors** : Halts

​		If the error is so bad that the program can not continue to operate properly, this is called a **non-recoverable** error (also called a **fatal error**). In such cases, the best thing to do is terminate the program.

```c++
double doDivision(int x, int y)
{
    if (y == 0)
    {
        std::cerr << "Error: Could not divide by zero\n";
        std::exit(1);
    }
    return static_cast<double>(x) / y;
}
```



### **Exceptions**

​		Because returning an error from a function back to the caller is complicated (and the many different ways to do so leads to inconsistency, and inconsistency leads to mistakes), C++ offers an entirely separate way to pass errors back to the caller: `exceptions`.

- The basic idea is that when an error occurs, an exception is “thrown”.
- If the current function does not “catch” the error, the caller of the function has a chance to catch the error.
-  If the caller does not catch the error, the caller’s caller has a chance to catch the error.
- The error progressively moves up the call stack until it is either caught and handled (at which point execution continues normally), or until main() fails to handle the error (at which point the program is terminated with an exception error).