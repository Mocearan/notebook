

# ffmpeg-lib

---

- `libavformat`：封装格式的生成和解析。
  - 封装了Protocol层和Demuxer、Muxer层，使得协议和格式对于开发者来说是透明的。

- `libavcodec`：声音/图像编解码；
  - 有一些Codec是具备自己的License的，`FFmpeg`不会默认添加像`libx264`、`FDK-AAC`等库的
  - 但是FFmpeg就像一个平台一样
    - 可以在编译时自行决定将其他的第三方的`Codec`以插件的方式添加进来
    - 然后为开发者提供统一的接口

- `libavutil`：公共的工具函数；
- `libswscale`：视频场景比例缩放、 色彩映射转换；
  - 图像进行格式转换的模块(yuv->rgb)
  - 缩放（1280x730 -> 8xx x480）

- `libpostproc`：用于后期效果处理
  - 使用AVFilter的时候需要打开该模块的开关，因为Filter中会使用到该模块的一些基础函数。

- `libavdevice`：多媒体输入输出设备交互
  - 需要编译出播放声音或者视频的工具ffplay，就需要确保该模块是打开的
  - 同时也需要SDL的预先编译，因为该设备模块播放声音与播放视频使用的都是SDL库。

- `libswresample`：高度优化的音频重采样、`rematrixing `和样本格式转换操作
  - 可以对数字音频进行声道数、数据格式、采样率等多种基本信息的转换

- `libavfilter`：过滤器（滤镜），修改多媒体文件内容
  - 提供了包括音频特效和视频特效的处理



- 容器文件、容器格式：特定封装格式的多媒体文件/流，如mp4/flv/mkv/ts/ps
- 媒体流：一段连续的多媒体数据，一段视频、音频、字幕等。可以是编码的或非编码的
  - 有些音频码流是纯PCM的
- 数据帧/数据包：
  - 解码前用于传输或存储的一个数据单元成为数据包
  - 数据包解码后得到一或多个数据帧，数据帧是编码器最小处理单元

> `av_register_all()`注册所有组件，在ffmpeg 4.0中弃用。
>
> ` avdevice_register_all()`对设备进行注册，比如`V4L2`等
>
> `avformat_network_init()`;初始化网络库以及网络加密协议相关的库（比如`openssl`）



## 重要结构

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20130914204051125)

### 协议层

​		主要存储视音频使用的协议的类型以及状态。

- `AVIOContext`
- `URLContext`
- `URLProtocol ` 


#### `AVIOContext`

​		**输出文件上下文**，它可用于向文件中写入数据。，许多模块都是使用AVIOContext来实现数据输入输出的操作。

- `buffer`：缓存指针，指向内存空间。
- `buffer_size`：缓存大小，单位为字节。
- `buf_ptr`：当前指针，表示缓存中数据当前的位置。
- `eof_reached`：标志位，表示当前缓存是否读取完毕，1表示读取完毕，0表示未读取完毕。
- `read_packet`：读取数据包的回调函数指针。
- `write_packet`：写入数据包的回调函数指针。
- `seek`：流定位函数的回调函数指针。
- `opaque`：不透明指针，用于在回调函数中传递参数。



#### `URLProtocol`

- 每种协议都对应一个 `URLProtocol `结构
- FFMPEG 中文件也被当做一种存取协议



### 封装格式层

- `AVFormatContext`
- `AVInputFormat/ AVOutputFormat`

> `AVFormatContext `API调用 `AVInputFormat `，主要是FFMPEG内部调用

####  `AVFormatContext`

​		`AVFormatContext `是一个贯穿全局的数据结构。主要存储视音频封装格式中包含的信息。

​		解码和编码过程中，使用`AVFormatContext`结构体读取或写入不同类型的音视频文件。

- 读取时，调用`avformat_open_input`函数获取`AVFormatContext`结构体，通过`av_read_frame`函数来读取每个音视频流的`AVPacket`
- 写入时，调用`avformat_alloc_output_context2`函数来初始化`AVFormatContext`结构体，使用`avformat_new_stream`函数来创建音视频流，并通过`av_write_frame`函数来写入每个音视频流的`AVPacket`

​		成员：

- `AVInputFormat* iformat`：输入格式，如果是读取文件时会指定该参数。
- `AVOutputFormat* oformat`：输出格式，如果是写入文件时会指定该参数。
- `void* priv_data`：私有数据，FFmpeg模块内部使用。
- `AVIOContext* pb`：IO上下文，用于读取或写入文件。
- `int nb_streams`：流的数量。
- `AVStream** streams`：音视频流数组，包含了所有的音视频流。
- `char* filename`：文件名。
- `int64_t start_time`：文件的开始时间，单位为微秒。
- `int64_t duration`：文件的持续时间，单位为微秒。
- `int bit_rate`：总比特率。
- `AVDictionary* metadata`：元数据，包含了文件的各种信息。
- `int n_programs`：程序数量。
- `AVProgram** programs`：程序数组，用于单独处理一组流。



#### `AVInputFormat / AVOutputFormat`

- `AVInputFormat `  demuxer 
- `AVOutputFormat  ` muxer

​		同一时间只能存在一个，每种封装格式（例如FLV, MKV, MP4, AVI）对应一个该结构体。



```c
typedef struct AVOutputFormat {
    const char *name;        /* 格式名称 */
    const char *long_name;   /* 格式全名 */
    const char *mime_type;   /* mime类型 */
    const char *extensions;  /* 文件扩展名 */

    int flags;               /* 格式标志 */
    const AVCodecTag * const *codec_tag; /* 音视频编码器标记 */

    const struct AVOutputFormat *next;
    /* ... */
} AVOutputFormat;

```

- `name`：格式名称，用于标识某种输出格式的简写名称，如“mp4”、“mpeg”等。

- `long_name`：格式全名，用于显示某种输出格式的全称，如“MPEG-4 Part 14”。

- `mime_type`：输出格式的mime类型，用于表示该格式的媒体类型，如“video/mp4”。

- `extensions`：该格式支持的文件扩展名，以空格分隔，如“mp4 m4v”。

- `flags`：格式标志，用于指示特定格式的属性，如“AVFMT_NOFILE”表示该格式不需要底层文件句柄。

- `codec_tag`：音视频编码器标记，用于指定音视频编解码器的标识。该标记包含了四个字符的编码器ID和一个与之对应的压缩编码器

  ID。

- `next`：指向下一个AVOutputFormat结构体的指针，用于构建输出格式链表



```c
AVOutputFormat ff_mp4_muxer = {
    .name              = "mp4",
    .long_name         = NULL_IF_CONFIG_SMALL("MP4 (MPEG-4 Part 14)"),
    .mime_type         = "video/mp4",
    .extensions        = "mp4",

    .video_codec       = AV_CODEC_ID_MPEG4,
    .audio_codec       = AV_CODEC_ID_AAC,

    /* ... */

    .codec_tag         = (const struct AVCodecTag *const []) {
        ff_mp4_obj_type, 0
    },

    /* ... */
};
```

> ​		在进行音视频编码时，需要将输入流的编码格式转换为输出流指定的编码格式，这通常需要使用特定的编解码器。但是，不同的编解码器对应的标签（codec_tag）是不一样的，这会导致输出流中的编码格式被错误地识别。为了解决这个问题，我们可以将输出流的编码格式的标签设置为 0，让 FFmpeg 自动识别输出流的编码格式，从而避免错误。
>
> ​		将标签设置为 0 并不适用于所有的输出流。有些编解码器要求必须指定正确的标签才能正常工作。此外，在设置标签的过程中，还需要进行相应的判断和错误处理，以确保编码器正常工作。
>
> ```c
> out_stream->codecpar->codec_tag = 0 
> ```
>
> 



### 编解码层

- `AVStream`
- `AVCodecContext`
- `AVCodec`

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/acec70e8ccf982d2c7ef7b36aac8902d.png)

#### `AVStream`

​		`AVStream`是一个贯穿全局的数据结构。来描述一个音视频流的各种属性和信息.

- 一个多媒体文件通常包含多个音频流和视频流
- 每个 `AVStream `存储一个视频/音频流的相关数据
- 每个`AVStream `对应一个 `AVCodecContext`，存储该视频/音频流使用解码方式的相关数据

​		成员：

- `index`：流的索引，表示在多媒体文件中该流的位置。

- `codecpar`：该流的编解码器参数，通过AVCodecParameters结构体包含了编解码器的具体信息。

- `time_base`：时间基准，表示单位时间内的时间戳数量。

- `r_frame_rate`：帧率，表示每秒钟的帧数。

- `id`：流的ID，在多媒体文件中唯一。

- `metadata`：流的元数据，包含了流的各种相关信息。

- `nb_frames`：流中帧的数量。

- ``start_time``：流的开始时间。

- `duration`：流的持续时间。

- `disposition`：流的位置信息，如前置流、后置流等。

- `avg_frame_rate`：平均帧率。

- `recommended_encoder_configuration`：推荐的编码器配置信息。

- `codec_info_duration`：编解码器信息的持续时间。




#### `AVCodec`

​		每种解码器都对应一个 `AVCodec `结构。每个媒体格式（如MP4、FLV等）都对应着一个或多个编解码器。

- `name`：编解码器名称，以字符串形式保存。
- `type`：编解码器类型，包括音频编码器、视频编码器、音频解码器和视频解码器等。
- `id`：编解码器的唯一标识符，以AVCodecID枚举类型表示。
- `capabilities`：编解码器的能力标志，以AV_CODEC_CAP_*常量表示，用于标识编解码器支持的特性和功能。
- `supported_framerates`：支持的帧率列表。
- `pix_fmts`：支持的像素格式列表。
- `supported_samplerates`：支持的采样率列表。
- `sample_fmts`：支持的采样格式列表。
- `channel_layouts`：支持的声道布局列表。
- `profiles`：支持的编解码器配置文件列表。
- `priv_class`：编解码器私有数据的类结构体。
- `init_static_data`：初始化静态数据的函数指针。
- `init`：初始化编解码器的函数指针。
- `encode2`：编码数据的函数指针。
- `decode`：解码数据的函数指针。
- `flush`：刷新编解码器缓存的函数指针。
- `close`：关闭编解码器的函数指针。

