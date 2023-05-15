# numeric

---

​		c++不是以数值计算为主要目的进行设计的。但在科学计算、数据库访问、网络、仪器控制、图形、仿真和金融分析中，通常伴随着数值计算。因此c++成为一个有吸引力的工具。

​		数值功能已经从简单的浮点数向量循环发展为需要更复杂的数据结构，c++的优势使得其广泛用于科学、工程、金融和其他涉及复杂数值的计算。随之支持这种计算的设施和技术出现了。



## Mathematical Functions 数学函数

​		``<cmath>``包含一组标准的数学函数，例如``sqrt()``、``log()``和``sin()``，参数类型为``float``、``double``和``long double``:

| **Selected Standard Mathematical Functions** |                                                   |
| :------------------------------------------- | ------------------------------------------------- |
| **abs(x)**                                   | Absolute value                                    |
| **ceil(x)**                                  | Smallest integer >= **x**                         |
| **floor(x)**                                 | Largest integer <= **x**                          |
| **sqrt(x)**                                  | Square root; **x** must be non-negative           |
| **cos(x)**                                   | Cosine                                            |
| **sin(x)**                                   | Sine                                              |
| **tan(x)**                                   | Tangent                                           |
| **acos(x)**                                  | Arccosine; the result is non-negative             |
| **asin(x)**                                  | Arcsine; the result nearest to 0 is returned      |
| **atan(x)**                                  | Arctangent                                        |
| **sinh(x)**                                  | Hyperbolic sine                                   |
| **cosh(x)**                                  | Hyperbolic cosine                                 |
| **tanh(x)**                                  | Hyperbolic tangent                                |
| **exp(x)**                                   | Base e exponential                                |
| **exp2(x)**                                  | Base 2 exponential                                |
| **log(x)**                                   | Natural logarithm, base e; **x** must be positive |
| **log2(x)**                                  | Natural logarithm, base 2; **x** must be positive |
| **log10(x)**                                 | Base 10 logarithm; **x** must be positive         |

​		对应``complex``的函数版本可以在``<complex>``中找到。对于每个函数，返回值类型与参数类型相同。

​		通过设置``errno``从``<cerrno>``到``EDOM``的域错误和``ERANGE``的范围错误来报告错误。例如:

```c++
errno = 0;         // clear old error state
double d = sqrt(-1)
if (errno==EDOM)
	cerr << "sqrt() not defined for negative argument\n";

errno = 0;         // clear old error state
double dd = pow(numeric_limits<double>::max(),2);
if (errno == ERANGE)
     cerr << "result of pow() too large to represent as a double\n";
```

​		更多的数学函数可以在``<cmath>``和``<cstdlib>``中找到。所谓的特殊数学函数，如``beta()``、``rieman_zeta()``和``sph_bessel()``，也在``<cmath>``中。



## Numerical Algorithms 数值算法

​		``<numeric>``中包含一小部分通用的数值算法。

| **Numerical Algorithms**             |                                                              |
| :----------------------------------- | ------------------------------------------------------------ |
| **x=accumulate(b,e,i)**              | **x** is the sum of **i** and the elements of [**b**:**e**)  |
| **x=accumulate(b,e,i,f)**            | **accumulate** using **f** instead of **+**                  |
| **x=inner_product(b,e,b2,i)**        | **x** is the inner product of [**b**:**e**) and [**b2**:**b2+(e-b)**), that is, the sum of **i** and **(*****p1)*****(*****p2)** for each **p1** in [**b**:**e**) and the corresponding **p2** in [**b2**:**b2+(e-b)**) |
| **x=inner_product(b,e,b2,i,f,f2)**   | **inner_product** using **f** and **f2** instead of **+** and * |
| **p=partial_sum(b,e,out)**           | Element **i** of [**out**:**p**) is the sum of elements [**b**:**b+i**] |
| **p=partial_sum(b,e,out,f)**         | **partial_sum** using **f** instead of **+**                 |
| **p=adjacent_difference(b,e,out)**   | Element **i** of [**out**:**p**) is ***(b+i)-*****(b+i-1)** for **i>0**; if **e-b>0**, then ***out**is ***b** |
| **p=adjacent_difference(b,e,out,f)** | **adjacent_difference** using **f** instead of **−**         |
| **iota(b,e,v)**                      | For each element in [**b**:**e**) assign **v** and increment **++v**; thus the sequence becomes **v**, **v+1**, **v+2**, ... |
| **x=gcd(n,m)**                       | **x** is the greatest common denominator of integers **n** and **m** |
| **x=lcm(n,m)**                       | **x** is the least common multiple of integers **n** and **m** |
| **x=midpoint(n,m)**                  | **x** is the midpoint between **n** and **m**                |

- 这些算法将常见操作推广到所有类型的序列上。
- 它们还将应用于这些序列元素的操作作为参数。
- 对于每种算法，通用版本都辅以一个应用该算法最常用操作符的版本。

```c++
list<double> lst {1, 2, 3, 4, 5, 9999.99999};
auto s = accumulate(lst.begin(),lst.end(),0.0);         // calculate the sum: 10014.9999
```

