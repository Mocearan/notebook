# js-inner-obj

---



## Number Object

在JavaScript中，数字不分为整数类型和浮点型类型，所有的数字都是由 浮点型类型。JavaScript 采用 IEEE754 标准定义的 64 位浮点格式表示数字，它能表示最大值（Number.MAX_VALUE）为 **±1.7976931348623157e+308**，最小值（Number.MIN_VALUE）为 **±5e-324**。

此格式用 64 位存储数值，其中 0 到 51 存储数字（片段），52 到 62 存储指数，63 位存储符号：

![img](https://www.runoob.com/wp-content/uploads/2013/08/490px-General_floating_point_frac.svg.png)

| 值 (aka Fraction/Mantissa) | 指数（Exponent）  | 符号（Sign） |
| :------------------------- | :---------------- | :----------- |
| 52 bits (0 - 51)           | 11 bits (52 - 62) | 1 bit (63)   |

![img](https://www.runoob.com/wp-content/uploads/2013/08/Float_mantissa_exponent.png)

​		整数（不使用小数点或指数计数法）最多为 15 位。小数的最大位数是 17，但是浮点运算并不总是 100% 准确：

```js
var x = 999999999999999;   // x 为 999999999999999
var y = 9999999999999999;  // y 为 10000000000000000

var x = 0.2+0.1; // 输出结果为 0.30000000000000004
```

​		如果前缀为 0，则 JavaScript 会把数值常量解释为八进制数，如果前缀为 0 和 "x"，则解释为十六进制数。

```js
var y = 0377;
var z = 0xFF;
```

​		以使用 toString() 方法 输出16进制、8进制、2进制。

```js
var myNumber=128;
myNumber.toString(16);   // 返回 80
myNumber.toString(8);    // 返回 200
myNumber.toString(2);    // 返回 10000000
```



### Infinity

​		当数字运算结果超过了JavaScript所能表示的数字上限（溢出），结果为一个特殊的无穷大（infinity）值，在JavaScript中以Infinity表示。同样地，当负数的值超过了JavaScript所能表示的负数范围，结果为负无穷大，在JavaScript中以-Infinity表示。

```js
myNumber=2;
while (myNumber!=Infinity)
{
    myNumber=myNumber*myNumber; // 重复计算直到 myNumber 等于 Infinity
}

var x = 2/0;
var y = -2/0;
```



### NaN		

​		NaN 属性是代表非数字值的特殊值。该属性用于指示某个值不是数字。可以把 Number 对象设置为该值，来指示其不是数字值。

```js
var x = 1000 / "Apple";
isNaN(x); // 返回 true
var y = 100 / "1000";
isNaN(y); // 返回 false

var x = 1000 / 0; // infinity
isNaN(x); // 返回 false
```



### 数字可以是数字或者对象

```js
var x = 123;
var y = new Number(123);
typeof(x) // 返回 Number
typeof(y) // 返回 Object

(x === y) // 为 false，因为 x 是一个数字，y 是一个对象
```



### Number 属性

| Number.MAX_VALUE         | 最大值                                                |
| ------------------------ | ----------------------------------------------------- |
| Number.MIN_VALUE         | 最小值                                                |
| Number.NaN               | 非数字                                                |
| Number.NEGATIVE_INFINITY | 负无穷，在溢出时返回                                  |
| Number.POSITIVE_INFINITY | 正无穷，在溢出时返回                                  |
| Number.EPSILON           | 表示 1 和比最接近 1 且大于 1 的最小 Number 之间的差别 |
| Number.MIN_SAFE_INTEGER  | 最小安全整数。                                        |
| Number.MAX_SAFE_INTEGER  | 最大安全整数。                                        |



### 数字方法

| Number.parseFloat()    | 将字符串转换成浮点数，和全局方法 [parseFloat()](https://www.runoob.com/jsref/jsref-parsefloat.html) 作用一致。 |
| ---------------------- | ------------------------------------------------------------ |
| Number.parseInt()      | 将字符串转换成整型数字，和全局方法 [parseInt()](https://www.runoob.com/jsref/jsref-parseint.html) 作用一致。 |
| Number.isFinite()      | 判断传递的参数是否为有限数字。                               |
| Number.isInteger()     | 判断传递的参数是否为整数。                                   |
| Number.isNaN()         | 判断传递的参数是否为 isNaN()。                               |
| Number.isSafeInteger() | 判断传递的参数是否为安全整数。                               |



### Number.prototype 方法

| toExponential() | 返回一个数字的指数形式的字符串，如：1.23e+2                  |
| --------------- | ------------------------------------------------------------ |
| toFixed()       | 返回指定小数位数的表示形式。`var a=123; b=a.toFixed(2); // b="123.00"` |
| toPrecision()   | 返回一个指定精度的数字。如下例子中，a=123 中，3会由于精度限制消失:`var a=123; b=a.toPrecision(2); // b="1.2e+2"` |



## String Object

​		一个字符串用于存储一系列字符。

- 一个字符串可以使用单引号或双引号：

  ```js
  var carname="Volvo XC60";
  var carname='Volvo XC60';
  ```

- 使用位置（索引）可以访问字符串中任何的字符

  ```js
  var character=carname[7];
  ```

- 可以在字符串中使用引号

  ```js
  var answer="It's alright";
  var answer="He is called 'Johnny'";
  var answer='He is called "Johnny"';
  ```

- 用转义字符(\)使用引号

  ```js
  var answer='It\'s alright';
  var answer="He is called \"Johnny\"";
  ```

  

### String 属性

- length

  ```js
  var txt="Hello World!";
  document.write(txt.length);
  
  var txt="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  document.write(txt.length);
  ```

  

- prototype

- constructor

### String 方法

- charAt()

- charCodeAt()

- concat()

- fromCharCode()

- indexOf()

  ​	定位字符串中某一个指定的字符首次出现的位置, 没找到对应的字符函数返回-1

  ```js
  var str="Hello world, welcome to the universe.";
  var n=str.indexOf("welcome");
  ```

- lastIndexOf()

- match()

  ​	查找字符串中特定的字符，并且如果找到的话，则返回这个字符

  ```js
  var str="Hello world!";
  document.write(str.match("world") + "<br>");
  document.write(str.match("World") + "<br>");
  document.write(str.match("world!"));
  ```

- replace()

  ​	用某些字符替换另一些字符

  ```js
  str="Please visit Microsoft!"
  var n=str.replace("Microsoft","Runoob");
  ```

  

- search()

- slice()

- split()

  ```js
  txt="a,b,c,d,e"   // String
  txt.split(",");   // 使用逗号分隔
  txt.split(" ");   // 使用空格分隔
  txt.split("|");   // 使用竖线分隔 
  ```

  

- substr()

- substring()

- toLowerCase()

- toUpperCase()

  ```js
  var txt="Hello World!";       // String
  var txt1=txt.toUpperCase();   // txt1 文本会转换为大写
  var txt2=txt.toLowerCase();   // txt2 文本会转换为小写
  ```

  

- valueOf()



## Date Object

​		日期对象用于处理日期和时间。

```js
var d = new Date();
var d = new Date(milliseconds); // 参数为毫秒
var d = new Date(dateString);
var d = new Date(year, month, day, hours, minutes, seconds, milliseconds);
```

- **milliseconds** 参数是一个 Unix 时间戳（Unix Time Stamp），它是一个整数值，表示自 1970 年 1 月 1 日 00:00:00 UTC（the Unix epoch）以来的毫秒数。
- **dateString** 参数表示日期的字符串值。
- **year, month, day, hours, minutes, seconds, milliseconds** 分别表示年、月、日、时、分、秒、毫秒。

```js
var today = new Date()
var d1 = new Date("October 13, 1975 11:13:00")
var d2 = new Date(79,5,24)
var d3 = new Date(79,5,24,11,33,0)
```

### 日期比较

```js
var x=new Date();
x.setFullYear(2100,0,14);
var today = new Date();

if (x>today)
{
    alert("今天是2100年1月14日之前");
}
else
{
    alert("今天是2100年1月14日之后");
}
```





### Date 属性

| [constructor](https://www.runoob.com/jsref/jsref-constructor-date.html) | 返回对创建此对象的 Date 函数的引用。 |
| ------------------------------------------------------------ | ------------------------------------ |
| [prototype](https://www.runoob.com/jsref/jsref-prototype-date.html) | 使您有能力向对象添加属性和方法。     |

### Date 方法

| [getDate()](https://www.runoob.com/jsref/jsref-getdate.html) | 从 Date 对象返回一个月中的某一天 (1 ~ 31)。                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [getDay()](https://www.runoob.com/jsref/jsref-getday.html)   | 从 Date 对象返回一周中的某一天 (0 ~ 6)。                     |
| [getFullYear()](https://www.runoob.com/jsref/jsref-getfullyear.html) | 从 Date 对象以四位数字返回年份。                             |
| [getHours()](https://www.runoob.com/jsref/jsref-gethours.html) | 返回 Date 对象的小时 (0 ~ 23)。                              |
| [getMilliseconds()](https://www.runoob.com/jsref/jsref-getmilliseconds.html) | 返回 Date 对象的毫秒(0 ~ 999)。                              |
| [getMinutes()](https://www.runoob.com/jsref/jsref-getminutes.html) | 返回 Date 对象的分钟 (0 ~ 59)。                              |
| [getMonth()](https://www.runoob.com/jsref/jsref-getmonth.html) | 从 Date 对象返回月份 (0 ~ 11)。                              |
| [getSeconds()](https://www.runoob.com/jsref/jsref-getseconds.html) | 返回 Date 对象的秒数 (0 ~ 59)。                              |
| [getTime()](https://www.runoob.com/jsref/jsref-gettime.html) | 返回 1970 年 1 月 1 日至今的毫秒数。                         |
| [getTimezoneOffset()](https://www.runoob.com/jsref/jsref-gettimezoneoffset.html) | 返回本地时间与格林威治标准时间 (GMT) 的分钟差。              |
| [getUTCDate()](https://www.runoob.com/jsref/jsref-getutcdate.html) | 根据世界时从 Date 对象返回月中的一天 (1 ~ 31)。              |
| [getUTCDay()](https://www.runoob.com/jsref/jsref-getutcday.html) | 根据世界时从 Date 对象返回周中的一天 (0 ~ 6)。               |
| [getUTCFullYear()](https://www.runoob.com/jsref/jsref-getutcfullyear.html) | 根据世界时从 Date 对象返回四位数的年份。                     |
| [getUTCHours()](https://www.runoob.com/jsref/jsref-getutchours.html) | 根据世界时返回 Date 对象的小时 (0 ~ 23)。                    |
| [getUTCMilliseconds()](https://www.runoob.com/jsref/jsref-getutcmilliseconds.html) | 根据世界时返回 Date 对象的毫秒(0 ~ 999)。                    |
| [getUTCMinutes()](https://www.runoob.com/jsref/jsref-getutcminutes.html) | 根据世界时返回 Date 对象的分钟 (0 ~ 59)。                    |
| [getUTCMonth()](https://www.runoob.com/jsref/jsref-getutcmonth.html) | 根据世界时从 Date 对象返回月份 (0 ~ 11)。                    |
| [getUTCSeconds()](https://www.runoob.com/jsref/jsref-getutcseconds.html) | 根据世界时返回 Date 对象的秒钟 (0 ~ 59)。                    |
| getYear()                                                    | 已废弃。 请使用 getFullYear() 方法代替。                     |
| [parse()](https://www.runoob.com/jsref/jsref-parse.html)     | 返回1970年1月1日午夜到指定日期（字符串）的毫秒数。           |
| [setDate()](https://www.runoob.com/jsref/jsref-setdate.html) | 设置 Date 对象中月的某一天 (1 ~ 31)。                        |
| [setFullYear()](https://www.runoob.com/jsref/jsref-setfullyear.html) | 设置 Date 对象中的年份（四位数字）。                         |
| [setHours()](https://www.runoob.com/jsref/jsref-sethours.html) | 设置 Date 对象中的小时 (0 ~ 23)。                            |
| [setMilliseconds()](https://www.runoob.com/jsref/jsref-setmilliseconds.html) | 设置 Date 对象中的毫秒 (0 ~ 999)。                           |
| [setMinutes()](https://www.runoob.com/jsref/jsref-setminutes.html) | 设置 Date 对象中的分钟 (0 ~ 59)。                            |
| [setMonth()](https://www.runoob.com/jsref/jsref-setmonth.html) | 设置 Date 对象中月份 (0 ~ 11)。                              |
| [setSeconds()](https://www.runoob.com/jsref/jsref-setseconds.html) | 设置 Date 对象中的秒钟 (0 ~ 59)。                            |
| [setTime()](https://www.runoob.com/jsref/jsref-settime.html) | setTime() 方法以毫秒设置 Date 对象。                         |
| [setUTCDate()](https://www.runoob.com/jsref/jsref-setutcdate.html) | 根据世界时设置 Date 对象中月份的一天 (1 ~ 31)。              |
| [setUTCFullYear()](https://www.runoob.com/jsref/jsref-setutcfullyear.html) | 根据世界时设置 Date 对象中的年份（四位数字）。               |
| [setUTCHours()](https://www.runoob.com/jsref/jsref-setutchours.html) | 根据世界时设置 Date 对象中的小时 (0 ~ 23)。                  |
| [setUTCMilliseconds()](https://www.runoob.com/jsref/jsref-setutcmilliseconds.html) | 根据世界时设置 Date 对象中的毫秒 (0 ~ 999)。                 |
| [setUTCMinutes()](https://www.runoob.com/jsref/jsref-setutcminutes.html) | 根据世界时设置 Date 对象中的分钟 (0 ~ 59)。                  |
| [setUTCMonth()](https://www.runoob.com/jsref/jsref-setutcmonth.html) | 根据世界时设置 Date 对象中的月份 (0 ~ 11)。                  |
| [setUTCSeconds()](https://www.runoob.com/jsref/jsref-setutcseconds.html) | setUTCSeconds() 方法用于根据世界时 (UTC) 设置指定时间的秒字段。 |
| setYear()                                                    | 已废弃。请使用 setFullYear() 方法代替。                      |
| [toDateString()](https://www.runoob.com/jsref/jsref-todatestring.html) | 把 Date 对象的日期部分转换为字符串。                         |
| toGMTString()                                                | 已废弃。请使用 toUTCString() 方法代替。                      |
| [toISOString()](https://www.runoob.com/jsref/jsref-toisostring.html) | 使用 ISO 标准返回字符串的日期格式。                          |
| [toJSON()](https://www.runoob.com/jsref/jsref-tojson.html)   | 以 JSON 数据格式返回日期字符串。                             |
| [toLocaleDateString()](https://www.runoob.com/jsref/jsref-tolocaledatestring.html) | 根据本地时间格式，把 Date 对象的日期部分转换为字符串。       |
| [toLocaleTimeString()](https://www.runoob.com/jsref/jsref-tolocaletimestring.html) | 根据本地时间格式，把 Date 对象的时间部分转换为字符串。       |
| [toLocaleString()](https://www.runoob.com/jsref/jsref-tolocalestring.html) | 根据本地时间格式，把 Date 对象转换为字符串。                 |
| [toString()](https://www.runoob.com/jsref/jsref-tostring-date.html) | 把 Date 对象转换为字符串。                                   |
| [toTimeString()](https://www.runoob.com/jsref/jsref-totimestring.html) | 把 Date 对象的时间部分转换为字符串。                         |
| [toUTCString()](https://www.runoob.com/jsref/jsref-toutcstring.html) | 根据世界时，把 Date 对象转换为字符串。实例：`var today = new Date(); var UTCstring = today.toUTCString();` |
| [UTC()](https://www.runoob.com/jsref/jsref-utc.html)         | 根据世界时返回 1970 年 1 月 1 日 到指定日期的毫秒数。        |
| [valueOf()](https://www.runoob.com/jsref/jsref-valueof-date.html) | 返回 Date 对象的原始值。                                     |

- [setFullYear()](https://www.runoob.com/jsref/jsref-setfullyear.html)  设置 Date 对象中的年份（四位数字）

  ```js
  var myDate=new Date();
  myDate.setFullYear(2010,0,14);// 第二个参数为月份， 0 到 11 之间的整数值，表示从一月到十二月
  ```

- [setDate()](https://www.runoob.com/jsref/jsref-setdate.html)  设置 Date 对象中月的某一天 (1 ~ 31)。

  ​	如果增加天数会改变月份或者年份，那么日期对象会自动完成这种转换。

  ```js
  var myDate=new Date();
  myDate.setDate(myDate.getDate()+5);
  ```

  

## Array  Object

​		创建一个数组，有三种方法。

下面的代码定义了一个名为 myCars的数组对象：

1. 常规方式:

   ```js
   var myCars=new Array();
   myCars[0]="Saab";   
   myCars[1]="Volvo";
   myCars[2]="BMW";
   ```

2. 简洁方式:

   ```js
   var myCars=new Array("Saab","Volvo","BMW");
   ```

3. 字面:

   ```js
   var myCars=["Saab","Volvo","BMW"];
   ```



​		所有的JavaScript变量都是对象。因此，可以在数组中有不同的变量类型。可以在一个数组中包含对象元素、函数、数组：

```js
myArray[0]=Date.now;
myArray[1]=myFunction;
myArray[2]=myCars;
```



### Array 属性

| [constructor](https://www.runoob.com/jsref/jsref-constructor-array.html) | 返回创建数组对象的原型函数。     |
| ------------------------------------------------------------ | -------------------------------- |
| [length](https://www.runoob.com/jsref/jsref-length-array.html) | 设置或返回数组元素的个数。       |
| [prototype](https://www.runoob.com/jsref/jsref-prototype-array.html) | 允许你向数组对象添加属性或方法。 |



### Array 方法