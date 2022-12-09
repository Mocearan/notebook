# 一、QT

## 1.简介

---

​		Qt常用于开发图形界面应用程序，但它除了绘制优秀的界面外还内置了很多其他的功能，多线程、访问数据库、图形处理、音视频处理、网络通信、文件操作等。

​		主要用于桌面级和嵌入式的开发。

## 2.优点

---

​		Qt相对于其他GUI库的有点：

- 简单易学：封装的好，不需要了解windows API
- 资料丰富：其用户数量庞大，无论是官方还是非官方的资料齐全
- 界面优秀：可以较为容易的做出漂亮的界面和炫酷的动画
- 独立安装：全部便以为本地代码，无需第三方依赖
- 跨平台：跨平台支持很好，成本低

## 3.Qt开发工具列表

| 工具      | 说明                                                         |
| --------- | ------------------------------------------------------------ |
| qmake     | 核心的项目构建工具， 可以生成跨平台的.pro项目文件，并能依据不同操作系统和编译工具生成响应的`Makefile`，用于构建可执行程序或链接库 |
| uic       | User Interface Compiler， 用户界面编译器，Qt使用XML语法格式的.ui文件定义用户界面，uic根据.ui文件生成用于创建用户界面的C++头文件，比如ui_xxx.h |
| moc       | Meta-Object Compiler，原对象编译器，moc处理C++头文件的类定义里面的Q_OBJECT宏，它会生成源代码文件，比如moc_*.cpp，其中包含响应类的原对象代码，原对象代码主要用于实现Qt信号/槽机制、运行时类型定义、动态属性系统。 |
| rcc       | Resource Compiler， 资源文件编译器，负责在项目构建过程中编译.qrc资源文件，将资源嵌入到最终的qt程序里。 |
| qtcreator | 集成开发环境，包含项目生成管理、代码编辑、图形界面可视化编辑、编译生成、程序调试、上下文帮助、版本控制系统集成等众多功能，还支持手机和嵌入式设备的程序生成部署。 |
| assistant | Qt助手，帮助文档浏览查询工具，Qt库所有模块和开发工具的帮助文档、实例代码等都可以检索到，是Qt开发必备神器，也可用于自学Qt |
| designer  | Qt设计时，专门用户可视化编辑图形用户界面（所见即所得），生成.ui文件用于Qt项目，或者用于设计QML界面。 |
| linguist  | Qt语言家，代码里用tr()宏包裹的就是可翻译的字符串，开发人员可用Iupdate命令生成项目的待翻译字符串文件.ts，用linguist翻译躲过语言.ts，翻译完成后用Irelease命令生成.qm文件，然后就可用于多国语言界面显示 |
| qmlscene  | 在Qt 4.x里是用qmlviewer进行QML程序的原型设计和测试，QT 5用qmlscene取代了旧的qmlviewer。新的qmlscene另外还支持Qt 5中的新特性scenegraph |

## 4. 项目组成和管理

在Qt creator左侧工具栏中单击”编辑“按钮，可以进入项目管理窗口，Qt Creator可以打开多个项目，但只有一个活动项目，活动项目名称加粗显示。

在项目名称节点下，分组管理着各种源文件，文件和分组如下：

- xxx.pro文件是项目管理文件，包括一些对项目的设置项
- Headers分组，该节点下是项目内的所有头文件（.h）
- Sources分组，该节点下是项目内的所有C++源文件（.cpp）
- Resources分组，该节点下是项目被的各种资源文件，如QML文件，图标（图片）文件，翻译文件等
  - 使用qrc文件加载资源

## 5.pro文件详解

在此文件中#为注释

```properties
TEMPLATE = APP						#表示项目使用的模板时app，是一般的应用程序
QT += qml quick gui widgets			 #表示项目中加入相应模块
TARGET = MYPlayer					#表示生成的目标可执行文件的名称
RESOURCES += resource/MYPlayer.qrc	  #资源项目文件
TRANSLATIONS += ./resource/ui/translation/MYPlayer_zh_CN.ts	# 翻译文件
SOURCES += \ 
	main.cpp \
	MainApp.cpp 
HEADERS += MainApp.h
```



