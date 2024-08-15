### Audio

---



## 声音的物理性质

​		声音是一种由物体振动引发的物理现象，物体的振动使其四周空气的压强产生变化，这种忽强忽弱变化以波的形式向四周传播，当被人耳所接收时，我们就听见了声音。



- 频率：音阶， 频率越高，波长越短，穿透性越差，衰减越快。

  > 人耳范围`20Hz~20KHz`，对`3kHz~4kH`z最为敏感

- 振幅：响度，能量大小的反映，常用分贝描述。

  > 在声压级较高时，听觉的频率特性会变得较为均匀。频率范围较宽的音乐，其声压以80～90dB为最佳，超过90dB将会损害人耳（105dB为人耳极限）

- 波形：音色，不同音色是因为不同介质产生的波形不同。声波的函数表示。

- 介质

  - 发声介质
  - 传播介质

  > 吸音主要是解决声音反射而产生的嘈杂感，录音棚
  >
  > 隔音主要是解决声音的透射而降低主体空间内的吵闹感，KTV

- 回声

  ​	声音在传播过程中遇到障碍物会反弹回来，再次被我们听到。

  ​	两种声音传到我们的耳朵里的时差小于80毫秒，我们就无法区分开这两种声音。

- 共鸣

  ​	声波能量引起另一物体振动，从而以同样的频率产生声音。



## 音频基本概念

- 采样：某时刻声音的采集样本

  - 样本需要经过数字住哪换才能存储为样本数据

  - 因为采样是离散的，所以虽然声音是连续变化的，但计算机中的声音表示是均匀离散的样本

- 采样频率：每秒钟采样的点的个数

- 采样精度（采样深度，位深）：存储每个样本的大小，常用的大小为8bit， 16bit，24bit

  - 一般为`16bit`，常用的也有`8bit / 24bit`
  - 位深越大，声色的还原度越高

- 通道数：单声道，双声道，四声道，5.1声道

  - 每个通道单独进行采样
  - 多个声道的采样数据一般按声道连续间隔排列
    - `123 123 123 123`

