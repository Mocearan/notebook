# type

​		**data type** tell compiler how to interpret the contents of memory in some meaningful way. 

​		Every name and every expression has a type that determines the operations that may be performed on it.

​		给原本无意义的**位序列**提供了语义含义。

---

​		c++是一种静态类型语言，类型系统是基础。编译器在处理任何实体（对象、值、引用、函数、名字和表达式）时，必须清楚的确定其类型。

​		类型与实现它们的机器硬件密切相关。c++语言本身提供的类型都是计算机硬件能直接处理的基础类型，没有内置的高级数据类型。作为补充，通过标准库来提供许多高级数据类型。

​		类型决定了实体能进行哪些操作，这些操作给原本无意义的**位序列**提供了语义含义。

​		**类型对于不同的数据类型，应用不同的数据解译方案**，这就使得同样的数据（纯粹的二进制数据，即**位序列**），可以表达多种多样的含义，从而构建起了计算机抽象现实世界的基础。

- A *type* defines a set of possible values and a set of operations (for an object).
- An *object* is some memory that holds a value of some type.
- A *value* is a set of bits interpreted according to a type.
- A *variable* is a named object.



## 类型检查

​		c++通过类型系统和一些辅助的特性，帮助程序员表达设计。

​		静态类型和编译时的类型检查，对内置类型和用户自定义类型提供同等的支持。

​		静态类型系统的目标是支持基于对象的编程风格而：将不同的想法表示为不同的类型，如整型、浮点型、字符串、原始内存和对象，而不是到处使用泛型。

​		c++保留了大多数C语言的类型特性，保留了语言特性到硬件的直接映射。

​		c++致力于完整的类型安全，但同C一样，是“强类型、弱检查”的。

## memory and type

​		The smallest unit of memory is a **binary digit** (also called a **bit**), which can hold a value of 0 or 1. 

​		Memory is organized into sequential units called **memory addresses** (or **addresses** for short). the memory address allows us to find and access the contents of memory at a particular location.

​		 the memory address allows us to find and access the contents of memory at a particular location.each memory address holds 1 byte of data.

​		We use a **data type** (often called a “type” for short) to tell the compiler how to interpret the contents of memory in some meaningful way. 

> When you give an object a value, the compiler and CPU take care of encoding your value into the appropriate sequence of bits for that data type, which are then stored in memory (remember: memory can only store bits). 

​		most objects actually take up more than 1 byte of memory. A single object may use 2, 4, 8, or even more consecutive memory addresses. The amount of memory that an object uses is based on its data type.

​		we typically access memory through variable names (and not directly via memory addresses), the compiler is able to hide the details of how many bytes a given object uses from us. 



​		计算机以2的整数次幂个bit为单位划块来处理内存数据，可寻址的最小内存块称为“字节”（byte）。存储的基本单元称为“字”（word），通常由几个字节组成，视不同机器有所不同。

​		c++中，一个字节要至少能容纳机器基本字符集中的字符。大多数机器的字节由8bit构成，字则由32或64bit构成，也就是4或8字节。

​		计算机将内存中的每个字节与一个数字（address，地址）关联，在一个字节为8bit、字为32bit的机器上，我们可能看待一个字的内存区如图：

![image-20220608063536846](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220608063536846.png)

​		左侧地址，右侧是每个字节的8bit。

​		可以使用某个地址来表示从这个地址开始的大小不同的比特串。为了赋予内存中某个地址明确地含义，必须首先知道存储在该地址的数据的类型。类型决定了数据所占的比特数以及该如何解释这些比特的内容。

​		即，**内存只是简单的存放了二进制数据，对这些二进制数据的定义与解释依赖于类型，不同类型有对这个数据的不同的解释方式。导致同一数据在不同的类型上有不同的表现。**



## 类型的分类

### 按照声明分类

- 基础类型：`std::is_fundamental`
  - `void`类型：`std::is_void`
  - `std::nullptr_t`：`std::is_null_pointer`
  - 算数类型：`std::is_arithmetic`
    - 浮点类型（float、double、long double）：`std::is_floating_point`
    - 整数类型：`std::is_integral`
      - `bool`类型
      - 字符类型
        - 窄字符类型
          - 通常字符类型：`char、signed char、unsigned char`
          - `char8_t`
        - 宽字符类型：`char16_t、char32_t、wchar_t`
      - 有符号整数类型：`short int、long int、long long int`
      - 无符号整数类型：`unsigned short int、unsigned int、unsigned long int、unsigned long long int`
