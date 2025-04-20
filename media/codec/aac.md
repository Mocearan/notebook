# AAC

---

​		AAC（Advanced Audio Coding）是一种现代的音频编码技术，用于数字音频的传输和存储领域。

​		AAC是音频最常用的编码格式之一，几乎所有的播放器都支持这个编码格式，比MP3的压缩率更高，保真性比MP3强

- 由 Fraunhofer IIS、杜比实验室、AT&T、Sony 等公司共同开发，在 1997 年推出的基于 MPEG-2 的有损数字音频压缩的专利音频编码标准。

- 2000 年，MPEG-4 标准在原 AAC 的基础上加上了 LTP（Long Term Prediction）、PNS（Perceptual Noise Substitution）、SBR（Spectral Band Replication）、PS（Parametric Stereo）等技术，并提供了多种扩展工具。

  - SBR（Spectral Band Replication， 频段重现）

  - PS（Parametric Stereo，参数化）技术

  - 为了区别于传统的 MPEG-2 AAC 又称为 MPEG-4 AAC



## 参考

[最简单的 AAC 音频码流解析程序_aac裸流结果-CSDN博客](https://blog.csdn.net/ProgramNovice/article/details/137225523)

[音频格式之AAC：(3)AAC编解码原理详解_aac音频采样率-CSDN博客](https://blog.csdn.net/littlezls/article/details/135777833)



## 特点

1. 更好的声音质量和更低的压缩比，减少了由于压缩而引入的失真和噪音。
2. 支持多种采样率、声道数和比特率。
3. 具有很好的灵活性和可扩展性。z

- 音频采样率限制在8KHz-96KHz之间

- AAC编码一般以1024个采样为一个音频帧

  - > 双通道、采样率为48KHz的话，则一个音频帧的长度大约为0.01秒

  - 如果每个音频帧的采样数不对，则可能会造成播放过快或过慢的问题



## AAC编码组件

​		AAC 属于感知音频编码。与所有感知音频编码类似，其原理是利用人耳听觉的掩蔽效应，对变换域中的谱线进行编码，去除将被掩蔽的信息，并控制编码时的量化噪声不被分辨。

​		在编码过程中，

- 时域信号先通过滤波器组（进行加窗 MDCT 变换）分解成频域谱线
- 同时时域信号经过心理声学模型获得信掩比，掩蔽阈值，M/S 立体声编码以及强度立体声编码需要的控制信息，还有滤波器组中应使用长短窗选择信息
- 瞬时噪声整形（TNS）模块将噪声整形为与能量谱包络形状类似，控制噪声的分布。
- 强度立体声编码和预测以及 M/S 立体声编码都能有效降低编码所需比特数，随后的量化模块用两个嵌套循环进行了比特分配并控制量化噪声小于掩蔽阈值，之后就是改进了码本的哈夫曼编码
- 这样，与前面各模块得到的边带信息一起，就能构成 AAC 码流了

​		AAC 系统包含了增益控制、滤波器组、心理声学模型、量化与编码、预测、TNS、立体声处理等多种高效的编码工具。这些模块或过程的有机组合形成了 AAC 系统的基本编解码流程。

​		在实际应用中，并不是所有的功能模块都是必需的，下表列出了 MPEG-2 AAC 各模块的可选性：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdM2cEaXcJMhIooiaHTSUyOcuwsR2iaTloCS8lIvPURyAfcIS3wMj2KvvjA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

​		

​		以上是 MPEG-2 AAC 各模块的介绍，在 MPEG－4 AAC 还新增了其他功能模块，比如：

1）**LTP（Long Term Prediction），长时预测模块。**它用来减少连续两个编码音框之间的信号冗余，对于处理低码率的语音非常有效。

2）**PNS（Perceptual Noise Substitution），知觉噪声替换模块。**当编码器发现类似噪音的信号时，并不对其进行量化，而是作个标记就忽略过去，当解码时再还原出来，这样就提高了效率。在具体操作上，PNS 模块对每个尺度因子带侦测频率 4k Hz 以下的信号成分。如果这个信号既不是音调，在时间上也无强烈的能量变动，就被认为是噪声信号。其信号的音调及能量变化都在心理声学模型中算出。

3）**SBR（Spectral Band Replication），频段复制技术。**音乐的主要频谱集中在低频段，高频段幅度很小，但很重要，决定了音质。对整个频段编码时，若为保护高频就会造成低频段编码过细导致编码效率较低；若只保存低频的主要成分而丢掉高频成分又会损失音质。SBR 把频谱切开，低频单独编码只保存主要成分，提高编码效率；高频单独放大编码，兼顾音质。

4）**PS（Parametric Stereo），参数立体声技术。**原本立体声双声道的编码输出是一个声道的两倍，但是两个声道的声音存在某种相似性。PS 存储一个声道的全部信息，然后花较少的字节用参数描述另一个声道的差异部分来提升编码效率。





### **Bitstream Formatter，码流解析**

​		在解码时，该模块将 AAC 数据流分解为各个工具模块对应的数据模块，并为每个工具模块提供与该工具相关的比特流数据信息。这个模块的输出包括：

- 无噪声编码频谱的分段信息
- 无噪声编码频谱
- Mid/Side 决策信息
- 预测器状态信息
- 强度立体声控制信息和耦合通道控制信息
- 时域噪声修整（TNS）信息
- 滤波器组控制信息
- 增益控制信息



### **Noiseless Decoding，无噪编解码**

- 无噪编码就是哈夫曼编码，它的作用在于进一步减少尺度因子和量化后频谱的冗余，即将尺度因子和量化后的频谱信息进行哈夫曼编码。
- 在解码时，该模块从码流解析模块获得输入的数据流，从中解码霍夫曼编码数据，并重建量化频谱、霍夫曼编码和 DPCM 编码的比例因子。

​		这个模块的输入包括：

- 无噪声编码频谱的分段信息
- 无噪声编码频谱

​		输出包括：

- 比例因子的解码整数表示
- 频谱的量化值



### **Inverse Quantization，量化和反量化**

​		在 AAC 编码中，逆量化频谱系数是由一个非均匀量化器来实现的，在解码中需进行其逆运算。

​		在解码时，该模块将频谱的量化值转换为整数值来表示未缩放的重建频谱。通过对量化分析的良好控制，比特率能够被更高效地利用。在频域调整量化噪声的基本方法就是用尺度因子来进行噪声整形，使用尺度因子这种机制是为了使用非均匀量化器在频域中改变量化噪声的比特分配。

> 此量化器是非均匀的量化器。
>
> 尺度因子就是一个用来改变在一个尺度因子带的所有的频谱系数的振幅增益值。

​		这个模块的输入包括：

- 频谱的量化值

​		输出包括：

- 未缩放的，逆量化的频谱



### **Rescaling，缩放因子处理**

​		解码时，该模块将比例因子的整数表示转换为实际值，然后将未缩放的逆量化频谱乘以相关比例因子。

​		这个模块的输入包括：

- 比例因子的解码整数表示
- 未缩放的，逆量化的频谱

​		输出包括：

- 缩放后的逆量化的频谱



### **M/S，Mid/Side 立体声编解码**

​		是联合立体声编码（Joint Stereo）的一种方案，编码时兼顾了这两个声道的共同信息量。

​		该模块基于 `Mid/Side` 决策信息将频谱对从 `Mid/Side` 模式转换为 `Left/Right `模式，以提高编码效率。

- 一般在左右声道信息相似度较高时使用
- 处理方式是将左右声道信息合并（L+R）得到新的一轨，再将左右声道信息相减（L-R）得到另外一轨
- 然后再将这两轨信息用心理声学模型和滤波器处理

​		这个模块的输入包括：

- Mid/Side 决策信息
- 和声道关联的缩放后的逆量化的频谱

​		输出包括：

- 在 M/S 解码之后，与声道对相关的缩放后的逆量化频谱



### **Prediction，预测**

​		解码时，该模块会在预测状态信息的控制下重新插入在编码时提取出的冗余信息。

​		该模块实现为二阶后向自适应预测器。对音频信号进行预测可以减少重复冗余信号的处理，提高效率。

​		这个模块的输入包括：

- 预测器状态信息
- 缩放后的逆量化的频谱

​		输出包括：

- 应用了预测的缩放后的逆量化的频谱



### **Intensity，强度立体声编解码**

​		是联合立体声编码（Joint Stereo）的一种方案，编码时兼顾了这两个声道的共同信息量。

​		一般在低流量时使用，利用了人耳对于低频信号指向性分辨能力的不足，将音频信息中的低频分解出来合成单声道数据，剩余的高频信息则合成另一个单声道数据，并记录高频信息的位置数据来重建立体声效果。

​		解码时，该模块对频谱对执行强度立体声解码。`Mid/Side Stereo `和 `Intensity Stereo` 都有利用部分相位信息的损失来换得较高的音色数据信息。

​		这个模块的输入包括：

- 逆量化的频谱
- 强度立体声控制信息

​		输出包括：

- 强度立体声道解码后的逆量化频谱



### **Dependently Switched Coupling，非独立交换耦合**

​		解码时，该模块基于耦合控制信息的指导，将非独立交换耦合声道中的相关数据添加到频谱中。

​		这个模块的输入包括：

- 逆量化的频谱
- 耦合控制信息

​		输出包括：

- 和非独立交换耦合声道耦合的逆量化频谱



### **TNS，瞬时噪音整形**

​		该模块实现了对编码噪声的精细时间结构的控制。

- 在编码时，TNS 处理过程会修整声音信号的时域包络。
- 在解码时，该模块会基于 TNS 信息的控制，在对应的逆处理过程中会还原实际的时域包络。
  - 这是通过对部分频谱数据进行滤波处理来实现的。
  - 这项神奇的技术可以通过在频率域上的预测，来修整时域上的量化噪音的分布。
  - 在一些特殊的语音和剧烈变化信号的量化上，TNS 技术对音质的提高贡献巨大。

​		这个模块的输入包括：

- 逆量化的频谱
- TNS 信息

​		输出包括：

- 逆量化的频谱



### **Filterbank/Block Switching，滤波器组/块切换**

​		解码时，该模块应用了在编码器中执行的频率映射的逆函数。

​		滤波器组工具使用了一个逆修正离散余弦变换（`IMDCT`），这个 `IMDCT `可以配置为支持一组 128 或 1024，或四组 32 或 256 频谱系数。

​		这个模块的输入包括：

- 逆量化的频谱
- 滤波器组控制信息

​		输出包括：

- 时域重建的音频信号



### **Gain Control，增益控制**

​		当输出时，该模块将单独的时域增益控制应用于已由编码器中的增益控制 `PQF `滤波器组创建的 4 个频带中的每个频带。然后，它会组合 4 个频带，并通过增益控制工具的滤波器组来重建时间波形。该模块仅可用于 `SSR(Scalable SampleRate) Profile`。

​		这个模块的输入包括：

- 时域重建的音频信号
- 增益控制信息

​		输出包括：

- 时域重建的音频信号



### **Independently Switched Coupling，独立交换耦合**

​		解码时，该模块基于耦合控制信息的指导，将独立交换耦合声道中的相关数据添加到时间信号中。

​		这个模块的输入包括：

- 滤波器组输出的时间信号
- 耦合控制信息

​		输出包括：

- 和独立交换耦合声道耦合的时间信号



### SBR 频带重现

​		`Spectral Band Replication`，能让低码率下音质更优秀。

​		SBR是一种全新的音频编码增强工具，在ISO/IEC 14496-3:2001/Amd.1:2003中被标准化。通过增加在特定比特率的音频带宽或改善在特定质量水平的编码效率，提供了改善低比特率音频和音频编码音质的可能性。

​		SBR可以扩大传统感知编码在低比特率下所能提供的有限的音频带宽，因此它的性能表现相当于或超过模拟FM的音频带宽（15kHz)。SBR也可以改善窄带音频编码的质量，可为广播电台提供12kHz音频带宽的纯音频频道，可用如诸如多语言广播等场合。

​		由于大多数音频编码都是被严格地限制带宽的，因此SBR的重要性不仅体现在提高音频质量上，而且也体现在提高音频的可读性和理解性上。SBR以**后处理**为主的，不过为能指导解码过程，在编码时编码器要先做一些预处理工作。

​		当联合SBR一起使用，其主体编码器（比如MP3）只负责处理音频频谱的低频部分。高频部分则由SBR解码器产生，这是紧跟在传统波形解码器之后的一个后处理过程。SBR基于对主体编码器处理得到的低频信号进行的分析，在解码器上重建了高频部分。为了确保精确的重建，一些指导信息以非常低的数据率夹杂在主体编码器产生的码流中一同传输。

​		重建对于和声和噪声成分同样有效，同时允许在频率范围和时间范围进行适当的修整。结果是，SBR实现了在非常低数据率下的全带宽音频编码，与主体编码器相比，明显地增加了压缩效率。

​		SBR的性能分析：

​		SBR可以改善感知音频编码的效率，在中等至低比特率下能提高大约30%（在某些特定情况下可能更高）。SBR具体能够提供的改善程度同时也依赖于其主体编码器。

​		举例说，联合MP3一起使用SBR（参考mp3PRO相关文章），我们可以在64kbps下达到相对传统的大于100kbps的立体声mp3的音频质量。SBR可应用于单声道，立体声甚至多声道的音频。

​		因此，可以说，SBR在主体编码器本身可编码的音频信号比特率范围和在有限的音频带宽下的可接受的编码噪音（coding artifacts）水平内提供了最大的效率。



### PS	参数立体声

​		Parametric Stereo，能让多个声道数据压缩更高效

​		在MPEG-4中被完整地定义，并且是aacPlus v2中的新的组成部分。为提高低比特率立体声信号的音频压缩效率。

​		参量立体声编码器从音频信号的立体声影像中分解出其参量表示，而在传统模式下原始信号只会被编码为单声道表示。立体声映像信息被表现为少量的高质量的参量立体声信息，与单声道信号在比特流中同时传输。然后，基于接收到的参量立体声信息，解码器便可重建立体声映像

​		合并有参量立体声的低比特率， 例如 24 kbps 的音频比特流， 所能让听众感受到的音频质量是显著地高于不具备参量立体声的相似音频比特流的质量。



## AAC编码流程

### MPEG-2 AAC编码流程

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMm6JSzz02Ipicn1UiarrT3XukaOTSzTlnN4kyDBdhxic7FQmnVEyL3516Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1" alt="图片"  />

### MPEG-2 AAC解码流程

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdM8fo9LBtib4utmGdZaQn6fxJOZ6Ccp7pFncrLlrBENArKjw8bEDZzhuQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



## AAC编码规格

​		为了能够适应于不同的应用场合，在 MPEG-2 AAC 标准中定义了三种不同编码规格：

> MPEG-4 标准包含了多种 AAC 的版本，在标准中定义了编解码工具模块、音频对象类型（Audio Object Types）、编码规格（Profiles）来指定编码器。

1）**MPEG-2 AAC LC（Low Complexity），低复杂度规格。**用于要求在有限的存储空间和计算能力的条件下进行压缩场合。在这种框架中，没有预测和增益控制这两种工具，TNS 的阶数比较低。编码码率在 96kbps-192kbps 之间的可以用该规格。MP4 的音频部分常用该规格。

- 低复杂度规则，码流128K，音质好。主要应用于中高码率场景的编码（>80Kbit/s)

