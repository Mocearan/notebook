# ffmpeg-cmd



---



## cmd 结构

```shell
ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...
```





## 参数

### 通用参数

#### `-h`查看帮助

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

  





#### ``-i`` 设定输入流

#### ``-f`` 设定输出格式（format）

#### ``-ss`` 开始时间

#### ``-t`` 时间长度



### 音频参数

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



