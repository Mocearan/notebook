# SIP

​		会话初始协议（Session Initiation Protocol）基于文本进行多媒体通信的应用层控制协议，用于创建、修改和终结一个或多个参与者加入的会话进行。

![image-20240516203738231](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203738231.png)

---

​		SIP协议的引入是为了实现IP电话功能（VoIP）（通过IP网络进行语音通话）。主要借鉴了HTTP协议和SMTP协议 。

​		通过与RTP/RTCP、SDP、RTSP等协议及DNS配合，SIP支持语音、视频、数据、E-mail、状态、IM、聊天、游戏等。SIP协议可在TCP或UDP之上传送，由于SIP本身具有握手机制，可首选UDP。

​		如果要完成一个视频通话或视频会议，首先SIP用于初始化一个Session，并负责传输SDP包；而SDP包中描述了一个Session中包含哪些媒体数据，邀请的人等等；当需要被邀请的人都通过各自的终端设备被通知到后，就可以使用RTSP来控制特定Media的通信，比如RTSP控制信息要求开始Video的播放，那么就开始使用RTP（或者TCP）实时传输数据，在传输过程中，RTCP要负责QoS等。

​		SIP功能：

- User location（用户定位）：确定参加通信的终端用户的位置
- User capabilities（用户能力）：确定通信的媒体类型和参数
- User availability（用户的可用性）：决定被叫方是否愿意参加通信
- Call setup（呼叫建立）：振铃，在主叫和被叫直接建立呼叫的参数
- Call handling（呼叫处理）：包括呼叫转移和终止

![image-20240516185959772](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516185959772.png)



​		支持IP电话的信令协议有两种：

- H.323 是ITU-T制定的在各种网络上提供多媒体通信的系列协议
  - 分组网上提供实时音频、视频和数据通信的标准
- IETF提出的在IP网上提供多媒体业务的标准
  - 具有更加简单易于实现，更容易支持智能终端和实现一些高级功能





## 参考

www.sipforum.org

