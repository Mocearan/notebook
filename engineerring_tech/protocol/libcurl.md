<h1>
    libcurl
</h1>

https://curl.haxx.se/libcurl/c/libcurl-tutorial.html

---

## overview

### 可移植性

`libcurl`背后的开发人员付出巨大努力，使得`libcurl`可以在诸多不同的操作系统和环境中工作。这使得使用者可以在不同的平台上以相同的方式写基于`libcurl`的代码。只要用户保证书写的代码可移植，`libcurl`亦保证该程序可移植。

libcurl是一个跨平台的开源网络协议库，支持http, https, rtsp等多种协议 。libcurl同样支持HTTPS证书授权，HTTP POST, HTTP PUT, FTP 上传, HTTP基本表单上传，代理，cookies,和用户认证。

### 两类接口

1. `easy interface`

   `easy interface`中的所有操作都以`curl_easy`作为前缀，`easy interface`使用同步和阻塞函数进行单词传输。

2. `multi interface`

   允许在单线程中进行多个并发传输，即所谓的多重接口.采用组合的方式可以一次性发送多条请求，且多个下载请求时异步进行的。



## 构建

有很多种不同的方式来编译C语言代码。这里使用UNIX平台下的编译方式。即使你使用的是其他的操作系统，你仍然可以通过阅读本文档来获取许多有用的信息。

### 1. 编译程序

你的编译器需要知道`libcurl`头文件位置。因此必须设置编译器的 include path 指向`libcurl`的安装安装目录。`curl-config`工具可用于获取此信息。

`$ curl-config --cflags`

### 2. 链接curl库

编译时，需要将目标文件链接来创建一个可执行程序。为此，需要连接 `libcurl`库，以及可能的`libcurl`本身依赖的第三方库，如`OpenSSL`，当然可能还需要连接一些其他的操作系统库。可以使用`curl-config`工具简化操作

`$ curl-config --libs`

### 3. 是否使用SSL

libcurl可以在许多方面构建和定制。

`libcurl`可以定制编译，根据实际需要是否支持某些特性，如是否支持SSL传输，像HTTPS和FTPS。如果需要支持SSL，必须在编译时正确的设置。可以使用`curl-config`工具来判断libcurl库是否支持了SSL

`$curl-config --feature`

### `libcurl`提供的特性

普遍认可的最佳实践是，在运行时确定`libcurl`的特性而不是在编译时（如果可能的话）。通过调用`lib_version_info` 并检查返回的结构的详细信息，您的程序可以确定当前正在运行的`libcurl`支持什么。

```c
// <curl.h>

 /*
 * NAME curl_version_info()
 *
 * DESCRIPTION
 *
 * This function returns a pointer to a static copy of the version info
 * struct. See above.
 */
CURL_EXTERN curl_version_info_data *curl_version_info(CURLversion);


/*
 * NAME curl_version()
 *
 * DESCRIPTION
 *
 * Returns a static ascii string of the libcurl version.
 */
CURL_EXTERN char *curl_version(void);
```

```c
// curl_version_info_data
typedef struct {
  CURLversion age;          /* age of the returned struct */
  const char *version;      /* LIBCURL_VERSION */
  unsigned int version_num; /* LIBCURL_VERSION_NUM */
  const char *host;         /* OS/host/cpu/machine when configured */
  int features;             /* bitmask, see defines below */
  const char *ssl_version;  /* human readable string */
  long ssl_version_num;     /* not used anymore, always 0 */
  const char *libz_version; /* human readable string */
  /* protocols is terminated by an entry with a NULL protoname */
  const char * const *protocols;

  /* The fields below this were added in CURLVERSION_SECOND */
  const char *ares;
  int ares_num;

  /* This field was added in CURLVERSION_THIRD */
  const char *libidn;

  /* These field were added in CURLVERSION_FOURTH */

  /* Same as '_libiconv_version' if built with HAVE_ICONV */
  int iconv_ver_num;

  const char *libssh_version; /* human readable string */

} curl_version_info_data;

// CURLversion
typedef enum {
  CURLVERSION_FIRST,
  CURLVERSION_SECOND,
  CURLVERSION_THIRD,
  CURLVERSION_FOURTH,
  CURLVERSION_LAST /* never actually use this */
} CURLversion;

/* The 'CURLVERSION_NOW' is the symbolic name meant to be used by
   basically all programs ever that want to get version information. It is
   meant to be a built-in version number for what kind of struct the caller
   expects. If the struct ever changes, we redefine the NOW to another enum
   from above. */
#define CURLVERSION_NOW CURLVERSION_FOURTH
```



 ```c
 // curl_info.c
 
 
 #include <curl/curl.h>
 #include <stdio.h>
 
 void show_version_info(curl_version_info_data* pInfo)
{
   printf("version: %s\n", pInfo->version);
  	printf("version num: %u\n", pInfo->version_num);
  	printf("host: %s\n", pInfo->host);
  	printf("features: %d\n", pInfo->features);
  	printf("ssl version: %s\n", pInfo->ssl_version);
  	printf("ssl version num: %lu\n", pInfo->ssl_version_num);
  	printf("libz version: %s\n", pInfo->libz_version);
  	printf("protocols : %s\n", pInfo->protocols);
  	printf("ares: %s\n", pInfo->ares);
  	printf("ares num: %d\n", pInfo->ares_num);
  	printf("libidn: %s\n", pInfo->libidn);
  	printf("iconv ver num: %d\n", pInfo->iconv_ver_num);
  	printf("libssh version: %s\n", pInfo->libssh_version);
}

int main(void)
{
   curl_version_info_data* p_version = curl_version_info(CURLVERSION_NOW);
  	show_version_info(p_version);
  	return 0;
}
 ```


### `autoconf`宏

当编写配置脚本来检测`libcurl`和设置变量时，`libcurl`提供了一个预置的宏来完成相应的工作。参阅`docs/libcurl/libcurl.m4`，其中包含了使用说明。



## 全局资源

### 全局初始化

```C
#define CURL_STATICLIB                //如果是静态库方式，需要包含这句
```



应用程序在使用`libcurl`之前，必须先初始化`libcurl`.`libcurl`只需初始化一次，可以使用一下语句进行初始化：

> ```c
> // <curl.h>
> 
> // 初始化libcurl 全局
> CURL_EXTERN CURLcode curl_global_init(long flags);
> ```
>
> 接收一个参数，用以确定如何初始化 libcurl：
>
> - `CURL_GLOBAL_ALL`： 会使libcurl初始化所有的子模块和一些默认的选项。通常这是一个比较好的默认参数。（最好用这个参数初始化，除非你真正知道你需要初始化什么模块）
> - `CURL_GLOBAL_WIN32`：只能用于windows平台。它告诉libcurl初始化winsock库。如果winsock库没有正确初始化，应用程序就不能使用socket。在应用程序中，只要初始化一次即可。
> - `CURL_GLOBAL_SSL`：如果libcurl 在编译时被设定支持SSL，那么该参数用于初始化相应的SSL库。同样在应用程序中，只要初始化一次即可。
>
> ​	这些参数是位模式的，即以十六进制表示的数字，以为着它们在>
> ```c
> 
> // flags
> #define CURL_GLOBAL_SSL (1<<0)	 	//初始化支持 安全套接字层。
> #define CURL_GLOBAL_WIN32 (1<<1)	//初始化win32套接字库。
> #define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32) //初始化所有的可能的调用。
> #define CURL_GLOBAL_NOTHING 0	 		//没有额外的初始化。
> #define CURL_GLOBAL_DEFAULT CURL_GLOBAL_ALL
> #define CURL_GLOBAL_ACK_EINTR (1<<2)
> 
> // CURLcode
> typedef enum {
>   CURLE_OK = 0,
>   CURLE_UNSUPPORTED_PROTOCOL,    /* 1 */
>   CURLE_FAILED_INIT,             /* 2 */
>   CURLE_URL_MALFORMAT,           /* 3 */
>   CURLE_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
>                                     7.17.0, reused in April 2011 for 7.21.5] */
>   CURLE_COULDNT_RESOLVE_PROXY,   /* 5 */
>   CURLE_COULDNT_RESOLVE_HOST,    /* 6 */
>   CURLE_COULDNT_CONNECT,         /* 7 */
>   CURLE_FTP_WEIRD_SERVER_REPLY,  /* 8 */
>   CURLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
>                                     due to lack of access - when login fails
>                                     this is not returned. */
>   CURLE_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
>                                     7.15.4, reused in Dec 2011 for 7.24.0]*/
>   CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */
>   CURLE_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
>                                     [was obsoleted in August 2007 for 7.17.0,
>                                     reused in Dec 2011 for 7.24.0]*/
>   CURLE_FTP_WEIRD_PASV_REPLY,    /* 13 */
>   CURLE_FTP_WEIRD_227_FORMAT,    /* 14 */
>   CURLE_FTP_CANT_GET_HOST,       /* 15 */
>   CURLE_OBSOLETE16,              /* 16 - NOT USED */
>   CURLE_FTP_COULDNT_SET_TYPE,    /* 17 */
>   CURLE_PARTIAL_FILE,            /* 18 */
>   CURLE_FTP_COULDNT_RETR_FILE,   /* 19 */
>   CURLE_OBSOLETE20,              /* 20 - NOT USED */
>   CURLE_QUOTE_ERROR,             /* 21 - quote command failure */
>   CURLE_HTTP_RETURNED_ERROR,     /* 22 */
>   CURLE_WRITE_ERROR,             /* 23 */
>   CURLE_OBSOLETE24,              /* 24 - NOT USED */
>   CURLE_UPLOAD_FAILED,           /* 25 - failed upload "command" */
>   CURLE_READ_ERROR,              /* 26 - couldn't open/read from file */
>   CURLE_OUT_OF_MEMORY,           /* 27 */
>   /* Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
>            instead of a memory allocation error if CURL_DOES_CONVERSIONS
>            is defined
>   */
>   CURLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
>   CURLE_OBSOLETE29,              /* 29 - NOT USED */
>   CURLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
>   CURLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
>   CURLE_OBSOLETE32,              /* 32 - NOT USED */
>   CURLE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
>   CURLE_HTTP_POST_ERROR,         /* 34 */
>   CURLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
>   CURLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
>   CURLE_FILE_COULDNT_READ_FILE,  /* 37 */
>   CURLE_LDAP_CANNOT_BIND,        /* 38 */
>   CURLE_LDAP_SEARCH_FAILED,      /* 39 */
>   CURLE_OBSOLETE40,              /* 40 - NOT USED */
>   CURLE_FUNCTION_NOT_FOUND,      /* 41 */
>   CURLE_ABORTED_BY_CALLBACK,     /* 42 */
>   CURLE_BAD_FUNCTION_ARGUMENT,   /* 43 */
>   CURLE_OBSOLETE44,              /* 44 - NOT USED */
>   CURLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
>   CURLE_OBSOLETE46,              /* 46 - NOT USED */
>   CURLE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
>   CURLE_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
>   CURLE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
>   CURLE_OBSOLETE50,              /* 50 - NOT USED */
>   CURLE_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
>                                      wasn't verified fine */
>   CURLE_GOT_NOTHING,             /* 52 - when this is a specific error */
>   CURLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
>   CURLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
>                                     default */
>   CURLE_SEND_ERROR,              /* 55 - failed sending network data */
>   CURLE_RECV_ERROR,              /* 56 - failure in receiving network data */
>   CURLE_OBSOLETE57,              /* 57 - NOT IN USE */
>   CURLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
>   CURLE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
>   CURLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
>   CURLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
>   CURLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
>   CURLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
>   CURLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
>   CURLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
>                                     that failed */
>   CURLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
>   CURLE_LOGIN_DENIED,            /* 67 - user, password or similar was not
>                                     accepted and we failed to login */
>   CURLE_TFTP_NOTFOUND,           /* 68 - file not found on server */
>   CURLE_TFTP_PERM,               /* 69 - permission problem on server */
>   CURLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
>   CURLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
>   CURLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
>   CURLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
>   CURLE_TFTP_NOSUCHUSER,         /* 74 - No such user */
>   CURLE_CONV_FAILED,             /* 75 - conversion failed */
>   CURLE_CONV_REQD,               /* 76 - caller must register conversion
>                                     callbacks using curl_easy_setopt options
>                                     CURLOPT_CONV_FROM_NETWORK_FUNCTION,
>                                     CURLOPT_CONV_TO_NETWORK_FUNCTION, and
>                                     CURLOPT_CONV_FROM_UTF8_FUNCTION */
>   CURLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
>                                     or wrong format */
>   CURLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
>   CURLE_SSH,                     /* 79 - error from the SSH layer, somewhat
>                                     generic so the error message will be of
>                                     interest when this has happened */
> 
>   CURLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
>                                     connection */
>   CURLE_AGAIN,                   /* 81 - socket is not ready for send/recv,
>                                     wait till it's ready and try again (Added
>                                     in 7.18.2) */
>   CURLE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
>                                     wrong format (Added in 7.19.0) */
>   CURLE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
>                                     7.19.0) */
>   CURLE_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
>   CURLE_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
>   CURLE_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
>   CURLE_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
>   CURLE_CHUNK_FAILED,            /* 88 - chunk callback reported error */
>   CURL_LAST /* never use! */
> } CURLcode;
> ```

