# kcp

> https://github.com/skywind3000/kcp

---

​		**KCP 能以比 TCP 浪费 10%-20% 带宽的代价，换取平均延迟降低 30%-40%，最大延迟降低 3 倍的传输速度。**`KCP `是一层纯算法实现，并不负责底层协议（如 `UDP`）的收发，需要使用者自己定义下层数据包的发送方式，并以 callback 的方式提供给 `KCP`。包括时钟也需要外部传递进来，内部不会有任何一次系统调用。

​		`KCP `整个协议的实现只有 `ikcp.h` 和 `ikcp.c` 两个源文件，可以方便的集成到用户自己的协议栈中。比如你实现了一个 `P2P`，或者某个基于 `UDP `的协议，而缺乏一套完善的 `ARQ`（自动重传请求，`Automatic Repeat-reQuest`）实现，那么简单的拷贝这两个文件到现有项目中，稍加改造适配，即可使用。



## 参考

https://mp.weixin.qq.com/s?__biz=MjM5MTkxOTQyMQ==&mid=2257484828&idx=1&sn=384ce6a7128ac3ce3f2cec5416d61ce0&chksm=a5d4e04e92a369588ec7d3fd831c62e0f8f38c2b7414653492b6b541fe3ab42bc6c1ae745053&scene=178&cur_album_id=2140155659944787969&search_click_id=#rd

