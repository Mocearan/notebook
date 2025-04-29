# MPEG-TS

---

​		`MPEG-TS`一种音视频容器格式，其中 TS 表示 `Transport Stream`。`MPEG2-TS` 定义于` MPEG-2 `第一部分：系统（即` ISO/IEC `标准 `13818-1` 或` ITU-T Rec. H.222.0`）。

​		TS 主要是用于传输流，主要用在容易发生传输误码的网络传输环境中。以` .ts` 为文件后缀。

​		它可以实时传输多层节目节目内容，这就要求从传输流的任一片段开始都是可以独立解码的，在直播中可以用到。

> **也正是因为 `TS `任一切片开始都可以独立解码，所以它非常适合按切片的方式存储直播内容。**
>
> TS 流中不支持快进快退，这个需要从协议层来支持，比如 HLS 协议对相关的能力做了定义。
>
> 这些频道、节目、音视频码流如何在一个TS信道里面进行区分是 TS 封装格式复杂的主要原因。
>
> ​		互联网IP网络体系下传输的数字信号，和数字电视基站广播的数字信号没有本质区别：
>
> - 互联网使用`TS`封装，通常传一路视频和一路音频，`TS`中很多字段用不到
>   - 单节目的`TS `封装 叫做` Single Program Transport Stream (SPTS)`。
> - 机顶盒之类的开发，想深入理解数字电视的知识，可以看 《Video Demystified: A Handbook for The Digital Engineer》。

## 参考

​		[DVB-SI/PSI_Destiny青羽的博客-CSDN博客](https://blog.csdn.net/kkdestiny/category_1553561.html)





##  工具

- elecard stream analyzer，支持 TS，FLV，MP4 等众多格式，可免费试用 30天
- MPEG-2 TS packet analyser ，界面清晰简洁。



##  格式

​		TS 流的结构整体可以分为几个层次：

- **TS（Transport Stream） 层，即传输流层。**对分组化基本流数据打包，加上了用于网络传输相关的信息。
- **PES（Packetized Elementary Stream） 层，即分组化基本流。**主要在音视频数据的基础上带上了时间戳信息。
- **ES（Elementary Stream） 层，基本流层。**主要包含实际的音视频数据，一般视频为 `H.264` 编码数据，音频为 `AAC `编码数据。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/a1a7238bf4fe0132fbe9be6dc7d85715.png" alt="在这里插入图片描述" style="zoom: 67%;" />



​		

### TS 层

​		`TS`流是基于`TS Packet`的流格式。`TS `层的数据包大小固定，必须为 `188 `字节204个字节。其中包括：

​		`TS header[ fixed header +adaptation field]`+`payload`

- `TS Header` 又包括固定长度部分和可选部分，提供关于传输方面的信息

  - 固定部分长度为 `4` 字节

  - 可选部分包括适配域（Adaptation Field）

    - `adaptation`可能不存在，主要作用是给不足188字节的数据包做填充

    - `PES packet <= 184`字节时，则一个`TS`包就可以放下，如果还有空余的地方，就填充无实际用处的字节，使其满足`184`字节

      > 除去`4`字节`ts header`需要`184`字节

- `payload`可以是`AAC`、`H264`、 `PAT`、`PMT`等

  - `payload`是`pes`数据
  - `188 - 4 - adaptation_feild_size`就是`payload`大小
  - 如果 PES包 > 184字节，则需要多个TS包封装
    - 分包时，实际上是将一个PES包切片，所以`PES Header`只需要首个TS包封装即可

- `204`字节在`188`字节后加`16`字节的`CRC`校验数据，其他格式一样

- 每个`TS`都有`PID`

  - **ts 层的内容是通过 PID 值来标识的，主要内容包括：PAT 表、PMT 表、音频流、视频流。**

<img src="https://mmbiz.qpic.cn/mmbiz_jpg/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrp9AqlKWIwGjGvibcd114dHBc4h7R4piakKcJsMRG2xxahs8QGAia0sDvpg/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 80%;" />

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/aHR0cHM6Ly9pbWFnZXMyMDE1LmNuYmxvZ3MuY29tL2Jsb2cvMTAxNjI2Ni8yMDE3MDUvMTAxNjI2Ni0yMDE3MDUxNTE5NTU1MjgyMi0yMDQ5MjY5NTY4LnBuZw)



#### Header

​		`TS Header` 主要包含的数据主要是传输流的头信息，用于传输和包分组。这些数据包括固定长度部分和可选部分。

<img src="https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130809122755796" alt="img" style="zoom: 80%;" />

<img src="https://img-blog.csdnimg.cn/img_convert/70e9597779db9cca5e499a5a6e339ac5.png#pic_center" alt="在这里插入图片描述" style="zoom: 67%;" />



| 字段名称                     | 字节位置                | 描述                                                         |
| ---------------------------- | ----------------------- | ------------------------------------------------------------ |
| 同步字节（Sync Byte）        | 第1个字节               | 固定为0x47，用于标识TS包的开始。                             |
| 传输错误指示（TEI）          | 第2个字节，第1位        | 当设置为1时，表示当前包中至少有一个不可纠正的错误位。        |
| 有效负载单元起始指示（PUSI） | 第2个字节，第2位        | 当设置为1时，表示当前包的有效负载部分以一个PES包或PSI/SI表的第一个字节开始。 |
| 传输优先级（Priority）       | 第2个字节，第3位        | 用于传输机制，但在解码时通常不被使用。                       |
| 包标识符（PID）              | 占13位                  | 用于标识TS包属于哪个特定的流或服务。                         |
| 传输加扰控制（Scrambling）   | 第4个字节，第1位和第2位 | 用于指示TS包是否被加扰以及加扰的方式。                       |
| 自适应字段控制（Adaptation） | 第4个字节，第3位和第4位 | 用于指示TS包是否包含自适应字段，以及自适应字段的位置和长度。 |
| 连续计数器（Counter）        | 第4个字节，第5位到第8位 | 用于检测包的丢失或重复，范围从0到15，每发送一个TS包，计数器加1。 |



##### fixed header

​		 `4 `字节

```c
typedef struct MPEGTS_FIXED_HEADER
{
	/* byte 0 */
	unsigned sync_byte : 8; // 同步字节，值为 0x47

	/* byte 1, 2 */
	unsigned transport_error_indicator : 1; // 传输错误指示位，置 1 时，表示传送包中至少有一个不可纠正的错误位
	unsigned payload_unit_start_indicator : 1; // 负载单元起始指标位，针对不同的负载，有不同的含义
	unsigned transport_priority : 1; // 传输优先级，表明该包比同个 PID 的但未置位的 TS 包有更高的优先级
	unsigned PID : 13; // 该 TS 包的 ID 号，如果净荷是 PAT 包，则 PID 固定为 0x00

	/* byte 3 */
	unsigned transport_scrambling_control : 2; // 传输加扰控制位
	unsigned adaptation_field_control : 2; // 自适应调整域控制位，分别表示是否有调整字段和负载数据
	unsigned continuity_counter : 4;// 连续计数器，随着具有相同 PID 的 TS 包的增加而增加，达到最大时恢复为 0
	/* 如果两个连续相同 PID 的 TS 包具有相同的计数，则表明这两个包是一样的，只取一个解析即可。 */
} MPEGTS_FIXED_HEADER; // MPEG-TS 包头占 4 字节
```

