# Range

---

​		STL提供的算法分为使用概念约束的版本和为了兼容性而没有进行约束的版本。

​		受概念约束版本在名字空间`std::range`中。`range`是泛化的由``{begin(), end()}``指明的序列：

- 一个`{begin(), end()}`迭代器对
- 一个`{begin(), n}`对
- 一个`{begin(), pred}`对，``pred``是一个谓词

​		使用`std::range`可以避免算法中对`{begin(), end()}`的直接使用。

```c++
template<forward_range R> requires sortable<iterator_t<R>>
void my_sort(R& r)  // modern, concept-constrained version of my_sort
{
  return std::sort(r.begin(),r.end());  // use the 1994-style sort
}
```



## range类型

​		不同的迭代器对应着不同的范围，他们都是概念

- `input_range`
- `forward_range`
- `bidirectional_range`
- `random_access_range`
- `contious_range`



## range view

​		`range`可以创造并操作一个`view`。`view`是一个代表可迭代对象的轻量对象。

```c++
auto bad()
{
 	vector v = {1, 2, 3, 4};
  	return std::ranges::filter_view{v,odd};               // v will be destroyed before the view
}
```

​		`view`接口与`range`接口非常相似，一般只要能使用`range`就可以使用`view`，而且方式相同。关键的区别是`view`不拥有它的元素，它不负责删除其基础`range`的元素——这是范围的责任。

```c++
struct Reading {
         int location {};
         int temperature {};
         int humidity {};
         int air_pressure {};
         //...
};

int average_temp(vector<Reading> readings)
{
         if (readings.size()==0) throw No_readings{};
         double s = 0;
         for (int x: views::elements<1>(readings))              // look at just the temperatures
                 s += x;
         return s/readings.size();
}
```





### 范围适配器

​		标准库提供了许多视图，也称为范围适配器(range adaptors)。



| **Standard-library views (range adaptors)** **<ranges>****v** is a **view**; **r** is **range**; **p** is a predicate; **n** is an integer |                                                              |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| **v=all_view{r}**                                            | **v** is all elements from **r**                             |
| **v=filter_view{r,p}**                                       | **v** is elements from **r** that meets **p**                |
| **v=transform_view{r,f}**                                    | **v** is the results of calling **f** on each element from **r** |
| **v=take_view{r,n}**                                         | **v** is at most **n** elements from **r**                   |
| **v=take_while_view{r,p}**                                   | **v** is elements from **r** until one doesn’t meet the **p** |
| **v=drop_view{r,n}**                                         | **v** is elements from **r** starting with the **n+1**th element |
| **v=drop_while_view{r,p}**                                   | **v** is elements from **r** starting with the first element that doesn’t meet **p** |
| **v=join_view{r}**                                           | **v** is a flattened version of **r**; the elements of **r** must be ranges |
| **v=split_view(r,d)**                                        | **v** is a range of sub-ranges of **r** determined by the delimiter **d**; |
|                                                              | **d** must be an element or a range                          |
| **v=common_view(r)**                                         | **v** is **r** described by a (**begin**:**end**) pair       |
| **v=reverse_view{r}**                                        | **v** is the elements of **r** in reverse order; **r** must have bidirectional access |
| **v=views::elements<n>(r)**                                  | **v** is the range of the **n**th elements of the **tuple** elements of **r** |
| **v=keys_view{r}}**                                          | **v** is the range of the first elements of the **pair** elements of **r** |
| **v=values_view{r}**                                         | **v** is the range of the second elements of the **pair** elements of **r** |
| **v=ref_view{r}**                                            | **v** is range of elements that are references to the elements of **r** |



```c++
void user(forward_range auto& r)
{
    std::ranges::filter_view v {r, [](int x) { return x%2; } }; // view (only) odd numbers from r

    cout << "odd numbers: "
    for (int x : v)
             cout << x <<' ';
}
```

​		许多范围是无限的。此外，我们通常只需要几个值。因此，有一些视图只从一个范围中获取少数值:

```c++
void user(forward_range auto& r)
{
     filter_view v{r, [](int x) { return x%2; } };        // view (only) odd numbers in r
//     std::ranges::take_view tv {v, 100 };   // view at most 100 element from v

//     cout << "odd numbers: "
//     for (int x : tv)
//        cout << x <<' ';
    for (int x : take_view{v, 3})
       cout << x <<' ';
}


```

​		上面这段代码还可以写成：

```c++
for (int x : take_view{ filter_view { r, [](int x) { return x % 2; } }, 3 })
        cout << x <<' ';
```

​		这种视图嵌套很快就会变得神秘难以理解，于是`range`提供了管道来简化。

