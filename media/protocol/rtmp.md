# RTMP

​		`Real Time Message Protocol`，实时消息协议

---

​		`Adobe`公司推出的应用层协议，用来解决多媒体数据传输流的多路复用（`Multiplexing`）和分包（`packetizing`）的问题。

​		`RTMP `提供设备兼容性和流媒体稳定性，但需要一个特定的 `Flash Media Server/client` 。但随着`Flash Player`退出历史舞台，RTMP协议也逐渐被更现代的协议替代（`webrtc、hls`）。

​		`RTMP`提供了一套全双工的可靠的多路复用消息服务，类似于`TCP`协议，用来在一对结点之间并行传输带时间戳的音频流，视频流，数据流。（在通讯双方之间传输与时间相关的并行流数据，如音频，视频和数据消息。）

​		通常情况下，不同类型的消息会被分配不同的优先级，当网络传输能力受限时，优先级用来控制消息在网络底层的排队顺序。

​		**由于协议设计对低延时、音视频同步等能力的良好支持，RTMP 是实时直播场景，尤其是推流上行链路中最常用的传输协议之一。**



- 客户端
  - `flash player`
  - `jwplayer`
  - `flowplayer`
  - 自定义
- 服务器端
  - `FMS`（**`Flash Media Server`**），`Adobe`官方的流媒体服务器，收费
  - **`Red5`：**开源流媒体服务器，基于`Java`。
  - `SRS`
  - `ZLMediaServer`

## 参考

