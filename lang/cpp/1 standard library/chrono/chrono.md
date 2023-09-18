# chrono

​		`#include <chrono>`

​		日期时间库，源于`boost`。

---

​		 `chrono`是一个模版库包含三个概念：

- 时间间隔`duration`
- 时钟`clocks`
- 时间点`timepoint`



## 关于时间

- 太阳的东升西落，引出了世界时（UT1）基于地球自转所定义的时间。

- 原子时（TAI），以铯原子跃迁周期作为基准定义时间长度。

- UTC时间以原子时作为计时基础，同时通过添加或减少闰秒（leap second）的方式与UT进行同步。我们日常使用的时间，实际就是UTC时间。

  > ​		地球自转的周期实际上并没有那么稳定，如果使用TAI，就会出现太阳在12点零几秒才到达了当天的最高点，对于日常使用实际是非常不便的。这种误差还会随着时间推移不断积累。为了综合TAI计时精度和UT实际应用的便利性，就有了协调世界时，即UTC时间。

## duration

​		`std::chrono::duration` 表示一个时间段。

```c++
template <class Rep, class Period = ratio<1>>
class duration;

// `Rep`表示一种数值类型，用来表示`Period`周期的类型（默认为整形）。
//		若 Rep 是浮点数，则 duration 能使用小数描述时钟周期
// `Period`是`ratio`类型，表示时钟周期
```



### ratio

​		用来表征某个时间单位与秒的比例

```c++
template <intmax_t N, intmax_t D = 1> 
class ratio;
// N代表分子，D代表分母，所以ratio表示一个与秒的比例值。
```

> 可以定义Period，比如ratio<1, -2>表示单位时间是-0.5秒。
>
> - ratio<2> 代表一个时钟周期是 2 秒
> - ratio<60 > 代表一分钟
> - 而 ratio<1,1000 > 代表的是 1/1000 秒，也就是 1 毫秒

​		常用的`duration<Rep,Period>`已经定义好了，在`std::chrono::duration`下：

```c++
ratio<3600, 1>        hours
ratio<60, 1>          minutes
ratio<1, 1>           seconds
ratio<1, 1000>        microseconds
ratio<1, 1000000>     microseconds
ratio<1, 1000000000>  nanosecons
    
    
纳秒：std::chrono::nanoseconds	duration<Rep*/* 至少 64 位的有符号整数类型 */*, std::nano>
微秒：std::chrono::microseconds	duration<Rep*/* 至少 55 位的有符号整数类型 */*, std::micro>
毫秒：std::chrono::milliseconds	duration<Rep*/* 至少 45 位的有符号整数类型 */*, std::milli>
秒： std::chrono::seconds	duration<Rep*/* 至少 35 位的有符号整数类型 */*>
分钟：std::chrono::minutes	duration<Rep*/* 至少 29 位的有符号整数类型 */*, std::ratio<60>>
小时：std::chrono::hours	duration<Rep*/* 至少 23 位的有符号整数类型 */*, std::ratio<3600>>
```



#### ratio提供的编译期计算功能

```c++
#include <iostream>
#include <ratio>

int main()
{
    using two_third = std::ratio<2, 3>;
    using one_sixth = std::ratio<1, 6>;

    using sum = std::ratio_add<two_third, one_sixth>; // 注意two_third，one_sixth和sum都是类型，不是变量
    std::cout << "2/3 + 1/6 = " << sum::num << '/' << sum::den << '\n';
    return 0;
}
```

利用ratio，我们就可以自己定义时长单位，比如说。我们要定义一刻钟类型`one_quarter_of_an_hour`

```cpp
using one_quarter_of_an_hour = duration<long long, ratio<60 * 15>>;
auto t = duration_cast<one_quarter_of_an_hour>(1h).count() // t == 4;
```

### ctor

​		`chrono `库中根据 `duration `类封装了不同长度的时钟周期（也可以自定义），基于这个时钟周期再进行周期次数的设置就可以得到总的时间间隔了（时钟周期 * 周期次数 = 总的时间间隔）。

```c++
// 1. 拷贝构造函数
duration( const duration& ) = default;

// 2. 通过指定时钟周期的类型来构造对象
template< class Rep2 >
constexpr explicit duration( const Rep2& r );

// 3. 通过指定时钟周期类型，和时钟周期长度来构造对象
template< class Rep2, class Period2 >
constexpr duration( const duration<Rep2,Period2>& d );
```

