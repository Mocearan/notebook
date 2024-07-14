# RTP

​		Real-time Transport Protocol,实时传输协议

[RTP/RTCP协议_5g中的rtcp报文-CSDN博客](https://blog.csdn.net/guoyunfei123/article/details/106261215?ops_request_misc=%7B%22request%5Fid%22%3A%22166149958616782414924248%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fblog.%22%7D&request_id=166149958616782414924248&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~hot_rank-2-106261215-null-null.article_score_rank_blog&utm_term=RTP%2FRTCP&spm=1018.2226.3001.4450)

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

[ RTP Payload Format for High Efficiency Video Coding (HEVC)](https://mirrors.nju.edu.cn/rfc/rfc7798.html)

[Real-Time Transport Protocol (RTP) Parameters (iana.org)](https://www.iana.org/assignments/rtp-parameters/rtp-parameters.xhtml)



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
  - v 版本号
  - P 填充标识
    - 如果该位为 1，说明该 RTP 包末尾包含了一个或多个填充字节，最后一个字节的值表示填充的字节数（包含最后一个字节本身），一般在一些需要固定块大小的加密算法中才需要填充
  - X 扩展标识
    -  如果该位为 1，说明有扩展头部信息（Extension header）
    -  RTP 固定头中的扩展标志位 X 置 1，则一个长度可变的扩展头部分被加到 RTP 固定头之后
  - CC: CSRC计数器
    - 共享媒体源个数，一般用于混音和混屏中，例如某个音频流是混合了其它音频后的数据，那么其它音频源就是该音频源的 CSRC
  - M：Mark 标记位
    - 对于不同的负载类型有不同含义，例如使用 RTP 荷载 H264 码流时，如果某个帧分成多个包进行传输，可以使用该位标记是否为帧的最后一个包
  - PT：Payload Type 负载类型
    - 接收端可以根据该信息查找相应的解码器进行解码，Payload Type 值对应的编解码类型参考[该文档](https://www.rfc-editor.org/rfc/rfc3551.html#page-32)
  - Sequence number：序列号
    - 每个 RTP 包序号递增加一，接收端根据序列号可以判断传输是否丢包，序列号初始值是随机的
  - Timestamp：
    - 相对时间戳信息，反映 RTP 数据包数据采样时间，一个帧的数据可能被分成多个 RTP 包发送
    - 同一个帧的时间戳是相同的，不同帧的时间戳是不相同的，该值初始值是随机的，单位的含义与数据采样频率有关
  - SSRC：媒体源的标识，不同的 SSRC 标识不同的媒体源，例如不同用户的音频就属于不同的媒体源，具有不同的 SSRC
  - CSRC identifiers：共享媒体源列表，表示对 RTP 包内载荷起作用的媒体源，参见 CC 解释，CSRC 最多 15 个
  
- 可选头部（Optional Header， 0~32 byte）
  - 4字节扩展头部描述

    -  profile定义，2byte

      - Profile：RTP 扩展头部有两种类型
        - one-byte header
          - 当 Profile = 0xBEDE 时表示使用 one-byte header 
        -  two-byte header
          - Profile = 0x1000 时表示使用 two-byte header
        - 扩展头部个数由 Extension header length 决定
      - profile 定义了一系列负载类型和对应的负载格式，也定义了特定于具体应用的 RTP 扩展和修改。
    
        - **RFC3551**(RTP/AVP)在 RFC3550 的基础上针对 RTP 档次进行补充形成 RTP/APVP 档次，被用在具有最小会话控制的音视频会议中，是其它扩展档次的基础。该档次在没有参数协商和成员控制的会话中非常有用。该档次也为音视频定义一系列编码和负载格式。对于具体的流媒体负载格式，IETF 也定义一系列协议详细描述，如 VP8 视频负载格式[6]和 H264 视频负载格式[7]，等等。
        - RFC3711(SRTP，也即 RTP/SAVP)是 RTP/AVP 在安全方面进行扩展形成的档次，为 RTP/RTCP 提供数据加密、消息认证、重放保护等功能。SRTP 具有高吞吐量和低数据膨胀等特点，是异构环境下对 RTP/RTCP 数据的有效保护。
        - RFC4585(RTP/AVPF)是 RTP/AVP 在及时反馈方面进行扩展形成的档次，使得接收端能够向发送端提供及时反馈，实现短时调整和基于反馈的修复机制。该协议定义早期 RTCP 报文以实现及时反馈，并定义一系列通用 RTCP 反馈报文和特定于应用的反馈报文，如 NACK、PLI、SLI、RPSI 等。
        - RFC5124(RTP/SAVPF)则是 RTP/SAVP 和 RTP/AVPF 的综合。SAVP 和 AVPF 在使用时，需要参与者借助于 SDP 协议[8]就档次和参数信息达成一致。但是对一个 RTP 会话来说，这两种档次不能同时被协商。而实际应用中，我们有同时使用这两种档次的需要。因此，RTP/SAVPF 档次应运而生，它能够使得 RTP 会话同时具有安全和及时反馈两方面的特性。
    
    - Extension header length， 2byte
    
      - 表示后面的 Extension header 共有几个字节，长度以 4 字节为单位
    
        > 例如 length = 3 表示 Extension header 一共占 3*4=12 个字节
    
      - 0~32 byte之间，不包括扩展头的4字节
    
  - Extension header：0~32字节可选头部
  
    - 由 ID，L，data 组成，可以是 one-byte header 或者 two-byte header 组织方式
  
    -    one-byte header  格式如下
  
      - 它由 ID，L，data 三部分组成。
  
      - ID 和 L 分别占 4 bit，加起来等于 one-byte，
  
      - ID 表示扩展头部 ID 标记，
  
      - L 表示 extension data 所占字节数 -1
  
        > ​	例如 L = 0 时实际 data 占一个字节，由于头部需要按 4 字节对齐，因此中间补充了 padding 数据，最后一个 extension header data 占 4 字节。
        >
        > ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1793bc0b92ad4b91b70c1b136093ef51.png)
  
    - wo-byte header 格式如下，
  
      - 它也是由 ID, L, data 三部分组成
      - ID，L 各占一字节
      -  L 表示 extension data 所占的字节数（不同于 one-byte header 需要减一）
      - ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/2bbe4dd31dec41cfb487e212aeedf85d.png)
  
    - 常见的 header extension ID 类型如下，关于具体某个扩展头部的含义可以参考 RFC 或者 [webrtc 文档](https://webrtc.googlesource.com/src/+/refs/heads/main/docs/native-code/rtp-hdrext)。
  
      > ```c++
      > enum RTPExtensionType : int {
      >   kRtpExtensionNone,
      >   kRtpExtensionTransmissionTimeOffset,
      >   kRtpExtensionAudioLevel,
      >   kRtpExtensionInbandComfortNoise,
      >   kRtpExtensionAbsoluteSendTime,
      >   kRtpExtensionAbsoluteCaptureTime,
      >   kRtpExtensionVideoRotation,
      >   kRtpExtensionTransportSequenceNumber,
      >   kRtpExtensionTransportSequenceNumber02,
      >   kRtpExtensionPlayoutDelay,
      >   kRtpExtensionVideoContentType,
      >   kRtpExtensionVideoTiming,
      >   kRtpExtensionFrameMarking,
      >   kRtpExtensionRtpStreamId,
      >   kRtpExtensionRepairedRtpStreamId,
      >   kRtpExtensionMid,
      >   kRtpExtensionGenericFrameDescriptor00,
      >   kRtpExtensionGenericFrameDescriptor = kRtpExtensionGenericFrameDescriptor00,
      >   kRtpExtensionGenericFrameDescriptor02,
      >   kRtpExtensionColorSpace,
      >   kRtpExtensionNumberOfExtensions  // Must be the last entity in the enum.
      > };
      > ```
      >
      > 
  
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

​		 时间戳反映了RTP分组中的数据的第一个字节的采样时刻。在一次会话开始时的时间戳初值也是随机选择的。即使是没有信号发送时，时间戳的数值也要随时间不断的增加。接收端使用时间戳可准确知道应当在什么时间还原哪一个数据块，从而消除传输中的抖动。时间戳还可用来使视频应用中声音和图像同步。

​		在RTP协议中并没有规定时间戳的粒度，这取决于有效载荷的类型。因此RTP的时间戳又称为媒体时间戳，以强调这种时间戳的粒度取决于信号的类型。例如，对于8kHz采样的话音信号，若每隔20ms构成一个数据块，则一个数据块中包含有160个样本（0.02×8000=160）。因此每发送一个RTP分组，其时间戳的值就增加160。

- 时间戳单位，采样频率的倒数
  - 采样频率90000Hz时间戳单位为1/90000s，帧率25则每帧占90000/25=3600个时间戳单位。

- 首先，时间戳就是一个值，用来反映某个数据块的产生（采集）时间点的，后采集的数据块的时间戳肯定是大于先采集的数据块的。有了这样一个时间戳，就可以标记数据块的先后顺序。   
- 第二，在实时流传输中，数据采集后立刻传递到RTP模块进行发送，那么，其实，数据块的采集时间戳就直接作为RTP包的时间戳。   
- 第三，如果用RTP来传输固定的文件，则这个时间戳就是读文件的时间点，依次递增。这个不再我们当前的讨论范围内，暂时不考虑。   
- 第四，时间戳的单位采用的是采样频率的倒数，例如采样频率为8000Hz时，时间戳的单位为1 / 8000 ，在Jrtplib库中，有设置时间戳单位的函数接口，而ORTP库中根据负载类型直接给定了时间戳的单位（音频负载1/8000，视频负载1/90000）   
- 第五，时间戳增量是指两个RTP包之间的时间间隔，详细点说，就是发送第二个RTP包相距发送第一个RTP包时的时间间隔（单位是时间戳单位）。  

> 如果采样频率为90000Hz，则由上面讨论可知，时间戳单位为1/90000，我们就假设1s钟被划分了90000个时间块，那么，如果每秒发送25帧，那么，每一个帧的发送占多少个时间块呢？当然是 90000/25 = 3600。因此，我们根据定义“时间戳增量是发送第二个RTP包相距发送第一个RTP包时的时间间隔”，故时间戳增量应该为3600。

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



### 组合包

​		不同类型的RTCP信息包可堆叠，不需要插入任何分隔符就可以将多个RTCP包连接起来形成一个RTCP组合包，然后由低层协议用单一包发送出去。由于需要低层协议提供整体长度来决定组合包的结尾，在组合包中没有单个RTCP包的显式计数。

​		组合包中每个RTCP包可独立处理，而不需要按照包组合的先后顺序处理。在组合包中有以下几条强制约束：

1、只要带宽允许，在SR包或RR包中的接收统计应该经常发送，因此每个周期发送的组合RTCP 包中应包含报告包。

2、每个组合包中都应该包含SDES CNAME，因为新接收者需要通过接收CNAME来识别源，并与媒体联系进行同步。

3、组合包前面是包类型数量，其增长应该受到限制。



## 会话

​		当应用程序建立一个RTP会话时，应用程序将确定一对目的传输地址。目的传输地址由一个网络地址和一对端口组成，有两个端口：一个给RTP包，一个给RTCP包，使得RTP/RTCP数据能够正确发送。RTP数据发向偶数的UDP端口，而对应的控制信号RTCP数据发向相邻的奇数UDP端口（偶数的UDP端口＋1），这样就构成一个UDP端口对。 RTP发送过程如下，接收过程则相反。

1) RTP协议从上层接收流媒体信息码流（如H.264），封装成RTP数据包；RTCP从上层接收控制信息，封装成RTCP控制包。
2) RTP将RTP 数据包发往UDP端口对中偶数端口；RTCP将RTCP控制包发往UDP端口对中的奇数端口。

注意：

1. 根据*RTP*包的序列号来排序。
2. 根据声音流和图像流的相对时间（即*RTP*包的时间戳），以及它们的绝对时间（即对应的*RTCP*包中的*RTCP*），可以实现声音和图像的同步。
3. 接收缓冲用来排序乱序了的数据包；播放缓冲用来消除播放的抖动，实现等时播放。





## 数据荷载

​		通常使用RTP协议包来荷载音视频码流数据。、

- 视频帧数据较大，可能需要多个 RTP 包承载一个视频帧。
- 音频帧一般较小，一般只用一个 RTP 包也可以承载。



### 荷载音频

#### 荷载AAC

​	RTP Payload 前面需要先加 4 个字节的荷载标识

```c
// 荷载标识
payload[0] = 0x00;
payload[1] = 0x10;
payload[2] = (frameLength & 0x1FE0) >> 5; // frameLength 记录在AAC frame header 中
payload[3] = (frameLength & 0x1F) << 3;
```
```c
// | RTP Header | 荷载标识（4byte） | ADTS Frame Data（不包括 Frame Header）|
//              | ------------------RTP Payload--------------------------|
```

​		接下来将 ADTS Frame Data 拷贝到 RTP Payload[4] 开始的位置，注意 ADTS Frame Header 无需拷贝。

> 代码示例见 `lib/jrtplib.md`



### 荷载视频

​		视频帧大小差别很大

- 较小的帧小于MTU
  - 可单包直接发送
  - 或者多帧组合发送
- 较大的帧大于MTU需要分片发送

​		依据上述不同，RTP荷载视频数据有三种模式：

- 单NALU模式
- 组合帧模式
- 分片封装模式

> 有关组合帧封包和分片封包类型包含好几种，这里介绍常用的两种：STAP-A和FU-A。
>
> 一般H264进行RTP封装，SPS/PPS采用单一NALU封装方式，I帧/P帧采用FU-A分片模式，如果带有SEI及AUD可过滤掉，也可以采用单一NALU封装方式
> 

#### 单一NALU模式

​			一个RTP包包含一个完整的视频帧，帧大小需要小于MTU，不会被IP层分片



##### h264

​	RTP头部 + 一个字节 NALU Header + NALU数据部分。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/9822e94a1d2143e2b0d03209945022a2.png)

