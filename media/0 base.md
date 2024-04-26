# media base

---



## Audio & Video（Image） 声与像

### Audio



#### 声音的物理性质

- 声音是波

  - 频率：音阶， 频率越高，波长越短，穿透性越差，衰减越快。

    > 人耳范围20Hz~20KHz，对3kHz~4kHz最为敏感

  - 振幅：响度，能量大小的反映，常用分贝描述。

    > 在声压级较高时，听觉的频率特性会变得较为均匀。频率范围较宽的音乐，其声压以80～90dB为最佳，超过90dB将会损害人耳（105dB为人耳极限）

  - 波形：音色，不同音色是因为不同介质产生的波形不同。

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
  
  

#### 声音的采集

​		将物理的声音转为电模拟信号

- 声波压缩麦克风中的碳膜发生振动
- 碳膜振动接触下方电极
- 接触时长和频率域声波的振幅和频率有关
- 完成声波信号到电信号的转换
- 再经过放大电路处理，可以进行采样量化处理

##### 数字音频

- 采样：在时间轴上对信号进行数字化。

  > ​		对模拟信号进行采样。
  >
  > ​		奈奎斯特定理（也称为采样定理），按比声音最高频率高2倍以上的频率对声音进行采样（也称为AD转换）,对于高质量的音频信号，其频率范围（人耳能够听到的频率范围）是20Hz～20kHz，所以采样频率一般为44.1kHz，这样就可以保证采样声音达到20kHz也能被数字化，从而使得经过数字化处理之后，人耳听到的声音质量不会被降低。

- 量化：在幅度轴上对信号进行数字化

  - 采样大小（位深）：一个采样数据占用的bit数，常用16bit。
  - 采样率：采样频率（一秒钟内采样次数），8k, 16k, 32k, 44.1k, 48k
  - 声道数：单声道、双声道、多声道。每个声道单独采样。

  > ​		比如用16比特的二进制信号来表示声音的一个采样，而16比特（一个short）所表示的范围是[-32768,32767]，共有65536个可能取值，因此最终模拟的音频信号在幅度上也分为了65536层

- 编码：按照一定的格式记录采样和量化后的数字数据，比如顺序存储或压缩存储

  > ​		通常所说的音频的裸数据格式就是脉冲编码调制（Pulse Code Modulation, PCM）数据。

  ​		编码后的二进制数据即表示将模拟信号转为数字信号。

##### 采样频率

​		每秒钟采样次数。

​		根据尼奎斯特定律，要从采样中完全恢复原始信号波形，采样频率至少是信号中最高频率的两倍。

​		人耳能听到的最高频率为20kHz，因此，采样频率一般为44.1kHz，以保证覆盖人耳听到的最大频率，数字化后的声音质量不会被降低。

- 22000 (22k) Hz      无线广播
- 44100 (44.1k)Hz     CD音质
- 48000 (48k) Hz       数字电视，DVD
- 96000 (96k) Hz       蓝光，高清DVD
- 192000(192k) Hz    蓝光，高清DVD



##### 位深

​		用以描述每个采样的值的范围。

​		采样值的精确度取决于用多少位的数据来描述，3位8个值，8位256个值，16位65536个值。



##### 通道数

​		单声道，双声道，四声道，5.1声道，多声道

- 交错模式：左右声道的数据间隔存储
- 非交错模式：左右声道的数据分别连续存储



##### 比特率、码率

- 比特率：裸数字音频，每秒传输的bit数，bps(Bit Per Second)

  ​	PCM数据比特率 = 采样大小 x 采样率 x 声道数

- 码率：压缩后的音频数据，的比特率。

  ​	码率越大，压缩效率越低，音质越好，压缩后的数据越大。

  ​	码率 = 音频文件大小 / 时长

  - 96 kbps， FM质量
  - 128-160 kbps， 一般质量音频
  - 192 kbps， CD质量
  - 256-320 kbps， 高质量音频

##### 编码帧

​		一次编码会将若干个采样打包在一起，作为一个编码单元。

- MP3，1152采样点作为一个编码单元
- AAC，通常1024个采样点作为一个编码单元，也有2048

