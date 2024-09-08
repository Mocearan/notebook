# PS

​		Program Stream （节目流），注意 Program 翻译成中文是 **节目** 的意思，不是**程序**。

---

​		PS 封装格式主要用在 不容易发生错误的环境，例如 DVD 光盘等多媒体文件的存储环节，在GB28181中使用了PS。



## 参考

[PS流及其封装 - shunxiang - 博客园 (cnblogs.com)](https://www.cnblogs.com/shawn-meng/articles/16305596.html)

[流媒体基础-RTP封装PS流 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/595923734)



## 基本结构

​		`PS`流由多个` PS GOP`组成。

- `PS GOP`

  - 包含一个或多个PS包
  - 以`Video I frame`起始，多帧集合
  - GOP之间的信息没有相互依赖性，可以进行剪切拼接

- `PS`包

  - 包含一个 `PS Header` 和 若干个`PES`包

  - `PS GOP`的首个`PS`的`PSH`后立即跟随`PSM`包

    - 首个`PS`的`PSH`可能含有名为`PS System Header`的扩展

  - `PS`包内的其他PES的出现顺序和内容没有特殊约束

    > 即PS包内可以包含交错出现的视频、音频和私有流等PES包，各PES包根据PSM的描述进行拆分

- `PS`流有一个结束码，占位32bit，其值为`0x000001B9`

  - PS流总是以`0x000001BA`开始，以`0x000001B9`结束
    - `0x000001BA`是`PSH`的起始码
  - 对于一个`PS`文件，有且只有一个结束码`0x000001B9`
  - 对于直播的`PS`流，应该是没有结束码的

> - `PS System Header`是`PSH`的扩展
> - `PSM`包是一种特殊的PES包，含有对其他`PES`负载数据类型的描述



![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1343107-20220524143245043-389558396.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/7afd4879bf0f2888314624299b908d4d.png#pic_center)



## `PS Header`

​		`PSH `(Program Stream pack Header)是PS包的包头，主要包含系统时间信息， 但`PSH`并不包含PS包负载数据的内容和长度信息，这些信息存在PS包负载的数据PES包内。

​		PS包中的PES包个数、类型和长度没有限制。但为系统同步，PSH以一定的频率在流中出现：`I`帧前必须有`PSH/PSM`，`P/B`帧前要有`PSH`。

- `pack_start_code  `，`32 bit`，  标识PS包的开始，固定为`0x000001BA`
- `01`，`2 bit`
- `SCR`，system_clock_reference_base ，`46 bit`
  - `system_clock_reference_base [32..30]`：占位3bit；
  - `marker_bit`：占位1bit；
  - `system_clock_reference_base [29..15]`：占位15bit；
  - `marker_bit`：占位1bit；
  - `system_clock_reference_base [14..0]`：占位15bit；
  - `marker_bit`：占位1bit；
  - `system_clock_reference_extension`：占位9bit；
  - `marker_bit`：占位1bit；
  - `program_mux_rate`字段：速率值字段，占位22bit，正整数
    - 表示P-STD接收此字段所在包的PS流的速率；这个值以每秒50字节作为单位；禁止0值；
  - `marker_bit`：标记字段，占位1bit，固定为 1；
  - `marker_bit`：标记字段，占位1bit，固定为 1；
- `reserved`，`5 bit`
- `pack_stuffing_length`，`3 bit`，规定了此字段之后填充字段的长度
- ---> loop
- `stuffing_byte`， 每个填充字节为`0xFF`的`8 bit`
  - 由编码器插入，例如满足信道的要求。由解码器丢弃。
  - 在每个包头中，应存在不多于 7 个的填充字节
- ----> loop end



```c
【00 00 01 ba】 7e 97 9f 18 24 01 04 d2 37 【fe】 ff ff
00 8b 0a 43 00 00 01 bb 00 12 82 69 1b 04 e1 7f 
....

/*
-  0x000001BA 代表 PS 包的开始（startcode）
- 跳过 9 个字节，暂时不关心它的内容，看第 10 个字节 fe
	- 对应着二进制数据的 1111 1110，它的后三位为 110 为十进制的 6
	- 接下来的六个字节是扩展内容
*/
```





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



## `PS system Header`

​		PS System Header，节目流系统标题，一般属于`PSH`头部后携带的扩展，作为一个单独PES分组出现。

- `system_header_start_code`，`32 bit`，标识系统头的起始，`0x00 00 01 BB`

- `header_length`，`16 bit`，表示此字段之后的系统首部字节长度

- `maker_bit`，`1 bit`，固定值为1

- `rate_bound`，`22 bit`，为一个大于或等于PS流所有PS包中的最大program_mux_rate值的整数；可以被解码器用来判断是否可以对整个流进行解码；

- `maker_bit`，`1 bit`，固定值为1

- `audio_bound`，`6 bit`，取值范围0到32间整数；大于或等于同时进行解码处理的PS流中的音频流的最大数目；

  >   6  audio_bound 为 0 到 32 闭区间内的一个整数，在解码过程同时被激活的节目流中，它被设置为大于或等于 ISO/IEC 13818-3 和 ISO/IEC 11172-3 音频流最大数的整数值。出于本子节的目的， 规定只要 STD 缓冲器非空或者只要显示单元正在 P-STD 模型中显示， ISO/IEC 13818-3 或 ISO/IEC11172-3 音频流的解码处理就被激活。

- `fixed_flag`，`1 bit`，置位1表示固定比特率操作，置位0则为可变比特率操作

- `CSPS_flag`，`1 bit`，置位1表示此PS流满足标准 2.7.9 中规定的限制

- `system_audio_lock_flag`，`1 bit`，表示音频采样率和 STD 的 system_clock_frequency 之间有一特定常数比例关系

  - ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524144942189-1937113685.png) 
  - ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524154315813-1595460249.png)

