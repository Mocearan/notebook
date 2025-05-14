# OpenGL

---

​		`OpenGL `是一套处理图形图像的统一规则。一般位于平台提供的基础绘制功能`API`之上一层。



## 参考

[1]了解 OpenGL 渲染架构: *https://www.jianshu.com/p/51be4551d36f*

[2]Learn OpenGL: *https://learnopengl-cn.github.io/01%20Getting%20started/01%20OpenGL/*

[3]OpenGL 工作机制: *https://juejin.cn/post/7121525553491869703*

[4]图形渲染管线的那些事: *https://juejin.cn/post/7119135465302654984*



[1]Learn OpenGL | 帧缓冲: *https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/05%20Framebuffers/*

[2]FBO 离屏渲染: *https://blog.csdn.net/Kennethdroid/article/details/98883854*

[3]关于 FBO 的一些理解: *https://www.cnblogs.com/chandler00x/p/3886062.html*



## 架构

​		`OpenGL `的渲染架构是 **Client/Server 模式**：

- `Client`（客户端）指的是在 `CPU `上运行的代码，如 `OC/C++/Java` 代码调用 `OpenGL `的一些 `API`
- 而 `Server`（服务端）则对应的是图形渲染管线，会调用 `GPU `芯片
- 不断用 `Client `通过 `OpenGL `通道向 `Server `端传输渲染指令，来间接的操作 `GPU `芯片

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk41mHk8Zk2ra230paBJhDiaAUXyUj2GPHxmA06XRq2hJxfdCYTpIxDACA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 67%;" />



### 通道

​		`OpenGL `提供了 3 个通道，用来从`Client`向`Server`中的顶点着色器（`Vertex Shader`）和片元着色器（`Fragment Shader`）传递参数和渲染信息。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4ibGwkMicTyIskfAqNWmyjYQX16XzONgviaKqO9FRyibL4Rpd7fyGoOianiag/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片"  />



这 3 个通道分别是：

- **`Attribute`（属性通道）**：通常用来传递经常可变参数。比如颜色数据、顶点数据、纹理坐标、光照法线这些变量。
- **`Uniform`（统一变量通道）**：通常用来传递不变的参数。比如变化矩阵。一个图形做旋转的时候，实质上是这个图形的所有顶点都做相应的变化，而这个变化的矩阵就是一个常量，可以用 Uniform 通道传递参数到顶点着色器的一个实例。再比如视频的颜色空间通常用 YUV，但是 YUV 颜色空间想要正常渲染到屏幕上面，需要转化成 RGBA 颜色空间，这个转换就需要把 YUV 的颜色值乘以一个转换矩阵转换为 RGBA 颜色值，这个转换矩阵也是一个常量，可以用 Uniform 通道传递参数到片元着色器的一个实例。
- **`Texture Data`（纹理通道）**：专门用来传递纹理数据的通道。

> 这 3 个通道中 `Uniform `通道和 `Texture Data` 通道都可以直接向顶点着色器和片元着色器传递参数，但是 `Attribute `只能向顶点着色器传递参数。
>
> 因为 `OpenGL `架构在最初设计的时候，`Attribute `属性通道就是顶点着色器的专用通道。片元着色器中是不可能有 `Attribute `的，但是可以使用 `GLSL `代码，通过顶点着色器把 `Attribute `信息间接传递到片元着色器中。
>
> 另外，虽然 Texture Data 通道能直接向顶点着色器传递纹理数据，但是向顶点着色器传递纹理数据本身是没有实质作用的，因为顶点着色器并不处理太多关于纹理的计算，纹理更多是在片元着色器中进行计算。



### 状态机

​		在 `Client/Server` 的渲染架构下，`OpenGl `的渲染流程其实是基于一个**状态机**来工作的。

​		用一系列变量描述 `OpenGL `此刻应当如何运行。`OpenGL `的状态通常被称为 `OpenGL `上下文（`Context`）。更改 OpenGL 状态的方式：设置选项，操作缓冲。使用当前 `OpenGL `上下文来渲染。

​		使用 `OpenGL`时，状态设置函数（`State-changing Function`）将会改变上下文。状态使用函数（`State-using Function`）会根据当前 `OpenGL `状态执行一些操作。

```c++
unsigned int VBO, VAO; 
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO); 

// 绑定VAO VBO
glBindVertexArray(VAO); 
glBindBuffer(GL_ARRAY_BUFFER, VBO); 
// 在当前VBO中处理数据
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
//解绑VBO VAO
glBindBuffer(GL_ARRAY_BUFFER, 0); 
glBindVertexArray(0);
```

>  OpenGL 的状态是可以**嵌套**的，所以上面的代码存在包含关系，因为一个 VBO 会被绑定于一个 VAO 中
>
> <img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4sQoIKU5xKRRx6AIPPKPjNrb8TNe4GGG4Y7ib7oI3sOuBfPicDVR4opBg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:50%;" />
>
> 通俗来说就是，执行了绑定 X 到解绑 X 之间的任何操作，都会影响到 X。也就是说OpenGL操作的对象是基于当前绑定的对象进行的。



### 渲染管线

​		一个一个状态的切换以及在不同状态中的渲染逻辑和数据处理构成了 OpenGL 的**渲染管线**。即一个**流程**。

​		`OpenGL `渲染管线的工序包括：**顶点着色器 → 图元装配 → 几何着色器 → 光栅化 → 片段着色器 → 测试与混合**。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4UhHiaSS5Rvz8LhSN7ROibcU7vmIZpLCf07UZ7HXEcjXge1OX6vRdOd8g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 80%;" />

> **渲染管线**做了哪些事情：
>
> 首先传入了图形的**顶点数据**，然后 OpenGL 内部会按照指定的**图元类型**自动将顶点连成图形，然后再将图形内的区域切成一个个**小片段**，然后给每个小片段自由**上色**，最后把被挡住的或者我们不想显示的区域的下片段**丢弃**，并且对有透明度的片段进行前后片段颜色的**混合**。

- **固定管线（Fixed Pipeline）**:**立即渲染模式（Immediate Mode）**，`OpenGL `的大多数过程都被库隐藏起来，每个计算核心上运行的是出厂时设定的若干配置好的程序以供选择。

- **可编程管线（programmable pipeline）**：允许用户通过编程接口，将一段程序注入到GPU的核心上运行以代替固定管线的预设程序，实现对GPU的可编程。

  > 这里的编程语言就是 `GLSL 语言`，它是一种类 C 的语言，专为图形计算量身定制，包含了一些针对向量和矩阵操作的有用特性，我们用它编写我们自己的顶点着色器和片段着色器。

​		

​		通过可编程管线注入到`GPU`核心运行的程序称为`Shader`，即着色器。 `OpenGL `主要有三种着色器：**顶点着色器**、**几何着色器**、**片段着色器**，其中顶点着色器和片段着色器为开发者必须提供，几何着色器为可选提供。



#### **顶点着色器（Vertex Shader）**

​		**确定绘制图形的形状，以及接收开发者传入的数据并传给后面阶段**。顶点着色器一次只接收处理一个顶点坐标，有多少个顶点就会执行多少次。

- 接收外部传入的顶点数据，根据需要对顶点数据进行变换处理之后，再将顶点数据传入下一个阶段图元装配。
- 另外顶点着色器也接收外部传进来的颜色值以及纹理采样器，然后再传递给下一个阶段进行图元装配处理。



#### **图元装配**

​		接收顶点着色器的输出数据，**将顶点着色器传来的顶点数据组装为图元**。

> 所谓图元，指的就是点、线、三角形等最基本的几何图形，再复杂的图形也离不开这些基本图形的组成。

​		另外，图元装配阶段还会将超出屏幕的顶点坐标进行裁剪，裁剪之后，顶点坐标被转化为屏幕坐标。



#### 几何着色器

​		非必选



#### **光栅化**

​		**将一个图元转化为一张二维的图片**。这张图片由若干个**片段（fragment）**组成，片段可以近似看成像素，但是又略有不同。一个片段包含渲染该片段所需要的位置、颜色和深度的全部信息。





#### **片段着色器（Fragment Shader）**

​		必须有的重要着色器，也是最后一个可以通过编程来控制屏幕上显示颜色的阶段（后面的混合测试阶段还可以改变片段的颜色），在这个阶段主要是**计算片段的颜色**。

​		这里片段着色器一次接收一个片段数据的输入，有几个片段就会执行所少次，根据具体需要灵活设置该片段的颜色。



#### **测试和混合**

​		测试是专门用来**丢弃一些不需要显示的片段**，其中测试主要包含**深度测试**和**模板测试**。

​		深度测试是在显示 3D 图形的时候，根据片段的深度来防止被阻挡的面渲染到其它面的前面。这里是 OpenGL 内部维护一个**深度缓冲**，保存这一帧中深度最小的片段的深度，然后对屏幕同一个位置的其他片段的深度再进行比较，深度比缓冲中大的片段则丢弃，直到找到深度最小的片段，就将其显示出来。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4rslX799WuLFW8xrZ6ibt4rSUB3M2icViaEBlvNVGxx1fDe8bMibsYz1orA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 50%;" />



> 每个方格表示一个片段，片段上的数值表示当前片段的深度，R 则表示深度无限，加号表示 2 个图形叠加一起，则由下面部分的图可知，当 2 个图形叠加在一起的时候，同一个位置的片段总是显示深度较小的那一个。

​		模板缓冲区用于控制屏幕需要显示的内容，屏幕大小决定了模板缓冲区大小；模板测试基于**模板缓冲区**，从而让我们完成想要的效果。模板测试类似于**与运算**：

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4PvGqibryUJ5N8owGPdnR5gibhM9WMtvmHRNJkRHiciaYlaib6P95jMP1ic1Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:50%;" />

> 模板就是每个片段位置有 0 也有 1，然后和缓冲中的图像数据对应片段进行类似与运算，也类似与拿一个遮罩罩住，只留下 1 的对应片段显示出来。

​		混合则是**计算带有透明度的片段的最终颜色**，在这个阶段会与显示在它背后的片段的颜色按照透明度进行叠加行成新的颜色，通俗讲就是形成透明物体的效果。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4hBw5js9G8dvsYL6szBdSa6Z5JkFRPOCpymsEXvoA9B8v1WefUEibcuw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 67%;" />



## Object

- **VBO（Vertex Buffer Objects）顶点缓冲区对象**，指的是在 `GPU `显存里面存储的顶点数据（位置、颜色）。
- **EBO/IBO（Element/Index Buffer Object）索引缓冲区对象**，指的是为了更高效的利用数据，存储索引来达到减少重复数据的索引数据。
- **VAO（Vertex Array Object）顶点数组对象**，主要作用是用于管理 `VBO `或 `EBO`，减少 `glBindBuffer`、`glEnableVertexAttribArray`、`glVertexAttribPointer` 这些调用操作，高效地实现在顶点数组配置之间切换。
- **FBO（Frame Buffer Object）帧缓冲区对象**，用于离屏渲染，是渲染管线的最终输出目标。



### VBO 

​		`GPU`显存中顶点数据缓冲区对象，避免每次绘制时 CPU 内存到 GPU 显存的数据拷贝，从而提升渲染性能。

```c
// 顶点数据：
GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// 使用 VBO：
GLuint VBO;
glGenBuffers(1, &VBO); // 创建 VBO 对象
glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把新创建的 VBO 绑定到 GL_ARRAY_BUFFER 目标上，同时也绑定到了 OpenGL 渲染管线上
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 将顶点数据 (CPU 内存) 拷贝到 VBO（GPU 显存）

// 绘制：
glDrawArrays(GL_TRIANGLES, 0, 3); // 使用 glDrawArrays 来绘制
```



### EBO

​		`GPU`显存中图元索引缓冲区对象，避免每次绘制时 CPU 内存到 GPU 显存的数据拷贝，从而提升渲染性能。

> 一个复杂图形通常由基础的图元组合而成，其中有很多顶点被各个图元共用。为描述各个图元，使用索引来描述各个图元所使用的顶点在顶点数组中的位置。

```c
// 这次我们只定义了 4 个顶点：
GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f, // 右上角
     0.5f, -0.5f, 0.0f, // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f,  0.5f, 0.0f  // 左上角
};

// 但是通过索引指定了每个三角形的 3 个顶点：
GLuint indices[] = { // 注意索引从 0 开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

// 使用 VBO：
GLuint VBO;
glGenBuffers(1, &VBO); // 创建 VBO 对象
glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把新创建的 VBO 绑定到 GL_ARRAY_BUFFER 目标上，同时也绑定到了 OpenGL 渲染管线上
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 将顶点数据 (CPU 内存) 拷贝到 VBO（GPU 显存）

// 使用 EBO：
GLuint EBO;
glGenBuffers(1, &EBO); // 创建 EBO 对象
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 把新创建的 EBO 绑定到 GL_ELEMENT_ARRAY_BUFFER 目标上，同时也绑定到了 OpenGL 渲染管线上
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 将顶点数据 (CPU 内存) 拷贝到 EBO（GPU 显存）

// 绘制：
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 使用 glDrawElements 来绘制
```



### VAO

 		 `VBO`、`EBO `减少 `CPU `到 `GPU `内存拷贝来提高性能。

​		**`VAO`** 储存绑定正确的缓冲对象并为每个物体配置所有顶点属性，用于管理 `VBO `或 `EBO`，减少 `glBindBuffer`、`glEnableVertexAttribArray`、`glVertexAttribPointer` 这些调用操作，高效地实现在顶点数组配置之间切换。。

```c
// 顶点数据：
GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// 创建 VBO：
GLuint VBO;
glGenBuffers(1, &VBO); // 创建 VBO 对象

// 创建 VAO：
GLuint VAO;
glGenVertexArrays(1, &VAO); // 创建 VAO 对象，注意这里用的是 glGenVertexArrays

// 在绑定 VAO 后操作 VBO，当前 VAO 会记录 VBO 的操作，我们下面用缩进表示操作 VBO 的代码：
glBindVertexArray(VAO); // 绑定 VAO，注意这里用的是 glBindVertexArray
    // 绑定 VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 把顶点数组复制到缓冲中供 OpenGL 使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
// 解绑 VAO
glBindVertexArray(0);

// ...省略其他代码...

// 会被调用多次的绘制代码：
glBindVertexArray(VAO); // 绑定使用 VAO 绘制
glDrawArrays(GL_TRIANGLES, 0, 3); // 使用 glDrawArrays 来绘制
glBindVertexArray(0); // 解绑 VAO
```



### FBO

​		建立了 `OpenGL `的渲染环境后，一块默认的帧缓冲区（`Default Frame Buffer`）随之创建，默认缓冲区的输出目标是显示设备。

​		默认的帧缓冲区与一系列缓冲区相关联，其配置信息是建立 `OpenGL Context `的时候用户自定义的。一般来讲，必要的是颜色缓冲区和深度缓冲区，模板缓冲区、累加缓冲区是可选的。

​		`FBO `可以支持离屏渲染。以类似于默认真缓冲区的结构和关联的其他缓冲区，支持将渲染管线的输出渲染到`FBO`中，而不是直接渲染到显示设备上。

> `FBO `虽被称为缓冲区对象，但不是真正的缓冲设施，`OpenGL `并没有为它分配存储空间去存储渲染所需的几何、像素数据，我们可以认为它是一个指针的集合，这些指针指向了颜色缓冲区、深度缓冲区、模板缓冲区、累积缓冲区等这些真正的缓冲区对象，我们把这里的『指向关系』叫做**附着**。

​		 FBO 中的附着点类型有：**颜色附着**、**深度附着**和**模板附着**。这些附着点指向的缓冲区通常包含在某些对象里，我们把这些对象叫做**附件**，附件的类型有：**纹理（Texture）**或**渲染缓冲区对象（Render Buffer Object，RBO）**。

<img src="https://mmbiz.qpic.cn/mmbiz_jpg/gUnqKPeSueg6RLSYgiaicK2ODQmHoBnPKXH60rQrgbIPpv0gbRedXYXwg3jQC8A1bFTiaac6pGSGHoklyVnysMbGQ/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom: 67%;" />

- **纹理（Texture）**是一个可以往上绘制细节的 2D 图片（甚至也有 1D 和 3D 的纹理）。
  - 除了图像以外，纹理也可以被用来储存大量的数据，这些数据可以发送到着色器上进行计算和处理。
- **渲染缓冲区对象（Render Buffer Object，RBO）**则是一个由应用程序分配的 2D 图像缓冲区，可以分配和存储颜色、深度或者模板值，可以用作 `FBO `中的颜色、深度或者模板附着。

​		`FBO `是 `OpenGL `渲染管线的最终目标，但其实 `FBO `本身不直接用于渲染，而是要为其绑定好附件后才能作为渲染目标。所以，建构一个完整的 `FBO `需要满足下列条件：

- 必须往 `FBO `里面加入至少一个附件（颜色、深度、模板缓冲）；
- 其中至少有一个是颜色附件；
- 所有的附件都应该是已经完全做好的（已经存储在内存之中）；
- 每个缓冲都应该有同样数目的样本。

#### 使用纹理

​		当把一个纹理（`Texture`）附加到 `FBO `上的时候，所有渲染命令会写入到纹理上，就像它是一个普通的颜色/深度或者模板缓冲一样。使用纹理的好处是，所有渲染操作的结果都会被储存为一个纹理图像，这样我们就可以简单的在着色器中使用了。

```c
// 创建和绑定 FBO：
GLuint fbo;
glGenFramebuffers(1, &fbo); // 创建 FBO
glBindFramebuffer(GL_FRAMEBUFFER, fbo); // 绑定 FBO，注意：如果这里用 glBindFramebuffer(GL_FRAMEBUFFER, 0) 则是激活默认的帧缓冲区

// 创建纹理：
GLuint texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // 创建纹理和分配存储空间。传入 NULL 作为纹理的 data 参数，不填充数据，填充纹理数据会在渲染到 FBO 时去做。
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glBindTexture(GL_TEXTURE_2D, 0);

// 将纹理添加为 FBO 的附件，连接在颜色附着点：
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

// 检测 FBO：
GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
if (status != GL_FRAMEBUFFER_COMPLETE)
    printf("Frame buffer incomplete!\n");
else
    printf("Frame buffer complete!\n");

// ...省略其他代码...

```



#### 使用RBO

```c
// 创建和绑定 FBO：
GLuint fbo;
glGenFramebuffers(1, &fbo); // 创建 FBO
glBindFramebuffer(GL_FRAMEBUFFER, fbo); // 绑定 FBO，注意：如果这里用 glBindFramebuffer(GL_FRAMEBUFFER, 0) 则是激活默认的帧缓冲区

// 创建 RBO：
GLuint rbo;
glGenRenderbuffers(1, &rbo); // 创建 RBO
glBindRenderbuffer(GL_RENDERBUFFER, rbo); // 绑定 RBO，所有后续渲染缓冲操作都会影响到当前的渲染缓冲对象
glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, m_width, m_height); // 为 RBO 的颜色缓冲区分配存储空间

// 将 RBO 添加为 FBO 的附件，连接在颜色附着点：
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);

// 检测 FBO：
GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
if (status != GL_FRAMEBUFFER_COMPLETE)
    printf("Frame buffer incomplete!\n");
else
    printf("Frame buffer complete!\n");

// ...省略其他代码...


```



### 内存布局

```c
// Vertex Shader 
#version 330 core
layout (location = 0) in vec3 position; // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 color;    // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(position, 1.0);
    ourColor = color; // 将 ourColor 设置为我们从顶点数据那里得到的输入颜色
}


// Fragment Shader 
#version 330 core
in vec3 ourColor;
out vec4 color;

void main()
{
    color = vec4(ourColor, 1.0f);
}


// 创建 VBO 
GLfloat vertices[] = {
    // 位置               // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

GLuint VBO;
glGenBuffers(1, &VBO);  
glBindBuffer(GL_ARRAY_BUFFER, VBO);  
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```



这时候对应的 VBO 布局格式如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehRLl1TgIxWXxoC8f9zrBX9PWt7h7v4bywcwz50d53c5hWqibnmFMTVhUjiaL03H3R0yQTsfiabhe11Q/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1&tp=webp)

当 VAO 只管理 VBO 时，布局格式如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehRLl1TgIxWXxoC8f9zrBX9RyEic3FSLicuTdedQmIznAsibkuhQ8vWp7IvG4CiaK8paJ9nH2WibFkDUFQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

当 VAO 管理 VBO 和 EBO 时，布局格式如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSuehRLl1TgIxWXxoC8f9zrBX9iazdYeQoDURXE4FxgMNS4lqiaI72q2uBBdjg9KleIibF9cTqy8Bq5Cq1g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)



## 颜色混合

​		混合就是把两种颜色混在一起。

​		某一像素位置当前存储在颜色缓冲区的颜色和将要画上去的颜色，通过某种方式混在一起，从而实现特殊的效果。

​		一次渲染过程包含了多个阶段，包括顶点着色器、图元组装、栅格化、片元着色器、测试和混合等，最后将结果输出到 `FrameBuffer` 上。渲染管线最后一个阶段就是混合。

![图片](https://mmbiz.qpic.cn/mmbiz_jpg/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4Iicib31hTlUtZ9ZI2rTQyfOyLic4icAaiaEIbRWBd5YW6BcZgWlqen1RK9Q/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1&tp=webp)



###  源色与目标色

​		将要画上去的颜色称为`源颜色（Source Color）`，把颜色缓冲区中当前留存的颜色称为`目标颜色（Destination Color）`。

​		针对 `OpenGL` 渲染场景：

- 源颜色：`Shader `中 `gl_FragColor` 的颜色。
- 目标颜色：`glClearColor` 的颜色。



### 开启/关闭颜色混合

- `glEnable(GL_BLEND)` 表示开启颜色混合
  - 会对源颜色 `gl_FragColor` 与目标色 `glClearColor `进行混合
- `glDisable(GL_BLEND)` 关闭颜色混合
  - 默认情况下 `OpenGL `的颜色混合就是关闭的
  - 颜色透明通道 `alpha `值，是不起作用的
  - 不受目标颜色影响，直接渲染源颜色

```c
glEnable(GL_BLEND); // 开启颜色混合
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置颜色混合模式
glDisable(GL_BLEND); // 关闭颜色混合

///////////////////////////////////////////////////////////////////////////////////

// 缓冲区颜色
glClearColor(1, 0, 0, 1); // rgba，红色
glClear(GL_COLOR_BUFFER_BIT);

// Shader
void main()
{
 gl_FragColor = vec4(0.0, 1.0, 0.0, 0.0); // rgba，绿色
}
```



### 颜色混合模式

​		开启了颜色混合后，还需要通过 `glBlendFunc` 函数指定混合模式。

​		参数可以理解为混合因子，第一个参数代表源颜色混合因子，第二个参数代表目标颜色混合因子。混合因子可选值如下：

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueiaXRjTBJzARS4mdSdhPOXk4p2YiaVaictVlv0Xlfn5vqBf8CxiaJZfBuNAAEFxIk08duOWG1H83zN4Pg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1&tp=webp)

​		可以组合出来的混合模式有很多种，开发中常用的混合模式有 2 种：

```cc
// 缓冲区颜色
glClearColor(1.0, 0.0, 0.0, 1.0); // rgba，红色
glClear(GL_COLOR_BUFFER_BIT);
glEnable(GL_BLEND);

// Shader
void main()
{
 gl_FragColor = vec4(0.0, 1.0, 0.0, 0.0); // rgba，绿色
}
```

> 不开启颜色混合最终输出是绿色，开启时如下：

- 设置颜色混合模式：`glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`
  - `GL_SRC_ALPHA` 表示源颜色混合因子为源颜色的 alpha 值，`GL_ONE_MINUS_SRC_ALPHA` 表示目标颜色混合因子为 1 减去源颜色的 alpha 值。
  - 计算方法：源色为 `(0.0, 1.0, 0.0, 0.0)`，目标色为 `(1.0, 0.0, 0.0, 1.0)`，源颜色 alpha 值为 `0.0`，那么目标颜色混合因子为 `1 - 0.0 = 1.0`，通过`源颜色 * 源颜色混合因子 + 目标色 * 目标色混合因子`，最终得到混合后颜色：`(0.0*0.0+1.0*1.0, 1.0*0.0+0.0*1.0, 0.0*0.0+0.0*1.0)`为红色。
- 设置颜色混合模式：`glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)`
  - `GL_ONE` 表示源颜色混合因子为 1，`GL_ONE_MINUS_SRC_ALPHA` 表示目标颜色混合因子为 1 减去源颜色的 alpha 值。
  - 计算方法：源色为 `(0.0, 1.0, 0.0, 0.0)`，目标色为 `(1.0, 0.0, 0.0, 1.0)`，源颜色混合因子 `1`，alpha 值为 `0.0`，所以目标透明因子为 `1 - 0.0 = 1.0`，通过`源颜色 * 源颜色混合因子 + 目标色 * 目标色混合因子`，最终混合后颜色 `(0.0*1+1.0*1.0, 1.0*1+0.0*1.0, 0.0*1+0.0*1.0)` 为黄色。



### 颜色预乘

​		针对于解码后的图片颜色为 `(r, g, b, a)`，颜色预乘则是 rgb 乘以 alpha 值，预乘后的结果为 `(r*a, g*a, b*a, a)`，现在 `iOS `和 `Android `平台系统解码后的图片均是预乘后的结果。

- 对于预乘后的图片，我们做混合情况下需要使用 `glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)`，因为源颜色已经乘过了。
- 对于没有预乘的图片，我们做混合情况下需要使用 `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`，这种方式会进行源颜色乘法。