​		使用AVCodec结构体时，根据编码和解码的不同，根据编解码器的支持情况，选择合适的像素合适、采样率、采样格式和升到布局等参数，得到更好的编解码效果。



#### `AVCodecContext`

​		**解码器上下文信息的结构体**，每个 `AVCodecContext `中对应一个 `AVCodec`，包含了大量的编解码器参数和状态信息，如编解码器类型、帧率、分辨率、编解码器参数、编码质量、音频采样率、通道数、码率等。

- av_class：用于访问AVCodecContext结构体的类结构体。
- codec_id：编解码器ID，即AVCodecID枚举类型的值。
- codec_type：编解码器类型，即AVMediaType枚举类型的值。
- bit_rate：比特率，以比特每秒（bps）为单位。
- sample_rate：采样率，以赫兹（Hz）为单位。
- channels：声道数。
- channel_layout：声道布局，可通过FFmpeg提供的API获取。
- time_base：时间基准，指定每个刻度代表的时间长度。
- ticks_per_frame：每帧所需的刻度数。
- width：视频帧宽度。
- height：视频帧高度。
- pix_fmt：像素格式，即AVPixelFormat枚举类型的值。
- sample_fmt：采样格式，即AVSampleFormat枚举类型的值。
- delay：编解码器输出的时延。
- codec：指向AVCodec的指针，表示使用的编解码器。
- codec_tag：编解码器标签。
- priv_data：编解码器私有数据。
- gop_size: 表示视频帧组（Group of Pictures，简称 GOP）的大小。
  - 当需要传输视频流时，较小的 gop_size 可以降低网络延迟和带宽消耗
  - 而当需要在本地进行视频剪辑和编辑时，较大的 gop_size 可以提高视频质量和编辑效率
- flags：编解码器状态标志。
- extradata：编解码器额外数据。
- extradata_size：额外数据的长度。
- qmin、qmax、qcompress、qblur、global_quality：视频编码参数，如最小/最大量化因子，压缩因子等等。
- strict_std_compliance：是否遵循标准的标志，即表示是否忽略一些非标准格式等非关键错误。
- level、profile：视频编码级别和配置文件。
- keyframe_interval：关键帧间隔。
- pkt_timebase：数据包时间基准。



#### `AVCodecParameters`

​		用于**描述音视频编解码器的相关参数**，包括编解码格式、宽高、采样率等等。

​		描述了编解码器的各种参数和属性，它们对于解码和编码过程非常重要。

- 解码过程中，使用`AVCodecParameters`结构体中的信息对解码器进行初始化，并从`AVPacket`中解码出`AVFrame`
- 编码过程中，使用`AVCodecParameters`结构体中的信息对编码器进行初始化，并将`AVFrame`编码成`AVPacket`

> ​		`AVCodecParameters`结构体在`FFmpeg`版本`3.0`及以后被引入。以前的版本中用的是`AVCodecContext`结构体，但是该结构体包含了更多与编解码器本身有关的信息。
>
> ​		`AVCodecParameters`被设计用于存储容器格式相关的编解码器信息，而将更多的编解码器相关信息存储在`AVCodecContext`中。
>
> - `AVCodecContext`结构体是一个重量级的结构体，AVCodecContext结构体是AVCodecParameters结构体的一个**超集**
>   - 包含了AVCodecParameters结构体所有信息
>   - 同时还包含了一些`AVCodecParameters`中没有的参数，比如一些与编解码器相关的控制信息
> - `AVCodecContext`结构体通常用于描述音视频的编解码器，可用于解码和编码等操作
>   - 在一些与编解码器有关的操作中，需要使用`AVCodecContext`
>   - 在**仅仅获取音视频流的参数信息时**，我们可以使用`AVCodecParameters`
> - 进行解码时，通常先从`AVCodecParameters`结构体中创建一个`AVCodecContext`结构体，再使用该结构体进行解码

```c

```

- enum AVCodecID codec_id：编解码器ID，表示采用的编解码器类型。
- `enum AVMediaType codec_type`：编解码器类型，包括视频、音频、字幕等。
- `int width`：视频宽度。
- `int height`：视频高度。
- `AVRational sample_aspect_ratio`：采样率。
- `int bits_per_raw_sample`：每个采样的比特数。
- `int format`：像素格式。
- `AVRational time_base`：时间基准，表示单位时间内的时间戳数量。
- `int channels`：音频通道数。
- `int sample_rate`：音频采样率。
- `uint64_t channel_layout`：音频通道布局。
- `int64_t bit_rate`：比特率。
- `AVDictionary* extradata`：额外数据，包含了关于编解码器和流的相关信息。



### 数据层

- `AVPacket`：压缩编码数据
- `AVFrame`：像素或音频采样数据



#### `AVPacket`

​		存储编码后的音视频数据包，FFmpeg中传递数据的基本单元。

​		AVPacket 可能包含多帧数据，因此它不仅表示单个数据帧，同时也可以表示多个数据帧组成的数据包。多帧数据可以通过 `AVPacket` 中的 `size` 和 `data` 字段进行访问，而每个数据帧的时间戳和时长则可以使用 `pts`、`dts` 和 `duration` 字段进行访问。

​		`AVPacket`作为音视频编解码过程中的中转数据格式，它在各个步骤中都发挥着重要的作用。

- 在解封装时，`AVPacket`用于存储从容器中读取的音视频压缩数据
- 在编码时，`AVPacket`用于存储编码后的压缩数据
- 而在编解码操作之间，`AVPacket`又作为数据传输的载体，通过FFmpeg的数据传输接口进行传递



```c
typedef struct AVPacket {
    int64_t pts;            // 表示该帧的显示时间戳
    int64_t dts;            // 表示该帧的解码时间戳
    uint8_t *data;          // 数据指针，指向存储音视频帧数据的缓冲区
    int size;               // 音视频帧数据的大小
    int stream_index;       // 音视频流的索引，表示该帧数据属于哪个流
    int flags;              // 标志位，用于记录该帧数据的一些状态。
    struct AVPacket *next;  // 下一帧数据指针
    int64_t pos;            // 数据在输入文件中的位置
    int64_t duration;       // 帧时长
} AVPacket;
```

- `pts `和 `dts：pts `表示显示时间戳，用于表示音视频帧在整个数据流中的播放时间；dts 表示解码时间戳，用于表示音视频帧在整个数据流中的解码时间。
- `data `和 `size`：data 是指向音视频帧数据的指针，size 表示数据长度。
- `stream_index`：表示当前音视频帧所属的流索引。
- `flags`：表示当前帧的一些特殊属性，例如是否为关键帧、是否为有效帧等。
- `pos`：表示音视频帧在输入文件中的位置。
- `duration`：表示帧时长，仅在某些情况下有意义，例如针对视频帧的场景。



#### `AVFrame`

​		用于存储编码前、解码后的音视频帧数据。它包含了一些与帧相关的信息，如帧的时间戳、持续时间、采样率、声道数、像素格式等等。

​		`av_frame `可以看做是一个音视频帧数据的容器，它仅存储数据，而不存储与数据相关的其他信息。这些信息通常存储在与其对应的 `AVPacket `中，或者在其他地方进行存储。

```c
typedef struct AVFrame {
    uint8_t *data[AV_NUM_DATA_POINTERS];
    int linesize[AV_NUM_DATA_POINTERS];
    uint8_t *buf[AV_NUM_DATA_POINTERS];
    int64_t pts;
    int64_t pkt_dts;
    int64_t best_effort_timestamp;
    int64_t pkt_pos;
    int64_t pkt_duration;
    AVRational sample_aspect_ratio;
    int format;
    int width, height;
    int channels;
    int channel_layout;
    enum AVColorRange color_range;
    enum AVColorSpace color_space;
    enum AVColorPrimaries color_primaries;
    enum AVColorTransferCharacteristic color_trc;
    int64_t opaque;
    struct AVDictionary *metadata;
    int flags;
    int64_t pkt_size;
    void *hw_frames_ctx;
    AVBufferRef *bufs[AV_NUM_DATA_POINTERS];
} AVFrame;
```

- `data`：是一个`uint8_t* `类型的**指针数组**，即缓冲区数据，用于存储音视频帧的数据。

  - 一个音视频帧可能包含多组数据，每个指针指向一个数据缓冲区

    > 如YUV420P包含 YUV三个分量的数据，P代表planar平面存储，每个分量要单独存储在一个缓冲区中
    >
    > Y：data[0] 、 U：data[1]、V：data[2]
    >
    > 而音频帧 data[0]指向PCM数据块

  - 对于视频帧，data数组中存储的是一帧图像的像素数据；对于音频帧，data数组中存储的则是一帧采样数据。

  - 使用AVFrame结构体时，先使用`av_frame_alloc`函数初始化一个`AVFrame`结构体，然后使用`av_frame_get_buffer`函数为`AVFrame`分配数据缓冲区。

    - **这一步分配缓冲区的过程会初始化data数组中的指针，每个指针将指向一块数据缓冲区。**

    - ```c
      #include <stdio.h>
      #include <libavcodec/avcodec.h>
      
      int main() {
        // 初始化AVFrame结构体
        AVFrame *frame = av_frame_alloc();
        frame->width = 1920;
        frame->height = 1080;
        frame->format = AV_PIX_FMT_YUV420P;
      
        // 分配缓冲区
        int ret = av_frame_get_buffer(frame, 32);
        if (ret < 0) {
          printf("Failed to allocate buffer for AVFrame.\n");
          return 1;
        }
      
        // 将数据存储到data数组中
        uint8_t *y_data = frame->data[0]; // 存储Y分量的数据
        uint8_t *u_data = frame->data[1]; // 存储U分量的数据
        uint8_t *v_data = frame->data[2]; // 存储V分量的数据
      
        // 接下来可以对y_data、u_data和v_data进行处理
      
        // 释放AVFrame结构体
        av_frame_free(&frame);
        return 0;
      }
      ```

