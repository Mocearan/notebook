# timer

---

## basic_waitable_timer

```c++
template<typename Clock, typename WaitTraits, typename Executor>
class basic_waitable_tiemr;
```



​		析构函数销毁定时器，等同于调用`cancel`，取消定时器上任何未完成的异步等待操作。

> ​		This function destroys the timer, cancelling any outstanding asynchronous wait operations associated with the timer as if by calling `cancel`.

```c++
// destructor
~basic_waitable_timer();

// 定时器析构是，无需额外的调用cancel。
```



### 成员函数

#### expires_at

```c++
// 以绝对时间的形式获取计时器的到期时间。 已弃置，推荐使用`expiry()`.
time_point expires_at() const;
// 将计时器的到期时间设置为绝对时间。  
std::size_t expires_at(const time_point& expiry_time);
// 将计时器的到期时间设置为绝对时间. 已弃置，推荐使用无error_code的版本
std::size_t expires_at(const time_point& expiry_time, asio::error_c);
```



​		





## steady_timer

​		Typedef for a timer based on the steady clock.。

```c++
typedef basic_waitable_timer< chrono::steady_clock > steady_timer;
```

​		The [`basic_waitable_timer`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/basic_waitable_timer.html) class template provides the ability to perform a blocking or asynchronous wait for a timer to expire.

```c++
~steady_timer();
```



### 成员函数

#### `async_wait`

 		初始化针对异步等待的定时器，立即返回。

```c++
template<typename WaitHandler = DEFAULT>
DEDUCED async_wait(WatiHandler && handler = DEFAULT);
```

​		定时执行的任务通过handler的方式注册，handler的执行时机是：

- 定时器过期
- 定时器被取消时，这时会将`asio::error::operation_aborted`传入定时器。

​		因此定时器的原型为：

```c++
void handler(const asio::error_code& error);
```

​		handler的调用同`post`一样，由特定的`io_context`对象去执行。无论如何，不会在`async_wait`中执行。

​		可以取消`async_wait`的类型有：

- `cancellation_type::terminal`
- `cancellation_type::partial`
- `cancellation_type::total`