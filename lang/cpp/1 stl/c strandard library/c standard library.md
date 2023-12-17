# C standard library

​		c++标准库还提供了C标准库的功能，并进行了适当调整以确保静态类型安全。许多库函数的描述依赖于C标准库来实现这些函数的语义。

​		在某些情况下，标准c++中指定的签名可能与C标准库中的签名不同，并且可能声明额外的重载，但是行为和先决条件(包括C的限制所隐含的)(自c++ 17起)是相同的，除非另有说明。

​		为了与C标准库兼容，c++标准库提供了下面列出的C头文件。这些头的预期用途仅用于互操作性。为了成为有效的ISO C, c++源文件可能需要包含这些头文件中的一个。不打算成为有效ISO C的源文件不应该使用任何C头文件。请看这里的描述。

![image-20231217111617040](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20231217111617040.png)

Except otherwise noted, the contents of each header `c*xxx*` is the same as that of the corresponding header `*xxx*.h` as specified in the [C standard library](https://en.cppreference.com/w/c/header). In the C++ standard library, however, the declarations (except for names which are defined as macros in C) are within namespace scope of the namespace std. It is unspecified whether these names (including any overloads added) are first declared within the global namespace scope and are then injected into namespace std by explicit [using-declarations](https://en.cppreference.com/w/cpp/language/namespace#Using-declarations).

Names which are defined as macros in C ([`assert`](https://en.cppreference.com/w/cpp/error/assert), [`offsetof`](https://en.cppreference.com/w/cpp/types/offsetof), [`setjmp`](https://en.cppreference.com/w/cpp/utility/program/setjmp), [`va_arg`](https://en.cppreference.com/w/cpp/utility/variadic/va_arg), [`va_end`](https://en.cppreference.com/w/cpp/utility/variadic/va_end) and [`va_start`](https://en.cppreference.com/w/cpp/utility/variadic/va_start)) must be defined as macros in the C++ standard library, even if C grants license for implementation as functions.

Names that are defined as functions in C must be defined as functions in the C++ standard library. This disallows the practice, allowed in C, of providing a masking macro in addition to the function prototype. The only way to achieve equivalent inline behavior in C++ is to provide a definition as an extern [inline function](https://en.cppreference.com/w/cpp/language/inline).

Identifiers that are keywords or operators in C++ cannot be defined as macros in C++ standard library headers. In particular, including the standard header [``](https://en.cppreference.com/w/cpp/header/ciso646) has no effect.

![image-20231217111643443](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20231217111643443.png)

​		C++标准库中除了定义C++语言特有的功能外，也兼容了C语言的标准库。C语言的头文件形如name.h，C++则将这些文件命名为cname。也就是去掉了.h后缀，而在文件名name之前添加了字母c，这里的c表示这是一个属于C语言标准库的头文件。因此，cctype头文件和ctype.h头文件的内容是一样的，只不过从命名规范上来讲更符合C++语言的要求。特别的，在名为cname的头文件中定义的名字从属于命名空间std，而定义在名为.h的头文件中的则不然。一般来说，C++程序应该使用名为cname的头文件而不使用name.h的形式，标准库中的名字总能在命名空间std中找到。如果使用.h形式的头文件，程序员就不得不时刻牢记哪些是从C语言那儿继承过来的，哪些又是C++语言所独有的。