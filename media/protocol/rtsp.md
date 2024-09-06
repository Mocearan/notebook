# RTSP

​	

---

​		`RTSP(Real Time Streaming Protocol)`，实时流传输协议, 是一个应用层协议，由哥伦比亚大学、网景和RealNetworks公司提交的IETF RFC标准。

​		定义了一对多应用程序如何有效地通过IP网络传送多媒体数据。RTSP在体系结构上位于RTP和RTCP之上, 它使用TCP或RTP完成数据传输。

- 遵循规范IETF RFC 2326,4567,6064，是基于文本的协议，语法和操作方法参考了HTTP/1.1
- 采用ISO10646字符集，使用UTF-8编码
- 提供一 种可扩展的框架，使能够提供可控制的，按需传输实时数据，比如音频和视频文件
- 源数据可以包括现场数据的反馈和存贮的文件
- rtsp对流媒体提供了诸如暂停，快进等控制，本身并不负责传输数据
  - rtsp作用相当于流媒体服务器的远程控制
- 允许同时多个串流需求控制(Multicast)
  - 时间上同步
- 选择发送通道,，传输数据可以通过传输层的tcp，udp协议
  - rtsp也提供了基于rtp传输机制的一些有效的方法
  - 承载RTSP的信令传输层协议
    - 可以为TCP，默认端口554
    - 也可以是RTSP-over-HTTP tunneling，则默认TCP端口为8080
  - 实时流数据由不同的协议传输，比如`RTP/RTCP完`成数据流和控制命令的传输
- 可以缓存，可以重定向
- 基于文本的实时流协商协议



