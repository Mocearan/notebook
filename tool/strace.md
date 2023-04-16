# strace

---

​		`strace`跟踪系统调用，展示程序与操作系统内核的交互过程。

## 系统调用

​		系统调用是操作系统内核内建函数，完成系统级别的功能。是提供给用户空间的接口，系统调用会进行用户空间到内核空间的转换。

​		系统调用使用`<errno.h>`中的`errno`全局变量来进行错误处理。

- 函数不发生错误，`errno`的值不会被设置，且不是`0`
- 全局通用，线程不安全
- `printf`这样的输出函数也可能造成`errno`的改变，应拷贝使用



## strace 查看系统调用

```C++
#include <iostream>

int main()
{
    int a;
    std::cin >> a;
    std::cout << a << std::endl;
    return 0;
}
```



- `strace`通过`fork/clone`创建一个子进程，在子进程中调用`exec`载入需要的程序。
- `execve("/usr/bin/test", ["test"], 0x7ffefb42d3b0 /* 32 vars */)`
- `brk(NULL)`，返回值为内存管理的起始地址
- 使用`mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)`进行匿名内存映射，以此获取`4096`字节的内存空间
  - 不使用具体的文件名，避免文件的创建打开过程中对文件名的访问，只能在父子进程间的进程间通信中使用
- `access("/etc/ld.so.preload", R_OK)` 检验`/etc/ld.so.preload`是否存在
- `open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC)` 尝试打开`/etc/ld.so.cache`
- `fstat(3, {st_mode=S_IFREG|0644, st_size=44805, ...})` 获取`/etc/ld.so.cache`文件信息
- `mmap(NULL, 44805, PROT_READ, MAP_PRIVATE, 3, 0)` 将`/etc/ld.so.cache`映射至内存
- `close(3)` 关闭`open`打开的`/etc/ld.so.cache`文件
- 从`/lib64/libc.so.6`的`libc`库中读取832字节，即ELF头信息，获取进程的进入点。
  - `open("/lib64/libc.so.6", O_RDONLY|O_CLOEXEC) = 3`
  - `read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0 &\2\0\0\0\0\0"..., 832)`
- `mprotect(0x7f3d0ed30000, 2093056, PROT_NONE)` 保护一段内存
- `munmap(0x7f3d0f14f000, 44805)` 将`/etc/ld.so.cache`文件从内存中去映射 
  - 对应`mmap(NULL, 106176928, PROT_READ, MAP_PRIVATE, 3, 0)`



## 跟踪信号传递

```c++
// read(0, 0x7f3810251000, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
// --- SIGTERM {si_signo=SIGTERM, si_code=SI_USER, si_pid=32238, si_uid=0} ---
// +++ killed by SIGTERM +++
// Terminated
```



## 统计系统调用数量

```shell
[root@VM-4-7-centos strace.cpp]# strace -c ./test
8
8
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 41.83    0.000064           6        10           mprotect
 18.30    0.000028           9         3         3 access
 11.76    0.000018           1        18           mmap
  9.80    0.000015           3         5           read
  5.23    0.000008           1         7           fstat
  4.58    0.000007           7         1           write
  4.58    0.000007           2         3           brk
  3.92    0.000006           6         1           munmap
  0.00    0.000000           0         5           open
  0.00    0.000000           0         5           close
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000153                    60         3 total
```



## strace 调试

​		使用`strace`调试的目的是查看源码之下，系统调用层面上产生的错误。

## 参数

- `-o`， 将`strace`的结果输出到文件，默认输出设备是`STDERR`
  - `-o file` 等同于`2>file`
- `-T`，显示每个系统调用消耗的时间
- `-t/-tt/-ttt` 记录每次系统调用发生的时间，精确到秒、微秒和UNIX时间戳的微秒
