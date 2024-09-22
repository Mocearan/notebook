# RTMP

​		Real Time Message Protocol, 实时消息协议

---

​		`Adobe`公司推出的应用层协议，用来解决多媒体数据传输流的多路复用（Multiplexing）和分包（packetizing）的问题。

​		`RTMP `提供设备的兼容性和流媒体的稳定性，但需要一个特定的 `Flash Media Server/client` 。主要用于第三方流应用程序和较旧的硬件编码器。但随着`Flash Player`推出历史舞台，RTMP协议也逐渐被更现代的协议替代（`webrtc、hls`）。

​		`RTMP`提供了一套全双工的可靠的多路复用消息服务，类似于TCP协议，用来在一对结点之间并行传输带时间戳的音频流，视频流，数据流。（在通讯双方之间传输与时间相关的并行流数据，如音频，视频和数据消息。）

​		通常情况下，不同类型的消息会被分配不同的优先级，当网络传输能力受限时，优先级用来控制消息在网络底层的排队顺序。

> - `RTMP`消息块流不提供任何控制的优先级别和相似形式
>
> - 但是可以用于高层协议提供这样的优先级，如通过消息的逻辑流实现优先级控制
>
> - **最高优先级的控制消息**包括协议控制消息（Protocol Control Messages）和用户控制消息（User Control Messages）
>
>   - 例如：一段实时视频服务会选择丢弃给缓慢的客户的视频信息确保音频信息可以及时被接收
>   - 当网络传输能力受限时，优先级用来控制消息在网络底层的排队顺序。
>
>   
>
> 通常情况下，优先级是这样的：**控制消息 > 音频消息 > 视频消息**。要使得这样的优先级能够得到有效执行，分块也是非常关键的措施：将大消息切割成小块，可以避免大的低优先级的消息（如视频消息）堵塞了发送缓冲从而阻塞了小的高优先级的消息（如音频消息或控制消息）。
>
> `RTMP消息块流`包含它自己的入队协议控制消息，也提供一个高层协议机制用于嵌入用户的控制消息。

## 参考

