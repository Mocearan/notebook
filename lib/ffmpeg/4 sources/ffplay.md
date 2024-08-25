# ffplay

---



## `options`

```c
// ffplay.c
static const OptionDef options[] = {
    CMDUTILS_COMMON_OPTIONS 
    { "x",                  OPT_TYPE_FUNC, OPT_FUNC_ARG, { .func_arg = opt_width }, "force displayed width", "width" },
    { "y",                  OPT_TYPE_FUNC, OPT_FUNC_ARG, { .func_arg = opt_height }, "force displayed height", "height" },
    { "fs",                 OPT_TYPE_BOOL,            0, { &is_full_screen }, "force full screen" },
    { "an",                 OPT_TYPE_BOOL,            0, { &audio_disable }, "disable audio" },
    { "vn",                 OPT_TYPE_BOOL,            0, { &video_disable }, "disable video" },
    { "sn",                 OPT_TYPE_BOOL,            0, { &subtitle_disable }, "disable subtitling" },
    { "ast",                OPT_TYPE_STRING, OPT_EXPERT, { &wanted_stream_spec[AVMEDIA_TYPE_AUDIO] }, "select desired audio stream", "stream_specifier" },
    { "vst",                OPT_TYPE_STRING, OPT_EXPERT, { &wanted_stream_spec[AVMEDIA_TYPE_VIDEO] }, "select desired video stream", "stream_specifier" },
    { "sst",                OPT_TYPE_STRING, OPT_EXPERT, { &wanted_stream_spec[AVMEDIA_TYPE_SUBTITLE] }, "select desired subtitle stream", "stream_specifier" },
    { "ss",                 OPT_TYPE_TIME,            0, { &start_time }, "seek to a given position in seconds", "pos" },
    { "t",                  OPT_TYPE_TIME,            0, { &duration }, "play  \"duration\" seconds of audio/video", "duration" },
    { "bytes",              OPT_TYPE_INT,             0, { &seek_by_bytes }, "seek by bytes 0=off 1=on -1=auto", "val" },
    { "seek_interval",      OPT_TYPE_FLOAT,           0, { &seek_interval }, "set seek interval for left/right keys, in seconds", "seconds" },
    { "nodisp",             OPT_TYPE_BOOL,            0, { &display_disable }, "disable graphical display" },
    { "noborder",           OPT_TYPE_BOOL,            0, { &borderless }, "borderless window" },
    { "alwaysontop",        OPT_TYPE_BOOL,            0, { &alwaysontop }, "window always on top" },
    { "volume",             OPT_TYPE_INT,             0, { &startup_volume}, "set startup volume 0=min 100=max", "volume" },
    { "f",                  OPT_TYPE_FUNC, OPT_FUNC_ARG, { .func_arg = opt_format }, "force format", "fmt" },
    { "stats",              OPT_TYPE_BOOL,   OPT_EXPERT, { &show_status }, "show status", "" },
    { "fast",               OPT_TYPE_BOOL,   OPT_EXPERT, { &fast }, "non spec compliant optimizations", "" },
    { "genpts",             OPT_TYPE_BOOL,   OPT_EXPERT, { &genpts }, "generate pts", "" },
    { "drp",                OPT_TYPE_INT,    OPT_EXPERT, { &decoder_reorder_pts }, "let decoder reorder pts 0=off 1=on -1=auto", ""},
    { "lowres",             OPT_TYPE_INT,    OPT_EXPERT, { &lowres }, "", "" },
    { "sync",               OPT_TYPE_FUNC, OPT_FUNC_ARG | OPT_EXPERT, { .func_arg = opt_sync }, "set audio-video sync. type (type=audio/video/ext)", "type" },
    { "autoexit",           OPT_TYPE_BOOL,   OPT_EXPERT, { &autoexit }, "exit at the end", "" },
    { "exitonkeydown",      OPT_TYPE_BOOL,   OPT_EXPERT, { &exit_on_keydown }, "exit on key down", "" },
    { "exitonmousedown",    OPT_TYPE_BOOL,   OPT_EXPERT, { &exit_on_mousedown }, "exit on mouse down", "" },
    { "loop",               OPT_TYPE_INT,    OPT_EXPERT, { &loop }, "set number of times the playback shall be looped", "loop count" },
    { "framedrop",          OPT_TYPE_BOOL,   OPT_EXPERT, { &framedrop }, "drop frames when cpu is too slow", "" },
    { "infbuf",             OPT_TYPE_BOOL,   OPT_EXPERT, { &infinite_buffer }, "don't limit the input buffer size (useful with realtime streams)", "" },
    { "window_title",       OPT_TYPE_STRING,          0, { &window_title }, "set window title", "window title" },
    { "left",               OPT_TYPE_INT,    OPT_EXPERT, { &screen_left }, "set the x position for the left of the window", "x pos" },
    { "top",                OPT_TYPE_INT,    OPT_EXPERT, { &screen_top }, "set the y position for the top of the window", "y pos" },
    { "vf",                 OPT_TYPE_FUNC, OPT_FUNC_ARG | OPT_EXPERT, { .func_arg = opt_add_vfilter }, "set video filters", "filter_graph" },
    { "af",                 OPT_TYPE_STRING,          0, { &afilters }, "set audio filters", "filter_graph" },
    { "rdftspeed",          OPT_TYPE_INT, OPT_AUDIO | OPT_EXPERT, { &rdftspeed }, "rdft speed", "msecs" },
    { "showmode",           OPT_TYPE_FUNC, OPT_FUNC_ARG, { .func_arg = opt_show_mode}, "select show mode (0 = video, 1 = waves, 2 = RDFT)", "mode" },
    { "i",                  OPT_TYPE_BOOL,            0, { &dummy}, "read specified file", "input_file"},
    { "codec",              OPT_TYPE_FUNC, OPT_FUNC_ARG, { .func_arg = opt_codec}, "force decoder", "decoder_name" },
    { "acodec",             OPT_TYPE_STRING, OPT_EXPERT, {    &audio_codec_name }, "force audio decoder",    "decoder_name" },
    { "scodec",             OPT_TYPE_STRING, OPT_EXPERT, { &subtitle_codec_name }, "force subtitle decoder", "decoder_name" },
    { "vcodec",             OPT_TYPE_STRING, OPT_EXPERT, {    &video_codec_name }, "force video decoder",    "decoder_name" },
    { "autorotate",         OPT_TYPE_BOOL,            0, { &autorotate }, "automatically rotate video", "" },
    { "find_stream_info",   OPT_TYPE_BOOL, OPT_INPUT | OPT_EXPERT, { &find_stream_info },
        "read and decode the streams to fill missing information with heuristics" },
    { "filter_threads",     OPT_TYPE_INT,    OPT_EXPERT, { &filter_nbthreads }, "number of filter threads per graph" },
    { "enable_vulkan",      OPT_TYPE_BOOL,            0, { &enable_vulkan }, "enable vulkan renderer" },
    { "vulkan_params",      OPT_TYPE_STRING, OPT_EXPERT, { &vulkan_params }, "vulkan configuration using a list of key=value pairs separated by ':'" },
    { "hwaccel",            OPT_TYPE_STRING, OPT_EXPERT, { &hwaccel }, "use HW accelerated decoding" },
    { NULL, },
};
```