​		帧长，指每帧播放持续的时间。 也可以指压缩后每帧的数据长度。

​		帧长 = 每帧采样数 / 采样频率 （ 48K，1152个采样点，帧长 = 1152 / 48000 = 0.024s）

​		





##### PCM

​		脉冲编码调制，是纯粹的电子音频数据，不涉及封装格式。

​		PCM数据一般需要以下几个概念：

- 量化格式（sampleFormat），位深
- 采样率（sampleRate）
- 声道数（channel）

##### WAV编码

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





#### 音频压缩

- 有损压缩

  ​	解压后的数据不能完全复原，压缩比越小，丢失的信息越多，信号还原后的失真就会越大。

  ​	消除冗余信息：人耳不能感知的冗余信号，包括人耳听觉范围之外的音频信号以及被掩蔽掉的音频信号等。

  - 听觉范围：20-20000hz

  - 人耳掩蔽效应：频域掩蔽效应和时域掩蔽效应。
    - 频域遮蔽：多个频率叠加的复合声音中，通过傅里叶变换等手段，去除掉被主频率遮蔽掉的声源信息。
    - 时域遮蔽：主频率前后一定时间范围内，声音强度呈梯度下降的被主频率声源所屏蔽。

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





#### 音频编码

​		将实际量化后的数字音频数据进行压缩存储，称为编码。

![image-20240225192246401](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225192246401.png)

![image-20240225192432266](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225192432266.png)

##### MP3

​		MP3的压缩比不错，其中一种实现为`LAME`编码。

​		使用`LAME`编码的中高码率的MP3文件，听感上非常接近源WAV文件，不同的应用场景下，应该调整合适的参数以达到最好的效果。

- 音质在128Kbit/s以上表现还不错
- 压缩比较高，大量软件和硬件都支持，兼容性好
- 使用与高比特率下对兼容性有要求的音乐欣赏



##### AAC编码

​		AAC是新一代的音频有损压缩技术。

- 开始基于MPEG-2音频编码技术
- MPEG-4标准出现后，AAC重新集成特性，加入了SBR（Spectral Band Replication， 频段重现）和PS（Parametric Stereo，参数化）技术。

​		通过你附加的编码技术（PS，SBR等），衍生出了

- LC-AAC （Low Complexity）

  ​	低复杂度规则，码流128K，音质好。主要应用于中高码率场景的编码（>80Kbit/s)

- HE-AAC

  ​	按频谱分别保存，扶贫编码保存主要部分，高频单独放大编码保存音质，码流64K左右。

  ​	AAC + SBR， 主要应用于中低码率场景下的编码（<=80Kbit/s) 

- HE-AAC v2

  ​	双声道中的生意你存在某种相似性，只需存储一个声道的全部信息，然后，花很少的字节用参数描述另一个声道和它的差异。

  ​	AAC + SBR + PS，主要应用于低码率场景的编码(<=48Kbit/s)


![image-20240225193009769](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225193009769.png)

​		大部分编码器都设置为<=48Kbit/s自动启用PS技术，而>48Kbit/s则不加PS。

- 小于128Kbit/s的码率下表现优异，并且多用于视频中的音频编码

- 适用于128Kbit/s以下的音频编码，多用于视频总的音频轨的编码



###### ADIF格式

​		Audio DataInterchange Format，这种格式的特征是可以确定的找到这个音频数据的开始，只能从头开始理解吗，不能在音频数据流中间开始。这种格式常用在磁盘文件中。

###### ADTS格式

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

##### Ogg编码

​		Ogg是一种非常有潜力的编码，在各种码率下都有比较优秀的表现，尤其是在中低码率场景下，音质好，且免费。

​		Ogg有着非常出色的算法，可以用更小的码率达到更好音质，128Kbit/s的Ogg比192Kbit/s甚至更高码率的MP3还要出色。但目前没有媒体服务软件支持，因此基于Ogg的数字广播还无法实现。

- 可以用比MP3更小的码率实现比MP3更好的音质，高中低码率下均有良好的表现，兼容性不够好，流媒体特性不支持。.
- 适用场合：语音聊天的音频消息场景



#### 音频重采样

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



### Image

#### 图像的物理性质

