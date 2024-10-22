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



## SIP机制

​		SIP是一个分层结构的协议，它的行为根据一组平等独立的处理阶段来描述，每个阶段之间是松耦合的关系。

​		SIP是一个C/S协议，客户和服务器之间的操作从第一个请求到最终响应为止的所有消息构成一个SIP事务。

- SIP的最低层是其语法和编码。其编码使用扩充的背景 – 诺尔表单语法(BNF)指定。
- 第二层是传输层。它定义了客户端如何发送请求和接收响应，以及服务器如何通过网络接收请求和发送响应。所有SIP元素都包含传输层。
- 接下来是事务层。
  - 事务是由客户机事务(使用传输层)发送到服务器事务的请求，以及从服务器事务发送回客户机的对该请求的所有响应。
  - 用户代理客户端(UAC)完成的任何任务都使用一系列事务进行。
  - 无状态代理不包含事务层。
- 交易层上方的图层称为交易使用者。除了无状态代理，每个SIP实体都是事务用户。



## SIP 网络基本结构

![image-20240516203853704](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203853704.png)

​		使用SIP建立通信网络，需要有一些实体的支持，本节主要介绍下这些实体都有哪些。在SIP中，每个网络元件由类似地址的 SIP URI (统一资源标识符)标识。

​		SIP基于分布式架构，网络中包含多个服务。这些服务是逻辑组件，在一个实体中可以包含其中多个服务。

### 用户代理

​		用户代理（UA，User Agent）也称SIP终端，是指支持SIP协议的多媒体会话终端。一般使用支持SIP协议的路由器作为SIP UA。是一个软终端或者是一个支持ISP协议的电话，对接收到的行为进行代理，发送到SIP网络中 。

​		UA 包括用户代理客户端（UAC，User Agent Client）和用户代理服务器（UAS，User Agent Server）。一般说的 UA 均是指二者的总称，因为在一次呼叫中，一个 SIP 终端既要处理 SIP 请求，又要发起SIP请求。

- **用户代理客户端（UAC）**是指在 SIP 会话建立过程中主动发送会话请求的设备。
  - 主叫SIP终端。当代理服务器向被叫终端发送会话请求时，它就成为用户代理客户端。
  - 发起SIP事务请求的功能实体
  - 将事件转换为SIP网络协议请求
- **用户代理服务器（UAS）**是指在 SIP 会话建立过程中接收会话请求的设备。
  - 被叫 SIP 终端。当代理服务器接收主叫终端发送会话请求时，也作为用户代理服务器。
  - 接收SIP事务请求的功能实体
  - 将SIP网络协议转换为动作 （如终端上收到 180 Ringing进行振铃）

​		

​		对于UAC或UAS来说，Sip协议是一种有状态的协议，同一个会话的前后相关的信令必须送到相同的UAC或UAS。

- Sip协议的via、record-route头域正是用于用来指示路由的字段
- via用于指示response返回的路径，不会影响后续request。
- record-route用于指示同一Call-ID后续请求的路径。
- rport是接收方收到via为内网的请求时，加上 自己看到的发送方的ip（received）和端口（rport），用于NAT。

### 代理服务器

​		代理服务器（Proxy Server）的作用就是传递主叫 UA 发送的会话请求到被叫 UA，然后将被叫 UA的响应传递回主叫 UA，它相当于主叫 UA 和被叫 UA 之间传递会话消息的路由。

- Proxy和Agent不同，一般指进行消息路由转发的功能组件
- 为其他的客户机代理，进行SIP消息的转接和转发功能，对SIP的请求和响应进行路由
- 代理服务器位于两个用户代理之间
  - 源和目标之间最多可以有70个代理服务器
  - 代理服务器接收到主叫UA的会话请求后，先在位置服务器查找UA的位置及主叫和被叫UA之间的呼叫策略信息，找到相应的UA并允许呼叫的UA，代理服务器才会向被叫UA发送会话请求。

