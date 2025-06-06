# 声明符

​		声明符用以提供声明语句中实体的名字，及对名字的索引方式（指针，引用，数组、函数）。

------

​		声明符是将标识符引入为名字的说明，并被``*/&/[]/ ()``运算符修饰所修饰。

> 声明符是修饰符。修饰基于某个类型的索引方式。
>
> 函数也视作一种数据的索引方式，即以索引到的代码段计算返回的类型。

​		对象位于内存某个地址中，访问该对象需要的要素有：

- 对象的地址，即从哪里开始读取对象
- 对象的类型，即构成完整的对象数据和对象操作需要读取多少数据
- 对象的访问方式，即如何读取内存中对象

​		从机器的角度出发，某个类型的对象被分配给用户，应当告诉用户对象的地址。当用户访问对象时，以该地址寻址，对象类型决定查找的宽度。这种方式是机器原始的数据访问形式，称为间接访问。c/c++以指针的形式将这种原始数据访问抽象到语言中，即用户保存对象的地址。

​		在编程的抽象层面上讲，用户应当关心的是对对象值的读写，对象的地址不是应当去关心的事情。将内存地址中存储的对象值，直接以名字表现整个对象。这种更高抽象的内存别名，称为直接访问。c/c++将这种直接访问抽象为对象或实体，即用户保存对象本身。在这种语境下，指针平凡化为一种保存地址的对象。

​		声明符的作用是以名字的方式，赋予对象身份，默认直接访问，或以*/&/[]/()`修饰符声明间接访问。

​		初始化声明符列表，是一或多个 以逗号分隔的初始化声明符列表，每个声明符恰好引入一个对象、指针、引用、函数或（对于 typedef 声明）类型别名。





## 指针声明符

​		A **pointer** is an object that holds a *memory address* (typically of another variable) as its value.

​		指针声明符用于声明指针，或指向成员变量的指针。

> ​		In modern C++, the pointers we are talking about here are sometimes called “raw pointers” or “dumb pointers”, to help differentiate them from “smart pointers” that were introduced into the language more recently.

​		The size of a pointer is dependent upon the architecture the executable is compiled for -- a 32-bit executable uses 32-bit memory addresses -- consequently, a pointer on a 32-bit machine is 32 bits (4 bytes). With a 64-bit executable, a pointer would be 64 bits (8 bytes). 

​		the type of the pointer has to match the type of the object being pointed to。

​		指针的具体实现应该与程序运行机器的寻址机制同步。大多数机器支持逐字节内存访问，其他的机器是逐机器字内存访问，需要自行从机器字中抽取字节。很少有机器能够直接寻址到一个二进制位。

- 因此，能独立分配且内置指针指向的最小对象是`char`类型。
- `bool`占用的内存至少和`char`一样多。
- 如果想把尺寸更小的值存储的更紧密，可以使用位运算、结构中的位域、`bitset`

### 指针的声明

```c++
type * attr cv declarator; //指针声明
type_name nested-name-specifier* attr cv declarator; //成员指针声明
```

```c++
S* D;
S C::* D;
```

> **attr 属性：**
>
> ​		为类型、对象、代码等引入由实现定义的属性。
>
> **顶层cv 限定符：**
>
> ​		声明指针变量本身的 `const/volatile `限定（而并非被指向类型，其限定是 *声明说明符序列* 的一部分）。
>
> **declarator 声明符：**
>
> ​		除引用声明符之外的任意声明符，可以是另一指针声明符（允许指针嵌套声明多级指针）
>
> > 不存在指向引用的指针和指向位域的指针。
>>
> 
>**nested-name-specifier: 嵌套名字说明符：**
> 
>​		名字和作用域解析运算符序列。
> 



### 指针的定义和初始化

```c++
指针声明说明符 = &val_address;
指针声明说明符 = new type;
```

​		指针无须在定义时赋初值，块作用域内不初始化会有不确定值（野指针，空悬指针）.

​		initializing a pointer with a literal value is disallowed:

```cpp
int* ptr{ 5 }; // not okay
int* ptr{ 0x0012FF7C }; // not okay, 0x0012FF7C is treated as an integer literal
```

> ​		使用new分配的堆上内存不光有使用权，还有管理责任。对于new出的内存，要在合适的时机对其调用delete 进行归还。归还后，该地址上的内存使用权和管理权都将因归还系统而失效，未再次申请前，对该地址的读写都是非法。尤其是写操作可能引发崩溃。
>
> > **Note:**
> >
> > ​		指针变量所记录的合法地址，应该是可被索引的地址，即不是系统中保留的保护地址，也不是未申请使用权的地址。（系统栈可视作默认申请好的一块连续内存）。
> >
> > ​		对系统保护地址的写操作，必然会引发崩溃。
> >
> > ​		所以，虽然指针变量实质上是一个整型数，但是也不能对其进行任意的赋值，为程序引入不可控的因素。因为，使用指针变量应当十分小心，对已有的对象且生命周期能够持续到该指针变量访问之外。
> >
> > ​		一般而言，使用了指针变量，那么对象的生命周期管理就要转交由指针变量进行管理，否则反向的通知指针对象声明周期的结束是一件困难的事情。对于栈上对象的指针，要格外注意生命周期与作用域的关联。一般也不对栈上的对象使用指针进行索引。
> >
> > ​		因此，指针变量的使用，一般是对于一块能够清晰把握生命周期，确定其在程序生命周期中驻留时间的内存对象进行地址索引。
>
> ​		delete/free 堆上分配的内存，指针变量本身不会被置为空指针，因为导致空悬指针的产生。关于在释放后置空还是保留空悬指针，仍有争议，但要尽量做到不去使用已经释放内存的指针。另，free本身不会对要释放的指针进行判空检查，而delete内部已经做过了判空检查。
>
> ​		关于c和c++对于动态内存的分配和管理，原则上成对使用。但c++的delete可以coverc的malloc，但free不能cover住delete。需要注意的是，malloc申请的是指定数目的无类型内存字节。其没有类型特性，需要对该地址进行类型转换才能进行读写，否则只能进行内存操作。（如mem系列）。
>
> ​		对于new和malloc来说，内存申请是有可能失败的。当虚拟内存不够，或者在移动物联网设备等内存有限而不使用虚拟内存直接使用的物理内存不够时，内存会分配失败，new会抛出异常，malloc会返回NULL。
>
> ​		关于double free，一种是不谨慎的对一个指针先后进行了两次释放，另一种是对同一块内存地址有多个指针索引，而对多个索引的指针所寻址的对象空间属性进行了超过一次的释放也会造成double free.会引发异常。



> **NOTE：**			
>
> ​		编译器并不负责检查空悬指针（无效指针），因为编译器一般不对值的有效性负责，试图拷贝或者以其他方式访问无效指针的值都将引发错误。（NULL，已释放的堆区内存，越界访问的内存等等）。这一点和试图使用未经初始化的变量是一样的。
>
> ​		访问无效指针的后果无法预计，因此程序员必须清楚任意给定的指针是否有效。





### 指针的操作

​		允许指针赋值、拷贝，生命周期内可以指向不同的对象。

​		拷贝、取值、赋值、合法的条件表达式运算、合法的逻辑判断（==，！=）。

> We can use assignment with pointers in two different ways:
>
> 1. To change what the pointer is pointing at (by assigning the pointer a new address)
> 2. To change the value being pointed at (by assigning the dereferenced pointer a new value)
>
> ```c++
> int x{ 5 };
> int* ptr{ &x }; // ptr initialized to point at x
> *ptr = 3; // change the value of x what the pointer is pointing at
> 
> int y{ 6 };
> ptr = &y; // // change ptr to point at y
> ```
>
> 

​		可以对指针进行以数据类型为基础的偏移寻址。

> ​		指针传递本质上也是值传递，即指针变量本身是被拷贝的。
>
> ​		即指针是间接级，无法通过指针变量自身改变其自身的值。当需要修改一个指针本身时，需要通过更高级间接级的指针。

#### `*` 寻址值属性（解引用）

​		按照值属性引用到的地址访问对象单元的空间属性。

```c++
int x ;
int* p = &x;
cout << *p << endl; //对可寻址变量p进行寻址，作为右值表达式时，直接对寻址得到的对象进行名称引用，引用值属性。

