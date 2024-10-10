

# TCP/IP 协议栈

---

![image-20230420211907967](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230420211907967.png)

​		TCP/IP协议族是一个四层协议系统，自底向上提供不同的服务。

![image.png](https://p3-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/3cdf7ead5559496caf6c61d98438494a~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAgSGFuYm94:q75.awebp?rk3s=f64ab15b&x-expires=1728457249&x-signature=qZwnuyPVW8n%2BZTEya0qM1mOSJVI%3D)

## TCP协议栈传输过程

![image-20230420211517215](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230420211517215.png)

1.   DNS域名解析：

     >   ​		应用程序Ping会判断发送的是主机名还是IP地址，调用函数gethostbyname()解析主机B，将主机名转换成一个32位的IP地址。

2.   ping程序向目的IP地址发送一个ICMP的ECHO包

3.   将目标主机的IP地址转换为48位硬件地址，在局域网内发送ARP请求广播，查找主机B的硬件地址。

4.   主机B的ARP协议层接受到主机A的ARP请求后，将本机的硬件地址填充到应答包，发送ARP应答到主机A

5.   发送ICMP数据包到主机B

6.   主机B接收到主机A的ICMP包，发送响应包

7.   主机A接受到主机B的ICMP响应包

## TCP/IP协议栈工作原理

​		发送端应用程序使用`send/write`函数向一个TCP连接写入数据时

- 内核TCP模块将数据首先复制到对应的内核发送缓冲区
- TCP模块调用IP模块提供的服务，传递TCP头部信息和TCP发送缓冲区中的数据
- UDP与之类似，不过UDP在发送完成后，即刻将内核缓冲区中的数据丢弃，因而不可靠。
  - 应用程序可以进行检测，如需重发需要将数据重新复制到内核发送缓冲区

![image-20230422204550222](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230422204550222.png)



## 链路层协议

​		链路层负责网络数据交换时物理连接的标准。链路层是物理连接领域标准化的结果，也是最基本的的领域。定义了`LAN、WAN、MAN`等网络标准。

​		数据链路层实现了网卡接口的网络驱动程序，处理数据在物理介质（以太网、令牌环等）的传输。不同的物理网络具有不同的电气特性，网络驱动程序隐藏了这些细节。

​		常用协议ARP协议和RARP协议，他们实现了IP地址和机器物理地址（MAC）之间的相互转换

- ARP（address resolve protocol）

  ​	网络层使用IP地址寻址一台机器，而数据链路层使用物理地址寻址一台机器。网络层必须先将目标机器的IP地址转换成物理地址才能使用数据链路层提供的服务。

- RARP（reverse address resolve protocol）

  ​	仅用于网络上的某些无盘工作站，因为缺乏存储设备，无盘工作站无法记住自己的IP地址。它们通过网卡上的物理地址来向网络管理者查询自身的IP地址，运行RARP服务的网络管理者通常存有该网络上所有机器的物理地址到IP地址的映射。

​		

### ARP

​		地址解析协议，将硬件MAC地址转换成逻辑IP地址。转换成功的映射会放置在本地ARP高速缓存中，下一次的地址解析会先查询高速缓存，不存在才进行新的ARP协议交互。

![image-20230420211641461](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230420211641461.png)

>   ![image-20210902160728483](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902160728.png)
>
>   [(100条消息) ARP协议分析_小白编程-CSDN博客](https://blog.csdn.net/iteye_13254/article/details/82297446?utm_medium=distribute.pc_relevant_t0.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-1.control&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-1.control)
>
>   [图解ARP协议（一） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/28771785)
>
>   [ARP协议的工作机制详解 (biancheng.net)](http://c.biancheng.net/view/6388.html)

### RARP

​		反地址解析协议，将逻辑IP地址转换成硬件MAC地址。



### 最大传输单元MTU

​		Maximum Transmission Unit，以太网和IEEE 802.3对数据帧的长度限制分别为1500和1492字节。

​		如果IP层一个数据报的大小大于MTU，那么需要在IP层进行分片（fragmentation）。

​		网络上两台主机交互时，两台主机之间要经过多个网络，每个网络的链路层可能又不同的MTU，其中两台通信主机路径上的最小MTU被称为路径MTU。

> 以太网最小46字节，不足就会补0填充。

### 以太网帧格式

​		![img](https://bkimg.cdn.bcebos.com/pic/b21bb051f8198618f1ac9f8b4aed2e738ad4e6d2?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U5Mg==,g_7,xp_5,yp_5/format,f_auto)

>   [以太网数据帧格式（结构）图解 (biancheng.net)](http://c.biancheng.net/view/6391.html)
>
>   ![image-20210902111629446](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902111629.png)
>
>   ![image-20210902111503744](C:/Users/52648/AppData/Roaming/Typora/typora-user-images/image-20210902111503744.png)

用于以太网的ARP请求或应答分组格式

![img](https://img-blog.csdnimg.cn/20200605172649351.png)

>   [(99条消息) 以太网协议和ARP协议_这是书生呀-CSDN博客](https://blog.csdn.net/weixin_43519514/article/details/106574533)
>
>   [4.4 ARP的分组格式 · TCP/IP协议详解卷一 · 看云 (kancloud.cn)](https://www.kancloud.cn/lifei6671/tcp-ip/139998)





## 网络层协议

​		网络层实现数据包的选路和转发。

​		WAN（Wide Area Network）使用众多分级的路由器来连接分散的主机或LAN设备（Local Area Network，局域网），因此通信的两台主机一般是通过一到多个中间结点（路由器）连接的。

- 网络层选择这些中间结点，确定两台主机之间的通信路径。
- 网络层向上层协议隐藏网络拓扑结构，使得在传输层和网络应用程序看来，通信的双方是直接相连的。

​		IP协议本身是面向消息的、不可靠的协议。每次传输数据时会帮我们选择路径，但并不一致。如果传输中发生路径错误，则选择其他路径。但如果发生数据丢失或错误，则无法解决。

### IP

​		IP（Internet Protocol）根据数据包的目的IP地址来决定如何传递它。

- 如果不能直接发送给目标主机，IP协议就为它寻找一个合适的下一跳路由器（next hop）并将数据包传递给该路由器转发
- 多次重复这一过程，数据包最终到达目标主机
- 或由于发送失败而被丢弃

​		IP协议使用逐级发送的方式确定通信路径。

###	ICMP

​		ICMP（Internet Control Message Protocol），网际控制协议，它是IP协议的重要补充，用于传递差错信息、时间、回显、网络信息等控制数据，检测网络连接。

![image-20230420213715522](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230420213715522.png)

- 8bit 类型

  ​	用于区分报文类型，它将ICMP报文分为两大类：

  - 差错报文：用来回应网络错误，如目标不可达、重定向
  - 查询报文：查询网络信息

- 8bit 代码

  ​	进一步细分不同的条件，如重定向报文可以分为网络重定向，主机重定向

- 16bit 校验和

  ​	使用CRC（Cyclic Redundancy Check）循环冗余校验对包括头部和内容的整个报文进行检验，确认传输过程中报文是否有损坏。

​		严格意义说`ICMP`不是网络层协议，因为它使用处于同一层的`IP`协议提供服务。

> ICMP实际上基于IP协议，在IP头部后加上：
>
> -   类型： ICMP类型，ping请求为8，应答为0
> -   代码：进一步划分ICMP的类型，用来查找产生错误的原因
> -   校验和：用于检查错误的数据
> -   标识符：通过标识符确认发送方，可以是进程ID
> -   序列号：唯一确定的一个报文，icmp_seq

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/806469-20180306123952687-1669696526.png)

>   [(99条消息) ICMP协议详解_honeyJ-CSDN博客_icmp协议](https://blog.csdn.net/baidu_37964071/article/details/80514340)
>
>   [[na\]完全理解icmp协议 - _毛台 - 博客园 (cnblogs.com)](https://www.cnblogs.com/iiiiher/p/8513748.html)



### IGMP



## 传输层协议

​		传输层以``IP``层提供的路径信息为基础，为两台主机上的应用程序提供端到端的对等通信，完成实际的数据传输。

​		传输层只关心通信的起始端和目的端，不在乎数据包的中转过程。

![image-20230420214635305](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230420214635305.png)

### TCP和UDP

​		TCP是一个面向连接有状态的协议，需要先与对方建立连接，然后才能发送数据，增加了重传、确认、有序传输、拥塞控制等能力，保证数据不丢失不重复。数据收发双方在一个确定的上下文中工作。

​		UDP无状态，没有确定的上下文，不用先建立可靠连接，可以任意发送数据，但不保证数据一定能够收发成功。没有重传和确认，没有有序控制，没有拥塞控制。

​		两者在数据传输形式上，TCP因为连接可靠，可以认为在连接存续期间能够不间断任意读写，故而以字节流的形式，有先后的进行数据传输。UDP因无连接，一次发送认为是一个单独的报文（类比收到一封信或电报），顺序发可能乱序收。

>   因为UDP通信较为简单，适合场景也比较多，如DNS服务，SNMP服务，这些场景对时延和丢包不是特别敏感。聊天室、多人游戏也都会用到UDP协议。

### TCP

​		TCP（transmission control protocol）传输控制协议，为应用层提供可靠有连接的流式传输协议。

- 可靠：TCP使用超时重传、数据确认等方式确保数据包被正确发送至目的端
- 有连接：TCP协议通信的双方必须先通过三次握手建立TCP连接，并在内核中为该连接维持一些必要的数据结构（连接状态、读写缓冲区、定时器等），通信结束时，双方必须关闭连接等待四次挥手，释放内核数据。
- 流式传输：数据包之间没有边界，持续从一端流入，被另一端读出，发送端可以以字节为单位向数据流中写入任意多的数据，接收端也以字节为单位从数据流中读出任意多的数据。

![image-20230420215337377](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230420215337377.png)

​		特点

- 基于字节流	

    对于segment数据段的边界不做任何标定。一次发送或接收可能处理 一个以上或以下的包，这就可能造成粘包问题，需要在应用层处理。

- 面向连接

- 可靠传输

- 缓冲传输

- 全双工

    发送和接收可以同时进行

- 流量控制

-   

### UDP

​		UDP（User Datagram Protocol）用户数据报协议，为应用层提供不可靠无连接的报式传输协议。

- 不可靠：UDP协议无法保证数据从发送端正确发送到目标端，数据在中途丢失或者目标端数据校验错误丢弃，UDP协议简单地通知应用程序发送失败。使用UDP协议要通过应用程序处理数据确认、超时重传等逻辑。
- 无连接：不保持持久的连接，应用程序每次发送数据都要明确指定接收端的地址（IP地址等）
- 报式传输：相对于数据流而言，每个UDP数据包都有一个长度，接收端必须以该长度为最小单位将其所有内容一次性读出，否则数据将被截断。

​		一般情况下UDP更加高效。

> **报文格式**
>
> ![image-20210903001405121](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210903001405121.png)

### 本地域套接字协议

​		TCP/UDP socket在本地地址 127.0.0.1上也要经过内核协议栈。而本地套接字，是一种单主机进程间通信的手段，减少了协议栈实现的复杂度，效率比TCP/UDP高。

​		类似的IPC（进程间通信）机制还有UNIX管道、共享内存、RPC等。

​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/20211003161400.png)



### SCTP

​		SCTP（stream control transmission protocol）流控制传输协议，是一种较新的传输层协议，为了在网络上传输电话信号而设计的。



## 应用层

​		应用层负责处理实际的工作逻辑。链路层、网络层、传输层负责处理网络通信细节，它们必须既稳定又高效，因此它们都在内核空间中实现。应用层处理众多不同的工作，需要用户编码实现，在用户空间中工作。

​		有些通用且常用的网络应用程序是操作系统需要的，它们在内核中实现并工作。

​		内核空间到用户空间的切换，除了中断现场的保护等，主要瓶颈还在于数据的从内核空间复制到用户空间。

- `ping`，是一个应用程序，它不是一个协议，使用ICMP报文检测网络连接
- `telnet`协议是一种远程登录协议
- `OSPF`，open shortest path first开放最短路径优先协议是一种动态路由更新协议，用于路由器之间的通信，告知对方各自的路由信息
- `DNS`，domain name service 域名服务协议，提供机器域名到IP地址的转换

​		应用层协议可能跳过传输层直接使用网络层提供的服务，比如ping程序和OSPF协议。应用层协议通常既可以使用tcp也可以使用udp如DNS协议。`/etc/services`中包括一些指明的应用层协议以及它们都能使用哪些传输层服务。