[[总结\]RTMP流媒体技术零基础学习方法_怎么学习流媒体-CSDN博客](https://blog.csdn.net/leixiaohua1020/article/details/15814587)

[rtmp规范1.0全面指南 - 职场亮哥 - 博客园 (cnblogs.com)](https://www.cnblogs.com/CHLL55/p/13588927.html)

[Introduction · RTMP协议规范 (gitbooks.io)](https://chenlichao.gitbooks.io/rtmp-zh_cn/content/)

[RTMP协议规范1.0中文版_rtmp specification 1.0-CSDN博客](https://blog.csdn.net/noahsun1024/article/details/52177007)



[手撕Rtmp协议_视界音你而不同的博客-CSDN博客](https://blog.csdn.net/mlfcjob/category_10023015.html)

[【流媒体】RTMP协议概述-CSDN博客](https://blog.csdn.net/weixin_42877471/article/details/141017117?spm=1001.2101.3001.6650.3&utm_medium=distribute.pc_relevant.none-task-blog-2~default~YuanLiJiHua~Position-3-141017117-blog-134497867.235^v43^pc_blog_bottom_relevance_base7&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~YuanLiJiHua~Position-3-141017117-blog-134497867.235^v43^pc_blog_bottom_relevance_base7&utm_relevant_index=6)

[RTMP协议和源码解析_rtmpconnect-CSDN博客](https://blog.csdn.net/xiaopangcame/article/details/134497867?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-8-134497867-blog-51248119.235^v43^pc_blog_bottom_relevance_base7&spm=1001.2101.3001.4242.5&utm_relevant_index=11)

[rtmp 协议详解-CSDN博客](https://blog.csdn.net/xiaopangcame/article/details/134754748)



[抛开fash，自己开发实现C++ RTMP直播流播放器 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7324205.html)



[搭建rtmp直播流服务之4:videojs和ckPlayer开源播放器二次开发(播放rtmp、hls直播流及普通视频) - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003678.html)

[H5浏览器播放RTMP直播流 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003697.html)

[videojs集成--播放rtmp流 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003710.html)

[H5浏览器播放RTMP直播流实现切换 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003724.html)

[HTML5视频直播及H5直播扫盲 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003793.html)



## 协议设计



### 数据传输流程

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpcBm1eAicRQ9JpD9qrDcYJa9jvHdrFraP0ibpKOJVtUZbbYLkY4ePZK8g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 50%;" />

在发送端：

- 把数据封装成消息（`Message`）；
- 把消息分割成块（`Chunk`）；
- 将分割后的块（`Chunk`）通过传输协议（如 TCP`）`协议发送到网络传输出去。

在接收端：

- 在通过 `TCP `协议收到后块（`Chunk`）数据；
- 先将块（`Chunk`）重新组装成消息（`Message`）；
- 通过对消息（`Message`）进行解封装就可以恢复出可处理数据。



### 分包

​		RTMP 有两个重要的概念：**消息**和**块**

- 消息，服务于数据封装，是 RTMP 协议中的基本数据单元
- 块，服务于网络传输

​		通过分层设计，**可以将大的消息（Message）数据分包成小的块（Chunk）通过网络来进行传输，是 RTMP 实现低延时的核心原因**。



### 多路复用

​		由于『消息』和『块』的分层设计，使得音频、视频数据在分割成块时对传输通道是透明的，这样**音频、视频数据就能够合到一个传输流中进行同步传输，实现了多路复用。这个流就是『块流（Chunk Stream）』**。

​		在 RTMP 直播中，实时生成视频 Chunk 和音频 Chunk，依次加入到数据流，通过网络发送到客户端。这样的**复用传输流，也是音视频同步的关键**。



### 优先级

- 块流（Chunck Stream）这一层没有优先级的划分，优先级的设计放在『消息流（Message Stream）』这层来实现
- 不同的消息具有不同的优先级，当网络传输能力受限时，优先级用来控制消息在网络底层的排队顺序
- 最高优先级的控制消息包括**协议控制消息（Protocol Control Messages）**和**用户控制消息（User Control Messages）**
  - 例如：一段实时视频服务会选择丢弃给缓慢的客户的视频信息确保音频信息可以及时被接收

​		通常情况下，优先级是这样的：**控制消息 > 音频消息 > 视频消息**。这样的优先级得到有效执行，分块也非常关键：将大消息切割成小块，可以避免大的低优先级的消息（如视频消息）堵塞了发送缓冲从而阻塞了小的高优先级的消息（如音频消息或控制消息）。

​		`RTMP`消息块流包含它自己的入队协议控制消息，也提供一个高层协议机制用于嵌入用户的控制消息。

### 块大小协商

​		`RTMP `发送端将`Message`切割成`Chunk`的过程中，是以` Chunk Size`（默认值 128 字节）为基准进行切割的。

- `Chunk Size `越大，切割时 `CPU `的负担越小
  - 但在带宽不够宽裕的环境下，发送比较耗时，会阻塞其他消息的发送。
- `Chunk Size `越小，利于网络发送
  - 但切割时 `CPU `的负担越大，而且服务器 `CPU` 负担也相对较大。不适用于高码率数据流的情况
- `Chunk Size `可以根据实际情况改变，通过发送控制命令 `Set Chunk Size` 的方式进行更新
- 充分考虑流媒体服务器、带宽、客户端的情况，通过 `Set Chunk Size` 可动态的适应环境，从而达到最优效果



### 压缩优化

​		`RTMP Chunk Header` 的长度不是固定的，分为：12 字节、8 字节、4 字节、1 字节四种。

- 最完整的 `RTMP Chunk Header` 长度是 12 字节。

  <img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrp6jUCbU2xeHElVBz5ydxh5xeTgicudzS8PLz6CkYpTXPVPKKV27ldVyQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 50%;" />

-  8 字节的 `RTMP Chunk Header`

  - 一般情况下，`msg stream id` 是不会变的
  - 所以针对视频或音频，除了第一个 `Chunk `的` RTMP Chunk Header` 是 12 字节的
  - 后续的 `Chunk `可省略这个 4 字节的字段

  <img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpeHhvH4y3zvtytoDUZKJ0ThYicEazQro3Etn0MSCf7puMcicL7kIMQvEQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:50%;" />

-  4 字节`RTMP Chunk Header`

  - 如果和前一条 `Chunk `相比，当前 `Chunk `的消息长度 `message length` 和消息类型 `msg type id`（视频为 9；音频为 8）字段相同，即可将这两部分也省去

  <img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrphAtgBa65YA6XIgjZB7rgEUCSClAnJ15kSqkZbIUJr98fXlKH2NiaG1w/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:50%;" />

- 1 字节`RTMP Chunk Header`

  - 如果和前一条 `Chunk `相比，当前 `Chunk `的 `msg stream id`、`msg type id`、`message length` 字段都相同
  - 而且都属于同一个消息（由同一个 Message 切割而来），那么这些 Chunk 的时间戳 `timestamp` 字段也会是相同的
  - 故后面的 Chunk 也可以省去 `timestamp` 字段

  ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrptiaMxQIS0eaJpyMLDyLCGNrQ6OA5LAUCmkNwytAOYIHYY5Ru1vE0DVA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### 优缺点

​		优点：在`c/s`之间保持稳定的连接

- **低延迟**：RTMP 通过保持一个持续的 TCP 连接，实现了低延迟的数据传输，适合实时互动和直播场景。
- **实时传输**：RTMP 可以传输实时音视频数据，适合用于需要实时性强的应用，如视频会议、直播和互动
- **分段传输**：RTMP 将音视频数据分割成较小的片段进行传输，确保了数据的实时性和传输的稳定性。
  - 通过将数据流分成大小相等的包，顺序传输到端后组包成媒体流
  - 音频数据默认为 64 字节，视频数据默认为 128 字节


​		缺点：

- 与 `HTML5 `播放器不兼容
  - `RTMP`属于`flash`技术生态
    - 必须使用`flash player`作为`client player`
    - 必须使用`FLV`封装格式
- RTMP 容易受到带宽问题的影响
- **复杂实现**：RTMP 需要专门的 RTMP 服务器和客户端来进行传输和接收，增加了实现和维护的复杂度
- 一些防火墙会墙掉 RTMP 或者其他的一些协议
  - 防火墙对 HTTP 非常友好，不会墙掉 HTTP，因此基于 HTTP 传输协议的成功率更高



### 细节

- 所有整数都是以网络字节序来表示的。
- 数据都是字节对齐的，有填充时用0
- 时间戳是用一个整数来表示的，代表相对于一个起始时间的毫秒数
  - 通常每个流的时间戳都从 0 开始，但这不是必须的，只要通讯双方使用统一的起始时间就可以了
  - 要注意的是，跨流的时间同步（不同主机之间）需要额外的机制来实现
  - 时间戳的长度只有 32 位，所以只能在 50 天内循环（49 天 17 小时 2 分钟 47.296 秒）
    - RTMP 应用在处理时间戳是应该使用连续的数字算法，并且应该支持回环处理
    - 可以假设所有相邻的时间戳间隔不超过 2^31-1 毫秒
    - 在此基础上，10000 在 4000000000 之后，3000000000 在 4000000000 之前
  - 时间戳增量也是以毫秒为单位的无符号整数。时间戳增量可能会是 24 位长度也可能是 32 位长度



## 消息

​		`RTMP `消息被设计为工作在 `RTMP `块流之上

- 理论上可以使用任意传输层协议来发送消息
- 客户端和服务器通过网络发送 `RTMP `消息进行通信
- 消息可以包括音频、视频、数据，甚至其它任何信息



### 格式

​		`Message Header + Message Body`。

```c
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Message Type  |                Payload length                 |
|   (1 byte)    |                  (3 bytes)                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Timestamp                               |
|                       (4 bytes)                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                Stream ID                      |
|                (3 bytes)                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

![img](https://img-blog.csdn.net/20130914205548375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- `Message Type ID`，`1 byte`，消息类型

  - `1-7` 用于协议控制

    > RTMP协议自身管理要使用的消息，用户一般情况下无需操作其中的数据

  - `8，9` 用于传输音频和视频数据

  - `15-20` 用于发送`AMF`编码的命令

    > 负责用户与服务器之间的交互，比如播放，暂停等等

- `Payload Length`，`3 byte`，负载的有效长度，大端序

  - 单位是字节

- `Timestamp`，`4 byte`，时间戳，大端序。

- `message Stream ID`，`3 byte`，任意值，大端序

  - 同一个`Message`被复用到不同的消息流中时，可以通过`message stream id`来区分它们
  - `message`划分成`Chunk` / 还原`Chunk`为`Message`，都根据这个ID来辨识是不是同一个消息的`Chunk`
  - `chunk stream id`描述一个传输流，`message stream id`通常与特定的流（如视频流或音频流）相关联。

- `message body`为`message type id`描述的各类消息，或音视频数据



### 消息类型

- 协议控制消息，主要用于协议内的控制，两大类

  - 协议控制消息：`Message Type ID = 1 2 3 5 6 `
  - 用户控制消息：`Message Type ID = 4 `

- 命令消息，主要用于发送`AMF0/AMF3`编码的命令 （15-20）

  - 命令消息

    - 负责用户与服务器之间操作命令的交互，比如播放、暂停等


    > - 主要有NetConnection 和 NetStream两个类
    > - 两个类分别有多个函数，该消息的调用，可理解为远程函数调用
    
    - `Message Type ID＝20`是`AMF0`编码，`Message Type ID＝17`是`AMF3`编码

  - 数据消息用

    - 音视频元数据，`Message Type ID = 15 18`

  - 共享消息，表示一个`Flash`类型的对象

    - 由键值对的集合组成，用于多客户端，多实例时使用
    - `Message Type ID＝19`为`AMF0`编码，`Message Type ID＝16`为`AMF3`编码

- Audio 音频数据，`Message Type ID = 8`

- Video 视频数据，`Message Type ID = 9`

- 聚合消息：多个`RTMP`子消息的集合

  - `Message Type ID＝22`



#### 协议控制消息

​		协议控制消息：`Message Type ID = 1 2 3 5 6 `

- 必须使用`0`作为 消息流ID，这些流也被称为控制流
- 同时使用`2`作为 块流ID
- 协议控制消息接收立即生效；解析时，时间戳字段被忽略



##### `Set Chunk Size(1)`

​		**设置块大小消息**。通知对端己方发送的`Chunk`分片大小。双方各自维护自己发送`Chunk`分片的大小。

​		默认`Chunk`分片大小为`128`字节，超过是必须发送本消息。

```c
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|0|                   chunk size (31 bits)                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- 首位必须为`0`，其余表示以字节为单位的`31 bit`块大小值
  - 有效值为`1 , 2147483647(0x7FFFFFFF)]`
    - 但所有大于`16777215(0xFFFFFF) `值是等价的
    - 因为没有一个块比一整个消息大，没有一个消息会大于`16777215`字节
      - 消息头已经决定表示 `Payload `的长度的字段是 3 字节
- 通常应该不低于 `128 `字节
- 每个方向上的最大块大小是独立的



##### `Abort Message(2)`

​		**中止消息**。通知对端，如果正在等待一条消息的部分块(已经接收了一部分)，那么可以终止并丢弃已经接收到的块。

​		发送该控制消息表示发送端不再传输同`Message`的`chunk`，接受端接收到这个消息后要丢弃这些不完整的`chunk`

> 应用可能在关闭时发送该消息，表明后面的消息或块流无须处理

```c
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                   chunk stream id (32 bits)                   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- 块流 ID（`chunk stream ID`）：32 比特，表示可以丢弃的消息的块流 ID



##### `Acknowledgement(3)`

​		**确认消息**。确认本端接收到的字节数。

- 收到字节数等于窗口大小（window size）后必须向对方向发送确认消息

  - 会话开始 、收到`Ackowledgement(3)`消息之后重置窗口大小
  - 两次`Acknowledgement(3)`之间能够发送的最大字节数就是窗口大小
  - 窗口大小是指 发送方 收到`Acknowledgement(3)`之前可以发送的最大字节数
    - 双方在会话开始时互相发送`Window Acknowledgement size(5)`来通知己方会发送若干字节后停止发送，等待确认对方收到的字节数


  > 不包括`tcp`包头，应该是`chunk`的大小，即从`tcp `的`recv`函数中获得的内容大小

```c
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                   sequence number (4 bytes)                   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- 序列号（sequence number）：4 字节，自上一个`Acknowledgement(3)`之后收到的字节数



##### `Window Acknowledgement Size(5)`

​		**窗口大小消息**。通知对方本端在两个`Acknowledgement(3)`间能发送的最大字节数。

- 会话开始时，双方都要先对端发送`Window Acknowledgement Size`，用于指明期望获得确认的大小，即己方要进入发送等待的大小
- 发送等于窗口大小的数据之后，就会等待对端的`Ackowledgement(3)`，确认对方已经收到的数据后，才能再次发送

> TCP 本身有滑动窗口跟ACK。所以一般 `Window Acknowledgement Size` 会设置得非常大。

```c
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Acknowledgement Window size (4 bytes)             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```



##### `Set Peer Bandwidth(6)`

​		**设置对端带宽消息**。限制对端的输出带宽。

- 限制对端后续两次`Ackowledgement(3)`间能发送的数据量

> - `Window Acknowledgement Size(5)`是通知对端，己方会在发送若干字节后停发等待`Ackowledgement(3)`
>   - 这是控制己方的输出带宽，告知对端己方的发送速度
> - 但己方收包处理速度未必适配对方的发送速度，因此要根据己方接收处理包的速度，来要求对方以合适窗口大小发送
>   - 否则可能会造成对端长时间等待，告知对端己方的接收速度

- 收到`Set Peer Bandwidth(6)`的一方可能要调整两个`Acknowledgement(3)`之间发送的最大字节数

  - 其值发生变化时，应该再次发送`Window Acknowledgement Size(5)`通知对端，己方进入发送等待的大小

  - 即对端申请更改发送速度，己方同意并更改后，告知对端己方更改后的发送速度


```c
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                 Acknowledgement Window size                   |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |   Limit Type  |
 +-+-+-+-+-+-+-+-+
```

​		限制类型取以下列值之一：

- `0 - Hard`：消息接收端应该将输出带宽限制为指定窗口大小。
- `1 - Soft`：消息接收端应该将输出带宽限制为指定窗口大小和当前窗口大小中较小值。
- `2 - Dynamic`：如果上一个消息的限制类型为 Hard，则该消息同样为 Hard，否则抛弃该消息。



#### 用户控制消息

​		告知对方执行该信息中包含的用户控制事件

​		和前面提到的协议控制信息（Protocol Control Message）不同，这是在`RTMP`协议层的，而不是在`RTMP chunk`流协议层的

- 接收到用户控制消息时，应该立马生效；时间戳可以忽略不用。
- 客户端或者服务器通过用户控制消息来通知对端用户控制相关的事件，该消息携带事件类型和事件数据。

##### `User Control Message(4)`

​		**用户控制消息**。这些消息包含`RTMP`流传输层所使用的信息。

- `chunk stream id == 2`
- `message stream id == 0`

```c
+------------------------------+-------------------------+
|     Event Type (16 bits)     |       Event Data        |
+------------------------------+-------------------------+
```

- `event type`， `2 byte`，事件类型。
- `event data`，可变长
  - 消息是通过 RTMP 块流层传送的，块大小应该足够大，以满足使用单个块可以空纳这类消息

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-3fcd9f8a959e4c862d37bbeb69ab0e44_1440w.webp)



用户控制消息支持以下事件：

| 事件                  | 描述                                                         |
| --------------------- | ------------------------------------------------------------ |
| Stream Begin (=0)     | 服务器发送这个事件来通知客户端一个流已就绪并可以用来通信。<br />默认情况下，这一事件在成功接收到客户端的连接命令之后以ID=0发送。<br />事件数据为4字节，代表了已就绪流的流ID。 |
| Stream EOF (=1)       | 服务器发送这一事件来通知客户端请求的流的数据回放已经结束。<br />在发送额外的命令之前不再发送任何数据。客户端将丢弃接收到的这个流的消息。<br />事件数据为4字节，代表了回放已结束的流的流 ID。 |
| StreamDry (=2)        | 服务器发送这一事件来通知客户端当前流中已没有数据。<br />当服务器在一段时间内没有检测到任何消息，它可以通知相关客户端当前流已经没数据了。<br />这一事件数据为4字节，代表了已没数据的流的流 ID。 |
| SetBuffer Length (=3) | 客户端发送这一事件来通知服务器缓冲区大小 (以毫秒为单位)，这个缓冲区用于缓存来自流的任何数据。<br />此事件在服务器开始处理流之前就发送。<br />事件数据的前4个字节代表了流ID，紧接其后的4个字节代表了以毫秒为单位的缓冲区的长度。 |
| Streams Recorded (=4) | 服务器发送这一事件来通知客户端当前流是一个录制流。<br />事件数据为4字节，代表了录制流的流 ID。 |
| PingRequest (=6)      | 服务器端发送这一事件用于测试客户端是否可达。<br />事件数据是为一个4字节的时间戳，代表了服务器端发送这一命令时的服务器本地时间。<br />客户端在接收到这一消息后会立即发送 PingResponse 回复。 |
| PingResponse（=7）    | 客户端发送这一事件用于回复服务器的PingRequest。<br />事件数据是为一个4字节的时间戳，该时间戳是从接收到的PingRequest的事件数据中获取的。 |



### 命令消息

​		在客户端与服务器之间传递 AMF 编码的命令。



#### AMF

​		**Action Message Format (AMF)**，一种紧凑的二进制格式，用于序列化`ActionScript object graphs`。 

​		可以透过 `AMF over HTTP`的方式将`flash`端资料编码后传回`server`，`server`端的`remoting adaptor`接收到资料后则会译码回正确的`native`对象，交给正确的程序处理。

​		`Message Type ID`为`15-20`的消息用于发送`AMF`编码的命令。

- 第一个字节是数据类型，然后根据数据类型确定数据的长度。
  - 其中，`string`是可变长度，两个字节长度值。
  - 如果是`object`，则`object`内部是个字典，`key`是字符串，`value`是数据
  - 这里还要对数据根据类型对数据解析，同上

​		*AMF数据类型： 

| Type                         | Byte code |
| ---------------------------- | --------- |
| Number                       | 0x00      |
| Boolean                      | 0x01      |
| String                       | 0x02      |
| Object                       | 0x03      |
| MovieClip                    | 0x04      |
| Null                         | 0x05      |
| Undefined                    | 0x06      |
| Reference                    | 0x07      |
| MixedArray                   | 0x08      |
| EndOfObject                  | 0x09      |
| Array                        | 0x0a      |
| Date                         | 0x0b      |
| LongString                   | 0x0c      |
| Unsupported                  | 0x0d      |
| Recordset                    | 0x0e      |
| XML                          | 0x0f      |
| TypedObject (Class instance) | 0x10      |
| AMF3 data                    | 0×11      |



####  `command message(17/20)`

​		**命令消息**。发送的是客户端与服务端之间的 `AMF `编码指令。

> - `20`-`AMF0`，`17`- `AMF3`
> - 可以用来执行对端上的一些操作，比如：connect、createStream、publish、play、pause 等命令。
> - 还可以用于通知发送方请求的命令状态，比如：onstatus、result 等命令。
> - 客户端或服务端也可以通过指令消息来实现远程过程调用（RPC)

​		命令消息包括：命令名、事务 ID、包含相关参数的命令对象。

- `Command Name`，发送端发送时会带有命令的名字，表示要执行什么事件
  - `connect`, `createStream`, `publish`, `play`, `pause `，响应消息使用`onstatus`，`result`
- `Transaction ID`，表示此次命令的标识
  - 远程过程调用的事务ID
- `Command Object`，命令参数
- `Optional User Arguments`，用户自定义的额外信息

​		接受端收到命令后，会返回以下三种消息中的一种：

> 都要带有收到的命令消息中的`TransactionId`，表示响应哪个命令

- `_result `消息表示接受该命令，对端可以继续往下执行流程
- `_error`消息代表拒绝该命令要执行的操作
- `method name`消息代表要在之前命令的发送端执行的函数名称

​		命令消息分为两大类：`netConnection`和`netStream`命令

> 多个`netstream`可以共用一个`netConnection`

- `netConnection`，服务器和客户端之间连接的高层表现对象
- `netStream`：音频流、视频流及其他相关数据传输流，我们会发送如播放、暂停等指令来控制数据流动。



##### `NetConnection Commands` 连接层命令

​		管理双端之间的双路连接状态，同时也提供了异步远程方法调用（RPC）在对端执行某方法。

​		下面的命令可以通过 `NetConnection `发送：

- `connect`
- `call`
- `createStream`
- `close`



###### `connect `

​		请求连接到服务器的应用实例。

| Field Name              | Type   | Description                                                  |
| ----------------------- | ------ | ------------------------------------------------------------ |
| Command Name            | String | Name of the command. Set to “connect”.                       |
| Transaction ID          | Number | Always set to 1.                                             |
| Command Object          | Object | Command information object which has the name-value pairs.<br />键值对集合表示的命令参数 |
| Optional User Arguments | Object | Any optional information 用户自定义的额外信息                |

​		响应：

| Field Name     | Type   | Description                                                  |
| -------------- | ------ | ------------------------------------------------------------ |
| Command Name   | String | `_result` 表示接受连接 or `_error`表示连接失败               |
| Transaction ID | Number | Transaction ID is 1 for connect responses.                   |
| Properties     | Object | Name-value pairs that describe the properties(fmsver etc.) of the connection.<br />该连接的描述属性的键值对 |
| Information    | Object | Name-value pairs that describe the response from the server. <br />来自于服务器的描述应答的键值对<br />`code`, `level`, `description` are names of few among such information. |



​		Connect 指令中会用到的键值对：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-95d3b2437a138e78816e8838a2647d9f_1440w.webp)

| Property        | Type    | Description                                                  | Example Value                           |
| --------------- | ------- | ------------------------------------------------------------ | --------------------------------------- |
| app             | String  | The Server application name the client is connected to.      | testapp                                 |
| flashver        | String  | Flash Player version. It is the same string as returned by the ApplicationScript getversion() function. | FMSc/1.0                                |
| swfUrl          | String  | URL of the source SWF file making the connection.            | file://C:/FlvPlayer.swf                 |
| tcUrl           | String  | URL of the Server. It has the following format. protocol://servername:port/appName/appInstance | rtmp://localhost:1935/testapp/instance1 |
| fpad            | Boolean | True if proxy is being used.                                 | true or false                           |
| audioCodecs     | Number  | Indicates what audio codecs the client supports.             | SUPPORT_SND_MP3                         |
| videoCodecs     | Number  | Indicates what video codecs are supported.                   | SUPPORT_VID_SORENSON                    |
| videoFunction   | Number  | Indicates what special video functions are supported.        | SUPPORT_VID_CLIENT_SEEK                 |
| pageUrl         | String  | URL of the web page from where the SWF file was loaded.      | http://somehost/sample.html             |
| object Encoding | Number  | AMF encoding method.                                         | AMF3                                    |

音频编码：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-741179552566cf4f0bd3001e1ac11921_1440w.webp)

| Codec Flag          | Usage                                             | Value  |
| ------------------- | ------------------------------------------------- | ------ |
| SUPPORT_SND_NONE    | Raw sound, no compression                         | 0x0001 |
| SUPPORT_SND_ADPCM   | ADPCM compression                                 | 0x0002 |
| SUPPORT_SND_MP3     | mp3 compression                                   | 0x0004 |
| SUPPORT_SND_INTEL   | Not used                                          | 0x0008 |
| SUPPORT_SND_UNUSED  | Not used                                          | 0x0010 |
| SUPPORT_SND_NELLY8  | NellyMoser at 8-kHz compression                   | 0x0020 |
| SUPPORT_SND_NELLY   | NellyMoser compression (5, 11, 22, and 44 kHz)    | 0x0040 |
| SUPPORT_SND_G711A   | G711A sound compression (Flash Media Server only) | 0x0080 |
| SUPPORT_SND_G711U   | G711U sound compression (Flash Media Server only) | 0x0100 |
| SUPPORT_SND_NELLY16 | NellyMouser at 16-kHz compression                 | 0x0200 |
| SUPPORT_SND_AAC     | Advanced audio coding (AAC) codec                 | 0x0400 |
| SUPPORT_SND_SPEEX   | Speex Audio                                       | 0x0800 |
| SUPPORT_SND_ALL     | All RTMP-supported audio codecs                   | 0x0FFF |

视频编码：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-da9c9dcf06b7d9c7a607fb34ec82f502_1440w.webp)

| Codec Flag                                    | Usage                               | Value  |
| --------------------------------------------- | ----------------------------------- | ------ |
| SUPPORT_VID_UNUSED                            | Obsolete value                      | 0x0001 |
| SUPPORT_VID_JPEG                              | Obsolete value                      | 0x0002 |
| SUPPORT_VID_SORENSON                          | Sorenson Flash video                | 0x0004 |
| SUPPORT_VID_HOMEBREW                          | V1 screen sharing                   | 0x0008 |
| SUPPORT_VID_VP6 (On2)                         | On2 video (Flash 8+)                | 0x0010 |
| SUPPORT_VID_VP6ALPHA (On2 with alpha channel) | On2 video with alpha channel        | 0x0020 |
| SUPPORT_VID_HOMEBREWV (screensharing v2)      | Screen sharing version 2 (Flash 8+) | 0x0040 |
| SUPPORT_VID_H264                              | H264 video                          | 0x0080 |
| SUPPORT_VID_ALL                               | All RTMP-supported video codecs     | 0x00FF |

视频功能：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-8b04e8fa4dd9c6237cf2a6bae511772f_1440w.webp)

| Function Flag           | Usage                                                       | Value |
| ----------------------- | ----------------------------------------------------------- | ----- |
| SUPPORT_VID_CLIENT_SEEK | Indicates that the client can perform frame-accurate seeks. | 1     |

对象编码：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-9dcd369208f2b024862f7044a03b3d7b_1440w.webp)

| Encoding Type | Usage                                                | Value |
| ------------- | ---------------------------------------------------- | ----- |
| AMF0          | AMF0 object encoding supported by Flash 6 and later. | 0     |
| AMF3          | AMF3 encoding from Flash 9 (AS3).                    | 3     |



###### `call`

​		请求一个`RPC`远程调用。`call `命令传递了 `RPC `的名字，在对端执行某进程。

| Field Name         | Type   | Description                                                  |
| ------------------ | ------ | ------------------------------------------------------------ |
| Procedure Name     | String | Name of the remote procedure that is called. 要调用的进程名称 |
| Transaction ID     | Number | If a response is expected we give a transaction Id. Else we pass a value of 0.<br />如果想要对端响应的话置为非0值，否则置为0 |
| Command Object     | Object | If there exists any command info this is set, else this is set to null type. 命令参数 |
| Optional Arguments | Object | Any optional arguments to be provided. 用户自定义的额外信息  |

​		响应结构为一个正常的命令结构，表示其调用了一个命令：

| Field Name     | Type   | Description                                                  |
| -------------- | ------ | ------------------------------------------------------------ |
| Command Name   | String | Name of the command.调用的命令名称                           |
| Transaction ID | Number | ID of the command, to which the response belongs. 同`call`中的`Transaction ID` |
| Command Object | Object | If there exists any command info this is set, else this is set to null type. 命令参数，没有为`null` |
| Response       | Object | Response from the method that was called. 调用该方法的响应   |



###### `creatStream`

​		创建一个逻辑控制通道，用于传递具体消息，音频，视频，描述数据。传输信息单元为`Chunk`。

> `NetConnection `是默认的通讯通道，流 ID 为 0。协议和一些指令消息，包括 `createStream`，使用默认通讯通道。

| Field Name     | Type   | Description                                                  |
| -------------- | ------ | ------------------------------------------------------------ |
| Procedure Name | String | Name of the command. Set to “createStream”.    `createStream` |
| Transaction ID | Number | Transaction ID of the command.  命令的会话ID                 |
| Command Object | Object | If there exists any command info this is set, else this is set to null type.  命令参数 |

- `Optional User Arguments`，用户自定义的额外信息

​		响应：

| Field Name     | Type   | Description                                                  |
| -------------- | ------ | ------------------------------------------------------------ |
| Command Name   | String | `_result` or `_error`; indicates whether the response is result or error.<br />`_result`表示接受连接，`_error`表示连接失败 |
| Transaction ID | Number | ID of the command that response belongs to.  命令的ID        |
| Command Object | Object | If there exists any command info this is set, else this is set to null type.  <br />如果存在任何命令信息则被设置，否则为`null` |
| Stream ID      | Number | The return value is either a stream ID or an error information object.<br />要么是一个`Stream ID`，要么是一个错误信息对象 |



###### `close`



##### `NetStream Commands` 控制流上的命令

​		基于 `NetConnection `的客户端至服务器间连接，`NetStream `表示可以传递音频流、视频流以及消息流的通道。

- 通道在客户端与服务器连接的 `NetConnection` 上进行流动
  - 在传输层协议之上只能连接一个`NetConnection`
  - 但一个`NetConnection`可以建立多个`NetStream`来建立不同的流通道传输多个数据流
- 服务端收到命令后会通过`onStatus`的命令来响应客户端，表示当前`NetStream`的状态



由客户端发送给服务端的命令：

- play
- play2
- deleteStream
- closeStream
- receiveAudio
- receiveVideo
- publish
- seek
- pause

由服务端响应客户端的命令：

- onStatus



###### `onStatus`

​		服务端通过给客户端发送 onStatus 命令来通知 NetStream 状态的更新。

| Field Name     | Type   | Description                                                  |
| -------------- | ------ | ------------------------------------------------------------ |
| Command Name   | String | Name of the command. Set to “onStatus”.                      |
| Transaction ID | Number | Transaction ID set to 0.                                     |
| Command Object | Null   | There is no command object for onStatus messages.            |
| Info Object    | Object | An AMF object having at least the following three properties: `level`(String): the level for this message, one of “warning”, “status”, or “error”; `code` (String): the message code, for example “NetStream.Play.Start”; and `description` (String): a human-readable description of the message. The Info object MAY contain other properties as appropriate to the code. |

- `Info Object`，`AMF`类型的`Object`
  - `level : warning / status / error` 消息的级别
  - `code`：代表具体状态的关键字，如`NetStream.Play.Start`
  - `description`：可读的消息描述
  - 除了这三种必要信息，用户还可以自己增加自定义的键值对

###### `play`

​		由客户端向服务器发起请求从服务器端接受数据，多次调用该指令也可创建一个播放清单。

​		如果传输的信息是视频的话就是请求开始播流。

> 如果你希望创建一个在不同 `live `或 `recorded `流间切换的动态播放清单，需要多次调用 `play `并传递 `false `以避免每次 `reset`。
>
> 相反地，如果你希望立即播放某一指定流，传递 `true` 以清除等待播放队列中的所有其他流。

| Field Name     | Type    | Description                                                  |
| -------------- | ------- | ------------------------------------------------------------ |
| Command Name   | String  | Name of the command. Set to “`play`”.                        |
| Transaction ID | Number  | Transaction ID set to 0.                                     |
| Command Object | Null    | Command information does not exist. Set to null type.        |
| Stream Name    | String  | Name of the stream to play. To play video (FLV) files, specify the name of the stream without a file extension (for example, “sample”). To play back MP3 or ID3 tags, you must precede the stream name with mp3: (for example, “mp3:sample”). To play H.264/AAC files, you must precede the stream name with mp4: and specify the file extension. For example, to play the file sample.m4v, specify “mp4:sample.m4v”. |
| Start          | Number  | An optional parameter that specifies the start time in seconds. The default value is -2, which means the subscriber first tries to play the live stream specified in the Stream Name field. If a live stream of that name is not found, it plays the recorded stream of the same name. If there is no recorded stream with that name, the subscriber waits for a new live stream with that name and plays it when available. If you pass -1 in the Start field, only the live stream specified in the Stream Name field is played. If you pass 0 or a positive number in the Start field, a recorded stream specified in the Stream Name field is played beginning from the time specified in the Start field. If no recorded stream is found, the next item in the playlist is played. |
| Duration       | Number  | An optional parameter that specifies the duration of playback in seconds. The default value is -1. The -1 value means a live stream is played until it is no longer available or a recorded stream is played until it ends. If you pass 0, it plays the single frame since the time specified in the Start field from the beginning of a recorded stream. It is assumed that the value specified in the Start field is equal to or greater than 0. If you pass a positive number, it plays a live stream for the time period specified in the Duration field. After that it becomes available or plays a recorded stream for the time specified in the Duration field. (If a stream ends before the time specified in the Duration field, playback ends when the stream ends.)  If you pass a negative number other than -1 in the Duration field, it interprets the value as if it were -1. |
| Reset          | Boolean | An optional Boolean value or number that specifies whether to flush any previous playlist. |

- `Stream Name`，要播放的流的名称
  - 如果播放FLV文件，指定不带文件扩展名的流名称
  - 如果回放MP3或ID3标签，必须要流名称前加`mp3:`前缀：`mp3:sample`
  - 播放`h264/aac`文件，在流名称前加上`mp4:`前缀，并且指定文件名后缀：`mp4:sample.m4v`

- `Start`，可选参数，表示从何时开始播流，以秒为单位
  - 默认为－2，代表选取对应该流名称的直播流，即当前正在推送的流开始播放
    - 如果对应该名称的直播流不存在，就选取该名称的流的录播版本
    - 如果这也没有，当前播流端要等待直到对端开始该名称的流的直播。
  - 如果传值－1，那么只会选取直播流进行播放，即使有录播流也不会播放
  - 如果传值或者正数，就代表从该流的该时间点开始播放，如果流不存在的话就会自动播放播放列表中的下一个流
- `Duration`，可选参数，表示回退的最小间隔单位，以秒为单位计数
  - 默认值为－1，代表直到直播流不再可用或者录播流停止后才能回退播放
  - 如果传值为0，代表从当前帧开始播放
  - 如果值为正数，会播放`Duration`字段指定该段时间的直播流，之后播放`Duration`字段指定该段时间的录播流
- `Reset`，
  - true代表清除之前的流，重新开始一路播放
  - false代表保留原来的流，向本地的播放列表中再添加一条播放流



###### `play2`

​		`play2` 命令可以在不改变播放内容的时间轴的前提下切换不同码率的流。服务器为所有支持的码率维护多个文件，这样客户端就可以请求 `play2` 命令。

​		这个消息是在`paly`之后再发送。

| Field Name     | Type   | Description                                                  |
| -------------- | ------ | ------------------------------------------------------------ |
| Command Name   | String | Name of the command. Set to “`play2`”.                       |
| Transaction ID | Number | Transaction ID set to 0.                                     |
| Command Object | Null   | Command information does not exist. Set to null type.        |
| Parameters     | Object | An AMF encoded object whose properties are the public properties described for the flash.net.NetStreamPlayOptions ActionScript object. |

- `parameters`，AMF编码的Flash对象，包括了一些用于描述`flash.net.NetstreamPlayOptions ActionScript obejct`的参数

  > `NetStreamPlayOptions` 对象的公共属性在 `ActionScript3 Language Reference` 中有详细描述。



###### `deleteStream`

​		删除流。用于客户端告知服务器端本地的某个流对象已被删除，不需要再传输此路流。

| Field Name     | Type   | Description                                           |
| -------------- | ------ | ----------------------------------------------------- |
| Command Name   | String | Name of the command. Set to “`deleteStream`”.         |
| Transaction ID | Number | Transaction ID set to 0.                              |
| Command Object | Null   | Command information does not exist. Set to null type. |
| Stream ID      | Number | The ID of the stream that is destroyed on the server. |

- `Stream ID`， 本地已删除，不再需要服务器传输的流的ID

​		服务器不需要发送任何应答。

###### `receiveAudio`

​		通知服务器端该客户端是否需要音频。

| Field Name     | Type    | Description                                                |
| -------------- | ------- | ---------------------------------------------------------- |
| Command Name   | String  | Name of the command. Set to “`receiveAudio`”.              |
| Transaction ID | Number  | Transaction ID set to 0.                                   |
| Command Object | Null    | Command information does not exist. Set to null type.      |
| Bool Flag      | Boolean | true or false to indicate whether to receive audio or not. |

- `Bool Flag`，是否接受音频
  - 如果为`false`，服务器不用回复

  - 如果为`true`，服务器在`onStatus`这个中回复两个状态:
    - `NetStream.Seek.Notify`

    - `NetStream.Play.Start`



###### `receiveVideo`

​		通知服务器端该客户端是否需要视频。

| Field Name     | Type    | Description                                                |
| -------------- | ------- | ---------------------------------------------------------- |
| Command Name   | String  | Name of the command. Set to “receiveVideo”.                |
| Transaction ID | Number  | Transaction ID set to 0.                                   |
| Command Object | Null    | Command information does not exist. Set to null type.      |
| Bool Flag      | Boolean | true or false to indicate whether to receive video or not. |

- `Bool Flag`，是否接受视频
  - 如果为`false`，服务器不用回复

  - 如果为`true`，服务器在`onStatus`这个中回复两个状态:
    - `NetStream.Seek.Notify`

    - `NetStream.Play.Start`



###### `publish`

​		由客户端向服务器发起请求推流到服务器。需要`onStatus`响应。

​		客户端发送 `publish `指令将已命名的流发布到服务器上。使用这个名称，任何客户端都可以播放此流，并接收已发布的音频、视频和数据消息。

| Field Name      | Type   | Description                                                  |
| --------------- | ------ | ------------------------------------------------------------ |
| Command Name    | String | Name of the command. Set to “publish”.                       |
| Transaction ID  | Number | Transaction ID set to 0.                                     |
| Command Object  | Null   | Command information does not exist. Set to null type.        |
| Publishing Name | String | Name with which the stream is published.   流名称            |
| Publishing Type | String | Type of publishing. Set to “live”, “record”, or “append”. `record`: The stream is published and the data is recorded to a new file. The file is stored on the server in a subdirectory within the directory that contains the server application. If the file already exists, it is overwritten. `append`: The stream is published and the data is appended to a file. If no file is found, it is created. `live`: Live data is published without recording it in a file. |

- `Publishing Type`，"live"、"record"、"append"中的一种
  - `live`，表示该推流文件不会在服务器端存储
  - `record`，表示该推流的文件会在服务器应用程序的子目录下保存以便后续播放
    - 如果文件不存在的话就会建立一个新文件写入
    - 如果文件已经存在的话删除原来所有的内容重新写入
  - `append`，也会将推流数据保存在服务器端
    - 如果文件不存在的话就会建立一个新文件写入
    - 如果对应该流的文件已经存在的话保存原来的数据，在文件末尾接着写入

服务器响应 `onStatus` 命令以标识发布的开始。



###### `seek`

​		定位到视频或音频的某个位置。毫秒为单位。

| Field Name     | Type   | Description                                           |
| -------------- | ------ | ----------------------------------------------------- |
| Command Name   | String | Name of the command. Set to “`seek`”.                 |
| Transaction ID | Number | Transaction ID set to 0.                              |
| Command Object | Null   | Command information does not exist. Set to null type. |
| milliSeconds   | Number | Number of milliseconds to seek into the playlist.     |

- `milliSeconds`，定位到该文件的xx毫秒处

​	当 seek 成功后，服务器响应一条 status 消息 `NetStream.Seek.Notify`。如失败则返回一条 `_error` 消息。

###### `pause`

​		暂停。客户端告知服务端停止或恢复播放。

| Field Name         | Type    | Description                                            |
| ------------------ | ------- | ------------------------------------------------------ |
| Command Name       | String  | Name of the command. Set to “pause”.                   |
| Transaction ID     | Number  | There is no transaction ID for this command. Set to 0. |
| Command Object     | Null    | Command information does not exist. Set to null type.  |
| Pause/Unpause Flag | Boolean | true or false, to indicate pausing or resuming play.   |
| milliSeconds       | Number  | Number of milliseconds to seek into the playlist.      |

- `Pause/Unpause Flag`，`true`表示暂停，`false`表示恢复
  - `true`，客户端请求暂停，服务端暂停对应的流会返回`NetStream.Pause.Notify`的`onStatus`命令来告知客户端当前流暂停
  - `false`，客户端请求恢复，服务端会返回`NetStream.Unpause.Notify`的`onStatus`命令来告知客户端当前流恢复
- `milliSeconds`，暂停或者恢复的时间，以毫秒为单位
- 响应失败，返回`_error`信息

> 操作成功后，如流为停止状态，服务器响应一条状态消息 `NetStream.Pause.Notify`；如流为播放状态，则返回 `NetStream.UnPause.Notify`。
>
> 如操作失败，则返回 `_error` 消息。

#### 数据消息

​		数据消息用于客户端向对端发送 Metadata 或者任意用户数据。Metadata 包函了数据的（音频、视频等）详细信息，像创建时间、时长、主题等等。

- `Message Type ID = 15 / 18`: Data Message， `Metadata `包括音视频编码、视频宽高等信息
  - `18`是`AMF0`编码，`15`是`AMF3`编码



####  `Shared Object Message(16/19)`

​		共享对象消息。在多个客户端之间同步的 `Flash `对象 (键值对集合)。每一个消息都可能包含多个事件。

- `19`表示`AMF0`，`16`是`AMF3`。

```c
+------+------+-------+-----+-----+------+-----+ +-----+------+-----+
|Header|Shared|Current|Flags|Event|Event |Event|.|Event|Event |Event|
|      |Object|Version|     |Type |data  |data |.|Type |data  |data |
|      |Name  |       |     |     |length|     |.|     |length|     |
+------+------+-------+-----+-----+------+-----+ +-----+------+-----+
       |                                                            |
       |<- - - - - - - - - - - - - - - - - - - - - - - - - - - - - >|
       |            AMF Shared Object Message body                  |
```



![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-14f035272c2e3ad991ec825c587d1b91_1440w.webp)

支持以下事件类型：

| 事件（Event）              | 取值（Value） | 描述（Description）                                          |
| -------------------------- | ------------- | ------------------------------------------------------------ |
| 创建（Use）                | 1             | 客户端向服务端发送，请求创建指定名称的共享对象。             |
| 释放（Release）            | 2             | 客户端通知服务端，共享对象已在本地删除。                     |
| 请求更新（Request Change） | 3             | 客户端请求修改共享对象的属性值。                             |
| 更新（Change）             | 4             | 服务端向除请求发送方外的其他客户端发送，通知其有属性的值发生了变化。 |
| 成功（Success）            | 5             | “请求更新”事件被接受后，服务端向发送请求的客户端回复此事件。 |
| 发送消息（Send Message）   | 6             | 客户端向服务端发送此事件，来广播一个消息。服务端收到此事件后向所有客户端广播一条消息，包括请求方客户端。 |
| 状态（Status）             | 7             | 服务端发送此事件来通知客户端错误信息。                       |
| 清除（Clear）              | 8             | 服务端向客户端发送此事件，通知客户端清除一个共享对象。服务端在回复客户端的“创建”事件时也会发送此事件。 |
| 移除（Remove）             | 9             | 服务端发送此事件，使客户端删除一个插槽。                     |
| 请求移除（Request Remove） | 10            | 客户端删除一个插槽时发送此事件。                             |
| 创建成功（Use Success）    | 11            | 当连接成功时服务端向客户端发送此事件。                       |



### 音频消息

​		客户端或服务器使用音频消息来发送音频数据

- `Message Type ID = 8`

### 视频消息

​		客户端或服务器使用音频消息来发送视频数据

- `Message Type ID = 9`



### 聚合消息

#### `Aggregate Message(22)`

​		是一个消息包含多个子 `RTMP `消息，子消息符合 `RTMP `消息格式。

```c
             +---------+-------------------------+
             | Header  | Aggregate Message body  |
             +---------+-------------------------+
                  The Aggregate Message format
+--------+-------+---------+--------+-------+---------+ - - - -
|Header 0|Message|Back     |Header 1|Message|Back     |
|        |Data 0 |Pointer 0|        |Data 1 |Pointer 1|
+--------+-------+---------+--------+-------+---------+ - - - -
                The Aggregate Message body format
```

- 组合消息的消息流 ID 会覆盖其中子消息的消息流 ID。
- 组合消息的时间戳和其中第一个子消息的时间戳的差值，是用来将所有子消息的时间戳重整为流时间的位移量。
  - 位移量会加到每一个子消息的时间戳上来换算出正常的流时间。
  - 第一个子消息的时间戳应该与组合消息的时间戳相同，所以位移量应该为 0。
- Back Pointer (反向指针) 包含前一个消息的长度（包括消息头），这样符合 flv 文件格式，可用于进行后退操作(向前 seek)。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-95aff3d3e08567cf0be7baf7bd7734a8_1440w.webp)

