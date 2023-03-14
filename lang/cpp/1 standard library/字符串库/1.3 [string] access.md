# access

---



## access character

### operator[]

- `charT& operator[](size_type index);`
- `const charT& operator[](size_type index) const;`

​		如果 pos > size()，那么行为未定义。

```c++
std::string sSource{ "abcdefg" };
std::cout << sSource[5] << '\n';
sSource[5] = 'X';
```



### at

- `charT& at(size_type index);`
- `const charT& at(size_type index) const;`

​		进行边界检查，非法访问时抛出 [std::out_of_range](https://zh.cppreference.com/w/cpp/error/out_of_range) 类型的异常

```c++
std::string sSource{ "abcdefg" };
std::cout << sSource.at(5) << '\n';
sSource.at(5) = 'X';
```



## c-style arrays

- `const charT* c_str() const;`

- `const charT* data() const;`

  A null terminator is appended. 

  The C-style string is owned by the std::basic_string and should not be deleted

```c++
std::string sSource{ "abcdefg" };
std::cout << std::strlen(sSource.c_str()); // 7

std::string sSource{ "abcdefg" };
const char* szString{ "abcdefg" };
// memcmp compares the first n characters of two C-style strings and returns 0 if they are equal
if (std::memcmp(sSource.data(), szString, sSource.length()) == 0)
    std::cout << "The strings are equal";
else
    std::cout << "The strings are not equal";
```

