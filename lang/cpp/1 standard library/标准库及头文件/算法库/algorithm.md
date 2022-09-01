# Algorithm

---

​		standard library provide some functions that come pre-tested, are efficient, work on a variety of different container types, and many support parallelization (the ability to devote multiple CPU threads to the same task in order to complete it faster).

​		The functionality provided in the algorithms library generally fall into one of three categories:

- **Inspectors** -- Used to view (but not modify) data in a container. Examples include searching and counting.
- **Mutators** -- Used to modify data in a container. Examples include sorting and shuffling.
- **Facilitators** -- Used to generate a result based on values of the data members. Examples include objects that multiply values, or objects that determine what order pairs of elements should be sorted in.



​		Note that most of the algorithms in the algorithms library do not guarantee a particular order of execution. For such algorithms, take care to ensure any functions you pass in do not assume a particular ordering, as the order of invocation may not be the same on every compiler.

​		The following algorithms do guarantee sequential execution: `std::for_each`, `std::copy`, `std::copy_backward`, `std::move`, and `std::move_backward`.



## `std::find`

​		searches for the first occurrence of a value in a container.

```c++
#include <algorithm>
#include <array>
#include <iostream>

int main()
{
    std::array arr{ 13, 90, 99, 5, 40, 80 };

    std::cout << "Enter a value to search for and replace with: ";
    int search{};
    int replace{};
    std::cin >> search >> replace;

    // Input validation omitted

    // std::find returns an iterator pointing to the found element (or the end of the container)
    // we'll store it in a variable, using type inference to deduce the type of
    // the iterator (since we don't care)
    auto found{ std::find(arr.begin(), arr.end(), search) };

    // Algorithms that don't find what they were looking for return the end iterator.
    // We can access it by using the end() member function.
    if (found == arr.end())
    {
        std::cout << "Could not find " << search << '\n';
    }
    else
    {
        // Override the found element.
        *found = replace;
    }

    for (int i : arr)
    {
        std::cout << i << ' ';
    }

    std::cout << '\n';

    return 0;
}
```



## std::find_if

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



## std::count / std::count_if

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



## std::sort

​		There’s a version of `std::sort` that takes a function as its third parameter that allows us to sort however we like. The function takes two parameters to compare, and returns true if the first argument should be ordered before the second. 

​		By default, `std::sort` sorts the elements in ascending order.

```c++
#include <algorithm>
#include <array>
#include <iostream>

bool greater(int a, int b)
{
    // Order @a before @b if @a is greater than @b.
    return (a > b);
}

int main()
{
    std::array arr{ 13, 90, 99, 5, 40, 80 };

    // Pass greater to std::sort
    std::sort(arr.begin(), arr.end(), greater);

    for (int i : arr)
    {
        std::cout << i << ' ';
    }

    std::cout << '\n';

    return 0;
}
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



## for_each

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