- `sync_byte （8 bit）`  ，同步字节，固定 `8 bit`

  - 一般为 `0100 0111 (0x47)` ，方便在某些场景下进行同步操作
    - 后面包中的数据不会出现 `0x47 `，以避免冲突
    - 表示后面是一个 `TS `分组，用于建立发送端和接收端包的同步
  - 是包中的第一个字节
  
  ```c
  MPEG_transport_stream()
  {
  	do {
  		transport_packet()
      } while(nextbits() == sync_byte) // sync_byte == 0x47
  }
  ```

- `tansport_error_indicator（1bit）`：传输错误标志位， `1 bit`

  - 发送时（调制前）值为 0。
  - 接收方的解调器在无法成功解调（即使有前向纠错机制）TS 分组内容时，将该位设置为 1，表示该 TS 分组损坏。
  - 当被置 1 后，在错误被纠正之前不能重置为 0。

- `payload_unit_start_indicator`（1bit）：负载起始标识，`1 bit`

  - 针对不同的负载，有不同的含义。

  - PES：
    - 置为 1， `PES `包的第一个字节开始，即此 TS 包为 PES 包的起始包，且此 TS 分组中有且只有一个 PES 包的起始字段

    - 置为 0，表示 TS 包不是 PES 包的起始包，是后面的数据包。

  - PSI：
    - 置为 1，`PSI `的第一个字节，即这个 TS 包是 PSI 的起始包
      - 则此 TS 包的负载的第一个字节带有 `pointer_field`，用来指示 PSI 数据在 `payload `中的位置；

    - 置为 0，表示 TS 包不带有 PSI 的第一个字节，即此 TS 包不是 `PSI `的起始包
      - 即在有效负载中没有 `pointer_filed`，有效负载的开始就是 `PSI `的数据内容

  - 对于空包，`payload_unit_start_indicator` 应该置为 0

  > ​		若 TS 包载荷为 `PAT`
  >
  > - 则当接收到 `TS `包的 `payload_unit_start_indicator `为 `1 `时，表明这个 `TS `包包含了 `PAT `头信息
  > - 从这个包里面解析出 `section_length `和 `continuity_counter`
  > - 然后继续收集后面的` payload_unit_start_indicator = 0 `的 `TS `包
  > - 并判断 `continuity_counter `的连续性，不断读出 TS 包中的净载荷（也就是 `PAT `数据）
  > - 用 `section_length `作为收集 `TS `包结束条件

- `transport_priority（1bit）`：传输优先标志，`1 bit`

  - 为 1 表明当前 TS 包的优先级比其他具有相同 `PID`，但此位没有被置 `1 `的 `TS `包高

    > 在相同 PID 的分组中具有更高的优先权。传输机制可以使用该字段优先考虑基本流内的该包数据。

- `PID（playload ID）(13 bit)`， 表示了`Packet`的类型，是`TS`流中唯一识别标志，识别 TS 分组，`13 bit`

  - 一个 PID 对应一个特定的 PES

    > Packet Data 是什么内容就是由`PID`决定的
    >
    > 在 TS 码流生成时，每一类业务（视频，音频，数据）的基本码流均被赋予一个不同的识别号 `PID`，解码器借助于 `PID `判断某一个 `TS `包属于哪一类业务的基本码流

  - `0x0000`，PAT表
    
    - PMT表的PID在PAT表中给出
  - `0x0001`，CAT
  - `0x0002`，TSDT
    - 传送流描述表（transport stream description table, TSDT）
  - `0x0003~0x000F`，保留
  - `0x0010~0x1FFE`，自由分配
    - `0x0012`，EIT,ST
    - `0x0013`，RST,ST
    - `0x0014`，TDT,TOT,ST
  - ...

- `transport_scrambling_control（2bits）`：有效负载加扰模式标志，`2 bit`

  - `00` 表示载荷未加扰。
    - 如果传输包包头中包括调整字段，不应被加密。
  - 其余值由具体系统定义。

- `adaption_field_control（2bits）`：适配域存在标志，`2 bit`

  - 表示此 `ts header`是否有可选头部和负载数据
  - `00`：保留；
  - `01`：表示无调整字段，只有有效负载数据；
  - `10`：表示只有调整字段，无有效负载数据；
  - `11`：表示有调整字段，且其后跟随有效负载数据；

- `continuity_counter（4bits）`：连续性计数器， `4 bit`

  - 取值为 `0x00` 到 `0x0F`，循环使用
  - 用于检查同一个 `PID `的 `TS `分组的连续性。每当一个 `TS `分组中包含载荷时，该计数器加 `1`
  - 在接收终端，如发现循环计数器的值有中断，表明数据在传输中有丢失

  

  

```c
// Packet header
//------bit 
//| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32| ...
//------2进制Packet---------------------------------------------------------------------------------------
//| 0| 1| 0| 0| 0| 1| 1| 1| 0| 0| 0| 0| 0| 1| 1| 1| 1| 1| 1| 0| 0| 1| 0| 1| 0| 0| 0| 1| 0| 0| 1| 0|
//------16进制Packet--------------------------------------------------------------------------------------
//|     4     |      7    |    0      |    7      |    e      |     5     |    1      |    2      | ...
//Header 信息--------------------------------------------------------------------------------------
//|1 sync_byte=0x47       | 2| 3| 4| 5 PID=0x07e5                         | 6   |  7  |  8        |

/*
sync_byte=01000111,                  就是0x47,这是DVB TS规定的同步字节,固定是0x47.
transport_error_indicator=0,         表示当前包没有发生传输错误.
payload_unit_start_indicator=0,      含义参考ISO13818-1标准文档
transport_priority=0,                表示当前包是低优先级.
PID=00111 11100101即0x07e5,       	Video PID
transport_scrambling_control=00,     表示节目没有加密
adaptation_field_control=01          即0x01,具体含义请参考ISO13818-1
continuity_counte=0010               即0x02,表示当前传送的相同类型的包是第3个
*/
```



##### adaptation field

​		适配域（Adaptation Field）中的数据主要用于携带一些额外信息以及做数据填充以确保 TS 的一个包大小固定在 188 字节。

