# RTP-playload

---

​		对于使用RTP荷载不同的码流，标准给出了不同的文档来说明。可以查看：[RFC RTP Payload](https://www.rfc-editor.org/search/rfc_search_detail.php?page=All&title=RTP Payload&pubstatus[]=Any&pub_date_type=any&sortkey=Number&sorting=ASC)



## 荷载音频

### 荷载AAC

> [Enscript Output (rfc-editor.org)](https://www.rfc-editor.org/rfc/pdfrfc/rfc6416.txt.pdf)

```c
// | RTP Header | 荷载标识（4byte） | ADTS Frame Data（不包括 Frame Header）|
//              | ------------------RTP Payload--------------------------|
```

- 音频帧一般较小，一般只用一个 RTP 包也可以承载
  - 一般一个RTP包也只荷载一帧
- RTP Payload 要去除ADTS头
- RTP Payload 前面加 4 个字节的荷载标识

```c
// 荷载标识
payload[0] = 0x00;
payload[1] = 0x10;
payload[2] = (frameLength & 0x1FE0) >> 5; // frameLength 记录在AAC frame header 中
payload[3] = (frameLength & 0x1F) << 3;
```



​		接下来将 ADTS Frame Data 拷贝到 RTP Payload[4] 开始的位置，注意 ADTS Frame Header 无需拷贝。

> 代码示例见 `lib/jrtplib.md`

```c
static int aac_parse(Track *tr, uint8_t *data, size_t len) 
{ 
    //XXX handle the last packet on EOF 
    int off = 0; 
    uint32_t payload = DEFAULT_MTU - AU_HEADER_SIZE; 
    uint8_t *packet = g_malloc0(DEFAULT_MTU); 
    if(!packet) return ERR_ALLOC; 
// trim away extradata 
//    data += AAC_EXTRA; 
//    len -= AAC_EXTRA; 
    packet[0] = 0x00; 
    packet[1] = 0x10; 
    packet[2] = (len & 0x1fe0) >> 5; 
    packet[3] = (len & 0x1f) << 3; 
    if (len > payload) { 
        while (len > payload) { 
            memcpy(packet + AU_HEADER_SIZE, data + off, payload); 
            mparser_buffer_write(tr, 
                                 tr->properties.pts, 
                                 tr->properties.dts, 
                                 tr->properties.frame_duration, 
                                 0, 
                                 packet, DEFAULT_MTU); 
            len -= payload; 
            off += payload; 
        } 
    } 
    memcpy(packet + AU_HEADER_SIZE, data + off, len); 
    mparser_buffer_write(tr, 
                         tr->properties.pts, 
                         tr->properties.dts, 
                         tr->properties.frame_duration, 
                         1, 
                         packet, len + AU_HEADER_SIZE); 
    g_free(packet); 
    return ERR_NOERROR; 
} 
static int aac_parse(Track *tr, uint8_t *data, size_t len)  
{  
    //XXX handle the last packet on EOF  
    int off = 0;  
    uint32_t payload = DEFAULT_MTU - AU_HEADER_SIZE;  
    uint8_t *packet = g_malloc0(DEFAULT_MTU);  
    if(!packet) return ERR_ALLOC;  
// trim away extradata  
//    data += AAC_EXTRA;  
//    len -= AAC_EXTRA;  
    packet[0] = 0x00;  
    packet[1] = 0x10;  
    packet[2] = (len & 0x1fe0) >> 5;  
    packet[3] = (len & 0x1f) << 3;  
    if (len > payload) {  
        while (len > payload) {  
            memcpy(packet + AU_HEADER_SIZE, data + off, payload);  
            mparser_buffer_write(tr,  
                                 tr->properties.pts,  
                                 tr->properties.dts,  
                                 tr->properties.frame_duration,  
                                 0,  
                                 packet, DEFAULT_MTU);  
            len -= payload;  
            off += payload;  
        }  
    }  
    memcpy(packet + AU_HEADER_SIZE, data + off, len);  
    mparser_buffer_write(tr,  
                         tr->properties.pts,  
                         tr->properties.dts,  
                         tr->properties.frame_duration,  
                         1,  
                         packet, len + AU_HEADER_SIZE);  
    g_free(packet);  
    return ERR_NOERROR;  
}  
```



## 荷载视频

​		荷载视频有三种模式：单一帧包，分片帧包，组合帧包。在H264中详细介绍。

​		[H.264视频的RTP荷载格式 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/4245439.html)



### H264

- 单一NALU模式

  ```
  以SPS为例，SPS一般小于MTU，可采用单NALU封装模式
  
  80 e0 be 8e 8c e8 56 d5 4a 9b 57 b3 | 67 | 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a
  - 80 e0 be 8e 8c e8 56 d5 4a 9b 57 b3 为RTP头
      - V=10B=2
      - Padding=0
      - Extension=0
      - CC=0
      - Mark=1
      - PT=96
      - SN=48782
      - TS=2364036821
      - SSRC=0x4a9b57b3
  - RTP头之后一个字节为NALU头
    - 就是SPS帧去掉00000001起始标识后的数据，为67
  - 之后为NALU单元RBSP数据
  	在编码时简单的做法就是RTP头后直接附加去除起始标识的NALU数据即可
  ```

- 组合包模式	

  ```
  此处以STAP-A组合类型为例，组合发送SPS和PPS。假设SPS和PPS的裸流如下
  
  //sps
  00 00 00 01 | 67 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a1 01 51 58 ae 2b 15 1d 08 0a 8a c5 71 58 a8 e8 40 54 56 2b 8a c5 47 42 02 a2 b1 5c 56 2a 3a 10 24 85 21 39 3c 9f 27 e4 fe 4f c9 f2 79 b9 b3 4d 08 12 42 90 9c 9e 4f 93 f2 7f 27 e4 f9 3c dc d9 a6 b4 03 c0 11 3f 2c a9 00 00 03 00 78 00 00 15 90 60 40 00 3e 80 00 00 11 94 06 f7 be 17 84 42 35
  //pps
  00 00 00 01 | 68 33 3c b0 00 00
  
  // 组合包
  | RTP header 12字节 | 78 【00 75】 【67】 64 00 29 ad 84 05 45 62 b8 ac 54 74 20 2a 2b 15 c5 62 a3 a1 01 51 58 ae 2b 15 1d 08 0a 8a c5 71 58 a8 e8 40 54 56 2b 8a c5 47 42 02 a2 b1 5c 56 2a 3a 10 24 85 21 39 3c 9f 27 e4 fe 4f c9 f2 79 b9 b3 4d 08 12 42 90 9c 9e 4f 93 f2 7f 27 e4 f9 3c dc d9 a6 b4 03 c0 11 3f 2c a9 00 00 03 00 78 00 00 15 90 60 40 00 3e 80 00 00 11 94 06 f7 be 17 84 42 35 【00 06】 【68】 33 3c b0 00 00
      
  - 78 STAP-A NALU头
  	- 其中F为0，NRI为3，type为24
  	- 24标识STAP-A类型，此类型标识后续负载为组合帧
  - SPS去掉起始标志，size为117，十六进制为0x75
  	- NALU 头 67
  - PPS去掉起始标志，size为0x06
  	- NALU 头 68
  ```

- 分片模式

  ```c
  80 60 be  91 8c e8 56 d5 4a 9b 57 b3 | 7c | 85
  
  - 80 60 be  91 8c e8 56 d5 4a 9b 57 b3， RTP header
  - 0x7c， FU indicator，其二进制为：01111100，F=0，NRI=3，type=28表示FU-A分片
  - 0x85， FU header， 其二进制为：10000101，S=1，E=0,type=5,表示I帧的起始FU-A分片
  ```



```c
static int h264_parse(Track *tr, uint8_t *data, size_t len) 
{ 
    h264_priv *priv = tr->private_data; 
//    double nal_time; // see page 9 and 7.4.1.2 
    size_t nalsize = 0, index = 0; 
    uint8_t *p, *q; 
    if (priv->is_avc) { 
        while (1) { 
            unsigned int i; 
            if(index >= len) break; 
            //get the nal size 
            nalsize = 0; 
            for(i = 0; i < priv->nal_length_size; i++) 
                nalsize = (nalsize << 8) | data[index++]; 
            if(nalsize <= 1 || nalsize > len) { 
                if(nalsize == 1) { 
                    index++; 
                    continue; 
                } else { 
                    fnc_log(FNC_LOG_VERBOSE, "[h264] AVC: nal size %d", nalsize); 
                    break; 
                } 
            } 
            if (DEFAULT_MTU >= nalsize) { 
                mparser_buffer_write(tr, 
                                     tr->properties.pts, 
                                     tr->properties.dts, 
                                     tr->properties.frame_duration, 
                                     1, 
                                     data + index, nalsize); 
                fnc_log(FNC_LOG_VERBOSE, "[h264] single NAL"); 
            } else { 
            // single NAL, to be fragmented, FU-A; 
                frag_fu_a(data + index, nalsize, DEFAULT_MTU, tr); 
            } 
            index += nalsize; 
        } 
    } else { 
        //seek to the first startcode 
        for (p = data; p<data + len - 3; p++) { 
            if (p[0] == 0 && p[1] == 0 && p[2] == 1) { 
                break; 
            } 
        } 
        if (p >= data + len) return ERR_PARSE; 
        while (1) { 
        //seek to the next startcode [0 0 1] 
            for (q = p; q<data+len-3;q++) { 
                if (q[0] == 0 && q[1] == 0 && q[2] == 1) { 
                    break; 
                } 
            } 
            if (q >= data + len) break; 
            if (DEFAULT_MTU >= q - p) { 
                fnc_log(FNC_LOG_VERBOSE, "[h264] Sending NAL %d",p[0]&0x1f); 
                mparser_buffer_write(tr, 
                                     tr->properties.pts, 
                                     tr->properties.dts, 
                                     tr->properties.frame_duration, 
                                     1, 
                                     p, q - p); 
                fnc_log(FNC_LOG_VERBOSE, "[h264] single NAL"); 
            } else { 
                //FU-A 
                fnc_log(FNC_LOG_VERBOSE, "[h264] frags"); 
                frag_fu_a(p, q - p, DEFAULT_MTU, tr); 
            } 
            p = q; 
        } 
        // last NAL 
        fnc_log(FNC_LOG_VERBOSE, "[h264] last NAL %d",p[0]&0x1f); 
        if (DEFAULT_MTU >= len - (p - data)) { 
            fnc_log(FNC_LOG_VERBOSE, "[h264] no frags"); 
            mparser_buffer_write(tr, 
                                 tr->properties.pts, 
                                 tr->properties.dts, 
                                 tr->properties.frame_duration, 
                                 1, 
                                 p, len - (p - data)); 
        } else { 
            //FU-A 
            fnc_log(FNC_LOG_VERBOSE, "[h264] frags"); 
            frag_fu_a(p, len - (p - data), DEFAULT_MTU, tr); 
        } 
    } 
    fnc_log(FNC_LOG_VERBOSE, "[h264] Frame completed"); 
    return ERR_NOERROR; 
} 
static int h264_parse(Track *tr, uint8_t *data, size_t len)  
{  
    h264_priv *priv = tr->private_data;  
//    double nal_time; // see page 9 and 7.4.1.2  
    size_t nalsize = 0, index = 0;  
    uint8_t *p, *q;  
    if (priv->is_avc) {  
        while (1) {  
            unsigned int i;  
            if(index >= len) break;  
            //get the nal size  
            nalsize = 0;  
            for(i = 0; i < priv->nal_length_size; i++)  
                nalsize = (nalsize << 8) | data[index++];  
            if(nalsize <= 1 || nalsize > len) {  
                if(nalsize == 1) {  
                    index++;  
                    continue;  
                } else {  
                    fnc_log(FNC_LOG_VERBOSE, "[h264] AVC: nal size %d", nalsize);  
                    break;  
                }  
            }  
            if (DEFAULT_MTU >= nalsize) {  
                mparser_buffer_write(tr,  
                                     tr->properties.pts,  
                                     tr->properties.dts,  
                                     tr->properties.frame_duration,  
                                     1,  
                                     data + index, nalsize);  
                fnc_log(FNC_LOG_VERBOSE, "[h264] single NAL");  
            } else {  
            // single NAL, to be fragmented, FU-A;  
                frag_fu_a(data + index, nalsize, DEFAULT_MTU, tr);  
            }  
            index += nalsize;  
        }  
    } else {  
        //seek to the first startcode  
        for (p = data; p<data + len - 3; p++) {  
            if (p[0] == 0 && p[1] == 0 && p[2] == 1) {  
                break;  
            }  
        }  
        if (p >= data + len) return ERR_PARSE;  
        while (1) {  
        //seek to the next startcode [0 0 1]  
            for (q = p; q<data+len-3;q++) {  
                if (q[0] == 0 && q[1] == 0 && q[2] == 1) {  
                    break;  
                }  
            }  
            if (q >= data + len) break;  
            if (DEFAULT_MTU >= q - p) {  
                fnc_log(FNC_LOG_VERBOSE, "[h264] Sending NAL %d",p[0]&0x1f);  
                mparser_buffer_write(tr,  
                                     tr->properties.pts,  
                                     tr->properties.dts,  
                                     tr->properties.frame_duration,  
                                     1,  
                                     p, q - p);  
                fnc_log(FNC_LOG_VERBOSE, "[h264] single NAL");  
            } else {  
                //FU-A  
                fnc_log(FNC_LOG_VERBOSE, "[h264] frags");  
                frag_fu_a(p, q - p, DEFAULT_MTU, tr);  
            }  
            p = q;  
        }  
        // last NAL  
        fnc_log(FNC_LOG_VERBOSE, "[h264] last NAL %d",p[0]&0x1f);  
        if (DEFAULT_MTU >= len - (p - data)) {  
            fnc_log(FNC_LOG_VERBOSE, "[h264] no frags");  
            mparser_buffer_write(tr,  
                                 tr->properties.pts,  
                                 tr->properties.dts,  
                                 tr->properties.frame_duration,  
                                 1,  
                                 p, len - (p - data));  
        } else {  
            //FU-A  
            fnc_log(FNC_LOG_VERBOSE, "[h264] frags");  
            frag_fu_a(p, len - (p - data), DEFAULT_MTU, tr);  
        }  
    }  
    fnc_log(FNC_LOG_VERBOSE, "[h264] Frame completed");  
    return ERR_NOERROR;  
}  
```



### H265

​		负载H265时，单一包和组合包的处理同H264。分片包有所不同。

​		` PayloadHeader + FU header + DON + FU payload + RTP padding`

- `PayloadHeader`即 `NALU Header`

  - 但`type`为`49`

  - ```c
    // h265 nalu header
    nal_unit_head{
        forbidden_zero_bit(1bit):    禁止位 == F
        nal_unit_type(6bit):        NALU类型
        nuh_reserved_zero_6bits(6bit): == layerid
        nuh_temporal_id_plis1(3bit): == TID
    };
    ```

- `FU header`，`1byte`，同H264

  - `S`：start 标记位，当该位为 1 时表示 NALU 的第一个分片
  - `E`：end 标记位，当该位为 1 时表示 NALU 的最后一个分片
  - `R`：保留位，接收者可以忽略该位。
  - `Type`：实际完整`NALU `的 `Type `类型（1-23）

- `DON`：Decoding Order Number

  - 当使用多slice编码模式时使用，用于判断一帧的每个slice是否收齐
  - 一般使用单slice，无此字段
  - 所以通常境况下，单一帧模式封装方式与H264一致，H265帧去掉起始位直接作为负载

```
80 60 94 22 c6 59 8d 6a 3c c5 fa f7 | 62 01 | 93

- 80 60 94 22 c6 59 8d 6a 3c c5 fa f7， RTP header
- 62 01， Payload header
	TID=01，LayID=0,type=49,F=0
- 93， FU header
	S=1,E=0,Futype=19，表示IDR_W_RADL
```



### 代码

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





## 荷载ps 流

[流媒体基础-RTP封装PS流 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/595923734)

[RTP协议全解析（H264码流和PS流） - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/5570286.html)

​		针对H264 做如下PS 封装：

- 每个`IDR NALU `前一般都会包含`SPS`、`PPS `、`SEI`等`NALU`
  - 这几个包各自打包为PES后，加在IDR PES之前，共同封装为一个PS
  
  - 一般情况下IDR帧很大，超过了RTP的负载长度限制（1400字节），所以上面这一个I帧要拆分成若干包RTP分多次发送
  
  - 所以一个`IDR PS` 包的首包由外到内顺序是：
  
    ​	`PS header | PS system header | PS system Map| PES SPS | PES PPS | PES SEI | PES I Frame`
  
    - `PES SPS` ：`PES header | SPS nalu`
    - `PES PPS`：`PES header | PPS nalu`
    - `PES SEI`：`PES header | SEI nalu`
    - `PES I Frame`：`PES header | h264 nalu`
  
  - 后续包没有PS相关结构，有传输协议直接负载剩余数据
  
    - `h264 nalu`
    - 其中`h264 nalu`是指接上一包中的`h264 nalu`数据
  
- 对于其它非关键帧的`PS `包，直接加上`PS`头和`PES `头就可以了
  - 顺序为：`PS header | PES header | h264 nalu`
  - P/B帧大小一般不超过1400字节，如果超过1400字节，也需分成多包进行传输，同IDR分包
  
- 当有音频数据时，将数据加上`PES header` 放到视频`PES `后就可以了
  - 顺序如下：`PS header  | PES header(video) | h264 raw data | PES header(audio) | aac raw data`
  
- 再用`RTP header `封装发送就可以了

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







