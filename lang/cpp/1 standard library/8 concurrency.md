

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





# task

​		任务(task)是可能与其他计算并发执行的执行单元。

​		线程是任务的系统级表示。`<thread>`中的`std::thread`以任务为参数启动一个与其他任务并发执行的线程。任务是一个函数或函数对象。

​		关于线程最简洁的思维模型是：将任务视为恰巧可以和其他任务并发的函数。这不是c++标准库支持的唯一模型，但它可以很好地满足广泛的需求。可以根据需要使用更微妙和棘手的模型(例如，依赖于共享内存的编程风格)。

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

​		忘记``join()``可能使得线程任务未结束而程序退出，因此标准库提供了``jthread``，它是一个“关联线程”，复合``RAII``在析构函数中进行``join()``:

```c++
void user()
{
    jthread t1 {f};          // f() executes in separate thread
	jthread t2 {F{}};       // F{}() executes in separate thread
}
```

​		因为依赖于析构函数来进行`join`，析构函数的顺序与对象的构造顺序相反，所以先等待`t2`再等待`t1`。

### passing argument

​		通常，任务需要处理数据，一般将数据(或指向数据的指针或引用)作为参数传递。

```c++
void f(vector<double>& v);        // function: do something with v

struct F {                                      // function object: do something with v
     vector<double>& v;
     F(vector<double>& vv) :v{vv} { }
     void operator()();                // application operator; §7.3.2
};

int main()
{
     vector<double> some_vec {1, 2, 3, 4, 5, 6, 7, 8, 9};
     vector<double> vec2 {10, 11, 12, 13, 14};

     jthread t1 {f,ref(some_vec)};    // f(some_vec) executes in a separate thread
     jthread t2 {F{vec2}};                 // F(vec2)() executes in a separate thread
}
```

​		如果希望在线程间避免数据竞争，可以通过值传递拷贝副本的方式让线程独立操作一个数据对象。即一定意义上的线程局部对象。

- `{f,ref(some_vec)}`使用线程可变模板构造函数，接受任意的参数序列
- ``ref()``是``<functional>``中的一个类型函数，``some_vec``视为一个引用，而不是一个对象。
  - 如果没有``ref()``， `some_vec`将通过值传递。
- 编译器检查第一个参数是否可以被调用，并构建必要的函数对象传递给线程。
  - `F::operator()()`和``F()``执行相同的算法，这两个任务的处理大致是等效的
  - 在这两种情况下，都会为线程构造一个函数对象来执行

### returning Results

- 通过非常量引用传递参数

  - 用于希望任务修改数据的值。
  - 这种返回结果的方式有点狡猾，但并不少见

  ```c++
  jthread t1 {f,ref(some_vec)};
  ```

- 通过常量引用传递输入数据，并将结果存放的位置作为一个单独的参数传递

  - 这种技术非常常见，但我认为通过引用返回结果不是特别优雅

  ```c++
  void f(const vector<double>& v, double* res);         // take input from v; place result in *res
  
  class F {
  public:
      F(const vector<double>& vv, double* p) :v{vv}, res{p} { }
      void operator()();                       // place result in *res
  private:
     const vector<double>& v;        // source of input
     double* res;                               // target for output
  };
  
  double g(const vector<double>&);  // use return value
   void user(vector<double>& vec1, vector<double> vec2, vector<double> vec3)
  {
      double res1;
      double res2;
      double res3;
  
      thread t1 {f,cref(vec1),&res1};    // f(vec1,&res1) executes in a separate thread
      thread t2 {F{vec2,&res2}};     // F{vec2,&res2}() executes in a separate thread
      thread t3 { [&](){ res3 = g(vec3); } };  // capture local variables by reference
  
      t1.join();   // join before using results
      t2.join();
      t3.join();
  
      cout << res1 << ' ' << res2 << ' ' << res3 << '\n';
  }
  ```




## thread

​		线程共享一个地址空间，这与进程不同，进程之间通常不直接共享数据。

