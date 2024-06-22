# RTP

​		Real-time Transport Protocol,实时传输协议

---

​		IETF提出的一个标准，对应的RFC文档为RFC3550（RFC1889为其过期版本）。RFC3550不仅定义了RTP，而且定义了配套的相关协议RTCP（Real-time Transport Control Protocol，即实时传输控制协议）。RTP通常用于实现多播音频会议和音视频会议等应用。

​    	实时传输协议（Real-time Transport Protocol，PRT）是在Internet上处理多媒体数据流的一种网络协议，利用它能够在一对一（unicast，单播）或者一对多 （multicast，多播）的网络环境中实现传流媒体数据的实时传输。

​		RTP通常使用UDP来进行多媒体数据的传输，但如果需要的话可以使用TCP或者 ATM等其它协议，整个RTP协议由两个密切相关的部分组成：RTP数据协议和RTP控制协议。实时流协议（Real Time Streaming Protocol，RTSP）最早由Real Networks和Netscape公司共同提出，它位于RTP和RTCP之上，其目的是希望通过IP网络有效地传输多媒体数据。

​		在多播音频会议中，音频数据通过一个多播地址和一对端口传输，其中一个端口用于传输音频数据（RTP），另一个端口用于传输控制包（RTCP）。

> RTP会话使用合法的偶数端口（2n），对应的RTCP包使用下一个奇数端口（2n+1）。

​		一般基于UDP，也可使用TCP。

​		RTP的T是transport，不同于RTSP/RTMP的T是Time。这意味着RTP是某种意义上基于UDP/TCP，加上适合实时流媒体传输的控制功能的传输协议。



## 标准

