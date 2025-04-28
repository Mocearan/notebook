# FLV

---

​		`FLV（Flash Video）`是 `Adobe `公司设计开发的一种流行的流媒体格式，由于其视频文件体积轻巧、封装简单等特点，使其很适合在互联网上进行应用。

- FLV可以使用` Flash Player` 进行播放

  > ​		在浏览器普遍支持 Flash 插件的时代，FLV 格式的视频非常流行。
  >
  > ​		但是随着主流的浏览器平台逐步放弃了对 Flash 插件的支持后，以及移动互联网的兴起，App 取代浏览器成为更多内容的载体，在短视频领域 FLV 的地位逐步被 MP4 取代。
- FLV 封装格式的文件后缀通常为`.flv`
- 直播场景下拉流比较常见的是` http-flv` 直播流
  - 具有延时低，易传输等特点

  > ​	在直播领域，由于 RTMP 推流、HTTP-FLV 播放的整套方案低延时的特性，以及服务端普遍提供 HTTP Web 服务，能更广泛的兼容 HTTP-FLV，使得 FLV 仍然是大多数直播产品的首选流媒体格式。

​		`FLV `格式的 `FLV Tag Heade`r 中携带时间戳，`FLV `将每一帧音视频数据（`Tag Body`）封装成包含时间戳等音视频元数据（`Tag Header`）的数据包（`Tag`）。当播放器拿到 `Tag `后，可根据时间戳等音视频元数据进行解码和播放。

> ​		`MP4、MKV` 等封装格式将音视频数据和音视频元数据、索引、时间戳等分开存放，必须拿到完整的音视频文件才能播放，因为里面的单个音视频数据块不带有时间戳信息，播放器不能将这些没有时间戳信息的数据块连续起来，因此不能实时的解码播放。（当然 MP4 后来扩展了 FMP4 用于流媒体）

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





## 格式

​		`FLV `是一个二进制文件，由一个文件头（`FLV header`）和一个 `FLV Body` 组成，`FLV Body` 中则由很多 `tag `和`Previous Tag Size`组成（`FLV body`）。

```
FLV Header(9 byte) |  PreviousTagSize | Tag(meta) |  PreviousTagSize | Tag(video) |  PreviousTagSize | Tag(audio) | .... | PreviewTagSize
```

- `tag `可以分成三类: `audio`, `video`, `script(data)`，分别代表音频流，视频流，脚本流
- 而每个 `tag `又由` tag header `和 `tag data` 组成
  -  `Video Tag` 和 `Audio Tag` 由 `Tag header` 和` Tag Data` 组成；`Script Tag` 只有 `Tag Data`
- `Previous Tag Size`占用4字节，记录前一个`tag`的大小，用于逆向读取
  - `FLV Header`后的一个`PreviewsTagSize`为`0`
- FLV 数据以大端序进行存储，在解析时需要注意



<img src="https://i-blog.csdnimg.cn/blog_migrate/76b84bb654aba677aa6edd8f39f813c8.jpeg#pic_center" alt="在这里插入图片描述" style="zoom: 67%;" />



<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpenv3HXib4viaRRDsbYaW8dCbVT1cLzsySyNS5uzLYQzZTj79kYKPY08Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 80%;" />





```c


typedef struct avc_video_tag {
    uint8_t avc_packet_type; // 0x00 - AVC sequence header, 0x01 - AVC NALU
    uint32_t composition_time;
    uint32_t nalu_len;
    void *data;
}   avc_video_tag_t;
```



### FLV Header

​		`9` 个字节，用来标识文件类型为 FLV 类型，以及后续的音视频标识。

​		`flv`文件最多只有一个音频流，一个视频流，不存在多个独立的音视频流在一个文件的情况。每种`tag`都属于一个流。

```c
/*
 * @brief flv file header 9 bytes
 */
typedef struct flv_header {
    uint8_t signature[3];
    uint8_t version;
    uint8_t type_flags; // 5bit reserved / 1bit audio / 1bit reserved / 1bit video
    uint32_t data_offset; // header size, always 9
} __attribute__((__packed__)) flv_header_t;
```

- 签名：
  - `8 bit`，`F(0x46)`
  - `8 bit`，`L(0x4c)`
  - `8 bit`，`V(0x56)`

- 版本：`8 bit`，FLV的版本
  - `0x01` 表示版本为1
