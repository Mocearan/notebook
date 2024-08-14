[直播相关讲解 -- 视频直播技术大全、直播架构、技术原理和实现思路方案整理(转载)-CSDN博客](https://blog.csdn.net/weixin_46932303/article/details/121581351)

[HLS直播协议在B站的实践 - 哔哩哔哩 (bilibili.com)](https://www.bilibili.com/read/cv18179472/)



## 协议选型

### `RTMP/HTTP-FLV`

​		RTMP和HTTP-FLV二者都建立在FLV封装之上。

- `RTMP`一般用作直播源在直播系统内部的推流传递
  - 推流到流媒体服务器：
  - 推流到直播`CDN`
- `HTTP-FLV`一般用于向客户端推流观看

![img](https://pics2.baidu.com/feed/79f0f736afc37931d93673e7a7b7ad4942a91125.jpeg@f_auto?token=5769d0830d1dc1ab0599e1b1ab775b74)

​		对比：

#### `RTMP`

- 可以推流和拉流
- `rtmp://`，推流地址和播放地址一致
- 一般只用作直播源推流到流媒体、推流到CDN
  - 客户端浏览器摒弃`Flash`
  - 高并发下不稳定
- 需要支持`RTMP`的流媒体服务器
  - `SRS`
  -  `ZLMediaServer `
  - `Red5`
  - `Nginx-rtmp`
    - 加入了RTMP插件的Nginx
- 延迟是比较低的，大概在1-3秒左右
- 是建立在TCP长连接通道上的，在封装音视频数据时会强制切片，限制每个数据包的大小
  - 强制切片也一定程度保证了实时性
  - 有一定的弱网抵抗能力
    - 因为每个数据包都不会太大，所以当某个数据包校验失败时，重新发送的成本不会太大
  - 由于合并数据包会加大CPU压力，所以是有一定的性能消耗的
- RTMP协议还有一些变种协议，如RTMPT、RTMPS等

![img](https://pics5.baidu.com/feed/54fbb2fb43166d22464ce8c40b5013fb9052d222.jpeg@f_auto?token=37f1815829b008477c522b823d5d6e59)

#### `http-flv`

- 一般只能用作拉流

- `http://`，基于HTTP协议的HTTP-FLV可以简单地理解为RTMP的HTTP协议版本

  - 功能和工作原理上是相似
  - 切片数据

-  HTTP-FLV延迟会略高于RTMP，但是HTTP-FLV相对RTMP适配更多的播放场景

- 一般需要需加入插件才能播放，如网页需要引入`flv.js`

  - > B站开源的`flv.js`

- 需要特定的流媒体服务器

  - 加入了HTTP-FLV插件的Nginx

  - > Nginx的HTTP-FLV插件是包含RTMP功能的，所以一般HTTP-FLV的流媒体服务，推流是以RTMP协议，拉流是用HTTP-FLV协议

- 现在比较流行的方案是，直播源推流是RTMP协议，直播拉流观看是HTTP-FLV协议。

![img](https://pics6.baidu.com/feed/267f9e2f070828386bbed00eeaeab30d4d08f1fc.jpeg@f_auto?token=d62fca0268226ecc560fea58b2bf37ea)



### `HLS`

- 一般只用作拉流观看
- 从严格意义上讲，HLS协议并不是流式协议
  - 通过HTTP协议批量下载静态文件
  - 这些静态文件都是直接写入磁盘的
  - 多个只有几秒长度的**.ts碎片视频文件**
  - 记录视频文件地址的**.m3u8索引文件**
- `http://****.m3u8`
  - 实际上这个地址就是索引文件的地址
  - 客户端获取到索引文件后，就可以下载对应的碎片视频文件并开始播放了