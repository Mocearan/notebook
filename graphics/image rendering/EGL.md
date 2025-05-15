# EGL

---

​		` OpenGL ES`定义了一套移动设备的图像渲染 `API`，但是并没有定义窗口系统。`EGL `是 `OpenGL ES` 渲染 `API `和本地窗口系统之间的一个中间接口层，它主要由系统制造商实现。

​		`EGL `提供如下机制：

- 与设备的原生窗口系统通信；
- 查询绘图图层的可用类型和配置；
- 创建绘图图层；
- 在 `OpenGL ES` 和其他图形渲染 `API `之间同步渲染；
- 管理纹理贴图等渲染资源。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh14ciaCSUowMCicdj6acicYomCiamz4L43fU4FZE1QVibjMhsic0icYcgFuzsEiarbfGep3F5Waib264kaBeQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:50%;" />

## 参考



https://www.khronos.org/egl



[一看就懂的 OpenGL 基础概念（2）：EGL，OpenGL 与设备的桥梁丨音视频基础](https://mp.weixin.qq.com/s?__biz=MjM5MTkxOTQyMQ==&mid=2257486808&idx=1&sn=dcbf5393e4bdfd548ea5b6fe5a231ee5&chksm=a5d41f8a92a3969c670ac272f7db8ed4e14118faa5f3a45f18e7e22de66b55495f09a3e922fb&scene=178&cur_album_id=2140155659944787969&search_click_id=#rd)

[1]EGL 作用及其使用: *https://blog.csdn.net/u010281924/article/details/105296617*

[2]EGL: *https://blog.csdn.net/Kennethdroid/article/details/99655635*

[3]iOS OpenGL ES 应用开发实践指南: *https://book.douban.com/subject/24849591/*

[4]iOS OpenGL ES Programming Guide: *https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/WorkingwithEAGLContexts/WorkingwithEAGLContexts.html*

[5]OpenGL ES 在 iOS 中的上下文环境搭建: *https://www.jianshu.com/p/c34c14589e0c*

## Android EGL

​		`Android `平台自` 2.0` 版本之后图形系统的底层渲染均由 `OpenGL ES `负责。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh14ciaCSUowMCicdj6acicYomnCkvJ2icKLicHD14sib5p5OYOfkSug7vIJ6taiaksCBFyN6xpEjCOdwZDw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片"  />

- **Display** 是对实际显示设备的抽象。在 Android 上的实现类是 `EGLDisplay`。
- **Surface** 是对用来存储图像的内存区域 FrameBuffer 的抽象，包括 Color Buffer、Stencil Buffer、Depth Buffer。在 Android 上的实现类是 `EGLSurface`。
- **Context** 存储 OpenGL ES 绘图的一些状态信息。在 Android 上的实现类是 `EGLContext`。



本地窗口相关的 API 提供了访问本地窗口系统的接口，而 EGL 可以创建渲染表面 EGLSurface ，同时提供了图形渲染上下文 EGLContext，用来进行状态管理，接下来 OpenGL ES 就可以在这个渲染表面上绘制。

使用 EGL 在平台实现渲染步骤大致如下：

- 1）调用 `eglGetDisplay` 来获得 EGLDisplay 对象，从而**建立与平台窗口系统的联系**，这个 EGLDisplay 将作为 OpenGL ES 的渲染目标；
- 2）调用 `eglInitialize` **初始化 EGL**；
- 3）调用 `eglChooseConfig` 来获得 EGLConfig 对象，从而**确定渲染表面的配置信息**；
- 4）通过 EGLDisplay 和 EGLConfig 对象，调用 `eglCreateWindowSurface` 或 `eglCreatePbufferSurface` 方法得到 EGLSurface，从而**创建渲染表面**，其中 eglCreateWindowSurface 用于创建屏幕上渲染区域，eglCreatePbufferSurface 用于创建离屏渲染区域；
- 5）通过 EGLDisplay 和 EGLConfig，调用 `eglCreateContext` 获得 EGLContext 对象，从而**创建渲染上下文**，OpenGL 的任何一条指令都是必须在自己的 OpenGL 上下文环境中执行；
- 6）调用 `eglMakeCurrent` 将 EGLSurface、EGLContext、EGLDisplay 三者绑定就**完成了上下文绑定**，绑定成功之后 OpenGL ES 的环境就创建好了，接下来就可以开始渲染了；

> 通过上面的步骤就做好了 EGL 的准备工作：一方面为 OpenGL ES 渲染提供了目标 EGLDisplay 及上下文环境 EGLContext，可以接收到 OpenGl ES 渲染出来的纹理；另一方面我们连接好了设备显示屏 EGLSurface（这里可能是 SurfaceView 或者 TextureView）。接下来，由于 OpenGL ES 的渲染必须新开一个线程，并为该线程绑定显示设备及上下文环境（EGLContext），所以 `eglMakeCurrent()` 就是来绑定该线程的显示设备及上下文的。

- 7）OpenGL ES 完成绘制后，调用 `eglSwapBuffers` 方法**交换前后缓冲，将绘制内容显示到屏幕上**，而离屏渲染不需要调用此方法；

> 这里需要注意的是 EGL 的工作模式是**双缓冲模式**，其内部有两个 FrameBuffer（帧缓冲区）：BackFrameBuffer 和 FrontFrameBuffer，当 EGL 将一个 FrameBuffer 显示到屏幕上的时候，另一个 FrameBuffer 就在后台等待 OpenGL ES 进行渲染输出。
>
> 直到调用了 `eglSwapBuffer()` 这条指令的时候，才会把前台的 FrameBuffer 和后台的 FrameBuffer 进行交换，这时界面呈现的就是 OpenGL ES 刚刚渲染的内容了。
>
> 这样做的原因是如果应用程序使用单缓冲绘图时可能会存在图像闪烁的问题，因为图像生成不是一下子被绘制出来的，而是按照从左到右、从上到下逐像素绘制的。如果最终图像不是在瞬间全部展示给用户，而是通过把绘制过程也展示出来了，这会导致用户看到的渲染效果出现闪烁。为了规避这个问题，可以使用双缓冲渲染：前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制，对用户屏蔽从左到右、从上到下逐像素绘制的过程，这样就可以避免闪烁了。

- 8）绘制结束后，不再需要使用 EGL 时，需要调用 `eglMakeCurrent` 取消绑定，调用 `eglDestroyContext`、`eglDestroySurface`、`eglTerminate` 等函数销毁 EGLDisplay、EGLSurface、EGLContext 三个对象。

> 在[《RenderDemo（1）：用 OpenGL 画一个三角形》](https://mp.weixin.qq.com/s?__biz=MjM5MTkxOTQyMQ==&mid=2257486728&idx=1&sn=359684a372e33215a1ea7b4f502f9e8c&scene=21#wechat_redirect) Android Demo 的 `KFGLContext` 类中就可以看到上面这套流程。
>
> 不过，如果你觉得上述配置 EGL 的流程太麻烦的话，Android 平台提供了 `GLSurfaceView` 类实现了 Display、Surface、Context 的管理，即 GLSurfaceView 内部实现了对 EGL 的封装，可以很方便地利用接口 GLSurfaceView.Renderer 的实现，使用 OpenGL ES API 进行渲染绘制。GLSurfaceView 提升了 OpenGL ES 开发的便利性，当然也相应的失去了一些灵活性。





## iOS EAGL

​	iOS 平台对 EGL 的实现是 **EAGL（Embedded Apple Graphics Library）**。OpenGL ES 系统与本地窗口（UIKit）系统的桥接由 EAGL 上下文系统实现。

与 Android EGL 不同的是，iOS EAGL 不会让应用直接向 BackFrameBuffer 和 FrontFrameBuffer 进行绘制，也不会让应用直接控制双缓冲区的交换（swap），系统自己保留了这些操作权，以便可以随时使用 Core Animation 合成器来控制显示的最终外观。

**Core Animation 是 iOS 上图形渲染和动画的核心基础架构。**可以使用托管多种 iOS 系统内容的图层（UIKit、Quartz 2D、OpenGL ES），来合成应用的用户界面或者其他视觉显示。

OpenGL ES 通过 CAEAGLLayer 与 Core Animation 连接，CAEAGLLayer 是一种特殊类型的 Core Animation 图层，它的内容来自 OpenGL ES 的 RenderBuffer，Core Animation 将 RenderBuffer 的内容与其他图层合成，并在屏幕上显示生成的图像。所以同一时刻可以有任意数量的层。Core Animation 合成器会联合这些层并在后帧缓存中产生最终的像素颜色，然后切换缓存。

<img src="https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh14ciaCSUowMCicdj6acicYomkkOwibxnHIiadiboC4LR605YicF8eYLVcqMiad68ApnWqZZLSt9X6IkhgDQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1" alt="图片" style="zoom:67%;" />

​																Core Animation 与 OpenGL ES 共享 RenderBuffer

一个应用提供的图层与操作系统提供的图层混合起来可以产生最终的显示外观。如下图所示，OpenGL ES 图层显示了一个应用生成的旋转立方体，但是在显示器顶部的显示状态栏图层则是由操作系统生成和控制的，此图显示的是合并两个图层来产生后帧缓存中的颜色数据的过程，交换后，我们看到的就是前帧缓存上的内容。

![图片](https://mmbiz.qpic.cn/mmbiz_png/gUnqKPeSueh14ciaCSUowMCicdj6acicYomJ8Wk4nrCzAp8tOhPoWtZKMzgRK0znaQy26YvO4S0OSNJtPCHkLbfbw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

​																					iOS 多图层合成

所以，iOS 的 EAGL 配置过程其实就是使用 CoreAnimation 的 layer 来支持 OpenGL ES 渲染的过程，步骤大致如下：

- 1）创建一个 EAGL 图层 `CAEAGLLayer` 对象，并设置好它的属性；
- 2）创建 OpenGL ES 上下文 `EAGLContext`，并设置为当前上下文环境；
- 3）创建一个颜色渲染缓冲区对象 `ColorRenderBuffer`，并调用 `renderbufferStorage:fromDrawable:` 为其分配存储空间，这里其实是将 CAEAGLLayer 的绘制存储区共享为了 ColorRenderBuffer 的绘制缓冲区。分配缓冲区需要的宽、高、像素格式等信息都会从 layer 中取得；

> 需要注意的是，如果 CAEAGLLayer 的 bounds 或其他属性变了，需要重新分配 ColorRenderBuffer 的存储空间，否则会出现 ColorRenderBuffer 和 CAEAGLLayer 的尺寸不匹配。

- 4）创建帧缓冲区 `FrameBuffer` 对象，并将 ColorRenderBuffer 绑定为它的附件；
- 5）从颜色渲染缓冲区 ColorRenderBuffer 获取宽高信息；
- 6）根据需要创建一个深度渲染缓冲区 `DepthRenderBuffer` 对象，并绑定为 FrameBuffer 的附件；
- 7）根据需要检测 FrameBuffer 的状态；
- 8）将 CAEAGLLayer 添加到 Core Animation 的图层树中；
- 9）在绘制动作完成后，调用 EAGLContext 的 `presentRenderbuffer:` 方法，就可以将绘制结果显示在屏幕上了。

> 在[《RenderDemo（1）：用 OpenGL 画一个三角形》](https://mp.weixin.qq.com/s?__biz=MjM5MTkxOTQyMQ==&mid=2257486728&idx=1&sn=359684a372e33215a1ea7b4f502f9e8c&scene=21#wechat_redirect) iOS Demo 的 `DMTriangleRenderView` 类中可以看到类似的流程，只不过 Demo 中我们是创建了一个 UIView 的子类，并重写它的 `+layerClass` 方法返回 `CAEAGLLayer` 类型来获得了一个 CAEAGLLayer 对象用于 OpenGL ES 渲染。
>
> 同样的，如果你觉得上述流程太麻烦，iOS 平台还提供了封装好的 `GLKView` 来简化我们使用 OpenGL ES，GLKView 是对 CAEAGLLayer 的封装，内嵌了配置 Core Animation 支持 OpenGL ES 的流程。



