## `main`

![image-20240824161353382](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240824161353382.png)

```c
/* Called from the main */
int main(int argc, char **argv)
{
    int flags, ret;
    VideoState *is;

    //* 初始化动态库
    init_dynload();

    //* 日志设置
    av_log_set_flags(AV_LOG_SKIP_REPEATED);
    parse_loglevel(argc, argv, options);

    /* register all codecs, demux and protocols */
    //* CONFIG_AVDEVICE 是 FFmpeg 项目中的一个预处理宏，用于启用设备支持（即输入/输出设备的支持功能）。
    //* 比如访问摄像头或麦克风等输入/输出设备，你需要在编译时通过 --enable-avdevice 选项来定义 CONFIG_AVDEVICE
    //*     编译 FFmpeg 时，. / configure --enable-avdevice
    //* 一些特定平台可能不支持设备功能，因此在这些平台上，CONFIG_AVDEVICE 可能不会被定义。
    //*     对于嵌入式系统或某些精简版的 FFmpeg 构建，可能会关闭设备支持，以减小体积和复杂性。
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avformat_network_init();

    //* ctrl+c ctrl+z 直接退出
    signal(SIGINT, sigterm_handler); /* Interrupt (ANSI).    */
    signal(SIGTERM, sigterm_handler); /* Termination (ANSI).  */

    //* 打印输出FFmpeg版本信息（编译时间，编译选项，类库信息等）。
    //* 就是ffplay播放时，媒体信息上面那一坨配置和库版本信息
    show_banner(argc, argv, options);

    //* 解析输入的选项
    ret = parse_options(NULL, argc, argv, options, opt_input_file);
    if (ret < 0)
        exit(ret == AVERROR_EXIT ? 0 : 1);

    //* 没有提供输入源，则打印使用方法
    if (!input_filename) {
        show_usage();
        av_log(NULL, AV_LOG_FATAL, "An input file must be specified\n");
        av_log(NULL, AV_LOG_FATAL,
               "Use -h to get full help or, even better, run 'man %s'\n", program_name);
        exit(1);
    }

    //* nodisp -> vn
    if ( display_disable ) {
        video_disable = 1;
    }
    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    //* an
    if (audio_disable)
        flags &= ~SDL_INIT_AUDIO;
    else {
        /* Try to work around an occasional ALSA buffer underflow issue when the
         * period size is NPOT due to ALSA resampling by forcing the buffer size. */
        //* 检查环境变量 SDL_AUDIO_ALSA_SET_BUFFER_SIZE 是否已设置。
        //* 处理 ALSA（Advanced Linux Sound Architecture）在缓冲区下溢时的一个潜在问题，特别是在使用非二次幂（NPOT）大小的缓冲区时。
        if ( !SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE") )
            SDL_setenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE","1", 1);
    }
    //* nodisp
    if ( display_disable )
        flags &= ~SDL_INIT_VIDEO;

    //* 初始化SDL
    if ( SDL_Init(flags) ) {
        av_log(NULL, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        av_log(NULL, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
        exit(1);
    }

    //* 禁用系统窗口事件和用户时间
    //* 可能是出于性能考虑，或者是因为程序在当前上下文中不需要处理这些事件。
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    //* 创建一个SDL窗口，并根据不同的条件设置窗口的属性和渲染器。
    if ( !display_disable ) {
        //* 窗口初始化时是隐藏的。
        int flags = SDL_WINDOW_HIDDEN;

        //* SDL 2.0.5以上窗口置顶
        if ( alwaysontop )
#if SDL_VERSION_ATLEAST(2,0,5)
            flags |= SDL_WINDOW_ALWAYS_ON_TOP;
#else
            av_log(NULL, AV_LOG_WARNING, "Your SDL version doesn't support SDL_WINDOW_ALWAYS_ON_TOP. Feature will be inactive.\n");
#endif
        
        //* 无边框设置
        if ( borderless )
            flags |= SDL_WINDOW_BORDERLESS;
        else
            flags |= SDL_WINDOW_RESIZABLE;

#ifdef SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR
        SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

        //* 硬件加速和vulkan配置
        //* 设置了硬件加速但没有手动开启vulkan，则自动开启vulkan渲染
        if ( hwaccel && !enable_vulkan ) {
            av_log(NULL, AV_LOG_INFO, "Enable vulkan renderer to support hwaccel %s\n", hwaccel);
            enable_vulkan = 1;
        }
        //* 如果开启vulkan，尝试获取vulkan渲染器
        if ( enable_vulkan ) {
            vk_renderer = vk_get_renderer();
            if ( vk_renderer ) {
                //* 获取成功且SDL 2.0.6 以上，SDL设置vulkan渲染
#if SDL_VERSION_ATLEAST(2, 0, 6)
                flags |= SDL_WINDOW_VULKAN;
#endif
            } else {
                av_log(NULL, AV_LOG_WARNING, "Doesn't support vulkan renderer, fallback to SDL renderer\n");
                enable_vulkan = 0;
            }
        }

        //* 通过前述配置创建SDL窗口
        window = SDL_CreateWindow(program_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, default_width, default_height, flags);
        //* 设置渲染的缩放质量为线性
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        if (!window) {
            av_log(NULL, AV_LOG_FATAL, "Failed to create window: %s", SDL_GetError());
            do_exit(NULL);
        }

        //* vulkan渲染器设置
        if ( vk_renderer ) {
            //* 解析vukkan参数
            AVDictionary *dict = NULL;

            if (vulkan_params) {
                int ret = av_dict_parse_string(&dict, vulkan_params, "=", ":", 0);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_FATAL, "Failed to parse, %s\n", vulkan_params);
                    do_exit(NULL);
                }
            }
            //* 创建vulkan渲染器
            ret = vk_renderer_create(vk_renderer, window, dict);
            av_dict_free(&dict);
            if (ret < 0) {
                av_log(NULL, AV_LOG_FATAL, "Failed to create vulkan renderer, %s\n", av_err2str(ret));
                do_exit(NULL);
            }
        } else {
            //* 没有vulkan渲染器，则由SDL根据系统创建一个使用硬件加速和垂直同步的渲染器
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if ( !renderer ) {
                //* 如果创建失败，则尝试创建无硬件加速的渲染器
                av_log(NULL, AV_LOG_WARNING, "Failed to initialize a hardware accelerated renderer: %s\n", SDL_GetError());
                renderer = SDL_CreateRenderer(window, -1, 0);
            }
            //* 获取渲染器信息，确保渲染器正确初始化
            if ( renderer ) {
                if (!SDL_GetRendererInfo(renderer, &renderer_info))
                    av_log(NULL, AV_LOG_VERBOSE, "Initialized %s renderer.\n", renderer_info.name);
            }
            //* 渲染器信息获取失败
            if ( !renderer || !renderer_info.num_texture_formats ) {
                av_log(NULL, AV_LOG_FATAL, "Failed to create window or renderer: %s", SDL_GetError());
                do_exit(NULL);
            }
        }
    }

    //* 打开视频流
    is = stream_open(input_filename, file_iformat);
    if (!is) {
        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
    }
    
    //* 开启事件循环处理播放逻辑、用户交互
    event_loop(is);

    /* never returns */

    return 0;
}
```



