# log



## 日志原理

​		fwrite 配合以下结合，将日志消息写入日志文件。

- `fflush`: 内部触发 `write` 把用户缓冲区数据刷新到内核缓冲区中
  int fflush( FILE *stream )
- `fsync`: 把内核缓冲区数据刷新到磁盘
  int fsync(int fd);

```c
int fwrite(const void *buffer, size_t size, size_t count, FILE *stream );
void setbuf(FILE *stream, char *buf);
```

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-1d1685021daeb64a618246e28f0cdd93_1440w.webp)



## log4cpp

​		log4cpp 性能很低，可以在客户端使用，不适合服务器端使用。这里通过介绍 log4cpp 只是为了对通用日志框架做个简单的梳理，并与后面提到的 muduo 库进行对比。

​		![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-f623137671a9e95fd2f7a2353e8b2a2a_1440w.webp)

​		log4cpp 的组成如下：

- Priority: 日志级别
- Layout：设置日志的格式
- Appender：日志的目的地
- Category：根据优先级记录日志。

陈硕认为除了日志的级别外，其他功能都是非必需的。



### 日志级别

日志输出级别应当在运行时可调，在必要的时候可以临时在线调整日志的输出级别。对于 muduo 库来说，调整日志的输出级别不需要重新编译，也不需要重启进程，只需要调用 `muduo::Logger::setLoglevel()`就能即时生效。

### 日志格式

log4cpp 可以支持多种格式 Layout，如 SimpleLayout（简单布局）， BasicLayout（基本布局）、 PatternLayout（格式化布局）。而陈硕认为日志的格式在项目整个生命周期几乎不会改变，因为程序员需要经常 parse 日志。如果改变了日志格式，会增加很多无意义的工作量。因此，muduo 库默认日志的格式是固定的，不需要运行时配置，这样可以节省每条日志解析格式字符串的开销，如果需要调整消息格式，直接修改代码重新编译即可。其格式包括

```text
 日期 时间 微秒 线程 级别 正文 源文件名:行号
```

日志消息格式的要点如下

- 尽量每条日志占一行。这样很容易用 awk, sed, grep 等命令工具快速联机分析日志。
- 时间戳精确到微秒。每条消息通过 `gettimeofday(2)`获得当前时间，该函数不是系统调用，因此不存在性能损失。
- 始终使用 GMT 时区。对于分布式系统而言，不需要本地时区转换。
- 打印线程 id。便于分析多线程程序的时序，也可以检测死锁。
- 打印日志级别。在线查错的时候优先查找 `ERROR` 日志，加速定位问题。
- 打印源文件名和行号。定位位置。

### 日志输出

log4cpp 有多个不同的目的地 Appender，如控制台，本地文件，远程服务器等。陈硕认为对于分布式系统中的服务进程而言，日志的目的地只有一个：本地文件。一是若发生网络故障，会造成故障扩大；二是增加网络带宽的消耗。

### 日志回滚

日志回滚有两个条件：文件大小和时间。

log4cpp 回滚日志时，采用文件改名的方式。它同时支持5个日志备份文件，数字越大，文件越旧。通俗来说，先删除最旧的 log.5，然后 log.4 改名 log.5，log.3 改名 log.4, ......, log 改名 log.1。最后新建一个.log，即当前要写入的日志文件。

muduo 库的日志回滚没有采用文件改名，dmesg.log 始终是最新日志，便于编写及时解析日志的脚本

在性能方面，统计文件大小时，log4cpp 调用 `lseek`来实现，这导致其性能极低。而 muduo 库在应用层增加当前写入日志的大小参数，无疑是一种巧妙的做法。

### 单例模式实现

- 日志系统：log4cpp
- 编译指令：`g++ log4cpp.cc -llog4cpp -lpthread`

