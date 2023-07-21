# js HTML DOM

​		通过 `HTML DOM`，`JavaScript `可访问 `HTML `文档的所有元素。

---

​		当网页被加载时，浏览器会创建页面的文档对象模型（Document Object Model）。

​		**HTML DOM** 模型被构造为**对象**的树：

![DOM HTML tree](https://www.runoob.com/images/pic_htmltree.gif)

​		通过可编程的对象模型，JavaScript 获得了足够的能力来创建动态的 HTML。

- JavaScript 能够改变页面中的所有 HTML 元素
- JavaScript 能够改变页面中的所有 HTML 属性
- JavaScript 能够改变页面中的所有 CSS 样式
- JavaScript 能够对页面中的所有事件做出反应



## 查找 HTML 元素

- 通过 id 找到 HTML 元素
- 通过标签名找到 HTML 元素
- 通过类名找到 HTML 元素

### 通过 id 查找 HTML 元素

```js
var x=document.getElementById("intro");
```

### 通过标签名查找 HTML 元素

​		方法返回 [HTMLCollection](https://www.runoob.com/jsref/dom-htmlcollection.html) 对象。

```js
var x=document.getElementById("main");
var y=x.getElementsByTagName("p");
```



###  通过类名找到 HTML 元素

```js
var x=document.getElementsByClassName("intro");
```



### HTMLCollection

​		HTMLCollection 是 HTML 元素的集合。类似一个包含 HTML 元素的数组列表。

| 属性 / 方法                                                  | 描述                                               |
| :----------------------------------------------------------- | :------------------------------------------------- |
| [item()](https://www.runoob.com/jsref/met-htmlcollection-item.html) | 返回 HTMLCollection 中指定索引的元素。             |
| [length](https://www.runoob.com/jsref/prop-htmlcollection-length.html) | 返回 HTMLCollection 中元素的数量。                 |
| [namedItem()](https://www.runoob.com/jsref/met-htmlcollection-nameditem.html) | 返回 HTMLCollection 中指定 ID 或 name 属性的元素。 |

```js
var myCollection = document.getElementsByTagName("p");
var i;
for (i = 0; i < myCollection.length; i++) {
    myCollection[i].style.backgroundColor = "red";
}
```

​		**HTMLCollection 不是一个数组！**HTMLCollection 看起来可能是一个数组，但其实不是。你可以像数组一样，使用索引来获取元素。HTMLCollection 无法使用数组的方法： valueOf(), pop(), push(), 或 join() 。

​		`getElementsByTagName `和 `getElementsByClassName `这两个方法查找多个 `dom `元素，返回的是 `htmlcollection `类型，是伪数组而不是真数组，故不能使用数组的方法。

​		我们可以使用数组原型配合 slice 方法，利用 call，apply，bind 方法将伪数组转为真数组。

```js
var x=document.getElementById("main");
var y=x.getElementsByTagName("p");
console.log(y)//在控制台我们可以看到原型proto为htmlcollection，是伪数组
//伪数组转为真数组方法1
console.log(Array.prototype.slice.call(y))//在控制台我们可以看到原型proto为Array(0)，是真数组

//伪数组转为真数组方法2
console.log(Array.prototype.slice.apply(y))//在控制台我们可以看到原型proto为Array(0)，是真数组

//伪数组转为真数组方法3
console.log(Array.prototype.slice.bind(y)())//在控制台我们可以看到原型proto为Array(0)，是真数组
```



### NodeList

​		**NodeList** 对象是一个从文档中获取的节点列表 (集合) 。NodeList 对象类似 [HTMLCollection](https://www.runoob.com/js/js-htmldom-elements.html) 对象。

​		一些旧版本浏览器中的方法（如：**getElementsByClassName()**）返回的是 NodeList 对象，而不是 HTMLCollection 对象。

​		所有浏览器的 **childNodes** 属性返回的是 NodeList 对象。大部分浏览器的 **querySelectorAll()** 返回 NodeList 对象。

```js
var myNodeList = document.querySelectorAll("p");
```

- NodeList 中的元素可以通过索引(以 0 为起始位置)来访问。

  ```js
  y = myNodeList[1];
  ```

- NodeList 对象 length 属性定义了节点列表中元素的数量。

  ```js
  var myNodelist = document.querySelectorAll("p");
  document.getElementById("demo").innerHTML = myNodelist.length;
  ```

```js
var myNodelist = document.querySelectorAll("p");
var i;
for (i = 0; i < myNodelist.length; i++) {
    myNodelist[i].style.backgroundColor = "red";
}
```

### HTMLCollection 与 NodeList 的区别

- [HTMLCollection](https://www.runoob.com/js/js-htmldom-collections.html) 是 HTML 元素的集合。NodeList 是一个文档节点的集合。
- NodeList 与 HTMLCollection 有很多类似的地方。
  - NodeList 与 HTMLCollection 都与数组对象有点类似，可以使用索引 (0, 1, 2, 3, 4, ...) 来获取元素。
  - NodeList 与 HTMLCollection 都有 length 属性。
- HTMLCollection 元素可以通过 name，id 或索引来获取。NodeList 只能通过索引来获取。
- 只有 NodeList 对象有包含属性节点和文本节点。

> **节点列表不是一个数组！**
>
> 节点列表看起来可能是一个数组，但其实不是。
>
> 你可以像数组一样，使用索引来获取元素。
>
> 节点列表无法使用数组的方法： valueOf(), pop(), push(), 或 join() 。

## 修改HTML

- document.write() 可用于直接向 HTML 输出流写内容。

  ```html
  <script>
  document.write(Date());
  </script>
  ```

   绝对不要在文档(DOM)加载完成之后使用 document.write()。这会覆盖该文档。

- 使用 innerHTML 属性

  ```html
  // document.getElementById(id).innerHTML=新的 HTML
  <p id="p1">Hello World!</p>
  <script>
  document.getElementById("p1").innerHTML="新文本!";
  </script>
  ```

- 改变 HTML 元素的属性

  ```html
  // document.getElementById(id).attribute=新属性值
  <img id="image" src="smiley.gif">
  
  <script>
  document.getElementById("image").src="landscape.jpg";
  </script>
  ```



## add/del HTML元素节点

### 添加元素

- appendChild() 添加新元素到尾部。

  创建新的 HTML 元素 (节点)需要先创建一个元素，然后在已存在的元素中添加它。

  ```html
  <div id="div1">
  <p id="p1">这是一个段落。</p>
  <p id="p2">这是另外一个段落。</p>
  </div>
   
  <script>
      // 1. 创建 <p> 元素:
      var para = document.createElement("p");
      // 2. 为 <p> 元素创建一个新的文本节点：
      var node = document.createTextNode("这是一个新的段落。");
      // 3. 文本节点添加到 <p> 元素中：
      para.appendChild(node);
  
      // 4. 在一个已存在的元素中添加 p 元素。
      var element = document.getElementById("div1");
      element.appendChild(para);
  </script>
  ```

- insertBefore() 将新元素添加到开始位置

  ```html
  <div id="div1">
  <p id="p1">这是一个段落。</p>
  <p id="p2">这是另外一个段落。</p>
  </div>
   
  <script>
  var para = document.createElement("p");
  var node = document.createTextNode("这是一个新的段落。");
  para.appendChild(node);
   
  var element = document.getElementById("div1");
  var child = document.getElementById("p1");
  element.insertBefore(para, child);
  </script>
  ```

### 移除元素

​		你需要知道该元素的父元素

```html
<div id="div1">
    <p id="p1">这是一个段落。</p>
    <p id="p2">这是另外一个段落。</p>
</div>
 
<script>
var parent = document.getElementById("div1");
var child = document.getElementById("p1");
parent.removeChild(child);
</script>
```

​		已知要查找的子元素，然后查找其父元素，再删除这个子元素

```js
var child = document.getElementById("p1");
child.parentNode.removeChild(child);
```



### 替换元素

​		replaceChild() 方法来替换 HTML DOM 中的元素

```html
<div id="div1">
    <p id="p1">这是一个段落。</p>
    <p id="p2">这是另外一个段落。</p>
</div>
 
<script>
    var para = document.createElement("p");
    var node = document.createTextNode("这是一个新的段落。");
    para.appendChild(node);

    var parent = document.getElementById("div1");
    var child = document.getElementById("p1");
    parent.replaceChild(para, child);
</script>
```



## 修改css

```html
// document.getElementById(id).style.property=新样式
<p id="p1">Hello World!</p>
<p id="p2">Hello World!</p>
<script>
document.getElementById("p2").style.color="blue";
document.getElementById("p2").style.fontFamily="Arial";
document.getElementById("p2").style.fontSize="larger";
</script>
```



## 响应事件

​		HTML DOM 允在事件发生时执行 JavaScript

​		HTML 事件的例子：

- 当用户点击鼠标时
- 当网页已加载时
- 当图像已加载时
- 当鼠标移动到元素上时
- 当输入字段被改变时
- 当提交 HTML 表单时
- 当用户触发按键时

```html
<button type="button"
	onclick="document.getElementById('id1').style.color='red'">
	点我!</button>
```

### HTML 事件属性

​		如需向 HTML 元素分配 事件，您可以使用事件属性。

```html
<button onclick="displayDate()">点这里</button>
// 向 button 元素分配 onclick 事件：
```

### HTML DOM 分配事件

```html
<script>
document.getElementById("myBtn").onclick=function(){displayDate()};
</script>
```



### 事件

#### onload 和 onunload 事件

- `onload `和 `onunload `事件会在用户进入或离开页面时被触发。
- `onload `事件可用于检测访问者的浏览器类型和浏览器版本，并基于这些信息来加载网页的正确版本。
- `onload `和 `onunload `事件可用于处理 `cookie`。

```html
<body onload="checkCookies()">
```

#### onchange 事件

​		常结合对输入字段的验证来使用。

```html
<input type="text" id="fname" onchange="upperCase()">
```



#### onmouseover 和 onmouseout 事件

​		可用于在用户的鼠标移至 HTML 元素上方或移出元素时触发函数。

```html
<div onmouseover="mOver(this)" onmouseout="mOut(this)" style="background-color:#D94A38;width:120px;height:20px;padding:40px;">Mouse Over Me</div>
<script>
function mOver(obj){
	obj.innerHTML="Thank You"
}
function mOut(obj){
	obj.innerHTML="Mouse Over Me"
}
</script>
```



#### onmousedown、onmouseup 以及 onclick 事件

​		onmousedown, onmouseup 以及 onclick 构成了鼠标点击事件的所有部分。

- 首先当点击鼠标按钮时，会触发 onmousedown 事件
- 当释放鼠标按钮时，会触发 onmouseup 事件
- 最后，当完成鼠标点击时，会触发 onclick 事件。



### eventlistener

​		 IE 8 及更早 IE 版本，Opera 7.0及其更早版本不支持` addEventListener() `和 `removeEventListener() `方法。但是，对于这类浏览器版本可以使用 detachEvent() 方法来移除事件句柄:

​		跨浏览器解决方法:

```js
var x = document.getElementById("myBtn");
if (x.addEventListener) {          // 所有主流浏览器，除了 IE 8 及更早版本
  x.addEventListener("click", myFunction);
} else if (x.attachEvent) {         // IE 8 及更早版本
  x.attachEvent("onclick", myFunction);
}
```



#### addEventListener()

​		用于向指定元素添加事件句柄。

```js
element.addEventListener(event, function, useCapture);
```

> - 第一个参数是事件的类型 (如 "click" 或 "mousedown").
>
>   > 不要使用 "on" 前缀。 例如，使用 "click" ,而不是使用 "onclick"。
>
> - 第二个参数是事件触发后调用的函数。
>
> - 第三个参数是个布尔值用于描述事件是冒泡还是捕获。该参数是可选的。
>
>   ​	事件传递有两种方式：冒泡与捕获。事件传递定义了元素事件触发的顺序。
>
>   - 在 *冒泡* 中，内层 元素的事件会先被触发，然后再触发外层元素。
>   - 在 *捕获* 中，外层元素的事件会先被触发，然后才会触发内层元素的事件
>
>   ```js
>   document.getElementById("myDiv").addEventListener("click", myFunction, true);
>   ```

```js
element.addEventListener("click", function(){ alert("Hello World!"); });
```

- 不会覆盖已存在的事件句柄。

- 可以向一个元素添加多个事件句柄。

  ```js
  element.addEventListener("mouseover", myFunction);
  element.addEventListener("click", mySecondFunction);
  element.addEventListener("mouseout", myThirdFunction);
  ```

- 同个元素添加多个同类型的事件句柄.

  ```js
  element.addEventListener("click", myFunction);
  element.addEventListener("click", mySecondFunction);
  ```

- 向任何 DOM 对象添加事件监听，不仅仅是 HTML 元素。

  ​	addEventListener() 方法允许你在 HTML DOM 对象添加事件监听， HTML DOM 对象如： HTML 元素, HTML 文档, window 对象。或者其他支持的事件对象如: xmlHttpRequest 对象。

  ```js
  window.addEventListener("resize", function(){
      document.getElementById("demo").innerHTML = sometext;
  });
  ```

- 当传递参数值时，使用"匿名函数"调用带参数的函数：

  ```js
  element.addEventListener("click", function(){ myFunction(p1, p2); });
  ```

​		使用 addEventListener() 方法时, JavaScript 从 HTML 标记中分离开来，可读性更强， 在没有控制HTML标记时也可以添加事件监听。



#### removeEventListener()

​		使用 `removeEventListener()` 方法来移除由 `addEventListener() `方法添加的事件句柄:

```js
element.removeEventListener("mousemove", myFunction);
```



## cookie

​		Cookie 用于存储 web 页面的用户信息。

### 什么是 Cookie？

​		当 web 服务器向浏览器发送 web 页面时，在连接关闭后，服务端不会记录用户的信息。当浏览器从服务器上请求 web 页面时， 属于该页面的 cookie 会被添加到该请求中。服务端通过这种方式来获取用户的信息。

​		Cookie 是一些数据, 存储于你电脑上的文本文件中。用于解决 "如何记录客户端的用户信息":

- 当用户访问 web 页面时，他的名字可以记录在 cookie 中。
- 在用户下一次访问该页面时，可以在 cookie 中读取用户访问记录。

​		Cookie 以名/值对形式存储，``username=John Doe``

​		



### Cookie 字符串

​		`document.cookie` 属性看起来像一个普通的文本字符串，其实它不是。

- cookie `信息是以名/值对的形式展示的。
- 设置了新的 cookie，旧的 cookie 不会被覆盖。 新 `cookie `将添加到 `document.cookie `中

### 创建Cookie

​		使用 **document.cookie** 属性来创建 、读取、及删除 cookie。

- 创建

  ```js
  document.cookie="username=John Doe";
  ```

- 过期

  还可以为 cookie 添加一个过期时间（以 UTC 或 GMT 时间）。

  默认情况下，cookie 在浏览器关闭时删除：

  ```js
  document.cookie="username=John Doe; expires=Thu, 18 Dec 2043 12:00:00 GMT";
  ```

- 路径

  用 path 参数告诉浏览器 cookie 的路径。默认情况下，cookie 属于当前页面。

  ```js
  document.cookie="username=John Doe; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";
  ```

  

### 读取cookie

​		将以字符串的方式返回所有的 cookie, `“cookie1=value; cookie2=value; cookie3=value;”`

```js
var x = document.cookie;
```

### 修改cookie

```js
document.cookie="username=John Smith; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";
```

### 删除cookie

您只需要设置 expires 参数为以前的时间即可.删除时不必指定 cookie 的值

```js
document.cookie = "username=; expires=Thu, 01 Jan 1970 00:00:00 GMT";
```



```js
function setCookie(cname,cvalue,exdays){
    var d = new Date();
    d.setTime(d.getTime()+(exdays*24*60*60*1000));
    var expires = "expires="+d.toGMTString();
    document.cookie = cname+"="+cvalue+"; "+expires;
}
function getCookie(cname){
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i=0; i<ca.length; i++) {
        var c = ca[i].trim();
        if (c.indexOf(name)==0) { return c.substring(name.length,c.length); }
    }
    return "";
}
function checkCookie(){
    var user=getCookie("username");
    if (user!=""){
        alert("欢迎 " + user + " 再次访问");
    }
    else {
        user = prompt("请输入你的名字:","");
          if (user!="" && user!=null){
            setCookie("username",user,30);
        }
    }
}
```

