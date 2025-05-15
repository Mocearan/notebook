# GLSL ES

---

​		编写 `Shader `则需要用到 `OpenGL Shader Language`（简称 `GLSL`）和 `OpenGL ES Shading Language`（简称 `GLSL ES`）。`GLSL ES` 是在 `GLSL` 的基础上新增和删除了部分特性。



## 参考

[1]Khronos: *https://registry.khronos.org/OpenGL/index_gl.php*

[2]GL SL Wiki: *https://en.wikipedia.org/wiki/OpenGL_Shading_Language#cite_note-13*

[3]Khronos OpenGL ES Registry: *https://registry.khronos.org/OpenGL/index_es.php#otherextspecs*

[4]GLSL ES 文档: *https://registry.khronos.org/OpenGL/specs/es/3.0/GLSL_ES_Specification_3.00.pdf*

[5]OpenGL Shading Language]: *https://en.wikipedia.org/wiki/OpenGL_Shading_Language#cite_note-13*

[6]Khronos OpenGL Registry: *https://registry.khronos.org/OpenGL/index_gl.php*

[7]LearnOpenGL: *https://learnopengl.com/*

[8]The OpenGL ES Shading Language: *https://registry.khronos.org/OpenGL/specs/es/2.0/GLSL_ES_Specification_1.00.pdf*

[9]The OpenGL ES Shading Language: *https://registry.khronos.org/OpenGL/specs/es/3.0/GLSL_ES_Specification_3.00.pdf*



## Shader 结构

​		`GLSL ES` 是一种类 `C` 语言，编译过程基于 `C++` 标准的一个子集。

```c
#version version_number  // 预处理

type variable_name; // 不同类型的变量

// 限定符
attribute type attribute_name;
uniform type uniform_name;
varying type varying_name;

//  main 函数，着色器的入口点
void main()
{
  // 处理输入并进行一些图形操作
  ...
  gl_Position = weird_stuff_we_processed; // 结果输出到内置的输出变量 gl_Position 中
}
```



## 预处理

​		`GLSL ES` 中完整的预处理指令列表：

```c
#define
#undef
//  跟 C++ 预处理器的标准一致

#if
#ifdef
#ifndef
#else
#elif
#endif
//  跟 C++ 中的相同指令操作结果一样，但是下面三点需要注意的地方:
// 		这些宏的后面只能跟整数常量运算或者 define 定义的宏定义
// 		未被 define 定义的识别符不会被默认为 0，所以使用未定义的宏会触发错误。
// 		不支持字母常量。

#error // 输出错误信息放到 Shader 的 logInfo 中，所以可以结合 OpenGL ES API glGetShaderInfoLog 来自定义错误类型。
#pragma // 实现对编译器的控制，比如 #pragma optimize(on/off) 可以开关编译器优化能力，默认是开。
//				#pragma debug(on/off) 可以开关调试模式，默认是关。

#extension // 控制是否启用某些扩展功能。
	// 这个预处理指令以 #extension extension_name : behavior 的格式使用。
	// extension_name 对应扩展能力，比较常见的扩展有 GL_EXT_texture_rg 和 GL_EXT_shader_framebuffer_fetch
	// ehavior 描述了扩展的行为方式具体有以下表格几种：
    	//require	如果指定的 extension_name 不支持则报错，所以 #extension all : require 这种方式一定会报错。
    	//enable	如果指定的 extension_name 不支持则发出警告， #extension all : enable 这种方式则报错。
    	//warn	使用不是被其它处于启用状态的扩展所包含的扩展时会发出警告，如果扩展参数为 #extension all : warn 则一定会触发警告，如果扩展不被支持，也会发出警告。
    	//disable	指明扩展被禁用，使用该扩展会抛出错误，如果扩展参数为 #extension all : disable（即默认设置），则不允许使用任何扩展。
#version // 声明所使用的 GLSL ES 语言的版本
	// 使用 GLSL ES 3.0 以上的版本则必须在 Shader 头部使用#version version_number声明版本。
	// 在 GLSL ES 1.0 则不强制要求，默认使用 #version 100。

#line
	// #line number 执行了这个指令后,紧随其后的一行代码则被认为是第 number 行。
```

> ​		`OpenGL `的一大特性就是对 `Extension `的支持，当一个显卡公司提出一个新特性或者渲染上的大优化，通常会以 Extension 的方式在驱动中实现。如果一个程序在支持这个 `Extension `的显卡上运行，开发者可以使用这个 `Extension `提供的一些更先进更有效的图形功能。通过这种方式，开发者不必等待一个新的 `OpenGL `规范面世，就可以使用这些新的渲染特性了，只需要简单地检查一下显卡是否支持此 `Extension`。	
>
> ​		了解更多扩展能力可以看 **Khronos OpenGL ES Registry**[3] 中 Extension 部分。



