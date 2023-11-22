# valgrind

---

​		valgrind是一套linux下的仿真调试工具集，由内核以及其他基于内核的调试工具组成。  

​		valgrind包含以下工具：

-  Memcheck：内存检查器，能够发现绝大多数的内存错误使用情况
  - 未初始化内存
  - 已释放内存
  - 内存访问越界
- Callgrind：类似`gprof`的分析工具，提供程序运行的很多信息。
  - 不需要在编译源代码时添加特殊选项，但推荐加上`-g`
  - 收集程序运行时的数据，建立函数调用关系图
  - 可以有选择的的进行cache模拟
  - 运行结束时会把分析数据写入一个文件 `callgrind_annotate`可以把文件内容转化成可读的形式
- Cachegrind：用来检查程序中缓存使用出现的问题。
  - 模拟CPU一级缓存I1、D1和二级缓存，能够精确指出程序中cache的丢失和命中
  - 还能提供cache丢失次数、内存引用次数以及每行代码、每个函数、每个模块及整个程序产生的指令数
- Helgrind：用来检查多线程程序中出现的竞争问题
  - 寻找内存中被多个线程访问，又没有一贯加锁的区域，往往是线程之间不同步的地方
  - 实现了名为Eraser的竞争检测算法
- Massif：堆栈分析器
  - 测量程序在堆栈中使用了多少内存
  - 告知堆块、堆管理块和栈的大小
  - 帮助减少内存的使用，在带有虚拟内存的现代系统中，还能加速程序的运行，减少程序停留在交换区中的几率
- Extension：可以利用Core模块，编写自己的特别内存调试工具



​		valgrind的默认工具是Memcheck，可以通过`--tool=Name`来指定其他的工具。

```shell
[root@VM-4-7-centos valgrind]# valgrind ./invalid_write
# pid
==18891== Memcheck, a memory error detector 		# tool
==18891== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al. 
==18891== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info # version
==18891== Command: ./invalid_write 
==18891== 

# ..................................

# 内存问题和泄露问题的总结
==18891== 
==18891== HEAP SUMMARY:
==18891==     in use at exit: 40 bytes in 1 blocks
==18891==   total heap usage: 2 allocs, 1 frees, 72,744 bytes allocated
==18891== 
==18891== LEAK SUMMARY:
==18891==    definitely lost: 40 bytes in 1 blocks
==18891==    indirectly lost: 0 bytes in 0 blocks
==18891==      possibly lost: 0 bytes in 0 blocks
==18891==    still reachable: 0 bytes in 0 blocks
==18891==         suppressed: 0 bytes in 0 blocks
==18891== Rerun with --leak-check=full to see details of leaked memory
==18891== 
==18891== For lists of detected and suppressed errors, rerun with: -s
==18891== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```



## 使用未初始化内存

```c++
int main()
{
    int a;
    std::cout << a  << std::endl;

    return 0;
}

// 程序跳转依赖一个未初始化的值
==25335== Conditional jump or move depends on uninitialised value(s)  
==25335==    at 0x4F4D1EC: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (locale_facets.tcc:874)
==25335==    by 0x4F5AD25: put (locale_facets.h:2375)
==25335==    by 0x4F5AD25: std::ostream& std::ostream::_M_insert<long>(long) (ostream.tcc:73)
==25335==    by 0x400798: main (in /home/lighthouse/MassLern/net/tool/valgrind/use_uninit)
==25335== 
// 使用了一个8字节的未初始化值
==25335== Use of uninitialised value of size 8
==25335==    at 0x4F4CF43: int std::__int_to_char<char, unsigned long>(char*, unsigned long, char const*, std::_Ios_Fmtflags, bool) (locale_facets.tcc:803)
==25335==    by 0x4F4D216: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (locale_facets.tcc:876)
==25335==    by 0x4F5AD25: put (locale_facets.h:2375)
==25335==    by 0x4F5AD25: std::ostream& std::ostream::_M_insert<long>(long) (ostream.tcc:73)
==25335==    by 0x400798: main (in /home/lighthouse/MassLern/net/tool/valgrind/use_uninit)
==25335== 
==25335== Conditional jump or move depends on uninitialised value(s)
==25335==    at 0x4F4CF55: int std::__int_to_char<char, unsigned long>(char*, unsigned long, char const*, std::_Ios_Fmtflags, bool) (locale_facets.tcc:806)
==25335==    by 0x4F4D216: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (locale_facets.tcc:876)
==25335==    by 0x4F5AD25: put (locale_facets.h:2375)
==25335==    by 0x4F5AD25: std::ostream& std::ostream::_M_insert<long>(long) (ostream.tcc:73)
==25335==    by 0x400798: main (in /home/lighthouse/MassLern/net/tool/valgrind/use_uninit)
==25335== 
==25335== Conditional jump or move depends on uninitialised value(s)
==25335==    at 0x4F4D24B: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (locale_facets.tcc:896)
==25335==    by 0x4F5AD25: put (locale_facets.h:2375)
==25335==    by 0x4F5AD25: std::ostream& std::ostream::_M_insert<long>(long) (ostream.tcc:73)
==25335==    by 0x400798: main (in /home/lighthouse/MassLern/net/tool/valgrind/use_uninit)
```



