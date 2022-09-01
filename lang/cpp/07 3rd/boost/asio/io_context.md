# io_context

```c++
class io_context : 
 public execution_context
```

> ​		All programs that use asio need to have **at least one I/O execution context, such as an io_context or thread pool object**.
>
> ​		An I/O execution context provides access to I/O functionality.

- 提供上层对IO对象的编程接口
- 代理操作系统接口

> ​		旧版本中为io_service，现在`io_service`也能够使用，但实际上已经通过typedef称为`io_context`的别名。



## 类型

### strand

​		提供序列化的事件处理执行。

```c++
class strand
{
  	asio::detail::strand_service& service_;
  	mutable asio::detail::strand_service::implementation_type impl_;
public:
    //...
	...
};
```

#### 成员函数

##### constractor

​		io_context对象，该对象被链用来分派准备运行的处理程序。  

```c++
/**
* Constructs the strand.
*
* @param io_context 被strand用来分发准备运行的处理程序
*/
explicit strand(asio::io_context& io_context)
    : service_(asio::use_service<
               asio::detail::strand_service>(io_context))
{
	service_.construct(impl_);
}
```

##### deconstractor

​		销毁一个strand， 通过strand递交的handler，仍然将以非并发的方式被分发。

```c++
/**
* Destroys a strand.
*
* Handlers posted through the strand that have not yet been invoked will
* still be dispatched in a way that meets the guarantee of non-concurrency.
*/
~strand(){}
```



##### wrap

​		已弃置，推荐使用`bind_executor`

​		创建一个包装的handler。并自动分发。

```c++
template<typename Handler>
unspecified wrap(Handler handler);

// handler , 要包装的处理程序
// strand 将根据需要赋值handler， 签名必须如下
void handler(A1 a1, ... An an);
```

​		返回一个函数对象，被调用时，它将包装好的处理程序传递给A的分派函数.

```c++
R f(A1 a1, ... An an);
strand.wrap(f);
void g(A1 a1, ... An an);

// 等价于
strand.dispatch(std::bind(f, a1, ..., an));
```

​		比如一个注册到``io_context``的任务没有通过`strand.wrap`来包装，那么它可能会被`io_context::run()`执行的多个线程抢占并直接开始执行。

```c++
#if !defined(ASIO_NO_DEPRECATED)
  /// (Deprecated: Use asio::bind_executor().) Create a new handler that
  /// automatically dispatches the wrapped handler on the strand.
  /**
   * This function is used to create a new handler function object that, when
   * invoked, will automatically pass the wrapped handler to the strand's
   * dispatch function.
   *
   * @param handler The handler to be wrapped. The strand will make a copy of
   * the handler object as required. The function signature of the handler must
   * be: @code void handler(A1 a1, ... An an); @endcode
   *
   * @return A function object that, when invoked, passes the wrapped handler to
   * the strand's dispatch function. Given a function object with the signature:
   * @code R f(A1 a1, ... An an); @endcode
   * If this function object is passed to the wrap function like so:
   * @code strand.wrap(f); @endcode
   * then the return value is a function object with the signature
   * @code void g(A1 a1, ... An an); @endcode
   * that, when invoked, executes code equivalent to:
   * @code strand.dispatch(boost::bind(f, a1, ... an)); @endcode
   */
  template <typename Handler>
#if defined(GENERATING_DOCUMENTATION)
  unspecified
#else
  detail::wrapped_handler<strand, Handler, detail::is_continuation_if_running>
#endif
  wrap(Handler handler)
  {
    return detail::wrapped_handler<io_context::strand, Handler,
        detail::is_continuation_if_running>(*this, handler);
  }
#endif // !defined(ASIO_NO_DEPRECATED)
```





### work

​		通过通知的方式，接管`io_context::run`的起停，保证`io_context::run`的运行和终止由`work`实例的构造和析构来掌控。

​		实际上相当于`run()`的`guard`类。可拷贝构造，但不能赋值。可以用作`handler class`的成员。

