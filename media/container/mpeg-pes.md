PES

---

​		Packetized Elementary Stream， 包装基础流。主要用于在传输过程中提供完整的同步和错误检测能力。PES在数据包中包含了时间戳和其他信息，使其适合实时传输。



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

- `packet_start_code_prefix  `，3byte， 起始码，固定为 `0x00 00 01`
- `stream_id  `，1 byte， PES 包中的负载流类型
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



### optional pes header

​		可选包头中的控制信息有很多，这里我们只给出与 pts、dts 有关的结构：

- `PTS_DTS_flags`  ，2 bit 
  - `10 `表示 PES 头部有 PTS 字段
  - `11 `表示有 PTS 和 DTS 字段
  - `00 `表示都没有
  - `10 `被禁止
- `pts  `，40 bit ，显示时间戳， 实际 pts 长度占用 33 bit
- `dts`，40 bit ，解码时间戳，实际 dts 长度占用 33 bit

> ​		。DTS/PTS是相对SCR（系统参考）的时间戳，以 `90000 `为单位。
>
> ​		`PTS/DTS` 到 `ms`的转换公式是 `PTS/90`。系统时钟频率为` 90Khz`，所以转换到秒为 `PTS/90000`。



![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/f655dba61708ecf066e06bac9330eea5.png#pic_center)





##   PES结构

​		Packetised Elementray Stream, 原始流分组。

- Elementary Stream, 原始流，指的是视频或音频压缩编码后产生的比特数据
- 而视频或音频原始流分别打包加上PTS/DTS等头部信息生成视频PES和音频PES。
  - 一个PES分组只包含一个原始流的编码数据。



节目流目录(PES)中字段的语义定义

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

​		需要注意的是，当对原始流进行PES分组，尤其是需要将一帧信息断开分成多个PES分组时，从第二个分组开始不需要PTS，PES_header_data_length和它前面的一个字节又都为0x00，很可能与后面断开的数据组合形成0x00 00 01等类似的伪起始码或关键字，令解码端在收到流不完整时产生误判，因此填充字节stuffing_byte至少必须加入1 byte以确保这种误判不会发生。

​		备注：PES Header长度由于是可变长度，可以简单用以下方法确认：PES Header长度=基本长度（9字节）+扩展长度（uface PS封装带PTS是7字节，不带PTS是3个字节）；基本长度（8字节的长度+1字节的PES扩展头字节数），扩展长度填充规则是，至少2字节，PES Header长度保持4字节对齐，PES分包后，只有首包含PTS。

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