## 6.Qt元对象系统

​		Qt的原对象系统（Meta-Object System）提供了对象之间通信的信号与槽机制、运行时类型信息和动态属性系统。

​		元对象系统由一下三个基础组成：

1. QObject类是所有使用原对象系统的基类
2. 在一个类的private部分声明Q_OBJECT宏，使得类可以使用原对象的特性，如动态属性、信号与槽
3. MOC（元对象编译器）为每个QObject的子类提供必要的代码来实现元对象系统的特性

​		构建项目时，MOC工具读取C++原文件，当它发现类的定义里有Q_OBJECT宏时，它就会为这个类生成另外一个包含有元对象支持代码的C++源文件，这个生成的源文件和类的实现文件一起被编译和链接。

## 7.Qt进本模块

| 模块                  | 描述                                             |
| --------------------- | ------------------------------------------------ |
| Qt Core               | 基础核心公共组件，其他模块都用到                 |
| Qt GUI                | 设计GUI界面的基础类，包括OpenGL                  |
| Qt Multimedia         | 音频、视频、摄像头和广播功能的类                 |
| Qt Multimedia Widgets | 实现多媒体功能的界面组件类                       |
| Qt Network            | 网络编程库的封装                                 |
| Qt QML                | 用于QML和JavaScript语言的类                      |
| Qt Quick              | 用于构建具有定制用户界面的动态应用程序的声明框架 |
| Qt Quick Control      | 创建桌面样式界面，基于Qt Quick的用户界面控件     |
| Qt Quick Dialogs      | 用于Qt Quick的系统对话框架类型                   |
| Qt Quick Layouts      | 用于Qt Quick 2界面元素的布局项                   |
| Qt SQL                | 使用SQL用于数据库操作的类                        |
| Qt Test               | 用于应用程序和库进行单元测试的类                 |
| Qt Widgets            | 用于构建GUI界面的C++图形组件类                   |

## 8.Qt定时器

​		QTimer类，如果Qt无法交付所请求的计时器事件数量，它将静默地丢弃一些计时器事件。所以有时候QTimer可能不能正常刷新，此时可以使用startTimer。

1. 用startTimer启动一个定时器，并返回定时器id。如果启动失败，返回0
2. 定时器每隔interval毫秒就会启动一次，直到调用KillTimer()。如果interval=0，当没有其他系统时间发生时，会调用一次
3. 当定时器发生时，会调用timerEvent(QTimerEvent* event)，若果多个定时器在运行，可以通过timeId()来区分。

​	如，startTimer（40）; // 一秒25帧，25*40ms=1000ms 

## 9.QThread

---

​		线程等待：

```c++
void msllep(unsigned long msecs);
// 强制当前线程睡眠msecs毫秒
```

## 10.QAudioFormat、QAudioOutput

​		需要在qmake中添加`QT += multimedia`

​		QAudioFormat：

| 参数               | 描述                                                     |
| ------------------ | -------------------------------------------------------- |
| Sample Rate        | 采样率，每秒音频数据采样数，HZ                           |
| Number of channels | 音频通道数（通常为单声道一个，立体声两个）               |
| Sample size        | 样本大小，通常为8或16                                    |
| Sample type        | 样本的数值表示（通常为有符号数整数，无符号整数或浮点数） |
| Byte order         | 样本的字节顺序（通常为小端，大端）                       |

| 函数                  | 功能                     |
| --------------------- | ------------------------ |
| setSampleRate()       | 设置采样率               |
| setSampleSize()       | 设置样本大小             |
| setChannelCount()     | 设置声道数               |
| setCodec(“audio/pcm”) | 将编码器设置为 audio/pcm |
| setByteOrder()        | 设置字节序               |
| setSampleType()       | 设置样本类型             |

​	QAudioOutput：创建对象时将QAudioFormat的对象赋给它即可