*p = 123; // 对可寻址变量p进行寻址，作为左值表达式时，对寻址得到的对象，引用对象属性。
```



#### & address-of operator

​		It’s worth noting that the address-of operator (&) doesn’t return the address of its operand as a literal. Instead, it returns a pointer containing the address of the operand, whose type is derived from the argument

```c++
int x{ 4 };
std::cout << typeid(&x).name() << '\n'; // print the type of &x
// int *
```

```c++
int array[]{ 9, 7, 5, 3, 1 };

std::cout << &array[1] << '\n'; // print memory address of array element 1
std::cout << array+1 << '\n'; // print memory address of array pointer + 1

std::cout << array[1] << '\n'; // prints 7
std::cout << *(array+1) << '\n'; // prints 7 (note the parenthesis required here)
```



#### 寻址偏移

​		指针可以按照当前寻址类型的宽度，通过`+/-`运算符，访问指定间隔数的对象单元空间属性。

> 对于同样的指针的偏移次数，宽度导致寻址的内存大小有差异，这和指针所索引的变量的类型有关。
>
> ![image-20201115130300520](https://gitee.com/masstsing/picgo-picserver/raw/master/20201115130305.png)
>
> ```c++
> int a;
> int *p = &a;
> 
> std::cout << *(p+0) << " " << *++p << std::endl;
> ```
>
> ​		`void*` 类型 只能进行模糊的内存数据操作，如拷贝移动，读写，分配删除之类，而不能进行与具体类型相关的移位或解引用的逻辑运算、算数运算等等。

```c++
#include <iostream>

using namespace std;

int main(void)
{
    int numbers[5]{1,2,3,4,5};
    // 使用从数组退化的指针
    for (int i = 0; i < sizeof numbers / sizeof numbers[0]; i++) // sizeof numbers 只有在参数传递时指针才退化
        cout << "numbers[" << i << "] " << *(numbers+i) << endl;

    char chars[3]{'a', 'b', 'c'};
    for (int i = 0; i < sizeof chars / sizeof chars[0]; i++)
        cout << "numbers[" << i << "] " << *(chars + i) << " " << int(*(chars + i)) << endl;

    void *p = numbers;
    //cout << "numbers[0] " << *(p + 1) << endl;
    		//‘void*’ is not a pointer-to-object type
    cout << "numbers[0] " <<*((int*)(p) + 1) << endl;
    return 0;
}
```

```c++
for (char* ptr{ name }; ptr != (name + arrayLength); ++ptr)
{
  if (isVowel(*ptr))
  {
    ++numVowels;
  }
}

auto numVowels{ std::count_if(std::begin(name), std::end(name), isVowel) };
// std::begin and std::end only work on arrays with a known size. If the array decayed to a pointer, we can calculate begin and end manually.
std::count_if(name, name + nameLength, isVowel)
```

​		同类型指针相减得到两个指针的步差。直接相加两个指针没有意义，也不允许。

#### 索引访问

​		可以对指针使用下标索引的形式，访问指定间隔数的对象单元空间属性。

​		现代编译器中索引访问和指针寻址偏移解引用生成的代码应该是一样的，不存在效率差异。

```c++
int a;
int *p = &a;