​		白光能被分解为三原色，红光（R）、绿光（G）、蓝光（B），即RGB。等量的三原色光叠加会变为白光。

​		电子设备显像是自发光的，不同于其他自然物体是由于反射日光。

#### 电子设备图像表示：RGB

​		像素(pixel)是一个图片的基本单位。

> pix是picture的缩写，el是element的缩写，像素即图像元素。

​		一个像素点由RGB三个通道组成，每个通道按照一定位数的值，显示不等量的对应三原色，从而构成该像素点的颜色。若干个颜色各异的像素点就组成一幅图，连续变换的一组图就形成一个视频。

​		RGB表示上，除了红绿蓝三个色彩通道，还有一个透明度通道（alpha分量， ARGB）。

- 分辨率，描述一个现象设备上的像素点数，即像素的大小或尺寸。

  > 如1920*1080，即横向1920个像素点，纵向1080个像素点。
  >
  > 分辨率越高， 像素越多，图像就越清晰。

- 位深，计算机表示数字图像时，每个像素用一定位数的数据长度来表示。

  ​	每个通道的位深越大，能够表示的颜色值就越多。

  > 每个通道上，按照一定的规则来定义对应颜色的分级。
  >
  > RGB通道的分级数值表示组成一个像素点的数据格式：
  >
  > - 浮点表示：0/0~10，OpenGL ES中每个通道点的表示使用这种方式。
  >
  > - 整数表示：RGB_8888,0~255或00~FF,8位表示一个分量通道上的值。即8位位深度。
  >
  >   > ​	一些平台上采用其他的整数表示方式，如Android采用RGB_565共16位比特来表示一个像素。
  >  >
  >   > RGB_8888表示的1280X720的图像大小为：1280X720X4=3.516MB
  >   >
  >   > ​		这也是位图（bitmap）在内存中所占用的大小，所以每一张图像的裸数据都是很大的。
  > 
  
  
  

​		RGB像素点中各个分量通常是按照顺序排列的，但有些图像处理要转换成其他的顺序，`opencv`经常转换成BGR的排列方式。

![image-20220907235455776](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220907235455776.png)

#### 图像的编码：压缩

​		由于图像裸数据很大，一般对图像进行压缩。

##### JPEG

​		静态图像压缩标准，ISO指定。

- 良好的压缩性能

- 较好的重建质量

- 图像处理领域

- 有损压缩

- 不能直接用于视频压缩

  > 视频需要考虑时域因素，不仅要考虑帧内编码，还要考虑帧间编码。（帧，视频中的一幅图像）



### video

​		视频是由一幅幅图像组成的。

#### 电视图像数字表示：YUV

​		视频帧的裸数据表示，更多的是YUV数据格式，主要应用于优化彩色视频信号的传输，使其向后兼容老式黑白电视。

​		之所以采用YUV色彩空间，是因为它的亮度信号Y和色度信号U、V是分离的。

> ​		如果只有Y信号分量而没有U、V分量，那么这样表示的图像就是黑白灰度图像。
>
> ​		彩色电视采用YUV空间正是为了用亮度信号Y解决彩色电视机与黑白电视机的兼容问题，使黑白电视机也能接收彩色电视信号。
>
> ​		同时降低色度的采样率而不会对图像质量影响太大，降低了视频信号传输时对频宽（带宽）的要求。

- Y，明亮度（Luminance或Luma），也称灰阶值

  > “亮度”是透过RGB输入信号来建立的，方法是将RGB信号的特定部分叠加到一起。

- U / V，色度（Chrominance或Chroma），它们的作用是描述影像的色彩及饱和度，用于指定像素的颜色。

  > ​	“色度”则定义了颜色的两个方面——色调与饱和度，分别用Cr和Cb来表示。
  >
  > 其中
  >
  > - Cr反映了RGB输入信号红色部分与RGB信号亮度值之间的差异
  > - Cb反映的则是RGB输入信号蓝色部分与RGB信号亮度值之间的差异。

​		最常用的表示形式是Y、U、V都使用8个bit来表示，所以取值范围就是0～255。