- 帧：一次编码会将若干个采样打包在一起，作为一个编码单元

  > 音视频文件播放时，为了音视频同步，程序需要根据每帧的播放时间戳进行有序播放。但是每个音频采样的时间间隔太小了，所以将一组采样打包为音频帧，匹配每帧图像的显示长度。
  >
  > ![img](https://pics4.baidu.com/feed/5882b2b7d0a20cf475b97840cd5ef43dadaf99e3.png@f_auto?token=15695ec22005ceadd75ec77b87ddf719)

  - MP3通常是1152个采样点作为一个编码单元
  - AAC通常是1024个采样点作为一个编码单元。

- 帧长：每帧播放持续的时间，每帧持续时间(秒) = 每帧采样点数 / 采样频率（HZ）。

  - 帧长也可以指压缩后每帧的数据长度。
  - 帧长 = 每帧采样数 / 采样频率 （ 48K，1152个采样点，帧长 = 1152 / 48000 = 0.024s）

  > MP3 48k，1152个采样点，每帧则为1152/48000=0.024 秒=24毫秒。

- PTS：播放时间戳

- 比特率：每秒采集的bit数，单位为：bps（Bit Per Second）

  - 间接衡量声音质量的一个标准。
  - 没有压缩的音频数据的比特率 = 采样频率 * 采样精度 * 通道数。
  - PCM数据比特率 = 采样大小 x 采样率 x 声道数

- 码率： 压缩后的音频数据的比特率。

  - 码率越大，压缩效率越低，音质越好，压缩后数据越大。 
  - 码率 = 音频文件大小/时长。
  - 常见的码率：
    - 96kbps：FM质量。
    - 128-160kbps：一般质量音频。
    - 192kbps： CD质量。
    - 256-320Kbps：高质量音频。

- 交错模式与非交错模式

  - 交错模式指数据以连续帧的方式存放。首先记录帧1的左声道样本和右声道样本，再开始帧2的记录，以此类推
  - 非交错模式指先记录一个周期内所有帧的左声道样本，再记录右声道样本



## 声音的采集

​		将物理的声音转为电模拟信号

- 声波压缩麦克风中的碳膜发生振动
- 碳膜振动接触下方电极
- 接触时长和频率域声波的振幅和频率有关
- 完成声波信号到电信号的转换
- 再经过放大电路处理，可以进行采样量化处理

### 数字音频

​		将模拟信号数字化。

#### 采样频率

​		每秒钟采样次数。在时间轴上对信号进行数字化。对模拟信号进行采样。

​		根据Nyguist采样定律，要从采样中完全恢复原始信号波形，采样频率必须至少是信号中最高频率的两倍。

​		人耳能听到的最高频率为20kHz，因此，采样频率一般为44.1kHz，以保证覆盖人耳听到的最大频率，数字化后的声音质量不会被降低。

- 22000 (22k) Hz      无线广播
- 44100 (44.1k)Hz     CD音质
- 48000 (48k) Hz       数字电视，DVD
- 96000 (96k) Hz       蓝光，高清DVD
- 192000(192k) Hz    蓝光，高清DVD

#### 采样量化

​	在幅度轴上对信号进行数字化，得要音频原始数据

- 采样大小（位深）：一个采样数据占用的bit数
  - 采样值的精确度取决于用多少位的数据来描述
    - 3位8个值
    - 8位256个值
    - 16位65536个值

  - 常用16bit

- 采样率：采样频率（一秒钟内采样次数），8k, 16k, 32k, 44.1k, 48k
- 声道数：每个声道单独采样。
  - 单声道，双声道，四声道，5.1声道，多声道

  - 交错模式：左右声道的数据间隔存储
  - 非交错模式：左右声道的数据分别连续存储


> ​		比如用16比特的二进制信号来表示声音的一个采样，而16比特（一个short）所表示的范围是[-32768,32767]，共有65536个可能取值，因此最终模拟的音频信号在幅度上也分为了65536层

### 数字音频编制

​		按照一定的格式记录采样和量化后的数字数据，比如顺序存储或压缩存储

> ​		通常所说的音频的裸数据格式就是脉冲编码调制（Pulse Code Modulation, PCM）数据。

​		编码后的二进制数据即表示将模拟信号转为数字信号。

#### PCM

​		脉冲编码调制，是纯粹的电子音频数据，不涉及封装格式。

​		PCM数据一般需要以下几个概念：

- 量化格式（sampleFormat），位深
- 采样率（sampleRate）
- 声道数（channel）

#### WAV编码

​		WAV编码是一种封装格式。有多种实现，一种是在PCM数据格式的前面加上44字节，分别用来描述PCM的采样率、声道数、数据格式等信息。

- 音质好
- 大量软件支持
- 适用于多媒体开发中的中间文件、保存音乐和音效素材

![img](https://pic4.zhimg.com/v2-5eeeaa5fbe0e4bb36ecb362f3ff3c237_b.webp?consumer=ZHI_MENG)

```c
Byte  1       2      3      4
   +------+------+------+------+
   |          chunckID         |
   +------+------+------+------+
   |         chunckSize        |	// total data
   +------+------+------+------+ 
   |           format          |    // "fmt" / "data"
   +------+------+------+------+
   |        subchunk1ID        |
   +------+------+------+------+
   |subchunk1Size| ~~~~~ ~~~~~   << 填充 
   +------+------+------+------+   
   | audioFormat | ~~~~~ ~~~~~   << 填充 
   +------+------+------+------+
   | numChannels | ~~~~~ ~~~~~   << 填充  // 声道数
   +------+------+------+------+
   |         SapmleTate  	  |  //  采样率
   +------+------+------+------+
   |  		ByteRate		 |  
   +------+------+------+------+
   |BlockAlign   | ~~~~~ ~~~~~    // 对齐字节数
   +------+------+------+------+
   | BitesPerSample| ~~~~ ~~~~      // 采样位深
   +------+------+------+------+
   | 		subChunk2ID		  |
   +------+------+------+------+
   | 		subChunk2Size     |
   +------+------+------+------+
   | data
```

```text
位置             举例（值）             描述
1 – 4	“RIFF”					Marks the file as a riff file. Characters are each 1 byte long.
5 – 8	File size (integer)		Size of the overall file – 8 bytes, in bytes (32-bit integer). Typically, you’d fill this in after creation.
9 -12	“WAVE”					File Type Header. For our purposes, it always equals “WAVE”.
13-16	“fmt “					Format chunk marker. Includes trailing null
17-20	16						Length of format data as listed above
21-22	1						Type of format (1 is PCM) – 2 byte integer
23-24	2						Number of Channels – 2 byte integer
25-28	44100					Sample Rate – 32 byte integer. Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz.
29-32	176400					(Sample Rate * BitsPerSample * Channels) / 8.
33-34	4						(BitsPerSample * Channels) / 8.1 – 8 bit mono2 – 8 bit stereo/16 bit mono4 – 16 bit stereo
35-36	16						Bits per sample
37-40	“data”					“data” chunk header. Marks the beginning of the data section.
41-44	File size (data)		Size of the data section.
```

```c
struct wavfile_header {
	char	riff_tag[4];
	int	riff_length;
	char	wave_tag[4];
	char	fmt_tag[4];
	int	fmt_length;
	short	audio_format;
	short	num_channels;
	int	sample_rate;
	int	byte_rate;
	short	block_align;
	short	bits_per_sample;
	char	data_tag[4];
	int	data_length;
};

 wavfile_init( )
{
	struct wavfile_header header;

	int samples_per_second = WAVFILE_SAMPLES_PER_SECOND;
	int bits_per_sample = 16;

	strncpy(header.riff_tag,"RIFF",4);
	strncpy(header.wave_tag,"WAVE",4);
	strncpy(header.fmt_tag,"fmt ",4);
	strncpy(header.data_tag,"data",4);

	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1;
	header.num_channels = 1;
	header.sample_rate = samples_per_second;
	header.byte_rate = samples_per_second*(bits_per_sample/8);
	header.block_align = bits_per_sample/8;
	header.bits_per_sample = bits_per_sample;
	header.data_length = 0;
}


void wavfile_close( FILE *file )
{
	int file_length = ftell(file);

	int data_length = file_length - sizeof(struct wavfile_header);

	fseek(file,sizeof(struct wavfile_header) - sizeof(int),SEEK_SET);
	fwrite(&data_length,sizeof(data_length),1,file);

	int riff_length = file_length - 8;
	fseek(file,4,SEEK_SET);
	fwrite(&riff_length,sizeof(riff_length),1,file);

	fclose(file);
}
```

```c
typedef unsigned char  uchar;  
typedef unsigned char  uint8;  
typedef unsigned short uint16;  
typedef unsigned long  uint32;  
typedef char           sint8;  
typedef short          sint16;  
typedef long           sint32;  
typedef float          fp32;  
typedef double         fp64;  
  
typedef enum BOOLEAN  
{  
    TRUE  = 1,  
    FALSE = 0  
} boolean;  

//wave文件头
typedef struct WaveHeader
{
	uint8 riff[4];             //资源交换文件标志
	uint32 size;               //从下个地址开始到文件结尾的字节数
	uint8 wave_flag[4];        //wave文件标识
	uint8 fmt[4];              //波形格式标识
	uint32 fmt_len;            //过滤字节(一般为00000010H)
	uint16 tag;                //格式种类，值为1时，表示PCM线性编码
	uint16 channels;           //通道数，单声道为1，双声道为2
	uint32 samp_freq;          //采样频率
	uint32 byte_rate;          //数据传输率 (每秒字节＝采样频率×每个样本字节数)
	uint16 block_align;        //块对齐字节数 = channles * bit_samp / 8
	uint16 bit_samp;           //bits per sample (又称量化位数)
} wave_header_t;
 
typedef struct WaveStruct
{
	FILE *fp;                  //file pointer
	wave_header_t header;      //header
	uint8 data_flag[4];        //数据标识符
	uint32 length;             //采样数据总数
	uint32 *pData;             //data
} wave_t;
wave_t wave;


/*
 * open *.wav file
 */
void WaveOpen(char *file, int raw, int mono_stereo)
{
	uchar temp = 0;
	uint8 read_bytes = 0;
	char *channel_mappings[] = {NULL,"mono","stereo"};
	uint32 total_time = 0;
	struct PlayTime        //播放时间
	{
		uint8 hour;
		uint8 minute;
		uint8 second;
	} play_time;
 
	if(NULL == (wave.fp=fopen(file, "rb")))                               /* open file */
	{
		printf("file %s open failure!\n", file);	
	}
 
	/* read heade information */
	if(4 != fread(wave.header.riff, sizeof(uint8), 4, wave.fp))           /* RIFF chunk */
	{
		printf("read riff error!\n");
		return;
	}
	if(1 != fread(&wave.header.size, sizeof(uint32), 1, wave.fp))         /* SIZE : from here to file end */
	{
		printf("read size error!\n");
		return;
	}
	if(4 != fread(wave.header.wave_flag, sizeof(uint8), 4, wave.fp))      /* wave file flag */
	{
		printf("read wave_flag error!\n");
		return;
	}
	if(4 != fread(wave.header.fmt, sizeof(uint8), 4, wave.fp))             /* fmt chunk */
	{
		printf("read fmt error!\n");
		return;
	}
	if(1 != fread(&wave.header.fmt_len, sizeof(uint32), 1, wave.fp))       /* fmt length */
	{
		printf("read fmt_len error!\n");
		return;
	}
	if(1 != fread(&wave.header.tag, sizeof(uint16), 1, wave.fp))           /* tag : PCM or not */
	{
		printf("read tag error!\n");
		return;
	}
	if(1 != fread(&wave.header.channels, sizeof(uint16), 1, wave.fp))      /* channels */
	{
		printf("read channels error!\n");
		return;
	}
	if(1 != fread(&wave.header.samp_freq, sizeof(uint32), 1, wave.fp))      /* samp_freq */
	{
		printf("read samp_freq error!\n");
		return;
	}
	if(1 != fread(&wave.header.byte_rate, sizeof(uint32), 1, wave.fp))      /* byte_rate : decode how many bytes per second */
	{                                                                       /* byte_rate = samp_freq * bit_samp */
		printf("read byte_rate error!\n");
		return;
	}
	if(1 != fread(&wave.header.block_align, sizeof(uint16), 1, wave.fp))       /* quantize bytes for per samp point */
	{
		printf("read byte_samp error!\n");
		return;
	}
	if(1 != fread(&wave.header.bit_samp, sizeof(uint16), 1, wave.fp))        /* quantize bits for per samp point */
	{                                                                        /* bit_samp = byte_samp * 8 */
		printf("read bit_samp error!\n");
		return;
	}
 
	/* jump to "data" for reading data */
	do
	{
		fread(&temp, sizeof(uchar), 1, wave.fp);
	}
	while('d' != temp);
	wave.data_flag[0] = temp;
	if(3 != fread(&wave.data_flag[1], sizeof(uint8), 3, wave.fp))                 /* data chunk */
	{
		printf("read header data error!\n");
		return;
	}
	if(1 != fread(&wave.length, sizeof(uint32), 1, wave.fp))                  /* data length */
	{
		printf("read length error!\n");
	}
 
	/* jduge data chunk flag */
	if(!StrCmp(wave.data_flag, "data", 4))
	{
		printf("error : cannot read data!\n");
		return;
	}
 
	total_time = wave.length / wave.header.byte_rate;
	play_time.hour = (uint8)(total_time / 3600);
	play_time.minute = (uint8)((total_time / 60) % 60);
	play_time.second = (uint8)(total_time % 60);
	/* printf file header information */
	printf("%s %ldHz %dbit, DataLen: %ld, Rate: %ld, Length: %2ld:%2ld:%2ld\n",
		   channel_mappings[wave.header.channels],             //声道
		   wave.header.samp_freq,                              //采样频率
		   wave.header.bit_samp,                               //每个采样点的量化位数
           wave.length,
		   wave.header.byte_rate,
		   play_time.hour,play_time.minute,play_time.second);
 
 
	//fclose(wave.fp);                                               /* close wave file */
}

/*
 * get wave data 
 */
uint32* GetWave(void)
{
	static uint32 buffer[1152] = {0};	
	uint16 n = 1152;
	uint16 p = 0;
 
	p = fread(buffer, sizeof(uint32), n, wave.fp);
	if(!p)
	{
		return 0;
	}
	else
	{
		for(; p<n; p++)
		{
			buffer[p] = 0;
		}
		return buffer;
	}
}
```

```c
//ckid：4字节 RIFF 标志，大写  

    wavHeader[0]  = 'R';  

    wavHeader[1]  = 'I';  

    wavHeader[2]  = 'F';  

    wavHeader[3]  = 'F';  

  

    //cksize：4字节文件长度，这个长度不包括"RIFF"标志(4字节)和文件长度本身所占字节(4字节),即该长度等于整个文件长度 - 8  

    wavHeader[4]  = (byte)(totalDataLen & 0xff);  

    wavHeader[5]  = (byte)((totalDataLen >> 8) & 0xff);  

    wavHeader[6]  = (byte)((totalDataLen >> 16) & 0xff);  

    wavHeader[7]  = (byte)((totalDataLen >> 24) & 0xff);  

  

    //fcc type：4字节 "WAVE" 类型块标识, 大写  

    wavHeader[8]  = 'W';  

    wavHeader[9]  = 'A';  

    wavHeader[10] = 'V';  

    wavHeader[11] = 'E';  

  

    //ckid：4字节 表示"fmt" chunk的开始,此块中包括文件内部格式信息，小写, 最后一个字符是空格  

    wavHeader[12] = 'f';  

    wavHeader[13] = 'm';  

    wavHeader[14] = 't';  

    wavHeader[15] = ' ';  

  

  //cksize：4字节，文件内部格式信息数据的大小，过滤字节（一般为00000010H）  

   wavHeader[16] = 0x10;  

    wavHeader[17] = 0;  

    wavHeader[18] = 0;  

    wavHeader[19] = 0;  

  

    //FormatTag：2字节，音频数据的编码方式，1：表示是PCM 编码  

    wavHeader[20] = 1;  

    wavHeader[21] = 0;  

  

    //Channels：2字节，声道数，单声道为1，双声道为2  

    wavHeader[22] = (byte) channels;  

    wavHeader[23] = 0;  

  

    //SamplesPerSec：4字节，采样率，如44100  

    wavHeader[24] = (byte)(sampleRate & 0xff);  

    wavHeader[25] = (byte)((sampleRate >> 8) & 0xff);  

    wavHeader[26] = (byte)((sampleRate >> 16) & 0xff);  

    wavHeader[27] = (byte)((sampleRate >> 24) & 0xff);  

  

    //BytesPerSec：4字节，音频数据传送速率, 单位是字节。其值为采样率×每次采样大小。播放软件利用此值可以估计缓冲区的大小；  

    //bytePerSecond = sampleRate * (bitsPerSample / 8) * channels  

    wavHeader[28] = (byte)(bytePerSecond & 0xff);  

    wavHeader[29] = (byte)((bytePerSecond >> 8) & 0xff);  

    wavHeader[30] = (byte)((bytePerSecond >> 16) & 0xff);  

    wavHeader[31] = (byte)((bytePerSecond >> 24) & 0xff);  

  

    //BlockAlign：2字节，每次采样的大小 = 采样精度*声道数/8(单位是字节); 这也是字节对齐的最小单位, 譬如 16bit 立体声在这里的值是 4 字节。  

    //播放软件需要一次处理多个该值大小的字节数据，以便将其值用于缓冲区的调整  

    wavHeader[32] = (byte)(bitsPerSample * channels / 8);  

    wavHeader[33] = 0;  

  

    //BitsPerSample：2字节，每个声道的采样精度; 譬如 16bit 在这里的值就是16。如果有多个声道，则每个声道的采样精度大小都一样的；  

   wavHeader[34] = (byte) bitsPerSample;  

    wavHeader[35] = 0;  

  

    //ckid：4字节，数据标志符（data），表示 "data" chunk的开始。此块中包含音频数据，小写；  

    wavHeader[36] = 'd';  

    wavHeader[37] = 'a';  

    wavHeader[38] = 't';  

    wavHeader[39] = 'a';  

  

    //cksize：音频数据的长度，4字节，audioDataLen = totalDataLen - 36 = fileLenIncludeHeader - 44  

    wavHeader[40] = (byte)(audioDataLen & 0xff);  

    wavHeader[41] = (byte)((audioDataLen >> 8) & 0xff);  

    wavHeader[42] = (byte)((audioDataLen >> 16) & 0xff);  

    wavHeader[43] = (byte)((audioDataLen >> 24) & 0xff);  
```



![image-20240224233212579](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240224233212579.png)





## 音频压缩

### 音频压缩原理

​		人耳掩蔽效应：数字音频压缩编码采取去除声音信号中冗余成分的方法来实现。所谓冗余成分指的是音频中不能被人耳感知到的信号，它们对确定声音的音色，音调等信息没有任何的帮助。冗余信号包含人耳听觉范围外的音频信号以及被掩蔽掉的音频信号等。

- 人耳所能察觉的声音信号的频率范围为20Hz～20KHz，除此之外的其它频率人耳无法察觉，都可视为冗余信号。
- 此外，根据人耳听觉的生理和心理声学现象，当一个强音信号与一个弱音信号同时存在时，弱音信号将被强音信号所掩蔽而听不见，这样弱音信号就可以视为冗余信号而不用传送。

​		这就是人耳听觉的掩蔽效应，主要表现在频谱掩蔽效应和时域掩蔽效应。

- 频域遮蔽：多个频率叠加的复合声音中，通过傅里叶变换等手段，去除掉被主频率遮蔽掉的声源信息。

  > 一个频率的声音能量小于某个阈值后，人耳就会听不到。当有另外能量较大的声音出现时， 该声音频率附近的阈值会提高很多。

- 时域遮蔽：主频率前后一定时间范围内，声音强度呈梯度下降的被主频率声源所屏蔽。

  > 当强音信号和弱音信号同时出现时，还存在时域掩蔽效应。即两者发生时间很接近的时候，也会发生掩蔽效应。
  > 时域掩蔽效应可以分成三种：前掩蔽，同时掩蔽，后掩蔽。前掩蔽是指人耳在听到强信号之前的短暂时间内，已经存在的弱信号会被掩蔽而听不到。同时掩蔽是指当强信号与弱信号同时存在时，弱信号会被强信号所掩蔽而听不到。后掩蔽是指当强信号消失后，需经过较长的一段时间才能重新听见弱信号，称为后掩蔽。这些被掩蔽的弱信号即可视为冗余信号。

### 音频压缩分类

- 有损压缩

  ​	解压后的数据不能完全复原，压缩比越小，丢失的信息越多，信号还原后的失真就会越大。

  ​	消除冗余信息：人耳不能感知的冗余信号，包括人耳听觉范围之外的音频信号以及被掩蔽掉的音频信号等。

  - 听觉范围：20-20000hz

- 无损压缩

  ​	解压后的数据可以完全复原，单纯进行数据的压缩。

  - 熵编码
    - 哈夫曼编码
    - 算术编码
    - 香农编码


```text
音频编码过程

---> 时域转频域变换-----> 量化编码 -----> 比特流格式化------> 比特流
 |-> 心理声学模型----------↑       辅助数据---↑
```





## 音频编码

​		将实际量化后的数字音频数据进行压缩存储，称为编码。

​		音频编解码器选型：OPUS、MP3、AAC、AC3和EAC3。

> [《详解音频编解码的原理、演进和应用选型等 - 知乎 (zhihu.com) 》](https://zhuanlan.zhihu.com/p/55218899)

![image-20240225192246401](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225192246401.png)

![image-20240225192432266](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225192432266.png)

### MP3

​		MP3的压缩比不错，其中一种实现为`LAME`编码。

​		使用`LAME`编码的中高码率的MP3文件，听感上非常接近源WAV文件，不同的应用场景下，应该调整合适的参数以达到最好的效果。

- 音质在128Kbit/s以上表现还不错
- 压缩比较高，大量软件和硬件都支持，兼容性好
- 使用与高比特率下对兼容性有要求的音乐欣赏



### AAC编码

​		AAC是新一代的音频有损压缩技术。是MP3的继任者。

#### ADIF格式

​		Audio DataInterchange Format，这种格式的特征是可以确定的找到这个音频数据的开始，只能从头开始理解吗，不能在音频数据流中间开始。这种格式常用在磁盘文件中。

#### ADTS格式

​		Audio Data Transport Stream，特征是每一帧都有一个同步字，所以可以再音频流的任何位置开始解码，类似于数据流格式。

> http://www.p23.nl/projects/aac-header/

- 结构

  ​	由7或9个字节组成，其中按位赋予不同的含义。 

![image-20240225194835753](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225194835753.png)

- 12bit，同步字， 所有位必须是1，`0xFFF`

  ​	通过识别`0xFFF`来识别一个ADTS头。

-  1bit，MPEG编码规范，`0 MPEG-4 / 1 MPEG-2`

- 2bit，layer，总是`0`

- 1bit，CRC保护标识，`0 CRC / 1 no CRC`

  ​	有CRC保护则在7字节基础上增加2个字节的CRC校验码，成为9个字节

- 2bit，profile，`MPEG-4`音频对象类型，即不同的AAC版本。

  - 1, AAC Main
  - 2, AAC LC
  - 5, SBR
  - 29, PS

- 4bit，MPEG-4采样率

  - 0, 96000HZ
  - 1, 88200HZ
  - 2, 64000HZ
  - 3, 48000HZ
  - 4, 44100HZ
  - 5, 32000HZ
  - 6, 24000HZ
  - 7, 22050HZ
  - 8, 16000HZ
  - 9, 12000HZ
  - 10, 11025HZ
  - 11, 8000HZ
  - 12, 7350HZ
  - 13, Reserved
  - 14, Reserved
  - 15: frequency is wirtten explictly

### Ogg编码

​		Ogg是一种非常有潜力的编码，在各种码率下都有比较优秀的表现，尤其是在中低码率场景下，音质好，且免费。

​		Ogg有着非常出色的算法，可以用更小的码率达到更好音质，128Kbit/s的Ogg比192Kbit/s甚至更高码率的MP3还要出色。但目前没有媒体服务软件支持，因此基于Ogg的数字广播还无法实现。

- 可以用比MP3更小的码率实现比MP3更好的音质，高中低码率下均有良好的表现，兼容性不够好，流媒体特性不支持。.
- 适用场合：语音聊天的音频消息场景



## 音频重采样

​		将音频三元组：位深、采样率和通道数，当前的值转换成另外一组值。

- 设备采集的音频数据与编码器要求的输入数据不一致。
- 样神奇要求的音频数据与要播放的音频数据不一致。
- 更方便运算
  - 回音消除中，将多声道变为单声道进行计算


​		是否需要重采样：

- 了解音频设备的参数
- 查看ffmpeg源码中对各平台不同编码器的实现

​		重采样的步骤：

- 创建采样上下文
- 设置参数
- 初始化重采样
- 进行重采样



