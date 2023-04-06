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



- 值与一个关联的键相关联

  ​	值是易变的数据，键（key）是获取该值的钥匙

- 增删对容器结构影响较小，因此对迭代器影响较小

- `std::map`以红黑树实现

  - 红黑树是一种非严格意义上的平衡二叉树
  - 红黑树自动排序





## 基本使用

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





# `std::set<T>`

​		`<set>`

---

- 值唯一
- 自动排序
- 以红黑树实现