- 当` TS Header` 的 `adaption_field_control `字段值为 `10` 或 `11` 时，说明该` TS packet `有适配域（Adaptation Field）。
- 适配域的格式基于采用若干标志符，以表示该字段的某些特定扩展是否存在
- 适配域由适配域长度、不连续指示器、随机存取器、PCR标志符、基本数据流优先级指示器、拼接点标志符、传送专用数据标志、调整字段扩展标志以及有相应标志符的字段组成
- `adaptation field `最少占用 `1 `个字节
  - 即` adaptation_field_length `字段，这样最少可以填充 1 个字节。


<img src="https://img-blog.csdnimg.cn/img_convert/34f29e448f68767e3d0b784f7692d4b2.png#pic_center" alt="在这里插入图片描述" style="zoom: 67%;" />

| 字段                                 | 大小   | 描述                             |
| ------------------------------------ | ------ | -------------------------------- |
| adaptation_field_length              | 8 bit  | 调整字段长度                     |
| discontinuity_indicator              | 1 bit  | 不连续指示器，一般为 0           |
| random_access_indicator              | 1 bit  | 随机存取器，一般为 0             |
| elementary_stream_priority_indicator | 1 bit  | 基本数据流优先级指示器，一般为 0 |
| PCR_flag                             | 1 bit  | PCR标志符，携带 pcr 时置 1       |
| OPCR_flag                            | 1 bit  | OPCR标志符，一般为 0             |
| splicing_point_flag                  | 1 bit  | 拼接点标志符，一般为 0           |
| transport_private_data_flag          | 1 bit  | 传送专用数据标志，一般为 0       |
| adaptation_field_extension_flag      | 1 bit  | 调整字段扩展标志，一般为 0       |
| PCR                                  | 40 bit | 当 PCR_flag=1 时携带             |
| stuffing_bytes                       | 不定   | 填充字节，取值0xff               |

- `adaptation_field_length`，适配域部分的长度，8 比特
  - 单位为字节，不包含当前字段字节。
  - 0 值表示传输流包中插入单个填充字节。
  - 当 `adaptation_field_control` 值为 `11` 时，此时表示后面有载荷，`adaptation_field_length` 值必须在 0 到 182 的区间内。
  - 当 `adaptation_field_control` 值为 `10` 时，此时表示后面无载荷，`adaptation_field_length` 值必须为 183。
  - 对于承载 PES 包的传输流包，只要存在欠充足的 PES 包数据就需要通过填充来完全填满传输流包的有效载荷字节。
- `discontinuity_indicator`，不连续指示位，1 比特。如果根据连续性计数器或 PCR 计算，确认当前分组处于不连续状态，则取值为 1。不连续性指示符用于指示两种类型
  的不连续性，系统时间不连续性和 `continuity_counter` 不连续性。
- `random_access_indicator`，随机访问指示位，1 比特。如果当前分组是一个 PES 的起始，取值为 1。
- `elementary_stream_priority_indicator`，ES 优先级指示位，1 比特。表示有效载荷内承载的基本流数据的优先级，`1` 表示该有效载荷具有
  比其他传输流包有效载荷更高的优先级。
- `PCR_flag`，PCR 标识位，1 比特。1 表示适配域存在 PCR 数据。
- `program_clock_reference_base` + `program_clock_reference_extension`，即 PCR（Program Clock Reference）数据。结构为 33 位的基础部分 + 9 位的扩展部分，共 42 比特。PCR 表示包含 `program_clock_reference_base` 最后比特的字节到达系统目标解码器输入端的预期时间。它用于时钟同步，使得解码后的内容可以正确地同步播放。
- `splice_countdown`，倒数计数器，8 比特。可以为正或负的值。正值表示传输流中有相同 PID 的距离拼接点剩余传输流包的数量。复制的传输流包和仅包含自适应字段的传输流包被排除。相关 `splice_countdown` 字段达到零的传输流包中的最后字节之后的位置即为拼接点的定位。在 `splice_countdown` 达到零的传输流包中，传输流包有效载荷的最后数据字节必须是编码音频帧或编码图像的最后字节。
- `private_data_byte`，私有数据，8 比特。可包含多组。
- `splice_type`，拼接类型，4 比特。从此字段的首次出现向前，在该字段存在的相同 PID 的所有连续传输流包中，它都具有相同的值，直至 `splice_countdown` 达到零的包出现时为止（包括该包）。
- `DTS_next_AU`，解码时间标记下一个存取单元，以 3 部分编码，共 33 比特。在连续和周期解码通过此拼接点的情况中，它表示跟随拼接点的第一个存取单元的解码时间。此解码时间以时间基表示，在 `splice_countdown` 字段达到零的传输流包中生效。从首次出现此字段向前，在它存在的相同 PID 的所有后续传输流中该字段均必须有相同值，直到 `splice_countdown` 字段达到零的包出现时为止（包括此包）。
- `stuffing_byte`，填充数据，8 比特。可包含多组。



​	<img src="https://img-blog.csdnimg.cn/img_convert/1f11d1c0a32eee64a1d5b00f836a64ae.png#pic_center" alt="在这里插入图片描述" style="zoom:67%;" />

​		针对不同的` ts payload`，`adaptation field `的应用方式也不同：

- 针对 `PAT`、`PMT`，不足` 188 Bytes` 的部分直接使用 `0xff `进行填充，而不会使用` adaptation field`（但是也有例外，有的编码器会携带）。

- 针对 `PES packe`t，才会使用` adaptation field `做填充。
  - `audio PES packet` 不会在 `adaptation field `中携带 `PCR `字段
  
  - `video PES packet `可以选择是否在` adaptation field` 中携带 `PCR `字段
  
    - 一般 `PES packet` 被拆分时，会在第一个和最后一个拆分包添加 `adaptation field`
    - 第一个拆分包的 `adaptation field` 才会携带 `pcr `时钟，且主要作用是为了携带 `pcr `时钟，而不是为了填充数据；
    - 最后一个拆分包的 `adaptation field `不会携带 `pcr `时钟，只做填充用。
  
    > `PCR `属于编码端的时钟，
    >
    > 作用是如果编码端时钟源与解码端时钟源不同步，那么解码端应该采用 `pcr `作为自己的时钟源，以同步编码端。
    >
    > 例如，编码端时钟是解码端的 2 倍，解码端是正常的物理时钟，这时一个物理世界 `5min `的视频，因为编码端时钟源走得太快，那么最后一个视频帧的 `dts `就是` 10min`。播放端直接播放的话，就会播放 `10min`，显得播放得很慢。所以播放端需要加快播放，方法就是采用 `pcr `时钟作为自己的时钟源，让自己的时钟走得跟编码端一样快，这样看起来就是正常速度播放了。



#### payload