- linesize：int 类型的指针数组，**存储音视频帧数据的行大小**

  - 表示`data`中每个缓冲区的大小

  - 行数据长度是按照 按行排列的采样数据量 计算得来的，采样像素数 * 每个像素占用的字节数

    > 如YUV420P包含 YUV三个分量的数据，P代表planar平面存储。
    >
    > 每个像素中，每个分量以8bit存储，每行中每个像素都采集Y，UV隔行隔列采集一个
    >
    > 因此，它的linesize[0]、linesize[1]和linesize[2]  分别为  图像宽度、图像宽度/2、图像宽度/2

  - 在使用`av_frame_get_buffer`初始化`AVFrame`时，会计算`linesize`的元素

  - extended_data：uint8_t* 类型的数组指针，用于存储额外的数据。通常用于存储多通道音频数据。

  - nb_samples：int 类型，每个通道的音频采样个数。

  - format：int 类型，表示音视频帧的像素格式。

  - width：int 类型，表示视频帧的宽度。

  - height：int 类型，表示视频帧的高度。

  - sample_rate：int 类型，表示音频帧的采样率。

  - channel_layout：uint64_t 类型，表示音频帧的声道布局。

  - pts：int64_t 类型，表示该帧的时间戳（presentation timestamp）以time base为单位。

  - pkt_pts：int64_t 类型，表示该帧所在数据包的时间戳。

  - pkt_dts：int64_t 类型，表示该帧所在数据包的解码时间戳。

  - metadata：AVDictionary* 类型，表示帧的元数据（metadata）信息。

  - pkt_pos：表示该帧数据在媒体文件中的位置，以字节为单位。



### 图像转换

#### `SwsContext`

​		**用于图像缩放和格式转换**。

```c
typedef struct SwsContext SwsContext;
```

- `swScale`：指向缩放函数的指针，用于将输入图像转换为输出图像。

  - ```c
    int (*swScale)(struct SwsContext *,							// 指向上下文结构体的指针；
                   const uint8_t* const srcSlice[],				 // 源图像的行指针数组；
                   const int srcStride[],						// 源图像每行的字节数组
                   int srcSliceY, int srcSliceH,				// 源图像裁剪的起始位置 / 源图像裁剪的高度
                   uint8_t* const dst[], const int dstStride[]); // 目标图像的行指针数组 / 目标图像每行的字节数组
    ```

  - 定义了缩放函数的基本框架。不同的缩放函数实现可以使用不同的算法和优化技术

  - FFmpeg库中提供了多种缩放函数，包括BICUBIC、BILINEAR、NEAREST

- `redBlueTable`：存储蓝色和红色通道的指针。

- `greenTable`：指向绿色通道的指针。

- `brightnessTable`：指向亮度表的指针。

- `contrastTable`：指向对比度表的指针。

- `swScaleFlags`：用于控制缩放函数的特定标志。



### 重采样

#### `SwrContext`

​		**用于音频重采样的上下文结构体**。存储重采样所需要的状态信息和中间数据。

```c
typedef struct SwrContext SwrContext;
struct SwrContext {
    int swr_flags;

    AVSampleFormat in_sample_fmt;
    AVSampleFormat out_sample_fmt;

    int64_t in_channel_layout;
    int64_t out_channel_layout;

    int in_sample_rate;
    int out_sample_rate;

    int nb_samples;

    struct ResampleContext *resample;

    int async;
    int available_samples;
    AVAudioResampleContext *avr;
    int allocated_samples;
    int8_t **midbuf;

    int matrix_encoding;
    int need_rematrix;
    void (*rematrix)(struct SwrContext *s, int32_t **matrix, int stride,
                     const int32_t *coefs, int channels);
    int rematrix_custom;

    void *log_ctx;
    int log_level_offset;
};

// SwrContext 中最重要的成员是 resample，它是运行时音频重采样内部操作状态的结构体
// 定义了一些关于重采样的参数和一些用于存储中间数据的变量
typedef struct ResampleContext ResampleContext;
struct ResampleContext {
    double factor;              // 采样率转换系数
    int phase_shift;            // 重采样器移位
    int in_incr, out_incr;      // 输入输出增量
    int32_t ideal_dst_incr;     // 目标增量
    int dst_incr;               // 实际目标增量
    int16_t *in_buffer;         // 输入缓冲区
    int in_buffer_count;        // 输入缓冲区大小
    int16_t *resample_ptr;      // 重采样指针
    int in_index, out_index;    // 输入输出索引
    int frac;                   // 分数部分
    int resample_slice;         // 重采样大小
    void *filter_bank;          // 滤波器状态结构体的指针
    void (*resample_one)(ResampleContext *c, int16_t *dst, const int16_t *src, int left, int right); // 单样本重采样函数
};
```

- swr_flags：重采样操作的一些标志，比如控制输出的采样数依赖于输入的采样数。
- in_sample_fmt/out_sample_fmt：输入和输出音频采样格式，比如 AV_SAMPLE_FMT_S16。
- in_channel_layout/out_channel_layout：输入输出的声道布局，比如 AV_CH_LAYOUT_STEREO。
- in_sample_rate/out_sample_rate：输入输出的采样率，比如 48000。
- nb_samples： SwrContext 内置的 FIFO 缓存的采样数。
- resample：音频重采样表示状态的结构体。
- async/available_samples：用于异步重采样的标志和采样数。
- beta1：存放音频重采样所需要的状态信息和缓存的指针。
- matrix_encoding：矩阵编码。
- rematrix：重编码函数。
- rematrix_custom：自定义重编码函数标记。
- log_ctx/log_level_offset：用于日志输出的上下文和其偏移量。





### util

#### `AVFifo`

​		FIFO（First In First Out）先进先出。

```c
struct AVFifo {
     uint8_t *buffer;
  
     size_t elem_size, nb_elems;
     size_t offset_r, offset_w;
     // distinguishes the ambiguous situation offset_r == offset_w
     int    is_empty;
  
     unsigned int flags;
     size_t       auto_grow_limit;
 };
```





#### `AVRational`

​		**表示有理数的结构体，用于描述时间基、帧率等涉及有理数的场合**。常常被用于描述媒体文件中的时间戳、帧率等涉及有理数的信息。

```c
typedef struct AVRational {
    int num; // 分子
    int den; // 分母
} AVRational;

AVCodecContext *codec_context = avcodec_alloc_context3(codec);
codec_context->time_base = (AVRational){1, 25}; // 设置时间基为1/25
```



#### `BITMAPFILEHEADER / BITMAPINFOHEADER`

​		用于描述位图文件的结构体。两者通常一起使用来完整地描述一个位图文件。

​		需要先读取`BITMAPFILEHEADER`结构体，获取文件的基本信息，然后再读取`BITMAPINFOHEADER`结构体，获取图像的详细信息，以便后续的处理。

- `BITMAPFILEHEADER`是位图文件的文件头
  - 描述整个位图文件的基本信息，包括文件类型、文件大小以及位图数据的偏移量等等
  - 它的作用类似于一个“外壳”，为解析位图文件提供了基本的信息
- `BITMAPINFOHEADER`是位图文件的图像信息头
  - 于描述位图文件中的图像信息，包括宽度、高度、位深、颜色平面数等等
  - 它的作用类似于一个“内核”，提供了位图图像的详细信息。



## 常用API



### `av_fast_malloc`

​		快速分配内存的函数。

```c
void av_fast_malloc(void *ptr, unsigned int *size, size_t min_size);
```

- `ptr`：指向当前已分配内存的指针；
- `size`：实例分配的大小。
- `min_size`：期望分配的大小。





### `av_calloc`

​		`FFmpeg`中的内存分配函数。

​		分配一个由`nmemb`个元素组成，每个元素大小为`size`字节的内存空间，并将空间中的每一位都初始化为`0`。

- `av_calloc`函数类似于C语言中的`calloc`函数
- 但`av_calloc`会使用`FFmpeg`内部的内存管理器进行内存分配
  - 可以保证分配出来的内存块是`FFmpeg`所支持的对齐方式
  - 可以有效避免由于内存对齐问题而导致的读写错误
- 使用`av_calloc`函数分配的内存需要使用`av_free`函数进行释放，并且需要保证释放的内存地址与`av_calloc`分配的地址一致
  - 否则可能会导致内存泄漏或其他问题

```c
void *av_calloc (size_t nmemb, size_t size);
```



```c
int *p = (int *)av_calloc(10, sizeof(int));
if (p == NULL) {
    // 内存分配失败，进行相关处理
} else {
    // 成功分配内存，进行相关操作
    p[0] = 1;
    p[1] = 2;
    // ...
    av_free(p); // 使用完后需要释放空间
}
```





### `avio_open2`

​		绑定`AVIOContext`对象并关联指定`URL`。

```c
int avio_open2(AVIOContext **s, const char *url, int flags, const AVIOInterruptCB *int_cb, AVDictionary **options);
```

