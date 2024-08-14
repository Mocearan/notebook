# mp4

​		MP4（MPEG-4 Part 14）是一种标准的数字多媒体容器格式，其扩展名为.mp4，以存储数字音频及数字视频为主，也可存储字幕和静止图像。

---

​		虽然被官方标准定义的唯一扩展名是.mp4，但第三方通常会使用各种扩展名来指示文件的内容：

- 同时拥有音频视频的MPEG-4文件通常使用标准扩展名.mp4；
- 仅有音频的MPEG-4文件会使用.m4a扩展名。

​		大部分数据可以通过专用数据流嵌入到MP4文件中，因此MP4文件中包含了一个单独的用于存储流信息的轨道。MP4是一种描述较为全面的容器格式，被认为可以在其中嵌入任何形式的数据，以及各种编码的音视频。目前得到广泛支持的编解码器或数据流格式有:

- 视频格式：H.264/AVC、H.265/HEVC、VP8/9等。
- 音频格式：AAC、MP3、Opus等。

​		因其可容纳支持比特流的视频流（如高级视频编码），MP4可以在网络传输时使用流式传输。

>在线解析 MP4 工具：
>MP4box.js：https://gpac.github.io/mp4box.js/test/filereader.html
>mp4parser：https://www.onlinemp4parser.com/



## 参考

https://b.goeswhere.com/ISO_IEC_14496-12_2015.pdf

[Introduction to QuickTime File Format Specification](https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFPreface/qtffPreface.html#//apple_ref/doc/uid/TP40000939-CH202-TPXREF101)



[MP4格式详解_mp4格式解析-CSDN博客](https://blog.csdn.net/weixin_39399492/article/details/127837222?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~Ctr-1-127837222-blog-137641428.235^v43^pc_blog_bottom_relevance_base7&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~Ctr-1-127837222-blog-137641428.235^v43^pc_blog_bottom_relevance_base7&utm_relevant_index=2)

## 概念与术语



- `Box`：MP4文件是由一个个Box组成的
  - `Header`+`Data`
  - `Data `可以存储媒体元数据和实际的音视频码流数据。
  - `Box`可直接存储数据块；也可包含其它`Box`，称为container box。

​	

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
  - ​	`MP4文件 -> Track ->Track -> Chunk -> Sample`

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/a6c9dc02e057320113277b4d3f7683a7.png)



## 工具

查看MP4文件的信息有很多方式，个人比较推荐的：

1. https://www.onlinemp4parser.com/
2. http://www.bento4.com/
3. SteamAnalyzer



## 结构

​		MP4文件由多个**box**组成，每个box存储不同的信息。box类型有很多，但基本结构都是一样的，都是header和body的组合；header是对box的整体描述也就是元数据，body实际存储的内容跟box类型有关。

​		box之间是树状结构：

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/8871e89471e9b225367b37d27855af2d.png)

​		真正的音视频数据都在mdat这个box中，其它box都是对文件、数据流的描述。

​		MP4中所有的序号都是从1开始：比如track_ID、chunk序号、sample序号都是从1开始

### box

​		box是MP4文件的基本存储结构，不同类型的数据被封装在不同类型的box中，并组成box树。

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/ce833f57afe09a5d23b6ee2bb2fd5d36.png)



一个box由两部分组成：`box header`、`box body`。

- `box header`：box的元数据

  - `box type`
    - 根结构下一般有4个box（`ftyp，free，mdat，moov`）
    - 4 个 ASCII 值，如果是 “uuid”，则表示此 box 为用户自定义拓展类型
  - `box size`
    - `size==1`时，存在`largesize`字段
    - `size==0`，表示该box为文件的最后一个box
  - Box Header 至少占 8 字节。必须包含 `box type`，`box size`
  - 在部分box中，还存在`version`、`flags`字段，这样的box叫做`Full Box`
  - box body中嵌套其他box时，这样的box叫做`container box`

- `box body`：box的数据部分

  - 实际存储的内容跟box类型有关

    > 比如`mdat`中`body`部分存储的媒体数据。



![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/ae481c68e66c918ad4ee60023eb2402e.png)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/ab54c4fa59cd4b9b858d519a8848f7fd.png)

### ftyp box

​		file type box. 一般在文件的开始位置，描述的文件的版本、兼容协议等。

​		ftyp是MP4文件的第一个Box，通过判断该Box来确定文件的类型。该Box有且仅有1个，并且只能被包含在文件层，而不能被其他Box包含。该Box放在文件的最开始，指示文件的相关信息。

- `major_brand`：比如常见的 isom、mp41、mp42、avc1、qt等。它表示“最好”基于哪种格式来解析当前的文件。

  > isom（ISO Base Media file）是在 MPEG-4 Part 12 中定义的一种基础文件格式。MP4 文件可能遵循的规范有mp41、mp42，而mp41、mp42又是基于isom衍生出来的。