## `stream_open`

```c
//* 为媒体播放器或相关应用程序设置和配置视频播放环境。
//* 返回一个指向 VideoState 结构的指针，如果打开失败则返回 NULL。
static VideoState *stream_open(const char *filename,
                               const AVInputFormat *iformat)
{
    VideoState *is;

    //*  分配内存并初始化为零
    is = av_mallocz(sizeof(VideoState));
    if (!is)
        return NULL;
    //* 初始化流索引
    is->last_video_stream = is->video_stream = -1;
    is->last_audio_stream = is->audio_stream = -1;
    is->last_subtitle_stream = is->subtitle_stream = -1;
    //* 复制传入的文件名，如果失败则进入失败处理
    is->filename = av_strdup(filename);
    if (!is->filename)
        goto fail;
    //* 初始化格式信息和左上顶点
    is->iformat = iformat;
    is->ytop    = 0;
    is->xleft   = 0;

    /* start video display */
    //* 初始化视频、字幕和音频的帧队列，创建时传入各个队列的大小等参数。如果任何一个初始化失败，则处理失败。
    if ( frame_queue_init(&is->pictq, &is->videoq, VIDEO_PICTURE_QUEUE_SIZE, 1) < 0 )
        goto fail;
    if (frame_queue_init(&is->subpq, &is->subtitleq, SUBPICTURE_QUEUE_SIZE, 0) < 0)
        goto fail;
    if (frame_queue_init(&is->sampq, &is->audioq, SAMPLE_QUEUE_SIZE, 1) < 0)
        goto fail;
    //* 初始化视频、音频和字幕的包队列。如果失败，处理失败。
    if (packet_queue_init(&is->videoq) < 0 ||
        packet_queue_init(&is->audioq) < 0 ||
        packet_queue_init(&is->subtitleq) < 0)
        goto fail;

    //* 创建一个条件变量，用于read线程和事件线程（主线程）间的同步
    if ( !(is->continue_read_thread = SDL_CreateCond()) ) {
        av_log(NULL, AV_LOG_FATAL, "SDL_CreateCond(): %s\n", SDL_GetError());
        goto fail;
    }

    //* 初始化视频、音频和外部时钟。
    init_clock(&is->vidclk, &is->videoq.serial);
    init_clock(&is->audclk, &is->audioq.serial);
    init_clock(&is->extclk, &is->extclk.serial);
    is->audio_clock_serial = -1;

    //* 设置初始音量，并处理超出范围的情况，确保音量在合法范围内（0-100）。
    if ( startup_volume < 0 )
        av_log(NULL, AV_LOG_WARNING, "-volume=%d < 0, setting to 0\n", startup_volume);
    if (startup_volume > 100)
        av_log(NULL, AV_LOG_WARNING, "-volume=%d > 100, setting to 100\n", startup_volume);
    startup_volume = av_clip(startup_volume, 0, 100);
    startup_volume = av_clip(SDL_MIX_MAXVOLUME * startup_volume / 100, 0, SDL_MIX_MAXVOLUME);
    is->audio_volume = startup_volume;
    is->muted = 0;
    //* 设置同步类型
    is->av_sync_type = av_sync_type;
    //* 创建read线程
    is->read_tid = SDL_CreateThread(read_thread, "read_thread", is);
    if (!is->read_tid) {
        av_log(NULL, AV_LOG_FATAL, "SDL_CreateThread(): %s\n", SDL_GetError());

    //* 错误处理，关闭输入流，返回NULL
fail:
        stream_close(is);
        return NULL;
    }
    //* 成功返回输入流
    return is;
}
```



## `stream_close`

