# AAC

---

​		AAC（Advanced Audio Coding）是一种现代的音频编码技术，用于数字音频的传输和存储领域。AAC是MPEG-2和MPEG-4标准中的一部分，可提供更高质量的音频数据。

​		AAC比MP3的压缩率更高，压缩后的文件越小，二是保真性比MP3强。ACC最开始是基于MPEG-2的音频编码技术，MPEG-4标准出现后，AAC重新集成了其特性，加入了SBR（Spectral Band Replication， 频段重现）和PS（Parametric Stereo，参数化）技术。



## 特点

1. 更好的声音质量和更低的压缩比，减少了由于压缩而引入的失真和噪音。
2. 支持多种采样率、声道数和比特率。
3. 具有很好的灵活性和可扩展性。



## 编码规格

​		通过附加的编码技术（PS，SBR等），衍生出了

- LC-AAC （Low Complexity）

  ​	低复杂度规则，码流128K，音质好。主要应用于中高码率场景的编码（>80Kbit/s)

- HE-AAC （v1）

  ​	按频谱分别保存，扶贫编码保存主要部分，高频单独放大编码保存音质，码流64K左右。

  ​	AAC + SBR， 主要应用于中低码率场景下的编码（<=80Kbit/s) 

- HE-AAC v2

  ​	双声道中的生意你存在某种相似性，只需存储一个声道的全部信息，然后，花很少的字节用参数描述另一个声道和它的差异。

  ​	AAC + SBR + PS，主要应用于低码率场景的编码(<=48Kbit/s)


![image-20240225193009769](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225193009769.png)

​		大部分编码器都设置为<=48Kbit/s自动启用PS技术，而>48Kbit/s则不加PS。

- 小于128Kbit/s的码率下表现优异，并且多用于视频中的音频编码

- 适用于128Kbit/s以下的音频编码，多用于视频总的音频轨的编码

| 名称     | 技术       | 码率           |
| -------- | ---------- | -------------- |
| LC-AAC   | 传统AAC    | 高码率>=80Kbps |
| HE-AAC   | AAC+SBR    | 中码率<=80Kbps |
| HE_AACv2 | AAC+SBR+PS | 低码率<=48Kbps |



### SBR 频带重现

​		SBR是一种全新的音频编码增强工具，在ISO/IEC 14496-3:2001/Amd.1:2003中被标准化。通过增加在特定比特率的音频带宽或改善在特定质量水平的编码效率，提供了改善低比特率音频和音频编码音质的可能性。

​		SBR可以扩大传统感知编码在低比特率下所能提供的有限的音频带宽，因此它的性能表现相当于或超过模拟FM的音频带宽（15kHz)。SBR也可以改善窄带音频编码的质量，可为广播电台提供12kHz音频带宽的纯音频频道，可用如诸如多语言广播等场合。

​		由于大多数音频编码都是被严格地限制带宽的，因此SBR的重要性不仅体现在提高音频质量上，而且也体现在提高音频的可读性和理解性上。SBR以**后处理**为主的，不过为能指导解码过程，在编码时编码器要先做一些预处理工作。

​		当联合SBR一起使用，其主体编码器（比如MP3）只负责处理音频频谱的低频部分。高频部分则由SBR解码器产生，这是紧跟在传统波形解码器之后的一个后处理过程。SBR基于对主体编码器处理得到的低频信号进行的分析，在解码器上重建了高频部分。为了确保精确的重建，一些指导信息以非常低的数据率夹杂在主体编码器产生的码流中一同传输。

​		重建对于和声和噪声成分同样有效，同时允许在频率范围和时间范围进行适当的修整。结果是，SBR实现了在非常低数据率下的全带宽音频编码，与主体编码器相比，明显地增加了压缩效率。

​		SBR的性能分析：

​		SBR可以改善感知音频编码的效率，在中等至低比特率下能提高大约30%（在某些特定情况下可能更高）。SBR具体能够提供的改善程度同时也依赖于其主体编码器。

​		举例说，联合MP3一起使用SBR（参考mp3PRO相关文章），我们可以在64kbps下达到相对传统的大于100kbps的立体声mp3的音频质量。SBR可应用于单声道，立体声甚至多声道的音频。

​		因此，可以说，SBR在主体编码器本身可编码的音频信号比特率范围和在有限的音频带宽下的可接受的编码噪音（coding artifacts）水平内提供了最大的效率。



### PS	参数立体声

​		在MPEG-4中被完整地定义，并且是aacPlus v2中的新的组成部分。为提高低比特率立体声信号的音频压缩效率。

​		参量立体声编码器从音频信号的立体声影像中分解出其参量表示，而在传统模式下原始信号只会被编码为单声道表示。立体声映像信息被表现为少量的高质量的参量立体声信息，与单声道信号在比特流中同时传输。然后，基于接收到的参量立体声信息，解码器便可重建立体声映像