​		使用组合消息有以下好处：

- 块流协议中，一个块最多只能发送一个消息，这样就使用组合消息，加大块大小，从而降低发送的块数量。
- 子消息在内存中连续存放，这样系统调用网络发送数据的性能更高。



## 块

​		**块（Chunk）是指消息的一个分片。消息在通过网络发送前被划分成更小的部分并且交叉的发送。**

- **在应用层进行多路复用和分包来降低延迟**，确保音视频同步，并提供流畅的播放体验

  > 数据在分割成块时**对传输通道是透明**的，多路音视频数据就能够**合到一个传输流中进行同步传输**，实现多路复用

### 机制

- 块有唯一的块流 ID， 用来标识块传送使用的是哪一个块流。

- 必须在一个`Chunk`发送完成后才能开始发送下一个`Chunk`

- 在接收端，将所有块根据块中的块流 ID 组装成消息
  - 分块将上层协议的大消息分割成小的消息
  - 保证大的低优先级消息（比如视频）不阻塞小的高优先级消息（比如音频或控制消息）

- 降低消息发送的开销，在块头中包含了压缩的原本需要在消息中所包含的信息

- 块大小是可配置的

  - 默认是`128`字节，`Set Chunk Size`可以设置`Chunk`的最大值
  - 最后一个数据块可以小于设定的固定长度

  - 发送端和接受端会各自维护一个`Chunk Size`，可以分别设置这个值来改变自己这一方发送的`Chunk`的最大大小

  - 越大的块 CPU 使用率越低，但是在低带宽的情况下，大的写入会阻塞其他内容的写入；而小一些的块不适合高比特率的流

    > - 大`Chunk`减少计算每个`chunk`的时间，从而减少`CPU`的占用率
    >   - 但会占用更多的时间在发送上，尤其是在低带宽的网络情况下，很可能会阻塞后面更重要信息的传输
    > - 小`Chunk`可以减少阻塞问题，但小`Chunk`会引入过多额外的信息（`Chunk`中的`Header`）
    >   - 少量多次的传输可能会造成发送的间断导致不能充分利用高带宽的优势，因此并不适合在高比特率的流中传输

  - 在实际发送时应对要发送的数据用不同的`Chunk Size`去尝试

    - 通过抓包分析等手段得出合适的`Chunk`大小
    - 在传输过程中可以根据当前的带宽信息和实际信息的大小动态调整`Chunk`的大小
    - 尽量提高`CPU`的利用率并减少信息的阻塞机率


