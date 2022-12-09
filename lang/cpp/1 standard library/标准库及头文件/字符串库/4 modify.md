# modify

---



## 赋值

### `operator=`

- `basic_string& operator= (const basic_string& str)`
- ``basic_string& operator= (const charT* str)``
- ``basic_string& operator= (charT c)``

```c++
std::string sString;

// Assign a string value
sString = std::string("One");
std::cout << sString << '\n';

// Assign a C-style string
sString = "Three";
std::cout << sString << '\n';

// Assign a char
sString = '5';
std::cout << sString << '\n';

// Chain assignment
std::string sOther;
sString = sOther = "Six";
std::cout << sString << ' ' << sOther << '\n';
```



### `assign()`

- ``basic_string& assign (const basic_string& str)``

- ``basic_string& assign (const charT* str)``

  ```c++
  std::string sString;
  const std::string sTwo("Two");
  
  sString.assign(sTwo);
  std::cout << sString << '\n';
  
  sString.assign("Four");
  std::cout << sString << '\n';
  ```

  

- `basic_string& assign(const basic_string& str, size_type index, size_type len);`

  ```c++
  const std::string sSource("abcdefg");
  std::string sDest;
  
  sDest.assign(sSource, 2, 4); // assign a substring of source from index 2 of length 4
  std::cout << sDest << '\n';
  ```

- `basic_string& assign(const charT* s, size_type len)`

  ```c++
  std::string sDest;
  sDest.assign("abcdefg", 4);
  ```

- `basic_string& assign(size_type len, charT c)`

  ```c++
  std::string sDest;
  sDest.assign(4, 'g');
  ```

  

## 拷贝

- `size_type copy(charT* buf, size_type len, size_type index = 0) const;`

  ​	将内容(不以null结尾)复制到字符数组.

  ```c++
  char szBuf[20]{}; // zero-ini
  int nLength{ static_cast<int>(sSource.copy(szBuf, 5, 10)) };
  // szBuf[nLength] = '\0';  // Make sure we terminate the string in the buffer
  
  std::cout << szBuf << '\n';
  ```

## 交换

- `void swap(basic_string& str)`
- `boid std::swap(basic_string& s1, basic_string& s2)`

​	交换两个字符串的值

```c++
std::string sStr1("red");
std::string sStr2("blue");

std::cout << sStr1 << ' ' << sStr2 << '\n';
std::swap(sStr1, sStr2);
std::cout << sStr1 << ' ' << sStr2 << '\n';
sStr1.swap(sStr2);
std::cout << sStr1 << ' ' << sStr2 << '\n';
```



## 尾插

### `operator+=`

- `basic_string& operator+=(const basic_string& s)`

  ```c++
  std::string sString{"one"};
  sString += std::string{" two"};
  std::cout << sString << '\n';
  ```

- `basic_string& operator+=(const charT* str)`

  ```c++
  std::string sString{"one"};
  sString += " two";
  ```

- `basic_string& operator+=(charT c);`

  ```c++
  std::string sString{"one"};
  sString += ' ';
  ```

  



### `append()`

- `basic_string& append(const basic_string& s)`

  ```c++
  std::string sString{"one"};
  std::string sThree{" three"};
  sString.append(sThree);
  
  std::cout << sString << '\n';
  ```

- `basic_string& append(const basid_string& s, size_type index, size_type num);`

  ```c++
  std::string sString{"one "};
  
  const std::string sTemp{"twothreefour"};
  sString.append(sTemp, 3, 5); // append substring of sTemp starting at index 3 of length 5
  std::cout << sString << '\n';
  ```

- `basic_string& append(const charT* s);`

  ```c++
  std::string sString{"one"};
  sString.append(" three");
  ```

- `basic_string& append(const charT* s, size_type len)`

  ```c++
  std::string sString{"one "};
  sString.append("threefour", 5); // one three
  ```

- `basic_string& append(size_type num, charT c)`

  ```c++
  std::string s{"aaa"};
  s.append(4, 'b');
  ```

- `template<class InputIt> basic_string& append(InputIt start, InputIt end);`

  - Appends all characters from the range [start, end) 



### `push_back()`

- `void push_back(charT c)`

  ```c++
  std::string sString{"one"};
  sString.push_back('2');
  ```

  

## 插入

### `insert()`

- `basic_string& insert(size_type index, const basic_string& str);`

  ```c++
  std::string sString("aaaa");
  sString.insert(2, std::string("bbbb"));
  ```

- `basic_string& insert(size_type index, const charT* str);`

  ```c++
  std::string sString("aaaa");
  sString.insert(4, "cccc");
  ```

- `basic_string& insert(size_type index, const basic_string& s, size_type start, size_type num);`

  ```c++
  std::string sString("aaaa");
  const string sInsert("01234567");
  
  sString.insert(2, sInsert, 3, 4); // inser [3, 7) into sString at index 2
  ```

- `basic_string& insert(size_type index, const charT* s, size_type len);`

  ```c++
  string sString("aaaa");
  sString.insert(2, "bcdef", 3); // aabcdaa
  ```

- `basic_string& insert(size_type index, size_type num, charT c)`

  ```c++
  string sString("aaaa");
  sString.insert(2, 4, 'c'); // aaccccaa
  ```

- `void insert(iterator it, size_type num, charT c);`

- `iterator insert(iterator it, charT c)`

- `void insert(iterator it, InputIt begin, InputIt end)`

  

## 删除

- `erase()`

  ​	移除字符

- `clear()`

  ​	清空字符串

## 更改

- `replace()`

  ​	替换字符串的指定部分

- `resize()`

  ​	展开或缩小字符串(在字符串末尾截断或添加字符)