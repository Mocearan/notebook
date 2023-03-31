# 概念

​		*概念（concept）*是在编译时求值，以谓词的形式用作约束时，成为模板接口的一部分的具名要求集合。

---

​		概念是指定一个或多个类型如何使用的编译时谓词。

​		类模板，函数模板，以及非模板函数（类模板的成员），可以关联到指定了对模板实参要求的约束，这些要求可以被用于选择最恰当的函数重载和模板特化。这种要求的具名集合称为概念。

​		概念和模板一起支持了C++泛型编程。



## 模板参数类型的约束

​		模板提供了以下功能：

- 不丢失信息的情况下传递类型（以及值和模板）作为参数。
  - 灵活性
  - 内联机会，当前的实现非常重视这一点。
- 在实例化时会将来自不同上下文的信息编织在一起。
  - 有优化机会。
- 能够将值作为模板参数传递。
  - 有编译时计算的机会。

​		换句话说，模板提供了一种强大的机制，用于编译时计算和类型操作，可以生成非常紧凑和高效的代码。

> 类型（类）可以包含代码和值。

- 模板首要用途是支持泛型编程， 模板提供（编译时）参数多态性。

  > 即专注于通用算法的设计、实现和使用的编程

- 泛型意味着一种抽象，可以接受各种类型，只要满足抽象的参数要求

### 具名要求

​		大多数模板参数必须满足特定的要求，以便模板能够正确编译并生成正确的代码。

> 也就是说，大多数模板应该是约束模板。
>
> 类型名称引入符typename是最不限制的，只需要参数是一个类型即可。
>
> 通常，我们可以做得更好。

​		满足操作的模板实参类型必须支持操作中对模板参数施加的运算，在没有概念的时候，通过具名要求来进行文本的约定提示。

```c++
template<typename Seq, typename Value>
Value sum(Seq s, Value v)
{
    for (const auto& x : s)
            v+=x;
    return v;
}
```

​		它的第一个模板参数是某种元素序列，以及它的第二个模板参数是某种数字。更具体地说，可以为sum()调用一对参数：

- 一个序列Seq，它支持begin()和end()，以便范围-for工作。
- 一个算术类型Value，它支持+=，以便可以添加序列的元素。

​		在我们尚不能使用概念的代码库中，我们必须使用命名约定和注释，例如：

```c++
template<typename Sequence，typename Number>
// requires Arithmetic<range_value_t<Sequence>,Number>
Number sum(Sequence s, Number n);
```


​		无论我们选择哪种符号，设计具有语义上有意义的模板参数约束都很重要。

### 概念

​		一旦我们定义了``Sequence``和``Number``的概念，编译器就能只通过模板接口来拒绝错误调用，而不查看模板的实现。

​		这提高了错误报告的质量。

```c++
template<Sequence Seq, Number Num>
Num sum(Seq s, Num v)
{
    for (const auto& x : s)
            v+=x;
    return v;
}
```

### `requires`

#### `require`子句

​		上述约束不完整，没有约束``Sequence``的元素能够和``Number``进行`+=`操作。我们可以这样做：

```c++
template<Sequence Seq, Number Num>
requires Arithmetic<range_value_t<Seq>,Num>
Num sum(Seq s, Num n);
```

- range_value_t``是标准库用于命名范围元素类型的。
- ``Arithmetic<X，Y>``用于指定类型``X``和``Y``算术运算的概念。

> ​		这样可以避免处理``vector<int>``、``vector<complex<double>>``等算数类型的同时，意外尝试计算``vector<string>``或``vector<int*>``。

​		通常，当算法需要不同类型的参数时，这些类型之间存在关系，最好明确说明。

​		上例只需要``+=``，但是为了简单和灵活性，不应该将模板参数约束得太紧。可能会希望``+``和``=``来替代``+=``。很自然的会想到使用通用概念（这里是``Arithmetic``）而不是狭窄的要求“有+=”。

​		除非规范是完整的，否则有些错误直到实例化时才会被发现。但是，即使是规范不完整也表达了意图，并且对于平稳的增量开发是必不可少的，其中我们最初认识不到所有需要的要求。使用成熟的概念库，初始规范将非常接近完美。

- ``requires Arithmetic<range_value_t<Seq>,Num>``被称为要求子句。
- ``template<Sequence Seq>``符号只是对显式使用``requires Sequence<Seq>``的简写。

