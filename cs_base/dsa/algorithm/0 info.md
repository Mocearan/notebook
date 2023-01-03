# 算法

---



​		基本运算及规定的运算顺序所构成的完整的解法，或者看成按照要求设计好的游戏那的确切的计算序列。

## 算法的特性

- 输入
- 输出
- 有穷性
- 确定性
- 可行性

## 算法的设计目标

- 正确性
- 可读性
- 健壮性
- 高效率与低存储量需求



## 算法复杂度分析

​		算法是研究以更少算力更少空间来解决问题的学科，其中算力以运行时间来衡量，空间以内存占用来衡量。二者随数据规模的增长趋势的数学特征称之为复杂度。

​		复杂度分析在纯粹理论角度，对解法的时空消耗进行估算，大体对时空消耗随数据规模的增长特征做出数量级上的判断，对解法的算力和内存需求进行性能判断。

### 大O表示法

1.  设每行基本语句的执行时间为一致的单位时间 unit_time

2.  那么解法的时间消耗为```T(n) = unit_time * f(n)```

    >   f(n) 是计算解法为得到解所需要执行的所有基本语句量
    >
    >   ```c++
    >   int cal(int n) {
    >      int sum = 0;		// 1
    >      int i = 1, j = 1;	//  1
    >      for (; i <= n; ++i) {		 // n
    >          j = 1;				    // n
    >          for (; j <= n; ++j)       // n*n
    >              sum = sum +  i * j;   // n*n
    >      }
    >   }
    >   ```
    >
    >   `f(n) = 2*n*n + 2*n + 2 = 2n²+2n+2`

3.  如果不关心具体的时间消耗到底是多少，只关心需要执行的基本语句量与时间消耗的渐进关系：```T(n) = O(f(n))```

    >   O(N) 渐近时间复杂度， 表示解法的时间消耗随数据规模（所需执行基本语句量）的变化趋势。

4.  对于复杂度的估算，只关心随语句量增长最大的数学特征，即数学量级最高的数学表征：``O(f(n)) = O(2n²+2n+2)≈O(n²)``

    >   需要注意：
    >
    >   1.  加法原则：总复杂等于量级最大的那段代码的复杂度
    >
    >       `T(n) = T1(n) + T2(n) = max(O(fn), O(g(n))) = O(max(f(n), g(n)))`
    >
    >       一个解法的复杂度等于量级最大的子解法的时间复杂度
    >
    >   
    >
    >       ```c++
    >           
    >       int cal(int n) {
    >          int sum_1 = 0;
    >          int p = 1;
    >          for (; p < 100; ++p) {
    >            sum_1 = sum_1 + p;
    >          }
    >           
    >          int sum_2 = 0;
    >          int q = 1;
    >          for (; q < n; ++q) {
    >            sum_2 = sum_2 + q;
    >          }
    >           
    >          int sum_3 = 0;
    >          int i = 1;
    >          int j = 1;
    >          for (; i <= n; ++i) {
    >            j = 1; 
    >            for (; j <= n; ++j) {
    >              sum_3 = sum_3 +  i * j;
    >            }
    >          }
    >           
    >          return sum_1 + sum_2 + sum_3;
    >        }
    >       ```
    >           
    >        `T(n) = T1(n) + T2(n) + T3(n) ≈ O(1) + O(n) + O(n²) ≈ O(n²) `
    >
    >   2.  乘法法则：嵌套块中的复杂度等于嵌套内外代码复杂度的乘积
    >
    >   `T(n) = T1(n) * T2(n) = O(f(n)) * O(g(n)) = O(f(n) * g(n))`
    >
    >   ```c++
    >   int f(int n) {		// T1(n) = O(f(n)) = O(2n + 2) ≈ O(n)
    >       int sum = 0;
    >       int i = 1;
    >       for (; i < n; ++i) {
    >           sum = sum + i;
    >       } 
    >       return sum;
    >   }
    >   
    >   int cal(int n) {    // T2(n) = O(g(n)) = O(2n + 2)
    >       int ret = 0; 
    >       int i = 1;
    >       for (; i < n; ++i) {
    >           ret = ret + f(i);
    >       } 
    >   } 
    >   ```
    >
    >   `T(n) = T1(n) * T2(n) ≈ O(n)* O(n) = O(n²)  `




### 常见复杂度

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/20210310171634.jpeg)

`O(2^n)`和`O(n!)`成为非多项式量级（NP），其余为多项式量级（P）。

当数据规模增长时，NP算法的时间复杂度会急剧增加，

#### O(1)

只要算法中不存在循环语句、递归语句、跳转语句，即顺序语句的行数再多，时间复杂度也是O(1)。

#### O(logn) / O(nlogn)

非常常见又最难分析。

```c++
i = 1;
while ( i <= n ) {
	i *= 2;
}

// cnt   1  2   3       k      x
// i     2 2*2  2*2*2  2^k    2^x  = n
// 即执行次数cnt 的最终值x = log2n
```

对于对数的底，可以通过换底公式都换成以2为底n的对数。所以在对数阶时间复杂度的表示方法里，忽略对数的底的影响，统一表示为`O(logn)`.

对于`O(logn)`的子解法，需要重复n此才能最终求解，则该解法的复杂度通过乘法法则可知为`O(nlogn)`.这是一种常见的复杂度量级，比如归并排序，快速排序。

#### O(m+n)、O(m*n)

决定复杂度的不止一个数据维度。

```c++
int cal(int m, int n) {
    int sum_1 = 0;
    int i = 1;
    for (; i < m; ++i) {
        sum_1 = sum_1 + i;
    }

    int sum_2 = 0;
    int j = 1;
    for (; j < n; ++j) {
        sum_2 = sum_2 + j;
    }

    return sum_1 + sum_2;
}
```

