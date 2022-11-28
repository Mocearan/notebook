# exception

---

​		

## why exception

​		异常是对通过错误号（error code）返回错误信息机制的改进。使用错误号进行错误处理相当简单，但与此同时有一些缺点。

- 返回值混淆

  ​	函数返回值将错误代码与实际有效的返回值混淆。
  ```c++
  int findFirstChar(std::string_view string, char ch)
  {
      // Step through each character in string
      for(std::size_t index{0};  index < string.length(); ++index) {
      	// If the charator matches ch, return index
          if(string[index] == ch)
              return index;
      }
      // If no match was found, return -1
      return -1;
  }
  ```

  - 可以通过`std::optional`，来判断是否有值

- 返回值与错误代码类型不兼容

  ​		函数只能有一个返回值，当需要返回的有效值和错误代码类型不兼容，就变得有些复杂。

  - 通过传出参数返回有效值，返回值传递错误代码

    ```c++
    bool devide(int x, int y, double& val)
    {
        if(y == 0) {
            val = 0.0;
            return false;
        }
        
        val = static_cast<double>(x) / y;
        return true;
    }
    ```

  - 通过结构化返回值，将多个返回值信息装进一个结构作为返回类型

    - `std::tuple`
    - struct

- 繁杂的错误序列

  ​	在很多东西都可能出错的调用中，需要频繁的检查错误。

  ```c++
  std::ifstream setupIni { "setup.ini" }; // open setup.ini for reading
  // If the file couldn't be opened (e.g. because it was missing) return some error enum
  if (!setupIni)
      return ERROR_OPENING_FILE;
  
  // Now read a bunch of values from a file
  if (!readIntegerFromFile(setupIni, m_firstParameter)) // try to read an integer from the file
      return ERROR_READING_VALUE; // Return enum value indicating value couldn't be read
  
  if (!readDoubleFromFile(setupIni, m_secondParameter)) // try to read a double from the file
      return ERROR_READING_VALUE;
  
  if (!readFloatFromFile(setupIni, m_thirdParameter)) // try to read a float from the file
      return ERROR_READING_VALUE;
  ```

- 错误码不能和构造函数很好的配合

  ​		因为构造函数没有返回值，来向调用者返回错误信息，通过传出参数则污染了对象构造的参数列表。

  ​		同时，不能主动的中断构造函数的，即使逻辑上的创建失败，对象仍然会被创建。而后需要主动的清理对象。

- 向调用方返回错误码，错误没有被强制要求处理

  ​	调用方可能不会处理错误，忽略该错误。导致该错误在堆栈上丢失。



​		简单来说，返回错误码的主要问题是错误处理代码最终与代码的正常控制流复杂地耦合在一起。

​		这反过来又限制了代码的布局方式和错误的合理处理方式。

​		异常处理提供了一种将错误或其他异常情况处理从代码的控制流中分离的机制。允许自由的处理错误。减轻返回代码造成的混乱。



## exception

- `throw` 用处产生一个异常或错误

  ```c++
  throw -1; // throw a literal integer value
  throw ENUM_INVALID_INDEX; // throw an enum value
  throw "Can not take square root of negative number"; // throw a literal C-style(const char*) string
  throw dx; // throw a double varable that was previously defined.
  throw MyException("fatal error"); // throw an object inherited by Exception.
  ```

  

- `try-catch` 用于捕获和处理异常

  ​	`try`块以观察者的方式，查找任何在`try`块中抛出的异常。

  ```c++
  try {
      // Statements that may throw exceptions you want to handle go here
      throw -1; // here's a trivial throw statement
  }
  ```

  ​	`catch`块用于处理单一类型异常

  ```c++
  catch(int x) {
      // handle an exception of type int here
      std::cerr << "We caught an int exception with value " << x << std::endl;
  }
  ```

  ​		一个`try`块后面至少拥有一个`catch`块，可以拥有多个顺序排列的`catch`块。

  ​		被`try`块捕获并路由到`catch`块中，视为异常已经处理，即便它是空的。并且在`catch`块之后将恢复正常执行流。

  ​		`catch`块通常会做三件事：

  - print error (console / log)
  - return value or error code to caller
  - throw another exception

  

  ​		Exceptions of fundamental types can be caught by value, but exceptions of non-fundamental types should be caught by const reference to avoid making an unnecessary copy (and, in some cases, to prevent slicing).

  ```c++
  catch (double) // note: no variable name since we don't use it in the catch block below
  {
      // Handle exception of type double here
      std::cerr << "We caught an exception of type double" << '\n';
  }
  ```

