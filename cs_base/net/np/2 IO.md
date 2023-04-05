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

## I/O接口

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
//    MSG_OOB， 指定接收带外数据，即通过紧急指针发送的数据
//    MSG_PEEK, 读缓冲区的数据但并不取走，数据还保留在缓冲区中

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
// msg:  指定多重缓冲区传输数据

///////////////////

// sockfd, socket 文件描述符, 不能是其他文件描述符
```



#### select / poll / epoll





### 异步

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
fcntl(fd, F_SETFL, flag | O_NONBLOCK);
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

