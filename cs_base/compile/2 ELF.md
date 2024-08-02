# ELF

​		ELF是一种用于二进制文件、可执行文件、目标代码、共享库和核心转储的标准文件格式。

---

​		ELF标准的目的是为软件开发人员提供一组二进制接口定义，这些接口可以拓展到多种操作环境中，减少重新编码、编译程序的负担。

## 发展

- Unix最早的可执行文件格式为`a.out`，它的设计简单。但
- 出现共享库的概念后，`a.out`的格式就不太适用了
- 于是设计产生了`COFF`格式标准
  - 在目标文件中引入了“段”机制
  - 不同的目标文件可以拥有不同的数量和类型的段
  - 定义了调试数据格式
- `ELF`基于`COFF`
  - 比`COFF`更具可扩展性与灵活性



## `readeld`

​		用来显示一个或多个ELF格式的目标文件信息，可以通过选项来控制。

- `-v`，版本
- `-h`，帮助
- `-a test`，显示test的全部信息
- `-h test`， 显示test的ELF头部信息，ELF文件开始的前52字节
- `-l test`，显示test的program head table中每个program head entry的信息
- `-S test`，显示test的section head table中每个section head entry的信息
- `-g test`，显示test的section group信息
- `-s test`，显示test的symbol table中每个symbol table entry的信息
- `-e test`，显示test的全部头信息，包括ELF head，section head和program head，等同于`-h -l -S test`
- `-n test`，显示test的note段信息
- `-r test`，显示test中可重定位段的信息
- `-d test`，显示test中dynamic section中的信息
- `-V test`，显示test中GNU version段信息

​		`readelf`和`objdump`的功能类似，但是`readelf`显示的信息更为具体，不依赖BFD库。



## `nm`

​		查看指定程序中的符号表相关内容。

- 第一列是当前符号的地址
- 第二列是当前符号的类型
- 第三列是当前符号的名称

​		用于：

- 判断指定程序中是否定义指定的符号
  - `nm -C proc | grep symbol`
  - `-C`可以排除c++中`name mangle`机制显示符号名
- 解决程序编译时`undefined reference`错误以及`multiple definition`的错误
- 查看某个符号的地址，以及在进程空间的大概位置(`.bss / .data /.text`)，具体可以通过第二列的类型来判断



## strip

​		能够去除执行文件中不必要的标识符及调试信息，可减小文件大小而不影响正常使用。

​		通常值在已经调试和测试过的生成模块上使用`strip`命令来减小对象文件所需的存储量开销。

​		选项：

- `-l`，从对象文件中去除行号信息
- `-r`，除外部符号和静态符号，去除全部符号表信息。同时去除调试段和`typchk`段。不去除重定位信息。
- `-t`，去除大多数符号表信息，不去除函数符号或行号信息
- `-V`，版本号
- `-x` 去除符号表信息，但不去除静态或外部符号信息。同时去除重定位信息，因此将不可能链接到该文件

## 类型

​		目标文件有三种类型：

- 可重定位的目标文件
- 可执行的目标文件
- 可被共享的目标文件



### 可重定位目标文件

​		以汇编器汇编产生的`.o`文件作为输入，链接器经连接处理后

- 生成一个可执行的目标文件或者一个可被共享的（动态库）对象文件（`.so`）
- 使用`ar`工具将一组`.o`文件直接归档为一个`.a`静态库文件



### 可执行的目标文件

​		`linux`中有两种可执行的文件：

- 由代码编译产生的可执行程序
  - 由操作系统直接运行
- 由解释器解释的脚本程序（shell、python）
  - 由操作系统中运行的程序去执行
  - `source x.sh`
  - `python x.py`



### 可被共享的目标文件

​		即动态库`.so`文件。动态库在使用过程中必须经过两个步骤：

- 链接器将其和其他可重定位的文件(`.o`)以及其他的`.so`链接后，生成另外的可共享的目标文件`.so`或者可执行的目标文件
- 运行时，动态链接器拿它和一个可执行的目标文件以及另外一些可共享的目标文件（`.so`）一起处理，在`linux`系统中创建一个进行映像



## ELF结构

​		有两种视图可以用来说明`ELF`的组成：

- 链接视图
  - 用于组成不同的可重定位文件，参与可执行文件或者可共享对象文件的链接构建
- 执行视图
  - 用于组成可执行文件或者可共享对象文件，进行运行时内存中的进行映像构建

|      链接视图      |      执行视图      |
| :----------------: | :----------------: |
|      ELF头部       |      ELF头部       |
| 程序头部表（可选） |     程序头部表     |
|       节区1        |       节区1        |
|        ...         |        ...         |
|       节区n        |       节区n        |
|        ...         |        ...         |
|     节区头部表     | 节区头部表（可选） |