- `minor_version`：提供 major_brand 的说明信息，比如版本号，不得用来判断媒体文件是否符合某个标准/规范；

- `compatible_brands`：文件兼容的brand列表。

| 字段                | 类型      | 描述                                                         |
| ------------------- | --------- | ------------------------------------------------------------ |
| major_brand         | 4 bytes   | 主版本号                                                     |
| minor_version       | 4 bytes   | 次版本号                                                     |
| compatible_brands[] | 4×n bytes | 指定兼容的版本，注意此字段是一个 list，可以包含多个版本号，每个版本号占 4 字节 |



### free box

​		free space box。free box(有的也叫skip)，这个可以不用关心，去掉它也不影响解封装。

​		如果存在，则通常出现在moov与mdat之间，即moov-free-mdat。

​		在实时拍摄视频，moov数据增多时，将free分配给moov使用。因为设备录制视频时并不能预先知道视频数据大小，如果moov在mdat之前，随着拍摄mdat的数据会增加，moov数据也会增多，如果没有free预留的空间，则要不停的向后移动mdat数据以腾出moov空间。

### mdat box

​		media data box。 存放具体的媒体数据，可从该Box解封装出真实的媒体数据。数据结构的意义需要参考`metadata`（主要在`sample table`中描述）

​		该Box一般都会存在，但非必须。

- mdat的位置比较灵活，可以位于moov之前，也可以位于moov之后，但必须和stbl中的信息保持一致。

- 在写mp4文件的时候，对于mdat这个Atom，一般是先将Atom size填写0，待数据写完之后，再回过来填入具体大小。

  > mdat 可以引用外部的数据，参见 moov -> udta -> meta，这里不讨论，只讨论数据存储在本文件中的形式。

- mdat 也是一个 box，拥有 box header 和 box body。

  - box body 部分，采用一个一个 samples 的形式进行存储

    - 即一个一个音频帧或视频帧的形式进行存储。

    - 没有同步字，没有分隔符，只能根据索引进行访问。即：

      ```
      Box header + Box Data
      ==>
      Box size + Box type + NALU + ... + NALU
      NALU = NALU length + NALU Header + NALU Data
      ```

    - 码流组织方式采用 `avcc `格式，即 `AUD + slice size + slice` 的形式。

      



### moov box

​		movie box。存储mp4文件的`metadata`信息，`moov`是一个`container box`，它包含`mvhd`、`track`和`udta`这三个子box。

​		`moov `是MP4文件中必须有的一个 Box，但只能存在一个，一般出现在 `ftyp `之后或 MP4 文件末尾。

1. `mvhd（moov header）`：用于简单描述一些所有媒体共享的信息。
2. `trak`：即track，轨道。用于描述音频流或视频流信息，可以有多个轨道，上面的样例中trak出现了 2 次，分别表示一路音频和一路视频流。
3. `udta（user data）`：用户自定义，可忽略。



### mvdh box

​		movie header box。全文件唯一， 记录整个媒体文件的描述信息，如创建时间、修改时间、时间度量标尺、播放时长等。

- `creation_time`：文件创建时间；
- `modification_time`：文件修改时间；
- `timescale`：一秒包含的时间单位（整数）。举个例子，如果timescale等于1000，那么，一秒包含1000个时间单位（比如track的duration为10,000，那么，track的实际时长为10,000/1000=10s）；
- `duration`：影片时长（整数），根据文件中的track的信息推导出来，等于时间最长的track的duration；
- `rate`：推荐的播放速率，32位整数，高16位、低16位分别代表整数部分、小数部分（[16.16]），举例 0x0001 0000 代表1.0，正常播放速度；
- `volume`：播放音量，16位整数，高8位、低8位分别代表整数部分、小数部分（[8.8]），举例 0x01 00 表示 1.0，即最大音量；
- `matrix`：视频的转换矩阵，一般可以忽略不计；
- `next_track_ID`：32位整数，非0，一般可以忽略不计。当要添加一个新的track到这个影片时，可以使用的track id，必须比当前已经使用的track id要大。也就是说，添加新的track时，需要遍历所有track，确认可用的track id；

mvhd 语法继承自 FullBox，注意下述示例出现的 version 和 flags 字段属于 FullBox header。

| 字段              | 类型        | 描述                                |
| ----------------- | ----------- | ----------------------------------- |
| version           | 1 Byte      | 版本，取 0 或 1，一般取 0           |
| flags             | 3 Bytes     | 标识                                |
| creation_time     | 8/16 Bytes  | 创建时间，当 version=0 时取 4 字节  |
| modification_time | 8/16 Bytes  | 修改时间，当 version=0 时取 4 字节  |
| timescale         | 8 Bytes     | 时间基                              |
| duration          | 8/16 Bytes  | 文件时长，当 version=0 时取 4 字节  |
| rate              | 8 Bytes     | 播放速率，默认取 0x00010000，即 1.0 |
| volume            | 4 Bytes     | 音量，默认取 0x0100，即 1.0         |
| reserved          | 4 Bytes     | 0                                   |
| reserved          | 2 x 8 Bytes | 0                                   |
| matrix            | 9 x 8 Bytes | 视频播放矩阵，可忽略                |
| pre_defined       | 6 x 8 Bytes | 0                                   |
| next_track_id     | 8 Bytes     | 下一个紧邻的 track box id           |