[RFC 6184 - RTP Payload Format for H.264 Video (ietf.org)](https://datatracker.ietf.org/doc/html/rfc6184)

[Real-Time Transport Protocol (RTP) Parameters (iana.org)](https://www.iana.org/assignments/rtp-parameters/rtp-parameters.xhtml)



## 应用场景

### 视频会议

RTP协议可以用于实现视频会议系统中的音视频传输，确保音视频数据的实时性和同步性。

###  流媒体

RTP协议可以用于流媒体服务中，如直播、点播、[视频监控](https://cloud.tencent.com/developer/techpedia/2303)等，保证音视频数据的实时传输和播放。

###  IP电话

RTP协议可以用于实现IP电话中的语音传输，确保语音数据的实时传输和质量。

###  语音对讲

RTP协议可以用于实现语音对讲系统中的音频传输，确保音频数据的实时传输和质量。

###  游戏实时语音

RTP协议可以用于游戏实时语音中，保证游戏玩家之间的语音交流的实时性和质量。

​		音频和视频会议中，音频和视频数据通常分别在不同的RTP会话中传输，每个会话使用不同的传输地址和端口。如果在会议中同时使用了音频和视频，通常会使用不同的RTP会话来传输这两种媒体。会议参与者可以根据RTCP包中的规范化名称（CNAME，Canonical Name）来检索关联的音频和视频数据，并根据RTCP包中的时间戳信息来实现音频和视频的同步。





## 概念

### 流媒体

​		流式传输是实现流媒体的关键技术。使用流式传输可以边下载边观看流媒体节目。由于Internet是基于分组传输的，所以接收端收到的数据包往往有延迟和乱序（流式传输构建在UDP上）。要实现流式传输，就是要从降低延迟和恢复数据包时序入手。在发送端，为降低延迟，往往对传输数据进行预处理（降低质量和高效压缩）。在接收端为了恢复时序，采用了接收缓冲；而为了实现媒体的流畅播放，则采用了播放缓冲。



### 混频器 Mixer

​		中间系统。因为RTP是一个多数据源协议，接收端需要处理多个数据来源（视频、音频、多发送端等）。

​		将一个或多个源的RTP数据包重新编码合成一个新的RTP数据包，然后转发出去。混频器可能会改变数据包的数据格式，并对各个流组合的新数据包生成一个新SSRC（混频器作为它们的同步源）。过贡献源列表（CSRC表）可以确认合成源的参与者。



### 转换器

​		中间系统，转发RTP数据包但不改变数据包的同步源标识符，可用于通过IP多播无法直接到达的用户区，如在防火墙两端使用转换器，外侧转换器通过安全连接将数据传输到内侧转换器。

​		![img](https://pic4.zhimg.com/80/v2-0ae7c935f58299315a533a5d5439a1f7_720w.webp)

![img](https://pic1.zhimg.com/80/v2-fd973eef1b3978f7cc5121965dacb0f0_720w.webp)

## 特性机制

1. **实时性**：RTP 是为实时数据传输设计的。虽然 UDP 也提供了低延迟的传输，但 RTP 在此基础上增加了**时间戳和序列号**，这有助于接收端同步和播放乱序到达的数据包。
2. **负载识别**：RTP 头部包含有关负载类型（例如音频、视频、编码格式等）的信息，这使得接收端可以正确地解码和播放流。
   - RTP协议支持多种音频和视频编码格式，包括MPEG、H.264、G.711、G.729等。
3. **性能监控**：与 RTP 一起使用的 RTCP (Real-time Transport Control Protocol) 允许发送端和接收端交换有关传输质量的信息，如丢包率、延迟等。这有助于动态调整传输参数以优化性能。
4. **多媒体同步**：在一个会话中传输多个媒体流（例如音频和视频）时，RTP 的时间戳可以帮助接收端同步这些流。
5. **可扩展性**：RTP协议支持扩展头部，可以在头部中添加一些自定义的信息。



### 实时性

- 时间戳

  ​	RTP协议使用时间戳来标识音视频数据的时间戳，以便在接收端进行同步播放。

  - 在发送端，时间戳的值为音视频数据的采样时间，可以通过采样率计算得到。
  - 在接收端，通过时间戳的值，可以确定音视频数据的播放时间。

- 序列号

  ​	RTP协议使用序列号来标识音视频数据的顺序，以便在接收端进行数据包的排序。

  - 在发送端，每个数据包都有一个唯一的序列号，表示该数据包在音视频[数据流](https://cloud.tencent.com/developer/techpedia/1734)中的位置。
  - 在接收端，通过序列号的值，可以确定音视频数据包的顺序，从而进行数据包的重组和播放。

- 帧同步

  ​	RTP协议可以使用帧同步技术，确保音视频数据在传输过程中的同步性。

  > ​	例如，在视频传输中，可以在I帧（关键帧）的前面添加一个附加信息，标识该帧的时间戳和序列号，以便在接收端正确地播放视频。

- 丢包重传

  ​	RTP协议在传输过程中，可能会发生数据包丢失的情况。RTP协议可以使用丢包重传机制，对丢失的数据包进行重传



### 丢包和延迟

- 丢包重传
  - 当音视频数据包丢失时，RTP协议可以使用丢包重传机制，对丢失的数据包进行重传。接收端可以通过缓存已经接收到的数据包，等待重传的数据包到达，然后进行数据包的重组和播放。
- 前向纠错
  - RTP协议可以使用前向纠错技术，对音视频数据进行冗余编码，以便在接收端检测和纠正错误。例如，可以将相邻的几个数据包进行编码，生成一个冗余数据包，当其中一个数据包丢失时，接收端可以使用冗余数据包来恢复数据。
- 延迟控制
  - RTP协议可以使用延迟控制技术，对数据包的延迟进行控制，以保证音视频数据的实时性。例如，在视频传输中，可以在I帧（关键帧）的前面添加一个附加信息，标识该帧的时间戳和序列号，以便在接收端正确地播放视频。
- 流媒体同步
  - RTP协议可以使用流媒体同步技术，对音视频数据进行同步，以保证音视频数据的同步播放。例如，在视频传输中，可以使用帧同步技术，确保音视频数据在传输过程中的同步性。

### 同步和重排

- 时间戳
  - RTP协议使用时间戳来标识音视频数据的时间戳，以便在接收端进行同步播放。在发送端，时间戳的值为音视频数据的采样时间，可以通过采样率计算得到。在接收端，通过时间戳的值，可以确定音视频数据的播放时间。
- 序列号
  - RTP协议使用序列号来标识音视频数据的顺序，以便在接收端进行数据包的排序。在发送端，每个数据包都有一个唯一的序列号，表示该数据包在音视频数据流中的位置。在接收端，通过序列号的值，可以确定音视频数据包的顺序，从而进行数据包的重组和播放。
- 帧同步
  - RTP协议可以使用帧同步技术，确保音视频数据在传输过程中的同步性。例如，在视频传输中，可以在I帧（关键帧）的前面添加一个附加信息，标识该帧的时间戳和序列号，以便在接收端正确地播放视频。
- 接收缓存
  - RTP协议在接收端使用接收缓存，用于缓存接收到的音视频数据包。接收端可以根据数据包的时间戳和序列号，对数据包进行排序和组合，从而实现音视频数据的同步和顺序播放。



## 协议结构

### 协议栈

![在这里插入图片描述](https://img-blog.csdnimg.cn/e0d7cd85325045fa84117036534f685f.png)



![在这里插入图片描述](https://img-blog.csdnimg.cn/750ca30b20cf4e2288eb9e29b01080f2.png)



![在这里插入图片描述](https://img-blog.csdnimg.cn/dcb599171e054b88b1459aac5c07e1d9.png)



### RTP头部结构

![在这里插入图片描述](https://img-blog.csdnimg.cn/8452c33f5e794d6a8eeebc5b4b140a6e.png)

- 固定头部（Fixed Header，12 byte）
  - 版本号
  - 填充位
  - 扩展位
    -  RTP 固定头中的扩展标志位 X 置 1，则一个长度可变的扩展头部分被加到 RTP 固定头之后
  - CSRC计数器
  - 负载类型
- 可选头部（Optional Header， 0~32 byte）
  - 4字节扩展头部

    - 2byte， profile定义

      - profile 定义了一系列负载类型和对应的负载格式，也定义了特定于具体应用的 RTP 扩展和修改。

        - **RFC3551**(RTP/AVP)在 RFC3550 的基础上针对 RTP 档次进行补充形成 RTP/APVP 档次，被用在具有最小会话控制的音视频会议中，是其它扩展档次的基础。该档次在没有参数协商和成员控制的会话中非常有用。该档次也为音视频定义一系列编码和负载格式。对于具体的流媒体负载格式，IETF 也定义一系列协议详细描述，如 VP8 视频负载格式[6]和 H264 视频负载格式[7]，等等。
        - RFC3711(SRTP，也即 RTP/SAVP)是 RTP/AVP 在安全方面进行扩展形成的档次，为 RTP/RTCP 提供数据加密、消息认证、重放保护等功能。SRTP 具有高吞吐量和低数据膨胀等特点，是异构环境下对 RTP/RTCP 数据的有效保护。
        - RFC4585(RTP/AVPF)是 RTP/AVP 在及时反馈方面进行扩展形成的档次，使得接收端能够向发送端提供及时反馈，实现短时调整和基于反馈的修复机制。该协议定义早期 RTCP 报文以实现及时反馈，并定义一系列通用 RTCP 反馈报文和特定于应用的反馈报文，如 NACK、PLI、SLI、RPSI 等。
        - RFC5124(RTP/SAVPF)则是 RTP/SAVP 和 RTP/AVPF 的综合。SAVP 和 AVPF 在使用时，需要参与者借助于 SDP 协议[8]就档次和参数信息达成一致。但是对一个 RTP 会话来说，这两种档次不能同时被协商。而实际应用中，我们有同时使用这两种档次的需要。因此，RTP/SAVPF 档次应运而生，它能够使得 RTP 会话同时具有安全和及时反馈两方面的特性。

        
    - 2byte，扩展头部长度，0~32 byte之间，不包括扩展头的4字节
  - 0~32字节可选头
- CSRC列表信息

- 负载数据（Payload Data，0~65535）
  - 实际的传递数据

```c
// 0  2 3 4   8  9      16                32
// ----------------------------------------
// |v |p|x| cc |M|  PT   |     seq        |
//-----------------------------------------
// |		ts						   |
//-----------------------------------------
// |		ssrc					    |
//------------------------------------------------|
// |  defined by profile |	 length		  |		 |
//-----------------------------------------|	optional header 
// |  0~32 byte optional header			  |      |
//------------------------------------------------|
// |		csrc						|
//------------------------------------------
//|          ....						 |

/*
- 版本号（V）：2比特，用来标志使用的RTP版本
- 填充位（P）：1比特，如果置位,则该RTP包的尾部就包含附加的填充字节。
- 扩展位（X）：1比特，如果置位,RTP固定头部后面就跟有一个扩展头部。
- CSRC计数器（CC）：4比特，含有固定头部后面跟着的CSRC的数目。
- 标记位（M）：1比特,该位的解释由配置文档（Profile）来承担.
- 载荷类型（PT）：7比特，标识了RTP载荷的类型。
- 序列号（SN）：16比特
	- 发送方在每发送完一个RTP包后就将该域的值增加1
	- 接收方可以由该域记录丢包量及恢复包序列
	- 序列号的初始值是随机的
- 时间戳：32比特，记录了该包中数据的第一个字节的采样时刻。
	- 根据发送端的时钟生成的，并且与 PTS/DTS 有关联但不完全相同
	- 在一次会话开始时，时间戳初始化成一个初始值。
	- 时间戳是去除抖动和实现同步不可缺少的。
	- 即使在没有信号发送时，时间戳的数值也要随时间而不断地增加（时间在流逝嘛）
- 同步源标识符(SSRC)：32比特，同步源就是指RTP包流的来源。
	- 在一个RTP会话中，每个数据流的SSRC都不同
	- 该标识符是随机选取的 RFC1889推荐了MD5随机算法。
- 贡献源列表（CSRC List）：0～15项，每项32比特
	- 用来标志对一个RTP混合器产生的新包有贡献的所有RTP包的源。
	- 由混频器将这些有贡献的SSRC标识符插入表中。
	- SSRC标识符都被列出来，以便接收端能正确指出交谈双方的身份。
*/

// 在实时音视频传输中，音频和视频数据通常是分开发送的。RTP 时间戳可以帮助接收端同步这两种类型的媒体流，确保音频和视频能够同时播放，从而提供更好的用户体验。
// RTP 时间戳的主要目的是为了在接收端同步音视频流的播放。接收端可以通过比较音频和视频 RTP 数据包的时间戳来同步音频和视频的播放。
// 在实际应用中，RTP 时间戳可能是基于编码中的 PTS 或 DTS 生成的，但经过了一定的转换。RTP 时间戳可能是 PTS 或 DTS 的一个简化版本，或者是经过缩放的版本，以适应 RTP 协议的需要。
// RTP 时间戳还用于处理网络抖动。由于网络的不稳定性，数据包可能会在不同的时间到达接收端。RTP 时间戳允许接收端重新排序数据包，确保音视频数据的连续播放。
// PTS/DTS 主要用于解码和显示视频帧，而 RTP 时间戳用于同步和排序 RTP 数据包。RTP 时间戳通常是基于发送端的时钟生成的，而 PTS/DTS 是基于编码的时间基准。
/*
    if (audioPacket->getTimestamp() < videoPacket->getTimestamp()) {
        // 如果音频 RTP 数据包的时间戳较小，可能需要延迟视频的播放，以实现同步
    } else if (audioPacket->getTimestamp() > videoPacket->getTimestamp()) {
        // 如果音频 RTP 数据包的时间戳较大，可能需要延迟音频的播放，以实现同步
    } else {
        // 如果 RTP 时间戳相等，音频和视频可以同时播放
    }
*/

// 按小端定义
struct rtp_hdr {
	unsigned char cc:4; // CSRC count
	unsigned char x:1; // header extension flag
	unsigned char p:1; // padding flag
	unsigned char version:2;
    
	unsigned char pt:7; // payload type
	unsigned char m:1; // marker bit
    
	uint16_t seq; // sequence number
	uint32_t ts; // timestamp
	uint32_t ssrc; // synchronization source
    uint32_t csrc[]; // contribution source list
};  // sizeof(rtp_hdr) 返回的结构大小不包括柔性数组的内存。
```



### RTP over TCP

​		RTP 默认是采用 UDP 发送的，格式为 RTP 头+RTP 载荷，**如果是使用 TCP，那么需要在 RTP 头之前再加上四个字节**

- 第一个字节：Magic(0x24)–辨识符
- 第二个字节：Channel–通道，在 SETUP 的过程中获取
- 第三第四个字节：Length–RTP 包(头部+载荷）的大小，最多只能 12 位，第三个字节保存高 4 位，第四个字节保存低 8 位

![rtpExample](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d06dcc585a0c0353d91be3d0f277ab6e.png)



### 名词

#### 序列号

​		用来区分RTP报文，检测丢包和乱序。是一个16位的二进制整数，以1递增，达到最大时自动恢复为0。

​		因为序列号永远连续递增，可以根据检测序列号递增判断是否丢包：接收到的包序列号大于1的跳跃认为丢包，根据序列号是否被打乱判断是否乱序。

#### 时间戳

​		时间戳记录了负载中第一个字节的采样时间，接收放可以根据时间戳确定数据到达是否受到了延迟抖动的影响。时间戳主要用于时间同步计算以及抖动控制。

​		时间戳的单位是采样频率的倒数：采样频率90000Hz时间戳单位为1/90000s，帧率25则每帧占90000/25=3600个时间戳单位。



#### 同步信源

​		同步信源是产生媒体流的信源，如麦克风、摄像机、RTP混合器等。是RTP包头中一个32位数字SSRC标识符，不依赖于网络地址，接收者将根据SSRC标识符来区分不同的信用，进行RTP报文的分组。

​		SSRC相当于一个RTP传输session的ID，即RTP包发出方的标识。同一个RTP会话中不会有两个相同的SSRC值，所以当RTP发送者传输地址改变时，SSRC会重新生成。

​		SSRC是随机产生的，并且保证唯一。



#### 贡献信源

​		RTP 的同步源 (SSRC) 标识符用于标识同步源，即 RTP 流的源。在复杂的场景中，例如视频会议，可能有多个同步源。SSRC 保证了 RTP 流能够被正确地同步和播放。

​		当混合器接收到一个或多个同步信源的RTP报文后，经过混合处理产生一个新的组合RTP报文，并把混合器作为组合后的RTP报文的SSRC。原来所有的SSRC都作为贡献信源（CSRC）传送给接收者，使得接收者知道组成组合报文的各个SSRC。

​		一个RTP包所属源，对RTP混频器生成的组合流起了作用，它就是一个贡献源。对生成包起作用的源的SSRC标识符会被混频器添加到RTP包头贡献源列表，这个列表叫做CSRC表。

​		一般混合的RTP流中，每隔一段时间，就会有一个RTP报文包含了完整的CSRC表。



#### 有效载荷

​		由RFC3551指定，有些负载类型诞生的较晚，没有具体的PT值，智能使用动态PT值，即96到127，H264被普遍指定为96.

## RTCP

​		RTP只负责传输数据包，需要与RTCP配合使用，由RTCP来保证RTP数据包的服务质量。

​		RTCP数据报携带有服务质量监控的必要信息，能够对服务质量进行动态的调整，并能够对网络拥塞进行有效的控制。由于RTCP数据报采用的是多播方式，因此会话中的所有成员都可以通过RTCP数据报返回的控制信息，来了解其他参与者的当前情况。

​		在一个典型的应用场合下，发送媒体流的应用程序将周期性地产生发送端报告SR，该RTCP数据报含有不同媒体流间的同步信息，以及已经发送的数据报 和字节的计数，接收端根据这些信息可以估计出实际的数据传输速率。另一方面，接收端会向所有已知的发送端发送接收端报告RR，该RTCP数据报含有已接收 数据报的最大序列号、丢失的数据报数目、延时抖动和时间戳等重要信息，发送端应用根据这些信息可以估计出往返时延，并且可以根据数据报丢失概率和时延抖动 情况动态调整发送速率，以改善网络拥塞状况，或者根据网络状况平滑地调整应用程序的服务质量。

​		RTP和RTCP配合使用，它们能以有效的反馈和最小的开销使传输效率最佳化，因而特别适合传送网上的实时数据。


| 类型 | 缩写                             | 用途                                                         |
| ---- | -------------------------------- | ------------------------------------------------------------ |
| 200  | SR（Sender Report）              | 发送端报告<br />所谓发送端是指发出RTP数据报的应用程序或者终端，发送端同时也可以是接收端。 |
| 201  | RR（Receiver Report）            | 接收端报告<br />所谓接收端是指仅接收但不发送RTP数据报的应用程序或者终端。 |
| 202  | SDES（Source Description Items） | 源点描述<br />主要功能是作为会话成员有关标识信息的载体，如用户名、邮件地址、电话号码等，此外还具有向会话成员传达会话控制信息的功能。 |
| 203  | BYE                              | 结束传输<br />主要功能是指示某一个或者几个源不再有效，即通知会话中的其他成员自己将退出会话。 |
| 204  | APP                              | 特定应用<br />由应用程序自己定义，解决了RTCP的扩展性问题，并且为协议的实现者提供了很大的灵活性。 |

![img](https://pic4.zhimg.com/80/v2-1954a33c0a99bc236f928b7d27ff23a7_720w.webp)

### SR

​		发送端报告分组SR（Sender Report）用来使发送端以多播方式向所有接收端报告发送情况。

- 相应的RTP流的SSRC
- RTP流中最新产生的RTP分组的时间戳和NTP，
- RTP流包含的分组数
- RTP流包含的字节数

```c
// 0  2 3     8       16                  32
// ----------------------------------------
// |v |p| RC  |   PT   |     length        |
//-----------------------------------------
// |		SSRC of Sender				 |
//-----------------------------------------
// |		NTP timerstamp,				 |
//-----------------------------------------
// |		RTP Timestamp				 |
//------------------------------------------
//|          Sender’s packet count		   |
//-------------------------------------------
//|			Sender`s octet count		  |
//-------------------------------------------
//|			SSRC_1						 |------------|
//------------------------------------------			|
//|Frac Lost|Cumulative number of pack lost |			|
//------------------------------------------			|
//|  Extended high est seq nunmber received |			|
//-------------------------------------------			|    ssrc_1
//|			Interarrival jitter			  |			   |
//------------------------------------------			|
//|			Last SR(LSR)				 |			  |
//------------------------------------------			|
//|			Delay since last SR(DLSR)	  |--------------
//------------------------------------------
//|			SSRC_2(SSRC of second source)  |------------
//------------------------------------------			   n个 ssrc
//|				....					|-------------
//------------------------------------------
//|			profile-specific extensions   |
//-----------------------------------------

/*
- 版本号（V）：2比特，用来标志使用的RTP版本
- 填充位（P）：1比特，如果置位,则该RTP包的尾部就包含附加的填充字节。
- 接收报告计数器（RC）：5比特，该SR包中的接收报告块的数目，可以为零。
- 包类型（PT）：8比特，SR包是200。
- 长度域（Length）：16比特，其中存放的是该SR包以32比特为单位的总长度减一。
- 同步源（SSRC）：SR包发送者的同步源标识符。与对应RTP包中的SSRC一样。
- NTP Timestamp（Network time protocol）SR包发送时的绝对时间值。
	- NTP的作用是同步不同的RTP媒体流。
- RTP Timestamp：与NTP时间戳对应
	- 与RTP数据包中的RTP时间戳具有相同的单位和随机初始值。
- Sender’s packet count：从开始发送包到产生这个SR包这段时间里，发送者发送的RTP数据包的总数. SSRC改变时，这个域清零。
- Sender`s octet count：从开始发送包到产生这个SR包这段时间里，发送者发送的净荷数据的总字节数（不包括头部和填充）。发送者改变其SSRC时，这个域要清零。
- 同步源n的SSRC标识符：该报告块中包含的是从该源接收到的包的统计信息。
- 丢失率（Fraction Lost）：表明从上一个SR或RR包发出以来从同步源n(SSRC_n)来的RTP数据包的丢失率。
- 累计的包丢失数目：从开始接收到SSRC_n的包到发送SR,从SSRC_n传过来的RTP数据包的丢失总数。
- 收到的扩展最大序列号：从SSRC_n收到的RTP数据包中最大的序列号，
- 接收抖动（Interarrival jitter）：RTP数据包接受时间的统计方差估计
- 上次SR时间戳（Last SR,LSR）：取最近从SSRC_n收到的SR包中的NTP时间戳的中间32比特。如果目前还没收到SR包，则该域清零。
- 上次SR以来的延时（Delay since last SR,DLSR）：上次从SSRC_n收到SR包到发送本报告的延时。
*/
```



## 会话

​		当应用程序建立一个RTP会话时，应用程序将确定一对目的传输地址。目的传输地址由一个网络地址和一对端口组成，有两个端口：一个给RTP包，一个给RTCP包，使得RTP/RTCP数据能够正确发送。RTP数据发向偶数的UDP端口，而对应的控制信号RTCP数据发向相邻的奇数UDP端口（偶数的UDP端口＋1），这样就构成一个UDP端口对。 RTP发送过程如下，接收过程则相反。

1) RTP协议从上层接收流媒体信息码流（如H.264），封装成RTP数据包；RTCP从上层接收控制信息，封装成RTCP控制包。
2) RTP将RTP 数据包发往UDP端口对中偶数端口；RTCP将RTCP控制包发往UDP端口对中的奇数端口。

注意：

1. 根据*RTP*包的序列号来排序。
2. 根据声音流和图像流的相对时间（即*RTP*包的时间戳），以及它们的绝对时间（即对应的*RTCP*包中的*RTCP*），可以实现声音和图像的同步。
3. 接收缓冲用来排序乱序了的数据包；播放缓冲用来消除播放的抖动，实现等时播放。



## jrtplib

​		`JRTPLIB`是一个面向对象的RTP库，它完全遵循RFC 1889设计，在很多场合下是一个非常不错的选择，下面就以JRTPLIB为例，讲述如何在Linux平台上运用RTP协议进行实时流媒体编程。

```c++
// 初始化
// 在使用JRTPLIB进行实时流媒体数据传输之前，首先应该生成RTPSession类的一个实例来表示此次RTP会话，然后调用Create() 方法来对其进行初始化操作。
// RTPSession类的Create()方法只有一个参数，用来指明此次RTP会话所采用的端口号。

#include "rtpsession.h"

RTPSession sess; 
sess.Create(5000);

//3.11版jrtplib库的Create方法被修改为Create(sessparams,&transparams)
RTPUDPv4TransmissionParams transparams;
RTPSessionParams sessparams;
sessparams.SetOwnTimestampUnit(1.0/8000.0);/*设置时间戳，1/8000表示1秒钟采样8000次，即录音时的8KHz*/
sessparams.SetAcceptOwnPackets(true);
transparams.SetPortbase(portbase);/*本地通讯端口*/
```



```c++
// 数据发送
// 当RTP 会话成功建立起来之后，接下去就可以开始进行流媒体数据的实时传输了。首先需要设置好数据发送的目标地址， RTP协议允许同一会话存在多个目标地址，这可以通过调用RTPSession类的AddDestination()、 DeleteDestination()和ClearDestinations()方法来完成。
// 注意：如果是需要发到另一个NAT设备后面终端，则需要通过NAT穿透
    
    
//RTP 协议允许同一会话存在多个目标地址
unsigned long addr = ntohl(inet_addr("127.0.0.1"));
sess.AddDestination(addr, 6000);

// 对于同一个RTP会话来讲，负载类型、标识和时戳增量通常来讲都是相同的，JRTPLIB允许将它们设置为会话的默认参数，这是通过调用 RTPSession类的SetDefaultPayloadType()、SetDefaultMark()和 SetDefaultTimeStampIncrement()方法来完成的。为RTP会话设置这些默认参数的好处是可以简化数据的发送
sess.SetDefaultPayloadType(0);
sess.SetDefaultMark(false);  
sess.SetDefaultTimeStampIncrement(10);
// sess.SendPacket(buffer, 5);

sess.SendPacket((void *)buffer,sizeof(buffer),0,false,8000);
```

```c++
int SendPacket(void *data,int len)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc)
int SendPacket(void *data,int len,unsigned short hdrextID,void *hdrextdata,int numhdrextwords)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc,unsigned short hdrextID,void *hdrextdata,int numhdrextwords)
```



```c++
// 数据接收
// 对于流媒体数据的接收端，首先需要调用RTPSession类的PollData()方法来接收发送过来的RTP或者 RTCP数据报。由于同一个RTP会话中允许有多个参与者（源），你既可以通过调用RTPSession类的GotoFirstSource()和 GotoNextSource()方法来遍历所有的源，也可以通过调用RTPSession类的GotoFirstSourceWithData()和 GotoNextSourceWithData()方法来遍历那些携带有数据的源。在从RTP会话中检测出有效的数据源之后，接下去就可以调用 RTPSession类的GetNextPacket()方法从中抽取RTP数据报，当接收到的RTP数据报处理完之后，一定要记得及时释放。
JRTPLIB为RTP数据报定义了三种接收模式，其中每种接收模式都具体规定了哪些到达的RTP数据报将会被接受，而哪些到达的RTP数据报将会被拒绝。通过调用RTPSession类的SetReceiveMode()方法可以设置下列这些接收模式：

 // RECEIVEMODE_ALL　　
 //		缺省的接收模式，所有到达的RTP数据报都将被接受； 
 //	RECEIVEMODE_IGNORESOME 　　
 //		除了某些特定的发送者之外，所有到达的RTP数据报都将被接受，而被拒绝的发送者列表可以通过调用AddToIgnoreList()、 DeleteFromIgnoreList()和ClearIgnoreList()方法来进行设置； 
//	RECEIVEMODE_ACCEPTSOME 　　
//		除了某些特定的发送者之外，所有到达的RTP数据报都将被拒绝，而被接受的发送者列表可以通过调用AddToAcceptList ()、DeleteFromAcceptList和ClearAcceptList ()方法来进行设置。 

sess_client.Poll();   //接收发送过来的 RTP 或者RTCP 数据报
sess_client.BeginDataAccess();

if (sess.GotoFirstSourceWithData()) {     //遍历那些携带有数据的源
     do {   
          sess.AddToAcceptList(remoteIP, allports,portbase);
           sess.SetReceiveMode(RECEIVEMODE_ACCEPTSOME);

           RTPPacket *pack;         
          pack = sess.GetNextPacket();            // 处理接收到的数据    
           delete pack;   }
     while (sess.GotoNextSourceWithData());
 }

sess_client.EndDataAccess();
```



```c++
// 控制信息
// RTPLIB 是一个高度封装后的RTP库，程序员在使用它时很多时候并不用关心RTCP数据报是如何被发送和接收的，因为这些都可以由JRTPLIB自己来完成。只要 PollData()或者SendPacket()方法被成功调用，JRTPLIB就能够自动对到达的 RTCP数据报进行处理，并且还会在需要的时候发送RTCP数据报，从而能够确保整个RTP会话过程的正确性。

// 而另一方面，通过调用RTPSession类提供的SetLocalName()、SetLocalEMail()、 SetLocalLocation()、SetLocalPhone()、SetLocalTool()和SetLocalNote()方法， JRTPLIB又允许程序员对RTP会话的控制信息进行设置。所有这些方法在调用时都带有两个参数，其中第一个参数是一个char型的指针，指向将要被设 置的数据；而第二个参数则是一个int型的数值，表明该数据中的前面多少个字符将会被使用。例如下面的语句可以被用来设置控制信息中的电子邮件地址：
sess.SetLocalEMail("xiaowp@linuxgam.comxiaowp@linuxgam.com",19);

// 在RTP 会话过程中，不是所有的控制信息都需要被发送，通过调用RTPSession类提供的 EnableSendName()、EnableSendEMail()、EnableSendLocation()、EnableSendPhone ()、EnableSendTool()和EnableSendNote()方法，可以为当前RTP会话选择将被发送的控制信息。
```

### sample

```c++
#include <stdio.h>
#include <string.h>
#include "rtpsession.h"

// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}

int main(int argc, char** argv)
{
  RTPSession sess;
  unsigned long destip;
  int destport;
  int portbase = 6000;
  int status, index;
  char buffer[128];

  if (argc != 3) {
    printf("Usage: ./sender destip destport\\n");
    return -1;
  }

  // 获得接收端的IP地址和端口号
  destip = inet_addr(argv[1]);
  if (destip == INADDR_NONE) {
    printf("Bad IP address specified.\\n");
    return -1;
  }
  destip = ntohl(destip);
  destport = atoi(argv[2]);

  // 创建RTP会话
  status = sess.Create(portbase);
  checkerror(status);

  // 指定RTP数据接收端
  status = sess.AddDestination(destip, destport);
  checkerror(status);

  // 设置RTP会话默认参数
  sess.SetDefaultPayloadType(0);
  sess.SetDefaultMark(false);
  sess.SetDefaultTimeStampIncrement(10);

  // 发送流媒体数据
  index = 1;
  do {
    sprintf(buffer, "%d: RTP packet", index ++);
    sess.SendPacket(buffer, strlen(buffer));
    printf("Send packet !\\n");
  } while(1);

  return 0;
}
```

```c++
#include <stdio.h>
#include "rtpsession.h"
#include "rtppacket.h"

// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}

int main(int argc, char** argv)
{
  RTPSession sess;
  int localport;
  int status;

  if (argc != 2) {
    printf("Usage: ./sender localport\\n");
    return -1;
  }

   // 获得用户指定的端口号
  localport = atoi(argv[1]);

  // 创建RTP会话
  status = sess.Create(localport);
  checkerror(status);

  do {
    // 接受RTP数据
    status = sess.PollData();
 // 检索RTP数据源
    if (sess.GotoFirstSourceWithData()) {
      do {
        RTPPacket* packet;
        // 获取RTP数据报
        while ((packet = sess.GetNextPacket()) != NULL) {
          printf("Got packet !\\n");
          // 删除RTP数据报
          delete packet;
        }
      } while (sess.GotoNextSourceWithData());
    }
  } while(1);

  return 0;
}
```



### nat

 rtp/rtcp传输数据的时候，需要两个端口支持。即rtp端口用于传输rtp数据，即传输的多媒体数据；rtcp端口用于传输rtcp控制协议信息。 rtp/rtcp协议默认的端口是rtcp port = rtp port + 1 。详细的说，比如A终端和B终端之间通过rtp/rtcp进行通信，

  ![img](http://1803.img.pp.sohu.com.cn/images/blog/2009/12/14/14/25/1263be13f2eg214.jpg)

  

如上图，

​                             本地IP:PORT                            NAT映射后IP:PORT

UACA RTP的发送和接收IP:PORT ： 192.168.1.100:8000                       61.144.174.230:1597

UACA RTCP的发送和接收IP:PORT：192.168.1.100:8001                       61.144.174.230:1602

UACB RTP的发送和接收IP:PORT ： 192.168.1.10:8000                         61.144.174.12:8357

UACB RTCP的发送和接收IP:PORT：192.168.1.10:8001                        61.144.174.12:8420

  

上图可以得到一下一些信息：

   (一) 本地端口 RTCP PORT = RTP PORT + 1;但是经过NAT映射之后也有可能满足这个等式，但是并不一定有这个关系。

  （二）在NAT设备后面的终端的本地IP:PORT并不被NAT外的设置可知，也就无法通过终端的本地IP:PORT与之通信。而必须通过NAT映射之后的公网IP:PORT作为目的地址进行通信。

  如上图的终端A如果要发送RTP数据到终端B，UACA发送的目的地址只能是：61.144.174.12:8357。同理，UACB发送RTP数据给UACA，目的地址只能是： 61.144.174.230:1597 。

  （三）也许看到这里，如何得到自己的外网IP:PORT呢？如何得到对方的外网IP:PORT呢？这就是NAT IP:PORT转换和穿孔（puncture），下回分解。

 

NAT 地址转换

 如上所述，终端需要知道自己的外网IP：port，可以通过STUN、STUNT、TURN、Full Proxy等方式。这里介绍通过STUN方式实现NAT穿透。

 STUN: Simple Traversal of UDP Through NAT。即通过UDP对NAT进行穿透。

STUNT:Simple Traversal of UDP Through NATs and TCP too.可以通过TCP对NAT进行穿透。

STUN是一个标准协议，具体的协议内容网络上很多。在此不累述了。

为 了通过STUN实现NAT穿透，得到自己的公网IP:PORT，必须有一个公网STUN服务器，以及我们的客户端必须支持STUN Client功能。STUN Client 通过UDP发送一个request到STUN服务器，该请求通过NAT设备的时候会把数据报头中的本地IP:PORT换成该本地IP:PORT对应的公网 IP:PORT，STUN服务器接收到该数据包后就可以把该公网IP:PORT 发送给STUN Client。这样我们就得到了自己的公网IP:PORT。这样别的终端就可以把该公网IP:PORT最为发送UDP数据的目的地址发送UDP数据。

 

推荐一款STUN client/server 程序代码，[http://sourceforge.net/projects/stun/files/ ](http://sourceforge.net/projects/stun/files/)

这是一款开源软件。在客户端中的主要函数是下面这个：

```c++
NatType stunNatType( 
    StunAddress4& dest, //in 公网STUN服务器地址，如stun.xten.net
	bool verbose,               //in 调试时是否输出调试信息
	bool* preservePort=0,        //out if set, is return for if NAT preservers ports or not
	bool* hairpin=0 ,            //out if set, is the return for if NAT will hairpin packetsNAT设备是否支持回环
	int port=0,                // in 本地测试端口port to use for the test, 0 to choose random port
	StunAddress4* sAddr=0    // out NIC to use ，返回STUN返回的本地地址的公网IP:PORT
  );
```



​       输入StunAddress和测试端口port,得到本地IP:PORT对应的公网IP:PORT.

#### 改造

jrtplib中对rtp rtcp端口的处理关系是：rtcp port = rtp port + 1 。这就有问题，本地端口可以按照这个等式来设置端口，但是经过NAT映射之后的公网端口是随机的，有可能并不满足上述等式。

```c++
 int portbase = 6000;            //设置本地rtp端口为6000

  transparams.SetPortbase(portbase);//默认的本地rtcp端口为6001.因为这里是本地设置，所一这样设置OK
  status = sess.Create(sessparams,&transparams);  
  checkerror(status);
  
  RTPIPv4Address addr(destip,destport); //设置目的地的rtp接收IP:PORT，公网传输的话就要设置为对方的rtp公网IP:PORT
  // AddDestination（）的内部处理是把addr.ip和addr.port+1赋给rtcp。这样如果对方在公网上，就有问题了。因为对方的rtcp port 可能不等于rtp port +1;这就导致对方收不到rtcp数据包。

  status = sess.AddDestination(addr); 
```



  通过跟踪AddDestination（）函数的实现，发现在class RTPIPv4Destination的构造函数中是这样构造一个发送目的地址的：

```c++
        RTPIPv4Destination(uint32_t ip,uint16_t rtpportbase)                    
    {
        memset(&rtpaddr,0,sizeof(struct sockaddr_in));
        memset(&rtcpaddr,0,sizeof(struct sockaddr_in));
        
        rtpaddr.sin_family = AF_INET;
        rtpaddr.sin_port = htons(rtpportbase);
        rtpaddr.sin_addr.s_addr = htonl(ip);
        

            rtcpaddr.sin_family = AF_INET;
            rtcpaddr.sin_port = htons(rtpportbase+1);//默认把rtp的端口+1赋给目的rtcp端口。
            rtcpaddr.sin_addr.s_addr = htonl(ip);

        RTPIPv4Destination::ip = ip;
    }
```

​    为了实现：可以自定义目的IP地址和目的rtp port和rtcp port。为了实现这么目标，自己动手改造下面几个函数：构造函数RTPIPv4Destination() 、RTPSession::AddDestination()，思路是在目的地址设置相关函数中增加一个rtcp ip 和port参数。

```c++
    RTPIPv4Destination(uint32_t ip,uint16_t rtpportbase,uint32_t rtcpip,uint16_t rtcpport)          
  {
    memset(&rtpaddr,0,sizeof(struct sockaddr_in));
    memset(&rtcpaddr,0,sizeof(struct sockaddr_in));
    
    rtpaddr.sin_family = AF_INET;
    rtpaddr.sin_port = htons(rtpportbase);
    rtpaddr.sin_addr.s_addr = htonl(ip);
    
    /**If rtcport has not been set separately, use the default rtcpport*/
    if ( 0 == rtcpport )
    {
      rtcpaddr.sin_family = AF_INET;
      rtcpaddr.sin_port = htons(rtpportbase+1);
      rtcpaddr.sin_addr.s_addr = htonl(ip);
    }else
    {
      rtcpaddr.sin_family = AF_INET;
      rtcpaddr.sin_port = htons(rtcpport);
      rtcpaddr.sin_addr.s_addr = htonl(ip);
    }
    
    RTPIPv4Destination::ip = ip;
  }

    int RTPSession::AddDestination(const RTPAddress &addr,const RTPIPv4Address &rtcpaddr)
{
  if (!created)
    return ERR_RTP_SESSION_NOTCREATED;
  return rtptrans->AddDestination(addr,rtcpaddr);
}
```

​    在调用RTPSession::AddDestination、定义RTPIPv4Destination的时候实参也相应增加目的rtcp参数。

​    这样改造之后就可以自定义独立的设置目的地址rtp ,rtcp端口了。



#### 移植

 把jrtplib移植到arm11平台，遇到一些问题，如下。
5.1 字节序的问题
   jrtplib中的报头的字节序问题，网上可以搜到一些，但都是只言片语，没有详细的把解决方案说出来。ARM采用的是Big-Endian, 而X86采用的是Little-Endian。目前我所采用的解决方法是让两台互相通信的主机所使用的jrtplib的Endian格式一致，都为 Big-Endian或都为Little-Endian，这只需要修改jrtplib-version/src/rtpconfig_unix.h 文件，默认是采用的Little-Endian方式，里面的注释部分有说若加
\#define RTP_BIG_ENDIAN
表示采用Big-Endian的字节方式，否则默认为Little-Endian方式。至于原因还没弄清楚。可以发邮件给作者问一下。

5.2 Can't retrieve login name的错误
  上述都没有问题了，又遇到另外的问题，在N800的客户端建立RTPSession的过程中，报了Can't retrieve login name的错误，在网上搜索后，找到一篇博客讲到嵌入式系统由于某些原因系统可能没有login name, 而在RTPSession的Create->InternalCreate->CreateCNAME方法，其中的getlogin_r, getlogin和getenv操作会因为logname为空而返回ERR_RTP_SESSION_CANTGETLOGINNAME的错误。我在 N800的机器上做了个实验，使用getlogin和getenv("LOGNAME")确实都不能得到登录名。要解决上述问题，可以对jrtplib的 源代码进行修改， 即修改RTPSession的CreateCNAME，即使getlogin_r, getlogin和getenv三个函数调用都不能得到登录名，也要设置一个登录名。



## 性能优化

###  使用合适的编解码器

编解码器的选择对RTP传输性能影响很大，不同的编解码器对带宽、延迟、丢包容忍等方面的要求不同。因此，选择合适的编解码器是优化RTP性能的重要一步。

###  选择合适的传输协议

RTP通常结合使用UDP或[TCP](https://cloud.tencent.com/developer/techpedia/1603)进行传输。UDP传输速度快，但不可靠，TCP传输可靠，但延迟较高。因此，需要根据具体应用场景选择合适的传输协议。

###  使用前向纠错（FEC）

FEC是一种通过添加冗余数据来恢复丢失数据的技术，可以提高RTP传输的可靠性。但是，FEC也会增加传输的带宽消耗，因此需要根据具体情况进行权衡。

###  使用RTCP

RTCP是RTP的控制协议，可以用来监测网络状态、调整发送速率等，从而优化RTP传输性能。例如，可以通过RTCP的反馈机制实现丢包重传。

###  选择合适的网络拓扑结构

对于多点通信场景，选择合适的网络拓扑结构可以减少[网络延迟](https://cloud.tencent.com/developer/techpedia/1611)和带宽消耗，从而提高RTP传输性能。

###  避免网络拥塞

网络拥塞会导致丢包率增加，从而影响RTP传输性能。因此，需要采取一些措施，如拥塞控制、流量控制等，避免网络拥塞。

###  优化网络质量

优化网络质量可以减少丢包率、延迟等问题，从而提高RTP传输性能。例如，可以采用QoS技术、多路径传输技术等来优化网络质量。



## 安全问题

###  数据泄露

由于RTP不提供[数据加密](https://cloud.tencent.com/developer/techpedia/1565)功能，音频和视频数据在传输过程中可能被窃听或篡改。这可能导致隐私泄露和[数据完整性](https://cloud.tencent.com/developer/techpedia/1767)受损。

###  未经授权的访问

RTP没有内置的访问控制机制，因此攻击者可能会加入到RTP会话中，接收或发送音视频数据。

###  拒绝服务攻击（DoS）

攻击者可以通过发送大量伪造的RTP数据包，消耗目标系统的网络带宽和处理资源，导致服务中断。

###  重放攻击

攻击者可能会截获并重新发送RTP数据包，导致音视频数据的重复播放。

###  信令劫持

攻击者可能会劫持与RTP相关的信令协议（如SIP或SDP），篡改会话参数，影响音视频质量或导致服务中断。



## 解决安全问题

###  使用安全实时传输协议（SRTP）

SRTP是RTP的一个扩展，提供了数据加密、完整性保护和重放攻击防护功能。通过使用SRTP，可以确保音视频数据在传输过程中的安全性。

###  实施访问控制

在应用层实现访问控制机制，以防止未经授权的访问。例如，可以使用密码保护的会议室、令牌认证等方法限制RTP会话的参与者。

###  配置防火墙和入侵检测系统（IDS）

通过[防火墙](https://cloud.tencent.com/developer/techpedia/1671)和IDS，可以阻止恶意流量和拒绝服务攻击。

###  使用安全的信令协议

使用安全版本的信令协议（如SIPS和SDPS），以保护会话参数和防止信令劫持。

###  监控网络流量

实时监控网络流量，及时发现异常行为和潜在攻击。



## rtp/rtsp/rtmp

1. RTP 通常用于 VoIP 和视频会议等实时应用。
2. RTSP 用于流媒体服务，允许客户端控制媒体流的播放、暂停和停止等操作。
   1. RTSP提供了一个可供扩展的框架，它的意义在于使得实时流媒体数据的受控和点播变得可能。总的说来，RTSP是一个流媒体表 示协议，主要用来控制具有实时特性的数据发送，但它本身并不传输数据，而是必须依赖于下层传输协议所提供的某些服务。RTSP 可以对流媒体提供诸如播放、暂停、快进等操作，它负责定义具体的控制消息、操作方法、状态码等，此外还描述了与RTP间的交互操作。
   2. RTSP 在制定时较多地参考了HTTP/1.1协议，甚至许多描述与HTTP/1.1完全相同。RTSP之所以特意使用与HTTP/1.1类似的语法和操作，在很 大程度上是为了兼容现有的Web基础结构，正因如此，HTTP/1.1的扩展机制大都可以直接引入到RTSP 中。
   3. 由RTSP 控制的媒体流集合可以用表示描述（Presentation Description）来定义，所谓表示是指流媒体服务器提供给客户机的一个或者多个媒体流的集合，而表示描述则包含了一个表示中各个媒体流的相关信 息，如数据编码/解码算法、网络地址、媒体流的内容等。
   4. 虽然RTSP服务器同样也使用标识符来区别每一流连接会话（Session），但RTSP连接并没有被绑定到传输层连接（如TCP等），也就是说在 整个 RTSP连接期间，RTSP用户可打开或者关闭多个对RTSP服务器的可靠传输连接以发出RTSP 请求。此外，RTSP连接也可以基于面向无连接的传输协议（如UDP等）。
3. RTMP 通常用于实时广播和 Flash 视频流传输。