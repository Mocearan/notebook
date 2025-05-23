# hls

​		`HTTP Live Streaming`，是由苹果公司提出基于HTTP的流媒体网络传输协议。

---

​		HLS协议被广泛的应用于视频点播和直播领域。

​		基本原理：

- 将整条流通过切割成一系列小的可以通过 HTTP 下载的媒体文件
- 提供一个配套的媒体列表文件
- 播放器先请求媒体列表文件
- 然后再取到对应的切片的文件
- 通过请求一个个小文件实现连续的播放，实现看上去是在播放一条流的效果
- 只需要标准的 HTTP 协议

  > 数据通过 HTTP 协议传输，所以完全不用考虑防火墙或者代理的问题

​		分段文件的时长很短，客户端可以很快的选择和切换码率，以适应不同带宽条件下的播放。不过 HLS 的这种技术特点，决定了它的延迟一般总是会高于普通的流媒体直播协议。

​		相对于常见的流媒体直播协议，HLS 最大的不同在于直播客户端获取到的并不是一个完整的数据流，而是连续的、短时长的媒体文件（如 MPEG-TS 格式），客户端不断的下载并播放这些小文件。





- Apple 的 Safari 浏览器直接就能打开 m3u8 地址， 譬如：http://demo.srs.com/live/livestream.m3u8

- Android 不能直接打开， 需要使用 html5 的 video 标签， 然后在浏览器中打开这个页面即可，
  譬如：

  - ```html
    <!-- livestream.html -->
    <video width="640" height="360"
    autoplay controls autobuffer
    src="http://demo.srs.com/live/livestream.m3u8"
    type="application/vnd.apple.mpegurl">
    </video>
    ```

- PC：video.js



## 参考

[原来爱优腾等视频网站都是用这个来播放流媒体的HLS (HTTP Live Streaming) 是苹果公司开发的流媒体传 - 掘金](https://juejin.cn/post/6954761121727250439)





架构

<img src="https://pic4.zhimg.com/80/v2-6cf6f240d7c75785cb0461f0f7fd1f97_720w.webp" alt="img" style="zoom: 80%;" />

- hls流媒体服务器 接收媒体输入流
  - 对它们进行编码
    - 通过预设的编码格式进行编码，或者通过流媒体协议接入已编码好的音视频流，根据流媒体预设条件确定是否需要转码
  - 封装成适合于分发的格式
    - 接收媒体编码器打包好的TS流，或者读取TS流的录像文件
    - 按照预设时间间隔把TS流切片成等时间间隔的TS流切片文件，并生成或更新索引文件(m3u8文件/playlist播放列表文件)
    - 每个新的切片生成之后，索引文件都要更新，索引文件用于定位切片文件的位置及有效性判断
  - 发送到分发服务器进行内容分发
- 分发组件为标准的 Web 服务器，提供HTTP服务器，并提供索引文件和切片文件下载的服务
  - 接收客户端请求
  - 传递处理过的媒体内容
  - 一般需要通过 CDN 网络来进行资源分发。
- 客户端软件请求媒体资源
  - 下载资源
  - 重新组装成用户可以观看的连续流
  - 并对其进行解封装、解码、渲染等一系列的处理从而展现给用户。



## 流派

​		HLS有两个版本，主要区别在于使用的封装容器不同。

- HLS-mpegts
  - 基于mpegts封装，mpegts因为有出色的纠错能力，常用于广播电视卫星传输
- HLS-fmp4
  - 2017年8月正式进入ISO RFC8216标准，fMP4(分段式MP4)的封装格式正式被HLS支持
  - fMP4不是一种新的封装格式，而是在传统MP4格式基础上增加若干新特性。
    - 传统的MP4因为文件结构所限，所有的索引和数据都是放在一起的，在播放时两者缺一不可，无法实现流式直播。
    - fMP4主要是将MP4文件切割成更细的粒度，播放器可以加载一小部分数据之后就可以进行播放，这样就可以实现直播。 


## 播放模式

​		HLS流媒体播放，通常包含 直播Live 和 VOD （点播）两种：

- 点播VOD是当前时间点可以获取到所有index文件和ts文件
  - 二级index文件中记录了所有ts文件的地址
  - 允许客户端访问全部内容
  - VOD点播拥有先进的下载技术
  - 包括加密认证技术和动态切换文件传输速率的功能（通常用于不同分辨率视频之间的切换）
    - 除非客户端进行比特率切换，否则无需再下载任何 index文件，只需顺序下载 ts 文件并播放就可以了
-  Live模会实时生成M3u8和ts文件，它的索引文件一直处于动态变化
  - 播放的时候需要不断下载二级index文件，以获得最新生成的ts文件播放视频
    - 播放的同时，新ts文件也在被生成中
    - 所以客户端实际上是下载一次二级index文件，然后下载ts文件，再下载二级index文件
    - 这个时候这个二级index文件已经被重写，记录了新生成的ts文件的下载地址，再下载新ts文件，如此反复进行播放
  - 如果索引文件的末尾没有`#EXT-X-ENDLIST`标志，说明它是一个Live视频流



## 流程



​		客户端与服务器通过 HTTP 协议进行交互，以两级 m3u8 嵌套为例，客户端先 GET 请求到一级 m3u8，一级 m3u8 里面包含了服务器端可以用于传播的一个或多个不同带宽的 URL，这 URL 可以获取到二级 m3u8；二级 m3u8 包含了多个 ts 分片的 duration 及其 URL， 最后通过这个 URL 下载 ts 分片。

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/8763cde2e6c08486f60fddebde268a45.png)





