# PS

​		Program Stream （节目流），注意 Program 翻译成中文是 **节目** 的意思，不是**程序**。

---

​		PS 封装格式主要用在 不容易发生错误的环境，例如 DVD 光盘等多媒体文件的存储环节，在GB28181中使用了PS。



## 参考

[PS流及其封装 - shunxiang - 博客园 (cnblogs.com)](https://www.cnblogs.com/shawn-meng/articles/16305596.html)

[流媒体基础-RTP封装PS流 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/595923734)



## 基本结构

- PS流或者文件可以视为多个 PS GOP

- 一个PS GOP由一个或多个PS包组成

  - 每个GOP是以I帧起始的多帧集合
  - 各GOP之间的信息没有相互依赖性，可以进行剪切拼接

- 一个PS包内包含一个PSH(PS Header)和若干个PES包

- 每个PS GOP的第一个PS包应当在包头PSH后立即跟随一个PSM包。

- PSM包是一种特殊的PES包，含有对其他PES负载数据类型的描述。

- PS包内的其他PES的出现顺序和内容没有特殊约束，即一个PS包内可以包含交错出现的视频、音频和私有流等PES包，各PES包根据PSM的描述进行拆分。

  

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1343107-20220524143245043-389558396.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/7afd4879bf0f2888314624299b908d4d.png#pic_center)

## PS Header

​		PSH (Program Stream pack Header)是PS包的包头，主要包含系统时间信息。

​		PSH主要包含了系统同步时间，MPEG2 part 1并没有定义PSH与其后数据之间的必然关系。

- PSH是一个PS包的包头，但PSH并不包含PS包负载数据的内容和长度信息
- 这些信息存在PS包负载的PES包内
  - 一个PS包内包含的PES包个数、类型和长度没有限制。
  - 一个PS流文件只需要有头上一个PSH，后面的视频、音频和私有数据PES包可以交错排列
  - 但是作为系统同步的需要，PSH应当以一定的频率在流中出现，I帧前必须有PSH，P/B帧前要有PSH。

| 字段                                                         | bit  | 说明                                                         |
| ------------------------------------------------------------ | :--: | ------------------------------------------------------------ |
| marker_bit                                                   |  1   | 其值为‘ 1’ 。                                                |
| pack_start_code                                              |  32  | 为0x000001BA，标识包的起始。                                 |
| pack_stuffing_length                                         |  3   | 指示跟随此字段的填充字节数。                                 |
| program_mux_rate                                             |  22  | 指示包期间 P-STD 接收节目流的速率，其中该节目流包含在包中。 program_mux_rate 值以 50 字节/秒为度量单位。0 值禁用。 |
| stuffing_byte                                                |  8   | 此为等于0xFF的固定 8 比特值，可以由编码器插入，例如满足信道的要求。它由解码器丢弃。在每个包头中，应存在不多于 7 个的填充字节。 |
| system_clock_reference_base system_clock_reference_extension | 33 9 | 系统时钟参考（ SCR）为分成两部分编码的 42 比特字段。第一部分，system_clock_reference_base 为 33 比特字段，其值由公式 5-1中给出的 SCR_base (i)给出。第二部分，system_clock_reference_extension，为 9 比特字段，其值由公式 5-2 中给出的 SCR_ext (i)给出。 SCR 指示在节目目标解码器的输入端包含system_clock_reference_base 最后比特的字节到达的预期时间。 SCR_base(i) = ((system_clock_frequency * t(i))DIV300)%2^33 (5-1) SCR_ext(i) = ((system_clock_frequency * t(i))DIV300)%300 (5-2) 我们精度要求没有这么高，system_clock_reference_extension设置为0即可。 |

```c
#define PS_HDR_LEN  14
static int gb28181_make_ps_header(char *pData, unsigned long long s64Scr)
{
    unsigned long long lScrExt = (s64Scr) % 100;    
    s64Scr = s64Scr / 100;
    bits_buffer_s      bitsBuffer;
    bitsBuffer.i_size = PS_HDR_LEN;    
    bitsBuffer.i_data = 0;
    bitsBuffer.i_mask = 0x80;
    bitsBuffer.p_data = (unsigned char *)(pData);
    memset(bitsBuffer.p_data, 0, PS_HDR_LEN);
    bits_write(&bitsBuffer, 32, 0x000001BA); /*start codes 起始码*/
    bits_write(&bitsBuffer, 2, 1); /*marker bits '01b'*/
    bits_write(&bitsBuffer, 3, (s64Scr>>30)&0x07); /*System clock [32..30]*/
    bits_write(&bitsBuffer, 1, 1); /*marker bit*/
    bits_write(&bitsBuffer, 15, (s64Scr>>15)&0x7FFF); /*System clock [29..15]*/
    bits_write(&bitsBuffer, 1, 1); /*marker bit*/
    bits_write(&bitsBuffer, 15, s64Scr & 0x7fff); /*System clock [14..0]*/
    bits_write(&bitsBuffer, 1, 1); /*marker bit*/
    bits_write(&bitsBuffer, 9, 0); /*SCR extension*/
    bits_write(&bitsBuffer, 1, 1); /*marker bit*/
    bits_write(&bitsBuffer, 22, (255)&0x3fffff); /*bit rate(n units of 50 bytes per second.)*/
    bits_write(&bitsBuffer, 2, 3); /*marker bits '11'*/
    bits_write(&bitsBuffer, 5, 0x1f); /*reserved(reserved for future use)*/
    bits_write(&bitsBuffer, 3, 0); /*stuffing length*/
    return 0;
}
```



## PS system Header

​		PS System Header，节目流系统标题，一般属于PSH头部后携带的扩展。

| 字段                         | bit  | 说明                                                         |
| ---------------------------- | :--: | ------------------------------------------------------------ |
| system_header_start_code     |  32  | 为0x000001BB，标识系统头的起始。                             |
| header_length                |  16  | 指示跟随 header_length 字段后的系统头的长度，以字节为单位。  |
| rate_bound                   |  22  | rate_bound 为大于或等于在任意节目流包中编码的 program_mux_rate 字段的最大值的整数值。它可供解码器使用来评估它是否有能力解码该完整流。 |
| audio_bound                  |  6   | audio_bound 为 0 到 32 闭区间内的一个整数，在解码过程同时被激活的节目流中，它被设置为大于或等于 ISO/IEC 13818-3 和 ISO/IEC 11172-3 音频流最大数的整数值。出于本子节的目的， 规定只要 STD 缓冲器非空或者只要显示单元正在 P-STD 模型中显示， ISO/IEC 13818-3 或 ISO/IEC11172-3 音频流的解码处理就被激活。 |
| fixed_flag                   |  1   | 置于‘ 1’时指示固定的比特速率操作。置于‘ 0’时指示可变比特速率操作。固定的比特速率操作期间，在该多路复用的 ITU-T H.222.0 建议书 ISO/IEC 13818-1 流中，所有 system_clock_reference 字段中的编码值应依从以下线性方程： SCR_base(i) = ((c1 × i + c2) DIV 300) % 2^33 (5-3) SCR_ext(i) = ((c1 × i + c2) DIV 300) % 300 (5-4) 其中： c1 为对所有 i 有效的实值常量, c2 为对所有 i 有效的实值常量, i 为该流中包含任意 system_clock_reference 字段最后比特的字节在 ITU-T H.222.0 建议书 ISO/IEC 13818-1 多路复用流中的索引号。 |
| CSPS_flag                    |  1   | 其值置于‘ 1’ ，则节目流满足 2.7.9 中规定的限制。             |
| system_audio_lock_flag       |  1   | 指示音频采样速率和系统目标解码器的 system_clock_frequency 之间存在特定的常量比率关系。 system_clock_frequency 在子节 2.5.2.1 中定义，音频采样速率在 ISO/IEC 13818-3 中指定。 system_audio_lock_flag 可以设置为‘ 1’ ，仅当对于节目流的所有音频基本流的所有显示单元而言， system_clock_frequency 与实际音频采样速率的比例 SCASR，为常量并等于下表中音频流所指示的标准采样速率所规定的值。![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524144942189-1937113685.png) ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524154315813-1595460249.png) |
| system_video_lock_flag       |  1   | 指示视频时间基和系统目标解码器的系统时钟频率之间存在特定的常量比率关系。 system_video_lock_flag 可以设置为‘ 1’ ，仅当对于 ITU-TH.222.0 建议书 ISO/IEC 13818-1 节目的所有视频基本流的所有显示单元而言， system_clock_frequency 与实际视频时间基的比例为常量值。 对于 ISO/IEC 11172-2 和 ITU-T H.262 建议书 ISO/IEC 13818-2 视频流，如果 system_video_lock_flag 设置为 '1'， system_clock_frequency 与实际视频帧速率的比例 SCFR，为常量并等于下表中对视频流所指示的标准帧速率所规定的值。 对于 ISO/IEC 14496-2 视频流，如果 system_video_lock_flag 设置为 '1'， ISO/IEC 14496-2 视频流的时间基， 由 vop_time_increment_resolution 规定， 锁定为 STC 并精确地等于 N 倍的 system_clock_frequency 除以 K，其中整数 N 和 K 在每个可视目标序列中有固定的值， K 大于或等于 N。 对于 ITU-T H.264 建议书 ISO/IEC 14496-10 视频流， AVC 时间基的频率由 AVC 参数 time_scale 规定。如果对于 AVC 视频流， system_video_lock_flag 设置为 '1' ，则 AVC 时间基的频率锁定为 STC 并精确地等于N 倍的 system_clock_frequency 除以 K，其中整数 N 和 K 在每个可视目标序列中有固定的值， K 大于或等于 N。 ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524150131274-292613267.png) ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524154412834-1884380689.png) 该比例 SCFR 的值是精确的。在标准速率为每秒 23.976、 29.97、 59.94 帧的情况中，实际帧速率与标准速率略有差别。 |
| video_bound                  |  5   | 在 0 到 16 的闭区间内取值，在解码过程同时被激活的节目流中，它被设置为大于或等于视流的最大数的整数值。对于本小节来说，规定只要 P-STD 模型中的一个缓冲器非空，或者只要显示单元正在 P-STD 模型中显示，视频流的解码处理就被激活。 |
| packet_rate_restriction_flag |  1   | 若 CSPS 标志设置为‘ 1’ ，则 packet_rate_restriction_flag 指示适用于该包速率的那些限制。若 CSPS 标志置于‘ 0’值，则 packet_rate_restriction_flag 的含义未确定。 |
| reserved_bits                |  7   | 此 7 比特字段由 ISO/IEC 保留供未来使用。除非由 ITU-T         |
| stream_id                    |  8   | 指示以下P-STD_buffer_bound_scale 和 P-STDP-STD_buffer_size_bound 字段所涉及的流的编码与基本流编号。 若 stream_id 等 于 ‘ 1011 1000 ’， 则 跟 随 stream_id 的 P-STD_buffer_bound_scale 和P-STD_buffer_size_bound 字段涉及节目流中的所有音频流。 若 stream_id 等 于 ‘ 1011 1001 ’， 则 跟 随 stream_id 的 P-STD_buffer_bound_scale 和P-STD_buffer_size_bound 字段涉及节目流中的所有视频流。 若 stream_id 取任何其他值，则它将是大于或等于‘ 1011 1100’的字节值并将解释为涉及依照表 2-22的流编码和基本流编号。 节目流中存在的每个基本流应有其 P-STD_buffer_bound_scale 和 P-STD_buffer_size_bound， 在每个系统头中通过此机制确切地一次指定。 |
| P-STD_buffer_bound_scale     |  1   | 指 示 用 于 解 释 后 续P-STD_buffer_size_bound 字段的标度因子。若前导 stream_id 指示音频流，则 P-STD_buffer_bound_scale 必有‘ 0’值。若前导 stream_id 指示视频流，则 P-STD_buffer_bound_scale 必有‘ 1’值。对所有其他流类型，P-STD_buffer_bound_scale 的值可以为‘ 1’或为‘ 0’ 。 |
| P-STD_buffer_size_bound      |  13  | P-STD_buffer_size_bound 为 13 比特无符号整数，规定该值大于或等于节目流中流 n 的所有包上的最大 P-STD 输入缓冲器尺寸 BSn。若 P-STD_buffer_bound_scale 有‘0’值，那么 P-STD_buffer_size_bound 以 128 字节为单位度量该缓冲器尺寸限制。 若 P-STD_buffer_bound_scale 有‘ 1’值，那么 P-STD_buffer_size_bound 以 1024 字节为单位度量该缓冲器尺寸限制。 |