​		这些算法适用于每个标准库序列，并且可以将操作作为参数提供。



###  Parallel Numerical Algorithms 并行数值算法

​		在``<numeric>``中，数值算法有与顺序算法略有不同的并行版本。特别是，并行版本允许以未指定的顺序对元素进行操作。并行数值算法可以接受执行策略参数:``seq / unseq / par / par_unseq``。



| **Parallel Numerical Algorithms**               |                                                              |
| :---------------------------------------------- | ------------------------------------------------------------ |
| **x=reduce(b,e,v)**                             | **x=accumulate(b,e,v)**, except out of order                 |
| **x=reduce(b,e)**                               | **x=reduce(b,e,V{})**, where **V** is **b**’s value type     |
| **x=reduce(pol,b,e,v)**                         | **x=reduce(b,e,v)** with execution policy **pol**            |
| **x=reduce(pol,b,e)**                           | **x=reduce(pol,b,e,V{})**, where **V** is **b**’s value type |
| **p=exclusive_scan(pol,b,e,out)**               | **p=partial_sum(b,e,out)** according to **pol**, excludes the ith input element from the ith sum |
| **p=inclusive_scan(pol,b,e,out)**               | **p=partial_sum(b,e,out)** according to **pol** includes the ith input element in the ith sum |
| **p=transform_reduce(pol,b,e,f,v)**             | **f(x)** for each **x** in [**b**:**e**), then **reduce**    |
| **p=transform_exclusive_scan(pol,b,e,out,f,v)** | **f(x)** for each **x** in [**b**:**e**), then **exclusive_scan** |
| **p=transform_inclusive_scan(pol,b,e,out,f,v)** | **f(x)** for eac                                             |

​		就像``<algorithm>``中的并行算法一样，可以指定一个执行策略:

```c++
vector<double> v {1, 2, 3, 4, 5, 9999.99999};
auto s = reduce(v.begin(),v.end());           // calculate the sum using a double as the accumulator

vector<double> large;
// ... fill large with lots of values ...
auto s2 = reduce(par_unseq,large.begin(),large.end());  // calculate the sum using available parallelism
```

​		执行策略`par、sec、unsec和par_unsec`隐藏在`<execution>`中的命名空间``std::execution``中。



## complex number 复数

​		标准库支持``class complex``类似的复数类型。为了支持`float`、`double`等复数，标准库中的``complex``是一个模板:

```c++
emplate<typename Scalar>
class complex {
public:
     complex(const Scalar& re ={}, const Scalar& im ={});    // default function arguments;
     // ...
};
```

​		支持复数的常用算术运算和最常用的数学函数。``sqrt()``和``pow()``(乘幂)函数是定义在``<complex>``中的常用数学函数

```c++
complex<float> fl, complex<double> db
complex<long double> ld {fl+sqrt(db)};
db += fl*3;
fl = pow(1/fl,2);
// ...
```



## random numbers 随机数

​		随机数在测试、游戏、模拟和安全等领域具有重要作用，标准库``<random>``提供的随机数生成器用来满足不同应用领域的需求。

​		一个随机数生成器由两部分组成:

- 产生随机或伪随机值序列的引擎
- 将这些值映射为一定范围内的数学分布的分布

​		分布的例子有``uniform_int_distribution``(生成的所有整数都是等可能的)、``normal_distribution``(“钟形曲线”)和``exponential_distribution``(指数增长);每个都有特定的范围。

```c++
using my_engine = default_random_engine;                 // type of engine
using my_distribution = uniform_int_distribution<>;      // type of distribution

my_engine eng {};                        // the default version of the engine
my_distribution dist {1,6};              // distribution that maps to the ints 1..6
auto die = [&](){ return dist(eng); };   // make a generator

int x = die();        // roll the die: x becomes a value in [1:6]
```

​		由于对通用性和性能的不懈关注，一位专家认为标准库中的随机数组件是“每个随机数库发展起来后想要成为的样子”。然而，它很难被认为是“新手友好”。using语句和lambda表达式使所做的事情更明显。

​		对于(任何背景的)新手来说，随机数库的完全通用接口可能是一个严重的障碍。一个简单的均匀随机数生成器通常就足以入门。

```c++
Rand_int rnd {1,10};        // make a random number generator for [1:10]
int x = rnd();                     // x is a number in [1:10]
```

​		那么，我们怎么得到这个呢?我们需要像die()那样，在类Rand_int中结合引擎和分布:

```c++
class Rand_int {
public:
        Rand_int(int low, int high) :dist{low,high} { }
        int operator()() { return dist(re); }             // draw an int
        void seed(int s) { re.seed(s); }                  // choose new random engine seed
private
        default_random_engine re;
        uniform_int_distribution<> dist;
};
```

​		这个定义仍然是“专家级别”的，但对于初学者来说，在c++课程的第一周，Rand_int()的使用是可以管理的。