- 最基础，最常用

2）**MPEG-2 AAC Main，主规格。**具有最高的复杂度，可以用于存储量和计算能力都很充足的场合。在这种框架中，利用了除增益控制以外的所有编码工具来提高压缩效率。

3）**MPEG-2 AAC SSR（Scalable Sample Rate），可变采样率规格。**在这种框架中，使用了增益控制工具，但是预测和耦合工具是不被允许的，具有较低的带宽和 TNS 阶数。对于最低的一个 PQF 子带不使用增益控制工具。当带宽降低时，SSR 框架的复杂度也可降低，特别适应于网络带宽变化的场合。

> Main 和 LC 框架是变化编码算法，采用 MDCT 作为其时/频分析模块，SSR 框架则采用混合滤波器组，先将信号等带宽地分成 4 个子带，再作 MDCT 变换。在三种方案里，通过选用不同模块在编码质量和编码算法复杂度之间进行折衷。

​		在 MPEG-4 AAC 标准中除了继承上面的三种规格进行改进外，通过附加的编码技术（PS，SBR等），新增了三种编码规格：



4）**MPEG-4 AAC LD（Low Delay），低延迟规格。**AAC 是感知型音频编解码器，可以在较低的比特率下提供很高质量的主观音质。但是这样的编解码器在低比特率下的算法延时往往超过 100ms，所以并不适合实时的双向通信。结合了感知音频编码和双向通信必须的低延时要求。可以保证最大 20ms 的算法延时和包括语音和音乐的信号的很好的音质。