```c++
#include <chrono>
#include <iostream>
using namespace std;
int main()
{
	//h(1) 时钟周期为 1 小时，共有 1 个时钟周期，所以 h 表示的时间间隔为 1 小时
    chrono::hours h(1);                          // 一小时

	//ms(3) 时钟周期为 1 毫秒，共有 3 个时钟周期，所以 ms 表示的时间间隔为 3 毫秒
    chrono::milliseconds ms{ 3 };                // 3 毫秒

	//ks(3) 时钟周期为 1000 秒，一共有三个时钟周期，所以 ks 表示的时间间隔为 3000 秒
    chrono::duration<int, ratio<1000>> ks(3);    // 3000 秒
	
	//d3(3.5) 时钟周期为 1000 秒，时钟周期数量只能用整形来表示，但是此处指定的是浮点数，因此语法错误
    // chrono::duration<int, ratio<1000>> d3(3.5);  // error

	//dd(6.6) 时钟周期为默认的 1 秒，共有 6.6 个时钟周期，所以 dd 表示的时间间隔为 6.6 秒
    chrono::duration<double> dd(6.6);               // 6.6 秒

    // 使用小数表示时钟周期的次数
    //hz(3.5) 时钟周期为 1/30 秒，共有 3.5 个时钟周期，所以 hz 表示的时间间隔为 1/30*3.5 秒
    chrono::duration<double, std::ratio<1, 30>> hz(3.5);
}

#include <chrono>
#include <iostream>
int main()
{
//ms 时间单位为毫秒，初始化操作 ms{3} 表示时间间隔为 3 毫秒，一共有 3 个时间周期，每个周期为 1 毫秒
    std::chrono::milliseconds ms{3};         // 3 毫秒

//s 时间单位为微秒，初始化操作 2*ms 表示时间间隔为 6000 微秒，一共有 6000 个时间周期，每个周期为 1 微秒
    std::chrono::microseconds us = 2*ms;     // 6000 微秒

    // 时间间隔周期为 1/30 秒
//hz 时间单位为秒，初始化操作 hz(3.5) 表示时间间隔为 1/30*3.5 秒，一共有 3.5 个时间周期，每个周期为 1/30 秒 
    std::chrono::duration<double, std::ratio<1, 30>> hz(3.5);

    std::cout <<  "3 ms duration has " << ms.count() << " ticks\n"
              <<  "6000 us duration has " << us.count() << " ticks\n"
              <<  "3.5 hz duration has " << hz.count() << " ticks\n";       
}
```



### fn

####  operator