## 数据类型

​		列举了 `GLSL ES 1.00.17` 和 `GLSL ES 3.00.6` 两个版本所包含的数据类型，粗体字体为 `GLSL ES 3.00.6` 特有。

| 数据类型                                                     | 含义                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| void、bool、int、float                                       | 无返回的函数或空参数列表、布尔类型、整型、浮点类型           |
| vec2、vec3、vec4                                             | 2、3、4 元浮点向量                                           |
| bvec2、bvec3、bvec4                                          | 2、3、4 元布尔向量                                           |
| ivec2、ivec3、ivec4                                          | 2、3、4 元整型向量                                           |
| **uvec2、uvec3、uvec4**                                      | 无符号整型的 2、3、4 元向量                                  |
| mat2、mat3、mat4                                             | 2x2、3x3、4x4 浮点矩阵                                       |
| **matmxn ( mat2x2、mat2x3、mat2x4、mat3x2、mat3x3、mat3x4、mat4x2、mat4x3、mat4x4 )** | mxn（2x2、2x3、2x4、3x2、3x3、3x4、4x2、4x3、4x4）浮点矩阵   |
| sampler2D、**isampler2D、usampler2D，sampler2DShadow**       | 浮点、整形、无符号整形二维纹理，带深度的浮点二维纹理         |
| samplerCube、**isamplerCube、usamplerCube，samplerCubeShadow** | 浮点、整形、无符号整形立方体贴图纹理，带深度的浮点立方体贴图纹理 |
| **sampler3D、isampler3D、usampler3D**                        | 浮点、整形、无符号整形三维纹理                               |
| **sampler2DArray、isampler2DArray、usampler2DArray，sampler2DArrayShadow** | 浮点、整形、无符号整形二维纹理数组，带深度的浮点二维纹理数组 |

​		`GLSL ES` 中还有 `struct` 和 `array` 两种数据结构：

- `struct` 可以通过使用 `struct` 关键字将其他已定义的类型聚合到一个结构中来创建自定义的类型。

  ```c
  struct light {
    float intensity;
    vec3 position;
  } lightVar;
  ```

- `array` 相同类型的变量可以通过声明名称后跟方括号 ( [ ] ) 括起大小来聚合到数组中，下面有几个数组使用的注意点：

- - 在函数声明中声明为形式参数的数组必须指定大小。
  - 只能声明一维数组，多维数组可以使用 `matrix` 替代。
  - 在 `Shader `中数组不能同时声明和初始化。



## 向量和矩阵

​		`vector` 和 `matrix` 在 `Shader `中的使用十分频繁，对顶点数据和纹理坐标的操作会通过 `vector` 类型，而使用一些投影矩阵或者缩放平移能力则会通过 `matrix` 来实现。

### 向量

#### 向量构造函数

​		向量初始化可以用构造函数来完成，在执行构造方法的时候会遵循下面的策略：

- 如果向量构造函数有一个标量参数，它用于将构造向量的所有分量初始化为该标量的值。
- 如果向量由多个标量、一个或多个向量、一个或多个矩阵混合构造而成，则向量的分量将从参数的分量按从左到右顺序构造。
- 如果使用多个标量来赋值，需要确保标量的个数要多于向量构造器中参数的个数。
- 如果构造函数参数的基本类型（bool、int 或 float）与正在构造的对象的基本类型不匹配，则会按构造方法的返回类型来转换参数

```c
vec3(float) // 构造每个分量都为 float 的三维向量
vec4(mat2) // vec4.x = mat2[0][0], vec4.y = mat2[1][0], vec4.z = mat2[0][1], vec4.w = mat2[1][1]
bvec4(int, int, float, float) // vec4.x = bool(int), vec4.y = bool(int), vec4.z = bool(float), vec4.w = bool(float)
vec2(vec3) // 丢弃 vec3 最后一个向量
vec3(vec4) // 丢弃 vec4 最后一个向量
vec3(vec2, float) // vec3.x = vec2.x, vec3.y = vec2.y, vec3.z = float
vec3(float, vec2) // vec3.x = float, vec3.y = vec2.x, vec3.z = vec2.y
vec4(vec3, float) // vec3.x = vec3.x, vec3.y = vec3.y, vec3.z = vec3.z, vec4.w = float
vec4(float, vec3) // vec3.x = float, vec3.y = vec3.x, vec3.z = vec3.y, vec4.w = vec3.z
```



#### 向量分量

​		向量最多可以有4个分量：

- 坐标向量：`.x / .y / .z / .w`
- 颜色向量：`.r / .g / .b / .a`
- 纹理向量： `.s / .t / .p / .q`

​		这些分量可以通过字母任意组合来创建一个和原来一样长（同类型）的新向量，但不允许从长度不足的向量中获取不存在的向量：

