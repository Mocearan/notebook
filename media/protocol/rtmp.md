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