- `s`：指向`AVIOContext`的指针，输入输出上下文
- `url`：要被打开的URL。可以是本地文件，也可以是网络地址
- `flags`：打开`URL`的选项标志，可以是以下标志的组合：
  - `AVIO_FLAG_READ`：以读取模式打开`URL`
  - `AVIO_FLAG_WRITE`：以写入模式打开`URL`
  - `AVIO_FLAG_READ_WRITE`：以读写模式打开`URL`
  - `AVIO_FLAG_NONBLOCK`：以非阻塞模式打开`URL`
  - `AVIO_FLAG_DIRECT`：禁用缓存，直接进行操作
  - `AVIO_FLAG_EXCL`：以排它模式打开`URL`
  - `AVIO_FLAG_CREAT`：如果`URL`不存在，则创建它
  - `AVIO_FLAG_TRUNC`：清除`URL`内容
  - `AVIO_FLAG_APPEND`：在`URL`的末尾进行写入
  - `AVIO_FLAG_NOBUFFER`：禁用缓存
- `int_cb`：`int_cb`是用于检测中断请求的回调函数指针，它允许在读取或写入数据时检测中断信号
  - 如果用户提供了一个非空回调函数，FFmpeg会定期调用它以允许中断。没有设置回调函数，设置为NULL
- `options`：指定的其他选项，是可选的
  - 经常用于传递应用程序特定的选项
  - 例如需要使用的传输协议、超时时间以及代理设置等。没有设置回调函数，设置为NULL

```c
// 输出文件上下文（&oFmtCtx->pb）与目标文件（dst）进行绑定
avio_open2(&oFmtCtx->pb, dst, AVIO_FLAG_WRITE, NULL, NULL);
// 写多媒体文件头到目的文件
ret = avformat_write_header(oFmtCtx, NULL);
// 用于将音频数据写入到目标文件中
av_interleaved_write_frame(oFmtCtx, &pkt);
//写多媒体文件尾到文件中
av_write_trailer(oFmtCtx);
```

​		通过输出文件上下文与目标文件进行绑定。该`AVIOContext`对象` oFmtCtx->pb`被关联到目标文件（即 dst 所指向的文件）。`avformat_write_header(oFmtCtx, NULL); av_interleaved_write_frame(oFmtCtx, &pkt);av_write_trailer(oFmtCtx); `就是向目标文件写入。



### `av_find_best_stream`

​		自动从多个流中找到某种类型的最佳的音视频流。它可以用于查找视频流、音频流、字幕流等。

​		**最佳的音视频流**是指在音视频文件中，最适合用来播放或者处理的音视频流。具体选取哪个流作为最佳流，取决于用户的需求和使用场景，通常需要考虑以下因素：

- 流类型：音频流、视频流、字幕流等。
- 媒体质量：码率、分辨率、帧率等。
- 编码格式：常见的音频编码格式有MP3、AAC等，常见的视频编码格式有H.264、H.265等，需要使用不同的解码器进行解码。

​		一般情况下，用户希望选取一个媒体质量较高，编码格式与设备兼容的流作为最佳流。

```c
int av_find_best_stream(AVFormatContext *ic, enum AVMediaType type, int wanted_stream_nb, int related_stream, AVCodec **decoder_ret, int flags);
```

- `ic`：输入的`AVFormatContext`。
- `type`：所需的`AVMediaType`。(获取音频流、视频流和媒体流等)
- `wanted_stream_nb`：需要查找的流的索引，如果为-1，表示不知道需要找哪个流，则会返回查找第一个的流的索引。
- `related_stream`：这个参数在查找时可以作为参考流，这可能有助于匹配某些媒体文件，设为-1则不会发生关联流。（通常设置为-1）
- `decoder_ret`：`AVCodec`类型的指针，表示要使用的解码器。如果为`NULL`，则会自动选择解码器。
- `flags`：此参数在查找过程中提供了附加提示。设置为`AVFIND_BEST_STREAM_SKIP_STREAM`时，函数将跳过第一个零帧数的流。设置为`AVFIND_BEST_STREAM_AUTOISO`时，函数将自动选择最佳ISO码率设置。（通常设置0）
- **函数返回值为找到的流的索引**，`-1`表示未找到。

```c
int video_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
if (video_stream_index < 0) {
    printf("Failed to find a video stream\n");
    return -1;
}
//查找输入文件中的最佳视频流，并将其索引存储在video_stream_index变量中
//如果找到了一个合适的流，则该函数将填充该流的解码器信息（如果有的话）。
```



### `av_guess_format`

​		自动猜测指定文件的输入或输出格式。它通过文件扩展名或已知的文件格式进行猜测。

​		通常情况下，它用于在打开文件之前自动检测文件格式，以便为`AVFormatContex`t结构体设置正确的`demuxer`或`muxer`。

1. 如果`short_name`不为空，则根据该名称猜测文件格式

   1. 如果能够找到该格式，则返回指向该文件格式的指针；否则，返回NULL。

2. 如果`filename`不为空，则根据文件名的扩展名猜测文件格式

   1. 如果能够找到该格式，则返回指向该文件格式的指针；否则，返回NULL。

3. 如果`mime_type`不为空，则根据文件的`MIME`类型猜测文件格式

   1. 如果能够找到该格式，则返回指向该文件格式的指针；否则，返回NULL。

4. 如果`short_name`、`filename`和`mime_type`都为空，则返回NULL。

5. `AVOutputFormat`结构体中的`name`字段表示该格式的名称，`extensions`字段表示该格式所有支持的扩展名，`mime_type`字段表示该格式的`MIME`类型

   > 因此，可以根据返回的``AVOutputFormat``结构体来获取该格式的信息，例如格式名称、支持的扩展名、MIME类型等等。

```c
AVOutputFormat *av_guess_format(const char *short_name, const char *filename, const char *mime_type);
```

- `short_name`：短格式名称，如“`mp4`”、“`mpeg`”等
- `filename`：要猜测文件格式的文件名，为`NULL`则表示只利用`short_name`来猜测格式
- `mime_type`：用于可选的mime类型，为`NULL`则表示只利用`short_name`和`filename`来猜测格式



### `av_guess_sample_aspect_ratio`

​		用于猜测给定帧的样本宽高比。

```c
AVRational av_guess_sample_aspect_ratio(AVFormatContext *format_context, AVStream *stream, AVFrame *frame);
```

- `format_context`：输入文件的上下文。
- `stream`：帧所在的流。
- `frame`：需要猜测样本宽高比的帧。
- 返回猜测出的样本宽高比。



### `av_guess_frame_rate`

​		猜测视频帧率。

​		根据流的时基和帧的时间戳猜测视频的帧率。如果输入的视频流中存在无效的帧率数据，那么猜测的结果可能不准确。如果需要精确的帧率信息，可以从`AVStream`结构体中的`r_frame_rate`字段中获取。

```c
AVRational av_guess_frame_rate(AVFormatContext *ctx, AVStream *stream, AVFrame *frame);
```

- `ctx`：输入的AVFormatContext上下文，指向包含此视频的容器格式上下文
- `stream`：输入的AVStream结构体，指向视频流
- `frame`：视频帧（AVFrame结构体）
- 返回值为一个AVRational结构体，表示猜测的帧率。如果无法猜测，则返回一个缺省的帧率。



### `av_get_bytes_per_sample`

​		计算指定采样格式的每个采样所占用的字节数。

```c
int av_get_bytes_per_sample(enum AVSampleFormat sample_fmt);
```

- `sample_fmt`，表示采样格式
- 返回值是一个整数，表示每个采样所占用的字节数

```c
// 计算16位有符号整数类型的采样每个采样所占用的字节数
int bytes_per_sample = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16); // 返回2，表示每个采样占用2个字节
```



### `av_strdup`

​		将一个字符串复制到新的内存空间中，并返回新的字符串指针。

​		类似于 C 标准库中的 strdup 函数，但是 av_strdup 函数使用的是 FFmpeg 中的内存分配函数 av_malloc 和 av_free，因此可以避免一些内存管理上的问题。另外，av_strdup 函数还可以用于复制 FFmpeg 中 AV 字符串（AVString）类型的字符串。

```c
char *av_strdup(const char *s);
```

- `s`：要复制的字符串。
- **返回值为新的字符串指针**，如果分配内存失败，则返回 NULL。



### `av_q2d`

​		将 `AVRational` 类型的有理数转换为双精度浮点数。

- `AVRational` 类型是 FFmpeg 库中用于表示有理数的结构体，包含了分子和分母。
- 视频编解码中，常常用到分数来表示帧率、码率等信息

```c
double av_q2d (AVRational a);
```

- `a`， 是一个 `AVRational` 类型的有理数。
- 返回一个双精度浮点数，表示将有理数 `a` 转换为浮点数后的值。

```c
double d = av_q2d(r);
```

```c
// 具体实现如下：
static inline double av_q2d(AVRational a)
{
    return a.num / (double) a.den;
}
```



```c
pkt.pts * av_q2d(in_stream1->time_base); // 是一个时间单位转换的操作，它将 pts（presentation time stamp）从流的时间基（time base）转换为实际时间单位（单位为秒）。
// 将 AVPacket 中存储的 pts 时间戳乘上输入媒体流的时间基转换为秒数，得到音视频帧在实际时间中的显示时间
```

- 输入或输出媒体流中的每个音视频帧都包含一个 `pts`（显示时间戳）和一个 `dts`（解码时间戳）
- AVPacket 结构体中的 `pts `和 `dts `字段存储的是时间戳，需要根据相应的时间基进行转换才能得到实际的时间值。
- `av_q2d(in_stream1->time_base)` 的作用是将时间基转换为秒数。
  - 具体来说，`av_q2d `函数可以将任意一个有理数时间基转换为秒数，
    

### `av_rescale_q`

​		用于将两个不同时间基的值进行转换。