5）**MPEG-4 AAC LTP（Long Term Predicition），长时预测规格。**在 Main Profile 的基础上增加了前向预测。

6）**MPEG-4 AAC HE（High Efficiency），高效率规格。**混合了 AAC 与 SBR（Spectral Band Replication，频段复制）技术。而新版本的 HE，即 HE v2 是 AAC 加上 SBR 和 PS（Parametric Stereo，参数立体声）技术。这样能在同样的效果上使用更低的码率。在编码码率为 32-96 Kbps 之间的音频文件时，建议首选这种规格。

- HE-AAC （v1）

  ​	按频谱分别保存，扶贫编码保存主要部分，高频单独放大编码保存音质，码流64K左右。

  ​	AAC + SBR， 主要应用于中低码率场景下的编码（<=80Kbit/s) 

- HE-AAC v2

  ​	双声道中的生意你存在某种相似性，只需存储一个声道的全部信息，然后，花很少的字节用参数描述另一个声道和它的差异。

  ​	AAC + SBR + PS，主要应用于低码率场景的编码(<=48Kbit/s)




![image-20240225193009769](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20240225193009769.png)

​		大部分编码器都设置为<=48Kbit/s自动启用PS技术，而>48Kbit/s则不加PS。

- 小于128Kbit/s的码率下表现优异，并且多用于视频中的音频编码