​		线程共享一个地址空间，因此可以通过共享对象进行通信。这种通信通常由锁或其他机制控制，以防止数据竞争。

### thread

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

### jthread



### stopping thread

​		有时，我们想要停止一个线程，因为我们不再对它的结果感兴趣。仅仅“终止”它通常是不可接受的，因为线程可以拥有必须释放的资源(例如，锁、子线程和数据库连接)。相反，标准库提供了一种礼貌地请求线程清理并离开的机制:``stop_token``。可以对线程进行编程，使其在具有``stop_token`并被请求停止时终止。

```c++
// 并行算法find_any()，产生多个寻找结果的线程。
// 当一个线程返回一个结果时，停止剩余的线程。
// find_any()生成的每个线程都调用find()来完成实际的工作。find()是一个非常简单的任务类型的例程，通过一个主循环测试是否继续

atomic<int> result = -1;    // put a resulting index here

template<class T> struct Range { T* first; T* last; };        // a way of passing a range of Ts

void find(stop_token tok, const string* base, const Range<string> r, const string target)
{
    for (string* p = r.first; p!=r.last && !tok.stop_requested(); ++p)
        if (match(*p, target)) {     // match() applies some matching criteria to the two strings
            result = p - base;            // the index of the found element
            return;
        }
}

// 一个简单的find_any()，它只生成两个线程运行find():
void find_all(vector<string>& vs, const string& key)
{
    int mid = vs.size()/2;
    string* pvs = &vs[0];

    stop_source ss1{};
    jthread t1(find, ss1.get_token(), pvs, Range{pvs,pvs+mid}, key);                     // first half of vs

    stop_source ss2{};
    jthread t2(find, ss2.get_token(), pvs, Range{pvs+mid,pvs+vs.size()} , key);     // second half of vs

    while (result == -1)
        this_thread::sleep_for(10ms);

    ss1.request_stop(); // we have a result: stop all threads
    ss2.request_stop();

    // ... use result ...
}
```

​		这里，``tok.stop_requested()``测试是否有其他线程请求终止此线程。``stop_token``是安全(无数据竞争)通信这类跨线程请求的机制。

​		`stop_sources`生成``stop_token``，通过``stop_token``将停止请求传递给线程。

- 同步和返回结果是我能想到的最简单的方法：将结果放入原子变量中，并在其上进行自旋循环。
- 当然，可以将这个简单的示例细化为使用多个搜索线程，使返回的结果更通用，并使用不同的元素类型。



### thread pool



## future / promise

​		标准库提供了一些工具，允许程序员在任务(可能并发完成的工作)的概念级别上操作，而不是直接在线程和锁的较低级别上操作:

- `future`和``promise``用于从独立线程上生成的任务返回值
- `packaged_task`帮助启动任务并连接返回结果的机制
- `async()`用于启动任务，其方式与调用函数非常相似

​		这些设施可以在``<future>``中找到。

​		``future``和``promise``允许在两个任务之间传输值，无需显式使用锁。“系统”有效地实现了转移。基本思想很简单:当一个任务想要传递一个值给另一个任务时，它将该值放入一个``promise``中。以某种方式，实现使该值在相应的将来出现，可以从中读取(通常由任务的启动器)。我们可以用图形表示:

![image-20230522072720467](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230522072720467.png)

```c++
X v = fx.get();   // if necessary, wait for the value to get computed

// 如果值还没有准备好，执行get()的线程被阻塞，直到值准备好。
// 如果无法计算该值，get()可能会抛出异常(从系统或从promise传输)。

