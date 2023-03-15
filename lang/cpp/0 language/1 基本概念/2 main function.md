# 主函数

​		A program shall contain a global function named `**main**`, which is the designated start of the program in hosted environment.

---

​		Programs typically terminate (finish running) when the last statement inside function *main* has executed (though programs may abort early in some circumstances, or do some cleanup afterwards).

​		When discussing functions, it’s fairly common shorthand to append a pair of parenthesis to the end of the function’s name.

>  *main()* or *doSomething()*

## 形式

它应当有下列形式之一：

1. ```int main(void) {body} ```
2. ```int main(int argc, char* argv[]) { body }```
3. ```/* 其他由实现定义的形式，返回类型为 int */```

> | argc   | -    | 非负数，表示从程序运行的环境传递给程序的实参个数。           |
> | ------ | ---- | ------------------------------------------------------------ |
> | argv   | -    | 指针，指向包含 argc + 1 个指针的数组的首元素。  数组末元素为空指针，若其前面有任何元素，则它们指向**空终止多字节字符串**，表示从执行环境传递给程序的若干参数。若 argv[0] 不是空指针，或等价地 argc > 0 ，则它指向表示用于调用程序的名称的字符串，或空字符串。 |
> | *body* | -    | main 函数的函数体                                            |
>
> > **空终止多字节字符串：**
> >
> > ​		空终止多字节字符串（ null-terminated multibyte string, NTMBS ），或“多字节字符串”，是尾后拥有**零值的字节（空终止字符, `\0`）**的非零字节序列。
> >
> > ps: c++/标准库/字符串库/空终止字节字符串
>
> 
>
> 名字 `argc` 和 `argv` ，以及形参的类型的表示方式是任取的：
>
> ​	```int main(int ac, char** av) ```同样合法。
>
> `main()` 的一种常见的由实现定义的形式还有（除 `argc` 和 `argv` 之外的）第三个参数，类型为 `char*[]` ，指向[指向执行环境变量的指针数组](http://pubs.opengroup.org/onlinepubs/9699919799/functions/exec.html)。



## 自立实现

对于有**宿主环境**中所指定的程序入口点为`main`，**自立程序**的入口点则是由实现定义的。

>   **宿主环境：**
>
>   ​	即有操作系统的环境。宿主环境中的程序，并非绝对的独立应用，受限于操作系统的资源及模式，实质上在运行时是被操作系统加载的进程。
>
>   ​	即在命令行环境下，一个运行起来的程序和键入的命令并无不同。
>
>   **自立程序：**
>
>   ​	硬件启动加载器，操作系统内核等等。
>
>   ​	当硬件没有操作系统或者操作系统没有运行起来，甚至操作系统本身的启动都依赖于绝对独立的程序，来引导一块硬件的运行或者操作系统的启动，这样的程序称之为自立程序，这种程序直接依赖于硬件资源。
>
>   https://zh.cppreference.com/w/cpp/freestanding



## 调用时机

在程序启动时，`main` 函数在**初始化**具有**静态存储期**的非局部对象之后被调用。

> **初始化：**
>
> ​	变量的*初始化*在构造时提供其初值。
>
> ps : c++/基础语法/初始化
>
> **存储期：**
>
> ​	是指对象在进程的不同阶段因分配存储空间的属性不同，造成对象拥有不同的生命周期。
>
> ​	PS: c++/基础语法/声明/说明符/声明说明符/存储类说明符/存储期



## 命令行参数

​		双形参形式的 `main` 函数的形参允许从执行环境中传递任意的多字节字符串（即，命令行参数）。

​		各个指针 `argv[1] .. argv[argc-1]` 指向对应的每个字符串的第一个字符。指针 `argv[0]` 指向表示用于执行该程序自身的名字的字符串（当执行环境不支持时为空字符串 ""）。

​		这些字符串是可修改的：比如可以用 **std::strtok**来使用它们，虽然对它们的修改并不会被传回给执行环境。由 `argv` 所指向的数组的大小至少为 `argc+1`，其最后一个元素 `argv[argc]` 保证为一个空指针。

>   **std::strtok**
>
>   ​	寻找字节字符串中的下个记号.
>
>   ps：c++/字符串库/空终止字节字符串/字符串检验/std::strtok
>
>   ```c++
>   #include <cstring>
>   #include <iostream>
>   
>   
>   int main(int argc, char* argv[])
>   {
>     for(int i = 0; i < argc; ++i) {
>         char *token = std::strtok(argv[i], "/");
>         while (token != NULL) {
>             std::cout << token << "\n";
>             token = std::strtok(NULL, "/");
>         }
>         std::cout << std::endl;
>     }
>     return 0;
>   }
>   ```
>
>   
>
>   ![image-20210127235653638](https://gitee.com/masstsing/picgo-picserver/raw/master/20210127235653.png)
>
>   **getopt:**
>
>   ​	linux下提供了帮助解析命令行参数的系统调用。
>
>   ```c++
>   头文件 #include<unistd.h>
>   int getopt(int argc,char * const argv[ ],const char * optstring);
>   extern char *optarg;
>   extern int optind, opterr, optopt;
>   /*
>      getopt() 所设置的全局变量包括：
>      optarg —— 指向当前选项参数（如果有）的指针。
>      optind —— 再次调用 getopt() 时的下一个 argv 指针的索引。
>      optopt —— 最后一个未知选项。
>   
>      参数 optstring为选项字符串， 告知 getopt()可以处理哪个选项以及哪个选项需要参数，如果选项字符串里的字母后接着冒号“:”，则表示还有相关的参数，全域变量optarg 即会指向此额外参数。
>      如果在处理期间遇到了不符合optstring指定的其他选项getopt()将显示一个错误消息，并将全域变量optopt设为“?”字符，如果不希望getopt()打印出错信息，则只要将全域变量opterr设为0即可。
>      optstring中的指定的内容的意义（例如getopt(argc, argv, "ab:c:de::");）
>      1.单个字符，表示选项（如下例中的abcde各为一个选项）。
>      2.单个字符后接一个冒号：表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。该参数的指针赋给optarg（如下例中的b:c:）。
>      3 单个字符后跟两个冒号，表示该选项后可以跟一个参数，也可以不跟。如果跟一个参数，参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。(如上例中的e::，如果没有跟参数，则optarg = NULL)
>   */
>   ```
>
>   详情请自行查阅linux系统编程相关手册。
>
>   ```c++
>   #include <unistd.h>
>   #include <cstdio>
>   
>   void show(char ch)
>   {
>      printf("ch     a:%c\n",     ch); 
>      printf("optarg a:%s\n", optarg); 
>      printf("optind a:%d\n", optind);
>      printf("opterr a:%d\n", opterr);
>      printf("optopt a:%d\n", optopt);
>      printf("\n");
>   }
>   
>   
>   int main(int argc, char *argv[])
>   {
>   	int ch;
>   
>   	while((ch = getopt(argc,argv,"a:bc::"))!= -1)
>   	{
>          show(ch);
>   	}
>   	return 0;
>   }
>   ```
>
>   ![image-20210204221932093](https://gitee.com/masstsing/picgo-picserver/raw/master/20210204221932.png)

​		将操作系统命令行上给定的参数转换为由 `argv` 所指代的多字节字符数组的方式，可能涉及到由实现定义的处理过程：

>   -   [分析 C++ 命令行参数](https://msdn.microsoft.com/en-us/library/17w5ykft.aspx) MSDN
>   -   [Shell 介绍](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_01) POSIX



## 返回值

The return value from `main` is sometimes called a **status code** (also sometimes called an **exit code**, or rarely a **return code**), as it is used to indicate whether the program ran successfully or not.

By definition, a status code of 0 means the program executed successfully.

A non-zero status code is often used to indicate failure (and while this works fine on most operating systems, strictly speaking, it’s not guaranteed to be portable).

The C++ standard only defines the meaning of 3 status codes: 0, EXIT_SUCCESS, and EXIT_FAILURE. 0 and EXIT_SUCCESS both mean the program executed successfully. EXIT_FAILURE means the program did not execute successfully.

> If you want to maximize portability, you should only use 0 or EXIT_SUCCESS to indicate a successful termination, or EXIT_FAILURE to indicate an unsuccessful termination.

## 环境变量参数

​	由实现定义的，返回类型为 int，第三个参数，带有env字符串数组参数的main函数实现，能够将系统配置的环境变量引入程序。

​	其结构与命令行参数一致。

```c++
int main(int argc, char* argv[], char* env[])
{
    char** p = env;
    while(p++) {
        std::cout << *p << std::endl;
    }

}
```

![image-20210204220514386](https://gitee.com/masstsing/picgo-picserver/raw/master/20210204220514.png)

>   **注意：**
>
>   ​	不能省略去前两个命令行参数，没有这样的main函数实现。
>
>   ​	在windows和linux上这个形式的main函数都有实现。
>
>   ![image-20210204220718578](https://gitee.com/masstsing/picgo-picserver/raw/master/20210204220719.png)
>
>   **getenv**
>
>   ​	c语言的标准库 <stdlib>中提供了`getenv`函数，通过环境变量的名称来获取环境变量的值。
>
>   ```c++
>   #include <unistd.h>
>   
>   int main( int argc, char* argv[], char* env[])
>   {
>      // std::cout << getenv("PATH") << std::endl;
>     char** p = env;
>     char* pn = *p;
>     char name[1024] {0};
>   
>      while(pn) {
>          int i = 0;
>          for(; pn[i] != '='; ++i) {
>              name[i] = pn[i];
>          }
>          name[i] = '\0';
>          std::cout << name << "\n\t" <<getenv(name) << std::endl;
>   
>          i = 0;
>          pn = *p++;
>      }
>   }
>   ```
>
>   ![image-20210204224402863](https://gitee.com/masstsing/picgo-picserver/raw/master/20210204224403.png)



## main函数的特性

### 1. 不能在程序的任何地方调用它

#### <1> 不能递归调用

> ![image-20210128000421731](https://gitee.com/masstsing/picgo-picserver/raw/master/20210128000422.png)
>
> 能够编译通过，但是运行时产生core dump内存段错误。



#### <2> 不能取其函数地址

> ![image-20210128001049280](https://gitee.com/masstsing/picgo-picserver/raw/master/20210128001049.png)
>
> centos7, g++ 10.1下测试可以正常取地址。

### 2. 不能预定义，不能重载

实际上，名字 `main` 在全局命名空间中对函数保留（虽然可以用作类、命名空间、枚举和非全局命名空间中的任何实体的名字，但不能在任何命名空间中将名为“main”的函数声明为具有 **C 语言链接性**(C++17 起)）。

>   **C语言链接性**
>
>   ​	提供以不同程序语言编写的模块间的链接。
>
>   ps: c++/基本语法/声明/其他声明/连接说明（语言链接）

### 3. 不能定义为被弃置（=delete），或生命为具有C语言链接性（c++17起），inline，static或constexpr

> **=delete 弃置函数**
>
> ps：c++/基础语法/函数/函数定义
>
> 
>
> **inline 内联说明**
>
> ps：c++/基础语法/声明/说明符/inline说明符
>
> 
>
> **static 静态链接性**
>
> 名字可从当前翻译单元中的所有作用域使用。
>
> ps：c++/基础语法/声明/说明符/存储类说明符/连接
>
> 
>
> **constexpr**
>
> 指定变量或函数的值可在**常量表达式**中出现
>
> ps：c++/基础语法/声明/说明符/声明说明符/constexpr说明符
>
> 
>
> **常量表达式**
>
> 定义能在编译时求值的**表达式**。
>
> ps：c++/基础语法/表达式/常量表达式

### 4.主函数的函数体不需要包含 return 语句

当控制达到 `main` 的末尾而未遇到返回语句时，其效果为执行 return 0;。

>   **return 语句**
>
>   终止当前函数并返回指定值（若存在）给调用方函数。
>
>   ps: c++/语句/跳转语句/return

### 5.先结束函数，以return相同实参调用std::exit销毁静态对象并结束程序。

​	执行返回（或当到达 main 的末尾时的隐式返回），等价于先正常离开函数（这将销毁具有自动存储期的对象），然后用和 return相同的实参来调用 std::exit。（std::exit随即销毁静态对象并终止程序）

> ​		When the `main()` function returns (either by reaching the end of the function, or via a `return statement`), a number of different things happen.
>
> First, because we’re leaving the function, all local variables and function parameters are destroyed (as per usual).
>
> Next, a special function called `std::exit()` is called, with the return value from `main()` (the `status code`) passed in as an argument. 

>   **std::exit**
>
>   导致正常程序终止发生。
>
>   ps: c++/标准库/工具库/程序支持工具/std::exit

​	以**函数 try 块**定义主函数捕捉不到静态对象的析构函数所抛出的异常（由隐含的 std::exit所销毁的）。

>   **函数try块：**
>
>   https://zh.cppreference.com/w/cpp/language/function-try-block
>
>   建立围绕整个函数体的异常处理块。
>
>   ps ： c++/基本语法/异常/函数try块

### 6.(C++14 起) 主函数的返回类型不能被推导（不允许 auto main() {...}）。