​		有两种类型的代理服务器：

- **无状态代理服务器（stateless）**，仅转发接收的消息。这种类型的服务器不存储呼叫或事务的任何信息。
- **状态代理服务器（stateful）**，会跟踪收到的每个请求和响应，如果需要，将来可以使用它。如果没有来自另一方的响应，它可以重传请求。



### 注册服务器

​		注册服务器（Registrar Server）接受来自用户代理的注册请求。它帮助用户在网络中验证自己。它将URI和用户的位置存储在数据库中，以帮助同一域中的其他SIP服务器。

​		注册的内容（如本地号码等信息）一般是存储在位置服务器上，供后续查询使用，二者都是逻辑组	件，一般存在于同一台服务器上，或者同域的一个数据库内。

- 接收GEGISTER请求完成用户地址的注册
- 可以支持用户鉴权功能

### 重定向服务器

​		重定向服务器（Redirect Server）用来指示客户端连接的新地址，客户端直接再次向这些新的地方发起请求。不接收或者拒绝呼叫，主要完成路由功能。

​		比如，主叫UA呼叫被叫UA，当重定向服务器收到主叫UA发送的会话请求消息后，查找被叫UA的位置信息，然后将其返回给主叫UA，使主叫UA重新向该位置发起会话请求。此位置可以是被叫UA的位置，也可以是一个代理服务器的位置。接下来主叫UA如同直接呼叫被叫UA或者向代理服务器呼叫的流程一样。

- 是指请求的资源位置发生了变化，通知用户向新的位置发起请求
- SIP中指将用户新的位置返回给呼叫方，呼叫方根据得到的新位置重新发起呼叫
- 只对消息进行响应，不向外发起请求消息

### 位置服务器

​		位置服务器（Location Server）是为代理服务器和重定向服务器等提供UA信息的设备，只有代理服务器或重定向服务器可以联系位置服务器。

​		位置服务器记录了注册服务器接收到的UA的信息。二者可以独立存在，也可以作为逻辑组件存在于同一台服务器上。

- 是一个数据库，存放终端用户当前的位置信息
- 为SIP重定向服务器或代理服务器提供被叫用户的位置信息









## 消息类型

​		SIP消息机制基于C/S方式，消息分为请求和响应。

- 为SIP终端提供定位服务
  - 定位方式： SIP URL
- 进行媒体属性协商
  - 封装协议： SDP（会话描述协议 ， Session Describe Protocol）
- 可以提供一下主要功能
  - 发起会话： INVITE
  - 改变会话：Re-INVITE（重发邀请，Cseq增加）
  - 结束会话：BYE，CANCEL

### 请求消息

​		起始行格式：`命令名称+目标URI+sip协议版本`

#### 请求方法

| 消息      | 功能                                                         |
| --------- | ------------------------------------------------------------ |
| INVITE    | 发起会话请求。                                               |
|           | 邀请用户加入一个会话，会话描述包含在消息体中。对于两方呼叫来说，主叫方在会话描述中指示其能够接收的媒体类型及参数，被叫方必须在成功响应消息 的消息体中指明其希望接受哪些媒体，还可以指示其将发送的媒体。如果收到的事关于参加会议的邀请，被叫方可以根据Call-ID或者会话描述中的标识确定用户已经加入该会议，并返回成功响应消息。 |
| ACK       | 对INVITE请求的响应消息的确认                                 |
|           | 证实已经收到的对于INVETE请求的最终响应，该消息只和INVATE消息配套使用 |
| BYE       | 结束会话                                                     |
| CANCEL    | 取消尚未完成的请求，对于已经完成的请求（已经收到最终响应的请求）没有影响 |
| REGISTER  | 注册                                                         |
| OPTIONS   | 查询服务器的能力                                             |
| PRACK     | 类似ACK，但只是一个临时的确认                                |
| INFO      | 传递呼叫中间交互内容                                         |
| COMET     |                                                              |
| PRACK     | 有条件的可靠相应确认（Provisional Reliable Response Acknowledgement） |
| NOTIFY    | 通知所有用户事件                                             |
| SUBSCRIBE | 新客户端加入事件通知                                         |
| PUBLISH   | 发送事件到服务器                                             |
| REFER     | 要求客户端发出一请求，通常应用于一个呼叫转移                 |
| MESSAGE   | 发送即时消息的方法                                           |
| UPDATE    | 修改session的状态不改变dialog的状态                          |