p[0];
p[1]; // 行为未定义，访问未经定义的对象空间。
```

> **以数组方式进行参数传递或返回值时，退化为指针**

​		要避免使用索引访问的方式进行寻址偏移，即在索引确定存在的情况下才进行索引访问，而寻址偏移可能寻址到索引不存在的内存地址上。

```c++
int* end{ &data[std::size(data)] };
int* end{ data.data() + std::size(data) }; // data() returns a pointer to the first element
```

​		一个小点：

```c++
// a[j] == *(&a[0]+j) == *(a+j) == *(j+a) == j[a]
3["hello"] == "hello"[3]
```



### 指针的分类

​		按照指针值属性寻址到的对象的不同，从而使得影响到指针的行为分类。

指针类型的值可以是以下几种之一：

- 指向对象或函数的指针，视作首指针

  - 包括数组名和函数名本身


  > ​	指向对象的指针表示该对象所占用的内存的首字节的地址。

- 对象尾后指针


​		对象的末尾后指针表示该对象所占用的存储之后的首字节的地址。

​		这对很多算法来说是很有用的，它与首指针共同为数组或其他形式的一组线性存储结构的数据定界。

> ​		这个地址可能未经分配，从而使得指针成为无效指针。
>
> ​		通过无效指针值间接寻址，和将无效指针值传递给解分配函数（释放内存的函数）均拥有未定义行为。
>
> ​		无效指针值的任何其他用法（无效指针通常只做标记位用，提供用于标记的只读地址）拥有由实现定义的行为。部分实现会将复制无效指针值的行为定义为造成由系统生成的运行时错误。
>
> **两个表示同一地址的指针也可能拥有不同的值：**
>
> ```c++
>struct C {
> int x, y;
> } c;
> 
> int* px = &c.x;   // px 的值为“指向 c.x 的指针”
> int* pxe= px + 1; // pxe 的值为“ c.x 的尾后指针”
> int* py = &c.y;   // py 的值为“指向 c.y 的指针”
> 
> assert(pxe == py); // == 测试两个指针是否表示相同地址
>              // 地址不同会引发断言
> 
>    *pxe = 1; // 地址相同没有引发断言，但这样使用也是未定义行为
> ```

- 该类型的空指针值

  - 见空指针

- 无效指针值

  - 见野指针

#### 常量指针 / 指针常量

​		`const`可以用以修饰指针的常量性，但对于指针的间接访问性质，这种常量性有两个方面：

- 指针本身的常量性
- 访问方式的常量性

​		即，通过间接访问形式，能够以这种方式更改源对象。

​		**常量指针**，指向常量的指针。**指针常量**，具有常量性的指针。**常量指针常量**，指向常量的具有常量性的指针。

```c++
const int* ; / int const *; // 常量指针
int* const ; // 指针常量
const int* const; / int const* const; // 常量指针常量
```



#### 对象指针

#### 函数指针

​		Just like it is possible to declare a non-constant pointer to a normal variable, it’s also possible to declare a non-constant pointer to a function.

​		因为函数名本身可以看做是指针的升维，所以函数指针可以看做是一个二级指针。

```c++
// fcnPtr is a pointer to a function that takes no arguments and returns an integer
int (*fcnPtr)();
// To make a const function pointer, the const goes after the asterisk:
int (*const fcnPtr)();

// assignment
// Note that the type (parameters and return type) of the function pointer must match the type of the function.
int (*fcnPtr)(){ &foo }; // fcnPtr points to function foo
fcnPtr = &goo; // fcnPtr now points to function goo
// Function pointers can also be initialized or assigned the value nullptr:
int (*fcnptr)() { nullptr }; // okay

// call
 (*fcnPtr)(5); // call function foo(5) through fcnPtr.
fcnPtr(5); // call function foo(5) through fcnPtr.

// callback
// Default the sort to ascending sort
void selectionSort(int* array, int size, bool (*comparisonFcn)(int, int) = ascending);

// alias
using ValidateFunction = bool(*)(int, int);
bool validate(int x, int y, ValidateFunction pfcn) ;// clean

// std::function
bool validate(int x, int y, std::function<bool(int, int)> fcn); // std::function method that returns a bool and takes two int parameters
fcnPtr = &goo; // fcnPtr now points to function goo
using ValidateFunction = std::function<bool(int, int)>; // type alias to std::function

// Type inference
auto fcnPtr{ &foo };
```

​		Default parameters won’t work for functions called through function pointers. Default parameters are resolved at compile-time (that is, if you don’t supply an argument for a defaulted parameter, the compiler substitutes one in for you when the code is compiled). However, function pointers are resolved at run-time.

​		Functions used as arguments to another function are sometimes called **callback functions**.

```c++
#include <utility> // for std::swap
#include <iostream>

bool evensFirst(int x, int y)
{
	// if x is even and y is odd, x goes first (no swap needed)
	if ((x % 2 == 0) && !(y % 2 == 0))
		return false;

	// if x is odd and y is even, y goes first (swap needed)
	if (!(x % 2 == 0) && (y % 2 == 0))
		return true;

        // otherwise sort in ascending order
	return ascending(x, y);
}


// Note our user-defined comparison is the third parameter
void selectionSort(int* array, int size, bool (*comparisonFcn)(int, int))
{
    // Step through each element of the array
    for (int startIndex{ 0 }; startIndex < (size - 1); ++startIndex)
    {
        // bestIndex is the index of the smallest/largest element we've encountered so far.
        int bestIndex{ startIndex };

        // Look for smallest/largest element remaining in the array (starting at startIndex+1)
        for (int currentIndex{ startIndex + 1 }; currentIndex < size; ++currentIndex)
        {
            // If the current element is smaller/larger than our previously found smallest
            if (comparisonFcn(array[bestIndex], array[currentIndex])) // COMPARISON DONE HERE
            {
                // This is the new smallest/largest number for this iteration
                bestIndex = currentIndex;
            }
        }

        // Swap our start element with our smallest/largest element
        std::swap(array[startIndex], array[bestIndex]);
    }
}

// Here is a comparison function that sorts in ascending order
// (Note: it's exactly the same as the previous ascending() function)
bool ascending(int x, int y)
{
    return x > y; // swap if the first element is greater than the second
}

// Here is a comparison function that sorts in descending order
bool descending(int x, int y)
{
    return x < y; // swap if the second element is greater than the first
}

// This function prints out the values in the array
void printArray(int* array, int size)
{
    for (int index{ 0 }; index < size; ++index)
    {
        std::cout << array[index] << ' ';
    }

    std::cout << '\n';
}

int main()
{
    int array[9]{ 3, 7, 9, 5, 6, 1, 8, 2, 4 };

    // Sort the array in descending order using the descending() function
    selectionSort(array, 9, descending);
    printArray(array, 9);

    // Sort the array in ascending order using the ascending() function
    selectionSort(array, 9, ascending);
    printArray(array, 9);
    
    selectionSort(array, 9, evensFirst);
    printArray(array, 9);

    return 0;
}
```

> Note: If a function parameter is of a function type, it will be converted to a pointer to the function type. 
>
> ```c++
> void selectionSort(int* array, int size, bool (*comparisonFcn)(int, int));
> void selectionSort(int* array, int size, bool comparisonFcn(int, int));
> ```
>
> 

#### 成员指针

##### 数据成员指针

##### 成员函数指针

#### 空指针 null pointer

​		当我们没有要指向的对象时，或者如果我们需要表示“没有对象可用”的概念，我们给指针赋值``nullptr``（空指针）。

​		不指向任何对象，在试图使用一个指针之前代码可以首先检查是否为空。实质上是对地址编码为0的地址进行索引（``NULL/nullptr`）。

​		所有指针类型只共享一个``nullptr``：