- `system_video_lock_flag`，`1 bit`，表示在系统目标解码器system_clock_frequency和视频帧速率之间存在一特定常数比例关系

  - ​    ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524150131274-292613267.png) 
  - ![img](https://img2022.cnblogs.com/blog/1343107/202205/1343107-20220524154412834-1884380689.png)

- `maker_bit`，`1 bit`，固定值为1

- `video_bound`，`5 bit`，取值范围0到16；大于或等于同时进行解码处理的PS流中的视频流的最大数目；

- `packet_rate_restriction_flag`，`1 bit`，分组速率限制标志字段

  - 若CSPS_flag == 1，则此字段表示哪种限制适用于分组速率；若CSPS_flag == 0，则此字段无意义；

- `reserved_bits`，`7 bit`，`1111 111`

- -----> loop，前面共6个字节

- `stream_id`，`8 bit`，表示其后的 P-STD_buffer_bound_scale 和 P-STD_buffer_size_bound 字段所涉及的流的编码和基本流的号码

  - 若 stream_id == 1011 1000，则其后的 P-STD_buffer_bound_scale 和 P-STD_buffer_size_bound 字段对应PS流中的所有音频流；
  - 若stream_id == 1011 1001，则其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段对应PS流中的所有视频流；
  - 若取其他值，则应大于 1011 1100 ，且按照标准对应Stream id(详见附录1)；
  - PS流中的每个原始流都应在每个系统首部中通过这种机制精确地规定一次它的 P-STD_buffer_bound_scale 和 P-STD_buffer_size_bound；

- `11`，`2 bit`

- `P-STD_buffer_bound_scale`，`1 bit`，表示用来解释后面P-STD_buffer_size_bound字段的比例因子

  - 如果之前的stream_id表示音频流，则此值应为0
  - 若之前的stream_id表示视频流，则此值应为1
  - 对于其他stream类型，此值可以0或1；

- `P-STD_buffer_size_bound`，`13 bit`，大于或等于所有PS流分组的P-STD输入缓冲区大小BSn的最大值

  - 若P-STD_buffer_bound_scale == 0，则P-STD_buffer_size_bound以128字节为单位
  - 若P-STD_buffer_bound_scale == 1，则P-STD_buffer_size_bound以1024字节为单位

- -----> loop end

```c
【00 00 01 ba】 7e 97 9f 18 24 01 04 d2 37 【fe】 ff ff
00 8b 0a 43 【00 00 01 bb】 00 12 【82 69 1b 04 e1 7f |
e0 e0 80 c0 c0 08 bd e0 80 bf e0 80】 00 00 01 bc
00 5e fa ff 00 24 40 0e 48 4b 01 00 10 15 83 bc

/*
- 00 00 01 bb 系统头起始码
- 00 12 2字节的header length，即18个字节的system header
- 82 69 1b 04 e1 7f , loop 前的各个字段
- e0 e0 80 c0 c0 08 bd e0 80 bf e0 80， loop，每3个字节表示一次loop中各个字段
	- e0 e0 80
	- c0 c0 08 
	- bd e0 80 
	- bf e0 80
*/
```

> - 视频流ID：`0XE0`
>
> - 音频流 ID：`0xc0`
>
> - 在海康的协议说明中：`0xBD`表示海康私有流
>   - 可能是海康摄像头中对画面的附加信息，比如移动侦测的红框
>
> - PS 包中的流类型（stream type）的取值如下：
>   - MPEG-4 视频流： 0x10；
>
>   - H.264 视频流： 0x1B；
>
>   - SVAC 视频流： 0x80；
>
>   - G.711 音频流： 0x90；
>
>   - G.722.1 音频流： 0x92；
>
>   - G.723.1 音频流： 0x93；
>
>   - G.729 音频流： 0x99；
>
>   - SVAC音频流： 0x9B。
>

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



## `PSM`

​		`program stream Map`，节目流映射 ，提供节目流中基本流的描述及其相互关系。作为一个单独PES分组出现。

> `0x 00 00 01 BC`

- `packet_start_code_prefix`，`24 bit`，包头起始码，固定为`0x000001`
  - 与后面的字段`map_stream_id`一起组成分组开始码，标志着分组的开始；
- `map_stream_id`，`8 bit`， 类型字段，标志此分组是什么类型
  - 值为`0xBC`，则说明此`PES`包为`PSM`
- `program_stream_map_length`，`16 bit`，表示此字段之后PSM的总长度
  - 最大值为`1018(0x3FA)`
- `current_next_indicator`，`1 bit`
  - 置位1表示当前PSM是可用的
  - 置位0则表示当前PSM不可以，下一个可用；
- `reserved`，`2 bit`，固定为1
- `program_stream_map_version`，`5 bit`，表示PSM的版本号，取值范围1到32
  - 随着PSM定义的改变循环累加
  - 若`current_next_indicator == 1`，表示当前PSM的版本号
  - 若`current_next_indicator == 0`，表示下一个PSM的版本号
- `reserved`，`7 bit`
- `marker_bit`，`1 bit`，固定为1
- `program_stream_info_length`，`16 bit`，表示此字段后面的`descriptor`字段的长度
- ----------> loop
- `descriptor`，program Stream信息描述字段，长度由前个字段确定
- ----------> loop end
- `elementary_stream_map_length`，`16 bit`，表示在这个PSM中所有ES流信息的总长度
  - 包括`stream_type`, `elementary_stream_id`, `elementary_stream_info_length`的长度，即`N*32bit`
  - 是不包括具体ES流描述信息`descriptor`的长度的
- ----------> loop
- `stream_type`，`8 bit`，表示原始流ES的类型
  - 这个类型只能标志包含在PES包中的ES流类型
  - 值`0x05`是被禁止的
  - 常见取值类型有
    - MPEG-4 视频流：`0x10`；
    - H.264 视频流：`0x1B`；
    - G.711 音频流：`0x90`；
  - 因为`PSM`只有在关键帧打包的时候才会存在，所以如果要判断`PS`打包的流编码类型，就根据这个字段来判断
- `elementary_stream_id`，`8 bit`，表示此`ES`流所在`PES`分组包头中的`stream_id`字段的值
  - `0x(C0~DF)`指音频
  - `0x(E0~EF)`为视频
- `elementary_stream_info_length`，`16 bit`，表示此字段之后的，`ES`流描述信息的长度
- `descriptor`，长度由前个字段确定，表示此类型的`ES`流的描述信息
  - 这个描述信息的长度是不包含在`elementary_stream_map_length`字段里面的；
- `CRC_32`，`32 bit`，CRC校验值
- ----------> loop end

​	

- 流ID表

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-7083f76e4dcd20227735f8224f47c174_1440w.webp)