​		当 TS Header 中的 `adaptation_field_control` 字段值为 `01` 或 `11` 时，这就表明存在载荷（Payload Data）部分。

​		载荷部分对应的是 `TS `层数据结构的 `data_byte` 字段。

- `data_byte`，该字段 8 比特一个单位，可包含多组。
  - 该数据必须来自 PES 包、PSI 分段以及 PSI 分段后的包中相连贯的数据字节，或者不在这些结构中的专用数据。
  - 如 PID 所指示的。在具有 PID 值`0x1FFF` 的空包情况中，`data_bytes` 可以指派为任何值。
  - `data_bytes` 数 N 通过 184 减去 `adaptation_field()` 中的字节数来确定。



### PES 层

<img src="https://mmbiz.qpic.cn/mmbiz_jpg/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpraThFDwwB1raYYzTkvgj4UryWc617pzoKFAP0D9PtZb23CZTtDBFVA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 80%;" />



PES 包中的部分字段解释：

- `stream_id`，表示基本流的类型和编号。
- `PES_packet_length`，表示 PES 包中在该字段后的数据字节数，该字段 16 比特。
- `PTS`，表示显示时间戳。分为 3 段，共 33 比特。
- `DTS`，表示解码时间戳。分为 3 段，共 33 比特。
- `ES_rate`，基本流速率，在 PES 流情况中，指定解码器接收 PES 包字节的速率。
- `trick_mode_control`，表示相关视频的特技方式，3 比特字段。这些特技方式包括：快进、慢动作、冻结帧、快速反向、慢反向等。
- `PES_packet_data_byte`，表示来自包 stream_id 或 PID 所指示的基本流的连贯数据字节。该字段 8 比特一个单位。

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpJGpibcU0mAd5jDcxNv2J5bPpNpnmHXuZFAibxuuu7QQCyGMnJs3KPBNw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpVfrWnvGnl1637Z0nj0uDSAQEw2D89Jd5BibFEy6OH8tjN92YUNO1ERQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehz5Hg9nJv4kicOZSBYolBrpOCIySER5GzNyaXdlfFHYaTPp8ZuyFyVicUWianPSeYQpq9jE96iaFXxIw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### ES层

ES 层指的就是音视频基本数据，一般视频为 H.264 编码数据，音频为 AAC 编码数据。

## PSI

