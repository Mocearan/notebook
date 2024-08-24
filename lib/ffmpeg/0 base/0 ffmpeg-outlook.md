# ffmpeg-outlook

---

​		FFmpeg是一套可以用来记录、转换数字音频、视频，并能将其转化为流的开源计算机程序。采用LGPL或GPL许可证。它提供了录制、转换以及流化音视频的完整解决方案。

> 它包含了非常先进的音频/视频编解码库libavcodec，为了保证高可移植性和编解码质量，libavcodec里很多code都是从头开发的。

​		这个项目最早由Fabrice Bellard发起，2004年至2015年间由Michael Niedermayer主要负责维护。许多FFmpeg的开发人员都来自MPlayer项目，而且当前FFmpeg也是放在MPlayer项目组的服务器上。项目的名称来自MPEG视频编码标准，前面的"FF"代表"Fast Forward"。

​		FFmpeg在Linux平台下开发，但它同样也可以在其它操作系统环境中编译运行，包括Windows、Mac OS X等。



## 参考

`ffmpeg`的官网地址是:https://www.ffmpeg.org/

`ffmpeg`的Github项目地址是:https://github.com/FFmpeg/FFmpeg

[gstream教程-CSDN博客](https://huahuaboy.blog.csdn.net/article/details/122484992)

[FFmpeg教程（超级详细版）-CSDN博客](https://blog.csdn.net/m0_37605642/article/details/121566820?ops_request_misc=%7B%22request%5Fid%22%3A%22172295105516800184128835%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=172295105516800184128835&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-121566820-null-null.142^v100^pc_search_result_base8&utm_term=FFmpeg&spm=1018.2226.3001.4187)

## 项目组成

 构成FFmpeg主要有三个部分，

- 工具软件

  - `ffmpeg.exe`：音视频转码、转换器
  - `ffplay.exe`：简单的音视频播放器
  - `ffserver.exe`：流媒体服务器
  - `ffprobe.exe`：简单的多媒体码流分析器

- SDK library，为各个不同平台编译完成的库。

  > - `Static`版本中的`exe`体积较大,那是因为相关的`Dll`都已经编译进`exe`里面去了。
  > - `Shared`版本中`exe`的体积相对小很多,是因为它们运行的时候还需要到相关的dll中调用相应的功能
  > - `Dev`版本用于开发,里面包含了库文件`xxx.lib`以及头文件`xxx.h`
  >
  > > `Static / Shared`包含了三个`exe`:`ffmpeg.exe`，`ffplay.exe`，`ffprobe.exe`

  - `libavcodec`：包含音视频编码器和解码器
  - `libavutil`：包含多媒体应用常用的简化编程的工具，如随机数生成器、数据结构、数学函数等功能
  - `libavformat`：包含多种多媒体容器格式的封装、解封装工具
  - `libavfilter`：包含多媒体处理常用的滤镜功能
  - `libavdevice`：用于音视频数据采集和渲染等功能的设备相关
  - `libswscale`：用于图像缩放和色彩空间和像素格式转换功能
  - `libswresample`：用于音频重采样和格式转换等功能

- 源代码

  - C语言实现，主要在Linux平台上进行开发。
  - FFmpeg不是一个孤立的工程，它还存在多个依赖的第三方工程来增强它自身的功能。