- 复合类型：`std::is_compound`
  - 引用类型：`std::is_reference`
    - 左值引用：`std::is_lvalue_reference`
      - 到对象的左值引用类型+
      - 到函数的左值引用类型
    - 右值引用：`std::is_rvalue_reference`
      - 到对象的右值引用类型
      - 到函数的右值引用类型
  - 指针类型：`std::is_pointer`
    - 指向对象的指针（对象指针）类型
    - 指向函数的指针（函数指针）类型
  - 指向成员的指针（成员指针）：`st d::is_member_pointer`
    - 指向数据成员的指针类型：`std::is_member_object_pointer`
    - 指向成员函数的指针类型：`std::is_member_function_pointer`
  - 数组类型：`std::is_array`
  - 函数类型：`std::is_function`
  - 枚举类型：`std::is_enum`
  - 类类型：
    - 非联合体类型：`std::is_class`、`struct`
    - 联合体类型：`std::is_union`

**对于除了引用和函数外的每个类型，类型系统还支持该类型的三个附加cv限定版本（const、volatile、const volatile）**

> **cv限定：**
>
> ​	类型说明符序列中，用以指定被声明对象的常量性或易变性。
>
> ps：c++/基础语言/声明/说明符/cv类型限定符

​		枚举类型和类类型也常被称为用户自定义类型（user-defined type）。基础类型和除用户自定义类型外的其他复合类型称为内置类型。

### 按照性质分类

​		根据类型的各项性质，将它们分组到不同的类别之中：

- 对象类型

  是非函数类型、非饮用类型且非`void`类型的（可有cv限定的）类型：`std::is_object`

- 标量类型

  可有cv限定的算数、指针、成员指针、枚举和`std::nullptr_t`类型：`std::is_scalar`

- 类型特征库中的具名要求类型

  - 平凡类型：`std::is_trivial`

  - POD类型：`std::is_pod`

  - 字面类型：`std::is_literal_type`

  - 其他类别

    > **类型支持：**
    >
    > ps：c++语言/标准库/语言支持库/类型支持
    >
    > **具名要求：**
    >
    > ​		C++标准的规范性文本中使用的具名要求，用以定义标准库的期待。
    >
    > ps：c++语言/标准库/具名要求

### 按照结构分类

> **Types in C++ are：**
>
> **Short version:** 
>
> -   Object types: scalars, arrays, classes, unions
> -   Reference types
> -   Function types
> -   (Member types) [see below]
> -   void
>
> ------
>
> **Long version**
>
> -   Object types
>     -   Scalars
>         1.  arithmetic (integral, float)
>         2.  pointers: T * for any type T
>         3.  enum
>         4.  pointer-to-member
>         5.  nullptr_t
>     -   Arrays: T[] or T[N] for any complete, non-reference type T
>     -   Classes: class Foo or struct Bar
>         1.  Trivial classes
>         2.  Aggregates
>         3.  POD classes
>         4.  (etc. etc.)
>     -   Unions: union Zip
> -   References types: T &, T && for any object or free-function type T
> -   Function types
>     -   Free functions: R foo(Arg1, Arg2, ...)
>     -   Member functions: R T::foo(Arg1, Arg2, ...)
> -   void
>
> Member types work like this. A member type is of the form T::U, but you can't have objects or variables of member type. You can only have member *pointers*. A member pointer has type T::* U, and it is a pointer-to-member-object if U is a (free) object type, and a pointer-to-member-function if U is a (free) function type.
>
> All types are complete except void, unsized arrays and declared-but-not-defined classes and unions. All incomplete types except void can be completed.
>
> All types can be const/volatile qualified.
>
> The <type_traits> header provides trait classes to check for each of these type characteristics.

c++支持广泛的数据类型，定义了内置数据类型也为程序员提供了自定义数据类型的机制。因此c++标准库定义了一些更加复杂的数据类型，比如可变长字符串和向量等。

> c++类型系统由语言支持的类型和标准库提供的类型支持共同构建。



## type aliases 类型别名

​		程序的复杂性部分来自于类型复杂性。

- 一些类型名，难记又容易写错，且无法明确体现其真实的目的和含义
- 难以确定限定使用的类型，只能从程序的上下文环境中获得帮助

​		类型别名（type alias）是某种类型的同义词。通常使用类型别名来简化一个命名复杂的类型名，或注记一个通用类型为特定场景下有语义的名字。

​		有两种用于定义类型别名的方法：

- typedef

  ```c++
  typedef double wages;
  typedef wages base, *p;//*p 是double* 的同义词
  ```