> ​		A **null value** (often shortened to **null**) is a special value that means something has no value. When a pointer is holding a null value, it means the pointer is not pointing at anything. Such a pointer is called a **null pointer**.
>
> ​		On modern architectures, the address `0` is typically used to represent a null pointer. However, this value is not guaranteed by the C++ standard, and some architectures use other values. 
>
> ​		The literal `0` can be interpreted as either an integer literal, or as a null pointer literal.  In certain cases, it can be ambiguous which one we intend.
>
> ```c++
> void print(int x) // this function accepts an integer
> {
> 	std::cout << "print(int): " << x << '\n';
> }
> 
> void print(int* ptr) // this function accepts an integer pointer
> {
> 	std::cout << "print(int*): " << (ptr ? "non-null\n" : "null\n");
> }
> 
> print(0); // always calls print(int) because 0 is an integer literal (may or may not be what we expect)
> print(NULL); // most likely calls print(int), but could call print(int*) depending on how NULL is defined (definitely not what we want)
> print(nullptr); // always calls print(int*) because nullptr only converts to a pointer type (good)
> ```
>
> ​		Using `nullptr` removes this ambiguity, since `nullptr` will only match a pointer type.

```c++
int* p1 = nullptr;
int* p2 = 0;
int* p3 = NULL; //#include <cstdlib>
int* ptr {}; // ptr is now a null pointer, and is not holding an address
```

​		在旧代码中，通常使用``0``或``NULL``即``(void*)0``代替nullptr。使用``nullptr``消除了整数（0或NULL）和指针（nullpter）之间的潜在混淆。新标准下，现在的c++程序最好使用nullptr，同时尽量避免使用NULL。

​		**Dereferencing a null pointer results in undefined behavior**,A null pointer doesn’t have an address. 

​		

#### void* 指针 

​		是一种泛型指针(generic pointer)，可用于存放除函数指针和成员指针之外的任意类型对象的地址。

​		`void* ` 指针能够做的事比较少：与别的指针比较、赋给另外一个void*指针。最主要的用途是用作函数的输入输出参数、

​		除此之外，使用`void*`指向的数据就必须把它显式转换为某一特定类型的指针。

> ​		because the void pointer does not know what type of object it is pointing to, dereferencing a void pointer is illegal. 
>
> ​		不能直接操作``void*``指针指向的对象，因为我们并不知道这个对象到底是什么类型，在使用时需要转化成具体的类型。在``void*``的视角来看内存空间也就是单纯的内存空间（位集合），没办法访问内存空间中存的对象数据（没有类型，无法解译数据）。
>
> ​		某些偏向底层的代码中，这种使用`void*`传递大尺寸数据的情况比较多。

​		the void pointer must first be cast to another pointer type before the dereference can be performed.

```c++
enum class Type
{
    tInt, // note: we can't use "int" here because it's a keyword, so we'll use "tInt" instead
    tFloat,
    tCString
};

void printValue(void* ptr, Type type)
{
    switch (type)
    {
    case Type::tInt:
        std::cout << *static_cast<int*>(ptr) << '\n'; // cast to int pointer and perform indirection
        break;
    case Type::tFloat:
        std::cout << *static_cast<float*>(ptr) << '\n'; // cast to float pointer and perform indirection
        break;
    case Type::tCString:
        std::cout << static_cast<char*>(ptr) << '\n'; // cast to char pointer (no indirection)
        // std::cout will treat char* as a C-style string
        // if we were to perform indirection through the result, then we'd just print the single char that ptr is pointing to
        break;
    default:
        assert(false && "type not found");
        break;
    }
}
```



#### 空悬指针 dangling pointer

​		a **dangling pointers** is a pointer that is holding the address of an object that is no longer valid (e.g. because it has been destroyed). Dereferencing a dangling pointer will lead to undefined results.

​		指针变量记录的内存位置上的对象已经销毁或内存使用权已被回收。（我们将向内核申请和归还内存，说明为对内存使用权的申请和归还）。

​		 指针变量的值明确，是确定的内存的地址。但该内存地址的使用权不可达或内存地址上的对象已经不复存在。

```c++
//1.
chara* dp = nullptr;
{
	char c;
    dp = &c;
}
// c对象不复存在，dp空悬

//2.
#include <stdlib.h>
char* dp = (char*)malloc(A_CONST);
free(dp); // 内存使用权释放，dp空悬
dp = nullptr; // 将空悬指针置为空指针

//3.
int* func(void)
{
 	int num = 123;
    //pass
    return &num;
}
// 返回局部栈上变量的指针，对象不复存在。
```

​		when an object is destroyed, any pointers to that object will be left dangling. Such pointers are *not* nulled automatically! It is the programmer’s responsibility to ensure that all pointers to an object that has just been destroyed are properly set to `nullptr`.

#### 野指针

​		A pointer that has not been initialized is sometimes called a **wild pointer**. Wild pointers contain a garbage address, and dereferencing a wild pointer will result in undefined behavior. 

​		没有初始化的指针就是野指针，一般情况下编译器会进行警告。

> 也就是说，野指针的值属性是不确定的，寻址的地址不是预期内的地址。
>
> 可能是内存中任意位置的记录。该位置可能合法存在，也可能并不能被合法引用。所以使用野指针的行为不可预测。
>
> ```c++
> int func()
> {
>      char *dp;//野指针，没有初始化。an uninitialized pointer (holds a garbage address)
>       int* ptr2{};     // a null pointer (we'll discuss these in the next lesson)
>        static char *sdp;//非野指针，因为静态变量会默认初始化为0
> }
> ```
>
> 有可能记录只读内存的地址，这样读取操作不会出现崩溃错误，但是会读非预期的值。但是写操作会引发程序崩溃。
>
> 有可能记录保护内存的地址，这样对读写操作都会引发崩溃。
>
> 有可能引用到别处某个已申请到使用权的内存地址，读取操作会得到错误的值，写操作会覆写该地址上的二进制数据，使得别处的使用错乱。（内存被踩）
>
> 
>

​		**advice**： 

​		初始化所有指针。使用未经初始化的指针是引发运行时错误的一大原因。
​	和其他变量一样，访问未经初始化的指针所引发的后果也是无法预计的。通常这一行为将造成程序崩溃，
而且一旦崩溃，要想定位到出错位置将是特别棘手的问题。
​	在大多数编译器环境下，如果使用了未经初始化的指针，则该指针所占内存空间的当前内容将被看做一个地址值。访问该指针，相当于去访问一个本不存在的位置上的本不存在的对象。（对不可寻址的值寻址）糟糕的是，如果指针所占内存空间中恰好有内容，而这些内容又被当做了某个地址，我们就很难分清它到底是合法还是非法了。