`libcurl`有默认的保护机制，如果在调用`curl_easy_perform`时没有进行全局初始化会造成失败。

### 全局资源释放

```c
/*
 * NAME curl_global_cleanup()
 *
 * DESCRIPTION
 *
 * curl_global_cleanup() should be invoked exactly once for each application
 * that uses libcurl
 */
CURL_EXTERN void curl_global_cleanup(void);
```



## easy interface

### 基本使用 GET

easy interface 的API都有相同的前缀：`curl_eas_*`

使用`easy interface`，必须首先创建一个easy handle，easy handle 用于执行每次操作。

基本上，每个线程都应该有自己的easy handle用于数据通信（如果需要的话），千万不要在多线程之间共享同一个easy handle。

#### 获取 easy handle

```c
// 获取一个easy handle

// <easy.h>
CURL_EXTERN CURL *curl_easy_init(void);

// CURL : easy handle
typedef void CURL;  // CURL* -> void*
```

> 它返回一个easy handle句柄，使它进行下一步：设置首选项。
>
> 一个句柄一个连接的逻辑实体，用以在接下来进行数据传输。

#### 设置 easy handle

`lincurl`许多属性使用字符串（`‘\0’`标识字符串结束）进行设置。通过`curl_easy_setopt`函数设置字符串属性时，`libcurl`内部会自动拷贝这些字符串，所以在设置完相关属性之后，字符串可以直接被释放掉（如果需要的话）。

```c
// 设置 easy handle 的选项和属性

// <easy.h>
CURL_EXTERN CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);

// 这是curl 中最重要的函数，它告知curl库，程序将有什么样的行为，通过不同的选项来控制行为。类似ioctl函数。
// curl : easy handle
// option : 各种CURLoption类型的选项，curl.h中有定义
// paramter...: 可以是个函数的指针，也可以是某个对象的指针，还可以使个long型的变量。它具体是什么，取决于第二个参数。
```

> 使用curl_easy_setopt函数可以设置easy handle的属性和操作，这些属性和操作控制`libcurl`如何与远程主机进行数据通信。
>
> 一旦在easy handle中设置了哪个赢的属性和操作，它们将一直作用在该easy handle。也就是说，重复使用同一个easy handle发出请求，设置的属性是不变的。
>
> 可以调用`curl_easy_reset`来在任何时候清除所有以前设置的选项，也可以使用`curl_easy_duphandle`克隆一个简单句柄（以及它的所有选项）。
>
> ```c
> /*
>  * NAME curl_easy_reset()
>  *
>  * DESCRIPTION
>  *
>  * Re-initializes a CURL handle to the default values. This puts back the
>  * handle to the same state as it was in when it was just created.
>  *
>  * It does keep: live connections, the Session ID cache, the DNS cache and the
>  * cookies.
>  */
> CURL_EXTERN void curl_easy_reset(CURL *curl)
> ```
>
> ```c
> /*
>  * NAME curl_easy_duphandle()
>  *
>  * DESCRIPTION
>  *
>  * Creates a new curl session handle with the same options set for the handle
>  * passed in. Duplicating a handle could only be a matter of cloning data and
>  * options, internal state info and things like persistent connections cannot
>  * be transferred. It is useful in multithreaded applications when you can run
>  * curl_easy_duphandle() for each new thread to avoid a series of identical
>  * curl_easy_setopt() invokes in every thread.
>  */
> CURL_EXTERN CURL* curl_easy_duphandle(CURL *curl)
> ```

##### 设置HTTP 版本

每一次http请求，都包含一个表示当前使用http版本的消息头。libcurl默认使用HTTP 1.1。可以通过CURLOPT_HTTP_VERSION属性来设置具体的版本号：:

```c
curl_easy_setopt(easyhandle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
```

##### 设置详细调试信息

```c
// CURLOPT_VERBOSE
在使用该选项且第 3 个参数为 1 时，curl 库会显示详细的操作信息。这对程序的调试具有极大的帮助。
```

##### 设置URL

easy handle中最基本、最常用的属性设置是`URL`，使用`CURLOPT_URL`设置传输的URL：

```c
curl_easy_setopt(handle, CURLOPT_URL, "http://domain.com/");

// 如果URL参数不写上协议头(如 “http://” 或者 "ftp:// 等等)，那么函数会自己进行猜解所给的主机上用的是哪一种服务协议。
// 假如给的这个地址是一个不被支持的协议，那么在其后执行curl_easy_perform() 函数或 curl_multi_perform() 函数时，libcurl将返回错误(CURLE_UNSUPPORTED_PROTOCOL)。 
// 这个选项是唯一一个在 curl_easy_perform()调用之前就一定要设置的选项。
```

##### 设置回调函数

假设你要获取URL所表示的远程主机上的资源。你需要写一段程序用来完成数据传输，你可能希望直接保存接收到的数据而不是简单的在输出窗口中打印它们。

> 回调函数将在libcurl接收到数据后被调用，因此函数多做数据保存的功能，如处理下载文件。原型：
>
> ```c
> size_t function(void* buffer, size_t size, size_t nmemb, void* userp);
> ```

使用下面的语句来注册回调函数，回调函数将会在接收到返回的数据时被调用：

```c
// CURLOPT_WRITEFUNCTION，CURLOPT_WRITEDATA  设置写回调

// 1）CURLOPT_WRITEFUNCTION选项用于设置接收数据回调函数，回调函数原型为： size_t function(void *ptr, size_t size, size_t nmemb, void *stream); 函数将在libcurl接收到数据后被调用，因此函数多做数据保存的功能，如处理下载文件。

// 2） CURLOPT_WRITEDATA选项用于指定CURLOPT_WRITEFUNCTION函数中的stream指针的来源。

// 3）如果没有通过CURLOPT_WRITEFUNCTION属性给easy handle设置回调函数，libcurl会提供一个默认的回调函数，它只是简单的将接收到的数据打印到标准输出。也可以通过CURLOPT_WRITEDATA属性给默认回调函数传递一个已经打开的文件指针，用于将数据输出到文件里。

curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);

curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &internal_struct);  
// 可以给回调函数传递一个自定义参数（void*，挂一个结构体）,`libcurl`不处理该参数，只是简单的传递：
```

> **NOTE：**一些平台相关的注意点。
>
> 在一些平台上，`libcurl`不能直接操作由应用程序打开的文件。所以，如果使用默认的回调函数，同时通过`CURLOPT_WRITEDATA`属性给easy handle 传递一个文件指针，应用程序可能会执行失败。如果你希望自己的程序能在任何平台上运行，就必须避免这种情况。
>
> 如果以win32动态链接库的形式来使用`libcurl`，在设置`CURLOPT_WRITEDATA`属性时，你必须也使用`CURLOPT_WRITEFUNCION`来注册回调函数。否则程序会执行失败。也就是说，在win32上没有默认的回调函数。

```c
 // CURLOPT_HTTPHEADER, CURLOPT_HEADERFUNCTION，CURLOPT_HEADERDATA   解析http头的回调函数

// libcurl有自己默认的请求头，如果不符合我们的要求，可以使用该选项自定义请求头。可以使用curl_slist_append进行自定义，重设，如果设置请求参数为空，则相当于删除该请求头。
    
// 1）CURLOPT_HEADERFUNCTION设置接收到http头的回调函数，原型为： size_t function(void *ptr,size_t size,size_t nmemb, void *stream); 
size_t function( void *ptr, size_t size,size_t nmemb, void *stream); 
// libcurl一旦接收到http 头部数据后将调用该函数。

// 2）CURLOPT_HEADERDATA传递指针给libcurl，该指针表明CURLOPT_HEADERFUNCTION函数的stream指针的来源。
```

```c
 //  CURLOPT_READFUNCTION, CURLOPT_READDATA   读回调函数

//  libCurl需要读取数据传递给远程主机时将调用CURLOPT_READFUNCTION指定的函数，
size_t function(void *ptr, size_t size, size_t nmemb,void *stream). 
   
    // CURLOPT_READDATA 表明CURLOPT_READFUNCTION函数原型中的stream指针来源。
```

```c
// LOPT_NOPROGRESS，CURLOPT_PROGRESSFUNCTION，CURLOPT_PROGRESSDATA  进度回调

// `libcurl`支持通信过程中的进度控制，将`CURLOPT_NOPROGRESS`设置为0开启进度支持，默认值为1.
// 对于大多数用程序进度表是无用的，而有趣的是能够指定进度回调。 然后，传递给libcurl的函数指针将不定期的将进度信息告知应用程序。 正常情况下每秒被libcurl调用一次。

int progress_callback(void *clientp,
                      double dltotal,
                      double dlnow,
                      double ultotal,
                      double ulnow);

/*
	CURLOPT_NOPROGRESS必须被设置为false
  `clientp`是一个用户自定义指针。用户程序通过CURLOPT_PROCESSDATA 属性将自定义指定传递给libcurl。libcurl对该参数不作任何处理，只是简单将其传递给回调函数。
  CURLOPT_PROGRESSDATA指定的参数将作为CURLOPT_PROGRESSFUNCTION指定函数的第一个参数
*/
```

##### 设置超时

```c
// CURLOPT_TIMEOUT，CURLOPT_CONNECTIONTIMEOUT
超时相关设置，时间单位为s
1）CURLOPT_TIMEOUT设置整个libcurl传输超时时间。
2）CURLOPT_CONNECTIONTIMEOUT设置连接等待时间。设置为0，则无限等待
```

##### 设置用户代理

```c
// CURLOPT_USERAGENT

该选项要求传递一个以 ‘\0’ 结尾的字符串指针，这个字符串用来在向服务器请求时发送 HTTP 头部中的 User-Agent 信息，有些服务器是需要检测这个信息的，如果没有设置User-Agent，那么服务器拒绝请求。设置后，可以骗过服务器对此的检查。
```



##### 设置重定向

```c
 // CURLOPT_FOLLOWLOCATION，CURLOPT_MAXREDIRS 重定向相关设置

// 1）CURLOPT_FOLLOWLOCATION 设置为非0，响应头信息Location，即curl会自己处理302等重定向
// 2）CURLOPT_MAXREDIRS 指定HTTP重定向的最大次数
```



##### 设置断点续传

```c
// CURLOPT_RANGE ，CURLOPT_RESUME_FROM/ CURLOPT_RESUME_FROM_LARGE 

// 1）CURLOPT_RANGE 指定char *参数传递给libcurl，用于指明http请求的range

// 2）CURLOPT_RESUME_FROM传递一个long参数作为偏移量给libcurl，指定开始进行传输的位置。CURLOPT_RESUME_FROM大小限制为2G，超过可以使用CURLOPT_RESUME_FROM_LARGE
```

##### 设置post字段

```c
// CURLOPT_POSTFIELDS，CURLOPT_POSTFIELDSIZE
// 1）CURLOPT_POSTFIELDS 传递一个作为HTTP “POST”操作的所有数据的字符串。
// 2）CURLOPT_POSTFIELDSIZE 设置POST 字节大小。
```

#####  设置HEAD请求

```c
// CURLOPT_NOBODY
	
设置该属性即可告诉libcurl我想发起一个HEAD请求 有时候你想查询服务器某种资源的状态，比如某个文件的属性：修改时间，大小等等，但是并不需要具体得到该文件，这时我们仅需要HEAD请求。
```

##### 设置压缩方式

```c
// CURLOPT_ACCEPT_ENCODING
设置libcurl对特定压缩方式自动解码，支持的方式有： “br, gzip, deflate”. 第3个参数为指定的压缩方式，如果设置为 " "，则表明三种都支持。
```

##### 设置限速

```c
// CURLOPT_MAX_RECV_SPEED_LARGE，CURLOPT_MAX_SEND_SPEED_LARGE 限速相关设置
1）CURLOPT_MAX_RECV_SPEED_LARGE，指定下载过程中最大速度，单位bytes/s。
2）CURLOPT_MAX_SEND_SPEED_LARG，指定上传过程中最大速度，单位bytes/s。
```



##### 设置禁止重用连接

用libcurl下载，然后使用netstat查看发现有大量的TCP连接保持在CLOSE_WAIT状态
查看libcurl的文档说明，有这样一个选项：

> **CURLOPT_FORBID_REUSE**
> Pass a long. Set to 1 to make the next transfer explicitly close the connection when done. Normally, libcurl keeps all connections alive when done with one transfer in case a succeeding one follows that can re-use them. This option should be used with caution and only if you understand what it does. Set to 0 to have libcurl keep the connection open for possible later re-use (default behavior).
>
> ```c
> // CURLOPT_FORBID_REUSE ，CURLOPT_FRESH_CONNEC 如果不使用长连接，需要设置这两个属性
> 1）CURLOPT_FORBID_REUSE 设置为1，在完成交互以后强迫断开连接，不重用。
> 2）CURLOPT_FRESH_CONNECT设置为1，强制获取一个新的连接，替代缓存中的连接。
> ```

也就是说，默认情况下libcurl完成一个任务以后，出于重用连接的考虑不会马上关闭
如果没有新的TCP请求来重用这个连接，那么只能等到CLOSE_WAIT超时，这个时间默认在7200秒甚至更高，太多的CLOSE_WAIT连接会导致性能问题
解决方法：

