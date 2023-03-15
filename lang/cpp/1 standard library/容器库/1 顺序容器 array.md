# std::array

​		`std::array` provides fixed array functionality that won’t decay when passed into a function. 

​		`std::array` is defined in the `<array>` header, inside the `std` namespace.

---





## 初始化

```c++

#include <array>

std::array<int, 3> myArray; // declare an integer array with length 3
std::array<int, 5> myArray = { 9, 7, 5, 3, 1 }; // initializer list
std::array<int, 5> myArray2 { 9, 7, 5, 3, 1 }; // list initialization
```

​		the length of a `std::array` must be known at compile time. 	

​		Unlike built-in fixed arrays, with std::array you can not omit the array length when providing an initializer:

```c++
std::array<int, > myArray { 9, 7, 5, 3, 1 }; // illegal, array length must be provided
std::array<int> myArray { 9, 7, 5, 3, 1 }; // illegal, array length must be provided
```

​		在c++17后，做出了一定的优化，能够在同时省略类型和`length`的情况下通过初始化列表来推导数组：

```c++
std::array myArray { 9, 7, 5, 3, 1 }; // The type is deduced to std::array<int, 5>
std::array myArray { 9.7, 7.31 }; // The type is deduced to std::array<double, 2>
```

​		在c++17后更推荐这种写法。

​		c++20提供了`std::to_array<>()`来提供单独针对于`length`的单独推断，但是代价是非原地构造（需要先构造一个传统数组，再逐一拷贝），应尽量避免在多次构造场景下这样使用，如循环中创建。

```c++
auto myArray1 { std::to_array<int, 5>({ 9, 7, 5, 3, 1 }) }; // Specify type and size
auto myArray2 { std::to_array<int>({ 9, 7, 5, 3, 1 }) }; // Specify type only, deduce size
auto myArray3 { std::to_array({ 9, 7, 5, 3, 1 }) }; // Deduce type and size
```

> 也许可以通过`std::move()`来进行优化？

### 复合元素类型的聚合初始化

​		首先指出，`std::array`类似于类类型，如下：

```c++
template<typename T, std::size_t size>
struct array {
    T values[size];
}
```

​		对于类似结构体嵌套数组的结构，由C而来的有一个特性：会按照内存顺序解开初始化列表中用以分隔元素的第一层花括号，对结构体内存进行紧凑初始化：

```c++
struct S
{
  int arr[3]{};
  int i{};
};
// These two do the same
S s1{ { 1, 2, 3 }, 4 };
S s2{ 1, 2, 3, 4 };
```

​		因此，对于`std::array<struct,n>`这样的结构，需要多加一层花括号来实现元素分隔。

```c++
struct House
{
    int number{};
    int stories{};
    int roomsPerStory{};
};

struct Array
{
    // This is now an array of House
    House value[3]{};
};

// This is wrong
Array houses{
    { 13, 4, 30 }, // value
    { 14, 3, 10 }, // ???
    { 15, 3, 40 }  // ???
};

// To initialize all houses, we need to do so in the first pair of braces.
Array houses{
    { 13, 4, 30, 14, 3, 10, 15, 3, 40 }, // value
};
// If we add braces around each element of the array, the initialization is a lot easier to read.
// With braces, this works.
Array houses{
    { { 13, 4, 30 }, { 14, 3, 10 }, { 15, 3, 40 } }
};x
```







## 属性

### `size`

​		`size_type` isn’t a global type (like `int` or `std::size_t`). Rather, it’s defined inside the definition of `std::array` (C++ allows nested types). `std::array::size_type` is just an alias for `std::size_t`, so we can use that instead.

```c++
std::cout << "length: " << myArray.size() << '\n';

for (std::size_t i{ 0 }; i < myArray.size(); ++i)
        std::cout << myArray[i] << ' ';


// Print the array in reverse order.
for (auto i{ myArray.size() }; i-- > 0; )
    std::cout << myArray[i] << ' ';
```

> `std::array<int, 5>::size_typ`
>
> `std::array` doesn’t decay to a pointer when passed to a function, the `size()` function will work.
>
> do not get this confused with the results of `sizeof()` on a native fixed array, which returns the actual size of the array in memory (the size of an element multiplied by the array length). Yes, this nomenclature is inconsistent.

​		



## 访问

### 索引访问

```c++
std::cout << myArray[1] << '\n';
```

> the subscript operator does not do any bounds-checking.the subscript operator does not do any bounds-checking.

### `at`

​		索引成功返回元素的引用，索引失败抛出`std::out_of_range`异常。

```c++
myArray.at(1) = 6; // array element 1 is valid, sets array element 1 to value 6
myArray.at(9) = 10; // array element 9 is invalid, will throw a runtime error
```

> does bounds checking。
>
> Because it does bounds checking, `at()` is slower (but safer) than `operator[]`.



## 赋值

### 聚合赋值

​		assign values to the array using an initializer list

```c++
std::array<int, 5> myArray;
myArray = { 0, 1, 2, 3, 4 }; // okay
myArray = { 9, 8, 7 }; // okay, elements 3 and 4 are set to zero!
myArray = { 0, 1, 2, 3, 4, 5 }; // not allowed, too many elements in initializer list!
```