​		[DVB-SI/PSI_Destiny青羽的博客-CSDN博客](https://blog.csdn.net/kkdestiny/category_1553561.html)

​		Program Specific Information，节目特定信息，用来描述`TS`的组成结构。

- TS 包是对 `PES `包的封装，但是不只是 `PES`，`TS `还可以是对 `PSI `数据的封装
-  `PSI `不是一个表，`PSI `是 `PAT`，`PMT`，`CAT`，`NIT`...这些表的统称。

​		`PSI`信息由四种类型的表组成，包括

- 节目关联表（PAT，Program Association Table）
  - 0x0000  解析 .ts 文件的第一步就是找到 PAT 表，然后获取 PMT 表的 PID 值
- 节目映射表（PMT，Program Map Table）
  - 在 PAT 中给出  根据 PMT 表找到 audio pes packet 和 video pes packet 的 PID 值
- 条件接收表（CAT，Conditional Access Table）
  - 0x0001  将一个或多个专用EMM流分别与唯一的PID相关联，描述了TS流的加密方式
- 网络信息表（NIT，Network Infomation Table）
  - PAT 中给出  描述整个网络，如多少个TS流，频点和调制方式等

> ​		PAT与PMT两张表帮助我们找到该传送流中的所有节目与流。PAT告诉我们，该TS流由哪些节目组成，每个节目的节目映射表PMT的PID是什么；而PMT告诉我们，该节目由哪些流组成，每一路流的类型与PID是什么。
>
> - 在解析TS流的时候，首先寻找PAT表
> - 根据PAT获取所有PMT表的PID；再寻找PMT表，获取该频段所有节目数据并保存。
> - 这样，只需要知道节目的PID就可以根据`Packet Heade`给出的`PID`过滤出不同的Packet，从而观看不同的节目。

> - 解析 `ts `流要先找到 `PAT `表，只要找到 `PAT `就可以找到 `PMT`，然后就可以找到音视频流了
> - `PAT `表的和 `PMT `表需要定期插入 `ts `流
>   - 因为用户随时可能加入 `ts `流，这个间隔比较小，通常每隔几个视频帧就要加入 `PAT `和 `PMT`
> - `PAT `和 `PMT `表是必须的，还可以加入其它表如 `SDT`（业务描述表）等
> - 不过 `HLS `流只要有 `PAT `和 `PMT `就可以播放了
>
> > - PAT 表：主要的作用就是指明了 PMT 表的 PID 值。
> > - PMT 表：主要的作用就是指明了音视频流的 PID 值。

### PAT表

​		Program Association Table，节目关联表， PID为`0x0000`。

​		PAT表描述了当前流的NIT（Network Information Table，网络信息表）中的PID、当前流中有多少不同类型的PMT表及每个PMT表对应的频道号。

​		作用是：

- 定义了当前TS流中所有的节目和节目编号
- 以及对应节目的PMT的位置，既PMT的TS包的PID值
- 同时还提供NIT的位置，即NIT的TS包的PID值

#### PAT表描述

​		定义了当前TS流中所有的节目

- PID为0x0000，它是PSI的根节点，要查寻找节目必须从PAT表开始查找
- PAT表主要包含频道号码和每一个频道对应的PMT的PID号码，以流ID唯一标识一个TS流：

| 字段名                                                       | 位   | 具体值                      | 次序                                                         | 说明                    |
| ------------------------------------------------------------ | ---- | --------------------------- | ------------------------------------------------------------ | ----------------------- |
| table_id                                                     | 8    | 0000                        | 第1个字节 0000 0000B(0x00)                                   | PAT的table_id只能是0x00 |
| section_syntax_indicator                                     | 1    | 1                           | 第2、3个字节1011 0000 0001 0001B(0xb0 11)                    | 段语法标志位，固定为1   |
| zero                                                         | 1    | 0                           |                                                              |                         |
| reserved                                                     | 2    | 11                          |                                                              |                         |
| section_length                                               | 12   | 0000 0001 0001B=0x011=17    | 段长度为17字节                                               |                         |
| **transport_stream_id**                                      | 16   | 0x0001                      | 第4、5个字节 0x00 0x01                                       |                         |
| reserved                                                     | 2    | 11                          | 第6个字节 1100 0001B(0xc1)                                   |                         |
| version_number                                               | 5    | 00000                       | 一旦PAT有变化，版本号加1                                     |                         |
| current_next_indicator                                       | 1    | 1                           | 当前传送的PAT表可以使用，若为0则要等待下一个表               |                         |
| section_number                                               | 8    | 0x00                        | 第7个字节0x00                                                |                         |
| last_section_number                                          | 8    | 0x00                        | 第8个字节 0x00                                               |                         |
| 开始循环--------------------------                           |      |                             |                                                              |                         |
| **program_number**                                           | 16   | 0x0000-第一次               | 2个字节(0x00 00)                                             | **节目号**              |
| reserved                                                     | 3    | 111                         | 2个字节1110 0000 0001 1111B(0xe0 1f)                         |                         |
| network_id(节目号为0时)<br />**program_map_PID**(节目号为其他时) | 13   | 0 0000 0001 1111B=31-第一次 | **节目号**为0x0000时,表示这是NIT，PID=0x001f，即31<br />**节目号**为0x0001时,表示这是PMT，PID=0x100，即256 |                         |
| 结束循环---------------------------                          |      |                             |                                                              |                         |
| CRC_32                                                       | 32   | --                          | 4个字节                                                      |                         |

> ​		PAT表描述了当前流的NIT（Network Information Table，网络信息表）中的PID、当前流中有多少不同类型的PMT表及每个PMT表对应的频道号。

- transport_stream_id，该ID标志唯一的流ID
- program_number，该号码标志Ｓ流中的一个频道
  - 该频道可以包含很多的节目(即可以包含多个Video PID和Audio PID)
- program_map_PID，*节目映射表的PID*
  - 因为可以有很多的频道,因此DVB规定PMT的PID可以由用户自己定义

```c
Packet Header		 |		 Packet Data
0x47 0x40 0x00 0x10	 |	0000 b0 11 00 01 c1 00 00 00 00 e0 1f 00 01 e1 00 24 ac48 84 ff ff…… ff ff
    
//* header
// bit 
//| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32| ...
//2进制Packet---------------------------------------------------------------------------------------
//| 0| 1| 0| 0| 0| 1| 1| 1| 0| 1| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 1| 0| 0| 0| 0|
//16进制Packet--------------------------------------------------------------------------------------
//|     4     |      7    |    4      |    0      |    0      |     0     |    1      |    0      | ...
//Header 信息--------------------------------------------------------------------------------------
//|1 sync_byte=0x47       | 2| 3| 4| 5 PID=0x0000                         | 6   |  7  |  8        |

/*
sync_byte 					0x47 		固定同步字节
transport_error_indicator 	 “0” 		 没有传输错误
payload_unit_start_indicator “1” 		 在前4个字节后会有一个调整字节。所以实际数据应该为去除第一个字节后的数据。
transport_priority 			“0” 		传输优先级低
PID 						0x0000 		PID=0x0000说明数据包是PAT表信息
transport_scrambling_control  “00” 		 未加密
adaptation_field_control 	 “01” 		 附加区域控制
continuity_counte 			“0000” 		包递增计数器
*/
    
//* Packet Data
00 | 00 b0 11 00 01 c1 00 00 00 00 e0 1f 00 01 e1 00 24 ac48 84 ff ff…… ff ff
// payload_unit_start_indicator="1", 去除第一个字节 00 才是有效的数据    
// PAT
// byte
// |00|b0|11|00|01|c1|00|00|00|00|e0|1f|00|01|e1|00|24|ac|48|84|ff|ff|……|ff|ff
```



#### PAT表的结构

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/8460cdc6e2f75a0bbfe03e871204c182.webp?x-oss-process=image/format,png#pic_center)

```c++

typedef struct TS_PAT_Program  
{  
    unsigned program_number   :  16;  //节目号  
    unsigned Reserved		 :3;   //保留位
    union map_or_net {
        unsigned program_map_PID :  13; // 节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个  
        unsigned network_PID : 13; //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID 
    };
    
}TS_PAT_Program;

// PAT表的结构
typedef struct TS_PAT  
{  
    unsigned table_id                     : 8; //固定为0x00 ，标志是该表是PAT表  
    unsigned section_syntax_indicator     : 1; //段语法标志位，固定为1  
    unsigned zero                         : 1; //0  
    unsigned reserved_1                   : 2; // 保留位  
     unsigned section_length               : 12; //表示从下一个字段开始到CRC32(含)之间有用的字节数  
    unsigned transport_stream_id          : 16; //该传输流的ID，区别于一个网络中其它多路复用的流  
    unsigned reserved_2                   : 2;// 保留位  
    unsigned version_number               : 5; //范围0-31，表示PAT的版本号  
    unsigned current_next_indicator       : 1; //发送的PAT是当前有效还是下一个PAT有效  
    unsigned section_number               : 8; //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段  
    unsigned last_section_number          : 8;  //最后一个分段的号码  
   
    std::vector<TS_PAT_Program> program;  
    //unsigned program_map_PID			:16
    //unsigned reserved_3                    : 3; // 保留位  
    //unsigned network_PID or program_map_PID : 13; //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID  
    unsigned CRC_32                        : 32;  //CRC32校验码  
} TS_PAT;   
```

```c++
// 解析PAT表
HRESULT CTS_Stream_Parse::adjust_PAT_table( TS_PAT * packet, unsigned char * buffer)  
{  
    packet->table_id                    = buffer[0];  
    packet->section_syntax_indicator    = buffer[1] >> 7;  
    packet->zero                        = buffer[1] >> 6 & 0x1;  
    packet->reserved_1                  = buffer[1] >> 4 & 0x3;  
    packet->section_length              = (buffer[1] & 0x0F) << 8 | buffer[2];   
   
    packet->transport_stream_id           = buffer[3] << 8 | buffer[4];  
   
    packet->reserved_2                    = buffer[5] >> 6;  
    packet->version_number                = buffer[5] >> 1 &  0x1F;  
    packet->current_next_indicator        = (buffer[5] << 7) >> 7;  
    packet->section_number                = buffer[6];  
    packet->last_section_number           = buffer[7];   
  
    int len = 0;  
    len = 3 + packet->section_length;  
    packet->CRC_32                        = (buffer[len-4] & 0x000000FF) << 24  
                                                  | (buffer[len-3] & 0x000000FF) << 16  
                                                  | (buffer[len-2] & 0x000000FF) << 8   
                                                  | (buffer[len-1] & 0x000000FF);   
   
    int n = 0;  
    //   从for开始，就是描述了当前流中的频道数目(N)，每一个频道对应的PMT PID是什么。
    // 解复用程序需要接收所有的频道号码和对应的PMT 的PID，并把这些信息在缓冲区中保存起来。在后部的处理中需要使用到PMT的 PID。
    for ( n = 0; n < packet->section_length - 12; n += 4 )  
    {  
        unsigned  program_num = buffer[8 + n ] << 8 | buffer[9 + n ];    
        packet->reserved_3           = buffer[10 + n ] >> 5;   
    
        packet->network_PID = 0x00;  
        if ( program_num == 0x00)  
       {    
            packet->network_PID = (buffer[10 + n ] & 0x1F) << 8 | buffer[11 + n ];  
  
            TS_network_Pid = packet->network_PID; //记录该TS流的网络PID  
  
             TRACE(" packet->network_PID %0x /n/n", packet->network_PID );  
        }  
        else  
        {  
           TS_PAT_Program PAT_program;  
           PAT_program.program_map_PID = (buffer[10 + n] & 0x1F) << 8 | buffer[11 + n];  
           PAT_program.program_number = program_num;  
           packet->program.push_back( PAT_program );  
           TS_program.push_back( PAT_program );//向全局PAT节目数组中添加PAT节目信息       
        }           
    }  
    return 0;  
}  
```

```c++
// 过滤PAT表信息的伪代码
int Video_PID=0x07e5,Audio_PID=0x07e6;
void Process_Packet(unsigned char*buff)
{
    int I; int PID=GETPID(buff);
    if(PID==0x0000) { Process_PAT(buff+4); }  // 如果PID为0x0000,则该Packet Data为PAT信息，因此调用处理PAT表的函数
    else{                                     // 这里buff+4 意味着从Packet Header之后进行解析（包头占4个字节）
        ……
    }
}
```



### PMT 

​		Program Map Table，节目映射表。如果一个TS流中含有多个频道，那么就会包含多个PID不同的PMT表。

​		用于:

- 指示组成某一套节目的视频、声频和数据在传送流中的位置，即对应的TS包的PID值
- 以及每路节目的节目时钟参考（PCR）字段的位置

 		 PMT表中包含的数据如下：

- 当前频道中包含的所有Video数据的PID
- 当前频道中包含的所有Audio数据的PID
- 和当前频道关联在一起的其他数据的PID(如数字广播,数据通讯等使用的PID)



#### PMT表描述

| 字段名                   | 位数 | 具体值               | 次序                                                         | 说明                                    |
| ------------------------ | ---- | -------------------- | ------------------------------------------------------------ | --------------------------------------- |
| table_id                 | 8    | 0x02                 | 第1个字节                                                    |                                         |
| section_syntax_indicator | 1    | 1B                   | 第2、3个字节1011 0000 0001 0010B=0xb012                      | 段语法标志                              |
| zero                     | 1    | 0B                   |                                                              |                                         |
| reserved                 | 2    | 11B=0x03             |                                                              |                                         |
| section_length           | 12   | 0000 0001 0010B=0x12 | 段长度,从program_number开始,到CRC_32(含)的字节总数           |                                         |
| program_number           | 16   | 0x0001               | 第4、5个字节 0x00 01                                         | 频道号码,表示当前的PMT关联到的频道      |
| reserved                 | 2    | 11B=0x03             | 第6个字节1100 0001B=0xc1                                     |                                         |
| version_number           | 5    | 00000B=0x00          | 版本号码,如果PMT内容有更新,则它会递增1通知解复用程序需要重新接收节目信息 |                                         |
| current_next_indicator   | 1    | 1B=0x01              | 当前未来标志符                                               |                                         |
| section_number           | 8    | 0x00                 | 第7个字节0x00                                                | 当前段号码                              |
| last_section_number      | 8    | 0x00                 | 第8个字节 0x00                                               | 最后段号码,含义和PAT中的对应字段相同    |
| reserved                 | 3    | 111B=0x07            | 第9、10个字节1110 0011 1110 1001B=0xe3e9                     |                                         |
| PCR_PID                  | 13   | 000111110B=0x3e9     | PCR(节目参考时钟)所在TS分组的PID                             |                                         |
| reserved                 | 4    | 1111B=0x0f           | 第11、12个字节1111 0000 0000 0000=0xf000                     |                                         |
| program_info_length      | 12   | 000000000000B=0x000  | 节目信息长度(之后的是N个描述符结构,一般可以忽略掉,这个字段就代表描述符总的长度,单位是Bytes)紧接着就是频道内部包含的节目类型和对应的PID号码了 |                                         |
| stream_type              | 8    | 0x1b                 | 第13个字节 0x1b                                              | 流类型,标志是Video还是Audio还是其他数据 |
| reserved                 | 3    | 111B=0x07            | 第14、15个字节1110 0011 1110 1001B=0xe3e9                    |                                         |
| elementary_PID           | 13   | 000111110 1001=0x3e9 | 该节目中包括的视频流，音频流等对应的TS分组的PID              |                                         |
| reserved                 | 4    | 1111B=0x0f           | 第16、17个字节1111 0000 0000 0000B=0xf000                    |                                         |
| ES_info_length           | 12   | 0000 0000 0000=0x000 |                                                              |                                         |
| CRC                      | 32   | ——                   | ——                                                           |                                         |

```c
Packet Header		 |		 Packet Data
0x47 0x43 0xe8 0x12	 |	00 02 b0 12 00 01 c1 00 00 e3 e9 f0 00 1b e3 e9 f0 00 f0 af b4 4f ff ff…… ff ff
    
//* header
// bit 
//| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32| ...
//2进制Packet---------------------------------------------------------------------------------------
//| 0| 1| 0| 0| 0| 1| 1| 1| 0| 1| 0| 0| 0| 0| 1| 1| 1| 1| 1| 0| 1| 0| 0| 0| 0| 0| 0| 1| 0| 0| 1| 0|
//16进制Packet--------------------------------------------------------------------------------------
//|     4     |      7    |    4      |   3       |    e      |     8     |    1      |    2      | ...
//Header 信息--------------------------------------------------------------------------------------
//|1 sync_byte=0x47       | 2| 3| 4| 5 PID=0x03e8                         | 6   |  7  |  8        |

/*
sync_byte 					0x47 		固定同步字节
transport_error_indicator 	 “0” 		 没有传输错误
payload_unit_start_indicator “1” 		 在前4个字节后会有一个调整字节。所以实际数据应该为去除第一个字节后的数据。
transport_priority 			“0” 		传输优先级低
PID 					   0x03e8 	    PID=0x03e8说明数据包是PMT表信息
transport_scrambling_control  “00” 		 未加密
adaptation_field_control 	 “01” 		 附加区域控制
continuity_counte 			“0010” 		包递增计数器
*/
    
//* Packet Data
00 | 02 b0 12 00 01 c1 00 00 e3 e9 f0 00 1b e3 e9 f0 00 f0 af b4 4f ff ff…… ff ff
// payload_unit_start_indicator="1", 去除第一个字节 00 才是有效的数据    
// PAT
// byte
// 02 b0 12 00 01 c1 00 00 e3 e9 f0 00 1b e3 e9 f0 00 f0 af b4 4f ff ff…… ff ff
```



#### PMT结构

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/fd157a20be24c0e26aedb0da27d78398.webp?x-oss-process=image/format,png#pic_center)

```c++
typedef struct TS_PMT_Stream
{
     unsigned stream_type                       : 8; //指示特定PID的节目元素包的类型。该处PID由elementary PID指定
     unsigned reserved						  : 3;
     unsigned elementary_PID                    : 13; //该域指示TS包的PID值。这些TS包含有相关的节目元素
    unsigned reserved						  : 4;
     unsigned ES_info_length                    : 12; //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数
     unsigned descriptor; 	// for : descriptor
}TS_PMT_Stream; 


//PMT 表结构体
typedef struct TS_PMT
{
    unsigned table_id                        : 8; //固定为0x02, 表示PMT表
     unsigned section_syntax_indicator        : 1; //固定为0x01
    unsigned zero                            : 1; //0x01
    unsigned reserved_1                      : 2; //0x03
    unsigned section_length                  : 12;//首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。
     unsigned program_number                    : 16;// 指出该节目对应于可应用的Program map PID
    unsigned reserved_2                        : 2; //0x03
    unsigned version_number                    : 5; //指出TS流中Program map section的版本号
     unsigned current_next_indicator            : 1; //当该位置1时，当前传送的Program map section可用；
                                                     //当该位置0时，指示当前传送的Program map section不可用，下一个TS流的Program map section有效。
     unsigned section_number                    : 8; //固定为0x00
    unsigned last_section_number            : 8; //固定为0x00
    unsigned reserved_3                        : 3; //0x07
    unsigned PCR_PID                        : 13; //指明TS包的PID值，该TS包含有PCR域，
            //该PCR值对应于由节目号指定的对应节目。
            //如果对于私有数据流的节目定义与PCR无关，这个域的值将为0x1FFF。
     unsigned reserved_4                        : 4; //预留为0x0F
    unsigned program_info_length            : 12; //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。
    
    // for : descript
    
     std::vector<TS_PMT_Stream> PMT_Stream;  //每个元素包含8位, 指示特定PID的节目元素包的类型。该处PID由elementary PID指定
//    unsigned reserved_5                        : 3; //0x07
//    unsigned reserved_6                        : 4; //0x0F
    
    unsigned CRC_32                            : 32; 
} TS_PMT;
```

```c++
// PMT 表的解析

HRESULT CTS_Stream_Parse::adjust_PMT_table ( TS_PMT * packet, unsigned char * buffer )
{ 
    packet->table_id                            = buffer[0];
    packet->section_syntax_indicator            = buffer[1] >> 7;
    packet->zero                                = buffer[1] >> 6 & 0x01; 
    packet->reserved_1                            = buffer[1] >> 4 & 0x03;
    packet->section_length                        = (buffer[1] & 0x0F) << 8 | buffer[2];    
    packet->program_number                        = buffer[3] << 8 | buffer[4];
    packet->reserved_2                            = buffer[5] >> 6;
    packet->version_number                        = buffer[5] >> 1 & 0x1F;
    packet->current_next_indicator                = (buffer[5] << 7) >> 7;
    packet->section_number                        = buffer[6];
    packet->last_section_number                    = buffer[7];
    packet->reserved_3                            = buffer[8] >> 5;
    packet->PCR_PID                                = ((buffer[8] << 8) | buffer[9]) & 0x1FFF;
 
 	PCRID = packet->PCR_PID;
 
    packet->reserved_4                            = buffer[10] >> 4;
    packet->program_info_length                    = (buffer[10] & 0x0F) << 8 | buffer[11]; 
    // Get CRC_32
	int len = 0;
    len = packet->section_length + 3;    
    packet->CRC_32                = (buffer[len-4] & 0x000000FF) << 24
                                      | (buffer[len-3] & 0x000000FF) << 16
                                      | (buffer[len-2] & 0x000000FF) << 8
                                      | (buffer[len-1] & 0x000000FF); 

 	int pos = 12;
    // program info descriptor
    if ( packet->program_info_length != 0 )
        pos += packet->program_info_length;    
    // Get stream type and PID    
    for ( ; pos <= (packet->section_length + 2 ) -  4; )
    {
        TS_PMT_Stream pmt_stream;
        pmt_stream.stream_type =  buffer[pos];
        packet->reserved_5  =   buffer[pos+1] >> 5;
        pmt_stream.elementary_PID =  ((buffer[pos+1] << 8) | buffer[pos+2]) & 0x1FFF;
  		packet->reserved_6     =   buffer[pos+3] >> 4;
  		pmt_stream.ES_info_length =   (buffer[pos+3] & 0x0F) << 8 | buffer[pos+4];
  
  		pmt_stream.descriptor = 0x00;
  		if (pmt_stream.ES_info_length != 0)
  		{
      		pmt_stream.descriptor = buffer[pos + 5];
      
              for( int len = 2; len <= pmt_stream.ES_info_length; len ++ )
              {
                pmt_stream.descriptor = pmt_stream.descriptor<< 8 | buffer[pos + 4 + len];
              }
       
      		pos += pmt_stream.ES_info_length;
      
  		}
          pos += 5;
          packet->PMT_Stream.push_back( pmt_stream );
          TS_Stream_type.push_back( pmt_stream );
   }

    return 0;
}
 
```

```c++
// 解复用
int Video_PID=0x07e5,Audio_PID=0x07e6;
void Process_Packet(unsigned char*buff)
{ 
  int i; int PID=GETPID(buff);
  if(PID==0x0000) { Process_PAT(buff+4); }              //PAT表的PID为0x0000
  else if(PID==Video_PID) { SaveToVideoBuffer(buff+4); }  //PID指示该数据包为视频包
  else if(PID==Audio_PID) { SaveToAudioBuffer(buff+4); }  //PID指示该数据包为音频包
  else{                               // buff+4 意味着要除去buff前4个字节（即包头）
  for( i=0;i<64;i++)
  { if(PID==pmt[i].pmt_pid) { Process_PMT(buff+4); Break; }
}
 
```

>  		解复用的意义在于，由于TS流是一种复用的码流，里面混杂了多种类型的包；解复用TS流可以将类型相同的Packet存入相同缓存，分别处理。这样就可以将Video、Audio或者其他业务信息的数据区分开来。



### SDT

​		Service Descriptor Table，业务描述表。

​		由于PID是一串枯燥的数字，用户不方便记忆、且容易输错，所以需要有一张表将节目名称和该节目的PID对应起来，DVB设计了SDT表来解决这个问题。 该表格标志一个节目的名称，并且能和PMT中的PID联系起来，这样用户就可以通过直接选择节目名称来选择节目了。

​	  SDT可以提供的信息包括：

- 该节目是否在播放中
- 该节目是否被加密
- 该节目的名称



## DVB搜台

​		机顶盒先调整高频头到一个固定的频率(如498MHZ)，如果此频率有数字信号，则COFDM芯片(如MT352)会自动把TS流数据传送给MPEG- 2 decoder。 

​		MPEG-2 decoder先进行数据的同步，也就是等待完整的Packet的到来.然

- 后循环查找是否出现PID== 0x0000的Packet，如果出现了，则马上进入分析PAT的处理，获取了所有的PMT的PID。
- 接着循环查找是否出现PMT，如果发现了，则自动进入PMT分析，获取该频段所有的频道数据并保存。
- 如果没有发现PAT或者没有发现PMT，说明该频段没有信号，进入下一个频率扫描。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130809195105562)