| 函数       | 作用                                                         |
| ---------- | ------------------------------------------------------------ |
| start()    | 返回一个指向内部QIODevice的指针，该指针用于将数据传输到系统的因匹配能输出。write可以直接向其中写入数据。 |
| stop()     | 停止音频输出，与系统资源分离                                 |
| suspend()  | 停止处理音频数据，保留缓冲区的音频数据                       |
| resume()   | 在suspend()后恢复处理音频数据                                |
| byteFree() | 返回一品缓冲区中的可用字节数                                 |
| ...        | ...                                                          |



## 11.其余Qt类

### （1）QDir

​		此类用于操作路径名及底层文件系统，获取关于目录路径及文件的相关信息，也可以用来获取Qt资源系统的文件信息。

​		QDir类使用相对或绝对路径来指向一个文件/目录。

> 绝对路径使用`/`作为目录分隔符，以`/`或盘符做起始：
>
> ```c++
> #include <QDir>
> QDir path1{"/ui/image"};
> QDir path2{"D:/"};
> ```
>
> 相对文件名是由一个目录名称或者文件名开始并且指定一个相对于当前路径的路径，当前路径为应用程序工作目录
>
> ```c++
> QDir path3{"images/isPlay.png"}；
> ```

| 函数           | 功能                                                         |
| -------------- | ------------------------------------------------------------ |
| path()         | 获得所关联的目录路径                                         |
| setPath()      | 设置新的路径                                                 |
| absolutePath() | 获得目录的绝对路径                                           |
| dirName()      | 返回绝对路径汇总Uzi后一个项目，即路径名，如果关联的是当前目录，返回‘.’ |
| mkdir()        | 创建一个目录                                                 |
| rename()       | 对关联目录进行重命名                                         |
| rmdir()        | 移除一个目录                                                 |
| exists()       | 检查目录是否存在                                             |
| refresh()      | 刷新目录内容                                                 |



### （2）QFont

​		Qt自带字体类，使用简单，此项目只是用其修改UI界面字体。

```c++
#include <QFont>

QFont font;

font.setFamily("微软雅黑");		 // 设置字体
font.setPixelSize(20);			// 设置文字像素大小
font.setPointSize(20);			// 文字大小
font.setUndreline(true);		// 下划线	
font.setStrickOut(true);		// 中划线
font.setOverline(true);			// 上划线
font.setItalic(true);			// 意大利斜体
font.setBold(true);				// 加粗
font.setStyle(QFont::StyleOblique); // 样式倾斜
font.setCapitalization(QFont::capitalize); // 首字母大写
font.setLetterSpacing(QFont::PercentageSpacing, 200); // 间距
```



### （3）QIcon

​		Qt对于icon图标的处理类，此程序只利用其设置程序图标。

```c++
setWindoeIcon(QIcon(path));
```

### （4）QDebug

​		Qt用于输出调试信息的类，其使用既可以类似cout来使用，也可以像printf一样使用，会自动换行。

​		输出到控制台的两种使用方式：

1. 将字符串当做参数传给qDebug()函数（可以不用添加头文件`#include <QDebug>`）

2. 使用流的方式输出多个字符串（需要添加头文件）

   ```c++
   qDebug() << "Hello World!";
   ```

### （5）QSetting

​		Qt对于配置文件(.ini)操作的类，远比直接读写文件方便。

​		.ini配置文件格式：节、键、值组成，封号注释

```ini
[settion] ; 节
name = value  ; key = value 
```

```c++
QSettings* defualtIni = new QSettings("default.ini", QSettings::IniFormat); //如果没有该文件会自行创建
// 前面的参数为ini文件的路径，此处为相对路径，即位域工作目录下，后面的参数为声明将设置存储值ini文件中
QString num1 = defaultIni->value("section1/num1".toString()); // 读取section1节中键num1的值存储至Qstring num1中
// 如果没有对应的值，则为 ""
defaultIni->setValue("sectin2/num5", QString("value5")); //没有该节或该键都会自行创建并赋值
```