- 类型
  - 保留字段：`5 bit`，`0 0000`
  - 音频标识，`1 bit`，是否存在音频流
    - `1` 存在
    - `0`不存在

  - 保留字段：`1 bit`，`0`
  - 视频表示：`1 bit`，是否存在视频流
    - `1` 存在
    - `0`不存在

- 文件头大小：`32 bit`
  - 版本为1是填`9`，表明`FLV`头的大小
  - 后期`FLV`拓展时使用，

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



#### `PreviousTagSize`

​		4个字节，记录前一个 `Tag `的大小，用于逆向读取处理。



#### Tag

​		`Tag `由 `Tag Header `和` Tag Body` 组成:

- `Tag Header `占 `11 `字节
- `Tag Body `一般分为三种类型：脚本（Script）数据、视频数据、音频数据
  - `Video Tag `和` Audio Tag` 由 `Tag header` 和 `Tag Data` 组成
  - `Script Tag` 只有 `Tag Data`

```c
typedef struct {
    // general tag header
    UB[2] Reserved; // 用于 FMS 的保留字段, 值为 0
    UB[1] Filter; // 指示 packet 是否需要预处理。0 = 不需要预处理。1 = packet 在渲染前需要预处理。未加密文件中此值为0，加密文件中此值为1。
    UB[5] TagType; // 8 = 音频，9 = 视频，18 = 脚本数据
    UI24 DataSize; // tag data 的长度。等于 tag 总长度 – 11
    UI24 Timestamp; // 当前tag的解码时间戳 (DTS)，单位是ms。第一个tag的DTS总是0
    UI8 TimestampExtended; // 和Timestamp字段一起构成一个32位值, 此字段为高8位。单位是ms
    UI24 StreamID; // 总为 0
    
    // video / audio tag header
    // script have not tag header
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

​		存放当前 `tag `的类型、数据区长度、时间戳等信息。`tag header `一般占 11 个字节。

```c
/*
 * @brief flv tag general header 11 bytes
 */
typedef struct flv_tag {
    uint8_t tag_type;
    uint32_t data_size; 
    uint32_t timestamp; 
    uint8_t timestamp_ext;
    uint32_t stream_id; // ? 24 bit
    void *data; // will point to an audio_tag or video_tag
}   flv_tag_t;
```

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

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpt1aQmuYaQS9g0mTWUkRSElTNJI0g7bnPSDicPyBKfiaIzkdttT95icP9A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

###### `audio tag header`

​	`1 byte`，包含了音频数据的参数信息

```c
typedef struct {
    UB [4] SoundFormat;
    UB [2] SoundRate;
    UB [1] SoundSize;
    UB [1] SoundType;
  IF SoundFormat == 10
    UI8 AACPacketType;
} AudioTagHeader;
```

```c
typedef struct audio_tag {
    uint8_t sound_format; // 0 - raw, 1 - ADPCM, 2 - MP3, 4 - Nellymoser 16 KHz mono, 5 - Nellymoser 8 KHz mono, 10 - AAC, 11 - Speex
    uint8_t sound_rate; // 0 - 5.5 KHz, 1 - 11 KHz, 2 - 22 KHz, 3 - 44 KHz
    uint8_t sound_size; // 0 - 8 bit, 1 - 16 bit
    uint8_t sound_type; // 0 - mono, 1 - stereo
    void *data;
}   audio_tag_t;
```

- `4 bit`，音频格式

  - `0`，按创建文件的平台的字节序顺序存储 16 位 PCM 样本

  - `2`，MP3

  - `3`， Linear PCM little endian，存储原始 PCM 样本。

    - 如果数据为 8 位，则样本为无符号字节。
    - 如果数据是 16 位，则样本存储为小端、有符号数字。
    - 如果数据是立体声，则左右样本交错存储：左-右-左-右-等等。

  - `4`，Nellymoser 16 kHz 

  - `5`，Nellymoser 8 kHz

  - `6`，Nellymoser

    >  SoundRate 字段不能表示 8 kHz 和 16 kHz 采样率。当在 SoundFormat 中指定 Nellymoser 8 kHz  或 Nellymoser 16 kHz 时，Flash Player 将忽略 SoundRate 和 SoundType 字段。对于其他 Nellymoser 采样率，请指定正常的 Nellymoser SoundFormat 并照常使用 SoundRate 和 SoundType 字段。

  - `7/8`，G711

  - `10`，AAC

    > SoundType 应为 1 （立体声），SoundRate 应为 3（44kHz）。这并不意味着 FLV 中的 AAC 音频总是立体声、44kHz。
    >
    > Flash Player会忽略 SoundType 和 SoundRate，而是根据 AAC Audio Data 中的 AAC sequence header 提取通道（单/双通道）和采样率。

  - `11`，Speex，音频是以 16 kHz 采样的压缩单声道，SoundRate 应为 0，SoundSize 应为 1，SoundType 应为 0 

- `2 bit`，采样率

  - `3`，`44-kHz`

- `1 bit`，采样精度

  - `0`，`snd 8bit`
  - `1`，`snd 16bit`

  > 压缩过的音频都是16bit

- `1 bit`，音频声道

  - `0`， `sndMono` 单声道
  - `1` `，sndStereo` 立体声，双声道

-  `AAC `格式下，`AudioTagHeader `中会多出一个字节 `AACPacketType`

  - 表示 `AACAUDIODATA `的类型：`0 = AAC sequence header`，`1 = AAC raw`
    - `AAC sequence header` 也就是包含了 `AudioSpecificConfig`，里面有一些更加详细音频的信息。
    - `AAC raw` 这种包含的就是音频 `ES `流了，也就是音频负载（`audio payload`）
  - 当 `SoundFormat `是 `AAC `时，`SoundType `需要设置为 1（立体声），`SoundRate `需要设置为 4（44k Hz）
    - 但这并不说明文件中 `AAC `编码的音频必须是 `44k Hz` 的立体声
    - 播放器在处理 `AAC `音频时，需要忽略 `AudioTagHeader `中的音频参数
    - 使用 `AudioSpecificConfig `的参数来初始化解码器
  
  

###### `audio tag body`

​		音频数据

```c
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
    AudioSpecificConfig; // AAC sequence header
  ELSE IF AACPacketType == 1
    Raw AAC frame data in UI8;
} AACAUDIODATA;
```

- 如果采用了加密，`Body `的类型是 `EncryptedBody`

  - 参考规范文档“附件 F. FLV 加密”章节
- 如果未采用加密，则 `Body `的类型是 `AudioTagBody`

<img src="https://i-blog.csdnimg.cn/blog_migrate/3d87a9b37c2ded2c694adfe38c66f95a.png#pic_center" alt="在这里插入图片描述" style="zoom: 67%;" />

###### `AAC sequence header`

​		在 FLV 的文件中，一般情况下 `AudioSpecificConfig `只会出现一次，即第一个` Audio Tag`。

​		如果音频使用 `AAC`，那么这个 `Tag `就是 `AAC sequence header`，即 `AAC `音频同步包

`AudioSpecificConfig `的结构在 `ISO/IEC-14496-3 Audio` 标准中有做说明

```c