![RTSP相关协议](https://raw.githubusercontent.com/Mocearan/picgo-server/main/01d27a729117e79f6a074bd7e2a18568.png)



## 协议特点

- 可扩展性: 新方法和参数很容易加入RTSP.
- 易解析: RTSP可由标准HTTP或MIME解析器解析.
- 安全: RTSP使用网页安全机制.
- 独立于传输: RTSP可使用不可靠数据报协议(EDP), 可靠数据报协议(RDP); 如要实现应用级可靠, 可使用可靠流协议.
- 多服务器支持: 每个流可放在不同服务器上, 用户端自动与不同服务器建立几个并发控制连接, 媒体同步在传输层执行.
- 记录设备控制: 协议可控制记录和回放设备.
- 流控与会议开始分离: 仅要求会议初始化协议提供, 或可用来创建惟一会议标识号. 特殊情况下, 可用SIP或H.323来邀请服务器入会.
- 适合专业应用: 通过SMPTE时标, RTSP支持帧级精度, 允许远程数字编辑.
- 演示描述中立: 协议没强加特殊演示或元文件, 可传送所用格式类型; 然而, 演示描述至少必须包括一个RTSP URL.
- 代理与防火墙友好: 协议可由应用和传输层防火墙处理. 防火墙需要理解SETUP方法, 为UDP媒体流打开一个“缺口”.
- HTTP友好: 此处, RTSP明智地采用HTTP观念, 使现在结构都可重用. 结构包括Internet内容选择平台(PICS). 由于在大多数情况下控制连续媒体需要服务器状态, RTSP不仅仅向HTFP添加方法.
- 适当的服务器控制: 如用户启动一个流, 必须也可以停止一个流.
- 传输协调: 实际处理连续媒体流前, 用户可协调传输方法.
- 性能协调: 如基本特征无效, 必须有一些清理机制让用户决定哪种方法没生效. 这允许用户提出适合的用户界面.



## 协议格式

​		语法和运作跟HTTP 1.1类似， 但并不特别强调时间同步,，所以比较能容忍网络延迟。

- HTTP传送HTML，RTSP传送的是多媒体数据。
-  HTTP请求由客户机发出, 服务器作出响应；使用RTSP时, 客户机和服务器都可以发出请求, 即RTSP可以是双向的。

### 请求消息

```shell
方法 URI RTSP版本 CR LF 
消息头 CR LF 
CR LF （空行）
消息体 CR LF 

# CR LF表示回车换行
# RTSP版本一般都是 RTSP/1.0
```

​		

### 响应应消息

```shell
RTSP版本 状态码 reason CR LF 
消息头 CR LF 
CR LF （空行）
消息体 CR LF

# 状态码是一个数值, 200表示成功
# reson 是与状态码对应的文本解释
```



### URL

​		`rtsp_url = ("rtsp:" | "rtspu:" | "rtsps:")//host:[port]/[abs_path]/content_name`

- rtsp:**使用可信的底层传输协议，例如TCP**
- rtspu:**使用不可信的底层传输协议，例如UDP**
- rtsps:**使用可信加密传输协议，例如TCP + TLS**
- host：可以是一个有效的域名或是IP地址。
- port：端口号，对于RTSP协议来说，**缺省的端口号为554**。当我们在确认流媒体服务器提供的端口号为554时，此项可以省略 说明：当HMS服务器使用的端口号为554时，我们在写点播链接时，可以不用写明端口号，但当使用非554端口时，在RTSP URL中一定要指定相应的端口。
- abs_path: 为`RTSPServer`中的媒体流资源标识
- RTSPURL用来标识RTSPServer的媒体流资源，可以标识单一的媒体流资源，也可以标 识多个媒体流资源的集合。

```http
rtsp://192.168.1.67:554/test
// 海康网络摄像头的RTSP地址格式为
rtsp://[username]:[password]@[ip]:[port]/[codec]/[channel]/[subtype]/av_stream
rtsp://admin:12345@192.168.1.67:554/h264/ch1/main/av_stream

rtsp://media.example.com:554/twister/audiotrack
```



### 方法 `function`

```

  Method      =   “DESCRIBE” 
          |   “ANNOUNCE”
          |   “GET_PARAMETER”
          |   “OPTIONS”
          |   “PAUSE”
          |   “PLAY”
          |   “RECORD”
          |   “REDIRECT”
          |   “SETUP”
          |   “SET_PARAMETER”
          |   “TEARDOWN"
```



#### `OPTIONS`

​		目的是获取服务器能够提供的可用方法:

```http
OPTIONS rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 1 		# 每个消息都有序号来标记，第一个包通常是option请求消息 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)   # 客户端使用的软件类型
```

​		服务器的回应信息包括提供的一些方法,例如:

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 1 		# 每个回应消息的cseq数值和请求消息的cseq相对应 
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE, GET_PARAMETER 	# 服务器提供的可用的方法 
```



#### `DESCRIBE`

​		C向S发起`DESCRIBE`请求，为了得到会话描述信息(SDP)。

​		客户端发送`describe`时，一般服务器会进行用户鉴权：

- 如果未携带`Authorization`鉴权信息或者认证失败，服务器会返回错误号为401的响应。
  - 客户端接收到401响应时，需要根据已知的用户鉴权信息，生成`Authorization`，再次发送`describe`
- 如果认证成功，服务器返回携带有SDP的响应信息。

> ​		根据携带的SDP信息，判断流媒体的 编码格式/封装格式`/payloadtype/timescale`，track标识等信息，客户端保存此信息用于后续码流的解复用和解码，根据track标识去申请想要的类型的码流。

```http
DESCRIBE rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 2 
token: 
Accept: application/sdp 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

​		服务器回应一些对此会话的描述信息(sdp):

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 2 
x-prev-url: rtsp://192.168.20.136:5000 
x-next-url: rtsp://192.168.20.136:5000 
x-Accept-Retransmit: our-retransmit 
x-Accept-Dynamic-Rate: 1 
Cache-Control: must-revalidate 
Last-Modified: Fri, 10 Nov 2006 12:34:38 GMT 
Date: Fri, 10 Nov 2006 12:34:38 GMT 
Expires: Fri, 10 Nov 2006 12:34:38 GMT 
Content-Base: rtsp://192.168.20.136:5000/xxx666/ 
Content-Length: 344 
Content-Type: application/sdp 

v=0 #以下都是sdp信息 
o=OnewaveUServerNG 1451516402 1025358037 IN IP4 192.168.20.136 
s=/xxx666 
u=http:/// 
e=admin@ 
c=IN IP4 0.0.0.0 
t=0 0 
a=isma-compliance:1,1.0,1 

a=range:npt=0- 
m=video 0 RTP/AVP 96 # m(media)表示媒体描述，下面是对会话中视频通道的媒体描述 
a=rtpmap:96 MP4V-ES/90000 
a=fmtp:96 
profile-level-id=245;config=000001B0F5000001B509000001000000012000C888B0E0E0FA62D089028307 

a=control:trackID=0 # trackID＝0表示视频流用的是通道0 
```

> ​	`fmtp`，format type，要根据码流的真实信息填写，不要随意填写。
>
> ​		在网页无插件播放时，越来越多的播放插件对这个字段要求很严格，因为网页客户端解码时，一般通过此字段来初始化解码库，所以此字段的填写，需要注意，要根据真实的编码参数来编写。

#### `SETUP`

​		客户端提醒服务器建立会话,并确定传输模式。

​		客户端通过`setup`来建立码流通道：

- 根据流媒体服务器返回的SDP描述信息建立码流传输通道
- 一个`setup`调用只能给一种媒体源建立码流传输通道
  - 如果SDP描述有多路媒体源，客户端需要根据不同的`track id`调用`setup`方法多次
- 一般`setup`建立通道可以是`rtp/avp`或者`rtp/avp/tcp`类型
  - 这里标识以`rtp`传输音视频，前者基于UDP传输，后者基于TCP传输
  - 两种类型在`Transport`头上有一定区别
    - 基于UDP传输码流时
      - 需要通过`client_port/server_port`来沟通两端UDP端口
      - 根据`setup`交互的信息，单独建立`RTP/RTCP`两路UDP通道
    - 如果是TCP
      - RTP与RTSP公用一个TCP通道
        - 因为码流传输与信令交互公用一个连接，不需要建立新的连接
        - 为了区分RTP、RTCP、RTSP的协议包
          - 通过在RTP/RTCP包头加上``$``开头的四个字节的``tcpheader``来区分是哪一路的`RTP/RTCP`，还是`RTSP`
          - `| magic number | channel number | embedded data length | data |`
            - `magic number`： 1 byte value of hex 0x24（$），标识传输的是数据不是rtsp协议
            - `channel number`： 1 byte value to denote the channel，信道ID，标识流的类型
            - `embedded data length `：2 bytes to denote the embedded data length，流长度
            - `data`: 表示RTP/RTCP包数据
          - 当TCP接收到的包开头为24时，可以判定其为RTP或者RTCP
            - `24 00 00 14`
            - RTP包长度为0x14，解析时只需要根据streamid及长度读取完整的RTP帧，去掉四字节头，通过RTP方式解析即可。
      - 没有`client_port`字段，出现`interleaved`字段
        - `interleaved=0-1`表示`streamid`
        - 通过`streamid`来区分数据包，`setup`方法中`interleaved=0-1`，标识RTP的`streamid=0`；RTCP的`streamid=1`
- 如果服务器需要认证，`setup`也需要带上认证信息`Authorization`

![rtp/avp](https://img-blog.csdnimg.cn/img_convert/4ac6b3dd55fa9ecddd5131cf55b3405b.png)

![rtp/avp/tcp](https://raw.githubusercontent.com/Mocearan/picgo-server/main/a91ba7b8489399dc0de2726f4056e9f8.png)

```http
SETUP rtsp://192.168.20.136:5000/xxx666/trackID=0 RTSP/1.0 
CSeq: 3 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

- `uri`中带有`trackID＝0`，表示建立id为0的码流传输通道
- `Transport`参数设置了
  - 传输模式
    - 如果是udp传输模式，则还会有`client_port=63538-63539`
      - 表示客户端侧此路码流的RTP端口为UDP63538，RTCP端口为UDP63539
  - 包的结构
    - 支持单播和组播
    - 后续数据包头部第二个字节位置就是`interleaved`
    - 接受端根据`interleaved`的值来区别是哪种数据包
    - 它的值是每个通道都不同的，`trackID＝0`的`interleaved`值有两个0或1
      - 0表示rtp包
      - 1表示rtcp包

​		服务器回应信息:

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 3 
Session: 6310936469860791894 //服务器回应的会话标识符 
Cache-Control: no-cache 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=6B8B4567 
```

- 服务器响应时，`Transport`字段中要包含客户端申请的信息
  - udp时则增加`server_port`字段，表示服务器发送RTP/RTCP的UDP端口
  - 可选增加`ssrc`表示
- 服务器端回复时增加`Session`ID，后续消息必须带上`Session`字段

#### `PLAY`

​		客户端应该在所有的`setup`消息被成功解析后发送`PLAY`请求。

- 在`range`中指定媒体的播放时间
  - 服务器在接到`PLAY`消息后会由`range`中指定的开始点开始发送媒体数据直到`range`中指定的结束点
  - 对于实时流`range`一般为`Range: npt=0.000-`
- `PLAY`可带有scale和speed字段用于点播速度控制。

```http
PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 4 
Session: 6310936469860791894 
Range: npt=0.000- //设置播放时间的范围 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

​		服务器回应信息:

​		服务器接收到play后，即可打开码流发送通道，发送码流。这里要注意服务器在给出play响应时，最好带有`RTP-Info`字段描述将要发送码流的RTP信息，比如第一包RTP的`seq`和`rtptime`，客户端可以根据此字段进行解复用

- `seq`和`rtptime`都是rtp包中的信息 

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 4 
Session: 6310936469860791894 
Range: npt=0.000000- 
RTP-Info: url=trackID=0;seq=17040;rtptime=1467265309  
		
```

#### `PAUSE`

​		客户端发起暂停请求，录像回放时会用到

```http
PAUSE rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
Cseq: 5
Session: 6310936469860791894
```

​		服务器回应:

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 5 
Session: 6310936469860791894
```



#### `RECORD`

​		C->S。

> ```
> 该方法根据演示描述初始化媒体数据记录范围, 时标反映开始和结束时间; 如没有给出时间范围, 使用演示描述提供的开始和结束时间. 如连接已经启动, 立即开始记录, 服务器数据请求URL或其他URL决定是否存储记录的数据; 如服务器没有使用URL请求, 响应应为201(创建), 并包含描述请求状态和参考新资源的实体与位置头. 支持现场演示记录的媒体服务器必须支持时钟范围格式, smpte格式没有意义
> ```

#### `TEARDOWN`

​		客户端发起关闭请求，停止媒体占用，并释放相关资源

```http
TEARDOWN rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 6
Session: 6310936469860791894 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

​		服务器回应:

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 6
Session: 6310936469860791894 
Connection: Close 
```



#### `ANNOUNCE`

​		`C->S/S->C`。

- 当从用户发往服务器时, ANNOUNCE将请求URL识别的演示或媒体对象描述发送给服务器
- 反之, ANNOUNCE实时更新连接描述. 如新媒体流加入演示, 整个演示描述再次发送, 而不仅仅是附加组件, 使组件能被删除 



#### `REDIRECT`

S->C 

> ```
> 重定向请求通知客户端连接到另一服务器地址. 它包含强制头地址, 指示客户端发布URL请求; 也可能包括参数范围, 以指明重定向何时生效. 若客户端要继续发送或接收URL媒体, 客户端必须对当前连接发送TEARDOWN请求, 而对指定主执新连接发送SETUP请求
> ```

#### 其他方法

​	以上方法都是交互过程中最为常用的,其它还有一些重要的方法如:`get/set_parameter,pause,redirect`等等

​	`SET_PARAMETER/GET_PARAMETER`，此方法基本没啥用，一般用来作为心跳使用，也是用`option`来维持心跳

- `GET_PARAMETER`请求检查RUL指定的演示与媒体的参数值. 没有实体体时, `GET_PARAMETER`也许能用来测试用户与服务器的连通情况
- 这个方法请求设置演示或URL指定流的参数值. 请求仅应包含单个参数, 允许客户端决定某个特殊请求为何失败. 如请求包含多个参数, 所有参数可成功设置, 服务器必须只对该请求起作用. 服务器必须允许参数可重复设置成同一值, 但不让改变参数值. 注意: 媒体流传输参数必须用SETUP命令设置. 将设置传输参数限制为SETUP有利于防火墙. 将参数划分成规则排列形式, 结果有更多有意义的错误指示



### `header`

```
| Header             | Type | Support | Methods                   |
|--------------------|------|---------|---------------------------|
| Accept             | R    | opt.    | entity                    |
| Accept-Encoding    | R    | opt.    | entity                    |
| Accept-Language    | R    | opt.    | all                       |
| Allow              | R    | opt.    | all                       |
| Authorization      | R    | opt.    | all                       |
| Bandwidth          | R    | opt.    | all                       |
| Blocksize          | R    | opt.    | All but OPTIONS, TEARDOWN |
| Cache-Control      | G    | opt.    | SETUP                     |
| Conference         | R    | opt.    | SETUP                     |
| Connection         | G    | req.    | all                       |
| Content-Base       | E    | opt.    | entity                    |
| Content-Encoding   | E    | req.    | SET_PARAMETER             |
| Content-Encoding   | E    | req.    | DESCRIBE, ANNOUNCE        |
| Content-Language   | E    | req.    | DESCRIBE, ANNOUNCE        |
| Content-Length     | E    | req.    | SET_PARAMETER, ANNOUNCE   |
| Content-Length     | E    | req.    | entity                    |
| Content-Location   | E    | opt.    | entity                    |
| Content-Type       | E    | req.    | SET_PARAMETER, ANNOUNCE   |
| Content-Type       | R    | req.    | entity                    |
| CSeq               | G    | req.    | all                       |
| Date               | G    | opt.    | all                       |
| Expires            | E    | opt.    | DESCRIBE, ANNOUNCE        |
| From               | R    | opt.    | all                       |
| If-Modified-Since  | R    | opt.    | DESCRIBE, SETUP           |
| Last-Modified      | E    | opt.    | entity                    |
| Proxy-Authenticate |      |         |                           |
| Proxy-Require      | R    | req.    | all                       |
| Public             | R    | opt.    | all                       |
| Range              | R    | opt.    | PLAY, PAUSE, RECORD       |
| Range              | R    | opt.    | PLAY, PAUSE, RECORD       |
| Referer            | R    | opt.    | all                       |
| Require            | R    | req.    | all                       |
| Retry-After        | R    | opt.    | all                       |
| RTP-Info           | R    | req.    | PLAY                      |
| Scale              | Rr   | opt.    | PLAY, RECORD              |
| Session            | Rr   | req.    | All but SETUP, OPTIONS    |
| Server             | R    | opt.    | all                       |
| Speed              | Rr   | opt.    | PLAY                      |
| Transport          | Rr   | req.    | SETUP                     |
| Unsupported        | R    | req.    | all                       |
| User-Agent         | R    | opt.    | all                       |
| Via                | G    | opt.    | all                       |
| WWW-Authenticate   | R    | opt.    | all                       |
```

- Type:
  - 类型 "g" 表示请求和响应中的通用请求头;
  - 类型 "R" 表示请求头;
  - 类型 "r" 表示响应头;
  - 类型 "e" 表示实体头字段.
- Support:
  - "req." 表示必须执行该字段.
  - "opt." 表示是可选的.
- entity 针对于返回一个信息主体的所有方法.



*常用头解析*:

```
| Header                  | Description                                                                           |
|-------------------------|---------------------------------------------------------------------------------------|
| CSeq                    | 命令的序列号, 逐1增加                                                                          |
| Content-Length          | 这个标记的存在说明后面有实体数据, 而且给出了这个数据块的大小, 单位是byte                                              |
| X-Playlist-Gen-Id       | 用来检查播放列表是否有效. 这个标记最初在客户端发送DESCRIBE命令后使用. 客户端在发送“SETUP”命令给服务器时必须回应一样的值                 |
| X-Playlist-Seek-Id      | 值必须和X-Playlist-Gen-Id 域的值相同, 在PLAY请求命令中使用.                                            |
| Blocksize               | 媒体包的总长度，单位是byte                                                                       |
| Session                 | Session ID是用作客户端和服务器之间是否是正确的连接。在客户端发送SETUP命令后，服务器会在应答消息头里面发送一个session值给客户端。这算建立的一个会话. |
| X-Accept-Authentication | 允许的authentication 方法.  NTLM, Digest 和 Basic 是标准的                                      |
| X-Broadcast-Id          | 是否是实况或者是先期录制的流。0 表示先期录制，其他的值表示是实况。                                                    |
| Range                   | 暂无中文释义                                                                                |
| Speed                   | 用来调整传输到客户端的流得速度。假如你的带宽可以接受更高速的数据传送，这个域的值可以设置大于1来加速下载数据. i.e. x1 rate                  |
| Server                  | 服务器类型和软件版本                                                                            |
| EOF                     | 文件结束标记，也是流的结束标记                                                                       |
| Date                    | 日期时间，下面举个例子：Tue, 18 Nov 2003 15:57:07 GMT                                             |
| Bandwidth               | 流需要的最大带宽，bits/秒                                                                       |
| Transport               | 使用什么协议来传输数据，比如TCP或者UDP等                                                               |
| Etag                    | 实体标记Entity tag，是一个分配给会话的值，就像”23180160″                                                |
| Supported               | 支持的COM modules , 有的是可选的.                                                              |
| Content-Type            | 此域用来表示命令或者应答的用意. 下面是常用的几种类型                                                           |
| \/                      | application/x-wms-Logconnectstats 这个在SET_PARAMETER命令中用到，表示将客户端的信息登记到服务器上。             |
| \/                      | application/sdp 这个表示接下来数据包里面的是sdp数据，它是在服务器对DESCRIBE命令的应答包中。                           |
| \/                      | application/x-wms-contentdesc 表示紧跟的数据是一个内容描述对象，它设置the layout of the dialog.           |
| \/                      | application/vnd.ms.wms-hdr.asfv1 表示跟着一个流媒体头信息(ASF header)，可以用BASIC 或者DIGEST来解码。       |
| \/                      | application/x-rtsp-packetpair 它被用来确定连接的可用带宽。                                          |
```



### 通用 header

```
  Request-header      =   Connection         
              |   Cache-Control
              |   CSeq
              |   Date
              |   From
              |   Via
              |   Content-Length
```



#### `CSeq`

​		表示一对请求响应的序列号，出现在所有请求和响应报文中。一个请求报文必须和它的响应报文的CSeq一致，任何重传的请求报文必须和原始报文的CSeq相同。

​		每次请求增加1.



#### `Content-Length`

​		说明`body`有数据, 而且给出了`body`大小, 单位是`byte`



### 请求 header

```
  Request-header      =   Accept
              |   Accept-Encoding
              |   Accept-Language
              |   Authorization
              |   From
              |   If-Modified-Since
              |   Range
              |   Referer
              |   User-Agent
              |   Allow
              |   Bandwidth
              |   Blocksize
              |   Conference
```



#### User-Agent

​		表示了客户端使用的是什么播放器，以及播放器的版本



### 响应 header

```
 Response-header    =   Location
              |   Proxy-Authenticate
              |   Public
              |   Retry-After
              |   Server
              |   Vary
              |   WWW-Authenticate
```





### 响应状态行

#### 状态码

- 1XX: Informational – 请求被接收到，继续处理
- 2XX: Success – 请求被成功的接收，解析并接受
- 3XX: Redirection – 为完成请求需要更多的操作
- 4XX: Client Error – 请求消息中包含语法错误或是不能够被有效执行
- 5XX: Server Error – 服务器响应失败，无法处理正确的有效的请求消息



```
| value | meaning                             |
|-------|-------------------------------------|
| ”100” | Continue (all 100 range)            |
| “200” | OK                                  |
| ”201” | Created                             |
| ”250” | Low on Storage Space                |
| ”300” | Multiple Choices                    |
| ”301” | Moved Permanently                   |
| ”302” | Moved Temporarily                   |
| ”303” | See Other                           |
| ”304” | Not Modified                        |
| ”305” | Use Proxy                           |
| ”350” | Going Away                          |
| ”351” | Load Balancing                      |
| ”400” | Bad Request                         |
| ”401” | Unauthorized                        |
| ”402” | Payment Required                    |
| ”403” | Forbidden                           |
| ”404” | Not Found                           |
| ”405” | Method Not Allowed                  |
| ”406” | Not Acceptable                      |
| ”407” | Proxy Authentication Required       |
| ”408” | Request Time-out                    |
| ”410” | Gone                                |
| ”411” | Length Required                     |
| ”412” | Precondition Failed                 |
| ”413” | Request Entity Too Large            |
| ”414” | Request-URI Too Large               |
| ”415” | Unsupported Media Type              |
| ”451” | Parameter Not Understood            |
| ”452” | reserved                            |
| ”453” | Not Enough Bandwidth                |
| ”454” | Session Not Found                   |
| ”455” | Method Not Valid in This State      |
| ”456” | Header Field Not Valid for Resource |
| ”457” | Invalid Range                       |
| ”458” | Parameter Is Read-Only              |
| ”459” | Aggregate operation not allowed     |
| ”460” | Only aggregate operation allowed    |
| ”461” | Unsupported transport               |
| ”462” | Destination unreachable             |
| ”500” | Internal Server Error               |
| ”501” | Not Implemented                     |
| ”502” | Bad Gateway                         |
| ”503” | Service Unavailable                 |
| ”504” | Gateway Time-out                    |
| ”505” | RTSP Version not supported          |
| ”551” | Option not supported                |
```



## 基本交互

​		要区分RTSP协议交互与流媒体码流传输。RTSP协议信令交互通常是基于TCP的方式进行协议交互，另外也可基于HTTP/websocket。归根结底，RTSP信令需要在长连接上进行多次请求响应完成信令交互。

​		无论基于哪种方式，其本质的交互流程都和TCP模式下一致。



### Rtsp over tcp

- 协议信令交互主要实现
  - 流媒体信息描述/码流通道建立/流媒体控制等功能
  - 通过`describe`来同步流媒体编码、封装、连接等信息
  - 通过`setup`来建立流媒体传输通道
  - 通过`play`来开启流媒体播放
  - 通过`teardown`来结束播放

- 流媒体码流的传输是通过RTSP交互建立的流媒体传输通道来传输码流，其传输协议一般为RTP/RTCP，其传输层可以为UDP或者TCP。

​		基本交互过程是标准的、友好的rtsp流程，但实际的需求中并不一定按部就班来。

```shell
1.C->S:OPTION request // 询问S有哪些方法可用 
1.S->C:OPTION response // S回应信息中包括提供的所有可用方法 
# 第一步，只要服务器客户端约定好，有哪些方法可用，则option请求可以不要。

2.C->S:DESCRIBE request // 要求得到S提供的媒体初始化描述信息 
2.S->C:DESCRIBE response // S回应媒体初始化描述信息，主要是sdp 
# 第二步，如果我们有其他途径得到媒体初始化描述信息（比如http请求等等），则我们也不需要通过rtsp中的describe请求来完成。

3.C->S:SETUP request // 设置会话的属性，以及传输模式，提醒S建立会话 
3.S->C:SETUP response // S建立会话，返回会话标识符，以及会话相关信息 

4.C->S:PLAY request // C请求播放 
4.S->C:PLAY response // S回应该请求的信息 
# 其中第3和4步是必需的！

5.S->C:发送流媒体数据 
# 第五步，可以根据系统需求的设计来决定是否需要。可以由专用的流媒体服务器进行推流

6.C->S:TEARDOWN request // C请求关闭会话 
6.S->C:TEARDOWN response // S回应该请求 
```



#### 实例

- 客户端：`VLC`
- RTSP服务器：`LIVE555 Media Server`

##### OPTIONS

```http
1)
# C(Client)-> M(Media Server) 
OPTIONS rtsp://192.168.1.109/1.mpg RTSP/1.0 
CSeq: 1 
user-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 

#1)M -> C 
RTSP/1.0 200 OK 
CSeq: 1 
Date: wed, Feb 20 2008 07:13:24 GMT 
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE 
```



```


2)C -> M 
DESCRIBE rtsp://192.168.1.109/1.mpg RTSP/1.0 
CSeq: 2 
Accept: application/sdp 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 

2)M -> C 
RTSP/1.0 200 OK 
CSeq: 2 
Date: wed, Feb 20 2008 07:13:25 GMT 
Content-Base: rtsp://192.168.1.109/1.mpg/ 
Content-type: application/sdp 
Content-length: 447 

v=0 
o =- 2284269756 1 IN IP4 192.168.1.109 
s=MPEG-1 or 2 program Stream, streamed by the LIVE555 Media Server 
i=1.mpg 
t=0 0 
a=tool:LIVE555 Streaming Media v2008.02.08 
a=type:broadcast 
a=control:* 
a=range:npt=0-66.181 
a=x-qt-text-nam:MPEG-1 or Program Stream, streamed by the LIVE555 Media Server 
a=x-qt-text-inf:1.mpg 
m=video 0 RTP/AVP 32 
c=IN IP4 0.0.0.0 
a=control:track1 
m=audio 0 RTP/AVP 14 
c=IN IP4 0.0.0.0 
a=control:track2 

3)C -> M 
SETUP rtsp://192.168.1.109/1.mpg/track1 RTSP/1.0 
CSeq: 3 
Transport: RTP/AVP; unicast;client_port=1112-1113 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 

3)M -> C 
RTSP/1.0 200 OK 
CSeq: 3 
Date: wed, Feb 20 2008 07:13:25 GMT 
Transport: RTP/AVP;unicast;destination=192.168.1.222;source=192.168.1.109;client_port=1112-1113;server_port=6970-6971 
Session: 3 

4)C -> M 
SETUP rtsp://192.168.1.109/1.mpg/track2 RTSP/1.0 
CSeq: 4 
Transport: RTP/AVP; unicast;client_port=1114-1115 
Session: 3 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 

4)M -> C 
RTSP/1.0 200 OK 
CSeq: 4 
Date: wed, Feb 20 2008 07:13:25 GMT 
Transport: RTP/AVP;unicast;destination=192.168.1.222;source=192.168.1.109;client_port=1114-1115;server_port=6972-6973 
Session: 3 

5)C -> M 
PLAY rtsp://192.168.1.109/1.mpg/ RTSP/1.0 
CSeq: 5 
Session: 3 
Range: npt=0.000- 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 

5)M -> C 
RTSP/1.0 200 OK 
CSeq: 5 
Range: npt=0.000- 
Session: 3 
RTP-Info: url=rtsp://192.168.1.109/1.mpg/track1;seq=9200;rtptime=214793785,url=rtsp://192.168.1.109/1.mpg/track2;seq=12770;rtptime=31721

(开始传输流媒体…)
```



### Rtsp over HTTP

​		RTSP-over-HTTP tunneling，通过HTTP隧道来传输RTSP协议和媒体流

- 需要RTSP服务器支持此种方式，开启HTTP隧道监听端口
- 客户端
  - 首先建立一个链接通过HTTP-GET方法来获取协议响应消息和媒体流
    - 数据流的发送封装方式与RTP/TCP一样，通过GET链路发送给客户端
    - 响应消息也是通过GET链路发送给客户端
  - 然后再建立一个链路，通过HTTP-POST方法来发送请求消息
    - POST 连接中发送RTSP请求消息，一般要进行BASE64编码，来隐藏RTSP信息

![rtsp/http](https://raw.githubusercontent.com/Mocearan/picgo-server/main/a4ae82046984a60dc07bf463bae575de.png)

### Rtsp over Websocket



## RTSP 推流模式

​		RTSP推流方式，当前用到的越来越少，在媒体分发领域有用到。

​		一般作为流媒体代理，应用CDN等场景，推流由客户端发起，视频由客户端推送给服务器端，因此流媒体描述需要由客户端通知到服务器，通过ANNOUNCE方法完成，其SDP描述与DESCRIBE一致，传输开始时，由客户端发送RECORD方法，然后由客户端推送rtp流到服务器。

![RTSP推流流程](https://raw.githubusercontent.com/Mocearan/picgo-server/main/86900b434b5b36620f4563291744c6e0.png)

```http
OPTIONS rtsp://10.45.12.141:554/live/0001 RTSP/1.0
CSeq: 1
User-Agent: znv
 
RTSP/1.0 200 OK
Server: EasyDarwin/7.3 
Cseq: 1
Public: DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, OPTIONS, ANNOUNCE, RECORD
 
ANNOUNCE rtsp://10.45.12.141:554/live/0001 RTSP/1.0
Content-Type: application/sdp
CSeq: 2
User-Agent: znv
Content-Length: 325
 
v=0
o=- 0 0 IN IP4 127.0.0.1
s=Media Server
c=IN IP4 192.168.1.108
t=0 0
a=tool:libavformat 57.71.100
m=video 0 RTP/AVP 96
a=rtpmap:96 H264/90000
a=fmtp:96 packetization-mode=1; sprop-parameter-sets=Z2QAHqw0ygsBJ/wFuCgoKgAAB9AAAYah0MALFAALE9d5caGAFigAFieu8uFA,aO48MA==; profile-level-id=64001E
a=control:streamid=0
RTSP/1.0 200 OK
Server: EasyDarwin/7.3 (Build/17.0325; Platform/Win32; Release/EasyDarwin; State/Development; )
Cseq: 2
 
SETUP rtsp://10.45.12.141:554/live/0001/trackid=0 RTSP/1.0
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;mode=record
CSeq: 3
User-Agent: znv
 
RTSP/1.0 200 OK
Server: EasyDarwin/7.3 
Cseq: 3
Cache-Control: no-cache
Session: 132169028622239
Date: Tue, 13 Nov 2018 02:49:48 GMT
Expires: Tue, 13 Nov 2018 02:49:48 GMT
Transport: RTP/AVP/TCP;unicast;mode=record;interleaved=0-1
 
RECORD rtsp://10.45.12.141:554/live/0001 RTSP/1.0
Range: npt=0.000-
CSeq: 4
User-Agent:znv
Session: 132169028622239
 
RTSP/1.0 200 OK
Server: EasyDarwin/7.3 
Cseq: 4
Session: 132169028622239
RTP-Info: url=rtsp://192.168.1.108:554/live.sdp/live.sdp
```



## 相关项目

```
      name      side? lang   url
    -----------------------------------------------------------------------  
    * live.com   C/S   C++   http://www.live555.com
    * darwin     S     C++   http://www.opensource.apple.com/darwinsource/
    * VLC        C     C     http://www.videolan.org
    * mplayer    C     C     http://www.mplayerhq.hu
    * omsp       C/S   C     http://streaming.polito.it/
         Fenice is Server 
         NeMeSI is Client
    * spook      S     C     http://www.litech.org/spook/
         网络摄像机
    * MPEG4IP    C     C     http://www.mpeg4ip.net/
```