```c
curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
// 最好再修改一下TCP参数调低CLOSE_WAIT和TIME_WAIT的超时时间
```

##### 设置信号中断

```c
// CURLOPT_NOSIGNAL
当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。如果不设置这个选项，libcurl将会发信号打断这个wait从而可能导致程序crash。 在多线程处理场景下使用超时选项时，会忽略signals对应的处理函数。
```

##### 设置缓冲区大小

```c
// CURLOPT_BUFFERSIZE
指定libcurl中接收缓冲区的首选大小（以字节为单位），但是不保证接收数据时每次数据量都能达到这个值。此缓冲区大小默认为CURL_MAX_WRITE_SIZE（16kB）。允许设置的最大缓冲区大小为CURL_MAX_READ_SIZE（512kB）。 允许设置的最小缓冲区大小为1024。
```



##### 设置长连接

```c
 /* enable TCP keep-alive for this transfer */
curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
/* keep-alive idle time to 120 seconds */
curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
/* interval time between keep-alive probes: 60 seconds */
curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);
```



##### 设置DNS

```c
// CURLOPT_IPRESOLVE   指定libcurl 域名解析模式。支持的选项有：
1）CURL_IPRESOLVE_WHATEVER：默认值，相当于PF_UNSPEC，支持IPv4/v6，具体以哪个优先需要看libc底层实现，Android中默认以IPv6优先，当IPv6栈无法使用时，libcurl会用IPv4。
2）CURL_IPRESOLVE_V4：.仅请求A记录，即只解析为IPv4地址。
3）CURL_IPRESOLVE_V6：.仅请求AAAA记录，即只解析为IPv6地址。
注意：该功能生效的前提是libcurl支持IPv6，需要在curl/lib/curl_config.h配置#define ENABLE_IPV6 1
```

```c
// CURLOPT_DNS_CACHE_TIMEOUT
设置libcurl DNS缓存超时时间，默认为60秒，即每60秒清一次libcurl自身保存的DNS缓存。
如果设置为0，则不适用DNS缓存，设置为-1，则永远不清缓存。
```

```c
// CURLOPT_DNS_USE_GLOBAL_CACHE
让libcurl使用系统DNS缓存，默认情况下，libcurl使用本身的DNS缓存。
```



#### 执行 easy handle

```c
// 使用easy handle 执行一个请求。

// <easy.h>
CURL_EXTERN CURLcode curl_easy_perform(CURL *curl)
    
/*
1.    CURLE_OK
    任务完成一切都好
2     CURLE_UNSUPPORTED_PROTOCOL
    不支持的协议，由URL的头部指定
3     CURLE_COULDNT_CONNECT
    不能连接到remote 主机或者代理
4     CURLE_REMOTE_ACCESS_DENIED
    访问被拒绝
5     CURLE_HTTP_RETURNED_ERROR
    Http返回错误
6     CURLE_READ_ERROR
    读本地文件错误
*/
```

> `curl_easy_perform` 将连接到远端站点，执行必要的命令并接受数据。
>
> 每当它接收到数据，它将调用我们前面设置的回调函数，`libcurl`可能一次接收到1B到若干KB数据。`libcurl`会尽可能多尽可能及时的传递这些数据给回调函数。
>
> 回调函数应该返回接收的数据长度，如果回调函数返回的数据长度与传递给它的长度不一致（即返回长度 != size * nmemb），`libcurl`将终止操作并返回一个错误代码。
>
> 传输完成时，函数将返回一个码，报告操作完成的情况。如果一个返回码不够，还可以使用`CURLOPT_ERRORBUFFER`将`libcurl`指向你的缓冲区，它将存储一个人类可读的消息以报告细节。

#### 重置easy handle

```c
/*
 * NAME curl_easy_reset()
 *
 * DESCRIPTION
 *
 * Re-initializes a CURL handle to the default values. This puts back the
 * handle to the same state as it was in when it was just created.
 *
 * It does keep: live connections, the Session ID cache, the DNS cache and the
 * cookies.
 */
CURL_EXTERN void curl_easy_reset(CURL *curl);
```



#### sample  GET

对于有些协议，下载文件可能包括许多复杂的子过程：日志记录，设置传输模式，选择当前文件夹，最后下载文件数据。使用`libcurl`并不需要关心这一切，你只需要简单地提供一个URL，`libcurl`会做好这些事。

下面的这个例子演示了如何获取网页源码，将其保存到本地文件，并同时将获取的源码输出到控制台上。


 ```c
 /**
  *	@brief libcurl接收到数据时的回调函数
  *
  *	将接收到的数据保存到本地文件中，同时显示在控制台上。
  *
  *	@param [in] buffer 接收到的数据所在缓冲区
  *	@param [in] size 数据长度
  *	@param [in] nmemb 数据片数量
  *	@param [in/out] 用户自定义指针
  *	@return 获取的数据长度
  */
 
 #include <curl/curl.h>

 #include <stdio.h>
 #include <iso646.h>


 size_t process_data(void* buffer, size_t size, size_t nmemb, void* user_p)
 {
    FILE *fp = (FILE *)user_p;
    size_t return_size = fwrite(buffer, size, nmemb, fp);
    cout << (char *)buffer << endl;
     printf("%s\n", (char *)buffer);
     return return_size;
 }
 



 int main(void)
 {
     // 初始化libcurl
    CURLcode return_code;

    return_code = curl_global_init(CURL_GLOBAL_SSL);
    if(CURLE_OK not_eq return_code){
        //cerr << "init libcurl failed." << endl;
        perror("init libcurl failed.\n");
        return -1;
    }

    FILE *fp = fopen("data.html", "ab+");

    // 获取easy handle
    CURL *easy_handle = curl_easy_init();
    if(NULL == easy_handle){
        perror("get a easy handle failed.\n");
        goto CLEANUP;
    }

    // set easy handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://blog.csdn.net/JGood");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &process_data);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);

    // 执行数据请求
    curl_easy_perform(easy_handle);

// 释放资源
 CLEANUP:
    fclose(fp);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}
 ```

```c
// gcc -o easy_down easy_download.c -lcurl

#include <curl/curl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>


// #define CURL_DEBUG 1
#define CURL_WAIT_TIMEOUT_MSECS 60000 // 60S
#define CURL_MULTI_MAX_NUM 5

static size_t receive_data_fun(void* ptr, size_t size, size_t nmemb, void* stream) 
{
    return fwrite(ptr, size, nmemb, (FILE *)stream);
}

static size_t read_head_fun(void* ptr, size_t size, size_t nmemb, void* stream)
{
    char head[2048] = {0};
    memcpy(head, ptr, size * nmemb + 1);
    printf("%s\n", head);
    return size * nmemb;
}

int main(int argc, char* argv[])
{
    if(argc < 3) {
        perror("Usage: easy_down [URL] [save_file]\n");
        return -1;
    }

    curl_global_init(CURL_GLOBAL_SSL);

    char *url = strdup(argv[1]);
    char *filename = strdup(argv[2])head;
    CURL *easy_handle;
    CURLcode res;

    // init
    FILE *save_file = fopen(filename, "w");
    if(save_file == NULL) {
        perror("open save file failed.\n");
        return -2;
    }

    easy_handle = curl_easy_init();
    if(easy_handle){
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, save_file);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, receive_data_fun);
        curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, read_head_fun);
#ifdef CURL_DEBUG
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1);
#endif

        // start down load
        res = curl_easy_perform(easy_handle);
        printf("curl fetch code %d\n", res);
    }

    // release
    if(save_file) 
        fclose(save_file);
    
    if(url)
        free(url);
    if(filename)
        free(filename);

    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    return 0;
}
```



#### libcurl 无法正常工作的时候

传输失败总是有原因的。你可能错误的设置了一些libcurl的属性或者没有正确的理解某些属性的含义，或者是远程主机返回内容无法被正确解析。

有一个黄金规则处理这些问题：将`CURLOPT_VERBOSE`选项设为1，libcurl会输出通信过程中的一些细节。如果使用HTTP，请求头/响应头也会被输出。在`CURLOPT_HEADER`设置为1时，这些信息将出现在消息内容中。

你对相关的协议了解越多，在使用libcurl时，就越不容易犯错。



#### 多线程与连接重用

##### 多线程问题

首先一个基本原则就是：**绝对不应该在线程之间共享同一个libcurl handle，无论是easy handle还是multi handle，一个线程一次只能使用一个handle**。

> `libcurl`是线程安全的，但有一些例外情况，更多信息，参阅:https://curl.haxx.se/libcurl/c/libcurl-thread.html
>
> 1. 信号（signals）
>    信号用于超时失效名字解析（timing out name resolves）。
> 2. SSL/TLS handler
>    libcurl依赖于三方库提供SSL/TSL支持，所以用多线程的方式访问HTTPS或FTPS的URL时，应该满足这些库对多线程操作的一些要求：
>    OpenSSL：http://www.openssl.org/docs/crypto/threads.html#DESCRIPTION
>    GNUTLS：http://www.gnu.org/software/gnutls/manual/html_node/Multi_002dthreaded-applications.html
>    NSS：宣称多线程安全

##### 重用 easy handle

easy handle在完成一次数据通信之后可以被重用，此时该easy handle已经可以再次使用。最好重新使用现有的easy handle，如果你创建另一个easy handle 来执行其他 的数据通信，`libcurl`内部可能会尝试重用上一次创建的连接。

对于一些协议，下载文件可能涉及登录的复杂过程，设置传送模式，改变当前目录并最终传送文件数据。`libcurl`为你解决了所有这些复杂的问题。给定一个文件的URL，`libcurl`将处理所有需要

```c
// 多线程中复用curl连接


void func()
{
   CURL* handle = curl_easy_init();
   pthread_setspecific(g_curl_key, (void*)handle);

   // curl API
   curl = (CURL *)pthread_getspecific(g_curl_key);
   curl_easy_setopt(curl, CURLOPT_URL, url);
   /* enable TCP keep-alive for this transfer */
   curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
   /* keep-alive idle time to 120 seconds */
   curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
   /* interval time between keep-alive probes: 60 seconds */
   curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);

   /* Now specify the POST data */
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);

   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &tmp);
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string);

   /* Perform the request, res will get the return code */
   curl_easy_perform(curl);
}



int main(void)
{
   curl_global_init();
   pthread_key_create(..., func); // 创建线程
   //...
   // 等待线程 ..join()
   curl_global_cleanup();
}
```



#### Headers

有些协议提供独立于数据体的 headers，即meta-data。可以将`CURLOPT_HEADER`设置为1，使 headers 也能出现在数据流中。

更有用的是，通过设置CURLOPT_HEADERDATA设置一个不同的指针传递给普通的写回调函数，从而使回调函数有所不同，libcurl能够将头部与数据分离。

或者，您可以通过使用CURLOPT_HEADERFUNCTION设置一个完全独立的函数来接收消息头。

这些头被一个接一个地传递给回调函数，它使你更容易添加自定义头解析器等。

##### libcurl 使用的HTTP消息头

当使用libcurl发送http请求时，它会自动添加一些http头。我们可以通过`CURLOPT_HTTPHEADER`属性手动替换、添加或删除相应的HTTP消息头。

> **HOST**
>
> http1.1（大部分http1.0)版本都要求客户端请求提供这个信息头。
>
> ** Pragma**
>
> "no-cache"。表示不要缓冲数据。
>
> **Accept**
>
> "*/*"。表示允许接收任何类型的数据。
>
> **Expect**
>
> 以POST的方式向HTTP服务器提交请求时，libcurl会设置该消息头为"100-continue"，它要求服务器在正式处理该请求之前，返回一个"OK"消息。如果POST的数据很小，libcurl可能不会设置该消息头。

##### 自定义操作

 当前越来越多的协议都构建在HTTP协议之上（如：soap），这主要归功于HTTP的可靠性，以及被广泛使用的代理支持（可以穿透大部分防火墙）。 这些协议的使用方式与传统HTTP可能有很大的不同。对此，libcurl作了很好的支持。

##### 自定义请求

HTTP支持GET, HEAD或者POST提交请求。可以设置`CURLOPT_CUSTOMREQUEST`来设置自定义的请求方式，libcurl默认以GET方式提交请求：:

```c
curl_easy_set(easy_handle, CURLOPT_CUSTOMREQUEST,"MYOWNREQUEST");
```

###### 修改headers

HTTP协议提供了消息头，请求消息头用于告诉服务器如何处理请求；响应消息头则告诉浏览器如何处理接收到的数据。在libcurl中，你可以自由的添加这些消息头：:

```c
 // 请求头、响应头多个参数设置。使用curl_slist_append函数一个个参数插入。
/*
 * NAME curl_slist_append()
 *
 * DESCRIPTION
 *
 * Appends a string to a linked list. If no list exists, it will be created
 * first. Returns the new list, after appending.
 */
CURL_EXTERN struct curl_slist *curl_slist_append(struct curl_slist *,
                                                 const char *);


// 最后需要用free all 释放
/*
 * NAME curl_slist_free_all()
 *
 * DESCRIPTION
 *
 * free a previously built curl_slist.
 */
CURL_EXTERN void curl_slist_free_all(struct curl_slist *);
```