![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/2910f95bdd81dd7e294738bb487446ff.png)



### trak box

​	track box。记录媒体流信息，文件中可以存在一个或多个 相互独立的`track`。trak box 用于描述每一路媒体流。一般的包含一个视频`track`和一个音频`track`。

​	`track`是`container box`，至少包含两个子box：`tkhd`和`mdia`。

- `tkhd（track header box）`：用于简单描述该路媒体流的信息，如时长，宽度等。
- `mdia（media box）`：用于详细描述该路媒体流的信息
- `edts（edit Box）`：子Box为elst（Edit List Box），它的作用是使某个track的时间戳产生偏移。

视频：

- `packets`：包含的packets的数量
- `steam type`: 流的类型，比如AVC/H.264 、 AAC
- `profile`: 编码器的profile，如High、Base等
- `level`: 编码器的level
- `width`: 宽
- `height`: 高
- `aspect ratio`: 纵横比， 5:9

音频：

- `sample rate`： 采样率
- `channels`: 声道数



### tkhd box

​		Track Header box。 包含关于媒体流的头信息，用以描述`track`信息！

- `version`：tkhd box的版本；

- `flags`：按位或操作获得，默认值是7（0x000001 | 0x000002 | 0x000004），表示这个track是启用的、用于播放的 且 用于预览的。

  - `Track_enabled`：值为0x000001，表示这个track是启用的，当值为0x000000，表示这个track没有启用；

  - `Track_in_movie`：值为0x000002，表示当前track在播放时会用到；

  - `Track_in_preview`：值为0x000004，表示当前track用于预览模式；

- `creation_time`：当前track的创建时间；

- `modification_time`：当前track的最近修改时间；

- `track_ID`：当前track的唯一标识，不能为0，不能重复；

- `duration`：当前track的完整时长（需要除以[timescale](https://so.csdn.net/so/search?q=timescale&spm=1001.2101.3001.7020)得到具体秒数）；

- `layer`：视频轨道的叠加顺序，数字越小越靠近观看者，比如1比2靠上，0比1靠上；

- `alternate_group`：当前track的分组ID，alternate_group值相同的track在同一个分组里面。同个分组里的track，同一时间只能有一个track处于播放状态。当alternate_group为0时，表示当前track没有跟其他track处于同个分组。一个分组里面，也可以只有一个track；

- `volume`：audio track的音量，介于0.0~1.0之间；

- `matrix`：视频的变换矩阵；

- `width、height`：视频的宽高；

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/04ee544dc7449a42c52bcca8db444522.png)

### edts box

​		Edit Box。不是所有的 MP4文件有这个 Box。它下边有一个elst（Edit List Box），使某个`track`的时间戳产生偏移。

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/02623e77c9d1a3b479460318210b2ab1.png)

- `segment_duration`: 表示该edit段的时长，以Movie Header Box（mvhd）中的timescale为单位,即 segment_duration/timescale = 实际时长（单位s）
- `media_time`: 表示该edit段的起始时间，以track中Media Header Box（mdhd）中的timescale为单位。如果值为-1(FFFFFF)，表示是空edit，一个track中最后一个edit不能为空。
- `media_rate`: edit段的速率为0的话，edit段相当于一个”dwell”，即画面停止。画面会在media_time点上停止segment_duration时间。否则这个值始终为1。

> ​		为使PTS从0开始，`media_time`字段一般设置为第一个CTTS的值，计算PTS和DTS的时候，他们分别都减去`media_time`字段的值就可以将PTS调整为从0开始的值
> ​		如果`media_time`是从一个比较大的值，则表示要求PTS值大于该值时画面才进行显示，这时应该将第一个大于或等于该值的PTS设置为0，其他的PTS和DTS也相应做调整



### mdia box

​		定义了`track`的媒体类型以及`sample`数据，描述`sample`信息。基本就是当前Track媒体头信息和媒体句柄以及媒体信息。它自身非常简单，就是一个标识而已，但最复杂的还是里面包含的子box。

​		它是一个`container box`，它必须包含`mdhd`，`hdlr `和 `minf`。

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/ad08f2d2a45bfb9d8e5255440a14dbfa.png)

### mdhd box

​		Media Header Atom Box。用于简单描述该路媒体流的信息。

> `mdhd `和 `tkhd `内容大致都是一样的。
>
> 不过`tkhd `通常是对指定的 `track `设定相关属性和内容。而 `mdhd `是针对于独立的 `media `来设置的。

