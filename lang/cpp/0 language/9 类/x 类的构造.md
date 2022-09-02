# 类的构造

---

​		类的创建经由构造函数和成员初始化器列表来实现。

​		构造函数，是类的一种特殊的非静态[成员函数](https://zh.cppreference.com/w/cpp/language/member_functions)，用于初始化该类类型的对象。

​		成员初始化器列表，构造函数定义中指定各个直接基类、虚基类和非静态数据成员的初始化器。 

> 不同于 [std::initializer_list](https://zh.cppreference.com/w/cpp/utility/initializer_list)。

> **两阶段构造**：
>
> ​		一般只在构造函数中完成各个对象的创建，而不做任何功能性的事件，以保证类的内存模型的成功创建。
>
> https://www.cnblogs.com/AKUN-FYK/p/11062935.html
>
> > ​		如果希望不调用初始化函数来完成启动功能，可以使用工程类或工厂方法来创建对象，调用完初始化功能后再返回对象。



## 成员初始化器列表



### 类内初始化

​		成员变量较多时，成员初始化器列表就会很冗长，也比较容器产生遗漏。

​		可以在类里声明变量的同时给它赋值，实现初始化。和在成员初始化器列表中初始化是一样的。

​		不同的是，不同构造函数不同初始化的成员不能类内初始化，类内初始化也不能依赖构造函数的参数。

```c++

class DemoInit final                  // 有很多成员变量的类
{
private:
    int                 a = 0;        // 整数成员，赋值初始化
    string              s = "hello";  // 字符串成员，赋值初始化
    vector<int>         v{1, 2, 3};   // 容器成员，使用花括号的初始化列表
public:
    DemoInit() = default;             // 默认构造函数
   ~DemoInit() = default;             // 默认析构函数
public:
    DemoInit(int x) : a(x) {}         // 可以单独初始化成员，其他用默认值
};

```





## 委托构造

​		delegating constructor.

​		多个不同形式的构造函数之间不能复用成员初始化的代码。传统做法是抽取init() 函数里，构造函数去调用。

​		效率和可读性较差，init() 不是真正的构造函数，实际上是未定式和值的分离，不是真正的初始化。

​		构造函数可以直接调用另一个构造函数，把构造工作“委托”出去，简单高效。

```c++

class DemoDelegating final
{
private:
    int a;                              // 成员变量
public:
    DemoDelegating(int x) : a(x)        // 基本的构造函数
    {}  

    DemoDelegating() :                 // 无参数的构造函数
        DemoDelegating(0)               // 给出默认值，委托给第一个构造函数
    {}  

    DemoDelegating(const string& s) : // 字符串参数构造函数
        DemoDelegating(stoi(s))        // 转换成整数，再委托给第一个构造函数
    {}  
};
```





##  默认构造



## 拷贝构造

​		使得类具有在创建时的拷贝语义，即可以通过拷贝同类另一个对象值的方式来创建一个新对象。

```c++
Name ( const Name & );
```

​		可以显式的弃置拷贝构造，来去除类通过拷贝创建的语义，使本类及其聚类不能进行拷贝初始化。

```c++
Name ( const Name & ) = delete;		// C++11
```

​		如不显式的给出定义，则默认的对所有成员进行拷贝构造来完成新对象的创建。如果有成员去除了拷贝创建语义，则因成员不能完全创建，造成本类不能完成拷贝创建。

```c++
Name ( const Name & ) = default;	// c++11
```





​		