```

​		`promise`的主要目的是提供简单的``put``操作(称为``set_value()``和``set_exception()``)来匹配``future``的``get()``。`promise`需要将类型X的结果发送给``future``，有两宗情况:传递一个值或者传递一个异常。例如:

```c++
void f(promise<X>& px)   // a task: place the result in px
{
    // ...
    try {
        X res;
        // ... compute a value for res ...
        px.set_value(res);
    }
    catch (...) {                 // oops: couldn't compute res
        px.set_exception(current_exception());         // pass the exception to the future's thread
        		// Current_exception()引用捕获的异常
    }
}
```

​		要处理通过``future``传递的异常，``get()``的调用者必须准备好在某处捕获它。

```c++
void g(future<X>& fx)            // a task: get the result from fx
{
    // ...
    try {
        X v = fx.get();   // if necessary, wait for the value to get computed
        // ... use v ...
    }
    catch (...) {                // oops: someone couldn't compute v
        // ... handle error ...
    }
}

// 如果错误不需要由g()本身处理，则代码将减少到最小:
void g(future<X>& fx)               // a task: get the result from fx
{
    // ...
    X v = fx.get();   // if necessary, wait for the value to be computed
    // ... use v ...
} // 现在，从fx的函数(f())抛出的异常被隐式地传播给g()的调用者，就像g()直接调用f()一样。
```





​		task模型基于`future/promise`模型，将线程执行的对象抽象为颗粒化的任务。

​		使用并发的方式实现并行，并行调度的核心是根据task graph，使得多核的任务始终是互相独立无依赖的。

![image-20211204183728198](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20211204183728198.png)

​		一开始就将task关系图交给调配器进行统筹，省去执行一个task后再评估情况分配资源的开销。



### promise



### future

​		future异步的获取结果，可以多线程，也可以用协程等其他计算子。



### packaged_task

​		`packaged_task`类型简化线程上运行的`future`和`promise`连接的任务的设置。

​		`packaged_task`提供包装器代码，用于将任务的返回值或异常放入`promise`中。通过调用``get_future``来请求它，``packaged_task``会得到一个与`promise`相对应的`future`。

```c++
double accum(vector<double>::iterator beg, vector<double>::iterator end, double init)
        // compute the sum of [beg:end) starting with the initial value init
{
    return accumulate(&*beg,&*end,init);
}