- `creation_time`：当前track的创建时间；

- `modification_time`：当前track的最近修改时间；

- `timescale`: 同mvhd中的`timescale`，但是需要注意虽然意义相同，但是值有可能不同，下边`stts`,`ctts`等时间戳的计算都是以`mdhd`中的`timescale`

- `duration`: track的时间长度。

- `language`: 媒体语言码。最高位为0，后面15位为3个字符

​		其中我们最关心的两个字段是`timescale`和`duration`，分别表示了该Track的时间戳和时长信息，这个时间戳信息也是PTS和DTS的单位。

### hdlr box

​		Handler Reference Box。主要解释了媒体的播放过程信息，用来设置不同`Track`的处理方式。声明当前`track`的类型，以及对应的处理器（`handler`）。

`handler_type`的取值包括：

- `vide（0x76 69 64 65）`：video track
- `soun（0x73 6f 75 6e）`：audio track
- `hint（0x68 69 6e 74）`：hint track
- `meta（0x6d 65 74 61）`：Timed Metadata track
- `auxv（0x61 75 78 76）`：Auxiliary Video track



### minf box

​		Media Information box。建立了时间到真实音视频sample的映射关系，是音视频数据操作的关键。

​		解释 `track `媒体数据的 `handler-specific` 信息，`media handler`用这些信息将媒体时间映射到媒体数据并进行处理。`minf `同样是个 `container box`，其内部需要关注的内容是 **`stbl`**，这也是 `moov `中最复杂的部分。

​		一般情况下，`minf`包含一个`header box`，一个“`dinf`”和一个“`stbl`”：

- `header box`根据`track type`（即`media handler type`）分为`vmhd`、`smhd`、`hmhd`和`nmhd`
- `dinf`为`data information box`
- `stbl`为`sample table box`

### *mhd box

​		Media Info Header Box。可分为“`vmhd`”、“`smhd`”、“`hmhd`”和“`nmhd`”，比如视频类型则为`vmhd`，音频类型为`smhd`

- `vmhd`

  - `graphics mode`：视频合成模式，为0时拷贝原始图像，否则与opcolor进行合成。

  - `opcolor`: 一组(red，green，blue)，graphics modes使用。

- `smhd`

  - `balance`：立体声平衡，[8.8] 格式值，一般为0表示中间，-1.0表示全部左声道，1.0表示全部右声道。
- `hmhd`
- `nmhd`



### dinf box

​		Data Information Box。描述了如何定位媒体信息，是一个container box。

​		“dinf”一般包含一个**“dref”(data reference box)**。“dref”下会包含若干个“url”或“urn”，这些box组成一个表，用来定位track数据。

> ​		简单的说，track可以被分成若干段，每一段都可以根据“url”或“urn”指向的地址来获取数据，sample描述中会用这些片段的序号将这些片段组成一个完整的track。一般情况下，当数据被完全包含在文件中时，“url”或“urn”中的定位字符串是空的。



### stbl box

​		Sample Table Box。是整个track中最重要的一个box。MP4文件的媒体数据部分在`mdat `box里，而`stbl`则包含了这些媒体数据的索引以及时间信息。

​		stbl 表在 MP4 文件中的位置：`moov -> trak -> mdia -> minf -> stbl`。

