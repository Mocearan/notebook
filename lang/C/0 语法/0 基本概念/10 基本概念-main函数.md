# 主函数

C语言/语法/基本概念/主函数

---

​		每个要在宿主环境中运行的C程序应当含有一个名为`main`的全局函数，它被指定为程序的启动点。

## 形式

它应当有下列形式之一：

1. ```int main(void) {body} ```
2. ```int main(int argc, char* argv[]) { body }```
3. ```/* 其他由实现定义的形式，返回类型为 int */```

> - argc
>
>   非负数，表示从程序运行的环境传递给程序的实参个数。
>
> - argv
>
>   指针，指向包含 argc + 1 个指针的数组的首元素。  
>
>   数组末元素为空指针，若其前面有任何元素，则它们指向一组字符串，表示从执行环境传递给程序的若干参数。
>
>   若 argv[0] 不是空指针，或等价地 argc > 0 ，则它指向表示本程序名的字符串，或空字符串。
>
> - body
>
>     main 函数的函数体
>
> 名字 `argc` 和 `argv` ，以及形参的类型的表示方式是任取的：
>
> ​	```int main(int ac, char** av) ```同样合法。
>
> `main()` 的一种常见的由实现定义的形式还有（除 `argc` 和 `argv` 之外的）第三个参数，类型为 `char*[]` ，指向[指向执行环境变量的指针数组](http://pubs.opengroup.org/onlinepubs/9699919799/functions/exec.html)。

## 返回值

​		如果使用返回语句，返回值会用作隐式调用exit()的参数。

​		值`0`和`EXIT_SUCCESS`表示成功终止，值`EXIT_FAILURE`表示失败终止。



## 调用时机

​		在程序启动时，`main` 函数在**初始化**具有**静态存储期**的对象之后被调用。指明在宿主环境（操作系统）中的程序入口点。



## 命令行参数

​		双形参形式的 `main` 函数的形参允许从执行环境中传递任意的多字节字符串（即，命令行参数）。

​		各个指针 `argv[1] .. argv[argc-1]` 指向对应的每个字符串的第一个字符。指针 `argv[0]` 指向表示用于执行该程序自身的名字的字符串（当执行环境不支持时为空字符串 ""）。

​		如果宿主环境不能同时提供大写和小写字母，则转换命令行参数为小写。

​		字符串可以修改，而且任何修改会留存到程序终止时，尽管这些参数不会传出到宿主环境。如通过`strtok`使用它们。

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i)
    {
        char *token = strtok(argv[i], "/");
        while (token != NULL)
        {
            printf("%s\n", token);
            token = strtok(NULL, "/");
        }
        printf("\n");
    }
    return 0;
}
```

> ![image-20210815011455850](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210815011455850.png)

### getopt

​	linux下提供了帮助解析命令行参数的系统调用。

```c++
头文件 #include<unistd.h>
int getopt(int argc,char * const argv[ ],const char * optstring);
extern char *optarg;
extern int optind, opterr, optopt;
/*
  getopt() 所设置的全局变量包括：
  optarg —— 指向当前选项参数（如果有）的指针。
  optind —— 再次调用 getopt() 时的下一个 argv 指针的索引。
  optopt —— 最后一个未知选项。

  参数 optstring为选项字符串， 告知 getopt()可以处理哪个选项以及哪个选项需要参数，如果选项字符串里的字母后接着冒号“:”，则表示还有相关的参数，全域变量optarg 即会指向此额外参数。
  如果在处理期间遇到了不符合optstring指定的其他选项getopt()将显示一个错误消息，并将全域变量optopt设为“?”字符，如果不希望getopt()打印出错信息，则只要将全域变量opterr设为0即可。
  optstring中的指定的内容的意义（例如getopt(argc, argv, "ab:c:de::");）
  1.单个字符，表示选项（如下例中的abcde各为一个选项）。
  2.单个字符后接一个冒号：表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。该参数的指针赋给optarg（如下例中的b:c:）。
  3 单个字符后跟两个冒号，表示该选项后可以跟一个参数，也可以不跟。如果跟一个参数，参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。(如上例中的e::，如果没有跟参数，则optarg = NULL)
*/
```

详情请自行查阅linux系统编程相关手册。

```c++
#include <unistd.h>
#include <cstdio>

