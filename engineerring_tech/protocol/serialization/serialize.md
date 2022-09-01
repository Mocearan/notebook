# 序列化

---

​		消息协议的实现方式经历了一些发展阶段，不断的将复杂性封装，使得消息协议的实现和使用更为简单。



## C/C++系 struct 作为消息实体

​		两端直接将`struct`作为实体发送，两端各自负责结构体的解析。

> `struct`实际上是一种二进制数据。

​		结构体的编解，包括对结构体中各个字段的解析，以及大小端的转换。

- struct的字节对齐

  - padding填充

- 数据的大小端转换

  

## 序列化实体类

​	序列化pod和消息实体类的实体。

​	通过序列化功能的提供者来保证字节对齐和大小端转换。

### ASN.1

> [ASN1 - 简书 (jianshu.com)](https://www.jianshu.com/p/44e6a9e1fbe2)
>
> [ASN.1入门（超详细） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/156898176)

### boost.serialization : archive

#### 非boost.serialization的archive

```c++
class CMessage
{
private:
    int id;
    std::string name;
    std::vector<std::string> groups;
    float coolNumbers[ 3 ];

public:
    CMessage( int nid = 1, const std::string &nname = "unknown" ) : id( nid ), name( nname )
    {
        coolNumbers[ 0 ] = 3.1416f;
        coolNumbers[ 1 ] = 2.7182f;
        coolNumbers[ 2 ] = 1.6180f;

        groups.push_back( "wheel" );
        groups.push_back( "lp" );
        groups.push_back( "root" );
        groups.push_back( "sudo" );
    }

    template <class T>
    void Serialize( T &archive )
    {
        archive &id &name &groups &coolNumbers;
    }
};



///
template <class T>
void Test( const T &testValue )
{
    std::stringstream s;
    Archive<std::stringstream> a( s );
    
    T value( testValue );
    a << value;
    
    value = T();
    a >> value;
}
int main(void)
{
	Test( CMessage(777, "beast"));
}

```



### xml

```xml
<person>
    <id>1234</id>
    <name>John Doe</name>
    <email>jdoe@example.com</email>
</person>
```



## json 实体

```json
{
    "id":1234,
    "name":"john Doe",
    "email":"jdoe@example.com"
}
```



## protobuffer 实体

```protobuf
// pb的编码字节
0A
08 4A 6F 68 6E 20 44 6F 65
10
D2 09
10 6A 64 6F 65 40 65 78 61 6d 70 6c 65 2E 63 6F 6D
```

### 优缺点

优点：

1. 性能好，效率高
2. 代码生成机制，数据解析类自动生成
3. 支持向后兼容和向前兼容
4. 支持多种编程语言（java、c++、python、。。。）

缺点：

1. 二进制格式导致可读性差（二进制格式）
2. 缺乏自描述