```c
#define SYS_HDR_LEN 18
static int gb28181_make_sys_header(char *pData)
{    
    bits_buffer_s bitsBuffer;
    bitsBuffer.i_size = SYS_HDR_LEN;
    bitsBuffer.i_data = 0;
    bitsBuffer.i_mask = 0x80;
    bitsBuffer.p_data = (unsigned char *)(pData);
    memset(bitsBuffer.p_data, 0, SYS_HDR_LEN);
    /*system header*/
    bits_write( &bitsBuffer, 32, 0x000001BB); /*start code*/
    bits_write( &bitsBuffer, 16, SYS_HDR_LEN-6); /* 减6，是因为start code加上length这两位，占了6个字节*/
    bits_write( &bitsBuffer, 1, 1); /*marker_bit*/
    bits_write( &bitsBuffer, 22, 50000); /*rate_bound*/
    bits_write( &bitsBuffer, 1, 1); /*marker_bit*/
    bits_write( &bitsBuffer, 6, 1); /*audio_bound*/
    bits_write( &bitsBuffer, 1, 0); /*fixed_flag */
    bits_write( &bitsBuffer, 1, 1); /*CSPS_flag */
    bits_write( &bitsBuffer, 1, 1); /*system_audio_lock_flag*/
    bits_write( &bitsBuffer, 1, 1); /*system_video_lock_flag*/
    bits_write( &bitsBuffer, 1, 1); /*marker_bit*/
    bits_write( &bitsBuffer, 5, 1); /*video_bound*/
    bits_write( &bitsBuffer, 1, 0); /*dif from mpeg1*/
    bits_write( &bitsBuffer, 7, 0x7F); /*reserver*/
    /*audio stream bound*/
    bits_write( &bitsBuffer, 8, 0xC0); /*stream_id 音频的流id*/
    bits_write( &bitsBuffer, 2, 3); /*marker_bit */
    bits_write( &bitsBuffer, 1, 0); /*PSTD_buffer_bound_scale*/
    bits_write( &bitsBuffer, 13, 512); /*PSTD_buffer_size_bound*/
    /*video stream bound*/
    bits_write( &bitsBuffer, 8, 0xE0); /*stream_id 视频的流id*/
    bits_write( &bitsBuffer, 2, 3); /*marker_bit */
    bits_write( &bitsBuffer, 1, 1); /*PSTD_buffer_bound_scale*/
    bits_write( &bitsBuffer, 13, 2048); /*PSTD_buffer_size_bound*/
    return 0;
}
```