### 异常机制

```c++
#include <iostream>
#include <string>

int main()
{
    try
    {
        // Statements that may throw exceptions you want to handle go here
        throw -1; // here's a trivial example
    }
    catch (int x)
    {
        // Any exceptions of type int thrown within the above try block get sent here
        std::cerr << "We caught an int exception with value: " << x << '\n';
    }
    catch (double) // no variable name since we don't use the exception itself in the catch block below
    {
        // Any exceptions of type double thrown within the above try block get sent here
        std::cerr << "We caught an exception of type double" << '\n';
    }
    catch (const std::string&) // catch classes by const reference
    {
        // Any exceptions of type std::string thrown within the above try block get sent here
        std::cerr << "We caught an exception of type std::string" << '\n';
    }

    std::cout << "Continuing on our merry way\n";

    return 0;
}
```

​		当`throw`一个异常后，它会沿着调用栈向下查找最近的try块，如果没有任何的显式`try`块，就会以`fail with an exception error`结束程序。

​		被`try`块处理的异常，通过`catch`查找与`throw`类型一致的处理块。不会进行隐式转换和提升转换，但会进行继承间转换。



### stack unwinding 栈解旋

​		通常，异常由一个被调用函数抛出，而在一个调用的函数中进行捕获。`thow`的异常会沿着调用栈向下查找`try`块。

​		当以这种方式捕获到异常时，执行点将从抛出异常的点跳转到处理异常的捕获块。

```c++
#include <cmath> // for sqrt() function
#include <iostream>

// A modular square root function
double mySqrt(double x)
{
    // If the user entered a negative number, this is an error condition
    if (x < 0.0)
        throw "Can not take sqrt of negative number"; // throw exception of type const char*

    return std::sqrt(x);
}

int main()
{
    std::cout << "Enter a number: ";
    double x {};
    std::cin >> x;

    try // Look for exceptions that occur within try block and route to attached catch block(s)
    {
        double d = mySqrt(x);
        std::cout << "The sqrt of " << x << " is " << d << '\n';
    }
    catch (const char* exception) // catch exceptions of type const char*
    {
        std::cerr << "Error: " << exception << std::endl;
    }

    return 0;
}	
```

​		如果在调用栈中匹配到响应的异常处理，执行流会跳转到异常处理的点。这需要从抛出异常的点，展开（退栈）足以到达处理异常的函数调用处，使之成为栈顶函数。称之为栈解旋。

​		栈解旋是正常的函数退栈，函数栈帧上的自动变量会执行正常的析构流程。

​		如果没有匹配到异常处理，栈可能并不会展开。

```c++
#include <iostream>

void D() // called by C()
{
    std::cout << "Start D\n";
    std::cout << "D throwing int exception\n";

    throw - 1;

    std::cout << "End D\n"; // skipped over
}

void C() // called by B()
{
    std::cout << "Start C\n";
    D();
    std::cout << "End C\n";
}

void B() // called by A()
{
    std::cout << "Start B\n";

    try
    {
        C();
    }
    catch (double) // not caught: exception type mismatch
    {
        std::cerr << "B caught double exception\n";
    }

    try
    {
    }
    catch (int) // not caught: exception not thrown within try
    {
        std::cerr << "B caught int exception\n";
    }

    std::cout << "End B\n";
}

void A() // called by main()
{
    std::cout << "Start A\n";

    try
    {
        B();
    }
    catch (int) // exception caught here and handled
    {
        std::cerr << "A caught int exception\n";
    }

    std::cout << "End A\n";
}

int main()
{
    std::cout << "Start main\n";

    try
    {
        A();
    }
    catch (int) // not called because exception was handled by A
    {
        std::cerr << "main caught int exception\n";
    }
    std::cout << "End main\n";

    return 0;
}
```



