# RTMP

​		Real Time Message Protocol, 实时消息协议

---

​		RTMP是由Adobe公司提出的一种应用层的协议，用来解决多媒体数据传输流的多路复用（Multiplexing）和分包（packetizing）的问题。

​		RTMP协议是应用层协议，是要靠底层可靠的传输层协议（通常是TCP）来保证信息传输的可靠性的。在基于传输层协议的链接建立完成后，RTMP协议也要客户端和服务器通过“握手”来建立基于传输层链接之上的RTMP Connection链接，在Connection链接上会传输一些控制信息，如SetChunkSize,SetACKWindowSize。其中CreateStream命令会创建一个Stream链接，用于传输具体的音视频数据和控制这些信息传输的命令信息。
​    RTMP协议传输时会对数据做自己的格式化，这种格式的消息我们称之为RTMP Message，而实际传输的时候为了更好地实现多路复用、分包和信息的公平性，发送端会把Message划分为带有Message ID的Chunk，每个Chunk可能是一个单独的Message，也可能是Message的一部分，在接受端会根据chunk中包含的data的长度，message id和message的长度把chunk还原成完整的Message，从而实现信息的收发。

​    播放一个RTMP协议的流一般经过以下三步：握手（Handshake），建立连接（connect），建立流（CreateStream），播放（play）。
​    客户-服务器间只能建立一个连接，但可以创建多个流，每个流代表了收发流媒体的一个通道。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/8e1fbff1fe2b0c6485843ff8bf97da10.png)

![img](https://img-blog.csdnimg.cn/img_convert/f29857b212e51e32cbc94e95c9a2435c.png)

 握手完成

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d42312a4ae4e73489e38cdb3f4f29b44.png)

 建立连接完成

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3b77b88dda296624e0ab21432a5d3d80.png)

创建流，开始播放



## 前景

​		在过去的10多年时间里（从2004年到2018年），网页视频播放一直是Flash技术的天下，我们所熟悉的众多视频网站和新闻门户网站一直都在使用Flash技术来播放网页视频。这里的主要原因是IE浏览器的高比例占有量和Flash插件在客户端的普及，还有就是主要[流媒体服务器](https://so.csdn.net/so/search?q=流媒体服务器&spm=1001.2101.3001.7020)产品对rtmp协议和flv视频格式的广泛支持，这是这一个时期的视频生态系统。
​    随着IE浏览器的衰落和新型浏览器（Chrome,Firefox,Safari等）的崛起，Flash视频播放进入了被淘汰的进程，HTML5 Video正在成为视频播放的主流技术。Youtube从2010年就开始尝试使用没有Flash的视频播放技术，到2018年前后完全实现了去Flash播放，国内的视频网站也在积极地进行这去flash的技术改进，相信也很快不再使用Flash播放器。Chrome浏览器在2020年彻底抛弃Flash技术，几大科技巨头包括Flash的母公司Adobe联合宣布将放弃Flash。这无疑会大大促进众多整个互联网行业的去Flash进程。

​    HTML5 Video与MSE（Media Source Extensions ）一起能够提供更加强大的视频播放和扩展应用（如双向视频）等。

​    要实现去Flash应当在播出时摒弃rtmp协议，采用HLS、HTTP-FLV、WebRTC、WebSocket来传输直播视频流，客户端使用MSE扩展实现数据接收和视频播放

​    HTML5的Video控件标签支持HLS协议播放，经测试确实支持m3u8的播放。
HTML5视频解码器没有指定，由浏览器开发者决定。当前各主流浏览器video元素主要支持三种视频格式ogg, mp4, webm。MPEG4 = 带有 H.264 视频编码和 AAC 音频编码的 MPEG 4 文件