# RTP-playload

---

​		对于使用RTP荷载不同的码流，标准给出了不同的文档来说明。可以查看：[RFC RTP Payload](https://www.rfc-editor.org/search/rfc_search_detail.php?page=All&title=RTP Payload&pubstatus[]=Any&pub_date_type=any&sortkey=Number&sorting=ASC)



## 荷载音频

### 荷载AAC

> [使用RTP包荷载AAC码流数据_aac rtp打包-CSDN博客](https://blog.csdn.net/weixin_38102771/article/details/128304673)
>
> [使用RTP包荷载AAC码流数据 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/642179319)
>
> [Enscript Output (rfc-editor.org)](https://www.rfc-editor.org/rfc/pdfrfc/rfc6416.txt.pdf)

​	RTP Payload 前面需要先加 4 个字节的荷载标识

```c
// 荷载标识
payload[0] = 0x00;
payload[1] = 0x10;
payload[2] = (frameLength & 0x1FE0) >> 5; // frameLength 记录在AAC frame header 中
payload[3] = (frameLength & 0x1F) << 3;
```

```c
// | RTP Header | 荷载标识（4byte） | ADTS Frame Data（不包括 Frame Header）|
//              | ------------------RTP Payload--------------------------|
```

​		接下来将 ADTS Frame Data 拷贝到 RTP Payload[4] 开始的位置，注意 ADTS Frame Header 无需拷贝。

> 代码示例见 `lib/jrtplib.md`



## 荷载视频

​		视频帧大小差别很大

- 较小的帧小于MTU
  - 可单包直接发送
  - 或者多帧组合发送
- 较大的帧大于MTU需要分片发送

​		依据上述不同，RTP荷载视频数据有三种模式：

- 单NALU模式
- 组合帧模式
- 分片封装模式

> 有关组合帧封包和分片封包类型包含好几种，这里介绍常用的两种：STAP-A和FU-A。
>
> 一般H264进行RTP封装，SPS/PPS采用单一NALU封装方式，I帧/P帧采用FU-A分片模式，如果带有SEI及AUD可过滤掉，也可以采用单一NALU封装方式

#### 单一NALU模式

​			一个RTP包包含一个完整的视频帧，帧大小需要小于MTU，不会被IP层分片



##### h264

​	RTP头部 + 一个字节 NALU Header + NALU数据部分。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/9822e94a1d2143e2b0d03209945022a2.png)

​		以SPS为例，SPS一般小于MTU，可采用单NALU封装模式，其封包后的二进制如下：

`80 e0 be 8e 8c e8 56 d5 4a 9b 57 b3 | 67 | 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a`

- `80 e0 be 8e 8c e8 56 d5 4a 9b 57 b3`为RTP头
  - ``V=10B=2,Padding=0,Extension=0,CC=0,Mark=1,PT=96,SN=48782,TS=2364036821,SSRC=0x4a9b57b3``
- RTP头之后一个字节为NALU头
  - 就是`SPS`帧去掉`00000001`起始位标识，为`67`
- 之后为NALU单元`RBSP`数据，在编码时简单的做法就是RTP头后直接附加去除起始标识的`NALU`数据即可



##### h265

​		RTP头部 + Payload Header + DONL + NALU数据部分

- PayloadHeader一般与NALU Header定义完全一致
- DONL：Decoding Order Number
  - 当使用多slice编码模式时使用，用于判断一帧的每个slice是否收齐
  - 一般使用单slice，无此字段
  - 所以通常境况下，单一帧模式封装方式与H264一致，H265帧去掉起始位直接作为负载
- VPS/SPS/PPS



#### 组合帧模式

##### h264

​		针对多个较小的帧，可以采用组合成一个RTP包发送的方式。

​		这种方式在视频传输中较少使用，一般较小的帧都是通过单一NALU模式发送。

![](https://raw.githubusercontent.com/Mocearan/picgo-server/main/391987886da14a289c28b33200efc718.png)

​		此处以STAP-A组合类型为例，组合发送SPS和PPS。假设SPS和PPS的裸流如下：

```c
//sps
00 00 00 01 | 67 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a1 01 51 58 ae 2b 15 1d 08 0a 8a c5 71 58 a8 e8 40 54 56 2b 8a c5 47 42 02 a2 b1 5c 56 2a 3a 10 24 85 21 39 3c 9f 27 e4 fe 4f c9 f2 79 b9 b3 4d 08 12 42 90 9c 9e 4f 93 f2 7f 27 e4 f9 3c dc d9 a6 b4 03 c0 11 3f 2c a9 00 00 03 00 78 00 00 15 90 60 40 00 3e 80 00 00 11 94 06 f7 be 17 84 42 35
//pps
00 00 00 01 | 68 33 3c b0 00 00

// 组合包
| RTP header 12字节 | 78 【00 75】 67 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a1 01 51 58 ae 2b 15 1d 08 0a 8a c5 71 58 a8 e8 40 54 56 2b 8a c5 47 42 02 a2 b1 5c 56 2a 3a 10 24 85 21 39 3c 9f 27 e4 fe 4f c9 f2 79 b9 b3 4d 08 12 42 90 9c 9e 4f 93 f2 7f 27 e4 f9 3c dc d9 a6 b4 03 c0 11 3f 2c a9 00 00 03 00 78 00 00 15 90 60 40 00 3e 80 00 00 11 94 06 f7 be 17 84 42 35 【00 06】  68 33 3c b0 00 00
```

- SPS去掉起始标志，size为117，十六进制为0x75
- PPS去掉起始标志，size为0x06
- 78 STAP-A类型头（NALU头）
  - 其中F为0，NRI为3，type为24
    - 24标识STAP-A类型，此类型标识后续负载为组合帧
- 00 75 表示SPS的size，后面跟的0x75个自己为去掉起始标志的SPS数据
- 00 06 表示PPS的size，后面跟的6个字节为去掉起始位的PPS数据



##### h265

![组合帧头](https://raw.githubusercontent.com/Mocearan/picgo-server/main/0438b67a92267f08e8df80e1a6c2e5e6.png)

- PayloadHeader 负载头，与H265 NALUheader类似，有F，TYPE,LayerID,TID组成
  - 一般F=0，LayerID=0，TID=1
  - 这里Type必须为48，标识组合包头

![不带DONL](https://raw.githubusercontent.com/Mocearan/picgo-server/main/b957d895a168b41933eaf55074905ca3.png)

​		带DONL的组合包封装模式与H264类似，一般情况下很少用到组合帧封装方式，小于MTU的帧一般是单一帧封装，减少解封装复杂性。



#### 分片模式

​		视频帧大小超过MTU，则RTP封装需要分片封装

##### h264

​		较常用的分片模式位`FU-A`，其格式如下：

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/62d17fa96daa466c8956bd249cf5e7b0.png)

- RTP 头部之后的第一个字节为 FU indicator
  - FU indicator 结构同NALU头，H264/H265不同
    - H264
      - ![fu indicator](https://raw.githubusercontent.com/Mocearan/picgo-server/main/e6a0e9ad779d928153e777169d6c0ab8.png)
      - F 和 NRI 与 NALU 中相同
      - Type 需要设置为 28/29，指示当前为分片模式 A/B。
    - H265，Type必须为49表示FU分片
- 第二个字节为 FU header
  - ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/920d6b1ace4e4e5e977ab726126e2d15.png)
  - S：start 标记位，当该位为 1 时表示 NALU 分片的第一个分片。
  - E：end 标记位，当该位为 1 时表示 NALU 分片的最后一个分片。
  - R：保留位，接收者可以忽略该位。
  - Type：NALU 原来的 Type 类型（1-23）。
- 这两个字节之后跟的是NALU的数据
  - 去掉起始位及NALU header之后的数据	
    - 通过FU indicator的F/NRI以及FU header即可组合成NALU header，RTP解封装的时候要生成NALU头及起始标识。
  - 按照分片大小进行分包
  - 同一个帧的分片的头两个字节除了起始和结束FU header中的S和E位不同，其他都一样

​	实例如下：

```c
80 60 be  91 8c e8 56 d5 4a 9b 57 b3 | 7c | 85
```

- `80 60 be  91 8c e8 56 d5 4a 9b 57 b3`， RTP header
- `0x7c`， FU indicator，其二进制为：01111100，F=0，NRI=3，type=28表示FU-A分片
- `0x85`， FU header， 其二进制为：10000101，S=1，E=0,type=5,表示I帧的起始FU-A分片

##### h265

![FU](https://raw.githubusercontent.com/Mocearan/picgo-server/main/9c6f2ec87838175b41336937de2c2901.png)

- PayloadHeader中F=0，LayerID=0，TID=1，Type必须为49表示FU分片
- FU header定义与FU-A定义基本一致
  - NALU Type在H265中为6bits表示，所以这里去掉了R，只保留S/E/TYPE格式
    - S：1bit,1-表示是首个分片报文,0-非首个分片报文
    - E：1bit,1-表示最后一个分片报文,0-非最后一个分片报文
    - FuType：6 bits,对应的NALU type
  - ![fu-header](https://raw.githubusercontent.com/Mocearan/picgo-server/main/530d84443466b68b033051bf9b592967.png)

​	实例如下：

```c
80 60 94 22 c6 59 8d 6a 3c c5 fa f7 | 62 01 | 93
```

- `80 60 94 22 c6 59 8d 6a 3c c5 fa f7`， RTP header
- `62 01`， Payload header，其按照格式解析后，TID=01，LayID=0,type=49,F=0，表示FU分片
- `93`， FU header，按照格式解析后，S=1,E=0,Futype=19，表示IDR_W_RADL

#### 代码

​		有关H264采用单一NALU及FU-A分片进行RTP封装发送的相关代码详解，引用FFMPEG源码进行解析，这里引用部分打包的代码，解码和这个过程相反：

```c
// libavformat/rtpenc_h264_hevc.c
static void nal_send(AVFormatContext *s1, const uint8_t *buf, int size, int last)
{
    RTPMuxContext *s = s1->priv_data;
    enum AVCodecID codec = s1->streams[0]->codecpar->codec_id;

    av_log(s1, AV_LOG_DEBUG, "Sending NAL %x of len %d M=%d\n", buf[0] & 0x1F, size, last);
    if (size <= s->max_payload_size) {//判断包大小是否小于等于RTP最大负载长度，一般RTP最大负载长度+RTP头小于MTU
        ...
            flush_buffered(s1, 0);
            ff_rtp_send_data(s1, buf, size, last);//这里调用此函数直接发送，ff_rtp_send_data中会对数据直接打RTP头后直接发送，
                                                  //这里由于小于MTU，所以采用单一帧发送模式
        ...
    } else {//视频帧长度大于MTU时，采用FU分片
        ...
        if (codec == AV_CODEC_ID_H264) {//只对H264进行注释，H265后续在说
            uint8_t type = buf[0] & 0x1F;//这里buf已经去掉起始标识00000001，buf[0]标识nalu header，这里取0~4位，即帧类型
            uint8_t nri = buf[0] & 0x60;//这里取5-6位，即：NRI，这里只是通过按位与的方式，保留了5-6位，并未真正转换为真实值，方面后买你组合
            //FU indicator字节
            s->buf[0] = 28;        /* FU Indicator; Type = 28 ---> FU-A */
            s->buf[0] |= nri; //因为nri只是保留了5-6位，这里直接按位或，即可组成fu-indicator
            // fu header
            s->buf[1] = type; //0~4帧类型
            s->buf[1] |= 1 << 7; //最高位起始位为1标识开始
            buf  += 1; //原始H264起始分片需要去掉nalu heder字节，这里直接跳过帧头
            size -= 1; //去掉头字节后，size要减去1

            flag_byte   = 1;
            header_size = 2;//fu-a 头长度
        } else {
            ...
        }

        while (size + header_size > s->max_payload_size) {
            memcpy(&s->buf[header_size], buf, s->max_payload_size - header_size);//发送缓冲buf中已经有了FU-A的2个头字节
            ff_rtp_send_data(s1, s->buf, s->max_payload_size, 0);//发送分片
            buf  += s->max_payload_size - header_size;//h264码流指针移动到未发送的起始位置
            size -= s->max_payload_size - header_size;//未发送的码流数据
            s->buf[flag_byte] &= ~(1 << 7);//更改fu-header，中间分片为00
        }
        s->buf[flag_byte] |= 1 << 6;//更改fu-header，最后一个分片，结束标志值为1
        memcpy(&s->buf[header_size], buf, size);
        ff_rtp_send_data(s1, s->buf, size + header_size, last);//打RTP头并发送
    }
}

```

​		有关H265采用单一NALU及FU-A分片进行RTP封装发送的相关代码详解，这里引用FFMPEG源码进行解析,这里引用部分打包的代码，解码和这个过程相反

```c
// libavformat/rtpenc_h264_hevc.c
static void nal_send(AVFormatContext *s1, const uint8_t *buf, int size, int last)
{
   ...
    if (size <= s->max_payload_size) {//单一帧封装模式
        int buffered_size = s->buf_ptr - s->buf;
        int header_size;
        int skip_aggregate = 0;

        if (codec == AV_CODEC_ID_H264) {
           ...
        } else {
            header_size = 2;//payloadheader的占用字节数，H265为两个字节，与NALU header一样
        }

       ...
            flush_buffered(s1, 0);
            ff_rtp_send_data(s1, buf, size, last);//这里调用此函数直接发送，ff_rtp_send_data中会对数据直接打RTP头后直接发送，与H264类似
        }
    } else {
       ...
        if (codec == AV_CODEC_ID_H264) {
          ...
        } else {
            uint8_t nal_type = (buf[0] >> 1) & 0x3F;//获取NALU type
            /*
             * create the HEVC payload header and transmit the buffer as fragmentation units (FU)
             *
             *    0                   1
             *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
             *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
             *   |F|   Type    |  LayerId  | TID |
             *   +-------------+-----------------+
             *
             *      F       = 0
             *      Type    = 49 (fragmentation unit (FU))
             *      LayerId = 0
             *      TID     = 1
             */
             //以下两个自己是payloadheader，type=49
            s->buf[0] = 49 << 1;
            s->buf[1] = 1;

            /*
             *     create the FU header
             *
             *     0 1 2 3 4 5 6 7
             *    +-+-+-+-+-+-+-+-+
             *    |S|E|  FuType   |
             *    +---------------+
             *
             *       S       = variable
             *       E       = variable
             *       FuType  = NAL unit type
             */
            s->buf[2]  = nal_type;//fu header的type就是nalu type
            /* set the S bit: mark as start fragment */
            s->buf[2] |= 1 << 7;//第一片，S=1

            /* pass the original NAL header */
            buf  += 2;//去掉nalu header的2个字节
            size -= 2;//去掉nalu header的2个字节，帧长度-2

            flag_byte   = 2;//单帧分片过程中只有fu-header中的s和e会变化，这里是其数组下标
            header_size = 3;//payloadheader+fuheader长度
        }

        while (size + header_size > s->max_payload_size) {
            memcpy(&s->buf[header_size], buf, s->max_payload_size - header_size);//发送缓冲buf中已经有了FU的3个头字节
            ff_rtp_send_data(s1, s->buf, s->max_payload_size, 0);//加上rtp头发送
            buf  += s->max_payload_size - header_size;
            size -= s->max_payload_size - header_size;
            s->buf[flag_byte] &= ~(1 << 7);//更改fu-header 中间分片，s=0 e=0
        }
        s->buf[flag_byte] |= 1 << 6;//最后一个分片，s=0,e=1
        memcpy(&s->buf[header_size], buf, size);
        ff_rtp_send_data(s1, s->buf, size + header_size, last);//发送最后一个分片
    }
}
```



### 荷载ps 流

​		针对H264 做如下PS 封装：

- 每个IDR NALU 前一般都会包含SPS、PPS 等NALU
  - 所以一个IDR NALU PS 包由外到内顺序是：
  - PS header | PS system header | PS system Map | PES header | h264 raw data
- 对于其它非关键帧的PS 包，直接加上PS头和PES 头就可以了。
  - 顺序为：PS header | PES header | h264raw data。
- 当有音频数据时，将数据加上PES header 放到视频PES 后就可以了。
  - 顺序如下：PS 包=PS头|PES(video)|PES(audio)
- 再用RTP 封装发送就可以了

> - GB28181 对RTP 传输的数据负载类型有规定（参考GB28181 附录B），负载类型中96-127
>
> -   RFC2250 建议96 表示PS 封装，建议97 为MPEG-4，建议98 为H264
>
> -  即我们接收到的RTP 包首先需要判断负载类型
>
>    - 若负载类型为96，则采用PS 解复用，将音视频分开解码。
>    - 若负载类型为98，直接按照H264 的解码类型解码。
>
>    PS 包中的流类型（stream type）的取值如下：
>
> -  MPEG-4 视频流： 0x10；
>
> -  H.264 视频流： 0x1B；
>
> -   SVAC 视频流： 0x80；
>
> -   G.711 音频流： 0x90；
>
> -  G.722.1 音频流： 0x92；
>
> - G.723.1 音频流： 0x93；
>
> -  G.729 音频流： 0x99；
>
> - SVAC音频流： 0x9B。

​    注：此方法不一定准确，取决于打包格式是否标准