```c
vec4 pos = vec4(1.0, 2.0, 3.0, 4.0);
vec4 swiz= pos.wzyx; // swiz = (4.0, 3.0, 2.0, 1.0)
vec4 dup = pos.xxyy; // dup = (1.0, 1.0, 2.0, 2.0)
```





### 矩阵

#### 矩阵构造函数

​		矩阵值初始化使用构造函数的时候是以列优先顺序完成的，其他的策略：

- 如果矩阵构造函数只有一个标量参数，则它是用于初始化矩阵对角线上的所有分量，其余分量初始化为 0.0。
- 从多个标量或向量或混合构造矩阵，矩阵将按列优先顺序构建和使用

```c
mat2(float) // 对角线分量为 float 的 2*2 矩阵
mat3(float) // 对角线分量为 float 的 3*3 矩阵
mat4(float) // 对角线分量为 float 的 4*4 矩阵
mat2(vec2, vec2); // 每个 vec2 构成矩阵的一列
mat3(vec3, vec3, vec3); // 每个 vec3 构成矩阵的一列
mat4(vec4, vec4, vec4, vec4); // 每个 vec4 构成矩阵的一列
mat2(float, float,
  float, float); // 前两个 float 为第一列，后两个 float 为二列
mat3(float, float, float,
  float, float, float,
  float, float, float); // 前三个 float 为第一列，中间三个 float 为第二列，最后后三个 float 为第三列
mat4(float, float, float, float,
  float, float, float, float,
  float, float, float, float,
  float, float, float, float);
```



#### 矩阵分量

​		可以使用数组下标语法访问矩阵的分量。

- 单个下标应用于矩阵会将矩阵视为列向量数组，并选择单个列，其类型是与矩阵的列大小相同的向量，就是第一个下标代表列数
- 第二个下标代表行数

```c
mat4 m;
m[1] = vec4(2.0); // 设置矩阵第二列全部为 2.0
m[2][3] = 2.0; // 设置第三列的第四个元素为 2.0
```



### 向量和矩阵的计算

​		当运算符对向量或矩阵进行运算时，会以分量方式独立地对向量或矩阵的每个分量进行运算。

```c
vec3 v, u;
float f;
v = u + f;
// 等同于：
v.x = u.x + f;
v.y = u.y + f;
v.z = u.z + f;

vec3 v, u, w;
w = v + u;
// 等同于：
w.x = v.x + u.x;
w.y = v.y + u.y;
w.z = v.z + u.z;

vec3 v, u;
mat3 m;
u = v * m;
// 等同于：
u.x = dot(v, m[0]); // dot(a,b) = a[0]*b[0]+a[1]*b[1]+...
u.y = dot(v, m[1]); 
u.z = dot(v, m[2]);

u = m * v;
// 等同于：
u.x = m[0].x * v.x + m[1].x * v.y + m[2].x * v.z;
u.y = m[0].y * v.x + m[1].y * v.y + m[2].y * v.z;
u.z = m[0].z * v.x + m[1].z * v.y + m[2].z * v.z;

mat3 m, n, r;
r = m * n;
// 等同于：
r[0].x = m[0].x * n[0].x + m[1].x * n[0].y + m[2].x * n[0].z;
r[1].x = m[0].x * n[1].x + m[1].x * n[1].y + m[2].x * n[1].z;
r[2].x = m[0].x * n[2].x + m[1].x * n[2].y + m[2].x * n[2].z;
r[0].y = m[0].y * n[0].x + m[1].y * n[0].y + m[2].y * n[0].z;
r[1].y = m[0].y * n[1].x + m[1].y * n[1].y + m[2].y * n[1].z;
r[2].y = m[0].y * n[2].x + m[1].y * n[2].y + m[2].y * n[2].z;
r[0].z = m[0].z * n[0].x + m[1].z * n[0].y + m[2].z * n[0].z;
r[1].z = m[0].z * n[1].x + m[1].z * n[1].y + m[2].z * n[1].z;
r[2].z = m[0].z * n[2].x + m[1].z * n[2].y + m[2].z * n[2].z;
```





## 限定符

​		类型前面可以使用限定符，`GLSL ES` 包含了四种类型的限定符，分别为`存储限定符`、`参数限定符`、`精度限定符`、`不变限定符`。

### 存储限定符

- `< none: default > `：默认情况下不带存储限定符，默认的局部可读写变量和函数入参都属于这一类。

- - 所有无指定存储限定符修饰的全局或局部变量，只能在当前 `Shader `空间进行内存分配和使用。
  - `Shader `中函数的返回和结构体都不能使用存储限定符。

- `const`：编译阶段确定的常量或只读函数参数。

