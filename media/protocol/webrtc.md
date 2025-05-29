# WebRTC

​		`Web Real-Time Communications`, Web 实时通信

---

​	 	`WebRTC `协 议 由 `W3C WEBRTC` 和 `IETF RTCWEB`工作组共同制定提出。是一项实时通讯技术，它允许网络应用或者站点，在不借助中间媒介的情况下，建立浏览器之间点对点（`Peer-to-Peer`）的连接，实现视频流和（或）音频流或者其他任意数据的传输。

​		`WebRTC`包含的这些标准使用户在无需安装任何插件或者第三方的软件的情况下，创建点对点（`Peer-to-Peer`）的数据分享和电话会议成为可能。

​    	`WebRTC `由用于 `Web `实时通信的 `JavaScript API` 和一组通信协议构成，支持网络上的任何已连接设备成为 `Web `上潜在的通信端点。

​		`WebRTC`是一个协议框架，定义了握手协议（`DTLS`），会话描述（`SDP`），加密传输协议（`SRTP/SRTCP`），`P2P`穿透（`STUN/TURN`），同时也加了一些新的拥塞控制算法`GCC/BBR`等。


​		`WebRTC`主要让浏览器具备三个作用。

- 获取音频和视频
- 进行音频和视频通信
- 进行任意数据的通信

​		`WebRTC`共分成三个`API`，分别对应上面三个作用。

- `MediaStream （又称getUserMedia）`
- `RTCPeerConnection`
- `RTCDataChannel`

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/0bc6ab50e4b5ec772cd94a20a2601563.png" alt="img" style="zoom: 67%;" />

​		`WebRTC`使用`SDP`进行媒体协商，使用基于`RTP`的`SRTP`进行媒体传输。

​		

## 参考

WebRTC 标准文档：https://www.w3.org/TR/webrtc/



[如何构建分布式SFU/MCU媒体服务器？-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1478326)

