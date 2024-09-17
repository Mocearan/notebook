# librtmp

---



## 参考



[linux 编译安装TRMPdump(libRTMP) - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7194118.html)

[简述RTMPDump与编译移植 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7195931.html)

[使用librtmp进行H264与AAC直播 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7196707.html)

[基于libRTMP的流媒体直播之 AAC、H264 推送 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7217793.html)

[libRTMP使用说明 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7227801.html)

[rtmplib rtmp协议过程分析 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/7355262.html)

[【原】librtmp源码详解 - 仲达超 - 博客园 (cnblogs.com)](https://www.cnblogs.com/Kingfans/p/7064902.html)

[Qt基于librtmp推送H.264](http://blog.csdn.net/caoshangpa/article/details/53125949)



## 流程

```c
/*分配与初始化*/
rtmp = RTMP_Alloc();
RTMP_Init(rtmp);
 
/*设置URL*/
if (RTMP_SetupURL(rtmp,rtmp_url) == FALSE) {
    log(LOG_ERR,"RTMP_SetupURL() failed!");
    RTMP_Free(rtmp);
    return -1;
}
 
/*设置可写,即发布流,这个函数必须在连接前使用,否则无效*/
RTMP_EnableWrite(rtmp);
 
/*连接服务器*/
if (RTMP_Connect(rtmp, NULL) == FALSE) {
    log(LOG_ERR,"RTMP_Connect() failed!");
    RTMP_Free(rtmp);
    return -1;
}
 
/*连接流*/
if (RTMP_ConnectStream(rtmp,0) == FALSE) {
    log(LOG_ERR,"RTMP_ConnectStream() failed!");
    RTMP_Close(rtmp);
    RTMP_Free(rtmp);
    return -1;
}
```



```c
/*定义包头长度,RTMP_MAX_HEADER_SIZE为rtmp.h中定义值为18*/
 
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)
 
RTMPPacket * packet;
unsigned char * body;
 
/*分配包内存和初始化,len为包体长度*/
packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE+len);
memset(packet,0,RTMP_HEAD_SIZE);
 
/*包体内存*/
packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
body = (unsigned char *)packet->m_body;
packet->m_nBodySize = len;
 
/*
* 此处省略包体填充
*/
packet->m_hasAbsTimestamp = 0;
packet->m_packetType = RTMP_PACKET_TYPE_VIDEO; /*此处为类型有两种一种是音频,一种是视频*/
packet->m_nInfoField2 = rtmp->m_stream_id;
packet->m_nChannel = 0x04;
packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
packet->m_nTimeStamp = timeoffset;
 
/*发送*/
if (RTMP_IsConnected(rtmp)) {
    ret = RTMP_SendPacket(rtmp,packet,TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
}
 
/*释放内存*/
free(packet);
```



```c
/*关闭与释放*/
RTMP_Close(rtmp);
RTMP_Free(rtmp);
```



### 下载拉流

- InitSocket()
- RTMP_Init(Struct RTMP)
- RTMP_ParseURL
- RTMP_SetupStream
- fopen
- RTMP_Connect()
- RTMP_ConnectStream
- Download
- CleanUp:RTMP_Close(&rtmp); fclose(file); CleanupSockets();





## 源码

### 宏

```c
#define RTMP_DEFAULT_CHUNKSIZE	128//默认块大小
 
#define RTMP_BUFFER_CACHE_SIZE (16*1024)//开辟16K字节空间
 
#define RTMP_PACKET_TYPE_AUDIO 0x08//音频的消息类型
#define RTMP_PACKET_TYPE_VIDEO 0x09//视频的消息类型
 
#define RTMP_MAX_HEADER_SIZE 18//块基本头+块消息头+扩展时间戳=3+11+4=18
 
#define RTMP_PACKET_SIZE_LARGE    0//块消息头类型0
#define RTMP_PACKET_SIZE_MEDIUM   1//块消息头类型1
#define RTMP_PACKET_SIZE_SMALL    2//块消息头类型2
#define RTMP_PACKET_SIZE_MINIMUM  3//块消息头类型3
```



### struct 

#### RTMP

```c
typedef struct RTMP
{
    int m_inChunkSize;
    int m_outChunkSize;
    int m_nBWCheckCounter;
    int m_nBytesIn;
    int m_nBytesInSent;
    int m_nBufferMS;
    int m_stream_id;
    int m_mediaChannel;
    uint32_t m_mediaStamp;
    uint32_t m_pauseStamp;
    int m_pausing;
    int m_nServerBW;
    int m_nClientBW;
    uint8_t m_nClientBW2;
    uint8_t m_bPlaying;
    uint8_t m_bSendEncoding;
    uint8_t m_bSendCounter;

    int m_numInvokes;
    int m_numCalls;
    RTMP_METHOD *m_methodCalls;
}
```



#### RTMPPacket

​		一个RTMPPacket对应RTMP协议规范里面的一个块（Chunk）

```c
typedef struct RTMPPacket  
  {  
    uint8_t m_headerType;//块消息头的类型（4种）  
    uint8_t m_packetType;//消息类型ID（1-7协议控制；8，9音视频；10以后为AMF编码消息）  
    uint8_t m_hasAbsTimestamp;  //时间戳是绝对值还是相对值  
    int m_nChannel;         //块流ID  
    uint32_t m_nTimeStamp;  //时间戳
    int32_t m_nInfoField2;  //last 4 bytes in a long header,消息流ID   
    uint32_t m_nBodySize;   //消息载荷大小  
    uint32_t m_nBytesRead;  //暂时没用到
    RTMPChunk *m_chunk;     //暂时没用到
    char *m_body;           //消息载荷，可分割为多个块载荷
  } RTMPPacket; 
```

### API

#### InitSocket

```c
int InitSockets()
{
#ifdef WIN32
    WORD version;
    WSADATA wsaData;

    version MAKEWORD(1, 1);       
    return (WSASTartup(version, &wsaData) == 1);
#else
    return TRUE;
#endif
}
```



#### RTMP_Init

```c
void RTMP_Init(RTMP *r)
{
#ifdef CRYPTO
    if(!RTMP_TLS_cts)
        RTMP_TLS_Init();
#endif

    memset(r, 0, sizeof(RTMP));
    r->m_sb.sb_socket = -1;
    r->m_inChunkSize  = RTMP_DEFAULT_CHUNKSIZE;
    r->m_outChunkSize = RTMP_DEFAULT_CHUNKSIZE;
    r->m_nBufferMS    = 30000;
    r->m_nClientBW    = 2500000;
    r->m_nClientBW2   = 2;
    r->m_nServerBW    = 2500000;
    r->m_fAudioCodecs = 3191.0;
    r->m_fVideoCodecs = 252.0;
    r->Link.timeout   = 20;
    r->Link.swfAge    = 30;
}
```



#### RTMP_ParseURL

​		URL 一般由三部分组成: 资源类型、存放资源的主机域名、资源文件名。

​		 语法格式为 `protocol://hostname[:port]/path/[:parameters][?query]#fragment protocol(协议名称)、hostname(主机名)、port(端口号)、path(路径)、parameters(参数)`。

```c
int RTMP_ParseURL(const char *url, int *protocol, AVal *host, unsigned int *port, AVal *playpath, AVal *app);
```





#### RTMP_SendPacket

```c
//queue:TRUE为放进发送队列,FALSE是不放进发送队列,直接发送
int RTMP_SendPacket(RTMP *r, RTMPPacket *packet, int queue)  
{  
  const RTMPPacket *prevPacket = r->m_vecChannelsOut[packet->m_nChannel];  
  uint32_t last = 0;//上一个块的时间戳
  int nSize;//消息载荷大小，可分割为多个块载荷大小
  int hSize;//块头大小
  int cSize;//块基本头大小增量
  char *header;//指向块头起始位置  
  char *hptr;
  char *hend;//指向块头结束位置 
  char hbuf[RTMP_MAX_HEADER_SIZE]; 
  char c;  
  uint32_t t;//相对时间戳  
  char *buffer;//指向消息载荷
  char *tbuf = NULL;
  char *toff = NULL;  
  int nChunkSize;//块载荷大小 
  int tlen;  
  //不是完整块消息头（即不是11字节的块消息头）  
  if (prevPacket && packet->m_headerType != RTMP_PACKET_SIZE_LARGE)  
  {    
      //前一个块和这个块对比
      //原理参考 例子—不分割消息
      if (prevPacket->m_nBodySize == packet->m_nBodySize  
      && prevPacket->m_packetType == packet->m_packetType  
      && packet->m_headerType == RTMP_PACKET_SIZE_MEDIUM)  
    packet->m_headerType = RTMP_PACKET_SIZE_SMALL;  
      //原理参考 例子—分割消息
      if (prevPacket->m_nTimeStamp == packet->m_nTimeStamp  
      && packet->m_headerType == RTMP_PACKET_SIZE_SMALL)  
    packet->m_headerType = RTMP_PACKET_SIZE_MINIMUM;  
      //上一个块的时间戳
      last = prevPacket->m_nTimeStamp;  
  }  
  //非法
  if (packet->m_headerType > 3)
  {  
      RTMP_Log(RTMP_LOGERROR, "sanity failed!! trying to send header of type: 0x%02x.",  
      (unsigned char)packet->m_headerType);  
      return FALSE;  
  }  
  //nSize暂时设置为块头大小；packetSize[] = { 12, 8, 4, 1 }  
  nSize = packetSize[packet->m_headerType]; 
  //块头大小初始化
  hSize = nSize;
  cSize = 0;  
  //相对时间戳，当块时间戳与上一个块时间戳的差值
  t = packet->m_nTimeStamp - last;  
  
  if (packet->m_body)  
  {  
 
      //m_body是指向载荷数据首地址的指针，“-”号用于指针前移 
      //header:块头起始位置 
      header = packet->m_body - nSize;  
      //hend：块头结束位置
      hend = packet->m_body;  
  }  
  else  
  {  
      header = hbuf + 6;  
      hend = hbuf + sizeof(hbuf);  
  }  
  //当块流ID大于319时  
  if (packet->m_nChannel > 319)  
    //块基本头是3个字节  
    cSize = 2;  
  //当块流ID大于63时  
  else if (packet->m_nChannel > 63)  
    //块基本头是2个字节  
    cSize = 1;  
  if (cSize)  
  {  
      //header指针指块头起始位置，“-”号用于指针前移 
      header -= cSize;  
      //当cSize不为0时，块头需要进行扩展，默认的块基本头为1字节，但是也可能是2字节或3字节
      hSize += cSize;  
  }  
  //如果块消息头存在，且相对时间戳大于0xffffff，此时需要使用ExtendTimeStamp  
  if (nSize > 1 && t >= 0xffffff)  
  {  
      header -= 4;  
      hSize += 4;  
  }  
  
  hptr = header;  
  //把块基本头的fmt类型左移6位。 
  c = packet->m_headerType << 6;  
  switch (cSize)  
  {  
    //把块基本头的低6位设置成块流ID
    case 0:  
      c |= packet->m_nChannel;  
      break;  
    //同理，但低6位设置成000000  
    case 1:  
      break;  
    //同理，但低6位设置成000001  
    case 2:  
      c |= 1;  
      break;  
  }  
  //可以拆分成两句*hptr=c;hptr++，此时hptr指向第2个字节  
  *hptr++ = c;  
  //cSize>0，即块基本头大于1字节  
  if (cSize)  
  {  
    //将要放到第2字节的内容tmp  
      int tmp = packet->m_nChannel - 64;  
    //获取低位存储于第2字节  
      *hptr++ = tmp & 0xff;  
    //块基本头是最大的3字节时  
      if (cSize == 2)  
    //获取高位存储于第三个字节（注意：排序使用大端序列，和主机相反）  
    *hptr++ = tmp >> 8;  
  }  
  //块消息头一共有4种，包含的字段数不同，nSize>1,块消息头存在。  
  if (nSize > 1)  
  {  
      //块消息头的最开始三个字节为时间戳，返回值hptr=hptr+3
      hptr = AMF_EncodeInt24(hptr, hend, t > 0xffffff ? 0xffffff : t);  
  }  
  //如果块消息头包括MessageLength+MessageTypeID(4字节)  
  if (nSize > 4)  
  {  
      //消息长度，为消息载荷AMF编码后的长度 
      hptr = AMF_EncodeInt24(hptr, hend, packet->m_nBodySize);  
      //消息类型ID
      *hptr++ = packet->m_packetType;  
  }  
  //消息流ID(4字节)  
  if (nSize > 8)  
    hptr += EncodeInt32LE(hptr, packet->m_nInfoField2);  
    
  //如果块消息头存在，且相对时间戳大于0xffffff，此时需要使用ExtendTimeStamp   
  if (nSize > 1 && t >= 0xffffff)  
    hptr = AMF_EncodeInt32(hptr, hend, t);  
  //消息载荷大小 
  nSize = packet->m_nBodySize;  
  //消息载荷指针
  buffer = packet->m_body;  
  //块大小，默认128字节  
  nChunkSize = r->m_outChunkSize;  
  
  RTMP_Log(RTMP_LOGDEBUG2, "%s: fd=%d, size=%d", __FUNCTION__, r->m_sb.sb_socket,  
      nSize);   
  //使用HTTP  
  if (r->Link.protocol & RTMP_FEATURE_HTTP)  
  {  
    //nSize:消息载荷大小；nChunkSize:块载荷大小 
    //例nSize：307，nChunkSize:128；  
    //可分为（307+128-1）/128=3个  
    //为什么减1？因为除法会只取整数部分！  
    int chunks = (nSize+nChunkSize-1) / nChunkSize;  
    //如果块的个数超过一个  
    if (chunks > 1)  
    {  
    //消息分n块后总的开销：  
    //n个块基本头，1个块消息头，1个消息载荷，这里是没有扩展时间戳的情况  
    //实际中只有第一个块是完整的，剩下的只有块基本头 
    tlen = chunks * (cSize + 1) + nSize + hSize;//这里其实多算了一个块基本头  
    //分配内存  
    tbuf = (char *) malloc(tlen);  
    if (!tbuf)  
       return FALSE;  
    toff = tbuf;  
    }  
  }  
  while (nSize + hSize)  
  {  
      int wrote;  
      //消息载荷小于块载荷（不用分块）  
      if (nSize < nChunkSize)  
      nChunkSize = nSize;  
  
      RTMP_LogHexString(RTMP_LOGDEBUG2, (uint8_t *)header, hSize);  
      RTMP_LogHexString(RTMP_LOGDEBUG2, (uint8_t *)buffer, nChunkSize);  
      if (tbuf)  
      {   
        memcpy(toff, header, nChunkSize + hSize);  
        toff += nChunkSize + hSize;  
      }  
      else  
      {  
        wrote = WriteN(r, header, nChunkSize + hSize);  
        if (!wrote)  
          return FALSE;  
      }  
      //消息载荷长度块载荷长度  
      nSize -= nChunkSize;  
      //Buffer指针前移1个块载荷长度  
      buffer += nChunkSize;  
      hSize = 0;  
        
      //如果消息没有发完  
      if (nSize > 0)  
      {  
        header = buffer - 1;  
        hSize = 1;  
        if (cSize)  
        {  
          header -= cSize;  
          hSize += cSize;  
        }  
        //块基本头第1个字节  
        *header = (0xc0 | c);  
        //如果块基本头大于1字节  
        if (cSize)  
        {  
          int tmp = packet->m_nChannel - 64;  
          header[1] = tmp & 0xff;  
          if (cSize == 2)  
          header[2] = tmp >> 8;  
        }  
       }  
  }  
  if (tbuf)  
  {  
      int wrote = WriteN(r, tbuf, toff-tbuf);  
      free(tbuf);  
      tbuf = NULL;  
      if (!wrote)  
        return FALSE;  
  }  
  
  /* we invoked a remote method */  
  if (packet->m_packetType == 0x14)  
  {  
      AVal method;  
      char *ptr;  
      ptr = packet->m_body + 1;  
      AMF_DecodeString(ptr, &method);  
      RTMP_Log(RTMP_LOGDEBUG, "Invoking %s", method.av_val);  
      /* keep it in call queue till result arrives */  
      if (queue) 
      {  
        int txn;  
        ptr += 3 + method.av_len;  
        txn = (int)AMF_DecodeNumber(ptr);  
        AV_queue(&r->m_methodCalls, &r->m_numCalls, &method, txn);  
      }  
  }  
  
  if (!r->m_vecChannelsOut[packet->m_nChannel])  
    r->m_vecChannelsOut[packet->m_nChannel] = (RTMPPacket *) malloc(sizeof(RTMPPacket));  
  memcpy(r->m_vecChannelsOut[packet->m_nChannel], packet, sizeof(RTMPPacket));  
  return TRUE;  
}  
```





## 打包H264码流

​		将H.264的NALU打包进消息载荷，就是如何给结构体RTMPPacket中的消息载荷m_body赋值

- SPS和PPS：RTMP推送视频流的封装形式和FLV格式相似，需要首先发送"AVC sequence header"
  - 包含的是重要的编码信息，没有它们，解码器将无法解码
  - "AVC sequence header"就是用来打包sps和pps的
- 其它NALU的打包



```c
// 调用x264 获取sps / pps，发送H264数据 

size = x264_encoder_encode(cx->hd,&nal,&n,pic,&pout);
 
int i,last;
for (i = 0,last = 0;i < n;i++) {
    if (nal[i].i_type == NAL_SPS) {
 
        sps_len = nal[i].i_payload-4;
        memcpy(sps,nal[i].p_payload+4,sps_len);
 
    } else if (nal[i].i_type == NAL_PPS) {
 
        pps_len = nal[i].i_payload-4;
        memcpy(pps,nal[i].p_payload+4,pps_len);
 
        /*发送sps pps帧*/
        send_video_sps_pps();    
 
    } else {
 
        /*发送普通帧*/
        send_rtmp_video(nal[i].p_payload,nal[i].i_payload);
    }
    last += nal[i].i_payload;
}
```

- 每一次调用`x264_encoder_encode`输出了n个帧
  - 要从这n个帧里找出sps和pps，剩下的分次全部发送nal
  - sps与pps的帧起始码都是`00 00 00 01`
  - 而普通帧的起始码可能是`00 00 00 01`也有可能`00 00 01`
  - H264的流的第1帧一定是sps帧(包含帧界定符为8个字节),第2帧一定是pps帧
  - sps,pps合成为一帧调用发送函数
  - 其他帧每个帧作为一个NAL发送
  - 如果x264_encoder_encode里没有sps帧与pps帧，则结果除去第一帧的起始码，所有帧做为一个整体调用发送函数，它们的类型是由第一帧类型决定。

### AVC sequence header

​		AVC sequence header其实就是AVCDecoderConfigurationRecord结构，该结构在标准文档“ISO/IEC-14496-15:2004”的5.2.4.1章节中有详细说明。其主要描述了打包SPS和PPS。



```c
int send_video_sps_pps()
{
	RTMPPacket* packet;
    unsigned char * body;
    int i;
    
    packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + 1024);
    memset(packet,0,RTMP_HEAD_SIZE);
    
    packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    body = (unsigned char *)packet->m_body;
    
    // ?
    memcpy(winsys->pps,buf,len);
    winsys->pps_len = len;
    
    // RTMP header
    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = i;
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = 0;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet->m_nInfoField2 = rtmp->m_stream_id;
    
    // AVC payload header
    i = 0;
    // FrameType / CodecID
    body[i++] = 0x17;// 1:Iframe  7:AVC ，元数据当做keyframe发送
    // VideoData
    body[i++] = 0x00; // AVCPacketType: AVC sequence header

    // 24 bit 组包时间戳
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;
    // 或者 直接 i+=3也行，因为缓冲区是提前置0过的
    
    /*AVCDecoderConfigurationRecord*/
    body[i++] = 0x01; // configurationVersion
    body[i++] = sps[1];	// AVCProfileIndication
    body[i++] = sps[2]; // profile_compattibility
    body[i++] = sps[3]; // AVCLevelIndication
    body[i++] = 0xff;	// lengthSizeMinusOne, always 0xFF
    
    /*sps*/
    // reserved 111 / Numbers of Sps (1) 
    body[i++]   = 0xe1; // 0xE1 & 0x1F ==> 0x1110 0001 ==> 0xe1
    // sps data length
    body[i++] = (sps_len >> 8) & 0xff;
    body[i++] = sps_len & 0xff;
    // sps data
    memcpy(&body[i],sps,sps_len);
    i +=  sps_len;
    
    /*pps*/
    // numbers of pps (1)
    body[i++]   = 0x01; // 0x01 & 0xFF == 1
    // pps data length
    body[i++] = (pps_len >> 8) & 0xff;
    body[i++] = (pps_len) & 0xff;
    // pps data
    memcpy(&body[i],pps,pps_len);
    i +=  pps_len;
    
    /*调用发送接口*/
    RTMP_SendPacket(rtmp,packet,TRUE);
    free(packet);    
 
    return 0;
}
```





### 一般NALU

​		去掉起始码后

```c
int send_rtmp_video(unsigned char * buf,int len)
{
    int type;
    long timeoffset;
    RTMPPacket * packet;
    unsigned char * body;

 	timeoffset = GetTickCount() - start_time;  /*start_time为开始直播时的时间戳*/
    
    /*去掉帧起始码*/
    if (buf[2] == 0x00) { /*00 00 00 01*/
        buf += 4;
        len -= 4;
    } else if (buf[2] == 0x01){ /*00 00 01*/
        buf += 3;
        len -= 3;
    }
    type = buf[0] & 0x1f;
    
    packet = (RTMPPacket *)base_malloc(RTMP_HEAD_SIZE + len + 9);
    memset(packet,0,RTMP_HEAD_SIZE);
 
    packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    packet->m_nBodySize = len + 9;
    
    // RTMP header
    packet->m_hasAbsTimestamp = 0;
    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nInfoField2 = winsys->rtmp->m_stream_id;
    packet->m_nChannel = 0x04;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nTimeStamp = timeoffset;
 
    /*send video packet*/
    body = (unsigned char *)packet->m_body;
    memset(body,0,len+9);
    
    // FrameType and CodecID
    if (type == NAL_SLICE_IDR) {
        body[0] = 0x17;// 1:I frame  7:AVC 
    } else {
        body[0] = 0x27;// 2:other frame  7:AVC 
    }
    
    // VideoData
    // AVCPacketType: AVC NALU
    body[1] = 0x01;   /*nal unit*/
    // 24 bit composition time
    body[2] = 0x00;
    body[3] = 0x00;
    body[4] = 0x00;
    // i+= 3，但为什么这里也全置0？
 
    // 4byte NALU size
    body[5] = (len >> 24) & 0xff;
    body[6] = (len >> 16) & 0xff;
    body[7] = (len >>  8) & 0xff;
    body[8] = (len ) & 0xff;
    
    /*copy NALU data*/
    memcpy(&body[9],buf,len);
    
    /*调用发送接口*/
    RTMP_SendPacket(rtmp,packet,TRUE);
    free(packet); 
}
```



## 打包AAC码流

​		RTMP推送音频流的封装形式和FLV格式相似，需要首先发送"AAC sequence header"。

```c
// 获得AAC sequence header的方法

char *buf;
int len;
faacEncGetDecoderSpecificInfo(fh,&buf,&len);
 
memcpy(spec_buf,buf,len);
spec_len = len;
 
/*释放系统内存*/
free(buf);
```



### AAC sequence header



```c
int cap_rtmp_sendaac_spec(unsigned char *spec_buf,int spec_len)
{
    RTMPPacket * packet;
    unsigned char * body;
    int len;
 
    len = spec_len;  /*spec data长度,一般是2*/
 
    packet = (RTMPPacket *)base_malloc(RTMP_HEAD_SIZE+len+2);
    memset(packet,0,RTMP_HEAD_SIZE);
 
    packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    body = (unsigned char *)packet->m_body;
    
    // RTMP header 
    packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet->m_nBodySize = len+2;
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = 0;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = rtmp->m_stream_id;
 
    /*AF 00 + AAC RAW data*/
    // 2 byte tag
    body[0] = 0xAF;
    body[1] = 0x00;
    //  AudioSpecificConfig
    memcpy(&body[2],spec_buf,len); /*spec_buf是AAC sequence header数据*/
    // faacEncGetDecoderSpecificInfo(fh,&buf,&len);
    
    //或者不用faac的方式，手搓
    uint16_t audio_specific_config = 0;
    audio_specific_config |= ((2 << 11) & 0xF800); // 2: AAC LC
    audio_specific_config |= ((4 << 7) & 0x0780); // 4: 44kHz
    audio_specific_config |= ((2 << 3) & 0x78); // 2: Stereo
    audio_specific_config |= 0 & 0x07; // padding: 000
    body[3] = (audio_specific_config >> 8) & 0xFF;
    body[4] = audio_specific_config & 0xFF;
        
    /*调用发送接口*/
    RTMP_SendPacket(rtmp,packet,TRUE);
 
    return TRUE;
}
```



### 一般音频帧

​		AAC的前7个字节（包括帧界定符）对于RTMP服务器来说是无用的，去掉前 7 个字节（若存在 CRC 校验，则去掉前 9 个字节）

 ```c
 void * cap_dialog_send_audio(unsigned char * buf,int len)
 {
     long timeoffset;
     timeoffset = GetTickCount() - start_time;
  
     buf += 7;
     len += 7;
  
     if (len > 0) {
         RTMPPacket * packet;
         unsigned char * body;
  
         packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE+len+2);
         memset(packet,0,RTMP_HEAD_SIZE);
  
         packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
         body = (unsigned char *)packet->m_body;
         
         // RTMP header 
         packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
         packet->m_nBodySize = len+2;
         packet->m_nChannel = 0x04;
         packet->m_nTimeStamp = timeoffset;
         packet->m_hasAbsTimestamp = 0;
         packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
         packet->m_nInfoField2 = rtmp->m_stream_id;
  
         /*AF 01 + AAC RAW data*/
         body[0] = 0xAF;
         body[1] = 0x01;
         
         // aac data，已经在开头跳过了
         memcpy(&body[2],buf,len);
  
         /*调用发送接口*/
         RTMP_SendPacket(rtmp,packet,TRUE);
         free(packet);
     }
     return 0;
 }
 ```