```c
//* 清理和释放与视频流相关的资源
static void stream_close(VideoState *is)
{
    /* XXX: use a special url_shutdown call to abort parse cleanly */
    //* 中断read线程
    is->abort_request = 1;
    //* 等待read线程退出。确保在继续清理其他资源之前，read线程已经安全退出。
    SDL_WaitThread(is->read_tid, NULL);

    /* close each stream */
    //* 逐个关闭音频、视频和字幕流
    if ( is->audio_stream >= 0 )
        stream_component_close(is, is->audio_stream);
    if (is->video_stream >= 0)
        stream_component_close(is, is->video_stream);
    if (is->subtitle_stream >= 0)
        stream_component_close(is, is->subtitle_stream);

    //* 关闭输入格式上下文 is->ic
    avformat_close_input(&is->ic);

    //* 销毁视频、音频和字幕的包队列
    packet_queue_destroy(&is->videoq);
    packet_queue_destroy(&is->audioq);
    packet_queue_destroy(&is->subtitleq);

    /* free all pictures */
    //* 来释放图像、音频样本和字幕的帧队列
    frame_queue_destroy(&is->pictq);
    frame_queue_destroy(&is->sampq);
    frame_queue_destroy(&is->subpq);

    //* 销毁用于read线程控制的条件变量，确保不再有挂起的线程等待这个条件。
    SDL_DestroyCond(is->continue_read_thread);

    //* 释放转换上下文
    sws_freeContext(is->sub_convert_ctx);

    //* 释放输入文件
    av_free(is->filename);

    //* 销毁视频、可视化和字幕相关的纹理图形资源
    if ( is->vis_texture )
        SDL_DestroyTexture(is->vis_texture);
    if (is->vid_texture)
        SDL_DestroyTexture(is->vid_texture);
    if (is->sub_texture)
        SDL_DestroyTexture(is->sub_texture);

    //* 释放 VideoState 输入流结构体本身，完成资源清理
    av_free(is);
}
```



## `read_thread`