- 适用于128Kbit/s以下的音频编码，多用于视频总的音频轨的编码

- **低码率**，128Kbps以下，不同模式的主观音质会有所区别，**AAC-HE**会比常用的AAC-LC**更加优秀**。

- 但一般音频码率都会在128Kbps以上，所以一般选用常规的AAC-LC即可。



### Audio Object Types

​		最主要的标记编码器的方式。下图是常用的 MPEG-4 音频对象类型：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMo3iaeXhvZsQiaUDzso8t4Y4HibJwsmuCucHohlhnJKqibxNQmerQDicDX6A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)







## AAC  ES

​		AAC ES是指仅经过AAC编码器编码，还未进行ADTS或ADIF封装的原始编码数据流。

​		AAC ES 通常由一系列连续的AAC音频帧组成：

- 每帧以特定标志符开始，表示这是一个AAC音频帧
- 每个音频帧拥有相同的长度（`1024`个样本时间段）
  - `1024`个样本时间段，但是并不一定包含相同数量的采样点
    - 采样率和声道数量可能会发生变化
- AAC ES 采用大端字节顺序，其中高位字节排在前面，低位字节排在后面
- AAC ES中，音频数据按照从左到右、自上而下的顺序排列



## AAC封装格式

​		AAC一般指AAC编码，但在比较广泛的上下文中，AAC指的是一种包含了编码和封装格式两部分的音频封装码流。