### （6）QFileInfo

​		Qt用于获取文件信息的类，可用于获取文件所在路径（去除文件名），获取文件名等操作，此项目中使用其来获取文件所在路径，以获取上一次打开相关文件的目录。

```c++
QFileInfo fileInfo(path); // 声明文件路径
QString  string1{fileInfo.path()}; // 获取文件所在路径（去除文件名）
```

其他操作

| 函数               | 功能                 |
| ------------------ | -------------------- |
| exists()           | 判断文件是否存在     |
| size()             | 获取文件大小         |
| isFile()           | 是否为普通文件       |
| isSymLink()        | 是否为符号链接       |
| symLinkTarget()    | 返回符号链接所指文件 |
| absoluteFIlePath() | 返回绝对路径         |

​		还有其他诸如此类的函数。

### （7）QObject

​		Qt中所有Qt累的基类，是Qt对象模型的核心，QObject的核心功能是信号槽的通信机制。

​		QML在没有特殊需求的情况下，无须使用QObject绑定机制，因为一般不会自己添加槽函数。

​		QWidget经常用connect这种连接机制。

​		只有继承了QOb类，才具有信号槽的功能，凡是QObject类，都应该在类声明中一开始就写Q_OBJECT，不管是否使用信号槽。这个宏的展开会为我们的类提供信号槽机制、国际化机制、Qt提供的不基于C++ RTTI的反射能力等多种能力。

### （8）QString

​		Qt中对于字符串操作的类，其特点为

- 采用Unicode编码，一个QChar占用两个字节
- 使用隐式共享技术来节省内存、减少不必要的数据拷贝
- 跨平台使用，不用考虑字符串的平台兼容性
- QString直接支持字符串和数字之间的相互转换
- QString直接支持字符串之间的大小比较（按照字典序）
- QString直接支持不同编码下的字符串转换
- QString直接支持`std::string`和`std::wstring`之间的相互转换
- QString直接支持正则表达式的使用

​		在我们转换一个字符串字面量为QString对象的时候我们需要使用QSstringLiteral函数。

​		在Qt中除了QML中使用的是`std::string`外，其他位置直接使用QString即可，否则在字符串传递的情况下可能会出错。

### （9）QApplication

​		Qt中用于管理GUI程序的控制流和主要设置的类，其包含窗口系统和其他力啊元处理过和发送的主事件循环。它也处理应用程序的初始化和首尾工作，并提供对话管理，QApplication可以对系统和应用的大部分设置项进行设置。

​		对于Qt系的任何一个GUI应用，不管其有多少窗口或者没有窗口，都有且仅有一个QApplication对象。

| 函数                        | 功能           |
| --------------------------- | -------------- |
| setFont()                   | 设置程序字体   |
| setApplicationName()        | 设置程序名     |
| setWindowIcon()             | 设置程序图标   |
| setApplicationDisplayName() | 设置程序显示名 |

### （10）QCoreApplication

​		继承关系： QApplication <= QGUIApplication <= QCoreApplication <= QObject

```c++
// 高清屏幕自适应设置
QCoreApplication::setAttribute(QT::AA_EnableHightDpiscaling);
```

| 函数                | 功能     |
| ------------------- | -------- |
| installTranslator() | 加载语言 |



### （11）QUrl

​		此类提供了一个方便的接口使用URLs，用于转换路径为url

### （12）QList

​		QList以列表形态存储并管理值，并能进行基于快速索引的访问，也可以进行快速的数据删除操作。

| 函数      | 功能         |
| --------- | ------------ |
| isEmpty() | 判断是否为空 |



### （13）QScopePointer

​		需要包含头文件QPointer

​		QPointer为Qt中的一个模板类，是QObject提供的一种监视指针。

```c++
QPointer<T> p;
```

​		QScopedPointer是Qt中的智能指针，包装了new操作符在堆上分配的堆上对象，能够保证动态创建的对象在任何时候可以被正确的删除，其拥有更严格的所有权，不能转让，一旦获取了对象的管理权就无法让出，超出作用域就自动删除，禁用了拷贝语义防止共享。

