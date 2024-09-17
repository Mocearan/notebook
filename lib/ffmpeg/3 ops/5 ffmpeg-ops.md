# ffmpeg-ops

---



## FFmpeg 解码 H.264 视频出现花屏和马赛克

### 发送数据包太大，超过了 FFmpeg 的默认最大值。

- 一种方法是控制播放源的发送数据大小，但这极大浪费了当前的网络带宽，非优选方案。

- 更好的做法是扩大接收端的接收缓冲区

  - 在 FFmpeg 的源码中，找到 udp.c 文件并修改 UDP_MAX_PKT_SIZE 默认值

    ![在这里插入图片描述](https://img-blog.csdnimg.cn/2020061522454561.png#pic_center)

    > 这里将 UDP_MAX_PKT_SIZE × 10，将缓冲区扩大了10倍。

### 网络情况较差时，因网络状况出现的丢包

排查方法：

1. 设全局变量：在丢包时将全局变量置为不同的值，最后在使用的地方根据全局变量的值来判断该帧是否完整，全局变量可在 FFmpeg 任意的头文件中设置（比如 avcodec.h）。
2. 修改rtpdec.c文件包含 missed %d package 的地方，这里出现丢包，需修改代码，对帧序号作标记。

​		在接收端根据RTP包的SeqNumber来判断是否丢包，如果丢包就标记一下。在mark为1或时间戳改变的时候，说明一帧结束了，此时如果标记为丢包了，就扔掉数据，没有丢包就给解码器。如果丢包的帧为I帧，则不仅丢掉当前I帧，此I帧之后的P帧也要丢掉，也就是说在下一个I帧到来之前，所有过来的包都丢掉，然后开始判断收到的RTP包是不是I帧。

判断函数：

```cpp
static bool isH264iFrame(byte[] paket)
{
	int RTPHeaderBytes = 0;

	int fragment_type = paket[RTPHeaderBytes + 0] & 0x1F;
	int nal_type = paket[RTPHeaderBytes + 1] & 0x1F;
	int start_bit = paket[RTPHeaderBytes + 1] & 0x80;

	if (((fragment_type == 28 || fragment_type == 29) && nal_type == 5 && start_bit == 128) ||
		fragment_type == 5 || fragment_type == 7 || fragment_type == 8)
	{
		return true;
	}

	return false;
}
```

### 解码出错。

排查方法：

1. 设全局变量：在解码出错时将全局变量置为不同的值，最后在使用的地方根据全局变量的值来判断该帧是否完整，全局变量可在 FFmpeg 任意的头文件中设置（比如 avcodec.h）。
2. 修改error_resilience.c文件 包含concealing %d DC, %d AC, %d MV errors in %c frame的地方。这里出现解包错误，需标记。
3. 修改h264_cavlc.c文件中包含 Invalid level prefix处 这里出错，需标记。
   修改h264_cavlc.c文件中包含dquant out of range处，出错，需标记。
   修改h264_cavlc.c文件中包含corrupted macroblock处，出错，需标记。
   修改h264_cavlc.c文件中包含negative number of zero coeffs at处，出错，需标记。
   修改h264_cavlc.c文件中包含mb_type %d in %c slice too large at %d %d处，出错，需标记。
   修改h264_cavlc.c文件中包含cbp too large处，出错，需标记。
4. 修改error_resilience.c文件中包含Cannot use previous picture in error concealment处，出错，需标记。
   修改error_resilience.c文件中包含Cannot use next picture in error concealment处，出错，需标记。
5. 修改h264.c文件中包含out of range intra chroma pred mode at处，出错，需标记。
   修改h264.c文件中包含top block unavailable for requested intra mode at处，出错，需标记。
   修改h264.c文件中包含left block unavailable for requested intra mode at处，出错，需标记。
6. 修改h264_slice.c文件中包含error while decoding MB处，出错，需标记。
7. 修改svq3.c文件中包含error while decoding MB处，出错，需标记。

当我们从网络中接收到RTP包，去了包头，拿到Payload数据之后一般就会送去解码，但是如果直接送去解码器解码，很可能会出现花屏。这个问题我很早就遇到过，当时查阅过资料，发现送给H264解码器的必须是一个NALU单元，或者是完整的一帧数据（包含H264 StartCode），也就是说我们拿到Payload数据之后，还要将分片的数据组成一个NALU或完整的一帧之后才送给解码器。

### 包乱序。

打印RTP包的SeqNumber看有没有不连续或乱序的问题，如果是用UDP传输，则RTP包容易发生乱序，需要对包按顺序进行重组再解码。



## ffmpeg将h264码流保存到文件

​		`av_read_frame()`得到的视频的AVPacket存为本地文件即可。

- MPEG2码流的时候，直接存储AVPacket即可
- H.264码流的时候，直接存储AVPacket后的文件可能是不能播放的

​		不能播放的原因和负载H264码流的封装格式有关：

- TS（MPEG2 Transport Stream），直接存储后的文件是可以播放的

- 复用格式是FLV，MP4则不行

  - 第一次存储AVPacket之前需要在前面加上H.264的SPS和PPS

    - AVCodecContext的extradata
    - 使用FFMPEG中的名为"h264_mp4toannexb"的bitstream filter 进行处理
    - 将处理后的extradata存入文件

    > ```c
    > FILE *fp=fopen("test.264","ab");
    > AVCodecContext *pCodecCtx=... 
    >     
    > unsigned char *dummy=NULL;   //输入的指针  
    > int dummy_len;  
    > AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");    
    > av_bitstream_filter_filter(bsfc, pCodecCtx, NULL, &dummy, &dummy_len, NULL, 0, 0);  
    > fwrite(pCodecCtx->extradata,pCodecCtx-->extradata_size,1,fp);  
    > av_bitstream_filter_close(bsfc);    
    > free(dummy);  
    > ```

  - 查看FFMPEG源代码我们发现，AVPacket中的数据起始处没有分隔符(0x00000001), 也不是0x65、0x67、0x68、0x41等字节，所以可以AVPacket肯定这不是标准的nalu

    - AVPacket前4个字表示的是nalu的长度，从第5个字节开始才是nalu的数据。
    - 直接将AVPacket前4个字节替换为0x00000001即可得到标准的nalu数据

    > ```c
    > char nal_start[]={0,0,0,1};  
    > fwrite(nal_start,4,1,fp);  
    > fwrite(pkt->data+4,pkt->size-4,1,fp);  
    > fclose(fp);  
    > ```









## 为什么不再需要 `av_register_all`

[ffmpeg4.x为什么不再需要调用av_register_all呢-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1910867)









## 读取rtsp 流 av_read_frame阻塞卡死

[ffmpeg 读取RTSP流 av_read_frame 阻塞 卡死 的问题 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/17225678.html)