> ​		`TS`包有可能是音视频数据，也有可能是表格(`PAT/PMT/…`)：`PAT,PMT,DATA,DATA,,,,,,,,PAT,PMT,DATA,DATA,,,,,,`
>
> - 每隔一段时间，发送一张`PAT`表，紧接着发送`DATA`数据
>
> - `PAT`表格里面包含所有`PMT`表格的信息
>
>   - 一个`PMT`表格对应一个频道，例如中央电视台综合频道
>
>   - 一个`PMT`里面包含所有节目的信息，例如`CCTV-1`到`CCTV-14`
>
>   - 在实际情况中有很多频道，所以PMT表格不止一张
>
>     - `PAT、PMT、PMT、PMT,,,DATA,DATA，DATA,,,,`
>
>   - 每个频道或节目都有自己的标识符（`PID`）
>
>     - 当得到一个`DATA`，解出里面的`PID`就知道是什么节目了,也知道节目所属频道
>
>     > 在看电视时，会收到所有节目的`DATA`，当选择某个节目时，机顶盒会把这个节目的`DATA`单独过滤出来，其他舍弃
>



## TS 流的生成和解析

1）TS 流的生成流程大致如下：

- 1、将原始的音视频数据编码后，组成基本码流（ES）；
- 2、将基本码流（ES）打包成 PES；
- 3、在 PES 中加入需要的信息，比如 PTS、DTS 等；
- 4、将 PES 包的数据装载到一系列固定长度为 188 字节的传输包（TS Packet）中；
- 5、在 TS 包中加入需要的信息，比如 PSI、PCR 等；
- 6、连输输出 TS 包形成具有恒定码率的 TS 流。