- - `Shader `中局部变量只能使用 `const` 存储限定符。
  - 任何类型变量都可以使用 `const` 修饰成只读，使用 `const` 修饰的变量声明和定义必须同时进行。
  - 结构体成员不能使用 `const`，但是结构体变量可以。
  - 数组和包含数组的结构体不能被 `const` 修饰，因为他们不能在定义时初始化。

- `attribute`：用于描述` OpenGL ES` 传递顶点数据给 `Vertex Shader `的变量所使用的存储限定符，这个限定符在 `GLSL ES 1.00` 版本中使用。

- - `attribute` 只能在` Vertex Shader `中使用，并且只读的。
  - `attribute` 只能修饰 `float、vec2、vec3、vec4、mat2、mat3、mat4` 类型的变量，不能修饰数组和结构体。
  - 数量限制需要结合后面的内置函数。

- `varying`：用来描述光栅化后 `Vertex Shader `传递给 `Fragment Shader `的插值数据变量，这个限定符在 `GLSL ES 1.00` 版本中使用。

- - `varying`修饰在变量在` Vertex Shader `中是可读写的，如果一个变量还没写就被读取会返回一个 `undefined `值，在 `Fragment Shader` 中是可读的。
  - 如果想要在` Vertex Shader `和 `Fragment Shade`r 传递一个变量，除了要用 `varying` 修饰之外，变量名还需要一样，这里变量精度不需要一致。
  - 在` Vertex Shader` 中定义的 `varying` 变量在  `Fragment Shader `没有使用是没有问题的，但是反过来则会报错。
  - `varying` 可以修饰 `float、vec2、vec3、vec4、mat2、mat3、mat4` 或者数组类型的变量，不能修饰结构体。
  - `varying` 修饰的必须是全局变量。

- `in、centroid in、out、centroid out`：输入或输出 `Shader `的变量所使用的存储限定符

  > 这些限定符在 `GLSL ES 3.00` 以上版本中使用，用来取代 `varying` 和 `attribute`。

- - 在同一渲染管线中，前一阶段的被 `out、centroid out` 修饰的变量的值会被拷贝到下一阶段用 `in、centroid in` 修饰的**同名变量**。
  - 这些限定符只能修饰全局变量，`centroid in` 不能用在 `Vertex Shader`，`centroid out` 不能用在 `Fragment Shader`。
  - `in、centroid in` 修饰的变量有数量限制，矩阵类型占用的限制量取决于矩阵的列数，一个 vec 类型占用一个数量额度，所以多个 `float `类型可以用一个 vec 来取代以此减少限制数额消耗。
  - 在` Vertex Shader `中 `in、centroid in` 限定符不能修饰 `bool`、数组和结构体类型，在` Fragment Shader `中不能修饰 `bool`、嵌套数组、结构体数组、包含数组的结构体和包含结构体的结构体。
  - 在 `Vertex Shader `中 `out、centroid out` 限定符不能修饰 bool、嵌套数组、结构体数组、包含数组的结构体和包含结构体的结构体，在 Fragment Shader 中不能修饰 `bool`、`matrix`、结构体类型。

- `uniform`：用来修饰在执行过程中保持不变的变量。

- - `uniform` 修饰的变量可以认为是真正意义上的全局变量，它的作用域可以同时跨越同一个渲染管线上的` Vertex Shader` 和 `Fragment Shader`，在同一渲染管线不同 `Shader `里 `uniform `修饰的同名变量精度必须也相同。
  - `uniform` 修饰的变量是只读的，只能在 `Shader `之外通过 `OpenGL ES API `来进行赋值。
  - `uniform` 可以修饰 `GLSL ES` 中的所有数据类型。
  - `uniform` 修饰的变量也有数量限制，但是定义了未使用的变量不算入限制数量中。

- `layout`：用来指定 `in、out` 限定符修饰的变量在 Shader 中的内存布局位置，以此避免需要通过 OpenGL ES 中的 `glGetXXXLocation` `API `去获取变量位置，在 `GLSL ES 3.00` 以上版本中使用，如下，

```c
layout(location = 3) in vec4 normal; // 不允许在 Fragment Shader 中使用
layout(location = 3) out vec4 color; // 不允许在 Vertex Shader 中使用
```

> 在 `GLSL ES 3.00.6` 版本中使用`in`、`out` 取代了 `GLSL ES 1.00.17` 版本中的 `attribute`、`varying`。



### 参数限定符

​		`GLSL ES` 中函数的参数也可以用参数限定符来修饰，有下面几种方式：

- `< none: default >` ：在没有显示指定参数限定符的情况下，默认 `in` 修饰函数参数，作用就如同 `C/C++ `中的形参。
- `in`：跟 `< none: default >` 一致。
- `out`：使用 `out` 修饰的参数，作用如同函数返回值，可以不传入参数值，其值在函数调用中初始化并返回。
- `inout`：修改 `inout` 修饰的参数限定符会影响函数调用上下文传入的参数，类型` C/C++ `中的引用参数。



