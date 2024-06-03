

# ffmpeg 组件

- ffmpeg 超快音视频编码器
- ffplay 简单多媒体
- ffprobe 简单多媒体流 

# ffmpeg 命令用法

```shell
ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...
```

```shell
ffplay [options] input_file
```

```shell
ffprobe [OPTIONS] INPUT_FILE
```



## ffmpeg 命令分类查询

```shell
-version # 显示版本
-buildconf # 显示编译配置

-bsfs # 显示可用比特流filter
-protocols # 显示可用的协议
-filters # 显示可用的过滤器
-pix_fmts # 显示可用的像素格式
-layouts # 显示标准声道名称
-sample_fmts # 显示可用的音频采样格式
-colors # 显示可用的颜色名称

-formats # 显示可用格式(muxers+demuxers)
-muxers # 显示可用复用器
-demuxers # 显示可用解复用器

-codecs # 显示可用编解码器(decoders+encoders)
-decoders # 显示可用解码器
-encoders # 显示可用编码器
```



```shell
ffmepg -h type=name # ffmepg -h type=name

# eg
ffmpeg -h muxer=flv
ffmpeg-h filter=atempo(atempo调整音频播放速率)
ffmpeg-h encoder=libx264
```



# ffmpeg 参数

## 主要参数

- ``-h`` 查看帮助

  ```shell
  ffmpeg -h 		# 基本信息
  ffmpeg -h long 	# 高级信息
  ffmpeg -h full 	# 所有信息，信息过多，可以通过 ffmpeg -h full > ffmpe_h_full.log导出到文件查看
  ```

- ``-i`` 设定输入流
- ``-f`` 设定输出格式（format）
- ``-ss`` 开始时间
- ``-t`` 时间长度



## 音频参数

- ``aframes ``设置要输出的音频帧数
- ``-b:a ``音频码率
- ``-ar``  设定采样率
- `-ac` 设定声音的channel数
- `-acodec` 设定声音编解码器
  - `copy` 表示原始表姐吗数据必须被拷贝
- `-an`不处理音频
- `-af` 音频过滤器

```shell
ffmpeg -i test.mp4 -b:a 192k -ar 48000 -ac 2 -acodec libmp3lame -aframes 200 out2.mp3
```



## 视频参数

- `-vframes` 设置要输出的视频帧数
- `-b` 设定视频码率
- `-b:v` 视频码率
- `-r`  设定帧率
- `-s` 设定画面的宽高
- `-vn` 不处理视频
- `-aspect` 设置纵横比
  - `4:3 / 16:9`
  - `1.3333 / 1.7777`
- `-vcodec` 设定视频编解码器
  - `copy` 表示原始编解码数据必须被拷贝
- `-vf` 视频过滤器

```shell
ffmpeg -i test.mp4 -vframes 300 -b:v 300k -r 30 -s 640x480 -aspect 16:9 -vcodec libx265
```



# ffmpeg 常用功能

## 提取视频

```shell
ffmpeg -i test.mp4 -vcodec copy -an test_copy.h264 # 保留编码格式
ffmpeg -i test.mp4 -vcodec copy -an video.mp4  # 保留封装格式
ffmpeg -i test.mp4 -vcodec libx264 -an test.h264 # 强制格式

# -acodec == -codec:a
# -vcodec == -codec:v
```



## 提取音频

```shell
ffmpeg -i test.mp4 -acodec copy -vn test.aac # 保留编码格式
ffmpeg -i test.mp4 -acodec copy -vn audio.mp4 # 保留封装格式
ffmpeg -i test.mp4 -acodec libmp3lame -vn test.mp3 # 强制编码格式

ffmpeg -i xxx.mp4 -vn -c:a libfdk_aac -ar 44100 -channels 2 -profile:a aac_he_v2 audio_name.aac # 重新采样编码
```



## 提取像素格式