## PSM

​		PS system Map，节目流映射 ，提供节目流中基本流的描述及其相互关系。作为一个PES分组出现。

节目流映射中字段的语义定义

| 字段                          | bit  | 说明                                                         |
| ----------------------------- | :--: | ------------------------------------------------------------ |
| packet_start_code_prefix      |  24  | 同跟随它的 map_stream_id 一起组成包起始码标识包的始端。 packet_start_code_prefix 为 0x000001。 |
| map_stream_id                 |  8   | 值为 0xBC。                                                  |
| program_stream_map_length     |  16  | 指示紧随此字段的program_stream_map 中的字节总数。此字段的最大值为 1018（ 0x3FA）。 |
| current_next_indicator        |  1   | 置‘ 1’时指示发送的节目流映射为当前有效。该比特置‘ 0’时，它指示发送的节目流映射尚未有效并且下一个节目流映射表将生效。建议设置为'1'。 |
| program_stream_map_version    |  5   | 整个节目流映射的版本号。每当节目流映射的定义改变时，该版本号必须增 1 模 32。 current_next_indicator 置为‘ 1’时， program_stream_map_version 应是当前有效的节目流映射的版本。 current_next_indicator 设置为‘ 0’时， program_stream_map_version 应是下一个有效的节目流映射的版本。 |
| program_stream_info_length    |  16  | 指示紧随此字段的描述符的总长。                               |
| marker_bit                    |  1   | 1                                                            |
| elementary_stream_map_length  |  16  | 指示在此节目流映射中所有基本流信息的以字节为单位的总长度。它包括stream_type、 elementary_stream_id 以及elementary_stream_info_length 字段。 |
| stream_type                   |  8   | 指示依照下表的流类型。 stream_type 字段仅标识 PES 包中包含的基本流。 0x05 赋值被禁用。 |
| elementary_stream_id          |  8   | 指示存储此基本流的 PES 包的 PES 包头内的 stream_id 字段的赋值。 |
| elementary_stream_info_length |  16  | 指示紧随此字段的描述符长度，以字节为单位。                   |
| CRC_32                        |  32  | 包含整个节目流映射处理后附件 A 定义的解码器中给出寄存器零输出的 CRC值。 |