​		在解析 `stbl `前，我们需要了解[chunk](https://so.csdn.net/so/search?q=chunk&spm=1001.2101.3001.7020)和sample的概念：

- `Sample`: video sample即为一帧或者一组连续视频帧，audio sample即为一段连续的音频。
- `Chunk`: 一个视频或者音频`track`中一个或多个sample的组合

​		`stbl `是一个`container box`，其子box包括，x描述了该路媒体流的解码相关信息、音视频位置信息、时间戳信息等：

1. `stsd`：sample description box，样本的描述信息。
2. `stts`：time to sample box，sample解码时间的压缩表。
3. `ctts`：composition time to sample box，sample的CTS与DTS的时间差的压缩表。
4. `stss`：sync sample box，针对视频，关键帧的序号。
5. `stsz`：sample size box，每个sample的字节大小。
6. `stsc`：sample to chunk box，sample-chunk映射表。
7. `stco/co64`：chunk offset box，chunk在文件中的偏移。

​		想要播放一个 MP4 文件，必须根据stbl正确找到每个sample并送给解码器。stbl用来描述每个sample的信息。

### stsd box

​		Sample Description Box。主要存储了编码类型和初始化解码器需要的信息。

- 给出视频、音频的编码、宽高、音量等信息，以及每个sample中包含多少个frame。
- stsd的内容和track的类型有关，也就是和`hdlr`的`handler_type`参数有关。

| 字段                  | 类型     | 描述                                  |
| --------------------- | -------- | ------------------------------------- |
| version               | 8 bit    | 取 0 或 1，一般取 0                   |
| flags                 | 24 bit   |                                       |
| entry_count           | 32 bit   | entry 个数                            |
| 开始循环              |          |                                       |
| AudioSampleEntry()    | 不定大小 | 子 box，当 handler_type=‘soun’ 时才有 |
| VisualSampleEntry()   | 不定大小 | 子 box，当 handler_type=‘vide’ 时才有 |
| HintSampleEntry()     | 不定大小 | 子 box，当 handler_type=‘hint’ 时才有 |
| MetadataSampleEntry() | 不定大小 | 子 box，当 handler_type=‘meta’ 时才有 |
| 结束循环              |          |                                       |

​		这里以视频为例，包含子box：`avc1`，表示是h.264的视频（不同的编码方式，stsd的子box的名称会有不同，但是box中的字段都是相同的）

- `data_reference_index`：利用这个索引可以检索与当前sample description关联的数据。数据引用存储在data reference box。
- `width、height`：像素宽高。
- `horizresolution、vertresolution`：每英寸的像素值(dpi)，[16.16]格式的数据。
- `frame_count`：每个sample中的视频帧数，默认是1。可以是一个sample中有多帧数据。
- `depth`: 位图的深度信息，比如 0x0018（24），表示不带alpha通道的图片；

然后，`avc1 box`下还有一个`avcc`，里边会有 **sps，pps** 等解码必要信息。

| Field                           | Comment                                                      |
| ------------------------------- | ------------------------------------------------------------ |
| type                            | box类型，包括 “预定义类型”、“自定义扩展类型”。预定义类型：比如ftyp、moov、mdat等预定义好的类型；自定义扩展类型：如果type==uuid，则表示是自定义扩展类型。size（或largesize）随后的16字节，为自定义类型的值（extended_type） |
| size                            | 包含box header在内的整个box的大小，单位是字节。当size为0或1时，需要特殊处理 |
| data_reference_index            | 当MP4文件的数据部分，可以被分割成多个片段，每一段对应一个索引，并分别通过URL地址来获取，此时，data_reference_index 指向对应的片段（比较少用到） |
| width、height                   | 视频的宽高，单位是像素                                       |
| horizresolution、vertresolution | 水平、垂直方向的分辨率（像素/英寸），默认是0x00480000（72dpi） |
| frame_count                     | 一个sample中包含多少个frame，对video track来说，默认是1      |
| compressorname                  | 仅供参考的名字，通常用于展示，占32个字节，比如 AVC Coding。第一个字节，表示这个名字实际要占用N个字节的长度。第2到第N+1个字节，存储这个名字。第N+2到32个字节为填充字节。compressorname 可以设置为0 |
| depth                           | 位图的深度信息，比如 0x0018（24），表示不带alpha通道的图片   |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1a4bae198d0028f02711fdd15a51a8b1.png)

​		audio track的stsd body内容（不同的编码方式，stsd的子box的名称会有不同，但是box种的字段都是相同的）：

| Field                | Comment                                                      |
| -------------------- | ------------------------------------------------------------ |
| type                 | box类型，包括 “预定义类型”、“自定义扩展类型”。预定义类型：比如ftyp、moov、mdat等预定义好的类型；自定义扩展类型：如果type==uuid，则表示是自定义扩展类型。size（或largesize）随后的16字节，为自定义类型的值（extended_type） |
| size                 | 包含box header在内的整个box的大小，单位是字节。当size为0或1时，需要特殊处理 |
| data_reference_index | 当MP4文件的数据部分，可以被分割成多个片段，每一段对应一个索引，并分别通过URL地址来获取，此时，data_reference_index 指向对应的片段（比较少用到） |
| channel_count        | 声道数，取值为1或2                                           |
| samplesize           | 采样位宽，一般为8bit或16bit                                  |
| samplerate           | 采样率                                                       |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/d4bc29094841a0e69d3a0bd06939b93c.png)

hint track的stsd body内容：

| Field | Comment  |
| ----- | -------- |
| data  | hint数据 |



### stts box

​		Decoding Time to Sample Box。存储了该 track 每个 sample 到 dts 的时间映射关系。主要用来推导每个帧的时长，描述了sample时序的映射方法，通过它可以找到任何时间的sample。

​		包含了一个压缩版本的表，通过这个表可以从解码时间映射到sample序号。表中的每一项是连续相同的编码时间增量(Decode Delta)的个数和编码时间增量。通过把时间增量累加就可以建立一个完整的time to sample表。

- `entry_count`：表中条目的个数。
- `sample_count`: 连续相同时间长度的sample个数。
- `sample_delta`：以timescale为单位的时间长度。

​		持续时间相同的连续的 Sample 可以放到一个 entry 里面，以达到节省空间的目的。

