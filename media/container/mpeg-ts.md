# MPEG-TS

​		Transport Stream （传输流）

---

​		以 .ts 结尾的文件 就是 MPEG-TS 封装格式。

​		其实还有一个封装格式 MPEG-PS ，ps 的 全称是 Program Stream （节目流）。注意 Program 翻译成中文是 **节目** 的意思，不是**程序**。PS 封装格式主要用在 不容易发生错误的环境，例如 DVD 光盘。



## MPEG

​		MPEG 全称 Moving Picture Experts Group （动态影像专家小组），该专家组是联合技术委员会（Joint Technical Committee， JTC1）的一部分，JTC1 是由 ISO（国际标准化组织）和 IEC（国际电工委员会）建立的。JTC1 负责信息技术，在 JTC1 中，下设有负责**“音频、图像编码以及多媒体和 超媒体信息”**的子组 SG29。在 SG29 子组中，又设有 多个工作小组，其中就包括 JPEG（联合图片专家组）和 负责活动图像压缩的工作组 WG11 。因此，可以认为 MPEG 是 ISO/IEC JTC1/SG29/WG11，成立于1988年。

​		把 MPEG 理解成一个组织就行。MPEG 主要制定影音压缩及传输的规格标准。MPEG 组织制定的标准目前一共有五个 ，MPEG-1、MPEG-2、MPEG-4、MPEG-7及MPEG-21，而 **MPEG-TS** 封装格式 的定义在 **MPEG-2** 标准 里面。



### MPEG-2

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

## MPEG中的流类型

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



##  封装结构

​		TS流是基于Packet的位流格式，每个包是188个字节（或204个字节，在188个字节后加上了16字节的CRC校验数据，其他格式一样）。整个TS流组成形式如下：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130809122755796)

```c
// bit 
//| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32| ...
//2进制Packet---------------------------------------------------------------------------------------
//| 0| 1| 0| 0| 0| 1| 1| 1| 0| 0| 0| 0| 0| 1| 1| 1| 1| 1| 1| 0| 0| 1| 0| 1| 0| 0| 0| 1| 0| 0| 1| 0|
//16进制Packet--------------------------------------------------------------------------------------
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

| Packet Header 字段序号 |                              |       |                                                              |
| ---------------------- | ---------------------------- | ----- | ------------------------------------------------------------ |
| 1                      | sync_byte                    | 8bits | 同步字节                                                     |
| 2                      | transport_error_indicator    | 1bit  | 错误指示信息（1：该包至少有1bits传输错误）                   |
| 3                      | payload_unit_start_indicator | 1bit  | 负载单元开始标志（packet不满188字节时需填充）<br />多个ts包组成一个视频帧时，第一个为1，后续为0 |
| 4                      | transport_priority           | 1bit  | 传输优先级标志（1：优先级高）                                |
| 5                      | PID                          | 13bit | Packet ID号码，唯一的号码对应不同的包                        |
| 6                      | transport_scrambling_control | 2bits | 加密标志（00：未加密；其他表示已加密）                       |
| 7                      | adaptation_field_control     | 2bits | 附加区域控制                                                 |
| 8                      | continuity_counter           | 4bits | 包递增计数器                                                 |

- `sync_byte`  

  - 一般以 0x47 开始，方便在某些场景下进行同步操作


  ```c
  MPEG_transport_stream()
  {
  	do {
  		transport_packet()
      } while(nextbits() == sync_byte) // sync_byte == 0x47
  }
  ```

![](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130809133716015)

- PID是TS流中唯一识别标志
  - playload ID， 表示了Packet的类型
  - Packet Data是什么内容就是由PID决定的
    - `0x0000`，Packet Data就是DVB的PAT表
    - `0x0001`，CAT
    - `0x0002`，TSDT
    - `0x0012`，EIT,ST
    - `0x0013`，RST,ST
    - `0x0014`，TDT,TOT,ST
    - ...



## PSI

​		Program Specific Information，你可以把 PSI 理解成 节目特定信息。TS 包是对 PES 包的封装，但是不只是 PES，TS 还可以是对 PSI 数据的封装

​		 PSI 不是一个表，PSI 是一个统称， PAT，PMT，CAT，NIT 这些都是 PSI。

​		在解析TS流的时候，首先寻找PAT表，根据PAT获取所有PMT表的PID；再寻找PMT表，获取该频段所有节目数据并保存。这样，只需要知道节目的PID就可以根据PacketHeade给出的PID过滤出不同的Packet，从而观看不同的节目。这些就是PAT表和PMT表之间的关系。

### PAT表

​		Program Association Table，节目关联表， PID为`0x0000`。

​		PAT表描述了当前流的NIT（Network Information Table，网络信息表）中的PID、当前流中有多少不同类型的PMT表及每个PMT表对应的频道号。

#### PAT表描述

​		定义了当前TS流中所有的节目，其PID为0x0000，它是PSI的根节点，要查寻找节目必须从PAT表开始查找。

 		PAT表主要包含频道号码和每一个频道对应的PMT的PID号码，以流ID唯一标识一个TS流：

- transport_stream_id，该ID标志唯一的流ID
- program_number，该号码标志Ｓ流中的一个频道
  - 该频道可以包含很多的节目(即可以包含多个Video PID和Audio PID)
- program_map_PID，*节目映射表的PID*
  - 因为可以有很多的频道,因此DVB规定PMT的PID可以由用户自己定义



#### 实例

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

|                                                              |      |                             |                                                              |                         |
| ------------------------------------------------------------ | ---- | --------------------------- | ------------------------------------------------------------ | ----------------------- |
| 字段名                                                       | 位   | 具体值                      | 次序                                                         | 说明                    |
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



#### PAT表的结构

```c++