### 索引赋值

```c++
myArray[2] = 6;
```





## 应用

### 传参

​		prevent the compiler from making a copy of the `std::array` when the `std::array` was passed to the function (for performance reasons).Always pass `std::array` by reference or `const` reference

```c++

void printLength(const std::array<double, 5>& myArray)
```

​		We can create a template function that parameterizes part or all of the type information, Passing std::array of different lengths to a function:

```c++
// printArray is a template function
template <typename T, std::size_t size> // parameterize the element type and size
void printArray(const std::array<T, size>& myArray)
{
    for (auto element : myArray)
        std::cout << element << ' ';
    std::cout << '\n';
}


std::array myArray5{ 9.0, 7.2, 5.4, 3.6, 1.8 };
printArray(myArray5);

std::array myArray7{ 9.0, 7.2, 5.4, 3.6, 1.8, 1.2, 0.7 };
printArray(myArray7);
```



### 遍历

​		Because the length is always known, range-based for-loops work with `std::array`

```c++
std::array myArray{ 9, 7, 5, 3, 1 };

for (int element : myArray)
    std::cout << element << ' ';
```



### 排序

​		using `std::sort`, which lives in the `<algorithm>` header:

```c++
std::array myArray { 7, 3, 1, 9, 5 };
    std::sort(myArray.begin(), myArray.end()); // sort the array forwards
//  std::sort(myArray.rbegin(), myArray.rend()); // sort the array backwards
```



# std::vector

​		`std::vector` provides dynamic array functionality that handles its own memory management. 

​		`std::vector` lives in the `<vector>` header.

---



## 初始化

```c++
#include <vector>

// no need to specify length at the declaration
std::vector<int> array;
std::vector<int> array2 = { 9, 7, 5, 3, 1 }; // use initializer list to initialize array (before C++11)
std::vector<int> array3 { 9, 7, 5, 3, 1 }; // use uniform initialization to initialize array

// as with std::array, the type can be omitted since C++17
std::vector array4 { 9, 7, 5, 3, 1 }; // deduced to std::vector<int>
```

### `std::vector<bool>`

​		`std::vector<bool>`在内部实现上，会将原本一个字节的`bool`类型，压缩用单个``bit``来存储，每8个值占用一个字节。

```c++
std::vector<bool> array { true, false, false, true, true };
std::cout << "The length is: " << array.size() << '\n';

for (int i : array)
  std::cout << i << ' ';
```



## 属性

### capacity

​		Unlike a built-in array or a std::array, which only remembers its length, std::vector contains two separate attributes: length and capacity. In the context of a std::vector, **length** is how many elements are being used in the array, whereas **capacity** is how many elements were allocated in memory.

```c++
std::vector<int> array { 0, 1, 2 };
array.resize(5); // set length to 5
// 0 1 2 0 0

std::vector<int> array{};
array = { 0, 1, 2, 3, 4 }; // okay, array length = 5
std::cout << "length: " << array.size() << "  capacity: " << array.capacity() << '\n';

array = { 9, 8, 7 }; // okay, array length is now 3!
std::cout << "length: " << array.size() << "  capacity: " << array.capacity() << '\n';
//  What happens if we try to access the array element with index 4? The answer is that it fails, since 4 is greater than the length of the array.
```

​		When a vector is resized, the vector may allocate more capacity than is needed. This is done to provide some “breathing room” for additional elements, to minimize the number of resize operations needed.

```c++
std::vector<int> v{ 0, 1, 2, 3, 4 };
std::cout << "size: " << v.size() << "  cap: " << v.capacity() << '\n';

v.push_back(5); // add another element
std::cout << "size: " << v.size() << "  cap: " << v.capacity() << '\n';
```





### size

​		 std::vector keeps track of its length.

```c++
std::cout << "The length is: " << array.size() << '\n';
```

> `size()` returns a value of nested type `size_type` (full type in the above example would be `std::vector<int>::size_type`), which is an unsigned integer.

### resize

```c++
std::vector array { 0, 1, 2 };
array.resize(5); // set size to 5

std::vector array { 0, 1, 2, 3, 4 };
array.resize(3); // set length to 3
```

> First, when we resized the vector, the existing element values were preserved!
>
> Second, new elements are initialized to the default value for the type (which is 0 for integers).

​		Resizing a vector is computationally expensive, so you should strive to minimize the number of times you do so.

## 访问

### 索引

```c++
array[6] = 2; // no bounds checking
```

> `[]` operator (which does no bounds checking) 

### at

```c++
array.at(7) = 3; // does bounds checking
```

>  `at()` function (which does bounds checking):



## 赋值

### 聚合赋值

​		assign values to the array using an initializer list

```c++
array = { 0, 1, 2, 3, 4 }; // okay, array length is now 5
array = { 9, 8, 7 }; // okay, array length is now 3
```

### 索引赋值

```c++
array[2] = 6;
```



## 增删

### push_back

​		pushes an element rear of the vector.

### back

​		 returns the value of the rear element of the vector.

### pop_back

​		pops the rear element off the vector.
