# mp4

​		`MP4`（`MPEG-4 Part 14`）是一种标准的数字多媒体容器格式，其扩展名为`.mp4`，以存储数字音频及数字视频为主，也可存储字幕和静止图像。

> ​		虽然被官方标准定义的唯一扩展名是`.mp4`，但第三方通常会使用各种扩展名来指示文件的内容：
>
> - 同时拥有音频视频的`MPEG-4`文件通常使用标准扩展名`.mp4`；
> - 仅有音频的`MPEG-4`文件会使用`.m4a`扩展名。

---

​		MP4 也称为 **MPEG-4 第 14 部分**，是继承 MPEG-4 第 12 部分的 ISO 基础媒体文件格式并略作扩展而来，定义于标准 ISO/IEC 14496-14 中，是一种标准的数字多媒体容器格式。

​		`MP4`是一种描述较为全面的容器格式，被认为可以在其中嵌入任何形式的数据，以及各种编码的音视频。目前得到广泛支持的编解码器或数据流格式有:

- 视频格式：`H.264/AVC`、`H.265/HEVC`、`VP8/9`等。
- 音频格式：`AAC`、`MP3`、`Opus`等。

> 大部分数据可以通过专用数据流嵌入到`MP4`文件中，因此`MP4`文件中包含了一个单独的用于存储流信息的轨道。

​		因其可容纳支持比特流的视频流（如高级视频编码），`MP4`可以在网络传输时使用流式传输。

​		使用场景：主要是点播场景。

优点：

1. `MP4`标准灵活，可扩展性好，很多格式基于`MP4`做了扩展，然后被应用到比较广的范围，比如 `CMAF`、`DASH`、`HLS`
2. `MP4`格式的开放性和灵活性，使得使用范围非常广泛，日常最常见的就是`MP4`格式了

缺点：

1. 普通`mp4`情况下，会导致播放延时较高
2. 如果`moov`在文件尾，播放器需要遍历整个文件后才会找到`moov`并播放



## 参考

https://b.goeswhere.com/ISO_IEC_14496-12_2015.pdf

