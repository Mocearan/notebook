# C 的历史

---



## 早期的C

- 1969 BCPL 到 B

  作为Unix的系统程序语言替代PDP-7汇编器

  >增加 ``++ ``、`--`、复合赋值
  >
  >保持为类BCPL的无类型语言

- 1971  B到NB

  > 类型（int、char、数组与指针）
  >
  > 数组到指针转换
  >
  > 到机器码的编译

- 1972  更名为C

  > 结构体、运算符 &&及||、预处理器、可移植I/O

- 1973  以C重写

  > 无符号整数、long、联合体、枚举、增强的类型安全

- 1978  The C Programming language

  > 第一版



## 标准C

- 1983  ANSI 建立委员会

- 1988  The C programming Language， 2 

- **1989  C89， ANSI C标准初版**

  > - 代码化的即存实践
  > - 新特性：volatile、enum、有符号整数、void、本地环境
  > - 来自C++：const、函数原型

- 1990  C90，  ANSI C 标准 -》 ISO/IEC 9899-1990

- 1995  C95，   ISO/IEC 9899 AM1

  > - 极大扩充了宽和多字节字符支持（`wctype.h`、`wchar.h`、对流I/O的添加和更改等）
  > - 双标符、iso646.h

- 1995  技术勘误 1 （ISO/IEC 9899 TCOR1）

  > http://www.open-std.org/jtc1/sc22/wg14/www/docs/tc1.htm

- 1996  技术勘误 2 （ISO/IEC  9899 TCOR2）

  > http://www.open-std.org/jtc1/sc22/wg14/www/docs/tc2.htm

- **1999  C99  ， ISO/IEC 9899:1999**

  > - 新特性：
  >
  >   `_Bool`、`long long`、`stdint.h`、`inttypes.h`、`restrict`、复合字面量、变长数组、伸缩数组成员、指定初始化器、`fenv.h`、变参数宏、复数、`__func__`、十六进制浮点格式（``%a``）、`lconv`的货币格式化、`isblank`、窄与宽字符串字面量的连接、枚举的尾逗号、类函数宏的空参数、``STDC_*pragma``、`va_copy`、`tmpnam`的空返回、`setvbuf`中的空指针、printf的hh与ll长度指定符、`snprintf`、`_Exit`、`tgmath.h`、仿`POSIX strftime`说明符
  >
  > - 来自C++：
  >
  >   `inline`、声明与代码混合、for循环的init子句中的声明、//注释、源代码中的通用字符名
  >
  > - 移除隐式函数声明和隐式int

- 2001  / 2004 / 2004 / 2007  / 2008 / 2009 / 2010  技术勘误/ TR

  > 2001：技术勘误 1 （ ISO/IEC 9899:1999/Cor.1:2001(E) ）
  >
  > ​		http://www.open-std.org/jtc1/sc22/wg14/www/docs/9899tc1/
  >
  > 2004：技术勘误 2 （ ISO/IEC 9899:1999/Cor.2:2004(E) ）
  >
  > ​		    Unicode TR（ ISO/IEC TR 19769:2004 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1040.pdf
  >
  > 2007：技术勘误 3 （ ISO/IEC 9899:1999/Cor.3:2007(E) ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
  >
  > ​			弃用gets
  >
  > ​			边界检查接口 TR（ ISO/IEC TR 24731-1:2007 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1225.pdf
  >
  > 2008：嵌入式 TR （ ISO/IEC TR 18037:2008 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1021.pdf
  >
  > 2009：十进制浮点数 TR（ ISO/IEC TR 24732:2009 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1241.pdf
  >
  > ​			特殊数学函数 TR（ ISO/IEC TR 24747:2009 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1182.pdf
  >
  > 2010：动态分配函数 TR（ ISO/IEC TR 24731-2:2010 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1248.pdf

- **2011， C11， ISO/IEC 9899:2011 **

  > http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

  > - 具线程的内存模型、`stdatomic.h`、`threads.h`、泛型函数、`_Alignas/_Alignof`、`_Noreturn`、`_Static_assert`、可分析性扩展、对负数和虚数类型的扩展、匿名结构体与联合体、独占文件打开模式、`quick_exit`
  > - 移除gets
  > - 来自边界检查接口TR： 边界检查接口
  > - 来自Unicode TR：``char16_t``、`char32_t`、`uchar.h`
  
- 2012， 技术勘误 1 ，  ISO/IEC 9899:2011/Cor 1:2012， 修正 DR 411

