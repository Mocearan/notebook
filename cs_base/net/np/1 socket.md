# socket

​		`socket`是系统``TCP/IP``协议栈的编程接口，用于网际交互，本地进程间通信。

---

- 在linux上，`socket`是一种文件类型，在使用`socket`进行网络数据交互的过程中，使用文件``I/O``接口读写数据。

  ​	socket的编程句柄也是文件描述符，文件描述符是系统分配给文件的数字ID，用以代表系统中的一个输入输出设备。

  ​	其中0,1,2被用作标准输入、标准输出、标准错误的设备ID。

- windows上的`socket`是参考BSD Unix套接字设计的，大致与linux套接字类似。

  - `winsock2.h / ws2_32.lib`
  - windows socket的变成句柄不是文件描述符


​		![image-20210903151813901](https://gitee.com/masstsing/picgo-picserver/raw/master/20210903151814.png)

​		

## 定长数据类型

​		网络编程中要处理大量的数据传递、IO，因此协议的设计是紧凑的，相关的接口一般使用定长数据类型来表示。

![image-20220316233833186](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220316233833186.png)

​		POSIX中定义的数据类型：

- `int8_t / uint8_t`
- `int16_t / uint16_t`
- `int32_t / uint32_t`
  - `<sys/types.h>`
- `sa_family_t` 地址族
- `socklen_t` 
  - `<sys/socket.h>`
- `in_addr_t` ip地址，声明为`uint32_t`
- `in_port_t`，端口号，声明为`uint16_t`
  - `<netinet/in.h>`

## 字节序

#### 大端字节序 Big Endian

​		最高有效位（MSB：Most Significant Bit）存储在最低内存地址处。

​		最低有效位（LSB：Lowest Significant Bit）存储在最高内存地址处。

![image-20220316235119658](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220316235119658.png)

#### 小端字节序 Littel endian

​		MSB存储在最高内存地址，LSB存储在最低内存地址。

![image-20220316235127982](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220316235127982.png)



#### 主机字节序与网络字节序

​		不同的主机有不同的字节序，为了使得各种异构平台之间能够通信，在通信时统一使用网络字节序。

> x86 小端字节序
>
> Motorola6800 大端字节序
>
> ARM字节序可配置*99*

- 网络字节序使用大端字节序
- 协议栈的实现会自动将传输的数据进行大小端转换，但地址结构需要认为保证字节序
  - 但地址结构是由编写代码的人以文本的方式填充的
  - 同时地址结构作为协议头部，是传输数据的一部分，协议栈无法判断这些人为填充的字节序
  - 因此，需要代码编写者来保证字节序的转换



#### 字节序转换函数

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);

uint16_t htons(uint16_t hostshort);

uint32_t ntohl(uint32_t netlong);

uint16_t ntohs(uint16_t netshort);

// h - host 本地主机字节序
// n - network 网络字节序
// s - short  16位
// l - long   32位
```





## 套接字类型

​		如果C/S两端通信使用的套接字类型不同，在客户端连接时，会提示“Protocol wrong type for socket ”

```c
/* Types of sockets.  */
enum __socket_type
{
  SOCK_STREAM = 1,		/* Sequenced, reliable, connection-based
				   byte streams.  */
#define SOCK_STREAM SOCK_STREAM
  SOCK_DGRAM = 2,		/* Connectionless, unreliable datagrams
				   of fixed maximum length.  */
#define SOCK_DGRAM SOCK_DGRAM
  SOCK_RAW = 3,			/* Raw protocol interface.  */
#define SOCK_RAW SOCK_RAW
  SOCK_RDM = 4,			/* Reliably-delivered messages.  */
#define SOCK_RDM SOCK_RDM
  SOCK_SEQPACKET = 5,		/* Sequenced, reliable, connection-based,
				   datagrams of fixed maximum length.  */
#define SOCK_SEQPACKET SOCK_SEQPACKET
  SOCK_DCCP = 6,		/* Datagram Congestion Control Protocol.  */
#define SOCK_DCCP SOCK_DCCP
  SOCK_PACKET = 10,		/* Linux specific way of getting packets
				   at the dev level.  For writing rarp and
				   other similar things on the user level. */
#define SOCK_PACKET SOCK_PACKET

  /* Flags to be ORed into the type parameter of socket and socketpair and
     used for the flags parameter of paccept.  */

  SOCK_CLOEXEC = 02000000,	/* Atomically set close-on-exec flag for the
				   new descriptor(s).  */
#define SOCK_CLOEXEC SOCK_CLOEXEC
  SOCK_NONBLOCK = 00004000	/* Atomically mark descriptor(s) as
				   non-blocking.  */
#define SOCK_NONBLOCK SOCK_NONBLOCK
};

```



### 面向连接的流式套接字 SOCK_STREAM

​		提供面向连接的、可靠的数据传输服务，数据无差错，无重复的发送，且按发送顺序接收。对应TCP协议。

> 一个连接的表示使用四元组：
>
> （ local_ip:local_port , peer_ip, peer_port）

​		因为面向无连接，故可以在一个建立好连接的通信信道上任意持续发送或接收数据。数据经套接字内核缓冲区，被消弭掉多次发送之间的边界。

### 面向无连接的数据报式套接字 SOCK_DGRAM

​		提供无连接服务，不提供无错保证，数据可能丢失或重复，并且接收顺序混乱。对应UDP协议。

​		因为面向无连接，所以两次发送的数据之间是不关联的，各自独立称为一个自封闭的报文。每个数据报文需要单独的接收。

### 原始裸套接字 SOCK_RAW

​		跨越传输层，直接对IP层进行封装操作。

​		



## 套接字地址结构



### 协议族 sa_family_t

```c
#include <sockaddr.h>
/* This macro is used to declare the initial common members
   of the data types used for socket addresses, `struct sockaddr',
   `struct sockaddr_in', `struct sockaddr_un', etc.  */