- 别名声明（alias declaration）  using

  ```c++
  using SI = Sales_item;
  SI item;
  ```




### using

​		In C++, **using** is a keyword that creates an alias for an existing data type.

`using type_name_t = type;`

> Many type alias names use a “_t” or “_type” suffix to help decrease the chance of naming collisions with other identifiers.

```c++
using distance_t = double; // define distance_t as an alias for type double
distance_t milesToDestination{ 3.4 }; // defines a variable of type double
```

​		An alias does not actually define a new type -- it just introduces a new identifier for an existing type. An alias is completely interchangeable with the aliased type.

> This allows us to do things that are syntactically valid but semantically meaningless. For example:
>
> ```c++
> using miles_t = long; 
> using speed_t = long;
> 
> miles_t distance{5};
> speed_t mhz{3200};
> // both long
> 
> // The following is syntactically valid (but semantically meaningless)
> distance = mhz;
> ```
>
> ​		Because the compiler does not prevent these kinds of semantic errors for type aliases, we say that aliases are not **type safe**. In spite of that, they are still useful.
>
> > ​		Some languages support the concept of a **strong typedef** (or strong type alias). A strong typedef actually creates a new type that has all the original properties of the original type, but the compiler will throw an error if you try to mix values of the aliased type and the strong typedef. As of C++20, C++ does not directly support strong typedefs (though enum classes are similar), but there are quite a few 3rd party C++ libraries that implement strong typedef-like behavior.

​		Because scope is a property of an identifier, type alias identifiers follow the same scoping rules as variable identifiers: a type alias defined inside a block has block scope and is usable only within that block, whereas a type alias defined in the global namespace has file scope and is usable to the end of the file.

​		If you need to use one or more type aliases across multiple files, they can be defined in a header file and #included into any code files that needs to use the definition:

```c++
// mytypes.h
#ifndef MYTYPES
#define MYTYPES

	using miles_t = long;
	using speed_t = long;

#endif // MYTYPES
```

​		

### typedef

​		**typedef** (which is short for “type definition”) is a keyword with the same semantics as “using”, but reversed syntax.

`typedef type type_name_t;`

​		Typedefs are still in C++ for historical reasons, but their use is discouraged.

​		the syntax for typedefs can get ugly with more complex types. 

> ```c++
> typedef int (*fcn_t)(double, char); // fcn_t hard to find
> using fcn_t = int(*)(double, char); // fcn_t easier to find
> ```



#### 型别名对指针、常量的处理原则

 		如果某个类型别名指代的是复合类型或常量，那么把它用到声明语句里就会产生意想不到到的后果。

>   **禁止替换：**
>
>   ​		使用了类型别名的声明语句，人们会错误的将原来的类型替换进别名语句来理解。这是错误的。

```c++
typedef char* pstring;

// cstr 指向char 的常量指针。即char* const pstring（顶层const）
// 不等同于 const char* pstring;（底层const）
// const pstring 是声明语句中的基本数据类型。
// 如果简单替换后，const char *cstr = 0; 基本数据成为const char，*成为了声明符的一部分，说明声明了一个const char类型的指针。
const pstring cstr = 0; 

// ps 是一个二级指针，存储了一个指向char类型的常量指针。 
// ps 本身底层const
const pstring *ps;  

// 也就是说，会将typedef、using定义的复杂类型当做个整体来看待，基于此别名的类型修饰，是针对这个复杂类型本身的。
```



### 使用场景

#### using type aliases for platform independent coding

​		One of the uses for type aliases is that they can be used to hide platform specific details. 

​		In order to make sure each aliased type resolves to a type of the right size, type aliases of this kind are typically used in conjunction with preprocessor directives:

```c++
#ifdef INT_2_BYTES
using int8_t = char;
using int16_t = int;
using int32_t = long;
#else
using int8_t = char;
using int16_t = short;
using int32_t = int;
#endif
```

> ​		The fixed-width integers (such as `std::int_fast16_t` and `std::int_least32_t`) and `size_t` type are actually just type aliases to various fundamental types.

#### using type aliases to make complex types simple

```c++
using pairlist_t = std::vector<std::pair<std::string, int>>; // make pairlist_t an alias for this crazy type

bool hasDuplicates(pairlist_t pairlist) // use pairlist_t in a function parameter
{
    // some code here
    return false;
}

int main()
{
     pairlist_t pairlist; // instantiate a pairlist_t variable

     return 0;
}
```



#### using type aliases for legibility

​		Type aliases can also help with code documentation and comprehension.

```c++
using testScore_t = int;
testScore_t gradeTest();
```



