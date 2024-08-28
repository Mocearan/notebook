# RTP

​		Real-time Transport Protocol,实时传输协议

---

​		是在Internet上传输多媒体数据流的一种网络协议。在视频监控、实时直播、语音电话、视频会议都能看到应用。

​		`webRTC / GB28181 / ONVIF`将其作为传输协议。在视频监控和实时视频传输领域`RTP`因其特点而具有统治地位。

- `RTP`的`T`是`transport`，不同于`RTSP/RTMP`的`T`是`Time`
  - 意味着`RTP`是基于`UDP/TCP`传输，并加上实时流媒体传输控制功能的传输协议
  - 一般基于`UDP`，如果需要的话可以使用`TCP`或者 `ATM`等其它协议
- 能够在单播或者多播网络环境中实现传流媒体数据的实时传输
- 由`RTP`数据传输协议和`RTCP`控制协议构成
  - 数据通过一个多播地址和一对端口传输
    - `RTP`端口用于传输音频数据，`RTCP`端口用于传输控制包
    - `RTP`会话使用合法的偶数端口（`2n）`，对应的RTCP包使用下一个奇数端口（`2n+1`）
  - `RTP`提供低延时的数据传输
  - `RTCP` 来完成流量控制和拥塞监控
    - 进行服务质量的监视与反馈、媒体间的同步，以及多播组中成员的标识
    - 基于`RTCP`增加`GCC / BBR`等拥塞控制算法提高网络传输性能
- `RTSP`虽然与`RTP/RTCP`关联不深，但`RTSP`仍旧是基于`RTP/RTCP`拓展而来
  - `RTSP`提供了对实时流的会话管理和媒体控制
  - 视频监控中摄像机、NVR、前后端之间都是用这个协议和RTP协议配合进行流媒体传输
  - 开始播放，暂停、快播这些播放功就需要靠RTSP来支持
  - 传输数据的仍然是`RTP`协议，质量保证的是`RTCP`
- 所以，`RTP/RTCP`实际上是流媒体领域中特殊的传输层
  - 设计RTP协议为了规避TCP协议的一些缺点
  - TCP协议在操作系统的协议栈上实现了流量和拥塞控制等机制，但是通用数据的一般方式
  - 但是流媒体传输UDP更适合传输，但也需要拥塞控制等功能
  - 所以RTP和RTCP协议，就是为解决音视频低延时等场景提供的


​		

## 参考