```c
struct curl_slist *headers=NULL; /* init to NULL is important */

headers = curl_slist_append(headers, "Hey-server-hey: how are you?");
headers = curl_slist_append(headers, "X-silly-content: yes");

/* pass our list of custom made headers */
curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);

curl_easy_perform(easyhandle); /* transfer http */

curl_slist_free_all(headers); /* free the header list */
```

 对于已经存在的消息头，可以重新设置它的值：

```c
headers = curl_slist_append(headers, "Accept: Agent-007");
headers = curl_slist_append(headers, "Host: munged.host.line");
```

###### 删除 headers

对于一个已经存在的消息头，设置它的内容为空，libcurl在发送请求时就不会同时提交该消息头：:

```c
headers = curl_slist_append(headers, "Accept");
```

###### 获取HTTP响应头

发出HTTP请求后，服务器会返回响应头信息和响应数据，如果仅仅是打印响应头的所有内容，则直接可以通过curl_esasy_setopt(curl, CURLOPT_HEADFUNCTION, print_func)来完成。这里需要获取的是相应头中特定 的信息，比如响应码、cookie列表等：

```c
CURLcode curl_easy_getinfo(CURL* curl, CURLINFO info, ...);

// 第三个参数的类型依据第二个参数
// info 
// CURLINFO_RESPONSE_CODE  	获取应答码
// CURLINFO_HEADER_SIZE    	头大小
// CURLINFO_COOKIELIST	  	cookie列表\
// CURLINFO_RESPONSE_CODE   取得response code，要求第 3 个参数是个 long型的指针。如果TCP就连接不上，值为0。 
// CURLINFO_EFFECTIVE_URL	取得本最终生效的URL，也即是如果有多次重定向，获取的值为最后一次URL，要求第 3 个参数是个 char 型的指针。
// CURLINFO_SIZE_DOWNLOAD 	获取下载字节数，要求第 3 个参数是个 double型的指针。注意，这个字节数只能反映最近一次的下载。
// CURLINFO_SPEED_DOWNLOAD 	获取平均下载数据，该选项要求传递一个double 型参数指针，这个速度不是即时速度，而是下载完成后的速度，单位是 字节/秒
// CURLINFO_TOTAL_TIME  	获取传输总耗时，要求传递一个 double 指针到函数中，这个总的时间里包括了域名解析，以及 TCP 连接过程中所需要的时间。
// CURLINFO_CONTENT_TYPE 	该选项获得 HTTP 中从服务器端收到的头部中的 Content-Type 信息。
// CURLINFO_CONTENT_LENGTH_DOWNLOAD 获取头部content-length，要求第 3 个参数是个 double型的指针。如果文件大小无法获取，那么函数返回值为 -1 。

// 除了获取应答信息外，这个函数还能获取curl的一些内部信息，如请求时间、连接时间等等。
```









### 上传数据到远端站点   PUT

libcurl提供协议无关的方式进行数据传输，因此上传到远程FTP站点与使用PUT请求上传数据到HTTP服务器非常相似。

1. 使用一个easy handle, 
2. 设置`CURLOPT_URL`属性
3. 编写读回调函数，在执行上传的时候，`libcurl`通过回调函数从应用程序将要上传的数据传递到`libcurl`库中。

> 回调函数原型：
> ```c
> size_t function(char* bufptr, size_t size, size_t nitems, void* userp);
> 
> /*
>     bufptr 指向应用程序暂存上传数据的缓冲区
>     nitems 是缓冲区的大小，因此也是在这个调用中我们可以返回给libcurl的最大数据量。
>     userp 自定义数据指针，我们设置它指向我们的struct，用于在应用程序和libcurl之间传递私有数据。libcurl不对该指针做任何操作，它只是简单的传递该指针。
> */
> ```

4. 注册回调函数，设置自定义指针
> ```c
> // 注册回调函数
> curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, read_function);
> // 设置自定义指针
> curl_easy_setopt(easy_handle, CURLOPT_READDATA, &filedata);
> ```

5. 告诉libcurl我们想上传文件：

> ```c
> curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
> ```
> 有些协议在预先知道上传文件大小的情况下，可能无法正确判断上传是否结束，所以最好预先使用`CURLOPT_INFILESIZE_LARGE`属性来告知`libcurl`要上传文件的大小。
>
>```c
> /* 在本例中，文件大小必须是curl_off_t 类型的变量 */
> curl_easy_setopt(easyhandle, CURLOPT_INFILESIZE_LARGE, file_size);
> ```
6. 调用`curl_easy_perform`
上传过程中，`libcurl`会调用用户提供的回调函数来从应用程序获取数据并上传。
程序应该在每次调用中返回尽可能多的数据，因为这可能会使上传执行尽可能快。回调应该返回它在缓冲区中写入的字节数。返回`0`将表示上传结束。

#### sample  PUT

将文件上传到FTP服务器。
```c
// gcc -o easy_put curl_easy_put.c -lcurl

#include <curl/curl.h>

#include <iso646.h>
#include <stdio.h>

/*
 * @ desc: 读取数据的回调 
 */
size_t read_data(void* buffer, size_t size, size_t nmemb, void* user_p)
{
    return fread(buffer, size, nmemb, (FILE *)user_p);
}

int main()
{
    // 初始化libcurl
    CURLcode code;
    code = curl_global_init(CURL_GLOBAL_SSL);
    if(CURLE_OK not_eq code) {
        perror("init libcurl failed.\n");
        return -1;
    }

    FILE *fp = fopen("a.html", "rb");
    if(NULL == fp){
        perror("can't open file.\n");
        curl_global_cleanup();
        return -1;
    }

    // 获取文件大小
    fseek(fp, 0, 2);
    int file_size = ftell(fp);
    rewind(fp);

    // 获取easy handle
    CURL *easy_handle = curl_easy_init();
    if(NULL == easy_handle) {
        perror("get a easy handle failed.\n");
        fclose(fp);
        curl_global_cleanup();
        return -1;
    }

    // 设置easy handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "ftp://127/0.0.1/upload.html");
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, fp);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, fp);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, fp);
    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, file_size);

    // 执行上传操作
    code = curl_easy_perform(easy_handle);
    if(code == CURLE_OK)
        printf("upload successfully.\n");
    else {
        fprintf(stderr, "upload failed. %d\n", code);
    }
        

    // 释放资源
    fclose(fp);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}
```



### HTTP POST

1. 简单的POST
   最常见的版本，如同HTML页面使用<form>表单标签提交数据一样。 通过设置`CURLOPT_POSTFIELDS`,只需要向libcurl提供一个包含数据的字符串即可。

   ```c
   char *data="name=daniel&project=curl";
   curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
   curl_easy_setopt(easyhandle, CURLOPT_URL, "http://posthere.com/");
   
   curl_easy_perform(easyhandle); /* post away! */
   ```

   ```c
   int main(int argc, char** argc)
   {
      code = curl_global_init(CURL_GLOBAL_SSL);
      CURL* easy_handle = curl_easy_init();
   
      curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:2210/Default.aspx");
      // 单个域post
      curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, "name=jgood&address=hangzhou");
   
      curl_easy_cleanup(easy_handle);
      curl_global_cleanup();
   
      return 0;
   }
   ```

2. post 二进制数据
   需要设置Content-Type头信息。 
   因为是二进制数据，所以libcurl不能对数据做`strlen()`操作，以确定大小，因此我们必须告诉libcurl的post数据的大小。 
   在libcurl请求中设置头文件信息是通过一个通用的方法，通过构建一个我们自己的头信息列表，然后将列表传递给libcurl。

   ```c
   struct curl_slist *headers=NULL;
   headers = curl_slist_append(headers, "Content-Type: text/xml");
   
   /* post binary data */
   curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, binaryptr);
   
   /* set the size of the postfields data */
   curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDSIZE, 23L);
   
   /* pass our list of custom made headers */
   curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
   
   curl_easy_perform(easyhandle); /* post away! */
   
   curl_slist_free_all(headers); /* free the header list */
   ```

#### multi-part post

虽然上面的简单示例涵盖了需要HTTP POST操作的大多数情况，但它们不会执行`multi-part form post`。 `multi-part form post`被认为是提交二进制数据（或大量数据）的更好方法<RFC 1867（在RFC 2388中更新）相关定义>。 

> multi-part, 就是在Post提交的时候，有不同的数据单元，每个单元有自己的名称与内容，内容可以是文本的也可以是二进制的。同时，每个数据单元都可以有自己的消息偷，MIME类型，这些数据单元组成一个链表，提交到HTTP服务器。

##### curl_formadd

> libcurl POST支持来创建和发布multi-part form post，但是这需要您自己构建一个formpost并提供给libcurl。 为了使这更容易，libcurl提供`curl_formadd`。 使用此功能，您可以向表单中添加不同的数据单元，然后提交到服务器。 


使用`curl_formadd`，可以向表单添加数据单元。当您完成添加部分时，您将发布整个表单。

```c
// 一个multi-part formposts例子
int miain()
{
   curl_global_init(CURL_GLOBAL_SSL);
   CURL* easy_handle = curl_easy_init();

   // 使用multi-parts form post
   curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:2210/Default.aspx");

   struct curl_httppost *post=NULL;
   struct curl_httppost *last=NULL;

   // 文本数据
   curl_formadd(&post, &last,
               CURLFORM_COPYNAME, "name",
               CURLFORM_COPYCONTENTS, "daniel", CURLFORM_END);
   curl_formadd(&post, &last,
               CURLFORM_COPYNAME, "project",
               CURLFORM_COPYCONTENTS, "curl", CURLFORM_END);
   curl_formadd(&post, &last,
               CURLFORM_COPYNAME, "logotype-image",
               CURLFORM_FILECONTENT, "curl.png", CURLFORM_END);

   // 文本文件中的数据
   curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_FILECONTENT, "ReadMe.txt", CURLFORM_END);


   /* Set the form info */
   curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post);
   curl_easy_perform(easy_handle); /* post away! */

   /* free the post data again */
   curl_formfree(post);
   curl_easy_cleanup(easy_handle);
   curl_global_cleanup();
   
   return 0;
}

```

multi-part formposts 是使用`MIME`样式分隔符和header的part的链表。 
这意味着每个单独的part中的每一个都获得一些描述单个内容类型，大小等的头信息。

这个小例子将显示如何设置header到一个特定的part：

```c
struct curl_slist *headers=NULL;
headers = curl_slist_append(headers, "Content-Type: text/xml");

curl_formadd(&post, &last,
             CURLFORM_COPYNAME, "logotype-image",
             CURLFORM_FILECONTENT, "curl.xml",
             CURLFORM_CONTENTHEADER, headers,
             CURLFORM_END);

curl_easy_perform(easyhandle); /* post away! */

curl_formfree(post); /* free post */
curl_slist_free_all(headers); /* free custom header list */
```

> ​		由于easy_handle上的所有设置都是固着的，后续调用`curl_easy_perform`，也会保持不变，直到设置发生变化为止。
>
> ​		如果您打算在下一个请求中执行一个简单的GET请求，那么需要告诉curl返回到普通的GET请求（easy handle 原始状态）。通过使用`CURLOPT_HTTPGET`选项，你强制一个easy_handle回到GET:
>
> ```c
> curl_easy_setopt(easyhandle, CURLOPT_HTTPGET, 1L);
> ```
>
> 只是将`CURLOPT_POSTFIELDS`设置为`""`或`NULL`不会真正的改变`libcurl`的POST行为，只是POST没有任何数据发送！

##### MIME API

libcurl中仍然支持已弃用的curl_formadd函数。
然而，它不应该再被用于新的设计，使用它的程序应该被转换为MIME API。