​		以SPS为例，SPS一般小于MTU，可采用单NALU封装模式，其封包后的二进制如下：

`80 e0 be 8e 8c e8 56 d5 4a 9b 57 b3 | 67 | 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a`

- `80 e0 be 8e 8c e8 56 d5 4a 9b 57 b3`为RTP头
  - ``V=10B=2,Padding=0,Extension=0,CC=0,Mark=1,PT=96,SN=48782,TS=2364036821,SSRC=0x4a9b57b3``
- RTP头之后一个字节为NALU头
  - 就是`SPS`帧去掉`00000001`起始位标识，为`67`
- 之后为NALU单元RBSP数据，在编码时简单的做法就是RTP头后直接附加去除起始标识的`NALU`数据即可



##### h265

​		RTP头部 + Payload Header + DONL + NALU数据部分。

- PayloadHeader一般与NALU Header定义完全一致
- DONL：Decoding Order Number
  - 当使用多slice编码模式时使用，用于判断一帧的每个slice是否收齐
  - 一般使用单slice，无此字段
  - 所以通常境况下，单一帧模式封装方式与H264一致，H265帧去掉起始位直接作为负载
- VPS/SPS/PPS



#### 组合帧模式

##### h264

​		针对多个较小的帧，可以采用组合成一个RTP包发送的方式。