|              |        |                                                              |
| ------------ | ------ | ------------------------------------------------------------ |
| 字段         | 类型   | 描述                                                         |
| version      | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags        | 24 bit |                                                              |
| entry_count  | 32 bit | 条目个数                                                     |
| 开始循环     |        |                                                              |
| sample_count | 32 bit | 单个entry中，具有相同时长（duration 或 sample_delta）的连续sample的个数 |
| sample_delta | 32 bit | 单个 sample 的播放时长，单位为 timescale，也可以说是相邻两个sample之间dts的差值 |
| 结束循环     |        |                                                              |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/ce7e891d846beea1a46a4ff32650eabd.png)



### ctts box

​		composition time to sample box。提供了`decoding time`到`composition time`的时间差`offset`的表，用于计算`pts`。

​		如果一个视频只有I帧和P帧，则ctts这个表就不需要了，因为解码顺序和显示顺序是一致的，但是如果视频中存在B帧，则需要ctts。这个表在Decoding time和composition time不一样的情况下时必须的。

​		如果box的version等于0，decoding time必须小于等于composition time，因而差值用一个无符号的数字表示。

​	有以下公式：`CT(n)=DT(n)+CTTS(n)`

> cts = pts - dts

​	注：CTTS(n)是未压缩的表的第n个sample对应的offset。

重要字段：

- `sample_count`：连续相同的offset的个数。
- `sample_offset`：CT和DT之间的offset。

| 字段           | 类型   | 描述                                                   |
| -------------- | ------ | ------------------------------------------------------ |
| version        | 8 bit  | 取 0 或 1，一般取 0                                    |
| flags          | 24 bit |                                                        |
| sample_counts  |        | 单个entry中，具有相同差值（pts-dts）的连续sample的个数 |
| sample_offsets |        | 从解码（dts）到渲染（pts）之间的差值                   |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/bee721b47a97b3b9795577decfc289e9.png)



###  stss box

​		Sample Size Box。对视频 track，关键帧所属 sample 的序号。包含sample的数量和每个sample的字节大小，这个box相对来说体积比较大的。

​		关键帧是为了支持随机访问。如果此表不存在，说明每一个 sample 都是一个关键帧。

- `sample_size`：指定默认的sample字节大小，如果所有sample的大小不一样，这个字段为0。
- `sample_count`：track中sample的数量。
- `entry_size`：每个sample的字节大

| 字段          | 类型   | 描述                   |
| ------------- | ------ | ---------------------- |
| version       | 8 bit  | 取 0 或 1，一般取 0    |
| flags         | 24 bit |                        |
| entry_count   | 32 bit | 条目个数               |
| 开始循环      |        |                        |
| sample_number | 32 bit | sample 计数，从 1 开始 |
| 结束循环      |        |                        |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/481e6d43197257e3e8a1d5603c6b18a5.png)



### stsc box

​		Sample To Chunk Box。存储了该 track 中每个 sample 与 chunk 的映射关系。

​		media中的sample被分为组成chunk。chunk可以有不同的大小，chunk内的sample可以有不同的大小。

​		通过stsc中的sample-chunk映射表可以找到包含指定sample的chunk，从而找到这个sample。结构相同的chunk可以聚集在一起形成一个entry，把一组相同结构的chunk放在一起进行管理，是为了压缩文件大小。这个entry就是stsc映射表的表项。

- first_chunk：一组chunk的第一个chunk的序号。
  chunk的编号从1开始。
- samples_per_chunk：每个chunk有多少个sample。
- sample_desc_idx：stsd 中sample desc信息的索引，即stsd 表项序号。

| 字段                     | 类型   | 描述                                                     |
| ------------------------ | ------ | -------------------------------------------------------- |
| version                  | 8 bit  | 取 0 或 1，一般取 0                                      |
| flags                    | 24 bit |                                                          |
| first_chunk              |        | 当前表项中，对应的第一个chunk的序号                      |
| samples_per_chunk        |        | 同一个entry中每个chunk包含的sample数                     |
| sample_description_index |        | chunk使用的stsd的序号，即不同chunk可以使用不同编解码信息 |



![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/c3b7c89c564499226bc7fea5e428ece0.png)

> 第1个entry的第一个chunk的序号是1，第2个entry的第一个chunk的序号是8225（即第1个entry包含索引值为1-8224的chunk），第3个entry的第一个chunk的索引值时8226，以此类推，最后一个entry就是从first_chunk开始一直到结束为止。
>
> 第一个entry每一个chunk包含一个sample，即第1-8224个chunk，每个chunk包含1个sample。第二个entry每一个chunk包含342个sample，即第8225的chunk个chunk包含342个sample。第三个entry每一个chunk包含382个sample，即第8226个chunk包含382个sample，以此类推。
>
> 以上所有chunk中的sample，对应的sample description的序号都是1，也就是用第一个stsd。

### stsz/stz2  box

​		Sample size box。 存储了该 track 中每个 sample 的字节大小。