### 块流

​		块流指通信的逻辑通道，为高层流媒体协议提供复用和分包的功能，多路音视频同步传输的流称为**块流 Chunk Stream**

- 一个或多个块流可能会复用同一个连接

- 每个块流承载来自同一个消息流的同一类消息

- 每个消息包含时间戳和负载类型信息

  > 块流除了工作在 `RTMP` 协议上，也可以使用其他协议来发送消息数据。

- `RTMP `块流和 `RTMP `协议组合可以适用于多种音视频应用，从一对一或一对多直播到视频会议都能很好的满足

- 当使用可靠传输协议（如 `TCP`）时，`RTMP `块流为所有消息提供了可靠的跨流端对端按时间戳顺序发送的机制

- `RTMP `块流不提供优先级控制，但是可以由上层协议提供这样的优先级

  - 当某个客户端网络比较慢时，可能会选择抛弃一些视频消息来保证声音消息能够及时接收

  > `RTMP `块流集成了带内（`in-band`）协议控制消息，而且为高层协议提供了嵌入自定义控制消息的机制。例如，一个实时视频服务器可能会参考每条消息发送和响应的时间，来决定是否要丢弃部分视频消息以满足较慢客户端能够流畅地接收音频数据。

- 块流除自身内置的协议控制消息外，还为上层协议提供了用户控制消息的机制



