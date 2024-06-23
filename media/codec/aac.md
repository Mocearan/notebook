# AAC

---

​		AAC 有两种格式：ADIF，ADTS

- `ADIF（Audio Data Interchange Format）`，音频数据交换格式
  - 只在文件头部存储用于音频解码播放的信息（例如采样率，通道数等）
  - 解码播放必须从文件头部开始，一般用于存储在本地磁盘中y音频文件的播放
- `ADTS（Audio Data Transport Stream）`，音频数据传输流
  - 可以将数据看做一个个的音频帧，而每帧都存储了用于音频解码播放的头信息（例如采样率，通道数等）
  - 可以从任何帧位置解码播放，更适用于流媒体传输



## ADTS

​		ADTS 格式的 AAC 码流是由一个个的 ADTS Frame 组成.

```c
 ...| ADTS Frame | ADTS Frame | ADTS Frame | ADTS Frame | ADTS Frame |...
```

​		每个 ADTS Frame 是由头部（固定头部+可变头部）和数据组成

```c
// ADTS Frame
```

| 序号 | 字段名称                        | 长度 (bits) | 说明                                                         |
| ---- | ------------------------------- | ----------- | ------------------------------------------------------------ |
| 1    | Syncword                        | 12          | ADTS Frame 头部的第一个字段，12bit 都为1（`FF F`）           |
| 2    | MPEG version                    | 1           | 0 表示 MPEG-41 表示 MPEG-2                                   |
| 3    | Layer                           | 2           | always 0                                                     |
| 4    | Protection Absent               | 1           | 是否存在 CRC 校验，0 表示存在 CRC 校验字段，1 表示不存在 CRC 校验字段 |
| 5    | Profile                         | 2           | 0 表示 AAC Main1 表示 AAC LC2 表示 AAC SSR                   |
| 6    | MPEG-4 Sampling Frequence Index | 4           | 采样率，0 表示 96000Hz，4 表示 44100Hz，11 表示 8000Hz[详见此处](https://wiki.multimedia.cx/index.php/MPEG-4_Audio#Sampling_Frequencies) |
| 7    | Private Stream                  | 1           | 编码时将该值设为 0，解码时忽略                               |
| 8    | MPEG-4 Channel Configuration    | 3           | 通道数                                                       |
| 9    | Originality                     | 1           | 编码时将该值设置为 0，解码时忽略                             |
| 10   | Home                            | 1           | 编码时将该值设为 0，解码时忽略                               |
| 11   | Copyrighted Stream              | 1           | 编码时将该值设为 0，解码时忽略                               |
| 12   | Copyrighted Start               | 1           | 编码时将该值设为 0，解码时忽略                               |
| 13   | Frame Length                    | 13          | ADTS 帧长度，包括头部所占的长度                              |
| 14   | Buffer Fullness                 | 11          | 值为 0x7FF 时表示动态码率                                    |
| 15   | Number of AAC Frames            | 2           | 值为 ADTS 帧里的 AAC 帧数量减一，为了兼容性一般一个 ADTS 帧包含一个 AAC 帧 |
| 16   | CRC                             | 16          | CRC 校验码                                                   |

> [该网站](https://www.p23.nl/projects/aac-header/)提供了一个解析 AAC ADTS Frame Header 的工具，你可以输入头部 7 或 9 个字节的数据，点击 Submit 就能看到头部各字段对应的含义。