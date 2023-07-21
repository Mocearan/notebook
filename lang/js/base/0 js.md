# js

---

​		`JavaScript `是一种轻量级的脚本语言。可插入 `HTML `页面，由所有的现代浏览器执行。

​		`DOM (Document Object Model)`（文档对象模型）是用于访问 `HTML `元素的正式 W3C 标准。

​		`JavaScript `已经由 `ECMA`（欧洲电脑制造商协会）通过 `ECMAScript `实现语言的标准化。称为`es`标准

​		`JavaScript `使用 `Unicode `字符集。

> [HTML UTF-8 参考手册 | 菜鸟教程 (runoob.com)](https://www.runoob.com/charsets/ref-html-utf8.html)

## 运行环境

​		`js`可以在浏览器中通过浏览器内置的脚本解析器运行，载体是单独的`js`文件或`html`中的`<script>`标签中。`<script>`标签可被放置在`html`页面的`<body>`和`<head>`部分。

​		`js`还可以在`node.js`环境中进行脚本解析运行，进而提供了非前端渲染使用的环境，使之有了开发后台程序的可能。



## 语法

### 数据类型

​		**值类型(基本类型)**：字符串（String）、数字(Number)、布尔(Boolean)、空（Null）、未定义（Undefined）、Symbol。

> *Symbol 是 ES6 引入了一种新的原始数据类型，表示独一无二的值。*

​		**引用数据类型（对象类型）**：对象(Object)、数组(Array)、函数(Function)，还有两个特殊的对象：正则（RegExp）和日期（Date）。

​		JavaScript 拥有动态类型。这意味着相同的变量可用作不同的类型。

​		变量的数据类型可以使用 **typeof** 操作符来查看：

```js
typeof "John"                // 返回 string
typeof 3.14                  // 返回 number
typeof false                 // 返回 boolean
typeof [1,2,3,4]             // 返回 object
typeof {name:'John', age:34} // 返回 object
```





### 字面量

```js
// number
3.14
1001
123e5
```

```js
// string
"John Doe"
'John Doe'
```

```js
// expression
5 + 6
5 * 10
```

```js
// array
[40, 100, 1, 5, 25, 10]
```

```js
// object
{firstName:"John", lastName:"Doe", age:50, eyeColor:"blue"}
```

```js
// function
function myFunction(a, b) { return a * b;}
```



### 变量

​		`javaScript `使用关键字 `var`来定义变量， 使用等号来为变量赋值：

```js
var x, length
x = 5
length = 6
```



​		`Undefined `这个值表示变量不含有值。

​		可以通过将变量的值设置为 `null `来清空变量。

```js
cars=null;
person=null;
```

​		值赋给尚未声明的变量，该变量将被自动作为 window 的一个属性。

```js
carname="Volvo";
```

​		非严格模式下给未声明变量赋值创建的全局变量，是全局对象的可配置属性，可以删除。

```js
var var1 = 1; // 不可配置全局属性
var2 = 2; // 没有使用 var 声明，可配置全局属性

console.log(this.var1); // 1
console.log(window.var1); // 1
console.log(window.var2); // 2

delete var1; // false 无法删除
console.log(var1); //1

delete var2; 
console.log(delete var2); // true
console.log(var2); // 已经删除 报错变量未定义
```

​		`JavaScript `中，函数及变量的声明都将被提升到函数的最顶部。

​		`JavaScript `中，变量可以在使用后声明，也就是变量可以先使用再声明。

```js
x = 5; // 变量 x 设置为 5

elem = document.getElementById("demo"); // 查找元素
elem.innerHTML = x;                     // 在元素中显示 x

var x; // 声明 x
```

​		 `JavaScript `严格模式(strict mode)不允许使用未声明的变量。



### let / const

​		在 2015 年以前，我们使用 `var `关键字来声明 `JavaScript `变量。

​		在 2015 后的 `JavaScript `版本 (ES6) 允许我们使用 `const `关键字来定义一个常量，使用 `let `关键字定义的限定范围内作用域的变量。



​		使用 `var `关键字声明的变量不具备块级作用域的特性，它在` {} `外依然能被访问到。

```js
{ 
    var x = 2; 
}
// 这里可以使用 x 变量


var x = 10;
// 这里输出 x 为 10
{ 
    var x = 2;
    // 这里输出 x 为 2
}
// 这里输出 x 为 2
```

​		ES6 可以使用 `let `关键字来实现块级作用域。

​		`let `声明的变量只在 `let `命令所在的代码块 `{}`内有效，在 `{} `之外不能访问。

```js
{ 
    let x = 2;
}
// 这里不能使用 x 变量

var x = 10;
// 这里输出 x 为 10
{ 
    let x = 2;
    // 这里输出 x 为 2
}
// 这里输出 x 为 10
```

```js
var i = 5;
for (var i = 0; i < 10; i++) {
    // 一些代码...
}
// 这里输出 i 为 10

var i = 5;
for (let i = 0; i < 10; i++) {
    // 一些代码...
}
// 这里输出 i 为 5
```

​		函数内使用 `var `声明的变量只能在函数内访问，如果不使用 `var `则是全局变量。在函数体内使用 **var** 和 **let** 关键字声明的变量有点类似。在函数体外或代码块外使用 **var** 和 **let** 关键字声明的变量也有点类似。

```js
// 使用 var
var x = 2;       // 全局作用域

// 使用 let
let x = 2;       // 全局作用域

// 使用 var
function myFunction() {
    var carName = "Volvo";   // 局部作用域
}

// 使用 let
function myFunction() {
    let carName = "Volvo";   //  局部作用域
}
```

​		在 JavaScript 中, 全局作用域是针对 JavaScript 环境。在 HTML 中, 全局作用域是针对 `window `对象。

​		使用 **var** 关键字声明的全局作用域变量属于 `window `对象,使用 **let** 关键字声明的全局作用域变量不属于 window 对象:

```js
var carName = "Volvo";
// 可以使用 window.carName 访问变量

let carName = "Volvo";
// 不能使用 window.carName 访问变量
```

​		`var`声明的变量可以通过再次声明来重置，但`let`声明的不可以。简单来说，`var`定义的就是传统`js`变量，而`let`定义的则是广义编程语言中的独立变量。因此`let`定义的变量不能进行声明提升。



###  操作符

```js
// 算术运算符  =  +  -  *  /
(5 + 6) * 10
```

```js
// 赋值运算符
x = 5
y = 6
z = (x + y) * 10
```

```js
// 逻辑运算符 ==  != <  > 
```



### 关键字



## JavaScript 保留关键字

​		`Javascript `的保留关键字不可以用作变量、标签或者函数名。有些保留关键字是作为 Javascript 以后扩展使用。

| abstract | arguments | boolean    | break     | byte         |
| -------- | --------- | ---------- | --------- | ------------ |
| case     | catch     | char       | class*    | const        |
| continue | debugger  | default    | delete    | do           |
| double   | else      | enum*      | eval      | export*      |
| extends* | false     | final      | finally   | float        |
| for      | function  | goto       | if        | implements   |
| import*  | in        | instanceof | int       | interface    |
| let      | long      | native     | new       | null         |
| package  | private   | protected  | public    | return       |
| short    | static    | super*     | switch    | synchronized |
| this     | throw     | throws     | transient | true         |
| try      | typeof    | var        | void      | volatile     |
| while    | with      | yield      |           |              |



### JavaScript 对象、属性和方法

​		您也应该避免使用 `JavaScript `内置的对象、属性和方法的名称作为 Javascript 的变量或函数名：

| Array     | Date     | eval     | function      | hasOwnProperty |
| --------- | -------- | -------- | ------------- | -------------- |
| Infinity  | isFinite | isNaN    | isPrototypeOf | length         |
| Math      | NaN      | name     | Number        | Object         |
| prototype | String   | toString | undefined     | valueOf        |

------

### Java 保留关键字

​		`JavaScript `经常与 Java 一起使用。您应该避免使用一些 Java 对象和属性作为 JavaScript 标识符：

| getClass | java | JavaArray | javaClass | JavaObject | JavaPackage |
| -------- | ---- | --------- | --------- | ---------- | ----------- |
|          |      |           |           |            |             |

------

### Windows 保留关键字

​		`JavaScript `可以在 `HTML `外部使用。它可在许多其他应用程序中作为编程语言使用。

​		在 `HTML `中，您必须（为了可移植性，您也应该这么做）避免使用 `HTML `和 `Windows `对象和属性的名称作为 `Javascript `的变量及函数名：

| alert          | all                | anchor      | anchors            | area               |
| -------------- | ------------------ | ----------- | ------------------ | ------------------ |
| assign         | blur               | button      | checkbox           | clearInterval      |
| clearTimeout   | clientInformation  | close       | closed             | confirm            |
| constructor    | crypto             | decodeURI   | decodeURIComponent | defaultStatus      |
| document       | element            | elements    | embed              | embeds             |
| encodeURI      | encodeURIComponent | escape      | event              | fileUpload         |
| focus          | form               | forms       | frame              | innerHeight        |
| innerWidth     | layer              | layers      | link               | location           |
| mimeTypes      | navigate           | navigator   | frames             | frameRate          |
| hidden         | history            | image       | images             | offscreenBuffering |
| open           | opener             | option      | outerHeight        | outerWidth         |
| packages       | pageXOffset        | pageYOffset | parent             | parseFloat         |
| parseInt       | password           | pkcs11      | plugin             | prompt             |
| propertyIsEnum | radio              | reset       | screenX            | screenY            |
| scroll         | secure             | select      | self               | setInterval        |
| setTimeout     | status             | submit      | taint              | text               |
| textarea       | top                | unescape    | untaint            | window             |

------

### HTML 事件句柄

​		除此之外，您还应该避免使用 `HTML `事件句柄的名称作为 `Javascript `的变量及函数名。

| onblur    | onclick    | onerror     | onfocus     |
| --------- | ---------- | ----------- | ----------- |
| onkeydown | onkeypress | onkeyup     | onmouseover |
| onload    | onmouseup  | onmousedown | onsubmit    |



### 语句

​		在 JavaScript 中，用分号来结束语句是可选的。

 JavaScript 语句标识符 (关键字) ：

| 语句         | 描述                                                         |
| :----------- | :----------------------------------------------------------- |
| break        | 用于跳出循环。                                               |
| catch        | 语句块，在 try 语句块执行出错时执行 catch 语句块。           |
| continue     | 跳过循环中的一个迭代。                                       |
| do ... while | 执行一个语句块，在条件语句为 true 时继续执行该语句块。       |
| for          | 在条件语句为 true 时，可以将代码块执行指定的次数。           |
| for ... in   | 用于遍历数组或者对象的属性（对数组或者对象的属性进行循环操作）。 |
| function     | 定义一个函数                                                 |
| if ... else  | 用于基于不同的条件来执行不同的动作。                         |
| return       | 退出函数                                                     |
| switch       | 用于基于不同的条件来执行不同的动作。                         |
| throw        | 抛出（生成）错误 。                                          |
| try          | 实现错误处理，与 catch 一同使用。                            |
| var          | 声明一个变量。                                               |
| while        | 当条件语句为 true 时，执行语句块。                           |

```js
var lastname="Doe", age=30, job="carpenter";
```



### 字符串

​		可以使用单引号或双引号：

​		可以在字符串中使用引号，只要不匹配包围字符串的引号即可：

```js
var answer="It's alright";
var answer="He is called 'Johnny'";
var answer='He is called "Johnny"';

// 也可以在字符串添加转义字符来使用引号
var x = 'It\'s alright';
var y = "He is called \"Johnny\"";
```

​		可以使用索引位置来访问字符串中的每个字符

```js
const name = "RUNOOB";
let letter = name[2];
```

​		可以使用内置属性 **length** 来计算字符串的长度：

```js
var txt = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var sln = txt.length;
```

​		可以使用 new 关键字将字符串定义为一个对象。不要创建 `String `对象，它会拖慢执行速度，并可能产生其他副作用。

```js
var firstName = new String("John")

var x = "John";
var y = new String("John");
typeof x // 返回 String
typeof y // 返回 Object

var x = "John";             
var y = new String("John");
(x === y) // 结果为 false，因为 x 是字符串，y 是对象
```

> === 为绝对相等，即数据类型与值都必须相等。



#### 字符串属性

| 属性        | 描述                       |
| :---------- | :------------------------- |
| constructor | 返回创建字符串属性的函数   |
| length      | 返回字符串的长度           |
| prototype   | 允许您向对象添加属性和方法 |

#### 字符串方法

更多方法实例可以参见：[JavaScript String 对象](https://www.runoob.com/jsref/jsref-obj-string.html)。

| 方法                | 描述                                                         |
| :------------------ | :----------------------------------------------------------- |
| charAt()            | 返回指定索引位置的字符                                       |
| charCodeAt()        | 返回指定索引位置字符的 Unicode 值                            |
| concat()            | 连接两个或多个字符串，返回连接后的字符串                     |
| fromCharCode()      | 将 Unicode 转换为字符串                                      |
| indexOf()           | 返回字符串中检索指定字符第一次出现的位置                     |
| lastIndexOf()       | 返回字符串中检索指定字符最后一次出现的位置                   |
| localeCompare()     | 用本地特定的顺序来比较两个字符串                             |
| match()             | 找到一个或多个正则表达式的匹配                               |
| replace()           | 替换与正则表达式匹配的子串                                   |
| search()            | 检索与正则表达式相匹配的值                                   |
| slice()             | 提取字符串的片断，并在新的字符串中返回被提取的部分           |
| split()             | 把字符串分割为子字符串数组                                   |
| substr()            | 从起始索引号提取字符串中指定数目的字符                       |
| substring()         | 提取字符串中两个指定的索引号之间的字符                       |
| toLocaleLowerCase() | 根据主机的语言环境把字符串转换为小写，只有几种语言（如土耳其语）具有地方特有的大小写映射 |
| toLocaleUpperCase() | 根据主机的语言环境把字符串转换为大写，只有几种语言（如土耳其语）具有地方特有的大小写映射 |
| toLowerCase()       | 把字符串转换为小写                                           |
| toString()          | 返回字符串对象值                                             |
| toUpperCase()       | 把字符串转换为大写                                           |
| trim()              | 移除字符串首尾空白                                           |
| valueOf()           | 返回某个字符串对象的原始值                                   |



#### 模板字符串

​		模板字符串是一种方便的字符串语法，允许你在字符串中嵌入表达式和变量。

​		模板字面量是用反引号（**`**）分隔的字面量，允许多行字符串、带嵌入表达式的字符串插值和一种叫带标签的模板的特殊结构。

```js
`string text`

`string text line 1
 string text line 2`

`string text ${expression} string text`

tagFunction`string text ${expression} string text`

// string text：将成为模板字面量的一部分的字符串文本。几乎允许所有字符，包括换行符和其他空白字符。但是，除非使用了标签函数，否则无效的转义序列将导致语法错误。
// expression：要插入当前位置的表达式，其值被转换为字符串或传递给 tagFunction。
// tagFunction：如果指定，将使用模板字符串数组和替换表达式调用它，返回值将成为模板字面量的值。
```

​		要转义模板字面量中的反引号（**`**），需在反引号之前加一个反斜杠（**\**）。

```js
`\`` === "`"; // true
```

​		除了普通字符串外，模板字面量还可以包含占位符——一种由美元符号和大括号分隔的嵌入式表达式：**${expression}**。

​		字符串和占位符被传递给一个函数（要么是默认函数，要么是自定义函数）。

​		默认函数（当未提供自定义函数时）只执行字符串插值来替换占位符，然后将这些部分拼接到一个字符串中。

```js
const name = 'Runoob';
const age = 30;
const message = `My name is ${name} and I'm ${age} years old.`;

let price = 10;
let VAT = 0.25;
let total = `Total: ${(price * (1 + VAT)).toFixed(2)}`;
```

```js
let header = "";
let tags = ["RUNOOB", "GOOGLE", "TAOBAO"];

let html = `<h2>${header}</h2><ul>`;
for (const x of tags) {
  html += `<li>${x}</li>`;
}

html += `</ul>`;
```



### 	数组

```js
var cars=new Array();
cars[0]="Saab";
cars[1]="Volvo";
cars[2]="BMW";
```

```js
// condensed array’
var cars=new Array("Saab","Volvo","BMW");
```

```js
// literal array
var cars=["Saab","Volvo","BMW"];
```

### object

​		对象由花括号分隔。在括号内部，对象的属性以名称和值对的形式 (name : value) 来定义。属性由逗号分隔：

```js
var person={firstname:"John", lastname:"Doe", id:5566};
```

​		空格和折行无关紧要。声明可横跨多行：

```js
var person={
firstname : "John",
lastname : "Doe",
id    : 5566
};
```

​		对象属性有两种寻址方式：

```js
name=person.lastname;
name=person["lastname"];
```

声明新变量时，可以使用关键词 "`new`" 来声明其类型：

```js
var carname=new String;
var x=   new Number;
var y=   new Boolean;
var cars=  new Array;
var person= new Object;
```

​		`JavaScript `变量均为对象。当您声明一个变量时，就创建了一个新的对象。



## js 类

​		每个类中包含了一个特殊的方法 **constructor()**，它是类的构造函数，这种方法用于创建和初始化一个由 **class** 创建的对象。

```js
class Runoob {
  constructor(name, url) {
    this.name = name;
    this.url = url;
  }
}
```

​		可以使用 **new** 关键字来创建对象：

```js
let site = new Runoob("菜鸟教程",  "https://www.runoob.com");
```

​		类声明和类表达式的主体都执行在严格模式下。比如，构造函数，静态方法，原型方法，getter 和 setter 都在严格模式下执行。

> JavaScript 是面向对象的语言，但 JavaScript 不使用类。
>
> 在 JavaScript 中，不会创建类，也不会通过类来创建对象（就像在其他面向对象的语言中那样）。
>
> JavaScript 基于 prototype，而不是基于类的。

### 类关键字

| 关键字                                                       | 描述                   |
| :----------------------------------------------------------- | :--------------------- |
| [extends](https://www.runoob.com/js/jsref-class-extends.html) | 继承一个类             |
| [static](https://www.runoob.com/js/jsref-class-static.html)  | 在类中定义一个静态方法 |
| [super](https://www.runoob.com/js/jsref-class-super.html)    | 调用父类的构造方法     |

###  构造方法

构造方法是一种特殊的方法：

- 构造方法名为 constructor()。
- 构造方法在创建新对象时会自动执行。
- 构造方法用于初始化对象属性。
- 如果不定义构造方法，JavaScript 会自动添加一个空的构造方法。

### 类表达式

​		类表达式可以命名或不命名。命名类表达式的名称是该类体的局部名称。

```js
// 未命名/匿名类
let Runoob = class {
  constructor(name, url) {
    this.name = name;
    this.url = url;
  }
};
console.log(Runoob.name);
// output: "Runoob"
 
// 命名类
let Runoob = class Runoob2 {
  constructor(name, url) {
    this.name = name;
    this.url = url;
  }
};
console.log(Runoob.name);
// 输出: "Runoob2"
```

### getter / setter

​		使用的是 get 和 set 关键字。getter 和 setter 都需要在严格模式下执行。

​		 getter / setter 是一个方法，当你想获取属性值时也不要使用括号。

​		很多开发者在属性名称前使用下划线字符 **_** 将 getter/setter 与实际属性分开：

```js
class Runoob {
  constructor(name) {
    this._sitename = name;
  }
  set sitename(x) {
    this._sitename = x;
  }
  get sitename() {
    return this._sitename;
  }
}
 
let noob = new Runoob("菜鸟教程");
noob.sitename = "RUNOOB";
document.getElementById("demo").innerHTML = noob.sitename;
```



### 继承

- `JavaScript `类继承使用 `extends `关键字。
- **super()** 方法用于调用父类的构造函数。

```js
class Site {
  constructor(name) {
    this.sitename = name;
  }
  present() {
    return '我喜欢' + this.sitename;
  }
}
 
class Runoob extends Site {
  constructor(name, age) {
    super(name);
    this.age = age;
  }
  show() {
    return this.present() + ', 它创建了 ' + this.age + ' 年。';
  }
}
 
let noob = new Runoob("菜鸟教程", 5);
document.getElementById("demo").innerHTML = noob.show();
```



### 静态方法

​		静态方法是使用 static 关键字修饰的方法，又叫类方法，属于类的，但不属于对象

```js
class Runoob {
  constructor(name) {
    this.name = name;
  }
  static hello() {
    return "Hello!!";
  }
}
 
let noob = new Runoob("菜鸟教程");
 
// 可以在类中调用 'hello()' 方法
document.getElementById("demo").innerHTML = Runoob.hello();
 
// 不能通过实例化后的对象调用静态方法
// document.getElementById("demo").innerHTML = noob.hello();
// 以上代码会报错
```

​		如果你想在对象 noob 中使用静态方法，可以作为一个参数传递给它：

```js
class Runoob {
  constructor(name) {
    this.name = name;
  }
  static hello(x) {
    return "Hello " + x.name;
  }
}
let noob = new Runoob("菜鸟教程");
document.getElementById("demo").innerHTML = Runoob.hello(noob);
```





## js对象

​		`JavaScript `中的所有事物都是对象：字符串、数值、数组、函数...

​		对象也是一个变量，但对象可以包含多个值（多个变量），每个值以 **name:value** 对呈现。

​		 `JavaScript `对象是属性和方法的容器。

### 对象属性

​		键值对在 JavaScript 对象通常称为 **对象属性**。

> 对象键值对的写法类似于：
>
> - PHP 中的关联数组
> - Python 中的字典
> - C 语言中的哈希表
> - Java 中的哈希映射
> - Ruby 和 Perl 中的哈希表

```js
var car = {name:"Fiat", model:500, color:"white"};

// JavaScript 对象可以跨越多行，空格跟换行不是必须的：
var person = {
    firstName:"John",
    lastName:"Doe",
    age:50,
    eyeColor:"blue"
};
```

​		你可以通过两种方式访问对象属性:

```js
person.lastName;
person["lastName"];
```

​			可以通过为对象赋值，向已有对象添加新属性

```js
person.firstname="John";
person.lastname="Doe";
person.age=30;
person.eyecolor="blue";

x=person.firstname;
```

​		JavaScript for...in 语句循环遍历对象的属性。

```js
var person={fname:"John",lname:"Doe",age:25}; 
 
for (x in person)
{
    txt=txt + person[x];
}
```

​		JavaScript 的对象是可变的，它们是通过引用来传递的。

```js
var y = person;  // 不会创建 person 的副本，是引用

var person = {firstName:"John", lastName:"Doe", age:50, eyeColor:"blue"}
var x = person;
x.age = 10;           //  x.age 和 person.age 都会改变
```



#### constructor 属性

​		**constructor** 属性返回所有 JavaScript 变量的构造函数。

```js
"John".constructor                 // 返回函数 String()  { [native code] }
(3.14).constructor                 // 返回函数 Number()  { [native code] }
false.constructor                  // 返回函数 Boolean() { [native code] }
[1,2,3,4].constructor              // 返回函数 Array()   { [native code] }
{name:'John', age:34}.constructor  // 返回函数 Object()  { [native code] }
new Date().constructor             // 返回函数 Date()    { [native code] }
function () {}.constructor         // 返回函数 Function(){ [native code] }

function isArray(myArray) {
    return myArray.constructor.toString().indexOf("Array") > -1;
}

function isDate(myDate) {
    return myDate.constructor.toString().indexOf("Date") > -1;
}
```



### 对象方法

​		对象的方法定义了一个函数，并作为对象的属性存储。

​		对象方法通过添加 () 调用 (作为一个函数)。

```js
// 创建对象方法：
var objectName = {
   ...
    methodName : function() {
    // 代码 
	}
}


objectName.methodName()
```

​		通常 fullName() 是作为 person 对象的一个方法， fullName 是作为一个属性。

​		如果使用 fullName 属性，不添加 **()**, 它会返回函数的定义：

​		在构造器函数内部定义对象的方法：

```js
function person(firstname,lastname,age,eyecolor)
{
    this.firstname=firstname;
    this.lastname=lastname;
    this.age=age;
    this.eyecolor=eyecolor;

    this.changeName=changeName;
    function changeName(name)
    {
        this.lastname=name;
    }
}
```



### this

​		面向对象语言中 `this `表示当前对象的一个引用。

​		但在 `JavaScript `中 `this `不是固定不变的，它会随着执行环境的改变而改变。

- 在方法中，`this `表示该方法所属的对象。

  ```js
  var person = {
    firstName: "John",
    lastName : "Doe",
    id       : 5566,
    fullName : function() {
      return this.firstName + " " + this.lastName; // 在对象方法中， this 指向调用它所在方法的对象。
    }
  };
  ```

- 如果单独使用，`this `表示全局对象。

  ​	在浏览器中，window 就是该全局对象为 [**object Window**]:

  ```js
  var x = this;
  
  "use strict";
  var x = this;
  ```

- 在函数中，`this `表示全局对象。

  ​	在函数中，函数的所属者默认绑定到 this 上。

  ​	在浏览器中，window 就是该全局对象为 [**object Window**]:

  ```js
  function myFunction() {
    return this;
  }
  ```

- 在函数中，在严格模式下，`this `是未定义的(`undefined`)。

  ```js
  "use strict";
  function myFunction() {
    return this;
  }
  ```

- 在事件中，`this `表示接收事件的元素。

  ​	在 HTML 事件句柄中，this 指向了接收事件的 HTML 元素：

  ```html
  <button onclick="this.style.display='none'">
  点我后我就消失了
  </button>
  ```

- 类似 `call()` 和 `apply() `方法可以将 `this `引用到任何对象。

  ​	在 `JavaScript `中函数也是对象，对象则有方法，`apply `和 `call `就是函数对象的方法。这两个方法异常强大，他们允许切换函数执行的上下文环境（`context`），即 `this `绑定的对象。

  ```js
  var person1 = {
    fullName: function() {
      return this.firstName + " " + this.lastName;
    }
  }
  var person2 = {
    firstName:"John",
    lastName: "Doe",
  }
  person1.fullName.call(person2);  // 返回 "John Doe"
  ```



### 构造器 constructor

```js
function Person(first, last, age, eyecolor) {
  this.firstName = first;
  this.lastName = last;
  this.age = age;
  this.eyeColor = eyecolor;
}
 
var myFather = new Person("John", "Doe", 50, "blue");
var myMother = new Person("Sally", "Rally", 48, "green");
```

​		在一个已存在构造器的对象中是不能添加新的属性.要添加一个新的属性需要在在构造器函数中添加：

```js
Person.nationality = "English"; // err

function Person(first, last, age, eyecolor) {
  this.firstName = first;
  this.lastName = last;
  this.age = age;
  this.eyeColor = eyecolor;
  this.nationality = "English";
}
```





### 内建对象

​		`JavaScript `提供多个内建对象，比如 `String`、`Date`、`Array `等等。

​		 对象只是带有属性和方法的特殊数据类型。

- 布尔型可以是一个对象。
- 数字型可以是一个对象。
- 字符串也可以是一个对象
- 日期是一个对象
- 数学和正则表达式也是对象
- 数组是一个对象
- 甚至函数也可以是对象

### 创建 JavaScript 对象

创建新对象有两种不同的方法：

- 使用 Object 定义并创建对象的实例。
- 使用函数来定义对象，然后创建新的对象实例。

#### 使用 Object

​		在 `JavaScript `中，几乎所有的对象都是 `Object `类型的实例，它们都会从 `Object.prototype` 继承属性和方法。

​		`Object `构造函数创建一个对象包装器。

​		`Object `构造函数，会根据给定的参数创建对象，具体有以下情况：

- 如果给定值是 `null `或 `undefined`，将会创建并返回一个空对象。
- 如果传进去的是一个基本类型的值，则会构造其包装类型的对象。
- 如果传进去的是引用类型的值，仍然会返回这个值，经他们复制的变量保有和源对象相同的引用地址。
- 当以非构造函数形式被调用时，`Object `的行为等同于 `new Object()`。

```js
// 以构造函数形式来调用
new Object([value])

// 等价于 o = new Boolean(true);
var o = new Object(true);

person=new Object();
person.firstname="John";
person.lastname="Doe";
person.age=50;
person.eyecolor="blue";
```

​		也可以使用对象字面量来创建对象

```js
{ name1 : value1, name2 : value2,...nameN : valueN }

person={firstname:"John",lastname:"Doe",age:50,eyecolor:"blue"};
```

#### 使用对象构造器

​		使用函数来构造对象：

```js
function person(firstname,lastname,age,eyecolor)
{
    this.firstname=firstname;
    this.lastname=lastname;
    this.age=age;
    this.eyecolor=eyecolor;
}

// 在JavaScript中，this通常指向的是我们正在执行的函数本身，或者是指向该函数所属的对象（运行时）

var myFather=new person("John","Doe",50,"blue");
var myMother=new person("Sally","Rally",48,"green");
```

​	

### prototype

​		所有的 JavaScript 对象都会从一个 prototype（原型对象）中继承属性和方法：

- `Date` 对象从 `Date.prototype` 继承。
- `Array` 对象从 `Array.prototype` 继承。
- `Person` 对象从 `Person.prototype` 继承。

​		所有 `JavaScript `中的对象都是位于原型链顶端的 `Object `的实例。

​		`JavaScript `对象有一个指向一个原型对象的链。当试图访问一个对象的属性时，它不仅仅在该对象上搜寻，还会搜寻该对象的原型，以及该对象的原型的原型，依次层层向上搜索，直到找到一个名字匹配的属性或到达原型链的末尾。

​		有的时候我们想要在所有已经存在的对象添加新的属性或方法。另外，有时候我们想要在对象的构造函数中添加属性或方法。使用 `prototype `属性就可以给对象的构造函数添加新的属性：

```js
function Person(first, last, age, eyecolor) {
  this.firstName = first;
  this.lastName = last;
  this.age = age;
  this.eyeColor = eyecolor;
}
 
Person.prototype.nationality = "English";
```

​		也可以使用 `prototype `属性给对象的构造函数添加新的方法：

```js
function Person(first, last, age, eyecolor) {
  this.firstName = first;
  this.lastName = last;
  this.age = age;
  this.eyeColor = eyecolor;
}
 
Person.prototype.name = function() {
  return this.firstName + " " + this.lastName;
};
```



## 函数

​		函数是由事件驱动的或者当它被调用时执行的可重复使用的代码块。

```js
function functionName(parameters) {
 //  执行的代码
}
```

### Function() 构造函数

​		在以上实例中，我们了解到函数通过关键字 **function** 定义。

​		函数同样可以通过内置的 `JavaScript `函数构造器`Function()`定义。

```js
var myFunction = new Function("a", "b", "return a * b");

//实际上，你不必使用构造函数。
var myFunction = function (a, b) {return a * b};
```

>  在 JavaScript 中，很多时候，你需要避免使用 **new** 关键字。

​		函数声明可以提升。

```js
myFunction(5);

function myFunction(y) {
    return y * y;
}
```



### 自调用函数

​		表达式后面紧跟 () ，则会自动调用。

```js
(function () {
    var x = "Hello!!";      // 我将调用自己
})();
```

### 函数是对象

​		在 JavaScript 中使用 **typeof** 操作符判断函数类型将返回 "function" 。

​		JavaScript 函数有 **属性** 和 **方法**。

- `arguments.length `属性返回函数调用过程接收到的参数个数
- `toString() `方法将函数作为一个字符串返回



### 箭头函数

​		ES6 新增了箭头函数。

```js
(参数1, 参数2, …, 参数N) => { 函数声明 }

(参数1, 参数2, …, 参数N) => 表达式(单一)
// 相当于：(参数1, 参数2, …, 参数N) =>{ return 表达式; }

当只有一个参数时，圆括号是可选的：
(单一参数) => {函数声明}
单一参数 => {函数声明}

没有参数的函数应该写成一对圆括号:
() => {函数声明}
```

```js
// ES5
var x = function(x, y) {
     return x * y;
}
 
// ES6
const x = (x, y) => x * y;
```

- 有的箭头函数都没有自己的 **this**。 不适合定义一个 **对象的方法**。

  ​	当我们使用箭头函数的时候，箭头函数会默认帮我们绑定外层 this 的值，所以在箭头函数中 this 的值和外层的 this 是一样的

- 箭头函数是不能提升的，所以需要在使用之前定义。

- 使用 **const** 比使用 **var** 更安全，因为函数表达式始终是一个常量

- 如果函数部分只是一个语句，则可以省略 return 关键字和大括号 {}，这样做是一个比较好的习惯:



### 参数

#### 默认参数

​		ES6 支持函数带有默认参数

```js
function myFunction(x, y = 10) {
    // y is 10 if not passed or undefined
    return x + y;
}
 
myFunction(0, 2) // 输出 2
myFunction(5); // 输出 15, y 参数的默认值
```

#### arguments 对象

​		`JavaScript `函数有个内置的对象 `arguments `对象。`arguments `对象包含了函数调用的参数数组。

```js
x = findMax(1, 123, 500, 115, 44, 88);
 
function findMax() {
    var i, max = arguments[0];
    
    if(arguments.length < 2) return max;
 
    for (i = 0; i < arguments.length; i++) {
        if (arguments[i] > max) {
            max = arguments[i];
        }
    }
    return max;
}
```



#### 通过值传递参数

在函数中调用的参数是函数的隐式参数。

JavaScript 隐式参数通过值来传递：函数仅仅只是获取值。

如果函数修改参数的值，不会修改显式参数的初始值（在函数外定义）。

隐式参数的改变在函数外是不可见的。



#### 通过对象传递参数

在JavaScript中，可以引用对象的值。

因此我们在函数内部修改对象的属性就会修改其初始的值。

修改对象属性可作用于函数外部（全局变量）。

修改对象属性在函数外是可见的。



### 调用

​		有 4 种调用方式。不同在于 **this** 的初始化。

​		在`Javascript`中，`this`指向函数执行时的当前对象。

- 函数不属于任何对象

  - 但是在 `JavaScript `中它始终是默认的全局对象。

  - 在 HTML 中默认的全局对象是 HTML 页面本身

  - 在 web 浏览器中全局对象是浏览器窗口（window 对象）。

    ```JS
    function myFunction() {
        return this;
    }
    myFunction();                // 返回 window 对象
    ```

- 将函数定义为对象

  ```JS
  var myObject = {
      firstName:"John",
      lastName: "Doe",
      fullName: function () {
          return this;
      }
  }
  myObject.fullName();          // 返回 [object Object] (所有者对象)
  ```

  **this**对象，拥有 JavaScript 代码。实例中 **this** 的值为 **myObject** 对象。

- 使用了 **new** 关键字, 则是调用了构造函数。

  ​	构造函数的调用会创建一个新的对象。新对象会继承构造函数的属性和方法。

  ```JS
  // 构造函数:
  function myFunction(arg1, arg2) {
      this.firstName = arg1;
      this.lastName  = arg2;
  }
   
  // This    creates a new object
  var x = new myFunction("John","Doe");
  x.firstName;                             // 返回 "John"
  ```

- 函数方法

  ​	**call()** 和 **apply()** 是预定义的函数方法。 两个方法可用于调用函数，两个方法的第一个参数必须是对象本身。

  ```js
  function myFunction(a, b) {
      return a * b;
  }
  myArray = [10, 2];
  myObject = myFunction.apply(myObject, myArray);  // 返回 20
  ```



### 闭包

​		闭包是一种保护私有变量的机制，在函数执行时形成私有的作用域，保护里面的私有变量不受外界干扰。直观的说就是形成一个不销毁的栈环境。

```js
var add = (function () {
    var counter = 0;
    return function () {return counter += 1;}
})();
 
add();
add();
add();
 // 计数器为 3
```





## 严格模式(use strict)

​		"use strict" 指令在 JavaScript 1.8.5 (ECMAScript5) 中新增。

​		它不是一条语句，但是是一个字面量表达式，在 JavaScript 旧版本中会被忽略。

​		严格模式通过在脚本或函数的头部添加` use strict; `表达式来声明。

```js

"use strict";
myFunction();

function myFunction() {
    y = 3.14;   // 报错 (y 未定义)
}
```

为什么使用严格模式:

- 消除Javascript语法的一些不合理、不严谨之处，减少一些怪异行为;

- 消除代码运行的一些不安全之处，保证代码运行的安全；
- 提高编译器效率，增加运行速度；
- 为未来新版本的Javascript做好铺垫。

​		"严格模式"体现了Javascript更合理、更安全、更严谨的发展方向，包括IE 10在内的主流浏览器，都已经支持它，许多大项目已经开始全面拥抱它。

​		另一方面，同样的代码，在"严格模式"中，可能会有不一样的运行结果；一些在"正常模式"下可以运行的语句，在"严格模式"下将不能运行。

### 严格模式的限制

- 不允许使用未声明的变量

  ```js
  "use strict";
  x = 3.14;                // 报错 (x 未定义)
  
  "use strict";
  x = {p1:10, p2:20};      // 报错 (x 未定义)
  ```

- 不允许删除变量或对象

  ```js
  "use strict";
  var x = 3.14;
  delete x;                // 报错
  ```

- 不允许删除函数。

  ```js
  "use strict";
  function x(p1, p2) {};
  delete x;                // 报错 
  ```

- 不允许变量重名:

  ```js
  "use strict";
  function x(p1, p1) {};   // 报错
  ```

- 不允许使用八进制:

  ```js
  "use strict";
  var x = 010;             // 报错
  ```

- 不允许使用转义字符:

  ```js
  "use strict";
  var x = \010;            // 报错
  ```

- 不允许对只读属性赋值:

  ```js
  "use strict";
  var obj = {};
  Object.defineProperty(obj, "x", {value:0, writable:false});
  
  obj.x = 3.14;            // 报错
  ```

- 不允许对一个使用getter方法读取的属性进行赋值

  ```js
  "use strict";
  var obj = {get x() {return 0} };
  
  obj.x = 3.14;            // 报错
  ```

- 不允许删除一个不允许删除的属性：

  ```js
  "use strict";
  delete Object.prototype; // 报错
  ```

- 变量名不能使用 "eval" 字符串:

  ```js
  "use strict";
  var eval = 3.14;         // 报错
  ```

- 变量名不能使用 "arguments" 字符串:

  ```js
  "use strict";
  var arguments = 3.14;    // 报错
  ```

- 不允许使用以下这种语句:

  ```js
  "use strict";
  with (Math){x = cos(2)}; // 报错
  ```

- 由于一些安全原因，在作用域 eval() 创建的变量不能被调用：

  ```js
  "use strict";
  eval ("var x = 2");
  alert (x);               // 报错
  ```

- 禁止this关键字指向全局对象。

  ```js
  function f(){
      return !this;
  } 
  // 返回false，因为"this"指向全局对象，"!this"就是false
  
  function f(){ 
      "use strict";
      return !this;
  } 
  // 返回true，因为严格模式下，this的值为undefined，所以"!this"为true。
  ```

  使用构造函数时，如果忘了加new，this不再指向全局对象，而是报错。

  ```js
  function f(){
      "use strict";
      this.a = 1;
  };
  f();// 报错，this未定义
  ```

### 保留关键字

为了向将来Javascript的新版本过渡，严格模式新增了一些保留关键字：

- implements
- interface
- let
- package
- private
- protected
- public
- static
- yield

"use strict";
var public = 1500;   // 报错





# JavaScript 代码规范

------

所有的 JavaScript 项目适用同一种规范。

------

## JavaScript 代码规范

代码规范通常包括以下几个方面:

- 变量和函数的命名规则
- 空格，缩进，注释的使用规则。
- 其他常用规范……

规范的代码可以更易于阅读与维护。

代码规范一般在开发前规定，可以跟你的团队成员来协商设置。

------

## 变量名

变量名推荐使用驼峰法来命名(**camelCase**):

firstName = "John";
lastName = "Doe";

price = 19.90;
tax = 0.20;

fullPrice = price + (price * tax);

------

## 空格与运算符

通常运算符 ( = + - * / ) 前后需要添加空格:

### 实例:

var x = y + z;
var values = ["Volvo", "Saab", "Fiat"];

------

## 代码缩进

通常使用 4 个空格符号来缩进代码块：

### 函数:

function toCelsius(fahrenheit) {
  return (5 / 9) * (fahrenheit - 32);
}

| ![Note](https://www.runoob.com/images/lamp.jpg) | 不推荐使用 TAB 键来缩进，因为不同编辑器 TAB 键的解析不一样。 |
| ----------------------------------------------- | ------------------------------------------------------------ |
|                                                 |                                                              |

------

## 语句规则

简单语句的通用规则:

- 一条语句通常以分号作为结束符。

### 实例:

var values = ["Volvo", "Saab", "Fiat"];

var person = {
  firstName: "John",
  lastName: "Doe",
  age: 50,
  eyeColor: "blue"
};

复杂语句的通用规则:

- 将左花括号放在第一行的结尾。
- 左花括号前添加一空格。
- 将右花括号独立放在一行。
- 不要以分号结束一个复杂的声明。

### 函数:

function toCelsius(fahrenheit) {
  return (5 / 9) * (fahrenheit - 32);
}

### 循环:

for (i = 0; i < 5; i++) {
  x += i;
}

### 条件语句:

if (time < 20) {
  greeting = "Good day";
} else {
  greeting = "Good evening";
}

------

## 对象规则

对象定义的规则:

- 将左花括号与类名放在同一行。
- 冒号与属性值间有个空格。
- 字符串使用双引号，数字不需要。
- 最后一个属性-值对后面不要添加逗号。
- 将右花括号独立放在一行，并以分号作为结束符号。

### 实例:

var person = {
  firstName: "John",
  lastName: "Doe",
  age: 50,
  eyeColor: "blue"
};

短的对象代码可以直接写成一行:

### 实例:

var person = {firstName:"John", lastName:"Doe", age:50, eyeColor:"blue"};

------

## 每行代码字符小于 80

为了便于阅读每行字符建议小于数 80 个。

如果一个 JavaScript 语句超过了 80 个字符，建议在 运算符或者逗号后换行。

### 实例:

document.getElementById("demo").innerHTML =
  "Hello Runoob.";

[尝试一下 »](https://www.runoob.com/try/try.php?filename=tryjs_line_break)

------

## 命名规则

一般很多代码语言的命名规则都是类似的，例如:

- 变量和函数为小驼峰法标识, 即除第一个单词之外，其他单词首字母大写（ **lowerCamelCase**）
- 全局变量为大写 (**UPPERCASE** )
- 常量 (如 PI) 为大写 (**UPPERCASE** )

变量命名你是否使用这几种规则： **hyp-hens**, **camelCase**, 或 **under_scores** ?

**HTML 和 CSS 的横杠(-)字符:**

HTML5 属性可以以 data- (如：data-quantity, data-price) 作为前缀。

CSS 使用 - 来连接属性名 (font-size)。

| ![Note](https://www.runoob.com/images/lamp.jpg) | - 通常在 JavaScript 中被认为是减法，所以不允许使用。 |
| ----------------------------------------------- | ---------------------------------------------------- |
|                                                 |                                                      |

**下划线:**

很多程序员比较喜欢使用下划线(如：date_of_birth), 特别是在 SQL 数据库中。

PHP 语言通常都使用下划线。

**帕斯卡拼写法(PascalCase):**

帕斯卡拼写法(PascalCase) 在 C 语言中语言较多。

驼峰法：

JavaScript 中通常推荐使用驼峰法，jQuery 及其他 JavaScript 库都使用驼峰法。

| ![Note](https://www.runoob.com/images/lamp.jpg) | 变量名不要以 $ 作为开始标记，会与很多 JavaScript 库冲突。 |
| ----------------------------------------------- | --------------------------------------------------------- |
|                                                 |                                                           |

------

## HTML 载入外部 JavaScript 文件

使用简洁的格式载入 JavaScript 文件 ( type 属性不是必须的):

<script src="myscript.js">

------

## 使用 JavaScript 访问 HTML 元素

一个糟糕的 HTML 格式可能会导致 JavaScript 执行错误。

以下两个 JavaScript 语句会输出不同结果:

### 实例

var obj = getElementById("Demo")

var obj = getElementById("demo")

[尝试一下 »](https://www.runoob.com/try/try.php?filename=tryhtml_syntax_javascript)

HTML 与 JavaScript 尽量使用相同的命名规则。

[访问 HTML(5) 代码规范](https://www.runoob.com/html/html5-syntax.html)。

------

## 文件扩展名

HTML 文件后缀可以是 **.html** (或 **.htm**)。

CSS 文件后缀是 **.css** 。

JavaScript 文件后缀是 **.js** 。

------

## 使用小写文件名

大多 Web 服务器 (Apache, Unix) 对大小写敏感： london.jpg 不能通过 London.jpg 访问。

其他 Web 服务器 (Microsoft, IIS) 对大小写不敏感： london.jpg 可以通过 London.jpg 或 london.jpg 访问。

你必须保持统一的风格，我们建议统一使用小写的文件名。