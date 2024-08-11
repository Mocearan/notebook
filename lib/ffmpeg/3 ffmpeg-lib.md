

# ffmpeg-lib

---

- `libavformat`：封装格式的生成和解析。
  - 封装了Protocol层和Demuxer、Muxer层，使得协议和格式对于开发者来说是透明的。

- `libavcodec`：声音/图像编解码；
  - 有一些Codec是具备自己的License的，`FFmpeg`不会默认添加像`libx264`、`FDK-AAC`等库的
  - 但是FFmpeg就像一个平台一样
    - 可以在编译时自行决定将其他的第三方的`Codec`以插件的方式添加进来
    - 然后为开发者提供统一的接口

- `libavutil`：公共的工具函数；
- `libswscale`：视频场景比例缩放、 色彩映射转换；
  - 图像进行格式转换的模块(yuv->rgb)
  - 缩放（1280x730 -> 8xx x480）

- `libpostproc`：用于后期效果处理
  - 使用AVFilter的时候需要打开该模块的开关，因为Filter中会使用到该模块的一些基础函数。

- `libavdevice`：多媒体输入输出设备交互
  - 需要编译出播放声音或者视频的工具ffplay，就需要确保该模块是打开的
  - 同时也需要SDL的预先编译，因为该设备模块播放声音与播放视频使用的都是SDL库。

- `libswresample`：高度优化的音频重采样、`rematrixing `和样本格式转换操作
  - 可以对数字音频进行声道数、数据格式、采样率等多种基本信息的转换

- `libavfilter`：过滤器（滤镜），修改多媒体文件内容
  - 提供了包括音频特效和视频特效的处理



- 容器文件、容器格式：特定封装格式的多媒体文件/流，如mp4/flv/mkv/ts/ps
- 媒体流：一段连续的多媒体数据，一段视频、音频、字幕等。可以是编码的或非编码的
  - 有些音频码流是纯PCM的
- 数据帧/数据包：
  - 解码前用于传输或存储的一个数据单元成为数据包
  - 数据包解码后得到一或多个数据帧，数据帧是编码器最小处理单元

> `av_register_all()`注册所有组件，在ffmpeg 4.0中弃用。
>
> ` avdevice_register_all()`对设备进行注册，比如`V4L2`等
>
> `avformat_network_init()`;初始化网络库以及网络加密协议相关的库（比如`openssl`）



## 重要结构

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130914204051125)

### 协议层

​		主要存储视音频使用的协议的类型以及状态。

- `AVIOContext`
- `URLContext`
- `URLProtocol ` 

#### `URLProtocol`

- 每种协议都对应一个 URLProtocol 结构。
- FFMPEG 中文件也被当做一种存取协议



### 封装格式层

- `AVFormatContext`
- `AVInputFormat/ AVOutputFormat`

> `AVFormatContext `API调用 `AVInputFormat `，主要是FFMPEG内部调用

####  `AVFormatContext`

​		`AVFormatContext `是一个贯穿全局的数据结构。主要存储视音频封装格式中包含的信息。

#### `AVInputFormat / AVOutputFormat`

- `AVInputFormat `  demuxer 
- `AVOutputFormat  ` muxer

​		同一时间只能存在一个，每种封装格式（例如FLV, MKV, MP4, AVI）对应一个该结构体。

### 编解码层

- `AVStream`
- `AVCodecContext`
- `AVCodec`

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/acec70e8ccf982d2c7ef7b36aac8902d.png)

#### `AVStream`

​		`AVStream`是一个贯穿全局的数据结构。保存数据流相关的编解码器、数据段信息。以及流概念本身的必要信息。

- 每个 AVStream 存储一个视频/音频流的相关数据；
- 每个`AVStream `对应一个 `AVCodecContext`，存储该视频/音频流使用解码方式的相关数据；

#### `AVCodecContext`

- 每个 `AVCodecContext `中对应一个 `AVCodec`，包含该视频/音频对应的解码器；



#### `AVCodec`

- 每种解码器都对应一个 `AVCodec `结构。



### 数据层

- `AVPacket`：压缩编码数据
- `AVFrame`：像素或音频采样数据





## 组件注册

### ffmpeg 3.x

- 执行av_register_all，把全局的解码器、编码器等结构体注册到各自全局的对象链表里，以便后面查找调用。

![img](https://developer.qcloudimg.com/http-save/yehe-7620466/3ce9fafec0380072101a0f0e7f41b8ad.png)



### ffmpeg 4.x

FFmpeg内部去做，不需要用户调用API去注册。以codec编解码器为例：

1. 在configure的时候生成要注册的组件`./configure:7203:print_enabled_components libavcodec/codec_list.c AVCodec codec_list $CODEC_LIST`这里会生成一个`codec_list.c `文件，里面只有`static const AVCodec * const codec_list[]`数组。
2. 在`libavcodec/allcodecs.c`将`static const AVCodec * const codec_list[]`的编解码器用链表的方式组织起来。

#### ffmpeg 4.0.2

FFmepg内部去做，不需要用户调用API去注册。

对于demuxer/muxer（解复用器，也称容器）则对应

1. libavformat/muxer_list.c libavformat/demuxer_list.c 这两个文件也是在configure的时候生成，也就是说直接下载源码是没有这两个文件的。
2. 在libavformat/allformats.c将demuxer_list[]和muexr_list[]以链表的方式组织。





## 流程

### 解码流程

​		ffmpeg4.x

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-47142db7477a14628a39cd46b5526e84_1440w.webp)

## player framwork

![img](https://developer.qcloudimg.com/http-save/yehe-7620466/4fbd7c6180f0c68f7aa5dd604c4b04d0.png)
>>>>>>> Stashed changes:lib/ffmpeg/ffmpeg-lib.md



## config

### VS

​	在vs上配置ffmpeg库除了一般的库配置之外，需要以`c`的方式添加头文件。

```c++
#ifdef _WIN32

//Windows
extern "C"
{
#include <ffmpeg/libavutil/avutil.h>
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
	#include <ffmpeg/libavutil/avutil.h>
#ifdef __cplusplus
};

#endif
#endif
```

- 同时在Windows的项目配置中，链接配置的命令行中添加`/SAFESEH:NO `.

