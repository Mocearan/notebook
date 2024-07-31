

# ffmpeg-build

---







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



