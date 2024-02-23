# config in vs

​		在vs上配置ffmpeg库除了一般的库配置之外，需要以`c`的方式添加头文件。

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

​		同时在链接配置的命令行中添加`/SAFESEH:NO `.



# ffmpeg 采集音频流程

- 打开输入设备

  ```c++
  extern "C"
  {
  #include <libavdevice/avdevice.h>  
  };
  ```

  - 注册设备

    ```c++
    avdevice_re 
    ```

    

  - 设置采集方式

    - avfoundation  (mac)
    - dshow  (windows)
    - alsa  (linux)

  - 打开音频设备

- 读取数据包

- 数据输出文件