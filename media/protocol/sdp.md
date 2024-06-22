# SDP

​		Session Description Protocol， 会话描述协议

---

​		SDP用于描述媒体详情和媒体协商的协议。它为会话通知、会话初始和其它形式的多媒体会话初始等操作提供服务。

​		SDP 的设计宗旨是通用性协议，所有它可以应用于很大范围的网络环境和应用程序，但 SDP 不支持会话内容或媒体编码的协商操作。

​		SDP 信息包括：

- 会话名称和目标；
- 会话活动时间；
- 构成会话的媒体；
- 有关接收媒体的信息、地址等。

​		

​		SIP终端之间通过SDP交换媒体信息，WebRTC中也使用了SDP。

## 标准

​		SDP是互联网建议标准[RFC 2327,4566–2006] 。



## 协议格式

​		sdp 格式由多行的`type=value`组成，SDP 信息是文本信息，UTF-8 编码采用 ISO 10646 字符设置。

​		sdp 会话描述由**一个会话级描述**和**多个媒体级描述**组成。会话级描述的作用域是整个会话，媒体级描述描述的是一个视频流或者音频流.

- 会话级描述由`v=`开始到第一个媒体级描述结束
- 媒体级描述由`m=`开始到下一个媒体级描述开始之前

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

```
v=0
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4
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


//字段解释
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

部分字段用法说明：
► a 字段:

a=rtpmap: <payload type> <encoding name>/<clockrate>[/<encoding parameters>] 如：a=rtpmap:96 DAHUA/90000;
a=downloadspeed: 下载倍速(取值为整型)
a=downloadspeed: 下载倍速(取值为整型)
a=setup:TCP 连接方式(表示本 SDP 发送者在 RTP over TCP 连接建立时是主动还是被动发起 TCP 连接, “active”为主动, “passive”为被动)
a=connection:new (表示采用 RTP over TCP 传输时新建或重用原来的 TCP 连接, 可固定采用新建 TCP 连接的方式)
a=svcspace: 空域编码方式
a=svctime: 时域编码方式

► s 字段:
使用s字段标识请求媒体流的操作类型。 “Play”代表实时点播; “Playback”代表历史回放; “Download”代表文件下载; “Talk”代表语音对讲
► u 字段:
u应填写视音频文件的URI。 普通方式采用http://存储设备ID[/文件夹]* /文件名, [/文件夹]* 为0-N 级文件夹
► m 字段:
m 字段描述媒体的媒体类型、 端口、 传输层协议、 负载类型等内容。 传输方式采用“RTP/AVP”标识传输层协议为 RTP over UDP, 采用“TCP/RTP/AVP”标识传输层协议为 RTP over TCP。
如：“m=video 6000 RTP/AVP 96” 标识媒体类型为视频或视音频, 传输端口为6000, 采用 RTP over UDP 传输方式, 负载类型为96。
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