> MPEG-4 的传输或存储格式一般都包含 Raw Data Blocks 或 Access Units，其中装载的是实际的音频编码数据的比特流。这些比特流又通过灵活的方式被分为代表不同声道的部分。
>
> 取到这些数据后，我们就要进一步解析它们的数据格式了。

​		MPEG-2 AAC 的音频编码数据格式有以下两种：

- `ADIF（Audio Data Interchange Format）`，音频数据交换格式
  - 有且只有一个文件头部，存储用于音频解码播放的信息（例如采样率，通道数等）
  - 所以解码播放必须从文件头部开始，一般用于存储在本地磁盘中音频文件的播放
- `ADTS（Audio Data Transport Stream）`，音频数据传输流
  - 有同步字的比特流，解码可以在这个流中任何位置开始
  - 可以将数据看做一个个的音频帧，而每帧都存储了用于音频解码播放的头信息（例如采样率，通道数等）
  - 多个音频帧构成一个音频序列
  - 可以从任何帧位置解码播放，更适用于流媒体传输

> [该网站](https://www.p23.nl/projects/aac-header/)提供了一个解析 AAC ADTS Frame Header 的工具，可以输入头部 7 或 9 个字节的数据，点击 Submit 就能看到头部各字段对应的含义。

​		MPEG-4 AAC 又增加了两种音频编码数据格式，新增的格式不仅针对传统的 AAC，还针对新的变体：AAC-LD、AAC-ELD。

- `LATM（Low-overhead MPEG-4 Audio Transport Multiplex）`
  - 这种格式有独立的比特流，允许使用 MPEG-4 中的错误恢复语法
- `LOAS（Low Overhead Audio Stream）`
  - 这种格式是带有同步信息的 `LATM`，可以支持随机访问或跳过

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMSjTaq8xeFZxbJFFwhmrgYNW5xejaFrGdXlH3kx3zoGSh1MoMehgR7g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMvZxqeCoG0VquVYYehVMno2zom51USsUsib6tmIiaTuYvuA1yx2wHaCBw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

### Audio Specific Config

​		在传输和存储` MPEG-4 `音频时，音频对象类型（Audio Object Types）和音频的基础信息（比如采样率、位深、声道）必须被编码，这些信息通常在 `AudioSpecificConfig `数据结构中来指定。

​		**AudioSpecificConfig 的信息使得我们可以不用传输 AAC 比特流就能让解码器理解音频的相关信息，这对于编解码器协商期间的设置很有用**，比如SIP的初始化协商阶段。

> MPEG-2 不会指定 AudioSpecificConfig，所以它的 ADIF 和 ADTS 采用固定的 1024 个采样的长度。

​		它包含了两部分内容：

- 常规区域，包含了大部分 MPEG-4 音频规格的通用字段；
- 特定区域，包含了不同音频对象类型（Audio Object Types）的特性字段。

> ​		比如，对于 AAC-LC、HE-AAC、AAC-LD 特定区域就会包含 `GASpecificConfig `特性内容；对于 AAC-ELD 则包含 ELDSpecificConfig 特性内容；对于 xHE-AAC(USAC) 则包含 UsacConfig 特性内容。
>
> ![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMr0rsvowNJcHjzd0eWmeglWd3zrNHCNrPTEq96x5FDjX53YuvRBedkw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

### ADIF

​		`ADIF（Audio Data Interchange Format）`，音频数据交换格式。

​		只在**文件头部存储**用于音频解码播放的头信息（例如采样率，通道数等），它的解码播放必须从文件头部开始，一般用于存储在本地磁盘中播放。

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdM4iasRs87ibiamU55w7TQAn1m3tZw8kHtnSpw0mwI9HRk353muCAaLyicbg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



### ADTS

​		`ADTS（Audio Data Transport Stream）`，音频数据传输流。由7或9个字节组成。

​		将数据看做连续的音频帧（`ADTS Frame`），**每帧都存储**了用于音频解码播放的头信息（例如采样率，通道数等），即可以从任何帧位置解码播放，更适用于流媒体传输。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMxibWyq9oF7ox6DA30e4Fzcdj1qByaExn70iaPB6UTw5bMjl8d150ownw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1" alt="图片" style="zoom:50%;" />

-  ADTS Frame 组成AAC 码流
  - ` ...| ADTS Frame | ADTS Frame | ADTS Frame | ADTS Frame | ADTS Frame |...`
- 把AAC的ES流打包成ADTS的格式即ADTS Frame
  - 在AAC ES流前添加7个字节的ADTS header
  - 每个 ADTS Frame 是由 头部和数据组成
    - 头部包含 固定头部和可变头部

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/3311dba799a441bda48618bab8811ba7.png)

​		

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/527383512e8c48eba228bdc71bfe98ba.png)



