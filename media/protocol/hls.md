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
- 
- 
- 
  
  [C++大型流媒体项目-从底层到应用层千万级直播系统实战_实战课程_慕课网 (imooc.com)](https://coding.imooc.com/class/857.html)

[HTTP-FLV协议 抓包分析详解_宁静致远的技术博客_51CTO博客](https://blog.51cto.com/fengyuzaitu/9017088)

[【音视频】HTTP-FLV协议-云社区-华为云 (huaweicloud.com)](https://bbs.huaweicloud.com/blogs/431811)

[FLV 和 HTTP-FLV - 简书 (jianshu.com)](https://www.jianshu.com/p/b90c6ef5972a)

[HTTP-FLV详解及分析_flv协议解析-CSDN博客](https://blog.csdn.net/qq_41839588/article/details/134340606)

[HTTP-FLV协议详解_flv文件变成hrrp流-CSDN博客](https://blog.csdn.net/weixin_39399492/article/details/131540586)

[从零开始搭建流媒体推流/拉流服务：深入解析HTTP-FLV的实现流程 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/700594706)

[流媒体协议之HLS详解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/693226518)

[什么是 HLS 流媒体？HLS 流媒体的应用场景及优缺点- ZEGO即构科技](https://www.zego.im/blog/1502.html)

[RTSP、RTMP、HLS流媒体协议的区别与联系 - 简书 (jianshu.com)](https://www.jianshu.com/p/f0556b885db6)

[HLS 协议详解及 HLS 速度优化 - 又拍云 (upyun.com)](https://www.upyun.com/tech/article/201/HLS 协议详解及 HLS 速度优化.html)

[音视频开发（6）RTMP、HLS等协议详细剖析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/698344293)

[流媒体之HLS协议(其三)-CSDN博客](https://blog.csdn.net/weixin_61503529/article/details/141999295)

[HLS直播协议详解_hls协议-CSDN博客](https://blog.csdn.net/qq_41839588/article/details/134093390)

[Qt实战--多画面播放器_ithewei的博客-CSDN博客](https://blog.csdn.net/gg_simida/category_9275796.html)

[ithewei/hplayer: A multi-screen player using Qt + FFmpeg. (github.com)](https://github.com/ithewei/hplayer/tree/master)

## 优缺点

​		优点：

- 使用 HTTP 协议网络兼容性好
  - HTTP 协议网络兼容性好，可以方便的透过防火墙或者代理服务器
  - 可以很方便的利用 CDN 进行分发加速
- 客户端实现起来也很方便
  - 只需要支持 HTTP 请求即可, HTTP 协议无状态, 只需要按顺序下载媒体片段即可
- 自带多码率自适应机制

​		缺点：

- 相比 RTMP 这类长连接协议，延时较高, 难以用到互动直播场景
- 对于点播服务来说， 海量碎片文件在文件分发、 一致性缓存、 存储等方面都有较大挑战



## 流派

​		HLS有两个版本，主要区别在于使用的封装容器不同。

- HLS-mpegts
  - 基于mpegts封装，mpegts因为有出色的纠错能力，常用于广播电视卫星传输
- HLS-fmp4
  - 2017年8月正式进入ISO RFC8216标准，fMP4(分段式MP4)的封装格式正式被HLS支持
  - fMP4不是一种新的封装格式，而是在传统MP4格式基础上增加若干新特性。
    - 传统的MP4因为文件结构所限，所有的索引和数据都是放在一起的，在播放时两者缺一不可，无法实现流式直播。
    - fMP4主要是将MP4文件切割成更细的粒度，播放器可以加载一小部分数据之后就可以进行播放，这样就可以实现直播。 



## 架构

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
- 客户端软件请求媒体资源
  - 下载资源
  - 重新组装成用户可以观看的连续流

![图片](https://i0.hdslb.com/bfs/article/4d837cb046568d2a1b4ff855ff2c7f990800527b.png@1256w_488h_!web-article-pic.webp)

![img](https://pic4.zhimg.com/80/v2-6cf6f240d7c75785cb0461f0f7fd1f97_720w.webp)



## m3u8 索引文件

​		也称m3u8播放列表。

- 直播：随着直播流的进行，切片器对实时流进行切片，生成新的切片后需要更新m3u8文件并老化前面的TS切片文件
- 录播：会带有`#EXT-X-ENDLIST`标签，表示录像文件的播放，切片器会对整个文件进行切片，并生成切片文件和m3u8
  - 此时索引文件和切片文件在播放过程中不允许改变
- 播放器不停的请求索引文件和切片文件下载媒体数据

### 未加密HLS

```m3u8
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:10
#EXT-X-MEDIA-SEQUENCE:0
#EXTINF:10, no desc
fileSequence0.ts
#EXTINF:10, no desc
fileSequence1.ts
#EXTINF:10, no desc
fileSequence2.ts
#EXTINF:10, no desc
fileSequence3.ts
#EXTINF:10, no desc
fileSequence4.ts
#EXTINF:10, no desc
fileSequence5.ts
#EXTINF:10, no desc
fileSequence6.ts
#EXTINF:10, no desc
fileSequence7.ts
```

- `EXTM3U`: 作为M3U文件的标识符，文件类型标识，必须要有
- `EXT-X-VERSION`: 协议的版本号，而且每个`M3U8`中只能出现一次该标签
  - 这里版本号为3
- `EXT-X-TARGETDURATION`: 表示最长分片的时长，必须字段，秒为单位,
  - 这里是10秒一个分片，也就是说这些ts切片的时长不能大于10的
- `EXT-X-MEDIA-SEQUENCE`：表示索引文件中第一个分片的序列号（整数值）
  - 如果M3U8文件中没有该字段，则`playlist`中第一个分片的序列号必须是`0`
- `EXTINF`：格式 `EXTINF:[duration],[title]`
  - `duration `表示下一个分片的时长，对于每个分片，必须有该字段
    - 对于`#EXT-X-VERSION`小于`3`的情况下，`duration`必须是整数
    - 其他情况下`duration`可以是浮点数和整数
  - `title：TS`切片文件的`URL`地址
    - 可以是相对地址，例如`2000kbps-0001.ts`，其标识地址与m3u8一致
    - 也可以是完整的地址，`http://xxxxxx.cn:5555/test/1102/test/segments3.ts`
- `EXT-X-ENDLIST`，表示录像文件的播放，是整个播放文件的索引列表，此时索引文件和切片文件在播放过程中不允许改变
- `EXT-X-PLAYLIST-TYPE`，可取值为`EVENT/VOD`
  - `EVENT`指的是分片工具只能在M3U8末尾添加新的分片的信息，但不能删除老的分片，通常比较适用于直播+录播的情况
  - `VOD`表示对录像文件进行的切片
  - 如果存在`#EXT-X-ENDLIST`标签时，可以忽略`EXT-X-PLAYLIST-TYPE`

- 为了能够实现码率自适应，HLS存在二级索引结构

  - 一级索引文件存放的是不同码率的HLS源的M3U8地址，也就是二级索引文件的地址

    ```m3u8
    #EXTM3U
    #EXT-X-STREAM-INF:PROGRAM-ID=1, BANDWIDTH=1280000,AVERAGE-BANDWIDTH=1000000, RESOLUTION=704x576
    http://example.com/d1.m3u8
    #EXT-X-STREAM-INF:PROGRAM-ID=2, BANDWIDTH=2560000,AVERAGE-BANDWIDTH=2000000, RESOLUTION=1280x720
    http://example.com/720p.m3u8
    #EXT-X-STREAM-INF:PROGRAM-ID=3, BANDWIDTH=7680000,AVERAGE-BANDWIDTH=6000000, RESOLUTION=1920x1080
    http://example.com/1080p.m3u8
    ```

  - 二级索引文件如上所示是真正记录了同一码率下TS切片序列的下载地址

### 加密HLS

```m3u8
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-KEY:METHOD=AES-128,URI="http://xxxxxx:5555//test/1102/test/segments.key" 
#EXT-X-MEDIA-SEQUENCE:0
#EXT-X-ALLOW-CACHE:YES
#EXT-X-TARGETDURATION:19
#EXTINF:13.966667,
http://xxxxxx:5555/test/1102/test/segments0.ts
#EXTINF:10.000000,
http://xxxxxx:5555/test/1102/test/segments1.ts
#EXTINF:10.000000,
http://xxxxxx:5555/test/1102/test/segments2.ts
#EXTINF:10.000000,
http://xxxxxx.cn:5555/test/1102/test/segments3.ts
#EXTINF:10.000000,
http://xxxxxxn.cn:5555/test/1102/test/segments4.ts
#EXTINF:7.033333,
http://xxxxxx:5555/test/1102/test/segments5.ts
#EXTINF:10.000000,
```

- `EXT-X-KEY`，m3u8给规定好的加密字段
  - 如果包含这个字段播放器就会先去请求这个`key`，然后拿这个这个`key`去访问加密的`TS`视频就可以播放了



## 改进

​		每次请求 TS 或 M3U8 有可能一个新的连接请求, 无法有效的标识客户端, 一旦出现问题, 基本无法有效的定位问题。

​		一般工业级的服务器都会对传统的 HLS 做一些改进，常见优化是对每个M3U8文件增加Session来标识一条 HLS 连接。

​		不管通过哪种方式, 最终通过唯一的 id 来标识一条流,，这样在排查问题时就可以根据这个 id 来定位播放过程中的问题。

## 现状

​		传统直播技术使用的传输协议主要是`RTMP`/`HTTP-FLV`。

![图片](https://i0.hdslb.com/bfs/article/622c8e53273522795efd207d8b5f0a25474be8b2.png@1256w_368h_!web-article-pic.webp)

- `RTMP/FL`V标准已经超过10年没有更新，在兼容性层面已经面临着很大挑战
  - 对h265的支持，当前业界是通过hack的方式进行追加
    - 但如ffmpeg都需要打补丁之后才能支持，各种系统工具支持性较差
    - 甚至也会出现各家hack的方式不同，导致系统无法对接
    - 涉及到出海的业务，由于无法对齐国际标准，难以与海外公司展开技术合作

- 主流的浏览器原生不支持`FLV`

  - 需要在`web`端进行一次额外的转封装操作
  - 会有额外开销
  - 在用户设备负载较高时，可能会影响观看体验

- 在流式协议上构建`P2P`成本较高，通常也会牺牲标准的兼容性

  - 在直播场景中，非常适合`p2p`的应用，可以极大降低带宽成本



- HLS(fmp4)标准一直处于MPEG标准的维护中

  - 对各种编码器和新技术， 如AV1，VVC支持度非常完善

  - 主流浏览器也支持fmp4格式，省去了中间转封装的开销，可以减少播放端处理压力

- CDN部署更容易

  - 因为HLS是基于文件传输
  - 传统的静态文件CDN只需要少量配置更改就可以支持相关业务，对传输内容不敏感

- HLS(fmp4)在DRM支持性上非常好，可以对版权内容做更好的保护

- 利用切片文件实现P2P较为方便

  - 文件P2P方案在业界成熟
  - 也可以做到比较好的分享率
  - 在直播场景中P2P可以有效节省带宽成本

> 缺点：
>
> ​		hls相对rtmp及httpflv的优点，我想仅仅是能够html5播放，仅此而已，hls本身有很多不好的设计，比如ts文件，ts文件是不停的在http请求，这就相当于http和长连接的区别，不仅带来服务器的资源消耗，还有http报文头一堆冗余信息。这对流媒体直播那金贵流量费来说，这不不是一个好选择。hls还有个致命的问题，那就是延时，本身就是分文件块传输的，延时自然比rtmp高出许多。如果将hls的ts文件缩小，或许稍许降低延时。但hsl真的不是面向大众直播的第一选择。

