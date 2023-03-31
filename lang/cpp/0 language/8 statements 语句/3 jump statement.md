# jump statement

---



## break

​		The **break statement** causes a while loop, do-while loop, for loop, or switch statement to end, with execution continuing with the next statement after the loop or switch being broken out of.

## continue

​		The **continue statement** provides a convenient way to end the current iteration of a loop without terminating the entire loop. `continue statements` work by causing the current point of execution to jump to the bottom of the current loop.

> ​		Be careful when using a `continue statement` with while or do-while loops. These loops typically change the value of variables used in the condition inside the loop body. If use of a `continue statement` causes these lines to be skipped, then the loop can become infinite!

## goto

​		unconditional jumps are implemented via a **goto statement**, and the spot to jump to is identified through use of a **statement label**. 

```c++
#include <cmath> // for sqrt() function

int main()
{
    double x{};
tryAgain: // this is a statement lable
    std::cout << "Enter a non-negative number: ";
    std::cin >> x;
    
    if( x < 0.0)
        goto tryAgain; // this is the goto statement
    
    std::cout << "The square root of " << x << " is " <<std::sqrt(x) << '\n';
    return 0;
}
```

​		Statement labels utilize a third kind of scope: **function scope**, which means the label is visible throughout the function even before its point of declaration.

​		Avoid `goto statements` (unless the alternatives are significantly worse for code readability).

> ​		One notable exception is when you need to exit a nested loop but not the entire function -- in such a case, a `goto` to just beyond the loops is probably the cleanest solution.
>
> ```c++
> void do_something(int n, int m)
> {
>     for(int i = 0; i < n; ++i)
>         for(int j = 0; j < m; ++j)
>             if(nm[i][j] == a)
>                 goto found;
>     found:
>     
> }
> ```
>
> 

​		c++中使用`goto`离开块，块上的自动变量的析构函数仍然会被调用。