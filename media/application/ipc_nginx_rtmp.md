# ipc_nginx_live

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