## 内存读写越界

```c++
int main()
{
    int len = 4;
    int *pt = (int *) malloc(len * sizeof(int));
    int *p = pt;

    for ( int i = 0; i < len;++i )
        p++;

    *p = 5;
    std::cout << "the value of p is " << *p<<"\n";
    return 0;
}

// 非法写入4字节
==27843== Invalid write of size 4
==27843==    at 0x40076C: main (in /home/lighthouse/MassLern/net/tool/valgrind/rw_overflow)
==27843==  Address 0x5afdc90 is 0 bytes after a block of size 16 alloc'd
==27843==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==27843==    by 0x400741: main (in /home/lighthouse/MassLern/net/tool/valgrind/rw_overflow)
```



## 内存重叠

```c++
int main()
{
    char x[50];
    int i;
    for ( i = 0; i < 50;++i )
        x[i] = i + 1;

    strncpy(x + 20, x, 20);
    strncpy(x + 20, x, 21);

    return 0;
}

// 源地址和目标地址重叠
==29646== Source and destination overlap in strncpy(0x1ffeffffc9, 0x1ffeffffb5, 21)
==29646==    at 0x4C2D843: __strncpy_sse2_unaligned (vg_replace_strmem.c:555)
==29646==    by 0x400585: main (in /home/lighthouse/MassLern/net/tool/valgrind/mem_cover)
```



## 内存管理错误

​		常见的分配方式有：静态存储、栈上分配、动态堆分配。静态存储和栈上分配无须自行管理存储，动态堆分配可能会出现管理错误。

- 申请释放不匹配
  - 内存泄露
  - `double free`
- 语言特性混用（`malloc / free`，`new/delete`）
- 释放后仍然读写

```c++
int main()
{
    char *p = (char *) malloc(10);
    delete p;
}

// 特性混用
==32743== Mismatched free() / delete / delete []
==32743==    at 0x4C2B6DF: operator delete(void*, unsigned long) (vg_replace_malloc.c:595)
==32743==    by 0x4006FD: main (in /home/lighthouse/MassLern/net/tool/valgrind/feature_mix)
==32743==  Address 0x5afdc80 is 0 bytes inside a block of size 10 alloc'd
==32743==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==32743==    by 0x4006E3: main (in /home/lighthouse/MassLern/net/tool/valgrind/feature_mix)
```

```c++
int main()
{
    char *p = (char *) malloc(10);
    free(p);
    p[1] = 'a';
    return 0;
}

// 非法写入
==1142== Invalid write of size 1
==1142==    at 0x4006DC: main (in /home/lighthouse/MassLern/net/tool/valgrind/write_afterdel)
==1142==  Address 0x5afdc81 is 1 bytes inside a block of size 10 free'd
==1142==    at 0x4C2B06D: free (vg_replace_malloc.c:540)
==1142==    by 0x4006D3: main (in /home/lighthouse/MassLern/net/tool/valgrind/write_afterdel)
==1142==  Block was alloc'd at
==1142==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==1142==    by 0x4006C3: main (in /home/lighthouse/MassLern/net/tool/valgrind/write_afterdel)
```

```c++
int main()
{
    char *p = (char *) malloc(10);
    free(p);
    free(p);
}

// double free
 ==1818== Invalid free() / delete / delete[] / realloc()
==1818==    at 0x4C2B06D: free (vg_replace_malloc.c:540)
==1818==    by 0x4006DF: main (in /home/lighthouse/MassLern/net/tool/valgrind/dbl_free)
==1818==  Address 0x5afdc80 is 0 bytes inside a block of size 10 free'd
==1818==    at 0x4C2B06D: free (vg_replace_malloc.c:540)
==1818==    by 0x4006D3: main (in /home/lighthouse/MassLern/net/tool/valgrind/dbl_free)
==1818==  Block was alloc'd at
==1818==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==1818==    by 0x4006C3: main (in /home/lighthouse/MassLern/net/tool/valgrind/dbl_free)
```

​		内存泄露分为

- 确定的
  - 直接泄露： definitely lost
  - 间接泄露：indirectly lost
  - 二者区别是是否有具名
- 可能的 ： possibly lost