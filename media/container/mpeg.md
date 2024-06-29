

# MPEG

​		MPEG 全称 Moving Picture Experts Group （动态影像专家小组），该专家组是联合技术委员会（Joint Technical Committee， JTC1）的一部分，JTC1 是由 ISO（国际标准化组织）和 IEC（国际电工委员会）建立的。JTC1 负责信息技术，在 JTC1 中，下设有负责**“音频、图像编码以及多媒体和 超媒体信息”**的子组 SG29。在 SG29 子组中，又设有 多个工作小组，其中就包括 JPEG（联合图片专家组）和 负责活动图像压缩的工作组 WG11 。因此，可以认为 MPEG 是 ISO/IEC JTC1/SG29/WG11，成立于1988年。

​		把 MPEG 理解成一个组织就行。MPEG 主要制定影音压缩及传输的规格标准。MPEG 组织制定的标准目前一共有五个 ，MPEG-1、MPEG-2、MPEG-4、MPEG-7及MPEG-21，而 **MPEG-TS** 封装格式 的定义在 **MPEG-2** 标准 里面。



## MPEG-2

​		MPEG-2 目前广泛用于互联网传输协议，以及早期的有线数字电视，无线数字电视，卫星电视， DVB，DVD，等等。

​		MPEG-2 标准目前分为10个部分，统称为 ISO/IEC 13818 国际标准，标题 是 “GENERIC CODING OF MOVING PICTURES AND ASSOCIATED AUDIO”。

| 编码    | 标题                     | 描述                                                         |
| ------- | ------------------------ | ------------------------------------------------------------ |
| 13818-1 | System（系统）重点       | 描述多个 音频码流，视频码流 和 基础数据码流，合成传输码流和节目码流的方式。 |
| 13818-2 | Video（视频）            | 描述视频编码的方法                                           |
| 13818-3 | Audio（音频）            | 描述与MPEG-1音频标准反向兼容的 音频编码方法                  |
| 13818-4 | Compliance（符合性测试） | 描述如何判断一个编码码流是否符合MPEG-2码流                   |
| 13818-5 | Software（软件实现）     | 描述了MPEG-2标准的第一、二、三部分的软件实现方法             |
| 13818-6 | DSM-CC （命令与控制）    | 描述交互式多媒体网络中服务器与用户间的会话信令集             |

​		MPEG-2标准 剩余的四个部分，因为没有广泛使用，就不列举了。

​		从上图的表格可以看出，MPEG-TS 封装格式的定义在 **13818-1** （System）里面。1990年 ATM视频编码专家组 跟 MPEG 组织合作，把 ISO/IEC 13818标准的 13818-1 搞成 ITU-TRec.H.220（System），把 13818-2 搞成 ITU-TRec.H.262 （Video）， ITU-TRec.H.220 跟 ITU-TRec.H.262 都是 **ITU-T** 标准的一部分。

​		其实各个组织之间是有合作的，对一些标准文档 做 扩展，衍生，单独提取之类的工作。

​		TS / PS， 早期主要是用在 **数字电视** 领域，我国数字电视标准用的是 DVB，全称是 Digital Video Broadcasting（数字视频广播）。

![image-20240623113447966](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240623113447966.png)

​		这些 频道、节目、音视频码流如何在TS里面进行区分是 TS 封装格式复杂的主要原因。

​		互联网IP网络体系下传输的数字信号，和数字电视基站广播的数字信号没有本质区别：

- 互联网使用TS封装，通常传一路视频和一路音频，TS中很多字段用不到
  - 单节目的TS 封装 叫做 Single Program Transport Stream (SPTS)。
- 机顶盒之类的开发，想深入理解数字电视的知识，可以看 《Video Demystified: A Handbook for The Digital Engineer》。
- 

## 流编码结构

### MPEG4流的结构

​		以`0x00 00 01`起始码为标志分割成若干处理单元。

​		进行PS封装是，最基本的处理单元是VOP视频帧。I帧之前的参数集数据与I帧合并到一个单元处理。

