

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

```c
• avcodec_alloc_context3(): 分配解码器上下文
• avcodec_free_context():释放解码器上下文，包含了avcodec_close()
    
• avcodec_find_decoder()：根据ID查找解码器
• avcodec_find_decoder_by_name():根据解码器名字
    
• avcodec_open2()：打开编解码器
• avcodec_close():关闭解码器
    
• avcodec_decode_video2()：解码一帧视频数据
• avcodec_decode_audio4()：解码一帧音频数据
• avcodec_send_packet(): 发送编码数据包
• avcodec_receive_frame(): 接收解码后数据

• avcodec_close():关闭解码器
```



## avformat

- 包括获取解码所需信息以生成解码上下文结构和读取音视频帧等功能
- 包含 demuxers 和 muxer 库；

```c
◼ avformat_alloc_context();负责申请一个AVFormatContext结构的内存,并进行简单初始化
◼ avformat_free_context();释放该结构里的所有东西以及该结构本身
    
◼ avformat_close_input();关闭解复用器。关闭后就不再需要使用avformat_free_context 进行释放。
◼ avformat_open_input();打开输入视频文件
    
◼ avformat_find_stream_info()：获取音视频文件信息
◼ av_read_frame(); 读取音视频包
◼ avformat_seek_file(); 定位文件
◼ av_seek_frame():定位文件
```





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