[beixiaocai/BXC_SipServer: C++开发的国标GB28181流媒体Sip信令服务器 (github.com)](https://github.com/beixiaocai/BXC_SipServer/tree/main)

[VoIP与SIP - 知乎 (zhihu.com)](https://www.zhihu.com/column/c_1446432274048393216)



## 标准

- RFC3261：SIP的基本协议，定义了SIP的基本功能，特性等。要搞SIP的话，这个协议是不能不看的。网上有人已经将它翻译成中文，再结合英文原版协议看，应该比较好理解。
- RFC3262：SIP中，如何定位服务器。这个没过多研究，一般的SIP协议栈都可以很好的支持，让它们去做就可以了。
- RFC3265：事件通知机制，可通过此协议进行一些事件监控。
- RFC3515：呼叫的转接。
- RFC3666：与PSTN连接时的一些特性的说明。
- RFC3911：通过Join的方式进行会议。

## SIP 网络基本结构

![image-20240516203853704](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203853704.png)

- SIP的最低层是其语法和编码。其编码使用扩充的背景 – 诺尔表单语法(BNF)指定。
- 第二层是传输层。它定义了客户端如何发送请求和接收响应，以及服务器如何通过网络接收请求和发送响应。所有SIP元素都包含传输层。
- 接下来是事务层。
  - 事务是由客户机事务(使用传输层)发送到服务器事务的请求，以及从服务器事务发送回客户机的对该请求的所有响应。
  - 用户代理客户端(UAC)完成的任何任务都使用一系列事务进行。
  - 无状态代理不包含事务层。
- 交易层上方的图层称为交易使用者。除了无状态代理，每个SIP实体都是事务用户。

​		SIP是一个C/S协议，客户和服务器之间的操作从第一个请求到最终响应为止的所有消息构成一个SIP事务。

​		SIP基于分布式架构，网络中包含多个服务：

- SIP UA
  - 是一个软终端或者是一个支持ISP协议的电话
  - 对接收到的行为进行代理，发送到SIP网络中 
  - 是发起和终止会话的实体
  - 包含两个功能实体：
    - UAC（User Agent Client） 用 户代理客户端
      - 发起SIP事务请求的功能实体
      - 将事件转换为SIP网络协议请求
    - UAS（User Agent Server）用户代理服务器
      - 接收SIP事务请求的功能实体
      - 将SIP网络协议转换为动作 （如终端上收到 180 Ringing进行振铃）
  - 对于UAC或UAS来说，Sip协议是一种有状态的协议，同一个会话的前后相关的信令必须送到相同的UAC或UAS。
    - Sip协议的via、record-route头域正是用于用来指示路由的字段
    - via用于指示response返回的路径，不会影响后续request。
    - record-route用于指示同一Call-ID后续请求的路径。
    - rport是接收方收到via为内网的请求时，加上 自己看到的发送方的ip（received）和端口（rport），用于NAT。
- Proxy Server 代理服务器
  - Proxy和Agent不同，一般指进行消息路由转发的功能组件
  - 为其他的客户机代理，进行SIP消息的转接和转发功能
  - 对SIP的请求和响应进行路由
- Register Server 注册服务器
  - 接收GEGISTER请求完成用户地址的注册
  - 可以支持用户鉴权功能
- Redirect Server 重定向服务器
  - 是指请求的资源位置发生了变化，通知用户向新的位置发起请求
  - SIP中指将用户新的位置返回给呼叫方，呼叫方根据得到的新位置重新发起呼叫
  - 只对消息进行响应，不向外发起请求消息
- Location Server 位置服务器
  - 是一个数据库，存放终端用户当前的位置信息
  - 为SIP重定向服务器或代理服务器提供被叫用户的位置信息

​		以上这些服务是逻辑组件，在一个实体中可以包含其中多个服务。







## 消息类型

​		SIP消息机制基于C/S方式，消息分为请求和响应。

- 为SIP终端提供定位服务
  - 定位方式： SIP URL
-  进行媒体属性协商
  - 封装协议： SDP（会话描述协议 ， Session Describe Protocol）
- 可以提供一下主要功能
  - 发起会话： INVITE
  - 改变会话：Re-INVITE（重发邀请，Cseq增加）
  - 结束会话：BYE，CANCEL

### 请求消息

| 消息               | 功能                                                         |
| ------------------ | ------------------------------------------------------------ |
| INVITE             | 发起会话请求。                                               |
|                    | 邀请用户加入一个会话，会话描述包含在消息体中。对于两方呼叫来说，主叫方在会话描述中指示其能够接收的媒体类型及参数，被叫方必须在成功响应消息 的消息体中指明其希望接受哪些媒体，还可以指示其将发送的媒体。如果收到的事关于参加会议的邀请，被叫方可以根据Call-ID或者会话描述中的标识确定用户已经加入该会议，并返回成功响应消息。 |
| ACK                | 对INVITE请求的响应消息的确认                                 |
|                    | 证实已经收到的对于INVETE请求的最终响应，该消息只和INVATE消息配套使用 |
| BYE                | 结束会话                                                     |
| CANCEL             | 取消尚未完成的请求，对于已经完成的请求（已经收到最终响应的请求）没有影响 |
| REGISTER           | 注册                                                         |
| OPTIONS            | 查询服务器的能力                                             |
|                    |                                                              |
| INFO               | 传递呼叫中间交互内容                                         |
| COMET              | 怕热condition met                                            |
| PRACK              | 有条件的可靠相应确认（Provisional Reliable Response Acknowledgement） |
| NOTIFY / SUBSCRIBE | 即时消息                                                     |



### 响应消息

| 消息 | 功能                                                         | 举例                             | 备注                                                         |
| ---- | ------------------------------------------------------------ | -------------------------------- | ------------------------------------------------------------ |
| 1xx  | 临时响应                                                     | 100 Trying                       | 呼叫方正在呼叫，但尚未联系到被呼叫方。                       |
|      | 表示已经接收到请求消息，正在对其进行处理                     | 180 Ringing（Processed Locally） | 被呼叫方已被联系到，其铃声正在响。收到此信息后，通常等待200 OK。 |
|      |                                                              | 181 Call Is Being Forward        | 呼叫正在被转发到另一个目的地。                               |
|      |                                                              | 183 Session progress             | 用于警告呼叫方频段（inband）错误。当从PSTN收到一个ISDN消息时，SIP网关会产生此响应。 |
| 2xx  | 成功                                                         | 200 OK                           |                                                              |
|      | 表示请求已经被成功接受，处理                                 |                                  |                                                              |
| 3xx  | 重定向                                                       | 300 Multiple Choices             | 有多个选项可用于处理请求。                                   |
|      | 表示需要采取进一步动作                                       | 301 Moved PerManently            | 请求的资源已永久移动到新的URI。                              |
|      |                                                              | 302 Moved Temporarily            | 请求的资源临时移动到新的URI。                                |
| 4xx  | 客户端错误                                                   | 400  Bad Request                 | 请求无法理解，因为语法有误。                                 |
|      | 表示请求消息中包含语法错误或者SIP服务器不能完成对改请求消息的处理 | 401 Unauthorized                 | 请求需要用户验证。                                           |
|      |                                                              | 404 Not Found                    | 服务器上未找到请求的资源。                                   |
|      |                                                              | 408 Request Timeout              |                                                              |
| 5xx  | 服务端错误                                                   | 500 Server Internal Error        | 服务器遇到意外情况，导致其无法完成请求。                     |
|      | 表示SIP服务器故障，不能完成对正确消息的处理                  | 503 Service Unavailable          | 临时的服务器维护或过载，服务器当前无法处理请求。             |
|      |                                                              | 505 Version Not Supported        |                                                              |
| 6xx  | 全局错误                                                     | 600 Busy Everywhere              |                                                              |
|      | 表示请求不能在当前任何SIP服务器上实现                        | 603 Decline                      |                                                              |



## 消息结构

### SIP地址

​		SIP中的地址采用因特网URL（统一资源定位，全球唯一地址），支持因特网IP地址和PSTN地址

​		地址格式：`sip: name:password@host:port; 传送参数; 用户参数; 方法参数; 生存期参数; 服务器地址参数? 头部名=头部值`

- `sip:alan@wcom.com`
- `sip:J.T.Krik<Kirk@starfleet.gov>` 加nickname
- `sip:guest@10.64.1.2`
- `sip:+1-522-3412@wcom.com;user=phone` 电话用户
- `sip:666000022@127.0.0.1:5061;User=phone; Sip:alice@register.com; method=REGISTER;`



### 一般格式

```http
start-line
message-header
CRLF
[ message-body ]
```



#### 请求格式

```HTTP
<CMD> <URI> <PROTO_VERSION>  首行       INVITE sip:bob.smith@nokia.com SIP/2.0
Call-id: value
Via: value
From: value
To: value
Contact: value
Cseq: value
Content-Type: value
Content-Length: value
Max-Forward: value
<EMPTY LINE> CRLF空行
<body>根据content-type携带的body，一般为SDP
```



#### 响应格式

```http
<PROTO_VERSION> <STATUS_CODE> <REASONL>  首行       SIP/2.0 100 Trying  /  SIP/2.0 183 Session in progress
Call-id: value
Via: value
From: value
To: value
Contact: value
Cseq: value
Content-Type: value
Content-Length: value
Max-Forward: value
<EMPTY LINE> CRLF空行
<body>根据content-type携带的body，一般为SDP
```



#### 消息头

- `from / to ` 
  - `显示名<接收者URI>；tag=n`
  - 所有请求和响应必须包含这两个字段
  - `from`， 请求的发起者，服务器将次字段从请求消息复制到响应消息
    - `From:name <SIP URL>; tag=xxx`
      - `From: "iwf" <sip:5136000@202.202.21.1>; tag=aab7090044b2-195254e9`
    - `tag`是必选的
  - `to`， 请求的接收者
    - `To:<sip:613000@202.202.21.1>`
    - 显示名和`tag`可选
- `Call-ID`
  - 全局的唯一标志，用来唯一确定一个呼叫
  - 通过随机字符串和名字或者IP地址混合产生
  - 在对话中的任一UA的所有请求和所有应答的Call-ID必须一致
- `Cseq`  命令序号、
  - 标识同一会话中不同事务的序号
    - 整个会话操作过程由不同的事务组成，每一事务所涉及的消息的CSeq序号必须相同
  
  - 通常由一个用作序号的整型数和消息类型组成
  - 序号的初始值可以为任意值，其后具有相同Call ID值，但不同请求方法、头部或消息体的请求，Cseq序号应该+1
  - 重发请求的序号保持不变
  - ACK和CANCEL请求的Cseq与对应的INVATE请求相同
  - BYE请求的Cseq值应大于INVATE请求，由代理服务器并行分发的请求，其Cseq值相同
  - 服务器将请求中的Cseq赋值到响应消息中去
  - `Cseq: 101 INVATE`
  
- `Via`
  - 指示请求消息经历的路径
    - 请求消息经过每一跳节点时，每一跳节点都把自身的IP地址信息放入顶层Via中
    - 响应消息则沿着请求消息记录下的传输路径反向传输，首先移走指明自身IP地址信息的顶层消息头
  - 防止请求消息传送环路，确保响应和请求的消息选择同样的路径
  - `Via: 发送协议(proto_name/proto_version/trans-layer) 发送方(send_ip:send_port) 参数 `
  - `Via: SIP/2.0/UDP/ 202.202.41.3:5060`
- `Contace`
  - 用于INVATE、ACK、REGISTER请求以及成功响应、呼叫进展响应和重定向响应消息
  - 作用是给出其后和用户直接通信的地址，就是天蝎最新的联系人地址，可以和当前发消息的地址不同
  - `Contact: sip:412444@202.11.4.9:5060`
- `Max-Forwards` 最大转发次数



![image-20240516202817840](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516202817840.png)

![image-20240516202835012](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516202835012.png)



### SDP 消息体

| Field           | Description                                                  | sample                                             |      |
| --------------- | ------------------------------------------------------------ | -------------------------------------------------- | ---- |
| Version         | `v=0`                                                        | v=0                                                | v    |
| Origin          | `o=<username><session id> <version> <network type> <address type> <address>` | `o=ajohnston +1-232-222-1212 IN IP4 host.wcom.com` | o    |
| Session Name    | `s=<session name>`                                           | `s=Lets Talk`                                      | s    |
| Times           | `t=<start time> <stop time>`                                 | `t=0.0`                                            | t    |
| Connection Data | `c=<network type> <address type> <connecton address>`        | `c=IN IP4 101.43.32.1`                             | c    |
| Media           | `m=<media> <port> <transport> <media format list>`           | `m=audio 49170 RTP/AVP 0 3`                        | m    |



## 消息流程

​		SIP协商中主叫方会带上自己支持的所有音频编码列表到被叫方，被叫方一般在回铃时从主叫支持的类型中选出一种或多种自己支持的编码，返回主叫后，双人按顺序选出第一个支持的编码。

### 会话建立

![image-20240516203601793](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203601793.png)



![image-20240516205800532](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516205800532.png)

![image-20240516205842471](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516205842471.png)

### 代理呼叫

![image-20240516203640447](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203640447.png)



### 重定向呼叫

![image-20240516203711159](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203711159.png)



### 注册流程

![image-20240516205657967](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516205657967.png)



## NAT rport

> 调试asterisk时发现从公网发来的INVITE的响应都不能正常返回到客户端，抓包发现响应都发到via头域中写明的那个port上去了，而实际asterisk是从NAT转换后的地址和端口接收的INVITE请求。
>
> 最终发现需要在via中加入rport字段，使得asterisk使用rport机制路由响应。

1. 客户端需要在`top most via`上带上`rport`参数
2. 服务端发现`via`中有`rport`参数时，将此`via`中加入`received`参数和`rport`参数
   1. 参数带的值分别是服务端看到的消息来源地址和端口（如果有NAT等地址转换设备，则即为转换后的IP和port）。
3. 当有需要发送的响应时，按照`rport`中的端口发送`SIP `响应。
   1. 也就是说IP和端口均完全遵照从哪里来的，发回哪里去的原则。
   2. 如果没有`rport`字段时，服务端的策略是IP使用`UDP`包中的地址，即从哪里来回哪里去，但是端口使用的`via`中的端口

### RFC文档中的例子：

- 客户端发送INVITE形如：

  ```http
  INVITE sip:user@example.com SIP/2.0
  Via: SIP/2.0/UDP 10.1.1.1:4540;rport;branch=z9hG4bKkjshdyff
  ```

- 服务端收到后根据自己所得到的源地址和端口，重写via，如有必要添加自己的`via`转发
  ```http
  INVITE sip:user@example.com SIP/2.0
  Via: SIP/2.0/UDP proxy.example.com;branch=z9hG4bKkjsh77
  Via: SIP/2.0/UDP 10.1.1.1:4540;received=192.0.2.1;rport=9988;branch=z9hG4bKkjshdyff
  ```

- 当需要给此请求发送响应时，发往`received,rport`两个参数记录的`IP`和`PORT`。

  - NAT设备再把发往转换后地址`192.0.2.1`的包发往`10.1.1.1`地址

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 10.1.1.1:4540;received=192.0.2.1;rport=9988;branch=z9hG4bKkjshdyff
  ```

  



## SIP协议栈实现

- PJSIP协议栈
  - 开源的SIP协议栈
  - 同时支持音频、视频并支持即时通讯
  - 具有非常完善的文档，对开发者非常友好，是开发即时通讯系统的首选
  - 具有非常好的移植性，几乎支持现今所有的操作系统系统：从桌面系统、嵌入式系统到智能手机
- ReSIProcate协议栈
  - SIPFoundry的开源项目
  - 在VOCAL的基础上建立的
    - 由于VOCAL开始只支持rfc3254，为了支持最新的rfc3261,ReSIProcate协议栈就这样诞生了
    - 但现在，ReSIProcate已经成为一个独立SIP协议栈了，它性能较为稳定，并且很多商业的应用都使用它
- OPAL协议栈
  - OPAL（Open Phone Abstraction Library）的前身是Openh323开源项目
    - 它包括几乎全部的Openh323全部代码
    - 并加入了SIP协议栈，使到H.323和SIP协议能并存
    - 开发者既可以使用功能全面的H.323协议，可以使用简单易用的SIP协议
  - Openh323是视频会议厂商最青睐的开源的H.323的开源项目
    - 很多免费视频会议系统的H.323协议栈都是采用Openh323
    - OPAL的出现使系统能支持SIP协议
- VOCAL协议栈
  - VOCAL项目vovida.org开发的开源SIP系统
  - VOCAL的SIP协议栈应该是目前功能最完善的SIP协议栈之一，其具有众多的使用者
  - 不支持window平台，所以限制了它的普及和推广
  - 在其他的linux操作系统上是最具有影响力的SIP协议栈
- sipX协议栈
  - 开源的SIP协议栈
  - 它和ReSIProcate都是由SIPFoundry开发
  - sipX是从reSIProcate分离出来的
    - sipX除了包括SIP 协议外，还包括了sipXphone,sipXproxy,sipXregistry等
    - 由它们构成了完整的SIP系统
    - 而且sipx还支持嵌入式系统，各个模块可以按需取舍
- oSIP协议栈
  - 使用ANSI C编写的开源SIP协议栈
  - 是体积最小的SIP协议栈
    - oSIP体积较小，很容易在小的操作系统上运行
    - 因此在实时操作系统 VxWorks当中，oSIP是使用最多的SIP协议栈



## Sip测试工具

SIPp是sip协议栈的一个免费开源的测试工具, 它模拟了一些基本的uac和uas功能，并建立和释放多个调用的INVITE和BYE的方法，可以读取xml和csv文件生成注册或呼叫流程，可以动态显示统计信息和动态调整呼叫速率。

模拟UAS：./sipp -sn uas -i 192.168.1.249 -p 5077 //监听5077端口

模拟UAC:./sipp -sn uac 192.168.1.249:5077 -i 192.168.1.249 -p 5078 -r 50 -rp 1000  //每1000ms发送50个呼叫

SIPp还支持使用脚本文件来自定义模拟的UAC或UAS的行为，具体可参考官网文档：http://sipp.sourceforge.net/index.html
