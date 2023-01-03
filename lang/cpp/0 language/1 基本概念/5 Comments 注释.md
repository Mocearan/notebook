# Comments

​	A **comment** is a programmer-readable note that is inserted directly into the source code of the program. 

​		注释实际上会被预处理器在**翻译阶段3**替换为空格，它们只需要被阅读源代码文本的人当做注记。

​		有一些工具可以解析各种约定俗成的注释格式，如`doxygen`



## C 风格注释

`/*注释*/`

​		C风格注释即多行注释，通常用于注释大块的文本，也可以用于注释单行文本。

>   ```c++
>   /*
>   *	此格式常用于指示文档块。第二行此星号简单地被当做注释的一部
>   * 	分。
>   */
>   ```
>
>   ```c++
>   /* /**/ */
>   C风格的多行注释不能进行嵌套，因为第一个/* 会向后就近匹配第一个出现的 */，造成第一个*/脱离注释块。
>   ```
>
>   > ​		Don’t use multi-line comments inside other multi-line comments. Wrapping single-line comments inside a multi-line comment is okay.
>
>   **其他排除源代码的方式**
>
>   1.  预处理指令 - 条件编译
>
>       ```c++\
>       #if 0
>       	std::cout << "this statement will not be compiled.";
>       #endif
>       ```
>
>   2.  条件块
>
>       ```c++
>       if(false) {
>           std::cout << "this statement will not be executed";
>       }
>       ```

## C++ 风格注释

`//`

​		C++风格注释即单行注释，可以将多个c++风格注释放在一起来组成多行注释。

```c++
#include <iostream>
/* C 风格注释
能含有多行 */
/* 或只有一行 */
 
/**************
 *  你能插入任意数量的 * ，
 *  但你不能嵌套注释
 */
 
// C++ 风格注释能注释一行
 
// 或者，能将
// 它们串在一起
 
int main()
{
  // 注释在预处理前被移除，
  // 故 ABC 为 1 ，而非 1//2134 ，
  // 而 "1 hello world"将得以打印
#define ABC 1//2134
  std::cout << ABC << " hello world\n";
 
  // 下方代码不会运行
  // return 1;
 
  // 下方代码会运行
  return 0;
}
```



>   **NOTE：**
>
>   ​		注释在预处理器阶段已经被替换为空格，所以宏不能用于组成注释，因此没有终止的多行注释不会从#include文件中溢出。





## 注释规范

​		Comment your code liberally, and write your comments as if speaking to someone who has no idea what the code does. Don’t assume you’ll remember why you made specific choices.
