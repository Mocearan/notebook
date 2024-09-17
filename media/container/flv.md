# FLV

---

​		FLV（Flash Video）是 Adobe 公司设计开发的一种流行的流媒体格式，由于其视频文件体积轻巧、封装简单等特点，使其很适合在互联网上进行应用。

- FLV可以使用 Flash Player 进行播放
  - Flash Player 插件已经安装在全世界绝大部分浏览器上，这使得通过网页播放 FLV 视频十分容易
  - 目前主流的视频网站如优酷网，土豆网，乐视网等网站无一例外地使用了 FLV 格式。
  - FLV 封装格式的文件后缀通常为“.flv”。直播场景下拉流比较常见的是 http-flv 直播流，具有延时低，易传输等特点。



​		FLV 是一个二进制文件，简单来说，其是由一个文件头（FLV header）和很多 tag 组成（FLV body）。tag 又可以分成三类: audio, video, script，分别代表音频流，视频流，脚本流，而每个 tag 又由 tag header 和 tag data 组成。



## 参考

https://blog.csdn.net/ProgramNovice/article/details/137507298

https://blog.csdn.net/ProgramNovice/article/details/137468819

[FLV 文件格式分析_flv格式解析-CSDN博客](https://blog.csdn.net/xiaopangcame/article/details/134653356)

[媒体文件格式分析之FLV - 懒人李冰 (lazybing.github.io)](https://lazybing.github.io/blog/2016/07/24/flv-parse/)

[flv格式详解+实例剖析 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9018548.html)

[flv 解封装 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020407.html)

[h264 aac 封装 flv - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020423.html)

[将h264和aac码流合成flv文件 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020453.html)

[librtmp接收flv流中提取h264码流:根据多个资料汇总 - DoubleLi - 博客园 (cnblogs.com)](https://www.cnblogs.com/lidabo/p/9020492.html)