### 未捕获异常

​		如果一个异常在调用栈中没有被处理，`std::terminate()`就会被调用，程序被终止。在这种情况下，调用栈可能会也可能不会被解旋。

​		如果一个调用栈未能正常解旋，调用栈上函数栈帧中的自动变量不会执行正常的析构流程。未解旋的调用栈保留了异常抛出的所有堆栈状态，更容易确定抛出未处理异常的情况并修复它。

​		发生异常未处理时，操作系统通常会通知程序发生了一个未处理的异常错误。具体实现由操作系统定义，可能包括打印错误信息，弹出错误对话框，或简单的崩溃。



### 捕获所有错误

​		应用程序可能面临：

- 调用的函数可能抛出任意类型的错误（包括自定义的）
-  未捕获的异常可能造成程序立即终止
- 为所有可能的异常类型添加捕获非常繁琐

​		可以使用`catch(...)`来捕获任意类型的异常：

```c++
try {
	throw 5; // throw an int exception
} catch(double) {
    ...
} catch(...) { // catch-all 
    ...
}
```

#### main wap try-catch(…)

​		通常会在`main`中catch-all所有的内容，防止程序立即退出，在退出之前打印错误，并保存程序状态。

```c++
#include <iostream>

int main()
{

    try
    {
        runGame();
    }
    catch(...)
    {
        std::cerr << "Abnormal termination\n";
    }

    saveState(); // Save user's game
    return 1;
}
```

​		这样做会导致程序退出时，实际上发生了栈解旋，无法复现异常的堆栈信息，因此需要在catch-all块中打印退出时的所有堆栈信息。

​		同时，这种方法适用于发布的release来防止意外未发现的bug，但在debug版本中禁用它，来使得在调试阶段尽量暴露问题来进行修复。

### rethrowing 再抛出

​		有些时候，在捕获到异常的时候，没有足够的上下文信息来支持在原地处理异常，会尝试将该异常再向上抛出，以保证错误信息能够完整的向上传递，而不是在原地未被处理而丢失。

​		但是在使用了具有继承链多态的异常处理情况下，可能在捕获到基类的错误中，不清楚要向上抛出什么具体类型的派生类。直接使用`throw;`，可以将捕获到的异常原样向上抛出，而不需要在当前关注具体的异常类型。

```c++
#include <iostream>
class Base
{
public:
    Base() {}
    virtual void print() { std::cout << "Base"; }
};

class Derived: public Base
{
public:
    Derived() {}
    void print() override { std::cout << "Derived"; }
};

int main()
{
    try
    {
        try
        {
            throw Derived{};
        }
        catch (Base& b)
        {
            std::cout << "Caught Base b, which is actually a ";
            b.print();
            std::cout << '\n';
            throw; // note: We're now rethrowing the object here
        }
    }
    // catch (Derived& d) {...}
    catch (Base& b)
    {
        std::cout << "Caught Base b, which is actually a ";
        b.print();
        std::cout << '\n';
    }

    return 0;
}
```





## 异常的应用

### 重载运算符

​		因为运算符的语义限制，重载运算符的参数数量的固定的。不能以引用和返回值的形式传递错误信息。此时，异常就很有效。

```c++
int & IntArray::operator[](const int index)
{
    if(index < 0 || index >= getLength())
        throw index;
    return data_[index];
}
```

​		`assert`是另一个途径。

```c++
int& IntArray::operator[](const int index)
{
	assert(index >0 && index < getLength());
    return data_[index];
}
```



### constructors fail

​		构造函数没有返回值，且构造函数的参数列表与对象的语义紧密相关，不适合添加错误传出。

​		另一方面是，如果构造失败，如何中断构造过程，并保证资源的释放。

- 抛出异常指示对象创建失败
- 对象的构造函数体被中止，但构造函数体之前的成员初始化列表已经执行
- 对象的析构函数不会被调用
- 对象的成员会正常析构

​		上述过程带来一个问题，是类自身的析构函数不被调用，那么在构造函数中抛出异常的类如何确保资源被释放？