### 块结构

​		在网络环境上传输时，`Message`被拆分成较小的数据块。实际的数据被分割成固定大小的数据块。

> - 避免优先级低的消息持续发送阻塞优先级高的数据
> - 对于数据量较小的`Message`，可以通过对`Chunk Header`的字段来压缩信息，从而减少信息的传输量

<img src="https://img-blog.csdn.net/20130914205506281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center" alt="img" style="zoom:25%;" />

```
 +--------------+----------------+--------------------+--------------+
 | Basic Header | Message Header | Extended Timestamp |  Chunk Data  |
 +--------------+----------------+--------------------+--------------+
 |                                                    |
 |<------------------- Chunk Header ----------------->|
```

- `chunk data`：上限为配置的最大块大小
- `chunk basic header`：`1-3` 字节，块基本头。
  - 对块流 ID 和块类型进行编码。
  - 块类型标明了消息头的编码格式。
  - 字段的长度取决于块流 ID，因为块流是一个变长的字段。
- `chunk messge header`：`0 / 3 / 7 / 11`字节，块消息头。
  - 对发送的消息（部分或和全部）的信息进行了编码。
  - 字段的长度取决于块头的块类型。
- `extended timestamp`：`0 / 4`字节，块扩展时间戳
  - 是否出现取决于块消息头中的时间戳（timestamp）或时间戳增量（timestamp delta）
- 载荷为`h264`数据时，要用`AMF3`进行编码



<img src="https://img-blog.csdn.net/20130914205513812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center" alt="img" style="zoom: 25%;" />

> ​		如果我们创建一个块，消息格式应该（SHOULD）包含下面的字段：
>
> - 时间戳：消息时间戳，该字段为 4 个字节。
> - 长度：消息负载长度，如果消息头（header）不能被省略，其长度也应计入其中。该字段占用块头中的 3 个字节。
> - 类型 ID：一些类型 ID 预留用于协议控制消息，这些传递信息的消息会同时被 RTMP 块流协议和高层协议处理。所有其他的类型 ID 则由高层协议使用，RTMP 块流会直接忽略这些值。实际上，RTMP 块流并不需要类型 ID ，所有消息都可以是同一类型，有时候应用可以通过该字段来区隔同步轨。该字段占用块头中的 1 个字节
> - 消息流 ID ：消息流 ID 可以是任意值，不同的消息流复用到同一块流，然后再根据各自消息流 ID 进行解复用。除此之外，别无用处。该字段占用块头中的 4 字节，使用小端格式。



#### `Chunk Basic Header` 基础头

​		将块类型和块流 ID 进行了编码。块类型决定了消息头的编码格式。

​		`Chunk Basic Header`的长度可能是1，2，或3个字节，至少1个字节。小端存储。

> 具体是多少字节取决于块流 ID。为了高效，协议的实现应该（SHOULD）使用能够容纳 ID 的最小表示来节省字节。

- `chunk type(fmt)`，`2 bit`，决定了后面`Chunk Message Header`的编码格式和长度