#### using type aliases for easier code maintenance

​		However, if you use type aliases, then changing types becomes as simple as updating the type alias (e.g. from `using studentID_t = short;` to `using studentID_t = long;`).





## type deduction 类型推导

​		现在编译器已经可以通过值来推导类型的类型，不再需要编程人员显式提供类型了。

###  auto 类型说明符

​		c++11新标准引入auto类型说明符。编译通过初始值来推导变量的类型。

>    https://zh.cppreference.com/w/cpp/keyword/auto
>

-  auto定义的变量必须有初始值。
- 使用auto也能在一条语句中声明多个变量，因为一条声明语句只能有一个基本数据类型。

```c++
int n = 200;
auto n = 200;
auto nx = n;
auto nl = 200L;
auto i = 0u, *p = &i;
std::cout << typeid(n1).name() << std::endl; // l -> long

Sales_item val1, val2;
auto item = val1 + val2;
std::cout << item << std::endl;
```



#### auto 复杂类型推导

​		编译器推断出的auto类型有时候和初始值的类型并不完全一样，编译器会适当地改变结果类型使其更符合初始化规则。

```c++
int i = 0, &r = i;
const int ci = i, & cr = ci;
```

##### 引用的推断

​		使用引用其实是使用引用的对象，特别是当引用被用作初始值时，真正参与初始化的其实是引用对象的值。

```c++
/*
	使用引用其实是使用引用的对象，特别是当引用被用作初始值时，真正参与初始化的其实是引用对象的值。
	此时编译器以引用对象的类型作为auto的类型。
*/
auto a = r;
std::cout << typeid(a).name(); // int
```

​		希望推导得到一个引用，需要附加指出。

```c++
/*
	设置一个类型为auto的引用时，初始值中的顶层常量属性仍然保留到引用中，
	如果我们给初始值绑定一个引用，则此时的常量就不是顶层常量了，而是底层常量，所以引用中保留的是底层const。
		（第二句解释， 就是比如g给个ci绑定了一个引用，这时ci的引用关系上就有了一个来自顶层的引用g，所以ci就不再是独立的顶层常量了，故不违背一般去掉顶层const的说法。引用也没有顶层const。
		也就是通过底层的被访问权限受限推导到顶层的访问权限受限。）
*/

auto& g = ci; //const int& 
std::cout << typeid(g).name() << std::endl;// int

//auto& h = 42; error， 不能为非常量引用绑定字面值
const auto& h = 42;//const int& 
std::cout << typeid(h).name() << std::endl;//int
```

##### const的推断

​		auto会忽略掉顶层const，同时底层const则会保留下来。

​		因为`auto`推断的本质是使用值来初始化，值本身没有变量的性质，所以顶层`const`。但底层`const`作用于语言内部机制（指针、引用的间接寻址），所以底层`const`会被语言机制保留。

```c++
/*
	auto一般会忽略掉顶层const，同时底层const则会保留下来
*/
auto b = ci;
std::cout << typeid(b).name()<<std::endl;//int, ci的顶层const特性被忽略掉了
			// 对于ci来说，是对i进行了拷贝，并对自身的内存限制了被访问权限
		// 以auto推断也是一样，既然创建了新的内存空间，内存空间的访问权限就使用默认权限而不沿用推断

auto c = cr;
std::cout << typeid(c).name() << std::endl;//int， 兼具上一条和 引用的推断

auto d = &i;
std::cout << typeid(d).name() << std::endl;//int *， 对i取地址得到一个整数的指针

auto e = &ci;
std::cout << typeid(e).name() << std::endl;//const int *，ci的内存地址是受限访问的，推导出一个对该内存的间接访问，当然这种访问方式的底层访问权限也必须是受限的。
```

​		如果希望推断得到一个顶层const，需要附加指出

```c++
const auto f = ci;
std::cout << typeid(f).name() << std::endl; // const int

const auto f = &ci;	// const int * const
std::cout << typeid(f).name() << std::endl; // 这个破玩意儿也会把顶层const去掉。。。int const *
```

##### 一条语句中定义多个变量的推导

```c++
/*
	在同一条语句中定义多个变量，切记，符号&和*值从属于某个声明符，
    而非基本数据类型的一部分，因此初始值必须是同一种类型
 */
auto k = ci, & l = i;
auto& m = ci, * p = &ci;
auto& n = i /*, * p2 = &ci*/; //ci是const int
const auto* p2 = &ci;
```



#### 自动类型推导的作用

