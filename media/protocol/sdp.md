# SDP

​		Session Description Protocol， 会话描述协议，定义了描述会话的统一格式

---

​		SDP用于描述媒体详情和媒体协商的协议。它为会话通知、会话初始和其它形式的多媒体会话初始等操作提供服务。

​		SDP 的设计宗旨是通用性协议，所有它可以应用于很大范围的网络环境和应用程序，但 SDP 不支持会话内容或媒体编码的协商操作。

​		SDP 信息包括：

- 会话名称和目的；
- 会话活动时间；
- 构成会话的媒体；
  - 媒体类型：video，audio...
  - 传输协议：rtp、UDP、IP
  - 媒体格式：MPEG video...
  - 多播或单播地址和硘

- 有关接收媒体的信息、地址等
- 可用的带宽信息
- 可信赖的接洽信息

​		其他的传输协议通过sdp来交换必要的信息,常用在实时音视频中用来交换信息;比如GB28181视频交互（SIP-INVITE)、RTSP（DESCRIBE)、WEBRTC等音视频交换协议中，用作描述音视频信息。

## 标准

​		SDP是互联网建议标准[RFC 2327,4566–2006] 。



## 协议格式

​		sdp 格式由多行的`type=value`组成，SDP 信息是文本信息，UTF-8 编码采用 ISO 10646 字符设置。

- `value`是一个字符串，具体格式视`type`而定
- `=`两侧不允许有空格

​		sdp 会话描述由**一个会话级描述**和**多个媒体级描述**组成。会话级描述的作用域是整个会话，媒体级描述描述的是一个视频流或者音频流.

- 会话级描述由`v=`开始到第一个媒体级描述结束
  - 必需的是v,o,s,t

- 媒体级描述由`m=`开始到下一个媒体级描述开始之前
  - m

- 媒体级描述实际是会话级描述的重载，媒体级中的缺省默认使用会话级描述中的值
- 可选的值有`*`标记

```
==Session description:==
v= (protocol version， 协议版本)
o= (owner/creator and session identifier， 会话参数)
s= (session name， 会话名称)
i=* （会话信息）
u= * (URI of description， URI描述)
e=* （Email 地址）
p=* （电话号码）
b=* （带宽信息）
c= * (connection information-not required if included in all media， 连接信息 ― 如果包含在所有媒体中，则不需要该字段)
```

```
==Time description:==
t= (time the session is active，会话活动时间)
z=* （时间区域调整）
k=* （加密密钥）
r=* （0 或多次重复次数）
```

```
==Media description:==
m= (media name and transport address)
i=* （媒体标题）
c= * (connection information-optional if included at session-level， 连接信息 — 如果包含在会话层则该字段可选)
b= * (bandwidth information，带宽信息)
k=* （加密密钥）
a= * (zero or more media attribute lines，0 个或多个会话属性行)
y= * (SSRC)
f= * (媒体描述)
```



### sample

```http
v=0
o=m1handley 2890844526 2890842807 IN IP4 126.16.64.4
s=SDP Seminar
i=A Seminar on the session description protocol
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps
e=mjh@isi.edu (Mark Handley)
b=AS:5050
c=IN IP4 224.2.17.12/127
t=2873397496 2873404696
a=recvonly
m=audio 49170 RTP/AVP 0
m=video 51372 RTP/AVP 31
m=application 32416 udp wb
a=orient:portrait
a=rtpmap:96 H264/90000
a=control:rtsp://10.86.77.14:554/h264/ch1/sub/av_stream/trackID=1

// 字段解释
V=0     ;Version 给定了SDP协议的版本
 o=<用户名> <会话id> <会话版本> <网络类型><地址类型> <地址>
<address>； Origin ,给定了会话的发起者信息
s=<sessionname> ;给定了Session Name
i=<sessiondescription> ; Information 关于Session的一些信息
u=<URI> ; URI
b=<modifier>:<value>; AS:5050：带宽5050 kb/s
e=<emailaddress>    ;Email
c=<networktype> <address type> <connection address> ;Connect Data包含连接数据
t=<会话起始时间> <结束时间>
a=<属性>:<值>
m=<媒体类型> <端口号> <传输协议> <媒体格式
a=rtpmap:96 H264/90000
格式为a=rtpmap:<媒体格式><编码格式>/<时钟频率>
a=framerate:25
表示帧率
a=control:rtsp://10.86.77.14:554/h264/ch1/sub/av_stream/trackID=1
表示这路视频流在这个会话中的编号
```



