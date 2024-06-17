# 网IO

---



## I/O

### IO阶段

​		典型的I/O分为两个阶段：数据准备和数据读写。

- 数据准备：根据对I/O事件就绪状态的行为，分为阻塞I/O和非阻塞I/O
  - 当请求的I/O操作未就绪，I/O请求不会被立即响应
  - 请求方需要等待请求的I/O事件就绪并完成I/O操作，无法放弃操作并持续等待称为阻塞
  - 请求方可立即放弃操作决定下一步操作称为非阻塞
- 数据读写：根据应用进程和内核的交互方式，分为同步I/O和异步I/O
  - 请求方主动拉取数据，称为同步；
    - 用户在请求I/O操作的上下文中，立即需要操作该I/O操作的内核缓冲区（从该缓冲区中拷贝出数据，或向该缓冲区写入数据）
  - 请求方被动接收应答方推送数据称为异步
    - 用户在请求I/O操作的上下文中，不直接操作该I/O操作的缓冲区，而是注册一个缓冲区让内核从该缓冲区中进行读写

​		一般在谈论I/O时，除了异步I/O接口，都是同步I/O。阻塞和非阻塞不是接口，而是文件对象的属性。

![image-20220720212715049](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720212715049.png)

​		同步操作中一般有一个重要的概念是超时，当不期望在最坏情况下无休止的等待数据准备，那么同步接口设计时提供设置超时时间，在规定内数据准备好了则正常的响应，如果在规定时间内没有准备好数据，则达到规定时间后接口也必须返回，一般也会通知当前的状态。

### 缓冲区

​		操作系统内核协议栈，会为已连接的``socket fd``配套基础设施：发送缓冲区和接收缓冲区。

- 缓冲区在每个`TCP socket`套接字中单独存在
  - `udp`没有发送缓冲区，只有已连接的``tcp socket fd``才会有发送缓冲区
- 缓冲区在创建套接字时由内核创建
- 缓冲区大小可以通过套接字选项来改变
- 不能无限制增大缓冲区
  - 内核缓冲区总是充满数据时会产生粘包问题
  - 同时网络的传输大小MTU也会限制每次发送的大小进行分片发送
  - 由于数据阻塞需要消耗大量内存资源，资源使用效率也得不到提高
- 关闭套接字时，内核也会将发送缓冲区的数据处理完，同时丢弃接收缓冲区中的数据



​		IO操作实际上不是将数据直接通过网络设备发送，而是经由内核协议栈的缓冲区，产生IO就绪的事件，然后进行实际的数据读写操作。

​		发送函数返回后，数据实质上并未完全被内核协议栈发送出去，但write实质上已经完成了功能可以返回，发送缓冲区的数据会在内核协议栈控制下被不断发送。

![image-20220720212044303](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720212044303.png)



![img](https://static001.geekbang.org/resource/image/fd/dc/fdcdc766c6a6ebb7fbf15bb2d1e58bdc.png)



## IO类型

​		阻塞与否是文件的IO属性，而非相关接口的行为。

### 阻塞 IO

### 非阻塞IO

## I/O 接口

​		I/O接口只有同步和异步之分，阻塞和非阻塞是文件描述符（套接字）的属性。

### 同步

#### read / write

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd,  const void *buf, size_t count);

// fd : 文件描述符, 可以是任意文件描述符
// buf ：  用于从fd对应的内核缓冲区（socket的接收发送缓冲区，文件的文件缓冲区）拷贝数据或向其中拷贝数据。
// count: 用于读取内核缓冲区的缓冲区大小，或向内核缓冲区拷贝的数据大小。