[Introduction to QuickTime File Format Specification](https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFPreface/qtffPreface.html#//apple_ref/doc/uid/TP40000939-CH202-TPXREF101)

[媒体文件格式分析之MP4 - 懒人李冰 (lazybing.github.io)](https://lazybing.github.io/blog/2016/07/26/mp4-parse-format/)

[MP4格式详解_mp4格式解析-CSDN博客](https://blog.csdn.net/weixin_39399492/article/details/127837222?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~Ctr-1-127837222-blog-137641428.235^v43^pc_blog_bottom_relevance_base7&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~Ctr-1-127837222-blog-137641428.235^v43^pc_blog_bottom_relevance_base7&utm_relevant_index=2)

[     NGINX 添加MP4、FLV视频支持模块       ](https://www.cnblogs.com/lidabo/p/9121637.html)

[     nginx 点播mp4方法       ](https://www.cnblogs.com/lidabo/p/9121659.html)

[     视频支持拖动进度条播放的实现（基于nginx）       ](https://www.cnblogs.com/lidabo/p/9121685.html)



1）ISO/IEC 14496-14

https://www.iso.org/obp/ui/#iso:std:iso-iec:14496:-14:ed-3:v1:en

2）ISO Base Media File Format
https://mpeg.chiariglione.org/standards/mpeg-4/iso-base-media-file-format

## 工具

​		查看`MP4`文件的信息有很多方式，个人比较推荐的：

1. https://www.onlinemp4parser.com/
2. http://www.bento4.com/
3. SteamAnalyzer

>在线解析 MP4 工具：
>MP4box.js：https://gpac.github.io/mp4box.js/test/filereader.html
>mp4parser：https://www.onlinemp4parser.com/





## 格式

### 概念

- `Box`：MP4文件是由一个个Box组成的

  - `Header`+`Data`
  - `Data `可以存储媒体元数据和实际的音视频码流数据。
  - `Box`可直接存储数据块；也可包含其它`Box`，称为`container box`

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
    - 这种类型的`Track`就不包含媒体数据
    - 可以包含一些将其他数据打包成媒体数据的指示信息或者字幕信息。
  - 简单来说，`Track`是音视频中可以独立操作的媒体单位。
  - ​	`MP4文件 -> Track ->Track -> Chunk -> Sample`

<img src="https://i-blog.csdnimg.cn/blog_migrate/a6c9dc02e057320113277b4d3f7683a7.png" alt="在这里插入图片描述" style="zoom: 50%;" />



### 结构

​		`MP4`文件由多个**`Box/FullBox`**组成，每个`box`存储不同的信息（数据），并组成`box`树。

#### box结构

​		`box`类型有很多，但基本结构一致：`header + body`；`FullBox `是 `Box `的扩展，其包含的 `Header `增加了 `version(8bits) `和` flags(24bits) `部分。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/ce833f57afe09a5d23b6ee2bb2fd5d36.png" alt="在这里插入图片描述" style="zoom: 67%;" />

- `header`是对`box`的整体描述，也就是元数据

  - `Header `至少占 8 字节

  - `Header `必须包含` size(32bits) `和 `type(32bits) `

  - `size `用于描述整个 `Box `的长度

    - `size == 0` ，表示这是文件中最后一个 Box
    - `size == 1` ，表示 `Box `长度需要更多 `bits `来描述
      - 这时在后面会定义 `64bits `的 `largesize `来描述 `Box `的长度

  - `type `用于描述 `Box` 的类型

    - 4 个 `ASCII `值
    - 根结构下(`mp4`文件第一层级)一般有4个`box`（`ftyp，free，mdat，moov`）
      - `free`也并不常见
    - `type `是 `uuid `时，代表 `Box `中的数据是用户自定义扩展类型

  - `Full Box`：`Header`存在`version`、`flags`字段

    > `body`中嵌套其他`box`时，叫做`container box`

- `body`实际存储的内容跟`box`类型有关

  > 比如`mdat`中`body`部分存储的媒体数据。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqbianpCFM9bFomyw9N54s0Nj31re3yxvpgmKckicLVWZZQYgQKsHicmwYQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:50%;" />

##### box的数据结构

```c++
aligned(8) class Box (unsigned int(32) boxtype, optional unsigned int(8)[16] extended_type) {
    unsigned int(32) size;
    unsigned int(32) type = boxtype;
    if (size==1) {
        unsigned int(64) largesize;
    } else if (size==0) {
        // box extends to end of file
    }
    if (boxtype==‘uuid’) {
        unsigned int(8)[16] usertype = extended_type;
    }
}
```

FullBox 的数据格式定义：

```c++
aligned(8) class FullBox(unsigned int(32) boxtype, unsigned int(8) v, bit(24) f) extends Box(boxtype) {
    unsigned int(8) version = v;
    bit(24) flags = f;
}
```



#### mp4文件结构

​		`box`之间是树状结构：

> 
>
> <img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/ae481c68e66c918ad4ee60023eb2402e.png" alt="在这里插入图片描述" style="zoom:50%;" />
>
> <img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/8871e89471e9b225367b37d27855af2d.png" alt="在这里插入图片描述" style="zoom: 50%;" />
>
> - 真正的音视频数据都在`mdat`这个`box`中，其它`box`都是对文件、数据流的描述。
> - `MP4`中所有的序号都是从`1`开始：比如`track_ID`、`chunk`序号、`sample`序号都是从1开始



## `box`类型



<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/ab54c4fa59cd4b9b858d519a8848f7fd.png" alt="在这里插入图片描述" style="zoom: 50%;" />

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAq9mfo6ic1lPyjP4cdbkEZr09zc8A1OFoesKnzq4YrKicVOFTicOQWJIfXA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

> 其中 `*` 表示当父 Box 存在时，则必须包含该 Box）

### **`ftyp box`**

​		`file type box`，包含文件的类型、版本、兼容信息等。

> 虽然这个 Box 是 MP4 文件所必须的，但是通常我们并不太关注这里的信息

- 通过判断该`Box`来确定文件的类型
- 在`MP4 `文件中，该 `Box `有且只有一个
- 需要尽可能放在文件最开始的位置
  - 除非有必要的固定长度文件签名信息 `Box `可以放在该 `Box `前面
  - 其他非固定长度的 `Box `数据都必须放在它后面
- 只能被包含在文件层，而不能被其他Box包含。

​		`ftyp Box` 的数据格式定义：

```c++
aligned(8) class FileTypeBox extends Box(‘ftyp’) {
    unsigned int(32) major_brand;
    unsigned int(32) minor_version;
    unsigned int(32) compatible_brands[]; // to end of the box
}
```

| 字段                | 类型      | 描述                                                         |
| ------------------- | --------- | ------------------------------------------------------------ |
| major_brand         | 4 bytes   | 主版本号                                                     |
| minor_version       | 4 bytes   | 次版本号                                                     |
| compatible_brands[] | 4×n bytes | 指定兼容的版本，注意此字段是一个 list，可以包含多个版本号，每个版本号占 4 字节 |

- `major_brand`：它表示“最好”基于哪种格式来解析当前的文件。

  - 比如常见的 `isom`、`mp41`、`mp42`、`avc1`、`qt`等。


  > `isom（ISO Base Media file）`是在 `MPEG-4 Part 12 `中定义的一种基础文件格式。
  >
  > MP4 文件可能遵循的规范有`mp41`、`mp42`，而`mp41`、`mp42`又是基于`isom`衍生出来的。

- `minor_version`：提供 `major_brand `的说明信息

  - 比如版本号，不得用来判断媒体文件是否符合某个标准/规范；

- `compatible_brands`：文件兼容的`brand`列表

> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqXJUSP2rjTQ9SMU4Fmsh5KVrmLVssic4bUgjAvMWNgOhr8w0PYvzL5uA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
>
> - 前 4 个字节 `00 00 00 20` 表示该 Box 的 size，即 32 字节。
> - 4 个字节 `66 74 79 70` 是该 Box 的 type，即 `ftyp`。
> - 接下来 4 个字节 `69 73 6F 6D` 是主 `brand`，表示该文件所遵循的标准规格
>   - 这里是 `isom`，即遵循` ISO Base Media File Format`。
> - 4 个字节 `00 00 02 00` 表示的是这个 `Box `格式的版本号
> - 16 个字节则是兼容的` compatible brands`，即该文件兼容的其他标准规格，这里是 `isom`、`iso2`、`avc1`、`mp41`。

### `free box`

​		free space box。free box(有的也叫skip)，这个可以不用关心，去掉它也不影响解封装。

​		如果存在，则通常出现在moov与mdat之间，即moov-free-mdat。

​		在实时拍摄视频，moov数据增多时，将free分配给moov使用。因为设备录制视频时并不能预先知道视频数据大小，如果moov在mdat之前，随着拍摄mdat的数据会增加，moov数据也会增多，如果没有free预留的空间，则要不停的向后移动mdat数据以腾出moov空间。

### **`mdat box`**

​		`media data box`。 存放具体的媒体数据，可从该Box解封装出真实的媒体数据。这个 Box 中的数据是没有结构的，数据结构的意义需要参考`metadata`（主要在`sample table`中描述）

​		该`Box`一般都会存在，但非必须。

> 因为 MP4 是支持将媒体数据放在其他文件中，并通过 moov 中的信息来索引。

- `mdat`的位置比较灵活，可以位于`moov`之前，也可以位于`moov`之后，但必须和`stbl`中的信息保持一致。

- 在写`mp4`文件的时候，对于`mdat`这个`Atom`，一般是先将`Atom size`填写`0`，待数据写完之后，再回过来填入具体大小。

  > `mdat `可以引用外部的数据，参见` moov -> udta -> meta`，这里不讨论，只讨论数据存储在本文件中的形式。

- `mdat `也是一个 box`，`拥有 `box header 和 box body。`

  - `box body `部分，采用一个一个 `samples` 的形式进行存储

    - 即一个一个音频帧或视频帧的形式进行存储。

    - 没有同步字，没有分隔符，只能根据索引进行访问。即：

      ```
      Box header + Box Data
      ==>
      Box size + Box type + NALU + ... + NALU
      NALU = NALU length + NALU Header + NALU Data
      ```

    - 码流组织方式采用 `avcc `格式，即 `AUD + slice size + slice` 的形式。


```c++
aligned(8) class MediaDataBox extends Box(‘mdat’) {
    bit(8) data[];
}
```





### **`moov box`**

​		`movie box`。包含文件中所有媒体数据的宏观描述信息（`metadata`信息）。

> 实际的音视频数据都存储在 `mdat `中，解析 `moov `中的数据，得到实际音视频数据的索引，确定每一帧数据的位置。
>
> **一般播放器的实现都需要读取到 moov 的数据才能开始播放流程。**

- `moov `在`MP4 `文件中，该 `Box `有且只有一个
  - 一般出现在 `ftyp `之后或 `MP4 `文件末尾。
- `moov`是一个`container box`
  - 包含`mvhd`、`track`和`udta`这三个子`box``
    - `mvhd（moov header）`：用于简单描述一些所有媒体共享的信息。
    - `trak`：即track，轨道。用于描述音频流或视频流信息，可以有多个轨道，上面的样例中trak出现了 2 次，分别表示一路音频和一路视频流。
    - `udta（user data）`：用户自定义，可忽略。
  - `moov `通常包含 1 个 `mvhd `和若干个 `trak`。

```c++
aligned(8) class MovieBox extends Box(‘moov’){
}
```



### mvhd box

​		`moov/mvhd`

​		`movie header box`。 记录整个媒体文件的描述信息，如创建时间、修改时间、时间度量标尺timescale、播放时长duration 等。

> 全文件唯一，因为`mvhd`存在于`moov`中

```c++
aligned(8) class MovieHeaderBox extends FullBox(‘mvhd’, version, 0) { // version：该 Box 的版本号。
    if (version==1) {
        unsigned int(64) creation_time;
        unsigned int(64) modification_time;
        unsigned int(32) timescale;
        unsigned int(64) duration;
    } else { // version==0
        unsigned int(32) creation_time;
        unsigned int(32) modification_time;
        unsigned int(32) timescale;
        unsigned int(32) duration;
    }
    template int(32) rate = 0x00010000; // typically 1.0
    template int(16) volume = 0x0100; // typically, full volume
    const bit(16) reserved = 0;
    const unsigned int(32)[2] reserved = 0;
    template int(32)[9] matrix = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
    // Unity matrix
    bit(32)[6] pre_defined = 0;
    unsigned int(32) next_track_ID;
}
```

​		`mvhd `语法继承自 `FullBox`，注意下述示例出现的 `version `和 `flags `字段属于` FullBox heade`r。

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

- `creation_time`：文件创建时间；
- `modification_time`：文件最新修改时间；
- `timescale`：一秒包含的时间单位（整数）， 1s 的时长被切割的单元数，它体现了时间的精度。

  > 举个例子，如果`timescale`等于`1000`，那么，一秒包含`1000`个时间单位， 每个单元即 `1/1000s`，即 `1ms`
  >
  > 比如`track`的`duration`为`10,000`，那么，`track`的实际时长为`10,000/1000=10s`
- `duration`：影片时长（整数）

  - 表示为多少个`timescale`

    > 如`duration == 10,000， timescale == 1000`，则为`10,000`个值为`1000`的`timescale`，即`10,000/1000=10s`
  - 根据文件中的`track`的信息推导出来，等于时间最长的`track`的`duration`；

- `rate`：播放速率，`32`位整数

  - 高`16`位、低`16`位分别代表整数部分、小数部分（`[16.16`]）

    > 举例 `0x0001 0000` 代表`1.0`，正常播放速度；

- `volume`：播放音量，16位整数

  - 高`8`位、低`8`位分别代表整数部分、小数部分（`[8.8]`）

    > 举例 `0x01 00 `表示 `1.0`，即最大音量；

- `matrix`：视频的图形变换矩阵数据，一般可以忽略不计；

  > 这里的默认值是 `{ 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 }`。
- `next_track_ID`：下一条 track 的 ID，32位整数

  - 值不能为 0

    - 一般，对于一个 `MP4 `文件来说这个值是现有的最大` track ID` 加 `1`

      > 比如，一个 MP4 包含音频和视频两个 track，对应的 ID 分别为 1 和 2，那么 next_track_ID 一般就为 3。

  - 当要添加一个新的`track`到这个影片时，可以使用的`track id`

  - 必须比当前已经使用的`track id`要大

    - 也就是说，添加新的`track`时，需要遍历所有`track`，确认可用的`track id`；

  - 一般可以忽略不计




> <img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/2910f95bdd81dd7e294738bb487446ff.png" alt="在这里插入图片描述" style="zoom: 33%;" />



### trak box

​		`moov/trak`

​		`track box`。记录媒体流信息，文件中可以存在一个或多个 相互独立的`track`。 

​		`trak Box `包含一个单独轨道的信息，用于描述每一路媒体流的空域和时域信息。一般的包含一个视频`track`和一个音频`track`。

​	`track`是`container box`，至少包含两个子box：`tkhd`和`mdia`。

- `tkhd（track header box）`：用于简单描述该路媒体流的信息，如时长，宽度等。
- `mdia（media box）`：用于详细描述该路媒体流的信息
- `edts（edit Box）`：作用是使某个`track`的时间戳产生偏移
  - 子`Box`为`elst`（`Edit List Box`）


> 有两种类型的 trak Box：media track 和 hint track。
>
> - media track 包含媒体轨道的信息，一个文件至少会包含一个 media track 类型的 Box
> - hint track 包含用于流媒体协议的打包信息。

```c++
aligned(8) class TrackBox extends Box(‘trak’) {
}
```



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

​		`moov/trak/tkhd`

​		`Track Header box`。 包含关于媒体流的头信息，用以描述`track`信息。包含该轨道的创建时间、标识该轨道 的 ID、轨道的播放时长、音量、宽高等信息。

```c++
aligned(8) class TrackHeaderBox extends FullBox(‘tkhd’, version, flags){
    if (version==1) {
        unsigned int(64) creation_time;
        unsigned int(64) modification_time;
        unsigned int(32) track_ID;
        const unsigned int(32) reserved = 0;
        unsigned int(64) duration;
    } else { // version==0
        unsigned int(32) creation_time;
        unsigned int(32) modification_time;
        unsigned int(32) track_ID;
        const unsigned int(32) reserved = 0;
        unsigned int(32) duration;
    }
    const unsigned int(32)[2] reserved = 0;
    template int(16) layer = 0;
    template int(16) alternate_group = 0;
    template int(16) volume = {if track_is_audio 0x0100 else 0};
    const unsigned int(16) reserved = 0;
    template int(32)[9] matrix= { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
    // unity matrix
    unsigned int(32) width;
    unsigned int(32) height;
}
```



- `version`：`tkhd box`的版本；
- `flags`：按位或操作获得，默认值是7（`0x000001 | 0x000002 | 0x000004`），表示这个track是启用的、用于播放的 且 用于预览的。
- `Track_enabled`：值为`0x000001`，表示这个`track`是启用的，当值为`0x000000`，表示这个`track`没有启用；
  
- `Track_in_movie`：值为`0x000002`，表示当前`track`在播放时会用到；
  
- `Track_in_preview`：值为`0x000004`，表示当前`track`用于预览模式；
- `creation_time`：当前`track`的创建时间；
- `modification_time`：当前track的最近修改时间；
- `track_ID`：当前`track`的唯一标识，不能为0，不能重复；
- `duration`：当前`track`的完整时长
  - 需要除以`timescale`得到具体秒数

  - 使用的 `timescale `是来自于` Movie Header Box(mvhd)`。

- `layer`：视频轨道的叠加顺序，数字越小越靠近观看者，比如1比2靠上，0比1靠上；
  - 通常这个值默认为 0。

- `alternate_group`：可替代的备份数据组。当前track的分组ID
  - `alternate_group`值相同的`track`在同一个分组里面，互为备份
    - 如果为 0，表示当前 `track `没有备份的 `track `数据；

    - 非 0，则表示可能存在 `group `号相同的 `track `数据作为备份

  - 同个分组里的`track`，同一时间只能有一个`track`处于播放状态
  - 一个分组里面，也可以只有一个`track`；

- `volume`：`audio track`的音量，介于`0.0~1.0`之间；以 `[8.8]` 形式的定点数表示
  - 只对音频轨道有效。
  - 可以多音频轨道组合，并复用 mvhd 的音量；也可以不同的音频轨道设置不同的音量。

- `matrix`：视频的变换矩阵；
  - 这里的默认值是 `{ 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 }`。

- `width、height`：视频的宽高；以 `[16.16]` 形式的定点数表示
  - 这个尺寸不用和图像 `sample` 的尺寸一致，所有图像 `sample `在做矩阵变换前都会缩放处理为该尺寸。




<img src="https://i-blog.csdnimg.cn/blog_migrate/04ee544dc7449a42c52bcca8db444522.png" alt="在这里插入图片描述" style="zoom: 67%;" />

### edts box

​		`moov/trak/edts`

​		`Edit Box`。不是所有的 MP4文件有这个 Box。它下边有一个`elst（Edit List Box）`，使某个`track`的时间戳产生偏移。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/02623e77c9d1a3b479460318210b2ab1.png" alt="在这里插入图片描述" style="zoom:33%;" />

- `segment_duration`: 表示该edit段的时长，以Movie Header Box（mvhd）中的timescale为单位,即 segment_duration/timescale = 实际时长（单位s）
- `media_time`: 表示该edit段的起始时间，以track中Media Header Box（mdhd）中的timescale为单位。如果值为-1(FFFFFF)，表示是空edit，一个track中最后一个edit不能为空。
- `media_rate`: edit段的速率为0的话，edit段相当于一个”dwell”，即画面停止。画面会在media_time点上停止segment_duration时间。否则这个值始终为1。

> ​		为使PTS从0开始，`media_time`字段一般设置为第一个CTTS的值，计算PTS和DTS的时候，他们分别都减去`media_time`字段的值就可以将PTS调整为从0开始的值
> ​		如果`media_time`是从一个比较大的值，则表示要求PTS值大于该值时画面才进行显示，这时应该将第一个大于或等于该值的PTS设置为0，其他的PTS和DTS也相应做调整



### mdia box

​		`moov/trak/mdia`

​		`Media Box`。包含声明当前轨道信息的所有对象。这个 Box 下面包含众多类型的子 Box。

- 定义了`track`的媒体类型以及`sample`数据，描述`sample`信息。
- 基本就是当前`Track`媒体头信息和媒体句柄以及媒体信息。
- 它自身非常简单，就是一个标识，但最复杂的还是里面包含的子`box`。

​		它是一个`container box`，它必须包含`mdhd`，`hdlr `和 `minf`。

```c++
aligned(8) class MediaBox extends Box(‘mdia’) {
}
```





<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/ad08f2d2a45bfb9d8e5255440a14dbfa.png" alt="在这里插入图片描述" style="zoom:33%;" />

### mdhd box

​		`moov/trak/mdia/mdhd`

​		`Media Header Box`。用于简单描述该路媒体流的信息。

> `mdhd `和 `tkhd `内容大致都是一样的。
>
> 不过`tkhd `通常是对指定的 `track `设定相关属性和内容。而 `mdhd `是针对于独立的 `media `来设置的。

```c++
aligned(8) class MediaHeaderBox extends FullBox(‘mdhd’, version, 0) { // version：该 Box 的版本号。这里是 0 或 1。
    if (version==1) {
        unsigned int(64) creation_time;
        unsigned int(64) modification_time;
        unsigned int(32) timescale;
        unsigned int(64) duration;
    } else { // version==0
        unsigned int(32) creation_time;
        unsigned int(32) modification_time;
        unsigned int(32) timescale;
        unsigned int(32) duration;
    }
    bit(1) pad = 0;
    unsigned int(5)[3] language; // ISO-639-2/T language code
    unsigned int(16) pre_defined = 0;
}
```

- `creation_time`：当前`track`的创建时间；

- `modification_time`：当前`track`的最近修改时间；

- `timescale`: 同`mvhd`中的`timescale`，但是需要注意虽然意义相同，但是值有可能不同
  - 下边`stts`,`ctts`等时间戳的计算都是以`mdhd`中的`timescale`

- `duration`: `track`的时间长度，以 timescale 定义的精度计量

- `pad`：占位符。

- `language`: 媒体语言码。该字段总长为 15bit，通常是和 pad 字段组合成为 2 字节的长度。

- `pre_defined`：无作用，默认 0。


> 其中最关心的两个字段是`timescale`和`duration`，分别表示了该`Track`的时间戳和时长信息，这个时间戳信息也是`PTS`和`DTS`的单位。

### hdlr box

​		`moov/trak/mdia/hdlr`

​		`Handler Reference Box`。主要解释了媒体的播放过程信息，用来设置不同`Track`的处理方式。声明当前`track`的类型，以及对应的处理器（`handler`）。

> 这个 `Box `可以作为 `mdia `的子 `Box `，也可以作为 `meta `的子 `Box`。

```c++
aligned(8) class HandlerBox extends FullBox(‘hdlr’, version = 0, 0) {
    unsigned int(32) pre_defined = 0;
    unsigned int(32) handler_type;
    const unsigned int(32)[3] reserved = 0;
    string name;
}
```

- handler_type`的取值包括：

  - `vide（0x76 69 64 65）`：video track

  - `soun（0x73 6f 75 6e）`：audio track

  - `hint（0x68 69 6e 74）`：hint track

  - `meta（0x6d 65 74 61）`：Timed Metadata track

  - `auxv（0x61 75 78 76）`：Auxiliary Video track

- `name`：这个主要是写名字方便人阅读的。

### minf box

​		Media Information box。建立了时间到真实音视频sample的映射关系，是音视频数据操作的关键。

​		解释 `track `媒体数据的 `handler-specific` 信息，`media handler`用这些信息将媒体时间映射到媒体数据并进行处理。`minf `同样是个 `container box`，其内部需要关注的内容是 **`stbl`**，这也是 `moov `中最复杂的部分。

​		一般情况下，`minf`包含一个`header box`，一个“`dinf`”和一个“`stbl`”：

- `header box`根据`track type`（即`media handler type`）分为`vmhd`、`smhd`、`hmhd`和`nmhd`
- `dinf`为`data information box`
- `stbl`为`sample table box`

### mhd box

​		`moov/trak/mdia/minf`

​		`Media Information Box`。，包含了描述该 trak 中媒体数据的所有特征信息。

​		可分为“`vmhd`”、“`smhd`”、“`hmhd`”和“`nmhd`”

- `vmhd`，视频类型
  - `graphics mode`：视频合成模式，为0时拷贝原始图像，否则与opcolor进行合成。
  
  - `opcolor`: 一组(red，green，blue)，graphics modes使用。
  
- `smhd`，音频类型

  - `balance`：立体声平衡，[8.8] 格式值，一般为0表示中间，-1.0表示全部左声道，1.0表示全部右声道。
- `hmhd`
- `nmhd`

​		`minf Box` 是一个` container box`，所以它的数据格式定义是：

```c++
aligned(8) class MediaInformationBox extends Box(‘minf’) {
}
```



### dinf box

​		Data Information Box。描述了如何定位媒体信息，是一个container box。

​		“dinf”一般包含一个**“dref”(data reference box)**。“dref”下会包含若干个“url”或“urn”，这些box组成一个表，用来定位track数据。

> ​		简单的说，track可以被分成若干段，每一段都可以根据“url”或“urn”指向的地址来获取数据，sample描述中会用这些片段的序号将这些片段组成一个完整的track。一般情况下，当数据被完全包含在文件中时，“url”或“urn”中的定位字符串是空的。



### stbl box

​		`moov/trak/mdia/minf/stbl`

​		`Sample Table Box`。是整个track中最重要的一个box。主要包含了时间和媒体采样数据的索引表，使用这部分数据可以按照时间检索出采样数据的位置、类型（是否 I 帧）、大小、实际偏移位置。

​		想要播放一个 `MP4 `文件，必须根据`stbl`正确找到每个`sample`并送给解码器。`stbl`用来描述每个`sample`的信息。

- `Sample`: video sample即为一帧或者一组连续视频帧，audio sample即为一段连续的音频。
- `Chunk`: 一个视频或者音频`track`中一个或多个sample的组合

​		`stbl `是一个`container box`

- 如果当前 `track `不包含数据，那么` stbl Box` 不需要包含任何子 `Box`
- 反之，其子box 描述了该路媒体流的解码相关信息、音视频位置信息、时间戳信息等：
  - `stsd`：sample description box，样本的描述信息。
  - `stts`：time to sample box，sample解码时间的压缩表。
  - `ctts`：composition time to sample box，sample的CTS与DTS的时间差的压缩表。
  - `stss`：sync sample box，针对视频，关键帧的序号。
    - 这个 Box 虽然是可选的，但也比较重要，它是关键帧的序号表，如果没有这个 Box 则表示所有采样都是关键帧。
  - `stsz`：sample size box，每个sample的字节大小。
  - `stsc`：sample to chunk box，sample-chunk映射表。
  - `stco/co64`：chunk offset box，chunk在文件中的偏移。

```c++
aligned(8) class SampleTableBox extends Box(‘stbl’) {
}
```



​		

### stsd box

​		`moov/trak/mdia/minf/stbl/stsd`

​		`Sample Description Box`。主要存储采样数据的细节信息，包括编码类型以及解码需要的各种初始化数据信息。

- 给出视频、音频的编码、宽高、音量等信息，以及每个`sample`中包含多少个`frame`。
- `stsd`的内容和`track`的类型有关，也就是和`hdlr`的`handler_type`参数有关。
  - 需要兼顾` video track、audio track、hint track `各种类型，所以它的数据格式定义比较复杂一些：


```c++
aligned(8) abstract class SampleEntry (unsigned int(32) format) extends Box(format) {
    const unsigned int(8)[6] reserved = 0;
    unsigned int(16) data_reference_index;
} 

class HintSampleEntry() extends SampleEntry (protocol) {
    unsigned int(8) data [];
}

// Visual Sequences
class VisualSampleEntry(codingname) extends SampleEntry (codingname) {
    unsigned int(16) pre_defined = 0;
    const unsigned int(16) reserved = 0;
    unsigned int(32)[3] pre_defined = 0;
    unsigned int(16) width;
    unsigned int(16) height;
    template unsigned int(32) horizresolution = 0x00480000; // 72 dpi
    template unsigned int(32) vertresolution = 0x00480000; // 72 dpi
    const unsigned int(32) reserved = 0;
    template unsigned int(16) frame_count = 1;
    string[32] compressorname;
    template unsigned int(16) depth = 0x0018;
    int(16) pre_defined = -1;
} 

// Audio Sequences
class AudioSampleEntry(codingname) extends SampleEntry (codingname) {
    const unsigned int(32)[2] reserved = 0;
    template unsigned int(16) channelcount = 2;
    template unsigned int(16) samplesize = 16;
    unsigned int(16) pre_defined = 0;
    const unsigned int(16) reserved = 0 ;
    template unsigned int(32) samplerate = {timescale of media}<<16;
} 

aligned(8) class SampleDescriptionBox (unsigned int(32) handler_type) extends FullBox('stsd', 0, 0){
    int i ;
    unsigned int(32) entry_count;
    for (i = 1 ; i <= entry_count ; i++){
        switch (handler_type){
            case ‘soun’: // for audio tracks
                AudioSampleEntry();
                break;
            case ‘vide’: // for video tracks
                VisualSampleEntry();
                break;
            case ‘hint’: // Hint track
                HintSampleEntry();
                break;
        }
    }
}
```

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

- `entry_count`：下面条目表中条目的数量。
- `SampleEntry`：对应的 `sample `条目。
- `data_reference_index`：利用这个索引可以检索与当前 `sample description` 关联的数据。
  - 数据引用存储在 `Data Reference Box(dref)`。
- `ChannelCount`：音频通道数，值为 1(mono，单声道) 或 2(stereo，立体音)。
- `SampleSize`：音频采样大小，默认大小是 `16bits`。
- `SampleRate`：音频采样率，以 `[16.16]` 定点数。
  - 通常是 `44100.0`、`48000.0 `等。
- `resolution`：图像分辨率，以 `[16.16]` 定点数表示。单位是 `pixels-per-inch`。
- `frame_count`：每个 `sample `包含的帧数。
  - 一般是 1 个 sample 对应 1 帧。
- `Compressorname`：一个用于信息展示的名字。
- `depth`：图像位深。比如，`0x0018 `表示不带 `alpha `值的颜色位深。
- `width & height`：当前 `sample `的最大尺寸，单位是像素。

> **以视频为例**，包含子box：`avc1`，表示是`h.264`的视频
>
> > 不同的编码方式，`stsd`的子`box`的名称会有不同，但是`box`中的字段都是相同的
>
> <img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/1a4bae198d0028f02711fdd15a51a8b1.png" alt="在这里插入图片描述" style="zoom:33%;" />
>
> - `data_reference_index`
> - `width、height`
> - `horizresolution、vertresolution`：每英寸的像素值(dpi)，[16.16]格式的数据。
> - `frame_count`：每个`sample`中的视频帧数，默认是`1`。
>   - 可以一个`sample`中有多帧数据。
> - ``depth``: 位图的深度信息，比如` 0x0018（24）`，表示不带`alpha`通道的图片；
>
> `avc1 box`下还有一个`avcc`，里边会有 **sps，pps** 等解码必要信息。
>
> | Field                           | Comment                                                      |
> | ------------------------------- | ------------------------------------------------------------ |
> | type                            | box类型，包括 “预定义类型”、“自定义扩展类型”。预定义类型：比如ftyp、moov、mdat等预定义好的类型；自定义扩展类型：如果type==uuid，则表示是自定义扩展类型。size（或largesize）随后的16字节，为自定义类型的值（extended_type） |
> | size                            | 包含box header在内的整个box的大小，单位是字节。当size为0或1时，需要特殊处理 |
> | data_reference_index            | 当MP4文件的数据部分，可以被分割成多个片段，每一段对应一个索引，并分别通过URL地址来获取，此时，data_reference_index 指向对应的片段（比较少用到） |
> | width、height                   | 视频的宽高，单位是像素                                       |
> | horizresolution、vertresolution | 水平、垂直方向的分辨率（像素/英寸），默认是0x00480000（72dpi） |
> | frame_count                     | 一个sample中包含多少个frame，对video track来说，默认是1      |
> | compressorname                  | 仅供参考的名字，通常用于展示，占32个字节，比如 AVC Coding。第一个字节，表示这个名字实际要占用N个字节的长度。第2到第N+1个字节，存储这个名字。第N+2到32个字节为填充字节。compressorname 可以设置为0 |
> | depth                           | 位图的深度信息，比如 0x0018（24），表示不带alpha通道的图片   |
>
> 上述的例子可能会以下图的形式表示：(`avcc`作为`stsd`的子`box`，没有`avc1`子`box`)
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqhgc2z8IJicqvqdZgndwATiaMKgIcZRmsBkH4ktCAxcjI2yJlgciaWxokQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
>
> >  `avcC` 条目里包含了视频的 SPS 和 PPS 的信息，这些都是视频解码需要的信息。
> >
> > SPS 和 PPS 是 H.264 流中的元信息，在 MP4 文件中单独存放在 `avcC` 中。
> >
> > 转换的时候，还需要将 SPS 和 PPS 提取出来，添加上 0x00000001，放在 H.264 视频流的开始位置。
>
> ​	对于 H.265，其元信息在 `hvcC` 类型 Box 中。



> `**audio track**`的`stsd body`内容（不同的编码方式，stsd的子box的名称会有不同，但是box种的字段都是相同的）：
>
> | Field                | Comment                                                      |
> | -------------------- | ------------------------------------------------------------ |
> | type                 | box类型，包括 “预定义类型”、“自定义扩展类型”。预定义类型：比如ftyp、moov、mdat等预定义好的类型；自定义扩展类型：如果type==uuid，则表示是自定义扩展类型。size（或largesize）随后的16字节，为自定义类型的值（extended_type） |
> | size                 | 包含box header在内的整个box的大小，单位是字节。当size为0或1时，需要特殊处理 |
> | data_reference_index | 当MP4文件的数据部分，可以被分割成多个片段，每一段对应一个索引，并分别通过URL地址来获取，此时，data_reference_index 指向对应的片段（比较少用到） |
> | channel_count        | 声道数，取值为1或2                                           |
> | samplesize           | 采样位宽，一般为8bit或16bit                                  |
> | samplerate           | 采样率                                                       |
>
> <img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/d4bc29094841a0e69d3a0bd06939b93c.png" alt="在这里插入图片描述" style="zoom:33%;" />
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqS9LsU4jejPE3VwDMw4M6tN0U0MoJK47bZa8smKxtibWGlwzhcld9t4Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

​		

> **hint track**的stsd body内容：
>
> | Field | Comment  |
> | ----- | -------- |
> | data  | hint数据 |



### stts box

​		`moov/trak/mdia/minf/stbl/stts(ctts)`

​		`Decoding Time to Sample Box`。 `stts `包含的信息是相邻两帧的解码间隔时间。

> ​		存储了该 `track `每个 `sample `到 `dts `的时间映射关系。主要用来推导每个帧的时长，描述了`sample`时序的映射方法，通过它可以找到任何时间的`sample`。

> 有` Decoding Time To Sample Box(stts)` 和 `Composition Time To Sample Box(ctts) `两种`time to sample box`

​		包含了一个压缩版本的表，通过这个表可以从解码时间映射到`sample`序号。表中的每一项是连续相同的编码时间增量(`Decode Delta`)的个数和编码时间增量。通过把时间增量累加就可以建立一个完整的`time to sample`表。

```c++

aligned(8) class TimeToSampleBox extends FullBox(’stts’, version = 0, 0) {
    unsigned int(32) entry_count;
    int i;
    for (i = 0; i < entry_count; i++) {
        unsigned int(32) sample_count;
        unsigned int(32) sample_delta;
    }
}
```

| 字段         | 类型   | 描述                                                         |
| ------------ | ------ | ------------------------------------------------------------ |
| version      | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags        | 24 bit |                                                              |
| entry_count  | 32 bit | 条目个数                                                     |
| 开始循环     |        |                                                              |
| sample_count | 32 bit | 单个entry中，具有相同时长（duration 或 sample_delta）的连续sample的个数 |
| sample_delta | 32 bit | 单个 sample 的播放时长，单位为 timescale，也可以说是相邻两个sample之间dts的差值 |
| 结束循环     |        |                                                              |

- `entry_count`：表中条目的个数。
- `sample_count`: 连续相同时间长度的`sample`个数。
  - 连续相同 `delta `
  - 持续时间相同的连续的 `Sample `可以放到一个 `entry `里面，以达到节省空间的目的。

- `sample_delta`：以`timescale`为单位的时间长度。

​		

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/ce7e891d846beea1a46a4ff32650eabd.png" alt="在这里插入图片描述" style="zoom:33%;" />



### ctts box

​		`composition time to sample box`。提供了`decoding time`到`composition time`的时间差`offset`的表，用于计算`pts`。

> `ctts `包含的信息是` decoding time `和 `composition time `的差值。

> `Time To Sample Box `中记录的时间信息，通常可以用来通过时间做 `seek `操作。

​		如果一个视频只有`I`帧和`P`帧，则`ctts`这个表就不需要了，因为解码顺序和显示顺序是一致的，但是如果视频中存在`B`帧，则需要`ctts`。这个表在`Decoding time`和`composition time`不一样的情况下时必须的。



```c++

aligned(8) class CompositionOffsetBox extends FullBox(‘ctts’, version = 0, 0) {
    unsigned int(32) entry_count;
    int i;
    for (i=0; i < entry_count; i++) {
        unsigned int(32) sample_count;
        unsigned int(32) sample_offset;
    }
}
```

| 字段           | 类型   | 描述                                                   |
| -------------- | ------ | ------------------------------------------------------ |
| version        | 8 bit  | 取 0 或 1，一般取 0                                    |
| flags          | 24 bit |                                                        |
| sample_counts  |        | 单个entry中，具有相同差值（pts-dts）的连续sample的个数 |
| sample_offsets |        | 从解码（dts）到渲染（pts）之间的差值                   |

- `version`

  - `version`等于`0`，`decoding time`必须小于等于`composition time`，因而差值用一个无符号的数字表示。

    > 有以下公式：`CT(n)=DT(n)+CTTS(n)`
    >
    > cts = pts - dts
    >
    > 注：`CTTS(n)`是未压缩的表的第n个sample对应的offset。

    ​	

- `sample_count`：连续相同的`offset`的个数。
- `sample_offset`：`CT`和`DT`之间的`offset`。记录以 `timescale `为精度的时间长度。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/bee721b47a97b3b9795577decfc289e9.png" alt="在这里插入图片描述" style="zoom:33%;" />

> 示例：
>
> 表中有一序列的 I、P、B 帧，他们是按照解码时间排列的。表中给出了各帧的 Decoding Time(DT) 和 Composition Time(CT)，并据此计算出了对应的 Decode delta 和 Composition offset。
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqsTY2WoIiae4ibPiaTEibkQzZjVoibRibHliavwQaxPkUz0ibop2CRXlKicXA3Mw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
>
> 下面则是与上面对应的，在 stts 存储的 Decode delta 的信息。因为，连续的 14 个 sample 的 Decode delta 都是 10，所以一条数据即可记录。DT 和 stts 表数据的计算公式：`DT(n+1) = DT(n) + STTS(n)`。
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqgQibxO5pxjgn11WqF4rhExUmo8SgGSzp4ADiaW3iagNnGJNMJU8pvgH6Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
>
> 下面是对应的在 ctts 存储的 Composition offset 的信息。CT、DT 和 ctts 表数据的计算公式：`CT(n) = DT(n) + CTTS(n)`。
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqs7ibBPMI1v0uJkfCdvOkicSjRsDN6g8OzWkLuosMrFZ2OQrA3DjQdoiaA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

###  stss box

​		`moov/trak/mdia/minf/stbl/stss`

​		`Sync Sample Box`。针对视频 `track`，包含可随机访问的 `Sample `序号列表，一般可以认为是关键帧序号列表。包含`sample`的数量和每个`sample`的字节大小，这个`box`相对来说体积比较大的。

​		关键帧是为了支持随机访问。如果此表不存在，说明每一个 `sample` 都是一个关键帧。

```c++
aligned(8) class SyncSampleBox extends FullBox(‘stss’, version = 0, 0) {
    unsigned int(32) entry_count;
    int i;
    for (i = 0; i < entry_count; i++) {
        unsigned int(32) sample_number;
    }
}
```

| 字段          | 类型   | 描述                   |
| ------------- | ------ | ---------------------- |
| version       | 8 bit  | 取 0 或 1，一般取 0    |
| flags         | 24 bit |                        |
| entry_count   | 32 bit | 条目个数               |
| 开始循环      |        |                        |
| sample_number | 32 bit | sample 计数，从 1 开始 |
| 结束循环      |        |                        |

- `entry_count`：下面条目表的数量。如果是 0 则表示视频流中没有可以随机访问的位置。
- `sample_number`：可随机访问的 sample 的序号，一般可以认为是关键帧的序号。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/481e6d43197257e3e8a1d5603c6b18a5.png" alt="在这里插入图片描述" style="zoom:33%;" />

> 一个视频 track 的 stss 的示例：
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqWibz9XLJlILqQ6B54nNUNZsaL3WMy6JNWZlgKON4pzGJE6OzhGMABhQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)



### stsc box

​		`moov/trak/mdia/minf/stbl/stsc`

​		`Sample To Chunk Box`。存储了该 `track `中每个 `sample `与 `chunk `的映射关系。媒体数据的 `sample `采样在文件中是以 `chunk `的形式组装起来的。这个 `Box `可以用来找到包含 `sample `的 `chunk`，以及 `chunk `的位置和描述信息。

> media`中的`sample`被分为组成`chunk`。`chunk`可以有不同的大小，`chunk`内的`sample`可以有不同的大小。

​		通过`stsc`中的`sample-chunk`映射表可以找到包含指定`sample`的`chunk`，从而找到这个`sample`。

​		结构相同的`chunk`可以聚集在一起形成一个`entry`，把一组相同结构的`chunk`放在一起进行管理，是为了压缩文件大小。这个`entry`就是`stsc`映射表的表项。

```c++
aligned(8) class SampleToChunkBox extends FullBox(‘stsc’, version = 0, 0) {
    unsigned int(32) entry_count;
    for (i = 1; i <= entry_count; i++) {
        unsigned int(32) first_chunk;
        unsigned int(32) samples_per_chunk;
        unsigned int(32) sample_description_index;
    }
}
```

| 字段                     | 类型   | 描述                                                     |
| ------------------------ | ------ | -------------------------------------------------------- |
| version                  | 8 bit  | 取 0 或 1，一般取 0                                      |
| flags                    | 24 bit |                                                          |
| first_chunk              |        | 当前表项中，对应的第一个chunk的序号                      |
| samples_per_chunk        |        | 同一个entry中每个chunk包含的sample数                     |
| sample_description_index |        | chunk使用的stsd的序号，即不同chunk可以使用不同编解码信息 |

- `entry_count`：表示后面表中的条目数量。
- `first_chunk`：一组`chunk`的第一个`chunk`的序号。
  - `chunk`的编号从1开始。
  - 这个字段理解起来有点复杂。
    - 每个条目可能对应着 1 个或多个 `chunk`
    - 这些 `chunk `共享后面的 `samples_per_chunk `和 `sample_description_index `字段
    - 其中的第一个 `chunk `则是这里的 `first_chunk`。
- `samples_per_chunk`：每个chunk有多少个sample。
- `sample_desc_idx：stsd `中sample desc信息的索引，即stsd 表项序号。

> <img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/c3b7c89c564499226bc7fea5e428ece0.png" alt="在这里插入图片描述" style="zoom:33%;" />
>
> - 第1个`entry`的第一个`chunk`的序号是1
>   - 第一个`entry`每一个`chunk`包含一个`sample`，即第`1-8224`个`chunk`，每个`chunk`包含1个`sample`。
> - 第2个`entry`的第一个`chunk`的序号是`8225`
>   - 即第1个`entry`包含索引值为`1-8224`的`chunk`
>   - 第二个`entry`每一个`chunk`包含`342`个`sample`，即第`8225`的`chunk`个`chunk`包含`342`个`sample`。
> - 第3个`entry`的第一个`chunk`的索引值时`8226`，以此类推
>   - 第三个`entry`每一个`chunk`包含382个`sample`，即第`8226`个`chunk`包含`382`个`sample`，以此类推。
> - 最后一个`entry`就是从`first_chunk`开始一直到结束为止。
>
> 以上所有`chunk`中的`sample`，对应的`sample description`的序号都是`1`，也就是用第一个`stsd`。

> 一个视频 track 的 stsc 的示例：
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqdKcScJ4WIicdichtgZ7oAibhyzugYn2T2Gbcg9iawDmjROBXnWpJeGIvSg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqMcLKVUibPoHxL4TNJFzpibUqJ5KsgkySdRzvlYC6DXXvMTWB5tX4gIXg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
>
> - `entry table `里面只有 2 条数据
>   - 这里与上面 `stco `示例的视频是同一个视频
>   - `stco `的信息显示，该视频 `track `有 `743 `个 `chunk`
> - 那么这里的 `stsc `的 `entry table` 的数据则表示，第 `[1, 2-1]` 个 chunk 都包含了 2 个 `sample`
> - 第 `[2, last=743]` 个 `chunk `都包含了 1 个 `sample`
> - 所以，总共是 `(1 * 2) + (743 - 2 + 1) * 1 = 744` 个 `sample`
> - 这个数量刚好和 `stsz `示例中显示的 `sample `数量对的上。

### stsz/stz2  box

​		` Sample Size Box`

​		Sample size box。 存储了该 track 中每个 `sample `的字节大小。

​		这个`box`相对来说体积比较大的。表明视频帧或者音频帧大小，`FFmpeg `里面的`AVPacket `的`size `数据大小，就是从这个`box`中来的。

​		有两种不同的box类型：`stsz`、`stz2`。

#### stsz

```c++
aligned(8) class SampleSizeBox extends FullBox(‘stsz’, version = 0, 0) {
    unsigned int(32) sample_size;
    unsigned int(32) sample_count;
    if (sample_size == 0) {
        for (i = 1; i <= sample_count; i++) {
            unsigned int(32) entry_size;
        }
    }
}
```

| 字段                       | 类型   | 描述                                                         |
| -------------------------- | ------ | ------------------------------------------------------------ |
| version                    | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags                      | 24 bit |                                                              |
| sample_size                |        | 默认的sample大小（单位是byte），通常为0。如果sample_size不为0，那么，所有的sample都是同样的大小。如果sample_size为0，那么，sample的大小可能不一样 |
| sample_count               |        | 当前track里面的sample数目。如果 sample_size==0，那么，sample_count 等于下面entry_size中的数据个数 |
| entry_size（sample_sizes） |        | 单个sample的大小（如果sample_size==0的话）                   |

- `sample_size`：
  - 如果所有的 `sample `都是一样的大小，那么这个字段的值就对应这个大小。
  - 如果 `sample `的大小各自不同，那么这个字段的值就是 `0`，这时候每个 `sample `的大小存储在` sample size table` 中。
- `sample_count`：表示 sample 的数量。
- `entry_size`：表示 `sample size table `中每个 `sample `的大小。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/85a6f285026ca71ed99e9266e3ba72f1.png" alt="在这里插入图片描述" style="zoom:33%;" />

> 一个视频 track 的 stsz 的示例：
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAq5iaTlncQDUWwKnDtJQJWL67W3vEQQxWOibczIwwWgC1brZD6OQicDV0xg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

#### stz2

| 字段         | 类型   | 描述                                                         |
| ------------ | ------ | ------------------------------------------------------------ |
| version      | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags        | 24 bit |                                                              |
| field_size   |        | entry表中，每个entry_size占据的位数（bit），可选的值为4、8、16。4比较特殊，当field_size等于4时，一个字节上包含两个entry，高4位为entry[i]，低4位为entry[i+1] |
| sample_count |        | 等于下面entry_size中的数据个数                               |
| entry_size   |        | 单个sample的大小                                             |



### stco/co64 box

​		`moov/trak/mdia/minf/stbl/stco(co64)`

​		`Chunk Offset Box`。存储了该 `track `中每个 `chunk `在文件中的偏移。这样就可以直接在文件中找到媒体数据，而不用解析box。

> **需要注意的是一旦前面的box有了任何改变，这张表都要重新建立。**		
>
> 这个偏移是相对文件初始位置的偏移。所以这里需要注意的一点是，当修改 `mdat Box `之前其他 `Box `的信息时，会影响到 `Chunk Offset`，这里的记录则需要做对应的更新。

> **需要注意**，这里 `stco` 只是指定的每个 `Chunk `在文件中的偏移位置，并没有给出每个 `Sample `在文件中的偏移。想要获得每个 `Sample `的偏移位置，需要结合 `Sample Size box` 和 Sample-To-Chunk 计算后取得。
>
> 在构建 `MP4 `文件的时候，需要特别注意 `moov `所处的位置，它对于`chunk_offset `的值是有影响的。有一些`MP4`文件的 `moov` 在文件末尾，为了优化首帧速度，需要将 `moov `移到文件前面，此时，需要对 `chunk_offset `进行改写。

​		针对小文件、大文件，有两种不同的box类型，分别是stco、co64，它们的结构是一样的，只是字段长度不同。

> ​		`stco `有两种形式，如果你的视频过大的话，就有可能造成 `chunkoffset `超过` 32bit` 的限制。所以，这里针对大 `Video `额外创建了一个 `co64 `的 Box`。`它的功效等价于 `stco`，也是用来表示 `sample `在` mdat box` 中的位置。只是，里面 `chunk_offset `是 `64bit `的。

```c++
aligned(8) class ChunkOffsetBox extends FullBox(‘stco’, version = 0, 0) {
    unsigned int(32) entry_count;
    for (i=1; i <= entry_count; i++) {
        unsigned int(32) chunk_offset;
    }
}

aligned(8) class ChunkLargeOffsetBox extends FullBox(‘co64’, version = 0, 0) {
    unsigned int(32) entry_count;
    for (i = 1; i <= entry_count; i++) {
        unsigned int(64) chunk_offset;
    }
}
```

| 字段         | 类型   | 描述                                                         |
| ------------ | ------ | ------------------------------------------------------------ |
| version      | 8 bit  | 取 0 或 1，一般取 0                                          |
| flags        | 24 bit |                                                              |
| chunk_offset |        | chunk 在文件中的位置（在文件本身中的 offset，而不是某个box内部的偏移） |

- `entry_count`：表示下面的条目表的条目数量。

  - 其实也就是 `Chunk `的数量。
  
- `chunk_offset`：chunk在文件中的位置。

  >  指的是在文件本身中的 `offset`，而不是某个`box`内部的偏移。



<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/3b302e6447f8ae0c9b8f6228bcdc93d7.png" alt="在这里插入图片描述" style="zoom:33%;" />

> 一个视频 track 的 stco 的示例：
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAqMcLKVUibPoHxL4TNJFzpibUqJ5KsgkySdRzvlYC6DXXvMTWB5tX4gIXg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### udta box

​		User Data Box。用户自定义数据



## `mp4`文件类型

​		一个 `MP4 `文件的典型结构如下：

```
  +-+-+-+-+-+-+-+-+-+-+-+-+-+
  |  ftyp  |  moov |  mdat  |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+
```

​		因为 `MP4 `标准中没有对 `moov `和 `mdat `的存放位置没有强制先后要求，所以它们的顺序不定。

​		在互联网视频的点播中

- 如果希望 `MP4 `文件被快速打开，则需要将 `moov `存放在 `mdat `的前面
- 如果`moov `放在 `mdat `的后面，则需要将 `MP4 `文件全部下载完才可以播放

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/fa1f7373789fe4d7ed91ed31e8b36f47.png" alt="img" style="zoom: 50%;" />





### 普通MP4，General MP4

- `box`模式为`ftyp-mdat-moov`
- 头在文件末尾、
- `FFMPEG`默认出的就是这种，写入比较简单，读取时需要`seek`
- 有些浏览器需要完全下载后才能开始播放，有些可以用`HTTP RANGE`跳过`mdat`读取`moov`
- `SRS`录制`MP4`文件可以用这种模式。

### 快速MP4，Faststart MP4

- `box`模式为`ftyp-moov-mdat`
- 头在文件开始
- `FFMEPG`需要指定参数，生成`MP4`后，再过一遍文件才能支持
- 对于某些浏览器会比较友好，但是就需要再过一遍文件
- `SRS`录制成`MP4`文件而言，容易造成`IO`阻塞
  - 等待的时间过长，导致服务线程挂起，具体看`ST`的实现，不可以长时间的做`CPU`操作或者磁盘操作

### 切片`MP4`，`Fragmented MP4`，或`FMP4`

- `box`模式为`ftyp-moov-moof-mdat`
- 分段模式
- 浏览器`H5`直接播放时，有些能播，有些不行。
- 一般用在`DASH`中，不会直接用浏览器播放，而是通过`JS`解析后，给`MSE`播放（`APPEND`到`Video`对象的`Buffer`中去，具体参考`MSE`的文章）
- `FFMPEG`需要指定参数才能生成
- 对于流媒体比较合适
- `SRS`生成`DASH`可以用这种

<img src="https://i0.hdslb.com/bfs/article/cf3c8621da31ed79fa76c6a0a8babfe1e166beeb.png@1256w_916h_!web-article-pic.webp" alt="图片" style="zoom: 80%;" />



## 解析过程

​		指解封装、播放的过程：先读取`box`中的内容，然后根据`box`中内容可以找到每一个`sample`也就是每一帧数据，然后进行解封装、播放。

​		详细过程如下：

1. 通过`stss`确定关键帧的数据和每个关键帧的序号。
2. 通过`stts`获取音视频流的总帧数、每一帧的`dts`和整体时长。
3. 通过`ctts`获取每一帧中`pts`与`dts`的差值。
4. 通过`stsz`获取每个`sample`的大小及整个流的总大小。
5. 通过`stco`获取音视频流中`chunk`的数据和在文件中的位置（不是在`mdat`中的位置）。
6. 通过`stsc`计算出流中`chunk`的数据和在文件中的位置，并通过`chunk`在文件中的位置及`chunk`中每个`sample`的大小，确定每个`sample`在文件中的位置和大小。
7. 整理每个流的码流信息和时间戳信息，构成整体播放的时间轴，之后进行解码的渲染播放。



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



### http播放mp4

[http协议的MP4文件播放问题的分析 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7279452.html)



### 使用`FFMpeg`将`moov`前置

​		为了优化首帧速度，需要将 `moov `移到文件前面，此时，需要对 `chunk_offset `进行改写。

​		使用`ffmpeg`将`moov`前移很简单，在写文件前设置这个`flag`即可：

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



### 5.1、moov 和 MP4 视频的秒开

了解了 MP4 的文件结构后，我们知道了 moov 这个 Box 包含着对视频来说非常重要的索引信息，所以一般播放器需要拿到这些信息才能完成解码器的初始化，开启播放流程。而 moov Box 在文件中的位置是可以随意放置的。它可以放在包含视频实际数据的 mdat Box 前面，也可以放在其后面。一般来讲，如果不做特别设置，moov 会放在 mdat 后面，因为从正常的处理流程上来讲，当所有的音视频数据都处理完成后，才能确定对应的宏观信息和索引信息，这时候才能确定 moov 的信息。

当 moov 放在 mdat 后面时，我们修改视频中 moov/udta 中的用户自定义信息时，不会对 mdat 的 Chunk Offset 造成影响，这样就不需要更新 stco 里的数据，编辑效率较高。但是，当从网络读取和播放 MP4 文件时就需要等待较长时间，直到播放器获取到 moov 的数据后才能初始化解码器并开始播放。

当 moov 放在 mdat 前面时，则与上述情况相反，这时候从网络读取和播放 MP4 文件时，就可以较快获取到 moov 的数据并开始播放。所以一般来说，**对于通过网络播放 MP4 视频的场景，都建议将视频处理为 moov 前置。**

我们可以使用 FFmpeg 将一个 moov 后置的 MP4 处理为前置：

- 

```
ffmpeg -i slow_play.mp4 -movflags faststart fast_play.mp4
在实际开发中，我们通过手机编辑视频时，要么是自己拍摄的，要么是从相册选择的。
```

在 iOS 中，可以通过系统提供 AVAssetExportSession 来导出视频，其中有一个 `shouldOptimizeForNetworkUse` 接口可以用来支持 fast start。一般在自己拍摄视频的场景可以这样来做。

- 
- 

```
/* indicates that the output file should be optimized for network use, e.g. that a QuickTime movie file should support "fast start" */@property (nonatomic) BOOL shouldOptimizeForNetworkUse;
```

如果只是从相册选择的视频，我们可以借由 AVAssetExportSession 来对视频重新打包来实现 moov 前置。当然，我们也可以自己写一段代码，单纯地针对 MP4 文件来完成 moov 前置的操作。

在实际应用场景中，我们还遇到过由于视频 moov 后置并且视频太大（分辨率 1920x1080，码率 5 Mbps，帧率 25 fps）导致在手机浏览器播放失败的情况。如下图：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuejJrgRTLgovpyFlfffh6IAq97jsJWXcuJNgYDibdCpIFb1ubAHicAaSzpW2IbKicMVDkQ9ovwSoNQU5g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

遇到报错：

```
Client closed connection before receiving entire response
```

这种情况可能是由于 moov 后置导致浏览器播放内核需要加载的数据太大而触发某些限制导致了主动断开连接。

### 5.2、MP4 视频的预加载

在视频相关的业务实现中，有很多需要我们关注的用户体验点，**视频迅速开播**和**无黑屏**是其中两个常见的关注点。

对视频进行预加载是提升这两个体验点的技术方案之一。那么现在问题来了：预加载多少数据比较合适呢？

在 iOS 中，系统提供的 AVPlayer 有一个特性，即使没有开始播放，当 AVPlayer 加载到足够的数据后，它会把视频的第一帧显示出来，就像是视频的封面一样。结合这个特性，如果我们能够预加载一定的数据量保证 AVPlayer 刚好能把视频首帧渲染出来，这样就能确保视频能迅速开播，并且用户一打开视频就能看到画面，实现了无黑屏。那么对于 AVPlayer 来说这个需要的数据量是多少呢？

通过我们对 AVPlayer 的反复试验，我们发现：AVPlayer 拿到第一个关键帧的 sample 数据即可渲染出首帧画面。基于上面我们对 MP4 文件结构的介绍，我们其实可以找出这个数据量的算法：

- 1）找到 moov 中的视频对应的 track，从中找出 Sync Sample Box(moov/trak/mdia/minf/stbl/stss)，找出第一个 sync sample 的序号 `x`。
- 2）从 Sample To Chunk Box(moov/trak/mdia/minf/stbl/stsc) 中找出序号为 x 的 sample 所在的 chunk 的序号 `k`，以及该 chunk 中在 sample x 之前的其他 sample 的数量 `m`。
- 3）结合 Sample Size Box(moov/trak/mdia/minf/stbl/stsz) 中的 sample size table 找出 sample x  的 size 以及其在它前面的 m 个 sample 的 size，计算这些 size 求和得到 `s`。
- 4）在 Chunk Offset Box(moov/trak/mdia/minf/stbl/stco(co64)) 中找到序号为 k 的 chunk 的 offset 值 `t`。
- 5）需要加载的数据量则为：`t + s`。

我们还可以通过下面的命令：

- 

```
ffprobe -show_frames -select_streams v -skip_frame nokey -show_entries frame=pict_type,pkt_pos,pkt_size,media_type -i <video_path>
```

找到第一帧视频 I 帧的 pkt_pos 和 pkt_size，二者相加就是从文件开始位置取得第一帧视频 I 帧所需要的字节数。



