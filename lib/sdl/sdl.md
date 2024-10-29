# SDL

---

​		SDL（Simple DirectMedia Layer）是一个跨平台开发库，旨在通过 `OpenGL `和 `Direct3D `提供对音频、键盘、鼠标、游戏杆和图形硬件的低级访问。它被视频播放软件、模拟器和流行游戏使用。

​		`SDL`习惯性的视为跨平台的`DirectX`，但`SDL`的定位是轻量级的多媒体框架库，用于简化低层次跨平台对于图像、声音、输出、UI控制、线程管理等接口。但是

- 更高级的图形和音效控制需要配合专业的图形、音频库（`openGL/Vulkan/Metal`和`openAL`）等；
- 更高级的UI控制需要配合丰富的UI组件库（`Qt/DuiLib`等）；
- 更高级的线程管理及其他基础设施需要配合更强大的工具库（`Boost/Qt/c++ std/folly/Abseil`)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/4f93a75a51504bcda91af189dac059fc.png)

## 参考

[Lazy Foo' Productions - Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/index.php)

[SDL入门_慕课手记 (imooc.com)](https://www.imooc.com/article/25190)

https://blog.csdn.net/u011734326/article/details/95337484

[**「SDL第二篇」窗口渲染**](https://www.imooc.com/article/25202#comment)

[**「SDL第五篇」彻底理解纹理（Texture）**](https://www.imooc.com/article/25379)

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



### 库管理

```c
SDL_Init()：初始化SDL系统
SDL_Quit()：退出SDL系统
```



#### `SDL_Init`

​	SDL 库的初始化函数，作用是初始化 SDL 库并加载所需要的子系统。这个函数必须在使用 SDL 库的其他函数之前被调用。

```c
int SDL_Init(Uint32 flags)
```

- `flags `参数是一个位掩码，用于指定需要初始化的子系统，常用的值包括：
  - `SDL_INIT_VIDEO`：表示初始化视频子系统，用于创建和管理窗口、渲染器等。
  - `SDL_INIT_AUDIO`：表示初始化音频子系统，用于播放和录制音频。
  - `SDL_INIT_JOYSTICK`：表示初始化手柄子系统，用于管理游戏手柄和游戏垫。
  - `SDL_INIT_GAMECONTROLLER`：表示初始化游戏控制器子系统，用于管理游戏控制器。
  - `SDL_INIT_EVENTS`：表示初始化事件子系统，用于处理 SDL 应用程序的事件循环和消息队列。
  - `SDL_INIT_TIMER`、`SDL_INIT_HAPTIC`、`SDL_INIT_SENSOR `
  - 同时初始化多个子系统可以通过位或来组合上述值
- 返回值表示成功与否，非0失败可以使用`SDL_GetError()`获取相关错误信息



#### `SDL_Quit`

​		SDL 库退出函数，作用是释放 SDL 库占用的资源并退出 SDL 应用程序。

​		调用该函数之后，SDL 应用程序会正常退出。

​		需要注意的是，调用 `SDL_Quit `函数后，所有 `SDL `窗口、渲染器、音频设备等都会被销毁，应该确保在调用 `SDL_Quit `函数前，所有这些对象的资源都被正常释放。

```c
void SDL_Quit(void)
```



#### `SDL_Delay`

​		用于让程序暂停一段时间。

```c
void SDL_Delay(Uint32 ms);
```

- `ms`参数表示需要让程序暂停的时间，以毫秒为单位。



#### `SDL_memset`

​		用于将一块内存中的每个字节都设置为指定的值。

```c
void *SDL_memset(void *dst, int value, size_t len);
```

- 参数`dst`是指向需要设置的内存块的指针
- 参数`value`表示用来设置内存块中每个字节的值，通常为0或者255
- 参数`len`表示内存块的大小，以字节为单位



### Events

​		事件是指与用户输入、系统操作等相关的行为。事件包括键盘事件、鼠标事件、窗口事件、定时器事件等等。

​		SDL_Event 结构包含有关事件类型、事件发生时间和事件参数的详细信息。

```c
SDL_Event  // 代表一个事件
```

- type：表示事件类型的整数。
  - SDL_QUIT：表示退出事件，当用户关闭窗口或按下键盘上的 Alt + F4 或 Cmd + Q 组合键时会触发该事件。
  - SDL_KEYDOWN / SDL_KEYUP：表示键盘按键事件，当用户按下或释放键盘上的按键时会触发该事件。
  - SDL_MOUSEMOTION / SDL_MOUSEBUTTONDOWN / SDL_MOUSEBUTTONUP：表示鼠标事件，当用户移动鼠标或按下、释放鼠标按钮时会触发该事件。
  - SDL_WINDOWEVENT：表示窗口事件，当窗口状态发生变化时会触发该事件，例如窗口大小发生变化，窗口获得或失去焦点等。
  - SDL_JOYAXISMOTION / SDL_JOYBALLMOTION / SDL_JOYHATMOTION / SDL_JOYBUTTONDOWN / SDL_JOYBUTTONUP / SDL_JOYDEVICEADDED / SDL_JOYDEVICEREMOVED：表示游戏手柄事件，当游戏手柄状态发生变化时会触发该事件，例如手柄移动、按下、释放按钮等。
  - SDL_CONTROLLERAXISMOTION / SDL_CONTROLLERBUTTONDOWN / SDL_CONTROLLERBUTTONUP / SDL_CONTROLLERDEVICEADDED / SDL_CONTROLLERDEVICEREMOVED：表示控制器事件，当控制器状态发生变化时会触发该事件，例如控制器移动、按下、释放按钮等。
  - SDL_USEREVENT：表示用户自定义事件，当应用程序调用 SDL_PushEvent 函数将自定义事件添加到事件队列时，会触发该事件。
- timestamp：表示事件发生时间的 Uint32 类型数值。
- window：表示事件所属的 SDL_Window 数据指针。
- key：表示键盘事件的 SDL_KeyboardEvent 数据结构。
- motion：表示鼠标移动事件的 SDL_MouseMotionEvent 数据结构。
- button：表示鼠标按钮事件的 SDL_MouseButtonEvent 数据结构。
- wheel：表示鼠标滚轮事件的 SDL_MouseWheelEvent 数据结构。
- jaxis：表示游戏手柄轴移动事件的 SDL_JoyAxisEvent 数据结构。
- jball：表示游戏手柄球移动事件的 SDL_JoyBallEvent 数据结构。
- jbutton：表示游戏手柄按钮事件的 SDL_JoyButtonEvent 数据结构。
- jdevice：表示游戏手柄设备事件的 SDL_JoyDeviceEvent 数据结构。
- caxis：表示游戏手柄轴移动事件的 SDL_ControllerAxisEvent 数据结构。
- cbutton：表示游戏手柄按钮事件的 SDL_ControllerButtonEvent 数据结构。
- cdevice：表示游戏手柄设备事件的 SDL_ControllerDeviceEvent 数据结构。
- quit：表示退出事件的 SDL_QuitEvent 数据结构。
- user：表示用户自定义事件的 SDL_UserEvent 数据结构。

```c

    
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



#### `SDL_PollEvent`

​		用于从事件队列中轮询(一直循环)事件并返回。它可以用于捕获用户输入，控制游戏状态等。

​		检查SDL事件队列中是否有事件，并将此事件从队列顶部弹出并返回。如果事件队列为空，则该函数将立即返回0。如果有事件，则该函数将返回1，并将事件结构体填充到指定的SDL_Event结构体中。



```c
SDL_Event event;

while (SDL_PollEvent(&event)) {
    switch (event.type) {
        case SDL_MOUSEMOTION:
            // 处理鼠标移动事件
            break;
        case SDL_MOUSEBUTTONDOWN:
            // 处理鼠标按下事件
            break;
        case SDL_MOUSEBUTTONUP:
            // 处理鼠标释放事件
            break;
        default:
            break;
    }
}
```



#### `SDL_WaitEvent`

​		**用于等待并获取下一个事件**。它可用于阻塞程序执行，直到有事件发生。

​		**检查事件队列中是否有事件。如果事件队列为空，则该函数会阻塞当前线程并等待事件的到来**。如果队列中有事件，则该函数将该事件弹出队列并返回一个非零值。

​		与SDL_PollEvent()不同，不需要指定要轮询的事件类型。

```c
SDL_Event event;

while (SDL_WaitEvent(&event)) {
    if (event.type == SDL_QUIT) {
        // 处理退出事件
        break;
    }
}
```

​		SDL_WaitEvent()函数会阻塞当前线程，因此如果等待事件的时间过长，将会导致程序停滞，并不利于程序的执行。

- 建议在需要响应用户输入的情况下使用SDL_WaitEvent()
- 在不需要响应事件的情况下建议使用SDL_PollEvent()

#### `SDL_AddTimer`

​		SDL中的**定时器函数，用于注册回调函数并创建一个定时器**。

```c
SDL_TimerID SDL_AddTimer(Uint32 interval, SDL_TimerCallback callback, void* param);
```

- `interval`：定时器触发时间间隔（毫秒）
- `callback`：回调函数指针，用于处理定时器触发时的事件
- `param`：回调函数的参数，可以为NULL
- 成功：返回定时器ID，失败：返回0



### Thread

​		SDL是跨平台的，因此它提供的跨平台的多线程封装接口。

​		

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

#### `SDL_Thread`

SDL_Thread结构体的指针，这个结构体包含了线程的ID和名称等信息。



#### `SDL_mutex`

​		互斥锁，是SDL库**提供的一种线程同步机制**，用于多个线程之间访问共享资源时的互斥和同步。

```c
typedef struct SDL_mutex {
    void *id;           // 同步机制的底层实现句柄，不同平台实现不同
} SDL_mutex;
```



#### `SDL_cond`

​		条件变量（condition variable），它是一种线程间同步原语，用于实现线程的协调和交互。条件变量允许一个或多个线程等待某个条件满足后再继续执行

#### `SDL_CreateThread`

​		**用于创建一个新线程**

```c
SDL_Thread* SDL_CreateThread(SDL_ThreadFunction fn, const char* name, void* data);
```

- `fn`是一个线程执行的函数

  - ```c
    typedef int (*SDL_ThreadFunction)(void* data);
    ```

- `name`是线程名称；

- `data`是传递给线程函数的数据参数。



#### `SDL_WaitThread`

​		**等待指定的线程执行完毕**。

​		`SDL_WaitThread`函数会挂起当前线程，等待指定的线程执行完毕，并接收指定线程的返回状态。如果线程已经退出，函数会立刻返回，并将线程的退出状态存入`status`指针所指的内存中。

```c
int SDL_WaitThread(SDL_Thread* thread, int* status);
```

- `thread`：要等待的线程。
- `status`：整型指针，用于存储线程的退出状态。
- 函数的返回值为0表示线程已正常退出，其他值表示错误。



#### `SDL_CreateMutex / SDL_DestroyMutex`

​		**创建互斥锁（mutex）的函数**。互斥锁是一种用于保护共享资源的同步原语，它可以确保同一时间内只有一个线程访问共享资源，以避免数据竞争和并发问题。

```c
SDL_mutex* SDL_CreateMutex(void)
```

- 返回一个指向新创建互斥锁的指针（SDL_mutex*类型），如果创建失败则返回 NULL。

​		**销毁互斥锁的函数**。

```c
void SDL_DestroyMutex(SDL_mutex* mutex);
```





#### `SDL_LockMutex / SDL_UnlockMutex`

​		SDL_LockMutex  **锁定互斥锁（mutex）的函数**.

​		如果互斥锁当前没有被其他线程锁定，则该线程会立即获得互斥锁并可以访问受保护的共享资源。如果互斥锁当前已经被其他线程锁定，那么该线程会一直阻塞等待直到互斥锁被释放并可以被该线程获得为止。

​		**在使用互斥锁时，要遵循“谁申请谁释放”的原则**，即使用 SDL_LockMutex() 函数锁定互斥锁的线程必须在访问完共享资源后使用 SDL_UnlockMutex() 函数释放互斥锁，否则其他线程将无法获得该锁访问共享资源。

```c
int SDL_LockMutex(SDL_mutex* mutex)
```

- **若函数成功锁定互斥锁，则返回0**；否则返回错误代码。

​		SDL_UnlockMutex**释放互斥锁（mutex）的函数**

```c
int SDL_UnlockMutex(SDL_mutex* mutex);
```



```c
#include <SDL.h>

// 定义一个全局变量作为共享的资源
int shared_data = 0;

SDL_mutex* mutex;

void some_function(void) {
    // 等待并锁定互斥锁
    SDL_LockMutex(mutex);

    // 访问受保护的共享资源
    shared_data += 1;

    // 解锁互斥锁
    SDL_UnlockMutex(mutex);
}

int main() {
    // 初始化SDL库
    SDL_Init(SDL_INIT_EVERYTHING);

    // 创建互斥锁
    mutex = SDL_CreateMutex();

    // 启动多个线程访问共享资源
    // ...

    // 最后释放互斥锁
    SDL_DestroyMutex(mutex);

    // 退出SDL库
    SDL_Quit();
    return 0;
}

```



#### `SDL_CreateCond / SDL_DestroyCond`

​		条件变量是一种线程间同步原语，它允许一个线程等待另一个线程满足某个条件后再继续执行，从而更好地控制线程的执行顺序和并发性。

​		**创建条件变量（condition variable）的函数**。

```c
SDL_cond* SDL_CreateCond(void)
```

- 返回一个指向新创建条件变量的指针（SDL_cond*类型），如果创建失败则返回 NULL。

​		**销毁条件变量（condition variable）的函数**

```c
void SDL_DestroyCond(SDL_cond* cond)
```



#### `SDL_CondSignal`

​		SDL库中用于发送条件变量（condition variable）信号的函数。

```c
int SDL_CondSignal(SDL_cond* cond)
```



#### `SDL_CondWait`

​		**等待条件变量（condition variable）信号的函数**.

​		**当一个线程调用 SDL_CondWait() 函数时，它会先解锁互斥锁（把自己的锁解开，所以使用SDL_CondWait函数之前，必须先获取到锁），然后阻塞，一直等待条件变量的信号（SDL_CondSignal或SDL_CondBroadcast），直到信号被发送给该条件变量。当线程接收到信号后，被唤醒，这时候它会重新获取互斥锁（重新获取就是尝试获取，如果其他线程不释放该锁，即使收到信号也不会向下执行。），以访问共享资源。**

```c
int SDL_CondWait(SDL_cond* cond, SDL_mutex* mutex)
```



#### 示例

```c
#include <SDL.h>
#include <stdio.h>

SDL_mutex *mutex = NULL;
SDL_cond *cond = NULL;

int thread_work(void *arg) {

    printf("Thread-2 Lock Mutex Start\n");
    SDL_LockMutex(mutex);
    printf("Thread-2 Lock Mutex Success\n");

    printf("Thread-2 Working with Mutex\n");
    sleep(4);

    printf("Thread-2 Wait Condition Start\n");
    SDL_CondWait(cond, mutex);
    printf("Thread-2 Wait Condition Success\n");

    printf("Thread-2 Unlock Mutex\n");
    SDL_UnlockMutex(mutex);

    printf("Thread-2 Finish\n");

    return 0;
}

#undef main
int main() {

    mutex = SDL_CreateMutex();
    cond = SDL_CreateCond();

    SDL_Thread *t = SDL_CreateThread(thread_work, "Thread-2", NULL);

    printf("Thread-1 Working\n");
    sleep(2);

    printf("Thread-1 Lock Mutex Start\n");
    SDL_LockMutex(mutex);
    printf("Thread-1 Lock Mutex Success\n");

    printf("Thread-1 Working with Mutex\n");
    sleep(2);

    printf("Thread-1 Send Condition Signal\n");
    SDL_CondSignal(cond);

    printf("Thread-1 Working with Mutex\n");
    sleep(2);

    printf("Thread-1 Unlock Mutex\n");
    SDL_UnlockMutex(mutex);

    printf("Thread-1 Wait Thread-2 Finish Start\n");
    SDL_WaitThread(t, NULL);
    printf("Thread-1 Wait Thread-2 Finish Success\n");

    printf("Thread-1 Destroy Mutex Start\n");
    printf("Thread-1 Destroy Condition Start\n");
    SDL_DestroyMutex(mutex);
    SDL_DestroyCond(cond);
    printf("Thread-1 Destroy Mutex Success\n");
    printf("Thread-1 Destroy Condition Success\n");

    return 0;
}
```



### Video

​		窗口、渲染器和纹理是三个重要的概念:

- “窗口”，即我们所看到的窗口界面。

  - 窗口是一个`SDL_Window`类型的结构体，包含了模块化各种窗口所需的数据和属性。

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
SDL_Delay()：工具函数，用于延时
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



#### `SDL_Rect`

​		**定义矩形区域的结构体**

```c
typedef struct SDL_Rect
{
    int x, y;
    int w, h;
} SDL_Rect;
```

#### `SDL_Surface`

​		SDL_Render对象中有一个**视频缓冲区，该缓冲区我们称之为SDL_Surface，它是按照像素存放图像的**。我们一般把真彩色的像素称为RGB24数据。也就是说，每一个像素由24位组成，每8位代表一种颜色，像素的最终颜色是由RGB三种颜色混合而成的。



#### `SDL_Texture`

​		**存储像素数据的结构体类型**，**它表示一个纹理对象**（texture）。纹理对象是一个在 GPU 内存中的预渲染的图像，可以用于快速渲染。

​		我们通常将一个位图（bitmap）或者一张图片加载到内存中，并将其创建为一个纹理对象。然后，我们可以使用 `SDL_RenderCopy` 函数将该纹理对象绘制到渲染目标上，并在需要时进行透明度混合（alpha blending）、裁剪（clipping）等处理。

​		`SDL_Texture` 中包含了很多纹理对象所需的信息，包括：

- `w` 和 `h`：表示纹理的宽度和高度，即像素数目。
- `access`：表示纹理的访问权限，在使用该纹理对象时，需要先调用 `SDL_QueryTexture` 函数来获取该访问权限。比较常用的权限有 `SDL_TEXTUREACCESS_STATIC`（静态访问，只能读取）和 `SDL_TEXTUREACCESS_STREAMING`（流式访问，可以读写）。
- `format`：表示纹理的像素格式，比如 RGBA、BGRA、RGB 等。
- `id`：用于标识该纹理对象的一个唯一的非零整数 ID。
- `renderer`：指向该纹理对象对应的渲染器（`SDL_Renderer` 类型）的指针。

​		

#### `SDL_Window`



#### `SDL_Render`

​		SDL_Render 是渲染器，它也是主存中的一个对象。对Render操作时实际上分为两个阶段：

1. 渲染阶段。在该阶段，用户可以画各种图形渲染到SDL_Surface或SDL_Texture 中;
2. 显示阶段。参SDL_Texture为数据，通过OpenGL操作GPU，最终将 SDL_Surfce 或SDL_Texture中的数据输出到显示器上。





#### `SDL_CreateWindow`

​			创建窗口的函数，作用是创建一个 SDL 窗口对象，用于显示图形界面。

```c
SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
```

- `title`：窗口的标题，以零结尾的字符串。
- `x`：窗口左上角在屏幕上的水平坐标，如果值为 SDL_WINDOWPOS_CENTERED，则窗口将在屏幕中央显示。
- `y`：窗口左上角在屏幕上的竖直坐标，如果值为 SDL_WINDOWPOS_CENTERED，则窗口将在屏幕中央显示。
  - x和y设置为`SDL_WINDOWPOS_UNDEFINED`表示让操作系统自己决定窗口的位置
- `w`：窗口的宽度（单位：像素）。
- `h`：窗口的高度（单位：像素）。
- `flags`：窗口的属性，是 `SDL_WindowFlags `枚举类型中的位掩码。常用的属性包括：
  - `SDL_WINDOW_SHOWN`：创建窗口时立即显示。
  - `SDL_WINDOW_RESIZABLE`：窗口可以调整大小。
  - `SDL_WINDOW_FULLSCREEN`：窗口全屏显示（覆盖任务栏等）。
  - `SDL_WINDOW_BORDERLESS`：窗口无边框，不能调整大小。
  - `SDL_WINDOW_ALLOW_HIGHDPI`：支持高分辨率显示（用于 Retina 屏幕等）。
  - `SDL_WINDOW_OPENGL`： 表示使用OpenGL渲染窗口。
- 返回值是一个 `SDL_Window `结构体指针，表示创建的窗口对象。
  - 如果创建失败，则返回 `NULL`，并通过调用 `SDL_GetError() `函数获取错误信息。

SDL_CreateWindow 函数只能用于创建基本的窗口对象，如果需要使用 OpenGL 或 Vulkan 渲染图形，需要使用 SDL_CreateWindowAndRenderer 或 SDL_CreateWindowFrom 函数。



#### `SDL_DestroyWindow`

​		销毁窗口的函数。

```c
void SDL_DestroyWindow(SDL_Window* window)
```

​		使用该函数后，程序将该窗口及其所有资源从内存中删除，将不再能够使用对该窗口的引用。

#### `SDL_CreateRenderer`

​		创建 SDL 渲染器（渲染上下文）。

```c
SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags)
```

- `window`：SDL 窗口指针，用于关联创建的渲染器对象和窗口。

- `index`：渲染器的索引值，决定了渲染器的具体实现，可以传入 -1 使用默认的渲染器。

- `flags`：渲染器的配置选项，可以设置渲染模式、硬件加速、缓冲模式等。 

  - 传入0表示表示创建一个仅支持软件渲染的渲染器对象，不使用硬件加速。

  - ```c
    SDL_RENDERER_SOFTWARE        // 兼容性模式，使用软件渲染
    SDL_RENDERER_ACCELERATED     // 硬件加速模式，充分利用GPU的硬件加速功能
    SDL_RENDERER_PRESENTVSYNC    // 垂直同步模式，来防止画面撕裂
    SDL_RENDERER_TARGETTEXTURE   // 可以将渲染的内容保存到纹理中
        
    int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    ```

  - 如果您不是大量使用纹理，并且不需要实时的VSync，那么使用默认的软件渲染器也是可以的。



#### `SDL_DestroyRenderer`

​		销毁渲染器的函数。

```c
void SDL_DestroyRenderer(SDL_Renderer* renderer)
```



#### `SDL_SetRenderDrawColor`

​		设置 SDL2 渲染器绘制颜色的函数，设置渲染器的绘制颜色，即下一次绘制将使用的颜色。

​		设置绘制颜色只是确定了使用哪种颜色进行绘制，但并不实际进行绘制。要在渲染目标上使用设置好的颜色进行绘制，必须使用绘制函数（例如 `SDL_RenderDrawLine`、`SDL_RenderDrawRect `等）进行调用。

```c
int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
```

- `renderer`：渲染器指针。
- `r`：红色通道，范围为 0-255。
- `g`：绿色通道，范围为 0-255。
- `b`：蓝色通道，范围为 0-255。
- `a`：Alpha 通道，范围为 0-255，0 表示完全透明，255 表示完全不透明。

```c
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 将绘制颜色设置为红色
SDL_RenderDrawRect(renderer, &rect); // 使用红色绘制矩形
```



#### `SDL_RenderClear`

​		用设置好的颜色（默认黑色）来 **“清空”**整个渲染目标。

```c
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//设置绘制颜色，白色
SDL_RenderClear(renderer);// 清空渲染目标,不指定绘制颜色的话，默认为黑色，由于上述设置，所以填充为白色
```





#### `SDL_RenderDrawRect`

​		可以在渲染目标上绘制一个矩形。

​		渲染器会在渲染目标上绘制一个由参数`rect`指定的矩形，矩形的边框为当前设置的绘制颜色，填充区域为空。

```c
int SDL_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect);
```

- `renderer`是渲染器指针
- `rect`是一个SDL_Rect结构体指针，表示绘制的矩形。
  - SDL_Rect结构体包含了四个整型变量，分别表示矩形的左上角坐标和宽度、高度。



#### `SDL_RenderFillRect`

​		在**渲染目标上绘制一个填充矩形**。

​		绘制的矩形会被填充，而不是仅仅绘制边框。

```c
int SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect);
```

- `renderer`是渲染器指针
- `rect`是一个SDL_Rect结构体指针，表示绘制的矩形。



#### `SdL_RenderCopy`

​		**用于在指定目标上渲染纹理的函数**。

```c
int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
```

- `renderer`：渲染器，用于指定渲染目标。
- `texture`：纹理对象，即要被渲染的纹理。
- `srcrect`：源矩形，用于指定从纹理中渲染哪部分。NULL表示全部
- `dstrect`：目标矩形，用于指定在渲染目标中的位置和大小。NULL表示全部

​		四个参数都必须指定，如果传递了NULL，则表示使用纹理的默认值。

​		在实际使用的过程中，**可以采用指定源矩形和目标矩形中的一些小技巧，例如将源矩形和目标矩形的数据类型转换为浮点型，以方便进行缩放和移动操作，还可以利用SDL_Rect结构体的裁切特性，仅渲染纹理的局部部分**。

​		只要设置了渲染目标，**那么在渲染结束前，一定需要使用`SDL_SetRenderTarget`将渲染目标还原回默认窗口，否则后续渲染的图像就会出现问题。**



#### `SDL_RenderPresent`

​		**更新渲染目标的显示。**将当前的渲染目标（通常是屏幕）上的绘制结果刷新到显示器上，即更新渲染器，使其显示最新的绘制结果。

​		每一次渲染操作后需要调用 SDL_RenderPresent 函数来在屏幕上显示所绘制的内容。如果没有调用此函数，程序将看不到任何绘制的内容。

```c
void SDL_RenderPresent(SDL_Renderer* renderer);
```

- `renderer`：渲染器指针。



#### `SDL_SetRenderTarget`

​		用于设置渲染目标（给谁渲染）的函数，在SDL中，渲染目标可能是默认的窗口，也可能是已创建的纹理。

​		使用`SDL_SetRenderTarget`函数可以将纹理设置为渲染目标，然后可以将其他纹理或图像渲染到此纹理上。同时，纹理可以具有透明度，方便透明效果的实现。

```c
int SDL_SetRenderTarget(SDL_Renderer*   renderer,
                        SDL_Texture*    texture);
```

- `renderer`：`SDL_Renderer` 指针，设置渲染目标使用的渲染器；
- `texture`：`SDL_Texture` 指针，用来作为渲染目标的纹理。
  - 如果将`texture`设为NULL，则会将渲染目标还原回默认窗口。
- 返回值：函数执行成功则返回 0，否则返回 -1。



#### `SDL_CreateTexture`

​		**创建 `SDL_Texture`（纹理对象） 的函数**，从一个已有的 `SDL_Renderer` 以及一个指定的格式、渲染宽高创建。

```c
SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h); 
```

- `renderer`：`SDL_Renderer` 指针，创建 `SDL_Texture` 时使用的渲染器；

- `format`：`Uint32`，用来指定纹理像素格式（即每个像素需要多少个字节来表示）。

  - SDL中提供了多种像素格式，包括 RGB、RGBA、ARGB、BGR、BGRA、ABGR等。

  - RGBA格式最为常见，它使用4个字节表示每个像素，分别代表红色、绿色、蓝色和 alpha（透明度）。

  - ```c
    Uint32 textureFormat = SDL_PIXELFORMAT_RGBA8888;
    SDL_Texture* texture = SDL_CreateTexture(renderer, textureFormat, SDL_TEXTUREACCESS_STREAMING, 640, 480);
    ```

- `access`：`int`，指定创建的纹理的访问方式，指定纹理的访问方式，有以下几种选项：

  - `SDL_TEXTUREACCESS_STATIC`：纹理被固定分配在显存中，不可修改。
    - 当纹理是静态的，即不需要频繁更新时
    - 可以保证纹理的渲染速度。
  - `SDL_TEXTUREACCESS_STREAMING`：纹理被用于频繁更新。显存与CPU共享
    - 纹理需要频繁更新时，支持lock/unlock操作来更新纹理内容
    - 意味着可能需要重复调用SDL_UpdateTexture()函数来更新图像。常见的用例是流式媒体或视频。
  - `SDL_TEXTUREACCESS_TARGET`：纹理可以当做渲染目标，贴到其他纹理或窗口的面上使用。
    - 纹理需要被用于渲染目标时使用，支持使用纹理作为渲染目标，并将纹理与窗口配合使用。

- `w`：`int`，创建纹理的宽度；

- `h`：`int`，创建纹理的高度。

- 返回值：返回创建失败时为 NULL，否则为一个 `SDL_Texture` 指针。



#### `SDL_DestroyTexture`

​		**销毁纹理的函数**，释放创建纹理时所使用的资源，包括纹理对象本身以及对应的显存资源。

​		如果创建了大量纹理，应该在不需要使用这些纹理时及时销毁，以提高系统的运行效率。

```c
void SDL_DestroyTexture(SDL_Texture* texture)
```



#### `SDL_UpdateTexture`

​		**用于更新纹理的函数**。它用于将位图内容复制到创建的纹理中。

​		每次更新纹理时都会替换整个纹理。要部分更新，请使用`SDL_UpdateTextureRect()`。

```c
int SDL_UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect, const void *pixels, int pitch)
```

- **texture**: 指向要更新的纹理的指针。
- **rect**: 更新的纹理的矩形区域。如果为NULL，则默认为整个纹理。
- **pixels**: 要复制到纹理中的像素数据的指针。
- **pitch**: 像素数据的长度（字节数）。

- 返回值：如果成功，则返回0，否则返回-1，并设置SDL_GetError()的错误信息。



#### `SDL_UpdateYUVTexture`

​		更新 YUV 格式的纹理的函数，将 YUV 数据更新到 SDL 窗口中进行显示。

```c
int SDL_UpdateYUVTexture(SDL_Texture* texture, const SDL_Rect* rect, const Uint8* dataY, int pitchY, const Uint8* dataU, int pitchU, const Uint8* dataV, int pitchV)
```

- `texture`: 需要更新的纹理。
- `rect`: 要更新的矩形区域，为NULL表示更新整个纹理。
- `dataY, dataU, dataV`: YUV 数据指针。
- `pitchY, pitchU, pitchV`: YUV 数据每行的字节数，通常为 width。



### Audio

#### 播放音频的基本流程

1. 打开音频设备
2. 设置音频参数
3. 通过音频回调函数向声卡的缓冲区填充数据
4. 播放音频
5. 关闭设备

​		注意的两个地方是

- 声卡向你要数据而不是你主动推给声卡
- 数据的多少由音频参数决定的

> 声卡每秒钟播放的数据量的大小它是能计算出来的，也就是通过你的声道数，采样数 等等算出你一秒钟输送的数据量。他会回调一个通知，告诉你要数据了，会调到你写的回调函数里。在这个回调函数中，你要把数据放到一个声卡指定的buff中，他就从buff中取数据。声卡的数据缓冲区大小不一样，都是有一定限制的，如果一下子推送很多数据，很难处理。还不如我给你要数据。需要多少 让你放多少。

#### `SDL_AudioSpec`

​		SDL_AudioSpec结构体是SDL实现音频处理的核心，使用它可以自定义音频规格并实现音频回调函数。开发者可以通过填写和配置SDL_AudioSpec结构体，实现对音频设备的控制和管理。

```c
typedef struct SDL_AudioSpec {
    int freq; // 音频采样率，表示每秒钟采样的次数。
    SDL_AudioFormat format; // 音频数据格式,表示每个采样点所占用的位数和正负号等信息。比如SDL支持的格式包括：S8、S16LSB、S16MSB、U16LSB、U16MSB、S32LSB、S32MSB等。
    Uint8 channels; // 声道数: 1 单声道, 2 立体声
    Uint8 silence; // 设置静音的值，因为声音采样是有符号的，所以0当然就是这个值
    Uint16 samples; // 音频缓冲区中的采样个数，要求必须是2的n次
    Uint16 padding; // 考虑到兼容性的一个参数
    Uint32 size; // 音频缓冲区的大小，以字节为单位
    SDL_AudioCallback callback; // 填充音频缓冲区的回调函数
    void *userdata; // 用户自定义的数据,用于传递额外的参数给回调函数。
} SDL_AudioSpec;

