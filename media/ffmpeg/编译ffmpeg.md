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