QScopePointer<T>:

| 函数    | 功能                                       |
| ------- | ------------------------------------------ |
| reset() | 删除器指向的现有对象，并将指针置为新的值。 |
| data()  | 返回此对象引用的指针的值                   |



### （14）QTranslator

​		该类为Qt类用于生成多语言版本的类，该类用于加载语言文件

| 函数   | 功能                                       |
| ------ | ------------------------------------------ |
| load() | 将翻译文件添加到要勇于翻译的翻译文件列表中 |

​		如果想要不加载语言文件，直接加载默认值，只需要在reset()不load()，并且直接installTranslator即可。

### （15）QStandardPaths

​		Qt中的系统标准路径类。所谓的系统标准路径指的是本地文件系统中，用户的特定目录或系统的配置目录。比如在windows系统中的“我的文档”，“视频”，“图片”等目录位置。

### （16）QQMLApplicationEngine

​		继承自QQMLEngine，提供了从一个QML文件里面加载应用程序的方式

### （17）QQMLContext

​		用于设置QML文件的上下文

### （18）QIODevice

​		Qt中用于对输入输出设备进行管理的类

| 函数                                    | 功能                            |
| --------------------------------------- | ------------------------------- |
| reset()                                 | 重新打开设备                    |
| close()                                 | 简单理解为关闭设备              |
| write(const char* data, qint64 maxSize) | 将至多maxSize大小的字节写入设备 |





# 二、QML

​		一种标记语言，类似于HTML，主要用于描述程序界面。

---

​		定义用户界面元素的外观和行为，可以借助css进行梅花，也可以借助JS进行交互。

> 其注释方式与C++相同

​		使用QML开发界面优点：

- QML灵活，效果较好
- QML是标记语言，容易编写和理解，易于学习
- QML界面简洁且美观，自带很多动画
- 不同平台的QML使用相同的渲染机制，界面效果一致，不会因操作系统变化



## QML基础语法

### 1.示例

​		一个QML文档分为 import和对象生命两个部分

- import
  	用于指定该文档所需要引入的模块。

  ​	通常是一个模块名和版本号，比如QtQuick 2.0，也可以引入自己的模块或者其他文件

- QML元素

  ​	一个QML文档有且只有一个根元素，类似XML文档的规定。QML文档中的元素同样类似XML文档。

### 2.元素关系

- 层次关系
  - 子元素处于相对于父元素的坐标系统中，即子元素的x、y的坐标值始终相对于父元素

### 3.基础属性

- 属性的值由其类型决定，如果一个属性没有给值，则会使用属性的默认值，自定义属性会给定系统的默认值

- 属性可以依赖于其他属性，这种行为被称为绑定，类似于信号槽机制，所依赖属性发生变化，该属性会得到通知，自动更新自己的值。int类型的属性会自动转换成字符串靠在值发生变化时，绑定依然成立

- 可以声明默认属性

  - id

    ​	标识符，必须唯一，相当于该元素的指针，可以直接用名字指代该元素。id不是字符串，而是一个特殊的标识符类型。一旦指定不可更改，命名改规则与C++指针一致，不允许反查ID

  - x、y坐标

    ​	相对于父元素的坐标

  - width、height

    ​	宽高

  - property int times：24

    ​	自定义属性，格式为``property <type> <name>:<balue>`

  - property alias anotherTimes：times

    ​	属性别名，别名即引用

  - text：“Greetings” + times

    ​	文本和值

  - onHeightChanged：console.log(‘height’, height)

    ​	属性值改变的信号处理回调，后面为控制台输出信息

  - focus:true

    ​	接收键盘事件需要设置focus

  - color:focus?”read”:”black”

    ​	根据focus值改变颜色

​	所有属性都有对应数值改变的槽函数，格式为`on+属性名+Changed`，属性名首字母大写。属性改变会发出响应的信号，如`height`属性改变，发出`heightCHanged`信号。

### 4.所用组件

#### 可视元素

1. Item  一般用作容器
   - Item是所有可视元素中最基本的的一个，其实所有其他可视元素的父元素
2. Image  图片组件
   - smooth  设置平滑
   - source 图面路径设置，资源总选取文件，相对路径前需要加`qrc:///`，绝对路径前需要加`file:///`