​		可以将一个时间轴上的值按比例转换为另一个时间轴上的值。该函数使用AVRatioal结构体定义时间基。

```c
int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq);
```

- `a`: 要转换的值
- `bq`: 要转换的值的时间基
- `cq`：要转换到的时间基
- 返回值：**转换后的值**，类型为int64_t



### `av_rescale_q_rnd`

​		用于时间戳值转换的函数。

​		在音视频编解码、滤波等操作中，常常需要将时间戳值从输入信息（如采样率、帧率）的时间基转换为输出信息（如输出采样率、帧率）的时间基。`av_rescale_q_rnd()` 可以较方便地实现这一过程。

```c
int64_t av_rescale_q_rnd(int64_t a, AVRational bq, AVRational cq, enum AVRounding rnd);
```

- `a` 表示需要进行时间戳转换的值
- `bq`为源时间戳的时间基
- `cq`为目标时间戳的时间基
- `rnd`为指定的取整方式
  - `AV_ROUND_ZERO`：向 0 取整
  - `AV_ROUND_INF`：向上取整（取离线条最远的整数）
  - `AV_ROUND_DOWN`：向下取整
  - `AV_ROUND_NEAR_INF`：根据最近值取整

​		该函数会将时间戳值 `a` 从 `bq` 时间基转换为 `cq` 时间基，并根据取整方式 `rnd` 进行取整

- **返回转换后的时间戳值**

```c
AVRational in_timebase = {1, 44100};
AVRational out_timebase = {1, 48000};
// 定义了输入和输出采样率对应的时间基分别为 1/44100 和 1/48000

for (int i = 0; i < 100; i++) {
    // // 采样点的时间戳从输入时间基转换为输出时间基
    int64_t ts = av_rescale_q_rnd(i * in_timebase.num, in_timebase, out_timebase, AV_ROUND_NEAR_INF);
    // 进行相应的处理
    ......
}
```



### `av_packet_rescale_ts`

​		将AVPacket结构体中的时间戳（pts、dts）从一个时间基转换为另一个时间基。

```c
int av_packet_rescale_ts(AVPacket *pkt, AVRational src_tb, AVRational dst_tb);
```

- `pkt`指向要进行时间戳转换的`AVPacket`结构体
- `src_tb`表示`pkt`中时间戳的时间基
- `dst_tb`表示需要将时间戳转换为的时间基



### `av_interleaved_write_frame`

​		用于将编码后的音视频数据写入输出文件。

```c
int av_interleaved_write_frame(AVFormatContext *context, AVPacket *packet);
```

- `context`：指向打开的媒体文件的`AVFormatContext`结构体指针。
- `packet`：指向`AVPacket`结构体的指针，包含待写入的音频或视频帧数据。
- 结果不是一个确定的返回值，通过查看`AVOutputFormat`结构体中的`write_packet`回调函数获得多媒体文件输出的状态。
  - 如果该回调函数返回负数，则表示写入数据出现了错误。



### `av_samples_get_buffer_size`

​		计算音频数据缓冲区大小的函数。

​		计算给定采样格式、通道数、采样数等参数下，音频数据缓冲区需要的总大小。

```c
int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
                               enum AVSampleFormat sample_fmt, int align);
```

- `linesize`：指向缓冲区每行数据的大小，如果为 NULL 则不计算；
- `nb_channels`：音频通道数；
- `nb_samples`：每个通道的采样数；
- `sample_fmt`：采样格式，例如 AV_SAMPLE_FMT_S16、AV_SAMPLE_FMT_FLT 等；
- `align`：缓冲区对齐方式，通常为 0。
  - `0 = default`，常用于提供冗余的大小，此函数之后常常跟着的是类malloc函数。
  - `1 = no alignment`，不对齐，其实即是按1字节对齐，也就是说，求的是音频数据的真实大小，常常用来计算出刚刚(调用此函数之前)转码出来的数据大小





### `avformat_open_input`

​		打开输入文件并分配和初始化`AVFormatContext`结构体的函数。

```c
int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
```

- `ps`：指向指向`AVFormatContext`结构体的指针
- `url`：输入文件的URL或文件路径
- `fmt`：指定输入文件的格式上下文，通常为NULL，根据文件名的后缀自动检测
  - 存在这种情况：文件的后缀和实际包装的内容不同，例如：文件后缀是flv，但是里面存储的是mp4文件
- `options`：指定打开输入流的额外选项，没有额外参数，给它默认值为`NULL`
- **返回值为0表示成功，否则表示失败**。如果该函数返回一个负数错误代码，您可以使用`av_err2str`转换函数将其转换为相应的错误消息

```c
AVFormatContext *fmt_ctx = NULL;
const char *filename = "input.mp4";
int ret = 0;

// 打开输入文件并分配AVFormatContext结构体
if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
	// av_err2str是FFmpeg中的一个函数，用于将错误代码转换为错误消息的字符串表示形式。 它的函数原型如下：
    printf("Could not open input file '%s' (error '%s')\n", filename, av_err2str(ret));
    return ret;
}
```





### `avformat_close_input`

​		关闭输入封装格式上下文的函数。

​		关闭一个打开的输入格式上下文，并释放其内存空间。函数会自动释放所有被该上下文对象所占用的资源，如打开的文件、解码器、各种缓冲区等。因此，当不再需要使用输入格式上下文对象的时候，应该调用该函数来释放其内存空间，避免内存泄漏。

​		如果传入的`ps`参数为`NULL`，则该函数不会执行任何操作。
```c
void avformat_close_input(AVFormatContext **ps);
```

- `ps`：指向待关闭的输入格式上下文的指针的指针

```c
AVFormatContext *fmtCtx;

avformat_open_input(&fmtCtx, "input.mp4", NULL, NULL);
// ... do something ...
avformat_close_input(&fmtCtx);
```



### `avformat_find_stream_info`

​		用来获取音视频文件中所有流的详细信息的函数，包括流的类型、编码信息、帧率、分辨率等。

```c
int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
```

- `ic`：AVFormatContext类型的指针，表示音视频文件的上下文。
- `options`：AVDictionary类型的指针，表示额外的选项参数，可以为NULL。
- **函数返回值为0表示成功，否则表示失败**。函数执行后，AVFormatContext中的成员变量AVStream中存储了音视频文件中所有流的详细信息。



### `avformat_alloc_context`

​		为``AVFormatContext``类型的结构体分配空间并初始化。

```c
AVFormatContext *avformat_alloc_context(void); // 返回一个分配好的指向AVFormatContext结构体的指针
```



### `avformat_free_context`

​		用于释放格式上下文内存空间的函数。

​		与`avformat_close_input`函数不同的是，`avformat_free_context`只释放格式上下文本身所占用的内存空间，而不会释放其他由该对象占用的资源，如打开的文件、解码器、各种缓冲区等。因此，当不再需要使用格式上下文对象的时候，如果不需要释放其他资源，则可以直接调用该函数释放其内存空间。

```c
void avformat_free_context(AVFormatContext *s); // 释放一个格式上下文对象所占用的内存空间，并将该对象置为NULL
```

- `s`：需要释放内存空间的格式上下文对象

```c
AVFormatContext *fmtCtx;

avformat_open_input(&fmtCtx, "input.mp4", NULL, NULL);
// ... do something ...
avformat_free_context(fmtCtx);
```



- 



### `avformat_alloc_output_context2`

​		用于生成一个带有输出格式的`AVFormatContext`对象，以用于将音视频编码数据写入到文件或其他媒体输出流中。

- 相当于`avformat_alloc_context`和`av_guess_format`的结合
  - 可以自动猜测输出格式，或者根据`oformat`参数手动指定输出格式
  - 如果同时指定了`oformat`参数和`format_name`参数，则以`oformat`参数为准

```c
int avformat_alloc_output_context2(AVFormatContext **ctx, AVOutputFormat *oformat,
                                   const char *format_name,
                                   const char *filename);
```

- `ctx`：输出格式的`AVFormatContext`对象指针
- `oformat`：输出格式的`AVOutputFormat`对象，用于指定输出数据的格式
- `format_name`：用于猜测输出格式的名称，如果oformat为NULL，则会根据该参数进行猜测，可以为NULL。，可以是 NULL
- `filename`：字符串参数，描述输出文件的文件名，可以是 NULL
- 函数的返回值为零，表示成功生成了`AVFormatContext`对象，否则返回负的错误码



### `avformat_new_stream`

​		创建一个新的流（Stream）。

```c
AVStream *avformat_new_stream(AVFormatContext *ctx, const AVCodec *codec);
```

- `ctx`: `AVFormatContext` 类型，指向 FFmpeg 的格式上下文，用于读写各种多媒体文件格式
- `codec`: `AVCodec` 类型，指向编/解码器的 AVCodec 结构体
- 成功：新创建的 AVStream 指针。失败：NULL





### `avformat_write_header`

​		向目标文件写入多媒体格式头。

​		使用该函数之前：

- 需要对 `AVFormatContext` 结构体进行初始化，包括设置格式参数和媒体流信息等
- 需要先使用 `avio_open2` 函数打开输出文件并将其与封装格式上下文关联起来

```c
int avformat_write_header(AVFormatContext *s, AVDictionary **options);
```

- `s` ：指向输出文件的`AVFormatContext`结构体的指针
- `options` ：指向 `AVDictionary` 字典对象的指针，可选参数，用于传递一些额外的信息
  - 通常情况下可以将其设置为NULL，表示不需要特殊选项、使用默认值
- 返回值是一个错误码，返回 0 表示操作成功，返回负数表示操作失败。







### `avcodec_alloc_context3`

​		分配并初始化编解码器上下文。

```c
AVCodecContext *avcodec_alloc_context3(const AVCodec *codec);
```

