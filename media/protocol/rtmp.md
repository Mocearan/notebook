# RTMP

​		Real Time Message Protocol, 实时消息协议

---

​		`Adobe`公司推出的应用层协议，用来解决多媒体数据传输流的多路复用（Multiplexing）和分包（packetizing）的问题。

​		`RTMP `提供设备的兼容性和流媒体的稳定性，但需要一个特定的 `Flash Media Server/client` 。主要用于第三方流应用程序和较旧的硬件编码器。但随着`Flash Player`推出历史舞台，RTMP协议也逐渐被更现代的协议替代（`webrtc、hls`）。

​		`RTMP`提供了一套全双工的可靠的多路复用消息服务，类似于TCP协议，用来在一对结点之间并行传输带时间戳的音频流，视频流，数据流。通常情况下，不同类型的消息会被分配不同的优先级，当网络传输能力受限时，优先级用来控制消息在网络底层的排队顺序。

> `RTMP`消息块流不提供任何控制的优先级别和相似形式，但是可以用于高层协议提供这样的优先级，例如：一段实时视频服务会选择丢弃给缓慢的客户的视频信息确保音频信息可以及时被接收。
>
> `RTMP消息块流`包含它自己的入队协议控制消息，也提供一个高层协议机制用于嵌入用户的控制消息。

​		优点：在`c/s`之间保持稳定的连接

- 无论用户的互联网连接质量如何，它都可以无缝低延迟进行流媒体传输
- 通过将数据流分成大小相等的包，顺序传输到端后组包成媒体流
  - 音频数据默认为 64 字节，视频数据默认为 128 字节

​		缺点：

- 与 `HTML5 `播放器不兼容
  - `RTMP`属于`flash`技术生态
    - 必须使用`flash player`作为`client player`
    - 必须使用`FLV`封装格式
- RTMP 容易受到带宽问题的影响



## 参考

