# OOP：object oriented programming

​		面向对象编程，以“封装、继承、多态”为基础，以设计模式为原则进行的客观抽象编程。

----

​		面向对象程序设计(Object Oriented Programming)作为一种新方法，其本质是以建立模型体现出来的抽象思维过程和[面向对象](https://baike.baidu.com/item/面向对象/2262089?fromModule=lemma_inlink)的方法。

​		模型是用来反映现实世界中事物特征的。任何一个模型都不可能反映客观事物的一切具体特征，只能对事物特征和变化规律的一种抽象，且在它所涉及的范围内更普遍、更集中、更深刻地描述客体的特征。

​		通过建立模型而达到的抽象是人们对客体认识的深化。

​		面向对象程序设计以对象为核心，该方法认为程序由一系列对象组成。类是对现实世界的抽象，包括表示静态属性的数据和对数据的操作，对象是类的实例化。对象间通过消息传递相互通信，来模拟现实世界中不同实体间的联系。在面向对象的程序设计中，对象是组成程序的基本模块。

## 特性

### 封装性：

封装是指将一个计算机系统中的数据以及与这个数据相关的一切操作语言（即描述每一个对象的属性以及其行为的程序代码）组装到一起，一并封装在一个有机的实体中，把它们封装在一个“模块”中，也就是一个类中，为软件结构的相关部件所具有的模块性提供良好的基础。在[面向对象技术](https://baike.baidu.com/item/面向对象技术/1595503?fromModule=lemma_inlink)的相关原理以及程序语言中，封装的最基本单位是对象，而使得软件结构的相关部件的实现“[高内聚](https://baike.baidu.com/item/高内聚/5296411?fromModule=lemma_inlink)、低耦合”的“最佳状态”便是面向对象技术的封装性所需要实现的最基本的目标。对于用户来说，对象是如何对各种行为进行操作、运行、实现等细节是不需要刨根问底了解清楚的，用户只需要通过封装外的通道对计算机进行相关方面的操作即可。大大地简化了操作的步骤，使用户使用起计算机来更加高效、更加得心应手。

### 继承性：

继承性是面向对象技术中的另外一个重要特点，其主要指的是两种或者两种以上的类之间的联系与区别。继承，顾名思义，是后者延续前者的某些方面的特点，而在面向对象技术则是指一个对象针对于另一个对象的某些独有的特点、能力进行复制或者延续。如果按照继承源进行划分，则可以分为单继承（一个对象仅仅从另外一个对象中继承其相应的特点）与多继承（一个对象可以同时从另外两个或者两个以上的对象中继承所需要的特点与能力，并且不会发生冲突等现象）；如果从继承中包含的内容进行划分，则继承可以分为四类，分别为取代继承（一个对象在继承另一个对象的能力与特点之后将父对象进行取代）、包含继承（一个对象在将另一个对象的能力与特点进行完全的继承之后，又继承了其他对象所包含的相应内容，结果导致这个对象所具有的能力与特点大于等于父对象，实现了对于父对象的包含）、受限继承、特化继承。

### 多态性：

从宏观的角度来讲，多态性是指在面向对象技术中，当不同的多个对象同时接收到同一个完全相同的消息之后，所表现出来的动作是各不相同的，具有多种形态；从微观的角度来讲，多态性是指在一组对象的一个类中，面向对象技术可以使用相同的调用方式来对相同的函数名进行调用，即便这若干个具有相同函数名的函数所表示的函数是不同的。 [2] 



## 技术

### 动态绑定/静态绑定

### 消息传递

### 接口与实现



## 设计原则

1) 单一职责原则
2) 接口隔离原则
3) 依赖倒转(倒置)原则
4) 里氏替换原则
5) 开闭原则
6) 迪米特法则
7) 合成复用原则

