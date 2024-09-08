# PES

---

​		`Packetized Elementary Stream`， 包装基础流，原始流分组。主要用于在传输过程中提供完整的同步和错误检测能力。

​		`PES`在数据包中包含了时间戳和其他信息，使其适合实时传输。PES 头主要是通过 PTS 和 DTS 来提供音视频同步的信息，防止解码器输入缓存上溢或下溢的关键。

- `Elementary Stream`，`ES`原始流，指的是视频或音频压缩编码后产生的比特数据
- 而视频或音频原始流分别打包加上`PTS/DTS`等头部信息生成视频`PES`和音频`PES`
  - 一个PES分组只包含一个原始流的编码数据

​		每一个 I帧、P帧、B帧 的包头都有一个PTS和DTS。

## 格式结构

​		PES包由包头和payload组成，

​		每个 pes packet 都包含一个完整的视频帧或音频帧，pes packet 结构如下：

```c
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  | pes header |   optional pes header    |      pes payload    |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      6 Byte           3~259 Byte              max 65526 Byte
```



### fixed PES header

​		固定6 Bytes

- `packet_start_code_prefix  `，`3 byte`， 起始码，固定为 `0x 00 00 01`
- `stream_id  `，`1 byte`， `PES` 包中的负载流类型
  - `Visual Object Sequence`，简称`VOS`，起始码`0x00 00 01 b0`
  - `Visual Object`，简称`VO`，起始码 `0x00 00 01 b5`
  - `MPEG4 Video Header Object`，简称`MVHO`，起始码` 0x00 00 01 00`
  - `Video Object Layer`，简称`VOL`，起始码` 0x00 00 01 20`
  - `MPEG4 Header UD`，简称`MHUD`，起始码 `0x00 00 01 b2`
  - `Video Object Plane`，简称`VOP`，起始码 `0x00 00 01 b6`
    - 包括`I frame`和`other frame`
    - 进行PS封装时，最基本的处理单元是VOP视频帧
    - I帧之前的参数集数据与I帧合并到一个单元处理
  
  - `Video Object Plane – other Frame`，简称`VOP`，起始码` 0x00 00 01 b6`
  - ...
  - 音频取值（`0xc0-0xdf`），通常为`0xc0`
  - 视频取值（`0xe0-0xef`），通常为`0xe0`
  
- `PES_packet_length  `，2 byte，  PES 包长度
  - 包括此字段后的可选包头和负载的长度
  - `0`表示长度不限制，只有视频数据长度会超过`0xff ff`

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-7083f76e4dcd20227735f8224f47c174_1440w.webp)

### optional pes header

​		当`fixed pes header`中的`stream_id`不是以下值时，进行`optional pes header`的解析

- `10`字段
- `program_stream_map`
- `padding_stream`
- `private_stream_2`
- `ECM`
- `EMM`
- `program_stream_directory`
- `DSMCC_stream`
- `ITU-T h.222.1`建议类型 E stream

​		可选包头中的控制信息有很多：

- `10`，`2 bit`
- `PES_scrambling_control`
- `PES_priority`
- `data_alignment_indicator`
- `copyright`
- `original_or_copy`
- `PTS_DTS_flags`  ，`2 bit` 
  - `10 `表示 PES 头部有 PTS 字段
  - `11 `表示有 PTS 和 DTS 字段
  - `00 `表示都没有
  - `10 `被禁止
- `ESCR_flag`
- `DSM_trick_mode_flag`
- `additional_copy_info_flag`
- `PES_CRC_flag`
- `PES_extension_flag`
- `PES_header_data_length`
- 
- `pts  `，40 bit ，显示时间戳， 实际 pts 长度占用 33 bit
- `dts`，40 bit ，解码时间戳，实际 dts 长度占用 33 bit

> ​		。DTS/PTS是相对SCR（系统参考）的时间戳，以 `90000 `为单位。
>
> ​		`PTS/DTS` 到 `ms`的转换公式是 `PTS/90`。系统时钟频率为` 90Khz`，所以转换到秒为 `PTS/90000`。



