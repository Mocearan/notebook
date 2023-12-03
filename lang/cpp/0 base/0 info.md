# info

---



The C++ language can be summarized as a set of mutually supportive facilities:

- A static type system with equal support for built-in types and user-defined types ([Chapter 1](ch01.xhtml#ch01), [Chapter 5](ch05.xhtml#ch05), [Chapter 6](ch06.xhtml#ch06))
- Value and reference semantics (§[1.7](ch01.xhtml#sec1_7), §[5.2](ch05.xhtml#sec5_2), §[6.2](ch06.xhtml#sec6_2), [Chapter 12](ch12.xhtml#ch12), §[15.2](ch15.xhtml#sec15_2))
- Systematic and general resource management (RAII) (§[6.3](ch06.xhtml#sec6_3))
- Support for efficient object-oriented programming (§[5.3](ch05.xhtml#sec5_3), class.virtual, §[5.5](ch05.xhtml#sec5_5))
- Support for flexible and efficient generic programming ([Chapter 7](ch07.xhtml#ch07), [Chapter 18](ch18.xhtml#ch18))
- Support for compile-time programming (§[1.6](ch01.xhtml#sec1_6), [Chapter 7](ch07.xhtml#ch07), [Chapter 8](ch08.xhtml#ch08))
- Direct use of machine and operating system resources (§[1.4](ch01.xhtml#sec1_4), [Chapter 18](ch18.xhtml#ch18))
- Concurrency support through libraries (often implemented using intrinsics) ([Chapter 18](ch18.xhtml#ch18))

The standard-library components add further essential support for these high-level aims.



​		使用const(第1.6节)、constexpr(第1.6节)、enum或enum类(第2.4节)来定义manifest常量，使用constexpr(第1.6节)、consteval(第1.6节)和inline(第5.2.1节)来避免函数调用开销，使用模板(第7章)来指定函数族和类型，使用命名空间(第3.3节)来避免名称冲突。





| **Standard-Library Suffixes for Literals** |                                     |                                           |
| :----------------------------------------- | :---------------------------------- | ----------------------------------------- |
| **<chrono>**                               | **std::literals::chrono_literals**  | **h, min**, **s**, **ms**, **us**, **ns** |
| **<string>**                               | **std::literals::string_literals**  | **s**                                     |
| **<string_view>**                          | **std::literals::string_literals**  | **sv**                                    |
| **<complex>**                              | **std::literals::complex_literals** | **i**, **il**, **if**                     |