#### 固定头部

​		`28 bit`

- `Syncword`， `12 bit`，ADTS文件的标志符

  - 它用于确定音频帧的开始位置和结束位置
  - 通常为`0xFFF`

- `MPEG version ID`， `1 bit`，  使用的MPEG版本

  - `0` 表示 `MPEG-4`
  - `1` 表示 `MPEG-2`

- `Layer`， `2 bit`，  所有位必须为`0`

- `Protection Absent` ，`1 bit`，  是否存在 CRC 校验字段

  - `0` 表示存在 CRC 校验字段
  - `1` 表示不存在 CRC 校验字段

- `Profile `， `2 bit`，AAC规格

  - `0` ，`AAC Main`
  - `1` ，`AAC LC`
  - `2` ，AAC SSR`
  - `11`， `AAC LTP`

- `MPEG-4 Sampling Frequence Index`，  `4 bit` ， 采样率索引

  - `0 `表示 `96000Hz`
  - `4 `表示 `44100Hz`
  - `11 `表示 `8000Hz`
  - [详见此处](https://wiki.multimedia.cx/index.php/MPEG-4_Audio#Sampling_Frequencies)

- `Private Stream` ，`1 bit`  

  - 编码时将该值设为 `0`
  - 解码时忽略

- `MPEG-4 Channel Configuration` ，`3 bit`，通道数，指示音频的通道数

  - 如单声道、立体声或多声道等
  - 取值为0时，通过`inband `的`PCE`设置`channel configuration`

- `Originality/copy`， `1 bit`，  `Originality`指示编码数据是否被原始产生

  - 编码时将该值设置为 `0`
  - 解码时忽略

- `home`，`1 bit`  

  - 编码时将该值设为 0
  - 解码时忽略

  

  

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20161028130919091)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/a54370ede8963b6c1025e5350d62c0b3.png)



#### 可变头部

​		`28 bit`

- `Copyrighted Stream`， `1 bit` 
  - 编码时将该值设为 0，解码时忽略
- `Copyrighted Start` ，`1 bit` 
  - 编码时将该值设为 0，解码时忽略
- `Frame Length` ，`13 bit`  ，ADTS 帧长度
  - 包括ADTS头部长度和AAC ES长度，单位Byte
-  `Buffer Fullness`，`11 bit` 
  -  `0x7FF `时表示动态码率
  - `0x000 `表示固定码率的码流
- `Number of AAC Frames` ，`2 bit`  
  - 值为 ADTS 帧里的 AAC 帧数量减一
  - 为了最大的兼容性通常每个ADTS frame 包含一个AAC frame，所以该值一般为0
  - AAC原始帧包含一段时间内`1024`个采样及相关数据

#### CRC

- CRC ，`16 bit `
  - 可能存在校验和字段，视固定头部中`Protection Absent`字段而定
  - 没有则ADTS头部7字节
  - 有则ADTS头部9字节





```c
// 贴上ffmpeg中添加ADTS头的代码，就可以很清晰的了解ADTS头的结构：