​		如果喜欢详尽的约束，可以等效地写为：

```c++
template<typename Seq，typename Num>
requires Sequence<Seq> 
	&& Number<Num> 
	&& Arithmetic<range_value_t<Seq>,Num>
Num sum(Seq s, Num n);
```


​		另一方面还可以使用两个符号之间的等价性来编写：

```c++
template<Sequence Seq,Arithmetic<range_value_t<Seq>> Num>
Num sum(Seq s, Num n);
```

#### `requires 表达式`

​		使用`` requires-expression``，我们可以检查一组表达式是否有效。通常在现有概念不能表达的情况下，使用`requires_expression`来增强某些检查。

​		例如编写 advance()，而不使用标准库概念 random_access_iterator：

```c++
template<forward_iterator Iter>
requires requires(Iter p, int i) { p[i]; p+i; }         // Iter has subscripting and integer addition
void advance(Iter p, int n)                // move p n elements forward
{
        p+=n;
}
```

> 第一个 requires 开始要求子句，第二个 requires 开始要求表达式

​		`requires-expression` 是一个谓词，如果其中的语句是有效代码，则为 true，否则为 false。

```c++
requires(Iter p, int i) { p[i]; p+i; }
```

​		可以将`requires-expression`视为泛型编程中的汇编代码，它非常领会，不会强制执行任何编程规范。`requires_expression`通常以某种形式出现在很有有用且有意思的公共代码底层，就像汇编代码通常在普通代码的底层发挥功能。

​		对于上述的例子，可以不用如此麻烦的定义`requires-expression`，它们实际上是进行了一部分`random_access_iterator`概念的检查：

```c++
template<random_access_iterator Iter>
void advance(Iter p, int n)
{
    p+=n; // a random-access iterator has +=
}
```

​		总是更倾向于使用具有明确语义并有恰当名字的概念来进行约束，在这些概念的实现中往往是通过`requires-expression`来完成的。

## 基于概念的重载

​		可以根据其概念的性质进行重载，和函数的参数列表重载类似。

```c++
template<forward_iterator Iter>
void advance(Iter p, int n)                 // move p n elements forward
{
    while (n--)
        ++p;         // a forward iterator has ++, but not + or +=
}

template<random_access_iterator Iter>
void advance(Iter p, int n)                // move p n elements forward
{
    p+=n;                 // a random-access iterator has +=
}
```

​		编译器将选择满足最强要求的模板参数。在这种情况下，`list`仅提供前向迭代器，`vector`提供随机访问迭代器。

```c++
void user(vector<int>::iterator vip, list<string>::iterator lsp)
{
    advance(vip,10);      // uses the fast advance()
    advance(lsp,10);      // uses the slow advance()
}
```

​		和一般重载一样，这是一种不需要运行时成本的编译时机制，如果编译器找不到最佳选择，则会给出模糊性错误。

​		和一般重载一样，这是一种不需要运行时成本的编译时机制，如果编译器找不到最佳选择，则会给出模糊性错误。

​		基于概念的重载规则比一般重载规则简单得多。首先考虑几个替代函数的单个参数：
- 如果参数不符合概念，则无法选择该替代方案。
- 如果参数仅与一个替代方案的概念匹配，则选择该替代方案。
- 如果两个替代方案的参数匹配概念，并且一个比另一个更严格（匹配另一个的所有要求并更多），则选择该替代方案。
- 如果两个替代方案的参数对概念的匹配程度相同，则存在歧义。

  > 要选择替代方案，它必须是
  >     - 对于其所有参数都匹配，且
  >     - 对于所有参数至少与其他替代方案一样好，且
  >     - 至少对一个参数更好的匹配。



## 自定义概念

​		标准库中定义好了很多概念，使用它们比创建新的概念更加容易。

​		如果需要时，可以自定义一些概念。再次说明，概念是一个谓词。

​		概念是被发现的，而不是被设计出来的。例如整数和浮点数、序列和更一般的数学概念，如环和向量空间。它们代表了应用领域的基本概念，即一件事物的定义和性质。这就是为什么它们被称为“概念”。

