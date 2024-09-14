# RTMP

​		Real Time Message Protocol, 实时消息协议

---

​		`Adobe`公司推出的应用层协议，用来解决多媒体数据传输流的多路复用（Multiplexing）和分包（packetizing）的问题。

​		`RTMP `提供设备的兼容性和流媒体的稳定性，但需要一个特定的 `Flash Media Server/client` 。主要用于第三方流应用程序和较旧的硬件编码器。但随着`Flash Player`推出历史舞台，RTMP协议也逐渐被更现代的协议替代（`webrtc、hls`）。

​		`RTMP`提供了一套全双工的可靠的多路复用消息服务，类似于TCP协议，用来在一对结点之间并行传输带时间戳的音频流，视频流，数据流。通常情况下，不同类型的消息会被分配不同的优先级，当网络传输能力受限时，优先级用来控制消息在网络底层的排队顺序。

> `RTMP`消息块流不提供任何控制的优先级别和相似形式，但是可以用于高层协议提供这样的优先级，例如：一段实时视频服务会选择丢弃给缓慢的客户的视频信息确保音频信息可以及时被接收。
>
> `RTMP消息块流`包含它自己的入队协议控制消息，也提供一个高层协议机制用于嵌入用户的控制消息。

## 参考

[[总结\]RTMP流媒体技术零基础学习方法_怎么学习流媒体-CSDN博客](https://blog.csdn.net/leixiaohua1020/article/details/15814587)

[rtmp规范1.0全面指南 - 职场亮哥 - 博客园 (cnblogs.com)](https://www.cnblogs.com/CHLL55/p/13588927.html)

[rtmp协议详解 - 简书 (jianshu.com)](https://www.jianshu.com/p/b2d02d9d92eb)

[手撕Rtmp协议_视界音你而不同的博客-CSDN博客](https://blog.csdn.net/mlfcjob/category_10023015.html)

[RTMP协议分析及H.264打包原理_rtmp h264 开源-CSDN博客](https://blog.csdn.net/caoshangpa/article/details/52872146)

[RTMP协议详解及Wiresahrk抓包分析（一）-阿里云开发者社区 (aliyun.com)](https://developer.aliyun.com/article/1472344)

[RTMP协议详解及Wiresahrk抓包分析（二）-阿里云开发者社区 (aliyun.com)](https://developer.aliyun.com/article/1472345?spm=a2c6h.12873639.article-detail.12.65ab75a9p6NGyB)

[RTMP协议详解及Wiresahrk抓包分析（三）-阿里云开发者社区 (aliyun.com)](https://developer.aliyun.com/article/1472346?spm=a2c6h.12873639.article-detail.8.6e5a10232mKe3e)

[带你吃透RTMP - 简书 (jianshu.com)](https://www.jianshu.com/p/b2144f9bbe28)

[直播推流实现RTMP协议的一些注意事项 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7232594.html)

[分类:rtmp源码分析 - 懒人李冰 (lazybing.github.io)](http://lazybing.github.io/blog/categories/rtmpyuan-ma-fen-xi/)

[RTSP/RTP/RTMP/HLS - 随笔分类(第4页) - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/category/639936.html?page=4)

## 优缺点

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

​			握手（Handshake），建立连接（connect），建立流（CreateStream），播放（play）。

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

  - 实际传输为了更好地实现多路复用、分包和信息的公平性，发送端会把`Message`划分为带有`Message ID`的`Chunk`
    - 每个`Chunk`可能是一个单独的`Message`，也可能是`Message`的一部分
  - 接受端根据`chunk`中包含的`data`的长度，`message id`和`message`的长度把`chunk`还原成完整的`Message`

  > Message是协议逻辑分片，chunk是传输物理分片




### 握手

- 客户端发送`c0,c1`块。服务器收到`C0`或`C1`后发送`S0`和`S1`
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



###  建立连接

- 客户端发送消息`connect`到服务器，请求服务建立连接
- 服务器接收到连接`connect`消息后，发送`Window Acknowledgement Size`消息到客户端确认窗口大小
  - 同时连接到`connect`命令中提到的应用程序
- 服务器发送`Set Peer Bandwidth`消息到客户端设置带宽
- 客户端处理`Set Peer Bandwidth`消息后，发送`Window Acknowledgement Size`消息到服务器端确认窗口大小
- 服务器发`Stream Begin`消息到客户端，进行开流
- 服务器发送`_result`消息，通知客户端连接的状态

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d42312a4ae4e73489e38cdb3f4f29b44.png)





### 创建流

- 客户端发送命令消息`createStream`到服务器端
- 服务器端接收到`createStream`后，返回`_result`消息，通知流的状态

![img](https://img-blog.csdn.net/20130915111350062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3b77b88dda296624e0ab21432a5d3d80.png)

### 播放

- 客户端发送`play`消息到服务器
- 收到`play`后，服务器发送`ChunkSize`消息设置块大小
- 服务器发送`streambegin`，告知客户端流ID
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
> ​		一种紧凑的二进制格式，用于序列化`ActionScript object graphs`。 可以透过 `AMF over HTTP`的方式将`flash`端资料编码后传回server，server端的`remoting adaptor`接收到资料后则会译码回正确的`native`对象，交给正确的程序处理。
>
> ​		AMF 包 会有一个 trans id 事务ID，然后服务器会返回一个 _result 带着客户端的 trans id ，表示执行成功或失败。AMF 大部分都是远程过程调用。



![img](https://img-blog.csdn.net/20130914205548375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- `Message Type ID`，`1 byte`，代表不同的消息类型
  - 1-7的消息用于协议控制
    - RTMP协议自身管理要使用的消息，用户一般情况下无需操作其中的数据
  - 为8，9的消息分别用于传输音频和视频数据
  - 15-20的消息用于发送AMF编码的命令，负责用户与服务器之间的交互，比如播放，暂停等等
- `Payload Length`，`1 byte`，消息负载的有效长度
- `Timestamp`，`4 byte`，消息的时间戳
- `Stream ID`，`3 byte`，可以是任意值，小端格式存储
  - 当同一个`Message`被复用到不同的消息流中时，可以通过消息流ID来区分它们
  - 划分成`Chunk`和还原`Chunk`为`Message`的时候都是根据这个ID来辨识是否是同一个消息的`Chunk`的


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

- chunk header + chunk data`

- `chunk header = chunk basic header + chunk message header + extended timesamp`



![img](https://img-blog.csdn.net/20130914205506281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20130914205513812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



#### `Chunk Basic Header`

​		`Chunk Basic Header`的长度至少1个字节，可能是1，2，或3个字节。

- `chunk type`，`2 bit`，决定了后面`Chunk Message Header`的编码格式和长度

- `stream id`，`CSID`，可变长字段，唯一表示一个特定的流通道

  - 表示一个音频流或视频流等

    - 视频流和音频流等对应的`CSID`相同，则为多路复用

  - RTMP协议支持用户自定义`［3，65599］`之间的`CSID`

    - 0，1，2不用作ID

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



#### `Chunk Message Header`

​		标识`chunk`负载所属消息，包含了将要发送的`Message`的信息
- 该字段的长度由`chunk basic header`中的`chunk type`决定
- 就是完整Message的Header

#### `Extended Timestamp`

​		当时间戳溢出时才出现。

​		当`chunk message header`的时间戳大于等于`0xffffff`的时候`chunk message header`后面的四个字节就代表扩展时间.



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



## 消息类型

​		消息主要分为三类: 协议控制消息、数据消息、命令消息等。

- 协议控制消息，主要用于协议内的控制，两大类
  - Message Type ID = 1 2 3 5 6 
  - Message Type ID = 4 
- 数据消息
  - Message Type ID = 8 9 18
    - 8: Audio 音频数据
    - 9: Video 视频数据
    - 18: Metadata 包括音视频编码、视频宽高等信息
- 命令消息，主要有NetConnection 和 NetStream两个类
  - 两个类分别有多个函数，该消息的调用，可理解为远程函数调用



### `Set Chunk Size(1)`

​		通知对端己方发送的Chunk分片大小。



### `Abort Message(2)`

### `Acknowledgement(3)`

​		确认已经收到多少数据。

### `Window Acknowledgement Size(5)`

​		告诉发送方，本端接受到这么多的数据之后就会发` Acknowledgement (3) `的RTMP。

​		 TCP 本身有滑动窗口跟ACK。所以一般 `Window Acknowledgement Size` 会设置得非常大。



### `Set Peer Bandwidth(6)`

​		客户端或服务端发送该消息来限制对端的输出带宽.



### `AMF(20)`

​	  action 包。`releaseStream` ，`FCPublish` ，`createStream`，`_result` 等等也是一个 AMF 包，他们 message header 里面的 type id 都是 0x14。

​		包里面有一个 字符串，表示要执行哪些 action （动作）。

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
