# pjsip

---

​		`PJSIP`是一个支持`SIP`、`SDP`、`RTP`、`RTCP`、`STUN`、`ICE`等协议实现的开源库。

​		它把基于`SIP`的多媒体框架和`NAT`穿透功能整合成高层次、抽象的多媒体通信`API`。

## 参考

 [pjlib深入剖析和使用详解 - DoubleLi - 博客园.url](resource\pjlib深入剖析和使用详解 - DoubleLi - 博客园.url) 

 [pjsip_chrycoder的博客-CSDN博客.url](resource\pjsip_chrycoder的博客-CSDN博客.url) 

 [pjsip架构介绍以及封装使用 - 简书.url](resource\pjsip架构介绍以及封装使用 - 简书.url) 

 [PJSIP开发指南中文版1-6章.url](resource\PJSIP开发指南中文版1-6章.url) 

 [PJSIP开源库详解 - Ray.floyd - 博客园.url](resource\PJSIP开源库详解 - Ray.floyd - 博客园.url) 

 [voip_百无求的博客-CSDN博客.url](resource\voip_百无求的博客-CSDN博客.url) 

## SIP协议栈结构

​		用于处理SIP消息

### 模块

- `pjlib`：平台抽象与框架（数据结构、内存分配、文件I/O、线程、线程同步等），是SIP协议栈的基石。

