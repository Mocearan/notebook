# 顺序容器

​		顺序容器以线性序列的方式存储元素，不对元素进行排序，元素顺序与插入他们的顺序相同。

​		有五种标准顺序容器：

- `std::array<T, N>`， 数组容器，长度固定，不能增删元素
- `std::vector<T>`， 向量容器，长度单向可变，自动扩容
- `std::deque<T>`，双端队列容器，长度双向可变，自动扩容

- `std::list<T>`， 双向链表容器，长度可变，高效增删，低效随机访问
- `std::forward_list<T>`，单向链表容器，长度可变，单向查找





# std::deque



## 操作

### 迭代器

- `begin() / end()`
- `rbegin() / rend()`
- `cbegin() / cend()`
- `crbegin() / crend()`

### 属性

- `size()`
- `max_size()`
- `empty()`
- `capacity()`
- `resize()`

### 访问

- `at()`
- `[]`
- `front()/back()`

### 赋值

- `={}`
- `[]=`
- `assign()`

### 增删

- `push_back()/pop_back`
- `insert`
- `emplace() / emplace_back()`
- `erase()`
- `clear()`
- `swap()`



# std::list

## 操作

### 迭代器

- `begin() / end()`
- `rbegin() / rend()`
- `cbegin() / cend()`
- `crbegin() / crend()`

### 属性

- `size()`
- `max_size()`
- `empty()`
- `resize()`
- `reverse`

### 访问

- `front()/back()`

### 赋值

- `={}`
- `assign()`

### 增删

- `swap()`
- `push_back/push_front`
- `emplace`
- `emplace_back/emplace_front`
- `insert`
- `pop_front/pop_back`
- `erase`
- `remove()`
- `remove_if`()
- `unique()`
- `clear()`
- `sort()`
- `merge()`
- `splice()`

# std::forward_list

## 操作

### 迭代器

- `begin() / end()`

- `cbegin() / cend()`

- `before_begin / cbefore_begin`

  

### 属性

- `max_size()`
- `empty()`
- `resize()`
- `reverse()`

### 访问

- `front()`

### 赋值

- `={}`
- `assign()`

### 增删

- `swap()`
- `push_front`
- `emplace_after`
- `emplace_front`
- `insert_after`
- `pop_front`
- `erase_after`
- `remove()`
- `remove_if`()
- `unique()`
- `clear()`
- `sort()`
- `merge()`
- `splice_after()`