无法在解法设计时确知m和n的量级大小关系，所以不能简单的使用加法法则，省略掉较小的一个。所以复杂度即为`O(m+n)`.

但是乘法法则继续有效。



#### 时间复杂度

```c++
// n表示数组array的长度
int find(int array[], int n, int x) {
    int i = 0;
    int pos = -1;
    for (; i < n; ++i) {
        if (array[i] == x) {
            pos = i;
            break;
        }
    }
    return pos;
}
```

​		该代码，在查找到相应的元素后会跳出循环结束任务。那么并不是每次执行该解法所执行的代码量是不定的，最好的情况是要查找的元素在数组首元素，那么时间复杂度就是`O(1)`，但最坏情况下数组中不存在待查元素，那么时间复杂度就是`O(n)`。

##### 最好/最坏情况时间复杂度

最好情况时间复杂度就是，在最理想的情况下，执行这个解法的时间复杂度。

最坏情况时间复杂度就是，在最糟糕的情况下，执行这个解法的时间复杂度。

##### 平均情况时间复杂度

最好情况时间复杂度和最坏情况时间复杂度都是极端假设下的复杂度，发生的概率并不大。平均情况时间复杂度则能更均衡的表征解法的整体复杂度水平。

需要考虑解法的整体加权平均值（期望）。

比如待查数据在不在数组中的概率为½， 在数组中时，每个位置上出现的概率为1/2，那么全概率为：

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/20210311145527.jpeg)

通常无需区分如上三种情况，因为大致量级都是一样的。只有当同一解法在不同情况下，时间复杂度有量级的差距，才会用这三种复杂度来区分。

##### 均摊时间复杂度

```c++
// array表示一个长度为n的数组
// 代码中的array.length就等于n
int array[] = new int[n];
int count = 0;

void insert(int val) {
    if (count == array.length) {
        int sum = 0;
        for (int i = 0; i < array.length; ++i) {
            sum = sum + array[i];
        }
        array[0] = sum;
        count = 1;
    }

    array[count] = val;
    ++count;
}
```

![image-20210311161107548](https://gitee.com/masstsing/picgo-picserver/raw/master/20210311161107.png)

平均时间复杂度在极端假设下复杂度才为`O(1)`，只有这种场景下，个别情况下复杂度才比较高为`O(n)`。并且高复杂度的子解法出现的频率是规律的，而且有一定的前后时序关系，一般是在一个高复杂度解法之后跟着一组低复杂度的操作，循环往复。

那么，可以将高复杂度的个别子解法的时间消耗均摊在一组低复杂度的每个操作上。在能够应用均摊时间复杂度的场合，一般均摊时间复杂度就等于最好情况时间复杂度。

均摊时间复杂度就是一种特化的平均时间复杂度。

```c++
// 全局变量，大小为10的数组array，长度len，下标i。
int array[] = new int[10]; 
int len = 10;
int i = 0;

// 往数组中添加一个元素
void add(int element) {
    if (i >= len) { // 数组空间不够了
        // 重新申请一个2倍大小的数组空间
        int new_array[] = new int[len*2];
        // 把原来array数组中的数据依次copy到new_array
        for (int j = 0; j < len; ++j) {
            new_array[j] = array[j];
        }
        // new_array复制给array，array现在大小就是2倍len了
        array = new_array;
        len = 2 * len;
    }
    // 将element放到下标为i的位置，下标i加一
    array[i] = element;
    ++i;
}
```



#### 空间复杂度

空间复杂度一般指渐进空间复杂度，表示解法需要的存储空间与数据规模之间的增长关系。

常见的空间复杂度为`O(1), O(n), O(n²)`.空间复杂度分析比时间复杂度分析简单一些。





#### 如何进行复杂度分析？

2.复杂度分析法则 

1）单段代码看高频：比如循环。

 2）多段代码取最大：比如一段代码中有单循环和多重循环，那么取多重循环的复杂度。

 3）嵌套代码求乘积：比如递归、多重循环等 

4）多个规模求加法：比如方法有两个参数控制两个循环的次数，那么这时就取二者复杂度相加。 

#### 常用的复杂度级别？

​		多项式阶：随着数据规模的增长，算法的执行时间和空间占用，按照多项式的比例增长。包括， O(1)（常数阶）、O(logn)（对数阶）、O(n)（线性阶）、O(nlogn)（线性对数阶）、O(n^2)（平方阶）、O(n^3)（立方阶） 

​		非多项式阶：随着数据规模的增长，算法的执行时间和空间占用暴增，这类算法性能极差。包括， O(2^n)（指数阶）、O(n!)（阶乘阶） 五、如何掌握好复杂度分析方法？ 复杂度分析关键在于多练，所谓孰能生巧。



#### 封底估算













## 算法策略

### 蛮力（枚举）

### 贪心

### 减而治之

​	![image-20220821175500752](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821175500752.png)

### 分而治之

![image-20220821180237994](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821180237994.png)

![image-20220821180305296](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821180305296.png)

## 迭代与递归

### 迭代分析

​		迭代算法概括而言是级数求和。



## 算法设计

### 递归

​		递归使用减而治之、分而治之的策略，应用递归跟踪、递推分析的手段来分析问题。

#### 递归跟踪

![image-20220821175906458](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821175906458.png)

![image-20220821175928237](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821175928237.png)

#### 递推分析

![image-20220821180141402](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821180141402.png)

![image-20220821180159925](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220821180159925.png)



### 迭代

#### memoization 记忆算法

​		将已计算过的实例的结果制表备查。

####  Dynamic programming 动态规划

​		将递归分析自顶而下分化求解的思路，颠倒为自底向上迭代求解。

​		从初始状态开始，依赖上一状态求解下一状态。
