# Algorithm

---

​		数据结构本身的作用只是存储数据，而基于数据结构的操作（插入删除访问等）。

​		基本人们对数据的使用方式，向数据结构中持续存储对象的操作比较少。相反基于当前数据结构中已有数据进行数据的组织如：排序、打印、提取子集、删除元素、搜索对象等更为常见。

​		因此，标准库提供容器上最常见的算法。

​		

​		STL provide some functions that come pre-tested, are efficient, work on a variety of different container types, and many support parallelization (the ability to devote multiple CPU threads to the same task in order to complete it faster).

​		In addition to container classes and iterators, STL also provides a number of generic algorithms for working with the elements of the container classes. These allow you to do things like search, sort, insert, reorder, remove, and copy elements of the container class.

​		The functionality provided in the algorithms library generally fall into one of three categories:

- **Inspectors** -- Used to view (but not modify) data in a container. Examples include searching and counting.
- **Mutators** -- Used to modify data in a container. Examples include sorting and shuffling.
- **Facilitators** -- Used to generate a result based on values of the data members. Examples include objects that multiply values, or objects that determine what order pairs of elements should be sorted in.



​		Note that most of the algorithms in the algorithms library do not guarantee a particular order of execution. For such algorithms, take care to ensure any functions you pass in do not assume a particular ordering, as the order of invocation may not be the same on every compiler.

​		The following algorithms do guarantee sequential execution: `std::for_each`, `std::copy`, `std::copy_backward`, `std::move`, and `std::move_backward`.



## 算法与迭代器

​		STL提供的通用算法，通过迭代器和容器粘合。算法往往通过容器的一对迭代器来访问容器，或通过迭代器来返回有效元素。

​		迭代器用于分离算法和容器。这种数据存储和算法分离的模型提供了通用性和和灵活性。

- 算法通过迭代器对其数据进行操作，对存储元素的容器一无所知。
- 容器对操作其元素的算法一无所知;它所做的只是根据请求提供迭代器

```c++
bool has_c(const string& s, char c)  // does s contain the character c?
{
	return find(s,c)!=s.end();
}

vector<string::iterator> find_all(string& s, char c)  // find all occurrences of c in s
{
      vector<char*> res;
      for (auto p = s.begin(); p!=s.end(); ++p)
           if (*p==c) res.push_back(&*p);
      return res; // move
}

string m {"Mary had a little lamb"};
for (auto p : find_all(m, a ))
  	if (*p!= a ) cerr << "a bug!\n";
```

>  ​		虽然这非常强大，可以组织复杂代码，但也有不好的一面：算法和容器类型的某些组合可能不起作用，可能会导致无限循环，或者可能会起作用，但性能非常差。所以使用这些，你要承担风险。

​		迭代器在每个不同的容器上的使用方式是等效一致的，因此可以泛化一些迭代器的操作：

```c++
template<typename C, typename V>
vector<typename C::iterator> find_all(C& c, V v)           // find all occurrences of v in c
	// 在上下文中未明确声明为类型的词，需要使用 typename 向编译器提示为类型，否则视为值
{
     vector<typename C::iterator> res;
     for (auto p = c.begin(); p!=c.end(); ++p)
         if (*p==v) res.push_back(p);
     return res;
}

// 或者用一般的指针来代替迭代器
template<typename C, typename V>
auto find_all(C& c, V v)           // find all occurrences of v in c
{
    vector<range_value_t<C>*> res;
    for (auto& x : c) if (x==v) res.push_back(&x);
    return res;
}
```

> 使用range-for循环和标准库``range_value_t``来命名元素的类型
>
> ```c++
> template<typename T>
> using range_value_type_t = T::value_type;
> ```

```c++
 string m {"Mary had a little lamb"};

 for (auto p : find_all(m, a ))                              // p is a string::iterator
  if (*p!= a ) cerr << "string bug!\n";

 list<int> ld {1, 2, 3, 1, -11, 2};
 for (auto p : find_all(ld,1))                               // p is a list<int>::iterator
  if (*p!=1) cerr << "list bug!\n";

 vector<string> vs {"red", "blue", "green", "green", "orange", "green"};
 for (auto p : find_all(vs,"red"))                  // p is a vector<string>::iterator
  	if (*p!="red") cerr << "vector bug!\n";

 for (auto p : find_all(vs,"green")) *p = "vert";
```





​		标准库不提供抽象来支持范围检查式写入容器，可以自行定义一个：

