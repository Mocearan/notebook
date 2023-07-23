# json

​		JSON is a lightweight, text-based, language-independent syntax for defining data interchange formats. 

​		`JSON`是一种轻量级的、基于文本的、独立于语言的语法，用于定义数据交换格式。

------------

​		`Json`(`JavaScript Object Notation`) 派生自`ECMAScript`编程语言，但独立于编程语言。JSON为结构化数据的可移植表示定义了一小组结构化规则。

​		本质上，`Json`是一个文本标记文件，有自己独立的语法，因此多用于跨语言的数据交换。各语言中`Json`映射的结构由语言进行定义。

​		本文档基于ECMA-404 2E。

> [ECMA-404, 2nd edition, December 2017 (ecma-international.org)](https://www.ecma-international.org/wp-content/uploads/ECMA-404_2nd_edition_december_2017.pdf)



## introduction

​		`JSON `最初由 Douglas Crockford 在 2001 年提出，旨在解决不同应用程序之间数据交换的问题。`JSON `的设计目标是简单、易于理解和实现，以及与现有的编程语言和数据结构兼容。

​		`JSON `在网络通信、数据存储和配置文件等领域广泛应用。它可以用于客户端和服务器之间的数据交换，也可以用于存储和传输结构化数据。由于 `JSON `的简洁性和易读性，它成为了许多编程语言和框架中的标准数据格式。



## scope

​		JSON  defines a small set of structuring rules for the portable representation of structured data.

​		`Json`定义了一个较小的结构规则，用来表示结构化的数据。



## Conformance

​		符合规范的`JSON`文本是一系列严格符合规范定义的`JSON`语法的`Unicode`代码点。

- 符合标准的`JSON`文本处理器不应该接受任何非标准的`JSON`文本输入。
- 符合规范的处理器可能通过语义限制来约束符合规范的`JSON`文本集合。



## JSON Text

​		A `JSON `text is a sequence of tokens formed from Unicode code points that conforms to the JSON value  grammar.

​		`JSON`文本是一个标记序列，由符合`JSON`值语法的`Unicode`代码点组成。

​		The set of tokens includes six structural tokens, strings, numbers, and three literal name tokens.

​		这组标记包括6个结构标识（符号）、字符串、数字和3个字面名称标记。

- 任何标记之前或之后都可以有无意义的空白。
- 空格是一个或多个下列代码点组成的序列:字符表(U+0009)、换行(U+000A)、回车(U+000D)和空格(U+0020)。  
- 任何标记中都不允许有空格，字符串中允许有空格。  

The six structural tokens: 

- `[`， U+005B left square bracket 
- `{` ，U+007B left curly bracket
- `` ]``， U+005D right square bracket
- `` }``， U+007D right curly bracket 
- ``:`` ，U+003A colon 
- `,`， U+002C comma

These are the three literal name tokens: 

- `true`， U+0074 U+0072 U+0075 U+0065 
- `false `，U+0066 U+0061 U+006C U+0073 U+0065 
- `null `，U+006E U+0075 U+006C U+006C



## JSON Value

​		A `JSON `value can be an `object`, `array`, `number`, `string`, `true`, `false`, or `null`.

![image-20230723165757379](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230723165757379.png)



## JSON Object

​		An object structure is represented as a pair of curly bracket tokens surrounding zero or more name/value pairs. 

​		对象结构是一对花括号包裹的零或多个`name/json-value`对。

- A name is a string.

  - `JSON`语法对命名字符串没有任何限制
  - 不要求名称字符串唯一
  - 也没有对名称/值对的顺序赋予任何意义。 

- A single colon token follows each name, separating the name from the value.

  每个冒号，分隔名称和值。

- A single  comma token separates a value from a following name. 

  每个逗号，分割键值对。

  

![image-20230723170347953](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230723170347953.png)



## JSON Array

​		An array structure is a pair of square bracket tokens surrounding zero or more values. 

​		数组结构是一对中括号包裹的零或多个`Json Value`

- The values are  separated by commas. 

​		虽然`JSON`语法没有对质的顺序进行任何含义的约束，但是`JSON`数组结构通常用于有序由语义的情况。



## Number

​		A number is a sequence of decimal digits with no superfluous leading zero. I

​		数字是不以`0`开始的十进制数字序列。

- 它前面可以有一个减号(U+002D)。
- 它可以有一个以小数点为前缀的小数部分(U+002E)。
- 它可以有一个指数，以e (U+0065)或e (U+0045)为前缀，还可以有+ (U+002B)或- (U+002D)。
- 这些数字是U+0030到U+0039之间的代码点
- 不能表示为数字序列的数值(例如`Infinity`和`NaN`)是不允许的。

​		![image-20230723171112319](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230723171112319.png)



## String

​		A string is a sequence of Unicode code points wrapped with quotation marks (U+0022).

​		字符串是用引号包裹起来的Unicode码位序列(U+0022)。

- 除了必须转义的代码点之外，所有的代码点都可以放在引号内:
  - 引号(U+0022)
  - 反向solidus (U+005C)
  - 控制字符U+0000到U+001F。
- 有些字符有两种转义序列表示。

- `\"` represents the quotation mark character (U+0022). 
- `\\ `represents the reverse solidus character (U+005C). 
- `\/ `represents the solidus character (U+002F). 
- ` \b `represents the backspace character (U+0008). 
- `\f `represents the form feed character (U+000C). 
- `\n `represents the line feed character (U+000A). 
- `\r `represents the carriage return character (U+000D). 
- `\t `represents the character tabulation character (U+0009

​		例如，只包含一个`\`字符的字符串可以表示为"\\"。

> 任何代码点都可以表示为由ISO/IEC 10646确定的十六进制转义序列。
>
> 如果代码点在基本的多语言平面(U+0000到U+FFFF)中，那么它可以表示为一个6个字符的序列:一个`\`，然后是小写字母`u`，然后是四个编码代码点的十六进制数字。
>
> 可使用十六进制数字©Ecma International 2017 5位数字(U+0030 ~ U+0039)或大写字母A ~ F (U+0041 ~ U+0046)或小写字母A ~ F (U+0061 ~ U+0066)。
>
> 例如，只包含一个`\`字符的字符串可以表示为`"\u005C"`。

​		The following four cases all produce the same result: 

- `"\u002F" `
- `"\u002f" `
- `"\/"` 
- `"/"`

为了转义不在基本多语言平面中的代码点，可以将字符表示为一个包含12个字符的序列，编码对应于代码点的UTF-16代理项对。

例如，一个只包含G谱号的字符串`(U+1D11E)`可以表示为`"\uD834\uDD1E"`。

然而，`JSON`文本的处理器是将这样的代理项对解释为单个代码点还是显式代理项对，这是由特定的处理器决定的语义决策。  

请注意，JSON语法允许Unicode当前不提供字符赋值的代码点

![image-20230723172816226](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20230723172816226.png)