- `codec `，要使用的编解码器的指针，如果不需要指定特定的编解码器，则设置为 `NULL`
- 返回值是指向已分配并初始化的 `AVCodecContext `结构体的指针
  - 如果出现任何错误或无法分配内存，则返回 `NULL`



### `avcodec_free_context`

​		释放`AVCodecContext`结构体内存的函数。

​		会释放AVCodecContext结构体所申请的内存空间，**使用该函数需要注意的是，传入的`AVCodecContext`结构体指针必须是通过`avcodec_alloc_context3`函数申请的。**

```c
void avcodec_free_context(AVCodecContext **avctx);
```

- `avctx`是`AVCodecContext`结构体指针的指针



### `avcodec_find_encoder`

​		用于根据编码器 ID 查找匹配的编码器并返回指针，该函数在使用编码器之前需要先注册编码器。

```c
AVCodec *avcodec_find_encoder(enum AVCodecID id);
```

- `id` 是 `enum AVCodecID` 类型的编码器 ID
- 如果找到了指定名称的编码器，则返回一个指向`AVCodec`结构体的指针。如果没有找到，则返回`NULL`



### `avcodec_find_encoder_by_name`

​		用于根据编码器名称查找匹配的编码器并返回指针，该函数在使用编码器之前需要先注册编码器。

```c
AVCodec *avcodec_find_encoder_by_name(const char *name);
```

- `name`参数是需要查找的编码器名字
- 如果找到了指定名称的编码器，则返回一个指向`AVCodec`结构体的指针。如果没有找到，则返回`NULL`



```c
// 查找x264编码器
AVCodec *codec = avcodec_find_encoder_by_name("libx264");

if (codec == NULL) {
    fprintf(stderr, "Could not find the x264 encoder.\n");
    return -1;
}

// 初始化AVCodecContext结构体
// 可以使用这个指针来获取编码器相关的信息和参数，进而进行后续的编码操作。
AVCodecContext *codec_context = avcodec_alloc_context3(codec);
if (codec_context == NULL) {
    fprintf(stderr, "Could not allocate codec context.\n");
    return -1;
}

// 设置编码参数
// 在实际使用中，我们还需要为AVCodecContext结构体设置一些编码器参数
codec_context->bit_rate = 500000;
codec_context->width = 1280;
codec_context->height = 720;
codec_context->time_base = (AVRational) {1, 25};

// ...
```



### `avcodec_find_decoder`

​		用于根据解码器ID查找解码器的函数。

- `avcodec_find_decoder`函数只能查找已经注册的解码器，因此在使用该函数之前需要确保使用`av_register_all`函数注册了所有的解码器
- `4.0`以前的版本中，也可以使用`avcodec_register_all`函数快速注册所有的解码器

```c
AVCodec *avcodec_find_decoder(enum AVCodecID id);
```

- `id`为`AVCodecID`类型的解码器ID，定义在AVCodecID枚举类型中。

```c
// 查找H.264解码器
AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
if (!codec) {
    fprintf(stderr, "未找到解码器\n");
    exit(1);
}

// 根据解码器申请AVCodecContext内存
AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
```



### `avcodec_open2`

​		用于打开解码器的函数。

​		将`AVCodecContext`和解码器绑定（也就是让`AVCodecContext`结构体的解码器参数和解码器相结合，让解码器知道怎么解压），初始化`AVCodecContext`和解码器，打开解码器以准备解码的过程。

​		**只能打开一个解码器一次，多次打开相同的解码器将会导致错误**。因此，在个别情况下需要使用多个AVCodecContext来打开多个解码器。

```c
int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
```

- `avctx`是`AVCodecContext`结构体指针，存储了解码器的参数信息
- `codec`是`AVCodec`结构体指针，表示要打开的解码器的信息
- `options`则是选项的字典。

```c
// 查找H.264解码器
AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
if (!codec) {
    fprintf(stderr, "未找到解码器\n");
    exit(1);
}

// 根据解码器申请AVCodecContext内存
AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

// 设置AVCodecContext参数
// ...

// 打开解码器
if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
    fprintf(stderr, "无法打开解码器\n");
    exit(1);
}
```



### `avcodec_send_packet`

​		将编码后的数据包发送给编解码器进行解码。

​		会将数据包中的数据发送到缓冲区，等待编码器从中取出数据。如果缓冲区已经被填满，则avcodec_send_packet会一直等待，直到缓冲区有足够的空间存放这个数据包。

```c
int avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt);
```

- `avctx`：指向已打开的`AVCodecContext`的指针。包含了摄像头、文件格式、编解码器等相关信息
- `avpkt`：指向要发送的`AVPacket`的指针。包含了编码数据的数据包。
- 返回值是一个整型，代表发送结果：
  - 0：成功发送。
  - AVERROR_EOF：当前输入流已结束。
  - AVERROR_EAGAIN：解码器暂时无法接受输入，需要等待。
  - AVERROR_EINVAL：无效参数。



### `avcodec_receive_packet`

​		用于获取编码器输出的压缩数据包。该函数从编码器中读取已经编码好的数据包。

- 从编码器中读取已经编码好的数据包，可以获取到压缩后的视频数据包，该数据包可以写入文件、网络等传输。
- 可以在 `avcodec_send_frame `函数调用之后，不断循环调用该函数读取输出压缩数据包，直至数据读取完毕。

```c
int avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt);
```

- AVCodecContext *avctx：编码器的上下文环境。
- AVPacket *avpkt：存储编码好的数据包对象。
- 返回值：
  - 返回 0 表示获取编码器输出的压缩数据包成功。
  - 返回 AVERROR_EOF 表示编码器输出队列被清空。
  - 返回其他值则代表获取编码器输出的压缩数据包失败。



```c
AVCodecContext *codec_ctx;  //编码器的上下文环境
AVPacket *avpkt;            //存储编码好的数据包对象

//必须先初始化编码器上下文环境与压缩数据包存储对象才能使用此函数

//从编码器中获取一帧压缩数据
int ret = avcodec_receive_packet(codec_ctx, avpkt);
if (ret == 0) {
    //压缩数据包获取成功
} else if (ret == AVERROR_EOF) {
    //编码器输出队列被清空
} else {
    //压缩数据包获取失败
}
```



### `avcodec_send_frame`

​		用于向编码器发送帧数据。

​		该函数将视频帧的数据送入编码器，对应的编码器将对该帧进行编码。包括视频像素数据及音频采样数据等。函数执行完后，所有的数据都会被加载到编码器的发送缓冲区中等待编码器进行编码。

```c
int avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame);
```

- `AVCodecContext *avctx`：编码器的上下文环境
- `const AVFrame *frame`：待编码的视频帧数据，包括视频像素数据及音频采样数据等
- 返回 0 表示成功。返回其他值则代表发送失败或者发送缓存已经被填满。



### `avcodec_receive_frame`

​		从编解码器的输出队列中获取解码后的输出帧，并将其存储在AVFrame结构体中。

- 需要先通过调用`avcodec_send_packet`将编码后的数据发送给编解码器进行解码
  - 调用`avcodec_receive_frame`函数之前，必须确定编解码器有待解码的数据包
  - 解码器队列中没有可用帧的情况下，函数会一直等待，直至队列中有可用帧
- `AVFrame`结构体中存储的是解码后的输出帧，包括视频像素数据和音频采样数据等

```c
int avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame);
```

- `avctx`是一个指向`AVCodecContext`结构体的指针，它包含了编解码器的相关信息
- `frame`是一个指向`AVFrame`结构体的指针，它用于存储解码后的输出帧
- 返回值为0：正常状态，表示函数成功执行，获取了有效的输出帧；异常AVERROR
  - AVERROR_EOF：表示"End of file"，即文件已经结束
    - 它的返回值通常用于描述音视频解码器、编码器等相关函数内部已经处理完数据流的情况
    - 通知调用者已经到达了输入流的末尾，不会再产生更多的数据
    - 输入音视频流中可能存在没有正常结束的情况，例如文件损坏、网络异常等导致数据流被截断
      - 此时，`AVERROR_EOF`并不是正确的结束信号，而只是表示当前的数据流已经结束，解码器已经完全解码完输入的数据
  - `AVERROR_EAGAIN`：表示"Try again"，即再试一次
    - 它的返回值通常不是错误，而是表示当前操作需要等待某些已经就绪的操作（如缓存中的数据已经就绪），或者需要等待更多数据到达
    - 通常用于描述解码器、编码器、输入输出设备等相关函数内部缓存区的数据中没有有效数据时的状态，需要增加数据输入
    - **通常表示解码器缓冲区中已经没有可用帧，需要等待更多的数据输入**
      - **此时，可以继续发送数据给解码器，并再次调用`avcodec_receive_frame`函数，直到下一帧数据解码完成为止**
  - `AVERROR_EINVAL` 表示参数不正确
  - 其他错误码：表示解码过程中出现了其他错误，可以通过调用`av_strerror`函数将错误码转换成错误信息。





### `avcodec_parameters_copy`

​		将一个`AVCodecParameters`结构体中的参数复制到另一个`AVCodecParameters`结构体中。

​		把输入流中的编码器参数复制到输出流的编码器参数，使得输出文件的编码器参数和输入文件保持一致。

​		**在使用该函数复制编码器参数时，需要注意目标`AVCodecParmaeters`结构体必须已经被分配内存，否则会引发错误**。

```c
int avcodec_parameters_copy(AVCodecParameters *dst, const AVCodecParameters *src);
```

- `dst`: `AVCodecParameters` 类型，指向目标 `AVCodecParameters` 结构体指针
- `src`: `AVCodecParameters` 类型，指向源 `AVCodecParameters `结构体指针
- 成功：0。失败：负数错误码