3. Rectangle   默认长方形组件
   - radius： 斑晶，可以绘制圆形
4. Text    文本组件
   - text
5. MouseArea   鼠标事件处理组件，不可见
   - onDoubleClicked
   - onEntered
   - onExited

可视元素通用属性

| 分组     | 属性                                                         |
| -------- | ------------------------------------------------------------ |
| 几何     | x和y用于定义元素左上角的坐标，width和height则定义元素的范围，z定义了元素上下层叠关系 |
| 布局     | anchors（具有left、right、top、bottom、vertical和horizontal center等属性），用于定位元素相对于其他元素的margins的位置 |
| 键盘处理 | key和keyNavigation属性用于控制键盘，focus属性则用于启用键盘处理，也就是获取焦点。 |
| 变形     | 提供scale和rotate变形以及更一般的针对xyz坐标值变换以及transformOrigin点的transform属性列表 |
| 可视化   | opacity属性用于控制透明度，visible属性用于控制显式/隐藏元素。clip属性用于剪切元素，smooth属性用于增强渲染质量 |
| 状态定义 | 提供一个由状态组成的列表states和当前状态state属性，同时还有一个tansitions列表，用于设置状态切换时的动画效果 |

#### 其他元素

1. Slider   进度条

   - stepSize   精度

2. Button   按钮

   - onClicked

3. FileDialog    选择文件框，需要执行open函数

   - title 标题

   - folder 默认选择路径

   - nameFilters  过滤器，过滤出可选择文件

     - `[“xxx(*.xx *.xx)”, "xxxx(*)"]`

       `["Video files(*.mp4 *.rmvb *.flv)", "All files(*)"]`

   - onAccepted    选定文件

   - onRejected     取消选定

4. Timer     计时器，需要调用start开启计时，也可以用stop结束计时

   - interval   时间
   - onTriggered   开始计时后经过响应时间后触发

5. Dialog    弹出框，默认含有ok按钮和关闭

   - title：弹出框的标题
   - onAccepted

6. GroupBox    提供带有标题的组框框架，相当于一个容器

   - title，容器的标题，不设置就没有

7. ColumnLayout    垂直布局组件，内部元素一一垂直排列，RowLayout为水平布局组件，需要包含Layouts

8. RadioButton     单选组件，在Controls 1.x版本时需要设置一个分组组件，将多个RadioButton包含在同一个分组组件内才可以实现单选，但在Controls 2.x版本年后取消了那个分组组件，只需要将RadioButton写在同一个组件下即可实现单选

   - text：文字显式
   - checked：是否处于选中状态，默认为false

# 三、QML与c++的交互

---

## 1.QMLRegisterType的使用

​		QMLRegisterType是一个可以将c++实现的类在QML中调用的，连接c++和QML的一个工具，其格式为

```c++
//qmlRegisterType<Calss>(const char* uri, int versionMajor, int versionMinor, const char* qmlName)
qmlRegisterType<MYPlay>("com.imooc.myplayer", 1, 0, "MYPlay");
qmlRegisterType<MYVideoOutput>("com.imooc.myplayer", 1, 0, "MYVideoOutput");
```

`<>`中为c++众的类名，第一个参数指的是QML中import后的内容，相当于头文件名，第二个第三个参数分别是主次版本号，第四个指的是QML中类的名字。第四个参数首字母一定要大写，否则会报很难排查的错误。

可以将多个类卸载同一个import头文件中，如上所示。

在QML中如果需要对应函数和属性需要使用其id。

c++的类如不需要显示继承自QObject即可，如需要显示继承自响应的QML显示类即可，如QQuickItem，QQuickPaintedItem等。