#define	__SOCKADDR_COMMON(sa_prefix) \
  sa_family_t sa_prefix##family
```



#### `sa_family_t`

```c
/* Address families.  */
#define	AF_UNSPEC	PF_UNSPEC
//#define	AF_LOCAL	PF_LOCAL
//#define	AF_UNIX		PF_UNIX
#define	AF_FILE		PF_FILE
//#define	AF_INET		PF_INET
#define	AF_AX25		PF_AX25
#define	AF_IPX		PF_IPX
#define	AF_APPLETALK	PF_APPLETALK
#define	AF_NETROM	PF_NETROM
#define	AF_BRIDGE	PF_BRIDGE
#define	AF_ATMPVC	PF_ATMPVC
#define	AF_X25		PF_X25
//#define	AF_INET6	PF_INET6
#define	AF_ROSE		PF_ROSE
#define	AF_DECnet	PF_DECnet
#define	AF_NETBEUI	PF_NETBEUI
#define	AF_SECURITY	PF_SECURITY
#define	AF_KEY		PF_KEY
#define	AF_NETLINK	PF_NETLINK
#define	AF_ROUTE	PF_ROUTE
#define	AF_PACKET	PF_PACKET
#define	AF_ASH		PF_ASH
#define	AF_ECONET	PF_ECONET
#define	AF_ATMSVC	PF_ATMSVC
#define AF_RDS		PF_RDS
#define	AF_SNA		PF_SNA
#define	AF_IRDA		PF_IRDA
#define	AF_PPPOX	PF_PPPOX
#define	AF_WANPIPE	PF_WANPIPE
#define AF_LLC		PF_LLC
#define AF_IB		PF_IB
#define AF_MPLS		PF_MPLS
#define AF_CAN		PF_CAN
#define AF_TIPC		PF_TIPC
#define	AF_BLUETOOTH	PF_BLUETOOTH
#define	AF_IUCV		PF_IUCV
#define AF_RXRPC	PF_RXRPC
#define AF_ISDN		PF_ISDN
#define AF_PHONET	PF_PHONET
#define AF_IEEE802154	PF_IEEE802154
#define AF_CAIF		PF_CAIF
#define AF_ALG		PF_ALG
#define AF_NFC		PF_NFC
#define AF_VSOCK	PF_VSOCK
#define	AF_MAX		PF_MAX
```



### 通用地址结构 sockaddr

​		`socket`设计用于多种协议族，不同的协议族（协议栈）之间的地质结构可能不同，而通用地址结构使用一个缓冲区来存放可能的任何形式，用`sin_family`来决定以何种方式来解析该地址。

```c
#include <sys/socket.h>
/* Structure describing a generic socket address.  */
struct sockaddr
{
    __SOCKADDR_COMMON ( sa_ );	/* Common data: address family and length.  */
    char sa_data[ 14 ];			/* Address data.  */
};
```

![image-20210904164519696](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210904164519696.png)

> 14字节的socket address data用于填充：`sin_port(2),sin_addr(4),sin_zero(8)`。

### 网际套接口地址结构 sockaddr_in

​		实际使用中，会在填充完网际套接字地址结构后转换成通用地址结构以调用`socket api`。

​		`sockaddr_in`是为``INET``即``IPv4``设计专用的地址结构，但为了与通用地址结构保持一致，所以仍旧需要添加`sin_family`指定地址族。

```c++
/// man 7 ip

struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* internet address */
};

/* Internet address. */
struct in_addr {
    uint32_t       s_addr;     /* address in network byte order */
};
```

​	

```c
#include <netinet/in.h>
// network address
/* Internet address.  */
typedef uint32_t in_addr_t;
struct in_addr
  {
    in_addr_t s_addr;
  };

// socket address
/* Structure describing an Internet socket address.  */
struct sockaddr_in
  {
    __SOCKADDR_COMMON (sin_);
    in_port_t sin_port;			/* Port number.  */
    struct in_addr sin_addr;	/* Internet address.  */

    /* Pad to size of `struct sockaddr'.  */
    unsigned char sin_zero[sizeof (struct sockaddr) -
			   __SOCKADDR_COMMON_SIZE -
			   sizeof (in_port_t) -
			   sizeof (struct in_addr)];
  };
```

#### `sin_port`

​		保存16位端口号，网络字节序。

#### `sin_addr`

​		保存32位IP地址信息，网络字节序。 

#### `sin_zero`

​		padding，为使`sockaddr_in`的大小和`sockaddr`结构体保持一致而插入的成员。

#### 预定义地址

```c
/* Address to accept any incoming messages.  */
#define	INADDR_ANY		((in_addr_t) 0x00000000) // 0.0.0.0
/* Address to send to all hosts.  */
#define	INADDR_BROADCAST	((in_addr_t) 0xffffffff)
/* Address indicating an error return.  */
#define	INADDR_NONE		((in_addr_t) 0xffffffff)
```

- `0.0.0.0`  和 `127.0.0.1` 的区别
  - `0.0.0.0`
    - 接受所有入口的消息
    - `INADDR_ANY（0.0.0.0）`在多宿主计算机（多网卡，多IP），只要端口号一致，就可以从不同IP地址接收数据。服务器优先考虑这种方式。
  - `127.0.0.1`只能接受本地局域网内的消息，127.0.0.1可以使用本地域名`localhost`替代。



```c
// sin_addr.s_addr 和 sin_port 都需要转换成网络字节序
// sin_addr.s_addr 等价的三种赋值方式：
    srvaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    srvaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    inet_aton("0.0.0.0", &servaddr.sin_addr);
