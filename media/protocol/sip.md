# SIP

​		会话启动协议SIP（Session Initiation Protocol）是一个在IP网络上基于文本进行多媒体通信的应用层控制协议，用于创建、修改和终结一个或多个参与者加入的会话进行。

www.sipforum.org

![image-20240516203738231](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203738231.png)

---

​		SIP协议的引入是为了实现IP电话功能（VoIP）（通过IP网络进行语音通话）。主要借鉴了HTTP协议和SMTP协议 

​		功能：

- 用户定位：确定参加通信的终端用户的位置
- 用户通信能力协商：确定通信的媒体类型和参数
- 用户意愿交互：确定被叫是否同意加入某个会话
- 建立呼叫：包括向被叫方振铃，确定主叫和被叫的呼叫参数
- 呼叫处理和控制：包括呼叫重定向，呼叫转移（转发），终止呼叫等

![image-20240516185959772](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516185959772.png)



​		支持IP电话的信令协议有两种：

- H.323 是ITU-T制定的在各种网络上提供多媒体通信的系列协议
  - 分组网上提供实时音频、视频和数据通信的标准
- IETF提出的在IP网上提供多媒体业务的标准
  - 具有更加简单易于实现，更容易支持智能终端和实现一些高级功能

​		

​		SIP是一个C/S协议，客户和服务器之间的操作从第一个请求到最终响应为止的所有消息构成一个SIP事务。



## 参考

[beixiaocai/BXC_SipServer: C++开发的国标GB28181流媒体Sip信令服务器 (github.com)](https://github.com/beixiaocai/BXC_SipServer/tree/main)



## SIP 网络基本结构

![image-20240516203853704](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240516203853704.png)

​		SIP基于分布式架构，网络中包含多个服务：

- UA
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



​		SIP协议简单，只包括6个主要请求和6类响应，基于文本格式，便于跟踪和处理，抑郁扩展和伸缩



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

| 消息 | 功能                                                         | 举例                             |
| ---- | ------------------------------------------------------------ | -------------------------------- |
| 1xx  | 临时响应                                                     | 100 Trying                       |
|      | 表示已经接收到请求消息，正在对其进行处理                     | 180 Ringing（Processed Locally） |
|      |                                                              | 181 Call Is Being Forward        |
| 2xx  | 成功                                                         | 200 OK                           |
|      | 表示请求已经被成功接受，处理                                 |                                  |
| 3xx  | 重定向                                                       | 300 Multiple Choices             |
|      | 表示需要采取进一步动作                                       | 301 Moved PerManently            |
|      |                                                              | 302 Moved Temporarily            |
| 4xx  | 客户端错误                                                   | 401 Unauthorized                 |
|      | 表示请求消息中包含语法错误或者SIP服务器不能完成对改请求消息的处理 | 408 Request Timeout              |
| 5xx  | 服务端错误                                                   | 503 Service Unavailable          |
|      | 表示SIP服务器故障，不能完成对正确消息的处理                  | 505 Version Not Supported        |
| 6xx  | 全局错误                                                     | 600 Busy Everywhere              |
|      | 表示请求不能在当前任何SIP服务器上实现                        | 603 Decline                      |



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



#### 重要字段

- `from / to ` 
  - 所有请求和响应必须包含这两个字段
  - `from`， 请求的发起者，服务器将次字段从请求消息复制到响应消息
    - `From:name <SIP URL>; tag=xxx`
      - `From: "iwf" <sip:5136000@202.202.21.1>; tag=aab7090044b2-195254e9`
  - `to`， 请求的接收者
    - `To:<sip:613000@202.202.21.1>`
- `Call-ID`
  - 包含一个全局的唯一标志，用来唯一确定一个呼叫
  - 通过随机字符串和名字或者IP地址混合产生
  - 在对话中的任一UA的所有请求和所有应答的Call-ID必须一直
- `Cseq`  命令序号
  - 客户在每个请求中应该如此字段，由请求方法和一个十进制序号组成
  - 序号的初始值可以为任意值，其后具有相同Call ID值，但不同请求方法、头部或消息体的请求，Cseq序号应该+1
  - 重发请求的序号保持不变
  - ACK和CANCEL请求的Cseq与对应的INVATE请求相同
  - BYE请求的Cseq值应大于INVATE请求，由代理服务器并行分发的请求，其Cseq值相同
  - 服务器将请求中的Cseq值赋值到响应消息中去
  - `Cseq: 101 INVATE`
- `Via`
  - 指示请求消息经历的路径
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