```shell
ffmpeg -i test.mp4 -t 3 -pix_fmt yuv420p out.yuv # 提取YUV 3秒数据，分辨率和源视频一致
ffmpeg -i test.mp4 -t 3 -pix_fmt yuv420p -s 320x240 out.yuv # 提取YUV 3秒数据， 分辨率转为320x240
```

```shell
ffmpeg -i test.mp4 -t 3 -pix_fmt rgb24 -s 320x240 out.rgb # 提取RGB 3秒数据，分辨率转为320x240
```

```shell
ffmpeg -s 320x240 -t 3 -pix_fmt yuv420p -i input.yuv -pix_fmt rgv24 out.rgb # YUV -> RGB
```



## 提取音频格式

```shell
ffmpeg -i test.mp3 -ar 48000 -ac 2 -f s16le out.pcm # 可以省略后缀 .pcm
ffmpeg -i test.mp4 -ar 48000 -ac 2 -f f32le out.pcm # 可以省略后缀 .pcm
ffmpeg -i test.mp3 -ar 48000 -ac 2 -smaple_fmt s16 out.wav
ffmpeg -i test.mp3 -ar 48000 -ac 2 -codec:a pcm_s16le out.wav

# wav是在pcm音频裸数据前面打上WAV头部
```



## 转封装

```shell
# 转封装保持编码格式
ffmpeg -i test.mp4 -vcodec copy -acodec copy out.ts
ffmpeg -i test.mp4 -codec copy out.ts # -codec == -vcodec + -acodec

# 转封装改变编码格式
ffmeg -i test.mp4 -vcodec libx265 -acodec libmp3lame out.mkv

# 转封装修改帧率
ffmpeg -i test.mp4 -r 15 -codec copy out.mp4 # error, 帧率不会改变
ffmepg -i test.mp4 -r 15 out.mp4

# 转封装修改视频码率
ffmpeg -i test.mp4 -b 400k out.mkv 
ffmpeg -i test.mp4 -b:v 400k out.flv

# 转封装修改音频码率
ffmpeg -i test.mp4 -b:a 192k out.mp4 # 不重新编码

# 转封装修改音视频码率
ffmpeg -i test.mp4 -b:v 400k -b:a 192k out.mp4 

# 转封装修改音频采样率
ffmpeg -i test.mp4 -ar 44100 out.mp4
```



## 拼接视频

​		准备好待拼接的文件片段，音视频封装格式编码格式需要统一，不统一则需要转码转封装。

```shell
ffmpeg-i 沙海02.mp4 -ss 00:05:00 -t 10 -codec copy 1.mp4
ffmpeg -i 复仇者联盟3.mp4 -ss 00:05:00 -t 10 -codec copy 2.mp4
ffmpeg-i 红海行动.mp4 -ss 00:05:00 -t 10 -codec copy 3.mp4
# 如果音视频格式不统一则强制统一为-vcodeclibx264-acodecaac

ffmpeg-i 1.mp4 -codec copy -vbsf h264_mp4toannexb 1.ts
ffmpeg-i 2.mp4 -codec copy -vbsf h264_mp4toannexb 2.ts
ffmpeg-i 3.mp4 -codec copy -vbsf h264_mp4toannexb 3.ts
# 分离某些封装格式（例如MP4/FLV/MKV等）中的H.264的时候，需要首先写入SPS和PPS，否则会导致分离出来的数据没有SPS、PPS而无法播放。H.264码流的SPS和PPS信息存储在AVCodecContext结构体的extradata中。需要使用ffmpeg中名称为“h264_mp4toannexb”的bitstream filter处理

ffmpeg-i 1.mp4 -codec copy 1.flv
ffmpeg-i 2.mp4 -codec copy 2.flv
ffmpeg-i 3.mp4 -codec copy 3.flv
```

​		拼接文件

- 视频分辨率可以不同，但是编码格式需要统一，封装格式也建议统一为TS输入流，输出时转为目标封装格式
- 音频编码格式需要统一，音频参数（采样率，声道数，位深等）也需要统一