```c++
template<typename C>
class Checked_iter {
public:
     using value_type = typename C::value_type;
     using difference_type = int;

     Checked_iter() { throw Missing_container{}; } // concept forward_iterator requires a default constructor
     Checked_iter(C& cc) : pc{ &cc } {}
     Checked_iter(C& cc, typename C::iterator pp) : pc{ &cc }, p{ pp } {}

     Checked_iter& operator++() { check_end(); ++p; return *this; }
     Checked_iter operator++(int) { check_end(); auto t{ *this }; ++p; return t; }
     value_type& operator*() const { check_end(); return *p; }

     bool operator==(const Checked_iter& a) const { return p==a.p; }
     bool operator!=(const Checked_iter& a) const { return p!=a.p; }
private:
     void check_end() const { if (p == pc->end()) throw Overflow{}; }
     C* pc {}; // default initialize to nullptr
     typename C::iterator p = pc->begin();
};

// 它的质量不足以进入标准库，但它提供了一个思路：
vector<int> v1 {1, 2, 3};            // three elements
vector<int> v2(2);                   // two elements

copy(v1,v2.begin());                   // will overflow
copy(v1,Checked_iter{v2});             // will throw
```

​		通过成对的迭代器来使用容器算法比较繁琐（``sort(vc .begin()， vc .end())``），可以使用`range`版本的算法（``sort(vec)``）。这两个版本是等价的。除了更简单和更不容易出错之外，`range`版本通常也更高效。``range-for``循环大致相当于直接使用迭代器的循环:

```c++
for (auto& x : v) cout<<x;   // write out all elements of v
for (auto p = v.begin(); p!=v.end(); ++p) cout<<*p;  // write out all elements of v
```



## `std::find / std::find_if`

​		searches for the first occurrence of a value in a container.

```c++
#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>

int main()
{
    std::list<int> li(6);
    std::iota(li.begin(), li.end(), 0);

    // Find the value 3 in the list
    auto it{ std::find(li.begin(), li.end(), 3) };

    // Insert 8 right before 3.
    li.insert(it, 8);

    for (int i : li) // for loop with iterators
        std::cout << i << ' ';

    std::cout << '\n';

    return 0;
}
```





​		The `std::find_if` function works similarly to `std::find`, but instead of passing in a value to search for, we pass in a callable object, such as a function pointer (or a lambda, which we’ll cover later) that checks to see if a match is found. 

​		`std::find_if` will call this function for every element until a matching element is found (or no more elements remain in the container to check).

```c++
#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

// Our function will return true if the element matches
bool containsNut(std::string_view str)
{
    // std::string_view::find returns std::string_view::npos if it doesn't find
    // the substring. Otherwise it returns the index where the substring occurs
    // in str.
    return (str.find("nut") != std::string_view::npos);
}

int main()
{
    std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };

    // Scan our array to see if any elements contain the "nut" substring
    auto found{ std::find_if(arr.begin(), arr.end(), containsNut) };

    if (found == arr.end())
    {
        std::cout << "No nuts\n";
    }
    else
    {
        std::cout << "Found " << *found << '\n';
    }

    return 0;
}
```



## `std::count / std::count_if`

```c++
#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

bool containsNut(std::string_view str)
{
	return (str.find("nut") != std::string_view::npos);
}

int main()
{
	std::array<std::string_view, 5> arr{ "apple", "banana", "walnut", "lemon", "peanut" };

	auto nuts{ std::count_if(arr.begin(), arr.end(), containsNut) };

	std::cout << "Counted " << nuts << " nut(s)\n";

	return 0;
}
```



## `std::copy / std::unique_copy`

```c++
void f(vector<Entry>& vec, list<Entry>& lst)
{
     unique_copy(vec.begin(),vec.end(),lst.begin());         // don't copy adjacent equal elements
}

list<Entry> f(vector<Entry>& vec)
{
     list<Entry> res;
     unique_copy(vec.begin(),vec.end(),back_inserter(res));           // append to res
     return res;
}
```

​		调用``back_inserter(res)``为``res``构造了一个迭代器，在容器的末尾添加元素，使容器扩容。因此，标准容器加上``back_inserter()``消除了使用容易出错的``realloc()``进行显式c风格内存管理。

​		标准库`std::list`有一个移动构造函数，使得按值返回``res``有效优化了大对象拷贝从而进行移动。

## `std::sort`

