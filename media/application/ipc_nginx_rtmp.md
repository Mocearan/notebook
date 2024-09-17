# ipc_nginx_rtmp

---

​		通过nginx以rtmp的方式直播rtsp拉流的网络摄像头。

- 搭建nginx服务器

  ```shell
  wget https://nginx.org/download/nginx-1.26.2.tar.gz
  git clone https://github.com/arut/nginx-rtmp-module.git
  
  ./configure --add-module=./module/nginx-rtmp-module --without-http_rewrite_module --with-http_ssl_module
  make
  make install
  ```

- 海康IPC需要禁用Ehome（ISUP）功能，因为码流不兼容

  - rtsp拉流不成功

- 在IPC同一局域网下使用ffmpeg以rtsp方式拉流，以rtmp方式推流

  ```shell
  ffmpeg -thread_queue_size 128 -rtsp_trasport tcp -i rtsp://admin:w12345@192.168.7.118:554/Streaming/Channels/101 -f flv -an -b 20000000 rtmp://124.222.73.231:1935/live/room
  ```

- 通过vlc用上述rtmp地址拉流播放

[「自己开发直播」rtmp-nginx-module实现直播状态、观看人数控制 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9004087.html)

[让你的 Nginx 的 RTMP 直播具有统计某频道在线观看用户数量的功能 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9004106.html)

[Nginx 的 RTMP 模块的在线统计功能 stat 在 multi-worker 模式下存在 Bug - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9004105.html)

[流媒体技术学习笔记之（三）Nginx-Rtmp-Module统计某频道在线观看流的客户数 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9004154.html)

[【推荐】Nginx基础知识之————多模块(非覆盖安装、RTMP在线人数实例安装测试) - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9004175.html)

[nginx statistics in multi-workers - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9015724.html)

[     windows下编译nginx+nginx_rtmp_modue(vs2013)       ](https://www.cnblogs.com/lidabo/p/9037968.html)

[     Windows下编译nginx-rtmp-module       ](https://www.cnblogs.com/lidabo/p/9037976.html)

[     windows10 vs2015编译 带nginx-rtmp-module 模块的32位nginx       ](https://www.cnblogs.com/lidabo/p/9037981.html)

[     windows下搭建nginx-rtmp服务器       ](https://www.cnblogs.com/lidabo/p/9077938.html)

[     Nginx+jwPlay搭建流媒体服务器，记忆播放       ](https://www.cnblogs.com/lidabo/p/9101407.html)

[     在Windows下搭建基于nginx的视频直播和点播系统       ](https://www.cnblogs.com/lidabo/p/9101459.html)