```shell
# 1
ffmpeg-i "concat:1.mp4|2.mp4|3.mp4" -codec copy out_mp4.mp4
ffmpeg-i"concat:1.ts|2.ts|3.ts" -codec copy out_ts.mp4 
ffmpeg-i "concat:1.flv|2.flv|3.flv" -codec copy out_flv.mp4

# 2
ffmpeg-f concat-i mp4list.txt -codec copy out_mp42.mp4	
ffmpeg-f concat-i tslist.txt -codec copy out_ts2.mp4
ffmpeg-f concat-i flvlist.txt -codec copy out_flv2.mp4

# 方法1只适用部分封装格式，比如TS流
# 建议使用方法2进行拼接，或者转为ts流再进行拼接
```



## 截图

```shell
ffmpeg -i test.mp4 -y -f image2 -ss 00:00:02 -vframes 1 -s 640x360 test.jpg
ffmpeg -i test.mp4 -y -f image2 -ss 00:00:02 -vframes 1 -s 640x360 test.bmp

# -y 确认覆盖
# -f 图片格式 image2
# -ss 起始时间
# -vframes 截取帧数
# -s 分辨率大小
```

## 视频图片集转换

```shell
ffmpeg -i test.mp4 -t 5 -s 640x360 -r 15 frame%03d.jpg # -r 指定gop提取中帧数，%03d为提取的图片集命名规则
ffmpeg -f image2 -i frame%03d.jpg -r 25 video.mp4 # -r 指定生成的视频gop帧率
```



## 视频GIF转换

```shell
ffmpeg -i test.mp4 -t 5 -r 1 image1.gif
ffmpeg -i test.mp4 -t 5 -r 25 -s 640x360 image2.gif

ffmpeg -f gif -i image2.gif image2.mp4
```



# ffplay 播放控制

```shell
q, ESC # 退出播放

c # 循环切换节目
a # 循环切换音频流
v # 循环切换视频流
t # 循环切换字幕流
s # 逐帧播放
w # 循环切换过滤器或显示模式

f # 全屏切换
鼠标左键双击 # 全屏切换

p, SPC # 暂停
m # 静音切换
9, 0 # 9减少音量，0增加音量
/, * # /减少音量，*增加音量
left/right # 向后/向前拖动10秒
down/up # 向后/向前拖动1分钟

鼠标右键单击 # 拖动与显示宽度对应百分比的文件进行播放
```



# ffplay 参数