​		这种方式在视频传输中较少使用，一般较小的帧都是通过单一NALU模式发送。

![](https://raw.githubusercontent.com/Mocearan/picgo-server/main/391987886da14a289c28b33200efc718.png)

​		此处以STAP-A组合类型为例，组合发送SPS和PPS。假设SPS和PPS的裸流如下：

```c
//sps
00 00 00 01 67 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a1 01 51 58 ae 2b 15 1d 08 0a 8a c5 71 58 a8 e8 40 54 56 2b 8a c5 47 42 02 a2 b1 5c 56 2a 3a 10 24 85 21 39 3c 9f 27 e4 fe 4f c9 f2 79 b9 b3 4d 08 12 42 90 9c 9e 4f 93 f2 7f 27 e4 f9 3c dc d9 a6 b4 03 c0 11 3f 2c a9 00 00 03 00 78 00 00 15 90 60 40 00 3e 80 00 00 11 94 06 f7 be 17 84 42 35
//pps
00 00 00 01 68 33 3c b0 00 00

// 组合包
| RTP header 12字节 | 78 【00 75】 67 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a1 01 51 58 ae 2b 15 1d 08 0a 8a c5 71 58 a8 e8 40 54 56 2b 8a c5 47 42 02 a2 b1 5c 56 2a 3a 10 24 85 21 39 3c 9f 27 e4 fe 4f c9 f2 79 b9 b3 4d 08 12 42 90 9c 9e 4f 93 f2 7f 27 e4 f9 3c dc d9 a6 b4 03 c0 11 3f 2c a9 00 00 03 00 78 00 00 15 90 60 40 00 3e 80 00 00 11 94 06 f7 be 17 84 42 35 【00 06】  68 33 3c b0 00 00|
```

- SPS去掉起始标志，size为117，十六进制为0x75
- PPS去掉起始标志，size为0x06
- 78 STAP-A类型头（NALU头）
  - 其中F为0，NRI为3，type为24
    - 24标识STAP-A类型，此类型标识后续负载为组合帧
- 00 75 表示SPS的size，后面跟的0x75个自己为去掉起始标志的SPS数据
- 00 06 表示PPS的size，后面跟的6个字节为去掉起始位的PPS数据



##### h265

![组合帧头](https://raw.githubusercontent.com/Mocearan/picgo-server/main/0438b67a92267f08e8df80e1a6c2e5e6.png)

- PayloadHeader 负载头，与H265 NALUheader类似，有F，TYPE,LayerID,TID组成
  - 一般F=0，LayerID=0，TID=1
  - 这里Type必须为48，标识组合包头

![不带DONL](https://raw.githubusercontent.com/Mocearan/picgo-server/main/b957d895a168b41933eaf55074905ca3.png)

​		带DONL的组合包封装模式与H264类似，一般情况下很少用到组合帧封装方式，小于MTU的帧一般是单一帧封装，减少解封装复杂性。



#### 分片模式

​		视频帧大小超过MTU，则RTP封装需要分片封装

##### h264 

​		较常用的分片模式位`FU-A`，其格式如下：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/62d17fa96daa466c8956bd249cf5e7b0.png)

- RTP 头部之后的第一个字节为 FU indicator
  - FU indicator 结构同NALU头，H264/H265不同
    - H264
      - ![fu indicator](https://raw.githubusercontent.com/Mocearan/picgo-server/main/e6a0e9ad779d928153e777169d6c0ab8.png)
      - F 和 NRI 与 NALU 中相同
      - Type 需要设置为 28/29，指示当前为分片模式 A/B。
    - H265，Type必须为49表示FU分片
- 第二个字节为 FU header
  - ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/920d6b1ace4e4e5e977ab726126e2d15.png)
  - S：start 标记位，当该位为 1 时表示 NALU 分片的第一个分片。
  - E：end 标记位，当该位为 1 时表示 NALU 分片的最后一个分片。
  - R：保留位，接收者可以忽略该位。
  - Type：NALU 原来的 Type 类型（1-23）。
- 这两个字节之后跟的是NALU的数据
  - 去掉起始位及NALU header之后的数据	
    - 通过FU indicator的F/NRI以及FU header即可组合成NALU header，RTP解封装的时候要生成NALU头及起始标识。
  - 按照分片大小进行分包
  - 同一个帧的分片的头两个字节除了起始和结束FU header中的S和E位不同，其他都一样

​	实例如下：

```c
80 60 be  91 8c e8 56 d5 4a 9b 57 b3 | 7c | 85
```

- `80 60 be  91 8c e8 56 d5 4a 9b 57 b3`， RTP header
- `0x7c`， FU indicator，其二进制为：01111100，F=0，NRI=3，type=28表示FU-A分片
- `0x85`， FU header， 其二进制为：10000101，S=1，E=0,type=5,表示I帧的起始FU-A分片

##### h265

![FU](https://raw.githubusercontent.com/Mocearan/picgo-server/main/9c6f2ec87838175b41336937de2c2901.png)

- PayloadHeader中F=0，LayerID=0，TID=1，Type必须为49表示FU分片
- FU header定义与FU-A定义基本一致
  - NALU Type在H265中为6bits表示，所以这里去掉了R，只保留S/E/TYPE格式
    - S：1bit,1-表示是首个分片报文,0-非首个分片报文
    - E：1bit,1-表示最后一个分片报文,0-非最后一个分片报文
    - FuType：6 bits,对应的NALU type
  - ![fu-header](https://raw.githubusercontent.com/Mocearan/picgo-server/main/530d84443466b68b033051bf9b592967.png)

​	实例如下：

```c
80 60 94 22 c6 59 8d 6a 3c c5 fa f7 | 62 01 | 93
```

- `80 60 94 22 c6 59 8d 6a 3c c5 fa f7`， RTP header
- `62 01`， Payload header，其按照格式解析后，TID=01，LayID=0,type=49,F=0，表示FU分片
- `93`， FU header，按照格式解析后，S=1,E=0,Futype=19，表示IDR_W_RADL

#### 代码

​		有关H264采用单一NALU及FU-A分片进行RTP封装发送的相关代码详解，引用FFMPEG源码进行解析，这里引用部分打包的代码，解码和这个过程相反：

```c
// libavformat/rtpenc_h264_hevc.c
static void nal_send(AVFormatContext *s1, const uint8_t *buf, int size, int last)
{
    RTPMuxContext *s = s1->priv_data;
    enum AVCodecID codec = s1->streams[0]->codecpar->codec_id;

    av_log(s1, AV_LOG_DEBUG, "Sending NAL %x of len %d M=%d\n", buf[0] & 0x1F, size, last);
    if (size <= s->max_payload_size) {//判断包大小是否小于等于RTP最大负载长度，一般RTP最大负载长度+RTP头小于MTU
        ...
            flush_buffered(s1, 0);
            ff_rtp_send_data(s1, buf, size, last);//这里调用此函数直接发送，ff_rtp_send_data中会对数据直接打RTP头后直接发送，
                                                  //这里由于小于MTU，所以采用单一帧发送模式
        ...
    } else {//视频帧长度大于MTU时，采用FU分片
        ...
        if (codec == AV_CODEC_ID_H264) {//只对H264进行注释，H265后续在说
            uint8_t type = buf[0] & 0x1F;//这里buf已经去掉起始标识00000001，buf[0]标识nalu header，这里取0~4位，即帧类型
            uint8_t nri = buf[0] & 0x60;//这里取5-6位，即：NRI，这里只是通过按位与的方式，保留了5-6位，并未真正转换为真实值，方面后买你组合
            //FU indicator字节
            s->buf[0] = 28;        /* FU Indicator; Type = 28 ---> FU-A */
            s->buf[0] |= nri; //因为nri只是保留了5-6位，这里直接按位或，即可组成fu-indicator
            // fu header
            s->buf[1] = type; //0~4帧类型
            s->buf[1] |= 1 << 7; //最高位起始位为1标识开始
            buf  += 1; //原始H264起始分片需要去掉nalu heder字节，这里直接跳过帧头
            size -= 1; //去掉头字节后，size要减去1

            flag_byte   = 1;
            header_size = 2;//fu-a 头长度
        } else {
            ...
        }

        while (size + header_size > s->max_payload_size) {
            memcpy(&s->buf[header_size], buf, s->max_payload_size - header_size);//发送缓冲buf中已经有了FU-A的2个头字节
            ff_rtp_send_data(s1, s->buf, s->max_payload_size, 0);//发送分片
            buf  += s->max_payload_size - header_size;//h264码流指针移动到未发送的起始位置
            size -= s->max_payload_size - header_size;//未发送的码流数据
            s->buf[flag_byte] &= ~(1 << 7);//更改fu-header，中间分片为00
        }
        s->buf[flag_byte] |= 1 << 6;//更改fu-header，最后一个分片，结束标志值为1
        memcpy(&s->buf[header_size], buf, size);
        ff_rtp_send_data(s1, s->buf, size + header_size, last);//打RTP头并发送
    }
}

```

​		有关H265采用单一NALU及FU-A分片进行RTP封装发送的相关代码详解，这里引用FFMPEG源码进行解析,这里引用部分打包的代码，解码和这个过程相反

```c
// libavformat/rtpenc_h264_hevc.c
static void nal_send(AVFormatContext *s1, const uint8_t *buf, int size, int last)
{
   ...
    if (size <= s->max_payload_size) {//单一帧封装模式
        int buffered_size = s->buf_ptr - s->buf;
        int header_size;
        int skip_aggregate = 0;

        if (codec == AV_CODEC_ID_H264) {
           ...
        } else {
            header_size = 2;//payloadheader的占用字节数，H265为两个字节，与NALU header一样
        }

       ...
            flush_buffered(s1, 0);
            ff_rtp_send_data(s1, buf, size, last);//这里调用此函数直接发送，ff_rtp_send_data中会对数据直接打RTP头后直接发送，与H264类似
        }
    } else {
       ...
        if (codec == AV_CODEC_ID_H264) {
          ...
        } else {
            uint8_t nal_type = (buf[0] >> 1) & 0x3F;//获取NALU type
            /*
             * create the HEVC payload header and transmit the buffer as fragmentation units (FU)
             *
             *    0                   1
             *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
             *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
             *   |F|   Type    |  LayerId  | TID |
             *   +-------------+-----------------+
             *
             *      F       = 0
             *      Type    = 49 (fragmentation unit (FU))
             *      LayerId = 0
             *      TID     = 1
             */
             //以下两个自己是payloadheader，type=49
            s->buf[0] = 49 << 1;
            s->buf[1] = 1;

            /*
             *     create the FU header
             *
             *     0 1 2 3 4 5 6 7
             *    +-+-+-+-+-+-+-+-+
             *    |S|E|  FuType   |
             *    +---------------+
             *
             *       S       = variable
             *       E       = variable
             *       FuType  = NAL unit type
             */
            s->buf[2]  = nal_type;//fu header的type就是nalu type
            /* set the S bit: mark as start fragment */
            s->buf[2] |= 1 << 7;//第一片，S=1

            /* pass the original NAL header */
            buf  += 2;//去掉nalu header的2个字节
            size -= 2;//去掉nalu header的2个字节，帧长度-2

            flag_byte   = 2;//单帧分片过程中只有fu-header中的s和e会变化，这里是其数组下标
            header_size = 3;//payloadheader+fuheader长度
        }

        while (size + header_size > s->max_payload_size) {
            memcpy(&s->buf[header_size], buf, s->max_payload_size - header_size);//发送缓冲buf中已经有了FU的3个头字节
            ff_rtp_send_data(s1, s->buf, s->max_payload_size, 0);//加上rtp头发送
            buf  += s->max_payload_size - header_size;
            size -= s->max_payload_size - header_size;
            s->buf[flag_byte] &= ~(1 << 7);//更改fu-header 中间分片，s=0 e=0
        }
        s->buf[flag_byte] |= 1 << 6;//最后一个分片，s=0,e=1
        memcpy(&s->buf[header_size], buf, size);
        ff_rtp_send_data(s1, s->buf, size + header_size, last);//发送最后一个分片
    }
}
```



### 荷载ps 流

​		针对H264 做如下PS 封装：

- 每个IDR NALU 前一般都会包含SPS、PPS 等NALU
  - 所以一个IDR NALU PS 包由外到内顺序是：
  - PSheader| PS system header | PS system Map | PES header | h264 raw data。
- 对于其它非关键帧的PS 包，直接加上PS头和PES 头就可以了。
  - 顺序为：PS header | PES header | h264raw data。
- 当有音频数据时，将数据加上PES header 放到视频PES 后就可以了。
  - 顺序如下：PS 包=PS头|PES(video)|PES(audio)
- 再用RTP 封装发送就可以了。

> - GB28181 对RTP 传输的数据负载类型有规定（参考GB28181 附录B），负载类型中96-127
>
> -   RFC2250 建议96 表示PS 封装，建议97 为MPEG-4，建议98 为H264
>
> -  即我们接收到的RTP 包首先需要判断负载类型
>
>   - 若负载类型为96，则采用PS 解复用，将音视频分开解码。
>   - 若负载类型为98，直接按照H264 的解码类型解码。
>
>   PS 包中的流类型（stream type）的取值如下：
>
> -  MPEG-4 视频流： 0x10；
> -  H.264 视频流： 0x1B；
> -   SVAC 视频流： 0x80；
> -   G.711 音频流： 0x90；
> -  G.722.1 音频流： 0x92；
> - G.723.1 音频流： 0x93；
> -  G.729 音频流： 0x99；
> - SVAC音频流： 0x9B。

​    注：此方法不一定准确，取决于打包格式是否标准





## 扩展头部的应用

[RTP扩展头部AudioLevel的应用（mediasoup显示当前通话Speaker）](https://blog.csdn.net/weixin_38102771/article/details/124222681?spm=1001.2014.3001.5502)

[RTP扩展头部AbsoluteSendTime的应用（REMB GCC拥塞控制） ](https://blog.csdn.net/weixin_38102771/article/details/127780907?spm=1001.2014.3001.5502)

[RTP扩展头部TransportSequenceNumber的应用（TFB GCC拥塞控制）](https://blog.csdn.net/weixin_38102771/article/details/128218672?spm=1001.2014.3001.5502)



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