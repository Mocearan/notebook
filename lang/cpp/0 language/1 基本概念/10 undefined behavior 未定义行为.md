# 未定义行为 UB

Renders the entire program meaningless if certain rules of the language are violated.

------------------

**Undefined behavior** (often abbreviated UB) is the result of executing code whose behavior is not well defined by the C++ language. 

Code implementing undefined behavior may exhibit *any* of the following symptoms:

- Your program produces different results every time it is run.
- Your program consistently produces the same incorrect result.
- Your program behaves inconsistently (sometimes produces the correct result, sometimes not).
- Your program seems like its working but produces incorrect results later in the program.
- Your program crashes, either immediately or later.
- Your program works on some compilers but not others.
- Your program works until you change some other seemingly unrelated code.

The nature of undefined behavior is that you never quite know what you’re going to get, whether you’ll get it every time, and whether that behavior will change when you make other changes.