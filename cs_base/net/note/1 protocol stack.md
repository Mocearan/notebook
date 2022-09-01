# net-protocol

---

## TCP/IP 协议栈

![image-20210901224713822](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210901224713822.png)



### TCP协议栈传输过程

![image-20210902161022512](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902161022.png)

1.   DNS域名解析：

     >   ​		应用程序Ping会判断发送的是主机名还是IP地址，调用函数gethostbyname()解析主机B，将主机名转换成一个32位的IP地址。

2.   ping程序向目的IP地址发送一个ICMP的ECHO包

3.   将目标主机的IP地址转换为48位硬件地址，在局域网内发送ARP请求广播，查找主机B的硬件地址。

4.   主机B的ARP协议层接受到主机A的ARP请求后，将本机的硬件地址填充到应答包，发送ARP应答到主机A

5.   发送ICMP数据包到主机B

6.   主机B接收到主机A的ICMP包，发送响应包

7.   主机A接受到主机B的ICMP响应包

### TCP/IP协议栈工作原理

​		在每一层，由对应的协议进行封装和解封装，得到对应于该层的数据后，传递至下一层。

### 链路层协议

链路层是物理连接领域标准化的结果，也是最基本的的领域。定义了LAN、WAN、MAN等网络标准。

链路层负责网络数据交换时物理连接的标准。

#### ARP

​		地址解析协议，将硬件MAC地址转换成逻辑IP地址。转换成功的映射会放置在本地ARP高速缓存中，下一次的地址解析会先查询高速缓存，不存在才进行新的ARP协议交互。

![image-20210902154849068](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902154849.png)

>   ![image-20210902160728483](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902160728.png)
>
>   [(100条消息) ARP协议分析_小白编程-CSDN博客](https://blog.csdn.net/iteye_13254/article/details/82297446?utm_medium=distribute.pc_relevant_t0.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-1.control&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-1.control)
>
>   [图解ARP协议（一） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/28771785)
>
>   [ARP协议的工作机制详解 (biancheng.net)](http://c.biancheng.net/view/6388.html)

#### RARP

​		反地址解析协议，将逻辑IP地址转换成硬件MAC地址。



#### 最大传输单元MTU

​		Maximum Transmission Unit，以太网和IEEE 802.3对数据帧的长度限制分别为1500和1492字节。

​		如果IP层一个数据报的大小大于MTU，那么需要在IP层进行分片（fragmentation）。

​		网络上两台主机交互时，两台主机之间要经过多个网络，每个网络的链路层可能又不同的MTU，其中两台通信主机路径上的最小MTU被称为路径MTU。

> 以太网最小46字节，不足就会补0填充。

#### 以太网帧格式

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





### 网络层协议

IP协议本身是面向消息的、不可靠的协议。每次传输数据时会帮我们选择路径，但并不一致。如果传输中发生路径错误，则选择其他路径。但如果发生数据丢失或错误，则无法解决。

####	ICMP

​		网际控制协议，用于传递差错信息、时间、回显、网络信息等控制数据。

![image-20210902153330781](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902153330.png)

![image-20210902153358318](https://gitee.com/masstsing/picgo-picserver/raw/master/20210902153358.png)

>   [(99条消息) ICMP协议详解_honeyJ-CSDN博客_icmp协议](https://blog.csdn.net/baidu_37964071/article/details/80514340)
>
>   [[na\]完全理解icmp协议 - _毛台 - 博客园 (cnblogs.com)](https://www.cnblogs.com/iiiiher/p/8513748.html)
>
>   

>   ICMP实际上基于IP协议，在IP头部后加上：
>
>   -   类型： ICMP类型，ping请求为8，应答为0
>   -   代码：进一步划分ICMP的类型，用来查找产生错误的原因
>   -   校验和：用于检查错误的数据
>   -   标识符：通过标识符确认发送方，可以是进程ID
>   -   序列号：唯一确定的一个报文，icmp_seq
>
>   ![img](https://images2018.cnblogs.com/blog/806469/201803/806469-20180306123952687-1669696526.png)



#### IGMP



### 传输层协议

传输层以IP层提供的路径信息为基础，完成实际的数据传输。故改成又成为传输层。

#### TCP和UDP

​		TCP是一个面向连接有状态的协议，需要先与对方建立连接，然后才能发送数据，增加了重传、确认、有序传输、拥塞控制等能力，保证数据不丢失不重复。数据收发双方在一个确定的上下文中工作。

​		UDP无状态，没有确定的上下文，不用先建立可靠连接，可以任意发送数据，但不保证数据一定能够收发成功。没有重传和确认，没有有序控制，没有拥塞控制。

​		两者在数据传输形式上，TCP因为连接可靠，可以认为在连接存续期间能够不间断任意读写，故而以字节流的形式，有先后的进行数据传输。UDP因无连接，一次发送认为是一个单独的报文（类比收到一封信或电报），顺序发可能乱序收。

>   因为UDP通信较为简单，适合场景也比较多，如DNS服务，SNMP服务，这些场景对时延和丢包不是特别敏感。聊天室、多人游戏也都会用到UDP协议。

#### TCP

​		可靠有连接的流式传输协议。

![image-20210902232653570](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210902232653570.png)

##### 特点

- 基于字节流	

    对于segment数据段的边界不做任何标定。一次发送或接收可能处理 一个以上或以下的包，这就可能造成粘包问题，需要在应用层处理。

- 面向连接

- 可靠传输

- 缓冲传输

- 全双工

    发送和接收可以同时进行

- 流量控制

-   

#### UDP

​		不可靠无连接的报文传输协议，一般情况下UDP更加高效。

> **报文格式**
>
> ![image-20210903001405121](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20210903001405121.png)

#### 本地域套接字协议

​		TCP/UDP socket在本地地址 127.0.0.1上也要经过内核协议栈。而本地套接字，是一种单主机进程间通信的手段，减少了协议栈实现的复杂度，效率比TCP/UDP高。

​		类似的IPC（进程间通信）机制还有UNIX管道、共享内存、RPC等。

​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/20211003161400.png)