- 流类型表：

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

>  注：H265的值定义为 0x24



```c
【00 00 01 ba】 7e 97 9f 18 24 01 04 d2 37 【fe】 ff ff
00 8b 0a 43 【00 00 01 bb】 00 12 【82 69 1b 04 e1 7f |
e0 e0 80 c0 c0 08 bd e0 80 bf e0 80】 【00 00 01 bc】
00 5e 【fa ff 00 24 40 0e 48 4b 01 00 10 15 83 bc
ed a7 00 ff ff ff 41 12 48 4b 00 01 02 03 04 05
06 07 08 09 0a 0b 0c 0d 0e 0f | 00 30 | 1b e0 00 1c
42 0e 07 10 10 ea 07 80 04 38 11 30 00 00 3a 99
2a 0a 7f ff 00 00 0e a6 1f fe f0 87 90 c0 00 0c
43 0a 01 40 fe 00 7d 03 03 e8 03 ff d3 af 06 38】
...
    
/*
- 00 00 01 bc， PSM起始码 + map_stream_id(二进制的 1011 1100)
	-  program_stream_map，即 PSM
- 00 5e ， program_stream_map_length，0x005E 即 94
	- 接下来的 94 个字节也是属于 PSM 段的
- fa ff， 两个字节的固定若干字段
	- current_next_indicator，1 bit
	- reserved，2 bit，固定为1
	- program_stream_map_version，5 bit
    - reserved，7 bit
    - marker_bit，1 bit，固定为1
- 00 24，program_stream_info_length, 后跟着的 descriptor 共占 36 字节.
- 40 0e 48 4b 01 00 10 15 83 bc ed a7 00 ff ff ff 41 12 48 4b 00 01 02 03 04 0506 07 08 09 0a 0b 0c 0d 0e 0f, descriptor
- 00 30，element_stream_map_length（基本流映射长度），表示接下来的 48 字节都是用来描述原始流信息的
- 1b e0 00 1c 42 0e 07 10 10 ea 07 80 04 38 11 30 00 00 3a 99 2a 0a 7f ff 00 00 0e a6 1f fe f0 87 90 c0 00 0c 43 0a 01 40 fe 00 7d 03 03 e8 03 ff， 描述原始流信息
	- 1b e0 00 1c 42 0e 07 10 10 ea 07 80 04 38 11 30 00 00 3a 99 2a 0a 7f ff 00 00 0e a6 1f fe f0 87
        - 1b，stream_type， 根据 GB28181 的定义可知为 H.264 编码
        - e0，表示其为视频流
        - 00 1c，接下来描述占用 28 字节
     - 90 c0 00 0c 43 0a 01 40 fe 00 7d 03 03 e8 03 ff
     	- 90，stream_type， 根据 GB28181 的定义可知为 G.711 编码
     	- c0，音频流
     	- 00 0c，接下来描述占用 12 字节
- d3 af 06 38，CRC_32，4 Byte
*/
```





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