- `chunk stream id(CSID)`，可变长字段，唯一表示一个特定的传输流通道
  - 表示一个音频流或视频流等

    - 视频流和音频流等对应的`CSID`相同，则为多路复用

  - `RTMP`协议支持用户自定义`［3，65599］`之间的`CSID`

    - `0，1，2`不用作`ID`，用于参与对`chunk basic header`后续长度的计算，见下

  - 判断`chunk type`所在字节的后`6 bit`

    - `> 1`，
      
      - `chunk basic header` `1`字节，此`6 bit`即`CSID`，取值`[3, 63]`
      - 如果 `== 2 (00 0010)`， 这个`RTMP`包是一个`Protocol Control Messag`，用户控制消息
      
      > ```c
      >    0 1 2 3 4 5 6 7
      >   +-+-+-+-+-+-+-+-+
      >   |fmt|   cs id   |
      >   +-+-+-+-+-+-+-+-+
      >  Chunk basic header 1
      > ```
      
      - 实际使用中，块基本头一般用 1 字节格式。
      
    - `== 0 (00 0000)` 
    
      -  ` chunk basic header`  2 字节，此`6 bit`后随的`8 bit`为`CSID - 64`
      - 块流 ID 的计算方法是:`第二个字节 + 64`。
      - `CSID `在［`0+64，255+64］`之间，`[0000 0000, 1111 1111] + 64`
    
      > ```c
      >   0                      1
      >   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
      >  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      >  |fmt|      0    |  cs id - 64   |
      >  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      >       Chunk basic header 2
      > ```
    
    - `== 1 (00 0001)`
    
      - ``chunk basic header` 3字节，此`6 bit`后随的`16 bit`为`CSID - 64`
      - 块流 ID 的计算方法是：`(第三个字节) * 256 + 第二个字节 + 64`
      - `CSID `在［`0+64，65535+64］`之间，`[0000 0000 0000 0000, 1111 1111 1111 1111] + 64`
    
      > ```c
      >   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
      >  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      >  |fmt|         1 |          cs id - 64           |
      >  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      >              Chunk basic header 3
      > ```

```c
// srs中的实现(srs_rtmp_stack.cpp)
int SrsProtocol::read_basic_header(char& fmt, int& cid)
{
    int ret = ERROR_SUCCESS;
    
    if ((ret = in_buffer->grow(skt, 1)) != ERROR_SUCCESS) {
        if (ret != ERROR_SOCKET_TIMEOUT && !srs_is_client_gracefully_close(ret)) {
            srs_error("read 1bytes basic header failed. required_size=%d, ret=%d", 1, ret);
        }
        return ret;
    }
    
    fmt = in_buffer->read_1byte();
    cid = fmt & 0x3f;
    fmt = (fmt >> 6) & 0x03;
    
    // 2-63, 1B chunk header
    if (cid > 1) {//大于1肯定是1个字节，所以直接返回即可
        srs_verbose("basic header parsed. fmt=%d, cid=%d", fmt, cid);
        return ret;
    }

    // 64-319, 2B chunk header
    if (cid == 0) {//为0表示2个字节
        if ((ret = in_buffer->grow(skt, 1)) != ERROR_SUCCESS) {
            if (ret != ERROR_SOCKET_TIMEOUT && !srs_is_client_gracefully_close(ret)) {
                srs_error("read 2bytes basic header failed. required_size=%d, ret=%d", 1, ret);
            }
            return ret;
        }
        
        cid = 64;
        cid += (u_int8_t)in_buffer->read_1byte();
        srs_verbose("2bytes basic header parsed. fmt=%d, cid=%d", fmt, cid);
    // 64-65599, 3B chunk header
    } else if (cid == 1) {//为1表示3个字节
        if ((ret = in_buffer->grow(skt, 2)) != ERROR_SUCCESS) {
            if (ret != ERROR_SOCKET_TIMEOUT && !srs_is_client_gracefully_close(ret)) {
                srs_error("read 3bytes basic header failed. required_size=%d, ret=%d", 2, ret);
            }
            return ret;
        }
        
        cid = 64;
        cid += (u_int8_t)in_buffer->read_1byte();
        cid += ((u_int8_t)in_buffer->read_1byte()) * 256;
        srs_verbose("3bytes basic header parsed. fmt=%d, cid=%d", fmt, cid);
    } else {
        srs_error("invalid path, impossible basic header.");
        srs_assert(false);
    }
    
    return ret;
}
```



#### `Chunk Message Header` 消息头

​		有四种不同的块消息头，其类型由块基本头中的 `fmt` 字段表示。

​		长度由`chunk basic header`中的`chunk type`决定。`RTMP `的实现应该尽可能为每一种块消息头使用最紧凑的表示。

> 将不同消息流混合到同一个的块流里是有可能的，但这样就不能有效地进行头部压缩来节省带宽。
>
> 块消息头的压缩本质上是利用同一消息在传输流中的连续性，一旦混合和多个消息流，这种连续性将被打破，即不能有效压缩块消息头部。

​		标识`chunk`负载所属消息，包含了将要发送的`Message`的信息。

> 不同的块消息头格式中，用到的一些字段做一下解释：
>
> - 时间戳（timestamp，3 字节）：对于类型为 0 块，这里发送的是绝对时间戳。
>   - 如果时间戳大于等于 16777215（`2 ^ 24 - 1`），这个字段必须（MUST）是 16777215
>     - 表明扩展时间戳（Extended Timestamp）字段使用全 32 位对时间进行了编码。
>   - 否则，这个字段应该（SHOULD）表示完整的时间戳。
> - 时间戳增量（timestamp delta，3 字节）：对于类型为 1 和类型为 2 的块，这里发送的是先前块与当前块之间的差值。
>   - 如果增量大于等于 16777215（`2 ^ 24 - 1`），这个字段必须（MUST）是 167777215，表明扩展时间戳（Extended Timestamp）字段使用全 32 位的时间戳增量进行了编码。
>   - 否则，这个字段应该（SHOULD）表示实际的时间戳增量。
> - 消息长度（message length，3 字节）：对于类型为 0 或者类型为 1 的块，这里发送的是消息的长度。
>   - 注意一般情况下，这与块有效负载的长度是不一样的。
>   - 所有块的有效负载长度除了最后一块外都是块的额定最大长度，最后一块长度是消息的剩余长度（对于长度很短的消息，这可能是消息的全部长度）。
> - 消息类型 ID（message type id，1 字节）：对于类型为 0 和类型为 1 的块，这里表示的是消息类型。
> - 消息流 ID（message stream id，4 字节）：对于类型为 0 的块，存储的是消息流 ID。
>   - 消息流 ID 是以小端（little-endian）格式存储的。
>   - 通常情况下，所有使用相同块流的消息应该来自于相同的消息流。
>   - 将不同消息流混合到同一个的块流里是有可能的，但这样就不能有效地进行头部压缩来节省带宽。
>   - 如果现一个消息流关闭了，随后又打开了一个新的消息流，这时可以通过发送块类型为 `0` 的块来复用之前的块流。

##### `fmt == 00`

​		`11` 字节长。这种类型必须（MUST）在块流开始的时候使用，不管是不是流的时间戳回溯（比如回溯定位）。

```c
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                   timestamp                   | message length|
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |      message length (cont)    |message type id| msg stream id |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |          message stream id (cont)             |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                Chunk Message Header - Type 0
```

- `fmt == 00`，`11 bytes`，能表示其他三种数据，但`Chunk stream`的开始第一个`chunk`和头信息中时间戳后退的时候必须采用这种格式
  - `timestamp`，`3 bytes`，消息的绝对时间戳，三个字节都为`1`时，实际的时间戳转存到`Extended Timestamp`
  - `message length`，`3 bytes`，实际发送的消息的数据如音频、视频帧等数据的长度，单位时字节。
    - 是指整个`Message`的长度，而不是单个`chunk`的负载长度
  - `Message type id`，1 bytes，表示实际发送的数据的类型，如8代表音频数据，9代表视频数据。
  - `message stream id`，4个字节，用于区分不同流的消息
    - 采用小端存储方式

##### `fmt == 01`

​		 `7 `个字节长。没有包含消息流 ID（message stream id）。

​		这个块使用了前一个块的消息流 ID。变长消息的流（比如很多视频格式）应该在块流的第一个块之后每个新消息的首个块之后使用这个格式。

> 一般在时间戳增长，但消息流单一且大小不稳定的消息流中出现，比如连续的视频消息流。

```c
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                   timestamp delta             | message length|
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |      message length (cont)    |message type id|  
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
                Chunk Message Header - Type 1
```

- `fmt == 01`，`7bytes`，去掉`message stream id`，表示此`chunk`和上一次发的`chunk`所在的流相同
  - 如果在发送端和对端有一个流链接的时候可以尽量采用这种格式
  - `timestamp delta`，`3 byte`s，和`type=0`时不同，存储的是和上一个`chunk`的时间差
    - 三个字节都为`1`时，实际的时间戳转存到`Extended Timestamp`
  - 其他字段与上面的解释相同

##### `fmt == 10`

​		 `3` 个字节长。不包含消息流 ID （message stream id）和消息长度（message length）。

​		这个块与上一个块有相同的消息流 ID 和消息长度。消息有固定长度的流（比如说，一些音频和数据格式）应该在流的第一个块之后每个消息的首个块之后使用这个格式。

> 一般在时间戳增长，但消息流单一且大小稳定的消息流中出现，比如连续的音频消息流。
>
> 此包给出时间戳增长的周期。

```c
  0                   1                   2     
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
 |                 timestamp delta               |  
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
          Chunk Message Header - Type 2
```

- `fmt == 10`，3 bytes，只保留`timestamp`，表示此 chunk和上一次发送的 chunk 所在的流、消息的长度和消息的类型都相同
  - `timestamp delta`，同上

##### `fmt == 11`

​		没有块消息头。不包含消息流 ID（message stream id）、消息长度（message length）和时间戳（timestamp）字段。

​		当单个消息被分解成多个块，除了第一个块之外的后续块应当使用类型 3 的块。这些块有相同消息流 ID、消息长度和时间戳。

​		由相同大小、消息流 ID 和时间戳增量的消息组成的流，在类型 2 的块之后所有块都应该使用该类型格式。

​		如果第一个消息和第二消息之间的时间戳增量与第一个消息的时间戳相同，则 0 类型的块之后可以马上发送 3 类型的块，而不必使用 2 类型的块来注册时间戳增量。如果类型 3 的块跟在类型 0 的块后面，那么 3 类型块的时间戳增量与 0 类型块的时间戳相同。



- `fmt == 11`，0 bytes，表示这个`chunk`的`Message Header`和上一个是完全相同的

  - 在`type == 0`的`chunk`后时，表示和前一个 `chunk `的时间戳都是相同

    - 就是一个 `Message `拆分成多个 `chunk`，这个 `chunk `和上一个 `chunk `同属于一个 `Message`

  - 在`type == 1 / 2`的`chunk`后时，和前一个 `chunk`的时间戳的差是相同的

    > 第一个 `chunk `的` type = 0，timestamp = 100`
    >
    > 第二个 `chunk `的` type = 2`，`timestamp delta = 20`，` timestamp = 100 + 20`
    >
    > 第三个 `chunk `的 `type = 3， timestamp delta = 20`，`timestamp = 120 + 20`



#### `Extended Timestamp` 拓展时间戳

​		取决于块消息头中编码的时间戳，当时间戳溢出时（块消息头中用3字节表示时间戳）才出现。

​		扩展时间戳占` 4 `个字节，能表示的最大数值就是 `0xFFFFFFF（ 4294967295）`。

> 当扩展时间戳启用时，`timestamp` 字段或者 `timestamp delta` 要全置为 1，表示应该去扩展时间戳字段来提取真正的时间戳或者时间戳增量。
>
> 注意扩展时间戳存储的是完整值，而不是减去时间戳或者时间戳增量的值。

​		当`chunk message header`的时间戳等于`0xffffff`的时候`chunk message header`后面的四个字节就代表扩展时间。





##### example

###### simple audio stream

简单的音频流消息，这个示例示范了信息冗余。

```c

+---------+-----------------+-----------------+-----------------+
|         |Message Stream ID| Message TYpe ID | Time  | Length  |
+---------+-----------------+-----------------+-------+---------+
| Msg # 1 |    12345        |         8       | 1000  |   32    |
+---------+-----------------+-----------------+-------+---------+
| Msg # 2 |    12345        |         8       | 1020  |   32    |
+---------+-----------------+-----------------+-------+---------+
| Msg # 3 |    12345        |         8       | 1040  |   32    |
+---------+-----------------+-----------------+-------+---------+
| Msg # 4 |    12345        |         8       | 1060  |   32    |
+---------+-----------------+-----------------+-------+---------+
           Sample audio messages to be made into chunks
