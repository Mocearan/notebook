# FLV

---

​		`FLV（Flash Video）`是 `Adobe `公司设计开发的一种流行的流媒体格式，由于其视频文件体积轻巧、封装简单等特点，使其很适合在互联网上进行应用。

- FLV可以使用` Flash Player` 进行播放
- FLV 封装格式的文件后缀通常为`.flv`
- 直播场景下拉流比较常见的是 http-flv 直播流
  - 具有延时低，易传输等特点


​		



## 参考

https://blog.csdn.net/ProgramNovice/article/details/137507298



[FLV 文件格式分析_flv格式解析-CSDN博客](https://blog.csdn.net/xiaopangcame/article/details/134653356)

[媒体文件格式分析之FLV - 懒人李冰 (lazybing.github.io)](https://lazybing.github.io/blog/2016/07/24/flv-parse/)

[flv格式详解+实例剖析 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9018548.html)

[flv 解封装 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020407.html)

[h264 aac 封装 flv - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020423.html)

[将h264和aac码流合成flv文件 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020453.html)

[librtmp接收flv流中提取h264码流:根据多个资料汇总 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020492.html)

[rtmp直播推流（一）－－flv格式解析与封装 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9018490.html)

## 结构

​		FLV 是一个二进制文件，由一个文件头（`FLV header`）和很多 `tag `和`Previous Tag Size`组成（`FLV body`）

`FLV Header(9 byte) |  PreviousTagSize | Tag(meta) |  PreviousTagSize | Tag(video) |  PreviousTagSize | Tag(audio) | .... | PreviewTagSize`

- `tag `可以分成三类: `audio`, `video`, `script`，分别代表音频流，视频流，脚本流
- 而每个 `tag `又由` tag header `和 `tag data` 组成
  -  `Video Tag` 和 `Audio Tag` 由 `Tag header` 和` Tag Data` 组成；`Script Tag` 只有 `Tag Data`
- `Previous Tag Size`占用4字节，记录前一个`tag`的大小，用于逆向读取
  - `FLV Header`后的一个`PreviewsTagSize`为`0`
- FLV 数据以大端序进行存储，在解析时需要注意

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/b8c7e57dd80b2cc7752461da6476dc0b.png#pic_center)

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/76b84bb654aba677aa6edd8f39f813c8.jpeg#pic_center)

```c
/*
 * @brief flv file header 9 bytes
 */
typedef struct flv_header {
    uint8_t signature[3];
    uint8_t version;
    uint8_t type_flags;
    uint32_t data_offset; // header size, always 9
} __attribute__((__packed__)) flv_header_t;

/*
 * @brief flv tag general header 11 bytes
 */
typedef struct flv_tag {
    uint8_t tag_type;
    uint32_t data_size;
    uint32_t timestamp;
    uint8_t timestamp_ext;
    uint32_t stream_id;
    void *data; // will point to an audio_tag or video_tag
}   flv_tag_t;

typedef struct audio_tag {
    uint8_t sound_format; // 0 - raw, 1 - ADPCM, 2 - MP3, 4 - Nellymoser 16 KHz mono, 5 - Nellymoser 8 KHz mono, 10 - AAC, 11 - Speex
    uint8_t sound_rate; // 0 - 5.5 KHz, 1 - 11 KHz, 2 - 22 KHz, 3 - 44 KHz
    uint8_t sound_size; // 0 - 8 bit, 1 - 16 bit
    uint8_t sound_type; // 0 - mono, 1 - stereo
    void *data;
}   audio_tag_t;

typedef struct video_tag {
    uint8_t frame_type;
    uint8_t codec_id;
    void *data;
}   video_tag_t;

typedef struct avc_video_tag {
    uint8_t avc_packet_type; // 0x00 - AVC sequence header, 0x01 - AVC NALU
    uint32_t composition_time;
    uint32_t nalu_len;
    void *data;
}   avc_video_tag_t;
```



### FLV Header

 		9 个字节，用来标识文件类型为 FLV 类型，以及后续的音视频标识。

​		`flv`文件最多只有一个音频流，一个视频流，不存在多个独立的音视频流在一个文件的情况。每种`tag`都属于一个流。

- 签名：`8 bit`，`F(0x46)`
- 签名：`8 bit`，`L(0x4c)`
- 签名：`8 bit`，`V(0x56)`
- 版本：`8 bit`，FLV的版本
  - `0x01` 表示版本为1
- 保留字段：`5 bit`，`0 0000`
- 音频标识，`1 bit`，是否存在音频流
  - `1` 存在
  - `0`不存在
- 保留字段：`1 bit`，`0`
- 视频表示：`1 bit`，是否存在视频流
  - `1` 存在
  - `0`不存在
- 文件头大小：`32 bit`
  - 版本为1是填`9`，表明FLV头的大小
  - 后期FLV拓展时使用，

```c
typedef struct {
    UI8 Signature; // 'F' (0x46)
    UI8 Signature; // 'L' (0x4C)
    UI8 Signature; // 'V' (0x56)
    UI8 Version; // FLV 版本。例如，0x01 表示 FLV 版本 1
    UI8 TypeFlags; // b0 是否存在视频流，b2 是否存在音频流，其他字段保留，值为 0
    UI32 DataOffset; // FLV Header 长度，单位：字节
} FLVHEADER;
```



### FLV body

​	` PreviousTagSize | Tag(meta) |  PreviousTagSize | Tag(video) |  PreviousTagSize | Tag(audio) | .... |  PreviousTagSize`

```c
typedef struct {
    UI32 PreviousTagSize0;
    FLVTAG Tag1;
    UI32 PreviousTagSize1;
    FLVTAG Tag2;
    ...
    UI32 PreviousTagSizeN-1;
    FLVTAG TagN;
    UI32 PreviousTagSizeN;
} FLVBODY;

```



#### Previous Tag Size

​		4个字节，记录前一个 Tag 的大小，用于逆向读取处理。



#### Tag

​		Tag 由 Tag Header 和 Tag Body 组成:

- Tag Header 占 11 字节
- Tag Body 一般分为三种类型：脚本（Script）数据、视频数据、音频数据
  - Video Tag 和 Audio Tag 由 Tag header 和 Tag Data 组成
  - Script Tag 只有 Tag Data

```c
typedef struct {
    // tag header
    UB[2] Reserved; // 用于 FMS 的保留字段, 值为 0
    UB[1] Filter; // 指示 packet 是否需要预处理。0 = 不需要预处理。1 = packet 在渲染前需要预处理。未加密文件中此值为0，加密文件中此值为1。
    UB[5] TagType; // 8 = 音频，9 = 视频，18 = 脚本数据
    UI24 DataSize; // tag data 的长度。等于 tag 总长度 – 11
    UI24 Timestamp; // 当前tag的解码时间戳 (DTS)，单位是ms。第一个tag的DTS总是0
    UI8 TimestampExtended; // 和Timestamp字段一起构成一个32位值, 此字段为高8位。单位是ms
    UI24 StreamID; // 总为 0
  IF TagType == 8
    AudioTagHeader Header;
  IF TagType == 9
    VideoTagHeader Header;
    
    // tag body
  IF TagType == 8
    AUDIODATA Data;
  IF TagType == 9
    VIDEODATA Data;
  IF TagType == 18
    SCRIPTDATA Data;
} FLVTAG;
```



##### tag header

​		存放当前 tag 的类型、数据区长度、时间戳等信息。tag header 一般占 11 个字节。

- `TagType`，`8 bit`，tag类型
  - `0x08`：8 音频数据
  - `0x09`：9 视频数据
  - `0x12`：18 脚本数据
- `DataSize`：`24 bit`，数据大小
  - 不包含`tag header`大小
- `Timestamp`：`24 bit`，时间戳，单位毫秒，相对于第一个`tag`时间戳的差
- `TimestampExtended`：`8 bit`，如果`Timestamp > 0xFFFFFF`，使用此字段存储时间戳的高`8 bit`
- `StreamID`：`24 bit`，`0`



##### audio tag

- `audio tag header`，`1 byte`，包含了音频数据的参数信息

  - `4 bit`，音频格式

    - `2`，MP3
    - `7/8`，G711
    - `10`，AAC

  - `2 bit`，采样率

    - `3`，`44-kHz`

  - `1 bit`，采样精度

    - `0`，`snd 8bit`
    - `1`，`snd 16bit`

    > 压缩过的音频都是16bit

  - `1 bit`，音频声道

    - `0`， `sndMono` 单声道
    - `1` `，sndStereo` 立体声，双声道

  -  AAC 格式下，AudioTagHeader 中会多出一个字节 AACPacketType

    - 表示 AACAUDIODATA 的类型：0 = AAC sequence header，1 = AAC raw

    - AAC sequence header 也就是包含了 AudioSpecificConfig，里面有一些更加详细音频的信息。

    - AAC raw 这种包含的就是音频 ES 流了，也就是音频负载（audio payload）

    - ```c
      typedef struct {
          UB [4] SoundFormat;
          UB [2] SoundRate;
          UB [1] SoundSize;
          UB [1] SoundType;
        IF SoundFormat == 10
          UI8 AACPacketType;
      } AudioTagHeader;
      ```

- `audio tag body`，音频数据

  - 如果采用了加密，Body 的类型是 EncryptedBody

    - 参考规范文档“附件 F. FLV 加密”章节

  - 如果未采用加密，则 Body 的类型是 AudioTagBody

  - ```c
    typedef struct {
      IF Encrypted
        EncryptedBody Body
      else
        AudioTagBody Body;
    } AudioTagBody;
    
    typedef struct {
      IF SoundFormat == 10 // Flash 播放器 9.0.115.0 及以上版本支持 AAC 格式。
        AACAUDIODATA SoundData;
      ELSE
        Varies by format
    } AudioTagBody;
    
    typedef struct {
      IF AACPacketType == 0
        AudioSpecificConfig;
      ELSE IF AACPacketType == 1
        Raw AAC frame data in UI8;
    } AACAUDIODATA;
    ```



![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/3d87a9b37c2ded2c694adfe38c66f95a.png#pic_center)

​		强调一下格式10格式：AAC。

- 声音类型应为 1 (立体声) 且采样率应为 3 (44 kHz)。
- 这并不表示 FLV 中的 AAC 音频总是立体声、44 kHz的数据。
- 实际上，Flash 播放器会忽略这两个值，而从已编码的 AAC 位流中提取出声道数和采样率信息。



##### video tag

- `video tag header`

  - `FrameType`：`4 bit`，帧类型
  - `CodecID`：`4 bit`， 编码类型
    - `7`：AVC
  - CodecID = 7，视频的格式是 AVC（H.264）的话，VideoTagHeader 会多出 4 个字节的信息
    - AVCPacketType，1 byte 
      - 0， AVC sequence header
      - 1， AVC NALU
      - 2，AVC end of sequence 
    - CompositionTime，3 byte，CTS
      - AVCPacketType == 0，时间戳偏移
      - AVCPacketType == 1， 0
    - 所以 H.264 编码的情况下 VideoHeader 长度是 5 个字节

- `video tag body`

  - 如果采用了加密，Body 的类型是 EncryptedBody

    - 参考规范文档“附件 F. FLV 加密”章节

  - 如果未采用加密，则 Body 的类型是 AudioTagBody

  - ```c
    typedef struct {
      IF Encrypted
        EncryptedBody Body
      else
        VideoTagBody Body;
    } VIDEODATA;
    
    typedef struct {
      IF FrameType == 5
        UI8 VideoData;
      ELSE (
      IF CodecID == 2
        H263VIDEOPACKET VideoData;
      IF CodecID == 3
        SCREENVIDEOPACKET VideoData;
      IF CodecID == 4
        VP6FLVVIDEOPACKET VideoData;
      IF CodecID == 5
        VP6FLVALPHAVIDEOPACKET VideoData;
      IF CodecID == 6
        SCREENV2VIDEOPACKET VideoData;
      IF CodecID == 7
        AVCVIDEOPACKET VideoData;
      )  
    } VideoTagBody;
    
    typedef struct {
      IF AVCPacketType == 0
        AVCDecoderConfigurationRecord Data;
      IF AVCPacketType == 1
        One or more NALUs
    } AVCVIDEOPACKET;
    ```

    > ​	AVCDecoderConfigurationRecord 包含着 H.264 解码相关比较重要的 sps 和 pps 信息，再给 AVC 解码器送数据流之前一定要把 sps 和 pps 信息送出，否则的话解码器不能正常解码。而且在解码器 stop 之后再次 start 之前，如 seek、快进快退状态切换等，都需要重新送一遍 sps 和 pps 的信息。



![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/56c230992f352fac2be8ce2db979b539.png#pic_center)

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/36f32718807a7271c6b375b1c4762295.png#pic_center)





#### script tag

​		又通常被称为 metadata tag，会放一些关于FLV视频和音频的元数据信息如：duration、width、height等。

​	`AMF1("onMetaData") | AMF2("wdith, height...")`

- 通常该类型 tag 会跟在 FLV header 后面作为第一个 tag 出现，而且只有一个
- 没有 script tag header，只有script tag body。一般来说，script tag 包含两个 AMF 包

​	

- - 

```c
typedef struct {
  IF Encrypted
    EncryptedBody Body
  else
    ScriptTagBody Body;
} SCRIPTDATA;

typedef struct {
    SCRIPTDATAVALUE Name;
    SCRIPTDATAVALUE Value;
} ScriptTagBody;
// ScriptTagBody 包含以 AMF 编码的 SCRIPTDATA。AMF 是一种紧凑二进制格式，用于序列化 ActionScript 对象图。


typedef struct {
    UI8 Type;
  IF Type == 0
    DOUBLE ScriptDataValue;
  IF Type == 1
    UI8 ScriptDataValue;
  IF Type == 2
    SCRIPTDATASTRING ScriptDataValue;
  IF Type == 3
    SCRIPTDATAOBJECT ScriptDataValue;
  IF Type == 7
    UI16 ScriptDataValue;
  IF Type == 8
    SCRIPTDATAECMAARRAY ScriptDataValue;
  IF Type == 10
    SCRIPTDATASTRICTARRAY ScriptDataValue;
  IF Type == 11
    SCRIPTDATADATE ScriptDataValue;
  IF Type == 12
    SCRIPTDATALONGSTRING ScriptDataValue;
} SCRIPTDATAVALUE;
```

> ​		SCRIPTDATAVALUE 的两个字段，Type 是类型，ScriptDataValue 是值。Type 的值确定 ScriptDataValue 的类型。
>
> ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/0c00bc24955dbfa9e495d72fba201a02.png#pic_center)

​		第一个 AMF 包（封装字符串类型数据）：

-  AMF 包类型，`1byte`

  - 一般总是`0x02`，表示字符串；

- 字符串的长度，`2 byte`

  - 一般总是` 0x000A（“onMetaData”长度）`

- 后面字节为字符串数据

  - 一般总为 “onMetaData”（6F,6E,4D,65,74,61,44,61,74,61）

- ```c
  typedef struct {
      UI16 StringLength; // StringData 字段的长度，单位字节
      STRING StringData; // 字符串实际数据，注意不带结束符 NUL
  } SCRIPTDATASTRING;
  
  ```

  

​		第二个 AMF 包（封装一个数组类型，这个数组中包含了音视频信息项的名称和值）：

- AMF 包类型，`1byte`

  - 一般总是 0x08，表示数组

- 数组元素的个数，`4 byte`

- 数组元素，是元素名称和值组成的对

  - 第 1-2 个字节表示元素名称的长度，假设为 L。后面跟着为长度为 L 的字符串。
  - 第 L+3 个字节表示元素值的类型。后面跟着为对应值，占用字节数取决于值的类型

- ```c
  typedef struct {
      UI32 ECMAArrayLength; // ECMA 数组元素数量
      SCRIPTDATAOBJECTPROPERTY[] Variables; // 变量名和变量值的列表，即 ECMA 数组
      SCRIPTDATAOBJECTEND ListTerminator; // 列表终止符
  } SCRIPTDATAECMAARRAY;
  
  // SCRIPTDATAOBJECTPROPERTY 类型定义了 ActionScript 对象或关联数组变量的对象属性。
  typedef struct {
      SCRIPTDATASTRING PropertyName;
      SCRIPTDATAVALUE PropertyData;
  } SCRIPTDATAOBJECTPROPERTY;
  ```

  常见的数组元素表示如下表：

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/a7171fb50347e81696c2d214fc7d3534.png#pic_center)

> AMF：Action Message Format，是 Adobe 设计的一种通用数据封装格式，在 Adobe 的很多产品中应用，简单来说，AMF 将不同类型的数据用统一的格式来描述。





## 实例

### onMetaData 对象

​		FLV 元数据对象应在名为 onMetadata 的 SCRIPTDATA 标签中携带。各种属性对通过 NetStream.onMetaData 属性运行的 ActionScript 程序有效。可用的属性根据创建 FLV 文件的软件而有所不同。

- onMetaData 标签通常会成为 FLV Body 中的第一个标签，紧跟在 FLV Header 之后。
  - onMetaData 标签中存储的是一些视频、音频及文件相关的元数据信息
  - 如视频帧率，音频采样率、文件长度等。
- onMetaData 标签的 Name 字段主要就是存储 “onMetaData” 字符串。
  - 第 1 个字节值是 0x02，表示 Name 字段是字符串类型
  - 第 2-3 个字节为 UI16 类型值，标识字符串的长度，值为 0x000A (“onMetaData” 这个字符串的长度)
  - 后面跟着的数据为具体的字符串，值为 “onMetaData”。
- onMetaData 标签的 Value 字段存储上表所示的各属性键值对。
  - 第 1 个字节值是 0x08，表示 Value 字段是数组类型
  - 第 2-5 个字节为UI32类型值，表示数组元素个数
  - 后面紧跟着数组，数组元素为属性名称和值组成的对（键值对）
  - 最后是数组的结束符

```c
ScriptTagBody onMetaData;

onMetaData.Name.Type == 0x02
onMetaData.Name.ScriptDataValue.StringLength == 0x000A
onMetaData.Name.ScriptDataValue.StringData == "onMetaData"

onMetaData.Value.Type == 0x08
onMetaData.Value.ScriptDataValue.ECMAArrayLength == 
onMetaData.Value.ScriptDataValue.Variables[0].PropertyName == {0x0005, "width"}   // SCRIPTDATASTRING 类型
onMetaData.Value.ScriptDataValue.Variables[0].PropertyData == {0x00, 1280.0}      // SCRIPTDATAVALUE 类型
onMetaData.Value.ScriptDataValue.Variables[1].PropertyName == {0x0005, "height"}  // SCRIPTDATASTRING 类型
onMetaData.Value.ScriptDataValue.Variables[1].PropertyData == {0x00, 720.0}       // SCRIPTDATAVALUE 类型
...
```

> PropertyName 的类型是 SCRIPTDATASTRING，第一个参数是字符串长度，第二个参数是字符串。
> PropertyData 的类型是 SCRIPTDATAVALUE，第一个参数是 Type，为 0 时，第二个参数是一个 Double 类型的变量。



## 工具

flvAnalyser是一款FLV分析工具。

下载地址：https://github.com/zymill/flvAnalyser



## parse

详见于：[最简单的 FLV 封装格式解析程序](https://blog.csdn.net/ProgramNovice/article/details/137507298)