流类型表：

|    值     | 描述                                                         |
| :-------: | ------------------------------------------------------------ |
|   0x00    | ITU-T ISO/IEC 保留                                           |
|   0x01    | ISO/IEC 11172-2 视频 (MPEG1)                                 |
|   0x02    | ITU-T H.262 建议书 ISO/IEC 13818-2 视频或 ISO/IEC 11172-2 受限参数视频流 (MPEG2) |
|   0x03    | ISO/IEC 11172-3 音频                                         |
|   0x04    | ISO/IEC 13818-3 音频                                         |
|   0x05    | ITU-T H.222.0 建议书 ISO/IEC 13818-1 private_sections        |
|   0x06    | ITU-T H.222.0 建议书 ISO/IEC 13818-1 PES 包含专用数据的 PES 包 |
|   0x07    | ISO/IEC 13522 MHEG                                           |
|   0x08    | ITU-T H.222.0 建议书 ISO/IEC 13818-1 附件 A DSM-CC           |
|   0x09    | ITU-T H.222.1 建议书                                         |
|   0x0A    | ISO/IEC 13818-6 类型 A                                       |
|   0x0B    | ISO/IEC 13818-6 类型 B                                       |
|   0x0C    | ISO/IEC 13818-6 类型 C                                       |
|   0x0D    | ISO/IEC 13818-6 类型 D                                       |
|   0x0E    | ITU-T H.222.0 建议书 ISO/IEC 13818-1 辅助                    |
|   0x0F    | 具有 ADTS 传输句法的 ISO/IEC 13818-7 音频 （AAC）            |
|   0x10    | ISO/IEC 14496-2 可视 (MPEG4)                                 |
|   0x11    | 具有 LATM 传输句法的 ISO/IEC 14496-3 音频，如 ISO/IEC 14496-3 所规定 |
|   0x12    | PES 包中承载的 ISO/IEC 14496-1 SL 包式流或 FlexMux 流        |
|   0x13    | ISO/IEC 14496_ sections 中承载的 ISO/IEC 14496-1 SL 包式流或 FlexMux 流 |
|   0x14    | ISO/IEC 13818-6 同步下载协议                                 |
|   0x15    | PES 包中承载的元数据                                         |
|   0x16    | metadata_sections 中承载的元数据                             |
|   0x17    | ISO/IEC 13818-6 数据转盘式磁带中承载的元数据                 |
|   0x18    | ISO/IEC 13818-6 目标转盘式磁带中承载的元数据                 |
|   0x19    | ISO/IEC 13818-6 同步下载协议中承载的元数据                   |
|   0x1A    | IPMP 流 （ ISO/IEC 13818-11, MPEG-2 IPMP 中规定）            |
|   0x1B    | ITU-T H.264 建议书                                           |
| 0x1C-0x7E | ITU-T H.222.0 建议书 ISO/IEC 13818-1 保留 (H265->0x24)       |
|   0x7F    | IPMP 流                                                      |

