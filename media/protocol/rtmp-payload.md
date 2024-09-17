# rtmp-payload

---





## payload aac

​		从FAAC里面压缩出来的一帧音频数据，要经过简单处理才能打包用RTMP协议发送到FMS上。包括保存成FLV文件，都要稍微处理一下，主要是把AAC的帧头去掉，并提取出相应的信息。

- 先发送一个`AAC sequence header`包：
  - `Audio tag(0xAF 00)` + `AudioSpecificConfig`
- 每帧AAC数据包，去掉ADTS头（7字节或9字节），加上`Audio tag`
  - `Audio tag(0xAF 01) + Raw AAC frame data`

![image-20240917205724790](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917205724790.png)

### aac 小述

​		AAC的帧头一般7个字节，或者包含CRC校验的话9个字节，这里面包括了声音的相关参数。

```
AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP (QQQQQQQQ QQQQQQQQ)
```

| Letter | Length (bits) | Description                                                  |
| ------ | ------------- | ------------------------------------------------------------ |
| A      | 12            | syncword 0xFFF, all bits must be 1                           |
| B      | 1             | MPEG Version: 0 for MPEG-4, 1 for MPEG-2                     |
| C      | 2             | Layer: always 0                                              |
| D      | 1             | protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC |
| E      | 2             | profile, the [MPEG-4 Audio Object Type](http://sun3eyes.blog.163.com/index.php?title=MPEG-4_Audio#Audio_Object_Types) minus 1 |
| F      | 4             | [MPEG-4 Sampling Frequency Index](http://sun3eyes.blog.163.com/index.php?title=MPEG-4_Audio#Sampling_Frequencies) (15 is forbidden) |
| G      | 1             | private stream, set to 0 when encoding, ignore when decoding |
| H      | 3             | [MPEG-4 Channel Configuration](http://sun3eyes.blog.163.com/index.php?title=MPEG-4_Audio#Channel_Configurations) (in the case of 0, the channel configuration is sent via an inband PCE) |
| I      | 1             | originality, set to 0 when encoding, ignore when decoding    |
| J      | 1             | home, set to 0 when encoding, ignore when decoding           |
| K      | 1             | copyrighted stream, set to 0 when encoding, ignore when decoding |
| L      | 1             | copyright start, set to 0 when encoding, ignore when decoding |
| M      | 13            | frame length, this value must include 7 or 9 bytes of header length: FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame) |
| O      | 11            | Buffer fullness                                              |
| P      | 2             | Number of AAC frames (RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame per ADTS frame |
| Q      | 16            | CRC if protection absent is 0                                |

- E  类型
  -  [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) Main
  -  [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) LC (Low Complexity
  -  [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) SSR (Scalable Sample Rate)
  -  [AAC](http://sun3eyes.blog.163.com/index.php?title=AAC) LTP (Long Term Prediction)
- F 采样频率
  - 0: 96000 Hz
  - 1: 88200 Hz
  - 2: 64000 Hz
  - 3: 48000 Hz
  - 4: 44100 Hz
  - 5: 32000 Hz
  - 6: 24000 Hz
  - 7: 22050 Hz
  - 8: 16000 Hz
  - 9: 12000 Hz
  - 10: 11025 Hz
  - 11: 8000 Hz
  - 12: 7350 Hz
- H 声道
  - 1: 1 channel: front-center
  - 2: 2 channels: front-left, front-right
  - 3: 3 channels: front-center, front-left, front-right
  - 4: 4 channels: front-center, front-left, front-right, back-center
  - 5: 5 channels: front-center, front-left, front-right, back-left, back-right
  - 6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
  - 7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel



### Audio Tag

​		`2 byte`描述AAC头的编码信息

- `4 bit`，指定音频的格式

  - 0 = Linear PCM, platform endian
    1 = ADPCM
    2 = MP3
    3 = Linear PCM, little endian
    4 = Nellymoser 16 kHz mono
    5 = Nellymoser 8 kHz mono
    6 = Nellymoser
    7 = G.711 A-law logarithmic PCM
    8 = G.711 mu-law logarithmic PCM
    9 = reserved
    **10 = AAC**
    11 = Speex
    14 = MP3 8 kHz
    15 = Device-specific sound

    > Formats 7, 8, 14, and 15 are reserved.
    > AAC is supported in Flash Player 9,0,115,0 and higher.
    > Speex is supported in Flash Player 10 and higher.

- `2 bit`，指定采样率

  - 0 = 5.5 kHz
    1 = 11 kHz
    2 = 22 kHz
    3 = 44 kHz

- `1 bit`，位深

  - 0 = 8-bit samples
    1 = 16-bit samples

- `1 bit`，声道

  - 0 = Mono sound
    1 = Stereo sound

- `1 byte`，AACPacketType，AACAUDIODATA的类型

  - 0 = AAC sequence header
  - 1 = AAC raw
  - 第一个音频包用0，后面的都用1



​		Tag后的数据，`8 bit * n`，n字节数据

- AAC sequence header时，AudioSpecificConfig
- AAC raw，Raw AAC frame Data



### AudioSpecificConfig

​		是**AudioSpecificConfig**结构，该结构则在“**ISO-14496-3 Audio**”中描述。

​		`2 byte`，由``AudioSpecificConfig``指定，该结构则在“ISO-14496-3 Audio”中描述

- `5 bit`，音频结构类型

  - 1，表示 ObjectProfile=1， AAC-MAIN
  - 2，表示 ObjectProfile=2， AAC-LC
  - 3，表示 ObjectProfile=3， AAC-SSR

- `4 bit`，采样率索引

  - 0: 96000 Hz

  - 1: 88200 Hz

  - 2: 64000 Hz

  - 3: 48000 Hz

  - 4: 44100 Hz

  - 5: 32000 Hz

  - 6: 24000 Hz

  - 7: 22050 Hz

  - 8: 16000 Hz

  - 9: 12000 Hz

  - 10: 11025 Hz

  - 11: 8000 Hz

  - 12: 7350 Hz

  - 13: Reserved

  - 14: Reserved

  - 15: frequency is written explictly

    > ​		通常aac固定选中44100，即应该对应为4，但是试验结果表明，当音频采样率小于等于44100时，应该选择3，而当音频采样率为48000时，应该选择2；
    >
    > ​		网上有博主说音频采样率小于等于 44100 时 SamplingFrequencyIndex 应当选择 3（48kHz），Bill 测试发现采样率等于 44100 时设置标记为 3 或 4 均能正常推送并在客户端播放，不过我们还是应当按照标准规定的行事，故此处的 SamplingFrequencyIndex 选 4。

- `4 bit`，声道数

  - 0: Defined in AOT Specifc Config
  - 1: 1 channel: front-center
  - 2: 2 channels: front-left, front-right
  - 3: 3 channels: front-center, front-left, front-right
  - 4: 4 channels: front-center, front-left, front-right, back-center
  - 5: 5 channels: front-center, front-left, front-right, back-left, back-right
  - 6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
  - 7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
  - 8-15: Reserved

  > 单声道对应1，双声道对应2，依次类推。

- `1 bit`，IMDCT窗口长度

- `1 bit`，是否依赖corecoder

- `1 bit`，扩展标识

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/2011122913505721.jpg)



### AAC sequence header

​		音频同步包大小固定为 4 个字节。

- 前两个字节被称为`Audio Tag [AACDecoderSpecificInfo]`，用于描述这个音频包应当如何被解析
  - 可以直接使用 FAAC 库的 faacEncGetDecoderSpecificInfo 函数来获取
  - 也可以根据自己的音频源进行计算。一般情况下，双声道，44kHz 采样率的 AAC 音频，其值为 0xAF00
  - `0xAF 00`，一般音频数据包为`0xAF 01`
- 后两个字节称为 `AudioSpecificConfig`，更加详细的指定了音频格式

![image-20240917205823730](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917205823730.png)

![image-20240917210917746](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917210917746.png)

### AAC raw data



![image-20240917210819140](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917210819140.png)

![image-20240917210929736](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917210929736.png)



## payload h264

​		从x264里面压缩出来的一帧音频数据，要经过简单处理才能打包用RTMP协议发送到FMS上。包括保存成FLV文件，都要稍微处理一下，主要是把ADTS的帧头去掉，并提取出相应的信息。

- 先发送一个`AVC sequence header`包：
  - `Video tag` 
- 每帧`AVC`数据包，去掉ADTS头（7字节或9字节），加上`Audio tag`
  - `Video tag + NALU frame data`

### Video Tag

- 4 bit，帧类型
  - **1，key frame (seekable frame)**
  - **2，inter frame (non-seekable-frame)**
  - 3，despoosable frame (h263)
  - 4，generated key frame
  - 5，video info / command frame
- `4 bit`，编码ID
  - 1，JPEG
  - 2，Sorenson H263
  - 3，Screen Video
  - 4，On2 VP6
  - 5，On2 VP6 (with aplha channel)
  - 6，Screen video version 2
  - **7，AVC**
- `8 bit`，AVC包类型
  - 0，AVC sequence header
  - 1，AVC  NALU
  - 2，AVC end of sequence (lower level NALU sequence ender is not required or supported)
- `24 bit`，组包时间戳
  - AVC  NALU包时(上一字段为 1 时)，添加 time offset
    - AVC时，3字节CompositionTime无意义，通常设置为0。
  - 否则为 `0`





​		Tag后的数据，`8 bit * n`，n字节数据

- AVC sequence header包时，`AVCDecoderConfigurationRecord`
- AVC NALU时，一个或多个NALU单元
  - can bne individual slices per FLC packets ; that is , full frames are not strictly required
- AVC end of sequence时，为空

![image-20240917211526660](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917211526660.png)



### AVCDecoderConfigurationRecord

​		是**AVCDecoderConfigurationRecord**结构，该结构在标准文档“**ISO-14496-15 AVC file format**”中有详细说明。

​		AVCDecoderConfigurationRecord主要包含了SPS和PPS的结构。

![image-20240917212925992](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917212925992.png)

- `Video Tag + AVCDecoderConfigurationRecord`

### AVC sequence header

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/2011122913292955.jpg)



### AVC raw data

![image-20240917222703205](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240917222703205.png)



### 代码

​		一个通过RTMP向RTMP server推送H264视频的实例：



```c++
#include <stdio.h>    
#include "RTMPStream\RTMPStream.h"    
    
int main(int argc,char* argv[])    
{    
    CRTMPStream rtmpSender;    
    
    bool bRet = rtmpSender.Connect("rtmp://192.168.1.104/live/test");    
    
    rtmpSender.SendH264File("E:\\video\\test.264");    
    
    rtmpSender.Close();    
} 
```



```c++
/********************************************************************  
filename:   RTMPStream.h 
created:    2013-04-3 
author:     firehood  
purpose:    发送H264视频到RTMP Server，使用libRtmp库 
*********************************************************************/   
#pragma once  
#include "rtmp.h"  
#include "rtmp_sys.h"  
#include "amf.h"  
#include <stdio.h>  
  
#define FILEBUFSIZE (1024 * 1024 * 10)       //  10M  
  
// NALU单元  
typedef struct _NaluUnit  
{  
    int type;  
    int size;  
    unsigned char *data;  
}NaluUnit;  
  
typedef struct _RTMPMetadata  
{  
    // video, must be h264 type  
    unsigned int    nWidth;  
    unsigned int    nHeight;  
    unsigned int    nFrameRate;     // fps  
    unsigned int    nVideoDataRate; // bps  
    unsigned int    nSpsLen;  
    unsigned char   Sps[1024];  
    unsigned int    nPpsLen;  
    unsigned char   Pps[1024];  
  
    // audio, must be aac type  
    bool            bHasAudio;  
    unsigned int    nAudioSampleRate;  
    unsigned int    nAudioSampleSize;  
    unsigned int    nAudioChannels;  
    char            pAudioSpecCfg;  
    unsigned int    nAudioSpecCfgLen;  
  
} RTMPMetadata,*LPRTMPMetadata;  
  
  
class CRTMPStream  
{  
public:  
    CRTMPStream(void);  
    ~CRTMPStream(void);  
public:  
    // 连接到RTMP Server  
    bool Connect(const char* url);  
    // 断开连接  
    void Close();  
    // 发送MetaData  
    bool SendMetadata(LPRTMPMetadata lpMetaData);  
    // 发送H264数据帧  
    bool SendH264Packet(unsigned char *data,unsigned int size,bool bIsKeyFrame,unsigned int nTimeStamp);  
    // 发送H264文件  
    bool SendH264File(const char *pFileName);  
private:  
    // 送缓存中读取一个NALU包  
    bool ReadOneNaluFromBuf(NaluUnit &nalu);  
    // 发送数据  
    int SendPacket(unsigned int nPacketType,unsigned char *data,unsigned int size,unsigned int nTimestamp);  
private:  
    RTMP* m_pRtmp;  
    unsigned char* m_pFileBuf;  
    unsigned int  m_nFileBufSize;  
    unsigned int  m_nCurPos;  
};  
```

```c++
#include <stdio.h>  
#include <math.h>  
  
UINT Ue(BYTE *pBuff, UINT nLen, UINT &nStartBit)  
{  
    //计算0bit的个数  
    UINT nZeroNum = 0;  
    while (nStartBit < nLen * 8)  
    {  
        if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8))) //&:按位与，%取余  
        {  
            break;  
        }  
        nZeroNum++;  
        nStartBit++;  
    }  
    nStartBit ++;  
  
  
    //计算结果  
    DWORD dwRet = 0;  
    for (UINT i=0; i<nZeroNum; i++)  
    {  
        dwRet <<= 1;  
        if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))  
        {  
            dwRet += 1;  
        }  
        nStartBit++;  
    }  
    return (1 << nZeroNum) - 1 + dwRet;  
}  
  
  
int Se(BYTE *pBuff, UINT nLen, UINT &nStartBit)  
{  
    int UeVal=Ue(pBuff,nLen,nStartBit);  
    double k=UeVal;  
    int nValue=ceil(k/2);//ceil函数：ceil函数的作用是求不小于给定实数的最小整数。ceil(2)=ceil(1.2)=cei(1.5)=2.00  
    if (UeVal % 2==0)  
        nValue=-nValue;  
    return nValue;  
}  
  
  
DWORD u(UINT BitCount,BYTE * buf,UINT &nStartBit)  
{  
    DWORD dwRet = 0;  
    for (UINT i=0; i<BitCount; i++)  
    {  
        dwRet <<= 1;  
        if (buf[nStartBit / 8] & (0x80 >> (nStartBit % 8)))  
        {  
            dwRet += 1;  
        }  
        nStartBit++;  
    }  
    return dwRet;  
}  
  
  
bool h264_decode_sps(BYTE * buf,unsigned int nLen,int &width,int &height)  
{  
    UINT StartBit=0;   
    int forbidden_zero_bit=u(1,buf,StartBit);  
    int nal_ref_idc=u(2,buf,StartBit);  
    int nal_unit_type=u(5,buf,StartBit);  
    if(nal_unit_type==7)  
    {  
        int profile_idc=u(8,buf,StartBit);  
        int constraint_set0_flag=u(1,buf,StartBit);//(buf[1] & 0x80)>>7;  
        int constraint_set1_flag=u(1,buf,StartBit);//(buf[1] & 0x40)>>6;  
        int constraint_set2_flag=u(1,buf,StartBit);//(buf[1] & 0x20)>>5;  
        int constraint_set3_flag=u(1,buf,StartBit);//(buf[1] & 0x10)>>4;  
        int reserved_zero_4bits=u(4,buf,StartBit);  
        int level_idc=u(8,buf,StartBit);  
  
        int seq_parameter_set_id=Ue(buf,nLen,StartBit);  
  
        if( profile_idc == 100 || profile_idc == 110 ||  
            profile_idc == 122 || profile_idc == 144 )  
        {  
            int chroma_format_idc=Ue(buf,nLen,StartBit);  
            if( chroma_format_idc == 3 )  
                int residual_colour_transform_flag=u(1,buf,StartBit);  
            int bit_depth_luma_minus8=Ue(buf,nLen,StartBit);  
            int bit_depth_chroma_minus8=Ue(buf,nLen,StartBit);  
            int qpprime_y_zero_transform_bypass_flag=u(1,buf,StartBit);  
            int seq_scaling_matrix_present_flag=u(1,buf,StartBit);  
  
            int seq_scaling_list_present_flag[8];  
            if( seq_scaling_matrix_present_flag )  
            {  
                for( int i = 0; i < 8; i++ ) {  
                    seq_scaling_list_present_flag[i]=u(1,buf,StartBit);  
                }  
            }  
        }  
        int log2_max_frame_num_minus4=Ue(buf,nLen,StartBit);  
        int pic_order_cnt_type=Ue(buf,nLen,StartBit);  
        if( pic_order_cnt_type == 0 )  
            int log2_max_pic_order_cnt_lsb_minus4=Ue(buf,nLen,StartBit);  
        else if( pic_order_cnt_type == 1 )  
        {  
            int delta_pic_order_always_zero_flag=u(1,buf,StartBit);  
            int offset_for_non_ref_pic=Se(buf,nLen,StartBit);  
            int offset_for_top_to_bottom_field=Se(buf,nLen,StartBit);  
            int num_ref_frames_in_pic_order_cnt_cycle=Ue(buf,nLen,StartBit);  
  
            int *offset_for_ref_frame=new int[num_ref_frames_in_pic_order_cnt_cycle];  
            for( int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )  
                offset_for_ref_frame[i]=Se(buf,nLen,StartBit);  
            delete [] offset_for_ref_frame;  
        }  
        int num_ref_frames=Ue(buf,nLen,StartBit);  
        int gaps_in_frame_num_value_allowed_flag=u(1,buf,StartBit);  
        int pic_width_in_mbs_minus1=Ue(buf,nLen,StartBit);  
        int pic_height_in_map_units_minus1=Ue(buf,nLen,StartBit);  
  
        width=(pic_width_in_mbs_minus1+1)*16;  
        height=(pic_height_in_map_units_minus1+1)*16;  
  
        return true;  
    }  
    else  
        return false;  
}  
```



```c++
/********************************************************************  
filename:   RTMPStream.cpp 
created:    2013-04-3 
author:     firehood  
purpose:    发送H264视频到RTMP Server，使用libRtmp库 
*********************************************************************/   
#include "RTMPStream.h"  
#include "SpsDecode.h"  
#ifdef WIN32    
#include <windows.h>  
#endif  
  
#ifdef WIN32  
#pragma comment(lib,"WS2_32.lib")  
#pragma comment(lib,"winmm.lib")  
#endif  
  
enum  
{  
    FLV_CODECID_H264 = 7,  
};  
  
int InitSockets()    
{    
#ifdef WIN32    
    WORD version;    
    WSADATA wsaData;    
    version = MAKEWORD(1, 1);    
    return (WSAStartup(version, &wsaData) == 0);    
#else    
    return TRUE;    
#endif    
}    
  
inline void CleanupSockets()    
{    
#ifdef WIN32    
    WSACleanup();    
#endif    
}    
  
char * put_byte( char *output, uint8_t nVal )    
{    
    output[0] = nVal;    
    return output+1;    
}    
char * put_be16(char *output, uint16_t nVal )    
{    
    output[1] = nVal & 0xff;    
    output[0] = nVal >> 8;    
    return output+2;    
}    
char * put_be24(char *output,uint32_t nVal )    
{    
    output[2] = nVal & 0xff;    
    output[1] = nVal >> 8;    
    output[0] = nVal >> 16;    
    return output+3;    
}    
char * put_be32(char *output, uint32_t nVal )    
{    
    output[3] = nVal & 0xff;    
    output[2] = nVal >> 8;    
    output[1] = nVal >> 16;    
    output[0] = nVal >> 24;    
    return output+4;    
}    
char *  put_be64( char *output, uint64_t nVal )    
{    
    output=put_be32( output, nVal >> 32 );    
    output=put_be32( output, nVal );    
    return output;    
}    
char * put_amf_string( char *c, const char *str )    
{    
    uint16_t len = strlen( str );    
    c=put_be16( c, len );    
    memcpy(c,str,len);    
    return c+len;    
}    
char * put_amf_double( char *c, double d )    
{    
    *c++ = AMF_NUMBER;  /* type: Number */    
    {    
        unsigned char *ci, *co;    
        ci = (unsigned char *)&d;    
        co = (unsigned char *)c;    
        co[0] = ci[7];    
        co[1] = ci[6];    
        co[2] = ci[5];    
        co[3] = ci[4];    
        co[4] = ci[3];    
        co[5] = ci[2];    
        co[6] = ci[1];    
        co[7] = ci[0];    
    }    
    return c+8;    
}  
  
CRTMPStream::CRTMPStream(void):  
m_pRtmp(NULL),  
m_nFileBufSize(0),  
m_nCurPos(0)  
{  
    m_pFileBuf = new unsigned char[FILEBUFSIZE];  
    memset(m_pFileBuf,0,FILEBUFSIZE);  
    InitSockets();  
    m_pRtmp = RTMP_Alloc();    
    RTMP_Init(m_pRtmp);    
}  
  
CRTMPStream::~CRTMPStream(void)  
{  
    Close();  
    WSACleanup();    
    delete[] m_pFileBuf;  
}  
  
bool CRTMPStream::Connect(const char* url)  
{  
    if(RTMP_SetupURL(m_pRtmp, (char*)url)<0)  
    {  
        return FALSE;  
    }  
    RTMP_EnableWrite(m_pRtmp);  
    if(RTMP_Connect(m_pRtmp, NULL)<0)  
    {  
        return FALSE;  
    }  
    if(RTMP_ConnectStream(m_pRtmp,0)<0)  
    {  
        return FALSE;  
    }  
    return TRUE;  
}  
  
void CRTMPStream::Close()  
{  
    if(m_pRtmp)  
    {  
        RTMP_Close(m_pRtmp);  
        RTMP_Free(m_pRtmp);  
        m_pRtmp = NULL;  
    }  
}  
  
int CRTMPStream::SendPacket(unsigned int nPacketType,unsigned char *data,unsigned int size,unsigned int nTimestamp)  
{  
    if(m_pRtmp == NULL)  
    {  
        return FALSE;  
    }  
  
    RTMPPacket packet;  
    RTMPPacket_Reset(&packet);  
    RTMPPacket_Alloc(&packet,size);  
  
    packet.m_packetType = nPacketType;  
    packet.m_nChannel = 0x04;    
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;    
    packet.m_nTimeStamp = nTimestamp;    
    packet.m_nInfoField2 = m_pRtmp->m_stream_id;  
    packet.m_nBodySize = size;  
    memcpy(packet.m_body,data,size);  
  
    int nRet = RTMP_SendPacket(m_pRtmp,&packet,0);  
  
    RTMPPacket_Free(&packet);  
  
    return nRet;  
}  
  
bool CRTMPStream::SendMetadata(LPRTMPMetadata lpMetaData)  
{  
    if(lpMetaData == NULL)  
    {  
        return false;  
    }  
    char body[1024] = {0};;  
      
    char * p = (char *)body;    
    p = put_byte(p, AMF_STRING );  
    p = put_amf_string(p , "@setDataFrame" );  
  
    p = put_byte( p, AMF_STRING );  
    p = put_amf_string( p, "onMetaData" );  
  
    p = put_byte(p, AMF_OBJECT );    
    p = put_amf_string( p, "copyright" );    
    p = put_byte(p, AMF_STRING );    
    p = put_amf_string( p, "firehood" );    
  
    p =put_amf_string( p, "width");  
    p =put_amf_double( p, lpMetaData->nWidth);  
  
    p =put_amf_string( p, "height");  
    p =put_amf_double( p, lpMetaData->nHeight);  
  
    p =put_amf_string( p, "framerate" );  
    p =put_amf_double( p, lpMetaData->nFrameRate);   
  
    p =put_amf_string( p, "videocodecid" );  
    p =put_amf_double( p, FLV_CODECID_H264 );  
  
    p =put_amf_string( p, "" );  
    p =put_byte( p, AMF_OBJECT_END  );  
  
    int index = p-body;  
  
    SendPacket(RTMP_PACKET_TYPE_INFO,(unsigned char*)body,p-body,0);  
  
    int i = 0;  
    body[i++] = 0x17; // 1:keyframe  7:AVC  
    body[i++] = 0x00; // AVC sequence header  
  
    body[i++] = 0x00;  
    body[i++] = 0x00;  
    body[i++] = 0x00; // fill in 0;  
  
    // AVCDecoderConfigurationRecord.  
    body[i++] = 0x01; // configurationVersion  
    body[i++] = lpMetaData->Sps[1]; // AVCProfileIndication  
    body[i++] = lpMetaData->Sps[2]; // profile_compatibility  
    body[i++] = lpMetaData->Sps[3]; // AVCLevelIndication   
    body[i++] = 0xff; // lengthSizeMinusOne    
  
    // sps nums  
    body[i++] = 0xE1; //&0x1f  
    // sps data length  
    body[i++] = lpMetaData->nSpsLen>>8;  
    body[i++] = lpMetaData->nSpsLen&0xff;  
    // sps data  
    memcpy(&body[i],lpMetaData->Sps,lpMetaData->nSpsLen);  
    i= i+lpMetaData->nSpsLen;  
  
    // pps nums  
    body[i++] = 0x01; //&0x1f  
    // pps data length   
    body[i++] = lpMetaData->nPpsLen>>8;  
    body[i++] = lpMetaData->nPpsLen&0xff;  
    // sps data  
    memcpy(&body[i],lpMetaData->Pps,lpMetaData->nPpsLen);  
    i= i+lpMetaData->nPpsLen;  
  
    return SendPacket(RTMP_PACKET_TYPE_VIDEO,(unsigned char*)body,i,0);  
  
}  
  
bool CRTMPStream::SendH264Packet(unsigned char *data,unsigned int size,bool bIsKeyFrame,unsigned int nTimeStamp)  
{  
    if(data == NULL && size<11)  
    {  
        return false;  
    }  
  
    unsigned char *body = new unsigned char[size+9];  
  
    int i = 0;  
    if(bIsKeyFrame)  
    {  
        body[i++] = 0x17;// 1:Iframe  7:AVC  
    }  
    else  
    {  
        body[i++] = 0x27;// 2:Pframe  7:AVC  
    }  
    body[i++] = 0x01;// AVC NALU  
    body[i++] = 0x00;  
    body[i++] = 0x00;  
    body[i++] = 0x00;  
  
    // NALU size  
    body[i++] = size>>24;  
    body[i++] = size>>16;  
    body[i++] = size>>8;  
    body[i++] = size&0xff;;  
  
    // NALU data  
    memcpy(&body[i],data,size);  
  
    bool bRet = SendPacket(RTMP_PACKET_TYPE_VIDEO,body,i+size,nTimeStamp);  
  
    delete[] body;  
  
    return bRet;  
}  
  
bool CRTMPStream::SendH264File(const char *pFileName)  
{  
    if(pFileName == NULL)  
    {  
        return FALSE;  
    }  
    FILE *fp = fopen(pFileName, "rb");    
    if(!fp)    
    {    
        printf("ERROR:open file %s failed!",pFileName);  
    }    
    fseek(fp, 0, SEEK_SET);  
    m_nFileBufSize = fread(m_pFileBuf, sizeof(unsigned char), FILEBUFSIZE, fp);  
    if(m_nFileBufSize >= FILEBUFSIZE)  
    {  
        printf("warning : File size is larger than BUFSIZE\n");  
    }  
    fclose(fp);    
  
    RTMPMetadata metaData;  
    memset(&metaData,0,sizeof(RTMPMetadata));  
  
    NaluUnit naluUnit;  
    // 读取SPS帧  
    ReadOneNaluFromBuf(naluUnit);  
    metaData.nSpsLen = naluUnit.size;  
    memcpy(metaData.Sps,naluUnit.data,naluUnit.size);  
  
    // 读取PPS帧  
    ReadOneNaluFromBuf(naluUnit);  
    metaData.nPpsLen = naluUnit.size;  
    memcpy(metaData.Pps,naluUnit.data,naluUnit.size);  
  
    // 解码SPS,获取视频图像宽、高信息  
    int width = 0,height = 0;  
    h264_decode_sps(metaData.Sps,metaData.nSpsLen,width,height);  
    metaData.nWidth = width;  
    metaData.nHeight = height;  
    metaData.nFrameRate = 25;  
     
    // 发送MetaData  
    SendMetadata(&metaData);  
  
    unsigned int tick = 0;  
    while(ReadOneNaluFromBuf(naluUnit))  
    {  
        bool bKeyframe  = (naluUnit.type == 0x05) ? TRUE : FALSE;  
        // 发送H264数据帧  
        SendH264Packet(naluUnit.data,naluUnit.size,bKeyframe,tick);  
        msleep(40);  
        tick +=40;  
    }  
  
    return TRUE;  
}  
  
bool CRTMPStream::ReadOneNaluFromBuf(NaluUnit &nalu)  
{  
    int i = m_nCurPos;  
    while(i<m_nFileBufSize)  
    {  
        if(m_pFileBuf[i++] == 0x00 &&  
            m_pFileBuf[i++] == 0x00 &&  
            m_pFileBuf[i++] == 0x00 &&  
            m_pFileBuf[i++] == 0x01  
            )  
        {  
            int pos = i;  
            while (pos<m_nFileBufSize)  
            {  
                if(m_pFileBuf[pos++] == 0x00 &&  
                    m_pFileBuf[pos++] == 0x00 &&  
                    m_pFileBuf[pos++] == 0x00 &&  
                    m_pFileBuf[pos++] == 0x01  
                    )  
                {  
                    break;  
                }  
            }  
            if(pos == nBufferSize)  
            {  
                nalu.size = pos-i;    
            }  
            else  
            {  
                nalu.size = (pos-4)-i;  
            }  
            nalu.type = m_pFileBuf[i]&0x1f;  
            nalu.data = &m_pFileBuf[i];  
  
            m_nCurPos = pos-4;  
            return TRUE;  
        }  
    }  
    return FALSE;  
}  
```





## 时间戳

​		直播流的时间戳不论音频还是视频，在整体时间线上应当呈现递增趋势。

​		时间戳计算方法是按照音视频分开计算，而音频时戳和视频时戳并不是在一条时间线上，这就有可能出现音频时戳在某一个时间点比对应的视频时戳小， 在某一个时间点又跳变到比对应的视频时戳大，导致播放端无法对齐。

​		采用的时间戳为底层发送 RTMP 包的时间，不区分音频流还是视频流，统一使用即将发送 RTMP 包的系统时间作为该包的时间戳。目前局域网测试播放效果良好，音视频同步且流畅。

​		多线程发送可能出现问题，因为可能时间戳出现问题。