```

下图展示该消息流分割成的块。从 `Chunk#3` 的块开始进行数据传输优化，从这里开始的块，块头只占用 1 字节（只有块基本头，没有块消息头）。

```c

+--------+---------+-----+------------+------- ---+------------+
|        | Chunk   |Chunk|Header Data |No. of     |Total No. of|
|        |Stream ID|Type |            |Bytes After|Bytes in the|
|        |         |     |            |Header     |Chunk       |
+--------+---------+-----+------------+-----------+------------+
|Chunk#1 |    3    |  0  | delta: 1000|   32      |    44      |
|        |         |     | length: 32,|           |            |
|        |         |     | type: 8,   |           |            |
|        |         |     | stream ID: |           |            |
|        |         |     | 12345 (11  |           |            |
|        |         |     | bytes)     |           |            |
+--------+---------+-----+------------+-----------+------------+
|Chunk#2 |    3    |  2  | 20 (3      |   32      |    36      |
|        |         |     | bytes)     |           |            |
+--------+---------+-----+----+-------+-----------+------------+
|Chunk#3 |    3    |  3  | none (0    |   32      |    33      |
|        |         |     | bytes)     |           |            |
+--------+---------+-----+------------+-----------+------------+
|Chunk#4 |    3    |  3  | none (0    |   32      |    33      |
|        |         |     | bytes)     |           |            |
+--------+---------+-----+------------+-----------+------------+
         Format of each of the chunks of audio messages
```



###### message split

​		超过 128 字节长度的消息，在发送的过程中被分割成了多个块

```c

+-----------+-------------------+-----------------+-----------------+
|           | Message Stream ID | Message TYpe ID | Time  | Length  |
+-----------+-------------------+-----------------+-----------------+
| Msg # 1   |       12346       |    9 (video)    | 1000  |   307   |
+-----------+-------------------+-----------------+-----------------+
                Sample Message to be broken to chunks
```

下图是被分割成的块：

```c

+-------+------+-----+-------------+-----------+------------+
|       |Chunk |Chunk|Header       |No. of     |Total No. of|
|       |Stream| Type|Data         |Bytes After| Bytes in   |
|       | ID   |     |             |Header     | the Chunk  |
+-------+------+-----+-------------+-----------+------------+
|Chunk#1|  4   |  0  | delta: 1000 |  128      |   140      |
|       |      |     | length: 307 |           |            |
|       |      |     | type: 9,    |           |            |
|       |      |     | stream ID:  |           |            |
|       |      |     | 12346 (11   |           |            |
|       |      |     | bytes)      |           |            |
+-------+------+-----+-------------+-----------+------------+
|Chunk#2|  4   |  3  | none (0     |  128      |   129      |
|       |      |     | bytes)      |           |            |
+-------+------+-----+-------------+-----------+------------+
|Chunk#3|  4   |  3  | none (0     |  51       |   52       |
|       |      |     | bytes)      |           |            |
+-------+------+-----+-------------+-----------+------------+
                 Format of each of the chunks
```

第一个块 `Chunk#1` 的头信息指明了消息总大小为 307 字节。

从两个示例观察到，类型为 3 的块有两种不同的使用途径：

- 1）是用来指示新消息的开始，这个消息的头继承于已有的状态数据（示例 1）。
- 2）用于同一个消息的连续块（示例 2）。

## 握手

​		`RTMP `的连接开始于握手。握手内容不同于协议的其它部分，它包含三个固定大小的块，而不是带头信息的变长块。

​		客户端（发起连接的端点）和服务器各自发送相同的三个块。这三个块客户端发送的被记做 C0，C1，C2；服务发送的被记做 S0，S1，S2。

​		RTMP 协议本身并没有规定这 6 个消息的具体传输顺序，但 RTMP 协议的实现者需要保证这几点：

- 客户端发送 C0 和 C1 块开始握手。
- 客户端必须（MUST）等接收到 S1 后才能发送 C2。
- 客户端必须（MUST）等接收到 S2 后才能发送其它数据。
- 服务器必须（MUST）等接收到 C0 才能发送 S0 和 S1，也可以（MAY）等接到 C1 一起之后。
- 服务器必须（MUST）等到 C1 才能发送 S2。
- 服务器必须（MUST）等到 C2 才能发送其它数据。

### 简单握手包结构

#### `c0 / s0`

​		单独的一个字节

```c
// C0 and S0 bits
 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+
|    version    |
+-+-+-+-+-+-+-+-+
```

- ##### 版本(8比特)

  - 在`C0`包内，这个字段代表客户端请求的RTMP版本号
  - 在`S0`包内，这个字段代表服务端选择的RTMP版本号
  - 版本`0-2`用在早期的产品中，现在已经被弃用
  - 服务器无法识别客户端的版本号，应该回复版本`3`
    - 客户端可以选择降低到版本`3`，或者中止握手过程。 
  - 版本`4-31`被预留用于后续产品
  - 版本`32-255`不允许使用
    - 为了区分`RTMP`协议和文本协议，文本协议通常以可打印字符开始

#### `c1 / s1`

​		固定为 1536 字节

```c
// C1 and S1 bits
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|				 time (4 bytes) 							| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| 				zero (4 bytes) 								| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 				random bytes						 		| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 			random bytes | | (cont) | | .... 		  		   | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
```

- 共`1526 Byte`

- 时间戳(4字节)
  - 该时间戳应该作为发送端所有后续块的时间戳起始时间
  - 时间戳取值可以是 0，也可以是其他的任意值
  - 为了同步多个块流，客户端可能希望多个块流使用相同的时间戳

- 零(4字节)，本字段必须为零
- random （1528 bytes），本字段可以包含任意数据
  - 由于握手的双方需要区分另一端，此字段填充的数据必须足够随机(以防止与其他握手端混淆)
  - 不过没必要为此使用加密数据或动态数据

#### `c2 / s2`

​		固定为 1536 字节，基本上是 S1 和 C1 的回传

```c
// C2 and S2 bits
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 					time (4 bytes) 							| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 					time2 (4 bytes) 						| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 					random echo 							| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 				random echo | | (cont) | | .... 			  | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
```

- time(4字节)，对端发送的时间戳，对 C2 来说是 S1， 对 S2 来说是 C1
- time2(4字节)，接收对端发送过来的握手包的时刻，对 C2 来说是 S1， 对 S2 来说是 C1
  - 握手的双方可以使用时间1和时间2字段来估算网络连接的带宽和/或延迟，但是不一定有用
  - 任何一端都可以用时间戳和时间戳 2 两个字段值和当前时间戳来快速的估算带宽和延迟，但这样可能并不实用
- 随机数据(1528字节)，返回对端发送过来的随机数据



### 复杂握手包结构

#### `c0 / s0`

```c
// C0 and S0 bits
+-+-+-+-+-+-+-+-+ 
| 	version 	| 
+-+-+-+-+-+-+-+-+ 
```

- ##### 版本(8比特)
- RTMP的版本，一般为3

#### `c1 / s1`

```c
// C1 and S1 bits
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|				 time (4 bytes) 							| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| 				version (4 bytes) 							| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 				  key (764 bytes)					 		| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 				digest (764 bytes)   		   				| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
```

- `key`和`digest`的顺序是不确定
  - `nginx-rtmp-module`中先`digest`后`key`

- 764 bytes key 结构：
  - random-data: (offset) bytes
  - key-data: 128 bytes
  - random-data: (764 - offset - 128 - 4) bytes
  - offset: 4 bytes

- 764 bytes digest 结构：
  - offset: 4 bytes
  - random-data: (offset) bytes
  - digest-data: 32 bytes
  - random-data: (764 - 4 - offset - 32) bytes



#### `c2 / s2`

```c
// C2 and S2 bits
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 					random-data (1504 bytes) 				 | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| 					random-dta (32 bytes) 			  		| 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
```

- time(4字节)，对端发送的时间戳
- time2(4字节)，接收对端发送过来的握手包的时刻
  - 握手的双方可以使用时间1和时间2字段来估算网络连接的带宽和/或延迟，但是不一定有用。
- 随机数据(1528字节)，包含对端发送过来的随机数据











## 流程

​		**发送端**：把编码封装好的音视频数据打包成消息 Message 后分割成块 Chunk，通过可靠传输协议（如 TCP）以 Audio Chunk 和 Video Chunk 的形式发送到网络传输出去。

​		**接收端**：通过 TCP 协议收到块数据，先将块重新组装成消息，再对消息进行解封装就可以恢复出可处理数据。

- `RTMP Connection`代表`c/s`之间基础连接关系，`c/s`之间只能有一个`RTMP Connection`
- `RTMP Stream`代表收发流媒体的一个通道，在一个`RTMP Connection`上可以有多个`RTMP Stream`（块流）

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/8e1fbff1fe2b0c6485843ff8bf97da10.png)

​		流程：

- 握手

  - 建立可靠传输链接，tcp握手建立`TCP Connection`

    > RTMP依赖可靠的传输层协议，如tcp，或有可靠保证的基于udp的传输协议。

  - `RTMP`握手

- 建立`RTMP Connection`

  - 在Connection链接上会传输一些控制信息
    - 如`SetChunkSize`,`SetACKWindowSize`，`CreateStream`等

- 建立`RTMP Stream`

  - `CreateStream`命令会创建一个`Stream`链接
  - 用于传输具体的音视频数据和控制这些信息传输的命令信息

-  RTMP通过 `RTMP Message `荷载数据

   - 实际传输为了更好地实现多路复用、分包和信息的公平性，发送端会把`Message`划分为带有`Message ID`的`Chunk`
     - 每个`Chunk`可能是一个单独的`Message`，也可能是`Message`的一部分
   - 接受端根据`chunk`中包含的`data`的长度，`message id`和`message`的长度把`chunk`还原成完整的`Message`

   > Message是协议逻辑分片，chunk是传输物理分片



### 握手

```c

+-------------+                            +-------------+
|   Client    |      TCP/IP Network        |     Server  |
+-------------+             |              +-------------+
       |                    |                     |
Uninitialized               |                Uninitialized
       |        C0          |                     |
       |------------------->|           C0        |
       |                    |-------------------->|
       |        C1          |                     |
       |------------------->|           S0        |
       |                    |<--------------------|
       |                    |           S1        |
  Version sent              |<--------------------|
       |        S0          |                     |
       |<-------------------|                     |
       |        S1          |                     |
       |<-------------------|               Version sent
       |                    |           C1        |
       |                    |-------------------->|
       |        C2          |                     |
       |------------------->|           S2        |
       |                    |<--------------------|
    Ack sent                |                   Ack Sent
       |        S2          |                     |
       |<-------------------|                     |
       |                    |           C2        |
       |                    |-------------------->|
Handshake Done              |               Handshake Done
      |                     |                     |
          Pictorial Representation of Handshake
```

- 客户端发送`c0,c1`块。服务器收到`C0`或`C1`后发送`S0`和`S1`
  - 因为可能乱序，所以`c0 / c1`收到任意一个就可以发送`s0`和`s1`

- 客户端收齐`S0`和`S1`后，开始发送`C2`
- 当服务器收齐`C0`和`C1`后，开始发送`S2`
- 客户端和服务器分别收到`S2`和`C2`后，握手完成，才能发送其他消息

