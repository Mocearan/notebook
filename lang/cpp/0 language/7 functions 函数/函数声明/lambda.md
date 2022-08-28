# lambda

---

​		构造闭包。

> 闭包：
>
> ​		无名函数对象，能够捕获作用域中的变量。
>
> https://en.wikipedia.org/wiki/Closure_(computer_science)



## 语法

​		`[ captures ] <tparams> requires ( params ) specs requires requires(optional) { body }`

> `params`为空时，可以省略`()`
>
> `params`中的参数类型可以使用`auto`进行推导
>
> ```c++
> timer.async_wait([this](const auto&){this->print();});
> ```
>
> 



## 捕获

​		逗号分隔的捕获符列表。

### 捕获方式

- `&`， 以引用方式捕获

  ​	引用捕获不会延长实体的生命周期。

  ​	如果闭包外的生命周期在闭包调用前结束，闭包中对该实体的引用成为悬垂引用，行为未定义。

- `=`， 以拷贝的方式捕获

  ​	可以缺省`=`，以只有标识符的方式表明拷贝捕获标识符。

  ​	拷贝捕获也绝非安全，比如指针的间接隐患。
  
  > ​		在lambda表达式中，通常以捕获`std::shared_ptr`的方式来延长生命周期。
  

#### 捕获指针

**指针的间接隐患**

​		如果拷贝的是指针，存在浅拷贝的问题。闭包中该指针引用的实体的生命周期仍不能保证。如果拷贝的对象中以成员指针的方式构造对象图，仍存在浅拷贝问题，成员的深拷贝依赖于该对象的拷贝构造。

> ​		这些应该整理到 指针 小节去，诸如此类的问题都引用指针小节的名词**间接隐患**

- 一般指针
- 对象指针

​		对于类内部的使用到类成员的lambda，通常先`shared_from_this`来获取``this``的`std::shared_ptr`，然后捕获该`std::shared_ptr`。

- 对于lambda来说，`std::shared_ptr`不能建立成员和对象之间的联系，所以还需要将this也捕获。

```c++
A::func(...)
{
 auto keep = shared_from_this();
 ... [this, keep] (...) {...} //此时不能使用&keep来引用绑定，因为keep本身将在绑定后在此作用域失效
}

// 如果确认作为保持对象生命周期的`std::shared_ptr`在被捕获后，在此上下文中不再有意义，则可以移动
[keep = std::move(keep)](...){...}

// 如果可以保证this在 该lambda被调用时，生命周期还没结束。则可以想一般指针的捕获一样。
[this](...){}
```

- 也可以不捕获this，但需要使用捕获的`std::shared_ptr`来调用相关的成员函数

  ```c++
  A::func(...)
  {
   auto keep = shared_from_this();
   ... [keep] (...) {keep->memfunc();...}
  }
  
  ```

  

### 默认捕获符

​		`&`和`=`作为捕获符列表的起始，其后不跟具体的标识符，以其语义相应的方式隐式捕获被使用的局部自动变量。



## lambda的本质

​		lambda本质上是一个函数对象的语法糖。lambda会被编译器翻译成一个带有`operator()`重载的类。

- `[](){}`

  ```c++
  auto lambda = [] () { }; // 1
  std::cout << sizeof lambda << std::endl; // 1
  
  class Lambda
  {
      public:
      Lambda(/* args */) {}
      ~Lambda() { }
  
      void operator()() const {}
  };
  
  Lambda  lamd;
  std::cout << sizeof lamd << std::endl; // 1
  ```

- `[](params){}`

  ```c++
  auto lambda = [] ( int a ) { };
  std::cout << sizeof lambda << std::endl; // 1
  
  class Lambda
  {
      public:
      Lambda() {}
      ~Lambda() { }
  
      void operator()( int a ) { }
  };
  Lambda lamb;
  std::cout << sizeof lamb << std::endl; // 1
  ```

- `[captrues](params){}`

  ```c++
  double b = 2.2;
  int c = 3;
  int d = 4;
  auto lambda = [b, &c, &d] ( int a ) { };
  std::cout << sizeof lambda << std::endl; // 24
  
  class Lambda
  {
      private:
      double b_;
      int &c_;
      int &d_;
      public:
      Lambda(double b, int c, int d): b_{b}, c_{c}, d_{d} { }
      ~Lambda() { }
  
      void operator()( int a ) { }
  };
  Lambda lamb(b, c, d);
  std::cout << sizeof lamb << std::endl; // 24
  ```

  

​		

