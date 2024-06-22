# SDP

​	Session Description Protocol， 会话描述协议

---

​		SDP用于描述媒体详情和媒体协商的协议。SDP是互联网建议标准[RFC 2327,4566–2006] 。

​		SIP终端之间通过SDP交换媒体信息，WebRTC中也使用了SDP。

```
==Session description:==

v= (protocol version)
o= (owner/creator and session identifier)
s= (session name)
u= * (URI of description)
c= * (connection information-not required if included in all media)
==Time description:==
t= (time the session is active)
==Media description:==
m= (media name and transport address)
c= * (connection information-optional if included at session-level)
b= * (bandwidth information)
a= * (zero or more media attribute lines)
y= * (SSRC)
f= * (媒体描述)
```



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