# cmdutils

---



## `parse_loglevel`

```c
//* 解析命令行参数中的日志级别设置和报告配置。
void parse_loglevel(int argc, char **argv, const OptionDef *options)
{
    //* 查找命令行参数中是否有 loglevel 选项，并返回它在 argv 数组中的索引。如果没有找到，idx 将为 0。
    int idx = locate_option(argc, argv, options, "loglevel");
    char *env;

    //* 验证传入的选项是否合理和有效。
    check_options(options);

    
    //* 如果没有 loglevel 选项，尝试查找 v 选项（通常用于设置详细输出）
    if ( !idx )
        idx = locate_option(argc, argv, options, "v");
    //* 如果找到了 loglevel 或 v 选项，并且后续有参数，设置日志级别。
    if ( idx && argv[idx + 1] )
        opt_loglevel(NULL, "loglevel", argv[idx + 1]);

    idx = locate_option(argc, argv, options, "report");
    env = getenv_utf8("FFREPORT");
    //* 如果找到了 report 或环境变量 FFREPORT 存在，则初始化报告文件
    if (env || idx) {
        FILE *report_file = NULL;
        init_report(env, &report_file);
        //* 写入命令行信息到报告文件
        if ( report_file ) {
            int i;
            //* 写入 "Command line:" 头部。
            fprintf(report_file, "Command line:\n");
            //* 遍历所有命令行参数，并将每个参数写入报告文件，参数之间以空格分隔，行尾换行。
            for ( i = 0; i < argc; i++ ) {
                dump_argument(report_file, argv[i]);
                fputc(i < argc - 1 ? ' ' : '\n', report_file);
            }
            //* 刷写落盘
            fflush(report_file);
        }
    }

    //* 释放通过 getenv_utf8 获取的环境变量字符串
    freeenv_utf8(env);

    //* hide_banner 隐藏banner信息
    idx = locate_option(argc, argv, options, "hide_banner");
    if (idx)
        hide_banner = 1;
}
```



## `parse_options`

​		依次解析命令行选项和参数，并根据情况调用指定的处理函数。

​		这是命令行应用程序中非常常见的一个模式，帮助程序灵活地解析用户输入的参数。

​		同时，它还注意到选项的标志和参数之间的关系，以确保正确处理。该函数的设计良好，可以易于扩展和维护。

```c
//* 解析命令行参数，包括识别选项和参数，并将它们传递给适当的处理函数。
//* optctx：一个指向上下文的指针，用于在解析选项时保存状态或数据。
//* argc：命令行参数的数量。
//* argv：命令行参数的字符串数组。
//* options：指向定义选项的数组的指针。
//* parse_arg_function：一个指向函数的指针，该函数用于处理未被识别的非选项参数。
int parse_options(void *optctx, int argc, char **argv, const OptionDef *options,
                  int (*parse_arg_function)(void *, const char*))
{
    const char *opt; //* 指向当前选项的指针。
    //* optindex：当前分析的参数索引。
    //* handleoptions：标志，指示是否仍在处理选项。
    int optindex, handleoptions = 1, ret;

    /* perform system-dependent conversions for arguments list */
    //* 参数列表转换
    prepare_app_arguments(&argc, &argv);

    /* parse options */
    //* 从 argv 中的第二个参数开始（argv[0] 通常是程序名称），循环遍历所有参数。
    optindex = 1;
    while (optindex < argc) {
        opt = argv[optindex++];

        //* 如果 handleoptions 为真且当前参数以 - 开头，则表示这是一个选项。
        if ( handleoptions && opt[0] == '-' && opt[1] != '\0' ) {
            //* 如果参数是 --，则设置 handleoptions 为假，并继续下一个循环，意味着不再处理当前选项。
            if ( opt[1] == '-' && opt[2] == '\0' ) {
                handleoptions = 0;
                continue;
            }
            //* 是选项，则opt 指向第二个字符以便后续处理。
            opt++;

            //* 解析当前选项（opt），传递上下文、当前选项及后续参数（argv[optindex]）。
            if ( (ret = parse_option(optctx, opt, argv[optindex], options)) < 0 )
                return ret;
            //* 成功解析后，更新 optindex，以便跳过解析过的参数。
            optindex += ret;
        } else {
            //* 如果当前参数不是选项，则检查是否提供了 parse_arg_function
            if ( parse_arg_function ) {
                //* 如果有，调用该函数解析非选项参数，传递上下文和当前参数（opt）。
                ret = parse_arg_function(optctx, opt);
                if (ret < 0)
                    return ret;
            }
        }
    }

    return 0;
}
```



## `OptionDef`

```c
typedef struct OptionDef {
    const char *name;       //* 选项的名称。例如“i”，“f”，“codec”等等。
    enum OptionType type;   //* 选项类型
    int flags;              //* 选项的标志

    //* // Union for storing different types of data 
    union
    {
        void *dst_ptr;
        int (*func_arg)(void *, const char *, const char *); //* 操作函数 
        size_t off;
    } u;
    const char *help;   //* 选项的帮助说明  
    const char *argname; //* 参数名称  

    union {
        /* Name of the canonical form of this option.
         * Is valid when OPT_HAS_CANON is set. */
        const char *name_canon; //* 标准选项名称
        /* A NULL-terminated list of alternate forms of this option.
         * Is valid when OPT_HAS_ALT is set. */
        const char * const *names_alt; //* 替代选项名称的列表
    } u1;
} OptionDef;
```

- `int flags`

  - ```c
    // Flags for the option type  
    #define OPT_FUNC_ARG    (1 << 0)  // 选项可以接受参数  
    #define OPT_EXIT        (1 << 1)  // 处理此选项后程序将立即退出  
    #define OPT_EXPERT      (1 << 2)  // 仅限高级用户，影响帮助输出  
    #define OPT_VIDEO       (1 << 3)  // 与视频相关的选项  
    #define OPT_AUDIO       (1 << 4)  // 与音频相关的选项  
    #define OPT_SUBTITLE    (1 << 5)  // 与字幕相关的选项  
    #define OPT_DATA        (1 << 6)  // 与数据相关的选项  
    #define OPT_PERFILE     (1 << 7)  // 针对每个文件的选项（ffmpeg专用）  
    #define OPT_FLAG_OFFSET (1 << 8)  // 选项是通过偏移量在optctx中指定的  
    #define OPT_OFFSET      (OPT_FLAG_OFFSET | OPT_PERFILE)  
    #define OPT_FLAG_SPEC   (1 << 9)  // 选项存储在SpecifierOptList中  
    #define OPT_SPEC        (OPT_FLAG_SPEC | OPT_OFFSET)  
    #define OPT_FLAG_PERSTREAM (1 << 10) // 选项适用于每个流  
    #define OPT_PERSTREAM   (OPT_FLAG_PERSTREAM | OPT_SPEC)  
    #define OPT_INPUT       (1 << 11) // 针对输入的选项  
    #define OPT_OUTPUT      (1 << 12) // 针对输出的选项  
    #define OPT_HAS_ALT     (1 << 13) // 选项有替代形式  
    #define OPT_HAS_CANON   (1 << 14) // 选项是某个其他选项的替代形式  
    #define OPT_DECODER     (1 << 15) // 针对独立解码器的选项  
    ```