> ​		对于基本用途，请考虑正则的概念。当类型的行为非常像``int``或`vector`时，它就是正则的。一个正则类型对象：
>
> - can be default constructed.
> - can be copied (with the usual semantics of copy, yielding two objects that are independent and compare equal) using a constructor or an assignment.
> - can be compared using **==** and **!=**.
> - doesn’t suffer technical problems from overly clever programming tricks.
>
> ​		`string`是另一种正则类型，与``int``一样，``string``也是``totally_ordered`。也就是说，可以使用具有适当语义的<、<=、>、>=和<=>来比较两个字符串。

​		一个概念不仅仅是一个语法概念，它从根本上是关于语义的。还没有任何语言支持来表达语义，必须依靠专业知识和常识来获得语义上有意义的概念。不要定义语义上没有意义的概念，比如可加的和可减的。相反，依靠领域知识来定义与应用程序领域中的基本概念相匹配的概念。

> ​		例如，不要定义``+``来除。

### 同类型相等性可比较

```c++
template<typename T>
concept EqualityComparable = 
requires (T a, T b) {
    {a == b} -> Boolean;
    {a != b} -> Boolean;
}
```

​	该概念表达了，给定两个类型的值，它们必须使用 `== / !=`继续你那个比较，这些操作的结果必须是`bool`值。

- `->`之后指定了`{...}`的结果，必须是概念
  - 这里需要定义Boolean概念，标准库并没有定义，需要自定义
  - Boolean用以指明可以用作条件的类型

```c++
static_assert(EqualityCompare<int>); // success

struct S{int a;};
static_assert(EqualityComparable<S>); // failed, struct not == and != operator
```



### 不同类型相等性可比较

```c++
template<typename T, typename T2 =T> 
// typename T2 =T表示如果我们不指定第二个模板参数，则T2将与T相同；T是默认模板参数。
concept Equality_comparable =
requires (T a, T2 b) {
    { a == b } -> Boolean;        // compare a T to a T2 with ==
    { a != b } -> Boolean;         // compare a T to a T2 with !=
    { b == a } -> Boolean;        // compare a T2 to a T with ==
    { b != a } -> Boolean;        // compare a T2 to a T with !=
};
```

​		这个`EqualityComparable`几乎和标准库`equality_comparable`相同。

```c++
static_assert(Equality_comparable<int,double>);   // 成功
static_assert(Equality_comparable<int>);                // 成功（T2默认为int）
static_assert(Equality_comparable<int,string>);     // 失败
```



### 定义算数运算

```c++
template<typename T, typename U = T>
concept Number =
    requires(T x, U y)  // Something with arithmetic operations and a zero
{
    x + y; x - y; x *y; x / y;
    x += y; x -= y; x *= y; x /= y;
    x = x; // copy
    x = 0;
};
```

​		首先确定算数运算的操作数的性质

- 给定一个参数类型，``Number<X>``检查X是否具有Number所需的属性。
- 给定两个参数，``Number<X，Y>``检查这两个类型是否可以使用所需的操作。

```c++
template<typename T, typename U = T>
concept Arithmetic = Number<T, U> && Number<U, T>;
```



### 定义sequence

```c++
template<typename S>
concept Sequence = requires (S a) {
    typename range_value_t<S>;                                    // S必须具有值类型
    typename iterator_t<S>;                                       // S必须具有迭代器类型

    { a.begin() } -> same_as<iterator_t<S>>;                    // S必须具有返回迭代器的begin() / end()
    { a.end() } -> same_as<iterator_t<S>>;

    requires input_iterator<iterator_t<S>>;                     // S的迭代器必须是input_iterator
    requires same_as<range_value_t<S>, iter_value_t<S>>;        // 元素和迭代器的值类型必须相同
};
```

> 最难定义的概念是那些代表基本语言概念的概念。因此，最好使用已建立库中的集合。如使用标准库中的`sequence`.
>
> ```c++
> template<typename S>
> concept Sequence = input_range<S>;   // simple to write and general
> ```

​		如果将``S``的值类型限制为``S::value_type``，可以使用一个更简单的``Value_type``

```c++
template<class S>
using Value_type = typename S::value_type;
```

​		对于简洁地表达简单的概念和隐藏复杂性来说，这是一种有用的技巧。

​		标准``value_type_t``的定义基本相似，但更复杂，因为它还处理没有名为``value_type``成员的序列（例如，内置数组）。

## 定义检查

​		为模板指定的概念用于在模板使用点检查参数，不用于检查模板定义中参数的使用情况。

```c++
template<equality_comparable T>
bool cmp(T a, T b)
{
        return a<b;
}
// 	这里的概念保证了``==``的存在，但不保证``<``：
bool b0 = cmp(cout,cerr);        // error: ostream doesn't support ==
bool b1 = cmp(2,3);                  // OK: returns true
bool b2 = cmp(2+3i,3+4i);        // error: complex<double> doesn't support <
```

> - `equality_comparable`概念检查将捕获传递ostream的尝试
> - 接受``int``类型和``complex<double>``
>   - 因为这两种类型支持``==``
>   - int支持<，因此cmp(2,3)编译
>   - 而``cmp(2+3i,3+4i)``则在cmp()的体被检查并实例化为不支持``<``的`complex<double>`时被拒绝。

​		将模板定义的最后检查延迟到实例化时间有两个好处:

- 可以在编码使用不完整的概念
  - 能够在开发概念、类型和算法的同时获得经验，并逐渐改进检查。
- 可以在模板中插入调试、跟踪、遥测等代码，而不会影响其接口。更改接口可能会导致大量的重新编译。

​		在开发和维护大型代码库时，两者都很重要。付出的代价是一些错误，比如在只保证==的情况下使用<，会在编译过程的很晚的时候被捕捉到。



## concept & auto

​		关键字``auto``可用于指示对象应具有与其初始化式相同的类型，

```c++
auto x = 1;                                          // x is an int
auto z = complex<double>{1,2};      // z is a complex<double>
```

​		然而，初始化不仅仅发生在简单的变量定义中:

```c++
auto g() { return 99; }                        // g() returns an int

