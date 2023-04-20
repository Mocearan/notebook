# 内存管理

---

​		

​		The memory that a program uses is typically divided into a few different areas, called segments:

- The code segment (also called a text segment), where the compiled program sits in memory. The code segment is typically read-only.
- The bss segment (also called the uninitialized data segment), where zero-initialized global and static variables are stored.
- The data segment (also called the initialized data segment), where initialized global and static variables are stored.
- The heap, where dynamically allocated variables are allocated from.
- The call stack, where function parameters, local variables, and other function-related information are stored.



## call stack segment

​		The **call stack** (usually referred to as “the stack”) has a much more interesting role to play. 

​		The call stack keeps track of all the active functions (those that have been called but have not yet terminated) from the start of the program to the current point of execution, and handles allocation of all function parameters and local variables.

​		The call stack is implemented as a stack data structure. 

​		The call stack segment holds the memory used for the call stack. When the application starts, the main() function is pushed on the call stack by the operating system. Then the program begins executing.

​		When a function call is encountered, the function is pushed onto the call stack. When the current function ends, that function is popped off the call stack. Thus, by looking at the functions pushed on the call stack, we can see all of the functions that were called to get to the current point of execution.

​		The stack itself is a fixed-size chunk of memory addresses. 

> On Windows, the default stack size is 1MB. On some unix machines, it can be as large as 8MB. If the program tries to put too much information on the stack, stack overflow will result.
>
> **Stack overflow** happens when all the memory in the stack has been allocated -- in that case, further allocations begin overflowing into other sections of memory.
>
> Stack overflow is generally the result of allocating too many variables on the stack, and/or making too many nested function calls (where function A calls function B calls function C calls function D etc…) On modern operating systems, overflowing the stack will generally cause your OS to issue an access violation and terminate the program.
>
> ```c++
> int main()
> {
>  int stack[10000000];
>  std::cout << "hi" << stack[0]; // we'll use stack[0] here so the compiler won't optimize the array away
> 
>  return 0;
> }
> 
> 
> void foo()
> {
>  foo();
>  std::cout << "hi";
> }
> 
> int main()
> {
>  foo();
> 
>  return 0;
> }
> ```
>
> 

​		the “items” we’re pushing and popping on the stack are called **stack frames**. A stack frame keeps track of all of the data associated with one function call. 

​		The “marker” is a register (a small piece of memory in the CPU) known as the stack pointer (sometimes abbreviated “SP”). The stack pointer keeps track of where the top of the call stack currently is.

​		We can make one further optimization: When we pop an item off the call stack, we only have to move the stack pointer down -- we don’t have to clean up or zero the memory used by the popped stack frame. 



​		Here is the sequence of steps that takes place when a function is called:

1. The program encounters a function call.
2. A stack frame is constructed and pushed on the stack. The stack frame consists of:

- The address of the instruction beyond the function call (called the **return address**). This is how the CPU remembers where to return to after the called function exits.
- All function arguments.
- Memory for any local variables
- Saved copies of any registers modified by the function that need to be restored when the function returns

1. The CPU jumps to the function’s start point.
2. The instructions inside of the function begin executing.

When the function terminates, the following steps happen:

1. Registers are restored from the call stack
2. The stack frame is popped off the stack. This frees the memory for all local variables and arguments.
3. The return value is handled.
4. The CPU resumes execution at the return address.



​		A technical note: on some architectures, the call stack grows away from memory address 0. On others, it grows towards memory address 0. As a consequence, newly pushed stack frames may have a higher or a lower memory address than the previous ones.





The stack has advantages and disadvantages:

- Allocating memory on the stack is comparatively fast.
- Memory allocated on the stack stays in scope as long as it is on the stack. It is destroyed when it is popped off the stack.
- All memory allocated on the stack is known at compile time. Consequently, this memory can be accessed directly through a variable.
- Because the stack is relatively small, it is generally not a good idea to do anything that eats up lots of stack space. This includes passing by value or creating local variables of large arrays or other memory-intensive structures.

## heap segment

​		The heap segment (also known as the “free store”) keeps track of memory used for dynamic memory allocation. 

​		use the new operator to allocate memory, this memory is allocated in the application’s heap segment.

​		The address of this memory is passed back by operator new, and can then be stored in a pointer. 