// sin_addr.s_addr 都被赋值为0
```



### 本地套接口地址结构 sockaddr_un

```c
#define	__SOCKADDR_COMMON(sa_prefix) \
  sa_family_t sa_prefix##family


/* Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  */
struct sockaddr_un
  {
    __SOCKADDR_COMMON (sun_);
    char sun_path[108];		/* Path name.  */
  };

```



### sockaddr

#### 地址获取

##### getsockname 

```c
#include <sys/socket.h>

// 获取
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// sockfd, 本地使用的 socket 套接字
// sockaddr, 用于传出地址结构
// addrlen， 地址结构长度
```

>![image-20210912232319613](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210912232319613.png)
>
>```c
>if ( ( connect( conn_fd, (struct sockaddr *) &srvaddr, sizeof( srvaddr ) ) ) < 0 )
>   ERR_EXIT( "connect" );
>
>struct sockaddr_in localaddr;
>socklen_t addrlen = sizeof( socklen_t );
>if ( getsockname( conn_fd, (struct sockaddr *) &localaddr, &addrlen ) < 0 )
>ERR_EXIT("getsockname failed");
>
>printf( "peer ip: %s, port: %d\n", inet_ntoa( localaddr.sin_addr )
>  , ntohs( localaddr.sin_port ) );
>```
>

##### getpeername

```c
#include <sys/socket.h>

// 获取连接对端的地址
int   getpeername(int   sockfd,   struct   sockaddr  *addr,  socklen_t *addrlen);
```

> 当accept时，传出地址结构使用的是空指针，后续如果需要使用地址，就需要用getpeername来获取。
>
> > accept地址结构使用NULL时，长度指针也需要使用NULL。
>
> ![image-20210912234929422](C:/Users/msi-/AppData/Roaming/Typora/typora-user-images/image-20210912234929422.png)



##### gethostname / sethostname

```c
#include <unistd.h>

// 获取主机名（主机名即域名）
int gethostname(char *name, size_t len);
int sethostname(const char *name, size_t len);
```



##### gethostbyname / gethostbyaddr

```c
#include <netdb.h>
extern int h_errno;

// 根据主机名获取所有IP地址
struct hostent *gethostbyname(const char *name);

#include <sys/socket.h>       /* for AF_INET */
// 根据addr地址结构获取域名
struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);
```

##### `struct hostent`

```c
// The hostent structure is defined in <netdb.h> as follows:

struct hostent {
 char  *h_name;            /* official name of host */
 char **h_aliases;         /* alias list */
 int    h_addrtype;        /* host address type */
 int    h_length;          /* length of address */
 char **h_addr_list;       /* list of addresses */
}
#define h_addr h_addr_list[0] /* for backward compatibility */



/*
h_name -----------------> official hostname \0
h_alias -----[]  ---> alias #1 \0
             []  ---> alias #2 \0
    		NULL
h_addrtype
h_length
h_addr_list ----[]----> IP addr #1
    	        []----> IP addr #2
     		   NULL
*/
```

The members of the hostent structure are:

- `h_name `
   The official name of the host.
  官方域名，代表某一主页，实际上一些著名公司的域名并未使用官方域名注册

- `h_aliases`
      An array of alternative names for the host, terminated by a NULL pointer.
      可以通过多个域名访问同一主页，同一Ip可以绑定多个域名。因此，除官方域名之外，还可以指定其他域名，通过h_aliases获得

- `h_addrtype`
      The type of address; always AF_INET or AF_INET6 at present.
      不仅支持IPv4还支持IPv6，因此可以通过此变量获取保存在h_addr_list中的ip地址的地址族信息，若是IPv4，则此变量为AF_INET

- `h_length`
      The length of the address in bytes.
      保存ip地址长度，IPv4为4字节，若IPv6为16字节

- `h_addr_list`
      An array of pointers to network addresses for the host (in network byte order), terminated by a NULL pointer.
      最重要的成员，易整数形式保存域名对应的ip地址。用户较多的网站可能分配多个ip给同一域名，利用多个服务器进行负载均衡。此时同样可以通过此变量获取ip地址信息。

- `h_addr `

  ​	The first address in h_addr_list for backward compatibility.

- 相关扩展函数

  ```c++
  void sethostent(int stayopen);
  
  void endhostent(void);
  
  void herror(const char *s);
  
  const char *hstrerror(int err);
  
  /* System V/POSIX extension */
  struct hostent *gethostent(void);
  
  /* GNU extensions */
  struct hostent *gethostbyname2(const char *name, int af);
  
  int gethostent_r(
   struct hostent *ret, char *buf, size_t buflen,
   struct hostent **result, int *h_errnop);
  
  int gethostbyaddr_r(const void *addr, socklen_t len, int type,
                   struct hostent *ret, char *buf, size_t buflen,
                   struct hostent **result, int *h_errnop);
  
  int gethostbyname_r(const char *name,
                   struct hostent *ret, char *buf, size_t buflen,
                   struct hostent **result, int *h_errnop);
  
  int gethostbyname2_r(const char *name, int af,
                    struct hostent *ret, char *buf, size_t buflen,
                    struct hostent **result, int *h_errnop);
  ```

  





#### 地址转换函数

​		IP地址本质上是一个整形数据，但常以易于记忆的点分十进制表示法来表示。

​		地址转换函数将一个 点分十进制的可读IP地址，和 32位整数（ipv4) 的标准协议地址相互转换

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 将一个点分十进制的可读IP地址，转换为一个包含32位整数（ipv4)的标准协议地址的in_addr结构体
int inet_aton(const char *cp, struct in_addr *inp);
	// inet_aton(addr, &addr_inet.sin_addr);

// 将一个点分十进制的可读IP地址，转换为一个32位整数（ipv4)的标准协议地址(网络字节序)。
// 并且可以检测无效的IP地址。
in_addr_t inet_addr(const char *cp);

in_addr_t inet_network(const char *cp);

// 将一个32位整数（ipv4)的标准协议地址，转换为一个点分十进制的可读IP地址。
char *inet_ntoa(struct in_addr in); // 注意char*

struct in_addr inet_makeaddr(int net, int host);

in_addr_t inet_lnaof(struct in_addr in);

in_addr_t inet_netof(struct in_addr in);


// Feature Test Macro Requirements for  glibc (see feature_test_macros(7)):
//		inet_aton(),  inet_ntoa(): _BSD_SOURC || _SVID_SOURCE
    

// a - address, 可读的点分十进制IP字符串
// n - network, 包含32位整数（ipv4)的标准协议地址的in_addr结构体
```