​		这个box相对来说体积比较大的。表明视频帧或者音频帧大小，FFmpeg 里面的AVPacket 的size 数据大小，就是从这个box中来的。

有两种不同的box类型：stsz、stz2。

| 字段                       | 类型   | 描述                                                         |
| -------------------------- | ------ | ------------------------------------------------------------ |
| version                    | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags                      | 24 bit |                                                              |
| sample_size                |        | 默认的sample大小（单位是byte），通常为0。如果sample_size不为0，那么，所有的sample都是同样的大小。如果sample_size为0，那么，sample的大小可能不一样 |
| sample_count               |        | 当前track里面的sample数目。如果 sample_size==0，那么，sample_count 等于下面entry_size中的数据个数 |
| entry_size（sample_sizes） |        | 单个sample的大小（如果sample_size==0的话）                   |



![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/85a6f285026ca71ed99e9266e3ba72f1.png)



stz2

| 字段         | 类型   | 描述                                                         |
| ------------ | ------ | ------------------------------------------------------------ |
| version      | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags        | 24 bit |                                                              |
| field_size   |        | entry表中，每个entry_size占据的位数（bit），可选的值为4、8、16。4比较特殊，当field_size等于4时，一个字节上包含两个entry，高4位为entry[i]，低4位为entry[i+1] |
| sample_count |        | 等于下面entry_size中的数据个数                               |
| entry_size   |        | 单个sample的大小                                             |



### stco/co64 box

​		Chunk Offset Box。存储了该 track 中每个 chunk 在文件中的偏移。这样就可以直接在文件中找到媒体数据，而不用解析box。

​		针对小文件、大文件，有两种不同的box类型，分别是stco、co64，它们的结构是一样的，只是字段长度不同。

> ​		stco 有两种形式，如果你的视频过大的话，就有可能造成 chunkoffset 超过 32bit 的限制。所以，这里针对大 Video 额外创建了一个 co64 的 Box。它的功效等价于 stco，也是用来表示 sample 在 mdat box 中的位置。只是，里面 chunk_offset 是 64bit 的。

- `chunk_offset`：chunk在文件中的位置。

  >  指的是在文件本身中的 offset，而不是某个box内部的偏移。

​		**需要注意的是一旦前面的box有了任何改变，这张表都要重新建立。**		

​		**需要注意**，这里 stco 只是指定的每个 Chunk 在文件中的偏移位置，并没有给出每个 Sample 在文件中的偏移。想要获得每个 Sample 的偏移位置，需要结合 Sample Size box 和 Sample-To-Chunk 计算后取得。

​		在构建 MP4 文件的时候，需要特别注意 moov 所处的位置，它对于chunk_offset 的值是有影响的。有一些MP4文件的 moov 在文件末尾，为了优化首帧速度，需要将 moov 移到文件前面，此时，需要对 chunk_offset 进行改写。

| 字段         | 类型   | 描述                                                         |
| ------------ | ------ | ------------------------------------------------------------ |
| version      | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags        | 24 bit |                                                              |
| chunk_offset |        | chunk 在文件中的位置（在文件本身中的 offset，而不是某个box内部的偏移） |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3b302e6447f8ae0c9b8f6228bcdc93d7.png)

### udta box

​		User Data Box。用户自定义数据



## 组织方式

​		一个 MP4 文件的典型结构如下：