// 成功返回实际读取的字节数，返回 0 表示EOF（网络中表示对端发送了FIN包，处理断连）。失败返回-1
```

>   ​		对于普通fd， write将向文件系统缓冲区拷贝字节流，拷贝的字节流大小通常和输入参数size的值相同，否则表示出错。
>
>   ​		对于sockfd， 代表一个双向连接，write将向tcp发送缓冲区拷贝字节流，拷贝的字节数有可能比请求的数量少。

>    读到FIN包在read看来就是返回0.



#### readv / writev

​		`readv / writev`批量发送数据，减少I/O函数的调用次数，能提高数据通信效率。

- `writev`可以将多个缓冲中的数据一起发送
- `readv`可以由多个缓冲分别接收

​		当需要传输的数据在不同的缓冲区中，需要多次调用`write / read`时，可以通过1此`writev / readv`来代替，并提高效率。

```c++
#include <sys/uio.h>

struct iovec
{
    void* iov_vase; // buffer address
    size_t iov_len; // buffer size
};

ssize_t writev(int filedes, const struct iovec* iov, int iovcnt); // return bytes / -1
ssize_t readv(int filedes, const struct iovec* iov, int iovcnt); // return bytes / -1
```



#### recv/ send

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);

ssize_t recvfrom(int sockfd
                 , void *buf, size_t len
                 , int flags
                 , struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t sendto(int sockfd
               , const void *buf, size_t len
               , int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
// src_addr  发送方地址
// dest_addr 接收方地址
// addr_len  地址长度

/////////////////////
// buf ：  用于从socket的缓冲区拷贝数据或向其中拷贝数据。
// len ：  用于读取socket缓冲区的 缓冲区大小，或向内核缓冲区拷贝的数据大小。
// flag :  指定收发的特殊行为，常用的有：
//       0   ， 
//    MSG_OOB， 指定收发带外数据，即通过紧急指针发送的数据，接收端触发SIGURG信号，recv(..., MSG_OOB)
//    MSG_PEEK, 读缓冲区的数据但并不取走，数据还保留在缓冲区中

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
// msg:  指定多重缓冲区传输数据

///////////////////

// sockfd, socket 文件描述符, 不能是其他文件描述符
```



```c
// MSG_OOB
void urg_handler(int signo)
{
    recv(sock, buf, sizeof(buf)-1, MSG_OOB)；
}

fcntl(sock, F_SETOWN, getpid()); // getpid()的进程处理sock上的
signal(SIGURG, urg_handler);
```





### 异步

#### aio

```c++
struct aiocb {
    int aio_fildes
    off_t aio_offset
    volatile void *aio_buf
    size_t aio_nbytes
    int aio_reqprio
    struct sigevent aio_sigevent
    int aio_lio_opcode
};

aio_read;
aio_write;
```



### IO复用

​		`select/poll/epoll`都是阻塞函数。

#### select

​		`select`通过`fd_set`结构向内核传递要进行I/O事件检测的范围。`fd_set`是以位来表示位置对应的`socket fd`是否有事件发生的数组。

```c++
typedef struct
{
	__fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
} fd_set;

/* Access macros for `fd_set'.  */
#define	FD_SET(fd, fdsetp)		__FD_SET (fd, fdsetp)
#define	FD_CLR(fd, fdsetp)		__FD_CLR (fd, fdsetp)
#define	FD_ISSET(fd, fdsetp)	__FD_ISSET (fd, fdsetp)
#define	FD_ZERO(fdsetp)			__FD_ZERO (fdsetp)
```

​		

```c++
#include <sys/select.h>
#include <sys/time.h>

struct timeval{
    long tv_sec;
    long tv_usec; //microseconds
};

int select(int maxfd, fd_set* rset, fd_set* wset, fd_set* eset, const struct timeval* timeout); // return count/-1;
// maxfd 检测的数量， 最大文件描述符值+1，因为文件描述符从0开始，+1为数量
// timeout, select中内置定时器，防止无事件发生时永久阻塞
```

- 设置文件描述符
  - 指定检测范围
  - 设置超时
- 调用`select`
- 处理检测结果

​		`select`性能优先，并发处理上百个连接后性能快速下降。

- 要检测所有的文件描述符
- 每次传递检测对象信息
  - 向系统调用拷贝传递大量数据的代价很高

#### poll



#### epoll

​		`epoll`是对`poll`的改进，内核版本`2.5.44`引入

​		相比于`select`，`epoll`无须每次传递检测对象信息，无需检测所有的文件描述符。`epoll`由操作系统负责保存检测对象文件描述符，只提供`epoll`操作接口给用户。

```c++
#include <sys/epoll.h>