int ff_adts_write_frame_header(ADTSContext *ctx, 
                               uint8_t *buf, int size, int pce_size) 
{ 
    PutBitContext pb; 
 
    init_put_bits(&pb, buf, ADTS_HEADER_SIZE); 
 
    /* adts_fixed_header */ 
    put_bits(&pb, 12, 0xfff);   /* syncword */ 
    put_bits(&pb, 1, 0);        /* ID */ 
    put_bits(&pb, 2, 0);        /* layer */ 
    put_bits(&pb, 1, 1);        /* protection_absent */ 
    put_bits(&pb, 2, ctx->objecttype); /* profile_objecttype */ 
    put_bits(&pb, 4, ctx->sample_rate_index); 
    put_bits(&pb, 1, 0);        /* private_bit */ 
    put_bits(&pb, 3, ctx->channel_conf); /* channel_configuration */ 
    put_bits(&pb, 1, 0);        /* original_copy */ 
    put_bits(&pb, 1, 0);        /* home */ 
 
    /* adts_variable_header */ 
    put_bits(&pb, 1, 0);        /* copyright_identification_bit */ 
    put_bits(&pb, 1, 0);        /* copyright_identification_start */ 
    put_bits(&pb, 13, ADTS_HEADER_SIZE + size + pce_size); /* aac_frame_length */ 
    put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */ 
    put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */ 
 
    flush_put_bits(&pb); 
 
    return 0; 
}
```



### LOAS

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh8rjibvrKYLZ7jWN4IUkSdMenAOqxZYOC72sbMamBptL9tLVNHz3L7oLIPDIRibuwLtnV7ra9Aa6uA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



## 实例：AAC文件解析

下图是用编辑器打开一个AAC文件，用十六进制查看的截图：

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/c74101fcc75e4e9486f6631b655a2daf.png)

没有CRC的情况下，文件开头的7个字节是ADTS帧头部，这里7个字节的数据是：0xff 0xf1 0x4c 0x80 0x20 0x02 0x80，我们按照ADTS帧的头部数据来解析看看这7个字节表示什么?

前面1-12bit为 0xff 0xf，对应了ADTS头部的 syncword 字段，表示ADTS帧的开始。

13-16bit为0x01，二进制是 0001，也就是说：

ID的1bit为0(MPEG-4)；
layer的2bit为00；
protection_absent的1bit为1，表示没有CRC，整个头部7个字节。
17-24bit为0x4c，二进制是 0100 1100，意思是：

profile_ObjectType的2bit为 01，结合前面ID为0，表示MPEG-4 AAC LC 规格；
sampling_frequency_index的4bit为0011，也就是0x3，表示采样率为48000；
private_bit的1bit为0；
剩余1bit，结合后面的再看；

25-32bit为0x80，二进制是 1000 0000，意思是：

channel_configuration的3bit(结合前面剩下的1bit)为 010，表示双声道；
original_copy的1bit为0；
home的1bit为0；
copyright_identification_bit的1bit为0；
copyright_identification_start的1bit为0；
剩余2bit，结合下个字段再看；

33-44bit为0x200，二进制是 0010 0000 0000，加上前面剩下的2bit，就是00 0010 0000 0000：

frame_length的13bit为00 0010 0000 000，也就是0x100，表示这个ADTS帧的长度是0x100；那么下个ADTS就是0x100开始的；
剩余1bit，留到下个字段再看；

45-56bit为0x280，二进制是 0010 1000 0000，加上前面剩下的1bit，就是 0 0010 1000 0000：

adts_buffer_fullness的11bit为 0 0010 1000 00，十六进制0xa0；
number_of_raw_data_blocks_in_frame的2bit为 00，表示包含一个AAC frame。
后面的ADTS帧也可以类似上面的过程去解析。frame_length是代表了整个ADTS的大小。