double comp2(vector<double>& v)
{
    packaged_task pt0 {accum};                                                  // package the task (i.e., accum)
    packaged_task pt1 {accum};

    future<double> f0 {pt0.get_future()};                                    // get hold of pt0's future
    future<double> f1 {pt1.get_future()};                                    // get hold of pt1's future

    double* first = &v[0];
    thread t1 {move(pt0),first,first+v.size()/2,0};                          // start a thread for pt0
    thread t2 {move(pt1),first+v.size()/2,first+v.size(),0};           // start a thread for pt1
    // ...

    return f0.get()+f1.get();                                                            // get the results
}
```

​		`packaged_task`模板将任务类型作为模板参数(这里是``double(double*，double*，double)``)，并将任务作为其构造函数参数(这里是``accum``)。``move()``操作是必需的，因为``packaged_task``不能被复制。``packaged_task``不能被复制的原因是它是一个资源句柄:它拥有自己的``promise``，并且(间接地)对其任务可能拥有的任何资源负责。

​		这段代码中没有显式使用锁，专注于要完成的任务，而不是将精力放在管理任务通信的机制上。这两个任务将在单独的线程上运行，因此可能是并行的。



### async

​		使用``async()``启动异步任务，可能隐式的使用线程。

```c++
double comp4(vector<double>& v)
         // spawn many tasks if v is large enough
{
    if (v.size()<10' 000)                 // is it worth using concurrency?
        return accum(v.begin(),v.end(),0.0);

    auto v0 = &v[0];
    auto sz = v.size();

    auto f0 = async(accum,v0,v0+sz/4,0.0);                    // first quarter
    auto f1 = async(accum,v0+sz/4,v0+sz/2,0.0);           // second quarter
    auto f2 = async(accum,v0+sz/2,v0+sz*3/4,0.0);       // third quarter
    auto f3 = async(accum,v0+sz*3/4,v0+sz,0.0);          // fourth quarter

    return f0.get()+f1.get()+f2.get()+f3.get();  // collect and combine the results
}
```

​		``std::async()``将解耦了可调用部分和结果的获取，并将两者与任务的实际执行解耦。

​		使用``async()``不显式的使用线程和锁，但有一个明显的限制：不能在需要`lock`共享资源的任务上使用`async()`，即应该在并行任务上而不是并发任务上使用`async()`。

​		使用``async()``时，具体使用多少个线程取决于``async()``根据它对调用时可用系统资源的分配策略来决定。例如，``async()``可能在决定使用多少线程之前检查空闲的CPU核心数。

​		c++标准库提供了一系列并行算法，所以通常无须手动的进行标准库算法的并行化。

​		`async()`不仅仅是一种专门用于提高性能的并行计算的机制。还可以让一个单工作流的执行单元变成多工作流，比如用来生成一个获取用户输入的任务，而不阻塞其他的执行流。



## coroutine

​		协程是在调用之间保持其状态的函数。

​		在这一点上，它有点像函数对象，但是在调用之间保存和恢复其状态是隐式的和完整的。考虑一个经典的例子:

```c++
generator<long long> fib()         // generate Fibonacci numbers
{
    long long a = 0;
    long long b = 1;
    while (a<b) {
        auto next = a+b;
        co_yield next;           // save state, return value, and wait
        a = b;
        b = next;
    }
    co_return 0;                      // a fib too far
}

void user(int max)
{
    for (int i=0; i++<max;)
        cout << fib() <<' ';
}

// The Fibonacci example was obviously synchronous. This allows some nice optimizations. 
// 例如，一个好的优化器可以内联fib()调用并展开循环，只留下一系列<<的调用，这些调用本身可以被优化到 cout << "1 2 3 5 7 12";
```

​		生成器返回值是协程在调用之间存储其状态的地方。当然，我们可以用同样的方式创建一个函数对象Fib，但是我们必须自己维护它的状态。对于更大的状态和更复杂的计算，保存和恢复状态变得乏味、难以优化，而且容易出错。

​		实际上，协程是一个在调用之间保存其堆栈帧的函数。``co_yield``返回一个值并等待下一次调用。``co_return``返回一个值并终止协程。

​		协程可以是同步的(调用者等待结果)或异步的(调用者在从协程寻找到结果之前做一些其他工作)。

​		协程被实现为一个非常灵活的框架，能够服务于非常广泛的潜在用途。只是在c++ 20中仍然缺少简化使用的库功能。不过，还是有一些建议的，而且通过网络搜索可以找到很好的实现[Cppcoro]就是一个例子。

> [lewissbaker/cppcoro: A library of C++ coroutine abstractions for the coroutines TS (github.com)](https://github.com/lewissbaker/cppcoro)



### Cooperative Multitasking

​		在《计算机程序设计艺术》的第一卷中，Donald Knuth赞扬了协同程序的有用性，但也哀叹很难给出简短的例子，因为协同程序在简化复杂系统方面最有用。

​		一个简单的示例是演示事件驱动模拟所需的原语。其关键思想是将系统表示为协作完成复杂任务的简单任务(协同程序)网络。基本上，每个任务都是执行大工作的一小部分的参与者。有些是生成请求流的生成器(可能使用随机数生成器，可能提供真实世界的数据)，有些是网络计算结果的一部分，有些生成输出。

> BS：	我个人更喜欢任务(协程)通过消息队列进行通信。组织这种系统的一种方法是将每个任务置于事件队列中，在产生结果后等待更多的工作。然后调度程序在需要时从事件队列中选择要运行的下一个任务。这是合作多任务的一种形式。我借用了Simula [Dahl,1970]的关键思想，形成了第一个c++库的基础。

​		这种设计的关键是

- 许多不同的协程在调用之间保持它们的状态。
- 一种多态形式，允许保留包含不同类型协程的事件列表，并独立于其类型调用它们。
- 从列表中选择要运行的下一个协程的调度器。

​		在这里，我将展示几个协程，并交替执行它们。对于这样的系统来说，不使用太多的空间是至关重要的。这就是为什么我们不为这样的应用程序使用进程或线程的原因。一个线程需要一到两个兆字节(主要用于它的堆栈)，一个协程通常只需要几十个字节。如果你需要成千上万的任务，那就会有很大的不同。协程之间的上下文切换也比线程或进程之间快得多。

- 首先，我们需要一些运行时多态性，以允许我们统一调用数十或数百种不同类型的协程:

  ```c++
  struct Event_base {
         virtual void operator()() = 0;
         virtual ~Event_base() {}
  };  
  // Event简单地存储一个动作并允许调用它;
  // 这个动作通常是一个协程。添加一个名称只是为了说明一个事件通常携带比协程句柄更多的信息。
  template<class Act>
  struct Event : Event_base {
         Event(const string n, Act a) : name{ n }, act{ move(a) } {}
         string name;
         Act act;
          void operator()() override { act(); }
  };
  
  void test()
  {
      vector<Event_base*> events = {             // create a couple of Events holding coroutines
          new Event{ "integers ", sequencer(10) },
          new Event{ "chars ", char_seq('a') }
      };
  
      vector order {0, 1, 1, 0, 1, 0, 1, 0, 0};    // choose some order
  
      for (int x : order)                          // invoke coroutines in order
          (*events[x])();
  
      for (auto p : events)                     // clean up
          delete p;
  }
  ```

- 上述程序只是一个传统的面向对象框架，用于在一组可能不同类型的对象上执行操作

- ``sequence``和``char_seq``是协程。它们在调用之间保持状态，对于这些框架的实际使用是必不可少的:

  ```c++
  task sequencer(int start, int strp =1)
  {
      auto value = start;
      while (true) {
          cout << "value: " << value << '\n';     // communicate a result
          co_yield 0;                // sleep until someone resumes this coroutine
          value += step;                   // update state
      }
  } // sequencer是一个协程，因为它在调用之间使用co_yield挂起自己。这意味着任务必须是协程句柄。
  // 这是一个琐碎的协程。它所做的就是生成一系列值并输出它们。在严肃的模拟中，该输出将直接或间接地成为其他协同程序的输入。
  
  task char_seq(char start)
  {
      auto value = start;
      while (true) {
          cout << "value: " << value << '\n';    // communicate result
          co_yield 0;
          ++value;
      }
  } // char_seq非常相似，但使用不同的类型来执行运行时多态性:
  ```

  ​		“魔力”在于返回类型任务;它保存调用之间协程(实际上是函数的堆栈帧)的状态，并确定``co_yield``的含义。从用户的角度来看，``task``是微不足道的，它只是提供了一个操作符来调用协程:

  ```c++
  struct task {
      void operator()();
      // ... implementation details ...
  }
  ```

  ​		如果``task``已经在一个库中，最好是标准库中，那么我们需要知道的就只有这些了，但事实并非如此，所以这里有一个如何实现这种协程句柄类型的提示。不过，还是有一些建议的，而且通过网络搜索可以找到很好的实现;[Cppcoro]库就是一个例子。

  ```c++
  struct task {
      void operator()() { coro.resume(); }
  
      struct promise_type {                                                              // mapping to the language features
          suspend_always initial_suspend() { return {}; }
          suspend_always final_suspend() noexcept { return {}; }            // co_return
          suspend_always yield_value(int) { return {}; }                           // co_yield
          auto get_return_object() { return task{ handle_type::from_promise(*this) }; }
          void return_void() {}
          void unhandled_exception() { exit(1); }
      };
  
      using handle_type = coroutine_handle<promise_type>;
      task(handle_type h) : coro(h) { }         // called by get_return_object()
      handle_type coro;                               // here is the coroutine handle
  };
  ```

  ​		强烈建议不要自己编写这样的代码，除非用来实现一个库，试图为他人省去麻烦。如果对原理和实现有所期求，网上有很多解释以供查看。



## fiber

## executors

​		executors基于sender/receiver模型。

> p0443、p2300两个提案
>
> [浅谈The C++ Executors - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/395250667?utm_source=wechat_session&utm_medium=social&utm_oi=27088287432704&utm_campaign=shareopn)







# sharing data

​		任务可能共享数据。访问共享数据必须同步，以限制同时只有一个任务对共享数据有访问权限。

- 多个任务同时读取不可变的数据是没有问题的
- 但要考虑如何确保一次最多只有一个任务可以访问一组给定的对象

## mutex

​		互斥量，即“互斥对象”，是线程之间普遍共享数据的关键元素。

```c++
mutex m;           // controlling mutex
int sh;                // shared data

void f()
{
     scoped_lock lck {m};           // acquire mutex
     sh += 7;                       // manipulate shared data
}        // release mutex implicitly
```

​		`lck`的类型推断为``scoped_lock<mutex>``。``scoped_lock``的构造函数通过调用``m.lock()``获得互斥量。如果另一个线程已经获得了互斥量，则该线程会等待(“阻塞”)，直到另一个线程完成它的访问。一旦线程完成了对共享数据的访问，``scoped_lock``就会调用``m.l unlock()``释放互斥量。当互斥量被释放时，等待它的线程会继续执行(“被唤醒”)。互斥和锁机制可以在``<mutex>``中找到。

​		注意使用``RAII``。使用资源句柄，如``scoped_lock``和``unique_lock``，比显式地锁定和解锁互斥量更简单、更安全。

​		互斥量和要保护的共享数据之间没有语言上的强绑定关系，它依赖于逻辑上的约定。程序员必须知道哪个互斥量应该对应哪个数据。显然，这是容易出错的，同样可以通过各种语言手段来明确对应关系：

```c++
class Record {
public:
     mutex rm;
     // ...
};
```

​		毫无疑问，对于一个名为``rec``的记录，你应该先获取``rec.rm``，然后再访问``rec``的其余部分。

​		需要同时访问多个资源的操作并不少见，这种情况容易导致死锁。

- 如果``thread1``获取了``mutex1``，然后试图获取``mutex2``
- 而``thread2``获取了``mutex2``，然后试图获取``mutex1``
- 那么两个任务都不会继续进行下去

​		`scoped_lock`允许同时获得几个锁，``scoped_lock``只在获取所有的``mutexes``参数后才会进行,并且在持有互斥锁的同时,永远不会阻塞(“sleep”)。``scoped_lock``的析构器确保在线程离开范围时释放``mutexes``。

```c++
void f()
{
        scoped_lock lck {mutex1,mutex2,mutex3};      // acquire all three locks
        // ... manipulate shared data ...
} // implicitly release all mutexes
```

​		通过共享数据任务间通信是非常底层的机制。程序员必须深刻了解各种任务完成了哪些工作，没有完成哪些工作。在这方面，使用共享数据不如调用和返回的概念。

​		另一方面，有些人相信共享一定比复制参数和返回值更高效。当涉及大量数据时，确实可以这样做，但加锁和解锁是相对昂贵的操作。另一方面，现代机器非常擅长复制数据，特别是紧凑的数据，如向量元素。因此，不要因为“效率”而选择共享数据进行通信，需要仔细考虑定量测试。

​		基本的互斥量允许一个线程一次访问数据。共享数据最常见场景是在多个读取线程和一个写入线程之间共享数据。``shared_mutex``支持这种“读写锁”习语。读取者将获得“共享”的互斥量，以便其他读取者仍然可以访问，而写入者将要求独占访问。

```c++
shared_mutex mx;         // a mutex that can be shared

void reader()
{
    shared_lock lck {mx};              // willing to share access with other readers
    // ... read ...
}

void writer()
{
    unique_lock lck {mx};              // needs exclusive (unique) access
    // ... write ...
}
```



## atomic

​		互斥锁是一种涉及操作系统的重量级机制。它允许在没有数据竞争的情况下完成任意数量的工作。然而，有一种更简单、更便宜的机制可以完成少量的工作:原子变量。例如，下面是经典的双重检查锁定的一个简单变体:

```c++
mutex mut;
atomic<bool> init_x;        // initially false.
X x;                        // variable that requires nontrivial initialization

if (!init_x) {
    lock_guard lck {mut}; // lock_guard而不是scoped_lock，因为只需要一个互斥锁，所以最简单的锁(lock_guard)就足够了。
    if (!init_x) {
        // ... do nontrivial initialization of x ...
        init_x = true;
    }
}

// ... use x ...
```

​		在存在竞争的数据类型上使用原子类型可以避免使用更为昂贵的互斥锁。如果init_x不是原子的，那么初始化将小概率的失败，从而导致难以发现的神秘错误，因为init_x上存在数据竞争。

### condition variable

​		Waiting for Events。

​		有时，线程需要等待如另一个线程完成任务或经过一定的时间的某种外部事件。

​		最简单的“事件”就是时间的流逝。使用``<chrono>``中的时间工具，我可以这样写:

```c++
auto t0 = high_resolution_clock::now();
this_thread::sleep_for(milliseconds{20}); // 默认情况下，this_thread可以引用唯一的线程。
auto t1 = high_resolution_clock::now();

cout << duration_cast<nanoseconds>(t1-t0).count() << " nanoseconds passed\n";
```

​		使用外部事件进行通信的基本支持由``<condition_variable>``中的``condition_variables``提供。

​		``condition_variable``是允许线程等待另一个线程的机制。它允许线程等待某些条件(事件)作为其他线程完成工作的结果。

​		``condition_variables``支持多种形式的优雅而高效的共享，但可能相当棘手。如两个线程通过队列传递消息进行通信的经典示例：

```c++
class Message {        // object to be communicated
        // ...
};

queue<Message> mqueue;              // the queue of messages
condition_variable mcond;              // the variable communicating events
mutex mmutex;                                 // for synchronizing access to mcond

// consumer()读取和处理消息:
void consumer()
{
    while(true) {
        unique_lock lck {mmutex};                           // acquire mmutex
        mcond.wait(lck,[] { return !mqueue.empty(); });     // release mmutex and wait;
        // re-acquire mmutex upon wakeup
        // don't wake up unless mqueue is non-empty
        auto m = mqueue.front();                            // get the message
        mqueue.pop();
        lck.unlock();                                       // release mmutex
        // ... process m ...
    }
}

// 对应的生产者如下所示:
void producer()
{
    while(true) {
        Message m;
        // ... fill the message ...
        scoped_lock lck {mmutex};               // protect operations
        mqueue.push(m);
        mcond.notify_one();                     // notify
    }                                           // release mmutex (at end of scope)
}
```

​		显式地使用``unique_lock``来保护对``queue``和``condition_variable``的操作。等待一个``condition_variable``会释放``lock``，直到等待结束(队列非空)，然后重新获取它。``mqueue.empty()``是对条件的显式检查，防止在唤醒时发现其他任务“先到达那里”，从而使条件不再成立。

​		使用``unique_lock``而不是``scoped_lock``有两个原因:

- 需要将锁传递给``condition_variable``的``wait()``方法。
  - ``scoped_lock``不能移动，但``unique_lock``可以移动。
- 在处理消息之前，要解锁保护条件变量的互斥锁。
  - `unique_lock`提供了诸如``lock()``和``unlock()``之类的操作，用于低级控制同步。

## semaphore

## barrier

## 线程间通信

​		通过对全局变量、信号量、互斥量、条件变量等来实现线程间通知和消费消息。

​		还可以使用`promise`和`future`等新特性来进行通信。

> ​		服务器编程中，通过半同步半reactor的`event loop` + 队列来实现主线程与子线程的通信。
>
> ​		线程通信不需要特别的手段，需要的是线程同步。