enum
  {
    EPOLL_CLOEXEC = 02000000,
#define EPOLL_CLOEXEC EPOLL_CLOEXEC
    EPOLL_NONBLOCK = 00004000
#define EPOLL_NONBLOCK EPOLL_NONBLOCK
  };

/* Creates an epoll instance.  
	Returns an fd for the new instance.
   	The "size" parameter is a hint specifying the number of file
   descriptors to be associated with the new instance. 
   	The fd returned by epoll_create() should be closed with close().  */
extern int epoll_create (int __size) __THROW;

/* Same as epoll_create but with an FLAGS parameter.  The unused SIZE
   parameter has been dropped.  */
extern int epoll_create1 (int __flags) __THROW;


/* Manipulate an epoll instance "epfd". 
	Returns 0 in case of success,-1 in case of error ( the "errno" variable will contain the specific error code ) 
	The "op" parameter is one of the EPOLL_CTL_* constants defined above. 
	The "fd" parameter is the target of the operation. 
	The "event" parameter describes which events the caller is interested in and any associated user data.  */
/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl().  */
#define EPOLL_CTL_ADD 1	/* Add a file descriptor to the interface.  */
#define EPOLL_CTL_DEL 2	/* Remove a file descriptor from the interface.  */
#define EPOLL_CTL_MOD 3	/* Change file descriptor epoll_event structure.  */

extern int epoll_ctl (int __epfd, int __op, int __fd,
		      struct epoll_event *__event) __THROW;



/* Wait for events on an epoll instance "epfd". 
	Returns the number of triggered events returned in "events" buffer. Or -1 in case of error with the "errno" variable set to the specific error code. 
	The "events" parameter is a buffer that will contain triggered events. 
	The "maxevents" is the maximum number of events to be returned ( usually size of "events" ). 
	The "timeout" parameter specifies the maximum wait time in milliseconds (-1 == infinite).

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int epoll_wait (int __epfd, struct epoll_event *__events,
		       int __maxevents, int __timeout);


/* Same as epoll_wait, but the thread's signal mask is temporarily
   and atomically replaced with the one provided as parameter.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int epoll_pwait (int __epfd, struct epoll_event *__events,
			int __maxevents, int __timeout,
			const __sigset_t *__ss);
```

​	

##### EPOLL事件结构

​		`epoll`通过`epoll_event`结构传递发生变化的文件描述符信息

```c++
typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event
{
  uint32_t events;	/* Epoll events */
  epoll_data_t data;	/* User data variable */
} __EPOLL_PACKED;
```

##### epoll 事件类型

```c++
enum EPOLL_EVENTS
  {
    EPOLLIN = 0x001, // 可读事件，需要从接收缓冲区读取数据
#define EPOLLIN EPOLLIN 
    EPOLLPRI = 0x002,
#define EPOLLPRI EPOLLPRI // 紧急事件，收到OOB数据的情况
    EPOLLOUT = 0x004,
#define EPOLLOUT EPOLLOUT // 可写事件，发送缓冲区为空等可以立即发送数据的情况
    EPOLLRDNORM = 0x040,
#define EPOLLRDNORM EPOLLRDNORM
    EPOLLRDBAND = 0x080,
#define EPOLLRDBAND EPOLLRDBAND
    EPOLLWRNORM = 0x100,
#define EPOLLWRNORM EPOLLWRNORM
    EPOLLWRBAND = 0x200,
#define EPOLLWRBAND EPOLLWRBAND
    EPOLLMSG = 0x400,
#define EPOLLMSG EPOLLMSG
    EPOLLERR = 0x008, // 发生错误的情况
#define EPOLLERR EPOLLERR
    EPOLLHUP = 0x010, // 断开连接或半关闭的情况，边缘触发下非常有用
#define EPOLLHUP EPOLLHUP
    EPOLLRDHUP = 0x2000,
#define EPOLLRDHUP EPOLLRDHUP
    EPOLLWAKEUP = 1u << 29,
#define EPOLLWAKEUP EPOLLWAKEUP
    EPOLLONESHOT = 1u << 30, // 发生一次事件后，响应文件描述符不再收到时间通知，需要使用EPOLL_CTL_MOD再次设置事件的检测
#define EPOLLONESHOT EPOLLONESHOT
    EPOLLET = 1u << 31 // 边缘触发模式
#define EPOLLET EPOLLET
  };
