



## 信令服务器

​		视频通话已成为通信的重要工具，让人们能够不受位置限制地进行面对面交流。WebRTC（Web 实时通信）是一种使浏览器和移动应用之间能够直接进行实时视频、音频和数据通信的技术。

​		

## 参考

[WebRTC开源项目-手把手教你搭建AppRTC - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/454470942)

[WebRTC 之通信 — PeerConnections 与信令服务 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/563023889)

[webRTC进阶-信令篇-之三：信令、stun、turn、ice - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/468792680)

[(52 封私信 / 84 条消息) 在多媒体通信中，信令服务器和流媒体服务器各充当什么角色？各自的主要功能是什么？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/281716536/answer/2509843321)

[信令服务器是什么 • Worktile社区](https://worktile.com/kb/ask/1402764.html)

[WebRTC → 信令服务器浅析与实现 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/649189927)

[流媒体中信令服务器的作用_百度搜索 (baidu.com)](https://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&tn=baidu&wd=流媒体中信令服务器的作用&oq=%E6%B5%81%E5%AA%92%E4%BD%93%E9%87%8D%E4%BF%A1%E4%BB%A4%E6%9C%8D%E5%8A%A1%E5%99%A8%E7%9A%84%E4%BD%9C%E7%94%A8&rsv_pq=94d9e2e000010e21&rsv_t=f0fdTO6uiT1sRV8Kyk6sbTa184bnyV3oeLJT4rwDSWRD74EqOpXIPbk26Mg&rqlang=cn&rsv_enter=1&rsv_dl=tb&rsv_btype=t&inputT=1689&rsv_sug3=18&rsv_sug1=11&rsv_sug7=100&rsv_sug2=0&rsv_sug4=2621)



## WebRTC 视频通话的基本工作流程

1. **用户媒体捕获**：每个客户端从其设备捕获音频和视频。
2. **对等连接建立**：创建对等连接对象以处理对等之间的连接。
3. **信令**：信令是交换用于初始化连接的会话控制消息的过程。这通常涉及一个信令服务器，以促进 ICE 候选人、会话描述（SDP）和其他元数据的交换。
4. **媒体交换**：一旦连接建立，媒体流（音频和视频）将直接通过对等连接在对等之间交换。

在视频通话中，通话中的每个参与者同时发送和接收视频流。这可能导致显著的带宽消耗，因为每个参与者需要维护多个点对点连接。



## 多对多视频通话中的挑战

- **带宽使用**：每个参与者必须将视频流上传到多个对等方，并从其他人那里下载多个视频流，这可能会迅速使网络带宽不堪重负。
- **可扩展性**：随着参与者数量的增加，所需的点对点连接数量呈指数增长，这使得保持稳定和高质量的连接变得困难。