- 在构造函数中，将可能抛出异常的代码放在`try-catch`中，捕获处理资源释放后再次抛出。

  ```c++
  class A
  {
      public:
      A()
      {
          try{
              // some exception throw
          } catch(...) {
              // handle resources release
              throw ...;
          }
  	}
  };
  ```

  

- 在构造函数可能抛出异常的类中，将需要动态分配和管理释放的资源单独进行RAII封装。

  ```c++
  class A
  {
      class Member
      {
          int* p{};
          public:
          	explicit Member(int len)
              {
                  p = new int[len];
              }
          	~Member()
              {
                  delete[] p;
              }
  	};
      private:
      	int x_{};
      	Member member_{10};
      	
  	public:
      	A(int x) : x_{x}
      	{
          	if( x <= 0)
                  throw 1;
      	}
      
      	~A()
          {
              std::cerr <<"~A\n";
          }
  };
  
  int main()
  {
  	try
  	{
  		A a{0};
  	}
  	catch (int)
  	{
  		std::cerr << "Oops\n";
  	}
      return 0;
  }
  ```

  



## 异常类型

​		异常类型可以是任意类型，但基础类型作为异常类型的表达式信息不完备的。且，因为不是单独的异常类型，在`try`可能抛出多个同类型错误的情况下，很难无歧义的处理。

```c++
try
{
    if(index > 10)
        throw index;
    
    if(a != 0)
        throw a;
}
catch (int value)
{
    // What are we catching here?
}
```

​		因此，一般设计单独的异常类，来进行异常的抛出与处理。

```c++
#include <iostream>
#include <string>
#include <string_view>

class ArrayException
{
private:
	std::string m_error;

public:
	ArrayException(std::string_view error)
		: m_error{ error }
	{
	}

	const std::string& getError() const { return m_error; }
};

class IntArray
{
private:
	int m_data[3]{}; // assume array is length 3 for simplicity

public:
	IntArray() {}

	int getLength() const { return 3; }

	int& operator[](const int index)
	{
		if (index < 0 || index >= getLength())
			throw ArrayException{ "Invalid index" };

		return m_data[index];
	}

};

int main()
{
	IntArray array;

	try
	{
		int value{ array[5] }; // out of range subscript
	}
	catch (const ArrayException& exception)
	{
		std::cerr << "An array exception occurred (" << exception.getError() << ")\n";
	}
}
```

​		注意，异常处理程序应该按引用而不是按值捕获类异常对象。这可以防止编译器复制异常，当异常是类对象时，复制异常的代价可能很高，并且可以防止在处理派生异常类时进行对象切片(稍后我们将讨论)。通常应该避免通过指针捕获异常，除非有特定的原因要这样做。

​		异常类可以通过继承的多态性使用，并且，抛出的异常是按catch块的顺序检查。

```c++
#include <iostream>

class Base
{
public:
    Base() {}
};

class Derived: public Base
{
public:
    Derived() {}
};

int main()
{
    try
    {
        throw Derived();
    }
    catch (const Base& base)
    {
        std::cerr << "caught Base";
    }
    catch (const Derived& derived)
    {
        std::cerr << "caught Derived";
    }

    return 0;
}
// cauch base
```

> Handlers for derived exception classes should be listed before those for base classes.



### std::exception

​		标准库中目前会抛出28个不同的异常，后续可能还会添加更多的异常类。

​		这些标准库中使用的异常类都继承自接口类`std::exception`，在`<exception>`中，被设计用于标准库抛出任何异常的基类。

​		在大多数情况下，当标准库抛出异常时，我们并不关心它具体的类型。我们只关心一些灾难性的错误，现在我们的程序正在爆炸。std::exception，让我们可以设置一个异常处理程序来捕获std::exception类型的异常，我们最终将在一个地方捕获std::exception和所有派生的异常。