注：H265的值定义为 0x24

```c
static int gb28181_make_psm_header(char *pData)
{
    bits_buffer_s bitsBuffer;
    bitsBuffer.i_size = PSM_HDR_LEN; 
    bitsBuffer.i_data = 0;
    bitsBuffer.i_mask = 0x80;
    bitsBuffer.p_data = (unsigned char *)(pData);
    memset(bitsBuffer.p_data, 0, PSM_HDR_LEN);//24Bytes
    bits_write(&bitsBuffer, 24,0x000001); /*start code*/
    bits_write(&bitsBuffer, 8, 0xBC); /*map stream id*/
    bits_write(&bitsBuffer, 16,18); /*program stream map length*/ 
    bits_write(&bitsBuffer, 1, 1); /*current next indicator */
    bits_write(&bitsBuffer, 2, 3); /*reserved*/
    bits_write(&bitsBuffer, 5, 0); /*program stream map version*/
    bits_write(&bitsBuffer, 7, 0x7F); /*reserved */
    bits_write(&bitsBuffer, 1, 1); /*marker bit */
    bits_write(&bitsBuffer, 16,0); /*programe stream info length*/
    bits_write(&bitsBuffer, 16, 8); /*elementary stream map length is*/
    /*audio*/
    bits_write(&bitsBuffer, 8, 0x90); /*stream_type 音频编码格式G711*/
    bits_write(&bitsBuffer, 8, 0xC0); /*elementary_stream_id*/
    bits_write(&bitsBuffer, 16, 0); /*elementary_stream_info_length is*/
    /*video*/
    bits_write(&bitsBuffer, 8, 0x1B); /*stream_type 视频编码格式H.264*/
    bits_write(&bitsBuffer, 8, 0xE0); /*elementary_stream_id*/
    bits_write(&bitsBuffer, 16, 0); /*elementary_stream_info_length */
    /*crc (2e b9 0f 3d)*/
    bits_write(&bitsBuffer, 8, 0x45); /*crc (24~31) bits*/
    bits_write(&bitsBuffer, 8, 0xBD); /*crc (16~23) bits*/
    bits_write(&bitsBuffer, 8, 0xDC); /*crc (8~15) bits*/
    bits_write(&bitsBuffer, 8, 0xF4); /*crc (0~7) bits*/
    return 0;
}
```





