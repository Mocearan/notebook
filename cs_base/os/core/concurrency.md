



​		在系统编程中，互斥量、条件变量和信号量是用来实现多线程同步的重要工具。它们可以帮助程序员控制线程之间的访问顺序和共享资源的安全性。

1. 互斥量（Mutex）是一种用于保护共享资源的同步原语。互斥量可以确保在同一时刻只有一个线程可以访问共享资源，防止多个线程同时修改数据而导致数据不一致的问题。在系统接口层面，可以使用pthread库中的pthread_mutex_init、pthread_mutex_lock和pthread_mutex_unlock等函数来创建和操作互斥量。在C++标准中，可以使用std::mutex类来实现互斥量。

示例：使用互斥量保护一个共享变量count，确保多个线程对count的访问是安全的。

```c
#include <pthread.h>
#include <iostream>

int count = 0;
pthread_mutex_t mutex;

void* increment(void* arg) {
    pthread_mutex_lock(&mutex);
    count++;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    std::cout << "Final count: " << count << std::endl;
    pthread_mutex_destroy(&mutex);
    return 0;
}
```

1. 条件变量（Condition Variable）是一种用于线程间通信的同步原语。条件变量可以让线程在等待某个条件成立时进入阻塞状态，并在条件满足时唤醒线程继续执行。在系统接口层面，可以使用pthread库中的pthread_cond_init、pthread_cond_wait和pthread_cond_signal等函数来创建和操作条件变量。在C++标准中，可以使用std::condition_variable类来实现条件变量。

示例：使用条件变量和互斥量实现一个生产者-消费者模型。

```c
#include <pthread.h>
#include <iostream>

int buffer;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        buffer = i;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer == -1) {
            pthread_cond_wait(&cond, &mutex);
        }
        std::cout << "Consumed: " << buffer << std::endl;
        buffer = -1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    buffer = -1;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t producerThread, consumerThread;
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
```

1. 信号量（Semaphore）是一种用于控制资源访问的同步原语。信号量可以限制同时访问某个资源的线程数量，防止资源的过度使用。在系统接口层面，可以使用pthread库中的sem_init、sem_wait和sem_post等函数来创建和操作信号量。在C++标准中，可以使用std::semaphore类来实现信号量。

示例：使用信号量控制线程的并发数量。

```c
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

sem_t semaphore;

void* worker(void* arg) {
    sem_wait(&semaphore);
    std::cout << "Thread started" << std::endl;
    // Do some work
    std::cout << "Thread finished" << std::endl;
    sem_post(&semaphore);
    return NULL;
}

int main() {
    sem_init(&semaphore, 0, 2); // Allow 2 threads to run concurrently
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, worker, NULL);
    pthread_create(&thread2, NULL, worker, NULL);
    pthread_create(&thread3, NULL, worker, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    sem_destroy(&semaphore);
    return 0;
}
```

总结：互斥量、条件变量和信号量是系统编程中常用的同步工具，通过它们可以实现线程之间的协作和资源的安全访问。在不同的编程语言和库中，可以使用相应的接口来操作这些同步原语，帮助开发者编写高效且可靠的多线程程序。



互斥量（Mutex）、条件变量（Condition Variable）和信号量（Semaphore）是多线程编程中常用的同步原语，它们各自有不同的作用和特点。下面是它们之间的区别：

1. 互斥量（Mutex）：
   - 作用：用于实现临界区（Critical Section），确保同一时间只有一个线程可以访问共享资源，从而避免竞态条件。
   - 特点：只能被一个线程锁定，其他线程必须等待解锁后才能继续执行。
   - 使用场景：适用于需要保护临界区的情况，确保共享资源的互斥访问。
2. 条件变量（Condition Variable）：
   - 作用：用于线程间的条件等待和通知，当某个条件不满足时，线程可以等待条件变量的通知。
   - 特点：配合互斥量使用，等待条件变量时会释放互斥量，收到通知后重新获取互斥量。
   - 使用场景：适用于线程间的协作，等待特定条件的发生或通知。