> MIME API
> [curl_mime_init](https://curl.haxx.se/libcurl/c/curl_mime_init.html) creates a multi-part body; you can then append new parts to a multi-part body using 
> [curl_mime_addpart](https://curl.haxx.se/libcurl/c/curl_mime_addpart.html). There are three possible data sources for a part: memory usin
> [curl_mime_data](https://curl.haxx.se/libcurl/c/curl_mime_data.html), file using 
> [curl_mime_filedata](https://curl.haxx.se/libcurl/c/curl_mime_filedata.html) and user-defined data read callback using 
> [curl_mime_data_cb](https://curl.haxx.se/libcurl/c/curl_mime_data_cb.html). 
> [curl_mime_name](https://curl.haxx.se/libcurl/c/curl_mime_name.html) sets a part's (i.e.: form field) name, while 
> [curl_mime_filename](https://curl.haxx.se/libcurl/c/curl_mime_filename.html) fills in the remote file name. With 
> [curl_mime_type](https://curl.haxx.se/libcurl/c/curl_mime_type.html), you can tell the MIME type of a part, 
> [curl_mime_headers](https://curl.haxx.se/libcurl/c/curl_mime_headers.html) allows defining the part's headers. When a multi-part body is no longer needed, you can destroy it using 
> [curl_mime_free](https://curl.haxx.se/libcurl/c/curl_mime_free.html).


```c
// 以下示例使用纯文本内容设置两个简单文本部分，然后使用二进制内容设置文件，并上传整个内容。

curl_mime *multipart = curl_mime_init(easyhandle);
curl_mimepart *part = curl_mime_addpart(multipart);
curl_mime_name(part, "name");
curl_mime_data(part, "daniel", CURL_ZERO_TERMINATED);
part = curl_mime_addpart(multipart);
curl_mime_name(part, "project");
curl_mime_data(part, "curl", CURL_ZERO_TERMINATED);
part = curl_mime_addpart(multipart);
curl_mime_name(part, "logotype-image");
curl_mime_filedata(part, "curl.png");

/* Set the form info */
curl_easy_setopt(easyhandle, CURLOPT_MIMEPOST,  multipart);

curl_easy_perform(easyhandle); /* post away! */

/* free the post data again */
curl_mime_free(multipart);
```

> 要为单个表单字段post多个文件，您必须在单独的部分中提供每个文件，所有文件都具有相同的字段名称。虽然函数`curl_mime_subparts`实现了嵌套的multi-part，但是RFC > 7578在第4.3章中不赞成使用这种多文件发布方式。
>
> 若要从已打开的文件指针设置数据源，请使用:
>
> ```c
> curl_mime_data_cb(part, 
> filesize, 
> (curl_read_callback) fread, 
> (curl_seek_callback) fseek, 
> NULL, 
> filepointer);
> ```

##### MIME API为SMTP和IMAP重新访问

除了支持HTTP multi-part formposts字段之外，MIME API还可以用于构建结构化的电子邮件消息并通过SMTP发送它们，或者将此类消息追加到IMAP目录中。

> 结构化电子邮件可能包含几个部分:有些由MUA内联显示，有些是附件。部邮件消息或分还可以被构造为multi-part，例如用来包含另一个电子提供几种文本格式的替代。这可以嵌套到任何级别。
>
> 要构建这样的消息，需要准备第n级multi-part，然后使用函数curl_mime_subparts将其作为父multi-part的源包括进来。一旦它被绑定到其父multi-part，第n级multi-part就属于它，不应该被显式释放。

电子邮件消息数据应该是ascii的，并且行长度是有限的: 幸运的是，一些传输编码是由标准定义的，以支持这种不兼容数据的传输。函数curl_mime_encoder告诉部分在发送之前必须对其源数据进行编码。它还为该部分生成相应的标题。如果您想要发送的部件数据已经在这样的方案中进行了编码，那么不要使用此函数(这会对其进行过度编码)，而是显式地设置相应的部件头。

在发送这样的消息时，libcurl在其前面加上设置为CURLOPT_HTTPHEADER的头列表，作为第0级mime部分头。

下面是一个使用内联纯/html文本替代和以base64编码的文件附件构建电子邮件消息的示例:

```c
 curl_mime *message = curl_mime_init(easyhandle);
 
 /* The inline part is an alternative proposing the html and the text
    versions of the e-mail. */
 curl_mime *alt = curl_mime_init(easyhandle);
 
 /* HTML message. */
 curl_mimepart *part = curl_mime_addpart(alt);
 curl_mime_data(part, "<html><body><p>This is HTML</p></body></html>",
                      CURL_ZERO_TERMINATED);
 curl_mime_type(part, "text/html");
 
 /* Text message. */
 part = curl_mime_addpart(alt);
 curl_mime_data(part, "This is plain text message",
                      CURL_ZERO_TERMINATED);
 
 /* Create the inline part. */
 part = curl_mime_addpart(message);
 curl_mime_subparts(part, alt);
 curl_mime_type(part, "multipart/alternative");
 struct curl_slist *headers = curl_slist_append(NULL,
                   "Content-Disposition: inline");
 curl_mime_headers(part, headers, TRUE);
 
 /* Add the attachment. */
 part = curl_mime_addpart(message);
 curl_mime_filedata(part, "manual.pdf");
 curl_mime_encoder(part, "base64");
 
 /* Build the mail headers. */
 headers = curl_slist_append(NULL, "From: me@example.com");
 headers = curl_slist_append(headers, "To: you@example.com");
 
 /* Set these into the easy handle. */
 curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
 curl_easy_setopt(easyhandle, CURLOPT_MIMEPOST, mime);
```

应该注意的是，将消息附加到IMAP目录需要在上传之前知道消息的大小。因此，在此上下文中不可能包含数据大小未知的部分。

##### 从弃用的form API 到 MIME API的转换

- 在建立multi-part时，必须遵守四项规则:

- 在构造multi-part前，必须先创建出easy handle。

- multi-part通过curl_mime_init(easyhandle)创建。

- 调用curl_mime_addpart(multipart)创建每个part。

完成后，multi-part必须绑定到easy handle使用`CURLOPT_MIMEPOST`而不是`CURLOPT_HTTPPOST`。

> 下面是一些 curl_formadd 到 MIME API 转换的例子:
>
> ```c
> curl_formadd(&post, &last,
>            CURLFORM_COPYNAME, "id",
>            CURLFORM_COPYCONTENTS, "daniel", CURLFORM_END);
>            CURLFORM_CONTENTHEADER, headers,
>            CURLFORM_END);
> ```
>
> becomes:
>
> ```c
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "id");
> curl_mime_data(part, "daniel", CURL_ZERO_TERMINATED);
> curl_mime_headers(part, headers, FALSE);
> ```
>
> 最后一个`curl_mime_headers`参数设置为TRUE将会导致在销毁多个部分时自动释放这些头文件，从而节省一个清理调用[curl_slist_free_all](https://curl.haxx.se/libcurl/c/curl_slist_free_all.html)。
>
> ```c#
> curl_formadd(&post, &last,
>            CURLFORM_PTRNAME, "logotype-image",
>            CURLFORM_FILECONTENT, "-",
>            CURLFORM_END);
> ```
>
> becomes:
>
> ```c
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "logotype-image");
> curl_mime_data_cb(part, (curl_off_t) -1, fread, fseek, NULL, stdin);
> ```
>
> curl_mime_name总是复制字段名。curl_mime_file不支持特殊文件名“-”:要读取打开的文件，使用fread()的回调源。由于数据大小未知，传输将被分块。
>
> ```c
> curl_formadd(&post, &last,
>            CURLFORM_COPYNAME, "datafile[]",
>            CURLFORM_FILE, "file1",
>            CURLFORM_FILE, "file2",
>            CURLFORM_END);
> ```
>
> becomes:
>
> ```c
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "datafile[]");
> curl_mime_filedata(part, "file1");
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "datafile[]");
> curl_mime_filedata(part, "file2");
> ```
>
> curl_mime_name总是复制字段名。curl_mime_file不支持特殊文件名“-”:要读取打开的文件，使用fread()的回调源。已被弃用的多个文件字段的多部分/混合实现被转换为具有相同名称的两个不同部分。由于数据大小未知，传输将被分块。
>
> ```
> curl_easy_setopt(easyhandle, CURLOPT_READFUNCTION, myreadfunc);
> curl_formadd(&post, &last,
>            CURLFORM_COPYNAME, "stream",
>            CURLFORM_STREAM, arg,
>            CURLFORM_CONTENTLEN, (curl_off_t) datasize,
>            CURLFORM_FILENAME, "archive.zip",
>            CURLFORM_CONTENTTYPE, "application/zip",
>            CURLFORM_END);
> ```
>
> becomes:
>
> ```c
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "stream");
> curl_mime_data_cb(part, (curl_off_t) datasize,
>                 myreadfunc, NULL, NULL, arg);
> curl_mime_filename(part, "archive.zip");
> curl_mime_type(part, "application/zip");
> ```
>
> 没有使用CURLOPT_READFUNCTION回调:它被直接设置回调读取函数的部分源数据所替代。
>
> ```c
> curl_formadd(&post, &last,
>            CURLFORM_COPYNAME, "memfile",
>            CURLFORM_BUFFER, "memfile.bin",
>            CURLFORM_BUFFERPTR, databuffer,
>            CURLFORM_BUFFERLENGTH, (long) sizeof databuffer,
>            CURLFORM_END);
> ```
>
> becomes:
>
> ```c
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "memfile");
> curl_mime_data(part, databuffer, (curl_off_t) sizeof databuffer);
> curl_mime_filename(part, "memfile.bin");
> ```
>
> curl_mime_data总是复制初始数据:因此数据缓冲区是空闲的，可以立即重用。
>
> ```c
> curl_formadd(&post, &last,
>            CURLFORM_COPYNAME, "message",
>            CURLFORM_FILECONTENT, "msg.txt",
>            CURLFORM_END);
> ```
>
> becomes:
>
> ```c
> part = curl_mime_addpart(multipart);
> curl_mime_name(part, "message");
> curl_mime_filedata(part, "msg.txt");
> curl_mime_filename(part, NULL);
> ```
>
> 使用`curl_mime_filedata`会将远程文件名设置为副作用:因此有必要为CURLFORM_FILECONTENT模拟清除该文件名。

#### Post 传递信息

See [curl_easy_getinfo](https://curl.haxx.se/libcurl/c/curl_easy_getinfo.html).



### 身份验证

#### 密码验证

许多协议在客户端向服务器发送请求时都要求提供用户名与密码，libcurl提供了多种方式来设置。

1. 一些协议支持在URL中直接指定用户名和密码
   `protocol://user:password@example.com/path/`。`libcurl`能正确是识别这种URL中的用户名与密码并执行相应的操作。如果提供的用户名与密码中有特殊字符，首先应该对其进行URL编码（U%XX，其中XX是一个两位数的十六进制数字。需要一定的编码将原始字符串转换成url编码或base64或者其他的序列化编码才能传输）。

2. 也可以通过`CURLOPT_USERPWD`来设置用户名与密码
   `user:passwd`
   ```c
   curl_easy_setopt(easyhandle, CUROPT_USERPWD, "user_name:password");
   ```

3. 如果通过代理服务器进行访问，代理服务器可能要求提供用户名和密码进行身份验证。libcurl提供了另一个选项`CURLOPT_PROXYUSERPWD`。它的使用非常类似于CURLOPT_USERPWD选项。
   ```c
   curl_easy_setopt(easyhandle, CURLOPT_PROXYUSERPWD, "user_name:password`"); 
   ```

4. Unix存储FTP用户名和密码在$HOME/.netrc文件中。要使curl使用这个文件，使用CURLOPT_NETRC选项:
   ```c
   curl_easy_setopt(easyhandle, CURLOPT_NETRC, 1L);
   ```
   该文件是私有的，只有用户可以阅读它，因为它可能包含纯文本的密码。
   libcurl能够使用这个文件确定为特定主机使用哪一组用户名和密码。
   作为对常规功能的扩展，libcurl还为非ftp协议(如HTTP)支持这个文件。

   这里有个很基础的例子来展示`.netrc`文件看起像什么：
   ```
   machine myhost.mydomain.com
   login userlogin
   password secretword
   ```

5. 有时密码不是可选的，比如使用SSL私钥进行安全传输时。

   在所有这些示例中，密码都是可选的，或者至少可以不使用密码，让libcurl尝试在不使用密码的情况下完成它的工作
要将已知的私钥密码传递给libcurl:

   ```c
   curl_easy_setopt(easyhandle, CURLOPT_KEYPASSWD, "keypassword");
   ```

#### HTTP 鉴权

上一章介绍了如何在libcurl中，对需要身份验证的URL设置用户名与密码。在使用HTTP协议时，客户端有很多种方式向服务器提供验证信息。默认的HTTP验证方法是"Basic”，它将用户名与密码以明文的方式、经Base64编码后保存在HTTP请求头中，发往服务器。当然这不太安全。


当前版本的`libcurl`支持的验证凡是有：`Basic`，`Digest`，`NTLM`，`Negotiate（SPNEGO）`。 你可以通过`CURLOPT_HTTPAUTH`设置具体的验证方式：

```c
curl_easy_setopt(easyhandle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST); 
```

向代理服务器发送验证信息可以通过`CURLOPT_PROXYAUTH`设置验证方式:

```c
 curl_easy_setopt(easyhandle, CURLOPT_PROXYAUTH, CURLAUTH_NTLM);
```

上述两种访问途径，都可以设置多种验证方式，通过将bit模式定义的宏以位或的形式连接即可。
`libcurl`会首先发送一个请求并接受返回以获知服务器支持的验证类型，然后从中选择最安全的一种。（增加了一次HTTP req/resp的消耗)

```c
curl_easy_setopt(easyhandle, CURLOPT_HTTPAUTH,  CURLAUTH_DIGEST|CURLAUTH_BASIC);
```

为了方便，可以使用`CURLAUTH_ANY`，它允许libcurl选择任何它支持的验证方式。

当请求多个类型时，libcurl会根据自己的内部偏好顺序选择它认为“最好”的可用类型。



#### Proxies 代理

> 代理: 一个通过验证的用户扮演另一个用户。

libcurl支持SOCKS和HTTP代理。使用代理，`libcurl`会把用户输入的URL提交给代理服务器，而不是直接根据URL去访问远程资源。

当前版本`libcurl`并未完全支持SOCKS代理的所有功能。

对于HTTP代理:HTTP对实际请求施加了某些限制。即使请求的URL不是一个合法的HTTP URL（比方你提供了一个ftp的url），它仍然会先被提交到HTTP代理。

##### proxy options

```c
// 设置libcurl使用的代理服务器 endpoint
curl_easy_setopt(easy_handle, CURLOPT_PROXY, "proxy-host.com:8080");

// 分开设置 endpoint
curl_easy_setopt(easy_handle, CURLOPT_PROXY, "proxy-host.com");
curl_easy_setopt(easy_handle, CURLOPT_PROXYPORT, "8080");

// 有些代理在需要用户身份验证:
curl_easy_setopt(easy_handle, CURLOPT_PROXYUSERPWD, "user:password");

// 告诉libcurl它是使用哪种代理(默认为HTTP代理):
curl_easy_setopt(easyhandle, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
```

##### 环境变量

libcurl会自动检查并使用一组环境变量，并根据这些环境变量来确定要使用的代理服务器。

环境变量的名称格式一般是：`"[protocol]_proxy"`(注意小写)。例如输入一个HTTP的URL，那么名称为 `"http_proxy"  `环境变量就会被检测是否存在，如果存在，libcurl会使用该环境变量指定的代理。相同的规则也适用于FTP。

这些环境变量的值的格式必须是这样的：`"[protocol://][user:password@]machine[:port]"`。libcurl会忽略掉`[protocol://]`，如果没有提供端口号，libcurl使用该协议的默认端口。

有两个比较特殊的环境变量：`'all_proxy'`与`'no_proxy'`。如果一个URL所对应的协议，它的环境变量没有设置，那么`'all_proxy'`指定的代理将被使用。`'no_proxy'`则指定了一个不应被使用的代理主机的列表。例如：no_proxy的值是'192.168.1.10'，即使存在`http_proxy`，它的值也是'192.168.1.10'，'192.168.1.10'也不会被作为代理。`no_proxy=”*”`表示不允许使用任何代理。

显式地将`CURLOPT_PROXY`属性设置为空，可以禁止libcurl检查并使用环境变量来使用代理。

##### SSL 和代理

SSL为点到点通信提供安全保障。它包含一些强壮的加密措施和其他安全检测，这使得上面讲到的代理方式不适用于SSL。除非代理服务器提供专用通道，对进出该代理服务器的数据不作任何检测或禁止。通过HTTP代理服务器打开SSL连接，意味着代理服务器要直接连接到目标主机的指定端口。因为代理服务器对在专用通道上传输的数据的类型毫无所知，所以它往往会使某些机制失效，如缓存机制。许多组织只允许在443端口上创建这种类型的数据通道。

##### 代理通道

正如上面讲到的，要使SSL工作必须在代理服务器创建专用数据通道，通常专用通道只被限制应用于HTTPS。通过HTTP代理在应用程序与目标之间创建一个专用数据通道，应该预防在该专有通道上执行非HTTP的操作，如进行FTP上传或执行FTP命令。代理服务器管理员应该禁止非法的操作。

通过CURLOPT_HTTPPROXYTUNNEL属性来告诉libcurl使用代理通道：

```c
curl_easy_setopt(easy_handle, CURLOPT_HTTPPROXYTUNNEL, 1L); 
```

有时候你想通过代理通道执行平常的HTTP操作，而实际上却可能使你不经过代理服务器而直接与远程主机进行交互。libcurl不会代替这种新引入的行为。

##### Proxy Auto-Config（代理自动配置）

许多浏览器支持自动配置代理，例如网景，`libcurl`并不支持这些。
libcurl没有办法解释或计算Javascript，因此它不支持这一点。

如果你想自己发明这个复杂的轮子，下面的建议在过去都被提及且使用：

- 根据Javascript的复杂性，编写一个脚本，把它翻译成另一种语言，并执行它。
- 读Javascript代码，用另一种语言重写相同的逻辑。
- 实现一个Javascript解释器;过去人们已经成功地使用了Mozilla的Javascript引擎。
- 要求你的管理员停止为一个静态代理设置类似的选项。



### 强制分块传输编码

请求在执行非get请求时设置了自定义消息头：“Transfer-Encoding: chunked”，libcurl将分块提交数据，即使要上传的数据的大小可能已经知道。默认情况下，如果上传数据大小未知，libcurl通常会自动切换到分块上传。

### FTP

并不是所有的协议都像HTTP那样，通过消息头告诉服务器该如何处理请求。对于FTP，就要使用另外的方式。

#### FTP 自定义命令

发送自定义的命令到FTP服务器，以为着你发送的命令必须是能被FTP必须能被FTP服务器理解的命令（FTP协议中定义的命令参考RFC959）。

一个在传输操作前删除给定文件的小例子:

```c
headers = curl_slist_append(headers, "DELE file-to-remove");

/* pass the list of custom commands to the handle */
curl_easy_setopt(easyhandle, CURLOPT_QUOTE, headers);

curl_easy_perform(easyhandle); /* transfer ftp data! */

curl_slist_free_all(headers); /* free the header list */
```
FTP服务器执行命令的顺序，同这些命令被添加到列表中顺序是一致的。发往服务器的命令列表中，只要有一个命令执行失败，FTP服务器就会返回一个错误代码，此时libcurl将直接返回`CURLE_QUOTE_ERROR`，不再执行剩余的FTP命令。

将`CURLOPT_HEADER`设置为1，libcurl获取目标文件的信息，并以HTTP消息头的样式来输出消息头。

#### FTP自定义请求
使用`CURLOPT_CUSTOMERQUEST`，可以向FTP服务器发送命令。“NLST”是FTP默认的列出文件列表的命令。下面的代码用于列出FTP服务器上的文件列表：
```c
int main(int argc, char** argv)
{
   curl_global_init(CURL_GLOBAL_SSL);
   CURL* easy_handle = curl_easy_init();
   
   curl_easy_setopt(easy_handle, CURLOPT_URL, "ftp://127.0.0.1/");
   curl_easy_setopt(easy_handle, CURLOPT_CUSTOMERQUEST, "NLST");

   curl_easy_perform(easy_handle);

   curl_easy_cleanup(easy_handle);
   curl_global_cleanup();

   return 0;
}
```

#### 我们需要的FTP特性

在使用FTP协议进行数据传输的时候，需要创建两个连接。第一个连接用于传输控制命令，另一个连接用于传输数据。（关于FTP的通信过程，请参考这篇文章：http://www.wangjia.net/bo-blog/post/698/）。 FTP通信需要创建两个连接这个事实往往被很多人忽略。根据第二个连接的发起方是谁，可以分为主动模式与被动模式。libcurl对此都提供了支持。libcurl默认使用被动模式，因为被动模式可以方便的穿透防火墙，NAT等问题。在被动模式下，libcurl要求ftp服务器打开一个新的端口监听，然后libcurl连接该端口用于数据传输。如果使用主动模式，程序必须告诉FTP服务器你监听的IP与端口，通过设置CURLOPT_FTPPORT属性来完成。。





### easy handle 共享数据及初始化

当使用easy interface时，您可以在easy handle 之间共享一些数据，并且当您使用多界面时，一些数据会自动共享。

当您向多个句柄添加easy handle时，这些easy handle将自动共享大量的数据，在使用easy interface时，这些数据会保存在每个easy handle基础上。

DNS缓存在多个easy handle之间共享，使得随后的名称解析更快，并且在持久连接和连接重用的连接池也被共享。 如果你使用easy interface，你仍然可以通过使用shared interface 在特定的 handle之间共享这些，请参见libcurl-share。

有些数据不会自动在easy handle之间共享，例如Cookie，所以唯一的方法是与share interface共享。

#### 持久化的好处

当执行多个请求时，应该多次重复使用同一个easy handle。

每次执行完`curl_easy_perform`，`licurl`会继续保持与服务器的连接。接下来的请求可以使用这个连接而不必创建新的连接（如果目标主机是同一个的话）。这样可以减少网络开销。

即使连接被释放了，`libcurl`也会缓存这些连接的会话信息，这样下次再连接到目标主机上时，就可以使用这些信息，从而减少重新连接所需的时间。

FTP连接可能会被保存较长的时间。因为客户端要与FTP服务器进行频繁的命令交互。对于有访问人数上限的FTP服务器，保持一个长连接，可以使你不需要排除等待，就直接可以与FTP服务器通信。

**libcurl会缓存DNS的解析结果。
在今后的libcurl版本中，还会添加一些特性来提高数据通信的效率。 
每个`easy handle`都会保存最近使用的几个连接，以备重用。默认是5个。可以通过`CURLOPT_MAXCONNECTS`属性来设置保存连接的数量。**

如果你不想重用连接，将`CURLOPT_FRESH_CONNECT`属性设置为1。这样每次提交请求时，`libcurl`都会先关闭以前创建的连接，然后重新创建一个新的连接。也可以将`CURLOPT_FORBID_REUSE`设置为`1`，这样每次执行完请求，连接就会马上关闭。

#### Cookie

cookie是一个键值对的集合，HTTP服务器发给客户端的cookie，客户端提交请求的时候，也会发送到服务器。服务器可以根据cookie来跟踪用户的会话信息。
cookie有过期时间，超时后cookie就会失效。cookie有域名和路径限制，cookie只能发给指定域名和路径的HTTP服务器。
cookie以消息头“Set-Cookie”的形式从HTTP服务器发送到客户端，客户端以消息头“Cookie”的形式将cookie提交到HTTP服务器。

在libcurl中，可以通过`CURLOPT_COOKIE`属性来设置发往服务器的cookie：

```c
curl_easy_setopt(easyhandle, CURLOPT_COOKIE, "name1=var1; name2=var2;");
```

下面的例子演示了如何使用libcurl发送cookie信息给HTTP服务器：

```c
int main(int argc, char** argv)
{
   curl_global_init(CURL_GLOBAL_SSL);
   CURL* easy_handle = curl_easy_init();

   curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:2210/Default.aspx");
   curl_easy_setopt(easy_handle, CURLOPT_COOKIE, "name=JGood;address=HangZhou");

   curl_easy_perform(easy_handle);
   curl_easy_cleanup(easy_handle);

   curl_global_cleanup();

   return 0;
}
```

在实际的应用场景中，你可能需要保存服务器发送给你的cookie，并在接下来的请求中，把这些cookie一并发往服务器。所以，可以把上次从服务器收到的所有响应头信息保存到文本文件中，当下次需要向服务器发送请求时，通过`CURLOPT_COOKIEFILE`告诉libcurl从该文件中读取cookie信息。

设置`CURLOPT_COOKIEFILE`属性意味着激活libcurl的cookie parser。在cookie parser被激活之后，cookie 信息将被保存在内存中，在接下来的请求中，libcurl会自动将这些cookie信息添加到消息头里，你的应用不需要做任何事情。大多数情况下，这些已经足够了。需要注意的是，通过`CURLOPT_COOKIEFILR`属性来激活cookie parser，给`CURLOPT_COOKIEFILE`属性设置的一个保存cookie信息的文本文件路径，可能并不需要在磁盘上实际存在。

如果你需要使用NetScape或者FireFox浏览器的cookie文件，你只要用这些浏览器的cookie文件的路径来初始化`CURLOPT_COOKIEFILE`属性，libcurl会自动分析cookie文件，并在接下来的请求过程中使用这些cookie信息。

libcurl甚至能够把接收到的cookie信息保存成能被NetScape/Mozilla的浏览器所识别的cookie文件，通过把这些称为cookie-jar。通过设置`CURLOPT_COOKIEJAR`选项，在调用`curl_cleanup`释放easy handle的时候，所有的这些 cookie信息都会保存到cookie-jar文件中。这就使得cookie信息能在不同的easy handle甚至在浏览器之间实现共享。







## multi interface

上面介绍的easy handle 以同步的方式进行数据传输，`curl_easy_perform`会一直阻塞到数据传输完毕后返回，且一次操作只能发送一次请求，如果要同时发送多个请求，必须使用多线程用多个easy handle去发送。

multi interface 以一种简单的、非阻塞的方式进行传输，它允许在一个线程中，同时提交多个相同类型的请求。在使用multi interface 之前，应该掌握easy interface的基本使用。因为multi interface 是建立在easy interface基础之上的，它只是简单的将多个easy handle 添加到一个 multi stack，然后同时传输而已。

使用multi interface 很简单，首先使用 curl_multi_init() 函数创建一个multi handle，然后使用 curl_easy_init()创建一个或多个easy handle，并按照上面击掌介绍的接口正常的设置相关的属性，然后通过`curl_add_handle` 将这些 easy handle 添加到 multi handle，然后调用 `curl_multi_perform`进行数据传输。

`curl_multi_perform `是异步非阻塞的函数，如果它返回CURLM_CALL_MULTI_PERFORM，表示数据通信正在进行。

通过select()来操作multi interface 将会使工作变得简单（每个easy handle在底层就是一个socket,通过select()来管理这些socket，在有数据可读/可写/异常的时候，通知应用程序）。在调用select()函数之前，应该使用 curl_multi_fdset 来初始化 fd_set 变量。

select() 函数返回时，说明受管理的底层socket可以执行相应的操作（接收数据或发送数据， 或者链接已经断开），此时应该马上调用`curl_multi_perform`，libcurl将会执行相应操作。使用select()时，应该设置一个较短的超时时间。在调用select()之前，不要忘记通过`curl_multi_fdset`来初始化fd_set，因为每次操作,fd_set中的文件描述符可能都不一样。

如果想终止multi stack 中的某以个easy handle的数据通信，可以调用``curl_multi_remove_handle` 函数将其从multi stack中取出。并释放掉easy handle(curl_easy_cleanup(handle))

当multi stack中的一个 easy handle完成数据传输的时候，同时运行的传输任务数量就会减少一个。当数据量降到0的时候，说明所有数据传输已经完成。

`curl_multi_info_read`用于获取当前已经完成的传输任务信息，它返回每一个easy handle的CURLcode状态码。可以根据这个状态码来判断每个easy handle传输是否成功。

>**multi interface 使用方法：**
>
>- curl_multi_init 初始化一个multi handle 对象
>- 初始化多个easy handler 对象，使用curl_easy_setopt 进行相关设置
>- 调用curl_multi_add_handle 把 easy handler 添加到multi curl 对象中
>- 添加完毕后，执行curl_multi_perform 方法进行并发的访问
>- 访问结束后，curl_multi_remove_handle 移除相关easy hanle对象，并使用curl_easy_cleanup进行资源释放。
>- 最后使用curl_multi_cleanup释放multi handle 资源。

### API

#### 初始化

```c
CURLM* curl_multi_init();
CURLMcode curl_multi_cleanup(CURLM* multi_handle);
```

#### 添加删除

```c
CURLMode curl_multi_add_handle(CURLM* multi_handle, CURL* easy_handle);
CURLcode curl_multi_remove_handle(CURLM* multi_handle, CURL* easy_handle);
```

#### 执行

```c
CURLMcode curl_multi_perform(CURLM* multi_handle, int* running_handles);
```

#### 等待超时 

```c
CURLMcode curl_multi_fdset(CURLM* multi_handle, fd_set* read_fd_set, fd_set* write_fd_set, fd_set* exc_fd_set. int* max_fd);

CURLMcode curl_multi_wait(CURLM* multi_handle, struct curl_waitfd extra_fds[], unsigned int extra_nfds, int timeout_ms, int* numfds);
```



旧的API使用`curl_multi_fdset`设置select或者poll模型触发。官网example：

```c
#ifdef _WIN32
	#define SHORT_SLEEP Sleep(100)
#else
	#define SHORT_SLEEP usleep(100000)
#endif

fd_set fdread;
fd_set fdwrite;
fd_set fdexcept;
int maxfd = -1;

long curl_time;

curl_multi_timeout(multi_handle, &curl_time);
if(curl_time < 0) curl_time = 1000;

timeout.tv_sec = curl_time / 1000;
timeout.tv_usec = (curl_time % 1000) * 1000;

FD_ZERO(&fdread);
FD_ZERO(&fdwrite);
FD_ZERO(&fdexcept);

// get file descriptors from the transfers.
mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcept, & maxfd);

if(maxfd == -1) {
    SHORT_SLEEP;
    rc = 0;
}
else {
    select(maxfd+1, &fdread, &fdwrite, &fdexcept, &timeout);
}

switch(rc) {
    case -1:
     	// select errpr
     	break;
    case 0:
     	// timeout or readavle/writable/ sockets
     	curl_multi_perform(multi_handle, &still_running);
        break;
}

/* if there are still transfers, loop! */
```

新的API中，官网更推荐使用`curl_multi_wait`的方式来实现，实现方便，也可以避免select中的file descriptors 不能大于 1024 的问题。`curl_multi_wait`会轮询multi上的所有easy 句柄，一直阻塞直到至少有一个被触发或者超时。

如果multi句柄正因为网络延时而挂起，会有一个更短更精确的时间来替代我们自己设置的超时时间timeout_ms。

`curl_wait`参数添加需要监听的socket，wait返回后，numfds返回被触发的事件数量，若为0表示超时或者没有事件等待。numfds的值包括multi栈上的和extra_fds新加的之和。

```c
CURL* easy_handle;
CURLM* multi_handle;

// add the individual easy handle
curl_multi_add_handle(multi_handle, easy_handle);

do {
    CURLcode mc;
    int numfds;
    
    mc = curl_multi_perform(multi_handle, &still_running);
    
    if(mc == CURL_OK){
        // wait for activity, timeout or "nothing".
        mc = curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
    }
    
    if(mc != CURLM_OK) {
        fprintf(stderr, "curl_multi failed, code %d.\n", mc);
        beak;
    }
    
      /* 'numfds' being zero means either a timeout or no file descriptors to
     wait for. Try timeout on first occurrence, then assume no file
     descriptors and no file descriptors to wait for means wait for 100
     milliseconds. */
    if(!numfds) {
        repeats++; // count number of repeated zero numfds
        if(repeats > 1) WAITMS(100); // sleep 100 ms
    }
    else repeats = 0;
} while(still_running);

curl_multi_remove_handle(multi_handle, easy_handle);
```

对于select和curl_multi_wait两种方式，都可以使用curl_multi_timeout获取一个合适的超时时间，当然，超时时间也可以我们自己设置。

##### 获取每个perform的执行信息

```c
CURLMsg* curl_multi_info_read(CULRM* multi_handle, int* msg_in_queue);
```

```c
/* call curl_multi_perform or curl_multi_socket_action first, then loop
   through and check if there are any transfers that have completed */
struct CURLMsg *m;
do {
  int msgq = 0;
  m = curl_multi_info_read(multi_handle, &msgq);
  if(m && (m->msg == CURLMSG_DONE)) {
    CURL *e = m->easy_handle;
    transfers--;
    curl_multi_remove_handle(multi_handle, e);
    curl_easy_cleanup(e);
  }
} while(m);
```



### sample

```c
// 使用multi interface进行网页抓取
// gcc -o multi_get curl_multi_get.c -lcurl

#include <curl/curl.h>

#include <stdio.h>

int main(int argc, char** argv)
{
    // 初始化
    curl_global_init(CURL_GLOBAL_SSL);
    CURLM *multi_handle = NULL;
    CURL* easy_arr[2] = {NULL, NULL};

    extern size_t save_sina_page(void *buffer, size_t size, size_t count, void *user_p);
    extern size_t save_sohu_page(void *buffer, size_t size, size_t count, void *user_p);
    FILE *fp_sina = fopen("sina.html", "ab+");
    FILE *fp_sohu = fopen("sohu.html", "ab+");

    multi_handle = curl_multi_init();

    // set easy handle
    easy_arr[0] = curl_easy_init();
    curl_easy_setopt(easy_arr[0], CURLOPT_URL, "http://www.sina.com.cn");
    curl_easy_setopt(easy_arr[0], CURLOPT_WRITEFUNCTION, &save_sina_page);
    curl_easy_setopt(easy_arr[0], CURLOPT_WRITEDATA, fp_sina);

    easy_arr[1] = curl_easy_init();
    curl_easy_setopt(easy_arr[1], CURLOPT_URL, "http://www.sohu.com");
    curl_easy_setopt(easy_arr[1], CURLOPT_WRITEFUNCTION, &save_sohu_page);
    curl_easy_setopt(easy_arr[1], CURLOPT_WRITEDATA, fp_sohu);

    // 添加到multi stack
    for (int i = 0; i < 2; ++i)
        curl_multi_add_handle(multi_handle, easy_arr[i]);

    int running_handle_count;
    while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count)) {
        printf("rinning handle: %d\n", running_handle_count);
    }

    while(running_handle_count){
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int max_fd;
        fd_set fd_read;
        fd_set fd_write;
        fd_set fd_except;

        FD_ZERO(&fd_read);
        FD_ZERO(&fd_write);
        FD_ZERO(&fd_except);

        curl_multi_fdset(multi_handle, &fd_read, &fd_write, &fd_except, &max_fd);
        int return_code = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &tv);
        if(SO_ERROR == return_code){
            perror("select error.\n");
            break;
        }
        else {
            while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count))
                printf("running handle: %d\n", running_handle_count);
        }
    }

    // 释放资源
    fclose(fp_sina);
    fclose(fp_sohu);
    curl_easy_cleanup(easy_arr[0]);
    curl_easy_cleanup(easy_arr[1]);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}


extern size_t save_sina_page(void *buffer, size_t size, size_t count, void *user_p)
{
    return fwrite(buffer, size, count, (FILE *)user_p);
}
extern size_t save_sohu_page(void *buffer, size_t size, size_t count, void *user_p)
{
     return fwrite(buffer, size, count, (FILE *)user_p);
}

```

```c
// multi interface 并发下载
// 仍有bug
// gcc -o multi_down multi_down.c -lcurl

#include <curl/curl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>


//#define CURL_DEBUG 1
#define CURL_WAIT_TIMEOUT_MSECS 1000 //1s
#define CURL_MULTI_MAX_NUM 5

typedef struct curl_obj {
    CURL *curl_handle;
    FILE *save_file;
    char *fetch_url;
    size_t (*receive_data_fun)(void *, size_t , size_t , void *);
    size_t (*read_head_fun)(void *, size_t , size_t , void *);
} curl_obj_t;

static size_t receive_data_fun(void *buf, size_t size, size_t nmemb, void *stream)
{
    return fwrite(buf, size, nmemb, (FILE*)stream); 
}

static size_t read_head_fun(void* buf, size_t size, size_t nmemb, void* stream)     
{
    // if stream : free(stream); should be malloc here and free outsize
    stream = malloc(size * nmemb);
    memcpy(stream, buf, size * nmemb);
    printf("%s\n", (char*)stream);
    free(stream);
    return size * nmemb;
}

int main(int argc, char* argv[])
{
    if(argc < 3){
        perror("Usage: easy_down [URL] [save_file]\n");
        return -1;
    }

    char *outfile_names[CURL_MULTI_MAX_NUM] = {0};
    curl_obj_t obj[CURL_MULTI_MAX_NUM] = {NULL, NULL, NULL, NULL, NULL};
    int multi_h_num = ((argc - 2) < CURL_MULTI_MAX_NUM ? (argc - 2) : CURL_MULTI_MAX_NUM);

    CURLM *multi_handle = curl_multi_init();
    for (int i = 0; i < multi_h_num; i++){
        obj[i].fetch_url = strdup(argv[i + 1]); // need free
        char out_filename[1024];
        //sprintf(out_filename, "storage/external_storage/%s", strrchr(argv[i + 1], '/'));
        sprintf(out_filename, "storage/external_storage/%s", argv[i+2]);
        printf("----save_file[%d] [%s]\n", i, out_filename);
        obj[i].save_file = fopen(out_filename,"w"); 
        if(!obj[i].save_file){ 
            printf("ERROR----fail!!!\n"); 
            goto release; 
        } 
         
        obj[i].curl_handle = curl_easy_init(); 
        obj[i].receive_data_fun = receive_data_fun; 
        obj[i].read_head_fun = read_head_fun; 
        if(obj[i].curl_handle){ 
            curl_easy_setopt(obj[i].curl_handle, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(obj[i].curl_handle, CURLOPT_URL, obj[i].fetch_url);//set down load url 
            curl_easy_setopt(obj[i].curl_handle, CURLOPT_WRITEDATA, obj[i].save_file);//set download file 
            curl_easy_setopt(obj[i].curl_handle, CURLOPT_WRITEFUNCTION, obj[i].receive_data_fun);//set call back fun             
#ifdef CURL_DEBUG 
            curl_easy_setopt(obj[i].curl_handle, CURLOPT_VERBOSE, 1); 
#else 
            curl_easy_setopt(obj[i].curl_handle, CURLOPT_HEADERFUNCTION, obj[i].read_head_fun);//set call back fun 
#endif  
            if(multi_handle) curl_multi_add_handle(multi_handle, obj[i].curl_handle);//add task 
        }else{ 
            printf("fetch [%s]----ERROR!!!\n",obj[i].fetch_url ); 
            //goto release; 
        } 
    } 
     
    int still_running = 0,repeats = 0;
    curl_multi_perform(multi_handle, &still_running); 
    do { 
        int numfds = 0; 
        long timeout_ms = CURL_WAIT_TIMEOUT_MSECS; 
        curl_multi_timeout(multi_handle, &timeout_ms);//get timeout ms instead 
        CURLMcode retcode = curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &numfds); 
        if (retcode != CURLM_OK) { 
            printf("ERROR----curl_multi_wait  errorcode[%d]\n",retcode);           
            break; 
        } 
        /* 'numfds' being zero means either a timeout or no file descriptors to
           wait for. Try timeout on first occurrence, then assume no file
           descriptors and no file descriptors to wait for means wait for 10
           milliseconds. */
        if(!numfds) {
            if(repeats++ > 60){
                printf("ERROR----timeout break!!! \n");           
                break;
            }else{
                usleep(10*1000);  /* sleep 10 milliseconds */
                continue;
            }
        }
        else{
            repeats = 0;
        }
          
        retcode = curl_multi_perform(multi_handle, &still_running); 
        if (retcode != CURLM_OK) { 
            printf("ERROR----curl_multi_perform  errorcode[%d]\n",retcode); 
            break; 
        } 
        //printf("still_running[%d]\tnumfds[%d]\n",still_running,numfds );

        int msgs_left = 0; 
        CURLMsg *msg = NULL; 
        while ((msg = curl_multi_info_read(multi_handle, &msgs_left)) != NULL){ 
            if (msg->msg == CURLMSG_DONE) { 
                long http_response_code = -1; 
                char* http_url = NULL; 
                curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &http_response_code); 
                curl_easy_getinfo(msg->easy_handle, CURLINFO_EFFECTIVE_URL, &http_url);
                printf("[%s]fetch done, response[%d]\n",http_url,http_response_code ); 
            } 
        } 
    } while (still_running); 
    
release:  
     printf("release\n"); 
     for(int i=0;i<multi_h_num;i++){ 
        if(obj[i].curl_handle){ 
            curl_multi_remove_handle(multi_handle, obj[i].curl_handle); 
            curl_easy_cleanup(obj[i].curl_handle); 
        } 
        if(obj[i].save_file){ 
            fclose(obj[i].save_file); 
            obj[i].save_file = NULL; 
        } 
        if(obj[i].fetch_url){ 
            free(obj[i].fetch_url); 
            obj[i].fetch_url = NULL; 
        } 
    } 
    if(multi_handle !=NULL){
        curl_multi_cleanup(multi_handle); 
    }
 
    return 0; 
}
```



## libcurl with c++

在连接libcurl时使用C ++而不是C时，基本上只有一件事要注意：

回调不能是非静态类成员函数(可以是普通函数或者类的静态成员函数)

```c++
class AClass {
    static size_t write_data(void *ptr, size_t size, size_t nmemb,
                             void *ourpointer)
    {
      /* do what you want with the data */
    }
};
```

### c++ sample

```c++
//#define CURL_STATICLIB                //如果是静态库方式，需要包含这句
 
#include <curl/curl.h>

#include <iostream>
#include <list>
#include <string>
#ifdef _DEBUG
    #pragma comment(lib,"libcurld.lib")
#else
    #pragma comment(lib,"libcurl.lib")
#endif
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "wldap32.lib" )
#pragma comment(lib, "Advapi32.lib")
   
   
std::wstring AsciiToUnicode(const std::string& str)
{
    // 预算-缓冲区中宽字节的长度  
    int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    // 给指向缓冲区的指针变量分配内存  
    wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
    // 开始向缓冲区转换字节  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
    std::wstring ret_str = pUnicode;
    free(pUnicode);
    return ret_str;
}
   
std::string UnicodeToUtf8(const std::wstring& wstr)
{
    // 预算-缓冲区中多字节的长度  
    int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    // 给指向缓冲区的指针变量分配内存  
    char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
    // 开始向缓冲区转换字节  
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
    std::string ret_str = pAssii;
    free(pAssii);
    return ret_str;
}
   
//ANSI转UTF8
std::string AsciiToUtf8(const std::string& str)
{
    return UnicodeToUtf8(AsciiToUnicode(str));
}

//UTF8转ANSI
std::string Utf8toAscii(const std::string strUTF8)
{
    std::string  strAnsi = "";
    //获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
    UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
    WCHAR *wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    CHAR *szBuffer = new CHAR[nLen + 1];
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;
    strAnsi = szBuffer;
    //清理内存
    delete[]szBuffer;
    delete[]wszBuffer;
    return strAnsi;
}

// reply of the requery  
size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
    if (stream == NULL || ptr == NULL || size == 0)
        return 0;

    size_t realsize = size * nmemb;
    std::string *buffer = (std::string*)stream;
    if (buffer != NULL)
    {
        buffer->append((const char *)ptr, realsize);
    }
    return realsize;
    /*
    std::string *str = (std::string*)stream;
    (*str).append((char*)ptr, size*nmemb);
    return size * nmemb;
    */
}

/*
功能：get http数据
参数：url：请求字符串。如果请求带参数数据，直接拼凑到url后面；比如：http://127.0.0.1:8080/api/Accounts/Login?uername=admin&password=123
listRequestHeader：请求头数据列表。
bResponseIsWithHeaderData：bool类型，表示响应体中是否包含应答头数据。true，包含，false，不包含。如果包含的话，应答数据中包含Content-Type，Server等信息。
nConnectTimeout：连接超时时间，单位为秒；
nTimeout：读写数据超时时间，单位为秒
返回值：CURLcode
*/
CURLcode curl_get_req(const std::string &url, std::string &response, std::list<std::string> listRequestHeader, bool bResponseIsWithHeaderData = false, int nConnectTimeout = 10, int nTimeout = 10)
{
    // init curl  
    CURL *curl = curl_easy_init();
    // res code  
    CURLcode res;
    if (curl)
    {
        // set params  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
        //curl_easy_setopt(m_curl, CURLOPT_PORT, 8089);    //port
        curl_easy_setopt(curl, CURLOPT_POST, 0); // get reqest 
        //构建HTTP报文头
        struct curl_slist* headers = NULL;
        if (listRequestHeader.size() > 0)
        {
            std::list<std::string>::iterator iter, iterEnd;
            iter = listRequestHeader.begin();
            iterEnd = listRequestHeader.end();
            for (iter; iter != iterEnd; iter++)
            {
                headers = curl_slist_append(headers, iter->c_str());
            }
            //headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
            //headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
            if (headers != NULL)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//设置http请求头信息
            }
        }
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        if (bResponseIsWithHeaderData)
        {
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);//响应体中是否包含了头信息，比如Content-Type:application/json;charset=UTF-8
        }
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout); // set transport and time out time  
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
        // start request  
        res = curl_easy_perform(curl);
        if (headers != NULL)
        {
            curl_slist_free_all(headers); //free the list again
        }
    }
    // release curl  
    curl_easy_cleanup(curl);
    return res;
}

CURLcode curl_get_req_ex(CURL *curl, const std::string &url, std::string &response, std::list<std::string> listRequestHeader, bool bResponseIsWithHeaderData = false, int nConnectTimeout = 10, int nTimeout = 10)
{
    // res code  
    CURLcode res;
    if (curl)
    {
        // set params 
                curl_easy_reset(curl);
        /* enable TCP keep-alive for this transfer */
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        /* keep-alive idle time to 120 seconds */
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
        /* interval time between keep-alive probes: 30 seconds */
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 30L);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
        //curl_easy_setopt(m_curl, CURLOPT_PORT, 8089);    //port
        curl_easy_setopt(curl, CURLOPT_POST, 0); // get reqest 
        //构建HTTP报文头
        struct curl_slist* headers = NULL;
        if (listRequestHeader.size() > 0)
        {
            std::list<std::string>::iterator iter, iterEnd;
            iter = listRequestHeader.begin();
            iterEnd = listRequestHeader.end();
            for (iter; iter != iterEnd; iter++)
            {
                headers = curl_slist_append(headers, iter->c_str());
            }
            //headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
            //headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
            if (headers != NULL)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//设置http请求头信息
            }
        }
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        if (bResponseIsWithHeaderData)
        {
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);//响应体中是否包含了头信息，比如Content-Type:application/json;charset=UTF-8
        }
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout); // set transport and time out time  
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
        // start request  
        res = curl_easy_perform(curl);
        if (headers != NULL)
        {
            curl_slist_free_all(headers); //free the list again
        }
    }
    return res;
}