AudioSpecificConfig() {
    audioObjectType = GetAudioObjectType();
    samplingFrequencyIndex; // 4 bslbf
    if (samplingFrequencyIndex == 0xf) {
        samplingFrequency; // 24 uimsbf
    }
    channelConfiguration; // 4 bslbf
    sbrPresentFlag = -1;
    psPresentFlag = -1;
    if (audioObjectType == 5 || audioObjectType == 29) {
        // ...
    }
    else {
        extensionAudioObjectType = 0;
    }
    switch (audioObjectType) {
    case 1: case 2: case 3: case 4: //...
        GASpecificConfig();
        break:
    case ...:
        //...
    }
    if (extensionAudioObjectType != 5 && bits_to_decode() >= 16) {
        //...
    }
    // ...

GetAudioObjectType() {
    audioObjectType; // 5 uimsbf
    if (audioObjectType == 31) {
        audioObjectType = 32 + audioObjectTypeExt; // 6 uimsbf
    }
    return audioObjectType;
}
```

下面是 `ISO/IEC 14496-3, 1.5.1.1 Audio object type definition`：

| ID   | Type     |
| ---- | -------- |
| 0    | Null     |
| 1    | AAC main |
| 2    | AAC LC   |
| 3    | AAC SSR  |
| 4    | AAC LTP  |
| 5    | SSR      |
| …    | …        |

下面是 `ISO/IEC 14496-3, 1.6.3.4 samplingFrequencyIndex`：

| index | value        |
| ----- | ------------ |
| 0x0   | 96000        |
| 0x1   | 88200        |
| 0x2   | 64000        |
| 0x3   | 48000        |
| 0x4   | 44100        |
| 0x5   | 32000        |
| 0x6   | 24000        |
| 0x7   | 22050        |
| 0x8   | 16000        |
| 0x9   | 12000        |
| 0xa   | 11025        |
| 0xb   | 8000         |
| 0xc   | 7350         |
| 0xd   | reserved     |
| 0xe   | reserved     |
| 0xf   | escape value |



​		我们常用的 `AAC `音频同步包的大小固定为 4 字节

- 前两个字节被称为 `AACDecoderSpecificInfo`，用于描述这个音频包应当如何被解析
- 后两个字节称为 `AudioSpecificConfig`，更加详细的指定了音频格式

​		下图是一个 AAC 音频同步包的示例：

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrp1ibpbd9uXUHWAlX5fKvvUhagLz7XxPibFXd2Iib9zh3bSQJibog4FkLKng/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 67%;" />



###### `AAC raw data`

​		完成 AAC 音频同步包的发送后，可以推送普通的 AAC 数据包了。

​		在发送数据包时，`AACDecoderSpecificInfo` 则变为 `0xAF01`，说明这个包是普通 AAC 数据包。

> **如果这里的 AAC 数据有包含 7 个字节 ADTS 头（若存在 CRC 校验，则是 9 个字节），那么要去掉这个头后，把裸数据放到这里。如果这里是采集到的裸数据，没有 ADTS 头，那么这里就不需要这样处理了。**
>
> 即，去掉AAC的封装，转换位FLV的封装，只保留AAC的编码。

​		下图是一个 AAC 音频数据包的示例：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpPaiafRdNRpKts3FsqqXlkDicAA43NnF8Zib3obHoPTdgrkZlx0RFok7ug/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

> **在解析 FLV 时，如果封装的是 AAC 的音频，要在每帧 AAC ES 流前把 7 个字节 ADTS 头添加回来，这是因为 ADTS 是解码器通用的格式，纯的 AAC ES 流要打包成 ADTS 格式的 AAC 文件，解码器才能正常解码。**
>
> ​		在打包 `ADTS `的时候，需要用到 `samplingFrequencyIndex `这个信息，`samplingFrequencyIndex `最准确的信息是存储在 `AudioSpecificConfig `中。
>
> ​		有关 `AudioSpecificConfig `结构解析的代码，可以参考 `ffmpeg/libavcodec/mpeg4audio.c` 中的 `avpriv_mpeg4audio_get_config` 函数。
>
> ```c
> 
> int avpriv_mpeg4audio_get_config(MPEG4AudioConfig *c, const uint8_t *buf, int bit_size, int sync_extension)
> {
>     GetBitContext gb;
>     int ret;
> 
>     if (bit_size <= 0)
>         return AVERROR_INVALIDDATA;
> 
>     ret = init_get_bits(&gb, buf, bit_size);
>     if (ret < 0)
>         return ret;
> 
>     return ff_mpeg4audio_get_config_gb(c, &gb, sync_extension);
> }
> 
> int ff_mpeg4audio_get_config_gb(MPEG4AudioConfig *c, GetBitContext *gb, int sync_extension)
> {
>     int specific_config_bitindex, ret;
>     int start_bit_index = get_bits_count(gb);
>     c->object_type = get_object_type(gb);
>     c->sample_rate = get_sample_rate(gb, &c->sampling_index);
>     c->chan_config = get_bits(gb, 4);
>     if (c->chan_config < FF_ARRAY_ELEMS(ff_mpeg4audio_channels))
>         c->channels = ff_mpeg4audio_channels[c->chan_config];
>     c->sbr = -1;
>     c->ps  = -1;
>     if (c->object_type == AOT_SBR || (c->object_type == AOT_PS &&
>         // check for W6132 Annex YYYY draft MP3onMP4
>         !(show_bits(gb, 3) & 0x03 && !(show_bits(gb, 9) & 0x3F)))) {
>         if (c->object_type == AOT_PS)
>             c->ps = 1;
>         c->ext_object_type = AOT_SBR;
>         c->sbr = 1;
>         c->ext_sample_rate = get_sample_rate(gb, &c->ext_sampling_index);
>         c->object_type = get_object_type(gb);
>         if (c->object_type == AOT_ER_BSAC)
>             c->ext_chan_config = get_bits(gb, 4);
>     } else {
>         c->ext_object_type = AOT_NULL;
>         c->ext_sample_rate = 0;
>     }
>     specific_config_bitindex = get_bits_count(gb);
> 
>     if (c->object_type == AOT_ALS) {
>         skip_bits(gb, 5);
>         if (show_bits_long(gb, 24) != MKBETAG('\0','A','L','S'))
>             skip_bits_long(gb, 24);
> 
>         specific_config_bitindex = get_bits_count(gb);
> 
>         ret = parse_config_ALS(gb, c);
>         if (ret < 0)
>             return ret;
>     }
> 
>     if (c->ext_object_type != AOT_SBR && sync_extension) {
>         while (get_bits_left(gb) > 15) {
>             if (show_bits(gb, 11) == 0x2b7) { // sync extension
>                 get_bits(gb, 11);
>                 c->ext_object_type = get_object_type(gb);
>                 if (c->ext_object_type == AOT_SBR && (c->sbr = get_bits1(gb)) == 1) {
>                     c->ext_sample_rate = get_sample_rate(gb, &c->ext_sampling_index);
>                     if (c->ext_sample_rate == c->sample_rate)
>                         c->sbr = -1;
>                 }
>                 if (get_bits_left(gb) > 11 && get_bits(gb, 11) == 0x548)
>                     c->ps = get_bits1(gb);
>                 break;
>             } else
>                 get_bits1(gb); // skip 1 bit
>         }
>     }
> 
>     //PS requires SBR
>     if (!c->sbr)
>         c->ps = 0;
>     //Limit implicit PS to the HE-AACv2 Profile
>     if ((c->ps == -1 && c->object_type != AOT_AAC_LC) || c->channels & ~0x01)
>         c->ps = 0;
> 
>     return specific_config_bitindex - start_bit_index;
> }
> ```

##### video tag

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpnewcOWSo4rs81vNrsHop05ojpvXW9bs4pryf1iaVoRKEHxjMCGLLicng/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 80%;" />

###### `video tag header`

```c
typedef struct video_tag {
    uint8_t frame_type;
    uint8_t codec_id;
    void *data;
}   video_tag_t;
```

- `FrameType`：`4 bit`，帧类型

  > FrameType == 5时，是视频信息或指令帧，否则是编码数据帧，按`CodecID`解析具体编码

- `CodecID`：`4 bit`， 编码类型
  - 2，则 `VideoTagBody `为 `H263 Video Packet`
  - 3，则 `VideoTagBody `为 `Screen Video Packet`
  - 4，则 `VideoTagBody `为 `VP6 FLV Video Packet`
  - 5，则 `VideoTagBody `为 `VP6 FLV Alpha Video Packet`
  - 6，则 `VideoTagBody `为 `Screen V2 Video Packet`
  - 7，则 `VideoTagBody `为 `AVC Video Packet`

- `AVC`格式下，`VideoTagHeader `会多出 4 个字节的信息

  - `AVCPacketType`，1 `byte `
    
    - `0`，` AVC sequence header`
    
      - `tag body`数据是`AVCDecoderConfigurationRecord`，**记录的是 AVC（H.264）的 sps 和 pps 信息**
    
        > **解码器在解码数据之前需要首先获取的 sps 和 pps 的信息。在做 seek 或者断流重连等操作引起解码器重启时，也需要给解码器再传一遍 sps 和 pps 信息。**
    
    - `1`，` AVC NALU`
    
      - `AVCPacket (tag body)` 包含 `1`个 或 多个 `NALU`
    
        > 一个 `AVCPacket `应该包含一个 `frame`，但是在` H.264 `的 `VCL `中可能会将 `frame `分成 `slice`，每个 `slice `作为 `RBSP `被封装在单独的 `NALU `中
        >
        > 因此一个 `AVCPacket `包含一个 `frame `时可能需要包含多个 `NALU `（即多个 `slice`）
    
    - `2`，`AVC end of sequence `

  - `CompositionTime`，`3 byte`，`CTS`
    - `AVCPacketType == 0`，时间戳偏移
    - `AVCPacketType == 1`，` 0`


> 所以 H.264 编码的情况下 `VideoTagHeader `长度是 5 个字节

###### `video tag body`

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



###### `AVC sequence header`

​		**在 FLV 的文件中，一般情况下 `AVCDecoderConfigurationRecord `只会出现一次，即第一个 Video Tag。**

​		**如果视频使用 `AVC`，那么这个 `Tag `就是 `AVC sequence header`，即 AVC 视频同步包。**

​		AVCDecoderConfigurationRecord 结构的在 `ISO/IEC-14496-15 AVC file format` 标准中有做说明。

​		下面是 `ISO/IEC 14496-15, 5.3.3.1.2 AVCDecoderConfigurationRecord`：

```c
aligned(8) class AVCDecoderConfigurationRecord {
    unsigned int(8) configurationVersion = 1;
    unsigned int(8) AVCProfileIndication;
    unsigned int(8) profile_compatibility;
    unsigned int(8) AVCLevelIndication; 
    bit(6) reserved = '111111'b;
    unsigned int(2) lengthSizeMinusOne; 
    bit(3) reserved = '111'b;
    unsigned int(5) numOfSequenceParameterSets;
    for (i = 0; i < numOfSequenceParameterSets; i++) {
        unsigned int(16) sequenceParameterSetLength ;
        bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit;
    }
    unsigned int(8) numOfPictureParameterSets;
    for (i = 0; i < numOfPictureParameterSets; i++) {
        unsigned int(16) pictureParameterSetLength;
        bit(8*pictureParameterSetLength) pictureParameterSetNALUnit;
    }
    if (profile_idc == 100 || profile_idc == 110 ||
        profile_idc == 122 || profile_idc == 144)
    {
        bit(6) reserved = '111111'b;
        unsigned int(2) chroma_format;
        bit(5) reserved = '11111'b;
        unsigned int(3) bit_depth_luma_minus8;
        bit(5) reserved = '11111'b;
        unsigned int(3) bit_depth_chroma_minus8;
        unsigned int(8) numOfSequenceParameterSetExt;
        for (i = 0; i < numOfSequenceParameterSetExt; i++) {
            unsigned int(16) sequenceParameterSetExtLength;
            bit(8*sequenceParameterSetExtLength) sequenceParameterSetExtNALUnit;
        }
    }
}
```

下面是 `ITU-T H.264(ISO/IEC 14496-10), A.2 Profiles`：

| idc  | Profile               |
| ---- | --------------------- |
| 66   | Baseline              |
| 77   | Main                  |
| 88   | Extended              |
| 100  | High                  |
| 110  | High 10               |
| 122  | High 4:2:2            |
| 144  | (High 4:4:4; removed) |
| 244  | High 4:4:4 Predictive |

下面是 `ITU-T H.264(ISO/IEC 14496-10), A.3 Levels`：

```c
Defined level_idc: 10, 9[^1], 11, 12, 13, 20, 21, 22, 30, 31, 32, 40, 41, 42, 50, 51, 52

[^1]: "level_idc==9" represents "Leve 1b" unless Baseline(Constrained Baseline), Main, or Extended profiles.
```

下面是 `ISO/IEC 14496-15, 8.3.3.1.2 HEVCDecoderConfigurationRecord`：

```c
aligned(8) class HEVCDecoderConfigurationRecord {
    unsigned int(8) configurationVersion = 1;
    unsigned int(2) general_profile_space;
    unsigned int(1) general_tier_flag;
    unsigned int(5) general_profile_idc;
    unsigned int(32) general_profile_compatibility_flags;
    unsigned int(48) general_constraint_indicator_flags;
    unsigned int(8) general_level_idc;
    bit(4) reserved = ‘1111’b;
    unsigned int(12) min_spatial_segmentation_idc;
    bit(6) reserved = ‘111111’b;
    unsigned int(2) parallelismType;
    bit(6) reserved = ‘111111’b;
    unsigned int(2) chromaFormat;
    bit(5) reserved = ‘11111’b;
    unsigned int(3) bitDepthLumaMinus8;
    bit(5) reserved = ‘11111’b;
    unsigned int(3) bitDepthChromaMinus8;
    bit(16) avgFrameRate;
    bit(2) constantFrameRate;
    bit(3) numTemporalLayers;
    bit(1) temporalIdNested;
    unsigned int(2) lengthSizeMinusOne; 
    unsigned int(8) numOfArrays;
    for (j=0; j < numOfArrays; j++) {
        bit(1) array_completeness;
        unsigned int(1) reserved = 0;
        unsigned int(6) NAL_unit_type;
        unsigned int(16) numNalus;
        for (i=0; i< numNalus; i++) {
            unsigned int(16) nalUnitLength;
            bit(8*nalUnitLength) nalUnit;
        }
    }
}
```

下面是 `ITU-T H.265(ISO/IEC 23008-2), A.3 Profiles`：

| idc  | Profile            |
| ---- | ------------------ |
| 1    | Main               |
| 2    | Main 10            |
| 3    | Main Still Picture |

下面是 `ITU-T H.265(ISO/IEC 23008-2), A.4 Tiers and levels`：

| flag | Tier |
| ---- | ---- |
| 0    | Main |
| 1    | High |

| idc  | Level |
| ---- | ----- |
| 30   | 1     |
| 60   | 2     |
| 63   | 2.1   |
| 90   | 3     |
| 93   | 3.1   |
| 120  | 4     |
| 123  | 4.1   |
| 150  | 5     |
| 153  | 5.1   |
| 156  | 5.2   |
| 180  | 6     |
| 183  | 6.1   |
| 186  | 6.2   |

下图是一个 AVC 视频同步包的示例，其中红框部分对应的是 VIDEODATA：

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpS25tAa2PjFtwPQtibChia0RVmEX7jib8hPou3qWINQt2MibLNvKPfqgwFA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 67%;" />

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpMrCwoGzWUDI4GN4Fqc4pWUDPkLjSbwicT15bwUvU5haAFMKoZF5gxcw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:67%;" />



###### `AVC raw data`

下图是一个 AVC 视频数据包的示例，其中红框部分对应的是 VIDEODATA：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpc5excMC28RiaaNAHib7G372oszFrXDbzJ8SwzXg5IF0k3Bp0jEYficZEw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrplAEXMI68bjCtYAt0CnzqCZU3Z3jDpZ8AKphcY61lvTkMBCnQxYE01Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

​	有关 `AVCDecoderConfigurationRecord `结构解析的代码，可以参考 `ffmpeg/libavformat/avc.c` 中的 `ff_isom_write_avcc` 函数。

```c
int ff_isom_write_avcc(AVIOContext *pb, const uint8_t *data, int len)
{
    if (len > 6) {
        /* check for H.264 start code */
        if (AV_RB32(data) == 0x00000001 ||
            AV_RB24(data) == 0x000001) {
            uint8_t *buf=NULL, *end, *start;
            uint32_t sps_size=0, pps_size=0;
            uint8_t *sps=0, *pps=0;

            int ret = ff_avc_parse_nal_units_buf(data, &buf, &len);
            if (ret < 0)
                return ret;
            start = buf;
            end = buf + len;

            /* look for sps and pps */
            while (end - buf > 4) {
                uint32_t size;
                uint8_t nal_type;
                size = FFMIN(AV_RB32(buf), end - buf - 4);
                buf += 4;
                nal_type = buf[0] & 0x1f;

                if (nal_type == 7) { /* SPS */
                    sps = buf;
                    sps_size = size;
                } else if (nal_type == 8) { /* PPS */
                    pps = buf;
                    pps_size = size;
                }

                buf += size;
            }

            if (!sps || !pps || sps_size < 4 || sps_size > UINT16_MAX || pps_size > UINT16_MAX)
                return AVERROR_INVALIDDATA;

            avio_w8(pb, 1); /* version */
            avio_w8(pb, sps[1]); /* profile */
            avio_w8(pb, sps[2]); /* profile compat */
            avio_w8(pb, sps[3]); /* level */
            avio_w8(pb, 0xff); /* 6 bits reserved (111111) + 2 bits nal size length - 1 (11) */
            avio_w8(pb, 0xe1); /* 3 bits reserved (111) + 5 bits number of sps (00001) */

            avio_wb16(pb, sps_size);
            avio_write(pb, sps, sps_size);
            avio_w8(pb, 1); /* number of pps */
            avio_wb16(pb, pps_size);
            avio_write(pb, pps, pps_size);
            av_free(start);
        } else {
            avio_write(pb, data, len);
        }
    }
    return 0;
}
```



##### script tag

​		又通常被称为 `metadata tag`，会放一些关于`FLV`视频和音频的元数据信息如：`duration`、`width`、`height`等。

​		`AMF1("onMetaData") | AMF2("wdith, height...")`

- 通常该类型 `tag `会跟在` FLV header `后面作为第一个 `tag `出现，而且只有一个
- 没有` script tag header`，只有`script tag body`
- 一般来说，`script tag `包含两个 `AMF `包

  > AMF 是一种紧凑二进制格式，用于序列化 ActionScript 对象图。

​	![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpSmPlLEbic2TXqAxaqjUEDv9d7foIxIsqSerYyxhu1iajukl74VAlG4sw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

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
// ScriptTagBody 包含以 AMF 编码的 SCRIPTDATA。


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

- `ScriptTagBody `中的 `Name `和 `Value `字段都是 `SCRIPTDATAVALUE `类型
  - `Name `最终对应的是 `String `类型
  - `Value `最终对应的是 `ECMA array `类型。
- `SCRIPTDATAVALUE `的两个字段
  - `Type `是类型，定义数据的类型
  - `ScriptDataValue `是数据值
    - `Type `的值确定 `ScriptDataValue `的类型
  - 这些数据都是以 `AMF(Action Message Format) `的形式编码。

> ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/0c00bc24955dbfa9e495d72fba201a02.png#pic_center)



###### 第一个 AMF 包

​	封装字符串类型数据

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
  
  

###### 第二个 AMF 包

​		封装一个数组类型，这个数组中包含了音视频信息项的名称和值

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
>
> ​	[winshining](https://links.jianshu.com/go?to=https%3A%2F%2Fgithub.com%2Fwinshining)/**[nginx-http-flv-module](https://links.jianshu.com/go?to=https%3A%2F%2Fgithub.com%2Fwinshining%2Fnginx-http-flv-module)** 的 meta 
>
> ```cpp
> 
> static ngx_rtmp_amf_elt_t       out_inf[] = {
> 
>     { NGX_RTMP_AMF_STRING,
>      ngx_string("Server"),
>      "NGINX HTTP-FLV (https://github.com/winshining/nginx-http-flv-module)", 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("width"),
>      &v.width, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("height"),
>      &v.height, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("displayWidth"),
>      &v.width, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("displayHeight"),
>      &v.height, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("duration"),
>      &v.duration, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("framerate"),
>      &v.frame_rate, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("fps"),
>      &v.frame_rate, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("videodatarate"),
>      &v.video_data_rate, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("videocodecid"),
>      &v.video_codec_id, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("audiodatarate"),
>      &v.audio_data_rate, 0 },
> 
>     { NGX_RTMP_AMF_NUMBER,
>      ngx_string("audiocodecid"),
>      &v.audio_codec_id, 0 },
> 
>     { NGX_RTMP_AMF_STRING,
>      ngx_string("profile"),
>      &v.profile, sizeof(v.profile) },
> 
>     { NGX_RTMP_AMF_STRING,
>      ngx_string("level"),
>      &v.level, sizeof(v.level) },
> };
> 
> static ngx_rtmp_amf_elt_t       out_elts[] = {
> 
>     { NGX_RTMP_AMF_STRING,
>      ngx_null_string,
>      "onMetaData", 0 },
> 
>     { NGX_RTMP_AMF_OBJECT,
>      ngx_null_string,
>      out_inf, sizeof(out_inf) },
> };
> ```



###### onMetaData 对象

​		`Script Tag` 里可以承载不同的数据，其中我们最关心是的音视频的 `metadata `元信息数据，这些信息是以一个 `Name `为 `onMetadata` 的 `SCRIPTDATA Tag `来存储的。

- `onMetaData `标签通常会成为` FLV Body `中的第一个标签，紧跟在 `FLV Header` 之后。
  - `onMetaData `标签中存储的是一些视频、音频及文件相关的元数据信息
  - 如视频帧率，音频采样率、文件长度等。
- `onMetaData `标签的 `Name `字段主要存储 “`onMetaData`” 字符串
  - 第 `1 `个字节值是 `0x02`，表示 `Name `字段是字符串类型
  - 第 `2-3 `个字节为 `UI16 `类型值，标识字符串的长度，值为 `0x000A` (“`onMetaData`” 这个字符串的长度)
  - 后面跟着的数据为具体的字符串，值为 “`onMetaData`”。
- `onMetaData `标签的 `Value `字段存储上表所示的各属性键值对。
  - 第` 1 `个字节值是 `0x08`，表示 `Value `字段是数组类型
  - 第` 2-5 `个字节为UI32类型值，表示数组元素个数
  - 后面紧跟着数组，数组元素为属性名称和值组成的对（键值对）
  - 最后是数组的结束符

​		`onMetadata `包含着不同的属性，这些属性对于不同的 `FLV `文件可能各不相同。

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpHyUPZKbfVItzduRsPBvibwTejw5lexLRe9WQgmA1wvuF6KCG1RaauUg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)



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

- `PropertyName `的类型是 `SCRIPTDATASTRING`，第一个参数是字符串长度，第二个参数是字符串。
- `PropertyData `的类型是 `SCRIPTDATAVALUE`，第一个参数是 `Type`，为` 0` 时，第二个参数是一个 `Double `类型的变量。



## 工具

flvAnalyser是一款FLV分析工具。

下载地址：https://github.com/zymill/flvAnalyser



## parse

详见于：[最简单的 FLV 封装格式解析程序](https://blog.csdn.net/ProgramNovice/article/details/137507298)