​		ELT文件头固定放置在不同类型对象文件的最前面，因此可以使用`file`命令来查看文件的ELF类型。

```shell
[root@VM-4-7-centos elf]# file helloworld 
helloworld: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.32, not stripped
```

- ELF头部（ELF header）：记录了除头部之外的分节信息
- 分节(section)：装载数据记录的最小容器
- 程序头部表（program headers)
- 分节头部表（section headers）

### ELF 头部

​		可以使用`readelf`工具读出ELF文件头的内容。

> `readelf`不支持显式`archive`（`.a`）文件

```shell
[root@VM-4-7-centos elf]# readelf -h helloworld 
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x4008c0					
  Start of program headers:          64 (bytes into file)
  Start of section headers:          39632 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         35
  Section header string table index: 34
```

- `  Entry point address: 0x4008c0`

  - 目标文件的程序进入点，指当前程序执行时的第一条要运行的指令地址
  - `0x0`意味着没有进入点，可重定位文件只是提供再链接，.o文件没有进入点
  - 可执行文件的进入点指向C库`_start`
  - `.so`的进入点指向`call_gmon_start`

- ` Number of section headers: 35`

  - 目标文件中的分节数量

- `Number of program headers:  9`

  - `.o`文件中`program heders`为`0`

  

### 分节 section

​		分节是ELF文件中用来装载内容数据的最小容器，每个section都撞在了性质属性相同的内容。

- `text section`，装载可执行代码
- `data section`，装载被初始化的数据
- `bss section`，装载未被初始化的数据
- `.rec section`，装载重定位条目
- `symtab / dynsym section`，装载符号信息
- `strtab / dynstr section`，装载字符串信息
- 其他为满足不同目的设置的`section`，如调试信息、动态链接和加载等。

​		可以使用`readelf -x [Nr]`查看对应分节中的内容

- 其中`[Nr]`是分节头部表中的序号
- 内容是机器码，不具备可读性
- 使用`objdump`可以查看其内容
  - `objdump -d -j .text helloworld`
  - `-d`对分节进行反汇编，即从机器码出发，推导出相应的汇编指令

#### `.text`

#### `.data`

#### `.strtab`

​		`readelf -x num file`以16进制方式显式指定段内容。

- `num`指定段的索引，或字符串指定文件中的段名
- 也可以用`hexdump`直接dumping出`.strtab`开头的32字节数据

#### `.symbol`

​		字符串表在真正链接和生成进程映像过程中是不需要使用的，但是对调试程序来说很有帮助。

​		使用`readelf -x`查看符号表的内容可读性太差，一般使用`readelf -s`或`objdump -t`来查看

### section head table 分节头部表

​		`readelf -S`可以查看ELF分节表的内容