> [(232条消息) 设计模式的7大原则_阿翔同学的博客-CSDN博客_设计模式原则](https://blog.csdn.net/qq_44065088/article/details/108097839)
>
> [设计模式六大原则是什么-常见问题-PHP中文网](https://m.php.cn/faq/470016.html)



### 设计方法

​		23种设计模式：

> [设计模式简介 | 菜鸟教程 (runoob.com)](https://www.runoob.com/design-pattern/design-pattern-intro.html)

- 创建型
  - 工厂模式（Factory Pattern）
  - 抽象工厂模式（Abstract Factory Pattern）
  - 单例模式（Singleton Pattern）
  - 建造者模式（Builder Pattern）
  - 原型模式（Prototype Pattern）
- 结构型
  - 适配器模式（Adapter Pattern）
  - 桥接模式（Bridge Pattern）
  - 过滤器模式（Filter、Criteria Pattern）
  - 组合模式（Composite Pattern）
  - 装饰器模式（Decorator Pattern）
  - 外观模式（Facade Pattern）
  - 享元模式（Flyweight Pattern）
  - 代理模式（Proxy Pattern）
- 行为型
  - 责任链模式（Chain of Responsibility Pattern）
  - 命令模式（Command Pattern）
  - 解释器模式（Interpreter Pattern）
  - 迭代器模式（Iterator Pattern）
  - 中介者模式（Mediator Pattern）
  - 备忘录模式（Memento Pattern）
  - 观察者模式（Observer Pattern）
  - 状态模式（State Pattern）
  - 空对象模式（Null Object Pattern）
  - 策略模式（Strategy Pattern）
  - 模板模式（Template Pattern）
  - 访问者模式（Visitor Pattern）
- 其它模式
  - MVC 模式（MVC Pattern）
  - 业务代表模式（Business Delegate Pattern）
  - 组合实体模式（Composite Entity Pattern）
  - 数据访问对象模式（Data Access Object Pattern）
  - 前端控制器模式（Front Controller Pattern）
  - 拦截过滤器模式（Intercepting Filter Pattern）
  - 服务定位器模式（Service Locator Pattern）
  - 传输对象模式（Transfer Object Pattern）





## 面向对象设计优点

​		面向对象出现以前，结构化程序设计是程序设计的主流，结构化程序设计又称为[面向过程](https://baike.baidu.com/item/面向过程?fromModule=lemma_inlink)的程序设计。在面向过程程序设计中，问题被看作一系列需要完成的任务，函数（在此泛指例程、函数、过程）用于完成这些任务，解决问题的焦点集中于函数。其中函数是面向过程的，即它关注如何根据规定的条件完成指定的任务。

比较面向对象程序设计和面向过程程序设计，还可以得到面向对象程序设计的其他优点：

1）数据抽象的概念可以在保持外部接口不变的情况下改变内部实现，从而减少甚至避免对外界的干扰；

2）通过继承大幅减少[冗余](https://baike.baidu.com/item/冗余/3687555?fromModule=lemma_inlink)的代码，并可以方便地扩展现有代码，提高编码效率，也减低了出错概率，降低软件维护的难度；

3）结合[面向对象分析](https://baike.baidu.com/item/面向对象分析/5243885?fromModule=lemma_inlink)、面向对象设计，允许将问题域中的对象直接映射到程序中，减少软件开发过程中中间环节的转换过程；

4）通过对对象的辨别、划分可以将软件系统分割为若干相对为独立的部分，在一定程度上更便于控制[软件复杂度](https://baike.baidu.com/item/软件复杂度?fromModule=lemma_inlink)；

5）以对象为中心的设计可以帮助开发人员从静态（属性）和动态（方法）两个方面把握问题，从而更好地实现系统；

6）通过对象的聚合、联合可以在保证封装与抽象的原则下实现对象在内在结构以及外在功能上的扩充，从而实现对象由低到高的升级。 [3] 



# c++对OOP的支持

- 具体类
- 抽象类
- 类层次中的类

## 具体类

​		具体类的基本思想是，使类的行为如同内置类型一样。

​		具体类的典型特征是，它的表现形式（成员构成）是其定义的一部分。其表现形式可以是直接的子对象，也可以是指向别处的子对象指针。

> 大多数类都以子对象的指针作为其表现形式，其实现在时空上可以达到最优：
>
> - 具体类可以放置与栈、静态内存，或其他对象中
> - 可以直接引用具体类对象（不仅仅通过指针来进行引用）
> - 创建对象后可以立即进行完整的初始化（如使用构造函数）
> - 拷贝对象
>
> 这种具体类，通常可以看做是带有精致接口的资源管理器。

很多有用的操作如果不需要直接访问类的表现形式，它们的定义可以与类的定义分离开来。即定义在类外。



## 抽象类

​		具体类的特征是表现形式即定义的一部分，它们与内置类型很相似。

​		抽象类型则将使用者使用的类，与表现形式完全隔离开来，只能通过所使用的类的接口来进行操作。

​		为了做到完全隔离，将接口分离到使用者所使用的类中，并在该类中放弃所有的纯局部变量。因为对该抽象类型的表现形式一无所知（甚至是大小），所以必须从动态内存中为对象分配内存（运行时提供创建表现形式所需的信息），然后通过指针或引用访问表现形式所分离的对象。

​		因此，抽象类的作用即成为使用者使用表现形式的完全意义上的纯粹接口。

```c++
class Container
{
public:
	virtual double& operator[](int) = 0;
    virtual int size() const = 0;
    virtual ~Container() {}
};
```

​		通过`virtual`提供的虚函数机制，关联了表现形式分离的类与接口类，通过重写虚函数，不同的表现形式可以通过一致的接口进行使用。

​		含有`=0`纯虚函数的类，即成为抽象类。抽象类本身不能进行初始化，必须关联到某个表现形式的类，将接口和实现对接才能形成完整的具体类进行初始化。（通过继承关联）

​		这个为其他类提供接口的抽象类，称为多态类型（polymorphic type）。即完成了对多态性的支持。

```c++
class VectorContainer : public Container
{
public: 
    VectorContainer(int s) : v{s} {0}
    ~VectorContainer(){}
    
    double& operator[](int i) { return v[i]; }
    int size() const { return v.size(); }
private: 
    Vector v;
};

class ListContainer : public Container
{
public: 
    ListContainer(int s) : v{s} {0}
    ~ListContainer(){}
    
    double& operator[](int i) { 
    	for(auto& x : v) {
            if(i == 0) return x;
            --i;
        }
         throw out_of_range("List container");
    }
    int size() const { return v.size(); }
private: 
    List v;
};

void main()
{
    Container& c = VectorContainer{1, 2, 3, 4};
    const int size = c.size();
    
    for(int i{0}; i not_eq size; ++i)
        cout << c[i] << ' ';
}
```