```c++
class io_context{
    class work {
      public:
        // 两个构造函数用来通知io_context开始run
        explicit work(asio::io_context& io_context);
        work(const work& other);
        // 析构函数通知io_context结束run，但run会等到所有未完成的任务都完成才退出
        ~work();
        
        // 获取work所保护的io_context对象
        asio::io_get_io_context();
    };
};
```





## 成员函数

### run

​		开启`io_context`对象的事件处理循环。

```c++
count_type run();

// count_type 已执行的处理程序数。

// 已弃置，推荐使用无error_code的重载
count_type run(asio::error_code& ec);
```

​		一直阻塞，可以通过其他线程调用`post`来向`io_context`持续分发任务。

> poll()函数也可以用来分派就绪的处理程序，但不会阻塞。  

​		默认情况下执行完所有注册的任务后，`run`将退出执行成为``stopped``状态。后续调用`run()、run_one()、poll()或poll_one()`是无效操作，将立即返回。，除非之前有对``restart()/reset()``的调用。

​				在同一个`io_context`上的同一个线程中，`run()`还没结束时就再次`run()`，可能导致死锁。调用者来避免这种情况出现。

​		直到所有的工作都完成并且不再有handler被分派，或者直到``io_context``成为`stopped`状态。  

- 可以通过`io_context::reset`函数来重新启用`run`事件处理循环。

  ```c++
  #include <iostream>
  #include <asio.hpp>
  #include <thread>
  
  void print( const asio::error_code & ) {
      std::cout << std::this_thread::get_id() << " Hello, world!" << std::endl;
  }
  
  using namespace std::chrono_literals;
  int main( void )
  {
      asio::io_context io;
      //asio::io_context::work work{io};
      asio::steady_timer t( io, 5s );
      t.async_wait(print); // not wait
      std::cout << std::this_thread::get_id() << " start run\n";
      
      io.run(); // wait, run print callback
      asio::steady_timer t2( io, 2s );
      t2.async_wait( print ); // not wait
      std::this_thread::sleep_for( 1.5s );
      if ( io.stopped() )
          io.reset();
      io.run();
      std::cout << std::this_thread::get_id() << " finish run\n";
  
      return 0;
  }
  ```

  

- 可以通过持有`io_context::work`的实例来保证`run`完成所有注册的任务后也不退出。



​		可以在多个线程中调用`io_context::run()`，使得这些线程共同组成`io_context`的一个线程池。

> ​		`io_context`可以从中执行handler。 所有在池中等待的线程都是等效的，io_context可以选择其中任何一个来调用handler。
>
> ```c++
> int main()
> {
>     asio::io_context ctx;
>     printer p { ctx };
>     std::thread t( [&ctx] {ctx.run();} );
>     ctx.run();
>     t.join();
> }
> ```
>
> 

### reset

​		重置``io_context``状态，为后续的再次``run()``调用做准备。  在调用``reset()``之后，``io_context::stopped()``函数将返回``false``。 

```c++
void reset();
```

​		如果``io_context``停止或耗尽工作而返回，则再次调用``run()、run_one()、poll()或poll_one()``函数之前可以调用`reset()`来重置`io_context`的状态，使其能够再次`run()`。

​		当``run()、run_one()、poll()或poll_one()``函数有未完成的调用时，不能调用此函数。



### restart

​		重启`io_context`，为后续再次`io_context::run()`调用做准备。在调用``restart()``之后，``io_context::stopped()``函数将返回``false``。 

```c++
void restart();
```

​		如果``io_context``停止或耗尽工作而返回，则再次调用``run()、run_one()、poll()或poll_one()``函数之前可以调用`restart()`来重启`io_context`，使其能够再次`run()`。

​		当``run()、run_one()、poll()或poll_one()``函数有未完成的调用时，不能调用此函数。



### stopped

​		确定io_context对象是否已经停止。  

​		显式调用`io_context::stop()`和工作耗尽，都可以使得`io_context`成为`stopped`状态。

```c++
bool stopped() const;
```

​		对``run()、run_one()、poll()或poll_one()``的调用将立即返回，而不调用任何处理程序。  