​		合并有参量立体声的低比特率， 例如 24 kbps 的音频比特流， 所能让听众感受到的音频质量是显著地高于不具备参量立体声的相似音频比特流的质量。



## 编码格式

​		AAC 有两种封装格式：ADIF，ADTS

- `ADIF（Audio Data Interchange Format）`，音频数据交换格式
  - 只有一个文件头部，存储用于音频解码播放的信息（例如采样率，通道数等）
  - 解码播放必须从文件头部开始，一般用于存储在本地磁盘中y音频文件的播放
- `ADTS（Audio Data Transport Stream）`，音频数据传输流
  - 可以将数据看做一个个的音频帧，而每帧都存储了用于音频解码播放的头信息（例如采样率，通道数等）
  - 多个音频帧构成一个音频序列
  - 可以从任何帧位置解码播放，更适用于流媒体传输

### ADIF 

​		![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/25cc8f265c27a1ac0655341c61db3815.png)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/fbdad59f57095b4bd84eab4365a90a05.png)

### ADTS

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3311dba799a441bda48618bab8811ba7.png)

​		ADTS 格式的 AAC 码流是由一个个的 ADTS Frame 组成.

> ​		一般的AAC解码器都需要把AAC的ES流打包成ADTS的格式，一般是在AAC ES流前添加7个字节的ADTS header。也就是说ADTS是AAC的frame header。

```c
 ...| ADTS Frame | ADTS Frame | ADTS Frame | ADTS Frame | ADTS Frame |...
```

​		每个 ADTS Frame 是由头部（固定头部+可变头部）和数据组成

```c
// ADTS Frame
```

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/527383512e8c48eba228bdc71bfe98ba.png)

### 固定头部

| 序号 | 字段名称                              | 长度 (bits) | 说明                                                         |
| ---- | ------------------------------------- | ----------- | ------------------------------------------------------------ |
| 1    | Syncword                              | 12          | ADTS文件的标志符，它用于确定音频帧的开始位置和结束位置，通常为`0xFFF`。 |
| 2    | MPEG version ID                       | 1           | 使用的MPEG版本<br />0 表示 MPEG-4，1 表示 MPEG-2             |
| 3    | Layer                                 | 2           | 所有位必须为0                                                |
| 4    | Protection Absent                     | 1           | 是否存在 CRC 校验<br />0 表示存在 CRC 校验字段；1 表示不存在 CRC 校验字段 |
| 5    | Profile                               | 2           | AAC规格<br />0 表示 AAC Main；1 表示 AAC LC；2 表示 AAC SSR；11 AAC LTP |
| 6    | MPEG-4 <br />Sampling Frequence Index | 4           | 采样率索引<br />0 表示 96000Hz，4 表示 44100Hz，11 表示 8000Hz<br />[详见此处](https://wiki.multimedia.cx/index.php/MPEG-4_Audio#Sampling_Frequencies) |
| 7    | Private Stream                        | 1           | 编码时将该值设为 0，解码时忽略                               |
| 8    | MPEG-4 Channel Configuration          | 3           | 通道数，指示音频的通道数，如单声道、立体声或多声道等<br />取值为0时，通过inband 的PCE设置channel configuration |
| 9    | Originality/copy                      | 1           | Originality指示编码数据是否被原始产生。<br />编码时将该值设置为 0，解码时忽略 |
| 10   | Home                                  | 1           | 编码时将该值设为 0，解码时忽略                               |

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20161028130919091)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/a54370ede8963b6c1025e5350d62c0b3.png)

### 可变头部

| 序号 | 字段名称             | 长度 (bits) | 说明                                                         |
| ---- | -------------------- | ----------- | ------------------------------------------------------------ |
| 11   | Copyrighted Stream   | 1           | 编码时将该值设为 0，解码时忽略                               |
| 12   | Copyrighted Start    | 1           | 编码时将该值设为 0，解码时忽略                               |
| 13   | Frame Length         | 13          | ADTS 帧长度，包括ADTS头部所占的长度和AAC ES长度，单位Byte    |
| 14   | Buffer Fullness      | 11          | 值为 0x7FF 时表示动态码率，0x000 表示固定码率的码流          |
| 15   | Number of AAC Frames | 2           | 值为 ADTS 帧里的 AAC 帧数量减一<br />为了最大的兼容性通常每个ADTS frame 包含一个AAC frame，所以该值一般为0。<br />一个AAC原始帧包含一段时间内1024个采样及相关数据 |

### CRC

| 序号 | 字段名称 | 长度 (bits) | 说明 |
| ---- | -------- | ----------- | ---- |
| 16   | CRC      | 16          | CRC  |