void show(char ch)
{
  printf("ch     a:%c\n",     ch); 
  printf("optarg a:%s\n", optarg); 
  printf("optind a:%d\n", optind);
  printf("opterr a:%d\n", opterr);
  printf("optopt a:%d\n", optopt);
  printf("\n");
}


int main(int argc, char *argv[])
{
	int ch;

	while((ch = getopt(argc,argv,"a:bc::"))!= -1)
	{
      show(ch);
	}
	return 0;
}
```

![image-20210204221932093](https://gitee.com/masstsing/picgo-picserver/raw/master/20210204221932.png)

​	将操作系统命令行上给定的参数转换为由 `argv` 所指代的多字节字符数组的方式，可能涉及到由实现定义的处理过程：

>   -   [分析 C++ 命令行参数](https://msdn.microsoft.com/en-us/library/17w5ykft.aspx) MSDN
>   -   [Shell 介绍](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_01) POSIX

### 环境变量参数

​		由实现定义的，返回类型为 int，第三个参数，带有env字符串数组参数的main函数实现，能够将系统配置的环境变量引入程序。

​	其结构与命令行参数一致。

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    for (int ndx = 0; ndx != argc; ++ndx)
        printf("argv[%d] --> %s\n", ndx, argv[ndx]);
    printf("argv[argc] = %p\n", (void *)argv[argc]);
}
```

> ![image-20210815010648556](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210815010648556.png)

> **注意：**
>
> ​	不能省略去前两个命令行参数，没有这样的main函数实现。
>
> ​	在windows和linux上这个形式的main函数都有实现。
>
> ![image-20210204220718578](https://gitee.com/masstsing/picgo-picserver/raw/master/20210204220719.png)
>
> **getenv**
>
> ​	c语言的标准库 <stdlib>中提供了`getenv`函数，通过环境变量的名称来获取环境变量的值。
>
> ```c
> #include <stdio.h>
> #include <stdlib.h>
> 
> int main(int argc, char *argv[], char *env[])
> {
>     // std::cout << getenv("PATH") << std::endl;
>     char **p = env;
>     char *pn = *p;
>     char name[1024] = {0};
> 
>     while (pn)
>     {
>         int i = 0;
>         for (; pn[i] != '='; ++i)
>         {
>             name[i] = pn[i];
>         }
>         name[i] = '\0';
>         printf("%s\n\t%s\n", name, getenv(name));
> 
>         i = 0;
>         pn = *p++;
>     }
> }
> ```
>
> ![image-20210815012116117](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210815012116117.png)

## main 函数的特性

1. 程序不能提供此函数的原型

2. 如果主函数的返回类型与int兼容，则从对main的首次调用的返回（非任何后继的、递归的调用），等价于以主函数返回值为参数执行`exit`函数。

   > ​		`exit`将调用`atexit`所注册的函数，冲入并关闭所有流，删除`tmpfile`所创建的文件， 并返回控制给执行环境。
   >
   > > **tmpfile：**
   > >
   > > ​		创建并打开一个临时文件。
   > >
   > > ps：c语言/语言/文件I/O/函数/文件上的操作

3. `main`函数的返回值

   - 在c99前，如果没有`return`的值，或没有`return`语句，则返回给宿主环境的终止状态未定义。
   - 在c99后
     - 如果返回类型与`int`不兼容（`void main(void)`），则返回给宿主环境的值未指定。
     - 如果与`int`兼容，则返回给执行环境的值与如同执行`return 0;`的结果相同。









