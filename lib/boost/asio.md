# Asio

---



## framework

​		1.66版本，boost.asio库重新设计了框架。新框架有几个核心概念：`Context`，`Scheduler`，`Service`，`Executor`，`Strand`。



### Context

- `asio`所有功能必须通过一个***`Context`***里调度执行
- 每个***`Context`*** 都有一个***`Service`*** 注册表，管理***`Service`***
  - 每个***`Context`*** 下的***`Service`*** 都是唯一的
-  每个***`Context`*** 都有一个***`Scheduler`***
-  ***`io_context`***是一种对io操作优先的优化***`Context`***，将io事件多路复用方法做成内嵌任务
  -  ***`io_context`*** 的`win`版本对***`Schdeluer`*** 进行了优化，聚合了`iocp`。
-  ***`Context`*** 必须通过在线程运行`poll()`或`run()`进入调度消费***`Scheduler`*** 执行队列并执行任务
  -  可以在多线程上同时运行poll()或run()，并且线程安全



### Scheduler

- 是一个***`Context`*** 的服务
- 有一个`op_queue`执行队列
- 所有***`Service`*** 的执行最终都依赖***`Scheduler`*** 调度
- ***`Scheduler`*** 的`dispatch()`方法将任务调度到执行队列



### Service

- 为某种功能提供调度以及功能服务
- 依赖所在的 ***`Context`*** 的 ***`Scheduler`*** 调度执行
- 每种 ***`Service`*** 都有一个`service_impl`类，并为这个类提供服务



## Executor

- 相当于`ios`中的可并行的`dispatch_queue`
- 相当于一个 ***`Context`*** 的服务，或者对 ***`Context`*** 的 ***`Execution`*** 行为的委托
- 最终依赖所在的***`Context`***的***`Scheduler`***调度服务

[浅谈The C++ Executors - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/395250667)

### Strand

- 相当于`ios`中的串行化的`dispatch_queue`
- 分两种服务，绑定本***`io Context`*** 以及可以指定***`Executor`*** (即不同类型的***Context*** )
- 每个***`Strand**`* 有独立的执行队列
- ***`Strand`*** 本身作为一个任务，必须在***`Scheduler`*** 进行调度分派。
- 同一个***`Strand**`* 同时只能在一条线程上分派执行队列
- 当多线程同时对***`Strand`*** 分派时，其它线程只能将任务缓冲到等待队列
- 利用本身强制串行化的特性，可代替同步锁，保护变量和代码，减少线程切换



![img](https://img2018.cnblogs.com/blog/665551/201911/665551-20191126183056303-1329771926.jpg)

> Scheduler为核心服务，所有服务最终都会依赖Scheduler的服务，位于图正中间两个红色的类

![img](https://img2018.cnblogs.com/blog/665551/201911/665551-20191126201306832-275618507.png)

> 位于接口层的每个功能都有一个basic_前缀的类，每个接口对象与一个service_impl对象关联，并凭借service_impl对象向对应的Service获取得服务支持。
>
> 接口层的功能分io与非io两类。io功能包括网络io，文件io以及串口io。非io功能包括定时器（timer），信号（signal）广播，以及媲美dispatch_queue的strand与executor。