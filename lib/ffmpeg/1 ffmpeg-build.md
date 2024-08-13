

# ffmpeg-build

---



## 参考

[CentOS 7.5下FFmpeg安装、简单使用总结_Linux教程_Linux公社-Linux系统门户网站 (linuxidc.com)](https://www.linuxidc.com/Linux/2018-10/154934.htm)

[Linux下ffmpeg的完整安装 - wanghetao - 博客园 (cnblogs.com)](https://www.cnblogs.com/wanghetao/p/3386311.html)

[Linux下安装ffmpeg - 小得盈满 - 博客园 (cnblogs.com)](https://www.cnblogs.com/freeweb/p/6897907.html)

[Linux中安装FFmpeg详解_Linux教程_Linux公社-Linux系统门户网站 (linuxidc.com)](https://www.linuxidc.com/Linux/2019-03/157443.htm)

[FFmpeg在Linux下编译使用 - CrazyDiode - 博客园 (cnblogs.com)](https://www.cnblogs.com/CoderTian/p/6655568.html)



## 编译配置



### `--enable-libx264`

​			`Unknown encoder 'libx264'`

- 缺少libx264库，需要安装该库：

  ```shell
  git clone http://git.videolan.org/git/x264.git
  cd x264
  ./configure --enable-static --enable-shared
  make && make install
  
  sudo vi /etc/ld.so.conf
  # 添加libx264.so路径
  sudo ldconfig
  ```

- rebuild ffmpeg

  ```shell
  ./configure --enable-gpl --enable-libx264
  make clean
  make && make install
  
  # 验证 ffmpeg -i input.mp4 -c:v libx264 -c:a copy -f mpegts output.ts
  ```



### `--enable-sdl2`

​		默认的源码安装不会编译出`ffplay`，需要下载SDL2库依赖，然后重新编译添加`sdl2`支持

- 下载SDL的源码

  ```shell
  wget http://libsdl.org/release/
  tar zxvf SDL2-2.0.12.tar.gz
  cd SDL2-2.0.12.tar.gz
  ./configure
  make && make install
  ```

- 重新编译配置`ffmpeg`

  ```shell
   ./configure --enable-gpl --enable-sdl
  make clean
  make && sudo make install
  ```

- 通过远程终端不能直接使用`ffplay`

  - 远程终端没有用户界面

## linux

```shell
git clone https://github.com/FFmpeg/FFmpeg.git

./configure
make && make install
```



## windows 

### direct lib

- 下载编译好的Windows版本：http://ffmpeg.zeranoe.com/builds/
- 添加环境变量

### msys2 

```shell
 # 编译64位 exe+lib
 ./../../sources/ffmpeg/configure   --prefix=./../../install/x64 \ 
 	--enable-gpl \
    --disable-asm \
    --enable-nonfree \
    --enable-shared \
    --disable-static \
    --enable-sdl2 \
    --enable-libfdk-aac \
    --enable-libx264 \
    --enable-libx265 \
    --enable-libmp3lame \
    --enable-libopus
```



```shell
 # 编译32位 lib
 ./../../sources/ffmpeg/configure  \
 	--prefix=./../../install/x86/ \ 
 	--enable-gpl \
    --disable-asm \
    --enable-nonfree \
    --enable-shared \
    --disable-static \
    --disable-program \
    --enable-libfdk-aac \
    --enable-libx264 \
    --enable-libx265 \
    --enable-libmp3lame \
    --enable-libopus
```





## ffmpeg in golang



[2023-04-04：使用 Golang 和 ffmpeg-go 库实现 demuxing_decoding.c，轻松掌握音视频-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/2295107)

[go语言中ffmpeg使用的方法是什么 - 问答 - 亿速云 (yisu.com)](https://www.yisu.com/ask/3974196.html)

[windows系统 ---go环境搭建 ---详细教程_windows go-CSDN博客](https://blog.csdn.net/xuezhe5212/article/details/139327521)

[2023-03-24：音视频mp3和h264混合（muxer）编码为mp4，用go语言编写。-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/2295089)

[2023-03-23：音视频解混合（demuxer）为PCM和YUV420P，用go语言编写。-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/2295088)

[2023-02-18：ffmpeg是c编写的音视频编解码库，请问用go语言如何调用？例子是03输出版本号。-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/2295058)