```c++
#include <cstddef> // for std::size_t
#include <exception> // for std::exception
#include <iostream>
#include <limits>
#include <string> // for this example

int main()
{
    try
    {
        // Your code using standard library goes here
        // We'll trigger one of these exceptions intentionally for the sake of the example
        std::string s;
        s.resize(std::numeric_limits<std::size_t>::max()); // will trigger a std::length_error or allocation exception
    }
    // This handler will catch std::exception and all the derived exceptions too
    catch (const std::exception& exception)
    {
        std::cerr << "Standard exception: " << exception.what() << '\n';
    }

    return 0;
}
```

> std::exception has a virtual member function named **what()** that returns a C-style string description of the exception. Most derived classes override the what() function to change the message. 

​		Sometimes we’ll want to handle a specific type of exception differently. In this case, we can add a handler for that specific type, and let all the others “fall through” to the base handler. 

```c++
try
{
     // code using standard library goes here
}
// This handler will catch std::length_error (and any exceptions derived from it) here
catch (const std::length_error& exception)
{
    std::cerr << "You ran out of memory!" << '\n';
}
// This handler will catch std::exception (and any exception derived from it) that fall
// through here
catch (const std::exception& exception)
{
    std::cerr << "Standard exception: " << exception.what() << '\n';
}
```

​		这样的继承层次结构允许我们使用特定的处理程序来针对特定的派生异常类，或者使用基类处理程序来捕获整个异常层次结构。

​		这允许我们在一定程度上控制我们想要处理的异常类型，同时确保我们不必做太多的工作来捕获层次结构中的“其他所有东西”。

​		我们可以在自己的程序中，抛出标准库中列出的28种异常，http://en.cppreference.com/w/cpp/error/exception。std::runtime_error (included as part of the stdexcept header) is a popular choice, because it has a generic name, and its constructor takes a customizable message:

```c++
#include <exception> // for std::exception
#include <iostream>
#include <stdexcept> // for std::runtime_error

int main()
{
	try
	{
		throw std::runtime_error("Bad things happened");
	}
	// This handler will catch std::exception and all the derived exceptions too
	catch (const std::exception& exception)
	{
		std::cerr << "Standard exception: " << exception.what() << '\n';
	}

	return 0;
}
```

​		也可以设计自己单独的异常类，继承`std::exception`接口。重写`what()`来使用和标准库一致的异常处理流程。

```c++
#include <exception> // for std::exception
#include <iostream>
#include <string>
#include <string_view>

class ArrayException : public std::exception
{
private:
	std::string m_error{}; // handle our own string

public:
	ArrayException(std::string_view error)
		: m_error{error}
	{
	}

	// std::exception::what() returns a const char*, so we must as well
	const char* what() const noexcept override { return m_error.c_str(); }
};

class IntArray
{
private:
	int m_data[3] {}; // assume array is length 3 for simplicity

public:
	IntArray() {}

	int getLength() const { return 3; }

	int& operator[](const int index)
	{
		if (index < 0 || index >= getLength())
			throw ArrayException("Invalid index");

		return m_data[index];
	}

};

int main()
{
	IntArray array;

	try
	{
		int value{ array[5] };
	}
	catch (const ArrayException& exception) // derived catch blocks go first
	{
		std::cerr << "An array exception occurred (" << exception.what() << ")\n";
	}
	catch (const std::exception& exception)
	{
		std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
	}
}
```

> Note that virtual function what() has specifier noexcept (which means the function promises not to throw exceptions itself). Therefore, our override should also have specifier noexcept.

​		Because std::runtime_error already has string handling capabilities, it’s also a popular base class for derived exception classes. Here’s the same example derived from std::runtime_error instead:

```c++
#include <exception> // for std::exception
#include <iostream>
#include <stdexcept> // for std::runtime_error
#include <string>

class ArrayException : public std::runtime_error
{
public:
	// std::runtime_error takes a const char* null-terminated string.
	// std::string_view may not be null-terminated, so it's not a good choice here.
	// Our ArrayException will take a const std::string& instead,
	// which is guaranteed to be null-terminated, and can be converted to a const char*.
	ArrayException(const std::string& error)
		: std::runtime_error{ error.c_str() } // std::runtime_error will handle the string
	{
	}

        // no need to override what() since we can just use std::runtime_error::what()
};

class IntArray
{
private:
	int m_data[3]{}; // assume array is length 3 for simplicity

public:
	IntArray() {}

	int getLength() const { return 3; }

	int& operator[](const int index)
	{
		if (index < 0 || index >= getLength())
			throw ArrayException("Invalid index");

		return m_data[index];
	}

};

int main()
{
	IntArray array;

	try
	{
		int value{ array[5] };
	}
	catch (const ArrayException& exception) // derived catch blocks go first
	{
		std::cerr << "An array exception occurred (" << exception.what() << ")\n";
	}
	catch (const std::exception& exception)
	{
		std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
	}
}
```