> ​		在广播电视系统中不传输很低和很高的数值，实际上是为了防止信号变动造成过载，因而把这“两边”的数值作为“保护带”，不论是Rec.601还是BT.709的广播电视标准中，Y的取值范围都是16～235, UV的取值范围都是16～240。

##### YUV的采样格式

​		YUV采用`A:B:C`表示法来描述Y、U、V采样频率的关系。

​		主要分为YUV 4:4:4 、YUV 4:2:2、YUV 4:2:0三种常用类型。

![image-20220908213114325](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908213114325.png)

> ​		黑点表示一个Y分量，空心圆表示像素点的一个UV分量组合。
>
> ​		YUV最常用的采样格式是4:2:0,它指的是对每行扫描线来说，只有一种色度分量是以2:1的抽样率来存储的。
>
> ​		相邻的扫描行存储着不同的色度分量，也就是说，如果某一行是4:2:0，那么其下一行就是4:0:2，再下一行是4:2:0，以此类推。
>
> ​		对于每个色度分量来说，水平方向和竖直方向的抽样率都是2:1，所以可以说色度的抽样率是4:1。
>
> > 对非压缩的8比特量化的视频来说，8×4的一张图片需要占用48字节的内存：
> >
> > ![image-20220821230236373](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821230236373.png)
> >
> > 上述描述意思是，对于一组四个Y，在U/V分量中采一个U和一个V。

##### YUV的排列格式

​		YUV是一系列相似格式的统称，针对具体的排列方式，可以分为多种格式。

- 打包（packed）格式

  ​	每个像素点的Y、U、V分量交叉排列，易像素点为单元连续存放在同一数组中。通常几个相邻的像素组成一个宏像素（macro-pixel)

  ![image-20220908211732290](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908211732290.png)

- 平面（planar）格式

  ​	将Y、U、V分量分别各自连续的存放在不同的数组中。

![image-20220908211748811](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908211748811.png)

![image-20220908221051838](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221051838.png)

##### YUV的数据存储

​		对非压缩的8比特量化的采样数据来说，每个分量占一个字节（char或byte）

- YUV 4:4:4

  ​	I444格式（YUV444P, P-planar），对于ffmpeg像素表示`AV_PIX_FMT_YUV444P`

  > planar YUV4:4:4， 24bpp(24bit per pixel)，（1Cr & Cb sample per 1X1 Y samples）
  >
  > > Y 4byte + U 4byte + V 4byte = 12byte
  > >
  > > 即4个像素占用12byte，每个像素占用3byte

  ![image-20220908214343304](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908214343304.png)

- YUV 4:2:2

  ​	I422 格式（YUV422P），对于ffmpeg像素表示`AV_PIX_FMT_YUV422P`

  > planar YUV4:2:2， 16bpp(16bit per pixel)，（1Cr & Cb sample per 2X1 Y samples）
  >
  > > Y 4byte + U 2byte + V 2byte = 8byte
  > >
  > > 即4个像素占用8byte，每个像素占用2byte

  ![image-20220908214659406](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908214659406.png)

- YUV 4:2:0

​	I420格式（YUV420P），对于ffmpeg像素表示`AV_PIX_FMT_YUV420P`

> planar YUV4:2:0， 12bpp(12bit per pixel)，（1Cr & Cb sample per 2X2 Y samples）
>
> > Y 4byte + U 1byte + V 1byte = 6byte
> >
> > 即4个像素占用6byte，每个像素占用1.5byte

![image-20220908215935314](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908215935314.png)

##### YUV和RGB的转化

​		凡是渲染到屏幕上的东西（文字、图片或者其他），都要转换为RGB的表示形式。

​		主要的转换标准是BT601和BT709.

![image-20220908221221048](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221221048.png)

![image-20220821232507499](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821232507499.png)

​	![image-20220908221533694](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221533694.png)

> 比较典型的场景：
>
> ​		iOS平台中使用摄像头采集出YUV数据之后，上传显卡成为一个纹理ID，这个时候就需要做YUV到RGB的转换（具体的细节会在后面的章节中详细讲解）。在iOS的摄像头采集出一帧数据之后（CMSampleBufferRef），我们可以在其中调用CVBufferGetAttachment来获取YCbCrMatrix，用于决定使用哪一个矩阵进行转换。
>
> ​		对于Android的摄像头，由于其是直接纹理ID的回调，所以不涉及这个问题。
>
> 其他场景下需要自行寻找对应的文档，以找出适合的转换矩阵进行转换。

