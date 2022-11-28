# 智能指针

---

​		自动管理对象生存期，异常安全。

> 裸指针除了用作函数接口外，永远不应使用裸指针。
>
> 首选unique_lock，知道编译不过，再考虑有必要时换用shared_ptr。

## unique_ptr

## shared_ptr

### 实现原理

### 线程安全性

### make_shared

> ```c++
> std::make_shared<A>(params); // 效率更高,一次new，空间上更少
> std::shared_ptr(new A(params)); // 两次new ，空间更多
> ```
>
> make_shared在某些情况下是不能使用的，比如当构造函数私有时。

## enable_shared_from_this

​		在构造函数和析构函数的函数栈上不能进行`shared_from_this()`的调用，会抛出`bad_weak_ptr`的运行异常。因为在构造和析构的函数栈上，对象语义都是不完整的，没有完整的内存模型，因而不能由此来获取智能指针。