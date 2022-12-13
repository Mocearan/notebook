# string constructor

---





## from numbers

​		`std::basic_string`没有提供从数字创建字符串的功能。一般是将数字转换成字符串。

```c++
std::string s{4}; // cannot convert parameter 4 from 'int' to 'std::basic_string'
```

- `std::ostringstream` format number to string

  ```c++
  template <typename T>
  inline std::string ToString(T x)
  {
  	std::ostringstream o;
      o << x;
      return o.str();
  }
  
  std::string sFour{ ToString(4) };
  std::string sSixPointSeven{ ToString(6.7) };
  std::string sA{ ToString('A') };
  ```

  >
  >
  >通常：
  >
  >```c++
  >using StringBuilder = std::ostringstream;
  >```

- `std::to_string()`

  ​	对于基本情况可以使用，但是和`std::cout`和上述`ToString()`略有不同。



​	对应的，可以从字符串转出：

```c++
template<typename T>
inline bool FromString(const std::string& s, T& x)
{
	std::istringstream is{s};
    return not (is >> x).fail(); // extract value into x, return success or not
}
```