typedef struct TS_PAT_Program  
{  
    unsigned program_number   :  16;  //节目号  
    unsigned program_map_PID :  13; // 节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个  
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
    unsigned reserved_3                    : 3; // 保留位  
    unsigned network_PID                    : 13; //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID  
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
{ int I; int PID=GETPID(buff);
  if(PID==0x0000) { Process_PAT(buff+4); }  // 如果PID为0x0000,则该Packet Data为PAT信息，因此调用处理PAT表的函数
  else{                                     // 这里buff+4 意味着从Packet Header之后进行解析（包头占4个字节）
    ……
  }
}
```



### PMT 

​		Program Map Table，节目映射表。如果一个TS流中含有多个频道，那么就会包含多个PID不同的PMT表。

  PMT表中包含的数据如下：

- 当前频道中包含的所有Video数据的PID
- 当前频道中包含的所有Audio数据的PID
- 和当前频道关联在一起的其他数据的PID(如数字广播,数据通讯等使用的PID)



#### 实例

```d
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



#### PMT结构

```c++
typedef struct TS_PMT_Stream
{
     unsigned stream_type                       : 8; //指示特定PID的节目元素包的类型。该处PID由elementary PID指定
     unsigned elementary_PID                    : 13; //该域指示TS包的PID值。这些TS包含有相关的节目元素
     unsigned ES_info_length                    : 12; //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数
     unsigned descriptor;
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
    
     std::vector<TS_PMT_Stream> PMT_Stream;  //每个元素包含8位, 指示特定PID的节目元素包的类型。该处PID由elementary PID指定
     unsigned reserved_5                        : 3; //0x07
    unsigned reserved_6                        : 4; //0x0F
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



### DVB搜台

​		机顶盒先调整高频头到一个固定的频率(如498MHZ)，如果此频率有数字信号，则COFDM芯片(如MT352)会自动把TS流数据传送给MPEG- 2 decoder。 

​		MPEG-2 decoder先进行数据的同步，也就是等待完整的Packet的到来.然

- 后循环查找是否出现PID== 0x0000的Packet，如果出现了，则马上进入分析PAT的处理，获取了所有的PMT的PID。
- 接着循环查找是否出现PMT，如果发现了，则自动进入PMT分析，获取该频段所有的频道数据并保存。
- 如果没有发现PAT或者没有发现PMT，说明该频段没有信号，进入下一个频率扫描。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130809195105562)

##  分析软件

- elecard stream analyzer，支持 TS，FLV，MP4 等众多格式，可免费试用 30天
- MPEG-2 TS packet analyser ，界面清晰简洁。