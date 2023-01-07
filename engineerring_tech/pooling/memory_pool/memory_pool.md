# Memory Pool

---

​		程序中涉及小块内存的频繁分配和释放，malloc和free的实现复杂性，性能表现不是很好，可以利用内存池来提高这种场景下的内存使用效率。



## SGI STL 二级空间配置器

​		SGI STL包含了一级空间配置器和二级空间配置器，其中一级空间配置器allocator采用malloc和free来 管理内存，和C++标准库中提供的allocator是一样的，但其二级空间配置器allocator采用了基于freelist 自由链表原理的内存池机制实现内存管理。

![image-20211128191446382](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20211128191446382.png)

### 空间配置器的相关定义

```c++
template <class _Tp, class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp) >
class vector : protected _Vector_base<_Tp, _Alloc>
```

​		可以看到，容器的默认空间配置器是__STL_DEFAULT_ALLOCATOR( _Tp)，它是一个宏定义，如下：

```c++
# ifndef __STL_DEFAULT_ALLOCATOR
# ifdef __STL_USE_STD_ALLOCATORS
# define __STL_DEFAULT_ALLOCATOR(T) allocator< T >
# else
# define __STL_DEFAULT_ALLOCATOR(T) alloc
# endif
# endif
```

​		``__STL_DEFAULT_ALLOCATOR``通过宏控制有两种实现，一种是``allocator< T >``，另一种 是``alloc``，这两种分别就是SGI STL的一级空间配置器和二级空间配置器的实现。

```c++
template <int __inst>
class __malloc_alloc_template // 一级空间配置器内存管理类 -- 通过malloc和free管理内存
    
template <bool threads, int inst>
class __default_alloc_template { // 二级空间配置器内存管理类 -- 通过自定义内存池实现内存管理
```

### 重要类型和变量定义

#### 内存池的粒度信息

```c++
enum {_ALIGN = 8};
enum {_MAX_BYTES = 128};
enum {_NFREELISTS = 16};
```

#### chunk块的头信息

```c++
union _Obj {
union _Obj* _M_free_list_link;
char _M_client_data[1]; /* The client sees this. */
};
```

#### 自由链表的数组

```c++
// 组织所有自由链表的数组，数组的每一个元素的类型是_Obj*，全部初始化为0
static _Obj* __STL_VOLATILE _S_free_list[_NFREELISTS];
```

#### chunk块的分配情况

```c++
// Chunk allocation state. 记录内存chunk块的分配情况
static char* _S_start_free;
static char* _S_end_free;
static size_t _S_heap_size;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_start_free = 0;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_end_free = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;
```

### 重要的辅助接口函数

#### round up

```c++
/*将 __bytes 上调至最邻近的 8 的倍数*/
static size_t _S_round_up(size_t __bytes)
{ return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }
```

> ```c++
> // enum {_ALIGN = 8};
> // (size_t) _ALIGN
> 0000 0001 -> 0000 0000 | 0000 0000 | 0000 0000 | 0000 1000
> // (size_t) _ALIGN - 1
> 0000 0000 | 0000 0000 | 0000 0000 | 0000 1000
> 0000 0000 | 0000 0000 | 0000 0000 | 0000 0111
> // ~((size_t) _ALIGN - 1) -> MASK ， 从8开始取位， 忽略低3三位
> 1111 1111 | 1111 1111 | 1111 1111 | 1111 1000
> // ((__bytes) + (size_t) _ALIGN-1) // 补7，以使得只要不是0就能进位到8
> 0000 0000 | 0000 0000 | 0000 0000 | 0000 0111 + __bytes
> // (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)) // 取得的数为8的倍数
> 0000 0000 | 0000 0000 | 0000 0000 | 0000 0111 + __bytes
> 1111 1111 | 1111 1111 | 1111 1111 | 1111 1000
> ```

#### index

```c++
/*返回 __bytes 大小的chunk块位于 free-list 中的编号*/
static size_t _S_freelist_index(size_t __bytes) {
return (((__bytes) + (size_t)_ALIGN-1)/(size_t)_ALIGN - 1); }

```



