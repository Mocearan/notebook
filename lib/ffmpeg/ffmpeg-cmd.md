# ffmpeg-cmd



---



## cmd 结构

```shell
ffmpeg [options] {[infile options] -i infile} ... {[outfile options] outfile} ...
```





## 参数

### `-h`查看帮助

```shell
ffmpeg -h 		# 基本信息
ffmpeg -h long 	# 高级信息
ffmpeg -h full 	# 所有信息，信息过多，可以通过 ffmpeg -h full > ffmpe_h_full.log导出到文件查看
```



- 查看某类组件帮助

  ```shell
  ffmepg -h type=name # ffmepg -h type=name
  
  # eg
  ffmpeg -h muxer=flv
  ffmpeg -h filter=atempo(atempo调整音频播放速率)
  ffmpeg -h encoder=libx264
  ```

  

### 通用参数

- ``-i`` 设定输入流
- ``-f`` 设定输出格式（format）
- ``-ss`` 开始时间
- ``-t`` 时间长度
- `-y`： 输出时覆盖输出目录已存在的同名文件
- `-target type` 设置目标文件类型(vcd,svcd,dvd)

  - `所有的格式选项（比特率，编解码以及缓冲区大小）自动设置 ，只需要输入如下的就可以了

    ```shell
    ffmpeg -i myfile.avi -target vcd /tmp/vcd.mpg
    ```

- `-hq` 高质量设置

- `-itsoffset offset ` 设置以秒为基准的时间偏移，该选项影响所有后面的输入文件

  - 该偏移被加到输入文件的时戳
  - 定义一个正偏移意味着相应的流被延迟了 offset秒
  - `[-]hh:mm:ss[.xxx]`的格式也支持




- `-title string` 设置标题
- `-author string `设置作者
- `-copyright string `设置版权
- `-comment string `设置评论



### 查询参数

- `-L` ：license
- `-version`： 显示版本
- `-buildconf`： 显示编译配置
- `-bsfs` ：显示可用比特流filter
- `-protocols` ： 显示可用的协议
- `-filters` ： 显示可用的过滤器
- `-pix_fmts` ： 显示可用的像素格式
- `-layouts` ： 显示标准声道名称
- `-sample_fmts` ： 显示可用的音频采样格式
- `-colors` ： 显示可用的颜色名称
- `-formats` ： 显示可用格式(muxers+demuxers)
- `-muxers` ： 显示可用复用器
- `-demuxers` ： 显示可用解复用器
- `-codecs` ： 显示可用编解码器(decoders+encoders)
- `-decoders` ： 显示可用解码器
- `-encoders` ： 显示可用编码器



### 音频参数

- ``aframes ``设置要输出的音频帧数
- ``-b:a ``音频码率
- ``-ar``  设定采样率
- `-ac` 设定声音的channel数
- `-an`不处理音频
- `-af` 音频过滤器
- `-acodec` 设定声音编解码器
  - 未设定时则使用与输入流相同的编解码器
  - `copy` 表示原始编码数据必须被拷贝
  
- `-c:a`：输出视频格式



```shell
ffmpeg -i test.mp4 -b:a 192k -ar 48000 -ac 2 -acodec libmp3lame -aframes 200 out2.mp3
```



### 视频参数

- `-vframes` 设置要输出的视频帧数
- `-bf` B帧数量控制
- `-g` 关键帧间隔控制
  - 视频跳转需要关键帧

- `-b` 设定视频码率（`Kbit/s`）
  - 缺省`200kb/s`
  - `-b:v` 视频码率

- `-bt tolerance`  设置视频码率容忍度`kbit/s`
  - `-maxrate bitrate`设置最大视频码率容忍度
  - `-minrate bitreate `设置最小视频码率容忍度

- `-bufsize size` 设置码率控制缓冲区大小
- `-r`  设定帧率（图像频率）
  - 用于视频截图
  - 缺省25
- `-s` 设定画面的宽高（分辨率）
  -  格式为`wXh `
  - 缺省160X128.
  - 下面的简写也可以直接使用：
    - `Sqcif `：128X96 
    - `qcif `：176X144 
    - `cif `：252X288 
    - `4cif `：704X576

- `-vn` 不处理视频
- `-aspect` 设置画幅纵横比
  - `4:3 / 16:9`
  - `1.3333 / 1.7777`
- `-croptop size` 设置顶部切除带大小 像素单位
  - `-cropbottom size `
  - `–cropleft size `
  - `–cropright size`

- `-padtop size` 设置顶部补齐的大小 像素单位
  - `-padbottom size `
  - `–padleft size `
  - `–padright size `
  - `–padcolor color ` 设置补齐条颜色(hex,6个16进制的数，红:绿:兰排列，比如 000000代表黑色)

- `-vcodec` 设定视频编解码器
  - 未设定时则使用与输入流相同的编解码器
  - `copy` 表示原始编解码数据必须被拷贝

- `-vf` 视频过滤器
- `-c:v`：输出视频格式
- `-sameq` 使用同样视频质量作为源（VBR）
- `-pass n` 选择处理遍数（1或者2）
  - 两遍编码非常有用
    - 第一遍生成统计信息
    - 第二遍生成精确的请求的码率

  - `-passlogfile file` 选择两遍的纪录文件名为file


```shell
ffmpeg -i test.mp4 -vframes 300 -b:v 300k -r 30 -s 640x480 -aspect 16:9 -vcodec libx265
```





## in use

- 提取视频

  ```shell
  ffmpeg -i test.mp4 -vcodec copy -an test_copy.h264 # 保留编码格式
  ffmpeg -i test.mp4 -vcodec copy -an video.mp4  # 保留封装格式
  ffmpeg -i test.mp4 -vcodec libx264 -an test.h264 # 强制格式
  
  # -acodec == -codec:a
  # -vcodec == -codec:v
  ```

- 提取音频

  ```shell
  ffmpeg -i test.mp4 -acodec copy -vn test.aac # 保留编码格式
  ffmpeg -i test.mp4 -acodec copy -vn audio.mp4 # 保留封装格式
  ffmpeg -i test.mp4 -acodec libmp3lame -vn test.mp3 # 强制编码格式
  
  ffmpeg -i xxx.mp4 -vn -c:a libfdk_aac -ar 44100 -channels 2 -profile:a aac_he_v2 audio_name.aac # 重新采样编码
  ```

- 提取像素格式

  ```shell
  ffmpeg -i test.mp4 -t 3 -pix_fmt yuv420p out.yuv # 提取YUV 3秒数据，分辨率和源视频一致
  ffmpeg -i test.mp4 -t 3 -pix_fmt yuv420p -s 320x240 out.yuv # 提取YUV 3秒数据， 分辨率转为320x240
  
  ffmpeg -i test.mp4 -t 3 -pix_fmt rgb24 -s 320x240 out.rgb # 提取RGB 3秒数据，分辨率转为320x240
  
  ffmpeg -s 320x240 -t 3 -pix_fmt yuv420p -i input.yuv -pix_fmt rgv24 out.rgb # YUV -> RGB
  ```

- 提取音频格式

  ```shell
  ffmpeg -i test.mp3 -ar 48000 -ac 2 -f s16le out.pcm # 可以省略后缀 .pcm
  ffmpeg -i test.mp4 -ar 48000 -ac 2 -f f32le out.pcm # 可以省略后缀 .pcm
  ffmpeg -i test.mp3 -ar 48000 -ac 2 -smaple_fmt s16 out.wav
  ffmpeg -i test.mp3 -ar 48000 -ac 2 -codec:a pcm_s16le out.wav
  
  # wav是在pcm音频裸数据前面打上WAV头部
  ```

- 转封装

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

- 拼接视频

  - 准备好待拼接的文件片段，音视频封装格式编码格式需要统一，不统一则需要转码转封装。

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
  # 建议使用方法2进行拼接，或者转为ts流再进行拼
  ```

- 截图

  ```shell
  ffmpeg -i test.mp4 -y -f image2 -ss 00:00:02 -vframes 1 -s 640x360 test.jpg
  ffmpeg -i test.mp4 -y -f image2 -ss 00:00:02 -vframes 1 -s 640x360 test.bmp
  
  # -y 确认覆盖
  # -f 图片格式 image2
  # -ss 起始时间
  # -vframes 截取帧数
  # -s 分辨率大小
  ```

- 视频图片集转换

  ```shell
  ffmpeg -i test.mp4 -t 5 -s 640x360 -r 15 frame%03d.jpg # -r 指定gop提取中帧数，%03d为提取的图片集命名规则
  ffmpeg -f image2 -i frame%03d.jpg -r 25 video.mp4 # -r 指定生成的视频gop帧率
  ```

- 视频GIF转换

  ```shell
  ffmpeg -i test.mp4 -t 5 -r 1 image1.gif
  ffmpeg -i test.mp4 -t 5 -r 25 -s 640x360 image2.gif
  
  ffmpeg -f gif -i image2.gif image2.mp4
  ```

  