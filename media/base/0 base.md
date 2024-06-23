# media base

---



## 流媒体

​		使用非明确边界的数据连续传输技术。

- 狭义上是音频和视频形成稳定和连续的传输流和回放流的一系列技术、方法和协议的总称，即流媒体技术；
- 广义上的流媒体是相对于传统的 下载-回放方式而言的，指的是一种从Internet上获取音频和视频等多媒体数据的新方法，它能够支持多媒体数据流的实时传输和实时播放。

​		目前实现流媒体传输主要有两种方法：顺序流（progressive streaming）传输和实时流（realtime streaming）传输，它们分别适合于不同的应用场合。

### 顺序流传输

​    	顺序流传输采用顺序下载的方式进行传输，在下载的同时用户可以在线回放多媒体数据，但给定时刻只能观看已经下载的部分，不能跳到尚未下载的部分，也不能在 传输期间根据网络状况对下载速度进行调整。

​		由于标准的HTTP服务器就可以发送这种形式的流媒体，而不需要其他特殊协议的支持，因此也常常被称作HTTP 流式传输。

​		顺序流式传输比较适合于高质量的多媒体片段，如片头、片尾或者广告等。

### 实时流传输

​    	实时流式传输保证媒体信号带宽能够与当前网络状况相匹配，从而使得流媒体数据总是被实时地传送，因此特别适合于现场事件。

​		实时流传输支持随机访问，即用户 可以通过快进或者后退操作来观看前面或者后面的内容。从理论上讲，实时流媒体一经播放就不会停顿，但事实上仍有可能发生周期性的暂停现象，尤其是在网络状 况恶化时更是如此。与顺序流传输不同的是，实时流传输需要用到特定的流媒体服务器，而且还需要特定网络协议的支持。







## 音视频封装

​		封装格式（容器），将已经编码压缩好的图片流、音频流、字幕信息按照一定的方案组织在一个文件中，便于软件播放。

​		一般来说，视频文件的后缀就是它的封装格式：

```
AVI / MKV / MPE / MPG / MPEG 
MP4 / WMV / MOV / 3GP
M2V / M1V / M4V / OGM
RM / RMS / RMM / RMVB / IFO
SWF / FLV / F4V
ASF / PMF / XMB / DIVX / PART
DAT / VOB / M2TS / TS / PS
```

​		H264 + AAC封装为FLV或MP4是最为流行的封装模式。



## 音视频同步

### 时间戳

- DTS（Decoding Time Stamp） 解码时间戳

  ​	播放器根据DTS解码一帧数据

- PTS（Presentation Time Stamp）显示时间戳

  ​	播放器根据PTS展示一帧数据

### 同步方式

- Audio Master：同步到音频
- Video Master： 同步音频到视频
- External Clock Master：同步音频和视频到外部时钟

​		一般情况下 Audio Master > External  Clock Master > Video Master





## 流媒体基本原理

### 录制

![image-20220816211216411](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220816211216411.png)

### 播放

![image-20220816211236450](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220816211236450.png)



