​		ffmpeg提供swscale或者libyuv进行RGB和YUV的转换。

- 从YUV转到RGB，如果值小于0要取0，大于255要取255.

![image-20220908221700552](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221700552.png)

#### 视频编码

​		同音频编码相似，视频的帧间编码也是通过去除冗余信息的方式来进行数据量的压缩。

​		相较于音频数据，视频数据有极强的相关性，也就是说有大量的冗余信息，包括空间上的冗余信息和时间上的冗余信息。

![image-20220821235406691](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821235406691.png)

**帧间编码**技术可以去除时间上的冗余信息

- 运动补偿：运动补偿是通过先前的局部图像来预测、补偿当前的局部图像，它是减少帧序列冗余信息的有效方法。
- 运动表示：不同区域的图像需要使用不同的运动矢量来描述运动信息。
- 运动估计：运动估计是从视频序列中抽取运动信息的一整套技术。

**帧内编码**技术可以去除空间上的冗余信息

​		相对于静态的JPEG编码，ISO针对视频也指定了标准：Motion JPEG即MPEG。

​		MPEG算法是适用于动态视频的压缩算法，它除了对单幅图像进行编码外，还利用图像序列中的相关原则去除冗余，这样可以大大提高视频的压缩比。

> 截至目前，MPEG的版本一直在不断更新中，主要包括这样几个版本：Mpeg1（用于VCD）、Mpeg2（用于DVD）、Mpeg4 AVC（现在流媒体使用最多的就是它了）

​		ITU-T制定的H.261、H.262、H.263、H.264一系列视频编码标准是一套单独的体系。

​		H.264集中了以往标准的所有优点，并吸取了以往标准的经验，采用的是简洁设计，这使得它比Mpeg4更容易推广。现在使用最多的就是H.264标准，H.264创造了多参考帧、多块类型、整数变换、帧内预测等新的压缩技术，使用了更精细的分像素运动矢量（1/4、1/8）和新一代的环路滤波器，这使得压缩性能得到大大提高，系统也变得更加完善。



##### IPB帧

​		视频压缩中，每帧都代表着一幅静止的图像。而在进行实际压缩时，会采取各种算法以减少数据的容量，其中IPB帧就是最常见的一种。

- I帧：帧内编码帧（intra picture）

  I帧通常是每个GOP的第一个帧，经过适度地压缩，作为随机访问的参考点，可以当成静态图像。

  > GOP（MPEG所使用的一种视频压缩技术）

  I帧可以看作一个图像经过压缩后的产物，I帧压缩可以得到6:1的压缩比而不会产生任何可觉察的模糊现象。

  I帧压缩可去掉视频的空间冗余信息，P帧和B帧是为了去掉时间冗余信息。

  > I帧自身可以通过视频解压算法解压成一张单独的完整视频画面，所以I帧去掉的是视频帧在空间维度上的冗余信息。

- P帧：前向预测编码帧（predictive-frame）

  通过将图像序列中前面已编码帧的时间冗余信息充分去除来压缩传输数据量的编码图像，也称为预测帧。

  > P帧需要参考其前面的一个I帧或者P帧来解码成一张完整的视频画面。

- B帧：双向预测内插编码帧（bi-directional interpolated prediction frame）

  既考虑源图像序列前面的已编码帧，又顾及源图像序列后面的已编码帧之间的时间冗余信息，来压缩传输数据量的编码图像，也称为双向预测帧。

  > B帧则需要参考其前一个I帧或者P帧及其后面的一个P帧来生成一张完整的视频画面，所以P帧与B帧去掉的是视频帧在时间维度上的冗余信息。

##### IDR帧

​		在H264的概念中有一个帧称为IDR帧（在H264的概念中有一个帧称为IDR帧）。