## socket

​	`<sys/socket.h>`

----

### socket

​		在系统中创建一个套接字文件并返回一个套接字描述符。

> 与一般文件操作的`creat / open`类同。

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int socket(int domain, int type, int protocol);

// domain 指定通信协议族 protocol family
// type  指定socket 数据传输类型
//    SOCK_STREAM   流式套接字.
//	  SOCK_DGRAM	数据报套接字
//	  SOCK_RAW		原始裸套接字
//    ...
// protocol  指定协议
//	 通常在给定的domain协议族中，只有一个协议支持特定的套接字类型
//   在这种情况下，protocol可以指定为0。

// 成功返回套接口文件描述字，简称套接字，是一个非负整数。
// 失败返回-1
```

- domain

  ​	协议地址族，提供套接字使用的协议分类。

  ​	![image-20210905005310112](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210905005310112.png)

  > `AF_*` 与 `PF_*`是通过宏来一致定义的，不同之处在于，AF是 address family ，表示地址族， PF 是 protocol family ， 表示协议族。

- type

  ​	套接字类型是指套接字的数据传输方式。

  ```c++
  /* Types of sockets.  */
  enum __socket_type
  {
    SOCK_STREAM = 1,		/* Sequenced, reliable, connection-based
  				   byte streams.  */
  #define SOCK_STREAM SOCK_STREAM
    SOCK_DGRAM = 2,		/* Connectionless, unreliable datagrams
  				   of fixed maximum length.  */
  #define SOCK_DGRAM SOCK_DGRAM
    SOCK_RAW = 3,			/* Raw protocol interface.  */
  #define SOCK_RAW SOCK_RAW
    SOCK_RDM = 4,			/* Reliably-delivered messages.  */
  #define SOCK_RDM SOCK_RDM
    SOCK_SEQPACKET = 5,		/* Sequenced, reliable, connection-based,
  				   datagrams of fixed maximum length.  */
  #define SOCK_SEQPACKET SOCK_SEQPACKET
    SOCK_DCCP = 6,		/* Datagram Congestion Control Protocol.  */
  #define SOCK_DCCP SOCK_DCCP
    SOCK_PACKET = 10,		/* Linux specific way of getting packets
  				   at the dev level.  For writing rarp and
  				   other similar things on the user level. */
  #define SOCK_PACKET SOCK_PACKET
  
    /* Flags to be ORed into the type parameter of socket and socketpair and
       used for the flags parameter of paccept.  */
  
    SOCK_CLOEXEC = 02000000,	/* Atomically set close-on-exec flag for the
  				   new descriptor(s).  */
  #define SOCK_CLOEXEC SOCK_CLOEXEC
    SOCK_NONBLOCK = 00004000	/* Atomically mark descriptor(s) as
  				   non-blocking.  */
  #define SOCK_NONBLOCK SOCK_NONBLOCK
  };
  
  ```

  - **SOCK_NONBLOCK**

    ​	同`fcntl`将文件设置成`O_NONBLOCK`非阻塞是一样的。

  - **SOCK_CLOEXEC**

    ​	如果没有该选项，需要使用`F_SETFD(long)`设置文件描述符的`FD_CLOEXEC`选项。

- protocol

  ​	如果在同一协议族存在多个数据传输方式相同的协议，那么protocol将决定具体使用的协议。

  ```c++
  /* Standard well-defined IP protocols.  */
  enum
    {
      IPPROTO_IP = 0,	   /* Dummy protocol for TCP.  */
  #define IPPROTO_IP		IPPROTO_IP
      IPPROTO_ICMP = 1,	   /* Internet Control Message Protocol.  */
  #define IPPROTO_ICMP		IPPROTO_ICMP
      IPPROTO_IGMP = 2,	   /* Internet Group Management Protocol. */
  #define IPPROTO_IGMP		IPPROTO_IGMP
      IPPROTO_IPIP = 4,	   /* IPIP tunnels (older KA9Q tunnels use 94).  */
  #define IPPROTO_IPIP		IPPROTO_IPIP
      IPPROTO_TCP = 6,	   /* Transmission Control Protocol.  */
  #define IPPROTO_TCP		IPPROTO_TCP
      IPPROTO_EGP = 8,	   /* Exterior Gateway Protocol.  */
  #define IPPROTO_EGP		IPPROTO_EGP
      IPPROTO_PUP = 12,	   /* PUP protocol.  */
  #define IPPROTO_PUP		IPPROTO_PUP
      IPPROTO_UDP = 17,	   /* User Datagram Protocol.  */
  #define IPPROTO_UDP		IPPROTO_UDP
      IPPROTO_IDP = 22,	   /* XNS IDP protocol.  */
  #define IPPROTO_IDP		IPPROTO_IDP
      IPPROTO_TP = 29,	   /* SO Transport Protocol Class 4.  */
  #define IPPROTO_TP		IPPROTO_TP
      IPPROTO_DCCP = 33,	   /* Datagram Congestion Control Protocol.  */
  #define IPPROTO_DCCP		IPPROTO_DCCP
      IPPROTO_IPV6 = 41,     /* IPv6 header.  */
  #define IPPROTO_IPV6		IPPROTO_IPV6
      IPPROTO_RSVP = 46,	   /* Reservation Protocol.  */
  #define IPPROTO_RSVP		IPPROTO_RSVP
      IPPROTO_GRE = 47,	   /* General Routing Encapsulation.  */
  #define IPPROTO_GRE		IPPROTO_GRE
      IPPROTO_ESP = 50,      /* encapsulating security payload.  */
  #define IPPROTO_ESP		IPPROTO_ESP
      IPPROTO_AH = 51,       /* authentication header.  */
  #define IPPROTO_AH		IPPROTO_AH
      IPPROTO_MTP = 92,	   /* Multicast Transport Protocol.  */
  #define IPPROTO_MTP		IPPROTO_MTP
      IPPROTO_BEETPH = 94,   /* IP option pseudo header for BEET.  */
  #define IPPROTO_BEETPH		IPPROTO_BEETPH
      IPPROTO_ENCAP = 98,	   /* Encapsulation Header.  */
  #define IPPROTO_ENCAP		IPPROTO_ENCAP
      IPPROTO_PIM = 103,	   /* Protocol Independent Multicast.  */
  #define IPPROTO_PIM		IPPROTO_PIM
      IPPROTO_COMP = 108,	   /* Compression Header Protocol.  */
  #define IPPROTO_COMP		IPPROTO_COMP
      IPPROTO_SCTP = 132,	   /* Stream Control Transmission Protocol.  */
  #define IPPROTO_SCTP		IPPROTO_SCTP
      IPPROTO_UDPLITE = 136, /* UDP-Lite protocol.  */
  #define IPPROTO_UDPLITE		IPPROTO_UDPLITE
      IPPROTO_MPLS = 137,    /* MPLS in IP.  */
  #define IPPROTO_MPLS           IPPROTO_MPLS
      IPPROTO_RAW = 255,	   /* Raw IP packets.  */
  #define IPPROTO_RAW		IPPROTO_RAW
      IPPROTO_MAX
    };
  ```

- 返回值

  ![image-20210905005418723](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210905005418723.png)



### bind

​		绑定一个本地主机地址到套接字，实质是将``socket``文件和网卡设备关联。

​		![image-20210905014638364](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210905014638364.png)

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);


// sockfd,  套接字， socket返回的监听套接字
// addr， 	要绑定的通用地址, 通常将需要的地址结构赋值后，转换成通用地址结构传递
//  struct sockaddr {
//     sa_family_t  sa_family;
//     char         sa_data[14];
//  }
// addrlen   地址长度，用于处理通用地址，根据长度将地址转换为对应的地址结构

//  成功返回0， 失败返回-1
```