- `Visual Object Sequence`，简称VOS，起始码`0x00 00 01 b0`
- `Visual Object`，简称VO，起始码 `0x00 00 01 b5`
- `MPEG4 Video Header Object`，简称MVHO，起始码` 0x00 00 01 00`
- `Video Object Layer`，简称VOL，起始码` 0x00 00 01 20`
- `MPEG4 Header UD`，简称MHUD，起始码 `0x00 00 01 b2`
- `Video Object Plane – I Frame`，简称VOP，起始码 `0x00 00 01 b6`
- `Video Object Plane – other Frame`，简称VOP，起始码` 0x00 00 01 b6`
- `Video Object Plane – other Frame`，简称VOP，起始码 `0x00 00 01 b6`
- `Video Object Plane – other Frame`，简称VOP，起始码 `0x00 00 01 b6`
- ...

### H264

​		标准 H.264 流以` 0x00 00 00 01 `起始码为标志分割成若干单元，称之为`Network Abstraction Layer unit`，缩写`NALu`。

​		H.264标准可以将一帧图像分成多个Slice进行编码，每个Slice会输出一个NALu，例如对一幅图像进行两场编码，则有两个NALu。

- (如果出现) `Access Unit Delimiter ` ，简称AUD ， 起始码 `0x00 00 01 09`
- `Sequence Parameter Set`，简称PPS，起始码 `0x00 00 01 67`
- `Picture Parameter Set`，简称，SPS起始码 `0x00 00 01 00`
- `Slice 0 of IDR Picture`，起始码，` 0x00 00 01 65`
- `Slice 1 of IDR Picture`，起始码， `0x00 00 01 05或01`

### H265

​		标准 H265 同 H264 多数语法，在 I 帧前后多一种 VPS 参数集。

### 定长帧音频及其他流式私有数据的结构

​		定长音频帧和其他流式私有数据的结构本文中不做具体规定，但要求其中不出现 0x000001 形式的伪起始码。视为流式私有数据传输。流式私有数据结构在智能数据交换文档中规定。



## 流类型

### ES

​		Elementary Stream，基础码流。不分段的音频、视频或其他信息的连续码流。

​		 一个 ES 包理解成 一个**H264编码后**的视频帧，或者一个音频帧，但是 ES 不只是音视频帧。

### PES

​		Packetized Elementary Stream， 打包的ES。把基本流ES分割成段，往ES包上面封装一层 加上 PTS 跟 DTS 等头部信息。

​		

### PS

​		Program Stream，节目流。将具有共同时间基准的一个或多个PES组合而成的单一数据流。用于播放或编辑系统的存储格式。



### TS

​		Transport Packet，传输包。将具有共同时间基准或独立时间基准的一个或多个PES组合而成的单一数据流。用于数据传输的屙屎。

​		固定188个字节，一个大的 PES 包 会拆成多个 小的 块，封装进去 TS 包进行传输。



### ps/ts

​		TS流的包结构是固定长度188字节的，而PS流的包结构是可变长度的。这导致了TS流的***\*抵抗传输误码\****的能力强于PS流。

> ​		误码的产生是由于在信号传输中，衰变改变了信号的电压,致使信号在传输中遭到破坏，产生误码。噪音、交流电或闪电造成的脉冲、传输设备故障及其他因素都会导致误码（比如传送的信号是1，而接收到的是0；反之亦然）。由于种种原因,数字信号在传输过程中不可避免地会产生差错。-- 百度百科

​		TCP ，UDP 协议有一个 **checksum** （校验和）字段 ，发生误码，底层直接丢弃数据包，不会丢上去给应用层。

​		实际上，对于 UDP 或者 TCP 来说，用 TS 封装格式还是 PS 封装格式 ，抵抗**传输误码**的能力是一样的，因为UDP ，TCP 这些协议帮你处理了误码情况。但是 TS 跟 PS 的标准不只是给 UDP 跟 TCP 用，录像机，DVD机，这些数字视频领域的设备，也会用到 PS 跟TS。

​		TS码流由于采用了固定长度的包结构，当传输误码破坏了某一TS包的同步信息时，接收机可在固定的位置检测它后面包中的同步信息，从而恢复同步，避免了信息丢失。而PS包由于长度是变化的，一旦某一 PS包的同步信息丢失，接收机无法确定下一包的同步位置，就会造成失步，导致严重的信息丢失。因此，在信道环境较为恶劣，传输误码较高时，一般采用TS码流；而在信道环境较好，传输误码较低时，一般采用PS码流。

​		由于TS码流具有较强的抵抗传输误码的能力，因此目前在传输媒体中进行传输的MPEG-2码流基本上都采用了TS码流的包格。