​		因为H264采用了多帧预测，所以I帧之后的P帧有可能会参考I帧之前的帧，这就使得在随机访问的时候不能以找到I帧作为参考条件，因为即使找到I帧，I帧之后的帧还是有可能解析不出来，而IDR帧就是一种特殊的I帧，即这一帧之后的所有参考帧只会参考到这个IDR帧，而不会再参考前面的帧。在解码器中，一旦收到一个IDR帧，就会立即清理参考帧缓冲区，并将IDR帧作为被参考的帧。



##### PTS / DTS

​		DTS（Decoding Time Stamp），解码时间戳，主要用于视频的解码。

> 表示该压缩包应该在什么时候被解码

​		PTS（Presentation Time Stamp），显示（表示）时间戳，主要用于在解码阶段进行视频的同步和输出。

> 视频的一帧图像什么时候显示给用户，取决于它的PTS

​		在没有B帧的情况下，DTS和PTS的输出顺序是一样的。因为B帧打乱了解码和显示的顺序，所以一旦存在B帧，PTS与DTS势必就会不同。

> 如果视频里各帧的编码是按输入顺序（显示顺序）依次进行的，那么解码和显示时间应该是一致的，但是事实上，在大多数编解码标准（如H.264或HEVC）中，编码顺序和输入顺序并不一致，于是才会需要PTS和DTS这两种不同的时间戳。



##### GOP

​		GOP（Group Of Picture），两个I帧之间形成的一组图片。

​		通常在为编码器设置参数的时候，必须要设置gop_size的值，其代表的是两个I帧之间的帧数目。

> ​		一个GOP中容量最大的帧就是I帧，所以相对来讲，gop_size设置得越大，整个画面的质量就会越好。
>
> ​		但是在解码端必须从接收到的第一个I帧开始才可以正确解码出原始图像，否则会无法正确解码。

​		在提高视频质量的技巧中，还有个技巧是多使用B帧，一般来说，I的压缩率是7（与JPEG差不多）, P是20, B可以达到50，可见使用B帧能节省大量空间，节省出来的空间可以用来更多地保存I帧，这样就能在相同的码率下提供更好的画质。

> 根据不同的业务场景，适当地设置gop_size的大小，以得到更高质量的视频。

##### 帧率

​		每秒处理的图片帧数，如25fps，即每秒处理25张图片。

​		帧率越高，视频越流畅，需要的设备性能越高。

​		人眼的视觉暂留使得图像帧率达到24帧就可以认为图像是连续的。

> 电影帧率一般是24fps，电视剧是25fps，监控行业常用25fps，音视频通话常用15fps。

##### 码率

​		单位时间的视频数据大小，如1Mbps，即每秒有1Mb的数据量。

​		码率越大，视频越清晰。但不是线性的关系，而是对数的关系。



##### Stride

​		内存中每行像素所占的空间，为了实现内存对齐，每行像素在内存中所占的空间并不一定是图像的宽度。

![image-20220908221919639](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221919639.png)

![image-20220908222150926](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908222150926.png)

![image-20220908222203828](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908222203828.png)



## 音视频封装

​		封装格式（容器），将已经编码压缩好的图片流、音频流、字幕信息按照一定的方案组织在一个文件中，便于软件播放。

​		一般来说，视频文件的后缀就是它的封装格式：

```
AVI / MKV / MPE / MPG / MPEG 
MP4 / WMV / MOV / 3GP
M2V / M1V / M4V / OGM
RM / RMS / RMM / RMVB / IFO
SWF / FLV / F4V
ASF / PMF / XMB / DIVX / PART
DAT / VOB / M2TS / TS / PS
```

​		H264 + AAC封装为FLV或MP4是最为流行的封装模式。



## 音视频同步

### 时间戳

- DTS（Decoding Time Stamp） 解码时间戳

  ​	播放器根据DTS解码一帧数据

- PTS（Presentation Time Stamp）显示时间戳

  ​	播放器根据PTS展示一帧数据

### 同步方式

- Audio Master：同步到音频
- Video Master： 同步音频到视频
- External Clock Master：同步音频和视频到外部时钟

​		一般情况下 Audio Master > External  Clock Master > Video Master

## 流媒体基本原理

### 录制

![image-20220816211216411](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220816211216411.png)

### 播放

![image-20220816211236450](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220816211236450.png)



