3. 信号量（Semaphore）：
   - 作用：用于控制同时访问某个资源的线程数量，限制并发访问。
   - 特点：可以设置初始值，通过sem_wait和sem_post等操作来控制信号量的值。
   - 使用场景：适用于资源管理和限制并发访问的情况，控制线程的并发数量。

总的来说，互斥量用于保护共享资源的互斥访问，条件变量用于线程间的条件等待和通知，信号量用于控制并发访问的数量。在实际编程中，根据不同的需求和场景选择合适的同步原语来确保线程间的正确同步和协作。



（1）使用条件变量可以一次唤醒所有等待者，而这个信号量没有的功能，感觉是最大区别。

（2）信号量是有一个值（状态的），而条件变量是没有的，没有地方记录唤醒（发送信号）过多少次，也没有地方记录唤醒线程（wait返回）过多少次。从实现上来说一个信号量可以是用mutex + counter + condition variable实现的。因为信号量有一个状态，如果想精准的同步，那么信号量可能会有特殊的地方。信号量可以解决条件变量中存在的唤醒丢失问题。

（3）在Posix.1基本原理一文声称，有了互斥锁和条件变量还提供信号量的原因是：“本标准提供信号量的而主要目的是提供一种进程间同步的方式；这些进程可能共享也可能不共享内存区。互斥锁和条件变量是作为线程间的同步机制说明的；这些线程总是共享(某个)内存区。这两者都是已广泛使用了多年的同步方式。每组原语都特别适合于特定的问题”。尽管信号量的意图在于进程间同步，互斥锁和条件变量的意图在于线程间同步，但是信号量也可用于线程间，互斥锁和条件变量也可用于进程间。应当根据实际的情况进行决定。信号量最有用的场景是用以指明可用资源的数量



在计算机科学中，原语（primitive）指的是一种基本的操作或指令，它们是不可再分解的最小单位。原语通常是由硬件或操作系统提供的，用于执行特定的操作或实现特定的功能。在多线程编程中，原语通常用于实现原子操作（atomic operation），保证某些操作的不可分割性，从而确保线程间的正确同步。

原语的特点包括：

1. 原子性（Atomicity）：原语是不可分割的操作，要么完全执行，要么完全不执行，不会被中断或分割成多个步骤。
2. 独立性（Indivisibility）：原语在执行过程中是不可被中断或打断的，不会被其他操作或线程干扰。
3. 原语是高效的：原语通常是底层的硬件指令或操作系统提供的原生支持，执行效率高。

在多线程编程中，常用的原语包括互斥量、条件变量和信号量等，它们用于实现线程间的同步和协作。通过使用原语，可以确保线程之间的操作是原子的，避免出现竞态条件（Race Condition）等问题。





# concurrency



## 互斥量

​		互斥量采用的是英文mutual exclusive(互相排斥之意)的缩写，即`mutex`，是多线程编程中，常用来进行同步访问的方式之一。

```c
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock (pthread_mutex_t * mutex, const struct timespec *abstime);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

int pthread_mutexattr_init (pthread_mutexattr_t *attr); // 初始化互斥量属性为默认属性
int pthread_mutexattr_destroy (pthread_mutexattr_t *attr);// 销毁互斥量属性
/* Get the process-shared flag of the mutex attribute ATTR.  */
int pthread_mutexattr_getpshared (const pthread_mutexattr_t * attr,int *pshared);
/* Set the process-shared flag of the mutex attribute ATTR.  */
int pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, int shared);

// attr 中 pshared 属性表示用这个属性对象创建的互斥锁的作用域，它的取值可以是 PTHREAD_PROCESS_PRIVATE或PTHREAD_PROCESS_SHARED。
//	PTHREAD_PROCESS_PRIVATE：默认属性，只有和创建这个互斥锁的线程在同一个进程中的线程才能访问这个互斥锁；
//	PTHREAD_PROCESS_SHARED：所创建的互斥锁将被保存在共享内存中，可以被多个进程中的线程共享。

//互斥锁类型：
//	PTHREAD_MUTEX_NORMAL；
//	PTHREAD_MUTEX_ERRORCHECK；
//	PTHREAD_MUTEX_RECURSIVE；
//	PTHREAD_MUTEX_DEFAULT。
```