​		因此建议初始化所有的指针，并且在可能得情况下，尽量等定义了对象之后再定义指向它的指针。如果实在不清楚指针应该指向何处，就把它初始化为nullptr或者0，这样程序就能检测并指导它没有指向任何具体的对象了。

​		Because of this, you should always initialize your pointers to a known value.

#### 多级指针

​		高级指针是对低级变量的索引，当使用高级指针时，多个 *修饰符的含义是，对本指针变量的值属性寻址得到的变量，可递归寻址的次数。

> ​		如三级指针，```int ***p```，对其寻址一次，得到一个二级指针变量```int **pp```，对这个二级指针变量寻址一次，得到一个一级指针变量```int *ppp```，对这个一级指针可再寻址得到一个其值属性不可再寻址的对象。
>
> ​		因此，传出参数需要更高一级的指针进行地址传递（passing by address）

![image-20220724165625364](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220724165625364.png)

​		对于一个高级指针，需要访问哪个级别的地址，就解引用到哪一级。

​		对于多级指针的堆上内存分配，因为分配器不能识别指针级别，即本质原因是无法的语法层面判别变量的可寻址性（都是整型数，指针如果不明确指明级别，无法判定其某一级的值是不是地址记录）。所以，对于动态分配多级指针，需要逐级分配。且多级指针不同于多维数组，没有长度对齐的限制。

```c++
int** array { new int*[10] }; // allocate an array of 10 int pointers — these are our rows
for (int count = 0; count < 10; ++count)
    array[count] = new int[count+1]; // these are our columns

for (int count = 0; count < 10; ++count)
    delete[] array[count];
delete[] array; // this needs to be done last
```

> ​		一般来说，声明符中修饰符的个数并没有限制。当有多个修饰符连写在一起时，按照其逻辑关系详加解释即可。
> ​		以指针为例，指针是内存中的内存，像其他对象一样也有自己的地址，因此允许把指针的地址再存放到另一个指针当中。
> ​		通过的个数可以区分指针的级别。也就是**表示指向指针的指针**，表示指向指针的指针的指针。
> ​		此时为了取到某个层级的值，需要对应多次的解引用。

### 指针与资源所有权

​		指针从资源角度来说，具有两种逻辑语义：

- 资源句柄，具有资源所有权
- 资源引用标识，具有资源访问权

​		在程序中，对同一资源的索引，需要十分小心，哪个指针是负责资源生命周期的资源句柄，其他的指针则是进行资源访问的引用标识。

​		一种比较好的策略是，将具有资源所有权的指针放置于`vector/string/unique_ptr`等具有RAII功能的资源句柄类中。即可假定所有不在资源句柄中的指针都不负责管理资源。

### 指针的最佳实践

- 在`delete`后置`nullptr`，避免空悬。并在访问指针之前判断是否访问空指针。

  - `delete`指针无须判空

    > If ptr is non-null, the dynamically allocated variable will be deleted. If it is null, nothing will happen.

- 明确所有权和访问权

  > 当多个指针指向同一对象时，明确负责维持对象生存期的指针，它拥有对象的所有权。负责销毁对象。
  >
  > 其他的指针具有访问权，只能对对象进行访问。

- 避免循环引用，无法避免时使用弱指针`std::weak_ptr`进行访问，使用`std::shared_ptr`维持对象。






## 引用声明符

​		与直接使用对象名来访问对象相比，指针存在以下问题：

- 不直观，对一个对象的访问形式`*p / p->`
- 同一个指针在不同的时刻，可以指向不同的对象
- 指针可能是`nullptr`，也可能指向一个非理想的值。

​		**引用**引用并非对象，是一个已存在对象的别名。定义并使用引用，同于直接对原对象的直接访问。

> ​		即引用本质上和被索引的对象身份相同。用以解决上述问题。
>
> ​		引用最常用的场景是进行参数传递和返回值传递。函数返回引用时，需要注意局部变量的生命周期

​		引用必须绑定在一个已存在的对象上（初始化），初始化完成后，引用将和它的初始值对象一直绑定在一起。引用只能绑定在对象上，而不能与字面值或者某个表达式的计算结果绑定在一起。

​		可以创建一个函数的引用，虽然这并不常用。

​		引用相比于指针，开销相等，主要区别在于：

> 使用引用的效率实际上是和指针一样的，4bytes（32bit），8bytes（64bit）

- 直接访问
- 一致身份，始终绑定在初始化的对象上
- 不存在空引用（但同指针一样具有空悬引用）

​		现代c++包含两种引用：左值引用和右值引用。还有组合了`const`的左值引用。共形成三种引用形式：