[[总结\]RTMP流媒体技术零基础学习方法_怎么学习流媒体-CSDN博客](https://blog.csdn.net/leixiaohua1020/article/details/15814587)

[rtmp规范1.0全面指南 - 职场亮哥 - 博客园 (cnblogs.com)](https://www.cnblogs.com/CHLL55/p/13588927.html)

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





## 流程

​		播放一个RTMP协议的流一般经过以下三步：握手（Handshake），建立连接（connect），建立流（CreateStream），播放（play）。

- `RTMP Connection`代表`c/s`之间基础连接关系，`c/s`之间只能有一个`RTMP Connection`
- `RTMP Stream`代表收发流媒体的一个通道，在一个`RTMP Connection`上可以有多个`RTMP Stream`

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

  - 实际传输为了更好地实现多路复用、分包和信息的公平性，发送端会把Message划分为带有`Message ID`的`Chunk`
    - 每个`Chunk`可能是一个单独的Message，也可能是Message的一部分
  - 接受端根据`chunk`中包含的data的长度，message id和message的长度把chunk还原成完整的`Message`

  > Message是协议逻辑分片，chunk是传输物理分片

  





### 握手

- 客户端发送C0、C1块。服务器收到C0或C1后发送S0和S1。
- 客户端收齐S0和S1后，开始发送C2。当服务器收齐C0和C1后，开始发送S2。
- 客户端和服务器分别收到S2和C2后，握手完成。

![img](https://img-blog.csdnimg.cn/img_convert/f29857b212e51e32cbc94e95c9a2435c.png)



#### 握手状态

- 未初始化
  - 协议版本被发送，客户和服务端都是未初始化的
  - 客户端在包c0中发送协议版本，然后发送c1
    - 服务端支持这个版本，它将会发送s0和s1作为响应
    - 服务端不支持，则会用相应的动作来响应，在RTMP中这个动作是结束这个连接
- 版本发送完成
  - 客户端和服务端在未初始化状态之后都进入到版本发送完成状态
  - 客户端等待包s1,而服务端等待包c1
  - 在收到相应的包后,客户端发送包c2,而服务端发磅包s2,状态变成询问发送完成
- 询问发送完成
  - 客户端和服务端等待s2和c2
- 握手完成
  - 客户端和服务端开始交换消息



###  建立连接

- 客户端发送命令消息中的“连接”(connect)到服务器，请求服务建立连接。
- 服务器接收到连接命令消息后，发送确认窗口大小(`Window Acknowledgement Size`)协议消息到客户端
  - 同时连接到连接命令中提到的应用程序
- 服务器发送设置带宽(Set Peer Bandwidth)协议消息到客户端
- 客户端处理设置带宽协议消息后，发送确认窗口大小(`Window Acknowledgement Size`)协议消息到服务器端
- 服务器发送用户控制消息中的“流开始”(`Stream Begin`)消息到客户端
- 服务器发送命令消息中的“结果”(_result)，通知客户端连接的状态

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d42312a4ae4e73489e38cdb3f4f29b44.png)

### 创建流

- 客户端发送命令消息中的“创建流”（createStream）命令到服务器端
- 服务器端接收到“创建流”命令后，发送命令消息中的“结果”(_result)，通知客户端流的状态

![img](https://img-blog.csdn.net/20130915111350062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3b77b88dda296624e0ab21432a5d3d80.png)

### 播放

- 客户端发送命令消息中的“播放”（play）命令到服务器
- 接收到播放命令后，服务器发送设置块大小（ChunkSize）协议消息
- 服务器发送用户控制消息中的“streambegin”，告知客户端流ID
- 播放命令成功的话，服务器发送命令消息中的“响应状态” `NetStream.Play.Start `& `NetStream.Play.reset`，告知客户端“播放”命令执行成功
- 在此之后服务器发送客户端要播放的音频和视频数据

![img](https://img-blog.csdn.net/20130915111446703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 结构

​		RTMP传输媒体数据的过程中

- 发送端
  - 媒体数据封装成消息
  - 消息分割成消息块
  - 消息块通过TCP协议发送出去
- 接收端
  - 通过TCP协议收到数据
  - 消息块重新组合成消息
  - 对消息进行解封装处理就可以恢复出媒体数据

### 消息 Message

​		`Message Header + Message Body`，消息包使用二进制数据流，通过AMF0/AMF3进行编码.

> **Action Message Format (AMF)**
>
> 一种紧凑的二进制格式，用于序列化`ActionScript object graphs`。 可以透过 `AMF overHTTP`的方式将`flash`端资料编码后传回server，server端的`remoting adaptor`接收到资料后则会译码回正确的`native`对象，交给正确的程序处理。

- Message Header：
  - 标志消息类型的Message Type ID
  - 标志消息长度的Payload Length
  - 标识时间戳的Timestamp
  - 标识消息所属媒体流的Stream ID

![img](https://img-blog.csdn.net/20130914205548375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- Message Type ID，代表不同的功能
  - 1-7的消息用于协议控制
    - RTMP协议自身管理要使用的消息，用户一般情况下无需操作其中的数据
  - 为8，9的消息分别用于传输音频和视频数据
  - 15-20的消息用于发送AMF编码的命令，负责用户与服务器之间的交互，比如播放，暂停等等
- Timestamp，消息的时间戳
- Payload Length，消息的有效长度
- Stream ID，可以是任意值。当同一个`Message`被复用到不同的消息流中时，可以通过消息流ID来区分它们。

### 传输块 chunk

​		`Message`被拆分成较小的数据块适合在网络环境上传输。实际的数据被分割成固定大小的数据块。

> 默认是128字节，最后一个数据块可以小于该固定长度

​		`chunk header + chunk data`

​		Chunk Header：

- 用于标识本块的Chunk Basic Header
  - 此包的唯一不变的部分,并且由一个独立的byte构成
  - 包括了2个作重要的标志位：`chunk type`以及`stream id`
  - `chunk type`决定了消息头的编码格式,该字段的长度完全依赖于`stream id``,stream id`是一个可变长的字段.
- 用于标识本块负载所属消息的Chunk Message Header
  - 包含了将要发送的`Message`的信息
  - 该字段的长度由`chunk basic header`中的`chunk type`决定
- 以及当时间戳溢出时才出现的`Extended Timestamp`
  - 当`chunk message header`的时间戳大于等于`0xffffff`的时候`chunk message header`后面的四个字节就代表扩展时间.

![img](https://img-blog.csdn.net/20130914205506281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20130914205513812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



### `c0 / s0`

```c
+-+-+-+-+-+-+-+-+ 
| version | 
+-+-+-+-+-+-+-+-+ 
C0 and S0 bits
```

- ##### 版本(8比特)

  - 在C0包内，这个字段代表客户端请求的RTMP版本号。
  - 在S0包内，这个字段代表服务端选择的RTMP版本号。
  - 版本0-2用在早期的产品中，现在已经被弃用；
  - 服务器无法识别客户端的版本号，应该回复版本3
    - 客户端可以选择降低到版本3，或者中止握手过程。 
  - 版本4-31被预留用于后续产品；
  - 版本32-255（为了区分RTMP协议和文本协议，文本协议通常以可打印字符开始）不允许使用

### `c1 / s1`

```c
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| time (4 bytes) | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| zero (4 bytes) | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| random bytes | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| random bytes | | (cont) | | .... | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
C1 and S1 bits
```

- 时间戳(4字节)
  - 客户端应该使用此字段来标识所有流块的时刻。
  - 时间戳取值可以为零或其他任意值。
  - 为了同步多个块流，客户端可能希望多个块流使用相同的时间戳。

- 零(4字节)，本字段必须为零。
-  随机数据(1528字节)
  - 包含任意数据。
  - 由于握手的双方需要区分另一端，此字段填充的数据必须足够随机(以防止与其他握手端混淆)。
  - 不过没必要为此使用加密数据或动态数据。  

### `c2 / s2`

```c
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| time (4 bytes) | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| time2 (4 bytes) | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| random echo | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
| random echo | | (cont) | | .... | 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
C2 and S2 bits
```

- 时间(4字节)，对端发送的时间戳
- 时间(4字节)，接收对端发送过来的握手包的时刻
  - 握手的双方可以使用时间1和时间2字段来估算网络连接的带宽和/或延迟，但是不一定有用。
- 随机数据(1528字节)，包含对端发送过来的随机数据

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