## 字段

| 属性名 | 描述                                                | 是否必选 | 备注                                                         |
| ------ | --------------------------------------------------- | -------- | ------------------------------------------------------------ |
| v      | SDP协议版本号，默认为0                              | 是       | 版本一般默认是0                                              |
| o      | 会话所有者标识（originator and session identifier） | 是       | `o=<用户名> <session id> <会话版本> <网络类型><地址类型> <地址>` |
| s      | 会话名称                                            | 是       | 本次会话的名称                                               |
| e      | email地址                                           | 否       | 一般不用                                                     |
| b      | b=AS:可用贷款                                       | 否       | 参数说明服务端本次流最大可用带宽                             |
| t      | 会话活动时间                                        | 否       | (time the session is active) 会话的起始时间和结束时间，无起止时间要求，可都赋值为0，如果是录像回放，可赋值npt时间 |
| c      | 数据连接描述                                        | 否       | `c=<networktype> <address type> <connection address>`,如果在媒体描述中存在，会话层可不用 |
| r      | 重复次数                                            | 否       | 一般不用                                                     |
| z      | 时区适配                                            | 否       | 一般不用                                                     |
| k      | 密钥                                                | 否       | 一般不用                                                     |
| a      | 会话附件属性，根据附件内容确定其意义                | 否       | 媒体描述层有很多附件属性定义                                 |
| m      | 媒体描述，此属性后直到下一个m出现都属于m的属性描述  | 是       | 视频会话时，必选                                             |



### 会话描述



#### `v (version)`  

​		必选，SDP的版本号，不包括次版本号。

- v=0

#### `o (origin)`

​		必选，对会话的发起者进行了描述

- `o=<username> <sessionid> <version> <network type> <address type> <address>`
  - `<username>`是用户的登录名。如果主机不支持`<username>`，则为 `-`。`<username>`不能含空格。
  - `<session id>`是一个数字串。在整个会话中，必须是唯一的。为了确保其唯一，建议使用NTP(Network Time Protocol) timestamp。
  - `<version>` 该会话公告的版本,供公告代理服务器检测同一会话的若干个公告哪个是最新公告.基本要求是会话数据修改后该版本值递增,建议用NTP时戳。
  - `<networktype>`网络类型，一般为”IN”,表示”internet”
  - `<address type>` 地址类型，一般为IP4
  - `<address>` 地址

#### `s (Session Name)`，

​		必选， 会话名，在整个会话中有且只有一个”s=”

- ` s=<sessionname>`



#### `c (Connection Data)`  

表示媒体连接信息。

- ` c=<networktype> <address type> <connection address>`

  - `  <network type>`：网络类型，一般为”IN”,表示”internet”

  - ` <address type>`：地址类型，一般为IP4

  - `<connection address>`：应用程序必须处理域名和ip地址两种情形。

    - 单播时，为域名或ip地址，推荐使用域名；

    - 多播，为ip地址，且ip后面必须有TTL（取值范围是0－255），地址和TTL决定了多播包被传播的范围。

    -  分层编码方案是一个数据流被分为多层，接受者能够通过申请不同层的流选择流的质量（包括带宽）如下

      - `<base multicastaddress>/<ttl>/<number of addresses>`

        - `c=IN IP4 224.2.1.1/127`

          - 如果`<number of addresses>`没有给定，则默认为1

        - `c=INIP4 224.2.1.1/127/3`

          > 等价于：
          >
          > c=IN IP4 224.2.1.1/127
          >
          > c=IN IP4 224.2.1.2/127
          >
          > c=IN IP4 224.2.1.3/127

- 一个会话声明中，会话级描述中必须有”c=”项或者在每个媒体级描述中有一个”c=”项。

  - 可能在会话级描述和每个媒体级描述中都有”c=”项。

#### `b (Bandwidth)` 

​		描述了建议的带宽，单位kilobits per second

- ` b=<modifier>:<bandwidth-value>`
- `<modifier>`：包括两种CT和AS。
  - CT：ConferenceTotal，总带宽。
  - AS：Application-SpecificMaximum，单个媒体带宽的最大值
- 扩展机制：`<modifier>`以`X－`开始。建议`modifier`越短越好。
  - 例`b=X-YZ:128`



#### `t (Times)` RepeatTimesand Time Zones