//用于向SDL音频设备填充音频数据并实现音频播放。
// userdata：SDL_AudioSpec结构中的用户自定义数据，一般情况下可以不用。
// stream：该指针指向需要填充的音频缓冲区。
// len：音频缓冲区的大小（以字节为单位）1024*2*2。
void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 *stream, int len);
```

- **当播放设备需要音频数据时，SDL将调用注册的回调函数，向其传递缓冲区指针和需要填充的数据长度**。回调函数需要在接收到这些数据后，快速将其填充到缓冲区中，并返回控制权给SDL音频系统，以便它能够将填充好的音频数据输出到相应的音频设备。
  - 从“userdata指针”中读取任何需要的数据。
  - 计算要向缓冲区中写入的数据量，它通常应该不大于len。
  - 将音频数据写入缓冲区中。如果数据不够，应该填充“沉默”数据以代替缺失的数据。
  - 返回控制权给SDL音频系统。
- 在SDL中，回调函数的限制和要求比较严格。回调函数必须从一个特殊的线程中调用，通常是SDL音频线程。此外，回调函数应该尽量避免长时间的计算或阻塞操作，以便及时响应SDL音频系统的请求。



#### `SDL_OpenAudio`

​		**打开音频设备并开始音频处理**。

```c
// desired：期望的参数。
// obtained：实际音频设备的参数，一般情况下设置为NULL即可。
int SDLCALL SDL_OpenAudio(SDL_AudioSpec * desired, SDL_AudioSpec * obtained); 
```

- `desired` 是指定期望的音频参数，`obtained` 是实际获得的音频参数
- 返回值是一个整数类型，为0表示成功，-1表示失败。如果返回值为-1，可以调用SDL_GetError()函数获取错误信息。



#### `SDL_OpenAudioDevice`

​		**打开 SDL 音频设备的函数**，**多线程环境下使用这个函数打开音频设备**

```c
SDL_AudioDeviceID SDL_OpenAudioDevice(
    						const char* device, 
    						int iscapture, 
    						const SDL_AudioSpec* desired, 
    						SDL_AudioSpec* obtained, 
    						int allowed_changes)

