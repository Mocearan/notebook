# constant

​		a **constant** is a fixed value that may not be changed. 

​		C++ has two kinds of constants: literal constants, and symbolic constants.

## literal constant

​		A **literal** (also known as a **literal constant**) is a fixed value that has been inserted directly into the source code.

> Literals and variables both have a value (and a type). However, the value of a literal is fixed and can’t be changed (hence it being called a constant), whereas the value of a variable can be changed through initialization and assignment.

​		**Literal constants** (usually just called **literals**) are unnamed values inserted directly into the code.

​		They are constants because their values can not be changed dynamically (you have to change them, and then recompile for the change to take effect).

### letaral type

​		The type of a literal is assumed from the value and format of the literal itself.

![image-20220427164338625](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220427164338625.png)

​		If the default type of a literal is not as desired, you can change the type of a literal by adding a suffix:

![image-20220427164436405](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220427164436405.png)

```c++
std::cout << 5; // 5 (no suffix) is type int (by default)
std::cout << 5u; // 5u is type unsigned int
std::cout << 5L; // 5L is type long

std::cout << 5.0; // 5.0 (no suffix) is type double (by default)
std::cout << 5.0f; // 5.0f is type float
```



### string literals

​		#### C-style Null-terminated strings

​		a string as a collection of sequential characters. 

​		The sequence of character constants inside double quotes represents a string constant.

`operator""`

```cpp
"Hello, world!"; // "Hello, world!" is a C-style string literal
```

- C++ will concatenate sequential string literals

  ```C++
  std::cout << "Hello," " world!";
  ```



> C++ also has literals for std::string and std::string_view. In most cases these won’t be needed, but they may occasionally come in handy when using type deduction, either via the `auto` keyword, or class template argument deduction.

#### C++-style strings

`operator“”s`

#### C++-style string view

`“operator”sv`

```c++
#include <iostream>
#include <string>      // for std::string
#include <string_view> // for std::string_view

int main()
{
 using namespace std::literals; // easiest way to access the s and sv suffixes

 std::cout << "foo\n";   // no suffix is a C-style string literal
 std::cout << "goo\n"s;  // s suffix is a std::string literal
 std::cout << "moo\n"sv; // sv suffix is a std::string_view literal

 return 0;
};
```





### integer literals

​		整数字面量拥有以下形式:

- *decimal-literal*	 *integer-suffix*(optional)(1)

  ​	是一个非零十进制位（`1`、`2`、`3`、`4`、`5`、`6`、`7`、`8`、`9`）后随零或多个十进制位（`0`、`1`、`2`、`3`、`4`、`5`、`6`、`7`、`8`、`9`）

- *octal-literal*         *integer-suffix*(optional)(2)

  ​	是数位零（`0`）后随零或多个八进制位（`0`、`1`、`2`、`3`、`4`、`5`、`6`、`7`）

- *hex-literal*          *integer-suffix*(optional)(3)

  ​	是字符序列 `0x` 或字符序列 `0X` 后随一或多个十六进制数位（`0`、`1`、`2`、`3`、`4`、`5`、`6`、`7`、`8`、`9`、`a`、`A`、`b`、`B`、`c`、`C`、`d`、`D`、`e`、`E`、`f`、`F`）

- *binary-literal*     *integer-suffix*(optional)(4)    (since C++14)

  ​	 是字符序列 `0b` 或字符序列 `0B` 后随一或多个二进制位（`0`、`1`）

  > hexadecimal values useful as a concise way to represent a value in memory. For this reason, hexadecimal values are often used to represent memory addresses or raw values in memory.

Optional single quotes (`'`) may be inserted between the digits as a separator. They are ignored by the compiler.

```c++
unsigned long long l1 = 18446744073709550592ull; // C++11
unsigned long long l2 = 18'446'744'073'709'550'592llu; // C++14
unsigned long long l3 = 1844'6744'0737'0955'0592uLL; // C++14
unsigned long long l4 = 184467'440737'0'95505'92LLU; // C++14
int bin { 0b1011'0010 };  // assign binary 1011 0010 to the variable, c++14
// Also note that the separator can not occur before the first digit of the value:
int bin { 0b'1011'0010 };  // error: ' used before first digit of value
```

#### *integer-suffix*

 if provided, may contain one or both of the following (if both are provided, they may appear in any order:

- unsigned-suffix(`u` /``U``)
- one of
  - long-suffix(`l/L`)
  - lonog-long-suffix(`ll/LL`) （c++11)
  - size-suffix(`z/Z`)  (c++23)

#### 整型选择

整数字面量的类型，是依赖于所用数字基底和 *整数后缀* 的列表中，首个能适合其值的类型。

![image-20220427173527304](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220427173527304.png)

