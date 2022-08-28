# Overloading

---



​		**Function overloading** allows us to create multiple functions with the same name, so long as each identically named function has different parameter types (or the functions can be otherwise differentiated).

​		Each function sharing a name (in the same scope) is called an **overloaded function** (sometimes called an **overload** for short).

​		Each function sharing a name (in the same scope) is called an **overloaded function** (sometimes called an **overload** for short).

​		Function overloading provides a great way to reduce the complexity of your program by reducing the number of function names you need to remember. It can and should be used liberally.

> overload based on name mangling



## overloading deferentiate

​		区别重载的条件：

- Number of parameters

- Type of parameters

  > parameters passed by value, the const qualifier is also not considered.
  >
  > ```c++
  > void print(int);
  > void print(const int); // not differentiated from print(int)
  > ```

  - ellipsis parameters are considered to be a unique type

    ```c++
    void foo(int x, int y);
    void foo(int x, ...); // differentiated from foo(int)
    ```

- member functions

  - const or volatile qualifier
  - ref-qualifiers

>  type aliases are not distinct from overloads using the aliased type.

> return type is not used to differentiate overloaded functions. 





## overload resolution

​		The process of matching function calls to a specific overloaded function is called **overload resolution**.

​		when the argument types in the function call don’t exactly match the parameter types in any of the overloaded functions, compiler matches a given function call to a specific overloaded function via overload resolution.



### rule

​		when a function call is made to an overloaded function, the compiler steps through a sequence of rules to determine which(if any) of the overloaded functions is the best match.

​		At each step, the compiler applies a bunch of different type conversions to the argument(s) in the function call. 

​		For each conversion applied, the compiler checks if any of the overloaded functions are now a match. After all the different type conversions have been applied and checked for matches, the step is done. The result will be one of three possible outcomes:

- No matching functions were found.

  ​	The compiler moves to the next step in the sequence.

- A single matching function was found. 

  ​	This function is considered to be the best match. The matching process is now complete, and subsequent steps are not executed.

- More than one matching function was found. 

  ​	The compiler will issue an ambiguous match compile error.

​		If the compiler reaches the end of the entire sequence without finding a match, it will generate a compile error that no matching overloaded function could be found for the function call.



### matching sequence

1. 编译器尝试进行完全匹配

   1. 查找形参精确匹配的重载

   2. 对实参进行一些一般转换

      > ​		The trivial conversion are a set of specific conversion rules will modify types(without modifying the value) for purpose of finding a match.
      >
      > for example:
      >
      > ​	non-const type -> const type
      >
      > ```c++
      > void print(const int){...}
      > int x{0};
      > print(x);
      > ```
      >
      > ​	non-reference <-> reference
      >
      > ```c++
      > void print(int&){...}
      > int x{0};
      > print(x);
      > 
      > void print(int){...}
      > int x{0};
      > int& y{x};
      > print(y);
      > ```

2. 没有精确匹配，编译器查找数值提升转换后能够匹配的参数。

   > certain narrow integral and floating point types can be automatically promoted to wider types

   ```c++
   void print(int){...}
   void print(double){...}
   print('a'); // print(int)
   print(true); // print(int)
   print(4.5f); // print(double)
   ```

3. 没有数值提升匹配，编译器查找数值转换后能够匹配的参数

   ```c++
   void print(double){...}
   void print(std::string){...}
   
   print('a'); //print(double)
   ```

   > numeric prototions > numeric conversions
   
4. 没有数值转换匹配，编译器查找用户定义转换能够匹配的参数。

   > certain types (e.g. classes) can define conversions to other types that can be implicitly invoked

   ```c++
   // We haven't coverd classes yet, so don't worry if this doesn't make sense 
   class X 
   {
   public:
     operator int() {return 0;} // Here's a user-defined conversion from X to int
   };
   
   void print(int){...}
   void print(double){...}
   
   X x; // Here, we're creating an object of type X(named x)
   print(x); // x is converted to type int using the user-defined conversion from X to int
   ```

   ​	在用户定义转换之后，编译器可能会用附加的隐式提升或转换来进行匹配。

5. 没有用户定义转换匹配，编译器查找带有省略号的变参重载

6. 综上未匹配，编译器提示编译错误，查找不到匹配的函数



### ambiguous match

 An ambiguous match occurs when the compiler finds two or more functions that can be made to match in the same step. When this occurs, the compiler will stop matching and issue a compile error stating that it has found an ambiguous function call.

```c++
void print(int){...}
void print(int){...}

print(5l); // 5l is type long
```

> - literal `5l` is of type `long`,
> - the compiler will first look to see if it can find an exact match for `print(long)`
> - but it will not find one. Next, the compiler will try numeric promotion,
> - but values of type `long` can’t be promoted, so there is no match here either.
>
> Following that, the compiler will try to find a match by applying numeric conversions to the `long` argument. In the process of checking all the numeric conversion rules, the compiler will find two potential matches. If the `long` argument is numerically converted into an `int`, then the function call will match `print(int)`. If the `long` argument is instead converted into a `double`, then it will match `print(double)` instead. Since two possible matches via numeric conversion have been found, the function call is considered ambiguous.

```c++
void print(unsigned int x)
{
}
void print(float y)
{
}

int main()
{
    print(0); // int can be numerically converted to unsigned int or to float
    print(3.14159); // double can be numerically converted to unsigned int or to float

    return 0;
}
```

>  expect `0` to resolve to `print(unsigned int)` and `3.14159` to resolve to `print(float)`, both of these calls result in an ambiguous match. The `int` value `0` can be numerically converted to either an `unsigned int` or a `float`

#### resolving ambiguous matches

1. define a new overloaded function that takes parameters of exactly type.

2.  explicitly cast the ambiguous argument(s) to match the type of the function you want to call.

   ```c++
   void print(unsigned int){...}
   print(static_cast<unsigned int>(x));
   ```

3. literal argument can use the literal suffix to ensure correct type supplied.

   ```c++
   print(0u);
   ```

### **Matching for functions with multiple arguments**

If there are multiple arguments, the compiler applies the matching rules to each argument in turn.

the function chosen must provide a better match than all the other candidate functions for at least one parameter, and no worse for all of the other parameters.

If no such function can be found, the call will be considered ambiguous (or a non-match).

```c++
void print(char c, int x){...}
void print(char c, double x){...}
void print(char c, float x){...}
print('x', 'a');
```

> the top function matches the second parameter via promotion, whereas the other functions require a conversion.