```

- `device`：要打开的音频设备的名称，可以为 NULL，表示打开默认的音频设备。
- `iscapture`：表示是否为录音设备。当其为 0 时，表示打开播放设备；当其为 1 时，表示打开录音设备。
- `desired`：期望的音频格式和采样率等信息。如果为 NULL，则表示使用默认的音频格式和采样率等信息。
- `obtained`：实际得到的音频格式和采样率等信息。如果为 NULL，则表示不需要获取实际得到的信息。
- `allowed_changes`：允许的音频格式和采样率等信息的改变量。如果为 0，则表示不允许任何改变；如果为 -1，则表示允许任何改变；如果为正整数，则表示允许指定数量的改变。
- **返回值为音频设备的 ID**，表示打开音频设备的结果。如果返回 0，则表示打开音频设备失败。



#### `SDL_CloseAudio`

​		SDL 库中用于**关闭音频设备的函数**。

​		当调用 SDL_CloseAudio 函数时，SDL 库会等待音频播放完成并清空音频缓冲区，然后关闭音频设备。因此，**在调用 SDL_CloseAudio 函数之后，应用程序需要等待一段时间，直到音频缓冲区中的数据被播放完毕，然后才能退出程序**。例如，可以在退出前使用 SDL_Delay 函数等待一段时间，确保所有的音频数据都已经被播放完毕。

```c
void SDL_CloseAudio(void);
```



#### `SDL_CloseAudioDevice`

​		关闭音频设备后，SDL 就不再调用回调函数来获取音频数据，并且音频设备的缓冲区也会被清空。

​		调用 SDL_CloseAudioDevice 函数之前，否则，如果音频设备的缓冲区中还有未播放的音频数据，那么这些音频数据就会被丢弃，从而导致数据丢失。

- 先等待音频设备播放完所有的音频数据，或者使用 SDL_Delay 函数延迟一段时间，以确保所有的音频数据都被播放完毕。
- 先使用 SDL_PauseAudioDevice 函数暂停音频设备，并将音频设备的缓冲区中的音频数据清空。

```c
void SDL_CloseAudioDevice(SDL_AudioDeviceID dev);
```



#### `SDL_PauseAudio`

​		**暂停或继续音频播放**。

​		当暂停音频播放时，音频设备会停止向缓冲区填充数据，并在接收到下一次 SDL_PauseAudio 函数的调用时恢复播放。若参数 pause_on 为 0，则音频缓冲区中的数据将不会被暂停，在恢复播放时，它将继续从最后一个位置开始播放。

```c
// 当pause_on设置为0的时候即可开始播放音频数据。设置为1的时候，将会播放静音的值。
void SDLCALL SDL_PauseAudio(int pause_on);
```

- **pause_on 是一个整数，表示是否要暂停音频播放。**
  - 若值为 1，则暂停播放(暂停音频设备输出流)
  - 若值为 0，则继续播放（才可以不断地从缓冲区中取出音频数据进行输）。



#### `SDL_PauseAudioDevice`

​		**启动或暂停 SDL 音频设备**

​		**当启动音频设备时，SDL 就会开始调用回调函数来获取音频数据，并将其写入音频设备的缓冲区中**。当暂停音频设备时，SDL 就不再调用回调函数，音频设备的缓冲区也不会被写入音频数据。

​		需要注意的是，当**使用 SDL_OpenAudioDevice 函数打开音频设备后，音频设备默认是暂停状态的**，需要使用 SDL_PauseAudioDevice 函数来启动音频设备。如果不调用 SDL_PauseAudioDevice 函数来启动音频设备，那么音频设备就不会播放任何音频数据。

​		另外，需要注意的是，当**使用 SDL_PauseAudioDevice 函数暂停音频设备时，如果音频设备的缓冲区中还有未播放的音频数据，那么这些音频数据就会被丢弃。因此，在暂停音频设备之前，应该先将音频设备的缓冲区中的音频数据清空，以免造成数据丢失。**

```c
void SDL_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on);
```

- `dev`：音频设备的 ID，表示要启动或暂停的音频设备。
- `pause_on`：表示是否暂停音频设备。当其为 0 时，表示启动音频设备；当其为 1 时，表示暂停音频设备。



#### `SDL_MixAudio`

​		用于将两个音频缓冲区的数据混合到一起，实现多个声音来源的混音，达到更好的音效效果。

​		在具体实现中，SDL_MixAudio会按照指定的音量大小，**将源缓冲区中的数据和目标缓冲区中的数据进行混合，并将混合结果存储到目标缓冲区中**。

​		SDL_MixAudio操作的两个缓冲区的音频数据格式必须一致，包括采样率、声道数以及数据类型等方面。此外，混合后的音量大小也需要根据具体应用场景进行合理的设置，避免音频数据质量降低或者输出音量过大导致系统无法承受。

```c
void SDL_MixAudio(uint8_t *dst, const uint8_t *src, uint32_t len, int volume);
```

- `dst`表示目标缓冲区的指针，用于存储混合后的音频数据
- `src``表示源缓冲区的指针，用于获取需要混合的音频数据`
- `len`表示需要处理的数据长度，以字节为单位
- `volume`表示混合后的音量大小

### 基本使用

1. 初始化SDL
2. 创建窗口
3. 创建渲染器
4. 清空缓冲区
5. 启动主循环等待用户关闭窗口
6. 绘制要显示的内容
7. 最终将缓冲区内容渲染到window窗口上
8. 销毁渲染器
9. 销毁窗口
10. 退出SDL

![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/f37ec72e0b774555a3950bb2a17abb37.png)



```c
int main(int argc, char* argv[])
{
    // 初始化 SDL 库
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("My Window",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480, SDL_WINDOW_SHOWN);

    // 等待用户关闭窗口
    bool is_quit = false;
    SDL_Event event;
    while (!is_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_quit = true;
            }
        }
    }

    // 释放窗口资源并退出 SDL 库
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
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