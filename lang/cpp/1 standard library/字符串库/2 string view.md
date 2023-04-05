## basic_string_view

​		字符序列上进行的最常见的操作是作为参数传递给一个函数去读，可以是pass by value，pass by reference 或者传递C风格的字符串指针。

​		在许多系统中，还提供其他标准库没有的字符串类型。

​		拷贝或初始化一个`std::string`是比较慢的，因为涉及到独立内存空间的构建。

​		在所有这些情况下，传递子字符串时，会有额外的复杂性。为此标准库提供了``string_view``，表示字符序列的一对指针和长度。

```c++
std::string_view => {std::stirng::begin(), std::string::size()}
							↓
                              |H|e|l|l|o| |w|o|r|l|d|\0|
```

​		`std::string_view`提供了对已经分配存储的字符串(包括 C风格字符串字面值， `std::string`，或``char``数组)的只读访问，而无需复制。它不拥有它所指向的字符，是`std::string`的观察者模式。

>  ​		可以将``std::string_view``视为改进的``const std::string&``：
>
>  - 它可以用于以多种不同方式存储的字符序列
>  - 可以很容易地传递一个子字符串
>  - 不需要创建一个字符串来传递c风格的字符串参数
>
>  ​        `std::string_view`的一个重要限制是它是某个字符序列的只读视图。
>
>  ​		例如，不能使用``string_view``将字符序列传递给将参数修改为小写的函数。为此，可以考虑使用``std::span``

```c++
std::string_view text{ "hello" }; // view the text "hello", which is stored in the binary
std::string_view str{ text }; // view of the same "hello"
std::string_view more{ str }; // view of the same "hello"

/////////////////////////////////////////////////////////////////////

string cat(string_view sv1, string_view sv2)
{
     string res {sv1};           // initialize from sv1
     return res += sv2;        // append from sv2 and return
}

string king = "Harold";
auto s1 = cat(king,"William");                          // HaroldWilliam: string and const char*
auto s2 = cat(king,king);                                   // HaroldHarold: string and string
auto s3 = cat("Edward","Stephen"sv);           // EdwardStephen: const char * and string_view
auto s4 = cat("Canute"sv,king);                      // CanuteHarold
auto s5 = cat({&king[0],2},"Henry"sv);           // HaHenry
auto s6 = cat({&king[0],2},{&king[2],4});         // Harold
```

> no more copies of the string “hello” are created. The string “hello” is stored in the binary and is not allocated at run-time.

​		`string_view`定义了一个范围，因此我们可以遍历它的字符。

```c++
void print_lower(string_view sv1)
{
    for (char ch : sv1)
        cout << tolower(ch);
}
```



​		When we copy a `std::string_view`, the new `std::string_view` observes the same string as the copied-from `std::string_view` is observing. 

```c++
#include <iostream>
#include <string>
#include <string_view>

void print(std::stirng_view str) // access “Hello, world!” through str, without making a copy of the string.
{
    std::cout << str << '\n';
}

int main()
{
    std::string_view s{"Hello, world!"}; // initialize std::string_view s with C-style string literal, provides read-only access, without making a copy of the string. 
    print(s);
}
```

> no copies of the string “Hello, world!” are made.

​		Prefer `std::string_view` over `std::string` when you need a read-only string, especially for function parameters.

​		Unlike `std::string`, `std::string_view` has full support for constexpr:

```c++
constexpr std::string_view s{ "Hello, world!" };
```

​		A `std::string_view` can be created using a `std::string` initializer, and a `std::string` will implicitly convert to a `std::string_view`:

```c++
std::string s{ "Hello, world" };
std::string_view sv{ s }; // Initialize a std::string_view from a std::string

printSV(s); // implicitly convert a std::string to std::string_view
```

​		`std::string_view` won’t allow implicit convert to a `std::string`, but explicit create a `std::string` with a `std::stirng_view` initializer is allowed, or we can convert an existing `std::string_view` to a `std::string` using `static_cast`

```c++
std::string_view sv{ "balloon" };

std::string str{ sv }; // okay, we can create std::string using std::string_view initializer

void printString(std::string str) { std::cout << str << '\n'; }
// printString(sv);   // compile error: won't implicitly convert std::string_view to a std::string

printString(static_cast<std::string>(sv)); // okay, we can explicitly cast a std::string_view to a std::string
```

​		可以把``string_view``看作是一种指针，要被使用，它必须指向某物。因此函数返回``std::string_view`` 常常是危险的。返回的字符串在`string_view`被使用之前就销毁了。

```c++
string_view bad()
{
    string s = "Once upon a time";
    return {&s[5],4};                // bad: returning a pointer to a local
}
```

 		对``std::string_view``的范围外访问行为是未定义的。使用``at()``进行范围检查的访问，会抛出``std::out_of_range``来尝试超出范围的访问，或者使用``gsl::string_span``.

​		change the viewed string, the changes are reflected in the `std::string_view`

```c++
char arr[]{"Gold"};
std::string_view str{arr};
arr[3] = 'f';

std::cout << str<< '\n'; // Golf
```

> Use `std::string_view` instead of C-style strings.
>
> Prefer `std::string_view` over `std::string` for read-only strings, unless you already have a `std::string`.