### listen

​		套接字用于监听进入的连接。

​		![image-20220704223103242](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220704223103242.png)

​		listen在bind之后和accept之前调用，将套接字从主动套接字变为被动套接字。

​		对于一个listen之后的监听套接字，内核要维护两个队列：

- 未完成队列（连接请求等待队列）

  客户端发起连接后，服务器收到连接请求，等待完成TCP三次握手过程

- 已完成队列

​		![image-20220704223036202](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220704223036202.png)

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int listen(int sockfd, int backlog);

// sockfd:   socket 函数返回的监听套接字
// backlog:   规定内核为此套接字排队的最大连接个数

//  成功返回0， 失败返回-1
```

- **backlog**

  ​	连接请求首先会通过等待连接请求的服务端监听套接字，进入连接请求等待队列。在内核完成相关创建工作后，将连接请求队列的请求添加到已完成队列中。

  推荐使用SOMAXCONN

  ![image-20220704223011409](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220704223011409.png)

  

### accept

​		从已完成队列返回第一个连接，如果已完成连接队列为空，则阻塞。

​		客户端调用connect函数之前，服务器端可能率先调用accept函数，进入阻塞状态等待客户端连接请求。

​		accept自动创建与该连接请求对应的客户端通信的socket，并通过`accept`返回该socket的fd。

​		![](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210905024228253.png)

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// sockfd, 服务器监听套接字
// addr，  返回对等方的套接字的通用地址
// addrlen， 返回对等方的套接字地址长度

// 成功返回用于通信的socket fd，为非负整数。失败返回-1

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/socket.h>

int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
// flags：
//        If flags is 0, then accept4() is the same as accept().  The following values can be bitwise ORed in flags to obtain different behavior:
//		SOCK_NONBLOCK   
//				Set  the  O_NONBLOCK  file  status flag on the new open file description.  Using this flag saves extra calls to fcntl(2) to achieve the same result.
//		SOCK_CLOEXEC    
//				Set the close-on-exec (FD_CLOEXEC) flag on the new file descriptor.  See the description of the O_CLOEXEC flag  in  open(2) for reasons why this may be useful.
```