### 精度限定符

​		在 `GLSL ES` 中精度限定符有三类，分别是：

- `highp` 在 `Vertex Shader` 中这个精度是必须的，在` Fragment Shade`r 中可选，一般用来描述顶点数据。
- `mediump` 在` Fragment Shader `这个精度是必须的，一般用来描述纹理数据。
- `lowp` 最小精度范围，一般用来描述颜色。

​		下面两张表格对应不同版本的 `GLSL ES` 下不同精度限定符的 `float `类型和 `int `类型的范围和精度：

- `GLSL ES 1.00`

| 精度限定符 | 浮点范围       | 浮点大小范围  | 浮点精度    | 整型范围       |
| :--------- | :------------- | :------------ | :---------- | :------------- |
| highp      | (-2^62，2^62)  | (-2^62，2^62) | 相对：2^-16 | （-2^16，2^16) |
| mediump    | (-2^14，2^14） | (-2^14，2^14) | 相对：2^-10 | （-2^10，2^10) |
| lowp       | (-2，2）       | (-2^-8，2)    | 绝对：2^-8  | （-2^8，2^8)   |

- `GLSL ES 3.00` （PS：float 类型基于 IEEE 754 标准）

| 精度限定符 | 浮点范围        | 浮点大小范围             | 浮点精度                         | 有符号整型范围     | 无符号整型范围 |
| :--------- | :-------------- | :----------------------- | :------------------------------- | :----------------- | :------------- |
| highp      | (-2^126，2^127) | (-2^126，2^127) 包含 0.0 | 相对：2^-24                      | （-2^31，2^31 - 1) | （0，2^32 - 1) |
| mediump    | (-2^14，2^14）  | (-2^14，2^14)            | 相对：2^-10                      | （-2^15，2^15 - 1) | （0，2^16 - 1) |
| lowp       | (-2，2）        | (-2^-8，2)               | 绝对： 2^-8(有符号) 2^-9(无符号) | （-2^8，2^8 - 1)   | （0，2^9 - 1)  |

- 合理的使用精度限定符可以提高渲染性能，但是如果选择精度不正确，可能会出现图像渲染结果失真。

- 数字常量、布尔变量、构造函数没有精度限定符。

- 一般情况下，运算结果的精度应该不低于运算时传入参数的精度。

- 有多个精度限定符修饰的变量参与运算，那么以更高精度限定符修饰的变量的精度为准。

- 某个参加运算的参数没有精度限定符，就以另外参加运算的参数的精度限定符为准，如果另外的参数也没有，那就看下一个操作中的参数的精度限定符，以此类推，一直到找到一个精度限定符为止。这里的下一个操作包括初始化赋值、作为别的函数的传入参数、作为别的函数的返回参数。如果依然找不到一个精度限定符，那么就认为当前的精度限定符为默认值，下面我们来介绍一下默认精度限定符：

- - 使用 `precision` 修饰精度限定符就能把当前精度指定为变量的的默认精度，只有 int、float、sampler 这几种类型可以使用 `precision` 指定默认精度，当 float 或 int 指定了默认精度后，所有 float 或 int 相关的向量和矩阵变量类型如果没有指定精度限定符, 那么就使用前面 float 的默认精度。
  - 使用 `precision` 修饰的精度限定符是有作用范围的。一个变量没有办法判断其精度，那么就使用最近的一个且在使用范围的默认精度限定符。如果只是某个代码块定义的一个默认精度限定符，那么出了这个代码块就无效。局部的默认精度限定符在所在代码块中会覆盖掉全局的默认精度限定符，最里层代码块的默认精度限定符总会覆盖掉外层的。同一个代码块中出现两个同一变量类型的默认精度限定符，则最后的那个会生效。

- 可以使用 `GL_FRAGMENT_PRECISION_HIGH` 来判断能否在 Fragment Shader 中使用高精度。

​		`Vertex Shader` 和` Fragment Shader `中有些类型内置了默认精度限定符，如下：

```c
// Vertex Shader
precision highp float;
precision highp int;
precision lowp sampler2D;
precision lowp samplerCube;

// Fragment Shader
// Fragment Shader 中没有用于浮点类型的默认精度限定符，所以对于 float、浮点向量、浮点矩阵变量的声明必须包含精度限定符，或者必须事先声明默认的 float 精度。
precision mediump int;
precision lowp sampler2D;
precision lowp samplerCube;
```



### invariant 限定符