/*
功能：post http数据
参数：url：请求字符串，比如：http://127.0.0.1:8080/api/Accounts/Login
postParams：请求附带的参数，比如uername=admin&password=123
listRequestHeader：请求头数据列表。
bResponseIsWithHeaderData：bool类型，表示响应体中是否包含应答头数据。true，包含，false，不包含。如果包含的话，应答数据中包含Content-Type，Server等信息。
nConnectTimeout：连接超时时间，单位为秒；
nTimeout：读写数据超时时间，单位为秒
返回值：CURLcode
*/
CURLcode curl_post_req(const std::string &url, const std::string &postParams, std::string &response, std::list<std::string> listRequestHeader, bool bResponseIsWithHeaderData = false, int nConnectTimeout = 10, int nTimeout = 10)
{
    // init curl  
    CURL *curl = curl_easy_init();
    // res code  
    CURLcode res;
    if (curl)
    {
        // set params  
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
        //curl_easy_setopt(m_curl, CURLOPT_PORT, 8089);    //port
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post reqest 
        //构建HTTP报文头
        struct curl_slist* headers = NULL;
        if (listRequestHeader.size() > 0)
        {
            std::list<std::string>::iterator iter, iterEnd;
            iter = listRequestHeader.begin();
            iterEnd = listRequestHeader.end();
            for (iter; iter != iterEnd; iter++)
            {
                headers = curl_slist_append(headers, iter->c_str());
            }
            //headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
            //headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
            if (headers != NULL)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//设置http请求头信息
            }
        }
        else
        {
            headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
            if (headers != NULL)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//设置http请求头信息
            }
        }
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //返回的头部中有Location(一般直接请求的url没找到)，则继续请求Location对应的数据 
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        if (bResponseIsWithHeaderData)
        {
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);//响应体中是否包含了头信息，比如Content-Type:application/json;charset=UTF-8
        }
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
        // start request  
        res = curl_easy_perform(curl);
        if (headers != NULL)
        {
            curl_slist_free_all(headers); //free the list again
        }
    }
    // release curl  
    curl_easy_cleanup(curl);
    return res;
}