- 左值引用(`lvalue reference`），引用期望改变的对象
- 常引用（`const reference`），引用不期望改变的对象
- 右值引用(`rvalue reference`)，引用期望销毁的对象（对象的值在被引用后不再被对象保留，对象进入未初始化状态）。

> 左值引用只能被左值初始化，右值引用只能被右值初始化，`const`引用能够被左值和右值初始化。
>
> 在使用右值进行初始化时，右值引用和`const`引用虽然都能延长对象的生命周期，但他们的目的是不同的：
>
> - 右值引用的目的是为了进行破坏性接管，使用右值引用接收的对象不应再被使用
> - 引用的目的是为了进行代价低廉的对象访问（间接访问），而`const`是为这种访问进行只读限制

​		使用三者的时候，首先在设计之初要依次明确所引用的资源：

1. 是否需要被移动

   > 关于移动，是对象本身的语义性质之一。在类的语义中进行说明。
2. 是否拷贝昂贵
3. 是否只读

### lvalue reference

​		An **lvalue reference** acts as an alias for an existing lvalue (such as a variable).

​		To declare an lvalue reference type, we use an ampersand (&) in the type declaration:

```c++
int &
```

​		An **lvalue reference variable** is a variable that acts as a reference to an lvalue (usually another variable).

​		Much like constants, all references must be initialized.

```c++
int& ref{x}; //  ref is an lvalue reference variable that can now be used as an alias for variable x
```

>  Modern C++ programmers tend to prefer attaching the ampersand to the type, as it makes clearer that the reference is part of the type information, not the identifier.

​		Lvalue references can’t be bound to non-modifiable lvalues or rvalues

> otherwise you’d be able to change those values through the reference, which would be a violation of their const-ness

​		In most cases, the type of the reference must match the type of the referent (there are some exceptions to this rule that we’ll discuss when we get into inheritance):

​		Lvalue references to `void` are disallowed.	

​		a reference in C++ cannot be **reseated**, meaning it cannot be changed to reference another object.

​		Reference variables follow the same scoping and duration rules that normal variables do.

​		the lifetime of a reference and the lifetime of its referent are independent.When a reference is destroyed before the referent, the referent is not impacted.

```c++
int x { 5 };

{
  int& ref { x };   // ref is a reference to x
  std::cout << ref; // prints value of ref (5)
} // ref is destroyed here -- x is unaware of this

std::cout << x; // prints value of x (5)
```

​		References are not objects in C++. A reference is not required to exist or occupy storage.

> If possible, the compiler will optimize references away by replacing all occurrences of a reference with the referent. However, this isn’t always possible, and in such cases, references may require storage.

​		Because references aren’t objects, they can’t be used anywhere an object is required.In cases where you need a reference that is an object or a reference that can be reseated, `std::reference_wrapper` provides a solution.

> you can’t have a reference to a reference, since an lvalue reference must reference an identifiable object

```c++
int var{};
int& ref1{ var };  // an lvalue reference bound to var
int& ref2{ ref1 }; // an lvalue reference bound to var
```

​		在c++中，引用的实现方式应该类似于指针常变量，每次使用引用进行访问实际上是对该指针常变量进行解引用然后访问对象。但实际上，引用不是对象，而指针是对象。

> 有些编译器因此做出优化，使用符号表来记录引用，运行时无须任何指针对象表示该引用。



#### Dangling references 悬垂引用

​		When an object being referenced is destroyed before a reference to it, the reference is left referencing an object that no longer exists. Such a reference is called a **dangling reference**. 

​		Accessing a dangling reference leads to undefined behavior.

```c++
std::string& f()
{
    std::string s = "Example";
    return s; // 退出 s 的作用域：调用其析构函数并解分配其存储
}
 
std::string& r = f(); // 悬垂引用
std::cout << r;       // 未定义行为：从悬垂引用读取
std::string s = f();  // 未定义行为：从悬垂引用复制初始化
```





#### pointer and reference

​		both pointers and references provide a way to indirectly access another object. The primary difference is that with pointers, we need to explicitly get the address to point at, and we have to explicitly dereference the pointer to get the value. With references, the address-of and dereference happens implicitly.

​		There are some other differences between pointers and references worth mentioning:

- References must be initialized, pointers are not required to be initialized (but should be).
- References are not objects, pointers are.
- References can not be reseated (changed to reference something else), pointers can change what they are pointing at.
- References must always point at an object, pointers can point to nothing (we’ll see an example of this in the next lesson).
- References are “safe” (outside of dangling references), pointers are inherently dangerous

​		**Favor references over pointers whenever possible**

 		pointer abilities are also inherently dangerous: A null pointer runs the risk of being dereferenced, and the ability to change what a pointer is pointing at can make creating dangling pointers easier:

​		Since references can’t be bound to null, we don’t have to worry about null references.

​		And because references must be bound to a valid object upon creation and then can not be reseated, dangling references are harder to create.

​		references should be favored over pointers, unless the additional capabilities provided by pointers are required.

### const reference

​		By using the `const` keyword when declaring an lvalue reference, we tell an lvalue reference to treat the object it is referencing as const.

​		Such a reference is called an **lvalue reference to a const value** (sometimes called a **reference to const** or a **const reference**).

​		初始化左值引用必须是一个实体左值，但初始化`const T&`常量引用无须是左值，甚至可以不是`T`类型的：

1. 如果必要的话，先执行目标为类型`T`的隐式类型转换。
2. 所得的值构造一个`T`类型的临时对象
3. 把这个临时对象作为`const T&`的初始值。

​		常量引用能够将临时对象的生命周期延长到常量引用的作用域（生命周期）结束为止。

```c++
const int x { 5 };    // x is a non-modifiable lvalue
const int& ref { x }; // okay: ref is a an lvalue reference to a const value
```

​		Lvalue references to const can also bind to modifiable lvalues.

> 引用是指针之外另一种间接访问原对象的途径，对引用的const修饰是在限定这种途径访问原对象的权限，而不影响原对象本身的可访问权限。

```c++
int x { 5 };          // x is a modifiable lvalue
const int& ref { x }; // okay: we can bind a const reference to a modifiable lvalue
std::cout << ref;     // okay: we can access the object through our const reference
ref = 7;              // error: we can not modify an object through a const reference
x = 6;                // okay: x is a modifiable lvalue, we can still modify it through the original identifier
```

​		**Favor `lvalue references to const` over `lvalue references to non-const` unless you need to modify the object being referenced.**

​		lvalues references to const can also bind to rvalues， a temporary object is created and initialized with the rvalue, and the reference to const is bound to that temporary object.And，lvalues references can extend the lifetime of temporary objects.

> ​		Temporary objects are normally destroyed at the end of the expression in which they are created.
>
> ​		To avoid dangling references in such cases, C++ has a special rule: When a const lvalue reference is bound to a temporary object, the lifetime of the temporary object is extended to match the lifetime of the reference.
>
> ```c++
> const int& ref { 5 }; // The temporary object holding value 5 has its lifetime extended to match ref
> 
> std::cout << ref; // Therefore, we can safely use it here
> ```

​		const左值引用，可以绑定一个可变左值，不可变左值，以及右值。这使得引用的使用方式更加灵活，允许向const左值引用的函数参数传递任意的左值或右值，而不进行实参的拷贝。



### rvalue reference

​		右值引用是在原变量地址上，将原标识符与该地址的关联注销，然后建立新标识符与该地址的关联关系。

​		右值引用必须接管对象的生命周期。因此，右值引用只能被右值初始化。

右值引用基于c++11的移动语义，字面上是将对象的内存模型进行转义，实际上是对同一块内存的标识符权限的转移。

> 右值引用应该也是基于指针的，因为其核心概念还是对于同一地址的动态绑定，所以要使用指针进行地址的记录与传递。所以使用右值引用的效率应该和使用指针进行参数传递的效率是一样的。
>
> 其区别就在于，右值引用会伴随着生命周期管理权的转移。
>
> 所以，使用右值引用时，要注意原对象（原标识符）应该不再使用。

​		右值引用无所谓`const`与否，因为右值引用完全且排他的接管了原来的对象。

```c++
int x{ 5 };
int &lref{ x }; // l-value reference initialized with l-value x
int &&rref{ 5 }; // r-value reference initialized with r-value 5	
int &&rref2{std::move(x)}; // 释放 x对对象的生命周期管理，并由rref2接管。
```

​		右值引用不能被任何形式的左值初始化。

- 右值引用能够延长所绑定对象的生命周期
  - lvalue reference to const 也能做延长所绑定对象的生命周期
- 非const右值引用允许修改rvalue

```c++
#include <iostream>

class Fraction
{
private:
    int num_;
    int denom_;

public:
    Fraction( int num = 0, int denom = 1 ): num_ { num }, denom_ { denom } { }
    friend std::ostream &operator<<( std::ostream &out, const Fraction &f1 )
    {
        return out << f1.num_ << "/" << f1.denom_;
    }
};

int main()
{
    auto &&rref { Fraction{3, 5} }; // rvalue reference to temporary fraction
    // f1 of operator<< binds to the temporary, no copies are created.
    std::cout << rref << '\n';

    return 0;
}// rref(and the temporary Fraction) goes out of scope here
```

​		右值引用实际上将右值、临时对象左值化到自身，因为右值引用必须接管对象的生命周期：

```c++
#include <iostream>

int main()
{
    int &&rref{ 5 }; // because we're initializing an r-value reference with a literal, a temporary with value 5 is created here
    // Although variable rref has type r-value reference to an integer, it is actually an l-value itself (as are all named variables). 
    rref = 10;
    std::cout << rref << '\n';

    return 0;
}
```

​		右值引用通常用于函数形参，且左值引用和右值引用作为不同类型形成重载。

```c++
#include <iostream>

void fun(const int &lref) // l-value arguments will select this function
{
	std::cout << "l-value reference to const\n";
}

void fun(int &&rref) // r-value arguments will select this function
{
	std::cout << "r-value reference\n";
}

int main()
{
	int x{ 5 };
	fun(x); // l-value argument calls l-value version of function
	fun(5); // r-value argument calls r-value version of function

	return 0;
}
```

​		几乎不应返回右值引用，就像不应该返回左值引用一样。大多数情况下，引用绑定的对象会出界析构，最终返回一个悬垂引用。

​		

### 引用的引用

​		引用的引用只能用于别名，或模板类型的参数，不能在声明语句中声明引用的引用。

​		发生引用的引用时，实际的值类别由以下规则推导。即左值引用永远绑定左值，左值永远优先。这种推导被称为引用折叠。

```c++
using r = int &&;
using l = int &;

using rr = r&&; // int && && -> int &&
using rl = r&;  // int && & -> int &
using lr = l&&; // int & && -> int &
using ll = l&;  // int & & -> int &
```



### 指针与引用

​		指针和引用是一体两面，各有不足，也各有所长。除了在重载运算符时基于语法不得不使用引用时，两者基本可以互换。

​		引用绑定对象，提供了直观而稳定的操作形式。

​		指针在对象间迁移，提供了灵活而又具有一定风险的操作形式。

​		应该在不同的场景下，应该以减少错误为目标在二者之间进行选择。

#### 引用的指针

​		引用本身不是一个对象，因此不能定义指向引用的指针。

#### 指针的引用

​		指针是对象，所以存在对指针的引用。

```cpp
int *p;
int *&r = p; // r是一个对指针p的引用
```

​		离变量名最近的符号（此例中是&r的符号&）对变量的类型有最直接的影响，因此r是一个引用。声明符的其余部分用以确定r引用的类型是什么。

> 此例中的符号＊说明r引用的是一个指针。



##  数组声明符

​		An **array** is an aggregate data type that lets us access many variables of the same type through a single identifier.

​		Each of the variables in an array is called an **element**. Elements do not have their own unique names. Use the array name, along with the **subscript operator ([])**, and a parameter called a **subscript** (or **index**) that tells the compiler which element we want. This process is called **subscripting** or **indexing** the array.

​		数组是增加了定界维度的指针，所谓数组的退化，即退化了定界维度，成为指针。

​		即数组也存在非法越界访问，指针语义的拷贝等问题。

### 数组分类

#### fixed array

​		 A **fixed array** (also called a **fixed length array** or **fixed size array**) is an array where the length is known at compile time.

```c++
// int prime[5]{}; // hold the first 5 prime numbers
int prime[5]{ 2, 3, 5, 7, 11 }; // use initializer list to initialize the fixed array
prime[0] = 2; // The first element has index 0
prime[1] = 3;
prime[2] = 5;
prime[3] = 7;
prime[4] = 11; // The last element has index 4 (array length-1)
```

​		An array subscript can be a literal value, a variable (constant or non-constant), or an expression that evaluates to an integral type.

> Note that non-const variables or runtime constants cannot be used:
>
> ```c++
> // using a non-const variable
> int daysPerWeek{};
> std::cin >> daysPerWeek;
> int numberOfLessonsPerDay[daysPerWeek]{}; // Not ok -- daysPerWeek is not a compile-time constant!
> 
> // using a runtime const variable
> int temp{ 5 };
> const int daysPerWeek{ temp }; // the value of daysPerWeek isn't known until runtime, so this is a runtime constant, not a compile-time constant!
> int numberOfLessonsPerDay[daysPerWeek]{}; // Not ok
> ```
>
> ​		枚举项可以作为下标，但枚举类成员不可以。

​		*arrays always count starting from 0! *,For an array of length N, the array elements are numbered 0 through N-1. This is called the array’s **range**.



##### 定长数组初始化

​		 C++ provides a more convenient way to initialize entire arrays via use of an **initializer list**. 

​		if there are less initializers in the list than the array can hold, the remaining elements are initialized to 0 (or whatever value 0 converts to for a non-integral fundamental type). This is called **zero initialization**.

```c++
int array[5]{ 7, 4, 5 }; // only initialize first 3 elements
// 7, 4, 5, 0, 0
```

​		Consequently, to initialize all the elements of an array to 0·

```c++
// Initialize all elements to 0
int array[5]{ };

// Initialize all elements to an empty string
std::string array[5]{ };
```

​		If the initializer list is omitted, the elements are uninitialized, unless they are a class-type

```c++
// uninitialized
int array[5];

// Initialize all elements to an empty string
std::string array[5];
```

​		Explicitly initialize arrays, even if they would be initialized without an initializer list.

​		the compiler can figure out the length of the array for you, and you can omit explicitly declaring the length of the array.

```c++
int array[]{ 0, 1, 2, 3, 4 }; // let the initializer list set length of the array
std::cout << "The array has: " << std::size(array) << " elements\n";
```

> `std::size`在c++17可用，作为低版本替代，可以使用`sizeof(array)/sizeof(array[0])`

​		以`new/malloc`分配的数组也是定长数组的一种。

#### VLA: Variable-length array

​		变长数组的实际意思是`以变量作为长度的数组`，区别于`以常数作为长度的数组`。c99开始支持vla。c++不支持vla。

​		

#### dynamic arrays

​		Fixed arrays decay into pointers, losing the array length information when they do, and dynamic arrays have messy deallocation issues and are challenging to resize without error.

​		传统的动态数组通过维护一个header结构，通过动态内存分配来实现动态数组，并防止退化。

​		`std::vector`完备的实现了这一抽象，往往也使用`std::vector`来替代数组使用。

> c++标准库提供了`std::array`，来提供非退化的`fixed array`抽象，但不具备动态扩容的抽象。

#### multidimensional array 多维数组

​		C++语言中没有多维数组，通常所说的多维数组其实是数组的数组。

​		An array of arrays is called a **multidimensional array**.

```c++
int array[3][5]; // a 3-element array of 5-element arrays
```

​		In a two-dimensional array, it is convenient to think of the first (left) subscript as being the row, and the second (right) subscript as being the column. This is called **row-major** order.

```c++
int array[3][5]
{
  { 1, 2 }, // row 0 = 1, 2, 0, 0, 0
  { 6, 7, 8 }, // row 1 = 6, 7, 8, 0, 0
  { 11, 12, 13, 14 } // row 2 = 11, 12, 13, 14, 0
};
```

> - 一些编译器支持不使用`inner braces`来全部初始化，而不是按行初始化
> - 但是推荐使用`inner braces`，可读性更好的同时，能够缺省的进行零初始化。

​		多维数组可以省略第一行，其本质是从初始化器列表推断多维数组声明信息中的类型信息：

> ```c++
> int array[][5]
> {
> { 1, 2, 3, 4, 5 },
> { 6, 7, 8, 9, 10 },
> { 11, 12, 13, 14, 15 }
> };
> ```
>
> ​		其本质是声明，即声明一个退化的指针，并为该退化的指针说明长度，以使得编译器重组该数组
>
> ```c++
> int (arra*)[5] {
> [,,,,],
> [,,,,],
> ...
> }
> ```
>
> ​		声明时只可见当前最外层的说明，所以推导式的声明必须通过非退化类型来提供完整的类型信息。因此，即便三维以上的高维数组，也只能省略第一维来推导。

​		当程序使用多维数组的名字时，也会自动将其转换成指向数组首元素的指针。

​		多维数组可以通过指针的使用，灵活的建立堆上内存。

```c++
int** array {new int*[10]}; // int* array[10];

int x{7};
int (*array)[5] {new int[x][5]}; // int (*array)[5], pointer to array[5], auto array{new int[x][5]}
```

​		在实际应用中，通常也将多维数组线性化，如3x5的二维数组，线性化为15个元素的一维数组。

​		通过使用auto或者decltype就能尽可能地避免在数组前面加上一个指针类型了、当然，使用标准库函数begin和end也能实现同样的功能，而且看起来更简洁一些

```c++
for(auto p = ia; p != ia + 3; ++p)
    for(auto q = *p; q != *p + 4; ++q)
        cout << *q << ' ';

for(auto p = begin(ia); p != end(ia); ++p)
    for(auto q = begin(*p); q != end(*p); ++q)
        cout << *q << ' ';
```

​		读、写和理解一个指向多维数组的指针是一个让人不胜其烦的工作，使用类型别名（参见2.5.1节，第60页）能让这项工作变得简单一点儿

```c++
using int_array = int[4];
typedef int int_array[4];

for(int_array *p = ia; p != ia + 3; ++p) {
    for(int *q = *p; q != *p + 4; ++q)
        cout << *q << ' ';
}
```



### 数组退化 array decay

​		when a fixed array is used in an expression, the fixed array will **decay** (be implicitly converted) into a pointer that points to the first element of the array.

```c++
int array[5]{ 9, 7, 5, 3, 1 };

// print address of the array's first element
std::cout << "Element 0 has address: " << &array[0] << '\n';

// print the value of the pointer the array decays to
std::cout << "The array decays to a pointer holding address: " << array << '\n';
```

​		对于数组来说，其类型是`type[len]`；对于指针来说，其类型是`type*`。

​		在大多数情况使用指针进行定位的上下文中，二者基本可以认为相同，但涉及关于偏移操作，因类型的不同而导致不同的行为。

```c++
int array[5]{ 9, 7, 5, 3, 1 };

// Deferencing an array returns the first element (element 0)
// The array (of type int[5]) gets implicitly converted into a pointer (of type int*)
std::cout << *array; // will print 9!
std::cout << array[0]; // will print 9

// The array decays into a pointer of type int*, and our pointer (also of type int*) has the same type
int* ptr{ array };
std::cout << *ptr << '\n'; // will print 9

//  sizeof returns the size of the entire array (array length * element size). 
std::cout << sizeof(array) << '\n'; // will print sizeof(int) * array length 20 
// sizeof returns the size of the pointer (in bytes).
std::cout << sizeof(ptr) << '\n'; // will print the size of a pointer 4

// &array is int(*)[5]
std::cout << array << '\n';	 // type int[5], prints 009DF9D4
std::cout << &array << '\n'; // type int(*)[5], prints 009DF9D4
// &ptr is int**
std::cout << ptr << '\n';	 // type int*, prints 009DF9D4
std::cout << &ptr << '\n';	 // type int**, prints 009DF9C8
```

> ​		A fixed array knows how long the array it is pointing to is. A pointer to the array does not.

​		C++ does not copy an array when an array is passed into a function. When passing an array as an argument to a function, a fixed array decays into a pointer.

```c++
#include <iostream>

void printSize(int* array)
// void printSize(int array[]) //  even if the parameter is declared as a fixed array
{
    // array is treated as a pointer here
    std::cout << sizeof(array) << '\n'; // prints the size of a pointer, not the size of the array!
}

int array[]{ 1, 1, 2, 3, 5, 8, 13, 21 };
std::cout << sizeof(array) << '\n'; // will print sizeof(int) * array 
printSize(array); // the array argument decays into a pointer here
```

>  C++ implicitly converts parameters using the array syntax ([]) to the pointer syntax (*), two function declarations are identical:

​		**Arrays in structs and classes don’t decay**, This yields a useful way to prevent decay if desired, and will be valuable later when we write classes that utilize arrays.



## 函数声明符