​		`std::stirng_view`可以通过遮蔽的方式，在不改变底层字符串的基础上，得到变更的结果。

```c++
std::stirng_view str{"Peach"};
str.remvoe_prefix(1); // each
str.remove_suffix(2); // ea
```

> 一旦进行了遮蔽，无法进行恢复，除非通过原字符串重置`std::stirng_view`。

​		`std::stirng_view`能够从非空终止字符数组进行构建。

```c++
char vowels[] { 'a', 'e', 'i', 'o', 'u'};
// vowels isn't null-terminated. We need to pass the length manually.
// Because vowels is an array, we can use std::size to get its length.
std::string_view str{vowels, std::size(vowels)};
std::cout << str << '\n'; // This is safe. std::cout knows how to print std::string_view.
```

​		`std::string_view`可能并非构建自空终止字符串，因此不能获取其底层字符串，需要先转换为`std::string`，再获取。

```c++
std::string_view sv{"balloon"};
sv.remove_suffix(3);
std::string str{sv};
auto szNullTerminated{str.c_str()};
```

​		making the parameter a `std::string_view` is the most flexible choice, because it can work efficiently with C-style string arguments (including string literals), `std::string` arguments (which will implicitly convert to `std::string_view`), and `std::string_view` arguments:

```c++
void printSV(std::string_view sv)
{
    std::cout << sv << '\n';
}

std::string s{ "Hello, world" };
std::string_view sv { s };

printSV(s);              // ok: pass std::string
printSV(sv);             // ok: pass std::string_view
printSV("Hello, world"); // ok: pass C-style string literal

return 0;
```

> ​		Note that we pass `std::string_view` by value instead of by const reference. This is because `std::string_view` is typically fast to copy, and pass by value is optimal for cheap to copy types.
>
> ​		if your function needs to call some other function that takes a C-style string or `std::string` parameter, then `const std::string&` may be a better choice, as `std::string_view` is not guaranteed to be null-terminated (something that C-style string functions expect) and does not efficiently convert back to a std::string.

​		`std::string_view`并不维持字符串的生命周期，对于字符串字面值来说，其存储在常量区，与程序的生命周期一致，不会造成问题；对于C-style字符串变量，`std::string`来说，`std::stirng_view`的生命周期可能长过它们。这会造成`UB`。

```c++
std::string_view askForName()
{
  std::cout << "What's your name?\n";

  // Use a std::string, because std::cin needs to modify it.
  std::string name{};
  std::cin >> name;

  // We're switching to std::string_view for demonstrative purposes only.
  // If you already have a std::string, there's no reason to switch to
  // a std::string_view.
  std::string_view view{ name };

  std::cout << "Hello " << view << '\n';

  return view;
} // name dies, and so does the string that name created.

std::string_view view{ askForName() };
// view is observing a string that already died.
std::cout << "Your name is " << view << '\n'; // Undefined behavior
```

​		Using `std::string_view::data()` of a non-null-terminated string can cause undefined behavior.Only use `std::string_view::data()` if the `std::string_view`‘s view hasn’t been modified and the string being viewed is null-terminated. 

```c++
 std::string_view str{ "balloon" };

  // Remove the "b"
  str.remove_prefix(1);
  // remove the "oon"
  str.remove_suffix(3);
  // Remember that the above doesn't modify the string, it only changes
  // the region that str is observing.

  std::cout << str << " has " << std::strlen(str.data()) << " letter(s)\n"; // all 6
  std::cout << "str.data() is " << str.data() << '\n';		// alloon
  std::cout << "str is " << str << '\n';					// all
```

> `std::string_view::data()`返回底层字符串的`const char*`，`cout / strlen`会以空终止字符串的逻辑查找字符串结尾。

​		`std::string_view`的实现不完整，在未来的版本中可能会进行完善

```c++
std::string s{ "hello" };
std::string_view v{ "world" };

// Doesn't work
std::cout << (s + v) << '\n';
std::cout << (v + s) << '\n';

// Potentially unsafe, or not what we want, because we're treating
// the std::string_view as a C-style string.
std::cout << (s + v.data()) << '\n';
std::cout << (v.data() + s) << '\n';

// Ok, but ugly and wasteful because we have to construct a new std::string.
std::cout << (s + std::string{ v }) << '\n';
std::cout << (std::string{ v } + s) << '\n';
std::cout << (s + static_cast<std::string>(v)) << '\n';
std::cout << (static_cast<std::string>(v) + s) << '\n';
```



### `std::string_view` literal

​		We can create string literals with type `std::string_view` by using a `sv` suffix after the double-quoted string literal.

```c++
std::cout << "foo\n";   // no suffix is a C-style string literal
std::cout << "goo\n"s;  // s suffix is a std::string literal
std::cout << "moo\n"sv; // sv suffix is a std::string_view literal
```

​		The “sv” suffix lives in the namespace `std::literals::string_view_literals`. The easiest way to access the literal suffixes is via using directive `using namespace std::literals`.

​		当从``const char*``构造一个``string_view``，这需要计算字符数。对于``"Stephen"sv``，长度在编译时计算。