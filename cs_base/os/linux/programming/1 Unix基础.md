# 登录名

通过登录名和口令密码来登录Unix系统

---

口令文件（`/etc/passwd`）可以对登录名进行配置，登录项由7个以冒号分隔的字段组成：

​	登陆名、加密口令、用户ID、组ID、注释字段、用户家目录、用户shell

`sar:x:205:105:Stephen Rago:/home/sar:/bin/bash`

>   现今系统中，加密口令已经移动到其他文件中。

# shell

shell是一个命令行解释器，读取用户输入，执行命令并可能向用户输出必要的信息

---

常用的shell程序有`sh/ bash/ csh/ ksh/ tcsh`。

不同的linux发行版使用不同的默认shell，多是`bash`，也有替代品`dash`



# 文件和目录

Unix文件系统是由目录和文件组成的层次结构

---

## 文件系统

文件系统起始于根目录 `/`

目录是包含目录项的文件。每个目录项都包含一个文件名，同时还包含说明该文件属性的信息。

>   文件属性是指文件类型、文件带下、文件所有者、文件权限以及文件的最后修改时间。

## 文件名

目录中的各个名字称为文件名，`/`和空字符不能用于文件名中。

为了可移植性，一般只用字母、数字、`.`、`-`、`_`

创建目录时会自动创建两个文件名：`.`和`..`，指向当前目录和上级目录

## 路径名

由`/`分隔的一个或多个文件名组成的序列构成路径名。

由`/`开头的路径名称为绝对路径，否则称为相对路径。

相对路径从当前目录开始搜索该路径。

## 工作目录

每个进程都有一个工作目录，所有相对路径都从工作目录开始解释。

对于shell，shell当前所处的目录即工作目录。

## 家目录（起始目录）

登录时，工作目录为当前用户的家目录。



# 输入和输出

## 文件描述符

一个小的非负整数，内核用以表示一个特定进程正在访问的文件。

内核打开或创建一个新文件时，向用户空间返回一个文件描述符，用以通过文件描述符对文件进行读写。

## 标准输入输出及标准错误

每当运行一个新的进程，所有shell都为其打开三个文件描述符：标准输入、标准输出、标准错误。对应文件描述符0、1、2

## 不带缓冲的I/O

 系统调用提供通过文件描述符来操作的不带缓冲区的I/O接口：

`open/creat/read/write/lseek/close`

## 标准I/O

通过c语言标准库，提供基于系统调用的带缓冲区的I/O接口。如`putc/gets/printf`



# 程序和进程

程序（program）是一个存储在磁盘上的某个目录中的二进制可执行文件。

进程（process）是被`exec`系列函数加载到内存中，执行其中二进制指令的过程。

> 进程拥有ID，类型为`pid_t`，长整型。

> 进程控制：`fork / exec / waitpid`

线程（thread）是执行程序二进制指令的工作线，称为控制线程。

> 多个控制线程可以充分利用多处理器的并行能力
>
> 一个进程的所有线程共享同一地址空间、文件描述符、栈以及进程相关的属性。因为能访问同一存储区，所以各线程在访问共享数据时需要采取同步措施避免不一致性。
>
> 线程拥有ID，只在它所属的进程中有效。



# 错误处理

Unix系统出错时，通常会返回一个负值。通过整型变量`errno`来设置具有特定信息的值

---

在支持线程的环境中，多个线程共享进程地址空间，每个线程都有术语自己的局部`errno`以避免线程之间对错误的处理：

```c
extern int *__errno_location(void);
#define errno (*__errno_location())
```

如果么有出错，errno的值不会被社会，只有当函数返回值出错时才检查错误值。

任何函数都不会将`errno`值设置为0，而且在`<errno.h>`中定义的所有常量都不为0.

出错恢复：致命性和非致命性。

> 非致命性：
>
> `EAGAIN/ENFILE/ENOBUFS/ENOLCK/ENOSPC/EWOULDBLOCK/ENOMEM`

### 系统调用的错误处理机制

​		系统调用的错误处理机制通常通过函数返回值来表示，并通过设置全局变量`errno`来加以描述。

​		`errno`定义在`<errno.h>`中，`extern int errno;`，通过`perror/strerror`来处理错误的信息。

```c
#include <string.h>
char* strerror(int errnum);

#include <stdio.h>
void perror(const char* msg);
```



```c
perror("msg");
等价于
fprintf(stderr, "msg:%s", strerror(errno));
```

错误号：

