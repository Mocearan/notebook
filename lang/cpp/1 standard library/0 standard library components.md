# standard library 

---



## components

- 运行时语言支持 Run-time language supoort
  - allocation / exception / run-time type information
- C标准库 The C standard library 
  - 非常小的修改，以尽量减少类型系统的冲突
- 国际字符集、本地化和子字符串只读视图的字符串
- 正则表达式匹配 regular experssion match
- 可扩展输入输出的I/O流框架
  - 用户可以在其中添加自己的类型、流、缓冲策略、区域设置和字符集（第11章）
  - 还提供了灵活的输出格式设置功能
- 以可移植方式进行文件系统操作
- 容器框架及容器上的算法 STL
  - 用户可以添加自己的容器和算法进行扩展
- 范围（Range） 
  - view / generator / pipe
- 基本类型和范围的概念
- 支持数值计算
  - 标准数学函数 / 复数 / 带算数运算的矢量 / 数学常数和随机数生成器
- 并发编程 concurrent programming
  - threads / locks
  - 提供基础的并发支持，用户可以添加新的模型
- 同步和异步的协程 coroutines
- 绝大多数STL算法和数值算法的并发版本
  - sort / reduce ...
- 元编程（metaprogramming）、STL风格的泛型编程、泛型编程支持工具
  - type functions
  - pair...
  - variant / optional
- 用于资源管理的智能指针
  - unique_ptr / shared_ptr
- 特定目的的容器
  - array / bitset / tuple
- 时间和时段的抽象
  - time_point / system_clock
- 日期支持
  - month / time_zone
- 常用单位的后缀
  - ms for milliseconds / i for imaginary
- 操作元素序列
  - views / string_view / spans



## Organization

​		标准库的设施被放置在命名空间``std``中，并通过模块或头文件向用户提供。

### `namespace std`

​		标准库定义在`namespace std`中，使用标准库设施时要使用或引入`std`。

```c++
std::string sheep {"Four legs Good; two legs Baaad!"};
std::list<std::string> slogans {"War is Peace", "Freedom is Slavery", "Ignorance is Strength"};
```

```c++
using namespace std;            // make std names available without std:: prefix
string s {"C++ is a general-purpose programming language"};      // OK: string is std::string
```

> `using namespace std; ` 称为`dump`，将名字空间的名字转储到`global namespace`。

​		标准库在`std`中提供了一些子名字空间：

- `std::chrono`
- `std::literals`
  - `std::literals::chrono_literals`
    -  y for year / d for day / h for hours / min for minutes / ms for milliseconds / ns for nanoseconds / s for seconds / us for microseconds
  - `std::literals::complex_literals`
    - i for imaginary doubles / if for imaginary floats / il for imaginary long doubles
  - `std::literals::string_literals`
    - s for strings
  - `std::literals::string_view_literals`
    - sv for string views
  - `std::numbers` 
    - 数学常数
  - `std::pmr`
    - 多态内存资源

​		要使用子命名空间中的后缀，我们必须将其引入到要使用它的命名空间中。

```c++
// no mention of complex_literals
auto z1 = 2+3i;          // error: no suffix 'i'

using namespace literals::complex_literals; // make the complex literals visible
auto z2 = 2+3i;          // ok: z2 is a complex<double>
```

​		后缀不能显式限定，因此我们只能在一个范围内引入一组后缀，而不会有歧义的风险。因此，用于与其他库一起工作的库的后缀被放置在子名称空间中。可以定义自己的后缀。

​		基于`range`和基于一对迭代器的容器算法，并不能完美的以重载进行判别：

```c++
using namespace std;
using namespace ranges;

void f(vector<int>& v)
{
    sort(v.begin(),v.end());       // error:ambiguous
	sort(v);                      // error: ambiguous
}
```

​		为了在使用传统的无约束模板时防止歧义，标准要求我们显式地将标准库算法的范围版本引入作用域:

```c++
using namespace std;

void g(vector<int>& v)
{
    sort(v.begin(),v.end());     // OK
    sort(v);                 	 // error: no matching function (in std)
    ranges::sort(v);             // OK
    
    using ranges::sort;          // sort(v) OK from here on
    sort(v);                     // OK
}
```



### `module`

​		目前还没有任何标准库模块。c++ 23可能会弥补这一遗漏。

​		可能成为标准的``std``模块，它提供来自命名空间``std``的所有功能。

### `header file`

​		每个标准库设施通过一些标准头文件来提供，下面是标准库标头的选择，它们都在命名空间std中提供声明,这个清单远远不够完整

|                   | **Selected Standard Library Headers**                        |
| :---------------- | ------------------------------------------------------------ |
| `<algorithm>`     | **copy()**, **find()**, **sort()**                           |
| `<array>`         | **array**                                                    |
| `<chrono>`        | **duration**, **time_point**, **month**, **time_zone**       |
| `<cmath>`         | **sqrt()**, **pow()**                                        |
| `<complex>`       | **complex**, **sqrt()**, **pow()**                           |
| `<concepts>`      | **floating_point**, **copyable**, **predicate**, **invocable** |
| `<filesystem>`    | **path**                                                     |
| `<format>`        | **format()**                                                 |
| `<fstream>`       | **fstream**, **ifstream**, **ofstream**                      |
| `<functional>`    | **function**, **greater_equal**, **hash**, **range_value_t** |
| `<future>`        | **future**, **promise**                                      |
| `<ios>`           | **hex**, **dec**, **scientific**, **fixed**, **defaultfloat** |
| `<iostream>`      | **istream**, **ostream**, **cin**, **cout**                  |
| `<map>`           | **map**, **multimap**                                        |
| `<memory>`        | **unique_ptr**, **shared_ptr**, **allocator**                |
| `<random>`        | **default_random_engine**, **normal_distribution**           |
| `<ranges>`        | **sized_range**, **subrange**, **take()**, **split()**, **iterator_t** |
| `<regex>`         | **regex**, **smatch**                                        |
| `<string>`        | **string**, **basic_string**                                 |
| `<string_view>`   | **string_view**                                              |
| `<set>`           | **set**, **multiset**                                        |
| `<sstream>`       | **istringstream**, **ostringstream**                         |
| `<stdexcept>`     | **length_error**, **out_of_range**, **runtime_error**        |
| `<tuple>`         | **tuple**, **get<>()**, **tuple_size<>**                     |
| `<thread>`        | **thread**                                                   |
| `<unordered_map>` | **unordered_map**, **unordered_multimap**                    |
| `<utility>`       | **move()**, **swap()**, **pair**                             |
| `<variant>`       | **variant**                                                  |
| `<vector>`        | **vector**                                                   |

​		来自C标准库的头文件，例如``<stdlib.h>``。对于每个这样的头文件，还有一个版本的名称前缀为``c``，删除了``.h``。这个版本，例如``<cstdlib>``，将其声明放在``std``和`global namespace`中。

​		`headers`反映了标准库的发展历史。因此，它们并不总是像我们所希望的那样具有逻辑性和容易记忆。这是使用`std`等模块的原因之一。