## 函数try块

​		函数尝试块的设计目的是允许您围绕整个函数体建立异常处理程序，而不是围绕代码块。

​		主要应用在派生类构造函数中捕获基类构造函数抛出的异常：

```c++
B(int x) try : A{x} // note addition of try keyword here
{
}
catch (...) // note this is at same level of indentation as the function itself
{
    // Exceptions from member initializer list or constructor body are caught here

    std::cerr << "Exception caught\n";

    throw; // rethrow the existing exception
}
```

​		在派生类构造函数中捕获异常，而不是延迟到派生类外部的构建代码中。

​		函数try块有一些限制，在异常捕获中有三个处理方式，rethrowing，抛出新异常和解决异常。构造函数的函数try块不允许解决异常，必须抛出新异常或rethrowing。and reaching the end of the catch block will implicitly rethrow.

![image-20221127164159949](../../../../../../AppData/Roaming/Typora/typora-user-images/image-20221127164159949.png)

​		Avoid letting control each the end of a function-level catch block. Instead, explicitly throw, rethrow, or return.

​		函数try块可以捕获派生类和基类构造函数抛出的异常。

```c++
B(int x) try : A{x} // note addition of try keyword here
{
    if (x <= 0) // moved this from A to B
        throw 1; // and this too
}
catch (...)
{
    std::cerr << "Exception caught\n";

    // If an exception isn't explicitly thrown here, the current exception will be implicitly rethrown
}
```

​		不要使用函数try块来清理资源。

> ​		当对象的构造失败时，不会调用类的析构函数。因此，您可能会倾向于使用函数try块来清理在失败前已经部分分配了资源的类。但是，引用失败对象的成员被认为是未定义的行为，因为在catch块执行之前对象是“死的”。这意味着您不能使用函数try在类之后进行清理。

​		Function try is useful primarily for either logging failures before passing the exception up the stack, or for changing the type of exception thrown.

## exception specification

​		In C++, all functions are classified as either **non-throwing** (does not throw exceptions) or **potentially throwing** (may throw an exception).

​		**Exception specifications** are a language mechanism that was originally designed to document what kind of exceptions a function might throw as part of a function specification. While most of the exception specifications have now been deprecated or removed, one useful exception specification was added as a replacement。



### exception satety guarantee

​		异常安全保证是关于在发生异常时函数或类将如何行为的契约准则。

- No guarantee -- There are no guarantees about what will happen if an exception is thrown (e.g. a class may be left in an unusable state)

- Basic guarantee -- If an exception is thrown, no memory will be leaked and the object is still usable, but the program may be left in a modified state.

- Strong guarantee -- If an exception is thrown, no memory will be leaked and the program state will not be changed. This means the function must either completely succeed or have no side effects if it fails. This is easy if the failure happens before anything is modified in the first place, but can also be achieved by rolling back any changes so the program is returned to the pre-failure state.

- No throw / No fail -- The function will always succeed (no-fail) or fail without throwing an exception (no-throw).

  > The no-throw guarantee: if a function fails, then it won’t throw an exception. Instead, it will return an error code or ignore the problem. No-throw guarantees are required during stack unwinding when an exception is already being handled; for example, all destructors should have a no-throw guarantee (as should any functions those destructors call). Examples of code that should be no-throw:
  >
  > - destructors and memory deallocation/cleanup functions
  > - functions that higher-level no-throw functions need to call
  >
  > The no-fail guarantee: a function will always succeed in what it tries to do (and thus never has a need to throw an exception, thus, no-fail is a slightly stronger form of no-throw). Examples of code that should be no-fail:
  >
  > - move constructors and move assignment (move semantics, covered in chapter M)
  > - swap functions
  > - clear/erase/reset functions on containers
  > - operations on std::unique_ptr
  > - functions that higher-level no-fail functions need to call