![a](https://img-blog.csdnimg.cn/c5f93eb5b2ff47bd9be7efed77864515.png)

```c++
#include <iostream>
#include <chrono>
using namespace std;

int main()
{
    chrono::minutes t1(10);
    chrono::seconds t2(60);
    chrono::seconds t3 = t1 - t2;
    cout << t3.count() << " second" << endl;
}
```

​		注意事项：duration 的加减运算有一定的规则，当两个 duration 时钟周期不相同的时候，会先统一成一种时钟，然后再进行算术运算，统一的规则如下：假设有 ratio<x1,y1> 和 ratio<x2,y2 > 两个时钟周期，首先需要求出 x1，x2 的最大公约数 X，然后求出 y1，y2 的最小公倍数 Y，统一之后的时钟周期 ratio 为 ratio<X,Y>

```c++
#include <iostream>
#include <chrono>
using namespace std;

int main()
{
    chrono::duration<double, ratio<9, 7>> d1(3);
    chrono::duration<double, ratio<6, 5>> d2(1);
    // d1 和 d2 统一之后的时钟周期
    chrono::duration<double, ratio<3, 35>> d3 = d1 - d2;
}
```



#### count

​     `duration`还有一个成员函数`count()`返回`Rep`类型的`Period`数量。即时钟上记录的时钟周期数量。

```c++
constexpr rep count() const;
```

> `time_since_epoch()`的返回值是类型是时间长度（duration类型），即从纪元起点到`now`对应时间点间的时间长度。
>
> 时间长度类型可以通过`count()`转化为数值类型，方便进一步在其他代码中使用

```c++
// duration::count
 #include <iostream>     // std::cout
 #include <chrono>       // std::chrono::seconds, std::chrono::milliseconds
                         // std::chrono::duration_cast
 
int main ()
{
   	using namespace std::chrono;
   	// std::chrono::milliseconds is an instatiation of std::chrono::duration:
   	milliseconds foo (1000); // 1 second
   	foo*=60;
  
   	std::cout << "duration (in periods): ";
   	std::cout << foo.count() << " milliseconds.\n";
  
   	std::cout << "duration (in seconds): ";
   	std::cout << foo.count() * milliseconds::period::num / milliseconds::period::den;
   	std::cout << " seconds.\n";
  
   	return 0;
}
```



#### duration_cast

​		`chrono`库提供了`duration_cast`类型转换函数。

```c++
template <class ToDuration, class Rep, class Period>
constexpr ToDuration duration_cast (const duration<Rep,Period>& dtn);
```

```c++
// duration constructor2
#include <iostream> 
#include <ratio>
#include <chrono>
int main ()
{
    typedef std::chrono::duration<int> seconds_type;
    typedef std::chrono::duration<int,std::milli> milliseconds_type;
    typedef std::chrono::duration<int,std::ratio<60*60>> hours_type;

 	hours_type h_oneday (24);                  // 24h
	seconds_type s_oneday (60*60*24);          // 86400s
	milliseconds_type ms_oneday (s_oneday);    // 86400000ms

 	seconds_type s_onehour (60*60);            // 3600s
	//hours_type h_onehour (s_onehour);          // NOT VALID (type truncates), use:
  	hours_type h_onehour (std::chrono::duration_cast<hours_type>(s_onehour));
   	milliseconds_type ms_onehour (s_onehour);  // 3600000ms (ok, no type truncation)
  
   	std::cout << ms_onehour.count() << "ms in 1h" << std::endl;
  
   	return 0;
 }
```





## timepoint

​		`std::chrono::time_point` 表示时间中的一个点，即时刻。它被实现成如同存储一个 `Duration` 类型的自 `Clock` 的纪元起始开始的时间间隔的值。

```c++
template<class Clock, class Duration = typename Clock::duration> 
class time_point;

// Clock：选用时钟
// Duration：用于计量从纪元起时间的 std::chrono::duration 类型

using unix_time_seconds = time_point<system_clock, seconds>;
```

​		`Clock` 必须满足*时钟* (`Clock`) 的要求或为 `std::chrono::local_t (C++20 起)`。

### 成员类型
| 成员类型   | 定义                                                         |
| ---------- | ------------------------------------------------------------ |
| `clock`    | `Clock` ，此时间点在此时钟上计量                             |
| `duration` | `Duration` ，用于计量从纪元起时间的 std::chrono::duration 类型 |
| `rep`      | `Rep` ，表示时期的计次数的算术类型                           |
| `period`   | `Period` ，表示时期周期的 std::ratio 类型                    |



```c++
#include <chrono>
#include <iostream>
 
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
 
void print_ms(const TimePoint& point)
{
    using Ms = std::chrono::milliseconds;
    const Clock::duration since_epoch = point.time_since_epoch();
    std::cout << std::chrono::duration_cast<Ms>(since_epoch).count() << " ms\n";
}
 
int main()
{
    const TimePoint default_value = TimePoint(); // (1)
    print_ms(default_value); // 0 ms
 
    const Clock::duration duration_4_seconds = std::chrono::seconds(4);
    const TimePoint time_point_4_seconds(duration_4_seconds); // (2)
    // 从纪元开始 4 秒
    print_ms(time_point_4_seconds); // 4000 ms
 
    const TimePoint time_point_now = Clock::now(); // (3)
    print_ms(time_point_now); // 43098276 ms
}
```



### ctor

```c++
std::chrono::time_point<Clock,Duration>::time_point;
    
// 创建表示 Clock 的纪元（即 time_since_epoch() 为零）的 time_point 。
time_point();  (C++11 起)  (C++14 前) 
constexpr time_point();    (C++14 起) 

// Clock 的纪元加上 d 的 time_point 。
explicit time_point( const duration& d );  (C++11 起) (C++14 前) 
constexpr explicit time_point( const duration& d ); (C++14 起)  
 
// 此构造函数仅若 Duration2 可隐式转换为 duration 才参与重载决议。
template<class Duration2> time_point(const time_point<Clock,Duration2>& t); (C++11 起) (C++14 前) 
template<class Duration2> constexpr time_point(const time_point<Clock,Duration2>& t); (C++14 起) 

// d	-	复制来源的 duration
// t	-	转换来源的 time_point
```

```c++
// time_point constructors
#include <iostream>
#include <chrono>
#include <ctime>
int main ()
{
    using namespace std::chrono;9  
   system_clock::time_point tp_epoch;    // epoch value
  
   time_point <system_clock,duration<int>> tp_seconds (duration<int>(1));
  
   system_clock::time_point tp (tp_seconds);
  
   std::cout << "1 second since system_clock epoch = ";
   std::cout << tp.time_since_epoch().count();
   std::cout << " system_clock periods." << std::endl;
  
   // display time_point:
   std::time_t tt = system_clock::to_time_t(tp);
   std::cout << "time_point tp is: " << ctime(&tt);
  
   return 0;
}
```



### fn

#### operator

![](https://img-blog.csdnimg.cn/166f7df1c1bf4ee78e21e8c58feb9661.png)

```c++
std::chrono::time_point<Clock,Duration>::operator+, operator-;

//  d_ += d
time_point& operator+=( const duration& d );	(C++17 前)
constexpr time_point& operator+=( const duration& d );(C++17 起)

//  d_ -= d
time_point& operator-=( const duration& d );	(C++17 前)
constexpr time_point& operator-=( const duration& d );(C++17 起)
```

```c++

#include <iostream>
#include <chrono>
#include <ctime>
 
int main()
{
    auto p1 = std::chrono::system_clock::now();
    auto today_time = std::chrono::system_clock::to_time_t(p1);
    std::cout << "now(): " << std::ctime(&today_time);
 
    auto p2 = p1;
    p2 += std::chrono::hours(24);
    today_time = std::chrono::system_clock::to_time_t(p2);
    std::cout << "now() + hours(24) : " << std::ctime(&today_time);
 
    auto p3 = p1;
    p3 -= std::chrono::hours(24);
    today_time = std::chrono::system_clock::to_time_t(p3);
    std::cout << "now() - hours(24) : " << std::ctime(&today_time);
}
```

#### time_since_epoch

​		获得1970年1月1日到`time_point`时间经过的`duration`.

> 如果`timepoint`以天为单位，函数返回的`duration`就以天为单位

```c++
std::chrono::time_point<Clock,Duration>::time_since_epoch;

duration time_since_epoch() const; //(C++11 起) (C++14 前)
constexpr duration time_since_epoch() const;//(C++14 起)

// 返回表示 *this 与 clock 的纪元间的时间量的 duration 。
```

```c++
#include <iostream>
#include <chrono>
#include <ctime>
 
int main()
{
    auto p0 = std::chrono::time_point<std::chrono::system_clock>{};
    auto p1 = std::chrono::system_clock::now();
    auto p2 = p1 - std::chrono::hours(24);
 
    auto epoch_time = std::chrono::system_clock::to_time_t(p0);
    std::cout << "epoch: " << std::ctime(&epoch_time);
    auto today_time = std::chrono::system_clock::to_time_t(p1);
    std::cout << "today: " << std::ctime(&today_time);
 
    std::cout << "hours since epoch: "
              << std::chrono::duration_cast<std::chrono::hours>(
                   p1.time_since_epoch()).count() 
              << '\n';
    std::cout << "yesterday, hours since epoch: "
              << std::chrono::duration_cast<std::chrono::hours>(
                   p2.time_since_epoch()).count() 
              << '\n';
} 
```



#### time_point_cast

​		由于各种`time_point`表示方式不同，`chrono`也提供了相应的转换函数 `time_point_cast`。

```c++
template <class ToDuration, class Clock, class Duration>
time_point<Clock,ToDuration> time_point_cast (const time_point<Clock,Duration>& tp);
```

```c++
// time_point_cast2
#include <iostream>
#include <ratio>
#include <chrono>   

int main () 
{   
    using namespace std::chrono;
    typedef duration<int,std::ratio<60*60*24>> days_type;

    time_point<system_clock,days_type> today = time_point_cast<days_type>(system_clock::now());
  
   std::cout << today.time_since_epoch().count() << " days since epoch" << std::endl;
   return 0;
}
```



### applacation

#### str <=> time

​		理论上，C++20提供了`std::parse`和format标准库完成日期时间字符串的相互转换，但现实是，这些特性还没有被当前的主流编译器所支持，我们也就不得不使用更传统的方式实现这一功能了

- str  -> time

  ```c++
  #include <iomanip>
  #include <sstream>
  
  std::tm tm = {};
  std::stringstream ss("2017-06-08 09:00:05");
  ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
  time_point<system_clock> tp2 = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  ```

- time -> str

  ```c++
  std::time_t tt = system_clock::to_time_t(tp2);
  std::tm tm = *std::gmtime(&tt); //UTC 时间
  //std::tm tm = *std::localtime(&tt); //使用本地时区
  std::stringstream ss;
  ss << std::put_time( &tm, "%Y-%m-%d %H:%M:%S");
  std::string result =  ss.str();
  
  //std::tm* ptm = std::localtime(&tt);
  //std::cout << "Current time is: " << std::put_time(ptm,"%c") << std::endl;
  
  // char buffer[80];
  // std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
  
  // std::ctime(&now_c);
  ```
  
  

## Clocks

​		每一个`clock`类中都有确定的`time_point`, `duration`, `Rep`, `Period`类型。

- `std::chrono::system_clock` 它表示当前的系统时钟
  - 系统中运行的所有进程使用`now()`得到的时间是一致的
  - 系统的时钟可以修改，甚至可以网络对时，因此使用系统时间计算时间差可能不准
- `std::chrono::steady_clock `
  - 为了表示稳定的时间间隔，后一次调用`now()`得到的时间总是比前一次的值大（这句话的意思其实是，如果中途修改了系统时间，也不影响`now()`的结果），每次`tick`都保证过了稳定的时间间隔
  - 是固定的时钟，相当于秒表。开始计时后，时间只会增长并且不能修改，适合用于记录程序耗时

- `std::chrono::high_resolution_clock` 系统可用的最高精度的时钟
  - 提供了最高的可用时间分辨率。
  - 它通常是`std::chrono::system_clock`或`std::chrono::steady_clock`中的一个类型别名，具体取决于具体平台和库实现。

​		在这些时钟类的内部有 `time_point`、`duration`、`Rep`、`Period `等信息，基于这些信息来获取当前时间，以及实现 `time_t `和 `time_point `之间的相互转换



### system_clock

​		大部分编译器的实现都使用Unix时间（Unix Time）作为这个时钟源的纪元（epoch）起点。从C++20开始，这个时间被确定为Unix Time。

```c++
struct system_clock { // wraps GetSystemTimePreciseAsFileTime/GetSystemTimeAsFileTime
    using rep                       = long long;
    using period                    = ratio<1, 10'000'000>; // 100 nanoseconds
    using duration                  = chrono::duration<rep, period>;
    using time_point                = chrono::time_point<system_clock>;
    static constexpr bool is_steady = false;

    _NODISCARD static time_point now() noexcept 
    { // get current time
        return time_point(duration(_Xtime_get_ticks()));
    }

    _NODISCARD static __time64_t to_time_t(const time_point& _Time) noexcept 
    { // convert to __time64_t
        return duration_cast<seconds>(_Time.time_since_epoch()).count();
    }

    _NODISCARD static time_point from_time_t(__time64_t _Tm) noexcept 
    { // convert from __time64_t
        return time_point{seconds{_Tm}};
    }
};


// rep：时钟周期次数是通过整形来记录的 long long
// period：一个时钟周期是 100 纳秒 ratio<1, 10’000’000>
// duration：时间间隔为 rep*period 纳秒 chrono::duration<rep, period>
// time_point：时间点通过系统时钟做了初始化 chrono::time_point<system_clock>，里面记录了新纪元时间点

// 返回表示当前时间的时间点。
static std::chrono::time_point<std::chrono::system_clock> now() noexcept;
// 将 time_point 时间点类型转换为 std::time_t 类型
static std::time_t to_time_t( const time_point& t ) noexcept;
// 将 std::time_t 类型转换为 time_point 时间点类型
static std::chrono::system_clock::time_point from_time_t( std::time_t t ) noexcept;
```

```c++
#define  _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;
int main()
{
    // 新纪元1970.1.1时间
    system_clock::time_point epoch;

    duration<int, ratio<60 * 60 * 24>> day(1);
    // 新纪元1970.1.1时间 + 1天
    system_clock::time_point ppt(day);

    using dday = duration<int, ratio<60 * 60 * 24>>;
    // 新纪元1970.1.1时间 + 10天
    time_point<system_clock, dday> t(dday(10));

    // 系统当前时间
    system_clock::time_point today = system_clock::now();

    // 转换为time_t时间类型
    time_t tm = system_clock::to_time_t(today);
    cout << "今天的日期是:    " << ctime(&tm);

    time_t tm1 = system_clock::to_time_t(today + day);
    cout << "明天的日期是:    " << ctime(&tm1);

    time_t tm2 = system_clock::to_time_t(epoch);
    cout << "新纪元时间:      " << ctime(&tm2);

    time_t tm3 = system_clock::to_time_t(ppt);
    cout << "新纪元时间+1天:  " << ctime(&tm3);

    time_t tm4 = system_clock::to_time_t(t);
    cout << "新纪元时间+10天: " << ctime(&tm4);
}

```



### utc_clock

​		`chrono`库中`utc_clock`提供的就是UTC时间，而`system_clock`提供的时间并不包含闰秒。

```c++
cout << "utc_clock:\t" << duration_cast<seconds>(utc_clock::now().time_since_epoch()).count() << endl;
cout << "system_clock:\t" << duration_cast<seconds>(system_clock::now().time_since_epoch()).count() << endl;
// 上述代码在ms visual studio 16.11.3 开启 /std:c++latest 后测试通过，gcc尚未支持

// utc_clock:      1632225520
// system_clock:   1632225493
```

> 为了便于使用，`utc_clock`使用了Unix时间的起始时间点，上述两者之差就是Unix时间起点到代码运行时所增加的闰秒数

​		不过接下来会有一个有趣的事情发生，如果我们将这两个表的时间打印出来

```cpp
cout << system_clock::now() << endl;
cout << utc_clock::now() << endl;
// 上述代码在ms visual studio 16.11.3 开启 /std:c++latest 后测试通过

// 2021-09-21 12:07:27.1916279
// 2021-09-21 12:07:27.1945413
```

​		我们会发现，他们的输出结果是一样的！（忽略高于秒的精度）

​		这就进一步引出了一个问题，这两个表的秒数不一样，但格式化后却是一样的，为什么？这就涉及到闰秒实际插入时的操作了。闰秒由IERS(International Earth Rotation and Reference Systems Service)决定。os的时间服务（如linux中的NTP）会根据该机构公布的信息，完成相关操作。对于UTC时间，在插入闰秒时，会出现23:59:59->23:59:60->00:00:00这样的操作，但对于系统时间，出现类似于23:59:60的时间会造成很大的麻烦。因此os的时间服务会用某种机制避免这种情况，例如，可以在UTC出现23:59:60时，让系统时间仍然保持在23:59:59，在这两秒中内，系统时间都是23:59:59。一个更好的方案是在插入闰秒的时间前后，略微增加系统时间秒的长度，实现平滑的过度。

​		现在我们将目光朝向`system_clock`和`utc_clock`的具体实现。事实上，`system_clock`恰如其名，这个时间是通过系统调用从OS获取的，具体涉及的系统调用如下表[[2\]](https://zhuanlan.zhihu.com/p/414702182#ref_2)

| 平台    | 系统调用                                                     |
| ------- | ------------------------------------------------------------ |
| Windows | GetSystemTimePreciseAsFileTime(&ft) GetSystemTimeAsFileTime(&ft) |
| 其他    | clock_gettime(CLOCK_MONOTONIC, &tp) gettimeofday(&tv, 0)     |

​		对于`utc_clock`目前仅MSVC提供了相关支持。通过代码可以看出[[3\]](https://zhuanlan.zhihu.com/p/414702182#ref_3)，UTC时间是系统时间经过闰秒调整后获得的。不过如果恰好处于闰秒调整阶段，这个处理就会更为复杂一些，也欢迎有兴趣的同学进一步探究

​		最后需要强调的是`system_clock`返回的是系统时间，而系统时间是**会发生调整**的，例如NTP时间同步，或者用户手动设置（突然想到以前用变速齿轮或者变速精灵等软件加速游戏速度的方法）。因此要注意，这个时间是有可能发生时间回退等情况的，在使用时务必做出充分的考量

### steady_clock

​		`std::chrono::steady_clock`是一个表示物理时间流逝的时钟，不受任何外部因素（如用户修改系统时间，夏令时等）的影响。

​		相比于`system_clock`，`steady_clock`最为重要的特性就是它一定是单调增加的，而且通常来说，它能够提供系统所支持的最高精度时间。另一个要注意的点是它的起始时间是不一定的，例如，它的起点可能是系统启动的时间，因此，它非常适合于测试时间区间，典型的应用就是测试程序运行时长。从代码中可以看出，`steady_clock`同样是由操作系统的系统调用完成，关键的系统调用如下表[[2\]](https://zhuanlan.zhihu.com/p/414702182#ref_2)

| 平台    | 系统调用                                                     |
| ------- | ------------------------------------------------------------ |
| z/OS    | gettimeofdayMonotonic(&ts)                                   |
| Windows | __QueryPerformanceFrequency() QueryPerformanceCounter(&counter) |
| Linux   | clock_gettime(CLOCK_MONOTONIC, &tp)                          |
| macOS   | mach_timebase_info(&MachInfo);                               |

另外请注意，chrono库中还提供了`high_resolution_clock`，顾名思义，大家可能会认为这个可以用于高精度计时，但目前在大多数平台上它仅仅是`system_clock`或`steady_clock`的别名，并没有提供更高精度的时间度量。这种不确定的实现也带来了最大的问题，`high_resolution_clock`若基于`system_clock`实现，那么潜在的时间回退风险，也使得其高精度变得缺乏意义，因而标准库并不建议用户使用

`steady_clock`的精确性和稳定性是如何达到的。

在我们向内核进发之前，先抛出几个问题

1. CPU是如何感知时间的？特别是现代CPU可以根据负载改变频率的情况下，如何稳定计时。
2. 在多核、多CPU硬件上运行的OS，又如何提供一个可靠的稳定时钟？
3. 既然高精度计时依赖于系统调用，那怎么避免或者减少系统调用本身带来的时间误差？

考虑到这些问题与OS和硬件的实现非常相关，因此本文仅讨论Linux和Intel的CPU。下面的部分参考了Intel的手册和一篇非常好的博文（欢迎感兴趣的同学阅读原文[[4\]](https://zhuanlan.zhihu.com/p/414702182#ref_4)或者我翻译的版本[[5\]](https://zhuanlan.zhihu.com/p/414702182#ref_5)）

如果我们百度“如何高精度测量程序运行时间”，可能会得到几个答案：使用标准库，使用`clock_gettime`，以及时间戳计数器（rdtsc指令）。当然我们已经知道，至少在Linux平台上，使用标准库就相当于使用了`clock_gettime`。因此，目前我们的比较对象就是`clock_gettime`和rdtsc。

从直觉上来看，使用rdtsc指令直达CPU，不论精确度和延迟都是最好的选择，也有不少人持有这一观点。要考察这一观点的正确性，还是要落实到CPU的设计中。

早期的Intel CPU，其时间戳计数器（即rdtsc读取的寄存器）增加速率是会受到CPU频率的影响，因此只能保证这个值是单调上升而不能确保稳定上升，随后的改进克服了这一不足，但仍然带来了一个问题，就是CPU休眠时这个计数器会停止增加。显然，这种设计仍然会带来一些麻烦。最终在Nehalem架构后，TSC成为了一个稳定可靠的时钟源，在CPU改变运行频率或是休眠时仍然能够稳定计时。

接下来的一个挑战是乱序执行，现代CPU为了提高处理速度，并不会完全按照代码顺序执行程序，而是会根据对指令的分析提前或者并行地执行某些代码。这就意味着，在执行rdtsc指令时，待测量的代码有可能已经执行完毕了，测量的结果自然也就失去了意义。不过这个问题还是有解决办法的，即换用rdtscp指令。rdtscp指令会在执行前增加一个内存屏障，从而解决这一问题。

但这个故事并没有结束，CPU执行TSC指令时，TSC在哪里？是多个CPU核心共享一个TSC寄存器，还是每个核心都有自己的TSC呢？答案是，每个核心都有自己的TSC。熟悉操作系统原理的同学都知道，OS会根据系统的负载对线程进行调度，同一个线程被切换到不同的CPU核上运行是家常便饭。在这种情况下，rdtsc可用的前提就是所有的TSC是同步的，否则时间就完全不可比了。更具有挑战性的情况是对称多处理器系统，只有所有CPU的所有CPU核的TSC得到同步，我们才能够用rdtsc获得可靠的时间度量。在Nehalem架构后，CPU支持跨核的同步，对于多处理器系统，则由具体的设计决定

你以为这就是全部了吗，很遗憾并不是，前方还有一个大BOSS：虚拟化。在使用云服务器、容器化技术已经是常规选择的现在，这并不是一件容易的事情。虚拟化软件最简单的方式是不对rdtsc指令进行处理，直接由物理机执行。但这也会引起与多线程场景下类似的问题，特别是虚拟机在不同宿主机上发生迁移时，这个问题就更为严重了。如果采用模拟的方法，也会引起一系列的问题。对于这种情况下的不同虚拟化软件处理方法的详细比较，请参考[[5\]](https://zhuanlan.zhihu.com/p/414702182#ref_5)

我们也可以使用下面的命令查看系统使用的时钟源

```bash
cat /sys/devices/system/clocksource/clocksource0/current_clocksource
```

我使用的阿里云服务器给出的结果是

```bash
kvm-clock
```

既然直接使用rdtsc会有许多潜在问题，那linux的`clock_gettime`是如何实现的呢？毕竟任何由软件所提供的功能，最终都要落实到硬件上去执行。目前clock_gettime的首选时钟源，其实就是TSC。但考虑到上述的一系列问题，Linux会采用专门的流程去验证当前的TSC是否是一个可靠的时钟源，并且还提供了相关的标志位，从而使用户了解当前系统TSC所支持的特性。

```bash
$ cat  /proc/cpuinfo | grep -E "constant_tsc|nonstop_tsc"
```

`constant_tsc`表明tsc以恒定速率增加而不受CPU频率变动影响，`nonstop_tsc`表明休眠时TSC也正常增长。如果Linux认为TSC是不可靠的，则会采用其他时钟源。

最后，是关于系统调用的问题。众所周知，系统调用由于会发生用户态和内核态的切换，是一个昂贵的操作，`gettimeofday`和`clock_gettime`也不例外。但显然系统调用的开销会影响到时间相关函数的使用。为了缓解这一问题，Linux引入了一种称之为虚拟动态共享对象（virtual dynamic shared object，vDSO）的机制[[6\]](https://zhuanlan.zhihu.com/p/414702182#ref_6)。直观理解就是系统把一些本属于内核态的代码映射到了用户态代码的地址空间中，在使用时就不需要经过标准的中断流程，从而大大减少调用开销。在x86平台上，使用了vDSO机制的函数有

| 函数名称             | 版本      |
| -------------------- | --------- |
| __vdso_clock_gettime | LINUX 2.6 |
| __vdso_getcpu        | LINUX 2.6 |
| __vdso_gettimeofday  | LINUX 2.6 |
| __vdso_time          | LINUX 2.6 |

`gettimeofday`和`clock_gettime`都有着其对应的vDSO版本。对于日常撸码，只需要正常使用标准库中的函数即可，系统会优先使用vSDO版本

​		至于如何测量时间这个问题，结论就是，优先使用`clock_gettime`，当然这也就意味着优先使用标准库。除非你知道自己在干什么，否则不要直接使用rdtsc。

​		关于代码中的时间处理，看起来是一个并不起眼的部分，但实际上，这是一个综合了语言、标准库、OS和硬件设计的一个综合性的问题。限于篇幅原因，本文对这个问题的讨论也就到此为止了，也希望各位读者能够有所收获。对此问题感兴趣的同学，欢迎进一步阅读参考文献中的相关内容





### fn

```c++
now() 			// 当前时间time_point
to_time_t() 	// time_point转换成time_t秒
from_time_t() 	// 从time_t转换成time_point
```



```c++
// system_clock example
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main()
{
    using std::chrono::system_clock;

    std::chrono::duration<int, std::ratio<60 * 60 * 24> > one_day(1);

    system_clock::time_point today = system_clock::now();
    system_clock::time_point tomorrow = today + one_day;

    std::time_t tt;

    tt = system_clock::to_time_t(today);
    std::cout << "today is: " << ctime(&tt);

    tt = system_clock::to_time_t(tomorrow);
    std::cout << "tomorrow will be: " << ctime(&tt);

    return 0;
}
```

```c++
// steady_clock example
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main()
{

    using namespace std::chrono;

    steady_clock::time_point t1 = steady_clock::now();

    std::cout << "printing out 1000 stars...\n";
    for ( int i = 0; i < 1000; ++i ) std::cout << "*";
    std::cout << std::endl;

    steady_clock::time_point t2 = steady_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout << "It took me " << time_span.count() << " seconds.";
    std::cout << std::endl;

    return 0;
}
```





## date

​		保存日期的基本类型为`std::chrono::year_month_day`

```c++
year_month_day ymd{year(2021), month(9), day(20)};

// 日期与时间点的转换
year_month_day ymd_now{std::chrono::floor<std::chrono::days>(now)}; // 这里floor是相当于time_point_cast，区别在于会向下取整，即舍弃精度高于day的部分
auto tp = sys_days{ymd_now}; // 转换为 system_clock 时间点，精度为 day
```

​		

### fn

#### operator

​		日期也可以进行加减运算

```c++
auto ymd2 = ymd + months(12); // 2022-09-20
```

​		`year_month_day`，`year_month`，`month_day`等多种类型，并定义了它们之间使用 `/`操作符的返回结果，最终实现了`auto ymd = 2021y / September / 20;`这种自然而又不那么自然的用法

## 字面量

​		chrono库大量使用了自定义字面量，简化了时间相关的操作

```c++
using std::chrono::literals; // 时间相关字面量的命名空间
auto one_second = 1s;
auto two_minutes = 2min;
auto dur_2m1s = two_minutes + one_second; // dur_2m1s.count() == 121

auto ymd2 = 2021y / September / 20; // 2021-09-20
```



​		相比于`seconds`乃至于`duration<*, *>`这样指定类型，字面量降低了手指关节和键盘的损耗。自定义字面量的定义也非常简单，例如，我们想要表达长度，我们就可以以米为基准，并定义其他长度

```c++
constexpr long double operator ""_m(long double v) // 下划线是自定义字面量的习惯用法，用于与标准库提供的字面量做区分
{
    return v;
}

constexpr long double operator ""_cm(long double v)
{
    return v / 100;
}

constexpr long double operator ""_mm(long double v)
{
    return v / 1000;
}


long double l1 = 1_m;
long double l2 = 100_cm;
long double l3 = 1000_mm; // l1 == l2 && l2 == l3;
```