```



##### epoll事件触发模式

​		`epoll`有两种事件通知机制：水平触发和边缘触发。二者的区别在于，在通知用户事件发生的方式上：

- 水平触发，只要缓冲区状态没有改变就持续通知
- 边缘触发，缓冲区状态改变时通知一次

​		两者在不能一次`read/write`调用处理完缓冲区上所有数据时才体现出差异。

​		关于缓冲区状态和事件

- 接收缓冲区

  - 空缓冲区
    - LT：没数据，不可读
    - ET：缓冲区从空到数据时，通知一次可读
  - 缓冲区不空
    - LT：有数据，可读
    - ET：缓冲区不空时，每当有新数据进入缓冲区，通知一次可读
  - 缓冲区满
    - LT：有数据且已满，可读
    - ET：缓冲区满时，不触发可读

- 发送缓冲区

  - 空缓冲区

    - LT：没数据，可写
    - ET：没数据，通知一次可写

  - 缓冲区不空

    - LT：有数据但还有空间，可写
    - ET：不通知

  - 缓冲区满

    - LT：没空间了，不可写
    - ET：不通知

    > 可写中ET的不通知是因为，系统通知用户可以将数据拷贝到发送缓冲区，无法静态的仅从缓冲区判定。
    >
    > 而且，写事件往往不会发生写不完的情况，因为用户拷贝数据到发送缓冲区由系统处理。而不是像读事件那样，用户从系统的接收缓冲区拷贝数据，存在读不完的情况。

​		需要注意的是：

- LT 模式下

  - 读事件触发后，可以按需收取想要的字节数，不用把本次接收到的数据收取干净

    ​	即不用循环到 recv 或者 read 函数返回 -1，错误码为 EWOULDBLOCK 或 EAGAIN）

  - 不需要写事件一定要及时移除，避免不必要的触发，浪费 CPU 资源

    - 一般不使用`epoll`来检测写事件，而是直接发送

  - 使用 LT 模式，可以自由决定每次收取多少字节，但是可能会导致多次触发

- ET 模式下

  - 读事件必须把数据收取干净

    ​	循环到 recv 或者 read 函数返回 -1，处理错误码为 EWOULDBLOCK 或 EAGAIN

  - 写事件触发后，如果还需要下一次的写事件触发来驱动任务（例如发上次剩余的数据），你需要继续注册一次检测可写事件

    - 一般不使用`epoll`来检测写事件，而是直接发送

  - 使用 ET 模式，必须每次都要将数据收完，其优点是触发次数少。



## IO模型

​		阻塞IO操作的返回时机，需要等待将所有数据完全拷贝至发送缓冲区后返回：

-   缓冲区容量大于一次拷贝的数据量，发送函数直接返回
-   缓冲区容量小于一次拷贝的数据量，阻塞等待缓冲区数据不断被封装发送（TCP-MSS，IP-MTU），应用数据不断被拷贝到发送缓冲区，直到某刻全部拷贝至内核发送缓冲区

### 阻塞I/O

![image-20220720233019356](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720233019356.png)

![image-20220720212222657](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720212222657.png)

​		使用阻塞I/O，用户进程会阻塞在相应的I/O函数上。在相应的条件满足时，相应的I/O函数会从内核空间返回到用户空间。

```c
/* 从socketfd描述字中读取"size"个字节. */

//          |     	   	size            |
//  buffer  ---------------|--length----|
//  	   ↑            result   
//  	buffer_pointer          
    