[[总结\]RTMP流媒体技术零基础学习方法_怎么学习流媒体-CSDN博客](https://blog.csdn.net/leixiaohua1020/article/details/15814587)

[rtmp规范1.0全面指南 - 职场亮哥 - 博客园 (cnblogs.com)](https://www.cnblogs.com/CHLL55/p/13588927.html)

[Introduction · RTMP协议规范 (gitbooks.io)](https://chenlichao.gitbooks.io/rtmp-zh_cn/content/)

[RTMP协议规范1.0中文版_rtmp specification 1.0-CSDN博客](https://blog.csdn.net/noahsun1024/article/details/52177007)





[手撕Rtmp协议_视界音你而不同的博客-CSDN博客](https://blog.csdn.net/mlfcjob/category_10023015.html)

[【流媒体】RTMP协议概述-CSDN博客](https://blog.csdn.net/weixin_42877471/article/details/141017117?spm=1001.2101.3001.6650.3&utm_medium=distribute.pc_relevant.none-task-blog-2~default~YuanLiJiHua~Position-3-141017117-blog-134497867.235^v43^pc_blog_bottom_relevance_base7&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~YuanLiJiHua~Position-3-141017117-blog-134497867.235^v43^pc_blog_bottom_relevance_base7&utm_relevant_index=6)

[RTMP协议和源码解析_rtmpconnect-CSDN博客](https://blog.csdn.net/xiaopangcame/article/details/134497867?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-8-134497867-blog-51248119.235^v43^pc_blog_bottom_relevance_base7&spm=1001.2101.3001.4242.5&utm_relevant_index=11)

[rtmp 协议详解-CSDN博客](https://blog.csdn.net/xiaopangcame/article/details/134754748)



[抛开flash，自己开发实现C++ RTMP直播流播放器 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7324205.html)



[搭建rtmp直播流服务之4:videojs和ckPlayer开源播放器二次开发(播放rtmp、hls直播流及普通视频) - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003678.html)

[H5浏览器播放RTMP直播流 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003697.html)

[videojs集成--播放rtmp流 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003710.html)

[H5浏览器播放RTMP直播流实现切换 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003724.html)

[HTML5视频直播及H5直播扫盲 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9003793.html)

## 优缺点

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




## 生态

​		`RTMP`被`Adobe`开发出来，用于在其`Flash`平台的流媒体系统之间传递数据的。

- 客户端
  - `flash player`
  - `jwplayer`
  - `flowplayer`
  - 自定义
- 服务器端
  - FMS（**Flash Media Server**），Adobe官方的流媒体服务器，收费
  - **Red5：**开源流媒体服务器，基于Java。
  - `SRS`
  - `ZLMediaServer`



## 概念

- 负载：包中所承载的数据。例如音频或视频数据。
- 包：一个数据包由固定头部和所承载的数据组成。一些底层协议可能需要定义数据包的封装格式。
- 端口：在一个计算机中用于区分不同目标的抽象定义。在 TCP/IP 协议中用一个小的正整数来表示端口。OSI 传输层的传输选择器就相当于端口。
- 传输地址：标识一个传输终端的网络地址和端口的组合，例如 IP 地址和 TCP 端口的组合。
- **消息流：允许消息传播的逻辑通道。**
- 消息流 ID：每个消息都会有一个对应的 ID，用于标识其所在的消息流。
- 块：消息的一个片段。消息在传输之前会被分割成更小的片段，因为每一块都很小，以至于可以给不同的块指定各自的优先级，通过这种方式保证多个流中数据可以按照时间戳的顺序传输。
- **块流：块向某一确定方向传播的逻辑通道。可以是客户端到服务端，也可以是服务端到客户端。**
- 块流 ID：每个块都会有一个对应的 ID，用于标识其所在的块流。
- **复用：**将独立的音频 / 视频数据整合为统一的音视频流，可以使多个音视频流同步传输。
- **复用分离：**复用的逆向过程。将合并的音视频数据分离为原始的音频和视频数据。
- 远程过程调用：客户端或服务端调用另一端的功能。
- 元数据：媒体数据的描述信息。
- 应用实例：服务器上可以和 Client 建立连接的应用。
- 动作消息格式：一个可用于序列化 ActionScript 对象图的紧凑的二进制格式。
- 字节序：字节的顺序，即多字节类型的数据在内存中的存放顺序。TCP/IP 各层协议将字节序定义为大端字节序，因此 TCP/IP 协议中使用的字节序通常称之为[网络字节序](https://zhida.zhihu.com/search?q=网络字节序&zhida_source=entity&is_preview=1)。
- 大字节序：高位字节排放在内存的低地址，低位字节排放在内存的高地址。
- 小字节序：低位字节排放在内存的低地址，高位字节排放在内存的高地址。



​		RTMP中：

- 所有整数都是以网络字节序来表示的。
- 数据都是字节对齐的，有填充时用0
- 时间戳是用一个整数来表示的，代表相对于一个起始时间的毫秒数
  - 通常每个流的时间戳都从 0 开始，但这不是必须的，只要通讯双方使用统一的起始时间就可以了
  - 要注意的是，跨流的时间同步（不同主机之间）需要额外的机制来实现
  - 时间戳的长度只有 32 位，所以只能在 50 天内循环（49 天 17 小时 2 分钟 47.296 秒）
    - RTMP 应用在处理时间戳是应该使用连续的数字算法，并且应该支持回环处理
    - 可以假设所有相邻的时间戳间隔不超过 2^31-1 毫秒
    - 在此基础上，10000 在 4000000000 之后，3000000000 在 4000000000 之前
  - 时间戳增量也是以毫秒为单位的无符号整数。时间戳增量可能会是 24 位长度也可能是 32 位长度。
- 块
  - 数据在分割成块时**对传输通道是透明**的，这样音视频数据就能够**合到一个传输流中进行同步传输**，实现了多路复用
    - 这个流称为**块流 Chunk Stream**
    - **在应用层进行多路复用和分包来降低延迟**，确保音视频同步，并提供流畅的播放体验
  - 每个块都有一个唯一的块流 ID，这些块通过网络进行传输
  - 在传输过程中，必须一个块发送完毕之后再发送下一个块
  - 在接收端，将所有块根据块中的块流 ID 组装成消息
    - 分块将上层协议的大消息分割成小的消息
    - 保证大的低优先级消息（比如视频）不阻塞小的高优先级消息（比如音频或控制消息）
  - 降低消息发送的开销，它在块头中包含了压缩的原本需要在消息中所包含的信息
  - 块大小是可配置的，这个可以通过一个设置块大小控制消息进行设定修改。
    - 越大的块 CPU 使用率越低，但是在低带宽的情况下，大的写入会阻塞其他内容的写入
    - 而小一些的块不适合高比特率的流
- 块流为上层流媒体协议提供复用和分包的功能
  - 一个或多个块流可能会复用同一个连接
  - 每个块流承载来自同一个消息流的同一类消息
  - 每个消息包含时间戳和负载类型信息
  - RTMP 块流和 RTMP 协议组合可以适用于多种音视频应用，从一对一或一对多直播到视频会议都能很好的满足
  - 当使用可靠传输协议（如 TCP）时，RTMP 块流为所有消息提供了可靠的跨流端对端按时间戳顺序发送的机制
  - RTMP 块流不提供优先级控制，但是可以由上层协议提供这样的优先级
    - 当某个客户端网络比较慢时，可能会选择抛弃一些视频消息来保证声音消息能够及时接收
  - 块流除自身内置的协议控制消息外，还为上层协议提供了用户控制消息的机制



## 结构



### 简单握手包结构

#### `c0 / s0`

​		单独的一个字节

```c
// C0 and S0 bits
+-+-+-+-+-+-+-+-+ 
| 	version 	| 
+-+-+-+-+-+-+-+-+ 
```

- ##### 版本(8比特)

  - 在C0包内，这个字段代表客户端请求的RTMP版本号
  - 在S0包内，这个字段代表服务端选择的RTMP版本号
  - 版本0-2用在早期的产品中，现在已经被弃用
  - 服务器无法识别客户端的版本号，应该回复版本3
    - 客户端可以选择降低到版本3，或者中止握手过程。 
  - 版本4-31被预留用于后续产品
  - 版本32-255不允许使用
    - 为了区分RTMP协议和文本协议，文本协议通常以可打印字符开始

#### `c1 / s1`

​		固定为 1536 字节

```c
// C1 and S1 bits
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

- 共1526 Byte

- 时间戳(4字节)
  - 该时间戳应该作为发送端点所有后续块的时间戳起始时间
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





### 传输块 chunk

​		在网络环境上传输时，`Message`被拆分成较小的数据块。实际的数据被分割成固定大小的数据块。必须在一个`Chunk`发送完成后才能开始发送下一个`Chunk`。

> - 避免优先级低的消息持续发送阻塞优先级高的数据
> - 对于数据量较小的Message，可以通过对Chunk Header的字段来压缩信息，从而减少信息的传输量

- 默认是128字节，最后一个数据块可以小于该固定长度

  - `Set Chunk Size`可以设置Chunk的最大值

  - 发送端和接受端会各自维护一个`Chunk Size`，可以分别设置这个值来改变自己这一方发送的`Chunk`的最大大小

  - 在实际发送时应对要发送的数据用不同的Chunk Size去尝试

    - 通过抓包分析等手段得出合适的Chunk大小
    - 在传输过程中可以根据当前的带宽信息和实际信息的大小动态调整Chunk的大小
    - 尽量提高CPU的利用率并减少信息的阻塞机率

    > 大一点的Chunk减少了计算每个chunk的时间从而减少了CPU的占用率，但是它会占用更多的时间在发送上，尤其是在低带宽的网络情况下，很可能会阻塞后面更重要信息的传输。
    >
    > 小一点的Chunk可以减少这种阻塞问题，但小的Chunk会引入过多额外的信息（Chunk中的Header），少量多次的传输也可能会造成发送的间断导致不能充分利用高带宽的优势，因此并不适合在高比特率的流中传输。

- `chunk header + chunk data`

  - `chunk data`：上限为配置的最大块大小

- `chunk header = chunk basic header + chunk message header + extended timestamp`

  - `chunk basic header`：1-3 字节
  - `chunk messge header`：`0 / 3 / 7 / 11`字节
  - `extended timestamp`：`0 / 4`字节
  
- 载荷为`h264`数据时，要是用AMF3进行编码




![img](https://img-blog.csdn.net/20130914205506281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20130914205513812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



#### `Chunk Basic Header` 基础头

​		`Chunk Basic Header`的长度至少1个字节，可能是1，2，或3个字节。小端存储.

- `chunk type(fmt)`，`2 bit`，决定了后面`Chunk Message Header`的编码格式和长度

- `chunk stream id(CSID)`，可变长字段，唯一表示一个特定的传输流通道
  - 表示一个音频流或视频流等
  
    - 视频流和音频流等对应的`CSID`相同，则为多路复用
  
  - RTMP协议支持用户自定义`［3，65599］`之间的`CSID`
  
    - 0，1，2不用作ID，用于参与对`chunk basic header`后续长度的计算，见下
  
  - 判断`chunk type`所在字节的后`6 bit`
  
    - `> 1`，`chunk basic header` 1字节，此`6 bit`即`CSID`，取值`[3, 63]`
      - 如果 `== 2 (00 0010)`， 这个RTMP包是一个`Protocol Control Messag`，用户定义包
  
    - `== 0 (00 0000)` ， ` chunk basic header`  2 字节，此`6 bit`后随的`8 bit`为`CSID - 64`
      - 计算时要在原值上 加64
      - `CSID `在［0+64，255+64］之间，`[0000 0000, 1111 1111] + 64`
    - `== 1 (00 0001)`，`chunk basic header` 3字节，此`6 bit`后随的`16 bit`为`CSID - 64`
      - 计算时要在原值上 加64
      - `CSID `在［0+64，65535+64］之间，`[0000 0000 0000 0000, 1111 1111 1111 1111] + 64`

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

​		标识`chunk`负载所属消息，包含了将要发送的`Message`的信息。长度由`chunk basic header`中的`chunk type`决定。

- `fmt == 00`，11 bytes，能表示其他三种数据，但Chunk stream的开始第一个chunk和头信息中时间戳后退的时候必须采用这种格式

  - `timestamp`，3 bytes，消息的绝对时间戳，三个字节都为`1`时，实际的时间戳转存到`Extended Timestamp`
  - `message length`，3 bytes，实际发送的消息的数据如音频、视频帧等数据的长度，单位时字节。
    - 是指整个Message的长度，而不是单个chunk的负载长度
  - `Message type id`，1 bytes，表示实际发送的数据的类型，如8代表音频数据，9代表视频数据。
  - `message stream id`，4个字节，用于区分不同流的消息
    - 采用小端存储方式

- `fmt == 01`，7bytes，去掉message stream id，表示此chunk和上一次发的chunk所在的流相同

  - 如果在发送端和对端有一个流链接的时候可以尽量采用这种格式
  - `timestamp delta`，3 bytes，和type=0时不同，存储的是和上一个chunk的时间差
    - 三个字节都为`1`时，实际的时间戳转存到`Extended Timestamp`
  - 其他字段与上面的解释相同

- `fmt == 10`，3 bytes，只保留`timestamp`，表示此 chunk和上一次发送的 chunk 所在的流、消息的长度和消息的类型都相同

  - `timestamp delta`，同上

- `fmt == 11`，0 bytes，表示这个chunk的Message Header和上一个是完全相同的

  - 在`type == 0`的`chunk`后时，表示和前一个 `chunk `的时间戳都是相同

    - 就是一个 `Message `拆分成多个 `chunk`，这个 `chunk `和上一个 `chunk `同属于一个 `Message`

  - 在`type == 1 / 2`的`chunk`后时，和前一个 `chunk`的时间戳的差是相同的

    > 第一个 chunk 的 type = 0，timestamp = 100
    >
    > 第二个 chunk 的 type = 2，timestamp delta = 20，则 timestamp = 100 + 20
    >
    > 第三个 chunk 的 type = 3，则 timestamp delta = 20，timestamp = 120 + 20

#### `Extended Timestamp` 拓展时间戳

​		取决于块消息头中编码的时间戳，当时间戳溢出时才出现。

​		扩展时间戳占 4 个字节，能表示的最大数值就是 0xFFFFFFF。

​		当`chunk message header`的时间戳等于`0xffffff`的时候`chunk message header`后面的四个字节就代表扩展时间。



###  消息 Message

​		RTMP传输媒体数据的过程中

- 发送端
  - 媒体数据封装成消息
  - 消息分割成消息块
  - 消息块通过TCP协议发送出去
- 接收端
  - 通过TCP协议收到数据
  - 消息块重新组合成消息
  - 对消息进行解封装处理就可以恢复出媒体数据



​		`Message Header + Message Body`。

- `message body`为`message type id`描述的各类消息，或音视频数据
  - 各类消息的结构建消息类型

![img](https://img-blog.csdn.net/20130914205548375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- `Message Type ID`，`1 byte`，代表不同的消息类型
  - 1-7的消息用于协议控制
    - RTMP协议自身管理要使用的消息，用户一般情况下无需操作其中的数据
  - 为8，9的消息分别用于传输音频和视频数据
  - 15-20的消息用于发送AMF编码的命令，负责用户与服务器之间的交互，比如播放，暂停等等
- `Payload Length`，`1 byte`，消息负载的有效长度，该字段是用大端序表示的。
- `Timestamp`，`4 byte`，消息的时间戳，该字段是用大端序表示的。
- `message Stream ID`，`3 byte`，可以是任意值，该字段是用大端序表示的
  - 当同一个`Message`被复用到不同的消息流中时，可以通过`message stream id`来区分它们
  - 划分成`Chunk`和还原`Chunk`为`Message`的时候都是根据这个ID来辨识是否是同一个消息的`Chunk`的
  - 和`chunk stream id`不同，`chunk stream id`描述一个传输流，`message stream id`通常与特定的流（如视频流或音频流）相关联。





## 消息类型

​		消息主要分为三类: 协议控制消息、数据消息、命令消息等。

- 协议控制消息，主要用于协议内的控制，两大类
  - 协议控制消息：Message Type ID = 1 2 3 5 6 
    - 必须使用0作为消息流ID(这些流也被称为控制流)，同时使用2作为块流ID
    - 协议控制消息接收立即生效；解析时，时间戳字段被忽略
  - 用户控制消息：Message Type ID = 4 
- 数据消息，用于音视频元数据
  - Message Type ID = 15 18
  - 15 / 18: Metadata 包括音视频编码、视频宽高等信息
    - AMF0编码时，Message Type ID＝18，AMF3编码时Message Type ID＝15.
- Audio 音频数据
  - Message Type ID = 8

- Video 视频数据
  - Message Type ID = 9

- 共享消息，表示一个Flash类型的对象，由键值对的集合组成，用于多客户端，多实例时使用
  - 当信息使用AMF0编码时，Message Type ID＝19，AMF3编码时Message Type ID＝16.

- 命令消息，主要用于发送`AMF0/AMF3`编码的命令，负责用户与服务器之间操作命令的交互
  - Message Type ID = 17 或 20
    - AMF0编码时，Message Type ID＝20，AMF3编码时Message Type ID＝17.

  - 比如播放、暂停等
  - 主要有NetConnection 和 NetStream两个类
  - 两个类分别有多个函数，该消息的调用，可理解为远程函数调用

- 聚合消息：多个RTMP子消息的集合
  - Message Type ID＝22

- 用户控制消息：告知对方执行该信息中包含的用户控制事件
  - Message Type ID=4
  - 和前面提到的协议控制信息（Protocol Control Message）不同，这是在RTMP协议层的，而不是在RTMP chunk流协议层的


> **Action Message Format (AMF)**
>
> ​		一种紧凑的二进制格式，用于序列化`ActionScript object graphs`。 可以透过 `AMF over HTTP`的方式将`flash`端资料编码后传回server，server端的`remoting adaptor`接收到资料后则会译码回正确的`native`对象，交给正确的程序处理。
>
> ​		AMF 包 会有一个 trans id 事务ID，然后服务器会返回一个 _result 带着客户端的 trans id ，表示执行成功或失败。AMF 大部分都是远程过程调用。



### 协议控制消息

#### `Set Chunk Size(1)`

​		通知对端己方发送的`Chunk`分片大小。默认Chunk分片大小为128字节。

```c
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|0|				 chunk size (32 bits) 						 | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- 首位必须为`0`
- 以字节为单位的`31 bit`块大小值
  - 有效值为1到 2147483647(0x7FFFFFFF，1和2147483647都可取)
  -  但是所有大于16777215(0xFFFFFF) 值是等价的，因为没有一个块比一整个消息大，并且没有一个消息会大于16777215字节
- 通常应该不低于 128 字节。每个方向上的最大块大小是独立的。

#### `Abort Message(2)`

​		通知对端，如果正在等待一条消息的部分块(已经接收了一部分)，那么可以丢弃之前已经接收到的块。

> ​		当一个Message被切分为多个chunk，接受端只接收到了部分chunk时，发送该控制消息表示发送端不再传输同Message的chunk，接受端接收到这个消息后要丢弃这些不完整的chunk。

- 应用可能在关闭的时候发送该消息，用来表明后面的消息没有必要继续处理了。

```c
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|						chunk stream id						 | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- chunk stream ID (块流 ID，32 位)：块流 ID，丢弃该CSID的所有已接收到的chunk。

#### `Acknowledgement(3)`

​		确认本端接收到的字节数。

- 在会话开始时，或上一次`Ackowledgement(3)`消息之后接收到了等于窗口大小的数据，必须发送一个`Ackowledgement(3)`

- 窗口大小是指收到`Acknowledgement(3)`之前最多可以发送的字节数
  - 双方在会话开始时互相发送`Window Acknowledgement size(5)`来通知对方，己方会在发送若干字节后停止发送等待确认对方收到的字节数
  - 这个确认消息即`Acknowledgement(3)`，发送方收到两次`Acknowledgement(3)`之间能够发送的最大字节数就是窗口大小
  - 不包括tcp包头，应该是chunk的大小，即从tcp 的recv函数中获得的内容大小

```c
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|					sequence number							 | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- sequence number (序列号，32 位)：自上一个`Acknowledgement(3)`之后收到的字节数

#### `Window Acknowledgement Size(5)`

​		通知对方本端在两个`Acknowledgement(3)`间能发送的最大字节数。

- 会话开始时，双方都要先对端发送`Window Acknowledgement Size`，用于指明期望获得确认的大小，即己方要进入发送等待的大小
- 发送等于窗口大小的数据之后，就会等待对端的`Ackowledgement(3)`，确认对方已经收到的数据后，才能再次发送

> TCP 本身有滑动窗口跟ACK。所以一般 `Window Acknowledgement Size` 会设置得非常大。

```c
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|			Acknowledgement Window Size						 | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```



#### `Set Peer Bandwidth(6)`

​		限制对端的输出带宽。

- 限制对端后续两次`Ackowledgement(3)`间能发送的数据量
- `Window Acknowledgement Size(5)`是通知对端，己方会在发送若干字节后停发等待`Ackowledgement(3)`，这是控制己方的输出带宽
  - 但是己方的收包处理速度不一定能够适配对方的发送速度，因此，要根据己方接收处理包的速度，来要求对方以合适窗口大小发送，否则可能会造成对端长时间等待
  - 因此，一方接收到一条新的`Set Peer Bandwidth(6)`，可能意味着己方要调整两个`Acknowledgement(3)`之间发送的最大字节数，其值发生变化时，应该再次发送`Window Acknowledgement Size(5)`通知对端，己方进入发送等待的大小


```c
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|			Acknowledgement Window Size						 | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Limit Type  |
+++++++++++++++
```

​		限制类型取以下列值之一：

- `0 - Hard`：限制输出带宽不超过指定的窗口大小。
- `1 - Soft`：限制输出带宽不超过指定的窗口大小，或者已经有限制在起作用的话，就取两个窗口大小之间的较小值。
- `2 - Dynamic`：如果先前的限制类型为 Hard，则这条消息的也被视为Hard消息，否则的话忽略这条消息。



### 用户控制消息

#### `User Control Message(4)`

​		用户控制消息。这些消息包含RTMP流传输层所使用的信息。

- `chunk stream id == 2`
- `message stream id == 0`

​		两个字节的event type，后面跟可变长度的event data：

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

####  `command message(17/20)`

​		命令消息发送的是客户端与服务端之间的 AMF 编码指令，客户端或服务端也可以通过指令消息来实现远程过程调用（RPC)。

​		20表示使用AMF0编码，17表示使用AMF3编码。

- `Command Name`，发送端发送时会带有命令的名字，表示要执行什么事件
  - connect, createStream, publish, play, pause ，响应消息使用onstatus，result
- `Transaction ID`，表示此次命令的标识
  - 远程过程调用的事务ID
- `Command Object`，命令参数
- `Optional User Arguments`，用户自定义的额外信息

​		接受端收到命令后，会返回以下三种消息中的一种：

- `_result `消息表示接受该命令，对端可以继续往下执行流程
- `_error`消息代表拒绝该命令要执行的操作
- `method name`消息代表要在之前命令的发送端执行的函数名称
- 都要带有收到的命令消息中的`TransactionId`，表示本次的回应作用于哪个命令

​		命令消息分为两大类：`netConnection`和`netStream`命令

- `netConnection`，服务器和客户端之间连接的高层表现对象
- `netStream`：音频流、视频流及其他相关数据传输流，我们会发送如播放、暂停等指令来控制数据流动。
- 多个`netstream`可以共用一个`netConnection`



#### `NetConnection Commands` 连接层命令

​		管理双端之间的连接状态，同时也提供了异步远程方法调用（RPC）在对端执行某方法。

##### `connect `

​		请求连接到服务器的应用实例。

- `Command Name`，`connect`
- `Transaction ID`，1
- `Command Object`，键值对集合表示的命令参数
  - 参考协议的官方文档
- `Optional User Arguments`，用户自定义的额外信息

​		响应：

- `Command Name`，`_result`表示接受连接，`_error`表示连接失败
- `Transaction ID`，1
- `Properties`，该连接的描述属性的键值对
  - `fmsver` Flash Media Server 版本等
- `Information`，来自于服务器的描述应答的键值对
  - `code / level / description /object-encoding`等



​		Connect 指令中会用到的键值对：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-95d3b2437a138e78816e8838a2647d9f_1440w.webp)

音频编码：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-741179552566cf4f0bd3001e1ac11921_1440w.webp)

视频编码：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-da9c9dcf06b7d9c7a607fb34ec82f502_1440w.webp)

视频功能：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-8b04e8fa4dd9c6237cf2a6bae511772f_1440w.webp)

对象编码：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-9dcd369208f2b024862f7044a03b3d7b_1440w.webp)



##### `call`

​		请求一个RPC远程调用。在对端执行某进程。

- `Procedure Name`，要调用的进程名称
- `Transaction ID`，如果想要对端响应的话置为非0值，否则置为0
- `Command Object`，命令参数
- `Optional User Arguments`，用户自定义的额外信息

​		响应结构为一个正常的命令结构，表示其调用了一个命令：

- `Command Name`，调用的命令名称
- `Transaction ID`，同`call`中的`Transaction ID`
- `Command Object`，命令参数，没有为`null`
- `Response`，调用该方法的响应

##### `creatStream`

​		创建一个逻辑控制通道，用于传递具体消息，音频，视频，描述数据。传输信息单元为Chunk。

> NetConnection 是默认的通讯通道，流 ID 为 0。协议和一些指令消息，包括 createStream，使用默认通讯通道。

- `Command Name`，`createStream`
- `Transaction ID`，命令的会话ID
- `Command Object`，命令参数
- `Optional User Arguments`，用户自定义的额外信息

​		响应：

- `Command Name`，`_result`表示接受连接，`_error`表示连接失败
- `Transaction ID`，命令的ID
- `Command Object`，如果存在任何命令信息则被设置，否则为`null`
- `Stream ID`，要么是一个`Stream ID`，要么是一个错误信息对象





#### `NetStream Commands` 控制流上的命令

​		基于 NetConnection 的客户端至服务器间连接，NetStream 定义了一条可以传递音频流、视频流以及消息流的通道。

​		在传输层协议之上只能连接一个`NetConnection`，但一个`NetConnection`可以建立多个`NetStream`来建立不同的流通道传输多个数据流。

​		服务端收到命令后会通过`onStatus`的命令来响应客户端，表示当前`NetStream`的状态



##### `onStatus`

- `Command Name`，`onStatus`
- `Transaction ID`，0
- `Command Object`，`null`
- `Info Object`，AMF类型的Object
  - `level : warning / status / error` 消息的级别
  - `code`：代表具体状态的关键字，如`NetStream.Play.Start`
  - `description`：可读的消息描述
  - 除了这三种必要信息，用户还可以自己增加自定义的键值对

##### `play`

​		由客户端向服务器发起请求从服务器端接受数据，多次调用该指令也可创建一个播放清单。

​		如果传输的信息是视频的话就是请求开始播流。

> 如果你希望创建一个在不同 live 或 recorded 流间切换的动态播放清单，需要多次调用 play 并传递 false 以避免每次 reset。
>
> 相反地，如果你希望立即播放某一指定流，传递 true 以清除等待播放队列中的所有其他流。

- `Command Name`，`play`
- `Transaction ID`，0
- `Command Object`，`null`
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



##### `play2`

​		可以将当前正在播放的流切换到同样数据但不同比特率的流上，服务器端会维护多种比特率的文件来供客户端使用play2命令来切换。

​		切换到不同的比特率的流上，而不用更改播放时间。

​		这个消息是在paly之后再发送。

- `Command Name`，`play2`
- `Transaction ID`，0
- `Command Object`，`null`
- `parameters`，AMF编码的Flash对象，包括了一些用于描述`flash.net.NetstreamPlayOptions ActionScript obejct`的参数



##### `deleteStream`

​		删除流。用于客户端告知服务器端本地的某个流对象已被删除，不需要再传输此路流。

- `Command Name`，`deleteStream`
- `Transaction ID`，0
- `Command Object`，`null`
- `Stream ID`， 本地已删除，不再需要服务器传输的流的ID

​		服务器不需要发送任何应答。

##### `receiveAudio`

​		通知服务器端该客户端是否需要音频。

- `Command Name`，`receiveAudio`

- `Transaction ID`，0

- `Command Object`，`null`

- `Bool Flag`，是否接受音频
  - 如果为`false`，服务器不用回复
  
  - 如果为`true`，服务器在`onStatus`这个中回复两个状态:
    - `NetStream.Seek.Notify`
  
    - `NetStream.Play.Start`
  



##### `receiveVideo`

​		通知服务器端该客户端是否需要视频。

- `Command Name`，`receiveAudio`

- `Transaction ID`，0

- `Command Object`，`null`

- `Bool Flag`，是否接受视频
  - 如果为`false`，服务器不用回复
  
  - 如果为`true`，服务器在`onStatus`这个中回复两个状态:
    - `NetStream.Seek.Notify`
  
    - `NetStream.Play.Start`
  



##### `publish`

​		由客户端向服务器发起请求推流到服务器。需要`onStatus`响应。

​		客户端发送 publish 指令将已命名的流发布到服务器上。使用这个名称，任何客户端都可以播放此流，并接收已发布的音频、视频和数据消息。

- `Command Name`，`publish`
- `Transaction ID`，0
- `Command Object`，`null`
- `Publishing Name`，流名称
- `Publishing Type`，"live"、"record"、"append"中的一种
  - `live`，表示该推流文件不会在服务器端存储
  - `record`，表示该推流的文件会在服务器应用程序的子目录下保存以便后续播放
    - 如果文件不存在的话就会建立一个新文件写入
    - 如果文件已经存在的话删除原来所有的内容重新写入
  - `append`，也会将推流数据保存在服务器端
    - 如果文件不存在的话就会建立一个新文件写入
    - 如果对应该流的文件已经存在的话保存原来的数据，在文件末尾接着写入



##### `seek`

​		定位到视频或音频的某个位置。毫秒为单位。

- `Command Name`，`seek`
- `Transaction ID`，0
- `Command Object`，`null`
- `milliSeconds`，定位到该文件的xx毫秒处
- 当定位成功，服务器发送 NetStream.Seek.Notify 的状态消息。失败的时候，它返回一个_error 的消息。



##### `pause`

​		暂停。客户端告知服务端停止或恢复播放。

- `Command Name`，`pause`
- `Transaction ID`，0
- `Command Object`，`null`
- `Pause/Unpause Flag`，`true`表示暂停，`false`表示恢复
  - `true`，客户端请求暂停，服务端暂停对应的流会返回`NetStream.Pause.Notify`的`onStatus`命令来告知客户端当前流暂停
  - `false`，客户端请求恢复，服务端会返回`NetStream.Unpause.Notify`的`onStatus`命令来告知客户端当前流恢复
- `milliSeconds`，暂停或者恢复的时间，以毫秒为单位
- 响应失败，返回`_error`信息





### 数据消息

#### `Data Message(15/18)`

​		用于传输`Metadata`或用户数据。18表示AMF0，15是AMF3。

#### `Audio Message (8)`

​		音频消息

#### `Video Message (9)`

​		视频消息

 

​		

### 共享对象消息

#### `Shared Object Message(16/19)`

​		在多个客户端之间同步的 Flash 对象 (键值对集合)。19表示AMF0，16是AMF3。

​		个消息都可以包含多个事件。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-14f035272c2e3ad991ec825c587d1b91_1440w.webp)

支持以下事件类型：

| 事件                 | 描述                                                         |
| -------------------- | ------------------------------------------------------------ |
| Use(=1)              | 客户端发送这一事件以通知服务器一个命名共享对象被创建。       |
| Release(=2)          | 当共享对象在客户端被删除时客户端发送这一事件到服务器。       |
| Request Change (=3)  | 客户端发送给服务器这一事件以请求共享对象命名参数值的改变。   |
| Change (=4)          | 服务器发送这一事件已通知发起这一请求之外的所有客户端，一个命名参数值的改变。 |
| Success (=5)         | 如果请求被接受，服务器发送这一事件给请求的客户端，以作为RequestChange事件的响应。 |
| SendMessage (=6)     | 客户端发送这一事件到服务器以广播一条消息。一旦接收到这一事件，服务器端将会给所有的客户端广播这一消息，包括这一消息的发起者。 |
| Status (=7)          | 服务器端发送这一事件以通知客户端异常情况。                   |
| Clear (=8)           | 服务器端发送这一消息到客户端以清理一个共享对象。服务器端也会对客户端发送的Use事件使用这一事件进行响应。 |
| Remove (=9)          | 服务器端发送这一事件通知客户端删除一个slot。                 |
| Request Remove (=10) | 客户端发送这一事件通知客户端删除一个slot。                   |
| Use Success (=11)    | 服务器端发送给客户端这一事件表示连接成功。                   |





### 聚合消息

#### `Aggregate Message(22)`

​		是一个消息包含多个子 RTMP 消息，子消息符合 RTMP 消息格式。

- 组合消息的消息流 ID 会覆盖其中子消息的消息流 ID。
- 组合消息的时间戳和其中第一个子消息的时间戳的差值，是用来将所有子消息的时间戳重整为流时间的位移量。
  - 位移量会加到每一个子消息的时间戳上来换算出正常的流时间。
  - 第一个子消息的时间戳应该与组合消息的时间戳相同，所以位移量应该为 0。
- Back Pointer (反向指针) 包含前一个消息的长度（包括消息头），这样符合 flv 文件格式，可用于进行后退操作。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-95aff3d3e08567cf0be7baf7bd7734a8_1440w.webp)

使用组合消息有以下好处：

- 块流协议中，一个块最多只能发送一个消息，这样就使用组合消息，加大块大小，从而降低发送的块数量。
- 子消息在内存中连续存放，这样系统调用网络发送数据的性能更高。

## AMF

​		动作消息格式。Message Type ID为15-20的消息用于发送AMF编码的命令。

​	对个数据的合集，数据的边界通过不同的类型，以及不同类型的长度来区分。

第一个字节是数据类型，然后根据数据类型确定数据的长度。其中，string是可变长度，两个字节长度值。
如果是object，则object内部是个字典，key是字符串，value是数据，这里还要对数据根据类型对数据解析，同上

### 数据类型

​		*AMF数据类型： 

- *Type      Byte code  
- *Number    0x00 
-  *Boolean   0x01 
-  *String    0x02 
-  *Object    0x03 
-  *MovieClip 0x04 
-  *Null      0x05 
-  *Undefined 0x06 
-  *Reference 0x07 
-  *MixedArray    0x08 
-  *EndOfObject   0x09 
-  *Array         0x0a 
-  *Date          0x0b 
-  *LongString    0x0c 
-  *Unsupported   0x0d 
-  *Recordset     0x0e 
-  *XML           0x0f 
-  *TypedObject (Class instance)  0x10 
-  *AMF3 data 0×11 



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

- 客户端发送消息`connect`到服务器，请求服务建立连接
- 服务器接收到连接`connect`消息后，发送`Window Acknowledgement Size`消息到客户端确认窗口大小
  - 同时连接到`connect`命令中提到的应用程序
- 服务器发送`Set Peer Bandwidth`消息到客户端设置带宽
- 客户端处理`Set Peer Bandwidth`消息后，发送`Window Acknowledgement Size`消息到服务器端确认窗口大小
- 服务器发`StreamBegin`消息到客户端，进行开流
- 服务器发送`_result`消息，通知客户端连接的状态

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

- 推送元数据
- 客户端发送`Audio Data`
- 客户端发送`ChunkSize`消息设置块大小
- 推送命令成功的话，服务器发送`_result`告知`publish result`，通知客户端`pushlish`命令执行成功
- 客户端发送视频数据
- 直到流完成





### 播放

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



![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-ce1c839c7b954d39fb5e0a742bdb269c_1440w.webp)





### 广播

​		在创建和更改共享对象时所交换的消息。它也说明了共享对象消息广播的过程。



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