### noexcept

​		The **noexcept specifier** defines a function as non-throwing.

```c++
void func() noexcept; // this function is non-throwing
```

​		注意，noexcept实际上并不阻止函数抛出异常或调用其他可能抛出异常的函数。相反，在抛出异常时，如果异常退出noexcept函数，则会调用std::terminate。

​		也就是说，必须在`noexcept`声明的函数中处理所有可能的异常，否则在程序运行中发生异常就会直接terminate.

> And note that if `std::terminate` is called from inside a noexcept function, stack unwinding may or may not occur (depending on implementation and optimizations), which means your objects may or may not be destructed properly prior to termination.

​		`noexcept`是一个向编译器声明的标识，不是函数原型的一部分。

​		The `noexcept` specifier has an optional Boolean parameter. `noexcept(true)` is equivalent to `noexcept`, meaning the function is non-throwing. `noexcept(false)` means the function is potentially throwing. These parameters are typically only used in template functions, so that a template function can be dynamically created as non-throwing or potentially throwing based on some parameterized value.



​		Functions that are implicitly non-throwing:

- Destructors

​		Functions that are non-throwing by default for implicitly-declared or defaulted functions:

- Constructors: default, copy, move
- Assignments: copy, move
- Comparison operators (as of C++20)

​		这些函数调用无论是显式还是隐式的调用了其他可能抛出异常的函数，他们也会被视为可能抛出异常的函数。如具有可能抛出异常的构造函数的类成员的类，调用可能抛出异常的类成员的赋值运算符的拷贝运算符。

​		Functions that are potentially throwing (if not implicitly-declared or defaulted):

- Normal functions
- User-defined constructors
- User-defined operators

#### noexcept operator

​		noexcept运算符也可以在函数内部使用。它接受一个表达式作为参数，如果编译器认为它会抛出异常或不抛出异常，则返回true或false。noexcept操作符在编译时做静态检查，实际上并不计算输入表达式。

```c++
void foo() {throw -1;}
void boo() {};
void goo() noexcept {};
struct S{};

constexpr bool b1{ noexcept(5 + 3) }; // true; ints are non-throwing
constexpr bool b2{ noexcept(foo()) }; // false; foo() throws an exception
constexpr bool b3{ noexcept(boo()) }; // false; boo() is implicitly noexcept(false)
constexpr bool b4{ noexcept(goo()) }; // true; goo() is explicitly noexcept(true)
constexpr bool b5{ noexcept(S{}) };   // true; a struct's default constructor is noexcept by default
```

​		noexcept运算符可用于有条件地执行代码，这取决于代码是否可能抛出。这是实现某些异常安全保证所必需的



#### when to use noexcept

​		并不是自己的代码中没有显式抛出异常就能够滥用`noexcept`。一般情况下，大多数函数都具有潜在的异常抛出，因此，调用其他的函数可能都是潜在有异常抛出的。
​		There are a few good reasons to mark functions a non-throwing:

