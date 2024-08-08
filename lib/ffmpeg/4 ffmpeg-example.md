

# ffmpeg 采集音频流程

- 打开输入设备

  - 注册设备

  - 设置采集方式

    - avfoundation  (mac)
    - dshow  (windows)
    - alsa  (linux)

  - 打开音频设备

- 读取数据包,数据输出文件

- 关闭设备



## 打开输入设备

```c++
extern "C"
{
#include <libavdevice/avdevice.h>  
#include <libavdevice/avdevice.h>
};
```

### 注册设备

```c++
// regist audio device
avdevice_register_all();
```

### 设置采集方式

```c++
// get format
AVInputFormat* format = av_find_input_format("dshow");
```

### 打开音频设备

```c++
AVFormatContext* fmt_ctx = nullptr;
AVDictionary* ops = nullptr;

//使用第一个音频设备
auto device_list = getDeviceNames();
std::string device_name = "audio=" + device_list[0];

char errors[1024];
// url can be net address or local path or media device
auto ret = avformat_open_input(&fmt_ctx, device_name.data(), format, &ops);
if (ret < 0) {
    av_strerror(ret, errors, 1024);
    fprintf(stderr, "Failed to open audio device, [%d]%s\n", ret, errors);
    return;
}
```

​	`ffmpeg`没有提供获取所支持设备列表的API，需要根据平台native的接口来实现。

```c++
#include <windows.h> // import Winmm.lib
#include <vector>
#include <string>

static std::vector<std::string> getDeviceNames()
{
	// windows api 获取音频设备列表（ffmpeg好像没有提供获取音频设备的api）
	unsigned int device_count = waveInGetNumDevs();
	std::vector<std::string> name_list;

	for (unsigned int i = 0; i < device_count; i++) {
		WAVEINCAPS wic;
		waveInGetDevCaps(i, &wic, sizeof(wic));
		//printf("\n音频输入设备：%s\n", wic.szPname);

		//转成utf-8
		int nSize = WideCharToMultiByte(CP_UTF8, 0, wic.szPname,
			static_cast<int>(wcslen(wic.szPname)), nullptr, 0, nullptr, nullptr);
		auto device_name(new char[nSize + 1]);
		memset(device_name, 0, static_cast<size_t>(nSize + 1));
		WideCharToMultiByte(CP_UTF8, 0, wic.szPname, static_cast<int>(wcslen(wic.szPname)),
			device_name, nSize, nullptr, nullptr);
		name_list.push_back(device_name);
		printf("audio input device:%s \n", device_name);

		delete device_name;
	}

	if (name_list.size() <= 0) {
		printf("not find audio input device.\n");
	}

	return name_list;
}
```



## 处理音频数据包

```c++
FILE* audio_file = fopen("./resource/audio_test.pcm", "wb");

AVPacket packet;
int count = 0;
while ((ret = av_read_frame(fmt_ctx, &packet)) == 0 && (count++ < 50)) {
    fwrite(packet.data, packet.size, 1, audio_file);
    fflush(audio_file);

    av_log(nullptr, AV_LOG_INFO, "packet size is %d(%p), count=%d\n", packet.size, packet.data, count);
    av_packet_unref(&packet); // 释放数据包引用
}
```



## 关闭输入设备

```c++
fclose(audio_file);
avformat_close_input(&fmt_ctx);
```

# ffmpeg 音频重采样

- 创建重采样上下文
- 设置重采样参数
- 初始化重采样
- 进行重采样

```c++
extern "C" {
 #include <libswresample/swresample.h>   
}
```

## 创建采样上下文

## 设置重采样参数

```c++
// swr_alloc_set_opts 会根据设置的参数，自动的创建一个采样上下问
SwrContext* swr_ctx = swr_alloc_set_opts(NULL, 
                                         AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_FLT, 44100,
                                         AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100, 
                                         0, NULL);
```



## 初始化重采样

```c++
if ( swr_init(swr_ctx) < 0) {
    printf("swr init fialed\n");
}
```



## 进行重采样

### 创建重采样缓冲区

```c++
int count = 0;
uint8_t** in_data;
int in_linesize;
uint8_t** out_data;
int out_linesize;
// 这里采样数要给大一些，保证分配的缓冲区足够大，否则在进行重采样时，会导致缓冲区溢出造成崩溃。
av_samples_alloc_array_and_samples(&in_data, &in_linesize, 2, 25600, AV_SAMPLE_FMT_S16, 0);
av_samples_alloc_array_and_samples(&out_data, &out_linesize, 2, 25600, AV_SAMPLE_FMT_FLT, 0); 
```

### 重采样

```c++
AVPacket packet;
while ((ret = av_read_frame(fmt_ctx, &packet)) == 0  && (count++ < 50)) {
    memcpy((void*)in_data[0], (void*)packet.data, packet.size);
    //重采样
    swr_convert(swr_ctx,
                out_data, 25600, // 采样数
                (const uint8_t**)in_data, 25600); // 采样数 
    av_packet_unref(&packet);
}

```



### 释放重采样缓冲区

```c++
if (in_data) {
    av_freep(&in_data[0]);
}
av_freep(&in_data);

if (out_data) {
    av_freep(&out_data[0]);
}
av_freep(&out_data);
```

## 释放重采样上下文

```c++
swr_free(&swr_ctx);
```



# ffmpeg 音频编码流程

- 创建编码器
  - 查找编码器
- 创建上下文
- 打开编码器
- 编码器处理数据
  - 数据输入编码器
  - 编码
- 释放资源