​		描述了会话的开始时间和结束时间。

- `  t=<start time> <stop time>`
- NTP时间，单位是秒
- `<stop time>`为零表示过了`<start time>`时间后会话一直持续。
- 当`<start time> `和`<stoptime>`均为零时表示持久会话。





### 媒体描述

​		SDP媒体描述及其扩展属性，在不同的协议中有定义有所不同，需要参照不同协议的协议规范来看。这里介绍的是比较通用的定义，以RTSP协议中SDP的媒体描述定义。

​		媒体描述扩展属性中有关c、b等属性不在做介绍，主要介绍m极其扩展属性`a=control\a=rtpmap\a=fmtp`进行详细介绍。



#### `m (Media Announcements)`

​	m 字段描述媒体的媒体类型、 端口、 传输层协议、 负载类型等内容。

​		此属性后直到下一个`m`出现都属于m的属性描述

​	格式：` m=<media> <port> <transport> <fmt list>`

> `m=video 6000 RTP/AVP 96` 说明媒体类型为视频或视音频, 传输端口为6000, 采用 RTP over UDP 传输方式, 负载类型为96。

- `media `媒体类型
  - `video` 视频
  - `audio` 音频
  - `application` 元数据
  - 根据不通协议规范，可扩展其他类型:
    - "data"（不向用户显示的数据） 
    - "control"（描述额外的控制通道）
- `port`：流媒体服务器发送数据的传输端口号
  - 表示服务器从本端口发送流
  - `0`表示随机端口发送
    - 如果是RTSP协议，一般为0
    - 后继协议`SETUP`时确定传输端口
    
  - 取决于c=行规定的网络类型和接下来的传送层协议：
    
    - 对UDP为1024-65535；
    - 对于RTP为偶数。奇数端口用来传输RTCP包
      - `m=video 49170/2 RTP/AVP 31`
    
  - 当分层编码流被发送到一个单播地址时，需要列出多个端口
  
    - `m=<media><port>/<number of ports> <transport> <fmt list>`
  
      > `m=video 49170/2 RTP/AVP 31`
      >
      > - 端口49170和49171为第一对RTP/RTCP端口
      > - 49172和49173为第二对的端口
      > - 传输协议是RTP/AVP
      > - 媒体格式为31（媒体格式是rtp头中payload参数对应的）
- `proto`：传输方式
  - `RTP/AVP`标识传输层协议为 RTP over UDP
  - `TCP/RTP/AVP`标识传输层协议为 RTP over TCP
  - 主流交互协议中，也用RTP/AVP表示既支持UDP又支持TCP
- `<fmt list>` 媒体格式。
  
  - RTP中用`payloadtype`来赋值，表示流的类型
  - 这里会和后面"`a=rtpmap:`"、“`a=fmtp:`”相关联
  - 对于音频和视频就是在RTP Audio/Video Profile定义的负载类型(payload type)。



### 属性

#### `a=control`

​		在所有`m`属性之前，一般会有`a=control: [...]`的扩展属性，表示后面的多媒体流控制地址有前缀`[...]`。如果`[...]`为``*``则没有前缀。这在SDP解析的时候可以作为查找依据。

​		在`m`属性之后的一个媒体描述中出现`a=control`，表示具体媒体流选择

```
v=0
o=- 1387444157003239 1 IN IP4 10.45.149.105
s=LIVE555 Streaming Media v2013.07.03
i=000100
t=0 0
a=tool:LIVE555 Streaming Media v2013.07.03
a=type:broadcast
a=control:*
a=range:npt=0-
a=x-qt-text-nam:LIVE555 Streaming Media v2013.07.03
a=x-qt-text-inf:000100
m=video 0 RTP/AVP 96
c=IN IP4 0.0.0.0
b=AS:8000
a=rtpmap:96 H264/90000
a=control:track1
m=audio 0 RTP/AVP 0
c=IN IP4 0.0.0.0
b=AS:512
a=control:track2
a=rtpmap:0 PCMU/8000
a=recvonly
m=audio 0 RTP/AVP 0
c=IN IP4 0.0.0.0
b=AS:512
a=control:track3
a=rtpmap:0 PCMU/8000
a=sendonly


这个例子中第一个m之前的a=control:*表示后面媒体描述中的控制信息没有前缀。包括三个流的描述，分别为视频、下行语音、上行语音组成。通过a=control:附加属性的值来区分，track1标识视频，track2表示下行预研、track3表示上行语音

如果第一个m之前的a=control:rtsp://10.45.12.141/h264/ch1/main/av_stream/
则后续三个媒体流的控制地址完全表示：
rtsp://10.45.12.141/h264/ch1/main/av_stream/track1
rtsp://10.45.12.141/h264/ch1/main/av_stream/track2
rtsp://10.45.12.141/h264/ch1/main/av_stream/track3
```