```c
// 基本错误
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */


#define	EDEADLK		35	/* Resource deadlock would occur */
#define	ENAMETOOLONG	36	/* File name too long */
#define	ENOLCK		37	/* No record locks available */
#define	ENOSYS		38	/* Function not implemented */
#define	ENOTEMPTY	39	/* Directory not empty */
#define	ELOOP		40	/* Too many symbolic links encountered */
#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	ENOMSG		42	/* No message of desired type */
#define	EIDRM		43	/* Identifier removed */
#define	ECHRNG		44	/* Channel number out of range */
#define	EL2NSYNC	45	/* Level 2 not synchronized */
#define	EL3HLT		46	/* Level 3 halted */
#define	EL3RST		47	/* Level 3 reset */
#define	ELNRNG		48	/* Link number out of range */
#define	EUNATCH		49	/* Protocol driver not attached */
#define	ENOCSI		50	/* No CSI structure available */
#define	EL2HLT		51	/* Level 2 halted */
#define	EBADE		52	/* Invalid exchange */
#define	EBADR		53	/* Invalid request descriptor */
#define	EXFULL		54	/* Exchange full */
#define	ENOANO		55	/* No anode */
#define	EBADRQC		56	/* Invalid request code */
#define	EBADSLT		57	/* Invalid slot */

#define	EDEADLOCK	EDEADLK

#define	EBFONT		59	/* Bad font file format */
#define	ENOSTR		60	/* Device not a stream */
#define	ENODATA		61	/* No data available */
#define	ETIME		62	/* Timer expired */
#define	ENOSR		63	/* Out of streams resources */
#define	ENONET		64	/* Machine is not on the network */
#define	ENOPKG		65	/* Package not installed */
#define	EREMOTE		66	/* Object is remote */
#define	ENOLINK		67	/* Link has been severed */
#define	EADV		68	/* Advertise error */
#define	ESRMNT		69	/* Srmount error */
#define	ECOMM		70	/* Communication error on send */
#define	EPROTO		71	/* Protocol error */
#define	EMULTIHOP	72	/* Multihop attempted */
#define	EDOTDOT		73	/* RFS specific error */
#define	EBADMSG		74	/* Not a data message */
#define	EOVERFLOW	75	/* Value too large for defined data type */
#define	ENOTUNIQ	76	/* Name not unique on network */
#define	EBADFD		77	/* File descriptor in bad state */
#define	EREMCHG		78	/* Remote address changed */
#define	ELIBACC		79	/* Can not access a needed shared library */
#define	ELIBBAD		80	/* Accessing a corrupted shared library */
#define	ELIBSCN		81	/* .lib section in a.out corrupted */
#define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC	83	/* Cannot exec a shared library directly */
#define	EILSEQ		84	/* Illegal byte sequence */
#define	ERESTART	85	/* Interrupted system call should be restarted */
#define	ESTRPIPE	86	/* Streams pipe error */
#define	EUSERS		87	/* Too many users */
#define	ENOTSOCK	88	/* Socket operation on non-socket */
#define	EDESTADDRREQ	89	/* Destination address required */
#define	EMSGSIZE	90	/* Message too long */
#define	EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	92	/* Protocol not available */
#define	EPROTONOSUPPORT	93	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EADDRINUSE	98	/* Address already in use */
#define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	ENETDOWN	100	/* Network is down */
#define	ENETUNREACH	101	/* Network is unreachable */
#define	ENETRESET	102	/* Network dropped connection because of reset */
#define	ECONNABORTED	103	/* Software caused connection abort */
#define	ECONNRESET	104	/* Connection reset by peer */
#define	ENOBUFS		105	/* No buffer space available */
#define	EISCONN		106	/* Transport endpoint is already connected */
#define	ENOTCONN	107	/* Transport endpoint is not connected */
#define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ECONNREFUSED	111	/* Connection refused */
#define	EHOSTDOWN	112	/* Host is down */
#define	EHOSTUNREACH	113	/* No route to host */
#define	EALREADY	114	/* Operation already in progress */
#define	EINPROGRESS	115	/* Operation now in progress */
#define	ESTALE		116	/* Stale file handle */
#define	EUCLEAN		117	/* Structure needs cleaning */
#define	ENOTNAM		118	/* Not a XENIX named type file */
#define	ENAVAIL		119	/* No XENIX semaphores available */
#define	EISNAM		120	/* Is a named type file */
#define	EREMOTEIO	121	/* Remote I/O error */
#define	EDQUOT		122	/* Quota exceeded */

#define	ENOMEDIUM	123	/* No medium found */
#define	EMEDIUMTYPE	124	/* Wrong medium type */
#define	ECANCELED	125	/* Operation Canceled */
#define	ENOKEY		126	/* Required key not available */
#define	EKEYEXPIRED	127	/* Key has expired */
#define	EKEYREVOKED	128	/* Key has been revoked */
#define	EKEYREJECTED	129	/* Key was rejected by service */

/* for robust mutexes */
#define	EOWNERDEAD	130	/* Owner died */
#define	ENOTRECOVERABLE	131	/* State not recoverable */

#define ERFKILL		132	/* Operation not possible due to RF-kill */

#define EHWPOISON	133	/* Memory page has hardware error */
```



# 用户标识

## 用户ID

## 组ID

## 附属组ID



# 信号

信号（signal）用于通知进程发生了某种情况。

---

对于信号有三种处理方式：

1.   忽略信号
2.   系统默认方式处理
3.   用户注册处理程序



# 时间值

-   日历时间，UTC，1970.1.1 00:00:00，时间戳秒数
    -   time_t
-   进程事件，CPU时间，时间片，tik
    -   clock_t

# 系统编程

---

​        在操作系统上，利用系统调用、C库进行系统资源的访问使用。



## 系统资源

- 处理器
- 输入输出
- 进程管理
- 内存
- 设备
- 定时器
- 进程间通信
- 网络



## 系统调用

​		操作系统提供的系统功能的封装，由这些系统调用能够请求内核服务。

![image-20220216213801053](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220216213801053.png)



​		系统调用与C库形式上都是以C函数提供，是因为操作系统大都以C实现，且C能够容易的被多种语言调用。

​		C库函数有些事基于系统调用实现的，如malloc, free调用brk， printf调用write。有些纯逻辑功能则不涉及系统调用。

​		C库函数应该看做与高于系统调用的包装和扩展。

### 系统调用的内核处理

​		每个系统调用被赋予一个系统调用号，i386平台上，执行一个系统调用是通过int 0x80指令完成的。

​		`eax`寄存器存放系统调用号，`ebx/ecx/edx/esi/edi`存储系统调用参数，对于超过5个参数的系统嗲用，用一个寄存器指向用户空间存储所有系统调用参数的缓存。 



