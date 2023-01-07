# istream

​		`basic_istream`对于常用字符类型定义了两个具体类：`istream / wistream`，`<istream>`

---



## 全局标准输入对象 

​		标准库提供两个全局`basic_istream`对象：`cin / wcin`



## 有格式输入

```c++
char buf[10]{};
std::cin >> buf;
```

​		`operator>>`提取整数值(`char`是整型）可能会忽略输入流中间的前导空格空白符。

```c++
int main()
{
    char ch;
    while (std::cin >> ch)
        std::cout << ch;

    return 0;
}

// [in] Hello my name is Alex
// [out] HellomynameisAlex
```

​		可以使用无格式输入的`get()`来读入空白符。

## 无格式输入

### get

​		从流中提取字符。

```c++
char ch;
while(std::cin.get(ch))
    std::cout << ch;

// [in] Hello my name is Alex
// [out] Hello my name is Alex
```



```c++
char str[11]{};
std::cin.get(tr, 11;
```

​		会在遇到换行符时结束提取，不会提取换行符。



### getline

​		从流提取字符，直到遇到换行符或指定的分隔符。

```c++
// Read up to 10 characters
std::cin.getline(strBuf, 11);
std::cout << strBuf << '\n';

// Read up to 10 more characters
std::cin.getline(strBuf, 11);
std::cout << strBuf << '\n';
```



### getcount

​	返回上次无格式输入操作提取到的字符数量

```c++
char strBuf[100];
std::cin.getline(strBuf, 100);
std::cout << std::cin.gcount() << " characters were read" << '\n';
```

​		不同于`basic_istream::get`，会提取换行符或分隔符，并计入``gcount()``，但不存储。



### igore

- `ignore()`

  ​	弃置（读出但不存储）流中当前第一个字符。

- `ignore(int num)`

  ​	弃置流中当前前num个字符。



### peek

​		只读但不提取流中的字符。

### unget

​		向流中返还上次读出字符。

### putback

​		向流中放回一个指定字符。