-   简化代码

    在使用模板的场景下，类型显得臃肿，但并不为人所关心。

    ![image-20200922235242386](https://gitee.com/masstsing/picgo-picserver/raw/master/20200922235242.png)

-   当改变初始化表达式返回值的数据类型时，不必修改变量类型

    ```c++
    auto val = 10 + 23;
    auto val = 10 + 23.0;
    ```

-    模板场景下的返回值类型后置

     https://zh.cppreference.com/w/cpp/keyword/decltype

     ```c++
     #include <iostream>
     
     // 神奇的是，这两个函数居然构成了重载？编译并没报错
     // 直到main中调用时才出现了二义性
     
     template<typename T, typename U>
     auto add(T t, U u) -> decltype(t + u)
     {
         return t + u;
     }
     
     template<typename T, typename U>
     auto add(T t, U u)
     {
         return t + u;
     }
     
     int main(void)
     {
     
         // 模板返回值类型后置
     //  auto f = add(10 , 12.1);
     //  std::cout << typeid(f).name() << " " << f << std::endl;
     
     //  auto ff = add(10 , 12.2);
     //  std::cout << typeid(ff).name() << " " << f << std::endl;
         return 0;
     }
     
     ```



### decltype 类型指示符

​		c++11新标准引入第二种类型说明符`decltype`， 选择并返回操作数的数据类型。

​		编译器分析表达式并得到它的类型，却不实际计算表达式的值。

```c++
decltype(f()) sum = x;
// 编译器不实际调用f()，而是从f()的函数签名中推导返回值类型作为sum的类型。
```

​		`decltype`的结果类型与表达式形式密切相关。

#### decltype(variable)

​		返回该变量的类型，包括顶层const和引用在内

```c++
void page63(const Sales_item si, const Sales_item& ss) 
{
    decltype(si) x("xxxxxxxxx"); // const Sales_item x
	decltype(ss) y{ x }; // const Sales_item &y
    		//ss是个引用，decltype(ss)返回的结果就是引用类型，因此作为引用的y必须初始化；
	// 引用从来都作为其所指对象的同义词出现，只有用在decltype处是一个例外。（ss得到一个引用类型)
}
```

#### decltype(expression)

- 如果`decltype`使用的是表达式而不是一个变量，则`decltype`返回表达式结果对应的类型。
- 有些表达式将向`decltype`返回一个引用类型。一般意味着该表达式的结果对象能作为一条赋值语句的左值。
- 如果表达式的内容是解引用操作或一个值，则`decltype`将得到引用类型。
- 解引用指针可以得到指针所指的对象，而且还能给这个对象赋值，因此`decltype（*z）`的类型是引用类型

```c++
void page63(const Sales_item si, const Sales_item& ss) 
{
    decltype(si) x("xxxxxxxxx");
	decltype(ss) y{ x };
    
    const decltype(ss + si) *z;//const Sales_item *z
    //ss + si使整体结果是一个具体的对象而非一个引用,而且得到的新的z也不再是const，因为相加之后，已经得到了一个具有新值的对象，而非原来某个对象的常值
    
    decltype(*z) a{ y }; // /const Sales_item &a
    //x也可以，变量不影响给常量赋初值
    //*z得到一个指针的值，即其指向的对象的地址，所以得到引用
}
```

> `decltype((variable)) `的结果永远是引用，而`decltype(varaible)`结果只有当variable本身就是一个引用时才是引用。

```c++
void page63(const Sales_item si, const Sales_item& ss) 
{
    decltype(*z) a{ y };//const Sales_item &a
    
/*
	结论：
       不加括号得到的就是该变量的类型，加上括号，相当于进行了运算.
       而且因为其是变量，变量是一种可以作为赋值语句左值的特殊表达式，所以这样的decltype就会得到引用类型
*/
    decltype((si)) b{ a };//const Sales_item &b
    decltype(si) c;// const Sales_item c , 如果是内置类型，未初始化，const要求初始化，这里默认初始化。
    
    int i = 0;
    decltype(i) d; // int d
    decltype((i)) e{ d }; //int &e, 这里加const为啥会失效？？？
}
```

​	

#### decltype((variable))

​			如果变量名加上了一对括号，则`decltype()`中被视为一条求值表达式而不是变量。

```c++
void page63() 
{
	//解释上文  变量作为赋值语句参与表达式得到引用
	int in1 = 3, in2 = 4;
	decltype(in1) in3 = in1;
	decltype(in1 = in2) in4 = in1;// in1 = in2 表达式生存期结束，产生匿名对象引用该表达式结果的地址，从而产生引用类型
}
```

