# 翻译阶段

​	编译器处理c++源文件时，严格按照下列顺序进行各个阶段的处理。

---

Prior to compilation, the code file goes through a phase known as **translation**.



## 1.字符翻译

### <1> ASCII码 字符翻译

（以实现定义方式）将源文件的各个单独字节，映射为*基本源字符集*的字符。特别是，操作系统相关的行尾指示符均被替换为换行字符。

> **基本源字符集（The *basic source character set* ）96个：**
>
> - 空白符 5个
>
>   - 空格符（space）
>   - 水平制表符（horizontal tab）
>   - 垂直制表符（vertical tab）
>   - 换页（form feed）
>   - 换行（new-line）
>
> - 数字字符 10个
>
>   从```‘0’```到```‘9’```
>
> - 大小写英文字母 52个
>
>   从```a```到```z```以及```A```到```Z```
>
> - 标点符号 29个
>
>   ```_ { } [ ] # ( ) < > % : ; . ? * + - / ^ & | ~ ! = , \ " '```

> 除前32位（0-31）的控制字符。

### <2> Unicode 字符翻译

任何无法被映射到基本源字符集中的字符的源文件字符，均被替换为其**通用字符名**（用 `\u` 或 `\U` 转义），或某种被等价处理的由实现定义的形式。

![image-20210117083524684](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210117083524684.png)

> **Unicode：	**
>
> https://en.wikipedia.org/wiki/Unicode

### <3> c++17前 的三标符翻译

将各个**三标符序列**替换为其对应的单字符表示.

> **三标符（trigraph sequence）：**
>
> 部分符号的三字符代用符号。
>
> ![image-20210117084339031](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210117084339031.png)
>
> 因为三标符的翻译，会导致在辨识注释和字符串字面量阶段之前因为出现 ??/被替换为 \，成为行末的\，进而在合并代码行，将文本中的下一行注释掉。
>
> 如```// Will the next line be executed?????/```



## 2. 代码行辨识

### <1> 合并代码行

​		当反斜杠出现于行尾（其后紧跟换行符）时，删除该反斜杠和换行符并将两个物理源码行组合成一个逻辑源码行。

这是单趟操作：如果有一行以两个反斜杠结束且后随一个空行，这三行不会合为一行。若于此阶段组成了通用字符名（\uXXXX），则行为未定义。

### <2> 行末辨识

​		若此步骤后，非空源文件不以换行符结束（无论是原本就无换行，还是以反斜杠结束），则其行为未定义 (C++11 前)在最后添加一个换行符 (C++11 起)。





## 3. 辨识注释和字符串字面量

### <1> 分解文本源文件

将源文件切分为 **注释**、**空白符**、**预处理记号**

> **预处理记号：**
>
> 1. 头文件名，如 ```<iostream> ```或 ```"myfile.h"```
> 2. 标识符
> 3. 预处理数字
> 4. 包括用户自定义的**字符常量**与**字符串字面量（literal）**
> 5. 运算符与标点（包括**代用记号**），如 +、<<=、<%、## 或 and
> 6. 不属于任何其他类型的单独单独非空白字符
>
> > **代用记号：**
> >
> > ​	部分符号的代用写法。
> >
> > ps: c++/基础语法/表达式/运算符
> >
> > **字面量：**
> >
> > 直接表达逻辑含义的值。
> >
> > ps: c++/基础语法/表达式/字面量
>
> 对于预处理记号的辨识有如下要点：
>
> - 头文件名预处理记号仅在```#include```指令中形成
>
>   ```c++
>   std::vector<int> x; // OK，<int> 不是头文件名
>   ```
>
> - 保留换行符。
>
>   未知名是否可将非换行空白字符序列缩减成单个空格字符
>
> - 最大吞噬
>
>   若一个给定字符前的输入已被解析为预处理记号，下一个预处理记号通常会由能构成预处理记号的最长字符序列够成，即使这样处理会导致后续分析失败。
>
>   ```c++
>   int foo = 1;
>   int bar = 0xE+foo;   // 错误：非法的预处理数字 0xE+foo
>   int baz = 0xE + foo; // OK
>                   
>   int quux = bar+++++baz; // 错误：bar++ ++ +baz，而非 bar++ + ++baz。
>   ```
>
>   对于最大吞噬，有两个例外：
>
>   > - 若以下一个字符开头的字符序列可作为原始字符串字面量的前缀和起始双引号，则下个预处理记号应当为原始字符串字面量。该字面量由匹配原始字符串模式的最短字符序列组成。
>   >
>   >   ```c++
>   >   #define R "x"
>   >   const char* s = R"y"; // 非良构的原始字符串字面量，而非 "x" "y"
>   >   const char* s2 = R"(a)" "b)"; // 原始字符串字面量后随普通字符串字面量
>   >   ```
>   >
>   > 
>   >
>   > - 若接下来三个字符是 ``<::``且后继字符不是```:```，则把``<``自身当做预处理记号（而非代用记号`<:` 的首字符）。
>   >
>   >   ```c++
>   >   struct Foo { static const int v = 1; };
>   >   std::vector<::Foo> x; // OK，<: 未被当作 [ 的代用记号
>   >   extern int y<::>;     // OK，同 extern int y[]。
>   >   int z<:::Foo::value:>; // OK，int z[::Foo::value];
>   >   ```
>   >
>   > 
>   >
>   > **原始字符串字面量：**
>   >
>   > ​	原始字符串字面量，即裸字符串。声明```R“()”```包裹的字符串，期间的任何字符都将作为字符串字面量的一部分而存在，不会对其间引起字符串结束的符号进行辨识。
>   >
>   > ​	ps : c++/基础语法/表达式/字面量



