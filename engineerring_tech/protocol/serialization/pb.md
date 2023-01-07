# protocol buffer

> https://developers.google.cn/protocol-buffers/docs/overview
>
> https://github.com/protocolbuffers/protobuf

---

​		protocol buffer （简称，pb）是谷歌开发的一种用于网际通信交互（协议 ）中数据存储的数据描述语言，也即序列化、编码，能够将结构化数据序列化。

> **序列化：** 
>
> ​		将有对应关系的数据，编码成无意义字节流或单一字符串】

​		protocol buffer是二进制格式， json/xml是文本格式。pb具有体积小，速度快，更好的向上、向下的兼容性优势。



## 使用方法

1. 编写.proto文件来定义协议
2. 使用protoc工具生成指定语言的API
   - `c++`，每个`.proto`生成一个`.h`和一个`.cc`，每个消息生成一个类
   - `java`，生成一个``.java``文件，包含每个消息类型的类，以及用于创建消息类实例的特殊Builder类。  
   - `python`,Python编译器会在.proto中生成一个带有每种消息类型的静态描述符的模块，然后与元类一起使用，在运行时创建必要的Python数据访问类。
   - `go`，生成一个`.pb.go`，每个消息生成一个类型
3. 将生成的API源码文件添加到工程中使用， 联编
   - 包括`setting/getting`字段，`serializing/parsing`输出输出流



## proto语法

### 语法版本

​		pb语法迭代了三个版本，部分语法不兼容。

​		需要在文件的 第一行/首行 指定使用的protobuf语法版本

```protobuf
syntax="proto3";
```

- 缺省为proto2，现一般使用proto3
- 所谓第一行/首行，指非空非注释的第一个文本行



### 导入

​		使用 import 将其他的``proto``文件导入来进行使用。

```protobuf
import "myproject/other protos.proto";
```

​		如果想将被导入的``proto``文件移动到一个新的位置，直接移动``proto``文件，需要在所有`import`该文件的进行更改。

​		可以通过间接`import`的方式来将`import`请求转发：

- 在新位置创建新的`proto`文件，将需要移动的``proto``文件中的定义拷贝到这个文件中
-  使用```improt public```来将`import`向下一级`import`传递

```protobuf
// new.proto

// ... all definitions are moved here
```

```protobuf
// old.proto
// this is the proto that all clients are improting.
import public "new.proto";

import "other.proto";
```

```protobuf
// client.proto
import "old.proto";
// You use definitions from old.proto and new.proto, but not other.proto
```

​		也就是说，默认的import是一种私有的导入，只能保留在当前proto文件中，不能间接地的导入。

​		protobuf 编译器命令```-I/ --proto_path``` ，在指定的一组目录中搜索`import`的文件

- 如果没有给出任何标志，则在调用编译器的目录中查找
- 通常，应该将```proto_path```设置为工程根目录，并对所有导入使用完全限定路径（从项目根目录出发的路径）



​	可以在定义为`syntax=proto2`的`proto`文件中导入定义为`syntax=proto3`的文件，反之也成立。

> 但需要注意，导入的`proto2`枚举不能用在`proto3`定义的消息中使用，但导入的`proto2`消息中使用`proto2`枚举是可以的。

### 消息

​		消息格式中的每一项对应一个字段，是承载具体数据的基元。

​		`type field_name = number`

```protobuf
message SearchRequest {
	string 	query = 1;
	int32	page_number = 2;
	int32	result_per_page = 3;
	repeated Result result = 1;
}

message Result {
  required string url = 1;
  optional string title = 2;
  repeated string snippets = 3;
}
```

- type可以是标量类型、枚举类型或其他的消息

#### 字段编号

​		在消息中，每个字段都有唯一的一个数字标识符。

​		这些标识符是用来在消息的二进制格式中识别各个字段的，一旦使用就不能够再改变。

- [1,15] 的标识编号在编码的时候会占用一个字节。

- [16,2047] 之内的标号则占用2个字节

  > 所以应该为那些频繁出现的消息元素保留【1,15】之内的标识编号
  >
  > 	要为将来有可能添加的、频繁出现的字段预留一些标识号

- 最小的标识号可以从1开始，最大 2^29 -1 ，即 536,870,911

  > ​		其中，[19000,19999] 字段编号是protobuf本身实现中的保留标识编号。
  >
  > > ​		从FieldDescriptor::kFirstReservedNumber 到 FieldDescriptor::kLastReservedNumber
  >
  > ​		在.proto文件中使用这些预留标识号，编译时就会报警。也不能使用先前定义的预留字段编号。

#### 字段约束

**proto2**

- `required`： 必须使用的字段
  - 每个格式良好的消息必须有且只有一个`required`字段

- `optional`：可选的字段，0或1个此字段
  - 未显式指明时，默认为optional

- `repeated`：字段可以出现0或任意多次
  - 重复的值的顺序会被保留
  - 重复字段视为动态大小的数组


> ​		由于历史原因，对标量数值类型的`repeated`字段的编码没有达到应有的效率。新代码应该使用特殊选项``[packed=true]``来获得更有效的编码。例如:
>
> ```protobuf
> repeated int32 samples = 4 [packed=true];
> ```
>

> **NOTE:**
>
> ​		`required`字段的使用必须小心。如果在系统中，一方将此字段改为`optional`字段，其他各方在处理含有该字段的消息时，可能因此会拒绝或丢弃该消息。
>
> ​		即对更改敏感。

**proto3**

- `singular`：可以出现0或1次的字段。
  - proto3中的默认字段约束
- `repeated`：可以出现0或任意多次，重复的值的顺序会被保留。
  - proto3中 `repeated`的标量类型会默认使用 packed  编码

> 在proto3中，删除了``required``约束和``optional``约束，但是隐含的，所有的字段都是``optional``的。
>
> 在proto3中，缺省默认隐含所有字段都是``singular``的。
>
> 原因在于：
>
> > **Required Is Forever** You should be very careful about marking fields as `required`. If at some point you wish to stop writing or sending a required field, it will be problematic to change the field to an optional field – old readers will consider messages without this field to be incomplete and may reject or drop them unintentionally. You should consider writing application-specific custom validation routines for your buffers instead. Some engineers at Google have come to the conclusion that using `required` does more harm than good; they prefer to use only `optional` and `repeated`. However, this view is not universal.
> >
> > **Required永远都是必须的**，在将字段标记为Required时应该非常小心。如果您希望在某个时候停止写入或发送必需的字段，那么将该字段更改为可选字段将会产生问题——老的readers会认为没有该字段的消息是不完整的，可能会无意中拒绝或删除它们。您应该考虑为缓冲区编写特定于应用程序的自定义验证例程。**谷歌的一些工程师已经得出结论，使用必需的产品弊大于利;他们喜欢只使用可选和重复**。然而，这种观点并不普遍。
>
> 所以在proto3中，只有单一和重复字段约束。



#### 保留字段

​		更新字段时，通过使用`reserved`来约束字段，使得使用该字段会被pb编译器报错。

```protobuf
enum Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar"; // 注意，不能在同一个保留语句中混合（同时使用）字段名和字段号。
}
```