### `avcodec_parameters_to_context`

​		将从`AVFormatContext`中获取的`AVCodecParameters`结构体中的参数赋值给`AVCodecContext`结构体。

- `AVCodecParameters`结构体则用于描述编解码器参数，包括音频流的采样率、视频流的宽度和高度等参数
- `AVCodecContext`结构体用于描述编解码器上下文

```c
int avcodec_parameters_to_context(AVCodecContext *codec, const AVCodecParameters *par);
```

- `codec`表示要被更新的`AVCodecContext`结构体
- `par`表示从中获取编解码器参数的`AVCodecParameters`结构体

```c
AVFormatContext *fmt_ctx;
AVCodec *dec;
AVCodecContext *codec_ctx;
AVCodecParameters *codecpar;
int stream_index;

// 打开文件
if (avformat_open_input(&fmt_ctx, "input.mp4", NULL, NULL) < 0) {
    return;
}

// 查找音频流
if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
    return;
}
stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);

// 获取AVCodecParameters结构体
codecpar = fmt_ctx->streams[stream_index]->codecpar;

// 分配AVCodecContext结构体
codec_ctx = avcodec_alloc_context3(dec);

// 将参数拷贝到AVCodecContext结构体中
// 从一个已打开的音频流中拷贝编码器参数到编解码上下文中，以便使用该编码器参数进行后续操作
if (avcodec_parameters_to_context(codec_ctx, codecpar) < 0) {
    return;
}

// 后续操作...
```



### `av_packet_unref`

​		释放AVPacket结构体中的资源。

​		通常是在一个音视频数据包已经使用完之后，为了回收内存资源而进行的操作。**该函数会将 AVPacket 结构体中的所有资源（内存、引用计数－1等等）全部清空，并将 AVPacket 结构体本身保留下来供后续复用**，比直接释放完整个结构体更高效。

```c
void av_packet_unref(AVPacket *pkt);
```



### `av_packet_free`

​		**会将 AVPacket 结构体以及其中的全部资源（内存、引用计数等等）全部释放，并将 AVPacket 结构体本身也一同释放，因此后续无法继续使用该 AVPacket 结构体**。

```c
void av_packet_free(AVPacket **pkt);
```



### `av_packet_move_ref`

​		用于对`AVPacket`数据包对象进行缓冲区引用的转移。

​		将目标AVPacket对象中的缓冲区引用值设置为源AVPacket对象中的缓冲区引用值。释放源src对象所占用的空间，但并没有释放src结构体。

```c
void av_packet_move_ref(AVPacket *dst, AVPacket *src);
```





### `av_frame_alloc`

​		用于申请一个 `AVFrame`结构体内存的函数。

​		一般在使用`AVFrame`结构体之前都需要先调用`av_frame_alloc`来进行初始化和内存申请。

​		函数会动态分配一个 AVFrame 结构体，然后将其内存全部清零，并返回一个指向它的指针。

```c
AVFrame *av_frame_alloc(void);
```



### `av_frame_free`

​		释放`AVFrame`结构体内存的函数。

```c
void av_frame_free(AVFrame **frame);
```





### `av_read_frame`

​		读取输入文件中的下一帧数据。该函数会自动从输入文件中解码一帧数据并将其存储在`AVPacket`结构体中。

- 调用者需要在每次读取完成后一帧数据后释放`pkt`数据的内存

  ` av_packet_unref(&pkt);`

```c
int av_read_frame(AVFormatContext *s, AVPacket *pkt);
```

- `s` ：指向 `AVFormatContext` 结构体的指针，用于表示输入多媒体格式上下文，即输入文件的基本信息和属性
- `pkt` ：指向 `AVPacket` 数据包结构体的指针，用于表示读取到的数据帧
- 函数返回值小于0时，就说明已经读取到文件的末尾或者发生了错误

```c
int ret;
AVPacket pkt;

while (av_read_frame(inputFormatContext, &pkt) >= 0) {
    /* 处理pkt中的音频、视频或字幕数据 */
    
    av_packet_unref(&pkt);
}
```



### `av_seek_frame`

​		用于定位媒体文件中特定时间戳的帧。

```c
int av_seek_frame(AVFormatContext *format_context, int stream_index, int64_t timestamp, int flags)
```

- `AVFormatContext *format_context`：媒体文件的格式上下文结构体，包含了媒体文件的格式信息和媒体流的相关信息。
- `int stream_index`：代表要定位到的媒体流的索引值。
- `int64_t timestamp`：代表要定位到的时间戳，单位是微秒。
- `int flags`：代表定位的标志，用于指定查找模式。可以选择以下值之一：
  - `AVSEEK_FLAG_BACKWARD`：表示向后查找，即定位到给定时间戳之前的最近帧。
  - `AVSEEK_FLAG_BYTE`：表示精确查找，即定位到给定时间戳所在的位置。
  - `AVSEEK_FLAG_ANY`：表示无限制查找，即从任意位置查找给定时间戳。
  - `AVSEEK_FLAG_FRAME`：表示以帧为单位查找，即定位到最接近给定时间戳的关键帧位置。
- 函数返回非负整数代表成功，返回错误代码代表失败



### `av_frame_get_buffer`

​		用于为AVFrame结构体中的data域分配内存空间。

​		进行视频编解码时，程序需要使用`AVFrame`结构体来存储视频帧数据。在使用这个结构体之前，需要先为其分配内存空间。`av_frame_get_buffer`函数为`AVFrame`中每个数据指针分配内存。

- `av_frame_alloc`分配`AVFrame`的内存
- `av_frame_get_buffer`分配`AVFrame`中各个数据字段所需要的内存，分配完成后数据区域就可以用于存储采样数据或者像素数据等

```c
int av_frame_get_buffer(AVFrame *frame, int align);
```

- `frame`是需要分配内存空间的`AVFrame`结构体
- `align`表示内存对齐的大小，通常设置为`0`，表示默认内存对齐方式
- 函数返回一个整数值，代表是否成功为 AVFrame 分配内存，成功时返回 0，失败时返回 `AVERROR`





### `av_frame_make_writeable`

​		用于确保 AVFrame 可以进行写入操作的函数。

​		 `av_frame_get_buffer `函数为 `AVFrame `的数据缓存区分配一定大小的内存空间。分配的内存空间可能并不足够用于存储实际数据，这时就需要使用 `av_frame_make_writable `函数确保 **确保frame中的data域**可以进行写入操作。

​		如果已经是可写状态，则直接返回 0，否则会进行新的缓存区的分配工作。调用该函数时需要注意当缓存区内存不足重新分配并拷贝数据可能会导致性能损失，如果分配失败将返回 AVERROR。

​		通常是由其他进程或其他线程写入数据，需要调用 `av_frame_make_writable `函数确保可写，否则将有可能导致系统崩溃或数据损坏。

```c
int av_frame_make_writable(AVFrame *frame);
```

-  `frame`，要被写入数据的 `AVFrame`

> ​		进行编码的时候，需要将frame传给编码器，编码器拿到frame之后，锁定frame中的data，然后拿这个数据进行编码，锁定的时候，如果我们想使用这个frame再向编码器传送一帧数据，就会发生冲突。
>
> ​		调用av_frame_make_writeable函数，会检测frame中的data域，看这个data域是否被锁定，若被锁定，就会重新为frame中data域分配一个buffer，如果没有锁定，说明现在这个data域是可写的。这样后面就可以向这个data域写入数据。



### `av_fifo_alloc2`

​		分配并初始化`AVFifo`。

```c
AVFifo* av_fifo_alloc2(size_t elems, size_t elem_size, unsigned int flags;	
```

- `elmes`: 队列的原始个数
- `elem_size`： 每个元素的字节大小。
- `flags`：指定 `FIFO` 缓冲区的标志
  - `AV_FIFO_FLAG_NONBLOCK `表示在缓冲区满或空时不阻塞，而是立即返回错误
  - `AV_FIFO_FLAG_AUTO_GROW `表示当 FIFO 缓冲区满时，自动扩展缓冲区的大小，而不会返回错误
- 成功时分配`AVFIifo `



### `av_fifo_read`

​		用于从 `FIFO `队列中读取数据的函数。

```c
int av_fifo_read(AVFifo * f, void * buf, size_t nb_elems);
```

- `f`：指向要读取数据的 AVFifo 指针。
- `buf`：指向可存放读取数据的缓冲区 nb_elems * av_fifo_elem_size(f) bytes will be written into buf on success.。
- `nb_elems`：元素个数。
- 函数返回值为非负值表示成功，负值表示失败。



### `av_fifo_write`

​		用于向`FIFO`队列中写入数据。

```c
int av_fifo_write(AVFifo * f, const void * buf, size_t 	nb_elems);
```

- `f`：待写入的AVfifo队列
- `buf`: 待写入的数据。
- `nb_elems`：写入的元素个数。
- 成功时返回非负数，失败时返回负数。



### `av_fifo_freep2`

​		释放`AVFifo`并将其指针置为`NULL`。

```c
void av_fifo_freep2(AVFifo** f);
```

- `f`：指向一个要释放的 AVFifo. 
  - `*f == NULL` 被允许



### `avchannel_layout_copy`

​		用于将一个音频通道布局（AV_CH_LAYOUT_XXX）从源中复制到目标中。

```c
uint64_t av_channel_layout_copy(uint64_t *dst, const uint64_t *src);
```

- `dst` 是目标音频通道布局的指针
- `src` 是源音频通道布局的指针

```c
AVCodecContext *ctx;
// 给 ctx 赋值
AVCodecContext temp_ctx;
uint64_t dst_layout = 0;
int ret;

temp_ctx = *ctx;
ret = av_channel_layout_copy(&dst_layout, &temp_ctx.channel_layout);
if (ret < 0) {
    fprintf(stderr, "Failed to copy audio channel layout: %s\n", av_err2str(ret));
    // 错误处理
}
```