​		在两个 `Program `中编译同个 `Vertex Shader` 并且输入是完全相同的，但是由于同个 `Vertex Shader `在不同的 `Program `中是独立编译的，最后 `gl_Position` 的值可能不完全相同，这可能会导致 `multi-pass` 算法中的几何对齐问题，为了避免这种情况发生，就需要使用到 `invariant` 限定符。

​		有两种方式使用 `invariant`，一种是声明过的变量，在后面使用 `invariant` 修饰一次这个变量，第二种是变量声明的时候同时使用 `invariant` 修饰，下面是在 `GLSL ES 1.00` 和 `GLSL ES 3.00` 版本下的例子：

```c
// GLSL ES 1.00： 
invariant gl_Position; // invariant 修饰内置变量

varying mediump vec3 Color;
invariant Color; // invariant 修饰声明过的变量

invariant varying mediump vec3 Color; // 变量声明的同时使用 invariant 修饰

// GLSL ES 3.00： 
invariant gl_Position; // invariant 修饰内置变量

out vec3 Color;
invariant Color; // invariant 修饰声明过的变量

invariant centroid out vec3 Color; // 变量声明的同时使用 invariant 修饰
```

​		在 `GLSL ES 1.00` 版本中，只有以下几种情况才能使用 `invariant` 限定符：

- 从顶点着色器输出的内置特殊变量。
- 顶点着色器输出的可变变量。
- 片段着色器的内置特殊输入变量。
- 输入到片段着色器的变量。
- 片段着色器的内置特殊输出变量。

​		在 `GLSL ES 3.00` 版本中，只有从 `Shader `出的变量可以使用 `invariant` 限定符，包括用户定义的输出变量和内置输出变量。由于只有输出可以声明为不变的，因此同一渲染管线中 `invariant` 修饰的输出仍将匹配后续阶段的输入，而无需用 `invariant`修饰输入。

​		为保证在不同 `Program `中同一个 `Shader `的特定输出变量完全一致，必须遵守以下条件：

- 输出变量被 `invariant` 修饰。
- 表达式和控制流的输入值必须相同。
- 纹理格式、纹素、纹理过滤方式设置必须一致。
- 所有输入值都以相同的方式操作。任何表达式中的所有操作必须相同，具有相同的操作数顺序和相同的结合性，中间变量和函数必须声明为相同精度的相同类型。影响输出值的任何控制流都必须相同，并且用于确定此控制流的任何表达式也必须遵循这些不变性规则。

​		默认情况下变量是没有 `invariant` 修饰的，可以使用下面语句打开默认 `invariant` 修饰：

```
#pragma STDGL invariant(all)
```

​		使用 `invariant` 会牺牲整体性能。因此慎用以上的全局设置方法，一般在 Debug 环境下使用。





## 函数

​		一个有效的 Shader 是由一系列全局声明和函数定义组成。

```c
// prototype
returnType functionName (type0 arg0, type1 arg1, ..., typen argn);
```

​		定义一个带返回或者不带返回值的函数如下：

```c
// 带返回值
returnType functionName (type0 arg0, type1 arg1, ..., typen argn)
{
 // do some computation
 return returnValue;
}
// 不带返回值
void functionName (type0 arg0, type1 arg1, ..., typen argn)
{
 // do some computation
 return; // 可写可不写
}
```

- 可选的可以在类型前面带参数限定符或者 `const`

- 数组和结构体都可以作为函数返回值或参数

  - 数组作为函数的返回值或参数的时候，数字大小必须是确定的。当一个数组以名字传递（不带括号表示大小）或者返回的时候，这时候数组需要跟函数内定义的大小相等。

-  return 后面只能带具体值，而不能是其他的函数调用

  ```c
  void func1() { }
  void func2() { return func1(); } // 非法的返回
  ```

- 函数返回值只能使用精度限定符，函数参数可以使用精度限定符和参数限定符。

- 函数是可以重载的，同一个函数名可以用于多个函数，只要参数类型不同即可。

- 自定义的函数可以有多个声明，所以如果一个函数名用相同的参数类型声明了两次，那么返回类型和所有限定符必须匹配，解析函数调用时，需要所有参数的类型也完全匹配。

- 函数 main 用作 Shader 的入口点。

  -  Vertex Shader 和 Fragment Shader 都必须包含一个名为 main 的函数
  - 这个函数不接受任何参数，不返回任何值，并且必须声明为 void 类型

  ```c
  void main()
  {
   ...
  }
  ```

- 跳转语句跟 `C/C++ `有些许的不同

  - 在 `Shader `中允许使用的跳转语句有：`continue; break; return; return expression; discard;`

    - 其中 `continue; break;` 只能用在循环中，作用和 C/C++ 相同。

    - `return;return expression;`的作用和 C/C++ 相同。

    - `discard;` 只能在` Fragment Shader` 中使用，它的作用是放弃对当前片元的操作

      > 在渲染管线中，`Fragment Shader `会对光栅化处理后生成的片段做一一处理，而 `discard;`可以用来决定哪些片元不处理。

  - 不能使用 `goto` 这种非结构化的控制流。

