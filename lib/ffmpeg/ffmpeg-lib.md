

# ffmpeg-lib

---

- `libavformat`：封装格式的生成和解析。
- `libavcodec`：声音/图像编解码；
- `libavutil`：公共的工具函数；
- `libswscale`：视频场景比例缩放、 色彩映射转换；
- `libpostproc`：用于后期效果处理
- `libavdevice`：多媒体设备交互
- `libswresample`：高度优化的音频重采样、rematrixing 和样本格式转换操作
- `libavfilter`：过滤器，修改多媒体文件内容



## 重要数据结构

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

####  `AVFormatContext`

​		`AVFormatContext `是一个贯穿全局的数据结构。主要存储视音频封装格式中包含的信息。

#### `AVInputFormat / AVOutputFormat`

​		同一时间只能存在一个。

### 编解码层

- `AVStream`
- `AVCodecContext`
- `AVCodec`
- `AVPacket`：解码前数据
- `AVFrame`：解码后数据

#### `AVStream`

​		`AVStream`是一个贯穿全局的数据结构。保存数据流相关的编解码器、数据段信息。以及流概念本身的必要信息。

- 每个 AVStream 存储一个视频/音频流的相关数据；
- 每个`AVStream `对应一个 `AVCodecContext`，存储该视频/音频流使用解码方式的相关数据；

#### `AVCodecContext`

- 每个 `AVCodecContext `中对应一个 `AVCodec`，包含该视频/音频对应的解码器；



#### `AVCodec`

- 每种解码器都对应一个 AVCodec 结构。



## avdevice

​		用于和多媒体设备交互。使用这个库可以读取电脑（或者其他设备上）的多媒体设备的数据，或者输出数据到指定的多媒体设备上。

​		使用`libavdevice`读取数据和直接打开视频文件比较类似。因为系统的设备也被FFmpeg认为是一种输入的格式（即`AVInputFormat`）。

```c
AVFormatContext *pFormatCtx = avformat_alloc_context();
avformat_open_input(&pFormatCtx, "test.h265",NULL,NULL);
```

​		使用`libavdevice`的时候，唯一的不同在于需要首先查找用于输入的设备。

```c
//av_find_input_format()
AVFormatContext *pFormatCtx = avformat_alloc_context();
AVInputFormat *ifmt = av_find_input_format("vfwcap"); // vfw设备作为输入设备
avformat_open_input(&pFormatCtx, 0, ifmt,NULL); // URL中指定打开第0个设备（在我自己计算机上即是摄像头设备）。

// 在Windows平台上除了使用vfw设备作为输入设备之外，还可以使用DirectShow作为输入设备：
AVFormatContext *pFormatCtx = avformat_alloc_context();
AVInputFormat *ifmt=av_find_input_format("dshow");
avformat_open_input(&pFormatCtx,"video=HP HD Camera",ifmt,NULL) ;
```

- `URL`的格式是`"video={device_name}"`，但是设备名称外面不能加引号。

  - 例如在上述例子中URL是`"video=Integrated Camera"`，而不能写成`"video=\"Integrated Camera\""`，否则就无法打开设备。

  - 这与直接使用ffmpeg.exe打开`dshow`设备有很大的不同。

    > 命令为：`ffmpeg -list_options true -f dshow -i video="Integrated Camera"`

-  `Dshow`的设备名称必须要提前获取，在这里有两种方法

  - 通过FFmpeg编程实现

    > 相当于输入了下面一条命令` ffmpeg -list_devices true -f dshow -i dummy`

    ```shell
    //Show Device
    void show_dshow_device(){
    	AVFormatContext *pFormatCtx = avformat_alloc_context();
    	AVDictionary* options = NULL;
    	av_dict_set(&options,"list_devices","true",0);
    	AVInputFormat *iformat = av_find_input_format("dshow");
    	printf("Device Info=============\n");
    	avformat_open_input(&pFormatCtx,"video=dummy",iformat,&options);
    	printf("========================\n");
    }
    ```

    - 该方法好处是可以使用程序自动获取名称。但是当设备名称中包含中文字符的时候，会出现设备名称为乱码的情况。
      - 如果直接把乱码的设备名作为输入的话，是无法打开该设备的。这时候需要把乱码ANSI转换为UTF-8。



## avcodec



## avformat

- 包括获取解码所需信息以生成解码上下文结构和读取音视频帧等功能
- 包含 demuxers 和 muxer 库；





## 流程

#### 解码流程

​		ffmpeg4.x

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-47142db7477a14628a39cd46b5526e84_1440w.webp)



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

