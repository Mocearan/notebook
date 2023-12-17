# cctype

​		改变某个字符的特性。

---

​		This header was originally in the C standard library as [``](https://en.cppreference.com/w/c/string/byte).

​		This header is part of the [null-terminated byte strings](https://en.cppreference.com/w/cpp/string/byte) library.

```c++
namespace std {
  int isalnum(int c);
  int isalpha(int c);
  int isblank(int c);
  int iscntrl(int c);
  int isdigit(int c);
  int isgraph(int c);
  int islower(int c);
  int isprint(int c);
  int ispunct(int c);
  int isspace(int c);
  int isupper(int c);
  int isxdigit(int c);
  int tolower(int c);
  int toupper(int c);
}
```



| [isalnum](https://en.cppreference.com/w/cpp/string/byte/isalnum) | checks if a character is alphanumeric (function)            |
| ------------------------------------------------------------ | ----------------------------------------------------------- |
| [isalpha](https://en.cppreference.com/w/cpp/string/byte/isalpha) | checks if a character is alphabetic (function)              |
| [islower](https://en.cppreference.com/w/cpp/string/byte/islower) | checks if a character is lowercase (function)               |
| [isupper](https://en.cppreference.com/w/cpp/string/byte/isupper) | checks if a character is an uppercase character (function)  |
| [isdigit](https://en.cppreference.com/w/cpp/string/byte/isdigit) | checks if a character is a digit (function)                 |
| [isxdigit](https://en.cppreference.com/w/cpp/string/byte/isxdigit) | checks if a character is a hexadecimal character (function) |
| [iscntrl](https://en.cppreference.com/w/cpp/string/byte/iscntrl) | checks if a character is a control character (function)     |
| [isgraph](https://en.cppreference.com/w/cpp/string/byte/isgraph) | checks if a character is a graphical character (function)   |
| [isspace](https://en.cppreference.com/w/cpp/string/byte/isspace) | checks if a character is a space character (function)       |
| [isblank](https://en.cppreference.com/w/cpp/string/byte/isblank)(C++11) | checks if a character is a blank character (function)       |
| [isprint](https://en.cppreference.com/w/cpp/string/byte/isprint) | checks if a character is a printing character (function)    |
| [ispunct](https://en.cppreference.com/w/cpp/string/byte/ispunct) | checks if a character is a punctuation character (function) |
| [tolower](https://en.cppreference.com/w/cpp/string/byte/tolower) | converts a character to lowercase (function)                |
| [toupper](https://en.cppreference.com/w/cpp/string/byte/toupper) | converts a character to uppercase (function)                |