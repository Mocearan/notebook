
#	泛型编程
---
​		 c++直接支持的泛型编程思想, 围绕着从具体的、有效的算法中抽象出泛型算法。

- 算法与不同的数据表示相结合，从而产生各种有用的应用。
- 表示基本操作和数据结构的抽象称为概念。

## lifting

​		好的抽象是从具体的例子中发展出来的。不要试图为没有实际禅城的需求和技术准备抽象，这样会导致不优雅和代码膨胀。相反，应该从具体的实际例子中开始抽象，并尽量消除不必要的细节。

​		从一段具体代码(最好是几段代码)进行泛化，同时保持性能的过程称为提升（lifting）。相反，开发模板的最佳方法通常是：

- 首先，写一个具体的版本
- 然后，调试、测试和测量它
- 最后，用模板参数替换具体类型。



```c++
double sum(const vector<int>& v)
{
    double res = 0;
    for (auto x : v)
            res += x;
    return res;
}
```

​		考虑一下是什么让这个代码没有它需要的那么通用:

- Why just **int**s?
- Why just **vector**s?
- Why accumulate in a **double**?
- Why start at **0**?
- Why add?

​		通过回答前四个问题可以改进得到一个标准库中的`accumulate`

```c++
template<forward_iterator Iter, Arithmetic<iter_value_t<Iter>> Val>
Val accumulate(Iter first, Iter last, Val res)
{
        for (auto p = first; p!=last; ++p)
                res += *p;
        return res;
}
```

- The data structure to be traversed has been abstracted into a pair of iterators representing a sequence (§[8.2.4](ch08.xhtml#sec8_2_4), §[13.1](ch13.xhtml#sec13_1)).
- The type of the accumulator has been made into a parameter.
- The type of the accumulator must be arithmetic .
- The type of the accumulator must work with the iterator’s value type (the element type of the sequence).
- The initial value is now an input; the type of the accumulator is the type of this initial value.

​		自然，重复begin()和end()是乏味的，所以我们可以简化一点用户界面:

```c++
template<forward_range R, Arithmetic<value_type_t<R>> Val>
Val accumulate(const R& r, Val res = 0)
{
        for (auto x : r)
                res += x;
        return res;
}
```

​		`range`是一个标准库概念，表示一个包含``begin()``和``end()``的序列。为了充分的通用性，我们还可以抽象``+=``运算。

​		`accumulate()`的成对迭代器版本和范围版本都很有用:成对迭代器版本用于一般用途，范围版本用于简化常用用途。



## CRTP

[c++中如何避免重复写大量相似的虚函数_百度搜索 (baidu.com)](https://www.baidu.com/s?ie=UTF-8&wd=c%2B%2B中如何避免重复写大量相似的虚函数)

[(7 封私信 / 81 条消息) C++中如何避免重复写大量相似的虚函数？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/564819820)

[crtp_百度搜索 (baidu.com)](https://www.baidu.com/s?ie=UTF-8&wd=crtp)

[CRTP介绍、使用和原理 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/476001202)