2）TS 流的解析流程大致如下：

- 1、从 TS 流中解析出 TS 包；
- 2、从 TS 包中获取流信息，比如 PSI、PCR 等；
- 3、获取特定节目的音视频 PID；
- 4、通过 PID 获取特定音视频相关的 TS 包，从中解析出 PES 包；
- 5、从 PES 包中获取 PTS、DTS 等时间戳信息，并从 PES 中解析出基本码流（ES）；
- 6、将基本码流数据交给解码器，解码出原始音视频数据。



## 实例

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/fbc79a66bba084e94d29bb0d2aef97ab.png#pic_center)

- 一个TS包，它的PID是0，说明它的负载内容是PAT信息

- 一个 PAT 包含整个 TS 流的信息，其中有一张表

  - 比较重要的两个属性 program_number 和program_map_PID，可能出现多对
    - 每一对 program_number 表示一个节目
    - 该节目对应的流信息应该放在一个 PID同为program_map_PID 的PMT 表中

- 解析该PMT信息，可以发现

  - 第一路流是MPEG2音频流
  - 第二路流是MPEG2视频流
  - 第三路流是DVB字幕流
  - ...

- 解析完毕，凡是比特流中PID号相同的TS包，所负载的内容为对应的数据流包

  - 把这些包一个一个找出来，把其中的有效码流一部分一部分的拼接起来

  - 然后送给解码器去解码。

    > 就一般的视频流而言，只要拼接成一个完整的PES包，就可以送出去给解码器，然后再继续拼接下一个PES包。