### <2> 字符串字面量还原

对于任何字符串字面量首尾双引号之间，发生的阶段1 、2的翻译，都被还原。(C++11 起)

### <3> 以一个空格字符替换每段注释



## 4.预处理阶段

### <1> 执行预处理器

### <2> #include  指令所引入的每个文件都经历 1-4阶段的处理，递归执行

​		传统的c++编写方式是`.h + .cpp`，声明与实现分离。

> 以前的计算机性能弱内存小，头和实现分离才方便处理。

​		现代的c++越来越多的引入了`.hpp`的文件格式，在`.hpp`里实现类的全部功能，更为现代。与其他语言的文本模式一致。

### <3> 此阶段结束时，所有预处理器指令都应从源代码中移除



## 5.  源代码字符转换到执行字符

### <1> 将字符字面量及字符串字面量中的所有字符从源字符集转换到*执行字符集*

​	可以是 UTF-8 这样的多字节字符集，只要阶段 1 中所列的*基本源字符集*的 96 个字符都拥有单字节表示即可。

### <2> 将字符字面量和非原始字符串字面量中的转义序列和通用字符名展开，并转换到*执行字符集*。

​	 若某个通用字符名所指定的字符不是执行字符集的成员，则结果是由实现定义的，但保证不是空（宽）字符。

>  **转义序列**
>
>  ​	转义序列用于在字符串字面量和字符字面量中表示某些特殊字符。
>
>  **注意：**
>
>  某些实现能以命令行选项控制此阶段所进行的转换：gcc 和 clang 用 -finput-charset 指定源字符集的编码，用 -fexec-charset 和 -fwide-exec-charset 指定无编码前缀的 (C++11 起)字符串和字符字面量中的执行字符集的编码，而 Visual Studio 2015 Update 2 及之后版本分别用 /source-charset 和 /execution-charset 指定源字符集和执行字符集。



## 6.拼接相邻的字符串字面量

```c++
const char* s3 = "\n"
                 "Hello\n"
                 "  World\n";

-> const char* s3 = "\nHello\n  World\n";
```



## 7. 编译阶段

​	将各个预处理记号转换成符号链接。

​	将所有记号当做一个**翻译单元**进行语法和语义分析并翻译成符号链接，生成二进制文件（汇编文件、库文件）。

### 翻译单元

​		一个经预处理过后的源代码文件（.c/.cpp/.hpp...)，会被编译器当做一个独立可单独编译的最小单元，编译它所需的所有依赖都会通过`#include`和源代码声明的方式引入。一个翻译单元经过编译后，会形成一个目标文件（.o)。

​		注意，此时说的依赖，都是指编译时的名字依赖，即只需要其声明。当链接时，会需要查找所有名字的定义，进而查找本翻译单元的定义或外部链接的定义。

>  https://www.dazhuanlan.com/2019/09/28/5d8ed410d7003/

### 编译时错误

​		在编译器发现的错误叫编译时错误。链接器发现的错误叫链接时错误。运行时才发现的错误叫做运行时错误或逻辑错误。

### 编译期极端 - 模板元编程

​		面向编译器编程，用一些指令或者关键字直接操作编译期的编译行为，来达到将部分在运行期的计算任务提前到编译期完成。

​		编译阶段的特殊性在于，它看到的都是 C++ 语法实体，比如 typedef、using、template、struct/class 这些关键字定义的类型，而不是运行阶段的对象。

​		简单的元编程如：