```cpp
 /* MyLogger.h */
 #ifndef __MYLOGGER_H__
 #define __MYLOGGER_H__
 #include <log4cpp/Category.hh>
 using namespace log4cpp;
 
 // 单例模式：构造和析构函数私有化
 class Mylogger {
 public:
     // 静态成员函数：返回类指针
     static Mylogger *getInstance();
     // 静态成员函数：类外释放对象
     static void destroy();
     void warn(const char * msg);
     void error(const char * msg);
     void debug(const char * msg);
     void info(const char * msg);
 
 private:
      Mylogger();
     ~Mylogger();
 private:
     // 静态成员：指向本类的指针
     static Mylogger *_pInstance;
     // Category对象的引用
     Category &_myCat; 
 };
 #endif
 /* MyLogger.cc */
 #include "MyLogger.h"
 #include <ilog4cpp/PatternLayout.hh>
 #include <log4cpp/FileAppender.hh>
 #include <log4cpp/OstreamAppender.hh>
 #include <log4cpp/Priority.hh>
 #include <iostream>
 using std::cout;
 using std::endl;
 using namespace log4cpp;
 
 // 类外初始化静态成员
 Mylogger *Mylogger::_pInstance = getInstance(); // 饿汉模式
 
 Mylogger::Mylogger()
 : _myCat(Category::getRoot().getInstance("mycat"))
 {
     cout << "Mylogger()" << endl;
     // 1、设置日志的格式: Layout
     // %c %d日期 %p优先级 %m消息 %n换行
     PatternLayout *ppl1 = new PatternLayout();
     ppl1->setConversionPattern("%d %c [%p] %m%n");
     PatternLayout *ppl2 = new PatternLayout();
     ppl2->setConversionPattern("%d %c [%p] %m%n");
 
     // 2、日志的目的地: Appender 参数: 名称(没用) + 流名字
     OstreamAppender *pos = new OstreamAppender("OstreamAppender", &cout);
     pos->setLayout(ppl1);
     RollingFileAppender *pfa = new RollingFileAppender("RollingFileAppender", name, 3 * 1024, 3);
     pfa->setLayout(ppl2);
     
     // 3、日志的种类
     _myCat.addAppender(pos);
     _myCat.addAppender(pfl);
     _myCat.setPriority(Priority::DEBUG);
 }
 Mylogger::~Mylogger() {
     cout << "~Mylogger()" << endl;
     // 回收资源
     Category::shutdown();
 }
 
 Mylogger *Mylogger::getInstance() {
     if(nullptr == _pInstance) {
         _pInstance = new Mylogger();
     }
     return _pInstance;
 }
 
 void Mylogger::destroy() {
     if(_pInstance) {
         delete _pInstance;
         _pInstance = nullptr;
     }
 }
 
 void Mylogger::warn(const char * msg) {
     _myCat.warn(msg);
 }
 
 void Mylogger::error(const char * msg) {
     _myCat.error(msg);
 }
 
 void Mylogger::debug(const char * msg) {
     _myCat.debug(msg);
 }
 
 void Mylogger::info(const char * msg) {
     _myCat.info(msg);
 }
 /* MyLoggertest.cc */
 #include "MyLogger.h"
 #include <iostream>
 #include <string>
 using std::cout;
 using std::endl;
 using std::string;
 
 #define prefix(msg) (string(__FILE__) + string("  ") \
         + string(__FUNCTION__) + string("  ") \
         + string (std::to_string(__LINE__)) \
         + string("  ") + msg ).c_str()
 
 #define LogError(msg) Mylogger::getInstance()->error(prefix(msg))
 #define LogInfo(msg) Mylogger::getInstance()->info(prefix(msg))
 #define LogWarn(msg) Mylogger::getInstance()->warn(prefix(msg))
 #define LogDebug(msg) Mylogger::getInstance()->debug(prefix(msg))
 
 string func(const string &msg) {
     string s1 = string(__FILE__) + string("  ") 
         + string(__FUNCTION__) + string("  ")
         + string (std::to_string(__LINE__)) 
         + string("  ") + msg;
 
     return s1;
 }
 
 void test() {
     Mylogger *pml = Mylogger::getInstance();
 
     logInfo("The log is info message");  
     logError("The log is error message");
     logWarn("The log is warn message");
     logDebug("The log is debug message");
 }
 
 int main(int argc, char **argv)
 {
     test();
     return 0;
 }
```