## 解析PS流思路

- 先找到`PSH`的起始码`0x00 00 01 BA`
- 解析`PS System header`，判断是否有`PSM`
  - `PSM`只有在关键帧打包的时候，才会存在
  - `IDR`包含了`SPS`、`PPS`、`SEI`和` I `帧
  - 因此将`SPS`、`PPS`、`SEI`、`I frame`的NALU 封装为一个PS 包，包括`PSH`、`PS system header`、`PSM`、各个`NALU`的`PES`
- 根据`PSM`确定`payload`中各`PES`包所负载的`ES`类型
- 根据各个`ES`流类型和`ID`从`PES`包中解析出具体的`ES`流
- 要从`PS`流中找出帧类型，必须将`PS`包解析成`ES`并组成完整的帧，然后再帧数据开始处根据`NALU`头部来判断帧类型



## H264封装规则

1. 一个网络抽象单元（NALu）视为一个处理单元，编码器一帧编码后输出可能是多个处理单元；
2. 除B帧外的帧输出的多个单元封装时应当在第一个单元头部添加PSH；
3. 当一个NALu单元长度超过系统PES包长限制时，可以将该NALu分割成多个PES包，但每个PES包内不应当包含多于1个NALu的数据；
4. 每个帧的第一个处理单元所封装成的第一个PES包包头应当带有PTS；
5. 每个I帧封装时应当在PSH之后添加PSM
6. 当元素流的基本特性（编码格式，帧率，分辨率，场编码等）发生变化时，变化后的第一帧封装时必须在PSH之后添加PSM；

> 需要添加PSM时，即产生IDR的时刻
>
> - 每个`IDR NALU `前一般都会包含`SPS`、`PPS `、`SEI`等`NALU`
> - 这几个包的NALU加上PES header之后构成一个PES包
> - 这三个PES包加在PSM和IDR PES之间

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