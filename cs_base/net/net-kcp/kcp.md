# kcp

---

## 参考

[GitHub - skywind3000/kcp: :zap: KCP - A Fast and Reliable ARQ Protocol](https://github.com/skywind3000/kcp)

[KCP协议：从TCP到UDP家族QUIC/KCP/ENET - zhoulujun - 博客园 (cnblogs.com)](https://www.cnblogs.com/zhoulujun/p/16063835.html)

[从入门到入魂！Linux中的kcp协议解析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/551210324)

[在网络中狂奔：KCP协议 (zhihu.com)](https://www.zhihu.com/tardis/zm/art/112442341?source_id=1005)

[(51 封私信 / 85 条消息) 既然 udp 更快，为什么这么多年主流一直用 tcp ? - 知乎 (zhihu.com)](https://www.zhihu.com/question/558479268/answer/2742475217?utm_id=0)



## 。。。

### 1、KCP 协议认识

（1）KCP 是一个开源的快速可靠传输协议，能以**比 TCP 多消耗 10%-20% 带宽**的代价，换取**平均延迟降低 30%-40%**的传输速度。项目地址：[https://github.com/skywind3000/kcp](https://link.zhihu.com/?target=https%3A//github.com/skywind3000/kcp)。

KCP 是一层纯算法实现，并不负责底层协议（如 UDP）的收发，需要使用者自己定义下层数据包的发送方式，并以 callback 的方式提供给 KCP。包括时钟也需要外部传递进来，内部不会有任何一次系统调用。KCP 整个协议的实现只有 ikcp.h 和 ikcp.c 两个源文件，可以方便的集成到用户自己的协议栈中。

（2）协议传输速度、超时计算、丢包重传分析

TCP 是为流量设计的（每秒内可以传输的数据量），信道流速很慢，但每秒流量很大，追求的是**充分利用带宽**；CP 超时计算是 **RTOx2**；TCP 丢包时会对**丢的那个包后的数据全部重传**。

KCP 是为流速设计的，**浪费部分带宽来提高传输速度**，它有正常模式和快速模式两种，正常模式同 TCP 一样使用公平退让法则，即发送窗口大小由：发送端发送缓存大小、接收端剩余接收缓存大小、丢包退让、慢启动这四要素决定；快速模式下通过多种策略跳过后两步，仅用前两项来控制发送频率，达到提高流速的结果；KCP 启动快速模式后超时计算只是 **RTOx1.5**（实验证明 1.5 这个值相对比较好），提高了传输速度；KCP 丢包时是选择性快速重传，**只重传真正丢失的数据包**。

（3）自动重传请求

**自动重传请求 ARQ**（Automatic Repeat-reQuest）模型响应有两种：**UNA（表示此编号之前的所有包已收到）**和 **ACK（表示此编号包已收到）**。只用 UNA 会导致丢包后只能全部重传，只用 ACK 则丢包后维护成本太高（某个中间包丢掉后，可能需要维护比较多的后续包的缓存），以往协议都是二选其一，而**KCP 协议中，除了单独的 ACK 包外，其他所有包都有 UNA 信息**。

### 2、KCP 基本使用

（1）KCP 的 input/output 方法用来对接下层的 UDP 收发模块。而ikcp_send、ikcp_recv提供给上层逻辑调用实现协议的收发。

KCP 的数据格式如下图所示：

（2）KCP 的发送和接收单元是 segment，即应用层的数据可能会拆分成多个 segment 发送。

conv：会话编号。

cmd：segment 类型，IKCP_CMD_ACK、IKCP_CMD_PUSH、IKCP_CMD_WASK、IKCP_CMD_WINS。

frg：是否最后一个 segment。0：数据包的最后一个 segment；1：数据包的中间切片 segment。

sn：下一个待发 segment 的序号。

una：待接收消息序号，表示这之前的所有 segment 都收到了。

len：segment 数据长度，不包含头。

（3）发送端

在发送端应用层通过 kcp_send 发送数据，KCP 会把用户数据拆分 KCP 数据包，通过 kcp_output 再以 UDP 包的方式发送出去。

创建 KCP 对象：

```cpp
// 初始化 kcp 对象，conv 为一个表示会话编号的整数。
// 和 tcp 的 conv 一样，通信双方需保证 conv 相同，相互的数据包才能够被认可，user 是一个给回调函数的指针。
ikcpcb* kcp = ikcp_create(conv, user);
```

设置回调函数：

```cpp
// KCP 下层协议的输出函数，KCP 需要发送数据时会调用它。
// buf/len 表示缓存和长度。
// user 指针为 kcp 对象创建时传入的值，用于区别多个 KCP 对象。
int udp_output(const char* buf, int len, ikcpcb* kcp, void* user) {
    ....
}
// 设置回调函数。ikcp_setoutput(kcp, udp_output);
```

发送数据：

```cpp
int ikcp_send(ikcpcb* kcp, const char* buffer, int len);
```

循环调用 update：

```cpp
// 以一定频率调用 ikcp_update 来更新 kcp 状态，并且传入当前时钟（单位毫秒）。
// 如 10ms 调用一次，或用 ikcp_check 确定下次调用 update 的时间不必每次调用。
ikcp_update(kcp, millisec);
```

（4）接收端

在接收端，UDP 收到的包，不断通过 kcp_input 喂给 KCP，KCP 会对这部分数据（KCP 协议数据）进行解包，重新封装成应用层用户数据，应用层通过 kcp_recv 获取。

创建 KCP 对象：

```cpp
// 初始化 kcp 对象，conv 为一个表示会话编号的整数。
// 和 tcp 的 conv 一样，通信双方需保证 conv 相同，相互的数据包才能够被认可，user 是一个给回调函数的指针。
ikcpcb* kcp = ikcp_create(conv, user)
```

读取一个下层协议的数据包：

```cpp
// 收到一个下层协议的数据包（比如 UDP 包）时需要调用：
ikcp_input(kcp, received_udp_packet, received_udp_size);
```

将 KCP 数据还原成发送端发送的 buffer 数据给应用层：

```cpp
int ikcp_recv(ikcpcb *kcp, char *buffer, int len);
```

（5）协议配置

协议默认模式是一个标准的 ARQ，需要通过配置打开各项加速开关。

工作模式：

```cpp
int ikcp_nodelay(ikcpcb* kcp, int nodelay, int interval, int resend, int nc)
// 普通模式：ikcp_nodelay(kcp, 0, 40, 0, 0);
// 极速模式：ikcp_nodelay(kcp, 1, 10, 2, 1);
```

nodelay：是否启用 nodelay 模式。0 不启用，1 启用。

interval：协议内部工作的 interval，单位毫秒，比如 10ms 或者 20ms。

resend：快速重传模式，默认 0 关闭，可以设置 2（2 次 ACK 跨越将会直接重传）。

nc：是否关闭流控，默认是 0 代表不关闭，1 代表关闭。

最大窗口：

```cpp
int ikcp_wndsize(ikcpcb* kcp, int sndwnd, int rcvwnd);
```

最大传输单元：

纯算法协议并不负责探测 MTU，默认 MTU 是 1400 字节，可以使用`ikcp_setmtu`来设置该值。该值将会影响数据包归并及分片时候的最大传输单元。

最小 RTO：

不管是 TCP 还是 KCP 计算 RTO 时都有最小 RTO 的限制，即便计算出来 RTO 为 40ms，由于默认的 RTO 是 100ms，协议只有在 100ms 后才能检测到丢包，快速模式下为 30ms，可以手动更改该值：

```cpp
kcp->rx_minrto = 10;
```