#### EMFILE

​		`accept`接收连接并创建套接字时，发现进程所打开的套接字达到了上限。

解决方法：

- 调高进程文件描述符数目

- 硬等其他套接字关闭

- 退出程序

- 关闭监听套接字
  - 什么时候打开？
  
- epoll模型，改用`edge trigger`。
  - 问题是如果漏掉了一次`accept`，程序就再也不会收到新的连接
  
- 预留一个空闲的文件描述符
  - 当`EMFILE`时，要考虑的是如何优雅的关闭与客户端的连接：
    - 先关闭这个空闲文件，使系统能够分配一个新的文件描述符，然后`再accept`拿到socket连接的文件描述符
    - 立即close该连接的描述符，优雅的与客户端断开连接
    - 重新打开空间文件，以备再次出现这种情况
  
  ```c++
  #include <unistd.h>
  #include <sys/types.h>
  #include <fcntl.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <signal.h>
  #include <sys/wait.h>
  #include <poll.h>
  
  #include <stdlib.h>
  #include <stdio.h>
  #include <errno.h>
  #include <string.h>
  
  #include <vector>
  #include <iostream>
  
  #define ERR_EXIT(m) \
          do \
          { \
                  perror(m); \
                  exit(EXIT_FAILURE); \
          } while(0)
  
  typedef std::vector<struct pollfd> PollFdList;
  
  int main(void)
  {
  	signal(SIGPIPE, SIG_IGN);
  	signal(SIGCHLD, SIG_IGN);
  
  	int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
  	int listenfd;
  
  	//if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  	if ((listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)) < 0)
  		ERR_EXIT("socket");
  
  	struct sockaddr_in servaddr;
  	memset(&servaddr, 0, sizeof(servaddr));
  	servaddr.sin_family = AF_INET;
  	servaddr.sin_port = htons(5188);
  	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  	int on = 1;
  	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  		ERR_EXIT("setsockopt");
  
  	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  		ERR_EXIT("bind");
  	if (listen(listenfd, SOMAXCONN) < 0)
  		ERR_EXIT("listen");
  
  	struct pollfd pfd;
  	pfd.fd = listenfd;
  	pfd.events = POLLIN;
  
  	PollFdList pollfds;
  	pollfds.push_back(pfd);
  
  	int nready;
  
  	struct sockaddr_in peeraddr;
  	socklen_t peerlen;
  	int connfd;
  
  	while (1)
  	{
  		nready = poll(&*pollfds.begin(), pollfds.size(), -1);
  		if (nready == -1)
  		{
  			if (errno == EINTR)
  				continue;
  			
  			ERR_EXIT("poll");
  		}
  		if (nready == 0)	// nothing happended
  			continue;
  		
  		if (pollfds[0].revents & POLLIN)
  		{
  			peerlen = sizeof(peeraddr);
  			connfd = accept4(listenfd, (struct sockaddr*)&peeraddr,
  						&peerlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
  
  			if (connfd == -1)
  			{
  				if (errno == EMFILE)
  				{
  					close(idlefd);
  					idlefd = accept(listenfd, NULL, NULL);
  					close(idlefd);
  					idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
  					continue;
  				}
  				else
  					ERR_EXIT("accept4");
  			}
  
  			pfd.fd = connfd;
  			pfd.events = POLLIN;
  			pfd.revents = 0;
  			pollfds.push_back(pfd);
  			--nready;
  
  			std::cout<<"ip="<<inet_ntoa(peeraddr.sin_addr)<<
  				" port="<<ntohs(peeraddr.sin_port)<<std::endl;
  			if (nready == 0)
  				continue;
  		}
  
  		//std::cout<<pollfds.size()<<std::endl;
  		//std::cout<<nready<<std::endl;
  		for (PollFdList::iterator it=pollfds.begin()+1;
  			it != pollfds.end() && nready >0; ++it)
  		{
  				if (it->revents & POLLIN)
  				{
  					--nready;
  					connfd = it->fd;
  					char buf[1024] = {0};
  					int ret = read(connfd, buf, 1024);
  					if (ret == -1)
  						ERR_EXIT("read");
  					if (ret == 0)
  					{
  						std::cout<<"client close"<<std::endl;
  						it = pollfds.erase(it);
  						--it;
  
  						close(connfd);
  						continue;
  					}
  
  					std::cout<<buf;
  					write(connfd, buf, strlen(buf));
  					
  				}
  		}
  	}
  
  	return 0;
  }
  ```
  
  ​		这里添加了一个空的文件描述符：
  
  ```cpp
  int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
  ```
  
  ​		然后如果accept出错出现EMFILE错误，则有：
  
  ```cpp
  if (connfd == -1)
  {
      if (errno == EMFILE)
      {
          close(idlefd);
          idlefd = accept(listenfd, NULL, NULL);
          close(idlefd);
          idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
          continue;
      }
      else
          ERR_EXIT("accept4");
  }
  ```
  
  