- 函数不能递归调用

- 还 `const` 不能修饰参数限定符 `out` 和 `inout`



## 内置变量和内置函数



### 内置变量

​		`Shader `可以通过使用内置变量与 `OpenGL ES` 的固定功能进行通信。

#### Vertex Shader

​	`	Vertex Shader `分别在 `GLSL ES 1.00` 和 `GLSL ES 3.00` 版本的内置变量：

```c
// --- GLSL ES 1.00 ---

// 指定绘制的图元方式为 GL_LINES、GL_TRIANGLE 的时候需要输出的内置变量，用来指定需要返回的顶点位置。
highp vec4 gl_Position; 

// 指定绘制的图元方式为 GL_POINTS 的时候需要输出的内置变量，用来指定需要绘制的点的大小。
mediump float gl_PointSize; 

// --- GLSL ES 3.00 ---

// 保存顶点的整数索引。
in highp int gl_VertexID;

// 当 Vertex Shader 中有多个实例模型（比如渲染了很多个盒子）的时候，这个变量用来指定每一个实例模型的索引。 
in highp int gl_InstanceID;

// 同 1.0 版本。
out highp vec4 gl_Position;
out highp float gl_PointSize;
```



#### Fragment Shader

​		` Fragment Shader` 分别在 `GLSL ES 1.00` 和 `GLSL ES 3.00` 版本的内置变量：

```c
// --- GLSL ES 1.00 ---

// 返回片元的坐标，由光栅化阶段差值计算所得。
mediump vec4 gl_FragCoord;

// 当前片元处于则正面返回 true
bool gl_FrontFacing;

// 指定当前片元的颜色，只能和 gl_FragData 二选一。
mediump vec4 gl_FragColor;

// gl_FragData[n] 表示第 n 个片段的颜色值，只能和 gl_FragColor 二选一。
mediump vec4 gl_FragData[gl_MaxDrawBuffers];

// 指定绘制的图元方式为 GL_POINTS 的时候需要输出的内置变量，返回当前片元代表的点的坐标。
mediump vec2 gl_PointCoord;

// --- GLSL ES 3.00 ---

// 同 1.0 版本。
in highp vec4 gl_FragCoord;
in bool gl_FrontFacing;
in mediump vec2 gl_PointCoord;

// 指定当前片元深度坐标
out highp float gl_FragDepth;
```



### 内置常量

​		`Shader `中的内置常量主要用来表示一些存储限定符或绘制单元的数量上限标准，这里其实主要是对硬件厂商的最低支持要求，意思是 `GPU `厂商必须最少遵守下面这些常量的数量定义，开发者在使用存储限定符或者绘制单元只要不超过下面的最大数量限制则一定不会有问题。

```c
// --- GLSL ES 1.00 ---

// Vertex Shader 中 Attributes 储存限定符的最大支持数量。
const mediump int gl_MaxVertexAttribs = 8; 

// Vertex Shader 中 Uniform 储存限定符修饰的 Vector 类型最大支持数量。
const mediump int gl_MaxVertexUniformVectors = 128; 

//  Varying 储存限定符修饰的 Vector 类型最大支持数量。
const mediump int gl_MaxVaryingVectors = 8; 

// Vertex Shader 中 texture unit 的最大支持数量。
const mediump int gl_MaxVertexTextureImageUnits = 0; 

// Vertex Shader 和 Fragment Shader 中有同名 Uniform 存储 texture 的时候，默认是一个 CombinedTexture，这里是最大 CombinedTexture 个数
// gl_MaxCombinedTextureImageUnits =  gl_MaxVertexTextureImageUnits + gl_MaxCombinedTextureImageUnits。
const mediump int gl_MaxCombinedTextureImageUnits = 8; 

const mediump int gl_MaxTextureImageUnits = 8;

// Fragment Shader 中 Uniform 储存限定符修饰的 Vector 类型最大支持数量。
const mediump int gl_MaxFragmentUniformVectors = 16; 

// 对应 glDrawBuffers()。
const mediump int gl_MaxDrawBuffers = 1; 


// --- GLSL ES 3.00 ---

// Vertex Shader 中 Attributes 储存限定符的最大支持数量。
const mediump int gl_MaxVertexAttribs = 16;

// Vertex Shader 中 Uniform 储存限定符修饰的 Vector 类型最大支持数量。
const mediump int gl_MaxVertexUniformVectors = 256;

// Vertex Shader 中输出 Vector 类型最大支持数量。
const mediump int gl_MaxVertexOutputVectors = 16;

// Fragment Shader 中输入 Vector 类型最大支持数量。
const mediump int gl_MaxFragmentInputVectors = 15;

// Vertex Shader 中 texture unit 的最大支持数量。
const mediump int gl_MaxVertexTextureImageUnits = 16;

// Vertex Shader 和 Fragment Shader 中有同名 Uniform 存储 texture 的时候，默认是一个 CombinedTexture，这里是最大 CombinedTexture 个数。
// gl_MaxCombinedTextureImageUnits =  gl_MaxVertexTextureImageUnits + gl_MaxCombinedTextureImageUnits。
const mediump int gl_MaxCombinedTextureImageUnits = 32;

const mediump int gl_MaxTextureImageUnits = 16;

// Fragment Shader 中 Uniform 储存限定符修饰的 Vector 类型最大支持数量。
const mediump int gl_MaxFragmentUniformVectors = 224;

// 对应 glDrawBuffers()。
const mediump int gl_MaxDrawBuffers = 4;

// 纹理偏移区间 min 到 max 的值，一般我们如果要取纹理的某个子区域的时候可以先根据这两个常数来判断是否在可选区间内。
const mediump int gl_MinProgramTexelOffset = -8;
const mediump int gl_MaxProgramTexelOffset = 7;

```