```shell
[root@VM-4-7-centos elf]# readelf -S helloworld 
There are 35 section headers, starting at offset 0x9ad0:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .interp           PROGBITS         0000000000400238  00000238
       000000000000001c  0000000000000000   A       0     0     1
  [ 2] .note.ABI-tag     NOTE             0000000000400254  00000254
       0000000000000020  0000000000000000   A       0     0     4
  [ 3] .hash             HASH             0000000000400278  00000278
       000000000000004c  0000000000000004   A       4     0     8
  [ 4] .dynsym           DYNSYM           00000000004002c8  000002c8
       0000000000000150  0000000000000018   A       5     1     8
  [ 5] .dynstr           STRTAB           0000000000400418  00000418
       00000000000001e3  0000000000000000   A       0     0     1
  [ 6] .gnu.version      VERSYM           00000000004005fc  000005fc
       000000000000001c  0000000000000002   A       4     0     2
  [ 7] .gnu.version_r    VERNEED          0000000000400618  00000618
       0000000000000060  0000000000000000   A       5     2     8
  [ 8] .rela.dyn         RELA             0000000000400678  00000678
       0000000000000030  0000000000000018   A       4     0     8
  [ 9] .rela.plt         RELA             00000000004006a8  000006a8
       0000000000000120  0000000000000018  AI       4    21     8
  [10] .init             PROGBITS         00000000004007c8  000007c8
       000000000000001a  0000000000000000  AX       0     0     4
  [11] .plt              PROGBITS         00000000004007f0  000007f0
       00000000000000d0  0000000000000010  AX       0     0     16
  [12] .text             PROGBITS         00000000004008c0  000008c0
       0000000000000232  0000000000000000  AX       0     0     16
  [13] .fini             PROGBITS         0000000000400af4  00000af4
       0000000000000009  0000000000000000  AX       0     0     4
  [14] .rodata           PROGBITS         0000000000400b00  00000b00
       000000000000004e  0000000000000000   A       0     0     8
  [15] .eh_frame_hdr     PROGBITS         0000000000400b50  00000b50
       000000000000004c  0000000000000000   A       0     0     4
  [16] .eh_frame         PROGBITS         0000000000400ba0  00000ba0
       000000000000013c  0000000000000000   A       0     0     8
  [17] .init_array       INIT_ARRAY       0000000000600dd8  00000dd8
       0000000000000018  0000000000000008  WA       0     0     8
  [18] .fini_array       FINI_ARRAY       0000000000600df0  00000df0
       0000000000000008  0000000000000008  WA       0     0     8
  [19] .dynamic          DYNAMIC          0000000000600df8  00000df8
       0000000000000200  0000000000000010  WA       5     0     8
  [20] .got              PROGBITS         0000000000600ff8  00000ff8
       0000000000000008  0000000000000008  WA       0     0     8
  [21] .got.plt          PROGBITS         0000000000601000  00001000
       0000000000000078  0000000000000008  WA       0     0     8
  [22] .data             PROGBITS         0000000000601078  00001078
       0000000000000010  0000000000000000  WA       0     0     8
  [23] .bss              NOBITS           00000000006010a0  00001088
       0000000000000118  0000000000000000  WA       0     0     32
  [24] .comment          PROGBITS         0000000000000000  00001088
       000000000000003f  0000000000000001  MS       0     0     1
  [25] .debug_aranges    PROGBITS         0000000000000000  000010c7
       0000000000000060  0000000000000000           0     0     1
  [26] .debug_info       PROGBITS         0000000000000000  00001127
       000000000000509a  0000000000000000           0     0     1
  [27] .debug_abbrev     PROGBITS         0000000000000000  000061c1
       0000000000000e1e  0000000000000000           0     0     1
  [28] .debug_line       PROGBITS         0000000000000000  00006fdf
       00000000000006e2  0000000000000000           0     0     1
  [29] .debug_str        PROGBITS         0000000000000000  000076c1
       00000000000014a0  0000000000000001  MS       0     0     1
  [30] .debug_loc        PROGBITS         0000000000000000  00008b61
       00000000000001e2  0000000000000000           0     0     1
  [31] .debug_ranges     PROGBITS         0000000000000000  00008d43
       00000000000000c0  0000000000000000           0     0     1
  [32] .symtab           SYMTAB           0000000000000000  00008e08
       00000000000007b0  0000000000000018          33    54     8
  [33] .strtab           STRTAB           0000000000000000  000095b8
       00000000000003ce  0000000000000000           0     0     1
  [34] .shstrtab         STRTAB           0000000000000000  00009986
       0000000000000145  0000000000000000           0     0     1
```

- `Address`表示进程的地址
- `Offset`表示该分节到文件头的距离
- `Size`表示分节的大小
- `EntSize`只对某些形式的分节有意义，如符号表`.symtab section`。
  - 内部包含一个表格、
  - 每个条目都有特定长度
- `Align`是地址对齐要求
- `Link / Info`记录`section head table`中的条目索引
- `Flags`表示对应分节的相关标志
  - `X`，`.text section`存储代码，即可执行
  - `W`，`.data / .bss`存放的是非堆栈定义的可写数据
    - `.data`存放初始化过的数据，如程序中定义的赋初值的全局变量
    - `.bss`存放未初始化的数据，意味着不确定这些数据在刚开始的值，即没必要为存储这些数据而在文件内多留出空间，因此`.bss`分节的大小总是0。动态链接器会在内存中开辟一定大小的空间来存放这些未初始化的数据，里面的内存单元都被初始化成0.可执行程序中虽然没有长度非0的`.bss`分节，但却记录着在程序运行时需要开辟多大的空间来容纳这些未初始化的数据
  - `A`，表示对应的分节是`Allocable`，是指在运行时进程需要使用它们，它们被加载器加载到内存中去
    - 一些`non-Allocable`的分节，它们只是被链接器、调试器或者其他类似工具所使用，并非参与进程的运行中去。都可以被`stip`工具从最后的可执行文件中删除掉，删除掉这些`section`的可执行文件依然能够运行，只不过没办法进行调试。
      - `.strtab`
      - `.symtab`等
    - 运行可执行程序时，`Allocable`分节会被加载器加载到内存，而`non-Allocable`的部分会继续留在可执行文件内。



### program head table

​		在执行视图下，使用`readelf -l`可以得到程序头部表的内容。可以显示出分节映射到段（`segment`）的信息。

- `R` 可读
- `E` 可执行
- `W` 可写

​		`linux`内核创建新的进程时，进程空间加载可执行程序的代码段和数据段后，还会加载动态链接器的代码段和数据（`linux`下通常是`/lib/ld-linux.so`符号链接所指向的那个程序，它本身就是一个动态库）。在这之后，内核将控制传递给动态链接库里面的代码。动态链接器接下来负责加载该命令程序所需要使用的各种动态库。加载完毕后，动态链接器才将控制传递给应用程序的`main`函数。之后程序才能运行。

