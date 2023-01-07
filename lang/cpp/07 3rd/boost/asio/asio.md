# asio

​		用于网络和底层IO编程的跨平台C++库。提供了c++环境下稳定的异步IO模型。

---

- 可移植性 Portability
- 可扩展性 Scalability
- 效率 Efficiency
- 像BSD Sockets一样通过API函数建立模型概念
- 易于使用 Ease of use
- 基于更多的抽象 Basis for further abstraction



> ​		All of asio classes can be used by simply including the `asio.hpp` header file.
>
> ​		包含`asio.hpp`头文件，就可以使用所有asio中的类。
>
> 但这存在一个问题就是编译时间的增加。未来会在`import`的帮助下得到改善。

## IO对象

​		对于IO资源的抽象，如socket，底层文件IO组件，都是IO对象。





## 同步模型

![image-20211121144026099](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20211121144026099.png)

- 程序创建IO对象
  - 如socket
- IO对象的IO事件通知到io_service
- io_service代理调用操作系统的API
- io_service同步的接收操作系统的返回结果 
- io_service将操作通知到IO对象
  - 失败可以通过错误码或抛出异常两种方式通知
- IO对象将结果返回程序

```c++
#include <iostream>
#include <asio.hpp>

using namespace std::chrono_literals;
int main()
{
    asio::io_context io;
    asio::steady_timer t( io, 5s );
    t.wait();
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
```



## 异步模型

> ​		Using asio's asynchronous functionality means having a callback function that will be called when an asynchronous operation completes. 

![image-20211121150217302](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20211121150217302.png)

- 程序创建IO对象
  - 如socket
- IO对象的IO事件通知到io_service
- io_service代理调用操作系统的API
- io_service直接返回用户程序执行
- io_service异步的接收操作系统结果
- io_service通过应用程序预先注册的处理程序Handler来处理操作结果

```c++
#include <iostream>
#include <asio.hpp>
#include <thread>

void print( const asio::error_code & )
{
    std::cout << std::this_thread::get_id() << " Hello, world!" << std::endl;
}

using namespace std::chrono_literals;
int main( void )
{
    asio::io_context io;
    asio::steady_timer t( io, 5s );
    asio::steady_timer t2( io, 2s );
    t.async_wait(print); // not wait
    t2.async_wait( print ); // not wait
    
    std::this_thread::sleep_for( 1.5s );
    std::cout << std::this_thread::get_id() << " start run\n";
    io.run(); // wait, run print callback
    std::cout << std::this_thread::get_id() << " finish run\n";

    return 0;
}
```

> - we must call the [io_context::run()](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/io_context/run.html) member function on the io_context object.
>
> - The asio library provides a guarantee that callback handlers will only be called from threads that are currently calling [io_context::run()](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/io_context/run.html).  
>
> - Therefore unless the [io_context::run()](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/io_context/run.html) function is called the callback for the asynchronous wait completion will never be invoked.
>
>   > 实际上是将结果的处理权转交至调用方
>
> - The io_context::run() function will also continue to run while there is still "work" to do. 
>
>   ​		 In this example, the work is the asynchronous wait on the timer, so the call will not return until the timer has expired and the callback has completed.
>
> - It is important to remember to give the io_context some work to do before calling [io_context::run()](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/io_context/run.html).
>
>   ​		 For example, if we had omitted the above call to [steady_timer::async_wait()](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/basic_waitable_timer/async_wait.html), the io_context would not have had any work to do, and consequently [io_context::run()](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/io_context/run.html) would have returned immediately.
>
>   > ​		async_wait实际上向io_service注册了timer的同时，也注册回调事件。并不是在run的时候才开始计时，而是在`async_wait`的时候已经开始计时。
>   >
>   > ​		这意味着，不run就不会执行回调，run起来会检查事件是否已经完成。



## 定时器