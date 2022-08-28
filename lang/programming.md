		The collection of physical computer parts that make up a computer and execute programs is called the **hardware**. 

​		When a computer program is loaded into memory and the hardware sequentially executes each instruction, this is called **running** or **executing** the program.

​		A **computer program** (also commonly called an **application**) is a set of instructions that the computer can perform in order to perform some task. 

## 编程语言：

​	编程语言都具有一组公共的语法特征，不同语言仅在特征的细节上有所不同。

最基本的特征：

- 内置数据类型
- 变量。
- 表达式和语句，用于操纵上述数据类型的具体值。
- 流程控制语句，这些结构允许我们有选择的执行一些语句或者重复的执行一些语句。
- 函数，用于定义可供随时调用的计算单元。

大多数编程语言通过两种方式来进一步扩充基本特征：

- 程序员自定义数据类型
- 将一些有用的功能封装成库函数提供给程序员。

运行时检查数据类型的语言称为动态数据语言。c++是一种静态数据类型语言，类型检查发生在编译时，编译器必须知道程序中每一个变量对应的数据类型。



### machine language

​		The limited set of instructions that a CPU can understand directly is called **machine code** (or **machine language** or an **instruction set**).

​		machine language instruction like: `10110000 01100001`

​		 each instruction is composed of a sequence of 1s and 0s. Each individual 0 or 1 is called a **binary digit**, or **bit** for short.The number of bits that make up a single command vary -- for example, some CPUs process instructions that are always 32 bits long, whereas some other CPUs (such as the x86 family, which you are likely using) have instructions that can be a variable length.

​		each set of binary digits is interpreted by the CPU into a command to do a very specific job, such as *compare these two numbers*, or *put this number in that memory location*.

​		This meant programs generally weren’t **portable** (usable without major rework) to different types of system, and had to be written all over again.

### Assembly Language

​		Because machine language is so hard for humans to read and understand, assembly language was invented. 

​		In an assembly language, each instruction is identified by a short abbreviation (rather than a set of bits), and names and other numbers can be used.

> 编程实际上是在不同粒度上进行别名组合。

​		assembly language like : `mov al, 061h`

​		the assembly program must be translated into machine language before it can be executed by the computer. This is done by using a program called an **assembler**.

​		assembly language still isn’t very portable -- a program written in assembly for one CPU will likely not work on hardware that uses a different instruction set, and would have to be rewritten or extensively modified.



### High-level Languages

​		These languages are called **high level languages**, as they are designed to allow the programmer to write programs without having to be as concerned about what kind of computer the program will be run on.

​		 C/C++: `a = 97;`

​		high level languages must be translated into a format the computer can understand before they can be run. There are two primary ways this is done: compiling and interpreting.

​		![Example of compiling](https://www.learncpp.com/images/CppTutorial/Chapter0/Compiling-min.png?ezimgfmt=rs:521x161/rscb2/ng:webp/ngcb2)

![Example of interpreting](https://www.learncpp.com/images/CppTutorial/Chapter0/Interpreting-min.png?ezimgfmt=rs:637x54/rscb2/ng:webp/ngcb2)

> https://stackoverflow.com/a/38491646

​		programs can be compiled (or interpreted) for many different systems, and you don’t have to change the program to run on different CPUs (you just recompile for that CPU). As an example:

​		![Example of portability](https://www.learncpp.com/images/CppTutorial/Chapter0/Portability-min.png?ezimgfmt=rs:481x261/rscb2/ng:webp/ngcb2)



## Programs

​		Programs are collections of instructions that manipulate data to produce a desired result.
