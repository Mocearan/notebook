# WebRTC

​		Web Real-Time Communications, Web 实时通信

---

​	 WebRTC 协 议 由 W3C WEBRTC 和IETF RTCWEB工作组共 同 制 定 提 出， WebRTC 协议是面向连接的流媒体技术代表之一，实 现 了 基 于 网 页 的 视 频 会 议 通 信。
​    WebRTC 是一项实时通讯技术，它允许网络应用或者站点，在不借助中间媒介的情况下，建立浏览器之间点对点（Peer-to-Peer）的连接，实现视频流和（或）音频流或者其他任意数据的传输。WebRTC包含的这些标准使用户在无需安装任何插件或者第三方的软件的情况下，创建点对点（Peer-to-Peer）的数据分享和电话会议成为可能。
​    2021年1月26日，W3C（万维网联盟）和 IETF （互联网工程任务组）同时宣布 WebRTC现发布为正式标准，将音视频通信带到 Web 上任何地方。
​    WebRTC 由用于 Web 实时通信的 JavaScript API 和一组通信协议构成，支持网络上的任何已连接设备成为 Web 上潜在的通信端点。WebRTC 已成为线上通信及协作服务的基石。所有Web 应用程序都可接入实时音视频通信系统。WebRTC 框架提供了构建块，Web 和应用程序开发人员可以从这些构建块无缝地将视频聊天添加到一系列应用程序，包括远程教育和远程医疗，娱乐和游戏，专业和工作场所协作。通过将基础标准化并作为免版税功能部署在 Web 浏览器以及其他设备和平台中，利用 WebRTC 建立安全的视听通信系统已成为一种内置功能，从而无需安装插件或下载单独的应用程序。

​    但目前WebRTC协议仍在不够成熟，技术难度较高实现较复杂。不过这次正式标准的发布，一定将极大地推进WebRTC在视频直播，流媒体传输领域的改革，相信不远的将来音视频通信领域将是WebRTC的天下。

​    WebRTC 标准文档：https://www.w3.org/TR/webrtc/
​    这个文档定义了一套WebIDL中的ECMAScript接口，使多媒体和一般的应用程序数据能够被发送到或者接收自其他实现了实时流协议的浏览器或设备。该规范和IETF RTCWEB开发的协议规范以及访问本地媒体设备的接口规范一同被开发。


WebRTC主要让浏览器具备三个作用。

- 获取音频和视频
- 进行音频和视频通信
- 进行任意数据的通信

WebRTC共分成三个API，分别对应上面三个作用。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/0bc6ab50e4b5ec772cd94a20a2601563.png)



MediaStream （又称getUserMedia）

- RTCPeerConnection
- RTCDataChannel



## 参考

[如何构建分布式SFU/MCU媒体服务器？-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1478326)

[服务器sfu是什么 • Worktile社区](https://worktile.com/kb/ask/1411471.html)

[史上最全的WebRTC服务器技术选型分析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/613619262)

[一文看懂WebRTC流媒体服务器 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/663832662)

[使用级联SFU改善媒体质量和规模-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1871182)