[GitHub - skywind3000/kcp: :zap: KCP - A Fast and Reliable ARQ Protocol](https://github.com/skywind3000/kcp)

[KCP协议：从TCP到UDP家族QUIC/KCP/ENET - zhoulujun - 博客园 (cnblogs.com)](https://www.cnblogs.com/zhoulujun/p/16063835.html)

[从入门到入魂！Linux中的kcp协议解析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/551210324)

[在网络中狂奔：KCP协议 (zhihu.com)](https://www.zhihu.com/tardis/zm/art/112442341?source_id=1005)

[(51 封私信 / 85 条消息) 既然 udp 更快，为什么这么多年主流一直用 tcp ? - 知乎 (zhihu.com)](https://www.zhihu.com/question/558479268/answer/2742475217?utm_id=0)



## 特性

​		`TCP `是为流量设计的（每秒内可以传输的数据量），追求的是充分利用带宽。

​		`KCP `是为流速设计的（单个数据包从一端发送到另一端需要的时间），浪费部分带宽来提高传输速度。

​		KCP 有正常模式和快速模式两种，快速模式下通过多种策略达到提高流速的结果。

> 正常模式同 TCP 一样使用公平退让法则，即发送窗口大小由：发送端发送缓存大小、接收端剩余接收缓存大小、丢包退让、慢启动这四要素决定；
>
> 快速模式下通过多种策略跳过后两步，仅用前两项来控制发送频率，达到提高流速的结果；KCP 启动快速模式后超时计算只是 **RTOx1.5**（实验证明 1.5 这个值相对比较好），提高了传输速度；KCP 丢包时是选择性快速重传，**只重传真正丢失的数据包**。



### `RTOx1.5`

​		RTO（Retransmission TimeOut），即重传超时时间。

​		TCP 超时计算是 `RTOx2`，这样连续丢三次包就变成 `RTOx8` 了，十分恐怖，而 **KCP 启动快速模式后不** **`x2`，只是 `x1.5`（实验证明 1.5 这个值相对比较好），提高了传输速度**。



### 选择性重传

​		`TCP `丢包时会对丢的那个包开始以后的数据全部重传，**`KCP `则是选择性重传，只重传真正丢失的数据包**。

​		`KCP `采用滑动窗口机制来提高发送速度。

- 由于 `UDP `是不可靠的传输方式，会存在丢包、包的乱序。而 `KCP `是可靠的且保证数据有序的协议。
- 为了保证包的顺序，接收方会维护一个接收窗口，接收窗口有
  - 起始序号 `rcv_nxt（待接收消息序号）`
  - 尾序号 `rcv_nxt + rcv_wnd（接收窗口大小）`
- 如果接收窗口收到序号为 `rcv_nxt` 的分片（刚好是接收端待接收的消息序号）
  - 那么 `rcv_nxt` 就加 1，也就是滑动窗口右移
  - 并把该数据放入接收队列供应用层取用。
- 如果收到的数据在窗口范围内但不是 `rcv_nxt`
  - 那么就把数据保存起来
  - 等收到 `rcv_nxt` 序号的分片时再一并放入接收队列供应用层取用。

​		当丢包发生的时候，假设第 `n` 个包丢失了，但是第` n+1`、`n+2` 个包都已经传输成功了，此时只重传第 `n` 个包，而不重传成功传输的` n+1`、`n+2 `号包，这就是选择重传。

​		为了能够做到选择重传，接收方需要告诉发送方哪些包它收到了。比如在返回的 `ACK `中包含 `rcv_nxt` 和 `sn`

- `rcv_nxt` 的含义是接收方已经成功按顺序接收了 `rcv_nxt` 序号之前的所有包
- 大于 `rcv_nxt` 的序号 `sn` 表示的是在接收窗口内的不连续的包

​		那么根据这两个参数就可以计算出哪些包没有收到了。

- 发送方接收到接收方发过来的数据时，首先解析 `rcv_nxt`，把所有小于 `rcv_nxt` 序号的包从发送缓存队列中移除。
- 然后再解析 `sn` （大于 `rcv_nxt`），遍历发送缓存队列，找到所有序号小于 `sn` 的包
- 根据我们设置的快速重传的阈值，对每个分片维护一个快速重传的计数
  - 每收到一个 `ACK `解析 `sn` 后找到了一个分片，就把该分片的快速重传的计数加 `1`
  - 如果该计数达到了快速重传阈值，那么就认为该分片已经丢失，可以触发快速重传
  - 该阈值在 `KCP `中可以设置。



### 快速重传

​		发送端发送了 `1,2,3,4,5` 几个包，然后收到远端的 ACK：`1,3,4,5`，当收到 ACK `3` 时，KCP 知道 `2` 被跳过 1 次，收到 ACK `4` 时，知道 `2` 被跳过了 2 次。

​		**当跳过次数大于等于设置的 resend 的值的时候，此时可以认为** **`2` 号丢失，不用等待超时，直接重传 `2` 号包，大大改善了丢包时的传输速度**。这就是 KCP 的快速重传机制。



### 自动重传请求

​		**自动重传请求 ARQ**（Automatic Repeat-reQuest）模型响应有两种：**UNA（表示此编号之前的所有包已收到，如 TCP）**和 **ACK（表示此编号包已收到）**。

​		只用 `UNA `会导致丢包后只能全部重传，只用 `ACK `则丢包后维护成本太高（某个中间包丢掉后，可能需要维护比较多的后续包的缓存）。以往协议都是二选其一，而**KCP 协议中，除了单独的 `ACK `包外，其他所有包都有 `UNA `信息**。



### 非退让流控

​		`KCP `正常模式同 `TCP `一样使用公平退让法则，即发送窗口大小由：`发送端发送缓存大小`、`接收端剩余接收缓存大小`、`丢包退让`、`慢启动`这四要素决定。但对于传送即时性要求很高的小数据时，可选择通过配置跳过后两步，仅用前两项来控制发送频率。

​		**以牺牲部分公平性和带宽利用率的代价，换取了流畅传输的收益**。



## 基本使用

​		KCP 的 input/output 方法用来对接下层的 UDP 收发模块。而 `ikcp_send`、`ikcp_recv` 提供给上层逻辑调用实现协议的收发。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpkbzZVtYQfLZZC0jkYPqkkhlRicTZJOGmUjCO0mKjqdKKjkGu7vlHicvQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 50%;" />

​		KCP 的数据格式如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrp8RFGHTO5IvMXwDTHnexKpf5E9woBz8WSPcnG7XdhMWx3ia9comeH7NQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

​		KCP 的发送和接收单元是 segment，即应用层的数据可能会拆分成多个 segment 发送。

- `conv`：会话编号。
- `cmd`：segment 类型，IKCP_CMD_ACK、IKCP_CMD_PUSH、IKCP_CMD_WASK、IKCP_CMD_WINS。
- `frg`：是否最后一个 segment。0：数据包的最后一个 segment；1：数据包的中间切片 segment。
- `sn`：下一个待发 segment 的序号。
- `una`：待接收消息序号，表示这之前的所有 segment 都收到了。
- `len`：segment 数据长度，不包含头。



### 发送端

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

### 接收端

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

### 协议配置

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