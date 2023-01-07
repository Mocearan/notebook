# 初始化

​		*Initialization* of a variable provides its initial value at the time of construction.

------------

​		When a variable is defined, you can also provide an initial value for the variable at the same time. This is called **initialization**. The value used to initialize a variable is called an **initializer**.

​		The initial value may be provided in the initializer section of a [declarator](https://en.cppreference.com/w/cpp/language/declarations) or a [new expression](https://en.cppreference.com/w/cpp/language/new). It also takes place during function calls: function parameters and the function return values are also initialized.

> 函数的形参和返回值实际上是在创建和销毁局部的函数栈时，进行创建并初始化的。它们属于函数的局部变量

​		默认情况下，无论是normal variable还是member variable都不会初始化。

```c++
int a;

struct Employee {
    int id; // note: no initializer here
    int age; 
    double wage;
}
Employee e;
```

​		如果一个对象没有初始化化，其行为将是未定义的。

​		Initialize your variables upon creation. 

> ​		You may eventually find cases where you want to ignore this advice for a specific reason,and that’s okay, as long the choice is made deliberately.
>
> > e.g. a performance critical section of code that uses a lot of variables, 
>
> [CppCoreGuidelines/CppCoreGuidelines.md at master · isocpp/CppCoreGuidelines (github.com)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es20-always-initialize-an-object)

​		不存在``multiple variables initialization``，初始化都是针对单个目标的（对象、引用或聚合体）或多个同质目标。只存在``multiple definition``:

```c++
int a = 5, b = 6; // copy initialization
int c( 7 ), d( 8 ); // direct initialization
int e { 9 }, f { 10 }; // brace initialization (preferred)

int a[] = {1, 2, 3}; // aggregate initialization
```

​		

## 初始化和未初始化

​		When a variable is defined, you can also provide an initial value for the variable at the same time. This is called **initialization**. 

​		when a variable is assigned a memory location by the compiler, the default value of that variable is whatever (garbage) value happens to already be in that memory location! A variable that has not been given a known value (usually through initialization or assignment) is called an **uninitialized variable**.

​		Initialization means the object was provided with an initial value at the point of definition. Uninitialized means the object has not been given a known value (through any means, including assignment).

> ​		Therefore, an object that is not initialized but is then assigned a value is no longer *uninitialized* (because it has been given a known value).

To recap:

- Initialization = The object is given a known value at the point of definition.
- Assignment = The object is given a known value beyond the point of definition.
- Uninitialized = The object has not been given a known value yet.

> This lack of initialization is a performance optimization inherited from C, back when computers were slow. 
>
> If C++ initialized all of those variables with default values upon creation, this would result in 100,000 initializations (which would be slow), and for little benefit (since you’re overwriting those values anyway).
>
> For now, you should always initialize your variables because the cost of doing so is miniscule compared to the benefit. **Once you are more comfortable with the language, there may be certain cases where you omit the initialization for optimization purposes. But this should always be done selectively and intentionally.**

​		Some compilers, such as Visual Studio, *will* initialize the contents of memory to some preset value when you’re using a debug build configuration. 

> If your compiler won’t let you compile and run the above program（use uninitialized variable） for this reason, here is a possible solution to get around this issue:
>
> ```c++
> #include <iostream>
> 
> void doNothing(int&) // Don't worry about what & is for now, we're just using it to trick the compiler into thinking variable x is used
> {
> }
> 
> int main()
> {
>     // define an integer variable named x
>     int x; // this variable is uninitialized
> 
>     doNothing(x); // make the compiler think we're assigning a value to this variable
> 
>     // print the value of x to the screen (who knows what we'll get, because x is uninitialized)
>     std::cout << x;
> 
>     return 0;
> }
> ```
>



## 初始化器形制

​		For each declarator, the initializer may be one of the following:

```c++
                          (0)	// no initializer, like ： int a;
( expression-list )		 (1)    // 直接初始化，comma-separated list of arbitrary expressions and braced-init-lists in parentheses
= expression			(2)	   // 拷贝初始化，the equals sign followed by an expression
{ initializer-list }	 (3)    // 统一初始化，braced-init-list: possibly empty, comma-separated list of expressions and other braced-init-lists
```

simplified example:

```c
int a;        	// no initializer
int b = 5; 		// initializer after equals sign
int c( 6 ); 	// initializer in parenthesis
int d { 7 }; 	// initializer in braces  
```

> 这是c++中常用的四种初始化形态，但它们之间的差异不仅限于形式上的不同，不同的初始化形态具有不同的规则。



## 对象初始化 Object Initialization

​		Depending on context, the initializer may invoke deferent initialization type。

​		一般谈论的初始化，都是指对象初始化

```c++
class Fraction
{
private:
    int m_numerator {};
    int m_denominator {};

public:
    Fraction() // default constructor
    {
         m_numerator = 0;
         m_denominator = 1;
    }

    // Constructor with two parameters, one parameter having a default value
    Fraction(int numerator, int denominator=1)
    {
        assert(denominator != 0);
        m_numerator = numerator;
        m_denominator = denominator;
    }

    int getNumerator() { return m_numerator; }
    int getDenominator() { return m_denominator; }
    double getValue() { return static_cast<double>(m_numerator) / m_denominator; }
};

```



### Default initialization

​		If no initializer is provided, the rules of [default initialization](https://en.cppreference.com/w/cpp/language/default_initialization) apply.

> In most cases, default initialization leaves a variable with an indeterminate value.

```c++
T object ;	(1)			// automatic, static, or thread-local storage duration, declared with no initializer;


new T		(2)			// dynamic storage duration, created by a new-expression with no initializer;
new T ( ) 	( before C++03 )  // (3) constructor is called, but base class or a non-static data member is not mentioned in a constructor initializer list 
```

```c++
Fraction frac; // Default-initialization, calls default constructor
```

> ```cpp
> Fraction frac {}; // Value initialization using empty set of braces
> ```

- structs and classes can self-initialize even when no explicit initializers are provided!

  ```c++
  struct Something
  {
      int x;       // no initialization value (bad)
      int y {};    // value-initialized by default
      int z { 2 }; // explicit default value
  };
  Something s1; // s1.x is uninitialized, s1.y is 0, and s1.z is 2
  ```

  - 显式的初始化值优先级高于默认初始化值。

    ```c++
    Something s2 { 5, 6, 7 }; // use explicit initializers for s2.x, s2.y, and s2.z (no default values are used)
    ```

  - 当提供聚合初始化器时，缺省值会被默认初始化

    ```c++
    Something s3 {}; // value initialize s3.x, use default values for s3.y and s3.z
    ```

    > `s3.x` also not initialized.

  

  



### Direct initialization

​		Initializes an object from explicit set of constructor arguments.

​		When an initializer is provided inside parenthesis / brace, this is called **direct initialization**.

​		直接初始化是相对拷贝初始化而言的，形式上是不需要等号参与到初始化的语法表达上。

```c++
T object ( arg );
T object ( arg1, arg2, ... ); 	//(1)  nonempty parenthesized list of expressions / braced-init-lists(带括号的初始化列表)

T object { arg };				//(2) (since C++11) non-class type with a single brace-enclosed initializer 
			// (note: for class types and other uses of braced-init-list, see list-initialization)

T ( other )
T ( arg1, arg2, ... )			//(3) //a prvalue temporary (until C++17)
    								//the result object of a prvalue (since C++17) 
    							//by functional cast / with a parenthesized expression list
    
static_cast< T >( other )		//(4) //a prvalue temporary (until C++17)
    								//the result object of a prvalue (since C++17) 
    							//by a static_cast expression
    
new T(args, ...)				//(5) with dynamic storage duration by a new-expression, with a non-empty initializer
    
Class::Class() : member(args, ...) { ... }	//(6) a base / a non-static member, by constructor initializer list

[arg](){ ... }					//(7) (since C++11) closure object members from the variables, caught by copy in a lambda-expression
```

```c++
Fraction threeQuarters(3, 4); // Direct initialization, also calls Fraction(int, int)
```

> ```cpp
> Fraction fiveThirds{ 5, 3 }; // List initialization, calls Fraction(int, int)
> ```

> 
>
> To provide a more consistent initialization mechanism, there’s **brace initialization** (also called **uniform initialization** or **list initialization**) that uses curly braces.
>
> 传统的直接初始化是指小括号初始化，用以初始化单个对象，不能用以进行聚合体的初始化；大括号直接初始化用作列表初始化。为了统一语法，支持了大括号初始化单个对象。
>
> Direct and copy brace initialization function almost identically, but the direct form is generally preferred.
>
> > ```c++
> > int width { 5 }; // direct brace initialization of value 5 into variable width (preferred)
> > int height = { 6 }; // copy brace initialization of value 6 into variable height
> > ```
>
> Brace initialization has the added benefit of disallowing “narrowing” conversions. 
>
> ```c++
> int width {4.5}; //error: not all double values fit into an int
> ```
>
> > Copy and direct initialization would drop the fractional part, resulting in initialization of value 4 into variable *width*. However, with brace initialization, this will cause the compiler to issue an error.
> >
> > Conversions that can be done without potential data loss are allowed.
>
> **Favor initialization using braces whenever possible.**



​		C++ does not support a direct or brace assignment syntax, such like:

```c++
int a;
a(3); // error
a{3}; // error
```

​		

​		A rule of thumb is, if the type is some kind of list and you don’t want to initialize it with a list, use direct initialization.



### Copy initialization

​		Initializes an object from another object.

​		Copy initialization is performed in the following situations:

​		拷贝初始化的语法形式从C语言继承而来，以等号赋值的形式来引起初始化。

```c++
T object = other;	//(1)	when a named variable (automatic, static, or thread-local) of a non-reference type T is declared with the initializer consisting of an equals sign followed by an expression.

T object = {other};	//(2)	(until C++11)when a named variable of a scalar type T is declared with the initializer consisting of an equals sign followed by a brace-enclosed expression (Note: as of C++11, this is classified as list initialization, and narrowing conversion is not allowed).

f(other)	//(3)	when passing an argument to a function by value

return other;	//(4)	 when returning from a function that returns by value

throw object;
catch (T object) //(5)	when throwing or catching an exception by value
T array[N] = {other-sequence};	(6)	as part of aggregate initialization, to initialize each element for which an initializer is provided
```

> For simple data types (like int), copy and direct initialization are essentially the same. For more complicated types, direct initialization tends to be more efficient than copy initialization.

```c++
Fraction six = Fraction{ 6 }; // Copy initialize a Fraction, will call Fraction(6, 1)
Fraction seven = 7; // Copy initialize a Fraction.  The compiler will try to find a way to convert 7 to a Fraction, which will invoke the Fraction(7, 1) constructor.
```

​		 avoid this form of initialization with classes, as it may be less efficient. Although direct-initialization, list-initialization, and copy-initialization all work identically with fundamental types, copy-initialization does not work the same with classes (though the end-result is often the same). 

> ​		This form of copy initialization is evaluated the same way as the following:
>
> ```cpp
> Fraction six(Fraction(6));
> ```
>
> ​		this can potentially make calls to both Fraction(int, int) and the Fraction copy constructor (which may be elided for performance reasons). However, because eliding isn’t guaranteed (prior to C++17, where elision in this particular case is now mandatory), it’s better to avoid copy initialization for classes, and use uniform initialization instead.

```c++
#include <iostream>
class Something
{
public:
	Something() = default;
	Something(const Something&)
	{
		std::cout << "Copy constructor called\n";
	}
};

Something foo()
{
	return Something(); // copy constructor normally called here
}
Something goo()
{
	Something s;
	return s; // copy constructor normally called here
}

int main()
{
	std::cout << "Initializing s1\n";
	Something s1 = foo(); // copy constructor normally called here

	std::cout << "Initializing s2\n";
	Something s2 = goo(); // copy constructor normally called here
}
```

> ​		The above program would normally call the copy constructor 4 times -- however, due to copy elision, it’s likely that your compiler will elide most or all of the cases. Visual Studio 2019 elides 3 (it doesn’t elide the case where goo() is returned), and GCC elides all 4.



### list-initialization  / uniform initialization

​		列表初始化实际上指使用参数列表进行初始化。

​		聚合体实际作为单一目标，若干数据项实际上看做多个“参数“。

​		这点与聚合初始化是不一样的，聚合初始化实际上是在一个初始化语句中，对多个同质的目标进行初始化，如结构体、联合体、数组等。

> As always, we prefer list initialization than direct initialization.



#### Direct-list-initialization

​		both explicit and non-explicit constructors are considered.

​		以初始化列表的形式进行直接初始化。

```c++
T object { arg1, arg2, ... };	//(1) initialization of a named variable with a braced-init-list (that is, a possibly empty brace-enclosed list of expressions or nested braced-init-lists)
T { arg1, arg2, ... }			//(2)  initialization of an unnamed temporary with a braced-init-list
new T { arg1, arg2, ... }		//(3) initialization of an object with dynamic storage duration with a new-expression, where the initializer is a braced-init-list
Class { T member { arg1, arg2, ... }; };	//(4) in a non-static data member initializer that does not use the equals sign
Class::Class() : member{arg1, arg2, ...} {...	//(5) in a member initializer list of a constructor if braced-init-list is used
```

```c++
Fraction fiveThirds{ 5, 3 }; // List initialization, calls Fraction(int, int)
```



#### Copy-list-initialization

​		both explicit and non-explicit constructors are considered, but only non-explicit constructors may be called

```c++
T object = {arg1, arg2, ...};	//(6)	initialization of a named variable with a braced-init-list after an equals sign
function( { arg1, arg2, ... } )	//(7)	in a function call expression, with braced-init-list used as an argument and list-initialization initializes the function parameter
return { arg1, arg2, ... } ;	//(8)	in a return statement with braced-init-list used as the return expression and list-initialization initializes the returned object
object[ { arg1, arg2, ... } ]	//(9)	in a subscript expression with a user-defined operator[], where list-initialization initializes the parameter of the overloaded operator
object = { arg1, arg2, ... }	//(10)	 in an assignment expression, where list-initialization initializes the parameter of the overloaded operator
U( { arg1, arg2, ... } )		//(11)	functional cast expression or other constructor invocations, where braced-init-list is used in place of a constructor argument. Copy-list-initialization initializes the constructor's parameter (note; the type U in this example is not the type that's being list-initialized; U's constructor's parameter is)
Class { T member = { arg1, arg2, ... }; };	(12)	 in a non-static data member initializer that uses the equals sign
```

```c++
Fraction seven = {7};
Fraction seven({7});
```





## 初始化语义

​		初始化语义是从`c++`语义学的角度来对初始化的形式进行分类。

### object initialization

​		single object initialization like above。

### value initialization

​		This is the initialization performed when an object is constructed with an empty initializer.

> ​	值初始化通常用以初始化临时的值语义对象。

```c++
T()	//(1) nameless temporary object , initializer consisting of an empty pair of parentheses 
T{}	//(5) (since C++11) or braces 

new T ()	//(2) dynamic storage duration, new-expression, initializer consisting of an empty pair of parentheses 
new T {}	//(6) (since C++11)  or braces 

Class::Class(...) : member() { ... }	//(3)  non-static data member / a base class, member initializer with an empty pair of parentheses 
Class::Class(...) : member{} { ... }	//(7) (since C++11) or braces 

T object {};	//(4) (since C++11) named object (automatic, static, or thread-local) is declared with the initializer consisting of a pair of braces.
```

> In most cases, **value initialization** will initialize the variable to zero (or empty, if that’s more appropriate for a given type). In such cases where zeroing occurs, this is called **zero initialization**.
>
> > **When should I initialize with { 0 } vs {}?**
> >
> > Use an explicit initialization value if you’re actually using that value.
> >
> > ```cpp
> > int x { 0 }; // explicit initialization to value 0
> > std::cout << x; // we're using that zero value
> > ```
> >
> > Use value initialization if the value is temporary and will be replaced.
> >
> > ```cpp
> > int x {}; // value initialization
> > std::cin >> x; // we're immediately replacing that value
> > ```
>
> 通常值初始化的使用类似如下：
>
> ```c++
> std::cout << double() << std::endl; // 即在一条语句中创建一个临时的值语义的对象做临时之用。
> ```

​		一般通过是否带有括号来区别值初始化和默认初始化：

```c++
Fraction frac {}; // Value initialization using empty set of braces
Fraction frac; // Default-initialization, calls default constructor
```

​		大多数情况下，值初始化和默认初始化会导致同样的结果，即默认构造函数被调用。

​		不同的一点在于，在使用值初始化时，编译器也许会在调用构造函数之前对成员进行零初始化，这将会造成一定的性能损耗。但是当成员没有默认的被任何初始化器初始化，那么默认初始化因为没有进行零初始化可能会造成错误。

```c++
class Fraction
{
private:
    // Removed initializers
    int m_numerator;
    int m_denominator;

public:
    // Removed default-constructor

    int getNumerator() { return m_numerator; }
    int getDenominator() { return m_denominator; }
    double getValue() { return static_cast<double>(m_numerator) / m_denominator; }
};

Fraction frac;
    // frac is uninitialized, accessing its members causes undefined behavior
    std::cout << frac.getNumerator() << '/' << frac.getDenominator() << '\n';
```

​		因此，在初始化类对象时，使用值初始化更加简单和安全。

### reference initialization

​		引用初始化是对象初始化的引用形式，即以引用的方式创建对象。

​		Binds a reference to an object.

​		A reference to `T` can be initialized with an object of type `T`, a function of type `T`, or an object implicitly convertible to `T`. 

​		Once initialized, a reference cannot be changed to refer to another object.

```c++
T & ref = target ;
T & ref = { arg1, arg2, ... };
T & ref ( target );
T & ref { arg1, arg2, ... }; 
// (1)	When a named lvalue reference variable is declared with an initializer

T && ref = target ;
T && ref = { arg1, arg2, ... };
T && ref ( target );
T && ref { arg1, arg2, ... };
// (2)	(since C++11) When a named rvalue reference variable is declared with an initializer

//given R fn ( T & arg ); / R fn ( T && arg ); (since C++11)
fn ( target )
fn ({ arg1, arg2, ... });
// (3) In a function call expression, when the function parameter has reference type

//inside T & fn () / T && fn () (since C++11)
return target ;
// (4)	In the return statement, when the function returns a reference type

//given T & ref ; / T && ref ; (since C++11)   inside the definition of Class
Class::Class(...) : ref ( target ) { ... }
// (5)	When a non-static data member of reference type is initialized using a member initializer
```



### aggregate initialization 聚合初始化

#### aggregate data type 聚合类型

​		在普遍的编程语言中，聚合类型（aggregate data type） 是任意可以包含多个数据成员的类型。

​		在c++ 中聚合类型必须满足几个标准：

- array type（`raw array` or `std::array`）
- class type (`struct / class / union`), that has
  - no private or protected direct (since C++17) non-static data members
  - no user-declared constructors(until C++11)
  - no user-provided constructors (explicitly defaulted or deleted constructors are allowed)(since C++11)(until C++17)
  - no user-provided, inherited, or explicit constructors (explicitly defaulted or deleted constructors are allowed)(since C++17)(until C++20)
  - no user-declared or inherited constructors(since C++20)
  - no virtual, private, or protected (since C++17) base classes
  - no virtual member functions
  - no default member initializers(since C++11)(until C++14)



#### aggregate initialization

​		**aggregate initialization**, which allows us to directly initialize the members of aggregates.

> ​		If an aggregate is defined with an initialization list:
>
> - If an explicit initialization value exists, that explicit value is used.
> - If an initializer is missing and a default member initializer exists, the default is used.
> - If an initializer is missing and no default member initializer exists, value initialization occurs.
>
> ​		If an aggregate is defined with no initialization list:
>
> - If a default member initializer exists, the default is used.
> - If no default member initializer exists, the member remains uninitialized.



-  通过`braced-init-list`初始化聚合类型

  >  **initializer list** as an initializer, which is just a list of comma-separated initialization values.

  ```c++
  T object {arg1, arg2, ... };	//(2)	(since C++11)
  T object = { .designator = arg1 , .designator { arg2 } ... };	//(3)	(since C++20)
  T object { .designator = arg1 , .designator { arg2 } ... };	//(4)	(since C++20)
  T object (arg1, arg2, ...);	//(5)	(since C++20)
  ```

  Aggregate initialization initializes *aggregates*. It is a form of [list-initialization](https://en.cppreference.com/w/cpp/language/list_initialization) (since C++11) or [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (since C++20).

  ```c++
  struct Employee {
      int id{};
      int age{};
      double wage {};
  }
  
  Employee frank = {1, 32, 3.44}; 	// copy-list initaialization using braced list (traditionally)
  Employee robert ( 3, 45, 62500.0 );  // direct initialization using parenthesized list (C++20)
  Employee joe { 2, 28, 45000.0 };     // list initialization using braced list (preferred)
  ```

  - 在`brace-init-list`中缺省的成员将进行`value-initialization`,被初始化为0.

    ```c++
    Employee joe { 2, 28 }; // joe.wage will be value-initialized to 0.0
    Employee joe {}; // value-initialize all members
    ```

  - 在`brace-init-list`中可以进行指定初始化`designated initialization`

    ​	 Designated initializers allow you to explicitly define which initialization values map to which members. The members must be initialized in the same order in which they are declared in the struct, otherwise an error will result. Members not designated an initializer will be value initialized.

    ```c++
    Foo f1{ .a{ 1 }, .c{ 3 } }; // ok: f.a = 1, f.b = 0 (value initialized), f.c = 3
    Foo f2{ .b{ 2 }, .a{ 1 } }; // error: initialization order does not match order of declaration in struct
    ```

    ​		因为指派初始化的有序性，因此，不要在添加新成员时，应尽量在定义的最后添加以保证有序性。

- 聚合类型可以使用`brace-init-list`进行赋值

  ```c++
  Employee joe { 1, 32, 60000.0 };
  joe = { joe.id, 33, 66000.0 }; // Joe had a birthday and got a raise
  // we needed to provide the current value for joe.id in our list as a placeholder
  ```

  - 在使用`brace-init-list`进行赋值时同样可以指派成员

    ```c++
    Employee joe { 1, 32, 60000.0 };
    joe = { .id = joe.id, .age = 33, .wage = 66000.0 }; // Joe had a birthday and got a raise
    ```

    > 本质上是创建一个`Employee`临时对象，然后用临时对象对原对象进行拷贝赋值，依赖类类型的拷贝赋值函数。

    

​		建议为所有的类类型成员提供默认值，保证即使初始化器列表有缺省时成员也都会被初始化。

​		在没有显式的初始化器列表能够提供时，更倾向于使用值初始化`{}`（空`brace-init-list`），而不是进行默认初始化。

