[服务器sfu是什么 • Worktile社区](https://worktile.com/kb/ask/1411471.html)

[史上最全的WebRTC服务器技术选型分析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/613619262)

[一文看懂WebRTC流媒体服务器 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/663832662)

[使用级联SFU改善媒体质量和规模-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1871182)

[WebRTC_椛茶的博客-CSDN博客](https://blog.csdn.net/weixin_38102771/category_12369537.html)

[椛茶-CSDN博客](https://blog.csdn.net/weixin_38102771?type=blog)

[实现WebRTC P2P连接 - 简书 (jianshu.com)](https://www.jianshu.com/p/f383fcaf0604)



[WebRTC](https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MjM5MTkxOTQyMQ==&action=getalbum&album_id=3885414430967922697&scene=173&subscene=&sessionid=svr_617b79790b0&enterid=1747318573&from_msgid=2257491031&from_itemidx=1&count=3&nolastread=1#wechat_redirect)





## 优缺点

优点：

- 延时低，可以优化到毫秒级别
- 弱网环境下表现优异

缺点：

- 设备端适配不好
- `webRTC`只适合解决第一公里和最后一公里的问题
- `webRTC`支持的音视频格式少
- `webRTC`只在弱网环境下有优势



## DTLS

​		`DTLS`是`UDP`版本的`TLS`协议，用于建立连接时交换密钥。



## SDP

​		`webRTC`通过`SDP`来描述`WebRTC`建立连接所需的ICE服务器信息、音视频编码信息等。

​		`SDP`可以通过`http / websocket`以及其他的方式交互



## `SRTP / SRTCP`

​		加密版本你的`RTP / RTCP`



## NAT

​		网络地址转换（`Network Address Translation`，简称 `NAT`）用于为设备分配一个公网 `IP `地址。

​		路由器会有一个公网 `IP `地址，连接到路由器的每个设备都会有一个私有 `IP `地址。请求将从设备的私有 `IP `转换为路由器的公网 `IP`，并附带一个唯一的端口。这样，即使你没有为每个设备分配唯一的公网 `IP`，仍然可以在互联网上被发现。



### ICE

​		交互式连接建立（`Interactive Connectivity Establishment`，简称 ICE）是一种框架，允许网络浏览器与对等方建立连接。

​		直接从对等方 A 连接到对等方 B 可能会遇到诸多问题:

- 比如需要绕过阻止建立连接的防火墙
- 在大多数设备没有公网 IP 地址的情况下提供一个唯一的地址
- 以及在路由器不允许直接连接对等方时通过服务器中继数据

​		`ICE `使用 `STUN `和/或 `TURN `服务器来实现这些功能。



### STUN

​		会话穿越实用工具（`Session Traversal Utilities for NAT`，简称 `STUN`）是一种协议，用于发现你的公网地址，并确定路由器中的任何限制，这些限制可能会阻止与对等方的直接连接。

​		客户端会向互联网上的 `STUN `服务器发送请求，服务器会回复客户端的公网地址以及客户端是否在路由器 `NAT `后可被访问。

![img](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuegQIvfGt7DNZGkQZUM59z3ic6iblh0iarb1ibu3m7hYshgLkYpmliaJEiaJ7YpGW8qoxHicbMgxo4vrhLIWw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1)





### TURN

​		有些路由器会对可以连接到网络中设备的对等方进行限制。这意味着即使通过 `STUN `服务器找到了公网 IP 地址，也可能不是任何人都能建立连接。在这种情况下，我们需要使用 `TURN`。

> 一些使用 `NAT `的路由器采用了一种称为“对称 `NAT`”的限制。这意味着路由器只接受你之前连接过的对等方的连接。

​		`Traversal Using Relays around NAT`（简称 `TURN`）旨在通过与 `TURN `服务器建立连接并中继所有信息来绕过对称 `NAT `的限制。你会与 `TURN `服务器建立连接，并告诉所有对等方将数据包发送到服务器，服务器再将数据包转发给你。这显然会带来一些开销，因此只有在没有其他替代方案时才使用。



![img](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuegQIvfGt7DNZGkQZUM59z3ic6vgYwZj0LfrSlVUapzib7w1PXuHoOnY9ENklBAIU0AE1icylxdXicCxxw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1)







## 传输协议

​		`WebRTC `实际上使用 **`SRTP`**（安全实时传输协议）来确保数据的安全性和认证。`SRTP`是基于`RTP`的协议。

​		 `RTP `是一种数据传输协议，配合控制协议`RTCP`来增强包括 服务质量（`QoS`）监测、参与者信息共享等功能。它不足以完全管理用户、成员资格、权限等，但为不受限制的多用户通信会话提供了所需的基本功能。

​		`RTP `在 `WebRTC `中的主要优势包括：

- 低延迟。
- 数据包按序列编号和时间戳进行重组
  - 如果不能保证数据包传输非乱序到达，使用 `RTP `可以在不保证顺序甚至不保证交付的传输中进行传输。
  - 这意味着 `RTP `可以用于 UDP 之上，以利用其性能以及其复用和校验和特性。
- `RTP `支持多播；虽然这对于 `WebRTC `目前还不重要，但随着 `WebRTC`增强以支持多用户会话，这在未来可能会变得重要。
- `RTP `并不限于在视听通信中使用。它可以用于任何形式的连续或活动数据传输，包括数据流、活动徽章或状态显示更新，或控制和测量信息传输。

​		`RTP `不包括的一些值得注意的功能：

- RTP 并不保证 **服务质量**（**`QoS`**）。
  - 虽然 `RTP `是为延迟关键场景设计的，但它本身并不提供任何确保 `QoS `的功能。
  - 它只提供必要的信息，以便在堆栈的其他地方实现 `QoS`。
- `RTP `不处理可能需要的资源分配或预留。



### RTCPeerConnection

​		每个 `RTCPeerConnection` 都可以访问服务对等连接的 `RTP `传输列表。

​		这些方法对应于 `RTCPeerConnection` 支持的以下三种传输类型：

- `RTCRtpSender`，处理将 `MediaStreamTrack` 数据编码并传输给远程对等方。
  - 可以通过调用 `RTCPeerConnection.getSenders()` 获取特定连接的发送器。
- `RTCRtpReceiver`，提供检查和获取传入 `MediaStreamTrack` 数据信息的功能。
  - 可以通过调用 `RTCPeerConnection.getReceivers()` 获取连接的接收器。
- `RTCRtpTransceiver`，是一个 `RTP `发送器和一个 `RTP `接收器的组合
  - 它们共享一个 `SDP mid` 属性，这意味着它们共享同一个 `SDP `媒体 `m-line`（表示一个双向 `SRTP `流）
  - 这些通过 `RTCPeerConnection.getTransceivers()` 方法返回
  - 每个 `mid` 和收发器之间有一对一的关系
  - 每个 `mid` 在每个 `RTCPeerConnection` 中是唯一的



### 5、利用 RTP 实现保留功能

由于 `RTCPeerConnection` 的流是使用 RTP 和上述接口实现的，因此可以利用对流内部的访问来进行调整。可以做的最简单的事情之一是实现一个“保留”功能，其中通话中的参与者可以点击一个按钮，关闭他们的麦克风，开始向另一个对等方发送音乐，而不是接受传入的音频。

**注意：** 这个例子使用了现代 JavaScript 特性，包括异步函数和 `await` 表达式。这极大地简化并使处理 WebRTC 方法返回的承诺的代码更具可读性。

在下面的例子中，我们将把打开和关闭“保留”模式的对等方称为本地对等方，而被保留的用户称为远程对等方。

#### 6、激活保留模式

##### 6.1、本地对等方

当本地用户决定启用保留模式时，将调用下面的 `enableHold()` 方法。它接受一个包含保留时要播放的音频的 `MediaStream` 作为输入。

```js
async function enableHold(audioStream) {
  try {
    await audioTransceiver.sender.replaceTrack(audioStream.getAudioTracks()[0]);
    audioTransceiver.receiver.track.enabled = false;
    audioTransceiver.direction = "sendonly";
  } catch (err) {
    /* 处理错误 */
  }
}
```

`try` 块中的三行代码执行以下步骤：

1. 将他们的传出音频轨道替换为包含保留音乐的 `MediaStreamTrack`。
2. 禁用传入音频轨道。
3. 将音频收发器切换为仅发送模式。

这会触发 `RTCPeerConnection` 的重新协商，通过向其发送 `negotiationneeded` 事件，你的代码将响应生成 SDP 提供，使用 `RTCPeerConnection.createOffer` 并通过信令服务器将其发送给远程对等方。

包含要播放的音频而不是本地对等方麦克风音频的 `audioStream` 可以来自任何地方。一种可能性是使用隐藏的 `<audio>` 元素，并使用 `HTMLAudioElement.captureStream()` 获取其音频流。

##### 6.2、远程对等方

在远程对等方，当我们收到方向设置为 `"sendonly"` 的 SDP 提供时，我们将使用 `holdRequested()` 方法处理它，该方法接受一个 SDP 提供字符串作为输入。

```js
async function holdRequested(offer) {
  try {
    await peerConnection.setRemoteDescription(offer);
    await audioTransceiver.sender.replaceTrack(null);
    audioTransceiver.direction = "recvonly";
    await sendAnswer();
  } catch (err) {
    /* 处理错误 */
  }
}
```

这里执行的步骤是：

1. 通过调用 `RTCPeerConnection.setRemoteDescription()` 将远程描述设置为指定的 `offer`。
2. 将音频收发器的 `RTCRtpSender` 的轨道替换为 `null`，这意味着没有轨道。这停止了收发器上的音频发送。
3. 将音频收发器的 `direction` 属性设置为 `"recvonly"`，指示收发器仅接受音频，不发送任何音频。
4. 使用名为 `sendAnswer()` 的方法生成并发送 SDP 回答，该方法使用 `createAnswer()` 生成回答，然后通过信令服务将结果 SDP 发送给另一个对等方。

#### 7、取消保留模式

##### 7.1、本地对等方

当本地用户点击界面小部件以禁用保留模式时，将调用 `disableHold()` 方法，以开始恢复正常功能的过程。

```js
async function disableHold(micStream) {
  await audioTransceiver.sender.replaceTrack(micStream.getAudioTracks()[0]);
  audioTransceiver.receiver.track.enabled = true;
  audioTransceiver.direction = "sendrecv";
}
```

这通过执行以下步骤来逆转 `enableHold()` 中的操作：

1. 将音频收发器的 `RTCRtpSender` 的轨道替换为指定流的第一个音频轨道。
2. 重新启用收发器的传入音频轨道。
3. 将音频收发器的方向设置为 `"sendrecv"`，表示它应该恢复发送和接收流式音频，而不仅仅是发送。

就像启用保留时一样，这会再次触发协商，导致你的代码向远程对等方发送新的提供。

##### 7.2、远程对等方

当远程对等方收到 `"sendrecv"` 提供时，它将调用其 `holdEnded()` 方法：

```js
async function holdEnded(offer, micStream) {
  try {
    await peerConnection.setRemoteDescription(offer);
    await audioTransceiver.sender.replaceTrack(micStream.getAudioTracks()[0]);
    audioTransceiver.direction = "sendrecv";
    await sendAnswer();
  } catch (err) {
    /* 处理错误 */
  }
}
```

这里 `try` 块中的步骤是：

1. 通过调用 `setRemoteDescription()` 将收到的提供存储为远程描述。
2. 使用音频收发器的 `RTCRtpSender` 的 `replaceTrack()` 方法将传出音频轨道设置为麦克风音频流的第一个轨道。
3. 将收发器的方向设置为 `"sendrecv"`，表示它应该恢复发送和接收音频。

从这一点开始，麦克风重新启用，远程用户再次能够听到本地用户并与之交谈。



## 信令

​		`WebRTC `创建对等连接时必须通过中间服务器。称之为 **信令通道** 或 **信令服务** 。可以是任何在建立连接之前交换信息的通信渠道，需要交换的信息是包含 `SDP `的 `Offer `和 `Answer`。

- 发起连接的对等方` A `将创建一个 `Offer`
- 通过选定的信令通道将此`Offer`发送给对等方 `B`
- 对等方 `B` 将从信令通道接收`Offer`并创建一个 `Answer`
- 然后通过信令通道将此`Answer`发送回对等方 `A`



