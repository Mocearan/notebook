# map

> https://www.qcrao.com/2019/05/22/dive-into-go-map/
> https://qcrao.com/2020/05/06/dive-into-go-sync-map/
> https://draveness.me/golang/docs/part2-foundation/ch03-datastructure/golang-hashmap/

----

​		很多中文书籍翻译为映射、哈希表或字典，可以将一个值（Value）唯一关联到一个特定的键（Key）上，可以用于实现特定键值的快速查找与更新。

​		由 Go 编译器与运行时联合实现的复合数据类型。

![img](https://static001.geekbang.org/resource/image/f6/28/f6ac7392831455d3cf85f189a9dc9528.jpg?wh=1920x1047)

​		

​		和切片类型一样，map 也是引用类型。传递的只是一个“描述符”.

​		当 map 变量被传递到函数或方法内部后，我们在函数内部对 map 类型参数的修改在函数外部也是可见的。

```go

package main
  
import "fmt"

func foo(m map[string]int) {
    m["key1"] = 11
    m["key2"] = 12
}

func main() {
    m := map[string]int{
        "key1": 1,
        "key2": 2,
    }

    fmt.Println(m) // map[key1:1 key2:2]  
    foo(m)
    fmt.Println(m) // map[key1:11 key2:12] 
}
```



## 声明

- 仅声明

  没有显式地赋予 map 变量初值，map 类型变量的默认值为 nil。

  `var map[key_type]value_type`

  ```go
  var m map[string]string
  var n map[int]string
  ```

  > ​		map 类型，因为它内部实现的复杂性，无法“零值可用”。所以，如果我们对处于零值状态的 map 变量直接进行操作，就会导致运行时异常（panic），从而导致程序进程异常退出
  >
  > ```go
  > 
  > var m map[string]int // m = nil
  > m["key"] = 1         // 发生运行时异常：panic: assignment to entry in nil map
  > ```

- 声明并初始化

  - 显式初始化了 map 类型变量 m为空

    不等同nil。进行键值对的插入操作，不会引发运行时异常

    `var map[key_type]value_type{}`

  - 显式列表初始化

    ```go
    // {int, []string}
    m1 := map[int][]string{
        1: []string{"val1_1", "val1_2"},
        3: []string{"val3_1", "val3_2", "val3_3"},
        7: []string{"val7_1"},
    }
    
    // {Position, string}
    type Position struct { 
        x float64 
        y float64
    }
    
    m2 := map[Position]string{
        Position{29.935523, 52.568915}: "school",
        Position{25.352594, 113.304361}: "shopping-mall",
        Position{73.224455, 111.804306}: "hospital",
    }
    
    ```

    Go 允许省略字面值中的元素类型:

    ```go
    m2 := map[Position]string{
        {29.935523, 52.568915}: "school",
        {25.352594, 113.304361}: "shopping-mall",
        {73.224455, 111.804306}: "hospital",
    }
    ```

-  make 为 map 类型变量进行显式初始化

  可以为 map 类型变量指定键值对的初始容量，但无法进行具体的键值对赋值

  `var map_name = make(map[key_type]val_type)`

  `var map_name = make(map[key_type]val_type, capacity)` 指定初始容量

  > ​		map 类型的容量不会受限于它的初始容量值，当其中的键值对数量超过初始容量后，Go 运行时会自动增加 map 类型的容量，保证后续键值对的正常插入。

  

## 操作

### 相同

​		如果两个 map 类型的 key 元素类型相同，value 元素类型也相同，那么我们可以说它们是同一个 map 类型，否则就是不同的 map 类型。

### 比较

​		map 类型要保证 key 的唯一性。Go 语言中要求，key 的类型必须支持“==”和“!=”两种比较操作符

​		在 Go 语言中，函数类型、map 类型自身，以及切片只支持与 nil 的比较，而不支持同类型两个变量的比较。

​		函数类型、map 类型自身，以及切片类型是不能作为 map 的 key 类型的。

```go

s1 := make([]int, 1)
s2 := make([]int, 2)
f1 := func() {}
f2 := func() {}
m1 := make(map[int]string)
m2 := make(map[int]string)
println(s1 == s2) // 错误：invalid operation: s1 == s2 (slice can only be compared to nil)
println(f1 == f2) // 错误：invalid operation: f1 == f2 (func can only be compared to nil)
println(m1 == m2) // 错误：invalid operation: m1 == m2 (map can only be compared to nil)
```

### 插入

​		非 nil 的 map 类型变量，我们可以在其中插入符合 map 类型定义的任意新键值对。

```go
m := make(map[int]string)
m[1] = "value1"
m[2] = "value2"
m[3] = "value3"
```

> 不需要自己判断数据有没有插入成功，因为 Go 会保证插入总是成功的.
>
> Go 运行时会负责 map 变量内部的内存管理，因此除非是系统内存耗尽，我们可以不用担心向 map 中插入新数据的数量和执行结果

​		key 已经存在于 map 中了，那我们的插入操作就会用新值覆盖旧值:

```go
m := map[string]int {
  "key1" : 1,
  "key2" : 2,
}

m["key1"] = 11 // 11会覆盖掉"key1"对应的旧值1
m["key3"] = 3  // 此时m为map[key1:11 key2:2 key3:3]
```

### 获取键值对数量

​		map 类型也可以通过内置函数 len，获取当前变量已经存储的键值对数量

```go

m := map[string]int {
  "key1" : 1,
  "key2" : 2,
}

fmt.Println(len(m)) // 2
m["key3"] = 3  
fmt.Println(len(m)) // 3
```

> 不能对 map 类型变量调用 cap，来获取当前容量

### 查找和数据读取

​		map 类型更多用在查找和数据读取场合.

```go

m := make(map[string]int)
v := m["key1"]
```

> 如果这个键在 map 中并不存在，我们也会得到一个值，这个值是 value 元素类型的零值。

- “comma ok”惯用法

  ```go
  
  m := make(map[string]int)
  v, ok := m["key1"]
  if !ok {
      // "key1"不在map中
  }
  
  // "key1"在map中，v将被赋予"key1"键对应的value
  ```

  只关心某个键是否在于 map 中，我们可以使用空标识符替代变量 v

  ```go
  
  m := make(map[string]int)
  _, ok := m["key1"]
  ... ...
  ```

删除数据

​		借助内置函数 delete 来从 map 中删除数据.

​		使用 delete 函数的情况下，传入的第一个参数是我们的 map 类型变量，第二个参数就是我们想要删除的键.

```go

m := map[string]int {
  "key1" : 1,
  "key2" : 2,
}

fmt.Println(m) // map[key1:1 key2:2]
delete(m, "key2") // 删除"key2"
fmt.Println(m) // map[key1:1]
```

### 遍历 map 

​		遍历 map 的键值对只有一种方法，通过 for range 语句对 map 数据进行遍历:

```go

package main
  
import "fmt"

func main() {
    m := map[int]int{
        1: 11,
        2: 12,
        3: 13,
    }

    fmt.Printf("{ ")
    for k, v := range m {
        fmt.Printf("[%d, %d] ", k, v)
    }
    fmt.Printf("}\n")
}

///////////////////////////

for k, _ := range m { 
  // 使用k
}

for k := range m {
  // 使用k
}


for _, v := range m {
  // 使用v
}
```

> 对同一 map 做多次遍历的时候，每次遍历元素的次序都不相同.
>
> 程序逻辑千万不要依赖遍历 map 所得到的的元素次序

- 对 map 的进行稳定次序遍历
  - 把key存到有序切片中，用切片遍历
  - 参考java的LinkedHashMap,能实现插入有序或者访问有序，就是使用额外的链表来保存顺序。
    go 中可以基于container/list来实现。github上现成的功能。
    https://github.com/elliotchance/orderedmap

## 实现

​		Go 运行时使用一张哈希表来实现抽象的 map 类型。运行时实现了 map 类型操作的所有功能，包括查找、插入、删除等。在编译阶段，Go 编译器会将 Go 语法层面的 map 操作，重写成运行时对应的函数调用。

![img](https://static001.geekbang.org/resource/image/60/0b/60810f564fcbbc8fa1d212ab2dc1ae0b.jpg?wh=1920x1047)

> ```go
> // 创建map类型变量实例
> m := make(map[keyType]valType, capacityhint) → m := runtime.makemap(maptype, capacityhint, m)
> 
> // 插入新键值对或给键重新赋值
> m["key"] = "value" → v := runtime.mapassign(maptype, m, "key") v是用于后续存储value的空间的地址
> 
> // 获取某键的值 
> v := m["key"]      → v := runtime.mapaccess1(maptype, m, "key")
> v, ok := m["key"]  → v, ok := runtime.mapaccess2(maptype, m, "key")
> 
> // 删除某键
> delete(m, "key")   → runtime.mapdelete(maptype, m, “key”)
> ```



​		与语法层面 map 类型变量（m）一一对应的是 runtime.hmap 的实例。hmap 类型是 map 类型的头部结构（header），它存储了后续 map 类型操作所需的所有信息：

![img](https://static001.geekbang.org/resource/image/2f/04/2f5ff72fbdb17cf0cb0b8da102c3e604.jpg?wh=1920x1047)

### bucket

​		真正用来存储键值对数据的是桶，也就是 bucket。每个 bucket 中存储的是 Hash 值低 bit 位数值相同的元素，默认的元素个数为 BUCKETSIZE。

> ​		值为 8，在 $GOROOT/src/cmd/compile/internal/gc/reflect.go 中定义，与 runtime/map.go 中常量 bucketCnt 保持一致。

​		某个 bucket（比如 buckets[0]) 的 8 个空槽 slot）都填满了，且 map 尚未达到扩容的条件的情况下，运行时会建立 overflow bucket，并将这个 overflow bucket 挂在上面 bucket（如 buckets[0]）末尾的 overflow 指针上，这样两个 buckets 形成了一个链表结构，直到下一次 map 扩容之前，这个结构都会一直存在。

### tophash

​		key 做哈希运算，并获得一个哈希值（hashcode）来进行插入和按key查询。

 		hashcode 低位区的值用于选定 bucket，高位区的值用于在某个 bucket 中确定 key 的位置。

![img](https://static001.geekbang.org/resource/image/ef/08/ef729c06cd8fa19f29f89df212c7ea08.jpg?wh=1920x1047)

​		每个 bucket 的 tophash 区域其实是用来快速定位 key 位置的，这样就避免了逐个 key 进行比较这种代价较大的操作。

​		尤其是当 key 是 size 较大的字符串类型时，好处就更突出了。这是一种以空间换时间的思路。

### key存储区域

​		tophash 区域下面是一块连续的内存区域，存储的是这个 bucket 承载的所有 key 数据。

​		运行时在分配 bucket 的时候需要知道 key 的 Size。Go 运行时就是利用 maptype 参数中的信息确定 key 的类型和大小的。

> ​		Go 运行时就会为这个变量对应的特定 map 类型，生成一个 runtime.maptype 实例。如果这个实例已经存在，就会直接复用。这个实例就是map类型的元信息。
>
> ```go
> 
> type maptype struct {
>     typ        _type
>     key        *_type
>     elem       *_type
>     bucket     *_type // internal type representing a hash bucket
>     keysize    uint8  // size of key slot
>     elemsize   uint8  // size of elem slot
>     bucketsize uint16 // size of bucket
>     flags      uint32
> } 
> ```
>
> ​		编译器会把语法层面的 map 操作重写成运行时对应的函数调用，这些运行时函数都有一个共同的特点，那就是第一个参数都是 maptype 指针类型的参数。
>
> ​		map 所用的 hash 函数也存放在 maptype.key.alg.hash(key, hmap.hash0) 中。同时 maptype 的存在也让 Go 中所有 map 类型都共享一套运行时 map 操作函数。节省了对最终二进制文件空间的占用。
>
> > ​		 C++ 为每种 map 类型创建一套 map 操作函数，是为了减少类型携带的元信息，提高运行时的效率。进行零开销抽象。

### value存储区

​		key 存储区域下方的另外一块连续的内存区域，存储的是 key 对应的 value。

​		这个区域的创建也是得到了 maptype 中信息的帮助。

​		Go 运行时采用了把 key 和 value 分开存储的方式，而不是采用一个 kv 接着一个 kv 的 kv 紧邻方式存储，这带来的其实是算法上的复杂性，但却减少了因内存对齐带来的内存浪费。

​	![img](https://static001.geekbang.org/resource/image/5b/5a/5bce9aaebc78bdea7d2999606891325a.jpg?wh=1920x1047)

>  Go 运行时使用的方案内存利用效率很高，而 kv 紧邻存储的方案在 map[int8]int64 这样的例子中内存浪费十分严重，它的内存利用率是 72/128=56.25%，有近一半的空间都浪费掉了。

​		如果 key 或 value 的数据长度大于一定数值，那么运行时不会在 bucket 中直接存储数据，而是会存储 key 或 value 数据的指针。目前 Go 运行时定义的最大 key 和 value 的长度是这样的：

```go

// $GOROOT/src/runtime/map.go
const (
    maxKeySize  = 128
    maxElemSize = 128
)
```

### map扩容

​		map 会对底层使用的内存进行自动管理。

​		在使用过程中，当插入元素个数超出一定数值后，map 一定会存在自动扩容的问题，也就是怎么扩充 bucket 的数量，并重新在 bucket 间均衡分配数据的问题。

​		Go 运行时的 map 实现中引入了一个 LoadFactor（负载因子），当 count > LoadFactor * 2^B 或 overflow bucket 过多时，运行时会自动对 map 进行扩容。目前 Go 最新 1.17 版本 LoadFactor 设置为 6.5

> （loadFactorNum/loadFactorDen）
>
> ```go
> 
> // $GOROOT/src/runtime/map.go
> const (
>   ... ...
> 
>   loadFactorNum = 13
>   loadFactorDen = 2
>   ... ...
> )
> 
> func mapassign(t *maptype, h *hmap, key unsafe.Pointer) unsafe.Pointer {
>   ... ...
>   if !h.growing() && (overLoadFactor(h.count+1, h.B) || tooManyOverflowBuckets(h.noverflow, h.B)) {
>     hashGrow(t, h)
>     goto again // Growing the table invalidates everything, so try again
>   }
>   ... ...
> }
> ```

- 如果是因为 overflow bucket 过多导致的“扩容”

  ​		实际上运行时会新建一个和现有规模一样的 bucket 数组，然后在 assign 和 delete 时做排空和迁移。

- 果是因为当前数据数量超出 LoadFactor 指定水位而进行的扩容

  ​		运行时会建立一个两倍于现有规模的 bucket 数组，但真正的排空和迁移工作也是在 assign 和 delete 时逐步进行的。

  ​		原 bucket 数组会挂在 hmap 的 oldbuckets 指针下面，直到原 buckets 数组中所有数据都迁移到新数组后，原 buckets 数组才会被释放。

  ![img](https://static001.geekbang.org/resource/image/6e/29/6e94c7ee51a01fcf2267a7e2145d6929.jpg?wh=1920x1047)



​		考虑到 map 可以自动扩容，map 中数据元素的 value 位置可能在这一过程中发生变化，所以 Go 不允许获取 map 中 value 的地址，这个约束是在编译期间就生效的。

```go

p := &m[key]  // cannot take the address of m[key]
fmt.Println(p)
```



## 并发性

​		hmap 实例自身是有状态的（hmap.flags），而且对状态的读写是没有并发保护的。map 实例不是并发写安全的，也不支持并发读写。

​		对 map 实例进行并发读写，程序运行时就会抛出异常：

```go

package main

import (
    "fmt"
    "time"
)

func doIteration(m map[int]int) {
    for k, v := range m {
        _ = fmt.Sprintf("[%d, %d] ", k, v)
    }
}

func doWrite(m map[int]int) {
    for k, v := range m {
        m[k] = v + 1
    }
}

func main() {
    m := map[int]int{
        1: 11,
        2: 12,
        3: 13,
    }

    go func() {
        for i := 0; i < 1000; i++ {
            doIteration(m)
        }
    }()

    go func() {
        for i := 0; i < 1000; i++ {
            doWrite(m)
        }
    }()

    time.Sleep(5 * time.Second)
}

// fatal error: concurrent map iteration and map write
```



​		如果我们仅仅是进行并发读，map 是没有问题的。Go 1.9 版本中引入了支持并发写安全的 sync.Map 类型，可以用来在并发读写的场景下替换掉 map.

> [sync package - sync - pkg.go.dev](https://pkg.go.dev/sync#Map)

