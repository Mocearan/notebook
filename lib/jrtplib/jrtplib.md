# jrtplib

---

​		`JRTPLIB`是一个面向对象的RTP库，它完全遵循RFC 1889设计，在很多场合下是一个非常不错的选择，下面就以JRTPLIB为例，讲述如何在Linux平台上运用RTP协议进行实时流媒体编程。

```c++
// 初始化
// 在使用JRTPLIB进行实时流媒体数据传输之前，首先应该生成RTPSession类的一个实例来表示此次RTP会话，然后调用Create() 方法来对其进行初始化操作。
// RTPSession类的Create()方法只有一个参数，用来指明此次RTP会话所采用的端口号。

#include "rtpsession.h"

RTPSession sess; 
sess.Create(5000);

//3.11版jrtplib库的Create方法被修改为Create(sessparams,&transparams)
RTPUDPv4TransmissionParams transparams;
RTPSessionParams sessparams;
sessparams.SetOwnTimestampUnit(1.0/8000.0);/*设置时间戳，1/8000表示1秒钟采样8000次，即录音时的8KHz*/
sessparams.SetAcceptOwnPackets(true);
transparams.SetPortbase(portbase);/*本地通讯端口*/
```



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
sess.SetDefaultTimeStampIncrement(10);
// sess.SendPacket(buffer, 5);

sess.SendPacket((void *)buffer,sizeof(buffer),0,false,8000);
```

```c++
int SendPacket(void *data,int len)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc)
int SendPacket(void *data,int len,unsigned short hdrextID,void *hdrextdata,int numhdrextwords)
int SendPacket(void *data,int len,unsigned char pt,bool mark,unsigned long timestampinc,unsigned short hdrextID,void *hdrextdata,int numhdrextwords)
```



```c++
// 数据接收
// 对于流媒体数据的接收端，首先需要调用RTPSession类的PollData()方法来接收发送过来的RTP或者 RTCP数据报。由于同一个RTP会话中允许有多个参与者（源），你既可以通过调用RTPSession类的GotoFirstSource()和 GotoNextSource()方法来遍历所有的源，也可以通过调用RTPSession类的GotoFirstSourceWithData()和 GotoNextSourceWithData()方法来遍历那些携带有数据的源。在从RTP会话中检测出有效的数据源之后，接下去就可以调用 RTPSession类的GetNextPacket()方法从中抽取RTP数据报，当接收到的RTP数据报处理完之后，一定要记得及时释放。
JRTPLIB为RTP数据报定义了三种接收模式，其中每种接收模式都具体规定了哪些到达的RTP数据报将会被接受，而哪些到达的RTP数据报将会被拒绝。通过调用RTPSession类的SetReceiveMode()方法可以设置下列这些接收模式：

 // RECEIVEMODE_ALL　　
 //		缺省的接收模式，所有到达的RTP数据报都将被接受； 
 //	RECEIVEMODE_IGNORESOME 　　
 //		除了某些特定的发送者之外，所有到达的RTP数据报都将被接受，而被拒绝的发送者列表可以通过调用AddToIgnoreList()、 DeleteFromIgnoreList()和ClearIgnoreList()方法来进行设置； 
//	RECEIVEMODE_ACCEPTSOME 　　
//		除了某些特定的发送者之外，所有到达的RTP数据报都将被拒绝，而被接受的发送者列表可以通过调用AddToAcceptList ()、DeleteFromAcceptList和ClearAcceptList ()方法来进行设置。 

sess_client.Poll();   //接收发送过来的 RTP 或者RTCP 数据报
sess_client.BeginDataAccess();

if (sess.GotoFirstSourceWithData()) {     //遍历那些携带有数据的源
     do {   
          sess.AddToAcceptList(remoteIP, allports,portbase);
           sess.SetReceiveMode(RECEIVEMODE_ACCEPTSOME);

           RTPPacket *pack;         
          pack = sess.GetNextPacket();            // 处理接收到的数据    
           delete pack;   }
     while (sess.GotoNextSourceWithData());
 }

sess_client.EndDataAccess();
```



```c++
// 控制信息
// RTPLIB 是一个高度封装后的RTP库，程序员在使用它时很多时候并不用关心RTCP数据报是如何被发送和接收的，因为这些都可以由JRTPLIB自己来完成。只要 PollData()或者SendPacket()方法被成功调用，JRTPLIB就能够自动对到达的 RTCP数据报进行处理，并且还会在需要的时候发送RTCP数据报，从而能够确保整个RTP会话过程的正确性。

// 而另一方面，通过调用RTPSession类提供的SetLocalName()、SetLocalEMail()、 SetLocalLocation()、SetLocalPhone()、SetLocalTool()和SetLocalNote()方法， JRTPLIB又允许程序员对RTP会话的控制信息进行设置。所有这些方法在调用时都带有两个参数，其中第一个参数是一个char型的指针，指向将要被设 置的数据；而第二个参数则是一个int型的数值，表明该数据中的前面多少个字符将会被使用。例如下面的语句可以被用来设置控制信息中的电子邮件地址：
sess.SetLocalEMail("xiaowp@linuxgam.comxiaowp@linuxgam.com",19);

// 在RTP 会话过程中，不是所有的控制信息都需要被发送，通过调用RTPSession类提供的 EnableSendName()、EnableSendEMail()、EnableSendLocation()、EnableSendPhone ()、EnableSendTool()和EnableSendNote()方法，可以为当前RTP会话选择将被发送的控制信息。
```

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



### nat

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

#### 改造

jrtplib中对rtp rtcp端口的处理关系是：rtcp port = rtp port + 1 。这就有问题，本地端口可以按照这个等式来设置端口，但是经过NAT映射之后的公网端口是随机的，有可能并不满足上述等式。

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



#### 移植

 把jrtplib移植到arm11平台，遇到一些问题，如下。
5.1 字节序的问题
   jrtplib中的报头的字节序问题，网上可以搜到一些，但都是只言片语，没有详细的把解决方案说出来。ARM采用的是Big-Endian, 而X86采用的是Little-Endian。目前我所采用的解决方法是让两台互相通信的主机所使用的jrtplib的Endian格式一致，都为 Big-Endian或都为Little-Endian，这只需要修改jrtplib-version/src/rtpconfig_unix.h 文件，默认是采用的Little-Endian方式，里面的注释部分有说若加
\#define RTP_BIG_ENDIAN
表示采用Big-Endian的字节方式，否则默认为Little-Endian方式。至于原因还没弄清楚。可以发邮件给作者问一下。

5.2 Can't retrieve login name的错误
  上述都没有问题了，又遇到另外的问题，在N800的客户端建立RTPSession的过程中，报了Can't retrieve login name的错误，在网上搜索后，找到一篇博客讲到嵌入式系统由于某些原因系统可能没有login name, 而在RTPSession的Create->InternalCreate->CreateCNAME方法，其中的getlogin_r, getlogin和getenv操作会因为logname为空而返回ERR_RTP_SESSION_CANTGETLOGINNAME的错误。我在 N800的机器上做了个实验，使用getlogin和getenv("LOGNAME")确实都不能得到登录名。要解决上述问题，可以对jrtplib的 源代码进行修改， 即修改RTPSession的CreateCNAME，即使getlogin_r, getlogin和getenv三个函数调用都不能得到登录名，也要设置一个登录名。

