# RTSP

​	Real Time Streaming Protocol

---

​		RTSP提供一 种可扩展的框架，使能够提供可控制的，按需传输实时数据，比如音频和视频文件。源数据可以包括现场数据的反馈和存贮的文件。rtsp对流媒体提供了诸如暂停，快进等控制，而它本身并不传输数据，rtsp作用相当于流媒体服务器的远程控制。传输数据可以通过传输层的tcp，udp协议，rtsp也提供了基于rtp传输机制的一些有效的方法。



## 协议格式

### 请求消息

```
方法 URI RTSP版本 CR LF 
消息头 CR LF 
CR LF （空行）
消息体 CR LF 
```

​		RTSP版本一般都是 RTSP/1.0.

### 回应消息

```
RTSP版本 状态码 reason CR LF 
消息头 CR LF 
CR LF （空行）
消息体 CR LF
```



### 方法

#### OPTION

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



#### DESCRIBE

​		C向S发起DESCRIBE请求,为了得到会话描述信息(SDP):

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

a=control:trackID=0//trackID＝0表示视频流用的是通道0 
```



#### SETUP

​		客户端提醒服务器建立会话,并确定传输模式:

```http
SETUP rtsp://192.168.20.136:5000/xxx666/trackID=0 RTSP/1.0 
CSeq: 3 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

- `uri`中带有`trackID＝0`，表示对该通道进行设置。
- `Transport`参数设置了
  - 传输模式
  - 包的结构
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



#### PLAY

​		客户端发送播放请求:

```http
PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 4 
Session: 6310936469860791894 
Range: npt=0.000- //设置播放时间的范围 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

​		服务器回应信息:

```http
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 4 
Session: 6310936469860791894 
Range: npt=0.000000- 
RTP-Info: url=trackID=0;seq=17040;rtptime=1467265309  
```

- `seq`和`rtptime`都是rtp包中的信息 
  		

#### PAUSE

​		客户端发起暂停请求:

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



#### TEARDOWN

​		客户端发起关闭请求:

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



#### 其他方法

​	以上方法都是交互过程中最为常用的,其它还有一些重要的方法如:`get/set_parameter,pause,redirect`等等



## 基本交互

​		基本交互过程是标准的、友好的rtsp流程，但实际的需求中并不一定按部就班来。

```shell
1.C->S:OPTION request //询问S有哪些方法可用 
1.S->C:OPTION response //S回应信息中包括提供的所有可用方法 
# 第一步，只要服务器客户端约定好，有哪些方法可用，则option请求可以不要。

2.C->S:DESCRIBE request //要求得到S提供的媒体初始化描述信息 
2.S->C:DESCRIBE response //S回应媒体初始化描述信息，主要是sdp 
# 第二步，如果我们有其他途径得到媒体初始化描述信息（比如http请求等等），则我们也不需要通过rtsp中的describe请求来完成。

3.C->S:SETUP request //设置会话的属性，以及传输模式，提醒S建立会话 
3.S->C:SETUP response //S建立会话，返回会话标识符，以及会话相关信息 

4.C->S:PLAY request //C请求播放 
4.S->C:PLAY response //S回应该请求的信息 
# 其中第3和4步是必需的！

5.S->C:发送流媒体数据 
# 第五步，可以根据系统需求的设计来决定是否需要。可以由专用的流媒体服务器进行推流

6.C->S:TEARDOWN request //C请求关闭会话 
6.S->C:TEARDOWN response //S回应该请求 
```



### 实例

- 客户端：`VLC`
- RTSP服务器：`LIVE555 Media Server`

```
1)C(Client)-> M(Media Server) 
OPTIONS rtsp://192.168.1.109/1.mpg RTSP/1.0 
CSeq: 1 
user-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 

1)M -> C 
RTSP/1.0 200 OK 
CSeq: 1 
Date: wed, Feb 20 2008 07:13:24 GMT 
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE 

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