```
  +-+-+-+-+-+-+-+-+-+-+-+-+-+
  |  ftyp  |  moov |  mdat  |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- 普通MP4，General MP4，box模式为ftyp-mdat-moov。

​		头在后面，FFMPEG默认出的就是这种，写入比较简单。读取时需要seek；所以有些浏览器会需要完全下载后才能开始播放，有些应该可以用HTTP RANGE跳过mdat读取moov。SRS录制MP4文件可以用这种模式。

- 快速MP4，Faststart MP4，box模式为ftyp-moov-mdat。

​		头在前面，FFMEPG需要指定参数，生成MP4后，再过一遍文件才能支持。比上面的对于某些浏览器会比较友好，但是就需要再过一遍文件。对于SRS录制成MP4文件而言，容易造成IO阻塞（等待的时间过长，导致服务线程挂起，具体看ST的实现，不可以长时间的做CPU操作或者磁盘操作）。

> ​		因为 MP4 标准中没有对 moov 和 mdat 的存放位置没有强制先后要求，所以它们的顺序不定。在互联网视频的点播中，如果希望 MP4 文件被快速打开，则需要将 moov 存放在 mdat 的前面；如果moov 放在 mdat 的后面，则需要将 MP4 文件全部下载完才可以播放

- 切片MP4，Fragmented MP4，或FMP4，box模式为ftyp-moov-moof-mdat。

​		分段模式，浏览器H5直接播放时，有些能播，有些不行。这种一般用在DASH中，不会直接用浏览器播放，而是通过JS解析后，给MSE播放（APPEND到Video对象的Buffer中去，具体参考MSE的文章）。FFMPEG也是需要指定参数才能生成。这种对于流媒体比较合适，SRS生成DASH可以用这种。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/fa1f7373789fe4d7ed91ed31e8b36f47.png)

![图片](https://i0.hdslb.com/bfs/article/cf3c8621da31ed79fa76c6a0a8babfe1e166beeb.png@1256w_916h_!web-article-pic.webp)

## 解析过程

​		指解封装、播放的过程，都是先读取box中的内容，然后根据box中内容可以找到每一个sample也就是每一帧数据，然后进行解封装、播放。

详细过程如下：

1. 通过stss确定关键帧的数据和每个关键帧的序号。
2. 通过stts获取音视频流的总帧数、每一帧的dts和整体时长。
3. 通过ctts获取每一帧中pts与dts的差值。
4. 通过stsz获取每个sample的大小及整个流的总大小。
5. 通过stco获取音视频流中chunk的数据和在文件中的位置（不是在mdat中的位置）。
6. 通过stsc计算出流中chunk的数据和在文件中的位置，并通过chunk在文件中的位置及chunk中每个sample的大小，确定每个sample在文件中的位置和大小。
7. 整理每个流的码流信息和时间戳信息，构成整体播放的时间轴，之后进行解码的渲染播放。





## 场景

使用场景：主要是点播场景。

优点：

1. MP4的标准非常灵活，可扩展性比较好，有很多常见的格式是基于MP4做了一些扩展，然后被应用到比较广的范围，比如 CMAF、DASH、HLS。
2. 因为MP4格式的开放性和灵活性，使得使用范围非常广泛，我们日常看电视剧，录视频时，最常见的就是MP4格式了。

缺点：

1. 普通mp4情况下，会导致播放延时较高。
2. 如果moov在文件尾，播放器需要遍历整个文件后才会找到moov并播放。



## 实例

### 如何计算一个sample在文件中的偏移？



**注意：上边提过了这个的timesacle是mdia/mdhd中的timescale，这里等于1000000
另外也先不考虑edts中偏移的影响！**

比如，想要获取时间戳在5.21s的sample在文件中的位置：

1. 那么5.21s在视频时间轴上就是5.21*1000000 = 5210000，**其实说白了就是微秒us**
2. 查表ctts，根据上边的公式，`CT(n)=DT(n)+CTTS(n)`，计算出DT，就是dts，这里是5210000-200000 = 5010000
3. 查表stts，从Decoding Time to Sample表中将**所有sample_count \* sample_delta 累加，要<=5010000**，找到这个最大的sample的num。这里是第150个，再加1，也就是第151个sample
4. 查表stsc，从sample-to-chunk table查找到对应的chunk number。
   ![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/7d58949ab5ef8646bf7dc28f132121c0.png)
   151-5 = 146，所以这里对应的是第147个chunk！
5. 查表stco，从chunk offset 表中查找到对应chunk在文件中的起始偏移量。为2830074
6. 最后查 stsz ，得到第 151 个 Sample 的 size 为 31523，因为sample_per_chunk=1，这个chunk里只有这一个sample，所以该chunk中需要读取的sample在文件中的起始偏移量 = 2830074！如果sample_per_chunk不为1，则需要手动加起来！

当然要解码，还需要找到视频的解码数据如sps、pps。





### 使用FFMpeg将moov前置

为了优化首帧速度，需要将 moov 移到文件前面，此时，需要对 chunk_offset 进行改写。

使用ffmpeg将moov前移很简单，在写文件前设置这个flag即可：

```cpp
AVDictionary *opt = nullptr;
av_dict_set(&opt, "movflags", "faststart", AV_DICT_MULTIKEY);
```





### 如何区分一个track描述的是视频流还是音频流？

方法一：

- 视频：track->tkhd中width、height不为零，volume为零
- 音频：track->tkhd中width、height为零，volume不为零

方法二：

- 视频：track->mdia->hdlr中handler是vide
- 音频：track->mdia->hdlr中handler是soun

方法三：

- 视频：track->mdia->minf中有vmhd box(Video Media Information Header)
- 音频：track->mdia->minf中有smhd box(Sound Media Information Header)



### 为什么mp4要采用这种chunk-sample这种方式进行逻辑上的存储呢？

总的来说是用时间换空间，因为chunk描述的是一组sample，这些sample相同的性质可以通过chunk去统一描述，比如stsc的sample_description_index就是直接对chunk描述，如果没有chunk则需要对每一个sample都描述一遍。但是在解析sample的时候就需要获取chunk的部分信息来帮助解析，没有那么直接，需要临时建一张表。









### hint track是什么？

hint track：这个特殊的track并不包含媒体数据，而是包含了一些将其它数据track打包成流媒体的指示信息。

下面是mp4官方文档对hint track的描述：

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/858afd41c15a5867a556519b1abcbfeb.png)





