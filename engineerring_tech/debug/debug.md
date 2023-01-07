

# debug

---



Once a problem has been identified, debugging the problem generally consists of five steps:

1. Find the root cause of the problem (usually the line of code that’s not working)
2. Ensure you understand why the issue is occurring
3. Determine how you’ll fix the issue
4. Repair the issue causing the problem
5. Retest to ensure the problem has been fixed and no new problems have emerged

## debuging strategy

- finding problems via code inspection

- finding problems by running program

  1. Figure out how to reproduce the problem
  2. Run the program and gather information to narrow down where the problem is
  3. Repeat the prior step until you find the problem

- reproducing the problem

  ​		The first and most important step in finding a problem is to be able to *reproduce the problem*. The reason is simple: it’s extremely hard to find an issue unless you can observe it occurring.

  ​		If the issue can be reproduced 100% of the time, that’s ideal, but less than 100% reproducibility can be okay. An issue that occurs only 50% of the time simply means it’ll take twice as long to diagnose the issue, as half the time the program won’t exhibit the problem and thus not contribute any useful diagnostic information.




## debug tactics

### printing isn’t great

While adding debug statements to programs for diagnostic purposes is a common rudimentary technique, and a functional one (especially when a debugger is not available for some reason), it’s not that great for a number of reasons:

1. Debug statements clutter your code.
2. Debug statements clutter the output of your program.
3. Debug statements must be removed after you’re done with them, which makes them non-reusable.
4. Debug statements require modification of your code to both add and to remove, which can introduce new bugs.

We can do better. We’ll explore how in future lessons.



### Conditionalizing your debugging code

One way to make it easier to disable and enable debugging throughout your program is to make your debugging statements conditional using preprocessor directives:

```c
#include <iostream>

#define ENABLE_DEBUG // comment out to disable debugging

int getUserInput()
{
#ifdef ENABLE_DEBUG
std::cerr << "getUserInput() called\n";
#endif
	std::cout << "Enter a number: ";
	int x{};
	std::cin >> x;
	return x;
}

int main()
{
#ifdef ENABLE_DEBUG
std::cerr << "main() called\n";
#endif
    int x{ getUserInput() };
    std::cout << "You entered: " << x;

    return 0;
}
```



### Using a logger

An alternative approach to conditionalized debugging via the preprocessor is to send your debugging information to a log file.

Most applications and operating systems write log files that can be used to help diagnose issues that occur.



### debug tools

#### step

A **debugger** is a computer program that allows the programmer to control how another program executes and examine the program state while that program is running. 

**Stepping** is the name for a set of related debugger features that let us execute (step through) our code statement by statement.

The **step into** command executes the next statement in the normal execution path of the program, and then pauses execution of the program so we can examine the program’s state using the debugger. If the statement being executed contains a function call, *step into* causes the program to jump to the top of the function being called, where it will pause.

The **step over** command executes the next statement in the normal execution path of the program. However, whereas *step into* will enter function calls and execute them line by line, *step over* will execute an entire function without stopping and return control to you after the function has been executed.

Some debuggers (such as Visual Studio Enterprise Edition and GDB 7.0) have introduced a stepping capability generally referred to as *step back* or *reverse debugging*.

#### running

This **Run to cursor** command executes the program until execution reaches the statement selected by your cursor.

 The **continue** debug command simply continues running the program as per normal, either until the program terminates, or until something triggers control to return back to you again (such as a breakpoint).

The **start** command performs the same action as *continue*, just starting from the beginning of the program. It can only be invoked when not already in a debug session.

#### breakpoint

A **breakpoint** is a special marker that tells the debugger to stop execution of the program at the breakpoint when running in debug mode.

The **set next statement** command allows us to change the point of execution to some other statement (sometimes informally called *jumping*). This can be used to jump the point of execution forwards and skip some code that would otherwise execute, or backwards and have something that already executed run again.

#### Watching variables

**Watching a variable** is the process of inspecting the value of a variable while the program is executing in debug mode. 

The **watch window** is a window where you can add variables you would like to continually inspect, and these variables will be updated as you step through your program. 

many debuggers will offer some way to quickly watch the value of *all* local variables in scope.

#### The call stack

The **call stack** is a list of all the active functions that have been called to get to the current point of execution. The call stack includes an entry for each function called, as well as which line of code will be returned to when the function returns. Whenever a new function is called, that function is added to the top of the call stack. When the current function returns to the caller, it is removed from the top of the call stack, and control returns to the function just below it.