### connect

![image-20210905031020483](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210905031020483.png)

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// sockfd， 用于连接通信的套接字
// addr，   要连接的对等方的套接字的通用地址
// addrlen， 要连接的对等方的套接字地址长度

// 成功返回0， 失败返回-1
```



### close

​		关闭套接字。

```c
#include <unistd.h>

int close(int fd);

// fd: 文件描述符
// close() returns zero on success.  On error, -1 is returned, and errno is set appropriately.
```

​		close会终止数据传送的两个方向，保证不发送的同时，也不再保证正确接收。

> ​		因为 TCP 是双向的，这里说的方向，指的是数据流的写入 - 读出的方向。
>
> ​		在绝大数情况下，TCP 连接都是先关闭一个方向，此时另外一个方向还是可以正常进行数据传输。

> ​		实际上是会主动向对端发送`FIN`段。如果这时对端仍然发送数据，接收会响应一个`RST`段，表示连接重置，对端收到后仍再次发送，就会导致对端出现`SIGPIPE`信号。

​		套接字可以被多个进程共享，比如fork的子进程会共享父进程的套接字。多个进程通过引用计数的方式来共享该套接字。close 会将当前进程引用的套接字计数减去，直到所有的进程都close该套接字，最后一个调用的close才会真正的关闭该套接字。

​		关闭套接字时：

- 输入方向，内核会将该套接字设置为不可读，任何读操作都会返回异常。
- 输出方向，内核会尝试将发送缓冲区剩余的数据发送给对端，并最后向对端发送一个FIN报文，接下来对该套接字的写操作会返回异常。

​		此时，如果对端没有检测到套接字关闭，还继续发送数据，就会收到一个RST报文，通知对端本侧已关闭。



#### shutdown

​		有选择的立即终止某个单向的数据发送或接收，也可以同时终止两个方向。

> 可以在发送全部完成后通过`shutdown`关闭发送，只等待对端的响应。
>
> shutdown会立即关闭指定套接字上的指定方向。

```c
#include <sys/socket.h>

/*
	The  shutdown()  call  causes all or part of a full-duplex connection on the socket associated with sockfd to be shut down.  
*/
int shutdown(int sockfd, int how);

// sockfd: socket 文件描述符
// how: 
//		SHUT_RD， 0， 关闭接收
//		SHUT_WR， 1， 关闭发送
// 		SHUT_RDWR， 2， 关闭接收和发送

// On success, zero is returned.  On error, -1 is returned, 
//	and errno is set appropriately
```

> **how：**
>
> - SHUT_RD
>
>   ​	关闭连接的读方向，对该套接字进行读操作直接返回`EOF`。
>
>   ​	套接字上接收缓冲区已有的数据将被丢弃，如果再有新的数据流到达，会对数据进行ACK，然后悄悄丢弃。
>
> - SHUT_WR
>
>   ​	关闭连接的写方向，就是“半关闭”的连接。
>
>   ​	不管套接字引用计数的值是多少，都会直接关闭连接的写方向。套接字上发送缓冲区已有的数据将被立即发送出去，并发送一个FIN报文给对端。应用程序如果对该套接字进行写操作会报错。
>
> - SHUT_RDWR
>
>   ​	上述两个操作各做一次，关闭套接字的读和写两个方向。
>
>   ​	与close不同的是，
>
>   - shutdown忽略引用计数，直接关闭套接字
>
>   - close关闭套接字的同事，会释放套接字对应的资源，但shutdown不会释放掉套接字对应的资源
>
>   - close的引用计数导致不一定会发送FIN，shutdown总是会发送FIN结束报文。
>
>     > 这在我们打算关闭连接通知对端的时候，是非常重要的。

> ​		`shutdown` how = 1时保证对方收到`FIN`分节，即收到一个`EOF`字符，而不管其他进程是否已经打开了套接字。
>
> ​		`close`不能保证这点，直到套接字引用计数减为0时才会发送，也就是所有进程都`close`了该文件描述符才会关闭。

```c
/// server

#include "../common/basedef.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 4096

static int count;
static void sig_int( int signo )
{
    printf( "\nreceived %d datagrams\n", count );
    exit( 0 );
}

int main( void )
{
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );

    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_port = htons( 5288 ),
        .sin_addr.s_addr = htonl( INADDR_ANY )
    };
    int ret = bind( listenfd, (struct sockaddr *) &servaddr, sizeof( servaddr ) );
    if ( ret < 0 )
        ERR_EXIT( "bind failed" );

    ret = listen( listenfd, 128 );
    if ( ret < 0 )
        ERR_EXIT( "listen failed" );

    signal( SIGINT, sig_int );
    signal( SIGPIPE, SIG_IGN );

    int connfd;

    if ( ( connfd = accept( listenfd, NULL, NULL ) ) < 0 )
        ERR_EXIT( "accept failed" );

    char message[ MAXLINE ];
    count = 0;

    for ( ;;) {
        int n = read( connfd, message, MAXLINE );
        if ( n < 0 )
            ERR_EXIT( "error read" );
        else if ( n == 0 )
            ERR_EXIT( "client closed" );

        message[ n ] = 0;
        printf( ( "receive %d bytes: %s\n", n, message ) );
        count++;

        char send_line[ MAXLINE ];
        sprintf( send_line, "Hi, %s", message );

        sleep( 5 );

        int write_nc = send( connfd, send_line, strlen( send_line ), 0 );
        printf( "send bytes: %zu \n", write_nc );
        if ( write_nc < 0 )
            ERR_EXIT( "error write" );
    }
}