[RTP/RTCP协议_5g中的rtcp报文-CSDN博客](https://blog.csdn.net/guoyunfei123/article/details/106261215?ops_request_misc=%7B%22request%5Fid%22%3A%22166149958616782414924248%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fblog.%22%7D&request_id=166149958616782414924248&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~hot_rank-2-106261215-null-null.article_score_rank_blog&utm_term=RTP%2FRTCP&spm=1018.2226.3001.4450)

[RFC 6184 - RTP Payload Format for H.264 Video (ietf.org)](https://datatracker.ietf.org/doc/html/rfc6184)

[ RTP Payload Format for High Efficiency Video Coding (HEVC)](https://mirrors.nju.edu.cn/rfc/rfc7798.html)

[Real-Time Transport Protocol (RTP) Parameters (iana.org)](https://www.iana.org/assignments/rtp-parameters/rtp-parameters.xhtml)

系统性学习，直接看RFC3550规范即可

[H.264 RTPpayload 格式------ H.264 视频 RTP 负载格式（包含AAC部分解析） - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7233279.html)

[音视频传输：RTP协议详解和H.264打包方案_rtsp和h264的关系-CSDN博客](https://blog.csdn.net/weixin_41422027/article/details/101530795)

[使用FFmpeg命令行进行UDP、RTP推流（H264、TS），ffplay接收-腾讯云开发者社区-腾讯云 (tencent.cn)](https://cloud.tencent.cn/developer/article/2038204)

## 概念

### 混频器 Mixer

​		中间系统。`RTP`是一个多数据源协议，接收端需要处理多个数据来源（视频、音频、多发送端等）。混频器将一个或多个媒体源的`RTP`数据包重新编码合成一个新的`RTP`数据包，然后转发出去。

- 混频器可能会改变数据包的数据格式
- 组合的新数据包生成一个以混频器作为同步源的新`SSRC`
  - 过贡献源列表（`CSRC`表）可以确认合成源的参与者



### 转换器 translator

​		中间系统，用于通过IP多播无法直接到达的用户区，如在防火墙两端使用转换器，外侧转换器通过安全连接将数据传输到内侧转换器。

​		转发RTP数据包但不改变数据包的同步源标识符。

​		![img](https://pic4.zhimg.com/80/v2-0ae7c935f58299315a533a5d5439a1f7_720w.webp)

![img](https://pic1.zhimg.com/80/v2-fd973eef1b3978f7cc5121965dacb0f0_720w.webp)



### 序列号

​		用来区分RTP报文，检测丢包和乱序。是一个16位的二进制整数，以1递增，达到最大时自动恢复为0。

​		因为序列号永远连续递增，可以根据检测序列号递增判断是否丢包：接收到的包序列号大于1的跳跃认为丢包，根据序列号是否被打乱判断是否乱序。

### 时间戳

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

### 同步信源

​		同步信源是产生媒体流的信源，如麦克风、摄像机、RTP混合器等。是RTP包头中一个32位数字SSRC标识符，不依赖于网络地址，接收者将根据SSRC标识符来区分不同的信用，进行RTP报文的分组。

​		SSRC相当于一个RTP传输session的ID，即RTP包发出方的标识。同一个RTP会话中不会有两个相同的SSRC值，所以当RTP发送者传输地址改变时，SSRC会重新生成。

​		SSRC是随机产生的，并且保证唯一。



### 贡献信源

​		RTP 的同步源 (SSRC) 标识符用于标识同步源，即 RTP 流的源。在复杂的场景中，例如视频会议，可能有多个同步源。SSRC 保证了 RTP 流能够被正确地同步和播放。

​		当混合器接收到一个或多个同步信源的RTP报文后，经过混合处理产生一个新的组合RTP报文，并把混合器作为组合后的RTP报文的SSRC。原来所有的SSRC都作为贡献信源（CSRC）传送给接收者，使得接收者知道组成组合报文的各个SSRC。

​		一个RTP包所属源，对RTP混频器生成的组合流起了作用，它就是一个贡献源。对生成包起作用的源的SSRC标识符会被混频器添加到RTP包头贡献源列表，这个列表叫做CSRC表。

​		一般混合的RTP流中，每隔一段时间，就会有一个RTP报文包含了完整的CSRC表。



### 有效载荷

​		由RFC3551指定，有些负载类型诞生的较晚，没有具体的PT值，智能使用动态PT值，即96到127，H264被普遍指定为96.



## 特点


- 乱序重排：在UDP低延迟传输基础上增加了时间戳和序列号，在接收端重排数据包
  - 时间戳，标识音视频数据的时间戳，在接收端进行音视频同步
    - 在发送端，时间戳的值为音视频数据的采样时间，可以通过采样率计算得到
    - 在接收端，通过时间戳的值，可以确定音视频数据的播放时间
  - 序列号，标识音视频数据的顺序，以便在接收端进行数据包的重排
    - 在发送端，每个数据包都有一个唯一的序列号，表示该数据包在音视频[数据流](https://cloud.tencent.com/developer/techpedia/1734)中的位置。
    - 在接收端，通过序列号的值，可以确定音视频数据包的顺序，从而进行数据包的重组和播放
- 丢包重传，RTP协议可以使用丢包重传机制，对丢失的数据包进行重传

- 负载识别：RTP 头部包含有关负载类型（例如音频、视频、编码格式等）的信息
- 支持多种音频和视频编码格式
  - 包括MPEG、H.264、G.711、G.729等。
- 性能监控：通过`RTCP`在收发两端交换传输质量信息，如丢包率、延迟等
  - 动态调整传输参数以优化性能
  - 使用延迟控制技术，对数据包的延迟进行控制，以保证音视频数据的实时性
- 多媒体同步：时间戳能够让接收端同步在一个会话中传输多个媒体流（例如音频和视频）
- 可扩展性：支持扩展头部
  - 可以在头部中添加一些自定义的信息，扩展功能
- 前向纠错：对音视频数据进行冗余编码，以便在接收端检测和纠正错误
  - 可以将相邻的几个数据包进行编码，生成一个冗余数据包，当其中一个数据包丢失时，接收端可以使用冗余数据包来恢复数据





## 协议结构

### 协议栈

![在这里插入图片描述](https://img-blog.csdnimg.cn/dcb599171e054b88b1459aac5c07e1d9.png)



### RTP

![在这里插入图片描述](https://img-blog.csdnimg.cn/8452c33f5e794d6a8eeebc5b4b140a6e.png)

```c
// 0  2 3 4   8  9      16                32
// ----------------------------------------
// |v |p|x| cc |M|  PT   |     seq        |
//-----------------------------------------
// |		ts						   |
//-----------------------------------------
// |		ssrc					    |
//------------------------------------------------|
// | defined by profile | Extension header length|| 
//-----------------------------------------|	optional header 
// |  0~4 byte Extension header		      |      |
//         .....
//------------------------------------------------|
// |		csrc						|
//------------------------------------------
//|          ....						 |
//------------------------------------------
//|           payload data				  |
//------------------------------------------
//|padding len|        padding             |
//------------------------------------------

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
- 负载数据 payload data： 0~65535
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

```





#### 固定头部

​		`Fixed Header，12 byte`

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/dc59599c01744d923bb93080cdea0091.png#pic_center)

- `v`：`2bit`，用来标志使用的RTP版本
- `P`：`1bit`，填充标识，如果该位为 1，则该RTP包的尾部就包含附加的填充字节
  - 填充字节的第一字节的值表示填充的字节数（包含该长度字节本身）
  - 一般在一些需要固定块大小的加密算法中才需要填充
- `X`：`1bit`，扩展标识，如果该位为 `1`，RTP固定头部后面就跟有一个扩展头部
- `CC`：`4bit`，`CSRC`计数器，共享媒体源个数，一般用于混音和混屏中
  - 构成混合媒体源的各个部分
  - 四位说明则最大支持15个CSRC，一般默认为0
- `M`： `1bit`，Mark 标记位，该位的解释由配置文档（Profile`）`来负责。
  - 使用 `RTP `荷载 `H264 `码流时，如果帧分包传输，可以用该位标记是否为帧的最后一个包
    - 视频的`I frame`比较大，需要通过多个`NALU`来传输，当看到`M`位为1时就认为是这个`I frame`结束
    - 一般不进行组合传输，一个`RTP`包荷载一个`NALU`
- `PT`：`7bit`，标识了`RTP`载荷的类型
  - 对应的编解码类型参考[该文档](https://www.rfc-editor.org/rfc/rfc3551.html#page-32)
  - https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/RTP_payload_formats%25E3%2580%2582
- `Sequence number`：`16bit`，每发送一个 `RTP `数据包，序列号增加1
  - 序列号初始值是随机的
  - 当达到最大值后，则重新从0开始
  - UDP是不可靠传输协议，通过这个字段进行重新排序，收到RTP数据第一件事就是排序
  - 发送端的音频和视频都是通过`RTP`传输的，但是两组独立的序列号
- `Timestamp`：`4byte`，记录了该包中数据的第一个字节的采样时刻
  - 时间戳是采样率的倒数，不是真实的时间
    - 反应的RTP报文第一个八位组的采样时刻，目的是为了接收端计算延迟、抖动和音视频同步
  - 一个帧的数据可能被分成多个 `RTP `包发送
    - 同一个帧的多个RTP包的时间戳是相同的，不同帧的时间戳是不相同的
- `SSRC`：`4 byte`，同步源就是指RTP包流的来源
  - 不同的 `SSRC `标识不同的媒体源
    - 例如不同用户的音频就属于不同的媒体源，具有不同的 `SSRC`
  - 在会话中标识RTP负载流的身份，给一个唯一标记值
    - 不依赖于网络地址，接收者将根据`SSRC`标识符来区分不同的信源，进行RTP报文的分组
  - 只要保证在一路音视频会话里面值不相同即可
  -  RFC1889推荐了MD5随机算法
- `CSRC identifiers`：`4 byte group`，贡献媒体源列表，用来标志对`RTP`混频器产生的混合包有贡献的所有源
  - `CSRC `最多 `15 `个，每项`32bit`
  - 具体数量由`CC`指定
  - 前面的`12`个字节是固定存在于每个`rtp`中，而后面的`CSRC`只有当混频器存在时才会被插入
  - 如果是1对1的流媒体传输，这个字段就不用处理，直接忽略该字段
  - 混流和混音时，则需要把各方的RTP同步信号源列出来，这样接收端就能正确指出交谈双方的身份





```c
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
    // non-optional header
    uint32_t csrc[]; // contribution source list
};  // sizeof(rtp_hdr) 返回的结构大小不包括柔性数组的内存。
```



#### 可选头部

​		`Optional Header`， `0~32 byte`

-  `profile `定义，`2 byte`

  - `Profile`：RTP 扩展头部有两种类型
    - `one-byte header`
      - 当` Profile = 0xBEDE` 时表示使用 `one-byte header `
    -  `two-byte header`
      - `Profile = 0x1000` 时表示使用` two-byte header`
    - 扩展头部个数由 Extension header length 决定
  
- `Extension header length`， `2 byte`

  - 表示后面的 `Extension header` 共有几个字节，长度以 4 字节为单位

    > 例如 length = 3 表示 Extension header 一共占 3*4=12 个字节

  - 0~32 byte之间，不包括扩展头的4字节
  
- `Extension header`, `0~4 byte`

  -  最多7个
    -  32 字节 - 4 字节 = 28 字
    -  28 字节 ÷ 4 字节 = 7 块


#### 拓展头部

​		`Extension header`，每个拓展头部4字节，

- 由 `ID / Length / data` 组成

- 可以是 `one-byte header` 或者` two-byte header `组织方式



##### one-byte header  

​		由` ID，L，data` 三部分组成

- ID 和 L 分别占 `4 bit`，加起来等于 `one-byte`

- ID 表示扩展头部 ID 标记，

- L 表示 ` extension data` 所占字节数减1

  > ​	例如 L = 0 时实际 `data `占一个字节，由于头部需要按 4 字节对齐，因此中间补充了 `padding `数据。`extension header `占 4 字节。
  >
  > ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1793bc0b92ad4b91b70c1b136093ef51.png)

##### two-byte header 

​		由 `ID, L, data `三部分组成

- ID，L 各占一字节
- L 表示 `extension data` 所占的字节数（不同于 one-byte header 需要减一）
- ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/2bbe4dd31dec41cfb487e212aeedf85d.png)

##### header extension ID 

​		关于具体某个扩展头部的含义可以参考 RFC 或者 [webrtc 文档](https://webrtc.googlesource.com/src/+/refs/heads/main/docs/native-code/rtp-hdrext)。

```cpp
enum RTPExtensionType : int {
kRtpExtensionNone,
kRtpExtensionTransmissionTimeOffset,
kRtpExtensionAudioLevel,
kRtpExtensionInbandComfortNoise,
kRtpExtensionAbsoluteSendTime,
kRtpExtensionAbsoluteCaptureTime,
kRtpExtensionVideoRotation,
kRtpExtensionTransportSequenceNumber,
kRtpExtensionTransportSequenceNumber02,
kRtpExtensionPlayoutDelay,
kRtpExtensionVideoContentType,
kRtpExtensionVideoTiming,
kRtpExtensionFrameMarking,
kRtpExtensionRtpStreamId,
kRtpExtensionRepairedRtpStreamId,
kRtpExtensionMid,
kRtpExtensionGenericFrameDescriptor00,
kRtpExtensionGenericFrameDescriptor = kRtpExtensionGenericFrameDescriptor00,
kRtpExtensionGenericFrameDescriptor02,
kRtpExtensionColorSpace,
kRtpExtensionNumberOfExtensions  // Must be the last entity in the enum.
};
```





#### RTP over TCP

​		RTP 默认是采用 UDP 发送的，格式为 RTP 头+RTP 载荷，**如果是使用 TCP，那么需要在 RTP 头之前再加上四个字节**

- `byte 1`：Magic(0x24)，辨识符
- `byte 2`：Channel，通道。在 `SETUP `的过程中获取
- `byte 3/4`：Length，RTP 包的大小
  - RTP头部+载荷
  - 最多只能 12 位
  - 第三个字节保存高 4 位，第四个字节保存低 8 位


![rtpExample](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d06dcc585a0c0353d91be3d0f277ab6e.png)



### RTCP

​		RTCP保证RTP数据包的服务质量。

- 动态调整服务质量
- 有效控制网络拥塞
- RTCP数据报采用的是多播方式
  - 会话中的所有成员都可以通过RTCP数据报返回的控制信息，来了解其他参与者的当前情况。






| 类型 | 缩写                             | 用途                                                         |
| ---- | -------------------------------- | ------------------------------------------------------------ |
| 200  | SR（Sender Report）              | 发送端报告<br />所谓发送端是指发出RTP数据报的应用程序或者终端，发送端同时也可以是接收端。 |
| 201  | RR（Receiver Report）            | 接收端报告<br />所谓接收端是指仅接收但不发送RTP数据报的应用程序或者终端。 |
| 202  | SDES（Source Description Items） | 源点描述<br />主要功能是作为会话成员有关标识信息的载体，如用户名、邮件地址、电话号码等，此外还具有向会话成员传达会话控制信息的功能。 |
| 203  | BYE                              | 结束传输<br />主要功能是指示某一个或者几个源不再有效，即通知会话中的其他成员自己将退出会话。 |
| 204  | APP                              | 特定应用<br />由应用程序自己定义，解决了RTCP的扩展性问题，并且为协议的实现者提供了很大的灵活性。 |

![img](https://pic4.zhimg.com/80/v2-1954a33c0a99bc236f928b7d27ff23a7_720w.webp)

#### SR

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



#### 组合包

​		不同类型的RTCP信息包可堆叠，不需要插入任何分隔符就可以将多个RTCP包连接起来形成一个RTCP组合包，然后由低层协议用单一包发送出去。由于需要低层协议提供整体长度来决定组合包的结尾，在组合包中没有单个RTCP包的显式计数。

​		组合包中每个RTCP包可独立处理，而不需要按照包组合的先后顺序处理。在组合包中有以下几条强制约束：

1、只要带宽允许，在SR包或RR包中的接收统计应该经常发送，因此每个周期发送的组合RTCP 包中应包含报告包。

2、每个组合包中都应该包含SDES CNAME，因为新接收者需要通过接收CNAME来识别源，并与媒体联系进行同步。

3、组合包前面是包类型数量，其增长应该受到限制。





### profile

​		定义了一系列负载类型和对应的负载格式，也定义了特定于具体应用的 RTP 扩展和修改。

- **RFC3551**(RTP/AVP)在 RFC3550 的基础上针对 RTP 档次进行补充形成 RTP/APVP 档次，被用在具有最小会话控制的音视频会议中，是其它扩展档次的基础。该档次在没有参数协商和成员控制的会话中非常有用。该档次也为音视频定义一系列编码和负载格式。对于具体的流媒体负载格式，IETF 也定义一系列协议详细描述，如 VP8 视频负载格式[6]和 H264 视频负载格式[7]，等等。
- RFC3711(SRTP，也即 RTP/SAVP)是 RTP/AVP 在安全方面进行扩展形成的档次，为 RTP/RTCP 提供数据加密、消息认证、重放保护等功能。SRTP 具有高吞吐量和低数据膨胀等特点，是异构环境下对 RTP/RTCP 数据的有效保护。
- RFC4585(RTP/AVPF)是 RTP/AVP 在及时反馈方面进行扩展形成的档次，使得接收端能够向发送端提供及时反馈，实现短时调整和基于反馈的修复机制。该协议定义早期 RTCP 报文以实现及时反馈，并定义一系列通用 RTCP 反馈报文和特定于应用的反馈报文，如 NACK、PLI、SLI、RPSI 等。
- RFC5124(RTP/SAVPF)则是 RTP/SAVP 和 RTP/AVPF 的综合。SAVP 和 AVPF 在使用时，需要参与者借助于 SDP 协议[8]就档次和参数信息达成一致。但是对一个 RTP 会话来说，这两种档次不能同时被协商。而实际应用中，我们有同时使用这两种档次的需要。因此，RTP/SAVPF 档次应运而生，它能够使得 RTP 会话同时具有安全和及时反馈两方面的特性。



## 性能优化

- 使用合适的编解码器：不同的编解码器对带宽、延迟、丢包容忍等方面的要求不同
- 选择合适的传输协议：根据具体应用场景选择合适的传输协议
  - UDP传输速度快，但不可靠，TCP传输可靠，但延迟较高
- 使用前向纠错（FEC）：通过添加冗余数据来恢复丢失数据，可以提高RTP传输的可靠性
  - 但FEC也会增加传输的带宽消耗，因此需要根据具体情况进行权衡
- 使用RTCP：监测网络状态、调整发送速率等，从而优化RTP传输性能
- 选择合适的网络拓扑结构：，选择合适的网络拓扑结构可以减少网络延迟和带宽消耗
- 避免网络拥塞：拥塞控制、流量控制等，避免网络拥塞
- 优化网络质量：采用QoS技术、多路径传输技术等来优化网络质量。



## 安全问题

- 数据泄露：RTP不提供数据加密功能，数据在传输过程中可能被窃听或篡改
- 未经授权的访问：RTP没有内置的访问控制机制，攻击者可能会加入到RTP会话中，接收或发送音视频数据
- 拒绝服务攻击（DoS）：攻击者可以发送大量伪造的RTP数据包，消耗目标系统的网络带宽和处理资源，导致服务中断。
- 重放攻击：攻击者可能会截获并重新发送RTP数据包，导致音视频数据的重复播放。
- 信令劫持：攻击者可能会劫持与RTP相关的信令协议（如SIP或SDP），篡改会话参数，影响音视频质量或导致服务中断。

​		 解决安全问题

- 使用安全实时传输协议（SRTP）
  - SRTP是RTP的一个扩展，提供了数据加密、完整性保护和重放攻击防护功能
  - 通过使用SRTP，可以确保音视频数据在传输过程中的安全性
- 实施访问控制：在应用层实现访问控制机制，以防止未经授权的访问
  - 例如，可以使用密码保护的会议室、令牌认证等方法限制RTP会话的参与者
- 配置防火墙和入侵检测系统（IDS）
  - 可以阻止恶意流量和拒绝服务攻击
- 使用安全的信令协议：如SIPS和SDPS，以保护会话参数和防止信令劫持
- 监控网络流量：及时发现异常行为和潜在攻击。





## Q&A

### 序列号的作用

- 乱序重排
- 丢包检测

### 时间戳的作用

- 乱序重排
- 重组UDP包
- 延迟检测

### RTCP的作用

- 通过反馈分配数据的传送质量来进行拥塞控制、监视网络和诊断网络中的问题
- SSRC（同步源标识）变化通知
  - 当网络拥塞发生时或者与会者程序发生变化时SSRC多会随之更新
  - 需要为RTP源提供额外的传送层标志
- 调整RTCP包的发送速度
  - 保证数据包能顺利到达接收端，因此需要依据参与者的数据来进行调整
- 传送会话控制信息

​		在一个典型的**应用场合**下：

- 发送媒体流的应用程序周期性地产生发送端报告SR
  - 含有不同媒体流间的同步信息
  - 和已经发送的数据报和字节的计数
  - 接收端根据这些信息可以估计出实际的数据传输速率
- 另一方面，接收端会向所有已知的发送端发送接收端报告RR
  - 含有已接收 数据报的最大序列号、丢失的数据报数目、延时抖动和时间戳等重要信息
  - 发送端应用根据这些信息可以估计出往返时延
    - 可以根据数据报丢失概率和时延抖动情况动态调整发送速率，以改善网络拥塞状况
    - 或者根据网络状况平滑地调整应用程序的服务质量



### RTP会话

1. 根据*RTP*包的序列号来排序。
2. 根据声音流和图像流的相对时间（即*RTP*包的时间戳），以及它们的绝对时间（即对应的*RTCP*包中的*RTCP*），可以实现声音和图像的同步。
3. 接收缓冲用来排序乱序了的数据包；播放缓冲用来消除播放的抖动，实现等时播放。



### RTP传输地址的确定

​		依赖于会话协议进行协商，如`RTSP / SIP / HTTP`等

- 协商过程中，会确定发送端`RTP`和`RTCP`的目的地址
  - 其中`RTSP`协议传输端口通过`SetUP`方法
- 通过`SDP`协议来进行媒体信息协商



### RTP 的荷载类型

​		RTP的荷载类型相当丰富

- 音频、视频、其他信息
- 编码后的数据：H264、H265、AAC、G7xx等
- 封装后的数据：PS，TS等
  - 安防中的国标流就是`RTP + PS`



### 拓展头部的作用

[RTP扩展头部AudioLevel的应用（mediasoup显示当前通话Speaker）](https://blog.csdn.net/weixin_38102771/article/details/124222681?spm=1001.2014.3001.5502)

[RTP扩展头部AbsoluteSendTime的应用（REMB GCC拥塞控制） ](https://blog.csdn.net/weixin_38102771/article/details/127780907?spm=1001.2014.3001.5502)

[RTP扩展头部TransportSequenceNumber的应用（TFB GCC拥塞控制）](https://blog.csdn.net/weixin_38102771/article/details/128218672?spm=1001.2014.3001.5502)

​		为RTP传输协议增加扩展性，方便扩展版本功能，同时允许用户增加私有信息。

- 大部分音视频场景都没有启用RTP扩展部分
- 但`WebRTC`中利用`RTP`扩展部分做了`FEC`的算法处理，当发生RTP丢包可以快速恢复包，在网络不好的时候特别有用
  - 前向纠错，核心思想就是一些异或运算