# WAV

---



## 参考

[【音视频 | wav】wav音频文件格式详解——包含RIFF规范、完整的各个块解析、PCM转wav代码_riff 规范-CSDN博客](https://blog.csdn.net/wkd_007/article/details/134125746)





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