> [ffplay Documentation (ffmpeg.org)](https://www.ffmpeg.org/ffplay.html)

```shell
-x width # 强制显示宽带。
-y height # 强制显示高度。
-video_size size # 帧尺寸设置显示帧存储(WxH格式)，仅适用于类似原始YUV等没有包含帧大小(WxH)的视频。
	# 比如: ffplay -pixel_format yuv420p -video_size 320x240 -framerate 5  yuv420p_320x240.yuv
-pixel_format format # 格式设置像素格式。
-fs # 以全屏模式启动。
-an # 禁用音频（不播放声音）
-vn # 禁用视频（不播放视频）
-sn # 禁用字幕（不显示字幕）
-ss pos # 根据设置的秒进行定位拖动，注意时间单位：比如'55' 55 seconds, '12:03:45' ,12 hours, 03 minutes and 45 seconds, '23.189' 23.189 second
-t duration # 设置播放视频/音频长度，时间单位如-ss选项


-bytes # 按字节进行定位拖动（0=off 1=on -1=auto）。
-seek_interval # interval 自定义左/右键定位拖动间隔（以秒为单位），默认值为10秒（代码没有看到实现）
-nodisp # 关闭图形化显示窗口，视频将不显示
-noborder # 无边框窗口
-volume vol # 设置起始音量。音量范围[0 ~100]
-f fmt # 强制使用设置的格式进行解析。比如-f s16le
-window_title # title 设置窗口标题（默认为输入文件名）
-loop number # 设置播放循环次数
-showmodemode # 设置显示模式，可用的模式值：0 显示视频，
	# 1 显示音频波形，2 显示音频频谱。
	# 缺省为0，如果视频不存在则自动选择2
-vf filtergraph # 设置视频滤镜
-af filtergraph # 设置音频滤镜


-stats # 打印多个回放统计信息，包括显示流持续时间，编解码器参数，流中的当前位置，以及音频/视频同步差值。默认情况下处于启用状态，要显式禁用它则需要指定-nostats。。
-fast # 非标准化规范的多媒体兼容优化。
-genpts # 生成pts。
-sync type  # 同步类型将主时钟设置为audio（type=audio），video（type=video）或external（type=ext），默认是audio为主时钟。
-ast audio_stream_specifier # 指定音频流索引，比如-ast3，播放流索引为3的音频流
-vst video_stream_specifier # 指定视频流索引，比如-vst4，播放流索引为4的视频流
-sst subtitle_stream_specifier # 指定字幕流索引，比如-sst5，播放流索引为5的字幕流
-autoexit 视频播放完毕后退出。


-exitonkeydown #键盘按下任何键退出播放
-exitonmousedown #鼠标按下任何键退出播放
-codec:media_specifier codec_name # 强制使用设置的多媒体解码器，media_specifier可用值为a（音频），v（视频）和s字幕。比如codec:v h264_qsv 强制视频采用h264_qsv解码
-acodeccodec_name # 强制使用设置的音频解码器进行音频解码
-vcodeccodec_name # 强制使用设置的视频解码器进行视频解码
-scodec codec_name # 强制使用设置的字幕解码器进行字幕解码
-autorotate # 根据文件元数据自动旋转视频。值为0或1 ，默认为1。
-framedrop # 如果视频不同步则丢弃视频帧。当主时钟非视频时钟时默认开启。若需禁用则使用-noframedrop
 -infbuf # 不限制输入缓冲区大小。尽可能快地从输入中读取尽可能多的数据。播放实时流时默认启用，如果未及时读取数据，则可能会丢弃数据。此选项将不限制缓冲区的大小。若需禁用则使用-noinfbu
```



# ffplay 常用功能

## 播放本地文件 

```shell 
ffplay -window_title "test time" -ss 2 -t 10 -autoexit test.mp4 ◼
ffplay buweishui.mp3 
```

## 播放网络流

```shell
ffplay -window_title "rtmp stream" rtmp://202.69.69.180:443/webcast/bshdlive-pc
```



## 指定解码器

```shell
ffplay  -vcodec mpeg4 test.mp4 # mpeg4解码器
ffplay -vcodec h264 test.mp4  # h264解码器
```



## 禁用音频/视频

```shell
ffplay test.mp4 -an # 禁用音频
ffplay test.mp4 -vn # 禁用视频
```



## 播放YUV视频

```shell
ffplay -pixel_format yuv420p -video_size 320x240 -framerate 5 yuv420p_test.yuv
```



## 播放RGB数据

```shell
ffplay -plxel_format rgb24 -video_size 320x240 -i rgb24_test.rgb
ffplay -pixel_foramt rgb24 -video_size 320x240 -framerate 5 -i rgb24_test.rgb
```



## 播放PCM数据

```shell
ffplay -ar 48000 -ac -f f32le 48000_2_f32.pcm
# -ar   set audio sampling rate (in Hz) (from 0 to INT_MAX) (default 0)
# -ac   set number of audio channels (from 0 to INT_MAX) (defau
```



# ffplay 过滤器

> [FFmpeg Filters Documentation](https://www.ffmpeg.org/ffmpeg-filters.html)

## 视频旋转

```shell
ffplay -i test.mp4 -vf transpose=1
```

## 视频反转

```shell
ffplay test.mp4 -vf hflip
 ffplay test.mp4 -vf vflip
 
 ffplay test.mp4 -vf hflip,transpose=1
```



## 音频变速

```shell
ffplay -i test.mp4 -af atempo=2
```



## 视频变速

```shell
ffplay -i test.mp4 -vf setpts=PTS/2

ffplay -i test.mp4 -vf setpts=PTS/2 -af atempo=2
```