> ​		如果直接删除或注释该字段，以后对该消息的更改会重用该字段的编号。如果对该更改却加载了旧版本的`.proto`文件，会导致严重的问题，包括数据损坏、隐私错误。
>
> ​		JSON也有相同的问题。
>
> > If you [update](https://developers.google.cn/protocol-buffers/docs/proto3#updating) a message type by entirely removing a field, or commenting it out, future users can reuse the field number when making their own updates to the type. This can cause severe issues if they later load old versions of the same `.proto`, including data corruption, privacy bugs, and so on. One way to make sure this doesn't happen is to specify that the field numbers (and/or names, which can also cause issues for JSON serialization) of your deleted fields are `reserved`. The protocol buffer compiler will complain if any future users try to use these field identifiers.



#### 字段默认值

**proto2**

​		`optional`字段可以设置默认值，当消息中不包含可选字段，解析时对应字段将使用该默认值。

```protobuf
optional int32 result_per_page = 3 [default = 10];
```

​		默认值缺省：

- string，空符串
- bytes，空字节串
- bool，false
- 数字类型，0
- 枚举类型，枚举中定义的第一个枚举值，0,
- 消息，没有设置，确切的值与语言相关

**proto3**

​		如果编码的消息中不包含对应的：

- `singular`字段，解析编码消息中相应字段会使用该字段的默认值。
- `repeated`字段，用语言相关的特性表示空列表。



​		对于标量字段，一旦解析了消息，就无法判断字段是被显式赋值为该默认值，还是缺省使用了默认值。

> 在定义消息时，尽量避免因为默认值而导致的 不设置/默认设置 之间的冲突。

​		标量字段的值如果在序列化时是默认值，那么不会将该字段序列化到编码串中。而是又各方产生的代码硬编码进代码中。

> 更多细节见语言相关的API



#### （proto3）未知字段

​		未知字段是格式良好的pb序列化数据，表示解析器无法识别的字段。

> 例如，当老的编码解析收到带有新字段的编码串，这些新字段将成为老的编码解析中的未知字段。

​		最初，proto3消息总是在解析过程中丢弃未知字段，但在3.5版本中，重新引入了未知字段的保留，以匹配proto2的行为。

​		在3.5及更高版本中，未知字段在解析过程中被保留，并包含在序列化的输出中。



#### 消息族

​		在一个proto文件中可以定义多个消息类型。

​		一般将多个相关的消息定义在一个proto文件中，如一个request和一个response

```protobuf
message SearchRequest {
	string 	query = 1;
	int32	page_number = 2;
	int32	result_per_page = 3;
}

message SearchResponse {
	...
}
```

> **NOTE：**
>
> ​		可以在一个``.proto``文件中定义多种消息(如message、enum和service)，但当在一个文件中定义了大量具有不同依赖关系的消息时，也会导致依赖关系膨胀。
>
> ​		建议每个.proto文件包含尽可能少的消息



#### 嵌套消息

​		可以在消息内部中定义、使用消息。

```protobuf
// proto2
message SearchResponse {
  message Result {
    required string url = 1;
    optional string title = 2;
    repeated string snippets = 3;
  }
  repeated Result result = 1;
}

// proto3
message SearchResponse {
	message Result {
		string url = 1;
		string title = 2;
		repeated string snippets = 3;
	}
	
	repeated Result results = 1;
}
```

​		如果要在外部复用这个内部定义的message，你需要以```_Parent_._Type_```的语法形式来使用。

```protobuf
message SomeOtherMessage {
	// proto2
	optional SearchResponse.Result result =1;
	// proto3
	SearchResponse.Result result = 1;
}
```

可以任意多层的嵌套消息。

```protobuf
// proto2
message Outer {       // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      optional int64 ival = 1;
      optional bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      optional string name = 1;
      optional bool   flag = 2;
    }
  }
}

// proto3
message Outer {		// level 0
	message MiddleA{ // level 1
		message Inner { // level 2
			int64 ival = 1;
			bool booly = 2;
		}
	}
	
	message MiddleB{ // level 1
		message Inner{	// level 2
			int32 ival = 1;
			bool booly = 2;
		}
	}
}
```



#### (proto2)消息组

​		一种已弃用的匿名嵌套定义消息类型方式

```protobuf
message SearchResponse {
    repeated group Result = 1 {
     required string url = 2;
     optional string title = 3;
     repeated string snippets = 4;
    }
}
```

​		组只是将嵌套消息类型和字段组合到单个声明中。 

​		可以将此消息视为具有一个名为``Result``的``result``类型字段(后一个名称被转换为小写，以便与前一个名称不冲突)。

​		 因此，这个示例与上面的`SearchResponse`完全相同，只是消息具有不同的编码串格式



####  更新消息类型

​		对消息进行更新时，尽量小的避免更改与此相关的代码，需要记住以下规则:

- 不要更改任何现有字段的标识编号

- 添加了新字段

  - 使用“旧”消息格式由代码序列化的任何消息仍然可以由新生成的代码解析
    - （proto2）都应该是`repeated`或`optional`的
    - 因为它们不会丢失任何必需的元素
  - 设置合理的默认值，以便新代码能够正确地与旧代码生成的消息交互
  - 新代码创建的消息可以由旧代码解析：旧二进制文件在解析时简单地忽略新字段。形成**未知字段**

- 可以删除字段，只要字段标识编号不会在更新后的消息类型中再使用。

  - （proto2）非`required`字段可以被删除
  - 相反，您可能想重命名字段，可以添加前缀```“OBSOLETE_”```，或者保留字段号，以便将来的``.proto``用户不会意外地重用该数字

- （proto2）可以将非`required`字段转换为`extension`，反之亦然。只要类型和字段编号保持不变

- `int32`、``uint32``、``int64``、``uint64``和``bool``都是兼容的

  - 可以将字段从其中一种类型更改为另一种类型，而不破坏向前或向后兼容性。
  - 如果从编码串中解析出一个不适合对应类型的数字，将得到与在c++中强制转换效果相同的结果
    - (例如，如果一个64位的数字被读取为int32，它将被截断为32位)。

- `sint32`和``sint64``彼此兼容，但与其他整数类型不兼容

- 字符串和字节串是兼容的，只要字节是有效的UTF-8

- 如果字节包含消息的编码版本，则嵌入的消息与字节兼容。

- fixed32与sfixed32兼容，fixed64与sfixed64兼容。

- 对于字符串、字节串和消息字段，可选与重复兼容。

  - 给定``repeated``字段的序列化数据作为输入，如果该字段是原始类型字段，则期望该字段为可选的客户端将接受最后一个输入值;如果该字段是消息类型字段，则将合并所有输入元素。
  - 注意，对于数字类型，包括布尔和枚举，这通常是不安全的。数值类型的重复字段可以以打包格式序列化，当需要可选字段时，将不能正确解析这些字段。

- 更改默认值通常是可以的

  - 默认值永远不会通过网络发送 

    > 如果一个程序接收到一个没有设置特定字段的消息，该程序将会看到在该程序的协议版本中定义的默认值。 
    >
    > 它将看不到在发送方代码中定义的默认值。  

- 就编码格式而言，``enum``兼容``int32``、``uint32``、``int64``和``uint64``(注意，如果不合适，值将被截断)。

  - 但是请注意，在对消息进行反序列化时，客户端代码可能会以不同的方式对待它们

    > 例如，未识别的proto3 enum类型将保留在消息中，但是在对消息进行反序列化时如何表示它是依赖于语言的。Int字段总是保留它们的值。

- 将一个值更改为一个新值的成员是安全的和二进制兼容的。

  如果您确信没有代码一次设置多个字段，那么将多个字段移到一个新的字段中可能是安全的。将任何字段移动到现有字段中是不安全的。





### 标量类型

​		表格展示了定义于proto文件中的类型，以及生成代码中与之对应的相应编程语言的内置类型。



| proto type | Notes                                                        | C++ type |
| ---------- | ------------------------------------------------------------ | -------- |
| double     |                                                              | double   |
| float      |                                                              | float    |
| int32      | 使用变长编码，对于负值的效率很低，如果字段有可能是负值，使用sint32来替换 | int32    |
| int64      | 同上                                                         | int64    |
| uint32     | 使用变长编码                                                 | uint32   |
| uint64     | 使用变长编码                                                 | uint64   |
| sint32     | 使用变长编码，当字段为负值时，比int32高效得多                | int32    |
| sint64     | 使用变长编码，当字段为负值时，比int64高效得多                | int64    |
| fixed32    | 固定4字节，如果数值总是比2^28大，会比uint32高效              | uint32   |
| fixed64    | 固定8字节，如果数值总是比2^56大，会比uint32高效              | uint64   |
| sfixed32   | 总是4字节                                                    | int32    |
| sfixed     | 总是8字节                                                    | int64    |
| bool       |                                                              | bool     |
| string     | 必须是utf-8或者7-bit ASCII编码的文本                         | string   |
| bytes      | 可以包含不超过2^32的任意字节序列                             | string   |

​		各种类型的序列化/编码细节见：`编码原理`

> - 在Java中，32位和64位的无符号整数都使用有符号的对应数字表示，最高位被存储在标志位中。
> - 【*】对于所有情况，设定值会执行 类型检查以确保此值是有效的 
> - 64位或者无符号32位整型在解码时被表示成long，但是在设置时可以使用int型值设定，在所有情况下，值必须符合其类型的要求
> - python中，string被再解码时被表示成Unicode，但是一个ASCII string可以被表示成str类型
> - integer在64位机器上使用，string在32位机器上使用



### 枚举类型

​		字段的取值只会是某一组值中的某一个

> ​		例如，假设要为每一个``SearchRequest`消息添加一个 `corpus`字段，而`corpus`的值可能是`UNIVERSAL`，`WEB`，`IMAGES`，`LOCAL`，`NEWS`，`PRODUCTS`或`VIDEO中``的一个。 其实可以很容易地实现这一点：通过向消息定义中添加一个枚举（enum）并且为每个可能的值定义一个常量就可以了。

```protobuf
// proto2
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3 [default = 10];
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  optional Corpus corpus = 4 [default = UNIVERSAL];
}


// proto3
message SearchRequest {
	string 	query = 1;
	int32	page_number = 2;
	int32	result_per_page = 3;
	enum Corpus {
		UNIVERSAL	= 0;
		WEB			= 1;
        IMAGES		= 2;
        LOCAL		= 3;
        NEWS		= 4;
        PRODUCTS	= 5;
        VIDEO		= 6；
	}
	Corpus	corpus = 4;
}
```

每个枚举类型必须将其第一个类型映射为0，这是因为：

- 必须有一个零值，以便我们可以使用0作为数值默认值。
- 零值需要时第一个元素，以便与proto2语义兼容，其中第一个枚举值总是默认值。



#### 枚举别名

在枚举类型中，将``allow_alias``选项设置为``true``，可以将相同的值分配给不同的枚举来定义别名。

> 不设置，protobuf编译器将在找到别名时生成 错误信息。

```protobuf
message MyMessage1 {
	enum EnumAllowingAlias{
	   option allow_alias = true;
        UNKNOW = 0;
        STARTED = 1;
        RUNNING = 1;
	}
}

message MyMessage2{
	enum EnumNotAllowingAlias{
		UNKNOW = 0;
		STARTED = 1;
		//RUNNING =  1;  // Uncommenting this  line will cause a compile error inside Google and a warning message outside.
	}
}
```



#### 枚举的限制

- 枚举常量被限制在32位整型值的范围内

  - 因此enum值是使用可变编码方式的，对负数不够高效，因此不推荐在enum中使用负数，

- 可以在一个消息定义的内部或外部定义枚举

  - 消息外部定义的枚举可以在proto文件的任意消息内使用
  - 定义在某个消息内部的枚举在另外的消息中使用时，需要使用```MessageType.EnumType```的语法格式

- 当您运行该协议缓冲区使用enum ``.proto``编译器,生成的代码将会有一个：

  - c++/java， 对应的枚举类型

  - 特别的为为Python，会产生一个``EnumDescriptor`类用来创建一组符号常量整数值。

> ​		**注意:**生成的代码可能会受到特定于语言的枚举数限制(某种语言的枚举数低至数千)。
>
> ​			在反序列化时，不能识别的枚举值将被保留在消息中， 尽管在反序列化消息时如何表示该值取决于语言：
>
> > ​		在支持值在指定符号范围之外的开放枚举类型的语言中，比如c++和Go，未知的枚举值被简单地存储为其基础整数表示。
> >
> > ​		在具有封闭枚举类型的语言(如Java)中，枚举中的case用于表示无法识别的值，可以使用特殊的访问器访问底层整数。在任何一种情况下，如果消息被序列化，那么无法识别的值仍将与消息一起序列化。
>

#### 枚举保留

​		同消息中的保留字段类似。

​		可以使用``max``关键字指定保留的数字值范围到可能的最大值：

```protobuf
enum Foo {
  reserved 2, 15, 9 to 11, 40 to max;
  reserved "FOO", "BAR";
}
```



### （proto2） extensions

​		`extensions`允许声明一个字段编号范围用于第三方扩展。

​		扩展是一个字段的占位符，该字段不由原始的`.proto`文件定义。这使得其他的`.proto`可以通过用字段号定义字段的类型，添加到消息中。

```protobuf
message Foo{
	extensions 100 to 199;
}
```

​		其他用户现在可以在他们自己的.proto文件中添加新的字段到Foo中，导入你的.proto中指定的字段好范围内：

```protobuf
extend Foo {
	optional int32 bar = 126;
}
```

​		当带有扩展字段的消息被编码时，编码字符串格式与拓展中定义的字段完全相同。但是程序中对于扩展的访问与常规字段的访问所有不同，需要使用用于扩展的特殊访问器：

```c++
Foo foo;
foo.SetExtension(bar, 15);
```

> 类似地，Foo类定义了模板访问器``HasExtension()``， `ClearExtension()`， `GetExtension()`， `MutableExtension()和AddExtension()`。
>
>  它们的语义都与为普通字段生成的相应访问器相匹配。
>
>  有关使用扩展的更多信息，请参见所选语言的生成代码参考。  

​	扩展可以是任何字段类型，包括消息类型，但不能是`oneof`或`maps`



##### 嵌套extensions

  可以内嵌的定义`extensions`:

```protobuf
message Baz {
  extend Foo {
  	optional int32 bar = 126;
  }
}
```

```c++
Foo foo;
foo.setExtension(Baz::bar, 15);
```

> 唯一的影响是bar被定义在Baz的范围内。
>
> 声明嵌套在消息类型中的扩展块并不意味着外部类型和扩展类型之间有任何关系。 特别地，上面的例子并不意味着Baz是Foo的任何子类。 它的意思是，符号条声明在Baz的范围内; 它只是一个静态成员。  

一个常见的模式是在扩展的字段类型的范围内定义扩展

>  例如，这里是一个Baz类型的Foo的扩展，其中的扩展被定义为Baz的一部分:  
>
> ```protobuf
> message Baz {
> extend Foo {
>  optional Baz foo_ext = 127;
> }
> }
> ```
>
> 没有要求具有消息类型的扩展必须在该类型中定义:
>
> ```protobuf
> message Baz {
>   ...
> }
> 
> // This can even be in a different file.
> extend Foo {
>   optional Baz foo_baz_ext = 127;
> }
> ```
>
> 事实上，为了避免混淆，最好使用这种语法。 如上所述，嵌套语法经常被不熟悉扩展的用户误认为是子类化。  

##### 选择 extensions 编号

​		需要注意，外部的多个`.proto`文件要避免使用相同字段编号向相同的消息添加扩展。

​		需要人为的在项目中约定扩展编号的使用，来避免发生冲突。

​		如果编号约定涉及非常大的字段号，可以使用``max``关键字指定扩展范围到最大可能的字段号: 

```protobuf
message Foo {
	extensions 1000 to max;
}
```

> `max` is 229 - 1, or 536,870,911.
>
> 扩展的字段编号也要避开`19000-19999`，（``::kFirstReservedNumber`` 到``::kLastReservedNumber``)，它们是protobuf实现所保留的。
>
> 你定义的拓展编号范围可以包含此范围，但是不能用这些数字进行实际的`extend`定义

​		







 



### 预定义字段

#### （proto3）Any

​		允许在没有指定它们的``.proto定义``的情况下作为嵌套类型来使用。

​		`Any`以序列化为``bytes``的形式包含任意消息，以及一个``URL``全局标识和解析该消息。

​		使用Any类型，需要```import google/protobuf/any.proto```

```protobuf
import "google/protobuf/any.proto"

message ErrorStatus{
	string message = 1;
	repeated goole.protobuf.Any details = 2;
}
```

​		对于给定的消息类型的默认类型URL是```type.googleapis.com/_packagename_._messagename_```

​		不同的语言实现通过运行时库`helpers`来支持类型安全的打包和解包``Any``类型的方式：

- 在Java中，Any类型将有特殊的pack()和unpack()访问器
- 在c++中有PackFrom()和UnpackTo()方法:

```c++
// Storing an arbitrary message type in Any.
NetworkErrorDetails details = ...;
ErrorStatus status;
status.add_details()->PackFrom(details);

// Reading an arbitrary message from Any.
ErrorStatus status = ...;
for (const Any& detail : status.details()) {
  if (detail.Is<NetworkErrorDetails>()) {
    NetworkErrorDetails network_error;
    detail.UnpackTo(&network_error);
    ... processing network_error ...
  }
}
```

> 目前，用于Any类型的动态库仍在开发中.

>  如果你已经熟悉了proto2语法，Any可以保存任意的proto3消息，类似于proto2消息可以允许扩展。
>
>   proto3中extend已经不可用了。

#### Oneof

​		在一组`optional`字段中，同时只有一个字段有效。使用`oneof`特性可以节省内存使用。

> proto3默认字段全部为optional

​		`oneof`字段除了在不同场景下会可以表达为不同的类型外，和普通字段是一样的。一旦设置了某个类型，其他类型的设置将会失效。可以使用依赖于语言的`case()`或`WhichOneof()`方法来检查当前哪个类型被`oneof`设置。

> 类似于C语言的union，但是似乎不能用其他未设置的字段来解析被设置的这个字段。

##### 使用Oneof

```Oneof Name { type name = number ; ... } ```

```protobuf
message SampleMessage {
	oneof test_oneof{
		string name = 4;
		SubMessage sub_message = 9;
	}
}
```

​		`oneof`中的类型可以是除`map`类型外的任意非`repeated`修饰的类型

​		在生成的代码中，``oneof``字段也拥有`` getter`` and ``setter``， 可以通过一个指定的方法来检查到底是哪个方法被设置。

**Oneof特性**：

-  设置Oneof会自动清除其他Oneof字段的值，所以设置多次后，只有最后一次设置的字段有值。

  ```c++
  SampleMessage mesasge;
  message.set_name("name");
  CHECK(message.has_name());
  message.mutable_sub_message(); // will clear name field
  CHECK(not message.has_name());
  ```

- 如果解析器在编码串中遇到同一个`oneof`中的多个成员，则只使用最后一个成员解析为消息字段。

- `oneof`字段不能是`repeated`修饰的

- 反射相关的API可以在``oneof``上生效

- 如果使用c++，需要确保代码不会导致内存泄露。下面的代码会崩溃，因为```sub_message```已经通过```set_name```删除了。

  ```c++
  SampleMessage message;
  SubMessage* sub_message = message.mutable_sub_message();
  message.set_name("name");	// will delete sub_message
  sub_message->set_...   // crashed here
  ```

- 在C++中，如果使用Swap()来交换两个带有``oneof``的消息，那么每个消息将以对方的``oneof case``结束

  ```c++
  // msg1 will have a sub_message and msg2 will have a name.
  SampleMessage msg1;
  msg1.set_name("name");
  
  SampleMessage msg2;
  msg2.mutable_sub_message();
  
  msg1.swap(&msg2);
  
  CHECK(msg1.has_sub_message());
  CHECK(msg2.has_name());
  ```

##### 向后兼容

​		在添加或删除某个字段时要小心。

​		如果检查``oneof``的值返回``None/NOT_SET``：

- 可能``oneof``没有被设置
- 被设置为``oneof``的不同版本的字段

没有办法区分，因为没有办法知道编码串中的未知字段是否是``oneof``的一个成员字段。



##### Tag重用问题

- 将字段删除或移入``oneof``

  在消息被序列化或者解析后，也许会失去一些信息（有些字段也许会被清除)

- 删除``oneof``或者添加``oneof``字段

  在消息被序列化或者解析后，这也许会清除你现在设置的``oneof``字段。

- 分离或者融合``oneof``

  行为与移动常规字段相似

#### Maps

​		创建关联映射

```protobuf
map<key_type, value_type> map_field = N;

// key_type   可以是任意整型或者string类型（除了浮点类型和字节之外的任何标量类型都可以）
// value_type 可以是任意类型

// example 
map<string, Project> projects = 3;
```

- `map`字段不能是``repeated``
  - (proto2) `repeated`/`optional`/`required`

- (proto2)`extensions`在`map`中不支持
- 序列化顺序和``map``迭代器顺序是未定义的，所以不能期望以固定顺序处理``map``
- 从``.proto``文件产生文本格式时，``map``会按照``key``的字典顺序排序，数值``key``会以数值进行排序
- 从序列化中解析或者合并时，如果有重复的key，则后一个key不会被使用
  - 当从文本格式中解析map时，如果存在重复的key，解析可能会失败。

- (proto3)如果向map提供了一个没有``value``的``key``，那么序列化时的行为依赖于语言
  - 在C++，java和python中以类型的默认值来为该``value``进行序列化
  - 在其他语言中，这个字段不会进行序列化




##### 向后兼容

​		`map`语法等效如下，因此即使是不支持map语法的protobuf实现也是可以处理你的数据的。

```protobuf
message MapFieldEntry{
	key_type key = 1;
	value_type value = 2;
}

repeated MapFieldEntry map_field = N;
```

​		任何支持映射的协议缓冲区实现都必须生成和接受可以被上述定义接受的数据。  



### 注释

​		可以使用c/c++风格的注释在`.proto`中进行注释。

```protobuf
/* SearchRequest represents a search query, with pagination options to
 * indicate which results to include in the response. */

// proto3
message SearchRequest {
  string query = 1;
  int32 page_number = 2;  // Which page number do we want?
  int32 result_per_page = 3;  // Number of results to return per page.
}

// proto2
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;  // Which page number do we want?
  optional int32 result_per_page = 3;  // Number of results to return per page.
}
```





### 包

​		用以消除名字冲突

```protobuf
package foo.bar

