

# Image



## 参考

[图像格式：常见图像格式RAW, RGB, YUV&&图像格式的解析、格式转换和看图软件_raw图片编码-CSDN博客](https://blog.csdn.net/weixin_45264425/article/details/132626509)

[FFMPEG 内部YUV转RGB过程 - 洛笔达 - 博客园 (cnblogs.com)](https://www.cnblogs.com/luoyinjie/p/13669368.html)

## 图像的物理性质

​		白光能被分解为三原色，红光（R）、绿光（G）、蓝光（B），即RGB。等量的三原色光叠加会变为白光。

​		电子设备显像是自发光的，不同于其他自然物体是由于反射日光。

## 基本概念

- 像素（pixel）：像素是构成图片的基本单位，每个像素就是一个视觉采样的单元

- 分辨率：是指图像的大小或尺寸，同时表达了图片的采样量
  - 比如`1920x1080`表示横`1920`个像素，纵`1080`个像素的排列，两百万像素
  
- 位深：是指在采样视觉数据时，用来描述数字图像颜色数据的数据量
  - 比如RGB中红色分量用`8bit`，YUV中Y分量用`8bit`
  - RGB中用红绿蓝三原色加上透明度alpha分量来描述一个像素，位深越大描述的像素颜色越多
  - YUV中用亮度（Y）、色度描述（U和V）一个像素，位深越大画面越细腻
  
- 跨距（Stride）：内存中每行像素所占的空间
  - 编码中存在内存对齐导致每行像素在内存中的数据表示和理论上的图像宽度并不一定相同
  
  - 也称为Padding，如果图像的每一行像素末尾拥有对其内容，Stride 的值一定大于图像的宽度值
  
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/8705ced6a6764833ab4ba32cbd14ef56.png)
  
    > 分辨率638x480的RGB24图像，内存处理的时候如果要以16字节对齐
    >
    > ​	638/16=119.625不能整除，需要在每行尾部填充6个字节：`640 / 16 = 120 /638->640 / 2*24bit=48bit=6byte `。
    >
    > ​	此时该图片的stride为``480*6=1920``字节。



## RGB:  电子设备图像表示

​		像素点由RGB三个通道描述，每个通道按照位深，显示不等量的对应三原色，从而构成该像素点的颜色。

​		RGB表示上，除了红绿蓝三个色彩通道，还有一个透明度通道（alpha分量， ARGB）。

​		每个通道上，按照一定的规则来定义对应颜色的分级。RGB通道的分级数值表示组成一个像素点的数据格式：

- 浮点表示：`0/0~10`，OpenGL ES中每个通道点的表示使用这种方式。

- 整数表示：`RGB_8888`,``0~255``或``00~FF``,8位表示一个分量通道上的值。即8位位深度。

  - 一些平台上采用其他的整数表示方式

    - 如Android采用`RGB_565`共`16bit`来表示一个像素

    - `RGB_8888` 表示的`1280X720`的图像大小为：`1280 x 720 x 4byte=3.516MB`

      > 这也是位图（bitmap）在内存中所占用的大小，所以每一张图像的裸数据都是很大的。

​		RGB像素点中各个分量通常是按照顺序排列的，但有些图像处理要转换成其他的顺序，`opencv`经常转换成BGR的排列方式。

![image-20220907235455776](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220907235455776.png)



## YUV:  电视图像数字表示

​		视频中的图片数据更多的是YUV数据格式，主要为了优化彩色视频信号的传输，使其向后兼容老式黑白电视。因为亮度信号Y和色度信号U、V是分离的。

​		同时降低色度的采样率而不会对图像质量影响太大，降低了视频信号传输时对频宽（带宽）的要求。

- 如果只有Y信号分量而没有U、V分量，那么这样表示的图像就是黑白灰度图像。

- Y，明亮度（Luminance或Luma），也称灰阶值

  - 亮度是通过RGB输入信号来建立的概念

  - 方法是将RGB信号的特定部分叠加到一起。

- U / V，色度（Chrominance或Chroma）

  - 它们的作用是描述影像的色彩（Cr）及饱和度（Cb），用于指定像素的颜色

    - Cr反映了RGB输入信号红色部分与RGB信号亮度值之间的差异
    - Cb反映的则是RGB输入信号蓝色部分与RGB信号亮度值之间的差异

  - U表示蓝色色度的信息，V表示红色色度的信息

    - 分离亮度和色度的方式有助于视频编码和压缩。

      > 因为人眼对亮度变化更敏感，而对颜色变化的敏感度较低。
      >
      > 因此，通过将图像的色度信息分开编码，减少色度采样，可以降低数据量同时保持图像质量。

    - 绿色的色度信息通常是通过蓝色（U）和红色（V）的差值来表示的。

      > 因为人眼对绿色的感知更为敏感，所以通过差值编码可以更有效地表示绿色色度信息，同时减少数据量和保持图像质量。

  - 最常用的表示形式是Y、U、V都使用8个bit来表示

    - 取值范围就是``0～255``

      > 为了防止信号变动造成过载，在广播电视系统中不传输很低和很高的数值。把边界的数值作为“保护带”。
      >
      > 不论是Rec.601还是BT.709的广播电视标准中，Y的取值范围都是16～235, UV的取值范围都是16～240。




### YUV的排列格式

​		YUV是一系列相似格式的统称，针对具体的排列方式，可以分为多种格式。

#### 打包（packed）格式

​	每个像素点的Y、U、V分量视作一个`packed`，以像素点为单元连续存储数据。

​	通常几个相邻的像素组成一个宏像素（macro-pixel)

