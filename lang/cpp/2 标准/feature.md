### C++ Feature Evolution

​		Here, I list the language features and standard-library components that have been added to C++ for the C++11, C++14, C++17, and C++20 standards.

#### 19.2.1 C++11 Language Features

Looking at a list of language features can be quite bewildering. Remember that a language feature is not meant to be used in isolation. In particular, most features that are new in C++11 make no sense in isolation from the framework provided by older features.

[1] Uniform and general initialization using **{}**-lists (§[1.4.2](ch01.xhtml#sec1_4_2), §[5.2.3](ch05.xhtml#sec5_2_3))

[2] Type deduction from initializer: **auto** (§[1.4.2](ch01.xhtml#sec1_4_2))

[3] Prevention of narrowing (§[1.4.2](ch01.xhtml#sec1_4_2))

[4] Generalized and guaranteed constant expressions: **constexpr** (§[1.6](ch01.xhtml#sec1_6))

[5] Range-**for**-statement (§[1.7](ch01.xhtml#sec1_7))

[6] Null pointer keyword: **nullptr** (§[1.7.1](ch01.xhtml#sec1_7_1))

[7] Scoped and strongly typed **enums**: **enum class** (§[2.4](ch02.xhtml#sec2_4))

[8] Compile-time assertions: **static_assert** (§[4.5.2](ch04.xhtml#sec4_5_2))

[9] Language mapping of **{}**-list to **std::initializer_list** (§[5.2.3](ch05.xhtml#sec5_2_3))

[10] Rvalue references, enabling move semantics (§[6.2.2](ch06.xhtml#sec6_2_2))

[11] Lambdas (§[7.3.3](ch07.xhtml#sec7_3_3))

[12] Variadic templates (§[7.4.1](ch07.xhtml#sec7_4_1))

[13] Type and template aliases (§[7.4.2](ch07.xhtml#sec7_4_2))

[14] Unicode characters

[15] **long long** integer type

[16] Alignment controls: **alignas** and **alignof**

[17] The ability to use the type of an expression as a type in a declaration: **decltype**

[18] Raw string literals (§[10.4](ch10.xhtml#sec10_4))

[19] Suffix return type syntax (§[3.4.4](ch03.xhtml#sec3_4_4))

[20] A syntax for attributes and two standard attributes: **[[carries_dependency]]** and **[[noreturn]]**

[21] A way of preventing exception propagation: the **noexcept** specifier (§[4.4](ch04.xhtml#sec4_4))

[22] Testing for the possibility of a **throw** in an expression: the **noexcept** operator

[23] C99 features: extended integral types (i.e., rules for optional longer integer types); concatenation of narrow/wide strings; **__STDC_HOSTED__**; **_Pragma(X)**; vararg macros and empty macro arguments

[24] **__func__** as the name of a string holding the name of the current function

[25] **inline** namespaces

[26] Delegating constructors

[27] In-class member initializers (§[6.1.3](ch06.xhtml#sec6_1_3))

[28] Control of defaults: **default** and **delete**(§[6.1.1](ch06.xhtml#sec6_1_1))

[29] Explicit conversion operators

[30] User-defined literals (§[6.6](ch06.xhtml#sec6_6))

[31] More explicit control of **template**instantiation: **extern template**s

[32] Default template arguments for function templates

[33] Inheriting constructors (§[12.2.2](ch12.xhtml#sec12_2_2))

[34] Override controls: **override** (§[5.5](ch05.xhtml#sec5_5)) and **final**

[35] A simpler and more general SFINAE (Substitution Failure Is Not An Error) rule

[36] Memory model (§[18.1](ch18.xhtml#sec18_1))

[37] Thread-local storage: **thread_local**

For a more complete description of the changes to C++98 in C++11, see [[Stroustrup,2013](ch19.xhtml#bib51)].

#### 19.2.2 C++14 Language Features

[1] Function return-type deduction; (§[3.4.3](ch03.xhtml#sec3_4_3))

[2] Improved **constexpr** functions, e.g., **for**-loops allowed (§[1.6](ch01.xhtml#sec1_6))

[3] Variable templates (§[7.4.1](ch07.xhtml#sec7_4_1))

[4] Binary literals (§[1.4](ch01.xhtml#sec1_4))

[5] Digit separators (§[1.4](ch01.xhtml#sec1_4))

[6] Generic lambdas (§[7.3.3.1](ch07.xhtml#sec7_3_3_1))

[7] More general lambda capture

[8] **[[deprecated]]** attribute

[9] A few more minor extensions

#### 19.2.3 C++17 Language Features

[1] Guaranteed copy elision (§[6.2.2](ch06.xhtml#sec6_2_2))

[2] Dynamic allocation of over-aligned types

[3] Stricter order of evaluation (§[1.4.1](ch01.xhtml#sec1_4_1))

[4] UTF-8 literals (**u8**)

[5] Hexadecimal floating-point literals (§[11.6.1](ch11.xhtml#sec11_6_1))

[6] Fold expressions (§[8.4.1](ch08.xhtml#sec8_4_1))

[7] Generic value template arguments (**auto**template parameters; §[8.2.5](ch08.xhtml#sec8_2_5))

[8] Class template argument type deduction (§[7.2.3](ch07.xhtml#sec7_2_3))

[9] Compile-time **if** (§[7.4.3](ch07.xhtml#sec7_4_3))

[10] Selection statements with initializers (§[1.8](ch01.xhtml#sec1_8))

[11] **constexpr** lambdas

[12] **inline** variables

[13] Structured bindings (§[3.4.5](ch03.xhtml#sec3_4_5))

[14] New standard attributes: **[[fallthrough]]**, **[[nodiscard]]**, and **[[maybe_unused]]**

[15] **std::byte** type (§[16.7](ch16.xhtml#sec16_7))

[16] Initialization of an **enum** by a value of its underlying type (§[2.4](ch02.xhtml#sec2_4))

[17] A few more minor extensions

#### 19.2.4 C++20 Language Features

[1] Modules (§[3.2.2](ch03.xhtml#sec3_2_2))

[2] Concepts (§[8.2](ch08.xhtml#sec8_2))

[3] Coroutines (§[18.6](ch18.xhtml#sec18_6))

[4] Designated initializers (a slightly restricted version of a C99 feature)

[5] **<=>** (the “spaceship operator”) a three-way comparison (§[6.5.1](ch06.xhtml#sec6_5_1))

[6] **[*****this]** to capture a current object by value (§[7.3.3](ch07.xhtml#sec7_3_3))

[7] Standard attributes **[[no_unique_address]]**, **[[likely]]**, and **[[unlikely]]**

[8] More facilities allowed in **constexpr** functions, including **new**, **union**, **try**-**catch, dynamic_cast**, and **typeid.**

[9] **consteval** functions guaranteeing compile-time evaluation (§[1.6](ch01.xhtml#sec1_6))

[10] **constinit** variables to guarantee static (not run-time) initialization (§[1.6](ch01.xhtml#sec1_6))

[11] **using** scoped **enum**s (§[2.4](ch02.xhtml#sec2_4))

[12] A few more minor extensions

#### 19.2.5 C++11 Standard-Library Components

The C++11 additions to the standard library come in two forms: new components (such as the regular expression matching library) and improvements to C++98 components (such as move constructors for containers).

[1] **initializer_list** constructors for containers (§[5.2.3](ch05.xhtml#sec5_2_3))

[2] Move semantics for containers (§[6.2.2](ch06.xhtml#sec6_2_2), §[13.2](ch13.xhtml#sec13_2))

[3] A singly-linked list: **forward_list** (§[12.3](ch12.xhtml#sec12_3))

[4] Hash containers: **unordered_map**, **unordered_multimap**, **unordered_set**, and **unordered_multiset** (§[12.6](ch12.xhtml#sec12_6), §[12.8](ch12.xhtml#sec12_8))

[5] Resource management pointers: **unique_ptr**, **shared_ptr**, and **weak_ptr** (§[15.2.1](ch15.xhtml#sec15_2_1))

[6] Concurrency support: **thread** (§[18.2](ch18.xhtml#sec18_2)), mutexes and locks (§[18.3](ch18.xhtml#sec18_3)), and condition variables (§[18.4](ch18.xhtml#sec18_4))

[7] Higher-level concurrency support: **packaged_thread**, **future**, **promise**, and **async()** (§[18.5](ch18.xhtml#sec18_5))

[8] **tuple**s (§[15.3.4](ch15.xhtml#sec15_3_4))

[9] Regular expressions: **regex** (§[10.4](ch10.xhtml#sec10_4))

[10] Random numbers: distributions and engines (§[17.5](ch17.xhtml#sec17_5))

[11] Integer type names, such as **int16_t**, **uint32_t**, and **int_fast64_t** (§[17.8](ch17.xhtml#sec17_8))

[12] A fixed-sized contiguous sequence container: **array** (§[15.3](ch15.xhtml#sec15_3))

[13] Copying and rethrowing exceptions (§[18.5.1](ch18.xhtml#sec18_5_1))

[14] Error reporting using error codes: **system_error**

[15] **emplace()** operations for containers (§[12.8](ch12.xhtml#sec12_8))

[16] Wide use of **constexpr** functions

[17] Systematic use of **noexcept** functions

[18] Improved function adaptors: **function** and **bind()** (§[16.3](ch16.xhtml#sec16_3))

[19] **string** to numeric value conversions

[20] Scoped allocators

[21] Type traits, such as **is_integral** and **is_base_of**(§[16.4.1](ch16.xhtml#sec16_4_1))

[22] Time utilities: **duration** and **time_point**(§[16.2.1](ch16.xhtml#sec16_2_1))

[23] Compile-time rational arithmetic: **ratio**

[24] Abandoning a process: **quick_exit** (§[16.8](ch16.xhtml#sec16_8))

[25] More algorithms, such as **move()**, **copy_if()**, and **is_sorted()** ([Chapter 13](ch13.xhtml#ch13))

[26] Garbage collection API; later deprecated (§[19.2.9](ch19.xhtml#sec19_2_9))

[27] Low-level concurrency support: **atomic**s (§[18.3.2](ch18.xhtml#sec18_3_2))

[28] A few more minor extensions

#### 19.2.6 C++14 Standard-Library Components

[1] **shared_mutex** and **shared_lock** (§[18.3](ch18.xhtml#sec18_3))

[2] User-defined literals (§[6.6](ch06.xhtml#sec6_6))

[3] Tuple addressing by type (§[15.3.4](ch15.xhtml#sec15_3_4))

[4] Associative container heterogenous lookup

[5] A few more minor extensions

#### 19.2.7 C++17 Standard-Library Components

[1] File system (§[11.9](ch11.xhtml#sec11_9))

[2] Parallel algorithms (§[13.6](ch13.xhtml#sec13_6), §[17.3.1](ch17.xhtml#sec17_3_1))

[3] Mathematical special functions (§[17.2](ch17.xhtml#sec17_2))

[4] **string_view** (§[10.3](ch10.xhtml#sec10_3))

[5] **any** (§[15.4.3](ch15.xhtml#sec15_4_3))

[6] **variant** (§[15.4.1](ch15.xhtml#sec15_4_1))

[7] **optional** (§[15.4.2](ch15.xhtml#sec15_4_2))

[8] A way of invoking anything that can be called for a given set of arguments: **invoke()**

[9] Elementary string conversions: **to_chars()** and **from_chars()**

[10] Polymorphic allocator (§[12.7](ch12.xhtml#sec12_7))

[11] **scoped_lock** (§[18.3](ch18.xhtml#sec18_3))

[12] A few more minor extensions

#### 19.2.8 C++20 Standard-Library Components

[1] Ranges, views, and pipelines (§[14.1](ch14.xhtml#sec14_1))

[2] **printf()**-style formatting: **format()** and **vformat()** (§[11.6.2](ch11.xhtml#sec11_6_2))

[3] Calendars (§[16.2.2](ch16.xhtml#sec16_2_2)) and time-zones (§[16.2.3](ch16.xhtml#sec16_2_3))

[4] **span** for read and write access to contiguous arrays (§[15.2.2](ch15.xhtml#sec15_2_2))

[5] **source_location** (§[16.5](ch16.xhtml#sec16_5))

[6] Mathematical constants, e.g., **pi** and **ln10e**(§[17.9](ch17.xhtml#sec17_9))

[7] Many extensions to **atomic**s (§[18.3.2](ch18.xhtml#sec18_3_2))

[8] Ways of waiting for a numbet of **thread**s: **barrier** and **latch**.

[9] Feature test macros

[10] **bit_cast<>** (§[16.7](ch16.xhtml#sec16_7))

[11] Bit operations (§[16.7](ch16.xhtml#sec16_7))

[12] More standard-library functions made **constexpr**

[13] Many uses of **<=>** in the standard library

[14] Many more minor extensions

#### 19.2.9 Removed and Deprecated Features

There are billions of lines of C++ “out there” and nobody knows exactly what features are in critical use. Consequently, the ISO committee removes older features only reluctantly and after years of warning. However, sometimes troublesome features are removed or *deprecated*.

By deprecating a feature, the standards committee expresses the wish that the feature will go away. However, the committee does not have the power to immediately remove a heavily used feature – however redundant or dangerous it may be. Thus, a deprecation is a strong hint to avoid the feature. It may disappear in the future. The list of deprecated features is in Appendix D of the standard [[C++,2020](ch19.xhtml#bib10)]. Compilers are likely to issue warnings for uses of deprecated features. However, deprecated features are part of the standard and history shows that they tend to remain supported “forever” for reasons of compatibility. Even features finally removed tend to live on in implementations because of user pressure on implementers.

- Removed: Exception specifications: **void f() throw(X,Y); // C++98; now an error**
- Removed: The support facilities for exception specifications, **unexpected_handler**, **set_unexpected()**, **get_unexpected()**, and **unexpected()**. Instead, use **noexcept** (§[4.2](ch04.xhtml#sec4_2)).
- Removed: Trigraphs.
- Removed: **auto_ptr**. Instead, use **unique_ptr**(§[15.2.1](ch15.xhtml#sec15_2_1)).
- Removed: The use of the storage specifier **register**.
- Removed: The use of **++** on a **bool**.
- Removed: The C++98 **export** feature. It was complex and not shipped by the major vendors. Instead, **export** is used as a keyword for modules (§[3.2.2](ch03.xhtml#sec3_2_2)).
- Deprecated: Generation of copy operations for a class with a destructor (§[6.2.1](ch06.xhtml#sec6_2_1)).
- Removed: Assignment of a string literal to a **char***. Instead use **const c har*** or **auto**.
- Removed: Some C++ standard-library function objects and associated functions. Most relate to argument binding. Instead use lambdas and **function** (§[16.3](ch16.xhtml#sec16_3)).
- Deprecated: Comparisons of **enum** values with values from a different **enum** or a floating point value.
- Deprecated: Comparisons between two arrays.
- Deprecated: Comma operations in a subscript (e.g., **[a,b]**). To make room for allowing user defined **operator[]()** with multiple arguments.
- Deprecated: Implicit capture of ***this** in lambda expressions. Instead, use **[=,this]** (§[7.3.3](ch07.xhtml#sec7_3_3)).
- Removed: The standard-library interface for garbage collectors. The C++ garbage collectors don’t use that interface.
- Deprecated: **strstream**; instead, use **spanstream** (§[11.7.4](ch11.xhtml#sec11_7_4)).