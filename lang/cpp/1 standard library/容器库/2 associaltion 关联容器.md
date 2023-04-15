# 关联容器

---





## 关联容器的数据结构

- `std::map / std::multimap / std::set / std::multiset` 以红黑树实现
- `std::unordered_*` 以哈希表实现

​	

​	以红黑树实现的数据结构：

- 链式结构
- insert不会导致迭代器失效
- 查找效率是`log2`

# ``std::map<K,V>``

​		``<map>``

---

​		线性搜索对于除``shortest lists``外的所有列表都是低效的。标准库提供了一个叫做``map``的平衡二叉搜索树(通常是红黑树).

​		一般的编程语言中，将`map`这样的数据结构称为关联数组或字典。

- 值与一个关联的键相关联

  ​	值是易变的数据，键（key）是获取该值的钥匙

- 增删对容器结构影响较小，因此对迭代器影响较小

- `std::map`以红黑树实现

  - 红黑树是一种非严格意义上的平衡二叉树
  - 红黑树自动排序
  
- 查找和插入优化

  - 查找代价是``O(log(n))``






## 基本使用

### 初始化

- 列表初始化

  ```c++
  map<string,int> phone_book {
      {"David Hume",123456},
      {"Karl Popper",234567},
      {"Bertrand Arthur William Russell",345678}
  };
  ```



### 访问

​	如果没有找到某个键，会将对应键插入到`map`中，并为其默认的值。

```c++
phone_book[s];
```

​		想避免无效输入，可以使用``find()``和``insert()``来代替``[]``

### 插入

```c++
// insert pair{K,V}
id_name_map.insert(std::pair<int, std::string>(1, "mass"));
id_name_map.insert({2, "bean"});
std::pair<std::map<int, std::string>::iterator, bool> insert_res = id_name_map.insert({3, "tsing"});
if(insert_res.second == true)
    // insert ok

// insert value_type
id_name_map.insert(std::map<int, std::string>::value_type{1, "mass"});

// operator[K] = V
id_name_map[1] = "mass";
```

- 如果key存在
  - `insert`失败
  - `operator[]`会返回`key`对应`value`的引用，从而覆盖旧值

### 查找

```c++
// if map.count(v)
if ( id_name_map.count(1) not_eq 0 )
    std::cout << "found\n";

// find
if ( auto it = id_name_map.find(1); it not_eq id_name_map.end() )
    std::cout << "found\n";
```



# `std::unordered_map<K,V>`

​		许多情况下，使用散列查找比在有序容器进行比较可以做得更好。标准库哈希容器被称为无序容器，因为它们不需要排序函数。

- 标准库为字符串以及其他内置和标准库类型提供了默认的散列函数。

- 可以提供自己的。哈希函数通常被实现为一个函数对象

  ```c++
  struct Record {
       string name;
       int product_code;
       // ...
  };
  
  struct Rhash {           // a hash function for Record
       size_t operator()(const Record& r) const
       {
         	return hash<string>()(r.name) ^ hash<int>()(r.product_code);
       }
  };
  unordered_set<Record,Rhash> my_set; // set of Records using Rhash for lookup
  ```

  > 设计好的哈希函数是一门艺术，通常需要了解要应用哈希函数的数据。通过使用异或(^)组合现有哈希函数来创建新的哈希函数非常简单，而且通常非常有效。但是，要注意确保参与散列的每个值都有助于区分这些值。例如，除非同一产品代码有多个名称(或同一名称有多个产品代码)，否则将两个哈希值组合在一起没有任何好处。

- 我们可以通过将哈希操作定义为标准库哈希的特化来避免显式传递哈希操作:

  ```c++
  namespace std {                // make a hash function for Record
  
      template<> struct hash<Record> {
          using argument_type = Record;
          using result_type = size_t;
  
          result_type operator()(const Record& r) const
          {
              return hash<string>()(r.name) ^ hash<int>()(r.product_code);
          }
      };
  }
  ```



​		注意``map``和``unordered_map``之间的区别:

- `map`需要一个排序函数(默认值是<)并生成一个有序序列。
- `unordered_map`需要一个相等函数(默认是==);它不维持元素之间的秩序。

​		给定一个好的哈希函数，对于大型容器，unordered_map要比map快得多。然而，具有糟糕哈希函数的unordered_map的最坏情况要比map糟糕得多。

## 基本使用

### 初始化

- 列表初始化

  ```c++
  unordered_map<string,int> phone_book {
       {"David Hume",123456},
       {"Karl Popper",234567},
       {"Bertrand Arthur William Russell",345678}
  };
  ```

### 访问

```c++
phone_book[s];
```



# `std::set<T>`

​		`<set>`

---

- 值唯一
- 自动排序
- 以红黑树实现