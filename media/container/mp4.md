# mp4

​		MP4（MPEG-4 Part 14）是一种标准的数字多媒体容器格式，其扩展名为.mp4，以存储数字音频及数字视频为主，也可存储字幕和静止图像。

---

​		虽然被官方标准定义的唯一扩展名是.mp4，但第三方通常会使用各种扩展名来指示文件的内容：

- 同时拥有音频视频的MPEG-4文件通常使用标准扩展名.mp4；
- 仅有音频的MPEG-4文件会使用.m4a扩展名。

​		大部分数据可以通过专用数据流嵌入到MP4文件中，因此MP4文件中包含了一个单独的用于存储流信息的轨道。目前得到广泛支持的编解码器或数据流格式有:

- 视频格式：H.264/AVC、H.265/HEVC、VP8/9等。
- 音频格式：AAC、MP3、Opus等。

​		因其可容纳支持比特流的视频流（如高级视频编码），MP4可以在网络传输时使用流式传输。

>在线解析 MP4 工具：
>MP4box.js：https://gpac.github.io/mp4box.js/test/filereader.html
>mp4parser：https://www.onlinemp4parser.com/



## 概念与术语

​		`MP4文件 -> Track ->Track -> Chunk -> Sample`

- `Box`：MP4文件是由一个个Box组成的

  - `Header`+`Data`
  - `Data `可以存储媒体元数据和实际的音视频码流数据。
  - `Box`可直接存储数据块；也可包含其它`Box`，称为container box。

- `Sample`：采样

  - 一帧视频，一帧音频（一段固定时间的音频数据）
  - 可以由多个`Sample`数据组成，存储媒体数据的单位是`sample`。

- `Chunk`：连续几个`sample`组成的单元被称为`chunk`

  - 每个`chunk`在文件中有一个偏移量，整个偏移量从文件头算起
  - `chunk`内，`sample`是连续存储的。

  > 例如视频流的一个`Sample`代表实际的`nal`数据，`Chunk`是数据存储的基本单位，它是一系列`Sample`数据的集合。

- `Track`：一些`chunk`的集合

  - 对于媒体数据而言就是一个视频序列或者音频序列，常说的音频/视频轨可对照该概念上。
  - 除了`Video Track`和`Audio Track外`，还可以有非媒体数据，比如`Hint Track`
    - 这种类型的Track就不包含媒体数据
    - 可以包含一些将其他数据打包成媒体数据的指示信息或者字幕信息。
  - 简单来说，`Track`是音视频中可以独立操作的媒体单位。

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/a6c9dc02e057320113277b4d3f7683a7.png)



## 整体结构