size_t readn(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int length = size;

    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            if (errno == EINTR)
                continue;     /* 考虑非阻塞的情况，这里需要再次调用read */
            else
                return (-1);
        } else if (result == 0)
            break;                /* EOF(End of File)表示套接字关闭 */

        length -= result;
        buffer_pointer += result;
    }
    return (size - length);        /* 返回的是实际读取的字节数*/
}
```



### 非阻塞I/O

![image-20220720233042161](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720233042161.png)

​		![image-20220720231542007](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720231542007.png)

​		使用非阻塞I/O， 用户进程不会阻塞在相应的I/O函数上，会从内核空间立即返回用户空间，如果没有数据，会返回-1， errno为`EWOULDBLOCK`。用户进程应该轮询I/O直到得到成功的返回。

​		这种单纯非阻塞的I/O模型应用较少，在内核没有接收到数据的时候，用户进程仍然需要维护持续的recv知道数据到来，这个过程中会反复在内核空间和用户空间切换，同时轮询造成CPU资源浪费。

> 这种占用CPU时间，轮询等待的机制，称为忙等待或忙轮询。

```c
// 1.
int flag = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flag | O_NONBLOCK);
// 2.
setsockopt(NONBLOCK)
```

```c
// set fd nonblock
// read
//    while egain read
```





### I/O复用

![image-20220720233338908](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720233338908.png)

![image-20220720233239242](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720233239242.png)

​		I/O复用是使用I/O复用函数接管对应文件描述符，在内核中管理其I/O事件的发生，当I/O事件触发 或 设置的阻塞等待时间超时，从I/O复用函数中返回，然后进行I/O。

​		linux中常用的I/O复用函数有，`select / poll / epoll`。

```c++
while(1) {
    select / poll / epoll // wait socket ready or timeout
    if(nready)
        // read
}
```







### 信号驱动 I/O

![image-20220720234532918](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720234532918.png)

![image-20220720234140776](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720234140776.png)

​		信号驱动I/O是使用`SIGIO`信号，将I/O事件托管给内核信号处理机制，当I/O事件触发`SIGIO`，会回调`signal`或`sigaction`设置的回调函数进行I/O，但这种I/O也是发生在用户空间，通过同步I/O接口来操作。

​		这种从内核中通知到用户空间，可以形成一种异步I/O机制，程序注册I/O信号处理事件后，直接进行其他操作。而I/O信号处理回调函数会被内核调用执行。提供了消息通知机制，不需要用户进程轮询就绪状态，减少了系统API的调用次数，提高了效率。

​		这种模型也不太常用， 

### 异步I/O

![image-20220720234509793](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720234509793.png)

![image-20220720234204943](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220720234204943.png)

​		使用linux异步I/O函数，注册一个缓冲区到内核空间，然后就可以执行其他操作。当内核检测到I/O事件，并将缓冲区填充后，会返回用户空间，并使用注册在异步I/O函数中的信号来通知用户进程。

> ​		系统内核级的异步I/O的性能优势来源于，真正的I/O发生在内核空间中，用户不用显式使用I/O函数就可以得到想要的数据。
>
> ​		由于大多数linux系统没有实现完善的异步I/O函数，所以这种内核级的异步I/O没有得到广泛的使用。
>
> ​		windows系统下的IOCP 完成端口，是一套完善的异步I/O模型。







## IO超时

​		有三种方法进行I/O的超时设置：

### alarm 信号

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

![image-20210921124347693](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210921124347693.png)

> 由于比较繁琐，一般不会使用alarm来实现超时。

### 套接字选项

​	`SO_SNDTIMEO`和`SO_RECVTIMEO`，通过`setsockopt`来进行设置。

```c
setsockopt(sock, SOL_SOCKET, SO_RECVTIMEO, 5);

int ret = read(sock, buf, sizeof(buf));
if(ret == -1 && errno == EWOULDBLOCK)
    errno = ETIMEOUT;
```

> 一般也不会选择这种方式，有些套接字的实现不支持这两个选项。

### select

​		使用多路复用I/O函数`select`，将描述符注册到内核来检测超时。