```c++

template<int N>
struct fib                   // 递归计算斐波那契数列
{
    static const int value =
        fib<N - 1>::value + fib<N - 2>::value;
};

template<>
struct fib<0>                // 模板特化计算fib<0>
{
    static const int value = 1;
};

template<>
struct fib<1>               // 模板特化计算fib<1>
{
    static const int value = 1;
};

// 调用后输出2，3，5，8
cout << fib<2>::value << endl;
cout << fib<3>::value << endl;
cout << fib<4>::value << endl;
cout << fib<5>::value << endl;
```

​		模板元编程的困难在于，过程无法调试，需要人工推导整个编译处理过程。



## 8. 模板实例化阶段

​	检验每个翻译单元，产生所要求的模板实例化的列表，其中包括**显式实例化**所要求的实例化。

​	定位模板定义，并进行所要求的实例化，以产生*实例化单元*。

>**显示实例化：**
>
>​	显式实例化定义强制实例化其所指代的 class、struct 或 union。
>
>ps: c++/基础语法/模板/类模板/模板实例化



## 9. 链接阶段

​	由链接器，将翻译单元、实例化单元和为满足外部引用所需的库组件汇集成一个程序映像，它含有在其执行环境中执行所需的信息。

> ### 注意
>
> 某些编译器不实现实例化单元（又称为[模板仓库](http://docs.oracle.com/cd/E18659_01/html/821-1383/bkagr.html#scrolltoc)或[模板注册表](http://www-01.ibm.com/support/knowledgecenter/SSXVZZ_12.1.0/com.ibm.xlcpp121.linux.doc/compiler_ref/fcat_template.html?lang=en)），而是简单地在阶段 7 编译每个模板实例化，存储代码于其所显式或隐式要求的对象文件中，然后由连接器于阶段 9 将这些编译后的实例化缩减到一个。
>
> **库**
>
> 一个苦事一些代码的集合，它们通常是由其他人编写的，我们通过`#include`来将所提供的头文件中的声明引入来访问库的功能。
>
> 一个声明是用于指出一段程序如何使用一条语句。



> ## Something went wrong!
>
> There are plenty of things that can go wrong the first time you try to work with multiple files. If you tried the above example and ran into an error, check the following:
>
> 1. If you get a compiler error about *add* not being defined in *main*, you probably forgot the forward declaration for function *add* in *main.cpp*.
> 2. If you get a linker error about *add* not being defined, e.g.
>
> ```
> unresolved external symbol "int __cdecl add(int,int)" (?add@@YAHHH@Z) referenced in function _main
> ```
>
> 2a. …the most likely reason is that *add.cpp* is not added to your project correctly. When you compile, you should see the compiler list both *main.cpp* and *add.cpp*. If you only see *main.cpp*, then *add.cpp* definitely isn’t getting compiled. If you’re using Visual Studio or Code::Blocks, you should see *add.cpp* listed in the Solution Explorer/project pane on the left or right side of the IDE. If you don’t, right click on your project, and add the file, then try compiling again. If you’re compiling on the command line, don’t forget to include both *main.cpp* and *add.cpp* in your compile command.
>
> 2b. …it’s possible that you added *add.cpp* to the wrong project.
>
> 2c. …it’s possible that the file is set to not compile or link. Check the file properties and ensure the file is configured to be compiled/linked. In Code::Blocks, compile and link are separate checkboxes that should be checked. In Visual Studio, there’s an “exclude from build” option that should be set to “no” or left blank.
>
> 1. Do *not* *#include “add.cpp”* from *main.cpp*. This will cause the compiler to insert the contents of *add.cpp* directly into *main.cpp* instead of treating them as separate files.

# 分离式编程

As programs get larger, it is common to split them into multiple files for organizational or reusability purposes. 

---

程序的源文件被依据功能的逻辑结构被分割成多个源文件。

每个分割的源文件都是一个独立的翻译单元，可以被编译器单独编译。编译产物被链接器链接后成为完整的程序。

> A code file with translations applied to it is called a **translation unit**.

When the compiler compiles a multi-file program, it may compile the files in any order. Additionally, it compiles each file individually, with no knowledge of what is in other files.

源文件之间的符号通过引用性声明引入【包含声明】，而源文件之间符号实现（定义）则通过链接来查找【链接定义】。

![img](https://www.learncpp.com/images/CppTutorial/Section1/IncludeLibrary.png?ezimgfmt=rs%3Adevice%2Frscb2-1)

![img](https://www.learncpp.com/images/CppTutorial/Section1/IncludeHeader.png?ezimgfmt=rs:647x377/rscb2/ng:webp/ngcb2)

### 头文件

The primary purpose of a header file is to propagate declarations to code files.

Header files usually have a .h extension, but you will occasionally see them with a .hpp extension or no extension at all. 

Header files allow us to put declarations in one location and then import them wherever we need them. This can save a lot of typing in multi-file programs.

头文件通常包含那些只能被定义一次的实体，如类，函数声明，const 和 constexpr变量等。

头文件也常用到其他头文件的功能，为了保证多次包含的情况下也能安全正常工作，需要进行一些特殊的头文件处理。

**头文件一旦改变，相关的源文件必须重新编译以获取更新过的声明**

> Header files are often paired with code files, with the header file providing forward declarations for the corresponding code file. Since our header file will contain a forward declaration for functions defined in *add.cpp*, we’ll call our new header file *add.h*.

#### 防卫性声明

Header guards are designed to ensure that the contents of a given header file are not copied more than once into any single file, in order to prevent duplicate definitions.

> 头文件定义时，可能需要引用其他对象的定义，这样在头文件被引用时，可能隐式的引用了其他头文件，如果再显式地引用相同的头文件。会造成依赖冲突。
>
> 所以必须在书写头文件时做防卫性声明。

```c++
#ifndef MASSPP_OSTYPE_H
#define MASSPP_OSTYPE_H
...
#endif // MASSPP_OSTYPE_H
    
//    这样使得多次包含时，相同变量在同一个编译单元只被定义一次。
//  #define、#ifndef、#endif 组合起来用于检查某个指定的预处理变量是否已经定义。来达到头文件防卫性声明的目的。
/*
	第一次包含头文件是，#ifndef的结果为真，预处理器将顺序执行后面的操作直至遇到#endif为止。
此时，预处理宏的值将被#define定义。头文件被拷贝到.cpp的实现文件中。再多次包含头文件时，
#ifndef的检查结果将为假。编译器将忽略#ifndef到#endif之间的部分。
*/
    
//  Some good suggestions are a naming convention of <PROJECT>_<PATH>_<FILE>_H , <FILE>_<LARGE RANDOM NUMBER>_H, or <FILE>_<CREATION DATE>_H
```

​		防卫性声明通过预处理指令控制宏来保证头文件多次包含后安全工作。

​		头文件即使（目前还）没有被包含在任何其他头文件中，也应该设置保护符。头文件保护符很简单，程序员只要习惯性地加上就可以了，没必要太在乎程序需不需要。

> `#pragma once`被部分编译器引入，实现`Include Guard`，但它是非标准的，不推荐使用。
>
> C++20 新增了`module`特性，可以实现一次性加载，但`Include guard`短期内还是无可替代的

#### 添加头文件的顺序

```c++
// project header files
// 3rd	header files
// cpp header files
// c header files
// os header files
```

> 也有反着的，但这种能最小化编译速度。
>
> ![image-20220328221521407](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220328221521407.png)
>
> That way, if one of your user-defined headers is missing an #include for a 3rd party library or standard library header, it’s more likely to cause a compile error so you can fix it.

#### 传递包含

When your code file #includes the first header file, you’ll also get any other header files that the first header file includes (and any header files those include, and so on). These additional header files are sometimes called **transitive includes**, as they’re included implicitly rather than explicitly.

However, you should not rely on the content of headers that are included transitively. The implementation of header files may change over time, or be different across different systems. 

This is easily avoided by explicitly including all of the header files the content of your code file requires.

#### header file best practices

Here are a few more recommendations for creating and using header files.

-   Always include header guards (we’ll cover these next lesson).
-   Do not define variables and functions in header files (global constants are an exception -- we’ll cover these later)
-   Give a header file the same name as the source file it’s associated with (e.g. *grades.h* is paired with *grades.cpp*).
-   Each header file should have a specific job, and be as independent as possible. For example, you might put all your declarations related to functionality A in A.h and all your declarations related to functionality B in B.h. That way if you only care about A later, you can just include A.h and not get any of the stuff related to B.
-   Be mindful of which headers you need to explicitly include for the functionality that you are using in your code files
-   Every header you write should compile on its own (it should #include every dependency it needs)
-   Only #include what you need (don’t include everything just because you can).
-   Do not #include .cpp files.

### troubleshooting

If you get a compiler error indicating that *add.h* isn’t found, make sure the file is really named *add.h*. Depending on how you created and named it, it’s possible the file could have been named something like *add* (no extension) or *add.h.txt* or *add.hpp*. Also make sure it’s sitting in the same directory as the rest of your code files.

If you get a linker error about function *add* not being defined, make sure you’ve added *add.cpp* in your project so the definition for function *add* can be linked into the program.