```c
/* this thread gets the stream from the disk or the network */
//* 多线程的入口回调函数，负责从磁盘或网络读取视频流。
//* 它的主要任务是打开媒体文件，读取流信息，并将数据包放入相应的队列中供其他线程使用。
static int read_thread(void *arg)
{
    VideoState *is = arg;
    AVFormatContext *ic = NULL;     //* 解码上下文
    int err, i, ret;
    int st_index[AVMEDIA_TYPE_NB];  //* 存储不同媒体类型流的索引
    AVPacket *pkt = NULL;           //* 存储读取到的数据包
    int64_t stream_start_time;      //* 流开始的时间戳
    int pkt_in_play_range = 0;      //* 流是否在用户指定的时间范围内
    const AVDictionaryEntry *t;

    //* 是否扫描所有节目
    int scan_all_pmts_set = 0;
    //* 包timestamp
    int64_t pkt_ts;

    //* 创建一个互斥锁用于线程同步。如果创建失败，处理失败。
    SDL_mutex *wait_mutex = SDL_CreateMutex();
    //* 互斥锁创建失败，处理失败
    if ( !wait_mutex ) {
        av_log(NULL, AV_LOG_FATAL, "SDL_CreateMutex(): %s\n", SDL_GetError());
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    //* 初始化 st_index 数组为 -1，表示尚未找到任何流
    memset(st_index, -1, sizeof(st_index));
    //* is->eof 设置为 0，表示未到达文件结束。
    is->eof = 0;

    //* 分配一个 AVPacket 数据包。如果分配失败，记录错误并处理失败。
    pkt = av_packet_alloc();
    if (!pkt) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate packet.\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    //* 创建解码上下文.如果分配失败，记录错误并处理失败。
    ic = avformat_alloc_context();
    if ( !ic ) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    //* 设置解码中断回调,用于在读取过程中可以中断操作。
    ic->interrupt_callback.callback = decode_interrupt_cb;
    ic->interrupt_callback.opaque = is;
    //* 设置格式选项，是否扫描所有节目
    if (!av_dict_get(format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
        av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
        scan_all_pmts_set = 1;
    }
    //* 打开指定的文件或网络地址，获取流信息
    err = avformat_open_input(&ic, is->filename, is->iformat, &format_opts);
    if ( err < 0 ) {
        //* 如果打开失败，调用 print_error 函数输出错误信息并退出。
        print_error(is->filename, err);
        ret = -1;
        goto fail;
    }
    //* 如果之前设置了 scan_all_pmts 选项，则清除该选项。
    if ( scan_all_pmts_set )
        av_dict_set(&format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);
    //* 检查是否有未识别的格式选项，如果有则记录错误并处理失败。
    if ((t = av_dict_get(format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
        ret = AVERROR_OPTION_NOT_FOUND;
        goto fail;
    }
    //* 将解码上下文指针保存到 VideoState 结构中。
    is->ic = ic;

    //* 如果需要生成时间戳，则设置相应的标志。
    if ( genpts )
        ic->flags |= AVFMT_FLAG_GENPTS;

    //* 获取媒体流流的编解码参数。如果失败，记录警告并处理失败。
    if ( find_stream_info ) {
        //* 媒体流信息的选项指针数组，每个媒体流对应一个AVDictionary*
        AVDictionary **opts;
        int orig_nb_streams = ic->nb_streams;

        //* 设置查找时所用的各个媒体流信息的选项
        err = setup_find_stream_info_opts(ic, codec_opts, &opts);
        if (err < 0) {
            av_log(NULL, AV_LOG_ERROR,
                   "Error setting up avformat_find_stream_info() options\n");
            ret = err;
            goto fail;
        }

        //* 获取每个媒体流的信息设置到ic中
        err = avformat_find_stream_info(ic, opts);
        //* 释放媒体流选项指针数组
        for ( i = 0; i < orig_nb_streams; i++ )
            av_dict_free(&opts[i]);
        av_freep(&opts);

        //* 检查并记录流的状态，如果找不到编解码参数，记录警告并退出
        if ( err < 0 ) {
            av_log(NULL, AV_LOG_WARNING,
                   "%s: could not find codec parameters\n", is->filename);
            ret = -1;
            goto fail;
        }
    }


    if ( ic->pb )
        //*  eof_reached 设置为 0，来指示流读取未结束，以便后续读取能够正确进行。
        ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end

    //* 设置seek的模式，是否按字节seek
    //* 这对于不支持字节寻址的格式（如 OGG）是必要的。
    if ( seek_by_bytes < 0 )
        seek_by_bytes = !(ic->iformat->flags & AVFMT_NO_BYTE_SEEK) &&
                        !!(ic->iformat->flags & AVFMT_TS_DISCONT) &&
                        strcmp("ogg", ic->iformat->name);

    //* 初始化最大帧持续时间，避免跳帧
    //* 当时间戳 discontinuity (断点) 存在时，最大帧持续时间设置为 10 毫秒，否则为3600毫秒。
    is->max_frame_duration = (ic->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

    //* 设置窗口标题,尝试从文件的元数据中获取标题并格式化为 “标题 - 文件名”。
    if ( !window_title && (t = av_dict_get(ic->metadata, "title", NULL, 0)) )
        window_title = av_asprintf("%s - %s", t->value, input_filename);

    /* if seeking requested, we execute it */
    //* 处理用户请求的起始时间
    if ( start_time != AV_NOPTS_VALUE ) {
        int64_t timestamp;
        //* 计算起始时间戳，并尝试在打开的媒体文件中寻址
        timestamp = start_time;
        /* add the stream start time */
        if (ic->start_time != AV_NOPTS_VALUE)
            timestamp += ic->start_time;
        ret = avformat_seek_file(ic, -1, INT64_MIN, timestamp, INT64_MAX, 0);
        if (ret < 0) {
            av_log(NULL, AV_LOG_WARNING, "%s: could not seek to position %0.3f\n",
                    is->filename, (double)timestamp / AV_TIME_BASE);
        }
    }

    //* 检查是否为实时流
    is->realtime = is_realtime(ic);

    //* 如果需要显示状态，调用 av_dump_format 输出流的信息
    if ( show_status )
        av_dump_format(ic, 0, is->filename, 0);

    //* 遍历所有流
    for ( i = 0; i < ic->nb_streams; i++ ) {
        AVStream *st = ic->streams[i];
        enum AVMediaType type = st->codecpar->codec_type;
        //* 初始化流的丢弃策略，
        st->discard = AVDISCARD_ALL;
        //* 根据用户指定的流类型（如视频、音频、字幕）更新对应的 st_index。
        if ( type >= 0 && wanted_stream_spec[type] && st_index[type] == -1 )
            if (avformat_match_stream_specifier(ic, st, wanted_stream_spec[type]) > 0)
                st_index[type] = i;
    }
    
    //* 检查用户请求的流类型，如果未找到匹配的流，记录错误并将其标记为无效(INT_MAX)。
    for ( i = 0; i < AVMEDIA_TYPE_NB; i++ ) {
        if ( wanted_stream_spec[i] && st_index[i] == -1 ) {
            av_log(NULL, AV_LOG_ERROR, "Stream specifier %s does not match any %s stream\n",
                wanted_stream_spec[i], av_get_media_type_string(i));
            st_index[i] = INT_MAX;
        }
    }

    //* 根据用户的流偏好和可用流选择最佳音频、视频和字幕流。
    if ( !video_disable )
        st_index[AVMEDIA_TYPE_VIDEO] =
            av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,
                                st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);
    if (!audio_disable)
        st_index[AVMEDIA_TYPE_AUDIO] =
            av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,
                                st_index[AVMEDIA_TYPE_AUDIO],
                                st_index[AVMEDIA_TYPE_VIDEO],
                                NULL, 0);
    if (!video_disable && !subtitle_disable)
        st_index[AVMEDIA_TYPE_SUBTITLE] =
            av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
                                st_index[AVMEDIA_TYPE_SUBTITLE],
                                (st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
                                 st_index[AVMEDIA_TYPE_AUDIO] :
                                 st_index[AVMEDIA_TYPE_VIDEO]),
                                NULL, 0);

    //* 设置视频流的默认窗口大小
    //* 设置显示模式
    is->show_mode = show_mode;
    //* 获取视频流的宽高以及比例，进而计算并设定默认窗口大小。
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        AVStream *st = ic->streams[st_index[AVMEDIA_TYPE_VIDEO]];
        AVCodecParameters *codecpar = st->codecpar;
        AVRational sar = av_guess_sample_aspect_ratio(ic, st, NULL);
        if (codecpar->width)
            set_default_window_size(codecpar->width, codecpar->height, sar);
    }

    /* open the streams */
    //* 打开流组件
    //* 根据前面找到的流索引，逐个打开音频、视频和字幕流，
    if ( st_index[AVMEDIA_TYPE_AUDIO] >= 0 ) {
        stream_component_open(is, st_index[AVMEDIA_TYPE_AUDIO]);
    }
    ret = -1;
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        ret = stream_component_open(is, st_index[AVMEDIA_TYPE_VIDEO]);
    }
    //* 如果没有其他显示模式，则默认设置为视频模式（SHOW_MODE_VIDEO）或者关系图模式（SHOW_MODE_RDFT）。
    if ( is->show_mode == SHOW_MODE_NONE )
        is->show_mode = ret >= 0 ? SHOW_MODE_VIDEO : SHOW_MODE_RDFT;

    if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
        stream_component_open(is, st_index[AVMEDIA_TYPE_SUBTITLE]);
    }

    //* 检查是否成功打开了音频或视频流。如果两者都未成功，则记录致命错误消息，并跳转到清理部分。
    if ( is->video_stream < 0 && is->audio_stream < 0 ) {
        av_log(NULL, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n",
               is->filename);
        ret = -1;
        goto fail;
    }

    //* 如果设置了 infinite_buffer 为负值而且流是实时流，则将其设置为 1，以便处理实时流时使用无限缓冲。
    if ( infinite_buffer < 0 && is->realtime )
        infinite_buffer = 1;

    //* 读取数据包并处理各种状态
    for ( ;;) {
        //* 检查是否有中止请求，如果有则退出循环。
        if ( is->abort_request )
            break;
        //* 检查播放状态是否发生变化(通过对比当前的pause状态和上一次的pause状态), 处理暂停和恢复播放的请求。
        if ( is->paused != is->last_paused ) {
            is->last_paused = is->paused;
            //* 如果暂停，则调用 av_read_pause，否则调用 av_read_play 继续播放。
            if (is->paused)
                is->read_pause_return = av_read_pause(ic);
            else
                av_read_play(ic);
        }
//* RTSP 和 MMSH 协议的特殊处理
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
        //* 如果正在暂停并且是 RTSP 或 MMSH 流，等待 10 毫秒以避免频繁读取数据包。
        if ( is->paused &&
                (!strcmp(ic->iformat->name, "rtsp") ||
                 (ic->pb && !strncmp(input_filename, "mmsh:", 5)))) {
            /* wait 10 ms to avoid trying to get another packet */
            /* XXX: horrible */
            SDL_Delay(10);
            continue;
        }
#endif
        //* 处理SEEK请求，更新播放状态和清空相应的包队列。
        if ( is->seek_req ) {
            //* 计算目标时间戳并调用 avformat_seek_file 进行寻址。
            int64_t seek_target = is->seek_pos;
            int64_t seek_min = is->seek_rel > 0 ? seek_target - is->seek_rel + 2 : INT64_MIN;
            int64_t seek_max = is->seek_rel < 0 ? seek_target - is->seek_rel - 2 : INT64_MAX;
// FIXME the +-2 is due to rounding being not done in the correct direction in generation
//      of the seek_pos/seek_rel variables
            //* 寻址
            ret = avformat_seek_file(is->ic, -1, seek_min, seek_target, seek_max, is->seek_flags);
            if ( ret < 0 ) {
                av_log(NULL, AV_LOG_ERROR,
                       "%s: error while seeking\n", is->ic->url);
            } else {
                //* 如果寻址成功，清空音频、视频和字幕的包队列
                if ( is->audio_stream >= 0 )
                    packet_queue_flush(&is->audioq);
                if ( is->subtitle_stream >= 0 )
                    packet_queue_flush(&is->subtitleq);
                if ( is->video_stream >= 0 )
                    packet_queue_flush(&is->videoq);
                //* 根据寻址标志更新时钟。
                if ( is->seek_flags & AVSEEK_FLAG_BYTE ) {
                    set_clock(&is->extclk, NAN, 0);
                } else {
                    set_clock(&is->extclk, seek_target / (double) AV_TIME_BASE, 0);
                }
            }
            is->seek_req = 0;
            is->queue_attachments_req = 1;
            is->eof = 0;
            if ( is->paused )
                step_to_next_frame(is);
        }
        //* 处理附加图片相关的数据包。
        if ( is->queue_attachments_req ) {
            //* 检查是否有请求附加图片（如封面图）
            if ( is->video_st && is->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC ) {
                if ((ret = av_packet_ref(pkt, &is->video_st->attached_pic)) < 0)
                    goto fail;
                //* 如果有，则将其放入视频队列
                packet_queue_put(&is->videoq, pkt);
                packet_queue_put_nullpacket(&is->videoq, pkt, is->video_stream);
            }
            is->queue_attachments_req = 0;
        }

        /* if the queue are full, no need to read more */
        //* 检查是否需要读取更多数据包
        //* 检查队列是否已满
        //*     非无限缓冲，视频队列和音频队列以及字幕队列大小之和超过 MAX_QUEUE_SIZE，或每个队列中有足够数据包
        //* 如果队列已满则等待10毫秒让消费线程取走数据包
        if ( infinite_buffer < 1 &&
            (is->audioq.size + is->videoq.size + is->subtitleq.size > MAX_QUEUE_SIZE
                || (stream_has_enough_packets(is->audio_st, is->audio_stream, &is->audioq) &&
                    stream_has_enough_packets(is->video_st, is->video_stream, &is->videoq) &&
                    stream_has_enough_packets(is->subtitle_st, is->subtitle_stream, &is->subtitleq))) ) {
            /* wait 10 ms */
            SDL_LockMutex(wait_mutex);
            SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
            SDL_UnlockMutex(wait_mutex);
            continue;
        }

        //* 处理循环播放和自动退出
        //* 确保了播放器在没有暂停的情况下，音频和视频的解码都已完成，并且没有剩余的帧需要播放。
        if ( !is->paused &&
            (!is->audio_st || (is->auddec.finished == is->audioq.serial && frame_queue_nb_remaining(&is->sampq) == 0)) &&
            (!is->video_st || (is->viddec.finished == is->videoq.serial && frame_queue_nb_remaining(&is->pictq) == 0)) ) {
            //* 循环计数决定是否重新开始播放
            if ( loop != 1 && (!loop || --loop) ) {
                //* 回到指定的开始时间（start_time），如果 start_time 是无效值（AV_NOPTS_VALUE），则回到 0。
                stream_seek(is, start_time != AV_NOPTS_VALUE ? start_time : 0, 0, 0);
            } else if (autoexit) { //* 如果设置了自动退出，则在播放结束后返回错误并进行清理。
                ret = AVERROR_EOF; //* 置返回值为 AVERROR_EOF，表示已到达文件结束。
                goto fail;
            }
        }
        
        //* 从输入流中读取数据包。
        ret = av_read_frame(ic, pkt);
        //* 返回值小于 0，表示读取出错：
        if ( ret < 0 ) {
            //* 处理 EOF（文件结束）情况，向队列中添加空包以指示流已结束。
            if ( (ret == AVERROR_EOF || avio_feof(ic->pb)) && !is->eof ) {
                if (is->video_stream >= 0)
                    packet_queue_put_nullpacket(&is->videoq, pkt, is->video_stream);
                if (is->audio_stream >= 0)
                    packet_queue_put_nullpacket(&is->audioq, pkt, is->audio_stream);
                if (is->subtitle_stream >= 0)
                    packet_queue_put_nullpacket(&is->subtitleq, pkt, is->subtitle_stream);
                //* 更新 eof 标识。
                is->eof = 1;
            }
            if ( ic->pb && ic->pb->error ) {
                //* 如果发生读取错误且 autoexit 被设置，则跳转到清理并退出的部分。
                if (autoexit)
                    goto fail;
                else
                    break;
            }
            SDL_LockMutex(wait_mutex);
            SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
            SDL_UnlockMutex(wait_mutex);
            continue;
        } else {
            //* 如果没有错误，恢复 is->eof 标识为 0。
            is->eof = 0;
        }
        
        /* check if packet is in play range specified by user, then queue, otherwise discard */
        //* 检查数据包是否在用户指定的播放范围内
        //* 计算数据包的播放时间戳和流的开始时间
        stream_start_time = ic->streams[pkt->stream_index]->start_time;
        pkt_ts = pkt->pts == AV_NOPTS_VALUE ? pkt->dts : pkt->pts;
        //* 后判断该数据包是否在用户指定的播放范围内
        pkt_in_play_range = duration == AV_NOPTS_VALUE ||
                (pkt_ts - (stream_start_time != AV_NOPTS_VALUE ? stream_start_time : 0)) *
                av_q2d(ic->streams[pkt->stream_index]->time_base) -
                (double)(start_time != AV_NOPTS_VALUE ? start_time : 0) / 1000000
                <= ((double)duration / 1000000);
        //* 如果数据包在播放范围内, 根据数据包的流索引，将其放入相应的队列。
        if (pkt->stream_index == is->audio_stream && pkt_in_play_range) {
            packet_queue_put(&is->audioq, pkt);
        } else if (pkt->stream_index == is->video_stream && pkt_in_play_range
                   && !(is->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
            packet_queue_put(&is->videoq, pkt);
        } else if (pkt->stream_index == is->subtitle_stream && pkt_in_play_range) {
            packet_queue_put(&is->subtitleq, pkt);
        } else {
            //* 如果数据包不在播放范围内，调用 av_packet_unref 释放该数据包。
            av_packet_unref(pkt);
        }
    }

    ret = 0;
    
fail:
    //* 如果发生错误，则发送退出事件以终止程序。
    
    //* 关闭输入流
    if ( ic && !is->ic )
        avformat_close_input(&ic);
    //* 释放数据包指针
    av_packet_free(&pkt);
    //* 如果返回值不是 0（表示发生错误），则生成退出事件，通知 SDL 退出事件处理循环。
    if (ret != 0) {
        SDL_Event event;

        event.type = FF_QUIT_EVENT;
        event.user.data1 = is;
        SDL_PushEvent(&event);
    }
    //* 销毁互斥锁
    SDL_DestroyMutex(wait_mutex);
    return 0;
}
```