int f(auto x) { /* ... */ }                        // take an argument of any type

int x = f(1);                                         // this f() takes an int
int z = f(complex<double>{1,2});     // this f() takes a complex<double>
```

​		关键字``auto``潜在含义是一种值受到的约束最少的概念：它只要求它必须是某种类型的值。

​		接受``auto``形参使函数成为函数模板。对于给定的概念，可以通过在``auto``前面加上一个概念来加强所有此类初始化的需求。

```c++
auto twice(Arithmetic auto x) { return x+x; }   // just for numbers
auto thrice(auto x) { return x+x+x; }           // for anything with a +

auto x1 = twice(7);   // OK: x1==14
string s "Hello ";
auto x2 = twice(s);   // error: a string is not Arithmetic
auto x3 = thrice(s);   // OK x3=="Hello Hello Hello "
```

​		除了用于约束函数参数外，概念还可以约束变量的初始化，这对于防止滥用``auto``和使用泛型函数记录代码需求非常方便：

```c++
auto ch1 = open_channel("foo");                      // works with whatever open_channel() returns
Arithmetic auto ch2 = open_channel("foo");           // error: a channel is not Arithmetic
Channel auto ch3 = open_channel("foo");              // OK: assuming Channel is an appropriate concept
            // and that open_channel() returns one
```

​		为了可读性和调试，类型错误要尽可能接近其起源。限制返回类型可以可以对此进行说明：

```c++
Number auto some_function(int x)
{
    // ...
    return fct(x);    // an error unless fct(x) returns a Number
    // ...
}
```

​		当然，我们可以通过引入一个局部变量来实现， 然而，这有点啰嗦，而且并不是所有类型都可以廉价复制：

```c++
auto some_function(int x)
{
    // ...
    Number auto y = fct(x);    // an error unless fct(x) returns a Number
    return y;
    // ...
}
```



## concept & type

A type

- 指定可隐式或显式应用于对象的操作集
- 依赖于函数声明和语言规则
- 指定对象在内存中的布局方式

A single-argument concept

- 指定可隐式或显式应用于对象的操作集
- 依赖于反映函数声明和语言规则的使用模式
- 没有说明物体的布局
- 允许使用一组类型

​		因此，用概念约束代码比用类型约束代码更具灵活性。

​		此外，概念可以定义几个参数之间的关系。

> ​		BS：我的理想是，最终大多数函数将被定义为模板函数，其参数受到概念的约束。不幸的是，对此的表示法支持还不完善：我们必须将概念用作形容词，而不是名词。例如:
>
> ```c++
> void sort(Sortable auto&);         // 'auto' required
> void sort(Sortable&);                 // error: 'auto' required after concept name
> ```