```c++
#include <cstddef>
#include <iostream>
#include <type_traits>
 
int main()
{
std::cout << 123    << '\n'
          << 0123   << '\n'
          << 0x123  << '\n'
          << 0b10   << '\n'
          << 12345678901234567890ull << '\n'
          << 12345678901234567890u   << '\n'; // 类型是 unsigned long long
                                              // 即使无 long long 后缀
 
//   std::cout << -9223372036854775808 << '\n'; // 错误：值 9223372036854775808
                // 不吻合 signed long long，这是无后缀整数字面量允许的最大类型
     std::cout << -9223372036854775808u << '\n'; // 应用于无符号值的一元减
                // 从 2^64 减去该值，给出 9223372036854775808
     std::cout << -9223372036854775807 - 1 << '\n'; // 计算值 -9223372036854775808
                                                    // 的正确方式
 
#if __cpp_size_t_suffix >= 202011L // C++23
     static_assert(std::is_same_v<decltype(0UZ), std::size_t>);
     static_assert(std::is_same_v<decltype(0Z), std::make_signed_t<std::size_t>>);
#endif
}
```



#### printng decimal / octal / hexadecimal / binary number

 Printing in decimal, octal, or hex is easy via use of std::dec, std::oct, and std::hex:

```c++
#include <iostream>

int main()
{
    int x { 12 };
    std::cout << x << '\n'; // decimal (by default)
    std::cout << std::hex << x << '\n'; // hexadecimal
    std::cout << x << '\n'; // now hexadecimal
    std::cout << std::oct << x << '\n'; // octal
    std::cout << std::dec << x << '\n'; // return to decimal
    std::cout << x << '\n'; // decimal

    return 0;
}
```

To use std::bitset, we can define a std::bitset variable and tell std::bitset how many bits we want to store. The number of bits must be a compile time constant. std::bitset can be initialized with an unsigned integral value (in any format, including decimal, octal, hex, or binary).

```c++
#include <bitset> // for std::bitset
#include <iostream>

int main()
{
	// std::bitset<8> means we want to store 8 bits
	std::bitset<8> bin1{ 0b1100'0101 }; // binary literal for binary 1100 0101
	std::bitset<8> bin2{ 0xC5 }; // hexadecimal literal for binary 1100 0101

	std::cout << bin1 << ' ' << bin2 << '\n';
	std::cout << std::bitset<4>{ 0b1010 } << '\n'; // create a temporary std::bitset and print it

	return 0;
}
```



#### converting

- **Converting binary to decimal** (uint)

  ​	We multiply each binary digit by its digit value (determined by its position). Summing up all these values gives us the total: Consider the 8 bit (1 byte) binary number 0101 1110. Binary 0101 1110 means (0 * 128) + (1 * 64) + (0 * 32) + (1 * 16) + (1 * 8) + (1 * 4) + (1 * 2) + (0 * 1). If we sum up all of these parts, we get the decimal number 64 + 16 + 8 + 4 + 2 = 94.

- **converting decimal to binary**(uint)

  ​	The first method involves continually dividing by 2, and writing down the remainders. The binary number is constructed at the end from the remainders, from the bottom up.

  ​	The second method involves working backwards to figure out what each of the bits must be. This method can be easier with small binary numbers.

  > 148 = (1 * 128) + (0 * 64) + (0 * 32) + (1 * 16) + (0 * 8) + (1 * 4) + (0 * 2) + (0 * 1) = 1001 0100

  

Signed integers are typically stored using a method known as **two’s complement**.

> 0 would have two representations: 0000 0000 (positive zero) and 1111 1111 (negative zero). By adding 1, 1111 1111 intentionally overflows and becomes 0000 0000.        

In two’s complement, the leftmost (most significant) bit is used as the sign bit. A 0 sign bit means the number is positive, and a 1 sign bit means the number is negative.     

- **Converting binary to decimal** (int)

  ​	If the sign bit is 0, just convert the number as shown for unsigned numbers above.

  ​	If the sign bit is 1, then we invert the bits, add 1, then convert to decimal, then make that decimal number negative (because the sign bit was originally negative).

- **converting decimal to binary**(int)         

  ​	Positive signed numbers are represented in binary just like positive unsigned numbers (with the sign bit set to 0).

  ​	Negative signed numbers are represented in binary as the bitwise inverse of the positive number, plus 1.

  



### floating point literals

There are two different ways to declare floating-point literals:

- standard notation
- scientific notation

