# debug tool

---

​		网络编程中会遇到各种各样的问题，对这些问题的诊断和分析需要不断积累经验。linux平台下的各种网络工具，提供了诊断分析的有力工具。

## ping

​		ping基于ICMP。

```shell
ping url
```

>   ```shell
>   [root@iZbp13ffnpp8l2q0xvjmydZ WeTalkServer2]# ping www.sina.com.cn
>   PING ww1.sinaimg.cn.w.alikunlun.com (101.226.28.204) 56(84) bytes of data.
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=1 ttl=55 time=8.41 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=2 ttl=55 time=8.36 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=3 ttl=55 time=8.37 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=4 ttl=55 time=8.40 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=5 ttl=55 time=8.42 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=6 ttl=55 time=8.38 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=7 ttl=55 time=8.39 ms
>   64 bytes from 101.226.28.204 (101.226.28.204): icmp_seq=8 ttl=55 time=8.39 ms
>   ^C
>   --- ww1.sinaimg.cn.w.alikunlun.com ping statistics ---
>   8 packets transmitted, 8 received, 0% packet loss, time 7015ms
>   rtt min/avg/max/mdev = 8.364/8.395/8.427/0.067 ms
>   ```
>
>   icmp_seq : 序列号，照此排序
>
>   TTL（time to live）：两个IP地址之间传输的时间
>
>   

## ifconfig / ipconfig

​		windows: ipconfig

​		linux : ifconfig

显式当前系统中所有网络设备，即网卡列表。

```shell

vagrant@ubuntu-xenial-01:~$ ifconfig
cni0      Link encap:Ethernet  HWaddr 0a:58:0a:f4:00:01
          inet addr:10.244.0.1  Bcast:0.0.0.0  Mask:255.255.255.0
          inet6 addr: fe80::401:b4ff:fe51:bcf9/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1450  Metric:1
          RX packets:2133 errors:0 dropped:0 overruns:0 frame:0
          TX packets:2216 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:139381 (139.3 KB)  TX bytes:853302 (853.3 KB)
```

>   ```
>   Link encap:Ethernet  HWaddr 0a:58:0a:f4:00:01  
>   	以太网设备，Mac地址：02:54:ad:ea:60:2e
>   ```
>
>   
>
>       inet addr:10.244.0.1  Bcast:0.0.0.0  Mask:255.255.255.0
>       inet6 addr: fe80::401:b4ff:fe51:bcf9/64 Scope:Link
>       	网卡的IPv4和IPv6地址，IPv4还显示了子网掩码及广播地址
>
>   >   ​		在每个 IPv4 子网中，有一个特殊地址被保留作为子网广播地址，比如这里的 10.0.2.255 就是这个子网的广播地址。当向这个地址发送请求时，就会向以太网网络上的一组主机发送请求。
>
>   ```
>    UP BROADCAST RUNNING MULTICAST  MTU:1450  Metric:1
>        网卡的状态，MTU 是最大传输单元的意思，表示的是链路层包的大小。1500 表示的是字节大小。
>   ```
>
>   >   Metric 就是用来确定多块网卡的优先级的，数值越小，优先级越高，1 为最高级。
>   >
>   >   ​		Linux 在一台主机上可以有多个网卡设备，很可能有这么一种情况，多个网卡可以路由到目的地。一个简单的例子是在同时有无线网卡和有线网卡的情况.



## netstat

​		netstat用于查看网络状态（IP地址和端口使用情况，tcp连接状态）

### 用法

#### all socket

```shell
netstat -alepn
```

>   会显示IPv4/IPv6 TCP, UDP, UNIX DOMAIN都显示。
>
>   对于 tcp来说，可以直观的查看连接四元组。
>
>   ```shell
>   tcp  0 0 127.0.0.1:2379  127.0.0.1:52464    ESTABLISHED 0   27710       3496/etcd
>   ```
>
>   >   本地 127.0.0.1 的端口 52464 连上本地 127.0.0.1 的端口 2379，状态为 ESTABLISHED，本地进程为 etcd，进程为 3496。

#### UNIX domain

```shell
netstat Socket -x -alepn
```

>   ```shell
>   
>   unix  3   [ ]   STREAM   CONNECTED   23209    1400/dockerd     /var/run/docker.sock
>   ```
>
>   ​		docker 在本地套接字的监听路径。/var/run/docker.sock 是本地套接字监听地址，dockerd 是进程名称，1400 是进程号。



## lsof

​		查找指定的IP地址或者端口上打开套接字的进程。

​		lsof可以和netstat结合使用：

```shel
lsof /var/run/docker.sock
```

​		定位端口上的进程：

```shell
lsof -i :8080
```



## tcpdump

​		抓包工具。

​		tcpdump 在开启抓包的时候，会自动创建一个类型为 AF_PACKET 的网络套接口，并向系统内核注册。当网卡接收到一个网络报文之后，它会遍历系统中所有已经被注册的网络协议，包括其中已经注册了的 AF_PACKET 网络协议。系统内核接下来就会将网卡收到的报文发送给该协议的回调函数进行一次处理，回调函数可以把接收到的报文完完整整地复制一份，假装是自己接收到的报文，然后交给 tcpdump 程序，进行各种条件的过滤和判断，再对报文进行解析输出。

>   tcpdump还可以导出文件pcap，放到wireshark中进一步分析。
>
>   ```shell
>   tcpdump host 10.1.11.133 and udp port 5060 -v -w gw.cap
>   ```

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/20211013111158.jpeg)

>   ```shell
>   192.168.33.11.41388 > 192.168.33.11.6443
>   		源地址（192.168.33.11.41388）到目的地址（192.168.33.11.6443）
>   ```
>
>   ```shell
>   Flags []
>   	包的标志:
>   [S]：SYN，表示开始连接
>   [.]：没有标记，一般是确认
>   [P]：PSH，表示数据推送
>   [F]：FIN，表示结束连接
>   [R] ：RST，表示重启连接
>   ```
>
>   ```shell
>   seq: 包序号，就是 TCP 的确认分组
>   cksum：校验码
>   win：滑动窗口大小
>   length：荷载的数据（payload）长度 length，如果没有数据则为 0
>   ```
>
>   >   Foreign Address 显示的 ``*.*``
>   >
>   >   ​		这个套接字正在监听端口等待连接进来，允许任何地址、任何端口来建立连接。

### 参数

#### -i 指定网卡

```shell
tcpdump -i eth0
```

#### src host

```shell
tcpdump src host hostname
```

#### [m:n] 偏移



## iftop

​		可以查网络io大户