message Open{
	...
}
```

  使用：``packet_name.message_name``

```protobuf
message Foo{
	...
	foo.bar.Open open = 1;
	...
}
```

包的声明符会根据使用语言的不同影响生成的代码

- c++：命名空间，``foo::bart::Open``
- java：java包，除非在proto文件中显式提供了```option java_package```。
- python，这个包声明符是被忽略的，因为python模块是按照其在文件系统中的位置进行组织的。
- GO：包可以被用作Go包名称，除非显式提供```option go_package```在proto文件中
- Ruby：生成的类可以被包装在内置的ruby名称空间中，转换成ruby所需的大小写样式（首字母大写；如果第一个符号不是第一个字母，则使用PB_前缀），例如Open会在```Foo::Bar```名称空间中。
- JavaNano：包会使用java包，除非你在你的文件中显式提供一个```option java_package```
- C#：包可以转换成PscalCase后作为名称空间，除非你在你的文件中显式的提供一个```option csharp_namespace```。例如，Open会在```Foo.Bar```

#### 包及名称的解析

​		首先从最内部开始查找，一次向外进行，每个包会被看做是其父类包的内部包。当然对于```foo.bat.Baz```这样以 . 分隔的意思是从最外围作用域开始的。

> protobuf 语言中类型名称的解析与c++是一致的：

​		协议缓冲区编译器通过解析导入的.proto文件来解析所有类型名。 每种语言的代码生成器知道如何引用该语言中的每种类型，即使它有不同的作用域规则。  



### 服务 services

​		如果想要将消息类型用在rpc系统中，可以在``.proto``文件中定义一个RPC服务接口，protobuf编译器会根据所选择的不同语言生成服务接口代码及存根(stubs)。

​		如需要定义一个RPC服务并具有一个方法，该方法能够接收SearchRequest并返回一个SearchResponse，此时可以在``.proto``文件中进行如下定义：

```protobuf
service SearchService {
	rpc Search (SearchRequest) return (SearchResponse);
}
```

> ​		最直观的使用protobuf的rpc系统是gRPC，一个由谷歌开发的语言和平台无关的开源rpc系统，gRPC在使用protobuf时非常有效，如果使用特殊的protobuf插件可以直接从proto文件中产生相关的RPC代码。
>
> ​		https://grpc.io/

> 如果不想使用gRPC，也可以使用protobuf用于自己的RPC实现
>
> ​		https://developers.google.cn/protocol-buffers/docs/proto#services

> 还有一些第三方的RPC实现使用protobuf：
>
> ​		https://github.com/protocolbuffers/protobuf/blob/master/docs/third_party.md

#### protobuf RPC

​		默认情况下，协议编译器将生成一个名为``SearchService``的抽象接口和相应的“存根”实现。 

​		存根将所有调用转发到``RpcChannel``，而``RpcChannel``又是一个抽象接口，您必须根据自己的RPC系统定义这个接口。 

> ​		例如，您可以实现一个``RpcChannel``，它序列化消息并通过HTTP将其发送到服务器。 
>
> ​		换言之，生成的存根为进行基于协议缓冲区的RPC调用提供了类型安全的接口，而不会将您锁定到任何特定的RPC实现中。 所以，在c++中，你可能会得到这样的代码:  

```c++
using google::protobuf;

protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
SearchService* service;
SearchRequest request;
SearchResponse response;

void DoSearch() {
  // You provide classes MyRpcChannel and MyRpcController, which implement
  // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
  channel = new MyRpcChannel("somehost.example.com:1234");
  controller = new MyRpcController;

  // The protocol compiler generates the SearchService class based on the
  // definition given above.
  service = new SearchService::Stub(channel);

  // Set up the request.
  request.set_query("protocol buffers");

  // Execute the RPC.
  service->Search(controller, request, response, protobuf::NewCallback(&Done));
}

void Done() {
  delete service;
  delete channel;
  delete controller;
}
```

​		所有服务类也都实现了``service``接口，该接口提供了一种在编译时调用特定方法的方法，而不需要知道方法名或其输入和输出类型。 在服务器端，这可以用于实现RPC服务器，您可以使用它注册服务。  

```c++
using google::protobuf;

class ExampleSearchService : public SearchService {
 public:
  void Search(protobuf::RpcController* controller,
              const SearchRequest* request,
              SearchResponse* response,
              protobuf::Closure* done) {
    if (request->query() == "google") {
      response->add_result()->set_url("http://www.google.com");
    } else if (request->query() == "protocol buffers") {
      response->add_result()->set_url("http://protobuf.googlecode.com");
    }
    done->Run();
  }
};

int main() {
  // You provide class MyRpcServer.  It does not have to implement any
  // particular interface; this is just an example.
  MyRpcServer server;

  protobuf::Service* service = new ExampleSearchService;
  server.ExportOnPort(1234, service);
  server.Run();

  delete service;
  return 0;
}
```



#### gRPC with protobuf

​		gRPC与pb良好配合，允许使用一个特殊的pb编译器插件直接从``.proto``文件生成相关的RPC代码。 

> 然而，由于使用proto2和proto3生成的客户机和服务器之间存在潜在的兼容性问题，建议使用proto3来定义gRPC服务。
>
> 如果你想在gRPC中使用proto2语法，但需要使用3.0.0或更高版本的pb编译器和库。



### 选项 option

​		``proto``文件中的单个声明可以用多个``option``进行注释。``option`` 不会改变声明的总体含义，但可能会影响在特定上下文中处理声明的方式。

> 可用 `option` 的完整列表在
>
> ​		```google/protobuf/descriptor.proto ``` 中定义。

#### 选项级别

- 文件级别
  - 应该写入顶级作用域，不包含在任何消息内部、enum或服务定义中。
- 消息级别
  - 用在消息定义的内部。
- 字段级别
  - 在字段定义中编写
- enum类型、enum值、服务类型及服务方法中的选项

到目前为止，并没有一种有效的选项能作用于所有的类型



#### 常用选项

##### ```java_package```

​		(文件选项) :这个选项表明生成``java``类所在的包.

​		如果在``.proto``文件中没有明确的声明``java_package``，就采用默认的包名。当然了，默认方式产生的`` java``包名并不是最好的方式，按照应用名称倒序方式进行排序的。如果不需要产生``java``代码，则该选项将不起任何作用。如：

```protobuf
option java_package = "com.example.foo";
```

##### ```java_outer_classname```：

​		(文件选项): 该选项表明想要生成Java类的名称。如果在.proto文件中没有明确的java_outer_classname定义，生成的class名称将会根据.proto文件的名称采用驼峰式的命名方式进行生成。如（foo_bar.proto生成的java类名为FooBar.java）,如果不生成java代码，则该选项不起任何作用。如：

```protobuf
option java_outer_classname = "Poonycopter";
```

##### ```java_multiple_files```：

​		(文件选项):导致在包级别定义顶级消息、枚举和服务，而不是在以.proto文件命名的外部类中定义。

```protobuf
option java_multiple_files = true;
```

##### ```optimize_for```

​		(文件选项)：可以被设置成```SPEED, CODE_SIZE, LITE_RUNTIME```。这些值将通过如下的方式影响c++及java代码的生成：

-  ```SPEED (default)```

​		protobuf 编译器将通过在消息类型上执行序列化、语法分析及其他通用的操作。这种代码时最优的。

-  ```CODE_SIZE```

​		protocol buffer编译器将会产生最少量的类，通过共享或基于反射的代码来实现序列化、语法分析及各种其它操作。

> 采用该方式产生的代码将比SPEED要少得多， 但是操作要相对慢些。当然实现的类及其对外的API与SPEED模式都是一样的。这种方式经常用在一些包含大量的.proto文件而且并不盲目追求速度的 应用中。

- ```LITE_RUNTIME```

​		protocol buffer编译器依赖于运行时核心类库来生成代码（即采用libprotobuf-lite 替代libprotobuf）。

> 这种核心类库由于忽略了一 些描述符及反射，要比全类库小得多。这种模式经常在移动手机平台应用多一些。编译器采用该模式产生的方法实现与SPEED模式不相上下，产生的类通过实现 MessageLite接口，但它仅仅是Messager接口的一个子集。

```protobuf
option optimize_for = CODE_SIZE;
```

##### ```cc_enable_arenas```

​		（文件选项）：对于c++产生的代码启用 arena allocation

https://developers.google.cn/protocol-buffers/docs/reference/arenas

##### ```objc_class_prefix```

​		（文件选项）：设置Objective-C类的前缀，添加到所有Objective-C从此.proto文件产生的类和枚举类型。没有默认值，所使用的前缀应该是苹果推荐的3-5个大写字符，注意2个字节的前缀是苹果所保留的。

##### ```deprecated```

​		（字段选项）：如果设置为`true`则表示该字段已经被废弃，并且不应该在新的代码中使用。

> 在大多数语言中没有实际的意义。在java中，这回变成`@Deprecated`注释，在未来，其他语言的代码生成器也许会在字标识符中产生废弃注释，废弃注释会在编译器尝试使用该字段时发出警告。如果字段没有被使用你也不希望有新用户使用它，尝试使用保留语句替换字段声明。

```protobuf
message A {
	int32 old_field = 6 [deprecated = true];
}
```

#### 自定义选项

​		`protobuf`允许自定义并使用选项。该功能是高级特性，对于大部分人是用不到的。

>  注意创建自定义选项使用了拓展，拓展只在proto3中可用。

​		自定义选项是对`google/protobuf/descriptor.proto`中选项的拓展，因此，可以如下定义自定义选项：

```protobuf
import "google/protobuf/descriptor.proto";

extend google.protobuf.MessageOptions {
  optional string my_option = 51234;
}

message MyMessage {
  option (my_option) = "Hello world!";
}
```

​		使用该选项时，选项名必须用圆括号括起来，以表明它是扩展名。

```c++
// c++中读取my_option的值
string value = MyMessage::descriptor()->options().GetExtension(my_option);

// MyMessage::descriptor()->options()  返回`MyMessage的MessageOptions 协议消息
// 同读取其他扩展一样来读取自定义选项
```

```java
// java中读取my_option的值
String value = MyProtoFile.MyMessage.getDescriptor().getOptions()
  .getExtension(MyProtoFile.myOption);
```

```python
# python
value = my_proto_file_pb2.MyMessage.DESCRIPTOR.GetOptions()
  .Extensions[my_proto_file_pb2.my_option]
```

​		可以为每种类型的构造定义自定义选项:

```protobuf
import "google/protobuf/descriptor.proto";

extend google.protobuf.FileOptions {
  optional string my_file_option = 50000;
}
extend google.protobuf.MessageOptions {
  optional int32 my_message_option = 50001;
}
extend google.protobuf.FieldOptions {
  optional float my_field_option = 50002;
}
extend google.protobuf.OneofOptions {
  optional int64 my_oneof_option = 50003;
}
extend google.protobuf.EnumOptions {
  optional bool my_enum_option = 50004;
}
extend google.protobuf.EnumValueOptions {
  optional uint32 my_enum_value_option = 50005;
}
extend google.protobuf.ServiceOptions {
  optional MyEnum my_service_option = 50006;
}
extend google.protobuf.MethodOptions {
  optional MyMessage my_method_option = 50007;
}

option (my_file_option) = "Hello world!";

message MyMessage {
  option (my_message_option) = 1234;

  optional int32 foo = 1 [(my_field_option) = 4.5];
  optional string bar = 2;
  oneof qux {
    option (my_oneof_option) = 42;

    string quux = 3;
  }
}

enum MyEnum {
  option (my_enum_option) = true;

  FOO = 1 [(my_enum_value_option) = 321];
  BAR = 2;
}

message RequestType {}
message ResponseType {}

service MyService {
  option (my_service_option) = FOO;

  rpc MyMethod(RequestType) returns(ResponseType) {
    // Note:  my_method_option has type MyMessage.  We can set each field
    //   within it using a separate "option" line.
    option (my_method_option).foo = 567;
    option (my_method_option).bar = "Some string";
  }
}
```

> **NOTE:**
>
> ​		在定义该选项的包外使用该选项，必须在选项名前面加上包名的前缀，和类型名一样。
>
> ```protobuf
> // foo.proto
> import "google/protobuf/descriptor.proto";
> package foo;
> extend google.protobuf.MessageOptions {
>   optional string my_option = 51234;
> }
> 
> // bar.proto
> import "foo.proto";
> package bar;
> message MyMessage {
>   option (foo.my_option) = "Hello world!";
> }
> ```

​		自定义选项是扩展，它们必须像任何其他字段或扩展一样被分配字段号。

> ​		在上面的例子中，我们使用了50000-99999范围内的字段号。
>
> ​		 此范围预留给各个组织内部使用，因此您可以为内部应用程序自由使用此范围内的号码。 但是，如果您打算在公共应用程序中使用自定义选项，那么一定要确保字段号是全局惟一的。
>
> ​		 获取全局唯一的字段号，发送一个请求，向``protobuf``全局扩展注册表添加一个条目。 
>
> ​		通常你只需要一个分机号码。 你可以通过在子消息中声明只有一个分机号的多个选项:  
>
> ```protobuf
> message FooOptions {
>   optional int32 opt1 = 1;
>   optional string opt2 = 2;
> }
> 
> extend google.protobuf.FieldOptions {
>   optional FooOptions foo_options = 1234;
> }
> 
> // usage:
> message Bar {
>   optional int32 a = 1 [(foo_options).opt1 = 123, (foo_options).opt2 = "baz"];
>   // alternative aggregate syntax (uses TextFormat):
>   optional int32 b = 2 [(foo_options) = { opt1: 123 opt2: "baz" }];
> }
> ```

> 另外，每个选项类型(文件级、消息级、字段级等)都有自己的数字空间，因此，如上可以用相同的数字声明``FieldOptions``和``MessageOptions``的扩展。  





### (proto3) JSon映射

​		``proto3``支持``JSON``的规范编码，以便于在系统之间更容易的共享数据。

> ``json``编码的数据字段丢失或者本身就是``null``，会解析成``protobuf``的字段默认值。
>
> `protobuf`中的默认值，会在转化成``json``编码的时候被省略掉，以节省空间。
>
> 具体实现可以提供在``json``编码中的可选默认值：

| proto3                    | json            | json example                             | Notes                                                        |
| ------------------------- | --------------- | ---------------------------------------- | ------------------------------------------------------------ |
| message                   | object          | {“fBar”:v, “g”:null,…}                   | 产生json对象，消息字段名可以被映射成lowerCamelCase形式，并且成为JSON对象键，null被接受并成为对应字段的默认值。 |
| enum                      | string          | “FOO_BSR”                                | 枚举值的名字在proto文件中被指定                              |
| map                       | object          | {“k”:v, …}                               | 所有的键都被转换成string                                     |
| repeated V                | array           | [v, …]                                   | null被视为空列表                                             |
| bool                      | true，false     | true，false                              |                                                              |
| string                    | string          | “hello world”                            |                                                              |
| bytes                     | base64   string | “YWJjMTIzIT8kKiYoKSctPUB+”               | JSON值将是使用标准base64编码并填充的字符串编码的数据。 接受标准或url安全的base64编码(带/不带padding)。 |
| int32, fixed32, uint32    | number          | 1, -10, 9                                | json值会使一个十进制数，数值型或者string类型都会接受         |
| int64， fixed64，  uint64 | string          | “1”，“-10”                               | json值回事一个十进制数，数值型或者string类型都会接受         |
| float， double            | number          | `1.1, -10.0, 0, "NaN", "Infinity"`       | JSON值会是一个数字或者一个指定的字符串如”NaN”,”infinity”或者”-Infinity”，数值型或者字符串都是可接受的，指数符号也可以接受 |
| Any                       | object          | {"@type": "url", "f": v, … }             | 如果一个Any保留一个特上述的JSON映射，则它会转换成一个如下形式：`{"@type": xxx, "value": yyy}`否则，该值会被转换成一个JSON对象，`@type`字段会被插入所指定的确定的值 |
| Timestamp                 | string          | "1972-01-01T10:00:20.021Z"               | 使用RFC 339，其中生成的输出将始终是归一化的，并且使用0，3，6或者9位小数 |
| Duration                  | string          | `"1.000340012s", "1s"`                   | 生成的输出总是0，3，6或者9位小数，具体依赖于所需要的精度，接受所有可以转换为纳秒级的精度 |
| Struct                    | object          | { … }                                    | 任意的JSON对象，见struct.proto                               |
| Wrapper types             | various types   | `2, "2", "foo", true, "true", null, 0, … | 包装器在JSON中的表示方式类似于基本类型，但是允许nulll，并且在转换的过程中保留null |
| FieldMask                 | string          | `"f.fooBar,h"`                           | 见fieldmask.proto                                            |
| ListValue                 | array           | `[foo, bar, …]`                          |                                                              |
| Value                     | value           |                                          | 任意JSON值                                                   |
| NullValue                 | null            |                                          | JSON null                                                    |
| Empty                     | object          | {}                                       | An empty JSON object                                         |

