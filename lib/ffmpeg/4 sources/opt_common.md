# opt_common

---



## `show_banner`

```c
//* 显示程序的信息横幅（banner），如版本号和版权信息。
void show_banner(int argc, char **argv, const OptionDef *options)
{
    //* 查找命令行参数中是否包含 version 选项，并获取其在 argv 数组中的索引。如果未找到，则 idx 为 0。
    int idx = locate_option(argc, argv, options, "version");
    //* 隐藏横幅或找到version信息则直接返回
    if ( hide_banner || idx )
        return;

    //* 打印程序的版本信息，带缩进
    print_program_info(INDENT | SHOW_COPYRIGHT, AV_LOG_INFO);
    //* 打印库信息，带缩进
    print_all_libs_info(INDENT | SHOW_CONFIG, AV_LOG_INFO);
    //* 打印库版本信息，带缩进
    print_all_libs_info(INDENT | SHOW_VERSION, AV_LOG_INFO);
}
```



## `CMDUTILS_COMMON_OPTIONS`

```c
// opt_common.h
#define CMDUTILS_COMMON_OPTIONS                                                                                         \
    { "L",            OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_license },     "show license" },                          \
    { "h",            OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_help },        "show help", "topic" },                    \
    { "?",            OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_help },        "show help", "topic" },                    \
    { "help",         OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_help },        "show help", "topic" },                    \
    { "-help",        OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_help },        "show help", "topic" },                    \
    { "version",      OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_version },     "show version" },                          \
    { "buildconf",    OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_buildconf },   "show build configuration" },              \
    { "formats",      OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_formats },     "show available formats" },                \
    { "muxers",       OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_muxers },      "show available muxers" },                 \
    { "demuxers",     OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_demuxers },    "show available demuxers" },               \
    { "devices",      OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_devices },     "show available devices" },                \
    { "codecs",       OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_codecs },      "show available codecs" },                 \
    { "decoders",     OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_decoders },    "show available decoders" },               \
    { "encoders",     OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_encoders },    "show available encoders" },               \
    { "bsfs",         OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_bsfs },        "show available bit stream filters" },     \
    { "protocols",    OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_protocols },   "show available protocols" },              \
    { "filters",      OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_filters },     "show available filters" },                \
    { "pix_fmts",     OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_pix_fmts },    "show available pixel formats" },          \
    { "layouts",      OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_layouts },     "show standard channel layouts" },         \
    { "sample_fmts",  OPT_TYPE_FUNC, OPT_EXIT,              { .func_arg = show_sample_fmts }, "show available audio sample formats" },   \
    { "dispositions", OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_dispositions}, "show available stream dispositions" },    \
    { "colors",       OPT_TYPE_FUNC, OPT_EXIT | OPT_EXPERT, { .func_arg = show_colors },      "show available color names" },            \
    { "loglevel",     OPT_TYPE_FUNC, OPT_FUNC_ARG | OPT_EXPERT, { .func_arg = opt_loglevel },     "set logging level", "loglevel" },         \
    { "v",            OPT_TYPE_FUNC, OPT_FUNC_ARG,          { .func_arg = opt_loglevel },     "set logging level", "loglevel" },         \
    { "report",       OPT_TYPE_FUNC, OPT_EXPERT,            { .func_arg = opt_report },       "generate a report" },                     \
    { "max_alloc",    OPT_TYPE_FUNC, OPT_FUNC_ARG | OPT_EXPERT, { .func_arg = opt_max_alloc },    "set maximum size of a single allocated block", "bytes" }, \
    { "cpuflags",     OPT_TYPE_FUNC, OPT_FUNC_ARG | OPT_EXPERT, { .func_arg = opt_cpuflags },     "force specific cpu flags", "flags" },     \
    { "cpucount",     OPT_TYPE_FUNC, OPT_FUNC_ARG | OPT_EXPERT, { .func_arg = opt_cpucount },     "force specific cpu count", "count" },     \
    { "hide_banner",  OPT_TYPE_BOOL, OPT_EXPERT,            {&hide_banner},                   "do not show program banner", "hide_banner" }, \
    CMDUTILS_COMMON_OPTIONS_AVDEVICE                                                                                    \
```