CURLcode curl_post_req_ex(CURL *curl, const std::string &url, const std::string &postParams, std::string &response, std::list<std::string> listRequestHeader, bool bResponseIsWithHeaderData = false, int nConnectTimeout = 10, int nTimeout = 10)
{
    // res code  
    CURLcode res;
    if (curl)
    {
        // set params
                curl_easy_reset(curl);
        /* enable TCP keep-alive for this transfer */
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        /* keep-alive idle time to 120 seconds */
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
        /* interval time between keep-alive probes: 30 seconds */
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 30L);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
        //curl_easy_setopt(m_curl, CURLOPT_PORT, 8089);    //port
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post reqest 
        //构建HTTP报文头
        struct curl_slist* headers = NULL;
        if (listRequestHeader.size() > 0)
        {
            std::list<std::string>::iterator iter, iterEnd;
            iter = listRequestHeader.begin();
            iterEnd = listRequestHeader.end();
            for (iter; iter != iterEnd; iter++)
            {
                headers = curl_slist_append(headers, iter->c_str());
            }
            //headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
            //headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
            if (headers != NULL)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//设置http请求头信息
            }
        }
        else
        {
            headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
            if (headers != NULL)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//设置http请求头信息
            }
        }
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //返回的头部中有Location(一般直接请求的url没找到)，则继续请求Location对应的数据 
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        if (bResponseIsWithHeaderData)
        {
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);//响应体中是否包含了头信息，比如Content-Type:application/json;charset=UTF-8
        }
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
        // start request  
        res = curl_easy_perform(curl);
        if (headers != NULL)
        {
            curl_slist_free_all(headers); //free the list again
        }
    }
    return res;
}