```c
  +-+-+-+-+-+-+-+-+-+-+-+
  | PAT                 | 
  |                     |
  | program_number  5   |___
  | program_map_PID 10  |   |
  |                     |   |
  | program_number  6   |___|__  
  | program_map_PID 11  |   |  |
  |                     |   |  |  
  | program_number  7   |   |  |   
  | program_map_PID 12  |   |  |
  |                     |   |  |
  |         ...         |   |  |
  |                     |   |  |
  +-+-+-+-+-+-+-+-+-+-+-+   |  |
                            |  |
  +-+-+-+-+-+-+-+-+-+-+-+   |  |
  | PMT                 |   |  |
  | TS Header PID = 10  |<——   |
  |                     |      |
  | stream_type    0x0f |______|__________________0x0f表示AAC音频，下方AAC数据打包PID=20,   
  | elementary_PID 20   |      |
  | stream_type    0x1b |______|__________________0x1b表示H264视频，下方H264数据打包PID=22 
  | elementary_PID 22   |      |
  |                     |      |  
  +-+-+-+-+-+-+-+-+-+-+-+      |
                               |
  +-+-+-+-+-+-+-+-+-+-+-+      |
  | PMT                 |      | 
  | TS Header PID = 11  |<————— 
  |                     |
  | stream_type    0x0f |   
  | elementary_PID 23   |
  | stream_type    0x1b |
  | elementary_PID 24   |
  |                     |  
  +-+-+-+-+-+-+-+-+-+-+-+
```



- 音频数据打包

  ```c
  裸ACC数据:
    +-+-+-+-+-+-+-+-+-+-+-+
    |         AAC         |          
    +-+-+-+-+-+-+-+-+-+-+-+
  添加PES头的ACC数据:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | AAC PES |         AAC         |          
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  添加TS头将PES分割之后的TS包，假设正好分割成2个TS包，包大小固定188字节，不够用adaptation域填充一般填充0xFF:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |TS | AAC PES |  AAC 1  |TS | adaptation|    AAC 2    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |- - - - Packet 1 - - - |- - - - - Packet 2 - - - - - |
    <假设 PID = 20 的TS包>            
  ```

- 视频数据打包

  ```c
  裸H264数据，一帧:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |              H264             |          
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  添加PES头之后的H264数据，一帧表示一个PES包:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | H264 PES|           H264                |          
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  将一个PES包分割之后，分别添加TS头之后的TS包。
  这里假设分割成3个TS包，每个包固定大小188字节(包含TS包头在内)，
  最后一个包不够188字节使用adaptaion域填充0xFF:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |TS | H264 PES| H264 1|TS |  H264 2     |TS | adaptation|  H264 3 |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |- - - Packet 1 - - - |- - Packet 2 - - | - - - - Packet 3 - - - -|
    <假设 PID = 22 的TS包>
  ```



![](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130809133716015)
