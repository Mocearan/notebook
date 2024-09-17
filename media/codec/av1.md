# AV1

---

​		AV1（AOMedia Video 1）是一种开放、免版税的视频编码格式，旨在用于视频传输和存储。它由开放媒体联盟（Alliance for Open Media, AOM）开发。

> AOM是一个由Google、Mozilla、Cisco、Amazon、Intel、Microsoft、Netflix等多家公司组成的联盟。



## 参考

[H265专利授权费、H266、AV1与十年原地踏步的SVAC (sh-anfang.org)](http://www.sh-anfang.org/page/2/8/9060.html)

[你知道h.265吧？但关于AV1编码格式你也来了解一下吗？-CSDN博客](https://blog.csdn.net/cybozu/article/details/136641316)

[分类:av1 - 懒人李冰 (lazybing.github.io)](https://lazybing.github.io/blog/categories/av1/)

[媒体文件格式分析之AVI - 懒人李冰 (lazybing.github.io)](https://lazybing.github.io/blog/2016/07/24/avi-parse-format/)

## 特点

- 高效压缩
  - 目标是提出一种比现行编码标准（H264/AVC，HECV/H265）压缩比更高的编码标准
  - 但在提出的早期，计算耗时严重，对实施编码和低延迟有挑战
- 免费开源
  - 完全开放且免版税
  - 与某些其他视频编码标准（如HEVC）不同
- 兼容性
  - AV1旨在支持从低端移动设备到高端生产和播放设备的广泛兼容性。
  - 越来越多的浏览器、操作系统和硬件开始原生支持AV1
  - 虽然在增长，但支持仍然相对有限
- 应用适用性
  - 在线视频流媒体（如YouTube和Netflix）到视频会议和实时广播
  - 主流现代浏览器已经支持了AV1
    - Google Chrome
    - Mozilla Firefox
    - Microsoft Edge
    - Opera
    - Safari

​		AV1编解码相对于较旧的标准（如H.264）在计算上更为密集，这意味着在没有专门硬件加速的情况下，高分辨率AV1视频的播放可能会对CPU造成较大压力，影响播放性能。