QMLRegisterSingletonType：

这是一个带回到的注册函数，作用是将C++类注册到QML系统中并带一个回到函数，该回调函数在每次qml中调用该类函数的时候调用，然后在回调函数里产生新的c++类提供给QML调用，此方法可以实现c++类的动态加载和动态切换。

```cpp
qmlRegisterSingletonType<MainAPP>("com.imooc.myplayer", 1, 0, "MainApp", &MainApp::GetInstance);
```

这些注册函数一般卸载main.cpp中，或者程序窗体框架类的初始化总，在注册时记得包含对应类的头文件。

这种情况下，在QML中需要直接使用第四个参数来进行相应函数的调用。

## 2.交互的c++类的实现

在注册完后，QML文件中import后就可以把注册函数的第四个参数当做QML组件使用，其默认属性参考C++所继承的类，比如继承自QQuickItem类（对应QML中Item组件）就会包含QML中Item组件的属性。如x，y，anchors，width，height等属性。属性的实现如下：c++类中用于储存属性值的成员，一般设置为私有，而后需要编写三个部分：

- 用于读取和更新对应属性值的函数，为了方便一般写作

  ```c++
  属性类型  属性名() const; // 是QML中的属性名，不是成员名，读取
  void set属性名(属性类型 参数); // 更新
  ```

- 信号，一般写作

  ```c++
  signals:
      void 属性名Changed(属性类型 参数);   // 只是一个信号，不需要实现
  ```

- 声明

  ```c++
  Q_PROPERTY(属性类型  属性名 READ 读取函数名 WRITE 更新函数名 NOTIFY 信号函数名)
      // 结尾无封号，注意里面都是函数名，不带括号和其他东西
  ```

​		声明和信号照着编写即可，无特殊问题，需要注意的是函数的编写，读取函数的时薪直接返回对应的用于存储属性值的成员即可，更新函数在编写时为了节省资源，一般会先比较传递过来的值是否与用于存储属性值的成员形同，如果相同就不再更新值，也不发送更新信号。更新函数在赋值过后需要发送值改变的信号，信号会发送给QML让他更新值。

发送信号的语句为：

```c++
emit 属性名Changed(用于存储属性值的成员); // 只要发送对应信号，QML就会更新对应属性的值
```

函数的调用：如果需要在QML中调用c++类中的函数，则需要在函数声明前添加关键字Q_INVOKABLE

```c++
Q_INVOKABLE void testFuc();
```



## 3.QML中对c++类的使用

在c++类注册以及QML中import后，QML中ji7u可以将类当做QML组件来使用：

```c++
MYPlay {
    id:myplay
}
```

这样QML中就写入了C++的类，尝试编写自己的属性：

假如c++头文件实现如下：

```c++
#ifndef TEST_H
#define TEST_H

#include  <QObject>
class Test : public QObject
{
  Q_OBject
  Q_PROPERTY(int testNum READ testNum WRITE setTestNum NOTIFY testNumChanged)
public:
    Test();
    ~Test();
public:
	Q_INVOKABLE void changedTestNum(int newValue);
    
    int testNum() const;
    void setTestNum(int newValue);
 signals:
    void testNumChaned(int newValue);
 private:
    int m_mTestNum{0};
};

#endif // TEST_H
```

cpp:

```c++
#include "Test.h"

Test::Test() {}
Test::~Test() {}

void Test::changeTestNum(int newValue)
{
    setTestNum(newValue);
}

int Test::testNum() const 
{
    return m_mTestNum;
}

void Test::setTestNum(int newValue)
{
    if(m_mTestNUm not_eq newValue) {
		m_mTestNum = newValue;
        emit testNumChanged(m_mTestNum);
    }
}
```

​		上面我们定义了一个属性testNum，它已经可以在QML中使用，我们还编写了一个可以在QML中调用的函数`changeTestNum`

QML：

