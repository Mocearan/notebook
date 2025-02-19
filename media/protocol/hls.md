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



## 流程



​		客户端与服务器通过 HTTP 协议进行交互，以两级 m3u8 嵌套为例，客户端先 GET 请求到一级 m3u8，一级 m3u8 里面包含了服务器端可以用于传播的一个或多个不同带宽的 URL，这 URL 可以获取到二级 m3u8；二级 m3u8 包含了多个 ts 分片的 duration 及其 URL， 最后通过这个 URL 下载 ts 分片。

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/8763cde2e6c08486f60fddebde268a45.png)



## m3u8 索引文件

​		也称m3u8播放列表（Playlist）。由多个独立行组成的文本文件，必须通过 URI（.m3u8 或 .m3u）或者 HTTP Content-Type 来识别（application/vnd.apple.mpegurl 或 audio/mpegurl）。

- 每行由用` \n `或者` \r\n `来标识换行

- 每一行可以是一个 URI、空白行或是一个 以` #` 号开头的字符串

  - 以` #` 开头的是 `tag `或者注释
  - 以 `#EXT `开头的是 `tag`
    - 有些 `tag `带有属性值，多个属性用逗号分隔
  - 其余的为注释， 在解析时应该忽略
  - `URI `表示 `ts `分片地址或是 `Playlist `地址
    -  URI 可以用绝对地址或者相对地址
    - 如果使用相对地址，那么是相对于当前 `Playlist `的地址

  

​		索引文件可以嵌套，一般只有一级索引和二级索引。

​		媒体流封分片装格式只支持 MPEG-2 传输流（ts）、WebVTT[WebVTT]文件或 Packed Audio 文件。

- 直播：随着直播流的进行，切片器对实时流进行切片，生成新的切片后需要更新m3u8文件并老化前面的TS切片文件
  - 播放器不停的请求索引文件和切片文件下载媒体数据 

- 录播：会带有`#EXT-X-ENDLIST`标签，表示录像文件的播放，切片器会对整个文件进行切片，并生成切片文件和m3u8
  - 此时索引文件和切片文件在播放过程中不允许改变



### 定义

- Media Playlist：二级 m3u8，携带 ts 分片 url 的 m3u8；
- Master Playlist：一级 m3u8；
- Media Segment：ts 分片；
- Attribute Lists：属性列表
  - 有的 tags 的值带有 Attribute Lists
  - 一个 Attribute List 是一个用逗号分隔的 attribute/value 对列表
  - 格式为：AttributeName=AttributeValue

### tag 分类

​		`tag` 以 `#EXT` 开头

#### basic tags

​		可以用在 Media Playlist 和 Master Playlist 里面。

- EXTM3U：必须在文件的第一行，标识是一个 Extended M3U Playlist 文件。
- EXT-X-VERSION：表示 Playlist 兼容的版本。



#### media segment tags

​		每一个 Media Segment 通过一系列的 Media Segment tags 跟一个 URI 来指定。有的 Media Segment tags 只应用于下一个 segment，有的则是应用所有下面的 segments。一个 Media Segment tag 只能出现在 Media Playlist 里面。

- **EXTINF**：用于指定 Media Segment 的 duration。
- EXT-X-BYTERANGE：用于指定 URI 的 sub-range。
- EXT-X-DISCONTINUITY：表示后续分片属性发生变化，如文件格式/编码/序号。
- EXT-X-KEY：表示 Media Segment 已加密，该值用于解密。
- EXT-X-MAP：表示 Media Segment 的头部信息，比如 PAT/PMT 或者 WebVTT 头。
- EXT-X-PROGRAM-DATE-TIME：和 Media Segment 的第一个 sample 一起来确定时间戳。



#### media playlist tags

​		Media Playlist tags 描述 Media Playlist 的全局参数。同样地，Media Playlist tags 只能出现在 Media Playlist 里面。

- EXT-X-TARGETDURATION：用于指定最大的 Media Segment duration。
- EXT-X-MEDIA-SEQUENCE：用于指定第一个 Media Segment 的序号。
- EXT-X-DISCONTINUITY-SEQUENCE：用于不同 Variant Stream 之间同步。
- EXT-X-ENDLIST：表示 Media Playlist 结束。
- EXT-X-PLAYLIST-TYPE：可选，指定整个 Playlist 的类型。
- EXT-X-I-FRAMES-ONLY：表示每个 Media Segment 均为 I-frame。



#### master playlist tags

​		Master Playlist tags 定义 Variant Streams，Renditions 和其他显示的全局参数。Master Playlist tags 只能出现在 Master Playlist 中。

- EXT-X-MEDIA：用于关联同一个内容的多个 Media Playlist 的多种翻译。
- EXT-X-STREAM-INF：用于指定下级 Media Playlist 相关属性。
- EXT-X-I-FRAME-STREAM-INF：与 EXT-X-STREAM-INF 类似，但指向的下级 Media Playlist 包含 Media Segment 均为 I-frame。
- EXT-X-SESSION-DATA：可以随意存放一些 session 数据。



#### media or master playlist tags

​		这里的 tags 可以出现在 Media Playlist 或者 Master Playlist 中。但是如果同时出现在同一个 Master Playlist 和 Media Playlist 中时，必须为相同值。

- EXT-X-INDEPENDENT-SEGMENTS：表示每个 Media Segment 可以独立解码。
- EXT-X-START：标识一个优选的点来播放这个 Playlist。



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

    - ` EXT-X-STREAM-INF`：格式`[attribute=value][，attribute=value]*`
      - `attribute`列表包含了可变码率的描述信息
  
      - `BANDWIDTH `指定码率峰值，单位`bits/s`；
  
      - `AVERAGE-BANDWIDTH`表示媒体流的的平均比特率，单位`bits/s`
  
      - `CODECS`包含媒体流中音视频编码格式相关的信息
  
      - `RESOLUTION`包含媒体流的中对应视频流的分辨率
  
      - `FRAME-RATE`视频流的帧率
  
      - `PROGRAM-ID `唯一ID，没啥用处 
  
  - 二级索引文件如上所示是真正记录了同一码率下TS切片序列的下载地址
  
    - 主索引文件和子索引文件都是`.M3U8`的`playlist`
  
    - 主索引文件只需下载一次，但对于直播节目子索引文件定期重新加载
    - 客户端可能会在任何时候改变到备用流,所有的切换都应该使用相同的音频文件
  

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



## 应用场景

- 跨平台：PC 主要的直播方案是 RTMP，也有一些库能播放 HLS，譬如 jwplayer，基于 osmf 的 hls 插件也一大堆
  - 所以实际上如果选一种协议能跨 PC/Android/IOS，那就是 HLS
- IOS 上苛刻的稳定性要求：IOS 上最稳定的当然是 HLS， 稳定性不差于 RTMP 在 PC-flash上的表现
- 友好的 CDN 分发方式：目前 CDN 对于 RTMP 也是基本协议，但是 HLS 分发的基础是 HTTP，所以 CDN 的接入和分发会比 RTMP 更加完善。能在各种 CDN 之间切换，RTMP 也能，只是可能需要对接测试
- 简单：HLS 作为流媒体协议非常简单，apple 支持得也很完善。Android 对 HLS 的支持也会越来越完善。

> SRS 支持 HLS 主要是作为输出的分发协议，直播以 RTMP+HLS 分发，满总各种应用场景。点播以 HLS 为主。

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