- `pjlib-util`：封装常用的算法(MD5、CRC32)、字符串、文件格式解析(XML，json）

- `pjsip-core`：`SIP`协议栈的核心
  
  - `SIP endpoint`
  
  - `SIP module`
  
    - `SIP transaction module`
  
    - `SIP UA module`
  
    - `SIP dialog module` ： 维护`sip`请求和响应之间关联关系的会话层表示
  
    - `application module`
  
    ```c
    /**
     * Module priority guidelines.
     */
    enum pjsip_module_priority
    {
        /** 
         * This is the priority used by transport layer.
         */
        PJSIP_MOD_PRIORITY_TRANSPORT_LAYER	= 8,
    
        /**
         * This is the priority used by transaction layer.
         */
        PJSIP_MOD_PRIORITY_TSX_LAYER	= 16,
    
        /**
         * This is the priority used by the user agent and proxy layer.
         */
        PJSIP_MOD_PRIORITY_UA_PROXY_LAYER	= 32,
    
        /**
         * This is the priority used by the dialog usages.
         */
        PJSIP_MOD_PRIORITY_DIALOG_USAGE	= 48,
    
        /**
         * This is the recommended priority to be used by applications.
         */
        PJSIP_MOD_PRIORITY_APPLICATION	= 64
    };
    ```
  
  - `SIP transport`
  
- `pjsip-simple`：SIP事件与出席(`presentation`)框架，如果你程序中要实现出席制定，则该库是必备的。

- `pjsip-ua`：`INVITE`会话的高层抽象，容易创建SIP会话。

  - 此外还实现了`SIP client`(自定义模块)的注册API

- `pjsua`：最高层次抽象，基于`pjsip-ua`和上述库做了高层次封装。具备`voip`功能



### 机制

- `Transport layer`通过`transport manager`将网络事件通知到`endpoint`
  - 支持`TCP`、`UDP`、`TLS`三种方式
- `endpoint`管理整个事件协议栈
- 线程安全：比较麻烦，但是整个协议中都保持一致的设计原则

![image-20250222230218658](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20250222230218658.png)

#### endpoint 机制

- 内存管理
- 事件处理
- 定时器
- 管理协议栈
- 事件循环（event loop）

##### 内存管理

​		`endpoint`对象为所有的`SIP`对象管理内存的分配与释放

- `pjsip`中动态内存都是通过内存池分配的
- 所有的`SIP`内存都需要在`endpoint`中完成分配
- 
  - 为了保证线程的安全性以及在整个应用中策略强一致性
  - `pjsip_endpt_create_pool() / pjsip_endpt_release_pool()`
  - 当`endpoint`被`pjsip_endpt_create()`创建时，应用一定要指明由`endpoint`使用的内存池工厂
  - 在整个生命周期内`endpoint`持有内存池工厂的指针，将来备用创建和释放内存

> ​		内存池和内存池工厂的关系如下：
>
> - 层次关系
>   1. `pj_pool_factory` (内存池工厂)
>      - 最顶层的内存管理组件
>      - 在创建 SIP 终端时传入 (`pjsip_endpt_create` 函数)
>      - 由 SIP 终端持续引用
>
>   2. `pjsip_endpoint` (SIP 终端)
>      - 保存了对内存池工厂的引用
>      - 作为内存分配的中间层
>      - 确保所有相关组件使用同一个内存池工厂
>   3. `pj_pool_t` (内存池)
>      - 通过 `pjsip_endpt_create_pool` 创建
>      - 实际由终端内部存储的内存池工厂创建
>      - 用于具体的内存分配操作
>
>
> - 工作流程
>
>   1. 创建 SIP 终端时传入内存池工厂
>
>   2. 终端保存工厂引用
>
>   3. 需要内存池时，通过终端的 `create_pool` 函数
>
>   4. 终端内部使用保存的工厂创建新的内存池
>
>
> - 优势
>
>   - 统一管理：所有内存分配来自同一个工厂
>
>   - 线程安全：通过终端接口访问保证安全
>
>   - 资源追踪：便于内存管理和调试

##### 事件处理

​		`pjsip`协议栈将事件作为`module`注册到`endpoint`中，以`endpoint`的事件循环驱动。以`module id`定义事件的处理顺序，越接近`transport`层`module id`越小。

- `endpoint`接收来自传输层的消息
  - 单实例`pjlib ioqueue`，采用`proactor`模式分发事件（`pjsip`本身不含任何线程）
- `endpoint`将消息向上层分配：
  - 消息先被传递到`transaction`模块处理
    - `transaction`模块在`transaction hash table`中查找消息对应的`transaction`
    - 如果查找到则接收并处理，然后消息由`transaction`继续向dialog传递
    - 如果没找到则不处理，继续向`dialog`传递
  - `dialog`如上进行查找和向应用层传递
  - 如果消息不能被所有协议栈上的模块处理，则消息由`endpoint`来做默认处理

##### 定时器

- `endpoint`为所有的`SIP`组件调度定时器
  - `endpoint `拥有一个独立定时器堆实例
  - 所有`SIP`组件的定时器创建和调度都需要通过`endpoint `完成
    - `pjsip_endpt_schedule_timer() / pjsip_endpt_cancel_timer()`
- `endpoint`的`poll`函数被调用时，检查定时器会否过期

##### 管理协议栈

- 应用层可以不止一层，由多个自定义模块。一起构成整个`sip`协议栈
- 自定义模块被加入到协议栈应用层中
  - 自定义模块作为一个节点被挂载在协议栈链表上
  - 节点中指定模块的名字、优先级以及一起回调钩子来处理事件
- 可以创建非应用层的自定义模块
  - 只要让优先级高于`transaction`或`dialog`就可以做到截获的目的



##### 事件循环

​		提供事件循环驱动协议栈的运行（event loop）

- `endpoint `提供独立的函数`pjsip_endpt_handle_events()`检查定时器和网络事件的出现
  - 应用可以指定检查的`timeout`
- `Pjsip `栈不创建线程，整个栈运行在应用所调用的线程
  - 不管API调用或者轮询函数调用
- 根据定时任务的时间，灵活调整等待网络事件的时间，尽量不浪费资源
  - 比如：当它找到定时器将在下个5S过期，他等待socket事件的时间，将不会比5S长
  - 在无网络事件出现时，应用程序等待超过5S是没有必要的
  - 当然每个平台的定时器的精度是不一样的



#### 线程安全机制

- 对象一定是线程安全的
  - `PJSIP`的对象一定是线程安全的，比如：
    - `PJLIB `对象，比如`ioqueue`
    - `PJSIP `对象，比如： `endpoint`、`transactions`、`dialogs`等
- 数据结构一定不是线程安全的
  - `PJLIB`的数据结构，比如：
    -  链表、数组、哈希表、字符串、内存池。
    - `SIP`消息的元素，`URLs`、`header fields`和`SIP Message`
  - 数据结构的线程安全由包含他们的对象保证
  - 如果数据结构是线程安全的，将会严重的影响协议栈的性能并且消耗操作系统的资源



##### 复杂性

- 一些对象在头文件中暴露了他们的声明（`pjsip_transaction `和 `pjsip_dialog`）

  - `API`暴露可以保证线程的安全性
    - 在访问数据结构前，必须通过`pj_mutex_lock`获取到对象的互斥锁

- `dialog`暴露的不同的`API`锁定`dialog`，导致多次上锁的问题

  - 应该调用`pjsip_dlg_inc_lock / pjsip_dlg_dec_lock`替代`pj­_mutex_lock / pj_mutex_unlock`

  - 两种处理方式区别是：

    - `Dialog`的`inc/dec`保证`dialog`不会在函数调用时销毁

      ```c
      Pjsip_dlg_inc_lock(dlg);
      Pjsip_dlg_end_session(dlg);
      Pjsip_dlg_dec_lock(dlg);
          
      // Dialog Inc/dec可以通过增加dialog的引用计数，防止crash的发生。
      // 在end_session时，dialog 不会被销毁，dialog 可能会在dec_lock函数中被销毁。
      ```

    - `pj_mutex_unlock`会因为`dialog`销毁了而导致`crash`

      ```c
      pj_mutext_lock(dlg->mutex);
      pjsip_dlg_end_session(dlg,…);
      pj_mutex_unlock(dlg->mutex);
      
      // 因为pjsip_dlg_end_session可能会销毁dialog在某些情况下。
      // 例如：INVITE事务没有被应答，事务会被马上销毁，导致dialog被立刻销毁。
      // 应用可能会crash
      ```

- 锁的调用顺序必须是正确的，否则引起死锁
  - 需要锁`dialog`和`dialog `的 `transaction`
  - 必须先获得`dialog mutex`再获取`transaction mutex`
  - 否则当其他的线程以相反的顺序拿锁时，将会引起死锁



##### 解决办法

- 应用程序很少会直接获取对象的锁，所以上面的问题很少会发生
- 应用应该使用对象的`API`访问对象
  - 对象的`API`会检查，保证`lock`的正确性，避免死锁和`crash`的产生
- 应用程序的回调被对象调用时，对象的锁已经获取到了
  - 所以应用能够安全的获取对象的数据结构，不必要获取对象的锁



## 媒体处理结构

​		用于处理RTP包

### 模块

- `media transport`
  - 媒体传输端口 ， 负责接收媒体流
  - 支持 RTP（UDP）、SRTP（加密）、ICE（NAT穿透）
- `media port`
  - 实现了各种媒体端口，每个`media port`上定义各种操作（创建、销毁、`get/put`等）
    - `file writer`
    - `file player`
    - `stream port`
    - `conference port`
    - `master port`

### 机制

![image-20250222230332207](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20250222230332207.png)

​		从右向左依次是：

- `media transport`
  - `stream port` 与`media transport`之间的连接是通过`attach`和`detach`操作完成的，该操作是在创建`stream port`执行
  - 为了能正常接收`RTP`流，需要为`media transport`提供轮询机制，通常使用`SIP endpoint`的`I\O queue`即可
    - 这是在创建`media transport`时通过参数设置的
- `stream port`
  - 上图中最重要的是`stream port` ，如果使用了`pjmedia`库，则必少不了`stream port `
  - `stream port` 中
    - 从接收`RTP`包的角度讲，`RTP`包会被做以下处理：
      - `decode RTP into frame ---> put each frame into jitter buffer ---> decode frame into samples with codec`
      - `jitter buffer`是一种缓冲技术，主要用于音频视频流的缓冲处理
    -  从发送`RTP`包的角度讲，除了包含媒体流数据的RTP包外，还会存在`keep alive UDP pakcet`
- `conference port`
- `sound device port`
  - 媒体流数据是通过各个`media port`操作进行传递的
  - 在上图中驱动媒体流由左往右流动的驱动器是`sound device port`
  - 该端口是通过`sound device`的硬件驱动不停向与它连接的`media port`实施`get or put fram `操作，从而媒体流得以流动
  - 媒体流处理模块中，像`sound device port`的端口，称为主动型端口或者驱动型端口
  - 媒体流处理模块中另外一个主动型端口就是`master port`
-  `sound device `
  - 前四个需要自己在程序里创建，最后一个`sound device` 是与`sound device port`相关联的
  - 创建`sound device port`的时候便会关联到`sound device`







