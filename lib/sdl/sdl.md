# SDL

---

​		SDL（Simple DirectMedia Layer）是一个跨平台开发库，旨在通过 `OpenGL `和 `Direct3D `提供对音频、键盘、鼠标、游戏杆和图形硬件的低级访问。它被视频播放软件、模拟器和流行游戏使用。

​		`SDL`习惯性的视为跨平台的`DirectX`，但`SDL`的定位是轻量级的多媒体框架库，用于简化低层次跨平台对于图像、声音、输出、UI控制、线程管理等接口。但是

- 更高级的图形和音效控制需要配合专业的图形、音频库（`openGL/Vulkan/Metal`和`openAL`）等；
- 更高级的UI控制需要配合丰富的UI组件库（`Qt/DuiLib`等）；
- 更高级的线程管理及其他基础设施需要配合更强大的工具库（`Boost/Qt/c++ std/folly/Abseil`)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/4f93a75a51504bcda91af189dac059fc.png)

## 参考

​		[Lazy Foo' Productions - Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/index.php)



## 特性

- 视频
  - 3D图形：SDL 可与 OpenGL API 或 Direct3D API 结合使用，用于 3D 图形。
  - 加速 2D 渲染 API：支持简单的旋转、缩放和 alpha 混合，所有这些都使用现代 3D API 加速，使用 OpenGL 和 Direct3D 支持加速。
  - 创建和管理多个窗口。
- 输入事件
  - 提供的事件和 API 函数用于：
    - 应用程序和窗口状态更改
    - 鼠标输入
    - 键盘输入
    - 操纵杆和游戏控制器输入
    - 多点触控手势
  - 可以使用SDL_EventState() 启用或禁用每个事件。
  - 事件在发布到内部事件队列之前通过用户指定的过滤器函数。
  - 线程安全事件队列。
- 力反馈： Windows、Mac OS X 和 Linux 支持力反馈。
- 音频
  - 设置8位和16位音频、单声道立体声或5.1环绕声的音频播放，如果硬件不支持格式，可选择转换。
  - 音频在单独的线程中独立运行，通过用户回调机制填充。
  - 专为定制软件混音器设计，但SDL_mixer提供完整的音频/音乐输出库。
- 文件 I/O 抽象
  - 用于打开、读取和写入数据的通用抽象。
  - 对文件和内存的内置支持。
- 共享对象支持
  - 加载共享对象（Windows 上的 DLL，Mac OS X 上的 .dylib，Linux 上的 .so）。
  - 共享对象中的查找函数。
- 线程
  - 简单的线程创建 API。
  - 简单线程本地存储 API。
  - 互斥体、信号量和条件变量。
  - 无锁编程的原子操作。
- 计时器
  - 获取经过的毫秒数。
  - 等待指定的毫秒数。
  - 在单独的线程中创建与代码一起运行的计时器。
  - 使用高分辨率计数器进行分析。
- CPU 特性检测
  - 查询 CPU 数量。
  - 检测 CPU 特性和支持的指令集。
- 大小端支持
  - 检测当前系统的字节序。
  - 用于快速交换数据值的例程。
  - 读取和写入指定字节序的数据。
- 查询电源管理状态。



## 架构


​		`SDL`将功能分成下列数个子系统（subsystem）：

- Video（图像）：图像控制以及线程（thread）和事件管理（event）。
- Audio（声音）：声音控制。
- Joystick（摇杆）：游戏摇杆控制。
- Game Controller：游戏控制器
- CD-ROM（光盘驱动器）：光盘媒体控制。
- Window Management（视窗管理）：与视窗程序设计集成，不过android默认就是对应的surfaceView的窗口，只允许存在一个。
- Events（事件驱动）：处理事件驱动。
- Haptic：触摸事件响应。
- Timer：时间管理。
- Everything：包含上述所有选项

​		除此之外，还有一些单独的官方扩充函数库。这些库由官方网站提供，并包含在官方文档中，共同组成了SDL的“标准库”：

- SDL_image—支持时下流行的图像格式：BMP、PPM、XPM、 PCX、GIF、JPEG、PNG、TGA。
- SDL_mixer—更多的声音输出函数以及更多的声音格式支持。
- SDL_net—网络支持。
- SDL_ttf—TrueType字体渲染支持。
- SDL_rtf—简单的RTF渲染支持。



### Events

​		事件是指与用户输入、系统操作等相关的行为。事件包括键盘事件、鼠标事件、窗口事件、定时器事件等等。

```c
SDL_Event  代表一个事件
    
SDL_WaitEvent()：等待一个事件。该函数会一直等待，直到有事件发生才会返回。
SDL_PushEvent()：发送一个事件。可以使用该函数自定义事件，并把它加入到事件队列中。
SDL_PumpEvents()：将硬件设备产生的事件放入事件队列，用于读取事件，在调用该函数之前，必须调用SDL_PumpEvents搜集键盘等事件。
SDL_PeepEvents()：从事件队列提取一个事件。从事件队列提取一个或多个事件。它可以指定要提取的事件类型，并且可以选择是否要移除事件。
    
SDL_PollEvent()  检查是否有事件发生
```

> ​		实际应用中，一般会在主循环中处理事件。主循环会一直运行，直到程序退出。每次循环都会调用 `SDL_PollEvent()` 函数来检查是否有事件发生。
>
> ​		对于各种不同的事件类型，可以通过SDL提供的函数和数据结构来进行处理。
>
> ​		如果有事件发生，就根据事件类型执行对应的操作。当没有事件发生时，可以进行渲染等其他操作。



### Thread

​		SDL是跨平台的，因此它提供的跨平台的多线程封装接口。

```c
SDL_CreateThread()  SDL线程创建：
SDL_WaitThead()  SDL线程等待：
SDL_CreateMutex() / SDL_DestroyMutex()  SDL互斥锁：
SDL_LockMutex() / SDL_UnlockMutex()  SDL锁定互斥：
SDL_CreateCond() / SDL_DestoryCond() SDL条件变量（信号量）：
SDL_CondWait() / SDL_CondSingal() SDL条件变量（信号量）等待/通知：
```

> ​		多线程可以用于实现一些比较耗时的操作，如图像处理、网络通信等等。
>
> ​		常见的做法是把这些操作放到一个新的线程中执行，以免阻塞主线程。
>
> ​		同时，需要注意线程间的同步，避免出现数据竞争等问题。
>
> ​		在SDL中，可以通过互斥锁和条件变量（信号量）来实现线程间的同步。

### Video

​		窗口、渲染器和纹理是三个重要的概念:

- “窗口”，即我们所看到的窗口界面。

- “渲染器”，用于把纹理渲染到窗口上。

- “纹理”，即需要渲染的图像数据。

  > 纹理与RGB数据的区别：
  >
  > ​		比如一个从左到右由红色渐变到蓝色的矩形，用存储RGB的话就需要把矩形中每个点的具体颜色值存储下来；
  >
  > ​		而纹理只是一些描述信息，比如记录了矩形的大小、起始颜色、终止颜色等信息，显卡可以通过这些信息推算出矩形块的详细信息。
  >
  > ​		所以相对于存储RGB而已，存储纹理占用的内存要少的多。

​		SDL还提供了一个简单的矩形结构 SDL_Rect，用于描述矩形的位置和大小。



```c
SDL_Init()：初始化SDL系统
SDL_Delay()：工具函数，用于延时
SDL_Quit()：退出SDL系统

SDL_Rect 一个简单的矩形结构

SDL_Window 代表了一个“窗口”
SDL_CreateWindow()：创建窗口SDL_Window

SDL_Renderer 代表了一个“渲染器”
SDL_CreateRenderer()：基于窗口创建渲染器SDL_Renderer
SDL_RenderCopy()：将纹理的数据拷贝给渲染器
SDL_RenderPresent()：显示

SDL_Texture 代表了一个“纹理”
SDL_CreateTexture()：基于渲染器创建纹理SDL_Texture
SDL_UpdateTexture()：设置纹理的数据
```

> ​		在窗口显示时，需要先创建窗口和渲染器，然后创建纹理并将其渲染到渲染器上，最后通过调用 SDL_RenderPresent() 显示出来。同时，可以使用 SDL_Delay() 函数来延时，让窗口停留一段时间后再退出程序。

####  基本使用

1. 初始化SDL
2. 创建窗口
3. 创建渲染器
4. 清空缓冲区
5. 绘制要显示的内容
6. 最终将缓冲区内容渲染到window窗口上
7. 销毁渲染器
8. 销毁窗口
9. 退出SDL

![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/f37ec72e0b774555a3950bb2a17abb37.png)





### Audio

```c
typedef struct SDL_AudioSpec {
    int freq; // 音频采样率
    SDL_AudioFormat format; // 音频数据格式
    Uint8 channels; // 声道数: 1 单声道, 2 立体声
    Uint8 silence; // 设置静音的值，因为声音采样是有符号的，所以0当然就是这个值
    Uint16 samples; // 音频缓冲区中的采样个数，要求必须是2的n次
    Uint16 padding; // 考虑到兼容性的一个参数
    Uint32 size; // 音频缓冲区的大小，以字节为单位
    SDL_AudioCallback callback; // 填充音频缓冲区的回调函数
    void *userdata; // 用户自定义的数据
} SDL_AudioSpec;

// userdata：SDL_AudioSpec结构中的用户自定义数据，一般情况下可以不用。
// stream：该指针指向需要填充的音频缓冲区。
// len：音频缓冲区的大小（以字节为单位）1024*2*2。
void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 *stream, int len);

// desired：期望的参数。
// obtained：实际音频设备的参数，一般情况下设置为NULL即可。
int SDLCALL SDL_OpenAudio(SDL_AudioSpec * desired, SDL_AudioSpec * obtained); 

// 当pause_on设置为0的时候即可开始播放音频数据。设置为1的时候，将会播放静音的值。
void SDLCALL SDL_PauseAudio(int pause_on);
```



## 性能与优化

​		SDL 的性能通常足以满足大部分 2D 游戏和应用程序的需求，但在某些情况下可能需要对其进行优化。以下是一些 SDL 性能特点和优化策略：

- 使用硬件加速：如果可能的话，尽量使用 SDL 的硬件加速功能。例如，当创建 SDL_Renderer 时，使用 SDL_RENDERER_ACCELERATED 标志。这将尽可能地利用图形处理器 (GPU) 来提高图形性能。
- 纹理格式与压缩：使用与显卡本地支持的像素格式，可以减少纹理转换的开销。此外，使用压缩纹理格式（如 DXT1，DXT3，DXT5 或 ASTC）可减少显存占用和纹理传输的开销。
- 避免频繁地锁定纹理：锁定纹理可能导致显著的性能损失，因为它会中断 GPU 和 CPU 之间的并行执行。尽量避免频繁地锁定和修改纹理，而是在初始化时一次性创建和设置纹理。
- 批处理绘制操作：将具有相同纹理和属性的绘制操作批量进行，可以减少状态切换的次数，从而提高性能。可以使用一些开源库如 SDL_gpu 来实现自动批处理。
- 合理地控制帧率：通过限制游戏帧率，可以降低 CPU 和 GPU 的负载。一般情况下，保持在 30fps 或 60fps 是合理的。可以使用 SDL_Delay 函数来实现帧率控制。
- 优化事件处理：减少事件处理的次数和复杂度。对于非关键事件（如鼠标移动），可以通过合并连续的事件来减少处理次数。
- 减少内存分配次数：避免在游戏循环中频繁地分配和释放内存。尽量使用预先分配的内存池和对象池来重复使用对象。
- 音频优化：对于音频处理，使用合适的音频格式和采样率。避免在游戏循环中频繁加载和解码音频文件。可以使用音频流来减少内存占用。
- 多线程与异步操作：利用多线程可以将一些耗时操作（如资源加载、网络通信等）放到后台线程中处理，从而不影响主线程的性能。



## 跨平台

​		在使用 SDL 进行跨平台开发时，特别是在 ARM 平台上，有一些注意事项可以帮助您确保项目的兼容性和性能。以下是一些建议：

- 端序问题：在跨平台开发时，请注意端序（字节序）问题。不同的处理器可能使用大端序或小端序。在处理多字节数据类型（例如整数、浮点数等）时，确保正确处理端序问题。使用 SDL 提供的字节序转换函数，如 SDL_Swap16()、SDL_Swap32() 和 SDL_Swap64()。

- 数据类型和字节大小：请确保使用具有一致字节大小的数据类型。例如，使用 int32_t 和 uint32_t 代替 int 和 unsigned int，以确保跨平台一致性。在需要时，可以使用 SDL 提供的数据类型，如 Sint16、Uint32 等。

- 编译器和选项：使用跨平台编译器，如 GCC 和 Clang，以确保源代码可以在各个平台上正确编译。使用适当的编译器选项来优化 ARM 平台的性能，例如：-march=armv7-a -mfpu=neon -mfloat-abi=softfp（针对 ARMv7 平台）。

- 硬件加速和 OpenGL ES 支持：请确保使用支持 ARM 平台的硬件加速功能。尽量使用 SDL_Renderer 的硬件加速渲染，并考虑在需要时使用 OpenGL ES。注意，SDL 2 默认支持 OpenGL ES 2.0。

- 触摸输入和屏幕旋转：在移动设备（如 Android 和 iOS）上，需要处理触摸输入和屏幕旋转。请确保使用 SDL 提供的触摸输入 API（如 SDL_GetNumTouchDevices() 和 SDL_GetTouchDevice()）来处理触摸输入，并在需要时处理屏幕旋转事件。

- 屏幕分辨率和 DPI：不同的设备可能具有不同的屏幕分辨率和 DPI。在布局和绘制界面元素时，确保考虑不同分辨率和 DPI 的设备。使用相对布局和可伸缩的用户界面元素，以适应不同屏幕尺寸。

- 资源管理：ARM 设备通常具有较低的内存和存储空间。请确保合理地管理资源，避免浪费内存。使用压缩的纹理格式，按需加载资源，及时释放不再使用的资源。

- 性能优化：请注意优化 ARM 平台的性能，尤其是在处理器性能较弱的设备上。请参阅上一个回答中关于 SDL 性能特点与优化策略的建议。

  



## QT vs SDL

​		Qt和SDL（Simple DirectMedia Layer）都是广泛使用的跨平台库，它们在一些功能上存在重叠。但它们的侧重点和使用场景有所不同。

​		Qt：Qt是一个功能丰富的**应用程序框架**，特别擅长于**创建具有复杂GUI的跨平台应用程序**。它提供了许多高级功能，如网络、数据库访问、XML处理等。Qt适**用于创建桌面应用程序、嵌入式系统和移动应用程序**。
​		SDL：SDL是一个**轻量级的多媒体库**，主要用于**开发游戏和多媒体应用程序**。它提供了一个低级别的、接近硬件的接口，用于图形、音频、输入和定时器。SDL的优势在于**对游戏和实时图形的支持，以及较低的性能开销**。



- 窗口管理和输入处理
  - Qt和SDL都提供了创建和管理窗口的功能。
  - 它们都可以处理键盘、鼠标和其他输入设备的事件。
- 图形渲染：Qt和SDL都支持2D图形渲染。
  - Qt拥有强大的图形库（包括QPainter和QGraphicsScene），可以处理各种2D绘图任务。
  - 而SDL则提供了基于SDL_Surface的简单图形渲染能力。此外，它们都支持使用OpenGL进行3D图形渲染。
- 图像处理：Qt和SDL都提供了处理和操作图像的功能。
  - Qt的`QImage`类提供了丰富的图像处理功能
  - SDL则通过`SDL_Surface`和`SDL_Image`库（一个额外的扩展库）提供图像处理功能。
- 音频播放：Qt和SDL都可以用于播放音频文件。
  - Qt提供了`QMediaPlayer`和`QAudioOutput`等类来处理音频播放。
  - SDL则通过`SDL_mixer`库（一个额外的扩展库）提供音频播放功能。
- 定时器：Qt和SDL都提供了定时器功能，用于在特定的时间间隔执行某些操作。
  - Qt提供了`QTimer`类
  - SDL提供了`SDL_AddTimer`和`SDL_RemoveTimer`函数。
- 多线程：Qt和SDL都支持多线程编程。
  - Qt提供了`QThread`类和相关的同步原语（如`QMutex、QSemaphore`等）。
  - SDL则提供了`SDL_Thread`、`SDL_CreateThread`、`SDL_WaitThread`等函数和同步原语（如`SDL_mutex`、`SDL_sem`等）。