## muduo 异步日志库

如何实现高性能的日志

- 批量写入：攒够数据，一次写入，glog / muduo
- 唤醒机制：通知唤醒 `notify` + 超时唤醒 `wait_timeout`
- 锁的粒度：刷新磁盘时，日志接口不会阻塞。这是通过双队列实现的，前台队列实现日志接口，后台队列实现刷新磁盘。
- 内存分配：移动语义，避免深拷贝；双缓冲，前台后台都设有。

### 异步日志机制

​		异步日志，用一个线程负责收集日志消息，并写入日志文件。其他业务线程只管往这个日志线程发送日志消息。

​		整个框架如图所示，该框架有一个日志缓冲队列来将日志前端的数据发送到后端（日志线程），这是一个典型的生产者与消费者模型。生产者前端不是将日志消息逐条分别传送给后端，而是将多条日志信息缓存拼成1个大的 buffer 发送给后端。同时，对于消费者后端线程来说，并不是每条日志消息写入都将其唤醒，而是当前端写满1个 buffer 的时候，才唤醒后端日志线程批量写入磁盘文件。

​		因此，后端日志线程的唤醒有两个条件

- buffer 写满唤醒：批量写入写满1个 buffer 后，唤醒后端日志线程，减少线程被唤醒的频率，降低系统开销。
- 超时被唤醒：为了及时将日志消息写入文件，防止系统故障导致内存中的日志消息丢失，超过规定的时间阈值，即使 buffer 未满，也会立即将 buffer 中的数据写入。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-10762f49ca12648e287023fd60678d27_1440w.webp)



### 双缓冲机制

​		muduo 库采用的是双缓冲机制（Google C++日志库也是如此）。其基本思路是准备两个缓冲区，bufferA 和 bufferB。前端负责向 A 中写入日志消息，后端负责将 B 中的数据写入文件。当 bufferA 写满后，交换 A 和 B，此时让后端将 A 的数据写入文件，前端向 B 中写入新的日志消息，如此往复。这样在追加日志消息的时候不必等待磁盘 IO 操作，同时也避免了每条新日志消息都触发唤醒后端日志线程。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-b12d975c6a31ac1fe309077f8664fa9d_1440w.webp)

在源码 AsyncLogging.h 中，缓冲区 A 和 B，实际上采用的是缓冲区队列，分别对应前台日志缓冲队列 buffers 和后台日志缓冲队列 buffersToWrite。交换 A 和 B 时，只需要交换其指针的指向即可。

另外，采用队列可以提升缓冲区的容错性。考虑下面这种情况：前端写满 buffer，触发交换，后端还未将数据完全写入磁盘，此时前端又写满交换后的 buffer 了，再次触发交换机制，无 buffer 可用，造成阻塞。



### 前端日志写入

​		`AsyncLogging::append()`：前端生成一条日志消息。

​		前端准备一个前台缓冲区队列 `buffers_`和两个 buffer。前台缓冲队列 `buffers_`用来存放积攒的日志消息。两个 buffer，一个是当前缓冲区 `currentBuffer`，追加的日志消息存放于此；另一个作为当前缓冲区的备份，即预备缓冲区 `nextBuffer`，减少内存的开销。

​		函数执行逻辑如下：

判断当前缓冲区 `currentBuffer_`是否已经写满

- 若当前缓冲区未满，追加日志消息到当前缓冲，这是最常见的情况
- 若当前缓冲区写满，首先，把它移入前台缓冲队列 `buffers_`。其次，尝试把预备缓冲区 `nextBuffer_`移用为当前缓冲，若失败则创建新的缓冲区作为当前缓冲。最后，追加日志消息并唤醒后端日志线程开始写入日志数据。