### 实例

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
    // 上面是 ps header + ps system header + psm
    // 下面是 pes
【00 00 01 e0】 00 2a 【8c 80 09 | 2f a5 e7 c6 09 ff ff
ff fc | 00 00 00 01 67 4d 00 2a 9d a8 l3 00 89 f9
66 e0 20 20 28 00 00 03 00 08 00 00 03 00 c4 20】
【00 00 01 e0 00 0e 8c 00 03 ff ff fc  00 00 00 01
68 ee 3c 80 】 【00 00 01 e0 00 0e 8c 00 02 ff fc 00
00 00 01 06 e5 01 80 80 】【 00 00 01 e0 ff c6 8c 00
05 ff ff ff ff fd 00 00 00 01 65 b8 00 00 25 4c
09 ff f8 80 c1 67 81 f3 f6 5f ca 1f 85 93 34 62
06 3f 27 5b 44 d1 02 bd c0 a3 6e 3c 7f ed b7 ff 
/*
- 0x000001 起始码
- e0，流类型，视频流
- 00 2a，PES_packet_length，其后的42字节为数据内容长度
- 8c， 1000 1100
	- 10 固定2位字段
	- 00 1100 ，6个各1 bit的数据位
        - `program_stream_map`
        - `PES_scrambling_control`
        - `PES_priority`
        - `data_alignment_indicator`
        - `copyright`
        - `original_or_copy`
- 80,1000 0000
	- PTS_DTS_flags，2 bit ， 10 表示 PES 头部有 PTS 字段
- 09，PES_header_data_length，PES_packet_length（00 2a）后两个字节的位置
	- 表示其后9个字节是PES的头部
- 2f a5 e7 c6 09 ff ff ff fc，PES的头部
- 00 00 00 01 67 4d 00 2a 9d a8 l3 00 89 f9 66 e0 20 20 28 00 00 03 00 08 00 00 03 00 c4 20，PES的荷载
	- 00 00 00 01 67， SPS

- 后续是连续三个PES包，解析方式同上
	- 【00 00 01 e0 00 0e 8c 00 03 ff ff fc  00 00 00 01 68 ee 3c 80 】 
		- 00 00 00 01 68， PPS
	- 【00 00 01 e0 00 0e 8c 00 02 ff fc 00 00 00 01 06 e5 01 80 80 】
		- 00 00 00 01 06，SEI
	- 【 00 00 01 e0 ff c6 8c 00 05 ff ff ff ff fd 00 00 00 01 65 b8 
		00 00 25 4c09 ff f8 80 c1 67 81 f3 f6 5f ca 1f 85 93 34 62 
		06 3f 27 5b 44 d1 02 bd c0 a3 6e 3c 7f ed b7 ff 。。。
		- ff c6， 数据内容长度 65478个字节
		- 8c 00
			- 00， PTS_DTS_flags值为0，这个PES包不包含PTS和DTS信息
		- 05，  PES Header 头部数据长度为5
		- 00 00 00 01 65，IDR
			
*/
```

> - 封装H.264
>   - 如果是RTP数据包荷载PS封装流，则是annexB格式，具备起始头
>   - 如果是RTP数据包荷载ES流的话，就是AVCC格式
> - 解析到 I 帧的时候，至数据末尾肯定不到 1400 个字节，而最后一个 PES 包的长度字段标志为 65478 字节，可以确定的是因为 I 帧过大，进行了分包



```c
#define PES_HDR_LEN 19
static int gb28181_make_pes_header(char *pData, int stream_id, int payload_len, unsigned long long pts, unsigned long long dts)
{
    bits_buffer_s bitsBuffer;
    bitsBuffer.i_size = PES_HDR_LEN;
    bitsBuffer.i_data = 0;
    bitsBuffer.i_mask = 0x80;
    bitsBuffer.p_data = (unsigned char *)(pData);
    memset(bitsBuffer.p_data, 0, PES_HDR_LEN);
    
    /*System Header*/
    bits_write( &bitsBuffer, 24,0x000001); /*start code*/
    bits_write( &bitsBuffer, 8, (stream_id)); /*streamID*/
    bits_write( &bitsBuffer, 16,(payload_len)+13); /*packet_len pes剩余头部以及后面的es长度之和，比如SPS长度+13*/
    bits_write( &bitsBuffer, 2, 2 ); /*'10'*/
    bits_write( &bitsBuffer, 2, 0 ); /*scrambling_control*/
    bits_write( &bitsBuffer, 1, 1 ); /*priority*/
    bits_write( &bitsBuffer, 1, 1 ); /*data_alignment_indicator*/
    bits_write( &bitsBuffer, 1, 0 ); /*copyright*/
    bits_write( &bitsBuffer, 1, 0 ); /*original_or_copy*/
    bits_write( &bitsBuffer, 1, 1 ); /*PTS_flag 是否有PTS*/
    bits_write( &bitsBuffer, 1, 1 ); /*DTS_flag 是否有DTS信息*/
    bits_write( &bitsBuffer, 1, 0 ); /*ESCR_flag*/
    bits_write( &bitsBuffer, 1, 0 ); /*ES_rate_flag*/
    bits_write( &bitsBuffer, 1, 0 ); /*DSM_trick_mode_flag*/
    bits_write( &bitsBuffer, 1, 0 ); /*additional_copy_info_flag*/
    bits_write( &bitsBuffer, 1, 0 ); /*PES_CRC_flag*/
    bits_write( &bitsBuffer, 1, 0 ); /*PES_extension_flag*/
    bits_write( &bitsBuffer, 8, 10); /*header_data_length*/ 
    
    /*PTS,DTS*/    
    bits_write( &bitsBuffer, 4, 3 ); /*'0011'*/
    bits_write( &bitsBuffer, 3, ((pts)>>30)&0x07 ); /*PTS[32..30]*/
    bits_write( &bitsBuffer, 1, 1 );
    bits_write( &bitsBuffer, 15,((pts)>>15)&0x7FFF); /*PTS[29..15]*/
    bits_write( &bitsBuffer, 1, 1 );
    bits_write( &bitsBuffer, 15,(pts)&0x7FFF); /*PTS[14..0]*/
    bits_write( &bitsBuffer, 1, 1 );
    bits_write( &bitsBuffer, 4, 1 ); /*'0001'*/
    bits_write( &bitsBuffer, 3, ((dts)>>30)&0x07 ); /*DTS[32..30]*/
    bits_write( &bitsBuffer, 1, 1 );
    bits_write( &bitsBuffer, 15,((dts)>>15)&0x7FFF); /*DTS[29..15]*/
    bits_write( &bitsBuffer, 1, 1 );
    bits_write( &bitsBuffer, 15,(dts)&0x7FFF); /*DTS[14..0]*/
    bits_write( &bitsBuffer, 1, 1 );
    return 0;
}
```





### 注意

​		当对原始流进行PES分组，尤其是需要将一帧信息分成多个PES分组时，从第二个分组开始不需要PTS。

​		PES_header_data_length和它前面的一个字节又都为0x00，很可能与后面断开的数据组合形成0x00 00 01等类似的伪起始码或关键字，令解码端在收到流不完整时产生误判，因此填充字节stuffing_byte至少必须加入1 byte以确保这种误判不会发生。

​		PES Header长度由于是可变长度，可以简单用以下方法确认：

- PES Header长度=基本长度（9字节）+ 扩展长度（uface PS封装带PTS是7字节，不带PTS是3个字节）
  - 基本长度（8字节的长度+1字节的PES扩展头字节数）
  - 扩展长度填充规则是，至少2字节
- PES Header长度保持4字节对齐，PES分包后，只有首包含PTS



### 句法



![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/f655dba61708ecf066e06bac9330eea5.png#pic_center)



##   PES结构



​		节目流目录(PES)中字段的语义定义

| 字段                                 | bit  | 说明                                                         |
| ------------------------------------ | :--: | ------------------------------------------------------------ |
| packet_start_code_prefix             |  24  | 同跟随它的 stream_id 一起组成包起始码标识包的始端。 packet_start_code_prefix 为比0x000001 。 |
| stream_id                            |  8   | 用来区分PES分组负载，例如，0xE0~0xEF表示负载为视频PES。stream_id定义如下（bit表示）： 1110 xxxx（0xE0~0xEF） 视频PES（我们使用0xE0） 110x xxxx（0xC0~0xDF） 音频PES（我们使用0xC0） 1011 1110（0xBE） 填充PES（填充数据为0xff） 1011 1111（0xBF） 不含时标数据的私有PES，不需要与其他流同步时使用 |
| PES_packet_length                    |  16  | 此字段后PES分组的字节数。‘0’表明对PES分组长度既没有说明也没有限制，这种情况只有在PES分组的有效负载是TS流分组中的视频原始流时才允许。 |
| PES_scrambling_control               |  2   | 表示PES分组有效负载的加密模式，不加密时建议置0，否则置01，10和11保留。 |
| PES_priority                         |  1   | 指示PES分组负载的优先级。1高优先级，0低优先级。非参考帧（包括B帧和，E帧，P帧）应置0，其余帧置1。 |
| data_alignment_indicator             |  1   | 置１表明此PES分组头部之后是data_stream_alignment_descriptor所定义的访问单元数据类型。若为‘０’，则没有定义是否对准。建议当是输入单元的第一个包时置1，其余置0。 |
| copyright                            |  1   | 置‘１’，表明相应PES分组的有效负载中的数据是有版权的，建议置0。 |
| original_or_copy                     |  1   | 建议置1。                                                    |
| PTS_DTS_flag                         |  2   | 标志PES首部含有PTS, DTS的状态。‘1x’表明PES首部含有PTS,若为‘x1’, 则首部含有DTS。PTS, presentation_time_stamp, DTS, decoding_time_stamp, 建议置‘10’或‘00’。 |
| ESCR_flag                            |  1   | 置‘１’表示ESCR的base和extension字段都在PES分组首部出现；建议置0。 |
| ES_rate_flag                         |  1   | 置‘１’表示PES首部有ES_rate字段；建议置0。                    |
| DSM_trick_mode_flag                  |  1   | 置‘１’表示PES首部有 8bit的trick_mode_field；建议置0。        |
| additional_copy_info_flag            |  1   | 置‘１’表示有additional_copy_info字段；建议置0。              |
| PES_CRC_flag                         |  1   | 置‘１’表示在PES首部分组中有CRC字段；建议置0。                |
| PES_extension_flag                   |  1   | 置‘１’可选字段出现，当需要在每帧添加私有数据时建议置1，其余情况置0 |
| PES_header_data_length               |  8   | 表示PES分组首部中可选字段和填充字节的总字节数。              |
| marker_bit                           |  1   | 1                                                            |
| PTS                                  |  33  | Presentation Time Stamp，表示该PES所包含的元素流单元的显示时刻。 |
| PES_private_data_flag                |  1   | 置‘１’表示PES分组首部中含有私有数据；当需要在每帧添加私有数据时建议置1，其余情况置0 |
| pack_header_field_flag               |  1   | 置‘１’表示一个ISO/IEC 11172组首部或程序流组首部存储在此PES分组首部，建议置0。 |
| program_packet_sequence_counter_flag |  1   | 置‘１’表示PES分组中有program_packet_sequence_counter和original_stuff_length字段。建议置0。 |
| P-STD_buffer_flag                    |  1   | 置‘１’表示PES分组首部中有P-STD_buffer_scale和P-STD_buffer_size字段；建议置0。 |
| PES_private_data                     | 128  | 128bit用户私有数据，见可用扩展字段说明。                     |
| PES_extension_flag2                  |  1   | 置‘１’表示出现PES_extension_field; 建议置0。                 |
| stuffing_byte                        |  8   | 填充字节，根据标准编码其应当在此填入0xff，解码器端应当将这些字节抛弃。 |