- 2013 / 2014 / 2015 / 2016  TS

  > 2013：安全代码规则 TS （ ISO/IEC TS 17961:2013 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1624.pdf
  >
  > 2014：FP TS 部分 1 ：二进制浮点算术（ ISO/IEC TS 18661-1:2014 ）
  >
  > ​			http://www.open-std.org/JTC1/sc22/wg14/www/docs/n1778.pdf
  >
  > ​		1.提供对 C11 的更改（主要对附录 F ），以覆盖所有基本要求及一些 IEC 60559:2011 的推荐（ C11 构建于 IEC 60559:1989 ）
  >
  > 2015 ：FP TS 部分 2 ：十进制浮点算术（ ISO/IEC TS 18661-2:2015 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1912.pdf
  >
  > 		1. 提供对 C11 的更改，以支持所有要求，加上一些 IEC 60559:2011 对十进制浮点算术的基本推荐。它替代了 ISO/IEC TR 24732:2009 。
  >
  > 2015：FP TS 部分 3 ：交换及扩展类型（ ISO/IEC TS 18661-3:2015 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1945.pdf
  >
  > ​			对C11的更改，以支持IEC 60559:2011对扩展浮点格式及交换格式的推荐，包括算术和非算术
  >
  > ​			FP TS 部分 4 ：补充的函数（ ISO/IEC TS 18661-4:2015 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1950.pdf
  >
  > ​			对 C11 的更改，以支持所有 IEC 60559:2011 推荐的数学运算，包括 π 单位的三角函数、平方根倒数、复利等。
  >
  > 2016：FP TS 部分 5 ：补充的属性（ ISO/IEC TS 18661-5:2016 ）
  >
  > ​			http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2004.pdf
  >
  > ​			对 C11 的更改，以支持所有 IEC 60559:2011 推荐的补充属性（求值模型、异常处理、可再现性等）

- **2018， C17， ISO/IEC 9899:2018**

> 包含弃用`ATOMIC_VAR_INIT`和对下列缺陷报告的修正。



## 未来发展

- 并行 TS （方案 [n2017](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2017.pdf) 2016-03-10 ）

- 事务性内存 TS （方案 [n1961](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1961.pdf) 2015-09-23 ）

- **`C23` （方案 [n2596](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2596.pdf) 2020-12-11 ）下个主要 C 语言标准版本****

  > 1. 来自 TS （语言）：十进制浮点类型
  > 2. 来自 C++ （语言）：单参数 [`_Static_assert`](https://zh.cppreference.com/w/c/language/_Static_assert) 、[属性](https://zh.cppreference.com/w/c/language/attributes)（ `[[nodiscard]]`、 `[[maybe_unused]]`、 `[[deprecated]]`、 `[[fallthrough]]` ）、 u8 [字符常量](https://zh.cppreference.com/w/c/language/character_constant)、二进制[整数常量](https://zh.cppreference.com/w/c/language/integer_constant)、移除旧式[函数定义](https://zh.cppreference.com/w/c/language/function_definition)、无名函数形参、数组类型及其元素类型的等同 cvr 限定
  > 3. 来自 TS （库）：扩展的二进制浮点算术、十进制浮点算术
  > 4. 来自 POSIX （库）： [`memccpy()`](https://zh.cppreference.com/w/c/string/byte/memccpy) 、 [`strdup()`](https://zh.cppreference.com/w/c/string/byte/strdup) 、 [`strndup()`](https://zh.cppreference.com/w/c/string/byte/strndup) 、 [`asctime_r()`](https://zh.cppreference.com/w/c/chrono/asctime) 、 [`ctime_r()`](https://zh.cppreference.com/w/c/chrono/ctime) 、 [`gmtime_r()`](https://zh.cppreference.com/w/c/chrono/gmtime) 、 [`localtime_r()`](https://zh.cppreference.com/w/c/chrono/localtime) 、 对 [strftime()](https://zh.cppreference.com/w/c/chrono/strftime)/[wcsftime()](https://zh.cppreference.com/w/c/chrono/wcsftime) 的修改
  > 5. 其他库工具： [`timespec_getres()`](https://zh.cppreference.com/w/c/chrono/timespec_getres)
  > 6. 未授予 DR 状态的问题列表： [n2556 (2020-08-02)](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2556.pdf)
  > 7. C11 后缺陷报告列表（包含为 C2x 安排者） [n2396 (2019-04)](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2396.htm)