![img](https://img-blog.csdnimg.cn/img_convert/f29857b212e51e32cbc94e95c9a2435c.png)



#### 握手状态

- 未初始化
  - 协议版本被发送，客户和服务端都是未初始化的
  - 客户端在包`c0`中发送协议版本，然后发送`c1`
    - 服务端支持这个版本，它将会发送`s0`和`s1`作为响应
    - 服务端不支持，则会用相应的动作来响应，在RTMP中这个动作是结束这个连接
- 版本发送完成
  - 客户端和服务端在未初始化状态之后都进入到版本发送完成状态
  - 客户端等待包`s1`,而服务端等待包`c1`
  - 在收到相应的包后,客户端发送包`c2`,而服务端发磅包`s2`,状态变成询问发送完成
- 询问发送完成
  - 客户端和服务端等待`s2`和`c2`
- 握手完成
  - 客户端和服务端开始交换消息



###  连接

```c
+--------------+                              +-------------+
|    Client    |              |               |    Server   |
+------+-------+              |               +------+------+
       |               Handshaking done              |
       |                      |                      |
       |                      |                      |
       |----------- Command Message(connect) ------->|
       |                                             |
       |<------- Window Acknowledgement Size --------|
       |                                             |
       |<----------- Set Peer Bandwidth -------------|
       |                                             |
       |-------- Window Acknowledgement Size ------->|
       |                                             |
       |<------ User Control Message(StreamBegin) ---|
       |                                             |
       |<------------ Command Message ---------------|
       |        (_result- connect response)          |
       |                                             |
```

- 客户端发送消息`connect`到服务器，请求服务建立连接
- 服务器接收到连接`connect`消息后，发送`Window Acknowledgement Size`消息到客户端确认窗口大小
  - 同时连接到`connect`命令中提到的应用程序
- 服务器发送`Set Peer Bandwidth`消息到客户端设置带宽
- 客户端处理`Set Peer Bandwidth`消息后，发送`Window Acknowledgement Size`消息到服务器端确认窗口大小
- 服务器发`StreamBegin`消息到客户端，进行开流
- 服务器发送`_result`消息，通知客户端连接的状态

  > 该命令中含有处理 ID（与 1 中收到相同），该消息同时还制定了部分属性，如 Flash Media Server 版本等。
  >
  > 除此之外，它还指定了连接响应相关的信息如 level、code、description、objectencoding 等。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d42312a4ae4e73489e38cdb3f4f29b44.png)





### 建流

- 客户端发送命令消息`createStream`到服务器端
- 服务器端接收到`createStream`后，返回`_result`消息，通知流的状态

![img](https://img-blog.csdn.net/20130915111350062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



### 推送元数据

- 客户端发送`publish`消息到服务器
- 服务器发送`streambegin`，表示流开始了
- 客户端发送`Data Message`提供媒体元数据

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-5be3a3e0cdcc9dd00394bdfddc616099_1440w.webp)



### 推送

```c

      +------------------+                       +---------+
      | Publisher Client |         |             |   FMS   |
      +---------+--------+         |             +----+----+
                |     Handshaking and Application     |
                |            connect done             |
                |                  |                  |
                |                  |                  |
        ---+--- |-- Command Messsage(createStream) -->|
    Create |    |                                     |
    Stream |    |                                     |
        ---+--- |<-------- Command Message -----------|
                |    (_result - command response)     |
                |                                     |
        ---+--- |---- Command Message(publish) ------>|
Publishing |    |                                     |
  metadata |    |<----- UserControl(StreamBegin) -----|
 from file |    |                                     |
           |    |------ Data Message (Metadata) ----->|
                |                                     |
```

- 推送元数据
- 客户端发送`Audio Data`
- 客户端发送`ChunkSize`消息设置块大小
- 推送命令成功的话，服务器发送`_result`告知`publish result`，通知客户端`pushlish`命令执行成功
- 客户端发送视频数据
- 直到流完成





### 播放

```c
     +-------------+                             +----------+
     | Play Client |             |               |  Server  |
     +------+------+             |               +-----+----+
            |        Handshaking and Application       |
            |               connect done               |
            |                    |                     |
            |                    |                     |
   ---+---- |----- Command Message(createStream) ----->|
Create|     |                                          |
Stream|     |                                          |
   ---+---- |<---------- Command Message --------------|
            |    (_result- createStream response)      |
            |                                          |
   ---+---- |------ Command Message (play) ----------->|
      |     |                                          |
      |     |<------------ SetChunkSize ---------------|
      |     |                                          |
      |     |<---- User Control (StreamIsRecorded) ----|
 Play |     |                                          |
      |     |<------ UserControl (StreamBegin) --------|
      |     |                                          |
      |     |<- Command Message(onStatus-play reset) --|
      |     |                                          |
      |     |<- Command Message(onStatus-play start) --|
      |     |                                          |
      |     |<------------ Audio Message --------------|
      |     |                                          |
      |     |<------------ Video Message --------------|
      |     |                     |                    |
                                  |
        Keep receiving audio and video stream till finishes
```

- 客户端发送`play`消息到服务器

- 收到`play`后，服务器发送`SetChunkSize`消息设置块大小

- 服务器发送一些另外一个协议数据 (用户控制)

  - 服务器发送`StreamIsRecorded`，告知客户端流是否被保存，告知客户端流ID
  - 服务器发送`streambegin`，告知客户端流ID

- `play`命令成功的话，服务器发送`onStatus`告知状态 `NetStream.Play.Start `& `NetStream.Play.reset`，通知客户端`play`命令执行成功

  - 在此之后服务器发送客户端要播放的音频和视频数据（`Audio Message / Video Message`）

- 如果流不存在，服务器在`onStatus`中告知状态`NetStream.Play.StreamNotFound`

  

![img](https://img-blog.csdn.net/20130915111446703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 推流

```c

     +--------------------+                     +-----------+
     |  Publisher Client  |        |            |   Server  |
     +----------+---------+        |            +-----+-----+
                |           Handshaking Done          |
                |                  |                  |
                |                  |                  |
       ---+---- |----- Command Message(connect) ----->|
          |     |                                     |
          |     |<----- Window Acknowledge Size ------|
  Connect |     |                                     |
          |     |<------ Set Peer BandWidth ----------|
          |     |                                     |
          |     |------ Window Acknowledge Size ----->|
          |     |                                     |
          |     |<----- User Control(StreamBegin) ----|
          |     |                                     |
       ---+---- |<-------- Command Message -----------|
                |    (_result- connect response)      |
                |                                     |
       ---+---- |--- Command Message(createStream) -->| 
   Create |     |                                     |
   Stream |     |                                     |
       ---+---- |<------- Command Message ------------| 
                |  (_result- createStream response)   |
                |                                     |
       ---+---- |---- Command Message(publish) ------>|
          |     |                                     |
          |     |<----- User Control(StreamBegin) ----|
          |     |                                     |
          |     |------ Data Message (Metadata) ----->|
          |     |                                     |
Publishing|     |------------ Audio Data ------------>| 
  Content |     |                                     |
          |     |------------ SetChunkSize ---------->|
          |     |                                     |
          |     |<--------- Command Message ----------|
          |     |      (_result- publish result)      |
          |     |                                     |
          |     |------------- Video Data ----------->|
          |     |                  |                  |
          |     |                  |                  |
                |    Until the stream is complete     |
                |                  |                  |
```

- 握手
- 连接
- 建流
- 推送



![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/16305798-567406fe7f615e35.png)

### 拉流

- 握手
- 连接
- 建流
- 播放

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3b77b88dda296624e0ab21432a5d3d80.png)



### 换流

```c
    +--------------+                          +-------------+
    | Play2 Client |           |              |    Server   |
    +--------+-----+           |              +------+------+
             |      Handshaking and Application      |
             |            connect done               |
             |                 |                     |
             |                 |                     |
    ---+---- |---- Command Message(createStream) --->|
Create |     |                                       | 
Stream |     |                                       |
    ---+---- |<---- Command Message (_result) -------|
             |                                       |
    ---+---- |------ Command Message (play) -------->|
       |     |                                       |
       |     |<---------- SetChunkSize --------------|
       |     |                                       |
       |     |<--- UserControl (StreamIsRecorded) ---| 
  Play |     |                                       |
       |     |<------ UserControl (StreamBegin) -----|
       |     |                                       |
       |     |<- Command Message(onStatus-playstart)-|
       |     |                                       |
       |     |<---------- Audio Message -------------|
       |     |                                       |
       |     |<---------- Video Message -------------|
       |     |                                       |
             |                                       |
    ---+---- |-------- Command Message(play2) ------>|
       |     |                                       |
       |     |<------- Audio Message (new rate) -----|
 Play2 |     |                                       |
       |     |<------- Video Message (new rate) -----|
       |     |                  |                    |
       |     |                  |                    |
       |  Keep receiving audio and video stream till finishes
                                |
```



![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-ce1c839c7b954d39fb5e0a742bdb269c_1440w.webp)





### 广播

​		在创建和更改共享对象时所交换的消息。它也说明了共享对象消息广播的过程。

```c

             +----------+                       +----------+
             |  Client  |          |            |  Server  |
             +-----+----+          |            +-----+----+
                   |   Handshaking and Application    |
                   |          connect done            |
                   |               |                  |
                   |               |                  |
          ---+---- |---- Shared Object Event(Use) --->|
Create and   |     |                                  |
connect      |     |                                  |
Shared Object|     |                                  |
          ---+---- |<---- Shared Object Event---------|
                   |       (UseSuccess,Clear)         |
                   |                                  |
          ---+---- |------ Shared Object Event ------>|
Shared object|     |         (RequestChange)          | 
Set Property |     |                                  |        
          ---+---- |<------ Shared Object Event ------|
                   |            (Success)             |
                   |                                  |
          ---+---- |------- Shared Object Event ----->| 
Shared object|     |           (SendMessage)          | 
Message      |     |                                  | 
Broadcast    |     |                                  |
          ---+---- |<------- Shared Object Event -----|
                   |           (SendMessage)          |
                   |                |                 |
                                    |                 |
```





![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-274570742bb927a7273b9920acf12eae_1440w.webp)

## 前景

​		IE浏览器和Flash插件在客户端的普及，以及流媒体协议对`rtmp`协议和`flv`视频格式的广泛支持，造就了`RTMP`协议在一个时期内的地位。

​    	IE浏览器衰落和新型浏览器（Chrome,Firefox,Safari等）的崛起，`flash player`被逐渐淘汰。`HTML5 Video`正在成为视频播放的主流技术。

> `HTML5 Video`与`MSE（Media Source Extensions ）`能够提供更加强大的视频播放和扩展应用（如双向视频）等。
>
> `HTML5`的`Video`控件标签支持`HLS`协议播放，经测试确实支持`m3u8`的播放
>
>  `HTML5`视频解码器没有指定，浏览器内置编解码器决定。
>
> 当前各主流浏览器video元素主要支持三种视频格式`ogg`, `mp4`, `webm`。
>
> > MPEG4 = 带有 H.264 视频编码和 AAC 音频编码的 MPEG 4 文件

​    `HLS`、`HTTP-FLV`、`WebRTC`、`WebSocket`等技术传输直播视频流，客户端使用`MSE`扩展实现数据接收和视频播放将更为流行。