## 改进

​		每次请求 TS 或 M3U8 有可能一个新的连接请求, 无法有效的标识客户端, 一旦出现问题, 基本无法有效的定位问题。

​		一般工业级的服务器都会对传统的 HLS 做一些改进，常见优化是对每个M3U8文件增加Session来标识一条 HLS 连接。

​		不管通过哪种方式, 最终通过唯一的 id 来标识一条流,，这样在排查问题时就可以根据这个 id 来定位播放过程中的问题。



## 应用场景

- 跨平台：PC 主要的直播方案是 RTMP，也有一些库能播放 HLS，譬如 jwplayer，基于 osmf 的 hls 插件也一大堆
  - 所以实际上如果选一种协议能跨 PC/Android/IOS，那就是 HLS
- IOS 上苛刻的稳定性要求：IOS 上最稳定的当然是 HLS， 稳定性不差于 RTMP 在 PC-flash上的表现
- 友好的 CDN 分发方式：目前 CDN 对于 RTMP 也是基本协议，但是 HLS 分发的基础是 HTTP，所以 CDN 的接入和分发会比 RTMP 更加完善。能在各种 CDN 之间切换，RTMP 也能，只是可能需要对接测试
- 简单：HLS 作为流媒体协议非常简单，apple 支持得也很完善。Android 对 HLS 的支持也会越来越完善。

> SRS 支持 HLS 主要是作为输出的分发协议，直播以 RTMP+HLS 分发，满总各种应用场景。点播以 HLS 为主。



## 优缺点

​		优点：

- 使用 HTTP 协议网络兼容性好
  - HTTP 协议网络兼容性好，可以方便的透过防火墙或者代理服务器
  - 可以很方便的利用 CDN 进行分发加速
- 客户端实现起来也很方便
  - 只需要支持 HTTP 请求即可, HTTP 协议无状态, 只需要按顺序下载媒体片段即可
- 自带多码率自适应机制
  - 客户端可以选择从许多不同的备用源中以不同的速率下载同样的资源（多码流自适应） 
  - 允许流媒体会话适应不同的数据速率


​		缺点：

- 相比 RTMP 这类长连接协议，延时较高, 难以用到互动直播场景
  - 最少有一个分片
  - 苹果公司在WWDC2019发布了新的解决方案来优化延迟性能，使得延迟从8s减低到1~2s

- 对于点播服务来说， 海量碎片文件在文件分发、 一致性缓存、 存储等方面都有较大挑战
- 存储多个比特率版本的内容需要更多的服务器空间和带宽，可能会增加成本
- 以不同的比特率和块来准备内容会增加内容创建和管理过程的编码复杂性

## 现状

​		传统直播技术使用的传输协议主要是`RTMP`/`HTTP-FLV`。

![图片](https://i0.hdslb.com/bfs/article/622c8e53273522795efd207d8b5f0a25474be8b2.png@1256w_368h_!web-article-pic.webp)

- `RTMP/FLV`标准已经超过10年没有更新，在兼容性层面已经面临着很大挑战
  - 对`h265`的支持，当前业界是通过`hack`的方式进行追加
    - 但如`ffmpeg`都需要打补丁之后才能支持，各种系统工具支持性较差
    - 甚至也会出现各家`hack`的方式不同，导致系统无法对接
    - 涉及到出海的业务，由于无法对齐国际标准，难以与海外公司展开技术合作

- 主流的浏览器原生不支持`FLV`

  - 需要在`web`端进行一次额外的转封装操作
  - 会有额外开销
  - 在用户设备负载较高时，可能会影响观看体验

- 在流式协议上构建`P2P`成本较高，通常也会牺牲标准的兼容性

  - 在直播场景中，非常适合`p2p`的应用，可以极大降低带宽成本



- `HLS(fmp4)`标准一直处于`MPEG`标准的维护中
  - 对各种编码器和新技术， 如`AV1`，`VVC`支持度非常完善
  
  - 主流浏览器也支持`fmp4`格式，省去了中间转封装的开销，可以减少播放端处理压力
  
- `CDN`部署更容易
  - 因为`HLS`是基于文件传输
  - 传统的静态文件`CDN`只需要少量配置更改就可以支持相关业务，对传输内容不敏感
  
- `HLS(fmp4)`在`DRM`支持性上非常好，可以对版权内容做更好的保护

- 利用切片文件实现`P2P`较为方便

  - 文件`P2P`方案在业界成熟
  - 也可以做到比较好的分享率
  - 在直播场景中`P2P`可以有效节省带宽成本

> 缺点：
>
> ​		hls相对rtmp及httpflv的优点，我想仅仅是能够html5播放，仅此而已，hls本身有很多不好的设计，比如ts文件，ts文件是不停的在http请求，这就相当于http和长连接的区别，不仅带来服务器的资源消耗，还有http报文头一堆冗余信息。这对流媒体直播那金贵流量费来说，这不不是一个好选择。hls还有个致命的问题，那就是延时，本身就是分文件块传输的，延时自然比rtmp高出许多。如果将hls的ts文件缩小，或许稍许降低延时。但hsl真的不是面向大众直播的第一选择。