### `swr_alloc`

​		用于创建 `SwrContext` 结构体的函数。

​		创建一个 `SwrContext` 结构体，并为其分配内存空间。需要注意的是，创建该结构体后需要使用 `swr_alloc_set_opts` 函数来设置音频的输入和输出参数，否则不能进行音频重采样。

​		使用完 `SwrContext` 结构体后，需要调用 `swr_free` 函数来释放内存空间。

```c
SwrContext *swr_alloc(void);
```

- 返回值为创建的 `SwrContext` 结构体的指针，如果创建失败，则返回 `NULL`



### `swr_free`

​		用于释放 `SwrContext` 结构体内存的函数。

​		释放成功后，`SwrContext` 结构体指针被设置为 `NULL`。

```c
void swr_free(SwrContext **s);
```

- `s` 参数为指向 `SwrContext` 结构体指针的指针，表示要释放的结构体





### `swr_alloc_set_opts2`

​		根据需要分配`SwrContext`，并设置/重置公共参数。

```c
SwrContext *swr_alloc_set_opts2(SwrContext *s,
                                int64_t out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
                                cint64_t in_ch_layout, enum AVSampleFormat in_sample_fmt, int in_sample_rate,
                                int log_offset, void *log_ctx);
```

- `s`: SwrContext 结构体指针，如果为NULL，则会创建一个新的SwrContext对象，否则会更新现有的SwrContext对象。
- `out_ch_layout`：输出通道布局；
- `out_sample_fmt`：输出采样格式；
- `out_sample_rate`：输出采样率；
- `in_ch_layout`：输入通道布局；
- `in_sample_fmt`：输入采样格式；
- `in_sample_rate`：输入采样率；
- `log_offset`：日志级别；
- `log_ctx`：AVClass 上下文指针
- **返回值：**成功时为0，错误时为负AVERROR代码。错误时，Swr上下文被释放，*s设置为NULL



### `swr_convert`

​		用于进行音频重采样的核心函数。

​		`swr_convert` 函数为音频数据进行重采样，输入缓冲区中的音频数据会被重采样为指定格式的输出缓冲区所需的样本数。

```c
int swr_convert(SwrContext *s, uint8_t **out, int out_count, const uint8_t **in, int in_count);
```

- `s`：要进行音频重采样的 SwrContext 结构体；
- `out`：输出音频数据的缓冲区地址；(重采样后的数据)
- `out_count`：每个通道可用于输出的可用空间的数量；（输出数据中每个通道的采样个数）
- `in`：输入音频数据的缓冲区地址；（需要进行重采样的数据）
- `in_count`：一个通道中可用的输入样本数量。（输入数据中每个通道的采样个数）
- 返回值为每个通道输出的样本数量，如果返回值为负数，则表示转换失败





### `swr_init`

​		用于初始化 `SwrContext` 结构体的函数。

​		调用 `swr_init` 函数后，`SwrContext` 中的各种参数会根据之前通过 `swr_alloc_set_opts` 函数设置的输入输出参数进行初始化，包括音频采样格式、样本率、声道布局等参数。初始化后即可调用 `swr_convert` 函数进行音频重采样。

```c
int swr_init(SwrContext *s);
```

- `s` 参数为要进行初始化的 `SwrContext` 结构体
- 返回值为初始化结果，成功返回 `0`，失败返回负数



### `sws_getContext`

​		初始化一个`SwsContext`结构体。它是一个图像缩放和格式转换的上下文。

​		一旦上下文被初始化，它就可以用于执行各种图像操作：

- 使用`sws_scale`函数将图像帧从源格式缩放到目标格式（以进行格式转换和大小调整）
- 使用`sws_getCachedContext`函数从上下文缓存中获取更快的转换速度

```c
struct SwsContext *sws_getContext(int srcW, 
                                  int srcH, 
                                  enum AVPixelFormat srcFormat,
                                  int dstW, 
                                  int dstH, 
                                  enum AVPixelFormat dstFormat,
                                  int flags, 
                                  SwsFilter *srcFilter, 
                                  SwsFilter *dstFilter, 
                                  const double *param);
```

- `srcW`和`srcH`：源图像的宽度和高度；
- `srcFormat`：源图像的像素格式；
- `dstW`和`dstH`：目标图像的宽度和高度；
- `dstFormat`：目标图像的像素格式；
- `flags`：选项标志（可以为0）；
- `srcFilter`和`dstFilter`：源图像和目标图像的过滤器；
- `param`：可选参数
- 返回值是一个指向SwsContext结构体的指针，该结构体包含用于图像转换的所有必要信息



### `sws_scale`

​		用于图像缩放和格式转换的函数。它可以将一个输入图像按照指定的参数进行缩放和格式转换，输出一个新的图像。

​		会对输入图像指定的区域进行缩放和色彩空间转换，生成一个新的图像。具体的缩放和转换方式由上下文决定，通常通过设定特定的参数来实现，例如指定目标图像的大小、目标图像的色彩空间等等。

```c
int sws_scale(SwsContext *c, const uint8_t *const srcSlice[], const int srcStride[], int srcSliceY, int srcSliceH,
              uint8_t *const dst[], const int dstStride[]);
```

- `SwsContext *c`：缩放和转换的上下文，由sws_getContext函数创建
- `const uint8_t *const srcSlice[]`：输入图像中所有行的指针的数组。每个元素是指向一行数据的指针
- `const int srcStride[]`：输入图像中每行数据的字节数，是一个数组，与srcSlice中的元素一一对应
- `int srcSliceY`：输入图像的起始行数
- `int srcSliceH`：输入图像的高度，从起始行开始计算
- `uint8_t *const dst[]`：输出缩放后图像中所有行的指针的数组，每个元素是指向一行数据的指针
- `const int dstStride[]`：输出缩放后图像中每行数据的字节数，是一个数组，与dst中的元素一一对应
- 返回值为输出的图像行数，如果发生错误则返回负值。

```c
#include <libswscale/swscale.h>
// 将一张BGR格式的1920x1080的图像缩放为YUV420格式的1280x720的图像。
// 需要注意的是，输入和输出图像的参数需要与创建上下文时设定的参数一致，否则可能无法正常转换并输出图像。
// 另外，如果需要将输出的图像保存为文件，还需要使用特定方式进行编码和写入
int main()
{
    // 输入图像的参数
    int srcWidth = 1920;
    int srcHeight = 1080;
    int srcChannels = 3;
    uint8_t *srcData = // 输入图像数据
    int srcStride[] = {srcWidth * srcChannels}; // 每行数据的字节数

    // 输出图像的参数
    int dstWidth = 1280;
    int dstHeight = 720;
    int dstChannels = 2; // 输出图像的色彩空间为YUV420格式
    int dstStride[] = {dstWidth * dstChannels}; // 每行数据的字节数
    uint8_t *dstData = (uint8_t*)malloc(dstStride[0] * dstHeight); // 分配输出图像数据的空间

    // 创建sws_scale上下文
    SwsContext *swsContext = sws_getContext(srcWidth, srcHeight, AV_PIX_FMT_BGR24,
                                            dstWidth, dstHeight, AV_PIX_FMT_YUV420P,
                                            SWS_BICUBIC, NULL, NULL, NULL);

    // 进行缩放和格式转换
    sws_scale(swsContext, &srcData, srcStride, 0, srcHeight, &dstData, dstStride);

    // 释放空间，并关闭上下文
    free(dstData);
    sws_freeContext(swsContext);
  
    return 0;
}
```







## 组件注册

### ffmpeg 3.x

- 执行av_register_all，把全局的解码器、编码器等结构体注册到各自全局的对象链表里，以便后面查找调用。

![img](https://developer.qcloudimg.com/http-save/yehe-7620466/3ce9fafec0380072101a0f0e7f41b8ad.png)



### ffmpeg 4.x

​		FFmpeg内部去做，不需要用户调用API去注册。以`codec`编解码器为例：

1. 在`configure`的时候生成要注册的组件`./configure:7203:print_enabled_components libavcodec/codec_list.c AVCodec codec_list $CODEC_LIST`这里会生成一个`codec_list.c `文件，里面只有`static const AVCodec * const codec_list[]`数组。
2. 在`libavcodec/allcodecs.c`将`static const AVCodec * const codec_list[]`的编解码器用链表的方式组织起来。

#### ffmpeg 4.0.2

​		FFmepg内部去做，不需要用户调用API去注册

​		对于demuxer/muxer（解复用器，也称容器）则对应

- `libavformat/muxer_list.c` `libavformat/demuxer_list.c` 这两个文件也是在`configure`的时候生成，也就是说直接下载源码是没有这两个文件的。
- 在`libavformat/allformats.c`将`demuxer_list[]`和`muexr_list[]`以链表的方式组织





## 流程

### 解码流程

​		ffmpeg4.x

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-47142db7477a14628a39cd46b5526e84_1440w.webp)

## player framwork

![img](https://developer.qcloudimg.com/http-save/yehe-7620466/4fbd7c6180f0c68f7aa5dd604c4b04d0.png)
>>>>>>> Stashed changes:lib/ffmpeg/ffmpeg-lib.md



## config

### VS

​	在vs上配置ffmpeg库除了一般的库配置之外，需要以`c`的方式添加头文件。

```c++
#ifdef _WIN32

//Windows
extern "C"
{
#include <ffmpeg/libavutil/avutil.h>
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
	#include <ffmpeg/libavutil/avutil.h>
#ifdef __cplusplus
};

#endif
#endif
```

- 同时在Windows的项目配置中，链接配置的命令行中添加`/SAFESEH:NO `.

