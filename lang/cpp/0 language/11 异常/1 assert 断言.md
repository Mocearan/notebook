# 错误处理

---

​		



## 断言

​		In programming, a **precondition** is any condition that must always be true prior to the execution of component of code.

​		An **invariant** is a condition that must be true while some component is executing.

​		a **postcondition** is something that must be true after the execution of some component of code.

​		Using a conditional statement to detect an invalid parameter (or to validate some other kind of assumption), along with printing an error message and terminating the program, is such a common response to problems that C++ provides a shortcut method for doing this.

- An **assertion** is an expression that will be true unless there is a bug in the program. 

  - If the expression evaluates to `true`, the assertion statement does nothing. 

  - If the conditional expression evaluates to `false`, an error message is displayed and the program is terminated (via `std::abort`). 

    > ​		This error message typically contains the expression that failed as text, along with the name of the code file and the line number of the assertion. 

  Use assertions to document cases that should be logically impossible.

  > ​		The goal of an assertion is to catch programming errors by documenting something that should never happen. If that thing does happen, then the programmer made an error somewhere, and that error can be identified and fixed. Assertions do not allow recovery from errors (after all, if something should never happen, there’s no need to recover from it), and the program will not produce a friendly error message.

### 动态断言

​		In C++, runtime assertions are implemented via the **assert** preprocessor macro, which lives in the `<cassert>` header.

​		强判定一个表达式必定为真。

> `assert` 虽然是一个宏，但在预处理阶段不生效，而是在运行阶段才起作用，所以又叫“动态断言”。

```c++
#include <cassert>

#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif
```

> - `assert` 将其参数表达式（必须拥有标量类型）与零（false）比较相等。
>
> - 若相等，则 `assert` 在标准错误输出上输出实现指定的诊断信息，并调用 `std::abort`
>
>   > `abort()` function terminates the program immediately, without a chance to do any further cleanup (e.g. close a file or database). Because of this, asserts should be used only in cases where corruption isn’t likely to occur if the program terminates unexpectedly.
>
> - `NDEBUG`，If the macro `NDEBUG` is defined, the assert macro gets disabled.、
>
>   > Some IDEs set `NDEBUG` by default as part of the project settings for release configurations.

```c++
assert(i > 0 && "i must be greater than zero"); // 判定的同时，使得断言的信息更详尽
assert(p != nullptr);
assert(!str.empty());
```

​		Assertions are also sometimes used to document cases that were not implemented because they were not needed at the time the programmer wrote the code:

```c++
// Assert with a message, covered in the next section
assert(moved && "Need to handle case where student was just moved to another classroom");
```



### 静态断言

​		A **static_assert** is an assertion that is checked at compile-time rather than at runtime, with a failing `static_assert` causing a compile error. 

​		关键字，进行编译时断言检查。可以配合错误提示更快的发现编译期的错误。

​		`static_assert(condition, diagnostic_message)`

```c++

template<int N>
struct fib
{
    static_assert(N >= 0, "N >= 0");

    static const int value =
        fib<N - 1>::value + fib<N - 2>::value;
};

///////////////////////


static_assert(
  sizeof(long) >= 8, "must run on x64");
  
static_assert(
  sizeof(int)  == 4, "int must be 32bit");
```

- static_assert 运行在编译阶段，只能看到编译时的常数和类型

> 
>
> ```c++
> 
> char* p = nullptr;
> static_assert(p == nullptr, "some error.");  // 错误用法
> ```
>

- 配合标准库里的`type_traits`，它提供了对应这些概念的各种编译期“函数”:

> ```c++
>// 假设T是一个模板参数，即template<typename T>
> 
> static_assert(
> is_integral<T>::value, "int");
> 
> static_assert(
>is_pointer<T>::value, "ptr");
> 
>static_assert(
> is_default_constructible<T>::value, "constructible");
> ```
> 

- Prior to C++17, the diagnostic message must be supplied as the second parameter. Since C++17, providing a diagnostic message is optional.