## Futex

[Futex系统调用，Futex机制，及具体案例分析[通俗易懂\]-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/2154718?areaSource=102001.17&traceId=lx2X6DVcrkyUqAI64Kxro)

[深入剖析C/C++在Linux内核下涉及到线程锁知识，以开源OceanBase项目实践学习-下篇-Futex详解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/668391649)

[深入探秘高性能并发：C++如何在Linux巧妙应用Futex实现线程锁同步（ob_latch.cpp篇）大篇幅（3万字） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/672165465)

[一文搞懂Linux线程同步原理 - 哔哩哔哩 (bilibili.com)](https://www.bilibili.com/read/cv28551350/)

http://www.360doc.com/content/22/0211/17/39821762_1016884736.shtml

## 条件变量

​		条件变量(condition variable)是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待某个条件为真，而将自己挂起；另一个线程使的条件成立，并通知等待的线程继续。为了防止竞争，条件变量的使用总是和一个互斥锁]结合在一起。

​		如果不使用条件变量，线程就需要 **轮询+休眠** 来查看是否满足条件，这样严重影响效率。

​		条件变量以“自定义条件”来判定是否阻塞（优点来了），那么它就可以唤醒一个线程或所有线程（notify_all类似广播）。

​		先解锁传入的互斥量，再让条件变量等待，从而使所在线程处于阻塞状态。阻塞等待结束时，系统会确保该线程再次持有互斥量(加锁)。Linux内核会为每个条件变量维护一个等待队列，调用了 pthread_cond_wait 或 pthread_cond_timedwait 的线程会按照调用时间先后添加到该队列中。

- pthread_cond_signal会唤醒该队列的第一个。
- pthread_cond_broadcast被唤醒的所有线程都会去争夺互斥锁，没抢到的线程会继续等待，拿到锁后同样会从条件等待函数返回。所以，被唤醒的线程第一件事就是再次判断条件是否满足！

​		同一个互斥量上可能有不同的条件变量，直白的说，条件变量就是一个互斥量上等待的不同条件。

```c
#include <pthread.h>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
pthread_cond_timedwait(cond, mutex, abstime);
pthread_cond_wait(cond, mutex);
pthread_cond_signal(cond);// 将至少解锁一个线程（阻塞在条件变量上的线程）。
pthread_cond_broadcast(cond);//将对所有阻塞在条件变量上的线程解锁。
```

​		

```c++

template <typename T>
class BlockQue {
public:
	BlockQue():m_end(false){}
	~BlockQue(){
		endBlockQue();
	}
    void addTask(T data){
		m_tasks.push(data);
		m_task_cv.notify_all();
	}
	void clearTask(bool del){
		//CAutoLock lock(m_mutexClr);
		while(!m_tasks.empty()){
			T data = std::move(m_tasks.front());
			m_tasks.pop();
			if(del){
				delete data;
			}
		}
	} 
	bool isEmpty(){
		return m_tasks.empty();
	}
	int size(){
		return m_tasks.size();
	}
    int getTask(T& data){
		//CAutoLock lock(m_mutexClr);
		std::unique_lock<std::mutex> lock{ m_lock };
		m_task_cv.wait(lock, [this]{
			return m_end.load() || !m_tasks.empty();//true则不阻塞
		});
		if (m_tasks.empty()){
			return -1;
		}
		data = std::move(m_tasks.front());
		m_tasks.pop();
		return 0;
	}
 
	void initBlockQue() {
		m_end.store(false);
	}
	void endBlockQue(){
		if (!m_end.load()){
			m_end.store(true);
			m_task_cv.notify_all();
		}
	}
private:
	std::atomic<bool> 		m_end;
    std::condition_variable m_task_cv;
	std::queue<T> 	m_tasks;
	std::mutex 		m_lock;
	//CMutexEx 		m_mutexClr;
};
```