​		There’s a version of `std::sort` that takes a function as its third parameter that allows us to sort however we like. The function takes two parameters to compare, and returns true if the first argument should be ordered before the second. 

​		By default, `std::sort` sorts the elements in ascending order.

```c++
bool greater(int a, int b)
{
    // Order @a before @b if @a is greater than @b.
    return (a > b);
}

std::array arr{ 13, 90, 99, 5, 40, 80 };
// Pass greater to std::sort
std::sort(arr.begin(), arr.end(), greater);
for (int i : arr)
    std::cout << i << ' ';
std::cout << '\n';
```

​		Because sorting in descending order is so common, C++ provides a custom type (named `std::greater`) for that too (which is part of the [functional](https://en.cppreference.com/w/cpp/header/functional) header). In the above example, we can replace:

```cpp
std::sort(arr.begin(), arr.end(), greater); // call our custom greater function

std::sort(arr.begin(), arr.end(), std::greater{}); // use the standard library greater comparison
// Before C++17, we had to specify the element type when we create std::greater
std::sort(arr.begin(), arr.end(), std::greater<int>{}); // use the standard library greater comparison
```

​		其原理是：

```c++
void sort(int *begin, int *end, std::function<bool(int, int)> compare);

// sort accepts a comparison function
void sort(int* begin, int* end, std::function<bool(int, int)> compare)
{
    for (auto startElement{ begin }; startElement != end; ++startElement)
    {
        auto smallestElement{ startElement };

        for (auto currentElement{ std::next(startElement) }; currentElement != end; ++currentElement)
        {
            // the comparison function is used to check if the current element should be ordered
            // before the currently "smallest" element.
            if (compare(*currentElement, *smallestElement))
            {
                smallestElement = currentElement;
            }
        }

        std::swap(*startElement, *smallestElement);
    }
}
// use std::greater to sort in descending order
// (We have to use the global namespace selector to prevent a collision
// between our sort function and std::sort.)
::sort(std::begin(array), std::end(array), std::greater{});
```



## std::reverse

​		翻转容器中的元素

```c++
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> vect{ 7, -3, 6, 2, -5, 0, 4 };

    // sort the vector
    std::sort(vect.begin(), vect.end());

    for (int i : vect)
    {
        std::cout << i << ' ';
    }

    std::cout << '\n';

    // reverse the vector
    std::reverse(vect.begin(), vect.end());

    for (int i : vect)
    {
        std::cout << i << ' ';
    }

    std::cout << '\n';

    return 0;
}
```



## std::for_each

​			takes a list as input and applies a custom function to every element. 

```c++
void doubleNumber(int& i)
{
    i *= 2;
}

std::array arr{ 1, 2, 3, 4 };

std::for_each(arr.begin(), arr.end(), doubleNumber);

for (int i : arr)
{
  std::cout << i << ' ';
}
```

​		 compare `std::for_each` to a range-based for-loop:

```c++
std::ranges::for_each(arr, doubleNumber); // Since C++20, we don't have to use begin() and end().
// std::for_each(arr.begin(), arr.end(), doubleNumber); // Before C++20

for (auto& i : arr)
{
    doubleNumber(i);
}
```

> ​		In the range-based for-loop, we have to add a new variable, `i`.
>
> ​		We could forget the ampersand, and `doubleNumber` wouldn’t affect the array. 
>
> ​		`std::for_each` can skip elements at the beginning or end of a container, for example to skip the first element of `arr`, [`std::next`](https://en.cppreference.com/w/cpp/iterator/next) can be used to advance begin to the next element.
>
> ```c++
> std::for_each(std::next(arr.begin()), arr.end(), doubleNumber);
> // Now arr is [1, 4, 6, 8]. The first element wasn't doubled.
> ```
>
> ​		Like many algorithms, `std::for_each` can be parallelized to achieve faster processing, making it better suited for large projects and big data than a range-based for-loop.



## std::min_element / std::max_element

​		查找容器中最大或最小的元素。



## std::iota 

​		生成一系列连续的值

```c++
#include <algorithm> // std::min_element and std::max_element
#include <iostream>
#include <list>
#include <numeric> // std::iota

int main()
{
    std::list<int> li(6);
    // Fill li with numbers starting at 0.
    std::iota(li.begin(), li.end(), 0);

    std::cout << *std::min_element(li.begin(), li.end()) << ' '
              << *std::max_element(li.begin(), li.end()) << '\n';

    return 0;
}
```