## H264封装规则

1. 一个网络抽象单元（NALu）视为一个处理单元，编码器一帧编码后输出可能是多个处理单元；
2. 除B帧外的帧输出的多个单元封装时应当在第一个单元头部添加PSH；
3. 当一个NALu单元长度超过系统PES包长限制时，可以将该NALu分割成多个PES包，但每个PES包内不应当包含多于1个NALu的数据；
4. 每个帧的第一个处理单元所封装成的第一个PES包包头应当带有PTS；
5. 每个I帧封装时应当在PSH之后添加PSM；
6. 当元素流的基本特性（编码格式，帧率，分辨率，场编码等）发生变化时，变化后的第一帧封装时必须在PSH之后添加PSM；

典型的H.264 I帧 PS包：
![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1343107-20220524163613065-1943205378.png)

典型的H.264 P/B帧PS包：
![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1343107-20220524163640853-1329747583.png)





## 音频封装规则

1. 在音视频同步时序条件允许的情况下，连续的一个或多个音频帧可以合并，每帧视为一个处理单元；
2. 当音频帧长度不大于PES包负载或网络传输单元所能承受的最大长度时，建议将每个音频帧封装成一个单独的PES包；
3. 每次处理的第一个音频帧所形成的第一个PES包头应当带有PTS，其余音频帧所形成的第一个PES包头建议带有PTS；
4. 当系统不包含视频流时，PSH在流中出现的间隔应当小于1秒，PSM的出现间隔应当小于4秒；当音频参数（码率，采样率，声道数等）发生变化时，必须在变化后立即出现PSH和PSM。

典型的音频帧数据包：
![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1343107-20220524163802958-821347166.png)



## 私有信息封装规则
1、在 PS 流中传输的海康私有信息一般一帧对应的一种信息视为一个处理单元；；

2、当一个处理单元的长度不大于 PES 包负载或网络传输单元所能承受的最大长度时，建议将其封装成一个单独的 PES 包；

3、每个处理单元的第一个 PES 包包头应当带有 PTS。

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/a2a0fb4217f4466baff9f262ba0d6c59.png)