```c++
 #ifndef _MY_CONDITION_H
 2 #define _MY_CONDITION_H
 3 
 4 #include <pthread.h>
 5 #include <time.h>
 6 #define INFINITE            0xFFFFFFFF
 7 
 8 
 9 class MyCondition
10 {
11 public:
12     MyCondition()
13     {
14         pthread_condattr_t cond;
15         pthread_condattr_init(&cond);
16         pthread_condattr_setclock(&cond, CLOCK_MONOTONIC);
17         pthread_cond_init(&m_pthCondt, &cond);
18         pthread_mutex_init(&m_mtxCondt, NULL);
19     }
20 
21     ~MyCondition()
22     {
23         pthread_cond_destroy(&m_pthCondt);
24         pthread_mutex_destroy(&m_mtxCondt);
25     }
26 
27     void ResetSignal()
28     {
29     }
30 
31     void Active()
32     {
33         pthread_mutex_lock(&m_mtxCondt);
34         pthread_cond_broadcast(&m_pthCondt);
35         pthread_mutex_unlock(&m_mtxCondt);
36     }
37 
38     bool timed_wait(int nSecond)
39     {
40         bool bRet = true;
41         if (INFINITE == nSecond)
42         {
43             pthread_mutex_lock(&m_mtxCondt);
44             if (0 != pthread_cond_wait(&m_pthCondt, &m_mtxCondt))
45                 bRet = false;
46             pthread_mutex_unlock(&m_mtxCondt);
47         }
48         else
49         {
50             pthread_mutex_lock(&m_mtxCondt);
51             struct timespec tv;
52             clock_gettime(CLOCK_MONOTONIC, &tv);
53             tv.tv_sec += nSecond;
54             if (0 != pthread_cond_timedwait(&m_pthCondt, &m_mtxCondt, &tv))
55                 bRet = false;
56             pthread_mutex_unlock(&m_mtxCondt);
57         }
58 
59         return bRet;
60     }
61 
62 private:
63     pthread_cond_t    m_pthCondt;
64     pthread_mutex_t   m_mtxCondt;
65 };
66 
67 #endif
```



### 虚假唤醒

​		唤醒操作（SetEvent和pthread_cond_signal）原本意图是唤醒一个等待的线程，但是在多核处理器下，可能会激活多个等待的线程，这种效应为“虚假唤醒”。linux帮助文档中提到：虽然虚假唤醒在pthread_cond_wait函数中可以解决，为了发生概率很低的情况而降低边缘条件（fringe condition）效率是不值得的，纠正这个问题会降低对所有基于它的所有更高级的同步操作的并发度。所以pthread_cond_wait的实现上没有去解决它。所以通常的解决办法是在线程被激活后还需要检测等待的条件是否满足。

​		因此一般使用循环来包裹wait来防止虚假唤醒:

```c++
while(list_empty(&productList)) // 条件不满足
{
	pthread_cond_wait(&product_cond, &product_mutex);
}
```



### 唤醒丢失

 		如果在等待条件变量（pthread_cond_wait）前，条件变量就被唤醒激活（pthread_cond_signal），那么这次唤醒就会丢失。

​		例如客户端向服务端发送同步消息时，客户端需要等到服务的回应再返回发送接口，这时需要在发送接口内部等待回应。（异步接口上实现同步操作）

