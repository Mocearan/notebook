

# 并发

​		并发是指同时执行多个任务。

---

​		并发通过在单个计算中使用多个处理器提高吞吐量或通过部分运行部分等待提高响应能力。

​		所有现代编程语言都对此提供了支持。c++标准库提供一种可移植的、类型安全的变体支持。标准库的支持主要是为了支持系统级的并发，而不是直接提供复杂的高层并发模型。可以通过标准库作为基础设施来构建更为高级的库。

​		标准库直接支持在一个地址空间中并发执行多个线程。为了实现这一点，c++提供了一个合适的内存模型和一组原子操作。原子操作允许无锁编程[Dechev,2010]。内存模型确保只要程序员避免数据竞争(不受控制的并发访问可变数据)，一切都会按照人们的逻辑预期工作。然而，大多数用户只会在标准库和基于标准库构建的库中看到并发。

​		并行/并发编程的技术发展，伴随着从`thread`、`thread pool`、`task`、`fiber`、`coroutine`、`executors`逐步发展演化。这些特性直接建立在操作系统提供的基础上，与之相比不会产生性能损失，也不能保证显著的性能提升。

​		c++支持协程。也就是说，函数在两次调用之间保持其状态。，、

​		不要认为并发是万灵药。如果一个任务可以按顺序完成，通常会更简单、更快。从一个线程传递信息到另一个线程可能非常昂贵。

​		并行算法是显式并发特性的替代方案，可以利用多个执行引擎来获得更好的性能。





## task

​		任务(task)是可能与其他计算并发执行的执行单元。

​		线程是任务的系统级表示。`<thread>`中的`std::thread`以任务为参数启动一个与其他任务并发执行的线程。任务是一个函数或函数对象。

```c++
void f();                             // function

struct F {                           // function object
     void operator()();     // F's call operator (§7.3.2)
};

void user()
{
     thread t1 {f};             // f() executes in separate thread
     thread t2 {F{}};         	// F{}() executes in separate thread

     t1.join();                   // wait for t1
     t2.join();                   // wait for t2
}
```

​		任务的定义，目标是保持任务完全独立，或以简单而明显的方式通信。可以将并发任务理解为恰巧与调用者并发运行的函数。通过传递参数，获取结果，确保之间没有数据竞争。



## thread

​		线程共享一个地址空间，这与进程不同，进程之间通常不直接共享数据。

​		线程共享一个地址空间，因此可以通过共享对象进行通信。这种通信通常由锁或其他机制控制，以防止数据竞争。

```c++
void f();                             // function

struct F {                           // function object
     void operator()();     // F's call operator (§7.3.2)
};

void user()
{
     thread t1 {f};             // f() executes in separate thread
     thread t2 {F{}};         	// F{}() executes in separate thread

     t1.join();                   // wait for t1
     t2.join();                   // wait for t2
}
```

​		`join()`确保在线程完成之前不会退出``user()``。`join`意味着“等待线程终止”。

​		忘记``join()``可能使得线程任务未结束而程序退出，因此标准库提供了``jthread``，它是一个“关联线程”，复合``RAII``在析构函数中进行``join()``:

```c++
void user()
{
    jthread t1 {f};          // f() executes in separate thread
	jthread t2 {F{}};       // F{}() executes in separate thread
}
```

​		因为依赖于析构函数来进行`join`，析构函数的顺序与对象的构造顺序相反，所以先等待`t2`再等待`t1`。



### thread

### mutex

### condition_variable

### semaphore

### barrier

## thread pool



## future / promise

​		task模型基于`future/promise`模型，将线程执行的对象抽象为颗粒化的任务。

​		使用并发的方式实现并行，并行调度的核心是根据task graph，使得多核的任务始终是互相独立无依赖的。

![image-20211204183728198](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20211204183728198.png)

​		一开始就将task关系图交给调配器进行统筹，省去执行一个task后再评估情况分配资源的开销。



### promise



### future

​		future异步的获取结果，可以多线程，也可以用协程等其他计算子。

## fiber

## coroutine

## executors

​		executors基于sender/receiver模型。

> p0443、p2300两个提案
>
> [浅谈The C++ Executors - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/395250667?utm_source=wechat_session&utm_medium=social&utm_oi=27088287432704&utm_campaign=shareopn)



## 线程间通信

​		通过对全局变量、信号量、互斥量、条件变量等来实现线程间通知和消费消息。

​		还可以使用`promise`和`future`等新特性来进行通信。

> ​		服务器编程中，通过半同步半reactor的`event loop` + 队列来实现主线程与子线程的通信。
>
> ​		线程通信不需要特别的手段，需要的是线程同步。