//实例1
void sample1()
{
    curl_global_init(CURL_GLOBAL_ALL);
    
    //post获取数据
    std::string strResponse = "",strResponseAnsi = "";
        strResponse.clear();
    CURLcode res = curl_post_req("http://127.0.0.1:8080/api/Accounts/Login", "username=admin&password=123", strResponse);
    if (res == CURLE_OK)
    {
        std::string strToken = "";
        strResponseAnsi = Utf8toAscii(strResponse);
    }

    //get获取数据
strResponse.clear();
    res = curl_get_req("http://127.0.0.1:8080/api/Accounts/Login?username=admin&password=123", strResponse);
    if (res == CURLE_OK)
    {
        int jj = 0;
    }

    curl_global_cleanup();
}

//实例2    
void sample2()
{
    //post json数据
    CURL * curl = curl_easy_init();
    std::string strResponse = "", strResponseAnsi = "";
    char szRequestUrl[256] = { 0 };
    CURLcode res = CURLE_OK;
    sprintf_s(szRequestUrl, "%s/api/GPS/AddOne", "http://127.0.0.1:8080");
    std::string strPostParams = "";

    try
    {
        boost::property_tree::ptree ptroot;
        ptroot.put("deviceid", "12345678");
        ptroot.put<unsigned int>("deviceStatus", 0);
        ptroot.put<unsigned int>("alarmFlag", 0);
        ptroot.put("lng", fLongitude);
        ptroot.put("lat", fLatitude);
        ptroot.put("speed", 0);
        ptroot.put("direction", 0);
        ptroot.put<int>("altitude", 10);
        ptroot.put("gpsTime", "2018-10-10 12:00:01");
        std::stringstream sstream;
        boost::property_tree::write_json(sstream, ptroot);
        strPostParams = sstream.str();
        bSuccess = true;
    }
    catch (boost::property_tree::ptree_error pt)
    {
        pt.what();
    }

    if (bSuccess)
    {
    std::string strAuthorization = "admin---";
        std::string strRequestHeaders = strAuthorization;
        std::list<std::string> listRequestHeader;
        listRequestHeader.push_back(strRequestHeaders);
        listRequestHeader.push_back("Content-Type:application/json;charset=UTF-8");
        res = curl_post_req_ex(curl, szRequestUrl, strPostParams, strResponse, listRequestHeader);
        if (res == CURLE_OK)
        {
            bSuccess = true;
        }
    }

    curl_easy_cleanup(curl);
}
```