​		When a dynamically allocated variable is deleted, the memory is “returned” to the heap and can then be reassigned as future allocation requests are received. Remember that deleting a pointer does not delete the variable, it just returns the memory at the associated address back to the operating system.



​		The heap has advantages and disadvantages:

- Allocating memory on the heap is comparatively slow.
- Allocated memory stays allocated until it is specifically deallocated (beware memory leaks) or the application ends (at which point the OS should clean it up).
- Dynamically allocated memory must be accessed through a pointer. Dereferencing a pointer is slower than accessing a variable directly.
- Because the heap is a big pool of memory, large arrays, structures, or classes can be allocated here.

# 动态内存管理

​		**Dynamic memory allocation** is a way for running programs to request memory from the operating system when needed. 

---



​		c++支持三种基本的内存分配方式：

- 静态内存分配

  ​	对静态和全局变量，内存分配当且仅当程序开始运行时发生一次，生命周期持续到程序结束。

- 自动内存分配

  ​	对函数形参或局部变量，内存分配在程序进入相应的块作用域时分配，生命周期持续到块作用域结束。

- 动态内存分配

  

​		静态和自动内存分配的对象大小在编译时必须明确，内存的分配和释放自动进行。

​		在编译时分配内存势必要对一些暂不明确大小的东西进行预测，提供一个大小来预先分配内存。这其实相当糟糕：

- 如果没有对象实际被使用，会导致内存的浪费。
- 无法确定预分配的内存中哪些被实际使用了
- 每个进程的栈大小受限且相当小，容易造成`stack overflow`
- 如果预分配的大小在程序运行中不够使用，容易造成内存溢出



​		系统维护一个相当于实际物理内存大小的堆空间，动态内存分配在运行时向系统申请所需要大小的内存空间，由堆空间返回相应大小的内存空间使用权。动态内存分配主动维护内存的申请和释放。

> ​		When you dynamically allocate memory, you’re asking the operating system to reserve some of that memory for your program’s use. If it can fulfill this request, it will return the address of that memory to your application. From that point forward, your application can use this memory as it wishes. When your application is done with the memory, it can return the memory back to the operating system to be given to another program.

## new / delete

### new operator

> new operator构成的表达式称为new 表达式

​		new表达式尝试申请存储空间，并且在已经申请的存储空间上，尝试构造并初始化一个无名的对象或对象数组。

​		new表达式返回一个指向它构造的对象或对象数组的纯右值指针。

```c++
new type; // dynamically allocate an integer (and discard the result)
new type{val}; // uniform initialization
new type(val); // direct initialization

new type[len]; // array new.
new type[len]();
new type[len]{val};
// len has to to a type that's convertible to std::size_t
```

#### Operator new failed

​		`new`失败会抛出`bad_alloc`异常，如果该异常未被捕获，则会造成程序`crash`。如果不期望程序`crash`，而是让`new`返回`nullptr`，需要添加`std::nothrow`。

```c++
new (std::nothrow) type;
```





### delete operator

> delete operator构成的表达式称为delete表达式

​		销毁先前由 new 表达式分配的对象，并释放获得的内存区域。

```c++
delete expr;

delete[] expr;
```

- delete空悬指针会造成``UB``

- 多个指针指向同一对象，在删除时，会造成`multiple dangling`

  ```c++
  int* ptr{new int{}};; 
  int* otherPtr{ptr};
  delete ptr; // ptr and otherPtr now dangling
  ptr = nullptr; // ptr nullptr, but otherPtr still dangling
  ```



#### memory leaks

​		当所有持有动态内存地址的指针被销毁，则没有任何信息访问该动态内存，也无法释放该动态内存。这被称为内存泄露`memory leaks`.

> ​		Memory leaks eat up free memory while the program is running, making less memory available not only to this program, but to other programs as well. Programs with severe memory leak problems can eat all the available memory, causing the entire machine to run slowly or even crash. Only after your program terminates is the operating system able to clean up and “reclaim” all leaked memory.

​		当唯一持有动态内存地址的指针，被指向其他的对象，也会造成该动态内存地址丢失从而泄露。

```c++
int val{5};
int* ptr{new int{}};
ptr = &val; // leak

int* ptr{new int{}};
ptr = new int{}; // lea
```



### 低层内存管理