```c++
//ClientSession.h
 2 #ifndef _CLIENT_SESSION_H_
 3 #define _CLIENT_SESSION_H_
 4 
 5 class ClientSession
 6 {
 7 public:
 8     ClientSession();
 9     ~ClientSession();
10 
11 public:
12     void OnRecvServerResponse();
13 
14 public:
15     int SendSyncMsg(string sMsg);
16 
17 private:
18     MyCondition m_objCond;
19     TcpServer m_objTcpServer;
20 };
21 
22 #endif
 1 //ClientSession.cpp
 2 
 3 #include "ClientSession.h"
 4 
 5 ClientSession::ClientSession()
 6 {
 7 }
 8 
 9 ClientSession::~ClientSession()
10 {
11 }
12 
13 void ClientSession::OnRecvServerResponse()
14 {
15     m_objCond.Active();                        //唤醒等待
16 }
17 
18 int SendSyncMsg(string sMsg)
19 {
20     long lRet = m_objTcpServer.SendData(sMsg.c_str(),sMsg.length());
21     if (lRet >= 0)                            //网络发送成功
22     {
23         if (m_objCond.time_wait(5000) >= 0)    //收到服务端回应
24         {
25             return 0;                        //发送成功
26         }
27         else                                //服务端回应超时
28         {
29             return -1;
30         }
31     }
32     return -1;
33 }

```





## 信号量

​		信号量是由E.W.Dijkstra为互斥和同步的高级管理提出的概念。它支持两种原子操作，一个是wait操作(减少信号量的值)，另一个是post操作(增加信号量的值)。

​		一般来说， 信号量是和某种预先定义的资源相关联的。信号量元素的值，表示与之关联的资源的个数。内核会负责维护信号量的值，并确保其值不小于0。

​	信号量按照初始化的信号量值，可以分为使用二值信号量(binary semaphore)和计数信号量(counting semaphore)

- 二值信号量：是使用最广泛的信号量。 对于这种信号量而言，它只有两种合法值：0和1，对应一个可用的资源。若当前有资源可用，则与之对应的二值信号量的值为1；若资源已被占用，则与之对应的二值信号量的值为0。

- 计数信号量：资源个数超过1个的信号量。假设计数信号量初始化的信号量值为5，表示该信号量有6中合法值：0、1、2、3、4、5。当取值为0时，表示没有资源可用了；其他合法值则表示资源的剩余数量。

  ​	它可以与多个相同的资源关联，允许多个线程并发的使用多个资源。在某种程度上来说，计数信号量是对互斥量的一个扩展，互斥量是同一时间内只允许一个线程访问共享资源，而计数信号量允许多个线程并发访问共享资源。

​		信号量即可用于进程间同步，也可用于线程间同步.条件变量需要和[互斥锁](https://so.csdn.net/so/search?q=互斥锁&spm=1001.2101.3001.7020)配合使用，而信号量之间使用就可以，所以信号量更像是条件变量和互斥锁的组合.信号量内部以计数方式来判定是否阻塞（有累计数量的功能），计数单次加减1即每次只能唤醒一个进程或线程。

​		Linux系统]中提供了两个信号量实现，一种是`System V信号量`，另一种是`POSIX信号量`，它们的作用是相同的，都是用于同步进程之间及线程之间的操作，以达到无冲突地访问共享资源的目的。

- System V信号量

  ```c
  #include <sys/types.h>
  #include <sys/ipc.h>
  #include <sys/sem.h>
  int semget(key_t key, int nsems, int semflg);
  int semctl(int semid, int semnum, int cmd,/* union semun arg*/);
  int semop(int semid, struct sembuf *sops, unsigned nsops);
  ```

