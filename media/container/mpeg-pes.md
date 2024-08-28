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

