# 自适应码率流式传输

---

​		自适应码率流式传输（`ABS`）是在线视频传输领域的一项关键技术，通过动态调整视频质量以适应观众的网络条件，确保流畅的播放体验。

​		与传统的单一质量视频流传输方法不同，`ABS `提供多个不同码率和分辨率的视频流。

- **多个视频流**：视频内容以多种码率和分辨率进行编码。
- **动态调整**：流式传输客户端根据实时网络条件和设备能力在这些视频流之间切换。
- **无缝播放**：结果是无缝的观看体验，缓冲最少，视频质量最优。

​		优势

- **提升观众体验**：通过适应观众的网络速度，`ABS `最小化缓冲并确保连续播放。
- **优化带宽使用**：ABS 提供观众互联网连接所能支持的最高质量流，必要时节省带宽。
- **设备兼容性**：它适应不同设备的能力，确保在高端和低端设备上都能获得最佳观看效果。

​		挑战

- **复杂性**：实现 `ABS `需要在多个码率下对视频进行编码，创建清单文件，并确保流式传输服务器和客户端支持动态切换。
- **增加存储需求**：存储每个视频的多个版本会增加存储需求。
- **延迟**：如果管理不当，分段和在流之间切换可能会引入延迟。

## 参考



## 原理

### 播放列表清单

​		播放列表清单是 ABS 的关键组成部分。它是一个列出所有可用视频流及其码率和段 URL 的文件。

- **HLS（HTTP Live Streaming）**：使用 `M3U8 `清单文件。
- **DASH（Dynamic Adaptive Streaming over HTTP）**：使用 `MPD`（Media Presentation Description）文件。



## 分段

​		视频被分割成小段（通常为 2-10 秒长）。每一段都在多个码率和分辨率下进行编码。

- **一致性**：每一段都是一个可以独立播放的独立文件。
- **同步性**：所有码率和分辨率必须同步，以确保顺畅切换。

​		不同码率的示例段文件：

- `low/segment1.ts`
- `mid/segment1.ts`
- `high/segment1.ts`



## 动态切换

​		流式传输客户端持续监控观众的网络条件，并切换到合适的流，以提供尽可能好的质量而不中断。

- **缓冲**：客户端保持少量视频数据缓冲区，以便于顺畅切换。
- **监控**：它测量下载速度、缓冲状态和设备性能等参数，以决定何时切换流。



## 实现ABS

- 首先，在服务端对上传的素材视频进行转码，生成不同分辨率的分段集合

  ```shell
  ffmpeg -i input.mp4 \
    -vf "scale=w=640:h=360" -c:v h264 -b:v 800k -g 48 -keyint_min 48 -profile:v baseline -preset veryfast -c:a aac -b:a 128k -hls_time 4 -hls_playlist_type vod -hls_segment_filename 'low/segment%d.ts' low.m3u8 \
    -vf "scale=w=1280:h=720" -c:v h264 -b:v 2500k -g 48 -keyint_min 48 -profile:v main -preset veryfast -c:a aac -b:a 128k -hls_time 4 -hls_playlist_type vod -hls_segment_filename 'mid/segment%d.ts' mid.m3u8 \
    -vf "scale=w=1920:h=1080" -c:v h264 -b:v 5000k -g 48 -keyint_min 48 -profile:v high -preset veryfast -c:a aac -b:a 128k -hls_time 4 -hls_playlist_type vod -hls_segment_filename 'high/segment%d.ts' high.m3u8
  ```

- 然后，创建清单文件，生成列出可用流的清单文件（HLS 的 M3U8 或 DASH 的 MPD）。

- 其次，配置流媒体分发服务器对分段集合和清单文件的索引路径

  - **带有 RTMP 模块的 NGINX**：适用于 `HLS`。
  - **Apache HTTP Server**：可以配置为提供 `HLS/DASH `内容。
  - **AWS Elemental Media Services**：提供可扩展的 `ABS `解决方案。

- 最后，在流媒体客户端上支持`ABS`功能

  - **Video.js**：一个流行的 `HTML5 `视频播放器，支持 `HLS `和 `DASH`。
  - **Shaka Player**：一个开源的 `JavaScript `库，用于 `DASH `和 `HLS`。
  - **JW Player**：一个具有丰富 `ABS `功能的商业播放器。

- 充分测试

  - **模拟网络条件**：在各种网络条件下测试播放器，以确保顺畅切换。
  - **监控性能**：使用分析工具监控观众体验，并根据需要优化设置。
  - **微调编码设置**：调整码率阶梯、段持续时间和缓冲区大小，以实现最佳性能。

















​	