#### `a=rtpmap`

​		格式：`a=rtpmap: <payload type> <encoding name>/<clockrate>[/<encoding parameters>] `

> `a=rtpmap:96 DAHUA/90000;`
>
> `a=rtpmap:96 H264/90000`

- `payloadtype` 负载类型

  - 在多路复用技术中表示一个流通道，这里与媒体信息中的`fmt`对应
  - 扩展描述媒体打包信息

- `encodingname`：编码方式

  - 表示流的编码方式，比如H264、H265等

- `clock rate`：采样的时钟频率

  - 视频流一般为`90000`

  - 如果为RTP流，时钟频率将作为RTP帧间隔的衡量

    > 例如帧间隔为40ms，则RTP包的时间戳间隔为`（40/1000）*90000=3600`



- 对于音频流，<编码参数> 说明了音频的通道数。通道数默认缺省值为1。对于视频流，现阶段没有<编码参数>。

  > ```
  > m=audio 49230 RTP/AVP 96 97 98          m=audio 8888 RTP/AVP 0     m=video 1234 RTP/AVP 96
  > a=rtpmap:96 L8/8000                     a=rtpmap:0 pcma/8000/1     a=rtpmap:96 H264
  > a=rtpmap:97 L16/8000
  > a=rtpmap:98 L16/11025/2
  > ```

- 在rtpmap中，实验性的编码方案也可以用。其格式名前一定为”X－”例：一种新的实验性的被称为GSMLPC的音频流，使用的动态负载类型为99。

  > ```
  > m=video 49232 RTP/AVP 99
  > a=rtpmap:99 X-GSMLPC/8000
  > ```



#### `a=fmtp`