### 响应消息

​		起始行格式：`sip协议版本+响应返回码+描述性短句`

| 消息 | 功能                                                         | 举例                             | 备注                                                         |
| ---- | ------------------------------------------------------------ | -------------------------------- | ------------------------------------------------------------ |
| 1xx  | 临时响应                                                     | 100 Trying                       | 呼叫方正在呼叫，但尚未联系到被呼叫方。                       |
|      | 表示已经接收到请求消息，正在对其进行处理                     | 180 Ringing（Processed Locally） | 被呼叫方已被联系到，其铃声正在响。收到此信息后，通常等待200 OK。 |
|      |                                                              | 181 Call Is Being Forward        | 呼叫正在被转发到另一个目的地。                               |
|      |                                                              | 182                              | 排队                                                         |
|      |                                                              | 183 Session progress             | 用于警告呼叫方频段（inband）错误。当从PSTN收到一个ISDN消息时，SIP网关会产生此响应。 |
| 2xx  | 成功                                                         | 200 OK                           | 表示请求已经被成功接受，处理                                 |
|      |                                                              |                                  |                                                              |
| 3xx  | 重定向                                                       | 300 Multiple Choices             | 有多个选项可用于处理请求。                                   |
|      | 表示需要采取进一步动作                                       | 301 Moved PerManently            | 请求的资源已永久移动到新的URI。                              |
|      |                                                              | 302 Moved Temporarily            | 请求的资源临时移动到新的URI。                                |
|      |                                                              | 303                              | 见其它                                                       |
|      |                                                              | 305                              | 使用代理                                                     |
|      |                                                              | 380                              | 代换服务                                                     |
| 4xx  | 客户端错误                                                   | 400  Bad Request                 | 请求无法理解，因为语法有误。                                 |
|      | 表示请求消息中包含语法错误或者SIP服务器不能完成对改请求消息的处理 | 401 Unauthorized                 | 请求需要用户验证。                                           |
|      |                                                              | 401                              | 无权                                                         |
|      |                                                              | 402                              | 要求付款                                                     |
|      |                                                              | 403                              | 禁止                                                         |
|      |                                                              | 404 Not Found                    | 服务器上未找到请求的资源。                                   |
|      |                                                              | 408 Request Timeout              |                                                              |
|      |                                                              | 405                              | 不允许的方法                                                 |
|      |                                                              | 406                              | 不接受                                                       |
|      |                                                              | 407                              | 要求代理权                                                   |
|      |                                                              | 408                              | 请求超时                                                     |
|      |                                                              | 410                              | 消失                                                         |
|      |                                                              | 413                              | 请求实体太大                                                 |
|      |                                                              | 414                              | 请求URI太大                                                  |
|      |                                                              | 415                              | 不支持的媒体类型                                             |
|      |                                                              | 416                              | 不支持的URI方案                                              |
|      |                                                              | 420                              | 分机无人接听                                                 |
|      |                                                              | 421                              | 要求转机                                                     |
|      |                                                              | 423                              | 间隔太短                                                     |
|      |                                                              | 480                              | 暂时无人接听                                                 |
|      |                                                              | 481                              | 呼叫腿/事务不存在                                            |
|      |                                                              | 482                              | 相环探测                                                     |
|      |                                                              | 483                              | 跳频太高                                                     |
|      |                                                              | 484                              | 地址不完整                                                   |
|      |                                                              | 485                              | 不清楚                                                       |
|      |                                                              | 486                              | 线路忙                                                       |
|      |                                                              | 487                              | 终止请求                                                     |
|      |                                                              | 488                              | 此处不接受                                                   |
|      |                                                              | 491                              | 代处理请求                                                   |
|      |                                                              | 493                              | 难以辨认                                                     |
| 5xx  | 服务端错误                                                   | 500 Server Internal Error        | 服务器遇到意外情况，导致其无法完成请求。                     |
|      | 表示SIP服务器故障，不能完成对正确消息的处理                  | 501                              | 没实现的                                                     |
|      |                                                              | 502                              | 无效网关                                                     |
|      |                                                              | 503 Service Unavailable          | 临时的服务器维护或过载，服务器当前无法处理请求。             |
|      |                                                              | 504                              | 服务器超时                                                   |
|      |                                                              | 505 Version Not Supported        |                                                              |
|      |                                                              | 513                              | 消息太长                                                     |
| 6xx  | 全局错误                                                     | 600 Busy Everywhere              |                                                              |
|      | 表示请求不能在当前任何SIP服务器上实现                        | 603 Decline                      | 拒绝                                                         |
|      |                                                              | 604                              | 都不存在                                                     |
|      |                                                              | 606                              | 不接受                                                       |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |
|      |                                                              |                                  |                                                              |