> [该网站](https://www.p23.nl/projects/aac-header/)提供了一个解析 AAC ADTS Frame Header 的工具，你可以输入头部 7 或 9 个字节的数据，点击 Submit 就能看到头部各字段对应的含义。
>
> [最简单的 AAC 音频码流解析程序_aac裸流结果-CSDN博客](https://blog.csdn.net/ProgramNovice/article/details/137225523)



### AAC ES

​		AAC ES（AAC Elementary Stream）是AAC音频编码的一种基本数据格式，也是AAC音频数据在流式传输和文件存储中的常见格式之一。

​		AAC ES不同于其他容器格式（如MP4、M4A等），它不包含额外的元数据或结构信息，仅包含未经任何封装或压缩处理的原始音频数据。这些原始数据可以作为音频文件或流传输的基础，同时也可以用于对AAC音频进行转码、编辑或重组。

​		AAC ES 通常由一系列连续的AAC音频帧组成，每个帧以一个特定的标志符开始，该标志符表示这是一个AAC音频帧。在AAC ES中，每个音频帧拥有相同的长度（1024个样本时间段），但是并不一定包含相同数量的采样点，因为采样率和声道数量可能会发生变化。

​		AAC ES 的另一个关键特征是其比特流顺序，即数字音频数据的组织方式。AAC ES 采用大端字节顺序，其中高位字节排在前面，低位字节排在后面。此外，在AAC ES中，音频数据按照从左到右、自上而下的顺序排列，与典型的文本文件不同。



```c
// 贴上ffmpeg中添加ADTS头的代码，就可以很清晰的了解ADTS头的结构：


int ff_adts_write_frame_header(ADTSContext *ctx, 
                               uint8_t *buf, int size, int pce_size) 
{ 
    PutBitContext pb; 
 
    init_put_bits(&pb, buf, ADTS_HEADER_SIZE); 
 
    /* adts_fixed_header */ 
    put_bits(&pb, 12, 0xfff);   /* syncword */ 
    put_bits(&pb, 1, 0);        /* ID */ 
    put_bits(&pb, 2, 0);        /* layer */ 
    put_bits(&pb, 1, 1);        /* protection_absent */ 
    put_bits(&pb, 2, ctx->objecttype); /* profile_objecttype */ 
    put_bits(&pb, 4, ctx->sample_rate_index); 
    put_bits(&pb, 1, 0);        /* private_bit */ 
    put_bits(&pb, 3, ctx->channel_conf); /* channel_configuration */ 
    put_bits(&pb, 1, 0);        /* original_copy */ 
    put_bits(&pb, 1, 0);        /* home */ 
 
    /* adts_variable_header */ 
    put_bits(&pb, 1, 0);        /* copyright_identification_bit */ 
    put_bits(&pb, 1, 0);        /* copyright_identification_start */ 
    put_bits(&pb, 13, ADTS_HEADER_SIZE + size + pce_size); /* aac_frame_length */ 
    put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */ 
    put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */ 
 
    flush_put_bits(&pb); 
 
    return 0; 
}
```





## 实例：AAC文件解析

下图是用编辑器打开一个AAC文件，用十六进制查看的截图：

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/c74101fcc75e4e9486f6631b655a2daf.png)

没有CRC的情况下，文件开头的7个字节是ADTS帧头部，这里7个字节的数据是：0xff 0xf1 0x4c 0x80 0x20 0x02 0x80，我们按照ADTS帧的头部数据来解析看看这7个字节表示什么?

前面1-12bit为 0xff 0xf，对应了ADTS头部的 syncword 字段，表示ADTS帧的开始。

13-16bit为0x01，二进制是 0001，也就是说：

ID的1bit为0(MPEG-4)；
layer的2bit为00；
protection_absent的1bit为1，表示没有CRC，整个头部7个字节。
17-24bit为0x4c，二进制是 0100 1100，意思是：

profile_ObjectType的2bit为 01，结合前面ID为0，表示MPEG-4 AAC LC 规格；
sampling_frequency_index的4bit为0011，也就是0x3，表示采样率为48000；
private_bit的1bit为0；
剩余1bit，结合后面的再看；

25-32bit为0x80，二进制是 1000 0000，意思是：

channel_configuration的3bit(结合前面剩下的1bit)为 010，表示双声道；
original_copy的1bit为0；
home的1bit为0；
copyright_identification_bit的1bit为0；
copyright_identification_start的1bit为0；
剩余2bit，结合下个字段再看；

33-44bit为0x200，二进制是 0010 0000 0000，加上前面剩下的2bit，就是00 0010 0000 0000：

frame_length的13bit为00 0010 0000 000，也就是0x100，表示这个ADTS帧的长度是0x100；那么下个ADTS就是0x100开始的；
剩余1bit，留到下个字段再看；

45-56bit为0x280，二进制是 0010 1000 0000，加上前面剩下的1bit，就是 0 0010 1000 0000：

adts_buffer_fullness的11bit为 0 0010 1000 00，十六进制0xa0；
number_of_raw_data_blocks_in_frame的2bit为 00，表示包含一个AAC frame。
后面的ADTS帧也可以类似上面的过程去解析。frame_length是代表了整个ADTS的大小。