- Non-throwing functions can be safely called from functions that are not exception-safe, such as destructors
- Functions that are noexcept can enable the compiler to perform some optimizations that would not otherwise be available. Because a noexcept function cannot throw an exception outside the function, the compiler doesn’t have to worry about keeping the runtime stack in an unwindable state, which can allow it to produce faster code.
- There are also a few cases where knowing a function is noexcept allows us to produce more efficient implementations in our own code: the standard library containers (such as `std::vector`) are noexcept aware and will use the noexcept operator to determine whether to use `move semantics` (faster) or `copy semantics` (slower) in some places. We cover move semantics in chapter M, and this optimization in lesson [M.5 -- std::move_if_noexcept](https://www.learncpp.com/cpp-tutorial/stdmove_if_noexcept/).

The standard library’s policy is to use `noexcept` only on functions that *must not* throw or fail. Functions that are potentially throwing but do not actually throw exceptions (due to implementation) typically are not marked as `noexcept`.

For your code, there are two places that make sense to use `noexcept`:

- On constructors and overloaded assignment operators that are no-throw (to take advantage of optimizations).
- On functions for which you want to express a no-throw or no-fail guarantee (e.g. to document that they can be safely called from destructors or other noexcept functions)



Best practice

​		Make constructors and overloaded assignment operators `noexcept` when you can. Use `noexcept` on other functions to express a no-fail or no-throw guarantee.

Best practice

​		If you are uncertain whether a function should have a no-fail/no-throw guarantee, error on the side of caution and do not mark it with `noexcept`. Reversing a decision to use noexcept violates an interface commitment to the user about the behavior of the function, and may break existing code. Making guarantees stronger by later adding noexcept to a function that was not originally noexcept is considered safe.

### dynamic exception specifications 动态异常说明

​		在c++ 11之前，直到c++ 17，都是使用动态异常规范来代替noexcept。动态异常规范语法使用throw关键字列出函数可以直接或间接抛出的异常类型:

```c++
int doSomething() throw(); // does not throw exceptions
int doSomething() throw(std::out_of_range, int*); // may throw either std::out_of_range or a pointer to an integer
int doSomething() throw(...); // may throw anything
```

​		由于编译器实现不完整、与模板函数不兼容、对它们工作方式的常见误解以及标准库大多不使用它们的事实等因素，动态异常规范在c++ 11中被弃用，并在c++ 17和c++ 20中从语言中移除。 See [this paper](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0003r0.html#2.0) for more context.

## 异常机制的危险性和缺点

### 发生异常时的资源清理

​		发生异常时，try块中抛出点之后的代码不会再执行。因此，关于对异常点之前发生的资源分配，要在catch块之后进行处理。

​		这也是java finally的由来。

```c++
try
{
    openFile(filename);
    writeFile(filename, data);
    // closeFile(filename);
}
catch (const FileException& exception)
{
    std::cerr << "Failed to write to file: " << exception.what() << '\n';
}

// Make sure file is closed
closeFile(filename);
```

​		这种对资源的管理，一般应该使用RAII进行封装：

```c++
#include <iostream>

Person* john{ nullptr };

try
{
    //john = new Person("John", 18, PERSON_MALE);
     auto* john { new Person("John", 18, PERSON_MALE) };
    std::unique_ptr<Person> upJohn { john }; // upJohn now owns john
    
    processPerson(john);
    
    // // when upJohn goes out of scope, it will delete john
}
catch (const PersonException& exception)
{
    std::cerr << "Failed to process person: " << exception.what() << '\n';
}

//delete john;
```



### 析构函数

​		与构造函数(抛出异常可以是指示对象创建失败的有用方法)不同，在析构函数中永远不应该抛出异常，即析构永远不应该失败。		

​		当堆栈展开过程中从析构函数抛出异常时，会出现此问题。如果发生这种情况，编译器将处于不知道是继续堆栈展开过程还是处理新异常的情况。最终的结果是您的程序将立即终止。

​		如果析构函数中确实有不可避免的失败情况，也应该完全避免在析构函数中使用异常。将消息写入日志文件。

### 性能问题

		- 增加了可执行文件大小
		- 额外的检查，导致运行速度变慢
		- 抛出异常时，必须展开堆栈找到适当的捕获，这是相当高昂的代价

> 需要注意的是，一些现代计算机架构支持一种称为零成本异常的异常模型。如果支持零成本异常，在无错误情况下(这是我们最关心性能的情况)没有额外的运行时成本。然而，在发现异常的情况下，它们会招致更大的损失。

当以下所有条件都为真时，最好使用异常处理:

- 正在处理的错误可能只会很少发生。
- 错误严重，否则无法继续执行。
- 不能在错误发生的地方处理错误。
- 没有其他好的方法可以将错误代码返回给调用者。

​		第四个项目是关键——是否有其他好的方法将错误代码返回给调用者?这取决于你的程序的细节。如果是这样(例如，你可以返回一个空指针，或一个状态码来表示失败)，这可能是更好的选择。如果没有，那么异常是合理的。