## `event_loop`

```c
//* 利用 SDL（Simple DirectMedia Layer）来处理与视频播放相关的事件。主要目的是管理用户通过键盘和鼠标输入进行的交互，从而实现视频播放的各种控制。
static void event_loop(VideoState *cur_stream)
{
    SDL_Event event;
    double incr, pos, frac;

    //* loop
    for ( ;;) {
        double x;
        //* 阻塞当前线程，直到有新的事件发生。
        refresh_loop_wait_event(cur_stream, &event);
        //* event handle
        switch ( event.type ) {
        //* 键盘输入处理
        case SDL_KEYDOWN:
            //* 按 ESC 键、q 键或者通过预定义的退出变量 （exit_on_keydown）来退出应用程序
            if ( exit_on_keydown || event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q ) {
                do_exit(cur_stream);
                break;
            }
            // If we don't yet have a window, skip all key events, because read_thread might still be initializing...
            //* 没有窗口就忽略所有键盘事件
            if ( !cur_stream->width )
                continue;
            //* 键盘事件
            switch ( event.key.keysym.sym ) {
            //* f 键可以在全屏和窗口模式之间切换
            case SDLK_f:
                toggle_full_screen(cur_stream);
                cur_stream->force_refresh = 1;
                break;
            //* 按 p 或 SPACE 键可以切换暂停/播放状态
            case SDLK_p:
            case SDLK_SPACE:
                toggle_pause(cur_stream);
                break;
            //* 按 m 键可以静音或取消静音。
            case SDLK_m:
                toggle_mute(cur_stream);
                break;
            //* KP_MULTIPLY（小键盘的乘号）或 0 键增加音量。
            case SDLK_KP_MULTIPLY:
            case SDLK_0:
                update_volume(cur_stream, 1, SDL_VOLUME_STEP);
                break;
            //* KP_DIVIDE（小键盘的除号）或 9 键降低音量
            case SDLK_KP_DIVIDE:
            case SDLK_9:
                update_volume(cur_stream, -1, SDL_VOLUME_STEP);
                break;
            //* 逐帧播放，按 s 键可以跳到下一帧。
            case SDLK_s: // S: Step to next frame
                step_to_next_frame(cur_stream);
                break;
            //* a、v、c、t 和 w 键可以在音频、视频和字幕频道之间切换。
            case SDLK_a:
                stream_cycle_channel(cur_stream, AVMEDIA_TYPE_AUDIO);
                break;
            case SDLK_v:
                stream_cycle_channel(cur_stream, AVMEDIA_TYPE_VIDEO);
                break;
            case SDLK_c:
                stream_cycle_channel(cur_stream, AVMEDIA_TYPE_VIDEO);
                stream_cycle_channel(cur_stream, AVMEDIA_TYPE_AUDIO);
                stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
                break;
            case SDLK_t:
                stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
                break;
            case SDLK_w:
                if (cur_stream->show_mode == SHOW_MODE_VIDEO && cur_stream->vfilter_idx < nb_vfilters - 1) {
                    if (++cur_stream->vfilter_idx >= nb_vfilters)
                        cur_stream->vfilter_idx = 0;
                } else {
                    cur_stream->vfilter_idx = 0;
                    toggle_audio_display(cur_stream);
                }
                break;
            //* 使用 PAGEUP 和 PAGEDOWN 键在可用的章节之间切换。
            case SDLK_PAGEUP:
                if (cur_stream->ic->nb_chapters <= 1) {
                    incr = 600.0;
                    goto do_seek;
                }
                seek_chapter(cur_stream, 1);
                break;
            case SDLK_PAGEDOWN:
                if (cur_stream->ic->nb_chapters <= 1) {
                    incr = -600.0;
                    goto do_seek;
                }
                seek_chapter(cur_stream, -1);
                break;
            //* 箭头键（←、→、↑、↓）可用于在视频中前后寻址。增加或减少的时间间隔由 seek_interval 指定（如果有）或使用标准值。
            case SDLK_LEFT:
                incr = seek_interval ? -seek_interval : -10.0;
                goto do_seek;
            case SDLK_RIGHT:
                incr = seek_interval ? seek_interval : 10.0;
                goto do_seek;
            case SDLK_UP:
                incr = 60.0;
                goto do_seek;
            case SDLK_DOWN:
                incr = -60.0;
            do_seek:
                    if (seek_by_bytes) {
                        pos = -1;
                        if (pos < 0 && cur_stream->video_stream >= 0)
                            pos = frame_queue_last_pos(&cur_stream->pictq);
                        if (pos < 0 && cur_stream->audio_stream >= 0)
                            pos = frame_queue_last_pos(&cur_stream->sampq);
                        if (pos < 0)
                            pos = avio_tell(cur_stream->ic->pb);
                        if (cur_stream->ic->bit_rate)
                            incr *= cur_stream->ic->bit_rate / 8.0;
                        else
                            incr *= 180000.0;
                        pos += incr;
                        stream_seek(cur_stream, pos, incr, 1);
                    } else {
                        pos = get_master_clock(cur_stream);
                        if (isnan(pos))
                            pos = (double)cur_stream->seek_pos / AV_TIME_BASE;
                        pos += incr;
                        if (cur_stream->ic->start_time != AV_NOPTS_VALUE && pos < cur_stream->ic->start_time / (double)AV_TIME_BASE)
                            pos = cur_stream->ic->start_time / (double)AV_TIME_BASE;
                        stream_seek(cur_stream, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
                    }
                break;
            default:
                break;
            }
            break;
        //*  鼠标输入处理
        case SDL_MOUSEBUTTONDOWN:
            if (exit_on_mousedown) {
                do_exit(cur_stream);
                break;
            }
            if ( event.button.button == SDL_BUTTON_LEFT ) {
                //* 快速双击左键会切换全屏。
                static int64_t last_mouse_left_click = 0;
                if (av_gettime_relative() - last_mouse_left_click <= 500000) {
                    toggle_full_screen(cur_stream);
                    cur_stream->force_refresh = 1;
                    last_mouse_left_click = 0;
                } else {
                    last_mouse_left_click = av_gettime_relative();
                }
            }
        //* 鼠标移动处理
        case SDL_MOUSEMOTION:
            //* 如果鼠标被移动且光标被隐藏，则会显示光标。
            if ( cursor_hidden ) {
                SDL_ShowCursor(1);
                cursor_hidden = 0;
            }
            //* 根据鼠标按键的状态（按下或移动）来决定是否进行视频的寻址。
            cursor_last_shown = av_gettime_relative();
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button != SDL_BUTTON_RIGHT)
                    break;
                x = event.button.x;
            } else {
                if (!(event.motion.state & SDL_BUTTON_RMASK))
                    break;
                x = event.motion.x;
            }
            if (seek_by_bytes || cur_stream->ic->duration <= 0) {
                uint64_t size =  avio_size(cur_stream->ic->pb);
                stream_seek(cur_stream, size*x/cur_stream->width, 0, 1);
            } else {
                int64_t ts;
                int ns, hh, mm, ss;
                int tns, thh, tmm, tss;
                tns  = cur_stream->ic->duration / 1000000LL;
                thh  = tns / 3600;
                tmm  = (tns % 3600) / 60;
                tss  = (tns % 60);
                frac = x / cur_stream->width;
                ns   = frac * tns;
                hh   = ns / 3600;
                mm   = (ns % 3600) / 60;
                ss   = (ns % 60);
                av_log(NULL, AV_LOG_INFO,
                        "Seek to %2.0f%% (%2d:%02d:%02d) of total duration (%2d:%02d:%02d)       \n", frac*100,
                        hh, mm, ss, thh, tmm, tss);
                ts = frac * cur_stream->ic->duration;
                if (cur_stream->ic->start_time != AV_NOPTS_VALUE)
                    ts += cur_stream->ic->start_time;
                stream_seek(cur_stream, ts, 0, 0);
            }
            break;
        //* 窗口事件
        case SDL_WINDOWEVENT:
            switch ( event.window.event ) {
            //* 窗口大小发生变化
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                //* 会更新流的宽度和高度
                screen_width = cur_stream->width = event.window.data1;
                screen_height = cur_stream->height = event.window.data2;
                //* 销毁纹理（如果存在）
                if ( cur_stream->vis_texture ) {
                    SDL_DestroyTexture(cur_stream->vis_texture);
                    cur_stream->vis_texture = NULL;
                }
                //* 在适用时调整渲染器的大小。
                if ( vk_renderer )
                    vk_renderer_resize(vk_renderer, screen_width, screen_height);
             //* 窗口被暴露，将强制刷新显示
            case SDL_WINDOWEVENT_EXPOSED:
                cur_stream->force_refresh = 1;
            }
            break;
        //* 退出处理
        //* 检查 SDL_QUIT 和自定义的退出事件（FF_QUIT_EVENT）
        case SDL_QUIT:
        case FF_QUIT_EVENT:
            do_exit(cur_stream);
            break;
        default:
            break;
        }
    }
}
```



## `is_realtime`

```c
//* 通过检查输入格式的名称以及流的 URL 来确定流的类型。
//* 如果流是 RTP、RTSP 或 SDP 格式
//* 或者其 URL 以 rtp : 或 udp : 开头
//* 则视为实时流，返回 1。否则，返回 0。
static int is_realtime(AVFormatContext *s)
{
    if ( !strcmp(s->iformat->name, "rtp")
        || !strcmp(s->iformat->name, "rtsp")
        || !strcmp(s->iformat->name, "sdp")
    )
        return 1;

    if ( s->pb && (!strncmp(s->url, "rtp:", 4)
        || !strncmp(s->url, "udp:", 4)
        )
    )
        return 1;
    return 0;
}
```