```c++
int main()
{
         constexpr int max = 9;
         Rand_int rnd {0,max};                                // make a uniform random number generator

         vector<int> histogram(max+1);                 // make a vector of appropriate size
         for (int i=0; i!=200; ++i)
                 ++histogram[rnd()];                            // fill histogram with the frequencies of numbers [0:max]

         for (int i = 0; i!=histogram.size(); ++i) {    // write out a bar graph
                 cout << i << '\t' ;
                 for (int j=0; j!=histogram[i]; ++j) cout <<'*' ;
                 cout << '\n' ;
         }
}
```

​		c++没有标准的图形库，所以我使用“ASCII图形”。显然，有很多开源和商业的c++图形和GUI库，但在本书中，我只介绍ISO标准的工具。

​		为了得到重复或不同的值序列，我们对引擎进行``seed``操作;也就是说，我们给它的内部状态一个新值.

```c++
Rand_int rnd {10,20};
for (int i = 0; i<10; ++i) cout << rnd() << ' ';     // 16 13 20 19 14 17 10 16 15 14
cout << '\n';
rnd.seed(999);
for (int i = 0; i<10; ++i) cout << rnd() << ' ';     // 11 17 14 19 20 13 20 14 16 19
cout << '\n';
rnd.seed(999);
for (int i = 0; i<10; ++i) cout << rnd() << ' ';     // 11 17 14 19 20 13 20 14 16 19
cout << '\n';
```

​		重复序列对于确定性调试非常重要。当我们不想重复时，使用不同的值是很重要的。如果你需要真正的随机数，而不是生成的伪随机序列，请查看``random_device``在你的计算机上的实现。



## valarray

​		`std::vector`是一种通用的值保存机制，具有灵活性，适合于容器、迭代器和算法的架构。

​		但`std::vector`不支持数学向量操作。``std::vector``添加这些操作很容易，但通用性和灵活性降低，且没有数值优化。数值优化是数值工作的必要行为。标准库在``<valarray>``中提供了一个类似向量的模板，称为``valarray``，它不太通用，更适合用于数值计算的优化:

```c++
template<typename T>
class valarray {
         // ...
};
```

​		`valarray`支持常见的算术操作和最常用的数学函数。

```c++
valarray<double>& a1, valarray<double>& a2);
valarray<double> a = a1*3.14+a2/a1; // numeric array operators *, +, /, and =
a2 += a1*3.14;
a = abs(a);
double d = a2[7];
```

​		这些操作是向量操作;也就是说，它们作用于所涉及向量的每个元素。

​		除了算术运算之外，``valarray``还提供步幅访问来帮助实现多维计算。



## 数字限制

​		在``<limits>``中，标准库提供了描述内置类型属性的类——例如float的最大指数或int的字节数。例如，我们可以断言一个char类型是有符号的:

```c++
static_assert(numeric_limits<char>::is_signed,"unsigned characters!");
static_assert(100000<numeric_limits<int>::max(),"small ints!");
```

​		第二个断言(仅)起作用是因为``numeric_limits<int>::max()``是一个``constexpr``函数(§1.6)。

​		可以为自己的用户定义类型定义``numeric_limits``。



## 类型别名

​		基本类型的大小在不同的c++实现上可能是不同的。如果需要指定整数的大小，可以使用``<stdint>``中定义的别名，例如``int32_t``和``uint_least64_t``。后者表示至少有64位的无符号整数。

​		奇怪的``_t``后缀是C语言时代遗留下来的，当时有一个名字反映出它是一个别名是很重要的。

​		其他常见的别名，如`size_t` (`sizeof`操作符返回的类型)和`ptrdiff_t`(一个指针减去另一个指针的结果类型)可以在``<stddef>``中找到。



## Mathematical Constants 数学常数

​		在进行数学计算时，我们需要常见的数学常数，如``e``、``pi``和``log2e``。标准库提供了这些功能，甚至更多。它们有两种形式:一种是模板，允许指定精确类型(例如``pi_v<T>``)，另一种是最常用的简称(例如``pi``表示``pi_v<double>``)。例如:

```c++
void area(float r)
{
    using namespace std::numbers;  // this is where the mathematical constants are kept

    double d = pi*r*r;
    float f = pi_v<float>*r*r;
}
```

​		在这种情况下，差异很小(我们必须打印精度为16左右才能看到它)，但在实际物理计算中，这种差异很快就会变得显著。在图形和人工智能领域，常量的精度很重要，在这些领域，更小的值表示变得越来越重要。

​		在``<numbers>``中:

- ``e(欧拉数)``
- ``log2e (log2 of e)``
- ``log10e (log10 of e)``
- ``pi``
- ``inv_pi (1/pi)``
- ``inv_sqrtpi (1/sqrt(pi))``
- ``ln2``
- ``ln10``
- ``sqrt2 (sqrt(2))``
- ``sqrt3 (sqrt(3))``
- ``inv_sqrt3 (1/sqrt3)``
- ``gamma(欧拉-马切罗尼常数)``
- ``phi(黄金比例)``

​		当然，我们想要更多的数学常数和不同领域的常数。这很容易做到，因为这样的常量是对double(或对域最有用的任何类型)进行特化的变量模板:

```c++
template<typename T>
constexpr T tau_v = 2*pi_v<T>;

constexpr double tau = tau_v<double>;
```

