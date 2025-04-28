# HTTP-FLV

​		HTTP-FLV（Flash Video over HTTP）是一种利用 HTTP 协议传输 FLV（Flash Video）格式的数据流媒体协议。

---

​		HTTP-FLV 主要用于实时流媒体播放，特别是在直播场景（低延迟、高效率）。

​		FLV 格式曾经是 Adobe Flash Player 使用的主要视频格式 ，尽管 Flash Player 已逐渐退出历史舞台，但 FLV 格式由于其封装简洁、播放灵活，依然在一些流媒体应用中保有一席之地。



## 参考

[HTTP协议下可拖动时间轴播放FLV的实现（伪流媒体） - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7279433.html)

[HTTP-FLV详解及分析_flv协议解析-CSDN博客](https://blog.csdn.net/qq_41839588/article/details/134340606)

[HTTP-FLV协议 抓包分析详解_宁静致远的技术博客_51CTO博客](https://blog.51cto.com/fengyuzaitu/9017088)

[HTTP协议详解之HTTP/1.1-CSDN博客](https://blog.csdn.net/weixin_39399492/article/details/129936400)

[从零开始搭建流媒体推流/拉流服务：深入解析HTTP-FLV的实现流程 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/700594706)

## 优缺点

​	优点：

- **低延迟**，接近于实时地观看直播内容
- **高效传输**：FLV 格式的数据封装简单，传输效率高
- **广泛支持**：HTTP 是一种标准的网络协议，得到广泛支持
  
  - 任何支持 HTTP 协议的服务器和客户端都可以使用 HTTP-FLV 进行视频流传输，无需特殊配置
  - 很多防火墙会墙掉RTMP，但是不会墙HTTP，因此HTTP-FLV出现奇怪问题的概率很小
  - HTTP-FLV流支持`http 302`
    - 方便CDN纠正DNS的错误
    - 在流媒体服务器需要更改流的源地址时，也可以通过重定向来通知客户端获取新的流源地址。
    - RTMP也有个302，可惜是播放器as中支持的
    - http 302可参考：[http状态码301和302详解及区别](https://baijiahao.baidu.com/s?id=1737055463583543990&wfr=spider&for=pc)
- **播放灵活**：边下载边播放，用户无需等待整个视频文件下载完成即可开始观看
- **简单实现**：实现 HTTP-FLV 相对简单，使用现有的 HTTP 服务器和客户端即可搭建流媒体传输系统
  
  - 开发和部署成本低，适合中小型流媒体服务提供商
  - flv是最简单的流媒体封装，http是最广泛的协议，这两个到一起维护性很高
- **兼容性**：虽然 Flash 技术逐渐被淘汰，但许多视频播放器仍支持 FLV 格式
  
  - 使用 HTTP-FLV 协议传输视频数据，可以兼容现有的 FLV 播放器，方便用户观看
  
  - > ​	**客户端**则需要使用支持 HTTP-FLV 的播放器（如 VLC 或基于 HTML5 的播放器），接收流数据并将它们组装成完整的 FLV Tag，就可以开始解码和播放测试视频了。

​	

​		缺点：

- 不支持双向通信：HTTP-FLV是一种单向的传输协议，只能由服务器向客户端发送数据，无法实现双向通信。如果需要实现双向通信（如客户端向服务器发送控制指令），则需要额外的机制或协议来实现。



​	使用场景：

（1）直播场景：HTTP-FLV可以用于实现实时的音视频直播，支持大规模用户同时观看直播内容。

（2）视频点播场景：HTTP-FLV可以用于实现在线视频点播，用户可以随时选择观看视频内容。

## 原理

​		 HTTP-FLV 利用 HTTP/1.1 分块传输机制发送 FLV 数据。虽然直播服务器无法知道直播流的长度，但是 HTTP/1.1 分块传输机制可以不填写 `conten-length` 字段而是携带 `Transfer-Encoding: chunked` 字段（`Connection:keep-alive`），这样客户端就会一直接受数据。

​		HTTP-FLV就是HTTP+FLV，将音视频数据封装成FLV格式，然后通过HTTP协议传输给客户端。

- 因为`FLV`格式带着时间戳，可以接收到数据直接解码播放，因此不需要添加时间戳的特殊流媒体协议



​		所有的HTTP FLV流都是一个HTTP FLV地址，譬如：`http://ossrs.net:8081/live/livestream.flv`，但是，流的形式却至少有三种：

- FLV文件，渐进式HTTP流
  - 放一个文件到nginx目录，可以访问下载在播放器播放，这是HTTP FLV文件，也就是渐进式下载流
  - 所谓渐进式下载，也就是用户观看时无法从未下载的地方开始看。
- FLV伪流
  - 一般说的HTTP FLV，比上面的渐进式流高级一点，譬如，一个120分钟的电影，作为渐进式流播放时，用户需要从60分钟开始看，如何支持呢？因为nginx是当做文件下载的，无法直接跳转到第60分钟（nginx也不知道60分钟对应的字节偏移是多少呀）
  - 后来有人就支持这种跳着播放，通过指定时间服务器从指定的位置开始给流，这种支持`flv?start=`，就是http flv的伪流，本质上还是点播流
- FLV直播流
  - 严格意义上的直播流
    - 有RTMP的所有特征，譬如集群、低延迟、热备、GOP cache
    - 而且有HTTP的优势，譬如302、穿墙、通用

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/27228626-2debcab534097cbe.png)



## 流程

​		客户端如果要使用HTTP-FLV协议播放直播内容，会先发送http get请求，来请求数据。响应头中的内容是我们理解HTTP-FLV的关键：

- 服务器的响应头中没有Content-Length

  > 如果有Content-Length则收到Content-Length大小的数据就会认为数据传输完就会断开连接。
  >
  > 如果响应头中没有Content-Length这个字段，客户端就会一直接收数据，直到socket连接断开。
  >
  > HTTP-FLV就是利用了这个原理，响应头中没有Content-Length，客户端就会一直接收数据。

- 使用分块传输的方式传输数据，服务器的响应头中Transfer-Encoding:chunked

  - 表示服务器端将向客户端发送一个空块，以指示响应的结束

  > 在实际测试中，响应头中也会出现没有Transfer-Encoding:chunked，而有Connection:close的情况，用于告诉http客户端继续接收数据，直到服务器端关闭连接。



### 整体流程

- rtmp推流到服务器，服务器转换成HTTP-FLV流

  - 直播场景一般是RTMP推，HTTP-FLV拉，这样`FLV`数据不用动，只用做协议转换就可以了

- 客户端播放，先发送`http get`请求到服务器

  - 请求的uri就是HTTP-FLV的播放地址，形如`http://ossrs.net:8081/live/livestream.flv`
  - ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/4bffefc0e5084ec953063b75dc9f3b73.png)

- 服务器向客户端发送`http`响应和发送`flv`格式的数据

  - 服务器向客户端发送数据的同时，客户端也在不停的发送ACK
  - ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/c417826b566ddc3a92e50bb19ecb7fa6.png)
  - 第一个flv包发送的是`script tag`，内容是`metadata`
    - 无论任何时刻进入直播间，无论当前音视频数据是什么，都要先发送`metadata`
    - FLV格式顺序发送数据
      - `FLV Header(9 byte) |  PreviousTagSize | Tag(meta) |  PreviousTagSize | Tag(video) |  PreviousTagSize | Tag(audio) | .... | PreviewTagSize` 

- 观众（客户端）侧收到数据后会进行组包操作，组成一个个完整的`flv tag`，然后进行播放

  - 按照flv封装格式的数据顺序发送，并且`flv header，previous tag size，tag header`大小都是固定的

  - 然后按照FLV流格式流式解析即可

    > 当获取到tag data的大小后，再根据接收到的数据长度来判断是否完整的接收了一个tag的数据，如果没有接收完数据则可以继续读取服务端发来的下一个数据包。如果接收完，则可以开始对下一个tag进行组包的操作。同时可以对组好包的tag进行播放或其它操作。

- 断开连接，用户直播结束，服务器主动断开连接 或 观众退出直播间，客户端主动断开连接

  - 服务器的http响应头中没有Content-Length，因为在直播场景中音视频数据是不间断产生的，所以服务器在发送数据时不知道数据的总大小，此时不能用Content-Length
  - 但是有Transfer-Encoding:chunked说明要分块传输数据
    - 分块发送数据，直到服务器向客户端发送一个空块，以指示响应的结束，才可以断开连接
    - 此时Connection:keep-alive
  - 也可以没有Transfer-Encoding:chunked，此时Connection:Close
    - 用于告诉http客户端继续接收数据，直到服务器端关闭连接