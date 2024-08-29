# jrtplib

---

​		`JRTPLIB`是一个面向对象的RTP库，它完全遵循RFC 1889设计，在很多场合下是一个非常不错的选择，下面就以JRTPLIB为例，讲述如何在Linux平台上运用RTP协议进行实时流媒体编程。

## 参考

[Windows 7（Win7）下Visual Studio 2012（VS2012）编译jrtplib与MinGW编译jrtplib_jrtplib 编译dll-CSDN博客](https://blog.csdn.net/caoshangpa/article/details/51152541)

## 基本使用

### 初始化

- 使用RTP协议，你必须创建一个`rtpsession`对象。

- 实际创建会话(session)，你必须调用`Create`成员函数.

  - `RTPSessionParams` 必须明确设置该类的时间戳单位参数，否则将不会成功创建会话。

    > Note that the own timestamp  unit must be set to a valid number, otherwise the session can't be created.
    >
    > ```c++
    > RTPSessionParams sessionparams;
    > sessionparams.SetOwnTimestampUnit(1.0/8000.0);
    > ```

  - 其他会话参数取决于实际RTP配置

  - 如果返回值值为负值，则表示出现了一些错误。 说明此错误代码可以通过调用`RTPGetErrorString`检索：

- 默认情况下，在IPv4发射机使用UDP，对于这个特定的发射器`RTPUDPv4TransmissionParams`

  > ```c++
  > RTPUDPv4TransmissionParams transparams;
  > transparams.SetPortbase(8000);
  > ```

```c++
// 初始化
// 在使用JRTPLIB进行实时流媒体数据传输之前，首先应该生成RTPSession类的一个实例来表示此次RTP会话，然后调用Create() 方法来对其进行初始化操作。

#include "rtpsession.h"

RTPSession sess; 

RTPSessionParams sessionparams;
sessionparams.SetOwnTimestampUnit(1.0/8000.0);

//3.11版jrtplib库的Create方法被修改为Create(sessparams,&transparams)
RTPUDPv4TransmissionParams transparams;
RTPSessionParams sessparams;
sessparams.SetOwnTimestampUnit(1.0/8000.0);/*设置时间戳，1/8000表示1秒钟采样8000次，即录音时的8KHz*/
sessparams.SetAcceptOwnPackets(true);
transparams.SetPortbase(portbase);/*本地通讯端口*/

int status = session.Create(sessionparams, &transparams);
if (status < 0)
{
    std::cerr << RTPGetErrorString(status) << std::endl;
    exit(-1);
}

// 如果会话(session)创建成功，这可能是一个很好的点去指定RTP和RTCP数据应该发送的目的地。
uint8_t localip[]={127,0,0,1};
RTPIPv4Address addr(localip,9000);

status = session.AddDestination(addr);
if (status < 0)
{
    std::cerr << RTPGetErrorString(status) << std::endl;
    exit(-1);
}

// 目标地址 dest 加入到 RTP 会话的接收列表，表示接收从该地址发送的 RTP 数据包。*
// m_VedioRTPSession.AddToAcceptList(dest);

...
// 清理RTPSession类。
// 另外，我们要等待最多10秒，以确保此数据包完成发送，否则次数据包将可能不会被发送。
delay = RTPTime(10.0);
session.BYEDestroy(delay,"Time's up",9);
```



### jthread

​		如果JRTPlib以JThread支持的情况下编译，输入的数据在后台处理。 

​		如果jthread支持在编译时没有被启用 或 如果在会话参数中指定了不使用轮询线程的参数 ，将必须调用RTPSession成员函数来处理传入的数据，必要时定期发送RTCP包。



### rtptime

​		创建包含160个沉默样本缓冲区并创建一个标明20毫秒(或0.020秒)的RTPTime实例。 我们还将存储当前时间，这样我们就知道一分钟过去了。 

```c++
uint8_t silencebuffer[160];

for (int i = 0 ; i < 160 ; i++)
    silencebuffer[i] = 128;

RTPTime delay(0.020);
RTPTime starttime = RTPTime::CurrentTime();
```





### 数据发送

```c++
// 数据发送
// 当RTP 会话成功建立起来之后，接下去就可以开始进行流媒体数据的实时传输了。首先需要设置好数据发送的目标地址， RTP协议允许同一会话存在多个目标地址，这可以通过调用RTPSession类的AddDestination()、 DeleteDestination()和ClearDestinations()方法来完成。
// 注意：如果是需要发到另一个NAT设备后面终端，则需要通过NAT穿透
    
    
//RTP 协议允许同一会话存在多个目标地址
unsigned long addr = ntohl(inet_addr("127.0.0.1"));
sess.AddDestination(addr, 6000);

// 对于同一个RTP会话来讲，负载类型、标识和时戳增量通常来讲都是相同的，JRTPLIB允许将它们设置为会话的默认参数，这是通过调用 RTPSession类的SetDefaultPayloadType()、SetDefaultMark()和 SetDefaultTimeStampIncrement()方法来完成的。为RTP会话设置这些默认参数的好处是可以简化数据的发送
sess.SetDefaultPayloadType(0);
sess.SetDefaultMark(false);  
sess.SetDefaultTimeStampIncrement(160); // 我们希望每分钟发送包含20毫秒(或160个样本)的数据包
// sess.SendPacket(buffer, 5);


// 发送一个包含160字节有效载荷数据的包。
bool done = false;
while (!done)
{
    status = session.SendPacket(silencebuffer,160);
    if (status < 0)
    {
        std::cerr << RTPGetErrorString(status) << std::endl;
        exit(-1);
    }

    //
    // Inspect incoming data here
    //

    RTPTime::Wait(delay);

    RTPTime t = RTPTime::CurrentTime();
    t -= starttime;
    if (t > RTPTime(60.0))
        done = true;
}

```

```c++
int SendPacket(void *data,int len)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc)
int SendPacket(void *data,int len,unsigned short hdrextID,void *hdrextdata,int numhdrextwords)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc,unsigned short hdrextID,void *hdrextdata,int numhdrextwords);
    
// sess.SendPacket((void *)buffer,sizeof(buffer),0,false,8000);
```



`SendRawData`和`SendPacket`是JRTP库中用于发送RTP数据包的两种不同方法，它们的区别在于数据的处理方式和发送方式：

1. `SendRawData`：

   方法用于直接发送原始的RTP数据，不对数据进行封包处理。

   这意味着用户必须自行确保传入的数据符合RTP格式，包括正确设置RTP头信息、负载类型、时间戳等。JRTP库不会对数据进行任何修改或封装，而是直接将原始数据发送出去。

   这种方法适用于那些已经对RTP格式非常熟悉且需要精细控制数据内容的开发者。使用SendRawData时，开发者需要负责所有RTP协议相关的细节。

2. `SendPacket`：

   发送经过JRTP库封装的RTP数据包。这意味着JRTP库会自动处理RTP头信息、负载类型、时间戳等，并将数据封装成符合RTP格式的数据包，然后再发送出去。这种方法更适用于那些希望简化RTP数据包发送过程、不需要深入了解RTP协议细节的开发者。

   JRTP库会自动处理RTP数据包的格式，让开发者能够更专注于业务逻辑。

### 数据接收

```c++
// 数据接收
// 对于流媒体数据的接收端，首先需要调用RTPSession类的PollData()方法来接收发送过来的RTP或者 RTCP数据报。由于同一个RTP会话中允许有多个参与者（源），你既可以通过调用RTPSession类的GotoFirstSource()和 GotoNextSource()方法来遍历所有的源，也可以通过调用RTPSession类的GotoFirstSourceWithData()和 GotoNextSourceWithData()方法来遍历那些携带有数据的源。在从RTP会话中检测出有效的数据源之后，接下去就可以调用 RTPSession类的GetNextPacket()方法从中抽取RTP数据报，当接收到的RTP数据报处理完之后，一定要记得及时释放。

sess_client.Poll();   //接收发送过来的 RTP 或者RTCP 数据报

session.BeginDataAccess(); // RTPSession::EndDataAccess和RTPSession::BeginDataAccess之间可以确保后台线程不会并发修改正试图访问的数据 
if (session.GotoFirstSource()) // RTPSession::GotoFirstSource和RTPSession::GotoNextSource 遍历参与者
{
    do
    {
        RTPPacket *packet;
        // 遍历那些携带有数据的源 
        while ((packet = session.GetNextPacket()) != 0) // 使用RTPSession::GetNextPacket成员函数可以取出当前选中的参与者的数据包
        {
            std::cout << "Got packet with extended sequence number " 
                      << packet->GetExtendedSequenceNumber() 
                      << " from SSRC " << packet->GetSSRC() 
                      << std::endl;
            session.DeletePacket(packet);// 当你不再需要这个包时，它必须被删除。
        }
    } while (session.GotoNextSource());
}
session.EndDataAccess();
```

​	JRTPLIB 为 RTP 数据报定义了三种接收模式，其中每种接收模式都具体规定了哪些到达的 RTP 数据报将会被接受，而哪些到达的 RTP 数据报将会被拒绝。通过调用 RTPSession 类的 SetReceiveMode() 方法可以设置下列这些接收模式： 

- `RECEIVEMODE_ALL　　`缺省的接收模式，所有到达的 RTP 数据报都将被接受； 
- `RECEIVEMODE_IGNORESOME　　`除了某些特定的发送者之外，所有到达的 RTP 数据报都将被接受，而被拒绝的发送者列表可以通过调用 `AddToIgnoreList()、DeleteFromIgnoreList() 和 ClearIgnoreList()` 方法来进行设置； 
- `RECEIVEMODE_ACCEPTSOME　　`除了某些特定的发送者之外，所有到达的 RTP 数据报都将被拒绝，而被接受的发送者列表可以通过调用 `AddToAcceptList ()、DeleteFromAcceptList 和 ClearAcceptList () `方法来进行设置。



​		关于当前选择源的信息可以调用RTPSession类成员函数GetCurrentSourceInfo得到， 这个函数返回一个RTPSourceData实例， 此实例包含这个源所有的信息：来自这个源发送者报告，接收报告，SDES信息等。

​		包也可以直接被处理在没有遍历这个源的情况下。 方法是： 通过重写RTPSession::OnValidatedRTPPacket成员函数.



​		如果想收到自己发出的包（自发自收）需要进行如下设置：

```c++
// 把本地监听端口和发包的目的端口都设置为相同值

sessionParams.SetAcceptOwnPackets(true);

// 接收要手动收包
session.poll();
```







### 控制信息

​		JRTPLIB 是一个高度封装后的RTP库，程序员在使用它时很多时候并不用关心RTCP数据报是如何被发送和接收的，因为这些都可以由JRTPLIB自己来完成。只要 PollData()或者SendPacket()方法被成功调用，JRTPLIB就能够自动对到达的 RTCP数据报进行处理，并且还会在需要的时候发送RTCP数据报，从而能够确保整个RTP会话过程的正确性。

​		另一方面，通过调用`RTPSession`类提供的`SetLocalName()、SetLocalEMail()、 SetLocalLocation()、SetLocalPhone()、SetLocalTool()和SetLocalNote()`方法， JRTPLIB又允许程序员对RTP会话的控制信息进行设置。所有这些方法在调用时都带有两个参数，其中第一个参数是一个char型的指针，指向将要被设 置的数据；而第二个参数则是一个int型的数值，表明该数据中的前面多少个字符将会被使用。例如下面的语句可以被用来设置控制信息中的电子邮件地址：

```c++
// 控制信息
sess.SetLocalEMail("xiaowp@linuxgam.comxiaowp@linuxgam.com",19);
```

​		在RTP 会话过程中，不是所有的控制信息都需要被发送，通过调用`RTPSession`类提供的` EnableSendName()、EnableSendEMail()、EnableSendLocation()、EnableSendPhone ()、EnableSendTool()和EnableSendNote()`方法，可以为当前RTP会话选择将被发送的控制信息。



### 内存管理

​		可以通过继承`RTPMemoryManager`派生一个类来接管内存管理器。

```c++
class MyMemoryManager : public RTPMemoryManager
{
public:
    MyMemoryManager() { }
    ~MyMemoryManager() { }

    // 第二个参数表示次内存块的类型是什么，这允许你处理不同类型的数据。
    void *AllocateBuffer(size_t numbytes, int memtype)
    {
        return malloc(numbytes);
    }

    void FreeBuffer(void *p)
    {
        free(p);
    }
};

MyMemoryManager mgr;
RTPSession session(0, &mgr);
```

​		随着内存管理系统的引入，RTPSession类扩展了RTPSession::DeletePacket 和RTPSession::DeleteTransmissionInfo 成员函数。这些成员函数可以分别用来释放 RTPPacket实例 和 RTPTransmissionInfo实例。



### sample

```c++
#include <stdio.h>
#include <string.h>
#include "rtpsession.h"

// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}

int main(int argc, char** argv)
{
  RTPSession sess;
  unsigned long destip;
  int destport;
  int portbase = 6000;
  int status, index;
  char buffer[128];

  if (argc != 3) {
    printf("Usage: ./sender destip destport\\n");
    return -1;
  }

  // 获得接收端的IP地址和端口号
  destip = inet_addr(argv[1]);
  if (destip == INADDR_NONE) {
    printf("Bad IP address specified.\\n");
    return -1;
  }
  destip = ntohl(destip);
  destport = atoi(argv[2]);

  // 创建RTP会话
  status = sess.Create(portbase);
  checkerror(status);

  // 指定RTP数据接收端
  status = sess.AddDestination(destip, destport);
  checkerror(status);

  // 设置RTP会话默认参数
  sess.SetDefaultPayloadType(0);
  sess.SetDefaultMark(false);
  sess.SetDefaultTimeStampIncrement(10);

  // 发送流媒体数据
  index = 1;
  do {
    sprintf(buffer, "%d: RTP packet", index ++);
    sess.SendPacket(buffer, strlen(buffer));
    printf("Send packet !\\n");
  } while(1);

  return 0;
}
```

```c++
#include <stdio.h>
#include "rtpsession.h"
#include "rtppacket.h"

// 错误处理函数
void checkerror(int err)
{
  if (err < 0) {
    char* errstr = RTPGetErrorString(err);
    printf("Error:%s\\n", errstr);
    exit(-1);
  }
}

int main(int argc, char** argv)
{
  RTPSession sess;
  int localport;
  int status;

  if (argc != 2) {
    printf("Usage: ./sender localport\\n");
    return -1;
  }

   // 获得用户指定的端口号
  localport = atoi(argv[1]);

  // 创建RTP会话
  status = sess.Create(localport);
  checkerror(status);

  do {
    // 接受RTP数据
    status = sess.PollData();
 // 检索RTP数据源
    if (sess.GotoFirstSourceWithData()) {
      do {
        RTPPacket* packet;
        // 获取RTP数据报
        while ((packet = sess.GetNextPacket()) != NULL) {
          printf("Got packet !\\n");
          // 删除RTP数据报
          delete packet;
        }
      } while (sess.GotoNextSourceWithData());
    }
  } while(1);

  return 0;
}
```





## RTP荷载AAC

```c++
// aac.h
#pragma once
 
#include <iostream>
 
struct AdtsFrame {
    bool crcProtectionAbsent;
    uint8_t profile;
    uint8_t frequenceIdx;
    uint16_t frameLength;
 
    uint8_t* buf;
    uint32_t maxSize;
    uint32_t len;
    uint8_t* header;
    uint32_t headerLen;
    uint8_t* body;
    uint32_t bodyLen;
};
 
int GetAdtsFrame(FILE* f, AdtsFrame* aframe);
AdtsFrame* AllocAdtsFrame();
AdtsFrame* AllocAdtsFrame(uint32_t bufferSize);
void FreeAdtsFrame(AdtsFrame* aframe);
int GetFrequenceFromIndex(uint8_t idx);
```

```c++
// aac.cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "aac.h"

// 定义 ADTS 帧头的长度
#define ADTS_HEADER_LENGTH 7

// 获取 ADTS 帧
int GetAdtsFrame(FILE* f, AdtsFrame* aframe) {
    uint8_t header[ADTS_HEADER_LENGTH];
    
    // 读取 ADTS 帧头
    if (fread(header, 1, ADTS_HEADER_LENGTH, f) != ADTS_HEADER_LENGTH) {
        return -1; // 读取失败
    }

    // 解析 ADTS 帧头
    if ((header[0] != 0xFF) || ((header[1] & 0xF6) != 0xF0)) {
        return -1; // 不是 ADTS 帧
    }

    aframe->profile = (header[2] >> 6) & 0x03; // Profile
    aframe->frequenceIdx = (header[2] >> 2) & 0x0F; // Sampling Frequency Index
    aframe->frameLength = ((header[3] & 0x03) << 11) | (header[4] << 3) | ((header[5] >> 5) & 0x07); // Frame Length
    aframe->headerLen = ADTS_HEADER_LENGTH; // Header Length
    aframe->bodyLen = aframe->frameLength - ADTS_HEADER_LENGTH; // Body Length

    // 分配内存给音频数据
    aframe->body = new uint8_t[aframe->bodyLen];
    if (fread(aframe->body, 1, aframe->bodyLen, f) != aframe->bodyLen) {
        delete[] aframe->body;
        return -1; // 读取失败
    }

    return aframe->bodyLen; // 返回有效数据长度
}

// 分配 ADTS 帧结构体
AdtsFrame* AllocAdtsFrame() {
    return new AdtsFrame();
}

// 释放 ADTS 帧结构体
void FreeAdtsFrame(AdtsFrame* aframe) {
    if (aframe) {
        delete[] aframe->body; // 释放音频数据
        delete aframe; // 释放结构体
    }
}

// 从频率索引获取采样率
int GetFrequenceFromIndex(uint8_t idx) {
    static const int frequencies[] = {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350};
    if (idx < sizeof(frequencies) / sizeof(frequencies[0])) {
        return frequencies[idx];
    }
    return -1; // 无效索引
}

```

```c++
#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtplibraryversion.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtppacket.h>
#include <jrtplib3/rtperrors.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "aac/aac.h"
 
using namespace std;
using namespace jrtplib;
 
const string SSRC = "10001";
const string AAC_FILE_PATH = "movie_file/lantingxv.aac";
const int MTU_SIZE = 1500;
const int MAX_RTP_PACKET_LENGTH = 1360;
const int AAC_PAYLOAD_TYPE = 97;
const int AAC_SAMPLE_NUM_PER_FRAME = 1024;
 
static void checkerror(int rtperr) {
    if (rtperr < 0) {
        std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
        exit(-1);
    }
}
 
int main(int argc, char** argv) {
 
    FILE* faac = fopen(AAC_FILE_PATH.c_str(), "rb");
    if (faac == NULL) {
        std::cout << "打开aac文件失败" << std::endl;
        exit(-1);
    }
 
    AdtsFrame* aframe = AllocAdtsFrame();
    int size = GetAdtsFrame(faac, aframe);
    if (size <= 0) {
        exit(0);
    }
    int frequence = GetFrequenceFromIndex(aframe->frequenceIdx);
    int frameRate = frequence / AAC_SAMPLE_NUM_PER_FRAME;
    uint32_t timestampInc = frequence / frameRate;
    fseek(faac, 0, SEEK_SET);
 
    // 获取本地用于发送的端口以及对端的IP和端口
    uint16_t localport;
    std::cout << "Enter local port(even): ";
	std::cin >> localport;
 
    std::string ipstr;
	std::cout << "Enter the destination IP address: ";
	std::cin >> ipstr;
	uint32_t destip = inet_addr(ipstr.c_str());
	if (destip == INADDR_NONE) {
		std::cerr << "Bad IP address specified" << std::endl;
		return -1;
	}
    destip = ntohl(destip);
 
    uint16_t destport;
	std::cout << "Enter the destination port: ";
	std::cin >> destport;
 
    // 设置RTP属性
    RTPUDPv4TransmissionParams tranparams;
    tranparams.SetPortbase(localport);
 
    RTPSessionParams sessparams;
    sessparams.SetOwnTimestampUnit(1.0/frequence);
 
    RTPSession sess;
    int status = sess.Create(sessparams, &tranparams);
    checkerror(status);
 
    RTPIPv4Address destAddr(destip, destport);
    status = sess.AddDestination(destAddr);
	checkerror(status);
 
    sess.SetDefaultPayloadType(AAC_PAYLOAD_TYPE);
    sess.SetDefaultMark(true);
    sess.SetDefaultTimestampIncrement(timestampInc);
 
    RTPTime sendDelay(0, 1000000/frameRate);
    uint8_t sendbuf[MTU_SIZE] = { 0 };
 
    while (true) {
        if (feof(faac)) {
            fseek(faac, 0, SEEK_SET);
        }
        int size = GetAdtsFrame(faac, aframe);
        if (size == 0) {
            continue;
        } else if (size < 0) {
            exit(0);
        } else {
            std::cout << "Adts Frame, profile: " << (int) aframe->profile << ", frequenceIdx: " << (int) aframe->frequenceIdx
                      << ", frameLength: " << aframe->frameLength << ", headerLen: " << aframe->headerLen << ", bodyLen: " << aframe->bodyLen << std::endl;
 
            if (size <= MAX_RTP_PACKET_LENGTH) {
                memset(sendbuf, 0, MTU_SIZE);
                sendbuf[0] = 0x00;
                sendbuf[1] = 0x10;
                sendbuf[2] = (aframe->frameLength & 0x1FE0) >> 5;
                sendbuf[3] = (aframe->frameLength & 0x1F) << 3;
                memcpy(sendbuf+4, aframe->body, aframe->bodyLen);
                sess.SendPacket((void*) sendbuf, aframe->bodyLen+4, AAC_PAYLOAD_TYPE, true, timestampInc);
            } else {
                std::cout << "frame size too large, just ignore it" << std::endl;
            }
            RTPTime::Wait(sendDelay);
        }
    }
    FreeAdtsFrame(aframe);
    if (faac) {
        fclose(faac);
        faac = NULL;
    }
    sess.BYEDestroy(RTPTime(3, 0), 0, 0);
 
    return 0;
}
```

```shell
g++ jrtp_aac.cpp aac/aac.cpp -ljrtp -o jrtp_aac
```

  `jrtp_aac `程序启动后，设置本端使用的发送端口以及对端地址后，进程就开始发包了，我们使用 VLC 设置 sdp 信息开始接收流并播放。需要在同一机器上。

```
m=audio 10004 RTP/AVP 97
a=rtpmap:97 mpeg4-generic/44100/2
a=fmtp:97 streamtype=5; profile-level-id=15; mode=AAC-hbr; sizelength=13; indexlength=3; indexdeltalength=3;
c=IN IP4 127.0.0.1
```



## RTP荷载H264

```c++
//h264.h
#pragma once
 
#include <iostream>
 
#define H264_PAYLOAD_TYPE 96
#define H264_SAMPLE_RATE  90000
 
struct NaluHeader {
    uint8_t Type : 5;
    uint8_t NRI  : 2;
    uint8_t F    : 1;
};
 
struct FuIndicator {
    uint8_t Type : 5;
    uint8_t NRI  : 2;
    uint8_t F    : 1;
};
 
struct FuHeader {
    uint8_t Type : 5;
    uint8_t R    : 1;
    uint8_t E    : 1;
    uint8_t S    : 1;
};
 
struct Nalu {
    int startCodePrefixLen;
    uint8_t forbiddenBit;
    uint8_t nalReferenceIdc;
    uint8_t nalUintType;
    uint32_t maxSize;
    uint32_t len;
    uint8_t* buf;
    uint16_t lostPackets;
};
 
int GetAnnexbNalu(FILE* f, Nalu* nalu);
Nalu* AllocNalu();
Nalu* AllocNalu(uint32_t bufferSize);
void FreeNalu(Nalu* nalu);
```



```c++
// h264.cpp
#include "h264.h"
 
static const uint32_t DEFAULT_NALU_BUFFER_SIZE = 80960;
 
// 0x00 0x00 0x01
bool isStartCode3(const uint8_t* buf) {
    if (buf[0] == 0 && buf[1] == 0 && buf[2] == 1) {
        return true;
    }
    return false;
}
 
// 0x00 0x00 0x00 0x01
bool isStartCode4(const uint8_t* buf) {
    if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) {
        return true;
    }
    return false;
}
 
// startCode: 0x000001 / 0x00000001
int GetAnnexbNalu(FILE* f, Nalu* nalu) {
    if (f == NULL || nalu == NULL || feof(f)) {
        return -1;
    }
    int n = fread(nalu->buf, 1, 3, f);
    if (n != 3) {
        return -1;
    }
 
    int startCodePrefixLen = 0;
    if (isStartCode3(nalu->buf)) {
        startCodePrefixLen = 3;
    } else {
        n = fread(nalu->buf+3, 1, 1, f);
        if (n != 1) {
            return -1;
        }
        if (isStartCode4(nalu->buf)) {
            startCodePrefixLen = 4;
        } else {
            return 0;
        }
    }
 
    int pos = startCodePrefixLen;
    bool nextStartCodeFound = false;
    int nextStartCodePrefixLen = 0; 
    while (!nextStartCodeFound) {
        if (feof(f)) {
            break;
        }
        nalu->buf[pos] = fgetc(f);
        if (nalu->buf[pos] == 0x01 && nalu->buf[pos-1] == 0x00 && nalu->buf[pos-2] == 0x00) {
            nextStartCodeFound = true;
            if (nalu->buf[pos-3] == 0x00) {
                nextStartCodePrefixLen = 4;
            } else {
                nextStartCodePrefixLen = 3;
            }
        }
        ++pos;
    }
    pos -= nextStartCodePrefixLen;
    
    if (nextStartCodeFound) {
        if (fseek(f, -nextStartCodePrefixLen, SEEK_CUR) != 0) {
            return -1;
        }
    }
 
    nalu->startCodePrefixLen = startCodePrefixLen;
    nalu->forbiddenBit = nalu->buf[startCodePrefixLen] & 0x80;
    nalu->nalReferenceIdc = nalu->buf[startCodePrefixLen] & 0x60;
    nalu->nalUintType = nalu->buf[startCodePrefixLen] & 0x1f;
    nalu->len = pos - startCodePrefixLen;
 
    return pos;
}
 
Nalu* AllocNalu(void) {
    return AllocNalu(DEFAULT_NALU_BUFFER_SIZE);
}
 
Nalu* AllocNalu(uint32_t bufferSize) {
    Nalu* n = (Nalu*) calloc(1, sizeof(Nalu));
    if (!n) {
        return NULL;
    }
    n->maxSize = bufferSize;
    n->buf = (uint8_t*) calloc(bufferSize, sizeof(uint8_t));
    if (n->buf == NULL) {
        free(n);
        return NULL;
    }
    return n;
}
 
void FreeNalu(Nalu* nalu) {
    if (nalu) {
        if (nalu->buf) {
            free(nalu->buf);
            nalu->buf = NULL;
        }
        free(nalu);
    }
}
```



```c++
// jrtp_h264.cpp

#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtplibraryversion.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtppacket.h>
#include <jrtplib3/rtperrors.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "h264.h"
 
using namespace std;
using namespace jrtplib;
 
const string SSRC = "10000";
const string H264_FILE_PATH = "./test_640x480_15_nv12.h264";
const int FRAME_RATE = 15;
const int MTU_SIZE = 1500;
const int MAX_RTP_PACKET_LENGTH = 1360;
 
void checkerror(int rtperr) {
	if (rtperr < 0) {
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}
 
int main() {
 
    // 获取本地用于发送的端口以及对端的IP和端口
    uint16_t localport;
    std::cout << "Enter local port(even): ";
	std::cin >> localport;
 
    std::string ipstr;
	std::cout << "Enter the destination IP address: ";
	std::cin >> ipstr;
	uint32_t destip = inet_addr(ipstr.c_str());
	if (destip == INADDR_NONE) {
		std::cerr << "Bad IP address specified" << std::endl;
		return -1;
	}
    destip = ntohl(destip);
 
    uint16_t destport;
	std::cout << "Enter the destination port: ";
	std::cin >> destport;
 
    // 设置RTP属性
    RTPUDPv4TransmissionParams tranparams;
    tranparams.SetPortbase(localport);
 
    RTPSessionParams sessparams;
    sessparams.SetOwnTimestampUnit(1.0 / H264_SAMPLE_RATE);
 
    RTPSession sess;
    int status = sess.Create(sessparams, &tranparams);
    checkerror(status);
 
    RTPIPv4Address destAddr(destip, destport);
    status = sess.AddDestination(destAddr);
	checkerror(status);
 
    sess.SetDefaultPayloadType(H264_PAYLOAD_TYPE);
    sess.SetDefaultMark(false);
    sess.SetDefaultTimestampIncrement(H264_SAMPLE_RATE / FRAME_RATE);
 
    FILE* fh264 = fopen(H264_FILE_PATH.c_str(), "rb");
    if (fh264 == NULL) {
        std::cout << "打开h264文件失败" << std::endl;
        exit(-1);
    }
 
    uint8_t sendbuf[MTU_SIZE] = { 0 };
    NaluHeader* naluHeader;
    uint8_t* naluPayload;
    FuIndicator* fuIndicator;
    FuHeader* fuHeader;
 
    Nalu* nalu = AllocNalu();
    RTPTime sendDelay(0.060);
 
    while (true) {
        if (feof(fh264)) {
            fseek(fh264, 0, SEEK_SET);
        }
        bool wait = true;
        int size = GetAnnexbNalu(fh264, nalu);
        if (size == 0) {
            continue;
        } else if (size < 0) {
            exit(0);
        } else {
            if (size <= MAX_RTP_PACKET_LENGTH) {
                memset(sendbuf, 0, MTU_SIZE);
                naluHeader = (NaluHeader*) &sendbuf[0];
                naluHeader->F = nalu->forbiddenBit >> 7;
                naluHeader->NRI = nalu->nalReferenceIdc >> 5;
                naluHeader->Type = nalu->nalUintType;
 
                naluPayload = &sendbuf[1];
                memcpy(naluPayload, nalu->buf+nalu->startCodePrefixLen+1, nalu->len-1);
 
                uint32_t timestampInc = 0;
                if (nalu->nalUintType == 1 || nalu->nalUintType == 5) { // 非IDR图像的片 / IDR图像的片
                    timestampInc = H264_SAMPLE_RATE / FRAME_RATE;
                } else {
                    wait = false;
                }
                sess.SendPacket((void *) sendbuf, nalu->len, H264_PAYLOAD_TYPE, true, timestampInc);
                std::cout << "SendPacket, size: " << size << ", type: " << (int) nalu->nalUintType << std::endl;
            } else {
                int nPacket = nalu->len / MAX_RTP_PACKET_LENGTH;
                int leftLen = nalu->len % MAX_RTP_PACKET_LENGTH;
                if (leftLen > 0) {
                    nPacket += 1;
                }
                int n = 0;
                while (n < nPacket) {
                    memset(sendbuf, 0, MTU_SIZE);
                    bool isFirst = (n == 0);
                    bool isLast = (n == nPacket-1);
                    fuIndicator = (FuIndicator*) &sendbuf[0];
                    fuIndicator->F = nalu->forbiddenBit >> 7;
                    fuIndicator->NRI = nalu->nalReferenceIdc >> 5;
                    fuIndicator->Type = 28;
 
                    fuHeader = (FuHeader*) &sendbuf[1];
                    fuHeader->S = isFirst ? 1 : 0;
                    fuHeader->E = isLast ? 1 : 0;
                    fuHeader->R = 0;
                    fuHeader->Type = nalu->nalUintType;
 
                    naluPayload = &sendbuf[2];
                    
                    int sendLen = 2;    // fu_indicator, fu_header
                    uint32_t timestampInc = 0;
                    bool mark = false;
                    if (isLast) {
                        sendLen += leftLen;
                        timestampInc = H264_SAMPLE_RATE / FRAME_RATE;
                        mark = true;
                    } else {
                        sendLen += MAX_RTP_PACKET_LENGTH;
                    }
                    memcpy(naluPayload, nalu->buf + nalu->startCodePrefixLen + n * MAX_RTP_PACKET_LENGTH + 1, sendLen);
 
                    sess.SendPacket((void*) sendbuf, sendLen, H264_PAYLOAD_TYPE, mark, timestampInc);
                    ++n;
                    std::cout << "SendPacket, size: " << size << ", type: " << (int) nalu->nalUintType << std::endl;
                }
            }
        }
        if (wait) {
            RTPTime::Wait(sendDelay);
        }
    }
 
    FreeNalu(nalu);
    sess.BYEDestroy(RTPTime(3, 0), 0, 0);
 
    return 0;
}
```



​		jrtp_h264 启动后，设置本端使用的 UDP 端口以及对端地址后，进程就开始发包了，我们使用 VLC 设置 sdp 信息接收流并播放。

    m=video 12500 RTP/AVP 96
    a=rtpmap:96 H264
    a=framerate:15
    c=IN IP4 127.0.0.1


​        12500 需要对应进程设置的对端地址，127.0.0.1 需要对应进程设置的对端 IP。使用 VLC 打开该文件后就可以看到发送的 H264 码流图像了。



## nat

 rtp/rtcp传输数据的时候，需要两个端口支持。即rtp端口用于传输rtp数据，即传输的多媒体数据；rtcp端口用于传输rtcp控制协议信息。 rtp/rtcp协议默认的端口是rtcp port = rtp port + 1 。详细的说，比如A终端和B终端之间通过rtp/rtcp进行通信，

  ![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/1263be13f2eg214.jpg)

  

如上图，

​                             本地IP:PORT                            NAT映射后IP:PORT

UACA RTP的发送和接收IP:PORT ： 192.168.1.100:8000                       61.144.174.230:1597

UACA RTCP的发送和接收IP:PORT：192.168.1.100:8001                       61.144.174.230:1602

UACB RTP的发送和接收IP:PORT ： 192.168.1.10:8000                         61.144.174.12:8357

UACB RTCP的发送和接收IP:PORT：192.168.1.10:8001                        61.144.174.12:8420

  

上图可以得到一下一些信息：

   (一) 本地端口 RTCP PORT = RTP PORT + 1;但是经过NAT映射之后也有可能满足这个等式，但是并不一定有这个关系。

  （二）在NAT设备后面的终端的本地IP:PORT并不被NAT外的设置可知，也就无法通过终端的本地IP:PORT与之通信。而必须通过NAT映射之后的公网IP:PORT作为目的地址进行通信。

  如上图的终端A如果要发送RTP数据到终端B，UACA发送的目的地址只能是：61.144.174.12:8357。同理，UACB发送RTP数据给UACA，目的地址只能是： 61.144.174.230:1597 。

  （三）也许看到这里，如何得到自己的外网IP:PORT呢？如何得到对方的外网IP:PORT呢？这就是NAT IP:PORT转换和穿孔（puncture），下回分解。

 

NAT 地址转换

 如上所述，终端需要知道自己的外网IP：port，可以通过STUN、STUNT、TURN、Full Proxy等方式。这里介绍通过STUN方式实现NAT穿透。

 STUN: Simple Traversal of UDP Through NAT。即通过UDP对NAT进行穿透。

STUNT:Simple Traversal of UDP Through NATs and TCP too.可以通过TCP对NAT进行穿透。

STUN是一个标准协议，具体的协议内容网络上很多。在此不累述了。

为 了通过STUN实现NAT穿透，得到自己的公网IP:PORT，必须有一个公网STUN服务器，以及我们的客户端必须支持STUN Client功能。STUN Client 通过UDP发送一个request到STUN服务器，该请求通过NAT设备的时候会把数据报头中的本地IP:PORT换成该本地IP:PORT对应的公网 IP:PORT，STUN服务器接收到该数据包后就可以把该公网IP:PORT 发送给STUN Client。这样我们就得到了自己的公网IP:PORT。这样别的终端就可以把该公网IP:PORT最为发送UDP数据的目的地址发送UDP数据。

 

推荐一款STUN client/server 程序代码，[http://sourceforge.net/projects/stun/files/ ](http://sourceforge.net/projects/stun/files/)

这是一款开源软件。在客户端中的主要函数是下面这个：

```c++
NatType stunNatType( 
    StunAddress4& dest, //in 公网STUN服务器地址，如stun.xten.net
	bool verbose,               //in 调试时是否输出调试信息
	bool* preservePort=0,        //out if set, is return for if NAT preservers ports or not
	bool* hairpin=0 ,            //out if set, is the return for if NAT will hairpin packetsNAT设备是否支持回环
	int port=0,                // in 本地测试端口port to use for the test, 0 to choose random port
	StunAddress4* sAddr=0    // out NIC to use ，返回STUN返回的本地地址的公网IP:PORT
  );
```



​       输入StunAddress和测试端口port,得到本地IP:PORT对应的公网IP:PORT.

### 改造

​		jrtplib中对rtp rtcp端口的处理关系是：rtcp port = rtp port + 1 。这就有问题，本地端口可以按照这个等式来设置端口，但是经过NAT映射之后的公网端口是随机的，有可能并不满足上述等式。

```c++
 int portbase = 6000;            //设置本地rtp端口为6000

  transparams.SetPortbase(portbase);//默认的本地rtcp端口为6001.因为这里是本地设置，所一这样设置OK
  status = sess.Create(sessparams,&transparams);  
  checkerror(status);
  
  RTPIPv4Address addr(destip,destport); //设置目的地的rtp接收IP:PORT，公网传输的话就要设置为对方的rtp公网IP:PORT
  // AddDestination（）的内部处理是把addr.ip和addr.port+1赋给rtcp。这样如果对方在公网上，就有问题了。因为对方的rtcp port 可能不等于rtp port +1;这就导致对方收不到rtcp数据包。

  status = sess.AddDestination(addr); 
```



  通过跟踪AddDestination（）函数的实现，发现在class RTPIPv4Destination的构造函数中是这样构造一个发送目的地址的：

```c++
        RTPIPv4Destination(uint32_t ip,uint16_t rtpportbase)                    
    {
        memset(&rtpaddr,0,sizeof(struct sockaddr_in));
        memset(&rtcpaddr,0,sizeof(struct sockaddr_in));
        
        rtpaddr.sin_family = AF_INET;
        rtpaddr.sin_port = htons(rtpportbase);
        rtpaddr.sin_addr.s_addr = htonl(ip);
        

            rtcpaddr.sin_family = AF_INET;
            rtcpaddr.sin_port = htons(rtpportbase+1);//默认把rtp的端口+1赋给目的rtcp端口。
            rtcpaddr.sin_addr.s_addr = htonl(ip);

        RTPIPv4Destination::ip = ip;
    }
```

​    为了实现：可以自定义目的IP地址和目的rtp port和rtcp port。为了实现这么目标，自己动手改造下面几个函数：构造函数RTPIPv4Destination() 、RTPSession::AddDestination()，思路是在目的地址设置相关函数中增加一个rtcp ip 和port参数。

```c++
    RTPIPv4Destination(uint32_t ip,uint16_t rtpportbase,uint32_t rtcpip,uint16_t rtcpport)          
  {
    memset(&rtpaddr,0,sizeof(struct sockaddr_in));
    memset(&rtcpaddr,0,sizeof(struct sockaddr_in));
    
    rtpaddr.sin_family = AF_INET;
    rtpaddr.sin_port = htons(rtpportbase);
    rtpaddr.sin_addr.s_addr = htonl(ip);
    
    /**If rtcport has not been set separately, use the default rtcpport*/
    if ( 0 == rtcpport )
    {
      rtcpaddr.sin_family = AF_INET;
      rtcpaddr.sin_port = htons(rtpportbase+1);
      rtcpaddr.sin_addr.s_addr = htonl(ip);
    }else
    {
      rtcpaddr.sin_family = AF_INET;
      rtcpaddr.sin_port = htons(rtcpport);
      rtcpaddr.sin_addr.s_addr = htonl(ip);
    }
    
    RTPIPv4Destination::ip = ip;
  }

    int RTPSession::AddDestination(const RTPAddress &addr,const RTPIPv4Address &rtcpaddr)
{
  if (!created)
    return ERR_RTP_SESSION_NOTCREATED;
  return rtptrans->AddDestination(addr,rtcpaddr);
}
```

​    在调用RTPSession::AddDestination、定义RTPIPv4Destination的时候实参也相应增加目的rtcp参数。

​    这样改造之后就可以自定义独立的设置目的地址rtp ,rtcp端口了。



### 移植

​		把jrtplib移植到arm11平台，遇到一些问题，如下。
5.1 字节序的问题
   jrtplib中的报头的字节序问题，网上可以搜到一些，但都是只言片语，没有详细的把解决方案说出来。ARM采用的是Big-Endian, 而X86采用的是Little-Endian。目前我所采用的解决方法是让两台互相通信的主机所使用的jrtplib的Endian格式一致，都为 Big-Endian或都为Little-Endian，这只需要修改jrtplib-version/src/rtpconfig_unix.h 文件，默认是采用的Little-Endian方式，里面的注释部分有说若加
\#define RTP_BIG_ENDIAN
表示采用Big-Endian的字节方式，否则默认为Little-Endian方式。至于原因还没弄清楚。可以发邮件给作者问一下。

5.2 Can't retrieve login name的错误
  上述都没有问题了，又遇到另外的问题，在N800的客户端建立RTPSession的过程中，报了Can't retrieve login name的错误，在网上搜索后，找到一篇博客讲到嵌入式系统由于某些原因系统可能没有login name, 而在RTPSession的Create->InternalCreate->CreateCNAME方法，其中的getlogin_r, getlogin和getenv操作会因为logname为空而返回ERR_RTP_SESSION_CANTGETLOGINNAME的错误。我在 N800的机器上做了个实验，使用getlogin和getenv("LOGNAME")确实都不能得到登录名。要解决上述问题，可以对jrtplib的 源代码进行修改， 即修改RTPSession的CreateCNAME，即使getlogin_r, getlogin和getenv三个函数调用都不能得到登录名，也要设置一个登录名。



## 源码解析

[jrtplib V3.11.1 收包流程_windows下jrtp收不到包-CSDN博客](https://blog.csdn.net/heker2010/article/details/74941491)