### 内置函数

在 `GLSL ES` 中内置函数基本上可以分为三大类：

- 一些无法在 `Shader `里用着色器语言来自定义的硬件能力，只能用内置函数来实现，比如纹理贴图。
- 可以在 `Shader `中用着色器语言来自定义但是实现起来十分琐碎繁杂的操作 `clamp`、`mix `等，并且这些操作可能有直接的硬件支持，编译器将表达式映射到复杂的汇编程序指令是非常困难的，使用内置函数可以避免这些问题。
- 一些功能会提供具有硬件加速能力的内置函数来给开发者使用，比如三角函数。

​		这三大类内置函数如果按功能来划分，在  `GLSL ES 1.00` 版本中可以划分为：角度和三角函数、指数函数、通用函数、几何函数、矩阵函数、向量关系函数、纹理查找函数这七类，在 `GLSL ES 3.00` 版本中新增了浮点打包和解包函数和片段处理函数，并且细化了一些函数的返回值类型。

​		下面是本文列举出的部分常用内置函数，想了解更多的内置函数的话可以阅读 **GLSL ES 文档( 93 - 122 页部分)**[4]。

| 内置函数                                                     | 函数作用                                                     |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| genType radians (genType degrees)                            | 角度转换为弧度                                               |
| genType degrees (genType radians)                            | 弧度转换为角度                                               |
| genType pow (genType x, genType y)                           | x^y                                                          |
| genType abs (genType x)                                      | 取绝对值                                                     |
| genType floor (genType x)                                    | 向下取整                                                     |
| genType ceil (genType x)                                     | 向上取整                                                     |
| genType mod (genType x, float y)                             | x % y                                                        |
| genType min (genType x, genType y)                           | 求两数的最小值                                               |
| genType max (genType x, genType y)                           | 求两数的最大值                                               |
| genType clamp (genType x,genType minVal,genType maxVal)      | 取中间值                                                     |
| genType mix (genType x,genType y,genType a)                  | x*(1-a) + y * a                                              |
| genType step (genType edge, genType x)                       | if（x < edge）return 0 else return 1                         |
| genType smoothstep (genType edge0, genType edge1, genType x) | if(x <= edge 0 ) return 0 else if(x >= edge1) return 1 else if(edge0 < x < edge1) return (0 - 1之间的平滑插值） |
| float length (genType x)                                     | x 为向量，√（x[0]+x[1]+...）                                 |
| float distance (genType p0, genType p1)                      | 求两个向量的距离等同 length (p0 – p1)                        |
| float dot (genType x, genType y)                             | xy都为向量，x[0]*y[0]+x[1]*y[1]+...                          |
| genType normalize (genType x)                                | x 为向量，x / length (x)                                     |
| bvec lessThan(vec x, vec y)                                  | if（x < y）return 1 else return 0                            |
| bvec lessThanEqual(vec x, vec y)                             | if（x <= y）return 1 else return 0                           |
| bvec greaterThan(vec x, vec y)                               | if（x > y）return 1 else return 0                            |
| bvec greaterThanEqual(vec x, vec y)                          | if（x >= y）return 1 else return 0                           |
| bvec equal(vec x, vec y)                                     | if（x = y）return 1 else return 0                            |
| vec4 texture2D (sampler2D sampler, vec2 coord ) vec4 texture2DProj (sampler2D sampler, vec3 coord ) | 使用纹理坐标 coord 在当前绑定到采样器的 2D 纹理中进行纹理查找。 对于投影（“Proj”）版本，纹理坐标 (coord.s, coord.t) 除以坐标最后一个分量。 |