```cpp
 void AsyncLogging::append(const char* logline, int len) {
   // 多线程加锁，线程安全
   MutexLockGuard lock(mutex_);
 
   // 判断当前缓冲是否已经写满，批量数据的积攒
   // 1、当前缓冲未满，还能写入数据
   if (currentBuffer_->avail() > len) {
     // 追加日消息到当前缓冲
     currentBuffer_->append(logline, len); 
   }
 
   // 2、当前缓冲写满，两件事
   // 其一，将写满的当前缓冲的日志消息写入前台缓冲队列 buffers
   // 其二，追加日志消息到当前缓冲，唤醒后台日志落盘线程
   else {
     // 其一、当前缓冲移入(move)前台缓冲队列 buffers
     buffers_.push_back(std::move(currentBuffer_));
 
     // 判断预备缓冲是否写满
     // 2.1、预备缓冲未满，复用该缓冲区
     if (nextBuffer_) {
       // 预备缓冲移用为当前缓冲
       currentBuffer_ = std::move(nextBuffer_); 
     }
     // 2.2、预备缓冲写满（极少发生），重新分配缓冲
     // 原因：前端写入速度太快，一下子把两块缓冲都用完了
     else {
       // 重新分配 buffer，用作当前缓冲
       currentBuffer_.reset(new Buffer); 
     }
     
     // 其二、追加日志信息到当前缓冲，唤醒日志落盘线程
     currentBuffer_->append(logline, len);    
     cond_.notify(); 
   }
 }
```

### 后端日志落盘

​		`AsyncLogging::threadFunc()`：后端日志落盘线程的执行函数。

​		后端同样也准备了一个后台缓冲区队列 `buffersToWrite` 和两个备用 buffer。后台缓冲区队列 `buffersToWrite` 存放待写入磁盘的数据。两个备用 buffer，`newBuffer1`和`newBuffer2`，分别用来替换前台的当前缓冲和预备缓冲，而这两个备用 buffer 最后会被`buffersToWrite`内的两个 buffer 重新填充，减少了内存的开销。

​		函数执行逻辑如下，注意思考如何锁的粒度如何减小，起到了什么作用。

- 唤醒日志落盘线程（超时或写满buffer），交换前台缓冲队列和后台缓冲队列。加锁
- 日志落盘，将后台缓冲队列的所有 buffer 写入文件。不加锁，这样做的好处是日志落盘不影响前台缓冲队列的插入，不会出现阻塞问题，极大提升了系统性能。

​		接下来，用图表示前端和后端的具体交互情况，注意配合源码认真分析。

​		一开始，分配好四个缓冲区，前端和后端各持有其中的两个。同时，前端和后端各有一个缓冲队列，初始时都是空的。结合源码分析以下过程：

​		case 1: 超时唤醒后端线程将当前缓冲区写入文件，此时前端写日志的频率不高。