/// client

#include "../common/basedef.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

int main( int argc, char *argv [] )
{
    if ( argc not_eq 2 )
        ERR_EXIT( "Usage: graceclient <IPaddress>" );

    int socket_fd = socket( PF_INET, SOCK_STREAM, 0 );
    
    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_port = htons( 5288 ),
        .sin_addr.s_addr = inet_addr( argv[ 1 ] )
    };
    int ret = connect( socket_fd, (struct sockaddr *) &servaddr, sizeof( servaddr ) );
    if ( ret < 0 )
        ERR_EXIT( "connect failed" );

    char send_line[ MAXLINE ], recv_line[ MAXLINE + 1 ];
    int n;

    fd_set readmask;
    fd_set allreads;

    FD_ZERO( &allreads );
    FD_SET( 0, &allreads );
    FD_SET( socket_fd, &allreads );

    for ( ;;) {
        readmask = allreads;
        ret = select( socket_fd + 1, &readmask, NULL, NULL, NULL );
        if ( ret <= 0 )
            ERR_EXIT( "select failed" );

        if ( FD_ISSET( socket_fd, &readmask ) ) {
            n = read( socket_fd, recv_line, MAXLINE );
            if ( n < 0 )
                ERR_EXIT( "read error" );
            else if ( n == 0 )
                ERR_EXIT( "server terminated" );

            recv_line[ n ] = '\0';
            fputs( recv_line, stdout );
            fputs( "\n", stdout );
        }
        if ( FD_ISSET( 0, &readmask ) ) {
            if ( fgets( send_line, MAXLINE, stdin ) not_eq NULL ) {
                if ( strncmp( send_line, "shutdown", 8 ) == 0 ) {
                    FD_CLR( 0, &allreads );
                    if ( shutdown( socket_fd, 1 ) ) 
                        ERR_EXIT( "shutdown failed" );
                } else if ( strncmp( send_line, "close", 5 ) == 0 ) {
                    FD_CLR( 0, &allreads );
                    if ( close( socket_fd ) )
                        ERR_EXIT( "close failed" );
                    sleep( 6 );
                    exit( 0 );
                } else {
                    int i = strlen( send_line );
                    if ( send_line[ i - 1 ] == '\n' )
                        send_line[ i - 1 ] = 0;

                    printf( "now sending %s\n", send_line );
                    size_t ret = write( socket_fd, send_line, strlen( send_line ) );
                    if ( ret < 0 )
                        ERR_EXIT( "write failed" );
                }
                printf( "send bytes: %zu \n", ret );
            }
        }
    }
}
```

>   ​		客户端连续输入1,2，close时，因服务端模拟处理，未能及时返回，而客户端已经关闭，服务端向客户端发送数据时产生SIGPIPE信号，客户端未收到服务端数据。
>
>   >   ​		客户端调用close并发送了FIN包，服务端接收到FIN包会将连接状态设置为对端已关闭写端，服务端在向客户端回送FIN包之前可以向对端发送数据也可不发送，实例程序就是发送了数据。
>   >
>   >   ​		服务端在对端关闭后，第一次发送数据对端会返回个RST包（因为客户端close已经关闭了读端），服务端调用read会返回-1，设置errno，此时服务端了解到客户端也不会再读数据了，如果服务端还是强行再次向客户端发送数据包，调用write后，协议栈会发现用户进程向一个没有读用户的管道做写操作，那么会触发SIGPIPE信号执行默认操作关闭用户进程。
>
>   ​		客户端连续输入1,2，shutdown时，因服务端模拟处理，未能及时返回，shutdown仅关闭写端保留读端，服务端读到EOF立即向客户端发送FIN，客户端在read中感知EOF，进行了正常退出。



## sockopt

```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname,
                      void *optval, socklen_t *optlen);
/* Set socket FD's option OPTNAME at protocol level LEVEL
   to *OPTVAL (which is OPTLEN bytes long).
   Returns 0 on success, -1 for errors.  */
int setsockopt(int sockfd, int level, int optname
               , const void *optval, socklen_t optlen);

// sockfd:   套接字
// level:   指定控制套接字的层次.可以取三种值:
//    1) SOL_SOCKET:通用套接字选项.
//    2) IPPROTO_IP:IPv4选项.
//    3) IPPROTO_IP6:IPv6选项.
//    4) IPPROTO_TCP:TCP选项.　
// optname:  控制的方式(选项的名称)
// optval:	是一个指向变量的指针, 具体的转型依赖optname传入的选项 
//	  类型：整形，套接口结构， 其他结构类型:linger{}, timeval{ }
// optlen:  set时为传入参数， get时为传出参数， optval的大小
```

> **level 及 optval**
>
> > https://blog.csdn.net/chary8088/article/details/2486377
> >
> > https://www.cnblogs.com/baiduboy/p/8127913.html
> >
> > [setsockopt函数解析 _四月的专栏-CSDN博客](https://blog.csdn.net/yufangbo/article/details/4663639)