​		附加属性的格式，H264和[H265](https://so.csdn.net/so/search?q=H265&spm=1001.2101.3001.7020)有一定的区别，这里分开来说明

h264格式：`a=fmtp:<payload type> profile-level-id=<xxx>; packetization-mode=1;sprop-parameter-sets=xxx,xxx`

> `a=fmtp:96 profile-level-id=420029; packetization-mode=1; sprop-parameter-sets=Z00AKpY1QPAET8s3AQEBQAABwgAAV+QB,aO48gA==`

- `payload type`：负载类型

  - 多路复用技术中表示一个流通道，这里与媒体信息中的`fmt`对应

- profile-level-id：

  - 三个十六进制表示的字节

    > `profile-level-id=42 00 29`

    - 第一个字节表示profile_idc（编码规格）

      - 一般有三个取值
      - 0x42(66)-baseline profile
      - 0x4D(77)-main profile
      - 0x58(88)-extened profile
      -  0x64(100)-high profile;

    - 第二个字节表示profile-iop

      - profile-iop每个字段对应一个含义，主要是区分是否严格按照编码规范，这里一般取`00`；

    - 第三个字节表示level_idc编码等级

      - 其等于编码等级*10

        > 上例中的编码等级为 `(2*16 + 9*1)/10=4.1`

- `packetization-mode`：表示封装方式

  - 0或不存在时, 必须使用单一NALU 单元模式
  - 1-必须使用非交错(non-interleaved)封包模式
  - 2-必须使用交错(interleaved)封包模式
  - 一般采用非交错模式

- `sprop-parameter-sets`：`sps,pps`数据的base64编码

  - 序列参数集和图像参数NAL单元，采用 Base64 进行编码.
  -  不同的参数集间用`,`号隔开

h265格式：`a=fmtp:96 sprop-vps=xxx;sprop-sps=xxx; sprop-pps=xxx`

> `a=fmtp:96 sprop-vps=QAEMAf//AIAAAAMAAAMAAAMAAAMAALUCQA==;sprop-sps=QgEBAWAAAAMAsAAAAwAAAwB7oAPAgBDlja5JMvTcBAQEAg==; sprop-pps=RAHA8vA8kAA=`

- `sprop-vps`：VPS的base64编码
- `sprop-sps`：SPS的base64编码
- `sprop-pps`: PPS的base64编码

`fmtp`还可以自定义其他属性，类似方式`a= fmtp:96 xxx=xxx;`，具体扩展含义，可以自定义，只要客户端和服务器规定一致即可。



#### `SuggestedAttributes`

- ` a=<TYPE>或 a=<TYPE>:<VALUES>`

- `a=framerate:<帧速率>`   单位:帧/秒        1s播放几个rtp包  倒数为一个rtp包承载的数据播放的时间单位s

  > 音频的话       a=framerate:50        1byte*8000hz*20ms=160B
  >
  > 则每个rtp包的音频数据量为160B  时间戳增值为160

- `a=lang:<语言标记>`  会话描述的缺省语言或媒体描述的语言



部分字段用法说明：
► a 字段:a=downloadspeed: 下载倍速(取值为整型)
a=downloadspeed: 下载倍速(取值为整型)
a=setup:TCP 连接方式(表示本 SDP 发送者在 RTP over TCP 连接建立时是主动还是被动发起 TCP 连接, “active”为主动, “passive”为被动)
a=connection:new (表示采用 RTP over TCP 传输时新建或重用原来的 TCP 连接, 可固定采用新建 TCP 连接的方式)
a=svcspace: 空域编码方式
a=svctime: 时域编码方式

► s 字段:
使用s字段标识请求媒体流的操作类型。 “Play”代表实时点播; “Playback”代表历史回放; “Download”代表文件下载; “Talk”代表语音对讲
► u 字段:
u应填写视音频文件的URI。 普通方式采用http://存储设备ID[/文件夹]* /文件名, [/文件夹]* 为0-N 级文件夹

► t 字段:
当回放或下载时, t 行值为开始时间和结束时间, 用“ ”分隔
► f 字段:
f = v/编码格式/分辨率/帧率/码率类型/码率大小 a/编码格式/码率大小/采样率
v表视频，编码格式：1. MPEG-4 2. H.264 3. SVAC 4. 3GP 帧率(十进制整数字符串表示) : 0~99
a表音频，编码格式：1. G.711 2. G.723.1 3.G.729 4. G.722.1
音频编码码率:

1———5.3 kbps (注: G.723.1 中使用)
2———6.3 kbps (注: G.723.1 中使用)
3———8 kbps (注: G.729 中使用)
4———16 kbps (注: G.722.1 中使用)
5———24 kbps (注: G.722.1 中使用)
6———32 kbps (注: G.722.1 中使用)
7———48 kbps (注: G.722.1 中使用)
8———64 kbps (注: G.711 中使用)

采样率:

1———8 kHz (注: G.711/ G.723.1/ G.729 中使用)
2———14 kHz (注: G.722.1 中使用)
3———16 kHz(注: G.722.1 中使用)
4———32 kHz(注: G.722.1 中使用)

目前主要的开源SIP协议栈包括有:

- \1. OPAL。它是基于Openh323的架构前提, 优化音视频的编解码和传输内容, 对所有的VOIP协议进行多层次的抽象, 处于不断成熟和完善的阶段
- \2. ReSIProcate。是支持新一代的rfc3261的独立SIP协议栈, 体现出高稳定性、兼容性强的特性
- \3. osip2。在对原有协议栈进行封装处理的前提下, 由C语言编写而成的SIP开发源码的协议栈, 具有较大的开发难度和工作量, 且必须与其他协议栈相整合使用
- \4. PJSIP。由C语言编写而成的一种开源协议栈, 适用于嵌入式SIP功能的开发和应用, 也是智能家居网关设计开发的首选



## 注意

​		如果SDP语法分析器不能识别某一类型(Type),则整个描述丢失。

​    如果”a=”的某属性值不理解,则予以丢失此属性。

​    会话级的描述就是媒体级描述的缺省值（就是说媒体级描述其实也是一个会话级描述，只不过没写出来的会话级描述参数都用的缺省值）。



## 例子

#### 媒体级会话VLC播放264视频的sdp文件
```
m=video 1234 RTP/AVP 96
a=rtpmap:96 H264
a=framerate:15
c=IN IP4 172.18.168.45
```

​		详细：http://blog.csdn.net/zhangjikuan/article/details/27378237



### 媒体级会话VLC播放g711音频的sdp文件

```
m=audio 8888 RTP/AVP 0   
a=rtpmap:0 pcma/8000/1                          这地方是a率压缩方式；pcmu就是u率型
a=framerate:25                                 8000/25=320Byte    每个rtp包的音频数据为320byte   时间戳增值为320
c=IN IP4 192.168.1.230
```

​		详细：http://blog.csdn.net/zhangjikuan/article/details/27379201

​		如果是同时播放音频和视频流只要把这两个媒体级会话合在一个sdp文件中就好了



### 点播mp4文件，MS_H返回给终端的sdp信息
```
v=0
o=HWPSS 3427743244 1084119141 IN IP4 127.0.0.1
s=test1.mp4
test1.mp4：	# 媒体文件名
c=IN IP4 0.0.0.0
t=0 0
a=control:*
a=range:npt=0-44.000000
 	# 44.000000：mvhd原子：ntohl (movie_header->duration)/ ntohl(movie_header->time_scale) 

m=video 0 RTP/AVP 96
	# 96：track->payload_type     视频：96音频：97
a=control:trackID=101
	# 101：轨道ID。 视频：101，102， 103 音频： 201，202， 203，204， 205
a=rtpmap:96 MP4V-ES/90000
	# 90000：track->time_scale
a=fmtp:96 profile-level-id=2;config=000001b0020;
	# profile-level-id=2：   	 mp4v的子原子esds中得来
	# config=000001b0020： 	 mp4v的子原子esds中得来

m=audio 0 RTP/AVP 97
a=control:trackID=201
a=rtpmap:97 mpeg4-generic/24000/1
	# 24000： track->time_scale
	# 1：mp4a的子原子esds中得来的
a=fmtp:97 streamtype=5;profile-level-id=15; mode=AAC-hbr; config=1308; SizeLength=13; IndexLength=3;IndexDeltaLength=3; Profile=1;
	# config=1308：mp4a的子原子esds中得来的
	# streamtype=5; profile-level-id=15; mode=AAC-hbr：写死
	# SizeLength=13; IndexLength=3; IndexDeltaLength=3; Profile=1：写死
```



  

4.直播
		编码器生成的sdp文件

```
v=0
o=- 2545495921 1885424500 IN IP4 192.168.225.158
s=111
c=IN IP4 192.168.225.153
b=RR:0
t=0 0

 

m=video 5088RTP/AVP 96
b=AS:949
a=rtpmap:96 H264/90000
a=fmtp:96 profile-level-id=4D4015;sprop-parameter-sets=Z01AFZZWCwSbCEiAAAH0AAAw1DBgAHP2AOg1cABQ,aO88gA==;packetization-mode=1
a=cliprect:0,0,576,352
a=framerate:25.
a=mpeg4-esid:201
a=x-envivio-verid:0002229D

 

m=audio 5090 RTP/AVP 97
b=AS:50
a=rtpmap:97 mpeg4-generic/24000/2
a=fmtp:97 profile-level-id=15; config=1310;streamtype=5; ObjectType=64; mode=AAC-hbr; SizeLength=13; IndexLength=3;IndexDeltaLength=3
a=mpeg4-esid:101
a=lang:eng
a=x-envivio-verid:0002229D
```

```
 # 点播上面的sdp文件，MS_H传给终端的sdp信息
v=0
	# MS_H全部是写死的
o=- 1702415089 4281335390 IN IP4 127.0.0.1
s=live
c=IN IP4 0.0.0.0
t=0 0
a=control:*
a=range:npt=0-

m=video 5088 RTP/AVP 96
b=AS:949
a=rtpmap:96 H264/90000
a=fmtp:96 profile-level-id=4D4015;sprop-parameter-sets=Z01AFZZWCwSbCEiAAAH0AAAw1DBgAHP2AOg1cABQ,aO88gA==;packetization-mode=1
a=cliprect:0,0,576,352
a=framerate:25.
a=mpeg4-esid:201
a=x-envivio-verid:0002229D
a=control:trackID=103
	# 此媒体描述是与编码器的媒体描述是一样的。

m=audio 5090 RTP/AVP 97
b=AS:50
a=rtpmap:97 mpeg4-generic/24000/2
a=fmtp:97 profile-level-id=15; config=1310;streamtype=5; ObjectType=64; mode=AAC-hbr; SizeLength=13; IndexLength=3;IndexDeltaLength=3
a=mpeg4-esid:101
a=lang:eng
a=x-envivio-verid:0002229D
a=control:trackID=201
	# 此媒体描述是与编码器的媒体描述是一样的。
```