![img](https://pic2.zhimg.com/80/v2-fa5d82320ff077979b9a17f3246031a5_1440w.webp)

​		case1: 超时唤醒

超时唤醒后端线程，先把 `currentBuffer_` 送入 `buffers_`，再把 `newbuffer1` 移用为 `currentBuffer_`。随后，交换 `buffers_`和 `buffersToWrite`。离开临界区，后端开始将 `buffersToWrite`中的 bufferA 写入文件。写完后（write done）再重新填充 `newbuffer1`，等待下一次 `cond_.waitForSeconds()` 返回。

​		case 2：超时前写满当前缓冲唤醒后端线程写入文件

![img](https://pic4.zhimg.com/80/v2-630ae9e51d985ccdff985a4f81210403_1440w.webp)

case2: 通知唤醒：缓冲区够用

写满`currentBuffer_`唤醒后端线程，把`currentBuffer_`送入`buffers_`，再把`newbuffer1`移用为`currentBuffer_`。随后，交换 `buffers_`和 `buffersToWrite`，最后用`newbuffer2`替换`nextBuffer_`，始终保证前端有两个空缓冲可用。离开临界区，后端开始将`buffersToWrite`中的 bufferA 和 bufferB 写入文件。写完后再重新填充 `newbuffer1`和`newbuffer2`，等待下一次 `cond_.waitForSeconds()` 返回。

上述这两种情况是最常见的。

case3: 用完了两个缓冲，需要重新分配新 buffer。可能原因：前端短时间内密集写入日志，或者后端文件写入速度较慢，导致前端耗尽了两个缓冲，并分配了新缓冲。

![img](https://pic3.zhimg.com/80/v2-66fab597a2efd22f505969711dba58c2_1440w.webp)

case3: 通知唤醒：缓冲区耗尽

写满`currentBuffer_`唤醒后端线程，但出于种种原因，后端线程并没有立刻开始工作，接下来预备缓冲`nextBuffer_`也写满了，前端线程新分配了缓冲区 E。当后端线程终于获取控制权后，将缓冲 C、D交给前端，并开始将缓冲 A, B, E依次写入文件。写完后再用缓冲A、B重新填充两块空闲缓冲，从而释放了缓冲E。

```cpp
 void AsyncLogging::threadFunc() {
   assert(running_ == true);
   latch_.countDown();
   // logFile 类负责将数据写入磁盘
   LogFile output(basename_, rollSize_, false);
 
   BufferPtr newBuffer1(new Buffer); // 用于替换前台的当前缓冲 currentbuffer
   BufferPtr newBuffer2(new Buffer); // 用于替换前台的预备缓冲 nextbuffer 
   newBuffer1->bzero();  
   newBuffer2->bzero();
 
   BufferVector buffersToWrite;  // 后台缓冲队列
   buffersToWrite.reserve(16);   // 两个不同的缓冲队列，涉及到锁的粒度问题
   
   // 异步日志开启，则循环执行
   while (running_) {
     assert(newBuffer1 && newBuffer1->length() == 0);
     assert(newBuffer2 && newBuffer2->length() == 0);
     assert(buffersToWrite.empty());
 
     // <---------- 交换前台缓冲队列和后台缓冲队列 ---------->
     { // 锁的作用域，放在外面，锁的粒度就大了，日志落盘的时候都会阻塞 append
       // 1、多线程加锁，线程安全，注意锁的作用域
       MutexLockGuard lock(mutex_);
 
       // 2、判断前台缓冲队列 buffers 是否有数据可读
       // buffers 没有数据可读，休眠
       if (buffers_.empty()) {
         // 触发日志的落盘 (唤醒) 的两个条件：1.超时 or 2.被唤醒，即前台写满 buffer
         cond_.waitForSeconds(flushInterval_); // 内部封装 pthread_cond_timedwait
       }
 
       // 只要触发日志落盘，不管当前的 buffer 是否写满都必须取出来，写入磁盘
       // 3、将当前缓冲区 currentbuffer 移入前台缓冲队列 buffers。
       // currentbuffer 被锁住 -> currentBuffer 被置空  
       buffers_.push_back(std::move(currentBuffer_)); 
       
       // 4、将空闲的 newbuffer1 移为当前缓冲，复用已经分配的空间
       currentBuffer_ = std::move(newBuffer1); // currentbuffer 需要内存空间
 
       // 5、核心：把前台缓冲队列的所有buffer交换（互相转移）到后台缓冲队列 
       // 这样在后续的日志落盘过程中不影响前台缓冲队列的插入
       buffersToWrite.swap(buffers_);      
 
       // 若预备缓冲为空，则将空闲的 newbuffer2 移为预备缓冲，复用已经分配的空间
       // 这样前台始终有一个预备缓冲可供调配
       if (!nextBuffer_) { 
         nextBuffer_ = std::move(newBuffer2);  
       }
     } // 注意这里加锁的粒度，日志落盘的时候不需要加锁了，主要是双队列的功劳
 
     // <-------- 日志落盘，将buffersToWrite中的所有buffer写入文件 -------->
     assert(!buffersToWrite.empty());
 
     // 6、异步日志消息堆积的处理。
     // 同步日志，阻塞io，不存在堆积问题；异步日志，直接删除多余的日志，并插入提示信息
     if (buffersToWrite.size() > 25) {
       printf("Dropped\n");
       // 插入提示信息
       char buf[256];
       snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
                Timestamp::now().toFormattedString().c_str(),
                buffersToWrite.size()-2);    
       fputs(buf, stderr);
       output.append(buf, static_cast<int>(strlen(buf)));
       // 只保留2个buffer(默认4M)
       buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());   
     }
 
     // 7、循环写入 buffersToWrite 的所有 buffer
     for (const auto& buffer : buffersToWrite) {
       // 内部封装 fwrite，将 buffer中的一行日志数据，写入用户缓冲区，等待写入文件
       output.append(buffer->data(), buffer->length());  
     }
 
     // 8、刷新数据到磁盘文件？这里应该保证数据落到磁盘，但事实上并没有，需要改进 fsync
     // 内部调用flush，只能将数据刷新到内核缓冲区，不能保证数据落到磁盘（断电问题）
     output.flush();   
    
     // 9、重新填充 newBuffer1 和 newBuffer2
     // 改变后台缓冲队列的大小，始终只保存两个 buffer，多余的 buffer 被释放
     // 为什么不直接保存到当前和预备缓冲？这是因为加锁的粒度，二者需要加锁操作
     if (buffersToWrite.size() > 2) {
        // 只保留2个buffer，分别用于填充备用缓冲 newBuffer1 和 newBuffer2
       buffersToWrite.resize(2);  
     }
     // 用 buffersToWrite 内的 buffer 重新填充 newBuffer1
     if (!newBuffer1) {
       assert(!buffersToWrite.empty());
       newBuffer1 = std::move(buffersToWrite.back()); // 复用 buffer
       buffersToWrite.pop_back();
       newBuffer1->reset();    // 重置指针，置空
     }
     // 用 buffersToWrite 内的 buffer 重新填充 newBuffer2
     if (!newBuffer2) {
       assert(!buffersToWrite.empty());
       newBuffer2 = std::move(buffersToWrite.back()); // 复用 buffer
       buffersToWrite.pop_back();
       newBuffer2->reset();   // 重置指针，置空
     }
 
     // 清空 buffersToWrite
     buffersToWrite.clear();  
   }
   
   // 存在问题
   output.flush();
 }
```



###  coredump 查找未落盘的日志

[muduo异步日志——core dump查找未落盘的日志](https://link.zhihu.com/?target=https%3A//www.yuque.com/docs/share/b29be8e3-765c-4937-891e-a2d6663b381d%3F)

通过异步日志的实现可以知道，日志消息并不是生成后立刻就会写入文件，而是先存放在前台缓冲区 `currentbuffer` 或者前台缓冲区队列 `buffers`中。每过一段时间后才会将缓冲区中的日志消息写到日志文件中。这样就会产生问题：如果程序在中途 core dump 了，那么在缓冲区中还未来得及写出的日志消息该如何找回呢？

现在，构造场景：主线程开启日志线程，写入100w条日志，当写到第50w条时人为往空指针写数据制造异常退出，引发 core dump。

```c
 void testCoredump() {
     AsyncLogging log("coredump", 200*1000*1000);
     log.start();   //开启日志线程
     g_asyncLog = &log;
     int msgcnt = 0;
     Logger::setOutput(asyncOutput); //设置日志输出函数
  
     // 写入100万条日志消息
     for(int i = 0; i < 1000000; ++i)   {
       LOG_INFO << "testCoredump" << ++msgcnt;
       if(i == 500000) {
         int *ptr = NULL;
         *ptr = 0x1234;  // 人为制造异常
       }
     } 
 }
```

### 3.5.1、gdb 升级

```bash
 # 编译 gdb 过程中需要 texinfo，安装 texinfo
 sudo apt-get install texinfo
 
 # 下载gdb11
 wget http://ftp.gnu.org/gnu/gdb/gdb-11.1.tar.gz
 tar -zxvf gdb-11.1.tar.gz
 # 编译
 cd gdb-11.1 
 ./configure 
 make 
 sudo make install
 
 # 将 gdb 放到 bin 目录
 mv /usr/bin/gdb /usr/bin/gdb_bak
 cp /opt/gdb-11.1/gdb /usr/bin/gdb
```

### 3.5.2、生成 core

寻找还未写出的日志消息，需要用到 coredump 生成的 core 文件

```bash
 # 查看 core 文件是否开启，默认不生成，0
 ulimit -c
 # 开启 core 文件生成，unlimited指的是core文件的最大大小，可以设置为其它数字
 ulimit -c unlimited
```

运行程序，生成了 core 文件和一个 .log 日志文件。使用 `tail -f`命令查看到日志文件中有 460132 条日志消息，其余日志消息未来得及写出。下面通过 core 文件查找剩下的日志消息。

### 3.5.3、gdb 调试 core 文件

使用 gdb 执行 coredump 文件。`gdb [execfile] [corefile]`

```bash
 gdb main_log_test core
```

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-762df119d38e0972a0c70d7812d5e6cf_1440w.webp)

gdb main_log_test core

通过 gdb 信息可以看到，`Program terminated with signal SIGSEGV, Segmentation fault`。`LWP` 是线程的标识，这里当前线程的 LWP 为4614，共有两个线程：LWP 4614 和 4615。崩溃是在主线程。

用 `thread info` 查看线程信息，用 `thread id` 切换线程栈。

使用 `thread 2`切换查看后端日志线程，可以看到线程2位于 pthread_cond_timewait

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-7dec857ec1cdf904ce48af16fb010832_1440w.webp)

thread 2

使用 `bt (backtrace)` 查看线程的堆栈信息。`frame id` 切换栈空间。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-699f1ba916a9228864c445d0f84cc4b3_1440w.webp)

frame 2

此时所在的环境，就相当于是 append 函数的栈帧未写出的日志消息，只可能存在于 `currentBuffer_`或`buffers_`中。可以通过`currentBuffer.get()`获取该 unique_ptr 所指向的LogStream 。可以用 print 打印。由于打印信息数量受到 FixedBuffer 的 max-value-size 限制，所以需要先设置 max-value-size 为无限大

```bash
 set max-value-size unlimited
 print *currentBuffer_.get()
```

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-f3385647320d50f82fd9097c682da9dd_1440w.webp)

print *currentBuffer_.get()

可以看到显示了一部分日志信息，还是有很多被省略了，因为 gdb 的终端输出长度有限制，默认为200个字符，可以修改这个限制。这样可以在屏幕上全部显示。

```bash
 show print elements # 可以看到限制200个字符
 set print elements unlimited
 show print elements # 修改成 unlimited
```

将终端上的所有打印的信息输出到指定文件

```bash
 set logging file gdbinfo.txt # 指定日志输出文件
 set logging on   # 开启日志拷贝
 set logging off  # 关闭日志拷贝
```

接下来，`print *currentBuffer_.get()`，一直按回车，打印 buffer 所有的数据，所有终端上的打印信息都会拷贝到 gdbInfo.txt 中。

vim 打开 gdbInfo.txt，所有数据都被当作了一行，这是因为在拷贝时，将 '\n' 作为了两个普通的字符而不是换行符。在 vim 中将其查找替换即可，在命令模式下输入：`%s/\\n/\r/g`，回车

![img](https://pic4.zhimg.com/80/v2-29840817a77b2e83a9a1ca65454e5907_1440w.webp)

可以查找到后台日志线程写入的第50w条日志。