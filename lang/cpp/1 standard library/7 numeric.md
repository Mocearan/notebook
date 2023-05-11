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