[Floating Point | Thomas Finley (tfinley.net)](https://tfinley.net/csarch-notes/2000/floating)

#### 科学计数法

Numbers in scientific notation take the following form: 

![](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220417120356817.png)

Because it can be hard to type or display exponents in C++, we use the letter ‘e’ (or sometimes ‘E’) to represent the “times 10 to the power of” part of the equation. 

For example, `1.2 x 10⁴` would be written as `1.2e4`

Here’s the most important thing to understand: The digits in the significand (the part before the ‘e’) are called the **significant digits**. The number of significant digits defines a number’s **precision**. The more digits in the significand, the more precise a number is.

##### How to convert numbers to scientific notation

Use the following procedure:

- Your exponent starts at zero.
- Slide the decimal so there is only one non-zero digit to the left of the decimal.
  - Each place you slide the decimal to the left increases the exponent by 1.
  - Each place you slide the decimal to the right decreases the exponent by 1.
- Trim off any leading zeros (on the left end of the significand)
- Trim off any trailing zeros (on the right end of the significand) only if the original number had no decimal point. We’re assuming they’re not significant unless otherwise specified.

![image-20220417120743894](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20220417120743894.png)







## symbolic constant

### symbolic type

- const variables
- constexpr variables
- enum
- macro

C++ actually has two different kinds of constants:

- compile-time constants

  ​	**Runtime constants** are constants whose initialization values can only be resolved at runtime (when your program is running). 

  ​		there are a few odd cases where C++ requires a compile-time constant instead of a run-time constant .

- Runtime constants

  ​	**Compile-time constants** are constants whose initialization values can be determined at compile-time (when your program is compiling).

  ​	Compile-time constants enable the compiler to perform optimizations that aren’t available with runtime constants.

### const variables

​		Any variable that should not be modifiable after initialization and whose initializer is not known at compile-time should be declared as const.

#### compile-time constants

​		simply put the `const` keyword either before or after the variable type, like so:

```c++
const double gravity { 9.8 };  // preferred use of const before type
int const sidesInSquare { 4 }; // "east const" style, okay, but not preferred
const int something { 1 + 2 }; // the compiler can resolve this at compiler time
```

​		Const variables *must* be initialized when you define them, and then that value can not be changed via assignment:

```c++
const double gravity; // error: const variables must be initialized
gravity = 9.9; // error: const variables can not be changed
```

#### Runtime constants

​		Any variable that should not be modifiable after initialization and whose initializer is known at compile-time should be declared as constexpr.

​		const variables can be initialized from other variables (including non-const ones):

```c++
const int usersAge { age };

void printInt(const int x) // x is a runtime constant because the value isn't known until the program is run
{
    std::cout << x;
}
```



### constexpr

​		ensures that a constant must be a compile-time constant.

```c++
constexpr double gravity { 9.8 }; // ok, the value of 9.8 can be resolved at compile-time
constexpr int sum { 4 + 5 }; // ok, the value of 4 + 5 can be resolved at compile-time


int age{};
std::cin >> age;
constexpr int myAge { age }; // compile error: age is a runtime constant, not a compile-time constant
```

​		Note that literals are also implicitly constexpr, as the value of a literal is known at compile-time.

​		Constexpr variables can also be used in constant expressions：

```c++
constexpr int x { 3 };
constexpr int y { 4 };
std::cout << x + y; // x + y evaluated at compile-time
//Similar to the literal case, the compiler can substitute in the value 7.
```



### macro

​		Avoid using #define to create symbolic constants macros. Use const or constexpr variables instead.

- because these #defined values aren’t variables, you can’t add a watch in the debugger to see their values. 
-  macros can have naming conflicts with normal code
-  macros don’t follow normal scoping rules



## Const function parameters and return values

```cpp
void printInt(const int x)
{
    std::cout << x;
}
```

​		Making a function parameter const enlists the compiler’s help to ensure that the parameter’s value is not changed inside the function. Note that we did not provide an explicit initializer for our const parameter -- the value of the argument in the function call will be used as the initializer in this case.

​		Function parameters for arguments passed by value should not be made const.

> we generally don’t care if the function changes the value of the parameter (since it’s just a copy that will be destroyed at the end of the function anyway)

​		Don’t use const with return by value.

## const variable name

- Some programmers prefer to use all upper-case names for const variables. 

- Others use normal variable names with a ‘k’ prefix. 

- However, we will use normal variable naming conventions, which is more common. 

  > ​		Const variables act exactly like normal variables in every case except that they can not be assigned to, so there’s no particular reason they need to be denoted as special.



## symbolic constants in multi-file program

​		In many applications, a given symbolic constant needs to be used throughout your code (not just in one location). These can include physics or mathematical constants that don’t change (e.g. pi or Avogadro’s number), or application-specific “tuning” values (e.g. friction or gravity coefficients). Instead of redefining these every time they are needed, it’s better to declare them once in a central location and use them wherever needed. That way, if you ever need to change them, you only need to change them in one place.



## magic numbers

​		**Avoid magic numbers, use symbolic constants instead**

​		A **magic number** is a literal (usually a number) that either has an unclear meaning or is used multiple times.

​		Using magic numbers is generally considered bad practice because, in addition to not providing context as to what they are being used for, they pose problems if the value needs to change.

​		Magic numbers aren’t always numbers -- they can also be strings or other types.

```c++
const int maxStudentsPerSchool{ numClassrooms * 30 };

const int maxStudentsPerClass { 30 }; // now obvious what 30 is
const int maxStudentsPerSchool{ numClassrooms * maxStudentsPerClass };
```



​		Note that literals used only once, and in obvious contexts, are not considered “magic”. The values -1, 0, 0.0, and 1 are often used in contexts that are obvious:

> ```cpp
> int idGenerator { 0 };         // fine: we're starting our id generator with value 0
> idGenerator = idGenerator + 1; // fine: we're just incrementing our generator
> int kmtoM(int km) { return km * 1000; } // fine: it's obvious 1000 is a conversion factor
> ```