![image-20220908211732290](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908211732290.png)

#### 平面（planar）格式

​		将Y、U、V分量数据分别存储。																

![image-20220908211748811](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908211748811.png)

![image-20220908221051838](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221051838.png)

### YUV的采样格式

​		YUV采用`A:B:C`表示法来描述Y、U、V采样频率的关系。

​		主要分为YUV 4:4:4 、YUV 4:2:2、YUV 4:2:0三种常用类型。

![image-20220908213114325](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908213114325.png)

- 黑点表示一个Y分量，空心圆表示像素点的一个UV分量组合。

- YUV最常用的采样格式是4:2:0,它指的是对每行扫描线来说，只有一种色度分量是以2:1的抽样率来存储的。

  - 相邻的扫描行存储着不同的色度分量

    - 如果某一行是4:2:0，那么其下一行就是4:0:2，再下一行是4:2:0，以此类推
    - 所以一组YUV实际上需要综合两行的采集组成

  - 对于每个色度分量来说，水平方向和竖直方向的抽样率都是2:1，所以可以说色度的抽样率是4:1。

    - 对于一组四个Y，在U/V分量中采一个U和一个V

    > ​		对非压缩的8比特量化的视频来说，8×4的一张图片需要占用48字节的内存（按planar存储）：
    >
    > ![image-20220821230236373](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821230236373.png)
    >
    > ![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/3cdccd104332441bbae4c9c04637342e.png)



### YUV的数据存储

​		对非压缩的8比特量化的采样数据来说，每个分量占一个字节（char或byte）

#### YUV 4:4:4

​	I444格式（YUV444P, P-planar），对于`ffmpeg`像素表示`AV_PIX_FMT_YUV444P`

> planar YUV4:4:4， 24bpp(24bit per pixel)，（1Cr & Cb sample per 1X1 Y samples）
>
> > Y 4byte + U 4byte + V 4byte = 12byte
> >
> > 即4个像素占用12byte，每个像素占用3byte

![image-20220908214343304](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908214343304.png)

#### YUV 4:2:2

​	I422 格式（YUV422P），对于ffmpeg像素表示`AV_PIX_FMT_YUV422P`

> planar YUV4:2:2， 16bpp(16bit per pixel)，（1Cr & Cb sample per 2X1 Y samples）
>
> > Y 4byte + U 2byte + V 2byte = 8byte
> >
> > 即4个像素占用8byte，每个像素占用2byte

![image-20220908214659406](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908214659406.png)

#### YUV 4:2:0

​	I420格式（YUV420P），对于ffmpeg像素表示`AV_PIX_FMT_YUV420P`

> planar YUV4:2:0， 12bpp(12bit per pixel)，（1Cr & Cb sample per 2X2 Y samples）
>
> > Y 4byte + U 1byte + V 1byte = 6byte
> >
> > 即4个像素占用6byte，每个像素占用1.5byte

![image-20220908215935314](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908215935314.png)

### YUV和RGB的转化

​		凡是渲染到屏幕上的东西（文字、图片或者其他），都要转换为RGB的表示形式。

​		通常情况下RGB和YUV直接的相互转换都是调用接口实现，比如`Ffmpeg`的`swscale`或者`libyuv`等库。主要转换标准是 BT601 和 BT709。

![image-20220908221221048](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220908221221048.png)

​		YUV(256 级别) 可以从8位 RGB 直接计算：

```
Y = 0.299R + 0.587G + 0.114B
U = -0.169R - 0.331*G + 0.5B
V = 0.5 R - 0.419G - 0.081B
```

8bit位深的情况下，TV range是16-235(Y)、16-240(UV) , 也叫Limited Range；PC range是0-255，也叫Full Range。而RGB没有range之分，全是0-255。

​		反过来，RGB 也可以直接从YUV (256级别) 计算：

```
R = Y + 1.402 (Y-128)
G = Y - 0.34414 (U-128) - 0.71414 (U-128)
B = Y + 1.772 (V-128)
```

- 从YUV转到RGB，如果值小于0要取0，大于255要取255.



​		比较典型的场景：

> ​		iOS平台中使用摄像头采集出YUV数据之后，上传显卡成为一个纹理ID，这个时候就需要做YUV到RGB的转换（具体的细节会在后面的章节中详细讲解）。在iOS的摄像头采集出一帧数据之后（CMSampleBufferRef），我们可以在其中调用CVBufferGetAttachment来获取YCbCrMatrix，用于决定使用哪一个矩阵进行转换。
>
> ​		对于Android的摄像头，由于其是直接纹理ID的回调，所以不涉及这个问题。
>
> ​		其他场景下需要自行寻找对应的文档，以找出适合的转换矩阵进行转换。

为什么解码出错显示绿屏？

> ​		因为解码失败时YUV分量都填为0值，然后根据公式：R = 1.402 * (-128) = -126.598，G = -0.34414 * (-128) - 0.71414 * (-128) = 44.04992 + 91.40992 = 135.45984，B = 1.772 * (-128) = -126.228。
> ​		RGB 值范围为[0，255]， 所以最终的值为：R = 0，G = 135.45984，B = 0。
> ​		此时只有G分量有值所以为绿色。

## 图像的编码：压缩

​		由于图像裸数据很大，一般对图像进行压缩。

### JPEG

​		静态图像压缩标准，ISO指定。

- 良好的压缩性能

- 较好的重建质量

- 图像处理领域

- 有损压缩

- 不能直接用于视频压缩

  > 视频需要考虑时域因素，不仅要考虑帧内编码，还要考虑帧间编码。（帧，视频中的一幅图像）