## 消息结构

​		SIP消息采用[ISO 10646]文本方式编码，分为两类：请求消息和响应消息。
（1）请求消息：客户端为了激活按特定操作而发给服务器的SIP消息。
（2）响应消息：用于对请求消息进行响应，指示呼叫的成功或失败状态。

​		每条SIP消息由以下三部分组成：起始行（ Start Line），SIP头，消息体；请求消息和响应消息都包括SIP头字段和SIP消息字段。

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

### 注册流程

​		所有的SIP终端作为User Agent都应该向注册服务器登记注册，以告知其位置、会话能力、呼叫策略等信息。

​		通常，SIP终端开机启动或者配置管理员执行注册操作时，就向注册服务器发送注册请求消息（REGISTER），该消息中携带了所有需要登记的信息。注册服务器收到注册请求消息后向终端发送回应消息，以告知其请求消息已收到。如果注册成功，就再向终端发送“200 OK”消息。

![image-20240516205657967](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516205657967.png)

### 会话建立



![image-20240516203601793](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203601793.png)



![image-20240516205800532](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516205800532.png)

![image-20240516205842471](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516205842471.png)

### 代理呼叫

​		SIP协议采用Client/Server模型，主要通过UA与代理服务器之间的通信来完成用户呼叫的建立过程。

- Telephone A需要呼叫电话Telephone B，两台路由器作为SIP终端（UA）。
- 当Telephone A拨完电话Telephone B的号码后，Router A向Proxy Server发送会话请求消息。
- Proxy Server通过查找Telephone B的号码所对应的信息，向Router B发送会话请求消息。
- Router B收到请求后，如果Telephone B可用，就向Proxy Server发送应答，并使Telephone B振铃。
- Proxy Server收到应答后，向Router A发送应答消息。
- 这里所说的应答包括：两个临时应答（100 Trying 和180 Ringing）和一个成功应答（200 OK）。

​		这个例子是一种简单的应用，只使用了一个代理服务器。在复杂的应用中，可以有多个代理服务器，以及注册服务器。

![image-20240516203640447](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203640447.png)



### 重定向呼叫

​		SIP重定向服务器收到会话请求消息后，不是转发会话请求消息，而是在回应消息中告知被叫SIP终端的地址。主叫终端从而重新直接向被叫终端发送会话请求消息。被叫终端也将直接向主叫终端发送应答消息。

​		从原理上来说，重定向服务器也可以向主叫终端回复一个代理服务器的地址，接下来的呼叫过程就和使用代理服务器的呼叫过程一样。

![image-20240516203711159](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203711159.png)







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