- POSIX信号量

  - 有名信号量由于其有名字， 多个不相干的进程可以通过名字来打开同一个信号量， 从而完成同步操作， 所以有名信号量的操作要方便一些， 适用范围也比无名信号量更广。

    ```c
    sem_t *sem_open(const char *name, int oflag);
    sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
    sem_close(sem_p);
    sem_unlink(sem_p);
    ```

  - 无名信号量，由于没有名字多用于线程之间

    ​	又称为`基于内存的信号量`，由于其没有名字，没法通过open操作直接找到对应的信号量，所以很难直接用于没有关联的两个进程之间。无名信号量多用于线程之间的同步。因为线程会共享地址空间， 所以访问共同的无名信号量是很容易办到的事情。

    ```c
    #include<semaphore.h>
    
    int sem_init(sem_t *sem,int pshared,unsigned int value);
    //pshared 用于声明信号量是在线程间共享还是在进程间共享。0表示在线程间共享，非零值则表示信号量将在进程间共享。 要想在进程间共享，信号量必须位于共享内存区域内。
    int sem_wait(sem_t *sem);
    int sem_trywait(sem_t *sem);
    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    // 这三个函数都是用于等待信号量， 它会将信号量的值减1。如果函数正处于阻塞，被信号中断，则返回-1，并且置errno为EINTR。
    // sem_wait：若信号量值大于0， 那么sem_wait函数将信号量的值减1之后会立刻返回。否则sem_wait函数陷入阻塞，待信号量的值大于0之后，再执行减1操作，然后成功返回。
    // sem_trywait：若信号量值大于0，那么sem_trywait函数将信号量的值减1之后会立刻返回。否则sem_trywait立刻返回失败， 并置errno为EAGAIN。
    // sem_timedwait：若信号量值大于0，那么sem_timedwait函数将信号量的值减1之后会立刻返回。否则sem_timedwait会等待一段时间，如果超过了等待时间，信号量的值仍为0，那么返回 -1，并置errno为ETIMEOUT。
    int sem_post(sem_t *sem); 
    // 如果发布信号量之前， 信号量的值是0，并且已经有线程正等待在该信号量上，调用sem_post之后，会有一个线程被唤醒，被唤醒的线程会继续sem_wait函数的减1操作。 如果有多个线程正等待在信号量上，那么将无法确认哪个线程会被唤醒。
    int sem_getvalue(sem_t *sem, int *sval);
    //如果值大于0，表示不需要等待；如果值为0，表示再申请资源时需要等待。这个值不会为负数，并且其返回的值可能已经过时了。
    int sem_destroy(sem_t *sem);
    ```

    

  

```c
// C++  --std=c++20
#include <semaphore>

#include <iostream>
#include <thread>
#include <semaphore>
#include <unistd.h>
 
using namespace std;
 
std::counting_semaphore streams_sem{0};
 
void custom_thread(){
  while (1){
    streams_sem.acquire();
    std::cout << "shopping!\n";
    usleep(50000);
  }
}
 
class Prod{
public:
    Prod(int nCnt){  m_needProCnt = nCnt; }
    void product() {
        int num = 0;
        while(num<m_needProCnt){
           cout<<" product production"<<endl;
           streams_sem.release();
           ++num;
        }
 
    }
    void inner_custom(){
       while(1) {
        if ( !streams_sem.try_acquire() )
        {
          cout<<"want? no way"<<endl;
          break;
        }
        std::cout << "want inner shopping!\n";
        usleep(100000);
       }
    }
private:
    int m_needProCnt;
};
 
int main(int argc, char const* argv[]){
  std::thread t1(custom_thread);
  t1.detach();
  Prod prod(20);
  std::thread t2(&Prod::product, &prod);
  std::thread t3(&Prod::inner_custom, &prod);
  t2.join();
  t3.join();
 
  return 0;
}
```

```c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
 
#include <semaphore.h>
 
  sem_t g_sem;///
 
  void func1(void* arg)
  {
      sem_wait(&sem); /线程1初始阻塞
      int *running=arg;
      printf("thread running1\n");
      printf("%d\n",*running);
  }
 
  void func2(void* arg)
  {
      printf("pthread2 running\n");
      sem_post(&sem); //线程2增加信号量，致使线程1阻塞被唤醒以执行后续代码
  }
 
  int main()
  {
      sem_init(&g_sem,0,0);信号量初始化为本进程内多线程使用，初始阻塞
      pthread_t thread[2];
      int  a = 1;
      pthread_create(&(thread[0]),NULL,(void*)func1,(void*)&a);
      sleep(3);
      pthread_create(&(thread[1]),NULL,(void*)func2,(void*)&a);
      pthread_join(thread[0],NULL);
      pthread_join(thread[1],NULL);
      sem_destroy(&sem); //信号量销毁
 
      return 0;
  }
```

