# 模板机制

​		构建通用的基础的抽象。

---

​		好的模板需要一些支持性的语言设施：

- 依赖于类型的值： `variable templates `变量模板
- 类型和模板的别名：``alias templates`` 别名模板
- 编译时选择机制：``if constexpr``
- 用于查询类型和表达式属性的编译时机制：``requires-expressions``

​		此外，``constexpr``函数和``static_asserts``经常参与模板设计和使用。



## Variable Templates

​		使用一个类型时，往往需要的是这个类型的常量和值。

​		在使用类模板时也是这样，定义`C<T>`时，也往往需要的是`C<T>`和依赖于`T`的类型的常量和变量。

> ​		大多数变量模板都是常，许多变量也是常量。术语没有跟上不可变性的概念。

```c++
template <class T>
constexpr T viscosity = 0.4;

template <class T>
constexpr space_vector<T> external_acceleration = { T{}, T{-9.8}, T{} };

auto vis2 = 2*viscosity<double>;
auto acc = external_acceleration<float>;
```

​		可以使用适当类型的任意表达式作为初始化器：

```c++
// 如果 T 不满足可赋值条件，则会在编译时触发 static_assert 断言失败，编译会停止并输出错误信息。
template<typename T, typename T2>
constexpr bool Assignable = is_assignable<T&,T2>::value;   
// 其中 is_assignable 是一个类型特征，用于判断一个类型是否可以被赋值为另一个类型。它的定义在标准库头文件 <type_traits> 中，具体实现可以参考该头文件中的源代码。

// 定义了一个模板函数 testing，该函数接受一个类型参数 T
template<typename T>
void testing()
{
    // // 使用 is_assignable 类型特征判断 T 是否可以被赋值为 double 和 string 类型。
	static_assert(Assignable<T&,double>, "can't assign a double to a T");
	static_assert(Assignable<T&,string>, "can't assign a string to a T");
}
```

>  经过一些大改造，这个想法成为概念定义的核心。

​		标准库使用变量模板提供数学常量，例如pi和log2e



## Aliases

​		为类型或模板引入同义词通常是有用的

- 与实现相关的类型，通过别名便于书写可移植性强的代码

  ```c++
  using size_t = unsigned int;
  ```

- 模板编程中能够通过别名来写书遵循某一约定的强一致性代码

  ```c++
  template<typename T>
  class Vector {
  public:
          using value_type = T;
          // ...
  };
  
  template<typename C>
  using Value_type = C::value_type;         // C的元素类型
  
  template<typename Container>
  void algo(Container& c)
  {
          Vector<Value_type<Container>> vec;                 // 在此处保留结果
          // ...
  }
  ```

- 通过别名绑定一些或所有模板参数，定义新的模板

  ```c++
  template<typename Key, typename Value>
  class Map {
          // ...
  };
  
  template<typename Value>
  using String_map = Map<string,Value>;
  
  String_map<int> m;         // m是一个Map<string,int>
  ```

  ​		这不同于模板特化，模板特化针对于某种类型，有特定的独立实现。而别名绑定是简化针对某种常用类型的使用。



## Compile-Time if

​		在模板元编程中，可能在编译时就需要针对某些条件来进行抉择。这样的问题在基础代码中很常见，其中通用性和最佳性能至关重要。

> ​		考虑编写一个操作，该操作可以使用``slow_and_safe(T)``或``simple_and_fast(T)``中的一个函数来实现。
>
> ​		如果涉及类层次结构，则基类可以提供``slow_and_safe``通用操作，派生类可以使用``simple_and_fast``实现进行覆盖。

​		使用`if constexpr`能够简约的实现：

```c++
template<typename T>
void update(T& target)
{
        // ...
    // is_trivially_copyable_v<T>是一个类型谓词（§16.4.1），它告诉我们一个类型是否可以被平凡地复制。
        if constexpr(is_trivially_copyable_v<T>)
                simple_and_fast(target);          // for "plain old data"
        else
                slow_and_safe(target);            // for more complex types
        // ...
}
```

- 编译器只检查``if constexpr``的选定分支，这样的特性提供了最佳性能和优化的局部性

- if constexpr不是文本操作机制，不能用于打破语法、类型和作用域的常规规则

  ```c++
  template<typename T>
  void bad(T arg)
  {
  	if constexpr(!is_trivially_copyable_v<T>)
  		try {  // Oops, the if extends beyond this line
  	g(arg);
      if constexpr(!is_trivially_copyable_v<T>)
          } catch(...) { /* ... */ }   // syntax error
  }
  ```

  ​	允许这样的文本操作可能会严重影响代码的可读性，并为依赖现代程序表示技术（如“抽象语法树”）的工具造成问题。

  ​		进行这样的尝试也是不必要的，因为可以使用不违反作用域规则的更清晰的解决方案：

  ```c++
  template<typename T>
  void good(T arg)
  {
  	if constexpr (is_trivially_copyable_v<T>)
  		g(arg);
  	else
  		try {
  			g(arg);
  		}
  		catch (...) { /* ... */ }
  }
  ```

  