#### json 选项

一个``proto3 json``实现也许会提供如下的选项：

- 具有默认值的字段：

  具有默认值的字段在``proto3 JSON``输出中默认被省略。

  实现可以提供一个选项，用默认值覆盖此行为和输出字段。

- 忽略未知字段：

  默认情况下，``Proto3 JSON``解析器应该拒绝未知字段，

  但是可以在解析中提供忽略未知字段的选项。

- 使用proto字段名替换小驼峰命名：

  默认情况下，``proto3 JSON printer`` 应该将字段名称转换为小驼峰并使用它作为``JSON``名称。

  实现可以提供使用``proto``字段名作为``JSON``名称的选项。

  ``Proto3 JSON``解析器需要同时接受转换后的小驼峰名称和``proto``字段名。

- 以整数而不是字符串的形式发出枚举值：

  在``JSON``输出中默认使用枚举值的名称。

  可以提供一个选项来代替使用枚举值的数值。









##  proto 文件生成代码

> When you run the [protocol buffer compiler](https://developers.google.cn/protocol-buffers/docs/proto3#generating) on a `.proto`, the compiler generates the code in your chosen language you'll need to work with the message types you've described in the file, including getting and setting field values, serializing your messages to an output stream, and parsing your messages from an input stream.
>
> 当您运行该.proto编译器,，编译器使用选择的语言生成代码， 你将使用消息描述文件所定义的消息类型来进行工作，包括获取和设置字段值，向一个输出流序列化消息， 从一个输入流解析消息。

- c++：每个 proto 文件生成一个.h 文件和一个 .cc文件， 每个message对应一个类
- java：每个proto文件生成一个.java文件，以及一个特殊的Builder类（该类用来创建消息类接口）
- python：每个proto文件中的每个消息类型生成一个含有静态描述符的模块，该模块与一个元类（metaclass）在运行时（runtime）被用来创建所需的python数据访问类。
- GO：编译器会为每个消息生成一个.pd.go文件
- ruby：每个proto生成了一个.rb 文件
- JavaNano：与没有Builder的java生成相同
- Object-c：每个proto文件生成 pbobjjc.h和pbobjcm文件，每个消息有一个对应的类
- C#：编译器会为每个proto生成了一个.cs文件，每个消息对应一个类
- Dart：每个消息生成一个.pb.dart文件，带一个对应的类



​		可以通过定义好的``.proto``文件来生成Java,Python,C++, Ruby, JavaNano, Objective-C,或者C# 代码，需要基于.proto文件运行protocol buffer编译器**protoc**。

​		对于Go,你还需要安装一个特殊的代码生成器插件。你可以通过GitHub上的[protobuf库](https://github.com/golang/protobuf/)找到安装过程。

```shell
protoc 
	--proto_path=IMPORT_PATH 
	--cpp_out=DST_DIR 
	--java_out=DST_DIR 
	--python_out=DST_DIR 
	--go_out=DST_DIR 
	--ruby_out=DST_DIR 
	--objc_out=DST_DIR 
	--csharp_out=DST_DIR 
  path/to/file.proto
```

- `IMPORT_PATH`：

  声明了一个.proto文件所在的解析import具体目录。如果忽略该值，则使用当前目录。

  如果有多个目录则可以多次调用`--proto_path`，它们将会顺序的被访问并执行导入。`-I=IMPORT_PATH`是`--proto_path`的简化形式。

- 当然也可以提供一个或多个输出路径：

  - `--cpp_out` 在目标目录DST_DIR中产生C++代码，可以在[C++代码生成参考](https://developers.google.cn/protocol-buffers/docs/reference/cpp-generated)中查看更多。

  - `--java_out` 在目标目录DST_DIR中产生Java代码，可以在 [Java代码生成参考](https://developers.google.cn/protocol-buffers/docs/reference/java-generated)中查看更多。

  - `--python_out` 在目标目录 DST_DIR 中产生Python代码，可以在[Python代码生成参考](https://developers.google.cn/protocol-buffers/docs/reference/python-generated)中查看更多。

  - `--go_out` 在目标目录 DST_DIR 中产生Go代码，可以在[GO代码生成参考](https://developers.google.cn/protocol-buffers/docs/reference/go-generated)中查看更多。

    。。。

  作为一个方便的拓展，如果DST_DIR以.zip或者.jar结尾，编译器会将输出写到一个ZIP格式文件或者符合JAR标准的.jar文件中。注意如果输出已经存在则会被覆盖，编译器还没有智能到可以追加文件。

您必须提供一个或多个.proto文件作为输入。可以同时指定多个.proto文件。尽管文件的命名是相对于当前目录的，但是每个文件必须驻留在一个IMPORT_PATHs中，这样编译器才能确定它的规范名称。



## proto文件代码风格

​		如果之前有了约定或风格，与之前保持一致。当新的项目使用时，尽量使用如下的风格。

### 文件格式

- 每行80个字符
- 两个空格进行缩进
- 字符串使用双引号

### 文件结构

​		文件名使用`lower_snake_case.proto`格式，文件内各部分应遵循如下规则：

1. 许可声明
2. 文件综述
3. 语法版本声明（`syntax=`）
4. 包
5. 导入（排序）
6. 文件选项
7. 其他

### 包

​		包名使用小驼峰，并且应该基于项目名且唯一。并且应该考虑基于包含pb类型定义的文件路径。

### 消息和字段名

​		消息使用驼峰命名法。

​		字段使用下划线分隔的蛇形命名法，包括`oneof`字段和扩展字段名。

```protobuf
message SongServerRequest {
	optional string song_name = 1;
}
```

​		由此生成的代码风格将如下：

```c++
const string& song_name() { ... }
void set_song_name(const string& x) { ... }
```

​		如果字段名中包含数字，数字会被下划线单独分隔：`song_name1` -> `song_name_1`



### repeated 字段

​		repeated字段名使用复数：

```protobuf
repeated string keys = 1;
repeated MyMessage accounts = 17;
```



### 枚举

​		枚举使用驼峰命名法，枚举值使用全大写名。

```protobuf
enum FooBar {
  FOO_BAR_UNSPECIFIED = 0;
  FOO_BAR_FIRST_VALUE = 1;
  FOO_BAR_SECOND_VALUE = 2;
}
```

​		每个枚举值以封号结束。给枚举值加前缀，而不是在一个封闭的消息来作为枚举的容器来限定。

​		零值的枚举应该被定义为未指明：`UNSPECIFIED`

### 服务

​		服务名和RPC方法名都使用驼峰命名法：

```protobuf
service FooService {
  rpc GetSomething(GetSomethingRequest) returns (GetSomethingResponse);
  rpc ListSomething(ListSomethingRequest) returns (ListSomethingResponse);
}
```



## protobuf 技术要点

### 多消息流

​		将多个消息写入单个文件或I/O流，则由使用方来维护每个消息的结束位置和下一个消息的开始位置。

> ​		pb的编码格式不是自定界的，所以pb解析器不能自动的确定消息的边界，解决这个问题的最简单方式是在写入消息本身之前，用定长的大小和pb消息本身组成一个PDU。
>
> ​		然后通过先读取定长的大小，解析该定长的数据。如果在此过程中，想避免拷贝到单独的缓冲区，查验`CodeInputStream`类，可以设置将读取限制在一定数量的字节。

### 大数据集合

​		PB不是为了除了大消息而设计的，如果超过1M字节的消息，可能需要考虑其他的方式。

​		PB非常适合处理大数据集合中的单个消息，通常大数据集合实际上只是小块的集合，其中每个小块可能是结构化的数据块。尽管PB不能一次处理整个集合，但是使用PB对每个部分进行编码，可以极大的简化问题：处理一组字节字符串，而不是一组结构化的数据。

​		PB不包含对大数据集合的支持，因为不同的情况需要不同的解决方案。有时，一个简单的记录列表就可以，但有时可能需要一个更像数据库的东西。每个解决方案都应该作为一个单独的库来开发。

​		

### 自描述消息

​		PB不含对自身类型的描述。如果只给出一个原始消息，没有相应的`.proto`文件定义其类型，很难提取到任何有用的数据。

​		但是`.proto`文件的内容本身可以使用PB来表示，`src/google/protobuf/descriptor.proto`定义了涉及消息类型。`protoc`可以使用`--descriptor_set_out`选项来输出`FileDescriptorSet`，它表示一组``.proto``文件。

​		定义一个自描述的消息：

```protobuf
syntax = "proto3";

import "google/protobuf/any.proto";
import "google/protobuf/descriptor.proto";

message SelfDescribingMessage {
  // Set of FileDescriptorProtos which describe the type and its dependencies.
  google.protobuf.FileDescriptorSet descriptor_set = 1;

  // The message and its type, encoded as an Any message.
  google.protobuf.Any message = 2;
}
```

​		使用`DynamicMessage`类，可以编写能够操作`SelfDescribingMessage`的工具。

​		PB中没有包含这个功能的原因是，在谷歌内部从来没有这样使用过。

​		这种技术需要支持使用`descriptor`的动态消息，在使用自描述消息之前，需要检查使用的系统是否支持描述符。

​		



## 编码原理

​		在应用程序中使用协议缓冲区不需要了解这一点，但了解不同的协议缓冲区格式如何影响编码消息的大小可能非常有用。

### Base-128 变长编码

要理解protobuf编码，首先需要理解变长编码。所谓变长编码是相对于定长编码而言的。

定长编码，使用固定字节数来表示，如 int32就使用4 bytes来表示。

变长编码，使用一个或多个字节序列化整数的方法，越小的数字占用越小的内存。如 int32使用1bytes即可。

>  base-128变长编码规则：
>
>  1. 除最后一个字节外，每个字节的最高位设置为1(msb, most significant)，表示还有其他的字节。其余的7个位用于存储该数字的两个补码表示
>  2. 最低的字节在前（小端字节序）
>
>  base-128 表示
>
>  ```protobuf
>  // 1
>  (base-128) 000 0001
>  
>  // 300
>  (base-128) 1010 1100 0000 0010
>  > 010 1100  000 0010 //去掉每个字节的 msb
>  > 000 0010 010 1100 // 小段转大端
>  > 0000 0001 0010 1100 // 补齐
>  >  1*(2^4*2^4) + 2*2^4 + (8+4)
>  > 256+32+12 = 300
>  ```

### 消息结构

​		protobuf是一系列key-value对，二进制版本的消息，只使用字段编号作为key。每个字段的名称和声明的类型只能在解码结束时，通过引用消息类型的定义 proto文件来确定。

​		当消息被编码时，key&value 被连接到一个字节流中。当消息被解码时，解析器需要跳过它不能识别的字段，这样可以将新字段添加到消息中，而不会破坏不知道它们的旧程序。为此，一个序列化后的消息中每个键值对中的键实际上是两个值 ：字段的标识编号，加上提供足以在其后的值序列中查找确定值长度的字段编码类型。

​		也就是说对于一个序列化后的消息：

- 包含一系列k/v对，每个字段的key是一个变长32位整数
- key的构成是，字段标识编号 和 编码类型，其中编码类型用于在后续的序列化值中查找正确的value序列。
- 字段编号的最后三位表示值的存储编码类型（消息中的每个键，都是带值的变长编码）
- 序列化后的流中，第一个数字总是变长编码的键

```(field_number << 3) | wire_type```

编码类型：

| Type | binary | Meaning          | Used For                                                 |
| ---- | ------ | ---------------- | -------------------------------------------------------- |
| 0    | 000    | Varint           | int32，int64，uint32，uint64，sint32，sint64，bool，enum |
| 1    | 001    | 64-bit           | fixed64, sfixed64, double                                |
| 2    | 010    | Length-delimited | string, bytes, embedded messages, packed repeated fields |
| 3    | 011    | Start group      | groups(deprecated)                                       |
| 4    | 100    | End group        | groups(deprecated)                                       |
| 5    | 101    | 32-bit           | fixed32, sfixed32, float                                 |

> ```(field_number << 3) | wire_type```
>
> -field_number:  字段序号
>
> -wire_type:  字段编码类型

### sample

#### **1** int32

encoding type : 000

如下消息：

```protobuf
message Test1{
	optional int32 a = 1;
}
```

如果a = 150，序列化后会有三个字节

```protobuf
(base-128) 08 96 01

// key 08, varint
08 : 1 << 3 | 000
> 0000 0001 << 3 | 0000 0000
> 0000 1000 | 0000 0000
> 0000 1000
> 08(hex)

// value 150, varint
150 : 128 + 16 + 4 + 2
> 1001 0110 // to binary
> 1, 001 0110 // Cut in seven bit
> 0000 0001, 0001 0110 // 补齐
> 0001 0110, 0000 0001 // to little endian
> 1001 0110, 0000 0001 // add msb
> 96(hex), 01(hex)
```





#### **2** Signed Interger

所有与编码类型’0’相关的protobuf类型都被编码为varint（变长编码）。但是，当编码负数时，有符号的int类型 和 标准的int类型之间有一个重要的区别。

如果你使用带有负数的类型，那么varint总是10字节长。它实际上被看做一个非常大的整型数。如果使用其中一种有符号类型，则生成的varint使用ZigZag编码，这种编码效率高得多 ：```sint32、sint64、int32、int64、int32、int64```.

ZigZag编码将有符号整型映射到无符号整型，因此绝对值较小的数字（例如 -1）也具有较小的varint编码值。它以“之”字形的方式在整数和负整数之间来回穿梭，因此 -1 被编码为1， 1被编码为2， -2 被编码为3，一次类推。

ZigZag编码：

> 换句话说，每个值都使用n进行编码
>
> for s, or`sint32` ： ```(n << 1) ^ (n >> 31)```
>
> for the 64-bit version：```(n << 1) ^ (n >> 63)```
>
> 请注意，第二个移位是算术移位。因此，换句话说，移位的结果要么是一个全为零位(如果为正)的数字，要么是一个全为一位(如果为负)的数字。(n> bbb31) n n
>
> 当or被解析时，它的值被解码回原始的已签名版本 sint32``sint64

| signed Original | Encoded As |
| --------------- | ---------- |
| 0               | 0          |
| -1              | 1          |
| 1               | 2          |
| -2              | 3          |
| 2               | 4          |
| …               | …          |
| 2147483647      | 4294967294 |
| -2147483648     | 4294967295 |



#### **3** Non-varint Number

非varint数值类型很简单，具有编码类型1， 它告知解析器需要固定的64位数据块。类似的，也具有编码类型5，告知解析器需要32位。

值都以小端字节序进行存储。

#### **4** string

encoding type : 010 

具有编码类型2（length-delimited），意味着该值是varint编码的长度，后面跟着指定字节数的数据。

```protobuf
message Test2 {
	optional string b = 2;
}
```

b = “testing”

```protobuf
(base-128) 12 07 74 65 73 74 69 6e 67
(key) 12 (length) 07 (UTF8 "testing")74 65 73 69 6e 67

// key 12, varint
12 : 2 << 3 | 010
> 0000 0010 << 3 | 0000 0010
> 0001 0000 | 0000 0010
> 0001 0010
> 12(hex)

// length 07
"testing" -> 7(dec) -> 07(hex)

// value "testing", varint
"testing" : (ASCII hex) 74 65 73 74 69 6e 67
```



#### **5** embedded messages 嵌套消息

encoding type : 010

```protobuf
message Test3 {
	optional Test1 c = 3;
}
```

这里是经过编码的版本，同样Test1的字段 c.a = 150

```protobuf
(base-128)  1a 03 08 96 01

// key 1a, varint
1a : 3 << 3 | 010
> 0000 0011 << 3 | 0000 0010
> 0001 1000 | 0000 0010
> 0001 1010
> 1a(hex)

// length 03
Test 1 : 08 96 01 -> 03(hex)

// value Test1::a = 150, varint
150 : (hex) 08 96 01
```



#### **6** 可选和重复元素

如果一个proto2消息定义有元素(没有选项)，则编码的消息有零个或多个具有相同字段号的键-值对。这些重复的值不必连续出现;它们可能与其他字段交叉。解析时保留了元素相对于其他字段的顺序，但丢失了元素相对于其他字段的顺序。

在proto3中，重复字段使用打包编码，你可以在下面读到。```repeated [packed=true]```

对于proto3中的任何非重复字段或proto2中的字段，编码的消息可能有也可能没有与该字段编号对应的键-值对。```optional```

通常，经过编码的消息不会有一个以上的非重复字段实例。但是，解析器应该处理它们所处理的情况。对于数值类型和字符串，如果同一字段出现多次，则解析器接受最后看到的值。

对于嵌入的消息字段，解析器合并同一字段的多个实例，就像使用如下方法：

	即后一个实例中的所有单一标量字段替换前一个实例中的所有单一标量字段，合并单一嵌入的消息，并连接重复的字段。

这些规则的效果是，解析两个编码消息的连接产生的结果与分别解析这两个消息并合并产生的对象产生的结果完全相同。```Message::MergeFrom```

```c++
MyMessage message;
message.ParseFromString(str1 + str2);

MyMessage message, message2;
message.ParseFromString(str1);
message2.ParseFromString(str2);
message.MergeFrom(message2);
```

此属性有时很有用，因为它允许您合并两条消息，即使您不知道它们的类型。

#### **7** 打包重复字段

v2.1.0 引入了打包的重复字段，它在proto2中被声明为重复字段，但是有特殊的应用，但是有特殊的选项开关。

在proto3中，默认情况下会打包标量数值类型的重复字段。这些功能类似于重复字段，但编码方式不同。包含零元素的打包重复字段不会出现在编码的消息中，否则，字段的所有元素都被打包到一个k/v中，编码类型为2（length-delimited），每个元素都找正常方式进行编码，不过前面没有键。

```protobuf
message Test4 {
	repeated int32 d = 4 [packed=true];
}
```

现在假设您构造a，为重复字段提供值

		d.a = 3
	
		d.a = 270
	
		d.a = 86942

然后，编码的形式将是:```Test4 d```

```protobuf 
(base-128) 22 06 03 8E 02 9E A7 05

// key : 22


// key 22, varint
22 : 4 << 3 | 010
> 0000 0100 << 3 | 0000 0010
> 0010 0000 | 0000 0010
> 0010 0010
> 22(hex)

// payload size (6 bytes)
06 : 03 8E 02 9E A7 05 -> 06(hex)

// value Test1::a = 3, varint
3 : (hex) 03
270 : 256 + 8 + 4 + 2
> 1 0000 1110 // to binary
> 10, 000 1110 // Cut in seven bit
> 0000 0010, 0000 1110 // 补齐
> 0000 1110, 0000 0010 // to little endian
> 1000 1110, 0000 0010 // add msb
> 8E(hex), 02(hex)
86942 :  
> 1 0101 0011 1001 1110 // to binary
> 1 01, 01 0011 1, 001 1110 // Cut in seven bit
> 0000 0101, 0010 0111, 0001 1110 // 补齐
> 0001 1110, 0010 0111, 0000 0101 // to little endian
> 1001 1110, 1010 0111, 0000 0101 // add msb
> 9E(hex), A7(hex), 05(hex)
```

只有原始数值类型（使用varint编码的，32bit或者64bit编码类型）的重复字段可以声明为“packed”.

**注意：** 尽管通常没有理由为一个打包的重复字段编码多个键值对，但编码器必须准备好接受多个键值对。在这种情况下，有效载荷应该连接在一起，每一对必须包含全部数量的元素。

protobuf解析器必须能够解析被编译的重复字段，就像它们没有被打包一样，反之亦然。这允许以向前和向后兼容的方式添加现有字段。```[pack=true]```

### 字段顺序

字段号可以在文件中以任意顺序使用。选择的顺序对消息的序列化方式没有影响。

序列化消息时，对于如何写入其已知或未知字段，没有保证顺序。序列化顺序是实现细节，任何特定实现的细节将来都可能更改。因此，protobuf必须能够以任何顺序解析字段。

影响：

- 不要假设序列化消息的字节输出是稳定的。对于具有传递字节字段(表示其他序列化协议缓冲区消息)的消息尤其如此。
- 默认情况下，对同一协议缓冲区消息实例重复调用序列化方法可能不会返回相同的字节输出;也就是说，默认的序列化是不确定的。
  - 确定性序列化只保证对特定二进制文件输出相同的字节。字节输出可能会在二进制文件的不同版本之间发生变化。
- 对于协议缓冲区消息实例，下列检查可能失败。```foo```
  - `foo.SerializeAsString() == foo.SerializeAsString()`
  - `Hash(foo.SerializeAsString()) == Hash(foo.SerializeAsString())`
  - `CRC(foo.SerializeAsString()) == CRC(foo.SerializeAsString())`
  - `FingerPrint(foo.SerializeAsString()) == FingerPrint(foo.SerializeAsString())`
- 下面是几个示例场景，其中逻辑上等价的protobuf消息，并可能序列化为不同的字节输出。
  - `bar`由旧服务器序列化，该服务器将某些字段视为未知。
  - `bar`由服务器序列化，该服务器使用不同的编程语言实现，并以不同的顺序序列化字段。
  - `ba`有一个以非确定性方式序列化的字段。
  - `bar`有一个字段，用于存储序列化方式不同的protobuf消息的序列化字节输出。
  - `bar`由一个新服务器序列化，该服务器由于实现更改而以不同的顺序序列化字段。
  - 都是连接的单个消息，但有不同的顺序



## 入门教程

本节中的每个教程都向您展示如何用您最喜欢的语言使用protobuf实现一个简单的应用程序，介绍该语言的协议缓冲区API，并向您展示创建和使用.proto文件的基础知识。还提供了每个应用程序的完整示例代码。

本教程并不假设您对protobuf有任何了解，但是假定您能够熟练地使用您所选择的语言编写代码，包括使用文件I/O。

### 编译安装

https://github.com/protocolbuffers/protobuf/blob/master/src/README.md    官方安装文档

> **参考：**
>
> https://blog.csdn.net/fangjin_kl/article/details/60778039
>
> http://www.cppblog.com/colorful/archive/2012/05/05/173761.html（【亲测好用】，下载Git上的all包）
>
> https://blog.csdn.net/u014360239/article/details/56915237（主要是由一些对Linux运行时库的原理解释）
>
> https://blog.csdn.net/qq_15267341/article/details/80107293（windows上超详细的安装教程）



### c++

本教程提供了程序员C++使用协议缓冲区的基本知识。通过快速创建一个简单的示例应用程序，它告诉你如何：

- 在文件中定义消息格式。`.proto`
- 使用protobuf编译器。
- 使用C++ protobuf API 编写和读取消息。

这并不是在c++中使用protobuf的全面指南。详细的参考：https://developers.google.cn/protocol-buffers/docs/proto， https://developers.google.cn/protocol-buffers/docs/reference/cpp， https://developers.google.cn/protocol-buffers/docs/reference/cpp-generated， https://developers.google.cn/protocol-buffers/docs/encoding 【protobuf 语言指南】【c++ API参考】【c++生成的代码指南】【编码说明】

#### 为什么使用protobuf

使用一个通讯簿程序来介绍使用。

可以从文件中读取和写入联系人详细信息。通讯簿中每个人都有姓名、身份证、电子邮件地址和联系人电话号码。

如何序列化和检索这样的结构化数据：

- 原始内存中的数据节后可以以二进制形式发送/保存。

  随着时间的推移，因为接收/读取代码必须与完全相同的内存布局、字节序等一起编译。此外，当文件以原始格式基类数据，并且为该格式连接的软件副本四处传播时，很难扩展该格式。

- 可以发明一种临时方法，将数据项编码为单个字符串，例如将4个int编码为“12:3：-23:67”。这是一种简单而灵活的方法，尽管它确实需要编写一次性代码和解析代码，而且分析会带来很小的运行时成本，这最适合对非常简单的数据进行编码。

- 将数据序列化为XML。

  这种方法的优势在于XML是具有人类可读性，并且有很多语言有绑定库。如果要与其他应用程序/项目共享数据，这可能是个不错的选择。但是，XML以空间密集型著称，编码/解码可能会对应用程序带来巨大的性能损失。此外，导航XML DOM树比在类中导航简单字段要复杂的多。

protobuf 是灵活、高效、自动化的解决方案，可以完全解决这个问题。使用protobuf，可以编写要存储的数据结构的说明。依据与此，protobuf编译器创建一个类，该类使用高效的二进制格式实现protobuf的自动编码和解码。生成的类 为包含protobuf的字段提供getter & setter，并负责作为单元读取和写入协议缓冲区的详细信息。重要的是，protobuf格式支持随着时间的推移扩展格式的想法，这样代码仍然可以读取使用旧格式编码的数据。```.proto```

#### 定义协议格式

为要序列化的每个数据结构添加一条消息，探后为消息中的每个字段指定名称和类型。

语法类似于c++或java。

```protobuf
// addressbool.proto

sysntax = "proto2";

package tutorial; // 包名，有助于防止不同项目之间的命名冲突，c++产生命名空间。

message Person {
	// 许多标准的简单数据类型都可以作为字段类型使用，包括bool、int32、float、double和string
	required string name = 1;
	required int32	id = 2;
	optional string email = 3;
	
	// 如果您希望某个字段具有预定义的值列表之一，还可以定义枚举类型——这里您希望指定电话号码可以是MOBILE、HOME或WORK的号码之一。
	enum PhoneType {
		MOBIE = 0;
		HOME = 1;
		WORK = 2;
	}
	
	// 您还可以通过使用其他消息类型作为字段类型来为您的消息添加进一步的结构
	// 您甚至可以定义嵌套在其他消息中的消息类型—如您所见，PhoneNumber类型是在Person内部定义的
	message PhoneNumber {
		required string number = 1;
		optional PhoneType type = 2 [default = HOME];
	}
	
	repeated PhoneNumber phones = 4;
}

message AddressBook {
	repeated Person people = 1;
	// 每个元素上的“= 1”、“= 2”标记标识字段在二进制编码中使用的唯一“标签”。
	// 标记号1-15编码时比较高的数字少一个字节，因此作为优化，您可以决定对常用的或重复的元素使用这些标记，而对不常用的可选元素保留标记16或更高。
	// 重复字段中的每个元素都需要重新编码标签号，因此重复字段特别适合这种优化。
}
```

- ```required```：

  必须为该字段提供一个值，否则将认为该消息“未初始化”。

  **如果在调试模式下编译libprotobuf，序列化未初始化的消息将导致断言失败**。在优化的构建中，将跳过检查，无论如何都将写入消息。但是，解析未初始化的消息总是会失败(解析方法返回false)。除此之外，必需字段的行为与可选字段完全相同。

- ```optional```：

  字段可能设置，可能未设置。如果未设置可选字段，则使用默认值。

  对于简单类型，可以指定自己的默认值。否则，将使用系统默认值：数字类型为零，字符串为空字符串，bool为false。嵌套的消息，默认值始终是消息的”默认实例“或者”protobuf type“，实例没有设置任何字段。调用访问器来获取未显式设置的可选(或必需)字段的值总是返回该字段的默认值。

- ```repeated```:

  该字段可以重复任何次（包括0）。重复值的顺序将保留在协议缓冲区中。将重复字段视为动态大小的数字组。

#### 编译proto文件

运行编译器，指定源目录（应用程序源代码所在的位置，如果不提供值，则使用当前目录），目标目录（生成代码所在的位置，通常与$SRC_DIR相同）和 .proto 的路径。

```shell
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
# c++ for --cpp_out
```

这将在指定的目标目录中生成以下文件:

addressbook.pb。h，声明生成类的头。

addressbook.pb。cc，它包含类的实现。

#### protobuf tutorial ： basic usage

##### 消息类实体

对于上述proto文件，每个消息生成一个类。查看person类，可以看到如下字段的接口。

```c++
// name
inline bool has_name() const ;
inline void clear_name() ;
inline const ::std::string& name() const;
inline void set_name(const ::std::string& value);
inline void set_name(const char* value);
inline ::std::string* mutable_name();

// id
inline bool has_id() const;
inline void clear_id();
inline int32_t id() const;
inline void set_id(int32_t value);

// email
inline bool has_email() const;
inline void clear_email();
inline const ::std::string& email() const;
inline void set_email(const ::std::string& value);
inline void set_email(const char* value);
inline ::std::string* mutable_email();

// phones
inline int phones_size() const;
inline void clear_phones();
inline const ::google::protobuf::RepeatedPtrField<::tutorial::Person_PhoneNumber >& phones() const;
inline ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >* mutable_phones();
inline const ::tutorial::Person_PhoneNumber& phones(int index) const;
inline ::tutorial::Person_PhoneNumber* mutable_phones(int index);
inline ::tutorial::Person_PhoneNumber* add_phones();
```

getter方法的名称是小写的字段名，setter方法以set_开头。_

_对于每个单独的(required or optional)字段，也有has_方法，如果该字段已经被设置，它会返回true。

每个字段都有一个clear_方法，它会将字段取消设置为空状态。

数字id字段只有上面描述的getter方法，而name和email字段有额外的方法，因为它们是字符串，一个mutable_ getter可以让你直接得到字符串的指针，还有一个额外的setter。

注意，您可以调用mutable_email()，即使电子邮件尚未设置，它将被自动初始化为一个空字符串。

如果你有一个自定义的消息字段在这个例子中,它还将有一个mutable_方法，但没有set_方法。（PhoneNumber phones）

repeated 字段还有一些特殊的方法：

- 检查重复字段的 ```_size```方法，换句话说，就是这个字段在消息中有几个。
- 通过索引获取一个特定的实例（phone number）
- 添加一个另外的实例到可编辑的消息中（repeated的标量类型有一个add_，它只允许传入新值）。

有关protobuf编译器为任何特定字段定义生成哪些成员的详细信息，请参阅c++生成的代码引用：https://developers.google.cn/protocol-buffers/docs/reference/cpp-generated。

##### 枚举和嵌套类

生成的代码包括对应于.proto枚举的PhoneType枚举，Person::PhoneType，其值为Person::MOBILE、Person::HOME和Person::WORK(实现细节稍微复杂一些，但您不需要理解它们才能使用枚举)。

编译器还为您生成了一个嵌套类，称为Person::PhoneNumber。如果您查看代码，您可以看到“真正的”类实际上被称为Person_PhoneNumber，但是Person内部定义的typedef允许您将其视为一个嵌套类。（typedef Person::PhoneNumber Person_PhoneNumbe）

唯一的区别是，如果你想在另一个文件中前向声明类，你不能在c++中前向声明嵌套类型，但是你可以前向声明Person_PhoneNumber。

##### 消息的标准方法

每个消息类还包含许多其他方法，让您检查或操作整个消息，包括:

- `bool IsInitialized() const;`:检查是否所有的required字段都已经设置。
- `string DebugString() const;`: 返回一个人类可读的消息表示形式，对于调试特别有用。
- `void CopyFrom(const Person& from);`用给定消息的值重写消息。
- `void Clear();` 将所有的元素清楚会空状态

这些方法和下面部分中描述的I/O方法实现了所有c++ protobuf 类共享的消息接口。有关更多信息，请参阅完整的Message API文档：https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message。

##### 序列化和反序列化

最后，每个protobuf类都有使用protobuf二进制格式写入和读取所选类型的消息的方法。包括：

- `bool SerializeToString(string* output) const;`序列化消息并将字节存储在给定字符串中。注意字节是二进制的，而不是文本;我们只使用string类作为方便的容器。
- `bool ParserFromString(const string& data)`: 从给定的string解析一个消息
- `bool SerializeToOstream(ostream* output) const;`:将一个消息写入给定的C++输出流。
- `bool ParseFromIstream(istream* input);`：从给定的C++输入流中解析一个消息

这些只是几个选项提供的解析和序列化。同样，请参阅消息API引用以获得完整列表：https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message

> **Protobuf 和 面向对象设计：**
>
> protobuf 类基本上是哑数据持有者（像C中的结构体），它们不能成为对象模型中良好的第一类公民。如果你想向生成的类添加更丰富的行为，最好的方法是将生成的协议缓冲区类包装在特定的应用程序自定义的类中。
>
> 包装protobuf是一个好主意，如果你没有控制proto的设计文件（如果重用来自其他项目的），这这种情况下，您可以使用包装器类来构建一个更适合应用程序的独特环境的接口：异常一些数据和方法，公开方便的函数，等等。您不应该通过继承生成的类来添加行为，这将破坏内部机制，并不是良好的面向对象实践。

##### 写入一个消息：序列化

创建并填充协议缓冲区类的实例，然后将它们写入输出流。

这是一个程序，它从一个文件中读取一个地址簿，根据用户的输入向它添加一个新的人，然后再将新的地址簿写回文件中。直接调用或引用由协议编译器生成的代码的部分被突出显示。

```c++
//g++ -o write write.cpp addressbook.pb.cc -lprotobuf
#include "addressbook.pb.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// This function fills in a Person message based on user input.
void PromptForAddress(tutorial::Person* person)
{
	cout << "Enter person ID number: ";
	int id;
	cin >> id;
	person->set_id(id);
	cin.ignore(256,'\n');

	cout << "Enter name: ";
	getline(cin, *person->mutable_name());

	cout << "Enter email address (blank for none): ";
	string email;
	getline(cin, email);
	if(! email.empty()) {
		person->set_email(email);
	}

	while(true) {
		cout << "Enter a phone number (or leave blank to finish): ";
		string number;
		getline(cin, number);
		if(number.empty()) {
			break;
		}
	
		tutorial::Person::PhoneNumber* phone_number = person->add_phones();
		phone_number->set_number(number);

		cout << "Is this a mobile, home, or work phone?";
		string type;
		getline(cin, type);
		if(type == "mobile") {
			phone_number->set_type(tutorial::Person::MOBILE);
		}else if (type == "home") {
			phone_number->set_type(tutorial::Person::HOME);
		}else if (type == "work") {
			phone_number->set_type(tutorial::Person::WORK);
		}else {
			cout << "Unknown phone type. Using default." << endl;
		}
	}
}




// Main function: Reads the entire address book from a file,
// 	adds one person based on user input, then writes it back out to the same
// 	file.
int main(int argc, char* argv[])
{
	//Verify that the version of the library that we linked against is 
	//compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if(argc != 2) {
		cerr << "Usage: " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
		return -1;
	}

	tutorial::AddressBook address_book;

	{
		// Read the existing address book.
		fstream input(argv[1], ios::in | ios::binary);
		if(!input) {
			cout << argv[1] << ": File not found. Creating a new file." << 	endl;
		}else if(! address_book.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
	}

	// Add an address.
	PromptForAddress(address_book.add_people());

	{
		// Write the new address book back to disk.
		fstream output(argv[1], ios::out | ios::trunc | ios::binary);
		if(!address_book.SerializeToOstream(&output)) {
			cerr << "Failed to write address book." << endl;
			return -1;
		}
	}

	//Optional: Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}
```

> 注意到```GOOGLE_PROTOBUF_VERIFY_VERSION```宏。
>
> 在使用c++ protobuf 库之前执行这个宏是一个很好的实践,虽然不是严格必要的。它验证您没有偶然链接到与您所编译的头的版本不兼容的库的版本。如果检测到版本不匹配，程序将中止。注意每个.pb.cc文件在启动时自动调用这个宏。

> 程序末尾对```ShutdownProtobufLibrary()```的调用。
>
> 所做的就是删除protobuf 库分配的所有全局对象。这对大多数程序来说是不必要的，因为进程无论如何都会退出，而操作系统会负责回收它的所有内存。但是，如果您使用内存泄漏检查器，要求释放所有最后一个对象，或者如果您正在编写一个库，该库可能会被单个进程加载和卸载多次，那么您可能需要强制 protobuf 清除所有内容。

##### 扩展protobuf

在已经发布了使用protobuf构建的项目后，迟早会有需求更改protobuf的协议定义。如果希望的是新的定义是向后兼容的，而旧的定义时向前兼容的，那必须遵循一些规则：

- 您不能更改任何现有字段编号
- 您不能添加或删除任何required字段
- 可以删除 optional 和 repeated 字段
- 可以添加新的 optional 或 repeated 字段，但必须使用新的 字段标号（从未在此protobuf中使用的字段编号，甚至未被已经删除的字段使用）

这些规则也有例外，但很少会那么用。如果遵循这些规则，旧代码将很容易解析新消息，并简单地忽略任何新字段。

对于旧代码，被删除的optional字段只有默认值，被删除的重复字段为空。新代码也将透明地读取旧消息。

但是，请记住，新的optional字段不会出现在旧消息中，因此您需要显式地检查它们是否用has_设置，或者在.proto文件中提供一个合理的默认值，在标记号后面加上[default = value]。如果没有为可选元素指定默认值，则使用特定类型的默认值:对于字符串，默认值是空字符串。对于布尔值，默认值为false。对于数值类型，默认值为零。

还要注意的是，如果您添加了一个新的repeated字段，您的新代码将无法分辨它是空的(新代码)还是根本没有设置(旧代码)，因为它没有has_标志。

##### 优化建议

c++ protobuf 库经过了极大的优化。然而，适当的使用可以进一步提高性能。这里有一些技巧，可以挤压出这个库的每一滴速度:

- 尽可能重用消息对象。

  消息试图保留它们分配的空间用于重用的内存，即使在它们被清除时也是如此。

  因此，如果您正在连续处理许多具有相同类型和类似结构的消息，那么每次重用相同的消息对象可以减轻内存分配器压力。但是，随着时间的推移，对象可能会变得臃肿，特别是当消息的“形状”发生变化或偶尔构造的消息比通常的要大得多时。您应该通过调用SpaceUsed方法来监视消息对象的大小，并在它们太大时删除它们。

- 对于从多个线程中分配大量小对象，系统的内存分配器可能没有得到很好的优化。尝试使用谷歌的tcmalloc代替。

##### 高级用法

协议缓冲区的用途不仅仅是简单的解析器和序列化。一定要研究c++ API reference，看看还可以用它们做什么。

**protocol message 类提供的一个关键特性是反射**。您可以遍历消息的字段并操作它们的值，而无需针对任何特定的消息类型编写代码。使用反射的一种非常有用的方法是将协议消息与其他编码(如XML或JSON)进行转换。反射的一种更高级的用法可能是查找相同类型的两个消息之间的差异，或者开发一种“协议消息的正则表达式”，在其中可以编写匹配特定消息内容的表达式。如果您发挥自己的想象力，就有可能将协议缓冲区应用于比您最初预期的更广泛的问题!

反射由```Message::Refletion interface```提供：https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message.Reflection



## API  Reference

https://developers.google.com/protocol-buffers/docs/reference/overview



本节包含了在c++、Java、Python、Go、c#、Objective C、JavaScript、Ruby、PHP和Dart中使用协议缓冲类的参考文档，以及protobuf本身的参考文档。每种语言的文档包括(如果有的话):

- protobuf编译器从.proto文件中生成的代码的参考指南。
- 为提供的源代码生成API文档。

### c++

描述了protobuf为任何给定的协议定义生成的C++代码，proto2 和 proto3生成的代码之间的任何差异都将突出显式。差异是封装在实现代码中的，而不是在两个版本中对于相同消息类和接口的表现。

#### 生成的C++代码

> c++ generated code guide : https://developers.google.cn/protocol-buffers/docs/reference/cpp-generated

##### proto编译器

```protoc --cpp_out=DIR {.proto,...}```

编译器为每个.proto文件输入创建一个头文件和.cpp的实现文件为输出。输出文件的名称是通过采用.proto文件名并进行两个更改来计算的：

- 扩展名（.proto)分别被.pb.h或.pb.cc替换为头文件和实现文件
- proto路径（由–proto_path=或-I命令行标志指定）被输出路径替换。

```protobuf
protoc --proto_path = src --cpp_out = build/gen src/foo.proto src/ bar/baz.proto

// build/gen/foo.pb.h
//build/gen/foo.pb.cc
//build/gen/bar/baz.pb.h
//build/gen/bar/baz.pb.cc
```

##### 包

如果proto文件包含package声明，则文件的全部内容将被放置在相应的C++ namespace中。

```protobuf
package foo.bar;

// namespace FOO::bar {
//     ...
//}
```

##### 消息

对于一个消息，protobuf 编译器将其生成为一个对应名字的从`google::protobuf::Message```继承的类

```protobuf
message Foo {}

// google::protobuf::Message::Foo
```

这个类是一个具体实现的类，没有任何未实现的纯虚方法遗留。在消息中非纯虚的虚方法可能被生成的消息中的类实现，具体是否实现取决于优化模式。默认情况下，生成的消息类的所有方法的实现都以最快速度为准。

除非在proto文件中指定了`option optimize_for = CODE_SIZE;`，实现则仅包括所有必须的方法的最小集合，并依赖其余基于反射的实现。这显著减小了所生成代码的大小，但同时也降低了性能。

如果指定了`option optimize_for = LITE_RUNTIME;`，将包括所有方法的快速实现。但是，它将实现不支持描述符或反射的	`google::protobuf::MessageLite`接口，只包含```Message```方法的子集。生成的代码只需要连接到```libprotobuf-lite.so```。精简库比完整库要小得多，并且适合于资源受限的系统，如移动设备。

你不应该去创建Foo的子类。如果实现了该类的子类，并重写了虚函数，这个重写的虚函数可能会被忽略，因为许多生成的方法的调用已经被 de-virtualized 来提升性能了。

```c++
// Message 接口定义了允许您检查，操作，读取或写入整个消息的方法，包括从二进制字符串进行解析和序列化


// 从给定的序列化二进制字符串（wire format）中解析消息。
bool ParseFromString(const string& data); 
// 将给定的消息序列化为二进制字符串
bool SerializeToString(string* output) const;


// 返回以text_format表示的 proto 消息字符串（仅用于调试）
string DebugStirng();
// 默认构造函数
Foo();
// 默认析构函数
~Foo();
// 拷贝构造函数
Foo(const Foo& other);
// 赋值运算符 
Foo& oprator=(const Foo& other);


// 返回解析此消息时遇到的未知字段集合(只读)
const UnknownFieldSet& unknown_fields() const;
// 返回指向解析消息时遇到的未知字段集合的指针(可修改)
UnknownFieldSet* mutable_unknown_fields();


// 返回类型的描述符。其中包含有关类型的信息，包括其具有的字段及其类型。可以与反射一起使用，以程序的方式检查字段。
static const Descriptor* descriptor();
// 返回其const单例对象，它与新构造的Foo实例相同（所有singular字段都未设置， 所有的repeated字段都为空）。如果调用消息的New()方法，可以将消息的默认实例用作工厂类。
```

##### 嵌套类型

一条消息可以在另一条消息中声明。

```protobuf
message Foo { message Bar{}}

// 编译器将生成两个类 Foo 和 Foo_Bar
// 编译器还将在内部的实现中生成一个 typedef Foo_Bar Bar;
```

这意味着可以像使用嵌套类一样使用嵌套类型的类```Foo::Bar```。但是C++不允许在前向声明中使用嵌套类型`Foo::Bar`，前向声明仍然需要使用`Foo_Bar`.

##### 字段

在消息类中，proto除了前述方法之外，还会为proto文件中消息中定义的每个字段生成一组 访问器方法。

与访问器方法一样，编译器会为包含其字段号的每个字段生成一个整数常量，整数常量名称包含字母`k`， 然后接上以驼峰命名的字段名，然后加上`FieldNumber`，如：

```protobuf
optional int32 foo_bar = 5;

// static const int kFooBarFieldNumber = 5;
```

对于返回const引用的访问器，当对消息接下来进行修改访问时，该引用可能会失效。这包括：

- 调用任何字段的任何非const访问器
- 调用从Message继承的任何非const方法或通过其他方法修改消息（如，对消息使用swap()方法）

相应地，只有在没有对消息进行修改访问时，才能保证返回引用的地址再访问器的不同调用间是相同的。

对于返回指针的字段访问器，当对该消息进行下一次修改或非修改访问时，该指针可能失效。这包括：

- 调用任何字段的任何访问器
- 调用从Message继承的任何方法或通过其他方式访问消息（如，通过拷贝构造函数赋值消息）。

相应地，永远不能保证在访问器的两次不通过调用中返回的指针的值是相同的。

###### singular 数值字段

**proto2**

```c++
// optional int32 foo = 1;
// required int32 foo = 1;
// 对上述任何一个字段定义，编译器将生成以下访问器方法

// 返回true来确认该字段是否被设置
bool has_foo() const ;

// 返回字段的方前置，如果未设置字段，则返回默认值.
int32 foo() const;

// 设置字段值。调用此函数后, has_foo() 将返回true并且foo()将返回 value。
void set_foo(int32 value);

// 清除字段的值，调用此函数后，has_foo() 将返回false并且 foo()返回默认值。
void clear_foo();

// 对于其他数字类型（包括bool），根据标量类型表将int32替换为对应的C++类型即可。
```

**proto3**

```c++
// int32 foo = 1;
// proto3 没有了required字段，所有的字段都是 optional的。
// 对于此字段，编译器将生成一下访问器方法：

// 返回字段的当前值。如果为这只该字段，则返回0
int32 foo() const;
// 设置字段的值。调用此方法后，foo()将返回value
void set_foo(int32 value);
// 清除字段值。调用此方法后，foo() 将返回0
void clear_foo();

// 对于其他数字类型（包括bool），根据标量类型表将int32替换为对应的C++类型即可。
```



###### repeated  数值字段

RepeatedField：https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.repeated_field

```c++
// repeated int32 foo = 1;

// 返回字段中当前的元素数。
int foo_size() const;

// 以给定的从0开始的索引返回元素，用索引[0, foo_size()) 之外的数值调用此方法将产生未定义行为。
int32 foo(int index) const;

// 在给定的从0开始的索引出设置元素的值
void set_foo(int index, int32 value);

// 使用给定值将新元素追加到字段的末尾。
void add_foo(int32 value);

// 从字段中删除所有元素，调用后，foo_size() 将返回0.
void clear_foo();

// 返回存贮字段元素的RepeatedField，这个容器类提供了类STL的迭代器和其他方法。
const RepeatedField<int32>& foo() const;

// 返回指向RepeatedField存贮类的可变对象的指针。此类提供了类似STL的迭代器和其他方法。
RepeatedField<int32>* mutable_foo();
```

对于其他数字字段类型（包括bool），依据标量类型表将int32替换为对应的C++类型即可。



###### singular 字符串字段

**proto2**

```c++
// optional string foo = 1;
// required string foo = 1;
// optional bytes foo = 1;
// required bytes foo = 1;
// 对上述任何一个字段定义，编译器将生成以下访问器方法

// 返回true来确认是否设置了字段
bool has_foo() const;

// 返回字段的当前值。如果未设置该字段，则返回默认值。
const string& foo() const;

// 设置字段的值，调用后， has_foo() 返回true， 并且 foo() 返回value。
void set_foo(const string& value);

// c++ 11 及更高版本， 设置字段的值，从传递的自渡船开始。调用 此方法后，has_foo() 将返回true 并且 foo() 返回的副本value。
void set_foo(string&& value);

// 使用C式样的以null终止的字符串设置字段的值。调用此方法后，has_foo() 将返回true并foo() 返回的副本value。
void set_foo(const char* value);

// 与上面类似，但是字符串大小是明确给出的，而不是通过查找空字符子类来确定的。
void set_foo(const char* value, int size);

// 返回指向string存储该字段值的可变对象的指针。如果在调用之前未设置该字段，则返回的字符串将为空（不是默认值）。调用此方法后，has_foo() 将返回true，foo() 并将返回写入给定字符串的任何值。
string* mutable_foo();

// 清除字段的值。调用此函数后，has_foo 将返回false并且 foo 返回默认值。
void clear_foo();

// 将string对象设置为字段值，并释放先前的字段值（如果存在）。如果string指针不是NULL，则该消息将获得所分配string对象的所有权。并且has_foo 返回true。该消息可随时删除以分配的string对象，因此对该对象的引用可能无效。否则，如果value为NULL，则行为与调用clear_foo相同。
void set_allocated_foo(string* value);

// 释放字段的所有权并返回string对象的指针，调用此方法后，调用者将会得分配的string对象的所有权，has_foo 将返回 false， foo 将返回默认值。
string* release_foo();
```

**proto3**

```c++
// string foo = 1;
// bytes foo = 1;
// proto3 没有了required字段，所有的字段都是 optional的。
// 对于此字段，编译器将生成一下访问器方法：

// 返回字段的当前值。如果未设置该字段，则返回空字符串/空字节
const string& foo() const;

// 设置字段的值。调用后，foo() 将返回value的副本。
void set_foo(const string& value);

// c++11及更高版本，设置字段值，从传递的字符串开始。调用后，foo 将返回value的副本
void set_foo(string&& value);

//使用C样式的以NULL终止的字符串设置字段的值。调用后，foo 将返回value的副本。
void set_foo(const char* value);

// 与上面类似，但是字符串大小是明确给出的，而不是通过查找空终止符字节来确定的。
void set_foo(const char* value, int size);

// 返回指向string存储该字段值的可变对象的指针。如果在调用之前未设置该字段，则返回的字符串将为空。调用此方法后，foo将返回写入给定字符串的任何值。
string* mutable_foo();

// 清除字段的值。调用此命令后，foo 将返回空字符串/ 空字节
void clear_foo();

// 将string对象设置为字段，并释放先前的字段值（如果存在），如果string指针不是NULL，则消息将获得分配的string对象的所有权。该消息可随时删除已分配的string对象，因此对该对象的饮用可能无效。否则，如果为value 为NULL，则行为与调用clear_foo相同。
void set_allocated_foo(string* value);

// 释放字段的所有权并返回string对象的指针。调用此方法后，调用者将获得分配的string对象的所有权，foo并将返回空字符串/空字节。
string* release_foo();
```



###### repeated 字符串字段

RepeatedPtrField:https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.repeated_field#RepeatedPtrField

```c++
// repeated string foo = 1;
// repeated bytes foo = 1;


// 返回字段中当前的元素数。
int foo_size() const;

// 以给定的从0开始的索引返回元素，用索引[0, foo_size()) 之外的数值调用此方法将产生未定义行为。
const string& foo(int index) const;

// 在给定的从0开始的索引处设置元素的值
void set_foo(int index, const string& value);

// 使用C样式的以空字符结尾的字符串在给定的从0开始的索引处设置元素的值
void set_foo(int index, const char* value);

// 与上面类似，但是字符串大小不是通过空字符判断，而是明确给出的
void set_foo(int inex, const char* value, int size);

// 返回指向可变string对象的指针，该对象在给定的从0开始的索引处存储元素的值。用索引[0, foo_size())  范围外的方法调用此方法将产生未定义的行为。
string* mutable_foo(int index);

// 使用给定值将新元素追加到字段的末尾
void add_foo(const string& value);

// 使用C样式的以空字符结尾的字符串将新元素追加到字段的末尾。
void add_foo(const char* value);

// 与上面类似，但是字符串大小是明确给出的，而不是通过查找空终止符字节来确定的。
void add_foo(const char* value, int size);

// 在该字段的末尾添加一个新的空字符串元素，并返回指向它的指针
string* add_foo();

// 从字段中删除所有元素，调用后，foo_size()将返回0
void clear_foo();

// 返回RepeatedPtrField存储类，此类提供了类似于STL的迭代器和其他方法。
const RepeatedPtrField<string>& foo() const;

// 返回指向RepeatedPtrField存储类的指针，此类提供了类似STL的迭代器和其他方法。
RepeatedPtrField<string>* mutable_foo();
```



###### singular 枚举字段

**proto2**

```c++
// enum Bar {
//  BAR_VALUE = 0;
//  OTHER_VALUE = 1;
// }
// optional Bar foo = 1;
// required Bar foo = 1;

// 返回true来确定是否设置了该字段
bool has_foo() const;

// 返回字段的当前值。如果未设置该字段，则返回默认值。
Bar foo() const;

// 设置字段的值。调用欧冠此函数后，has_foo() 将返回true， 并且foo() 将返回 value。在调试模式下（即定义NDEBUG），如果value与定义的任何值都不匹配Bar，则此方法将中止该过程。
void set_foo(Bar value);

// 清除字段值。调用此函数后，has_foo() 将 返回false 并且 foo() 返回默认值。
void clear_foo();
```

**proto3**

```c++
//enum Bar {
//    BAR_VALUE = 0;
//    OTHER_VALUE = 1;
//}
// Bar foo = 1;

//返回字段的当前值，如果未设置该字段，则返回默认值（0）。
Bar foo() const;

// 设置字段的值。调用此方法后，foo()将返回value
void set_foo(Bar value);

// 清除字段的值，调用后 foo 将返回默认值
void clear_foo();
```



###### repeated 枚举字段

```c++
// enum Bar {
//    BAR_VALUE = 0;
//    OTHER_VALUE = 1;
//}
// repeated Bar foo = 1;

// 返回字段中当前的元素数。
int foo_size() const;

// 以给定的从0开始的索引返回元素，用索引[0, foo_size()) 之外的数值调用此方法将产生未定义行为。
Bar foo(int index) const;

// 在给定的从0开始的索引处设置元素的值。在调试模式下（即未定义NDEBUG），如果value与任何已定义的Bar的值都不匹配，则此方法中止该进程。
void set_foo(int index, Bar value);

// 使用给定值将新元素追加到字段的末尾在调试模式下（即未定义NDEBUG），如果value与任何已定义的Bar的值都不匹配，则此方法中止该进程。
void add_foo(Bar value);

// 从字段中删除所有元素，调用后，foo_size()将返回0
void clear_foo();

// 返回RepeatedField存储类，此类提供了类似于STL的迭代器和其他方法。
const RepeatedField<int>& foo() const;

// 返回指向RepeatedField存储类的指针，此类提供了类似STL的迭代器和其他方法。
RepeatedField<int>* mutable_foo();
```





###### singular 嵌套消息

```c++
// message Bar {}

// proto2
// optioanl Bar foo = 1;
// required Bar foo = 1;

// proto3
// Bar foo = 1;

// 返回true以确定是否设置了该字段。
bool has_foo() const;

// 返回字段的当前值。如果字段没有设置，返回一个没有设置任何字段的Bar(可能是Bar::default_instance())。
const Bar& foo() const;

// 返回指向存储该字段值的可变对象的指针Bar。如果未在调用之前设置该字段，则返回的Bar不会设置任何字段（即，它将与新分配的字段相同Bar）。调用此方法后，has_foo()将返回true， foo（）返回对相同实例的引用Bar。
Bar* mutable_foo();

// 清除字段的值。调用此函数后，has_foo 将返回false 并且 foo 返回默认值。
void clear_foo();

// 设置Bar对象为字段，并释放之前的字段值（如果存在）。如果Bar指针不为空，则消息获取已分配Bar对象的所有权，has_foo将返回true。否则，如果Bar为NULL，其行为与调用clear_foo 相同。
void set_allocated_foo(Bar* bar);

// 释放字段的所有权并返回Bar对象的指针，调用此方法后，调用者将获得分配的Bar对象的所有权，has_foo 将返回 false， foo将返回默认值。
Bar* release_foo();


```



###### repeated 嵌套歇息

```c++
//message Bar{}

//repeated Bar foo = 1;

//返回字段中当前的元素数
int foo_size() const;

// 以给sing的从零开始的索引返回元素。用索引[0, foo_size()) 范围外的值调用此方法将产生未定义的行为。
const Bar& foo(int index) const;

// 返回指向可变Bar对象的指针，该对象在给定的从0开始的索引处存储元素的值。用索引[0, foo_size()), 范围外的值调用此 方法将产生未定义的行为。
Bar* mutable_foo(int index);

// 在该字段的末尾添加一个新元素，并返回指向该元素的指针。返回Bar的变量是可变的，并且不会设置任何字段，（即，它将与新分配的Bar相同）
Bar* add_foo();

// 从字段中删除所有元素，调用后，foo_size() 将返回0
void clear_foo();

// 返回RepeatedPTRField 存储类，此类提供了类似于STL的迭代器和其他方法
const RepeatedPtrField<Bar>& foo() const;

// 返回指向RepeatedPtrField存储类的指针，此类提供了类似于STL的迭代器和其他方法。
RepeatedPtrField<Bar>* mutable_foo();
```



###### Oneof 数字字段

```c++
//oneof oneof_name {
//    int32 foo = 1;
//    ...
//}

// 如果Oneof case为KFoo，则返回true， （proto2）
bool has_foo() const;

// 如果Oneof case是KFoo则返回字段的当前值，否则返回默认值。
int32 foo() const;

// 如果在同一个Oneof中设置了其他Oneof字段，调用clear_oneof_name
// 设置此字段的值，并将Oneof的case设置为kFoo
// has_foo() 将返回true（proto2），foo（）将返回value。oneof_name_case（）将返回KFoo。
void set_foo(int32 value);

// 如果Oneof case不是KFoo，没有任何改变
// 如果oneof case为KFoo，则清除该字段的值和Oneof case，has_foo（proto2) 将返回false，foo()返回默认值，oneof_name_case() 将返回ONEOF_NAME_NOT_SET
void clear_foo();
```

对于其他数字字段类型（包括`bool`），` int32`根据[标量值类型表](https://developers.google.cn/protocol-buffers/docs/reference/language.shtml#scalar)将其替换为相应的C ++类型。

###### Oneof 字符串字段

```c++
//oneof oneof_name{
//    string foo = 1;
//    ...
//}

//oneof oneof_name {
//    bytes foo = 1;
//}

// 如果Oneof case为KFoo，则返回true， （proto2）
bool has_foo() const;

// 如果Oneof case是KFoo则返回字段的当前值，否则返回默认值。
const string& foo() const;

// 如果在同一个Oneof中设置了其他Oneof字段，调用clear_oneof_name
// 设置此字段的值，并将Oneof的case设置为kFoo
// has_foo() 将返回true（proto2），foo（）将返回value。oneof_name_case（）将返回KFoo。
void set_foo(const string& value);
void set_foo(const char* value);
void set_foo(const char* value, int size);

// 如果在同一个Oneof中设置了其他Oneof字段，调用clear_oneof_name
// 设置Oneof的case设置为kFoo，并返回一个存贮这字段值的可变string对象的指针。如果Oneof case在调用之前不是KFoo， 则返回的字符串将为空（不是默认值）
// has_foo() 将返回true（proto2），foo（）将返回value。oneof_name_case（）将返回KFoo。
string* mutable_foo();

// 如果Oneof case不是KFoo，没有任何改变
// 如果oneof case为KFoo，则清除该字段的值和Oneof case，has_foo（proto2) 将返回false，foo()返回默认值，oneof_name_case() 将返回ONEOF_NAME_NOT_SET
void clear_foo();

// 调用clear_oneof_name（）
// 如果string指针为NULL：value设置为该字段值，并且设置Oneof case为KFoo。该消息拥有所分配的string对象的所有权，has_foo返回true（proto2）
// 如果string指针为NULL，has_foo（）返回false（proto2），并且oneof_name_case()返回ONEOF_NAME_NOT_SET。
void set_allocated_foo(string* value);

// 如果Oneof case不是KFoo， 返回NULL
// 清楚 Oneof case，释放字段的所有权，并且返回字符串对象的指针。调用此方法后，调用者将获得字符串对象的所有权。has_foo返回false（proto2），foo 返回默认值，oneof_name_case 返回ONEOF_NAME_NOT_SET。
string* release_foo();
```



###### Oneof 枚举字段

```c++
//enum Bar {
//    BAR_VALUE = 0;
//    OTHER_VALUE = 1;
//}

//oneof oneof_name {
//    Bar foo = 1;
//  	...
//}


// 如果Oneof case为KFoo，则返回true， （proto2）
bool has_foo() const;

// 如果Oneof case是KFoo则返回字段的当前值，否则返回默认值。
Bar foo() const;

// 如果在同一个Oneof中设置了其他Oneof字段，调用clear_oneof_name
// 设置此字段的值，并将Oneof的case设置为kFoo
// has_foo() 将返回true（proto2），foo（）将返回value。oneof_name_case（）将返回KFoo。
// 调试模式下（即NDEBUG没有被定义），如果value没有匹配到任何已定义的Bar，则此方法将中止该进程。
void set_foo(Bar value);

// 如果Oneof case不是KFoo，没有任何改变
// 如果oneof case为KFoo，则清除该字段的值和Oneof case，has_foo（proto2) 将返回false，foo()返回默认值，oneof_name_case() 将返回ONEOF_NAME_NOT_SET
void clear_foo();
```



###### Oneof 嵌套字段

```c++
//message {}
//
//oneof oneof_name {
//    Bar foo = 1;
//    ...
//}

// 如果Oneof case为KFoo，则返回true
bool has_foo() const;

// 如果Oneof case是KFoo则返回字段的当前值，否则返回Bar::default_instance()。
const Bar& foo() const;

// 如果在同一个Oneof中设置了其他Oneof字段，调用clear_oneof_name
// 设置Oneof的case设置为kFoo，并返回一个存贮这字段值的可变string对象的指针。如果Oneof case在调用之前不是KFoo， 则返回的字符串将为空（不是默认值）
// has_foo() 将返回true（proto2），foo（）将返回value。oneof_name_case（）将返回KFoo。
Bar* mutable_foo();

// 如果Oneof case不是KFoo，没有任何改变
// 如果oneof case为KFoo，则清除该字段的值和Oneof case，has_foo 将返回false，foo()返回默认值，oneof_name_case() 将返回ONEOF_NAME_NOT_SET
void clear_foo();

// 调用clear_oneof_name（）
// 如果Bar指针为NULL：bar设置为该字段值，并且设置Oneof case为KFoo。该消息拥有所分配的string对象的所有权，has_foo返回true, oneof_name_case() 返回kFoo
// 如果string指针为NULL，has_foo（）返回false（proto2），并且oneof_name_case()返回ONEOF_NAME_NOT_SET。(行为就想clear_oneof_name())
void set_allocated_foo(string* value);

// 如果Oneof case不是KFoo， 返回NULL
// 清楚 Oneof case，释放字段的所有权，并且返回字符串对象的指针。调用此方法后，调用者将获得字符串对象的所有权。has_foo返回false（proto2），foo 返回默认值，oneof_name_case 返回ONEOF_NAME_NOT_SET。
Bar* release_foo();
```



###### Map 字段

```c++
// map<int32, int32> weight = 1;

// 返回一个不可变的map
const google::protobuf::Map<int32, int32>& weight();

// 返回一个可变的Map
google::protobuf::Map<int32, int32>* mutable_weight();
```

google::protobuf::Map是一种特殊的容器类型，用于protobuf中存储映射字段

**它实现了std::map和std::unordered_map最常用子集**。

```c++
template<typename Key, typename T> 
class Map
{
    // Member types
    typedef Key key_type;
    typedef T mapped_type;
    typedef MapPair< Key, T> value_type;
    
    // Iterators
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    
    // Capacity
    int size() const;
    bool empty() const;
    
    // Element access
    T& operator[](const Key& key);
    const T& at(const Key& key) const;
    T& at(const Key& key);
    
    // Lookup
    int count(const Key& key) const;
    const_iterator find(const Key& key) const;
    iterator find(const Key& key);
    
    // Modifiers
    pair<iterator, bool> insert(const value_type& value);
    template<class InputIt>
        void insert(InputIt first, InputIt last);
    size_type erase(const Key& Key);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void clear();

    // Copy
    Map(const Map& other);
    Map& operator=(const Map& other);
};
```

t添加数组的最简单方式是使用正常的map语法：

```c++
std::unique_ptr<ProtoName> my_enclosing_proto(new ProtoName);
(*my_enclosing_proto->mutable_weight())[my_key] = my_value;
```

```pair<iterator, bool> insert(const value_type& value)```将隐式的进行```value_type```实例的深拷贝。最常用的插入方法是：

```c++
T& operator[](const Key& key): map[new_key] = new_mapped;
```

将protobuf：：Map用作std::map

`google::protobuf::Map`支持相同的迭代器API的`std::map`和`std::unordered_map`。

如果您不想直接使用`google::protobuf::Map`，通过执行以下操作将转换为标准map：

```c++
std::map<int32, int32> standard_map(message.weight().begin(), message.weight().end());
```

这将导致整个map的拷贝。

同时，还可以从一个标准map来构造一个google::protobuf::Map:

```c++
google::protobuf::Map<int32, int32> weight(standard_map.begin(), standard_map.end());
```

**解析未知值**

在wire上，.proto map等价于每个键/值对的映射条目消息，而映射本身是映射条目的重复字段。与普通消息类型一样，解析后的映射条目消息也可能具有未知字段:例如，定义为`map<int32, string>`.的映射中类型为int64的字段。

如果map条目消息的有线格式中有未知字段，则将丢弃它们。

如果map条目消息的有线格式中有一个未知的枚举值，那么在proto2和proto3中会以不同的方式处理它。在proto2中，整个映射条目消息被放入包含消息的未知字段集中。在proto3中，它被放到一个map字段中，就好像它是一个已知的枚举值一样。



###### Any 字段

```protobuf
import "google/protobuf/any.proto"
    
message ErrorStatus {
    string message = 1;
    google.protobuf.Any details = 2;
}
```

在我们生成的代码中，该details字段的getter 返回的实例是`google::protobuf::Any`

```c++
class Any {
  public:
    // Packs the given message into this Any using the default type URL
    // prefix "type.googleapis.com".
    void PackFrom(const google::protobuf::Message& message);
    
    // Packs the given message into this Any using the given URL
    // prefix
    void PackFrom(const google::protobuf::Message& message, const string& type_url_prefix);
    
    // Unpacks this Any to a Message. Returns false if this Any 
    // represents a different protobuf type or parsing fails
    bool UnpackTo(google::protobuf::Message* message) const;
    
    // Returns true if this Any represents the given protobuf type.
    template<typename T> bool Is() const;
};
```



###### oneof

```protobuf
oneof oneof_name {
	int32 foo_int = 4;
	string foo_string = 9;
}
```

```c++
enum OneofNameCase {
    kFooInt = 4;
    kFooString= 9;
    ONEOF_NAME_NOT_SET = 0;
}

// 返回表示设置了哪个字段的枚举。如果没有字段被设置，则返回ONEOF_NAME_NOT_SET
OneofNameCase oneof_name_case() const;

// 如果Oneof字段集使用指针(消息或字符串)，则将对象释放，并将Oneof的case设置为ONEOF_NAME_NOT_SET
void clear_oneof_name();
```



##### 枚举

```protobuf
enum Foo {
	VALUE_A = 0;
	VALUE_B = 5;
	VALUE_C = 1234;
}
```

protobuf 将生成一个名为Foo的C++ 枚举类型，会带有相同的值集合。同时会生成如下的全局函数。

```c++
// 返回类型的描述符，该描述符包含有关此枚举类型定义那些值的信息
const EnumDescriptor* Foo_descriptor();

// 如果给定的数值能匹配到Foo中定义的任意值，就返回true。在上面的实例中，如果输入时0,5,1234 就返回true
bool Foo_InsValid(int value);

// 返回给定数值的名称，如果不存在这样的值，则返回一个空字符串。如果此值有多个值（别名），则返回定义的第一个值、在上面的示例中，Foo_Name(5)将返回“VALUE_B"
const string& Foo_Name(int value);

// 如果name是此枚举的有效值名称，则将该值分配给value，并返回true。否则返回false，在上面的事例中，Foo_parse（“VALUE_C", &some_foo) 将返回true，并设置some_foo为1234
bool Foo_Parse(const string& name, Foo* value);

// 枚举的最小有效值
const Foo Foo_MIN;

// 枚举的最大有效值
const Foo Foo_MAX;

// 始终定义为Foo_MAX + 1 
const int Foo_ARRAYSIZE;
```

> **将整数强制转换为proto2枚举时要小心。** 如果将整数强制转换为proto2枚举值，则该整数*必须*是than枚举的有效值之一，否则结果可能不确定。如有疑问，请使用生成的`Foo_IsValid()`函数测试强制转换是否有效。将proto2消息的枚举类型的字段设置为无效值可能会导致断言失败。如果在解析proto2消息时读取了无效的枚举值，它将被视为[未知字段](https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.unknown_field_set)。这些语义已在proto3中更改。只要将整数整数适合int32，就可以将其强制转换为proto3枚举值。解析proto3消息时，无效的枚举值也将保留，并由枚举字段访问器返回。

> **在switch语句中使用proto3枚举时要小心。**Proto3枚举是开放式枚举类型，其可能的值超出指定符号的范围。解析proto3消息时将保留无法识别的枚举值，并由枚举字段访问器返回。即使列出了所有已知字段，没有默认大小写的proto3枚举上的switch语句也将无法捕获所有大小写。这可能导致意外的行为，包括数据损坏和运行时崩溃。**始终添加默认大小写或`Foo_IsValid(int)`在开关外部显式调用以处理未知的枚举值。**

您可以在消息类型内定义一个枚举。在这种 情况下，协议缓冲区编译器会生成代码， 从而使枚举类型本身看起来已嵌套在消息的类中。

在Foo_descriptor()  与 Foo_IsValid() 函数声明为静态方法。实际上，枚举类型本身机器值在全局范围内使用乱码进行声明，并使用typedef 和一系列常量定义导入到类的范围内。这样做只是为了解决声明排序的问题。不要依赖于混乱的顶级名称；假设枚举确实嵌套在消息类中。

##### 拓展（proto2）

```protobuf
message Foo {
	extensions 100 to 199;
}

// protobuf 编译器将产生一些额外的方法给Foo：
// HasEXtension()
// ExtensionSize()
// ClearExtension()
// GetExtension()
// SetExtension()
// MutableExtension()
// AddExtension()
// SetAllocatedExtension()
// ReleaseExtension()
//  	这些方法中的第一个参数都是一个扩展标识符，标识一个扩展字段。其余的参数进而返回值与对应的访问器方法的参数和返回值完全相同，这些访问器方法将于扩展标识符类相同的普通（非扩展）字段生成。（GetExtension() 对应于没有特殊前缀的访问器）

extend Foo {
	optional int32 bar = 123;
	repeated int32 repeated_bat = 124;
}


```

```c++
// 对于singular 拓展字段bar，protobuf编译器生成“扩展标识符” bar，可以将其与Foo的拓展访问器一起使用，以访问此扩展。
Foo foo;
assert(!foo.HasExtension(bar));
foo.SetExtension(bar, 1);
assert(foo.HasExtension(bar));
assert(foo.GetExtension(bar) == 1);
foo.ClearExtension(bar);
assert(!foo.HasExtension(bar));

// 对于 repeated 扩展字段 repeated_bar， 编译器会生成一个名为 repeated_bar的扩展标识符，可以将其与Foo的扩展名访问器一起使用
Foo foo;
for(int i = 0; i < kSize; ++i) {
    foo.AddExtension(repeated_bar, i);
}
assert(foo.ExtensionSize(repeated_bar) == kSize );
for(int i = 0;i < kSize; ++i) {
    assert(foo.GetExtension(repeated_bar, i) == i);
}
```

扩展标识符的完整实现很复杂，并且使用了模板的特殊技巧。但是不必担心在拓展标识符的使用上如何使用。

扩展可以声明为嵌套在另一种类型内，例如，一种常见的模式是执行一下操作：

```protobuf
message Baz {
	extend Foo {
		optional Baz foo_ext = 124;
	}
}
```

这种情况下，扩展标识符foo_ext 声明为嵌套的Baz。

```c++
Foo foo;
Baz* baz = foo.MutableExtension(Baz::foo_ext);
FillInMyBaz(baz);
```

##### service

如果proto文件包含以下行：

```protobuf
optional cc_generic_services = true;
```

然后，protobuf编译器将根据文件中找到的服务定义生成代码。但是，生成的代码可能并不理想，因为它没有绑定到特定的RPC系统，因此需要比定制一个系统 的代码更多的间接级别。如果不希望生成此代码，请在文件中添加：

```protobuf
optional cc_generic_services = false;
```

如果以上两行均未给出，则该选项默认为false，因为不支持使用常规服务、基于proto语言定义的RPC系统应提供插件：https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin.pb。以生成适合系统的代码，这些插件可能需要禁用抽象服务，以便他们可以生成自己的相同名称的类。

本节的其余部分描述了启用抽象服务时，protobuf编译器将生成的内容。

###### 接口

```protobuf
service Foo{
	rpc Bar(FooRequest) returns(FooResponse);
}
```

protobuf 编译器将生成一个Foo类代表该服务。Foo中定义的每个方法都有一个虚方法。

```c++
virtual void Bar(RpcController* controller, const FooRequest* request, FooResponse* response, Closure* done);
```

这些参数等效于`Service::CallMethod()`的参数，不同的是，方法参数是银行的，请求和相应指定了它们的确切类型。

这些生成的方法是虚拟的，饭不是纯虚的。默认实现只是调用`Controller->SetFailed()`，并带有一个错误消息，表明该方法未实现，然后调用done回调。在实现自己的服务器时，必须将生成的服务子类化，并释放地实现其方法。

Foo将Service接口子类化。protobuf 编译器自动生成以下方法的实现Service：

- `GetDescriptor`：返回服务的ServiceDescriptor
- `CallMethod`：根据提供的方法描述符确定正在调用的方法，并直接调用该方法，将请求和相应消息对象向下转换为正确的类型。
- `GetRequestPrototype和GetResponsePrototype`：返回给定方法的正确类型的请求或相应的默认实例。
- `static ServiceDescriptor descriptor()`：返回类型的描述符，其中包含有关此服务具有哪些方法以及它们的输入和输出类型是什么的信息。

###### stub 存根

protobuf编译器还会生成每个服务接口的“stub”实现，由希望发送请求到实现该服务的服务器的客户端使用。对于Foo的服务，Foo_stub将实现stub。与嵌套消息一样，使用typedef，也即Foo::Stub。

Foo_stub是Foo的子类，还实现了如下方法：

- `Foo_Stub(RpcChannel* channel)`：构造一个新的存根，该存根在给定的通道上发送请求。
- `Foo_Stub(RpcChannel* channel, ChannelOwnership ownership)`：构造一个新的存根，该存根在给定通道上发送请求，并可能拥有该通道。如果`ownership`是，`Service::STUB_OWNS_CHANNEL`则在删除存根对象时，它也会同时删除该通道。
- `RpcChannel* channel()`：返回此存根的通道，并传递给构造函数。

stub作为通道的包装器另外实现了服务的每个方法。调用其中一个方法只是调用`channel-&gt;CallMethod()`。

protobuf 库不包括RPC实现。但是，它包含了将生成的服务类连接到您选择的任意RPC实现所需的所有工具。您只需要提供`RpcChannel`和``RpcController`的实现。有关更多信息，请参阅`service.h`的文档。https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.service

##### 插件插入点

想要扩展C ++代码生成器输出的[代码生成器插件](https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin.pb)以使用给定的插入点名称来插入以下类型的代码。除非另有说明，否则每个插入点都出现在`.pb.cc`文件和`.pb.h`文件中。

- `includes`：包括指令。
- `namespace_scope`：声明属于文件的包/名称空间，但不属于任何特定类。出现在所有其他命名空间范围代码之后。
- `global_scope`：位于文件名称空间之外的顶级声明。出现在文件的最后。
- `class_scope:TYPENAME`：属于消息类的成员声明。 `TYPENAME`是完整的原型名称，例如`package.MessageType`。出现在全班其他所有公开声明之后。该插入点仅出现在`.pb.h`文件中。

不要生成依赖于标准代码生成器声明的私有类成员的代码，因为这些实现细节可能会在协议protobuf 未来版本中更改。

> c++ Arena Allocation guide:https://developers.google.cn/protocol-buffers/docs/reference/arenas

Arena分配是仅在c++实现中拥有的功能，可以帮助在使用protobuf是优化内存使用，并提高性能。在启用Arena竞技场分配的过程中，proto文件会在c++生成的代码中添加用于使用Arena的其他代码。

#### C++ API

> c++ API: https://developers.google.cn/protocol-buffers/docs/reference/cpp