```qml
Item {
	id: item
	width: 50
	height: 50
	property int basic: test.testNum // 自定义属性绑定了test的属性testNUm
	property int advanced:
	{
		item.width = 60
		item.height = 60
		console.log(Test.testNum)
		console.log(item.width)
		console.log(item.height)
		console.log(basic)
		
		return test.testNum
	}
	
	Test {
		id:test
	}
	MouseArea {
		anchors.fill: parent
		onClicked: {
			test.changeTestNum(5)
		}
	}
}
```

​		定义了一个50X50大小的Item，设置了两个自定义属性都绑定了test的testNum属性，我们还定义了一个MouseArea空间填满了整个Item，当我们单击Item的时候，它会调用Test类的changeTestNum函数来更新m_mTestNum的值，由于一开始默认值为0，现在传入的值为5，所以会更新他的值并发送testNumChanged信号。信号首先影响testNum属性，被更新为5，然后两个绑定到testNum属性的自定义属性，basic的值也会被更新为5，而advanced的值会使得它所包含的一系列语句同步输出响应调试信息，这种较为高级的用法在我们实现实时更换语言的时候会用到。	

​		如果我们再次点击item的时候，由于T类的m_mTestNum已经为5，则它并未执行更新并发送信号的语句，响应的值不会再次刷新，也就不会再输出响应的调试信息了。



### 4.QML中把c++类的相应组件当作对象传回给c++

​		QML中的每一个组件相当于一个QObject，我们如果需要在QML中吧一个C++类的组件当做对象传给另一个C++类的话，只需要给需要获取的C++类写一个单回执属性为QOBject*的属性即可：

```c++
// 第一个类GetTest类
Q_PROPERTY(QObject* source READ source WRITE setSource NOTIFY sourceChanged)
public:
	QObject* source() const;
	void setSource(QObject* source);
private:
	QObject* m_mSource{nullptr};

// 第二个类为Test类
```

QML中：

```qml
GetTest {
	id: gettest
	source: test // 将test组件直接赋给source即可
}
Test {
	id: test
}
```

​	我们在获取这种对象后一般是要将其转换会对应类对象的，其转换语句为：

```c++
//GetTest类中
Test* test = qobject_cast<Test*>(source());
```



# 四、C++补充

---

## 1.List

list将元素按准许存储在链表中，与向量vector想比，它允许快速的插入和删除，但是随机访问比较慢

| 函数            | 作用                         |
| --------------- | ---------------------------- |
| assign()        | 给list赋值                   |
| back()          | 返回最后一个元素             |
| begin()         | 返回指向第一个元素的迭代器   |
| clear()         | 删除所有元素                 |
| empty()         | 判空                         |
| end()           | 返回尾后的迭代器             |
| erase()         | 删除一个元素                 |
| front()         | 返回第一个元素               |
| get_allocator() | 返回list的配置器             |
| insert()        | 插入一个元素到list中         |
| max_size()      | 返回list能容纳的最大元素数量 |
| merge()         | 合并两个list                 |
| pop_back()      | 删除最后一个元素             |
| pop_front()     | 删除第一个元素               |
| push_back()     | 尾插                         |
| push_front()    | 头插                         |
| rbegin()        | 首元素逆向迭代器             |
| remove()        |                              |
| remove_if()     |                              |
| rend()          |                              |
| resize()        |                              |
| reverse()       | 倒转元素                     |
| size()          |                              |
| sort()          |                              |
| splice()        |                              |
| swap()          |                              |
| unique()        | 去重                         |

## 2.mutex（互斥锁）

## 3.string

​		QML中可以直接使用一些基础的C++函数，比如string中的to_string和substring函数。



# 五、操作

---

## 1.语言翻译更新

1. 在Pro文件中添加TRANSLATIONS=xxxx.ts
2. 编译程序后，在QT creator中 工具-外部-qt语言家-更新翻译   ，会生成.ts文件
3. 使用Qt linguist打开.ts文件选择对应语句填写对应译文并勾选翻译内容
4. Qt creator 工具-外部-qt语言家-发布更新 , 生成.qm文件
5. 在项目中加载.qm文件





