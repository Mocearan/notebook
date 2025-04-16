

# GB28181 feature

---

​		会话通道中

- **注册、实时视音频点播**：控制采用`SIP`协议`IETF RFC3261`中规定的`REGISTER`、`INVITE`请求和响应实现
- **历史视音频回放控制**：采用`SIP`扩展协议`IETF RFC29765`规定的`INFO`方法实现
- **前端设备控制、信息查询、报警事件通知和分发等**：应用的会话控制采用`SIP`扩展协议`IETF RFC34287`规定的`MESSAGE`方法实现

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/917645-20180423091003505-1189103411.png)

### 注册

​		注册指的是设备或系统进入联网系统时向SIP服务器（`SIP UAS`）进行注册登记的工作模式。

- 使用**REGISTER**方法进行注册和注销；
- 注册和注销应进行认证，认证方式应支持**数字摘要**认证方式，高安全级别的宜支持**数字证书**认证；
- 注册成后，SIP代理在注册过期时间到来之前，应向注册服务器进行刷新注册；
- 若注册失败，应在不短于60s的时间间隔内重新发起注册；
- 系统、设备注册过期时间可配置；
- SIP代理是否注册成功决定SIP服务器在离线状态；
- 对于处于**开启网络地址转换(NAT)功能的路由器**内侧的SIP代理，应该支持使用TCP发起SIP注册，并在注册成功后保持TCP连接不关闭，SIP代理及服务器在该TCP通道里发送所有请求及响应SIP消息。若TCP通道断开，则认为SIP代理异常掉线，SIP代理应在一定时间间隔内重新发起注册；

- 设备（或者下级平台）向上级平台发送注册信息，携带上级平台和自己的国标ID等信息
- 上级平台收到注册信息后
  - 首先校验其中的上级平台国标ID是否正确，如果不正确，则拒绝接入
  - 校验其中的设备（或者下级平台）国标ID，是否已经在平台中添加
    - 如果已经添加，则允许接入
    - 如果没有添加过，则拒绝连接
- 注销流程也一致，请求中的Expires为0表示注销



#### 设备身份认证

- 低安全级别：数字摘要认证
- 高安全级别：数字证书认证

  - 网络层宜采用`IPSec`或传输层宜采用`TLS`对`SIP`消息实现逐跳安全加密

    > 逐跳安全加密【链路加密】是指：
    >
    > ​	在离开一个节点【网络中的中继节点】进入信道时加密，在离开信道通过节点时解密。
    >
    > ​	因此数据在信道中呈现密文形式，在节点中呈现明文形式。

  - 宜在应用层采用`S/MIME`机制的端到端加密
    - 在源端进行加密，在传输过程中不解密，一直到达目的端才进行解密因此对于中继节点来说，用户数据是不可知的密文


  - 会话密钥宜采用`RSA`算法加密，传输内容宜采用`DES`、`3DES`、`AES128`算法加密

  - 数据存储宜采用`3DES`、`AES(128位)`、`SM1`等算法进行加密




#### 设备配置

1. 本地SIP端口：默认5060，相机针对GB28181协议开放的通信端口；
2. 传输协议：支持UDP和TCP协议；
3. 勾选“启动”按钮，启用GB28181-2016协议；
4. SIP服务器ID：服务器标识，详见GB28181协议定义；
5. SIP服务域：SIP服务器ID的前10位；
6. SIP服务器地址：**SIP服务所在机器的IP地址**；
7. SIP服务端口：**SIP服务端口**；

​		启用GB28181后，设备定期对外发送REGISTER命令，如无服务端连接，由ICMP协议返回464目标不可达。



#### 基本注册流程

- 1:`SIP  `代理向 `SIP  `服务器发送 `REGISTER  `请求

  - 通过添加`Authenticate`字段增加 `SIPUA `安全能力

  ```http
  REGISTER sip:130909115229300920@10.64.49.44:7100 SIP/2.0
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK4162288924
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
  To: <sip:130909113319427420@10.64.49.218:7100>
  Call-ID: 143225205
  CSeq: 1 REGISTER
  Contact: <sip:130909113319427420@10.64.49.218:7100>
  Max-Forwards: 70
  User-Agent: Hikvision
  Expires: 7200
  Content-Length: 0
  ```

- 2:`SIP  `服务器向 `SIP UA`发送响应`401`，表示未认证

  - `WWW-Authenticate: <认证方案><参数=值><参数=值>...`
  
    - 认证方案：
  
      - `basic`，`base64`编码的明文加密
      - `digest`，通过摘要加密 `md5 / sha256`等
  
    - `realm`、`nonce`和`opaque`通常是在基于摘要认证（Digest Authentication）时使用的参数，其含义如下：
  
      - `realm`：表示认证域或者保护区域，一般用来描述资源的域或服务器的标识。
        - 客户端在发起认证请求时，会在该域下进行验证，确保在同一认证域中的资源使用相同的凭据。
      - `nonce`：是服务器生成的一个随机数或者“一次性”值，用于防止重放攻击。
        - 每次服务器发起认证挑战时，`nonce`值通常会不同，客户端需要将该`nonce`与密码等信息结合后生成响应，从而提高安全性。
      - `opaque`：是服务器返回的一个任意字符串，客户端在后续请求中必须原样返回该值。
        - `opaque`字段主要用来维护状态或者附加某些验证信息，对客户端是透明的，其具体含义由服务器决定。
  
      简单来说，这三个参数共同构成了一个挑战-响应认证机制，用以保证通信过程中的身份验证安全。
  
  
  ```http
  SIP/2.0 401 Unauthorized
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK4162288924
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
  To: <sip:130909113319427420@10.64.49.218:7100>;tag=916944766
  Call-ID: 143225205
  CSeq: 1 REGISTER
  WWW-Authenticate: Digest realm="hik", nonce="a8afe6fcbee6331d89d3eb0d3d19ce39", opaque="a853e4f25298413f9bf3a9aa6767857d", algorithm=MD5
  User-Agent: Hikvision
  Expires: 7200
  Content-Length: 0
  ```


- 3:`SIP UA `重新向 SIP 服务器发送 `REGISTER  `

  
  - 在请求的Authorization字段给出信任书，包含认证信息
  
  ```http
  REGISTER sip:130909115229300920@10.64.49.44:7100 SIP/2.0
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK3997518011
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
  To: <sip:130909113319427420@10.64.49.218:7100>
  Call-ID: 143225205
  CSeq: 2 REGISTER
  Contact: <sip:130909113319427420@10.64.49.218:7100>
  Authorization: Digest username="admin", realm="hik", nonce="a8afe6fcbee6331d89d3eb0d3d19ce39", uri="sip:130909115229300920@10.64.49.44:7100", response="907ddb1bcc25174d7de4a96c947fb066", algorithm=MD5, opaque="a853e4f25298413f9bf3a9aa6767857d"
  Max-Forwards: 70
  User-Agent: Hikvision
  Expires: 7200
  Content-Length: 0
  ```

- 4:`SIP  `服务器对请求进行验证

  - 如果检查出 `SIP UA`身份合法，向 `SIP  UA`发送成功响`200` 
    - 服务器一般将认证信息记录在数据库，通过数据查询服务或直接访问数据库查询认证信息
  - 如果身份不合法则发送拒绝服务应答
  
  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK3997518011
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
  To: <sip:130909113319427420@10.64.49.218:7100>;tag=705514612
  Call-ID: 143225205
  CSeq: 2 REGISTER
  Contact: <sip:130909113319427420@10.64.49.218:7100>
  User-Agent: Hikvision
  Date: 2013-09-10T16:01:51
  Content-Length: 0
  ```

![sip监控域是什么 监控区域什么意思_字段_06](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20192524_65fac7a47e6d996244.png)



#### 数字整数双向认证

- `Authorization`的值为 `Capability`
  - 参数`algorithm `的值分为三部分
    - 第一部分为非对称算法描述，取值为 RSA
    - 第 二 部分为摘要算法描述，取值为 MD5/SHA-1/SHA-256中的一个或者多个
    - 第三部分为对称算法的描述，取值为 DES/3DES/ SM1中的一个或者多个
- `SIPUA `收到`401`响应后，得到`nonce`中的`a`和`b`两部分
  - a为SIP服务做的摘要，b为SIPUA的公钥加密的报文
- 首先用`SIPUA `私钥解密`b`，得到结果`c`
- 对结果`c`用`401`响应中`algorithm `指定的算法做摘要，得到结果`d`
- 用`sip`服务器公钥解密`a`，得到结果`d`
- 结果`d`进行匹配，如果相匹配则信任该结果，否则丢弃
- `SIPUA `重新向 `SIP`服务器发送 `Register`请求

  - `Authorization`取值为 `Asymmetric`

  - 参数`nonce`的值与上面`2:`中的相同

  - `response`的值为用本消息中`algorithm `指明的算法对`[c+nonce]`做摘要的结果



#### SIP信令认证

​		保证信令来源安全。注册成功后，信令交互时采用数字摘要方式，对除 `Register`消息以外的每一条消息都做数字摘要。

- 启用了 Date字段信令消息的头域
  -  Date比对有效时间范围可设,初设值为10min,应在校时精度范围内
  -  Date精确到秒
- 在头域中增加 Note字段
  - 值为 Digest，有两个参数 nonce、algorithm
  - Note=(Digest nonce="",algorithm =)
    - nonce的值为algorithm[From+to+CallID+Date+seed+消息体]数字摘要经过BASE64编码后 的值
    - algorithm 的值为数字摘要的算法名称,“+”为字符串连接运算
- 当跨域访问时
  - 若该信令是由本域的用户发起
    - 信令安全路由网关宜将发送到外域的信令添加` Monitor-User-Identity`头域
    - 其取值为信令安全路由网关ID 和用户的身份信息
  - 若该信令不是由本域 的用户发起
    - 只在原有` Monitor-User-Identity`域值前添加信令安全路由网关ID
    - 各段分隔符为`-`
  - 用户的身份为用户ID以及用户身份属性信息

![sip监控域是什么 监控区域什么意思_服务器_12](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20192525_65fac7a52cea526772.png)



### 注销

- SIP代理向SIP服务器发送REGISTER请求，**Expires字段的值为0，表示SIP代理要注销**；

  ```http
  REGISTER sip:130909115229300920@10.64.49.44:7100 SIP/2.0
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK1670314216
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=1928169842
  To: <sip:130909113319427420@10.64.49.218:7100>
  Call-ID: 3187228566
  CSeq: 1 REGISTER
  Contact: <sip:130909113319427420@10.64.49.218:7100>
  Max-Forwards: 70
  User-Agent: Hikvision
  Expires: 0
  Content-Length: 0
  ```

- SIP服务器向SIP代理响应401(未认证)，返回的消息头里给出适合SIP代理的认证体制和参数；

  ```http
  SIP/2.0 401 Unauthorized
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK1670314216
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=1928169842
  To: <sip:130909113319427420@10.64.49.218:7100>;tag=1002632848
  Call-ID: 3187228566
  CSeq: 1 REGISTER
  WWW-Authenticate: Digest realm="hik", nonce="42dc1acbe02b15743942c0af9314768b", opaque="8e3da4136ac9ab537de09a9932c2a9a3", algorithm=MD5
  User-Agent: Hikvision
  Expires: 0
  Content-Length: 0
  ```

- SIP代理重新向SIP服务器发送REGISTER请求，在请求的Authorization中给出信任书，包括认证信息，Expires字段的值为0；

  ```http
  REGISTER sip:130909115229300920@10.64.49.44:7100 SIP/2.0
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK317693249
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=1928169842
  To: <sip:130909113319427420@10.64.49.218:7100>
  Call-ID: 3187228566
  CSeq: 2 REGISTER
  Contact: <sip:130909113319427420@10.64.49.218:7100>
  Authorization: Digest username="admin", realm="hik", nonce="42dc1acbe02b15743942c0af9314768b", uri="sip:130909115229300920@10.64.49.44:7100", response="7328353ed26e3f2edf3ff57e834d5665", algorithm=MD5, opaque="8e3da4136ac9ab537de09a9932c2a9a3"
  Max-Forwards: 70
  User-Agent: Hikvision
  Expires: 0
  Content-Length: 0
  ```

- SIP服务对请求进行认证，如检测SIP代理身份合法，向SIP代理响应200 OK，如检测不合法发送拒绝服务应答；

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK317693249
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=1928169842
  To: <sip:130909113319427420@10.64.49.218:7100>;tag=409851991
  Call-ID: 3187228566
  CSeq: 2 REGISTER
  Contact: <sip:130909113319427420@10.64.49.218:7100>
  User-Agent: Hikvision
  Date: 2013-09-10T14:04:41
  Content-Length: 0
  ```




### 注册重定向

​		注册重定向是2022新增内容

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/60839df81f13a9fbb854c9114957db87.png)

#### 重定向流程

1. 设备或系统段的SIP代理向SIP重定向服务器发送REGISTER请求；
2. SIP重定向服务器向SIP代理响应401(未认证)，返回的消息头里给出适合SIP代理的认证体制和参数；
3. SIP代理重新向SIP重定向服务器发送REGISTER请求，在请求的Authorization中给出信任书，包括认证信息；
4. **SIP重定向服务器验证请求，如检测SIP代理身份合法，SIP重定向服务器根据内部策略选择设备需注册的SIP服务器A，并回复注册请求响应302，响应消息携带Contact头域和Expires，Contact格式为sip:SIP服务器A编码@目的IP地址端口，在此头域中携带SIP服务A的地址**；
5. SIP代理收到302重定向请求后，向SIP服务器A发起注册；
6. 6~8：完成注册重定向流程后，后续所有请求和响应动作由SIP服务器A和设备直接完成；
7. **重定向后，如果SIP代理检测SIP服务器A离线或注册被SIP服务器拒绝，SIP代理应重新执行注册重定向流程，向SIP重定向服务器发起注册请求**；

### 保活

- 当UA发现工作异常时， 应立即向本SIP监控域的SIP服务器发送状态信息
- 无异常时，定时向本SIP监控域的SIP服务器发送状态信息
- SIP设备宜在状态信息中携带故障子设备描述信息
- 通过周期性的状态信息报送，实现注册服务器与原设备之间的状态检测，即心跳机制
  - 心跳发送方、接收方需统一配置间隔，默认心跳间隔`60s`
  - 心跳发送方、接收方需统一配置超时次数，连续超时达到“超时次数认为对方下线，默认超时次数`3`次
  - 心跳接收方在心跳发送方上线状态下，检测到连续超时达到超时次数认为心跳发送方离线
  - 心跳发送方在心跳接收方上线状态下，检测到心跳响应消息连续超时达到超时次数认为心跳接收方离线

- 状态信息报送采用IETF RFC3427中定义的方法`MESSAGE`实现

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-a6e9bfc7b299c7b6f0bd0692cb3f1881_1440w.webp)



#### 保活信令xml格式

```xml
<! -- 命令类型:设备状态信息报送(必选)--> 
<elementname="CmdType"fixed="Keepalive"/> 
<! -- 命令序列号(必选)--> 
<elementname="SN" type="integer"minInclusivevalue= "1"/> 
<! -- 源设备的设备/系统编码(必选)--> 
<elementname="DeviceID" type="tg:deviceIDType"/> 
<! -- 是否正常工作(必选)--> 
<elementname="Status"type="tg:resultType"/> 
<! --故障设备列表-->
<elementname="Info">     
    <complexType>         
        <sequence>             
            <elementname="DeviceID"type="tg:deviceIDType" minOccurs="0" maxOccurs="unbounded"/>         
        </sequence>     
    </complexType> 
</element>
```



#### 保活信令流程

- 源设备向SIP服务器发送设备状态信息报送命令

  - 设备状态信息报送命令采用`MESSAGE`方法携带
    - `Content-type: Application/MANSCDP+xml`
    - 状态信息报送命令采用`MANSCDP`协议格式定义， 详细描述见`GB/T 28181—2016`中`A.2.5`状态信息报送

  ```http
  MESSAGE sip:130909115229300920@10.64.49.44:7100 SIP/2.0
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK2672759896
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=308835751
  To: <sip:130909115229300920@10.64.49.44:7100>
  Call-ID: 63810466
  CSeq: 20 MESSAGE
  Content-Type: Application/MANSCDP+xml
  Max-Forwards: 70
  User-Agent: Hikvision
  Content-Length:   150
  
  <?xml version="1.0"?>
  <Notify>
      <CmdType>Keepalive</CmdType>
      <SN>2749</SN>
      <DeviceID>130909113319427420</DeviceID>
      <Status>OK</Status>
  </Notify>
  ```

- SIP服务器收到命令后返回`200 OK`

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK2672759896
  From: <sip:130909113319427420@10.64.49.218:7100>;tag=308835751
  To: <sip:130909115229300920@10.64.49.44:7100>;tag=1578583786
  Call-ID: 63810466
  CSeq: 20 MESSAGE
  User-Agent: Hikvision
  Content-Length: 0
  ```



### 设备查询

- 源设备向目标设备发送信息查询命令，目标设备将结果通过查询应答命令返回给源设备；
- 设备信息查询命令包括：
  - 设备目录
  - 设备信息
  - 设备状态信息
  - 设备配置
  - 预置位、看守位
  - 巡航轨迹列表
  - 巡航轨迹
  - PTZ精准状态
  - 存储卡状态等
- 信息查询的范围：本地SIP监控域、跨SIP监控域；
- 网络设备信息查询命令均采用**`MESSAGE`**方法；
- 目录查询应答命令支持多响应消息传输；
- 源设备：
  - SIP客户端
  - 网关或联网系统
- 目标设备：
  - SIP设备端
  - 网关或联网系统

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/dda2a75501c8e5ae8913c4524e44cf75.png)

​		设备查询使用同一套流程，根据查询的命令不同，请求和相应荷载的XML结构不同。

#### 设备查询流程

- SIP服务器向目标设备转发设备查询命令

  ```http
  MESSAGE sip:130909113319427420@10.64.49.218:7100 SIP/2.0
  Via: SIP/2.0/UDP 10.64.49.44:7100;rport;branch=z9hG4bK32925498
  From: <sip:130909115229300920@10.64.49.44:7100>;tag=1307626839
  To: <sip:130909113319427420@10.64.49.218:7100>
  Call-ID: 2367611040
  CSeq: 20 MESSAGE
  Content-Type: Application/MANSCDP+xml    // 设备信息查询命令采用MANSCDP协议格式；
  Max-Forwards: 70
  User-Agent: Hikvision
  Content-Length:   124
  
  <?xml version="1.0" encoding="GB2312"?>
  <Query>
      <CmdType>Catalog</CmdType>
      <SN>471</SN>
      <DeviceID>130909113319427420</DeviceID>
  </Query>
  ```

- 目标设备收到命令后返回200OK;

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 10.64.49.44:7100;rport=7100;branch=z9hG4bK32925498
  From: <sip:130909115229300920@10.64.49.44:7100>;tag=1307626839
  To: <sip:130909113319427420@10.64.49.218:7100>;tag=1981225076
  Call-ID: 2367611040
  CSeq: 20 MESSAGE
  User-Agent: Hikvision
  Content-Length: 0
  ```

- 目标设备向SIP服务器发送设备查询响应命令

  ```http
  MESSAGE sip:51000000002000000001@5100000000 SIP/2.0
  Via: SIP/2.0/UDP 10.18.44.86:5060;rport;branch=z9hG4bK897441835
  From: <sip:62010201001320000393@5100000000>;tag=1617705135
  To: <sip:51000000002000000001@5100000000>
  Call-ID: 17524357
  CSeq: 20 MESSAGE
  Content-Type: Application/MANSCDP+xml  			// 设备信息查询应答命令采用MANSCDP协议格式；
  Max-Forwards: 70
  User-Agent: IP Camera
  Content-Length:   586
  
  <?xml version="1.0" encoding="GB2312"?>
  <Response>
  <CmdType>Catalog</CmdType>
  <SN>1708</SN>
  <DeviceID>62010201001320000393</DeviceID>
  <SumNum>1</SumNum>
  <DeviceList Num="1">
  <Item>
  <DeviceID>62010201001320000393</DeviceID>
  <Name>Camera 01</Name>
  <Manufacturer>Hikvision</Manufacturer>
  <Model>IP Camera</Model>
  <Owner>Owner</Owner>
  <CivilCode>5100000000</CivilCode>
  <Address>Address</Address>
  <Parental>0</Parental>
  <ParentID>51000000002000000001</ParentID>
  <SafetyWay>0</SafetyWay>
  <RegisterWay>1</RegisterWay>
  <Secrecy>0</Secrecy>
  <Status>ON</Status>
  </Item>
  </DeviceList>
  </Response>
  ```

- SIP服务器收到命令后返回200OK;

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 10.64.49.44:7100;rport=7100;branch=z9hG4bK32925498
  From: <sip:130909115229300920@10.64.49.44:7100>;tag=1307626839
  To: <sip:130909113319427420@10.64.49.218:7100>;tag=1981225076
  Call-ID: 17524357
  CSeq: 20 MESSAGE
  User-Agent: Hikvision
  Content-Length: 0
  ```

  







#### 目录检索

​		先要询问设备的视频通道的列表，为点播做准备。

![image.png](https://p3-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/8299b91f643440f5a5e20517d85de941~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAgSGFuYm94:q75.awebp?rk3s=f64ab15b&x-expires=1728027543&x-signature=XR5aAJ8GgwlymbIV3PfDd2kxNOY%3D)



##### 目录检索请求格式

```xml
<?xml version="1.0" encoding="GB2312"?>
<Query>
    <! -- 命令类型:设备信息查询(必选)-->
    <CmdType>Catalog</CmdType>
    <! -- 命令序列号(必选)，消息中的SN值用于与请求命令的匹配处理,响应命令中的SN值应使用请求命令中的SN值。--> 
    <SN>1708</SN>
    <! -- 目标设备的设备编码(必选)-->
    <DeviceID>62010201001320000393</DeviceID>
</Query>
```

##### 目录检索响应格式

```xml
<?xml version="1.0" encoding="GB2312"?>
<Response>
    <! -- 命令类型:设备目录查询(必选)-->
    <CmdType>Catalog</CmdType>
    <SN>1708</SN>
    <! -- 目标设备/区域/系统的编码,取值与目录查询请求相同(必选)-->
    <DeviceID>62010201001320000393</DeviceID>
    <! -- 查询结果总数(必选)-->
    <SumNum>1</SumNum>
    <! -- 设备目录项列表,Num 表示目录项个数，注意双引号-->
    <DeviceList Num="1">
        <Item>
            <! -- 设备/区域/系统编码(必选)-->
            <DeviceID>62010201001320000393</DeviceID>
            <! -- 设备/区域/系统名称(必选)-->
            <Name>Camera 01</Name>
            <! -- 当为设备时,设备厂商(必选)-->
            <Manufacturer>Hikvision</Manufacturer>
            <! -- 当为设备时,设备型号(必选)-->
            <Model>IP Camera</Model>
            <! -- 当为设备时,设备归属(必选)-->
            <Owner>Owner</Owner>
            <! -- 行政区域(必选)-->
            <CivilCode>5100000000</CivilCode>
            <! -- 当为设备时,安装地址(必选)-->
            <Address>Address</Address>
            <! -- 当为设备时,是否有子设备(必选)1有,0没有-->
            <Parental>0</Parental>
            <! -- 父设备/区域/系统ID(必选)-->
            <ParentID>51000000002000000001</ParentID>
            <! -- 信令安全模式(可选)缺省为0; 0:不采用;2:S/MIME 签名方式;3:S/ MIME加密签名同时采用方式;4:数字摘要方式-->
            <SafetyWay>0</SafetyWay>
            <! -- 注册方式(必选)缺省为1;1:符合IETFRFC3261标准的认证注册模 式;2:基于口令的双向认证注册模式;3:基于数字证书的双向认证注册模式-->
            <RegisterWay>1</RegisterWay>
            <! -- 保密属性(必选)缺省为0;0:不涉密,1:涉密-->
            <Secrecy>0</Secrecy>
            <! -- 设备状态(必选)-->
            <Status>ON</Status>
            ... 其他请参考协议文档
        </Item>
    </DeviceList>
</Response>
```



#### 设备信息

​		包括设备ID等信息。

##### 设备信息查询请求格式

```xml
<?xml version="1.0" encoding="GB2312"?>
<Query>
    <! -- 命令类型:设备信息查询(必选)-->
    <CmdType>DeviceInfo</CmdType>
    <! -- 命令序列号(必选)，消息中的SN值用于与请求命令的匹配处理,响应命令中的SN值应使用请求命令中的SN值。--> 
    <SN>1708</SN>
    <! -- 目标设备的设备编码(必选)-->
    <DeviceID>62010201001320000393</DeviceID>
</Query>
```



##### 设备信息查询响应格式

```xml
<?xml version="1.0" encoding="GB2312"?>
<Response>
    <! -- 命令类型:设备目录查询(必选)-->
    <CmdType>Catalog</CmdType>
    <SN>1708</SN>

    <! -- 设备/区域/系统编码(必选)-->
    <DeviceID>62010201001320000393</DeviceID>
    <! -- 设备/区域/系统名称(必选)-->
    <Name>Camera 01</Name>
    <! -- 当为设备时,设备厂商(必选)-->
    <Manufacturer>Hikvision</Manufacturer>
    <! -- 当为设备时,设备型号(必选)-->
    <Model>IP Camera</Model>
    <! -- 当为设备时,设备归属(必选)-->
    <Owner>Owner</Owner>
    <! -- 行政区域(必选)-->
    <CivilCode>5100000000</CivilCode>
    <! -- 当为设备时,安装地址(必选)-->
    <Address>Address</Address>
    <! -- 当为设备时,是否有子设备(必选)1有,0没有-->
    <Parental>0</Parental>
    <! -- 父设备/区域/系统ID(必选)-->
    <ParentID>51000000002000000001</ParentID>
    <! -- 信令安全模式(可选)缺省为0; 0:不采用;2:S/MIME 签名方式;3:S/ MIME加密签名同时采用方式;4:数字摘要方式-->
    <SafetyWay>0</SafetyWay>
    <! -- 注册方式(必选)缺省为1;1:符合IETFRFC3261标准的认证注册模 式;2:基于口令的双向认证注册模式;3:基于数字证书的双向认证注册模式-->
    <RegisterWay>1</RegisterWay>
    <! -- 保密属性(必选)缺省为0;0:不涉密,1:涉密-->
    <Secrecy>0</Secrecy>
    <! -- 设备状态(必选)-->
    <Status>ON</Status>
    ... 其他请参考协议文档

</Response>
```



#### 设备状态

##### 设备状态请求格式

```xml
<?xml version="1.0" encoding="GB2312"?>
<Query>
    <! -- 命令类型:设备信息查询(必选)-->
    <CmdType>DeviceStatus</CmdType>
    <! -- 命令序列号(必选)，消息中的SN值用于与请求命令的匹配处理,响应命令中的SN值应使用请求命令中的SN值。--> 
    <SN>1708</SN>
    <! -- 目标设备的设备编码(必选)-->
    <DeviceID>62010201001320000393</DeviceID>
</Query>
```



##### 设备状态响应格式

```xml
<?xml version="1.0" encoding="GB2312"?>
<Response>
    <! -- 命令类型:设备目录查询(必选)-->
    <CmdType>Catalog</CmdType>
    <SN>1708</SN>

    <DeviceID>xxx</DeviceID>
	<Result>OK</Result>
	<Online>ONLINE</Online>
	<Status>OK</Status>
	<DeviceTime>2023-09-15T21:30:15</DeviceTime>
	<Alarmstatus Num="0">
	</Alarmstatus>
	<Encode>ON</Encode>
	<Record>OFF</Record>

</Response>
```





### 设备控制

​		`GB28181`的设备控制应满足以下基本要求:

- 源设备向目标设备发送控制命令

  > 摄像机云台控制、远程启动、录像控制报警布防/撤防、报警复位、强制关键顿、拉框放大、拉框缩小、看守位控制、PTZ 精准控制、存储卡格式化、目标跟踪、软件升级、设备配置等

  > 源设备向目标设备发送控制命令后，目标设备不需要发送应答命令的控制命令包括：
  >
  > - 摄像机云台控制
  > - 远程启动
  > - 强制关键帧
  > - 拉框放大/缩小
  > - PTZ精准控制
  > - 存储卡格式化
  > - 目标跟踪

  > 源设备向目标设备发送控制命令后，目标设备需要发送应答命令的控制命令包括：
  >
  > - 录像控制
  > - 报警布防/撤防
  > - 报警复位
  > - 看守位控制
  > - 软件升级
  > - 设备配置

- 设备配置的内容

  > 基本参数、视频参数范围、SVAC 编码、SVAC 解码视频参数属性、录像计划、报警录像、视频画面遮挡、画面翻转、报警上报开关、前端 OSD、图像抓拍配置

- 控制应采用 `IETF RFC3428` 中的 `MESSAGE `方法实现;

- 源设备向目标设备发送摄像机云台控制、远程启动、强制关键帧、拉框放大、拉框缩小、PTZ 精准控制、存储卡格式化、目标跟踪命令后,目标设备不发送应答命令

- 源设备向目标设备发送录像控制、报警布防/撤防、报警复位、看守位控制、软件升级、设备配置命令后

  - 目标设备应发送应答命令表示执行的结果

- 源设备包括 SIP 客户端网关或者联网系统，目标设备包括 SIP 设备网关或者联网系统



#### 无应答

![img](https://i0.hdslb.com/bfs/new_dyn/b3dd3c7f06c9000d8f6dfb0c85bcb11a407536888.png@1256w_632h_!web-article-pic.webp)

​		控制命令流程描述如下：

- 1:源设备向`SIP  `服务器发送设备控制命令
  - 设备控制命令采用 `MESSAGE  `方法携带
- 2:SIP服务器收到命令后返回`200 OK`
- 3:SIP服务器向目标设备发送设备控制命令
  - 设备控制命令采用` MESSAGE  `方法携带
- 4:目标设备收到命令后返回`200 OK`



#### 有应答

![sip监控域是什么 监控区域什么意思_信令_16](https://raw.githubusercontent.com/Mocearan/picgo-server/main/20192525_65fac7a5e3e1675497.png)

​		请求和响应是通过XML报文中的SN序号进行标识的



#### 控制定义

##### 指令格式

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/84cfdda2f898d7a69d275a046e955f5a.png)

##### PTZ

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/8116e62d695665962c182d2dc6397746.png)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/6d705eef61a6e7b0efb915c116176d61.png)



##### FI

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/e048645a534862aba045dbb6242dc802.png)

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/85b4bfc1d1a746bcfc415918f19ec6af.png)



##### 控制格式



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Control>
    <CmdType>DeviceControl</CmdType>
    <SN>130</SN>
    <DeviceID>xxx</DeviceID>
    <PTZCmd>A50F010800FA00B7</PTZCmd>
</Control>

A50F010800FA00B7   	上
A50F010400FA00B3   	下
A50F010400FA00B3	左
A50F0101FA0000B0	右
A50F010AFAFA00B3	左上
A50F0106FAFA00AF	左下
A50F0109FAFA00B2	右上
A50F0105FAFA00AE	右下
A50F0100000000B5	停止
A50F01100000A065	镜头放大
A50F01200000A075	镜头缩小
A50F014400FA00F3	光圈放大
A50F014800FA00F7	光圈缩小
A50F0141FA0000F0	镜头聚焦
A50F0142FA0000F1	镜头放焦
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Control>
    <CmdType>DeviceControl</CmdType>
    <SN>130</SN>
    <DeviceID>xxx</DeviceID>
    <Result>...</Result>
</Control>
```



```c++
enum PTZControlType
{
	PTZ_CTRL_HALT = 0,			// 停止
	PTZ_CTRL_RIGHT,		        // 右转
	PTZ_CTRL_RIGHTUP,		    // 右上
	PTZ_CTRL_UP,		        // 上转
	PTZ_CTRL_LEFTUP,		    // 左上
	PTZ_CTRL_LEFT,		        // 左转
	PTZ_CTRL_LEFTDOWN,		    // 左下
	PTZ_CTRL_DOWN,		        // 下转
	PTZ_CTRL_RIGHTDOWN,		    // 右下
	PTZ_CTRL_ZOOM,              // 镜头放大/缩小
	PTZ_CTRL_IRIS,              // 光圈放大/缩小
	PTZ_CTRL_FOCUS,             // 镜头聚焦/放焦
};

std::string CMySipContext::ParsePTZCmd(CMyGBDevice* device, const std::string& gbid, PTZControlType ptzType, int paramValue)
{
	unsigned char ptzCmdStr[8] = { 0xA5, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
	PTZControlType type = ptzType;
	if (0 == paramValue)
		type = PTZ_CTRL_HALT;

	switch (type)
	{
	case PTZ_CTRL_HALT:
		break;
	case PTZ_CTRL_RIGHT:     // 右
		ptzCmdStr[3] = 0x01;
		ptzCmdStr[4] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_RIGHTUP:   // 右上
		ptzCmdStr[3] = 0x09;
		ptzCmdStr[4] = paramValue & 0xFF;
		ptzCmdStr[5] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_UP:        // 上
		ptzCmdStr[3] = 0x08;
		ptzCmdStr[5] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_LEFTUP:    // 左上
		ptzCmdStr[3] = 0x0A;
		ptzCmdStr[4] = paramValue & 0xFF;
		ptzCmdStr[5] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_LEFT:      // 左
		ptzCmdStr[3] = 0x02;
		ptzCmdStr[4] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_LEFTDOWN:  // 左下
		ptzCmdStr[3] = 0x06;
		ptzCmdStr[4] = paramValue & 0xFF;
		ptzCmdStr[5] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_DOWN:      // 下
		ptzCmdStr[3] = 0x04;
		ptzCmdStr[5] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_RIGHTDOWN: // 右下
		ptzCmdStr[3] = 0x05;
		ptzCmdStr[4] = paramValue & 0xFF;
		ptzCmdStr[5] = paramValue & 0xFF;
		break;
	case PTZ_CTRL_ZOOM:
		if (paramValue > 0)
		{
			ptzCmdStr[3] = 0x10;
			ptzCmdStr[6] = (paramValue & 0x0F) << 4;
		}
		else if (paramValue < 0)
		{
			ptzCmdStr[3] = 0x20;
			ptzCmdStr[6] = ((-paramValue) & 0x0F) << 4;
		}
		break;
	case PTZ_CTRL_IRIS:
		if (paramValue > 0)
		{
			ptzCmdStr[3] = 0x44;
			ptzCmdStr[5] = paramValue & 0xFF;
		}
		else if (paramValue < 0)
		{
			ptzCmdStr[3] = 0x48;
			ptzCmdStr[5] = (-paramValue) & 0xFF;
		}
		break;
	case PTZ_CTRL_FOCUS:
		if (paramValue > 0)
		{
			ptzCmdStr[3] = 0x41;
			ptzCmdStr[4] = paramValue & 0xFF;
		}
		else if (paramValue < 0)
		{
			ptzCmdStr[3] = 0x42;
			ptzCmdStr[4] = (-paramValue) & 0xFF;
		}
		break;
	default:
		break;
	}

	std::string cmdstr;
	char tmp[8] = { 0, };
	for (int i = 0; i < 7; i++)
	{
		ptzCmdStr[7] += ptzCmdStr[i];
		sprintf(tmp, "%02X", ptzCmdStr[i]);
		cmdstr += tmp;
	}
	sprintf(tmp, "%02X", ptzCmdStr[7]);
	cmdstr += tmp;

	char szPTZInfo[200] = { 0 };
	pj_ansi_snprintf(szPTZInfo, 200,
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<Control>\n"
		"<CmdType>DeviceControl</CmdType>\n"
		"<SN>130</SN>\n"
		"<DeviceID>%s</DeviceID>\n"
		"<PTZCmd>%s</PTZCmd>\n"
		"</Control>\n", gbid.c_str(), cmdstr.c_str()
	);

	return szPTZInfo;
}

```





#### 配置定义





### 媒体传输方式

​		基于GB/T28181-2022版本，实时流的传输方式包括3种：

- UDP
- TCP被动
- TCP主动

```http
INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107:5060;tag=xxx
To: sip:xxx@192.168.0.111:5060
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 18467 INVITE
Allow: INVITE, ACK, BYE, CANCEL, UPDATE, PRACK
Supported: 100rel
Subject: xxx:xx,xxx:xx
Content-Type: application/sdp
Content-Length:   211

v=0
o=xxx 0 0 IN IP4 192.168.0.107
s=Play
c=IN IP4 192.168.0.107
t=0 0
m=video 36000 RTP/AVP 96 98 97   # 36000为SIP服务数据接收端口
a=recvonly
a=rtpmap:96 PS/90000
a=rtpmap:98 H264/90000
a=rtpmap:97 MPEG4/90000
y=0100000001
```



#### UDP

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/493e597be7e5fe4696a1e3e3df317b0e.jpeg)

- **m字段指定传输方式为RTP/AVP；**

SIP服务器发送INVITE请求；

```http
INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107:5060;tag=xxx
To: sip:xxx@192.168.0.111:5060
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 18467 INVITE
Allow: INVITE, ACK, BYE, CANCEL, UPDATE, PRACK
Supported: 100rel
Subject: xxx:xx,xxx:xx
Content-Type: application/sdp
Content-Length:   211

v=0
o=xxx 0 0 IN IP4 192.168.0.107
s=Play
c=IN IP4 192.168.0.107
t=0 0
m=video 36000 RTP/AVP 96 98 97   # 36000为SIP服务数据接收端口
a=recvonly
a=rtpmap:96 PS/90000
a=rtpmap:98 H264/90000
a=rtpmap:97 MPEG4/90000
y=0100000001
```

设备返回200 OK；

```http
SIP/2.0 200 OK
Call-ID: xxx
Contact: <sip:sss@192.168.0.111:5060>
Content-Length: 226
Content-Type: application/sdp
CSeq: 18467 INVITE
From: <sip:xxx@192.168.0.107:5060>;tag=xxx
To: <sip:xxx@192.168.0.111:5060>;tag=xxx
User-Agent: SIP UAS V.2016.xxxx
Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx

v=0
o=34020000001310000002 0 0 IN IP4 192.168.0.111
s=Play
i=VCam Live Video
c=IN IP4 192.168.0.111
t=0 0
m=video 9712 RTP/AVP 96     # 9712为设备端数据发送端口
a=sendonly
a=rtpmap:96 PS/90000
a=streamprofile:0
y=0100000001
f=v/0/0/0/0/0a/0/0/0
```





#### TCP被动

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/4ad3796db27874a1aa80f58470ad11bd.jpeg)

- **m字段指定传输方式为TCP/RTP/AVP；**
- **sdp信息中增加"a=setup:passive"；**
- **SIP服务器启动端口监听，设备发起tcp连接请求；**



SIP服务器发送INVITE请求；

```http
INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107:5060;tag=xxx
To: sip:xxx@192.168.0.111:5060
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 18480 INVITE
Allow: INVITE, ACK, BYE, CANCEL, UPDATE, PRACK
Supported: 100rel
Subject: xxx:xx,xxx:xx
Content-Type: application/sdp
Content-Length:   211

v=0
o=xxx 0 0 IN IP4 192.168.0.107
s=Play
c=IN IP4 192.168.0.107
t=0 0
m=video 36000 TCP/RTP/AVP 96 98 97 
a=recvonly
a=rtpmap:96 PS/90000
a=rtpmap:98 H264/90000
a=rtpmap:97 MPEG4/90000
a=setup:passive
y=0100000001

```

设备返回200 ok

```http
SIP/2.0 200 OK
Call-ID: xxx
Contact: <sip:xxx@192.168.0.111:5060>
Content-Length: 264
Content-Type: application/sdp
CSeq: 18480 INVITE
From: <sip:xxx@192.168.0.107>;tag=xxx
To: <sip:xxx@192.168.0.111>;tag=xxx
User-Agent: SIP UAS V.2016.xxxx
Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx

v=0
o=xxx 0 0 IN IP4 192.168.0.111
s=Play
i=VCam Live Video
c=IN IP4 192.168.0.111
t=0 0
m=video 9724 TCP/RTP/AVP 96    # 9724为设备端数据发送端口
a=sendonly
a=rtpmap:96 PS/90000           # 流数据为PS封装
a=streamprofile:0
a=setup:active                 # 设备为主动发送方
a=connection:new
y=0100000001
f=v/0/0/0/0/0a/0/0/0
```





#### TCP主动

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/68adcde9639d7e4af487de609adecf90.jpeg)

- **m字段指定传输方式为TCP/RTP/AVP；**
- **sdp信息中增加"a=setup:active"；**
- 设备返回200 OK，报文的SDP信息中包含tcp监听端口；
- SIP服务器根据设备监听端口发起TCP连接请求；

SIP服务器发送INVITE请求；

```http
INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107:5060;tag=xxx
To: sip:xxx@192.168.0.111:5060
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 19100 INVITE
Allow: INVITE, ACK, BYE, CANCEL, UPDATE, PRACK
Supported: 100rel
Subject: xxx:xx,xxx:xx
Content-Type: application/sdp
Content-Length:   211

v=0
o=xxx 0 0 IN IP4 192.168.0.107
s=Play
c=IN IP4 192.168.0.107
t=0 0
m=video 38000 TCP/RTP/AVP 96 98 97 
a=recvonly
a=rtpmap:96 PS/90000
a=rtpmap:98 H264/90000
a=rtpmap:97 MPEG4/90000
a=setup:active
y=0100000001
```

设备返回200 OK

```http
SIP/2.0 200 OK
Call-ID: xxx
Contact: <sip:xxx@192.168.0.111:5060>
Content-Length: 264
Content-Type: application/sdp
CSeq: 19100 INVITE
From: <sip:xxx@192.168.0.107>;tag=xxx
To: <sip:xxx@192.168.0.111>;tag=xxx
User-Agent: SIP UAS V.2016.xxxx
Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx

v=0
o=xxx 0 0 IN IP4 192.168.0.111
s=Play
i=VCam Live Video
c=IN IP4 192.168.0.111
t=0 0
m=video 9704 TCP/RTP/AVP 96    # 9704为设备端监听
a=sendonly
a=rtpmap:96 PS/90000           # 流数据为PS封装
a=streamprofile:0
a=setup:passive                # 设备为被动接收方
a=connection:new
y=0100000001
f=v/0/0/0/0/0a/0/0/0
```



#### jrtp收流端

##### tcp被动收流

```c++
// TcpServer为封装的socket类

int CGBTcpServerStreamReceiver::Start()
{
	if (m_localIP.empty() || m_localPort <= 0)
		return -1;

	if (m_tcpServer.get())
		return 0;

	m_tcpServer = std::make_shared<TcpServer>(TcpDataCB, this);
	if (!m_tcpServer.get())
		return -1;

	if (0 != m_tcpServer->TcpCreate() 
		|| 0 != m_tcpServer->TcpBind(m_localPort) 
		|| 0 != m_tcpServer->TcpListen(5))
		return -1;

	m_thread = std::thread(TcpDataThread, this);
	return 0;
}
```

```c++
// 在线程内等待连接，连接成功后接收数据并回调至应用层处理
void CGBTcpServerStreamReceiver::TcpDataWorker()
{
	bool bAccept = false;
	uint8_t payload;
	while (m_running)
	{
		if (!bAccept)
		{
            // 等待设备连接
			if (0 == m_tcpServer->TcpAccept())
			{
				bAccept = true;
                // 连接成功后,初始化rtp参数
				if (0 != InitRtpSession_())
				{
					break;
				}
			}

			continue;
		}

		Poll();
		BeginDataAccess();

        // 开始接收数据
		if (GotoFirstSourceWithData())
		{
			do
			{
				RTPPacket* packet = nullptr;
				while (nullptr != (packet = GetNextPacket()))
				{
					payload = packet->GetPayloadType();
					if (0 == payload)
					{
						DeletePacket(packet);
						continue;
					}

					struct rtp_packet_tcp data;
					data.mark = packet->HasMarker();
					data.pts = packet->GetTimestamp();
					data.seq = packet->GetSequenceNumber();
					data.data = packet->GetPayloadData();
					data.len = (int)packet->GetPayloadLength();

					m_payload = payload;

					if (m_lastSeq < 0)
					{
						m_lastSeq = data.seq - 1;
					}

					if (m_lastSeq = data.seq - 1)
					{
						PackData_(data.data, data.len);
					}

					DeletePacket(packet);
				}

			} while (GotoNextSourceWithData());
		}

		EndDataAccess();
		Sleep(30);
	}

	Destroy();
}
```

```c++
int CGBTcpServerStreamReceiver::InitRtpSession_()
{
	const int packSize = 45678;
	RTPSessionParams sessionParams;
	sessionParams.SetProbationType(RTPSources::NoProbation);
	sessionParams.SetOwnTimestampUnit(90000.0 / 25.0);
	sessionParams.SetMaximumPacketSize(packSize + 64);

	m_rtpTcpTransmitter = new RTPTCPTransmitter(nullptr);
	m_rtpTcpTransmitter->Init(true);
	m_rtpTcpTransmitter->Create(65535, 0);

	if (0 != Create(sessionParams, m_rtpTcpTransmitter))
		return -1;

	if (0 != AddDestination(RTPTCPAddress(m_tcpServer->GetClientSocket())))
		return -1;

	return 0;
}

```





##### tcp主动收流

```c++
// TcpClient为封装的客户端socket类

int CGBTcpClientStreamReceiver::Start(int streamType)
{
	if (m_localIP.empty() || m_localPort <= 0)
		return -1;

	if (m_tcpClient.get())
		return 0;

    // 创建socket
	m_tcpClient = std::make_shared<TcpClient>(TcpDataCB, this);
	if (!m_tcpClient.get() || 0 != m_tcpClient->TcpCreate())
		return -1;

	// connect
	int ret = m_tcpClient->TcpConnectByTime(m_localIP.c_str(), m_localPort, 5);
	if (0 != ret)
		return -1;

    // 初始化rtp session
	if (0 != InitRtpSession_())
		return -1;

    // 启动接收线程
	m_thread = std::thread(TcpDataThread, this);
}

```

```c++
// 初始化rtp参数
int CGBTcpClientStreamReceiver::InitRtpSession_()
{
	const int packSize = 45678;
	RTPSessionParams sessionParams;
	sessionParams.SetProbationType(RTPSources::NoProbation);
	sessionParams.SetOwnTimestampUnit(90000.0 / 25.0);
	sessionParams.SetMaximumPacketSize(packSize + 64);

	m_rtpTcpTransmitter = new RTPTCPTransmitter(nullptr);
	m_rtpTcpTransmitter->Init(true);
	m_rtpTcpTransmitter->Create(65535, 0);

	if (0 != Create(sessionParams, m_rtpTcpTransmitter))
		return -1;

    // 添加客户端socket
	if (0 != AddDestination(RTPTCPAddress(m_tcpClient->GetClientSocket())))
		return -1;

	return 0;
}

```

```c++
//在线程内等待连接，连接成功后接收数据并回调至应用层处理
//同tcp passive收流流程。
```





##### 收包解包

```c++
// 打开文件，创建解码器
void RtpUnpackDlg::StartRtpUnpack()
{
	if (m_thread.joinable())
		return;

	QString filePath = ui.le_filePath->text();
	if (filePath.isEmpty())
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("文件路径为空"), QMessageBox::Ok);
		return;
	}

	m_rtpPayloadParam.payload = 100;
	m_rtpPayloadParam.encoding = "PS";
	m_rtpPayloadParam.frtp = fopen(filePath.toStdString().c_str(), "rb");
	if (!m_rtpPayloadParam.frtp)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("打开输入文件失败"), QMessageBox::Ok);
		return;
	}

	m_rtpPayloadParam.fout = fopen(outFilePath.toStdString().c_str(), "wb");
	if (!m_rtpPayloadParam.fout)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("打开输出文件失败"), QMessageBox::Ok);
		return;
	}

	struct rtp_payload_t handler;
	handler.alloc = RtpPacketAlloc;
	handler.free = RtpPacketFree;
	handler.packet = RtpDecodePacket;
	m_rtpPayloadParam.decoder = rtp_payload_decode_create(100, "PS", &handler, this);

	m_thread = std::thread(DataReadThread, this);
}

```

```c++
//读数据
void RtpUnpackDlg::RtpDataReadWorker()
{
	while (m_running)
	{
		// 先读两个字节的头
		unsigned char s2[2];
		if (2 != fread(s2, 1, 2, m_rtpPayloadParam.frtp))
			break;

		m_rtpPayloadParam.size = (s2[0] << 8) | s2[1];
		assert(ctx.size < sizeof(ctx.packet));
		if (m_rtpPayloadParam.size != (int)fread(m_rtpPayloadParam.packet, 1, m_rtpPayloadParam.size, m_rtpPayloadParam.frtp))
			break;

        // 塞数据
		if (m_rtpPayloadParam.packet[1] < RTCP_FIR || m_rtpPayloadParam.packet[1] > RTCP_LIMIT)
			rtp_payload_decode_input(m_rtpPayloadParam.decoder, m_rtpPayloadParam.packet, m_rtpPayloadParam.size);
	}

	fclose(m_rtpPayloadParam.frtp);
	fclose(m_rtpPayloadParam.fout);
}

```

```c++
// 解包
int RtpUnpackDlg::DecodePacket(const void* packet, int bytes, uint32_t timestamp, int flags)
{
	static const unsigned char start_code[4] = { 0, 0, 0, 1 };
	static unsigned char buffer[2 * 1024 * 1024] = {0, };

	size_t size = 0;
	if (0 == strcmp("H264", m_rtpPayloadParam.encoding) 
		|| 0 == strcmp("H265", m_rtpPayloadParam.encoding)
		|| 0 == strcmp("PS", m_rtpPayloadParam.encoding))
	{
		memcpy(buffer, start_code, sizeof(start_code));
		size += sizeof(start_code);
	}

	memcpy(buffer + size, packet, bytes);
	size += bytes;

	fwrite(buffer, 1, size, m_rtpPayloadParam.fout);

    // 新增界面播放功能
	if (m_playWidget)
		m_playWidget->AddData(CODEC_VIDEO_H264, (void*)buffer, size);

	return 0;
}

```



#### jrtp发流端

​		下级平台或设备发流，对接特定SIP服务器或上级平台。

##### UDP发流

- 初始化rtp参数；
- 裸流数据做PS复用；
- 组RTP包发送；

```c++
// 初始化rtp参数
int CUdp::InitRtp_()
{
	RTPSessionParams sessionParams;
	sessionParams.SetMinimumRTCPTransmissionInterval(10);
	sessionParams.SetOwnTimestampUnit(1.0 / 90000.0);
	sessionParams.SetAcceptOwnPackets(true);
	sessionParams.SetMaximumPacketSize(1450);

	RTPUDPv4TransmissionParams transParams;
	transParams.SetRTPSendBuffer(2*1024*1024);
	transParams.SetBindIP(m_ip);
	transParams.SetPortbase((uint16_t)m_port);

	if (0 != Create(sessionParams, &transParams))
	{
		return -1;
	}

	SetDefaultPayloadType((uint8_t)m_payload);
	SetDefaultTimestampIncrement(3600);
	SetDefaultMark(true);

	RTPIPv4Address addr(ntohl(inet_addr(m_ip), (uint16_t)m_port);
	if(0 != AddDestination(addr))
    {
		return -1;
	}

	return 0;
}

```

```c++
//流数据复用为PS
// 使用ireader开源库进行ps复用
// 初始化
CData2PS::CData2PS()
{
	struct ps_muxer_func_t func;
	func.alloc = Alloc;
	func.free = Free;
	func.write = Packet;
	m_ps = ps_muxer_create(&func, this);
	// TODO codecid待补充
	m_ps_stream = ps_muxer_add_stream(m_ps, PSI_STREAM_H264, nullptr, 0);
}


// 塞数据
int CData2PS::InputData(void* data, int len)
{
	if (!m_ps)
		return -1;

	uint64_t clock = time64_now();
	if (0 == m_ps_clock)
		m_ps_clock = clock;

	return ps_muxer_input(m_ps, m_ps_stream, 0, (clock - m_ps_clock) * 90, (clock - m_ps_clock) * 90, data, len);
}

```

```c++
//发送rtp包
// 调用jrtplib中SendPacket(data, len);接口发送数据

// 以下为SendPacket部分源码
// 主要流程:
// 1. 构建packet
// 2. 发送rtp数据
int RTPSession::SendPacket(const void *data,size_t len,
                uint8_t pt,bool mark,uint32_t timestampinc)
{
	int status;

	if (!created)
		return ERR_RTP_SESSION_NOTCREATED;
	
	BUILDER_LOCK
	if ((status = packetbuilder.BuildPacket(data,len,pt,mark,timestampinc)) < 0)
	{
		BUILDER_UNLOCK
		return status;
	}
	if ((status = SendRTPData(packetbuilder.GetPacket(),packetbuilder.GetPacketLength())) < 0)
	{
		BUILDER_UNLOCK
		return status;
	}
	BUILDER_UNLOCK
	
	SOURCES_LOCK
	sources.SentRTPPacket();
	SOURCES_UNLOCK
	PACKSENT_LOCK
	sentpackets = true;
	PACKSENT_UNLOCK
	return 0;
}

// 构建包
int RTPPacketBuilder::PrivateBuildPacket(const void *data,size_t len,
	                  uint8_t pt,bool mark,uint32_t timestampinc,bool gotextension,
	                  uint16_t hdrextID,const void *hdrextdata,size_t numhdrextwords)
{
	RTPPacket p(pt,data,len,seqnr,timestamp,ssrc,mark,numcsrcs,csrcs,gotextension,hdrextID,
	            (uint16_t)numhdrextwords,hdrextdata,buffer,maxpacksize,GetMemoryManager());
	int status = p.GetCreationError();

	if (status < 0)
		return status;
	packetlength = p.GetPacketLength();

	if (numpackets == 0) // first packet
	{
		lastwallclocktime = RTPTime::CurrentTime();
		lastrtptimestamp = timestamp;
		prevrtptimestamp = timestamp;
	}
	else if (timestamp != prevrtptimestamp)
	{
		lastwallclocktime = RTPTime::CurrentTime();
		lastrtptimestamp = timestamp;
		prevrtptimestamp = timestamp;
	}
	
	numpayloadbytes += (uint32_t)p.GetPayloadLength();
	numpackets++;
	timestamp += timestampinc;
	seqnr++;

	return 0;
}

// 发送包
int RTPSession::SendRTPData(const void *data, size_t len)
{
	if (!m_changeOutgoingData)
		return rtptrans->SendRTPData(data, len);

	void *pSendData = 0;
	size_t sendLen = 0;
	int status = 0;

	status = OnChangeRTPOrRTCPData(data, len, true, &pSendData, &sendLen);
	if (status < 0)
		return status;

	if (pSendData)
	{
		status = rtptrans->SendRTPData(pSendData, sendLen);
		OnSentRTPOrRTCPData(pSendData, sendLen, true);
	}

	return status;
}

// 底层实现
int RTPUDPv4Transmitter::SendRTPData(const void *data,size_t len)	
{
	if (!init)
		return ERR_RTP_UDPV4TRANS_NOTINIT;

	MAINMUTEX_LOCK
	
	if (!created)
	{
		MAINMUTEX_UNLOCK
		return ERR_RTP_UDPV4TRANS_NOTCREATED;
	}
	if (len > maxpacksize)
	{
		MAINMUTEX_UNLOCK
		return ERR_RTP_UDPV4TRANS_SPECIFIEDSIZETOOBIG;
	}
	
	destinations.GotoFirstElement();
	while (destinations.HasCurrentElement())
	{
        // 调用sendto函数实现udp包的发送
		sendto(rtpsock,(const char *)data,len,0,(const struct sockaddr *)destinations.GetCurrentElement().GetRTPSockAddr(),sizeof(struct sockaddr_in));
		destinations.GotoNextElement();
	}
	
	MAINMUTEX_UNLOCK
	return 0;
}

```





##### tcp被动发流

- 上级平台或sip服务器以主动方式连接，对于下级平台或者设备（数据发送端）为被动方式；
- 下级平台或者设备（数据发送端）启动端口监听；
- 接收上级平台或sip服务器tcp连接请求；
- 向上级平台或sip服务器发送流数据；

```c++
// 创建socket、bind、listen，启动数据接收线程；
// TcpServer为封装的socket类

int CGBTcpServer::Start()
{
	if (0 != m_localPort || m_tcpServer.get())
		return 0;

	int ret = -1;
	do 
	{
		m_tcpServer = std::make_shared<TcpServer>(nullptr, this);
		if (!m_tcpServer.get())
			break;

		ret = m_tcpServer->TcpCreate();
		if (0 != ret)
			break;

		ret = m_tcpServer->TcpBind(m_localPort);
		if (0 != ret)
			break;

		ret = m_tcpServer->TcpListen(5);
		if (0 != ret)
			break;

		m_thread = std::thread(TCPData2PSThread, this);
		return 0;
	} while (0);

	Stop();
	return ret;
}

```

```c++
//在线程内等待连接，连接成功后接收数据并回调至应用层处理
void CGBTcpServer::TCPData2PSWorker()
{
	if (!m_pspacker)
		m_pspacker = new(std::nothrow) CData2PS(PSTCPDataCB, this);

	bool bAccept = false;
	while (m_running)
	{
		if (!bAccept)
		{
			if (0 == m_tcpServer->TcpAccept())
			{
				bAccept = true;
				if (0 != InitRtp_())
				{
					break;
				}
			}

			continue;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

```

```c++
//初始化rtp参数
int CGBTcpServer::InitRtp_()
{
	const int packetSize = 45678;
	RTPSessionParams sessionparams;
	sessionparams.SetProbationType(RTPSources::NoProbation);
	sessionparams.SetOwnTimestampUnit(1.0 / packetSize);
	sessionparams.SetMaximumPacketSize(packetSize + 64);

	m_rtpTcpTransmitter = new RTPTCPTransmitter(nullptr);
	m_rtpTcpTransmitter->Init(true);
	m_rtpTcpTransmitter->Create(65535, 0);

	int status = Create(sessionparams, m_rtpTcpTransmitter);
	if (status < 0)
	{
		return status;
	}

	status = AddDestination(RTPTCPAddress(m_tcpServer->GetClientSocket()));
	if (0 != status)
		return status;

	SetDefaultPayloadType(96);
	SetDefaultMark(false);
	SetDefaultTimestampIncrement(160);
	return 0;
}

```

```c++
//4将数据封装为PS；
//5tcp方式发包
// 调用jrtplib中SendPacket(data, len);接口发送数据

// 以下为tcp方式SendPacket部分源码
int RTPTCPTransmitter::SendRTPData(const void *data,size_t len)	
{
	return SendRTPRTCPData(data, len);
}

int RTPTCPTransmitter::SendRTPRTCPData(const void *data, size_t len)
{
	if (!m_init)
		return ERR_RTP_TCPTRANS_NOTINIT;

	MAINMUTEX_LOCK
	
	if (!m_created)
	{
		MAINMUTEX_UNLOCK
		return ERR_RTP_TCPTRANS_NOTCREATED;
	}
    
    // #define RTPTCPTRANS_MAXPACKSIZE							65535
	if (len > RTPTCPTRANS_MAXPACKSIZE)
	{
		MAINMUTEX_UNLOCK
		return ERR_RTP_TCPTRANS_SPECIFIEDSIZETOOBIG;
	}
	
	std::map<SocketType, SocketData>::iterator it = m_destSockets.begin();
	std::map<SocketType, SocketData>::iterator end = m_destSockets.end();

	vector<SocketType> errSockets;
	int flags = 0;
#ifdef RTP_HAVE_MSG_NOSIGNAL
	flags = MSG_NOSIGNAL;
#endif // RTP_HAVE_MSG_NOSIGNAL

	while (it != end)
	{
		uint8_t lengthBytes[2] = { (uint8_t)((len >> 8)&0xff), (uint8_t)(len&0xff) };
		SocketType sock = it->first;

        // 调用send接口发送数据
        // 1. 先发送2字节头(固定格式)
        // 2. 再发送数据
		if (send(sock,(const char *)lengthBytes,2,flags) < 0 ||
			send(sock,(const char *)data,len,flags) < 0)
			errSockets.push_back(sock);
		++it;
	}
	
	MAINMUTEX_UNLOCK

	if (errSockets.size() != 0)
	{
		for (size_t i = 0 ; i < errSockets.size() ; i++)
			OnSendError(errSockets[i]);
	}

	// Don't return an error code to avoid the poll thread exiting
	// due to one closed connection for example

	return 0;
}

```



##### tcp主动发流

- 上级平台或sip服务器启动tcp监听连接，对于下级平台或者设备（数据发送端）为主动方式；
- 下级平台或者设备（数据发送端）发起tcp连接；
- 接收上级平台或sip服务器tcp响应；
- 向上级平台或sip服务器发送流数据；

```c++
// 创建socket、connect、初始化rtp，启动数据接收线程
// TcpClient为封装的客户端socket类

int CGBTcpClient::Start()
{
	if (0 != m_localPort || m_tcpClient.get())
		return 0;

	int ret = -1;
	do
	{
		m_tcpClient = std::make_shared<TcpClient>(nullptr, this);
		if (!m_tcpClient.get() || 0 != m_tcpClient->TcpCreate())
			break;

		ret = m_tcpClient->TcpConnectByTime(m_localIP.c_str(), m_localPort, 5);
		if (0 != ret)
			break;

		ret = InitRtp_();
		if (0 != ret)
			break;

		m_thread = std::thread(RTPPackerThread, this);
		return 0;
	} while (0);

	Stop();
	return ret;
}

```

```c++
// 初始化rtp参数
int CGBTcpClient::InitRtp_()
{
	const int packSize = 45678;
	RTPSessionParams sessionParams;
	sessionParams.SetProbationType(RTPSources::NoProbation);
	sessionParams.SetOwnTimestampUnit(90000.0 / 25.0);
	sessionParams.SetMaximumPacketSize(packSize + 64);

	m_rtpTcpTransmitter = new RTPTCPTransmitter(nullptr);
	m_rtpTcpTransmitter->Init(true);
	m_rtpTcpTransmitter->Create(65535, 0);

	if (0 != Create(sessionParams, m_rtpTcpTransmitter))
		return -1;

	if (0 != AddDestination(RTPTCPAddress(m_tcpClient->GetClientSocket())))
		return -1;

	return 0;
}

```

```c++
//视音频数据复用为PS
// 发送数据，同tcp passive发流
```





### 实时预览

​		实时视音频点播采用SIP协议中的`INVITE`方法实现会话连接，采用`RTP/RTCP`协议(`IETF RFC3550`)实现媒体传输。

​		实时视音频点播需要上述的媒体流保活机制。

#### 客户端发起

​		`SIP`服务器采用`B2BUA`（背靠背用户代理）方式建立媒体服务器与接收者，媒体服务器与发送者之间的媒体流信令过程。

​		流程较为复杂，因为实际视频监控系统中，用户不直接跟前端监控设备交互，而是与监控管理平台交互。

- 媒体接收者，通常是用户的客户端
- SIP服务器，是单独的信令服务器，是监控管理平台
- 媒体服务器，通常是监控系统中的媒体网关
- 媒体流发送者，是前端的摄像机、录像机

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/v2-c9f0de1bcc6cb2fba6c5896e22adbd16_1440w.webp)

> - 信令`1、8、9、10、11、12`为 `SIP`服务器接收到客户端的呼叫请求后，通过 `B2BUA  `方式建立 媒体流接收者与媒体服务器之间的媒体流信令过程
>   - 由SIP服务器作为中继，将媒体流接收者和媒体服务器之间的信令代理转发，称为背靠背用户代理
> - 信令`2～7`为 `SIP`服务器通过三方呼叫控制，建立媒体服务器与媒体流发送者之间的媒体流信令过程
>   - 媒体服务器与媒体流发送者之间没有直接的信令交互，由SIP服务器作为第三方来控制呼叫
> - 信令`13～16 `为媒体流接收者断开与媒体服务器之间的媒体流信令过程
> - 信令`17～20 `为 `SIP  `服务器断开媒体服务器与媒体流发送者之间的媒体流信令过程

​		命令流程描述如下：

- 1: 媒体流接收者向SIP服务器发送 `Invite  `消息

  ```http
  INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=
  Max-Forwards: 70
  From: sip:xxx@192.168.0.107:5060;tag=
  To: sip:xxx@192.168.0.111:5060
  Contact: <sip:xxx@192.168.0.107:5060>
  Call-ID: 
  CSeq: 18467 INVITE
  Allow: INVITE, ACK, BYE, CANCEL, UPDATE, PRACK
  Supported: 100rel
  Subject: xxx:xx,xxx:xx
  Content-Type: application/sdp
  Content-Length:   211
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.107
  s=Play
  c=IN IP4 192.168.0.107
  t=0 0
  m=video 36000 RTP/AVP 96 98 97
  a=recvonly
  a=rtpmap:96 PS/90000
  a=rtpmap:98 H264/90000
  a=rtpmap:97 MPEG4/90000
  y=0100000001
  ```

  ```c++
  static std::string CreateRealStreamSDP(const GB28181MediaContext& mediaContext)
  {
      char str[500] = { 0 };
      pj_ansi_snprintf(str, 500,
                       "v=0\n"
                       "o=%s 0 0 IN IP4 %s\n"
                       "s=Play\n"
                       "c=IN IP4 %s\n"
                       "t=0 0\n"
                       "m=video %d RTP/AVP 96 98 97\n"
                       "a=recvonly\n"
                       "a=rtpmap:96 PS/90000\n"
                       "a=rtpmap:98 H264/90000\n"
                       "a=rtpmap:97 MPEG4/90000\n"
                       "y=001\n",
                       mediaContext.GetDeviceId().c_str(),
                       mediaContext.GetRecvAddress().c_str(),
                       mediaContext.GetRecvAddress().c_str(),
                       mediaContext.GetRecvPort()
                      );
      return str;
  }
  
  ```

  

--> `SIP`服务器通过三方呼叫控制，建立媒体服务器与媒体流发送者之间的媒体流信令过程

- 2:SIP服务器收到 `Invite  `请求，通过三方呼叫控制建立媒体服务器和媒体流发送者之间的媒体连接

  - 向媒体服务器发送 `Invite`  消息，此消息不携带 `SDP `消息体

- 3:媒体服务器以`200 OK  `响应`SIP `服务器的 `Invite  `请求

  - 携带 `SDP  `消息体，描述媒体服务器接收媒体流的IP、端口、媒体格式等

  ```http
  SIP/2.0 100 Trying
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=
  From: <sip:xxx@192.168.0.107:5060>;tag=
  To: <sip:xxx@192.168.0.111:5060>
  Call-ID: 
  CSeq: 18467 INVITE
  User-Agent: IP Camera
  Content-Length: 0
  ```

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=
  From: <sip:xxx@192.168.0.107:5060>;tag=
  To: <sip:xxx@192.168.0.111:5060>;tag=
  Call-ID: 
  CSeq: 18467 INVITE
  Contact: <sip:xxx@192.168.0.111:5060>
  Content-Type: application/sdp
  User-Agent: IP Camera
  Content-Length:   187
  
  v=0
  o=xxx 2243 2243 IN IP4 192.168.0.111
  s=Play
  c=IN IP4 192.168.0.111
  t=0 0
  m=video 15060 RTP/AVP 96
  a=sendonly
  a=rtpmap:96 PS/90000
  a=filesize:0
  y=0100000001
  ```

- 4:`SIP  `服务器向媒体流发送者发送 `Invite  `请求

  - 携带 消息`3 `中`200 OK `响应消息体
  - `s `字段为Play`”代表实时点播
  - 增加`y`字段描述 `SSRC `值 ，`f` 字段描述媒体参数

- 5:媒体流发送者以`200 OK  `响应SIP 服务器的 `Invite `请求

  - 携带 `SDP `消息体，描述了媒体流发送者发送媒体流的 IP、端口、媒体格式、SSRC 字段等内容

- 6:`SIP  `服务器向媒体服务器发送 `ACK `请求

  - 携带消息`5`中`200 OK  `响应消息体，完成与媒体服务器的 Invite  会话建立过程

- 7:`SIP` 服务器向媒体流发送者发送` ACK  `请求

  - 不携带消息体，完成`SIP`服务器与媒体流发送者的 `Invite `会话建立过程
  - 媒体流发送者向媒体服务器发送实时媒体流

---> 完成三方呼叫控制后，`SIP  `服务器通过`B2BUA  `代理方式建立媒体流接收者和媒体服务器之间的媒体连接。

-  8: `SIP`服务器向媒体服务器发送`Invate`请求

   - 携带消息1中`SDP`
   - 增加 `SSRC `值，转发给媒体服务器

-  9:媒体服务器回复`200 OK` 响应

   - 携带 SDP 消息体，消息体中描述了媒体服务器发送媒体流的 IP、端口、媒体格式、SSRC  值等内容。

-  10:`SIP`服务器将消息`9`转发给媒体流接收者

-  11:媒体流接收者发送`ACK `消息，完成与 `SIP  `服务器的 `Invite  `会话建立过程

   ```http
   ACK sip:xxx@192.168.0.111:5060 SIP/2.0
   Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=
   Max-Forwards: 70
   From: sip:xxx@192.168.0.107:5060;tag=
   To: sip:xxx@192.168.0.111:5060;tag=
   Call-ID: 
   CSeq: 18467 ACK
   Content-Length:  0
   ```

-  12:`SIP `服务器将消息`11`转发给媒体服务器，完成与媒体服务器的 `Invite  `会话建立过程

---> 媒体服务器向媒体接收者发送实时媒体流。

----> 媒体接收者断开媒体流，这里使用SIP服务侧断流，实际上还有设备侧断流，详情看断流一节

- 13: 媒体流接收者向 `SIP  `服务器发送 `BYE `消息
  - 断开消息1、10、11建立的同媒体流接收者的`Invite  `会话
- 14:`SIP `服务器收到 `BYE `消息后回复`200 OK  `响应，会话断开
- 15:`SIP  `服务器收到 `BYE  `消息后向媒体服务器发送 `BYE  `消息
  - 断开消息8、9、12 建立的同媒体服务器的 `Invite  `会话
- 16:媒体服务器收到 `BYE  `消息后回复`200 OK` 响应，会话断开
- 17:`SIP   `服务器向媒体服务器发送 `BYE  `消息
  - 断开消息2、3、6建立的同媒体服务器的 `Invite`会话
- 18:媒体服务器收到 `BYE `消息后回复`200 OK `响应，会话断开
- 19:`SIP   `服务器向媒体流发送者发送` BYE ` 消息
  - 断开消息4、5、7建立的同媒体流发送者的`Invite  `会话
- 20:媒体流发送者收到 `BYE  `消息后回复`200 OK  `响应，会话断开



#### 第三方呼叫

![sip监控域是什么 监控区域什么意思_服务器_14](https://s2.51cto.com/images/blog/202403/20192525_65fac7a59bc8c39386.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)

​		命令流程描述如下:

- 1:`SIP`服务器向媒体服务器发送`Invite`消息
  - 此消息不携带`SDP`消息体
- 2:媒体服务器收到`SIP`服务器的`Invite`请求后，回复`200OK` 响应
  - 携带`SDP`消息体，消息体中描述了媒体服务器接收媒体流的IP、端口、媒体格式等内容
- 3:SIP服务器收到媒体服务器返回的`200OK` 响应后,向媒体流发送者发送Invite请求,请求中携带消息2中媒体服务器回复的200OK 响应消息体,s字段为“Play”代表实时点播,增加y 字段描述SSRC值,f字段描述媒体参数
- 4:媒体流发送者收到SIP服务器的Invite请求后,回复200OK 响应,携带SDP消息体,消息 体中描述了媒体流发送者发送媒体流的IP、端口、媒体格式、SSRC字段等内容
- 5:SIP服务器收到媒体流发送者返回的200OK 响应后,向媒体服务器发送 ACK 请求,请求 中携带消息4中媒体流发送者回复的200OK 响应消息体,完成与媒体服务器的Invite会话 建立过程
- 6:SIP服务器收到媒体流发送者返回的200OK 响应后,向媒体流发送者发送 ACK 请求,请 求中不携带消息体,完成与媒体流发送者的Invite会话建立过程
- 7:SIP服务器向媒体流接收者发送Invite消息,此消息不携带SDP消息体
- 8:媒体流接收者收到SIP服务器的Invite请求后,回复200OK 响应,携带SDP消息体,消息 体中描述了媒体流接收者接收媒体流的IP、端口、媒体格式等内容
- 9:SIP服务器收到媒体流接收者返回的200OK 响应后,向媒体服务器发送Invite请求,请求 中携带消息8中媒体流接收者回复的200OK 响应消息体,s字段为“Play”代表实时点播,增 加y字段描述SSRC值
- 10:媒体服务器收到SIP服务器的Invite请求后,回复200OK 响应,携带SDP消息体,消息体 中描述了媒体服务器发送媒体流的IP、端口、媒体格式、SSRC字段等内容
- 11:SIP服务器收到媒体服务器返回的200OK 响应后,向媒体流接收者发送 ACK 请求,请求 中携带消息10中媒体服务器回复的200OK 响应消息体,完成与媒体流接收者的Invite会话 建立过程
- 12:SIP服务器收到媒体服务器返回的200OK 响应后,向媒体服务器发送 ACK 请求,请求中 不携带消息体,完成与媒体服务器的Invite会话建立过程
- 13:SIP服务器向媒体流接收者发送 BYE 消息,断开消息7、8、11建立的同媒体流接收者的 Invite会话
- 14:媒体流接收者收到 BYE消息后回复200OK 响应,会话断开
- 15:SIP服务器向媒体服务器发送 BYE 消息,断开消息9、10、12建立的同媒体服务器的Invite 会话
- 16:媒体服务器收到 BYE消息后回复200OK 响应,会话断开
- 17:SIP服务器向媒体服务器发送 BYE 消息,断开消息1、2、5建立的同媒体服务器的Invite 会话
- 18:媒体服务器收到 BYE消息后回复200OK 响应,会话断开
- 19:SIP 服务器向媒体流发送者发送 BYE 消息,断开消息3、4、6建立的同媒体流发送者的 Invite会话
- 20:媒体流发送者收到 BYE消息后回复200OK 响应,会话断开

注：invite不携带SDP时，响应的SDP数据包含的ip和端口为接收媒体流的端口。



### 文件检索

​		主要用于区域、设备、录像时间段、录像地点、录像报警为条件的查询。

- 用Message消息发送检索请求和返回查询结果，传送结果的Message消息可以发送多条；
- 文件检索请求和应答命令采用MANSCDP协议格式定义；

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/e5ef2d6536e259ce4446e95dd9be0c46.png)

#### 文件检索流程

1. 目录检索方向目录拥有方发送目录查询请求，包含检索条件；

   ```xml
   <?xml version="1.0" encoding="UTF-8"?>
   <Query>
   	<CmdType>RecordInfo</CmdType>
   	<SN>10000</SN>
   	<DeviceID>xxx</DeviceID>
   	<StartTime>2023-10-14T00:00:00</StartTime>
   	<EndTime>2023-10-14T23:00:00</EndTime>
   	<FilePath></FilePath>
   	<Address></Address>
   	<Secrecy>0</Secrecy>
   	<Type>all</Type>
   	<RecorderID>all</RecorderID>
   </Query>
   
   ```

2. 目录拥有方向目录检索方发送200 OK，无消息体；

   ```http
   SIP/2.0 200 OK
   Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx
   From: <sip:xxx@192.168.0.107>;tag=xx
   To: <sip:xxx@xx>;tag=xx
   Call-ID: xxx
   CSeq: 18467 MESSAGE
   User-Agent: IP Camera
   Content-Length: 0
   ```

3. **目录拥有方向目录检索方发送查询结果，消息体中含文件目录，当一条Message消息无法传送完所有查询结果时，采用多条消息传送；**

   ```xml
   <?xml version="1.0" encoding="GB2312"?>
   <Response>
   	<CmdType>RecordInfo</CmdType>
   	<SN>17430</SN>
   	<DeviceID>xxx</DeviceID>
   	<Name>Camera 01</Name>
   	<SumNum>1</SumNum>
   	<RecordList Num="1">
   		<Item>
   			<DeviceID>xxx</DeviceID>
   			<Name>Camera 01</Name>
   			<FilePath>file_path</FilePath>
   			<Address>Address 1</Address>
   			<StartTime>2023-10-14T14:15:52</StartTime>
   			<EndTime>2023-10-14T14:31:28</EndTime>
   			<Secrecy>0</Secrecy>
   			<Type>time</Type>
   			<FileSize>19396096</FileSize>
   		</Item>
   	</RecordList>
   </Response>
   ```

   

4. 目录检索方向目录拥有方发送200 OK，无消息体

   ```xml
   SIP/2.0 200 OK
   Via: SIP/2.0/UDP 192.168.0.111:5060;rport=5060;received=192.168.0.111;branch=xxx
   Call-ID: xxx
   From: <sip:xxx@xx>;tag=xxx
   To: <sip:xxx@xx>;tag=xxx
   CSeq: 20 MESSAGE
   Content-Length:  0
   ```



### 文件下载

- SIP服务器接收到媒体接收者发送的视音频文件下载请求后向媒体流发送者发送媒体文件下载命令
- 采用`RTP/RTCP`实现媒体传输，媒体流接收者直接将视频流保存为媒体文件
- 媒体流接收者或SIP服务器可通过配置查询等方式获取媒体流发送者支持的下载发送倍速，并在请求的SDP消息体中携带下载倍速
- 视音频文件下载宜支持媒体流保活机制



![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/afcabed8764fa9efa2ef975112934036.png)

> - 信令1、8、9、10、11、12为 SIP 服务器接收到客户端的呼叫请求后通过B2BUA  代理方式建立 媒体流接受者与媒体服务器之间的媒体链接信令过程
> - 信令2～7为 SIP  服务器通过三方呼叫控制建立 媒体服务器与媒体流之间的媒体链接信令过程
> - 信令13～16 为媒体流发送者回放、下载到文件结束向 媒体接收者发送下载完成的通知消息过程
> - 信令17～20为断开媒体流接收者断开与媒体服务器之间的 媒体链接信令过程
> - 信令21～24 为 SIP  服务器断开媒体服务器与媒体流发送者之间的媒体链接信令过程 

​		命令流程描述如下：

- 1:媒体流接收者向SIP  服务器发送 Invite  消息

  - 消息头域中
    - 携带 Subject  字段，表明点播的视 频源 ID、发送方媒体流序列号、媒体流接收者 ID、接收端媒体流序列号标识等参数
  - SDP  消息 体中
    - s 字段为“Download”  代表文件下载
    - u 字段代表下载通道 ID 和下载类型
    - t 字段代表下载 时间段
    - 可扩展a 字段携带下载倍速参数，规定此次下载设备发流倍速，若不携带则默认为1倍速

  ```http
  INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
  Max-Forwards: 70
  From: sip:xxx@192.168.0.107;tag=xxx
  To: sip:xxx@192.168.0.111
  Contact: <sip:xxx@192.168.0.107:5060>
  Call-ID: xxx
  CSeq: 15724 INVITE
  Allow: SUBSCRIBE, NOTIFY, INVITE, ACK, BYE, CANCEL, UPDATE
  Supported: 
  Subject: xxx:xx,xxx:xx
  Content-Type: application/sdp
  Content-Length:   251
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.107
  s=Download
  c=IN IP4 192.168.0.107
  t=1697955233 1697958114      # 下载的文件开始时间和结束时间
  m=video 36000 RTP/AVP 96 98 97
  a=recvonly
  a=rtpmap:96 PS/90000
  a=rtpmap:98 H264/90000
  a=rtpmap:97 MPEG4/90000
  a=downloadspeed:8            # 8倍速下载
  y=001
  ```

  

- 2:SIP  服务器收到 `Invite  `请求后，通过三方呼叫控制建立媒体服务器和媒体流发送者之间的媒体连接。

  - 向媒体服务器发送 Invite  消息，此消息不携带 SDP  消息体

- 3:媒体服务器收到 SIP  服务器的 `Invite  `请求后，回复200 OK  响应

  - 携带 SDP 消息体，消息体中描述了媒体服务器接收媒体流的IP、端口、媒体格式等内容

- 4:SIP  服务器收到媒体服务器返回的200 OK  响应后，向媒体流发送者发送 Invite  请求

  - 请求 中携带消息3中媒体服务器回复的200 OK 响应消息体
    - s 字段为“Download” 代表文件下 载
      - 增加 y 字段描述 SSRC  值 ，f字段描述媒体参数

- 5:媒体流发送者收到 SIP  服务器的 Invite  请求后，回复200 OK  响应

  - 携带 SDP  消息体，消息 体中描述了媒体流发送者发送媒体流的 IP、端口、媒体格式、SSRC 字段等内容，可扩展 a 字段携带文件大小参数

- 6:SIP  服务器收到媒体流发送者返回的200 OK  响应后，向媒体服务器发送 ACK  请求，请求 中携带消息5中媒体流发送者回复的200 OK  响应消息体，完成与媒体服务器的 Invite  会话建立过程

- 7:SIP  服务器收到媒体流发送者返回的200 OK  响应后，向媒体流发送者发送 ACK  请求，请求中不携带消息体，完成与媒体流发送者的 Invite  会话建立过程

- 8:完成三方呼叫控制后，SIP  服务器通过 B2BUA  代理方式建立媒体流接收者和媒体服务器之间的媒体连接。在消息1中增加 SSRC 值，转发给媒体服务器

- 9:媒体服务器收到 Invite  请求，回复200 OK  响应，携带 SDP  消息体，消息体中描述了媒体服务器发送媒体流的 IP、端口、媒体格式、SSRC 值等内容

- 10:SIP   服务器将消息9转发给媒体流接收者，可扩展a 字段携带文件大小参数

  ```http
  # 100 Trying
  SIP/2.0 100 Trying
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx
  From: <sip:xxx@192.168.0.107>;tag=xxx
  To: <sip:xxx@192.168.0.111>
  Call-ID: xxx
  CSeq: 15724 INVITE
  User-Agent: IP Camera
  Content-Length: 0
  
  # 200 OK
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx
  From: <sip:xxx@192.168.0.107>;tag=xxx
  To: <sip:xxx@192.168.0.111>;tag=904233038
  Call-ID: xxx
  CSeq: 15724 INVITE
  Contact: <sip:xxx@192.168.0.111:5060>
  Content-Type: application/sdp
  User-Agent: IP Camera
  Content-Length:   198
  
  v=0
  o=xxx 2131 2131 IN IP4 192.168.0.111
  s=Download
  c=IN IP4 192.168.0.111
  t=0 0
  m=video 15060 RTP/AVP 96
  a=sendonly
  a=rtpmap:96 PS/90000
  a=filesize:62224924          # 请求的文件大小
  y=001
  ```

  

  

- 11: 媒体流接收者收到200 OK 响应后，回复 ACK 消息，完成与 SIP 服务器的 Invite  会话建立过程

  ```http
  ACK sip:xxx@192.168.0.111:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
  Max-Forwards: 70
  From: sip:xxx@192.168.0.107;tag=xxx
  To: sip:xxx@192.168.0.111;tag=xx
  Call-ID: xx
  CSeq: 26500 ACK
  Content-Length:  0
  ```

  

- 12:SIP  服务器将消息11转发给媒体服务器，完成与媒体服务器的 Invite  会话建立过程

- 13:   媒体流发送者在文件下载结束后发送会话内 Message  消息，通知 SIP  服务器回放已结束，消息体格式应符合 A.2.5.4  媒体通知的要求

  ```http
  MESSAGE sip:xxx@192.168.0.107:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=xxx
  From: <sip:xxx@192.168.0.111>;tag=xxx
  To: <sip:xxx@192.168.0.107>;tag=xxx
  Call-ID: xxx
  CSeq: 2 MESSAGE
  Contact: <sip:xxx@192.168.0.111:5060>
  Content-Type: Application/MANSCDP+xml
  Max-Forwards: 70
  User-Agent: IP Camera
  Content-Length:   174
  
  <?xml version="1.0" encoding="GB2312"?>
  <Notify>
  <CmdType>MediaStatus</CmdType>
  <SN>151</SN>
  <DeviceID>xxx</DeviceID>
  <NotifyType>121</NotifyType>
  </Notify>
  ```

  

- 14:SIP  服务器收到消息17后转发给媒体流接收者

- 15:媒体流接收者收到消息18后回复200 OK 响应，进行链路断开过程

- 16:SIP  服务器将消息19转发给媒体流发送者

- 17:媒体流接收者向 SIP 服务器发送 BYE 消息，断开消息1、10、11建立的同媒体流接收者的Invite  会话

- 18:SIP   服务器收到 BYE 消息后回复200 OK  响应，会话断开

- 19:SIP  服务器收到 BYE  消息后向媒体服务器发送 BYE  消息，断开消息8、9、12 建立的同媒体服务器的 Invite  会话

- 20:媒体服务器收到 BYE 消息后回复200 OK 响应，会话断开

- 21:SIP  服务器向媒体服务器发送 BYE  消息，断开消息2、3、6建立的同媒体服务器的 Invite会话

- 22:媒体服务器收到 BYE  消息后回复200 OK  响应，会话断开

- 23:`SIP   `服务器向媒体流发送者发送 `BYE`  消息，断开消息`4、5、7`建立的同媒体流发送者的`Invite  `会话

- 24:媒体流发送者收到 `BYE  `消息后回复`200 OK ` 响应，会话断开



### 录像回放

​		选择开始结束时间进行视音频文件下载，满足以下基本要求：

- `Gb28181`标准中，采用`INFO`消息体携带`MANSRTSP`进行录像回放的请求
  - `GBT 28181-2016 4.3.5`
- 采用`RTP/RTCP`实现媒体传输
- 使用 `MANSRTSP`协议中的` Play、Pause、Teardown`的请求和应答控制回放
- 媒体流接收者可为SIP客户端、SIP设备（如视频解码器），媒体流发送者可为SIP设备、网关、媒体服务器；

- 媒体流接收者或 `SIP`服务器可通过  配置查询  等方式获取媒体流发送者支持  的下载发送倍速，并在请求的 `SDP `消息体中携带指定下载倍速。
  - 媒体流发送者可在 `Invite `请求对应的`200 OK `响应` SDP  `消息体中，扩展携带下载文件的大小参数以便于媒体流接收者计算下载进度，当媒体流发送者不能提供文件大小参数时
  - 媒体流接收者应支持根据码流中取得的时间计算下载进度
- 视音频文件下载采用媒体流保活机制



![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/acdbcd53137d438889f057dff4fde261407536888.png%25401256w_1648h_!web-article-pic.webp)



​		命令流程描述如下：

- 1:媒体流接收者向SIP  服务器发送 Invite  消息

  - 消息头域中
    - 携带 Subject  字段，表明点播的视 频源 ID、发送方媒体流序列号、媒体流接收者 ID、接收端媒体流序列号标识等参数
  - SDP  消息 体中
    - s 字段为“Download”  代表文件下载
    - u 字段代表下载通道 ID 和下载类型
    - t 字段代表下载 时间段
    - 可扩展a 字段携带下载倍速参数，规定此次下载设备发流倍速，若不携带则默认为1倍速

  ```http
  INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
  Max-Forwards: 70
  From: sip:xxx@192.168.0.107;tag=xx
  To: sip:xxx@192.168.0.111
  Contact: <sip:xxx@192.168.0.107:5060>
  Call-ID: xxx
  CSeq: xx INVITE
  Allow: INVITE, ACK, BYE, CANCEL, UPDATE, PRACK  # 支持的方法
  Supported: 100rel
  Subject: xxx:xx,xxx:xx
  Content-Type: application/sdp
  Content-Length:   233
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.107
  s=Playback                  # 请求方法 
  c=IN IP4 192.168.0.107
  t=1697264152 1697266715     # 请求的开始时间和结束时间
  m=video 36000 RTP/AVP 96 98 97
  a=recvonly
  a=rtpmap:96 PS/90000
  a=rtpmap:98 H264/90000
  a=rtpmap:97 MPEG4/90000
  y=0100000001
  ```

  

  ```c++
  static std::string CreatePlayBackSDP(const GB28181MediaContext& mediaContext)
  {
      char str[500] = { 0 };
      pj_ansi_snprintf(str, 500,
                       "v=0\n"
                       "o=%s 0 0 IN IP4 %s\n"
                       "s=Playback\n"
                       "c=IN IP4 %s\n"
                       "t=%lld %lld\n"
                       "m=video %d RTP/AVP 96 98 97\n"
                       "a=recvonly\n"
                       "a=rtpmap:96 PS/90000\n"
                       "a=rtpmap:98 H264/90000\n"
                       "a=rtpmap:97 MPEG4/90000\n"
                       "y=001\n",
                       mediaContext.GetDeviceId().c_str(),
                       mediaContext.GetRecvAddress().c_str(),
                       mediaContext.GetRecvAddress().c_str(),
                       mediaContext.GetPBStartTime(),
                       mediaContext.GetPBEndTime(),
                       mediaContext.GetRecvPort()
                      );
      return str;
  }
  ```

  

- 2:SIP  服务器收到 `Invite  `请求后，通过三方呼叫控制建立媒体服务器和媒体流发送者之间的媒体连接。向媒体服务器发送 Invite  消息，此消息不携带 SDP  消息体

- 3:媒体服务器收到 `SIP`服务器的 `Invite  `请求后，回复`200 OK`  响应

  - 携带 SDP 消息体，消息体中描述了媒体服务器接收媒体流的IP、端口、媒体格式等内容

- 4:SIP  服务器收到媒体服务器返回的`200 OK`  响应后，向媒体流发送者发送 Invite  请求，请求 中携带消息3中媒体服务器回复的200 OK 响应消息体，s 字段为“`Download`” 代表文件下 载，`u `字段代表下载通道 ID 和下载类型，t 字段代表下载时间段，增加 y 字段描述 SSRC  值 ，f字段描述媒体参数，可扩展a 字段携带下载倍速，将倍速参数传递给设备

- 5:媒体流发送者收到 SIP  服务器的 Invite  请求后，回复200 OK  响应，携带 SDP  消息体，消息 体中描述了媒体流发送者发送媒体流的 IP、端口、媒体格式、`SSRC `字段等内容，可扩展 a 字段携带文件大小参数

- 6:SIP  服务器收到媒体流发送者返回的200 OK  响应后，向媒体服务器发送 ACK  请求，请求 中携带消息5中媒体流发送者回复的200 OK  响应消息体，完成与媒体服务器的 Invite  会话建立过程

- 7:SIP  服务器收到媒体流发送者返回的200 OK  响应后，向媒体流发送者发送 ACK  请求，请求中不携带消息体，完成与媒体流发送者的 Invite  会话建立过程

- 8:完成三方呼叫控制后，SIP  服务器通过 B2BUA  代理方式建立媒体流接收者和媒体服务器之间的媒体连接。在消息1中增加 SSRC 值，转发给媒体服务器

- 9:媒体服务器收到 Invite  请求，回复200 OK  响应，携带 SDP  消息体，消息体中描述了媒体服务器发送媒体流的 IP、端口、媒体格式、SSRC 值等内容

- 10:SIP   服务器将消息9转发给媒体流接收者，可扩展a 字段携带文件大小参数

  ```http
  SIP/2.0 100 Trying
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx
  From: <sip:xxx@192.168.0.107>;tag=xxx
  To: <sip:xxx@192.168.0.111>
  Call-ID: xxx
  CSeq: 26500 INVITE
  User-Agent: IP Camera
  Content-Length: 0
  ```

  ```http
  SIP/2.0 200 Ok
  ...
  
  v=0
  o=xxx 1080 1080 IN IP4 192.168.0.111
  s=Playback
  c=IN IP4 192.168.0.111
  t=0 0
  m=video 15060 RTP/AVP 96
  a=sendonly
  a=rtpmap:96 PS/90000   # 为PS流
  a=filesize:0
  y=0100000001
  ```

  

- 11: 媒体流接收者收到200 OK 响应后，回复 ACK 消息，完成与 SIP 服务器的 Invite  会话建立过程

  ```http
  ACK sip:xxx@192.168.0.111:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
  Max-Forwards: 70
  From: sip:xxx@192.168.0.107;tag=xxx
  To: sip:xxx@192.168.0.111;tag=xx
  Call-ID: xx
  CSeq: 26500 ACK
  Content-Length:  0
  ```

  

- 12:SIP  服务器将消息11转发给媒体服务器，完成与媒体服务器的 Invite  会话建立过程

- 13：媒体流接收者向SIP  服务器发送 `Info`消息，进行回放控制

- 14：SIP服务器将收到的`Info`消息转发给媒体发送者

- 15：媒体发送者向`SIP`服务器发送控制结果

- 16：`SIP`服务器转发控制结果给媒体接收者

- 17:   媒体流发送者在文件下载结束后发送会话内 Message  消息，通知 SIP  服务器回放已结束，消息体格式应符合 A.2.5.4  媒体通知的要求

- 18:SIP  服务器收到消息17后转发给媒体流接收者

- 19:媒体流接收者收到消息18后回复200 OK 响应，进行链路断开过程

- 20:SIP  服务器将消息19转发给媒体流发送者

- 21:媒体流接收者向 SIP 服务器发送 BYE 消息，断开消息1、10、11建立的同媒体流接收者的Invite  会话

- 22:SIP   服务器收到 BYE 消息后回复200 OK  响应，会话断开

- 23:SIP  服务器收到 BYE  消息后向媒体服务器发送 BYE  消息，断开消息8、9、12 建立的同媒体服务器的 Invite  会话

- 24:媒体服务器收到 BYE 消息后回复200 OK 响应，会话断开

- 25:SIP  服务器向媒体服务器发送 BYE  消息，断开消息2、3、6建立的同媒体服务器的 Invite会话

- 26:媒体服务器收到 BYE  消息后回复200 OK  响应，会话断开

- 27:SIP   服务器向媒体流发送者发送 BYE  消息，断开消息4、5、7建立的同媒体流发送者的Invite  会话

- 28:媒体流发送者收到 BYE  消息后回复200 OK  响应，会话断开



### 断流

​		发送`BYE`消息来结束流。

- `Call-ID`：该字段判断请求端口哪一路视频



#### SIP服务侧断流

​		主动停止命令的下达或是无人点播等事件的触发

- SIP服务会向指定设备发送BYE消息
- 设备侧收到SIP服务发送的BYE消息后，设备会立即停止正在进行的视频流推送，并向SIP服务回复一个200OK消息
- IP服务收到设备返回的200OK响应，确认推流已经成功停止，向收流媒体告知端口取消监听等资源释放流程

#### 设备侧断流

​		设备侧连不到服务侧端口或者设备侧算力不足等原因

- 设备侧会向SIP服务发送BYE消息

  ```http
  BYE sip:xxx@192.168.0.107:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=
  From: <sip:xxx@192.168.0.111:5060>;tag=
  To: <sip:xxx@192.168.0.107:5060>;tag=
  Call-ID: 
  CSeq: 2 BYE
  Max-Forwards: 70
  User-Agent: IP Camera
  Content-Length: 0
  ```

- SIP服务收到设备侧发送的BYE消息后，向收流媒体告知端口取消监听等资源释放流程，并向设备侧回复一个200OK消息

  ```http
  SIP/2.0 200 OK
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport=5060;received=192.168.0.111;branch=
  Call-ID: 
  From: <sip:xxx@192.168.0.111:5060>;tag=
  To: <sip:xxx@192.168.0.107:5060>;tag=
  CSeq: 2 BYE
  Content-Length:  0
  ```

- 设备侧收到设备返回的200OK响应，设备会立即停止正在进行的视频流推送（此流程也可在第一步操作）



### 语音广播

​		语音广播功能实现，用户通过语音输入设备，向前端语音输出设备的语音广播。

- 语音流发送者：语音输入设备，联网系统，SIP服务器
- 语音流接收者：语音输出设备，联网系统

![sip监控域是什么 监控区域什么意思_服务器_18](https://s2.51cto.com/images/blog/202403/20192526_65fac7a6395b056567.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=/format,webp/resize,m_fixed,w_1184)

命令流程描述如下:

- 1:SIP服务器向语音流接收者发送语音广播通知消息,消息中通过 To头域标明作为目的地址 的语音流接收者ID,消息采用 Message方法携带。

  ```http
  MESSAGE sip:xxx@192.168.0.111:5060 SIP/2.0
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
  Max-Forwards: 70
  From: <sip:xxx@192.168.0.107>;tag=xxx
  To: <sip:xxx@xx>
  Contact: <sip:xxx@192.168.0.107:5060>
  Call-ID: xxx
  CSeq: 18467 MESSAGE
  Content-Type: Application/MANSCDP+xml
  Content-Length:   186
  
  <?xml version="1.0" encoding="UTF-8"?>
  <Notify>
  <CmdType>Broadcast</CmdType>
  <SN>130</SN>
  <!--语音输入设备的设备编码-->
  <SourceID>xxx</SourceID>
  <!--语音输出设备的设备编码-->
  <TargetID>xxx</TargetID>
  </Notify>
  
  ```

  

- 2:语音流接收者收到语音广播通知消息后,向SIP服务器发送200OK 响应。

  ```http
  SIP/2.0 200 OK
  Call-ID: xxx
  Content-Length: 0
  CSeq: 18467 MESSAGE
  From: <sip:xxx@192.168.0.107>;tag=xxx
  To: <sip:xxx@xxx>;tag=xxx
  User-Agent: SIP UAS V.2016.xxxx
  Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx
  
  ```

  

- 3:语音流接收者向SIP服务器发送语音广播应答消息,消息中通过 To头域标明作为目的地 址的SIP服务器ID,消息采用 Message方法携带。

  ```http
  MESSAGE sip:xxx@192.168.0.107 SIP/2.0
  Call-ID: xxx
  Content-Length: 180
  Content-Type: Application/MANSCDP+xml
  CSeq: 2 MESSAGE
  From: <sip:xxx@192.168.0.111:5060>;tag=xxx
  Max-Forwards: 70
  To: <sip:xxx@192.168.0.107>
  User-Agent: SIP UAS V.2016.xxxx
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=xxx
  
  <?xml version="1.0" encoding="GB2312" standalone="yes" ?>
  <Response>
  <CmdType>Broadcast</CmdType>
  <SN>130</SN>
  <DeviceID>xxx</DeviceID>
  <Result>OK</Result>
  </Response>
  
  ```

  

- 4:SIP服务器收到语音广播应答消息后,向语音流接收者发送200OK 响应。

  ```http
  INVITE sip:xxx@192.168.0.107:5060 SIP/2.0
  Call-ID: xxx
  Contact: <sip:xxx@192.168.0.111:5060>
  Content-Length: 240
  Content-Type: application/sdp
  CSeq: 1 INVITE
  Expires: 120
  From: <sip:xxx@192.168.0.111:5060>;tag=xxx
  Max-Forwards: 70
  Subject: xxx:xxx,xxx:xxx
  To: <sip:xxx@192.168.0.107:5060>
  User-Agent: SIP UAS V.2016.xxxx
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=xxx
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.111
  s=Play
  i=VCam BroadCast Session
  c=IN IP4 192.168.0.111
  t=0 0
  m=audio 9712 RTP/AVP 8 96
  a=recvonly
  a=rtpmap:8 PCMA/8000/1
  a=rtpmap:96 PS/90000
  y=0022371912
  f=v/0/0/0/0/0a/1/8/1
  
  ```

  

- 5:语音流接收者向SIP服务器发送Invite消息,消息中通过 To头域标明作为目的地址的语音 流发送者ID,消息头域中携带Subject字段,表明请求的语音流发送者ID、发送方媒体流序列 号、语音流接收者ID、接收方媒体流序列号等参数,SDP消息体中s字段为“Play”代表实时点 播,m 字段中媒体参数标识为“audio”表示请求语音媒体流。

  ```http
  INVITE sip:xxx@192.168.0.107:5060 SIP/2.0
  Call-ID: xxx
  Contact: <sip:xxx@192.168.0.111:5060>
  Content-Length: 240
  Content-Type: application/sdp
  CSeq: 1 INVITE
  Expires: 120
  From: <sip:xxx@192.168.0.111:5060>;tag=xxx
  Max-Forwards: 70
  Subject: xxx:xxx,xxx:xxx
  To: <sip:xxx@192.168.0.107:5060>
  User-Agent: SIP UAS V.2016.xxxx
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=xxx
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.111
  s=Play
  i=VCam BroadCast Session
  c=IN IP4 192.168.0.111
  t=0 0
  m=audio 9712 RTP/AVP 8 96
  a=recvonly
  a=rtpmap:8 PCMA/8000/1
  a=rtpmap:96 PS/90000
  y=0022371912
  f=v/0/0/0/0/0a/1/8/1
  
  ```

  

- 6:SIP服务器收到Invite请求后,通过三方呼叫控制建立媒体服务器和语音流发送者之间的 媒体连接。向媒体服务器发送Invite消息,此消息不携带SDP消息体。

- 7:媒体服务器收到SIP服务器的Invite请求后,回复200OK 响应,携带SDP消息体,消息体 中描述了媒体服务器接收媒体流的IP、端口、媒体格式等内容。

-  8:SIP服务器收到媒体服务器返回的200OK 响应后,向语音流发送者发送Invite请求,消息 中通过 To头域标明作为目的地址的语音流发送者ID,消息头域中携带 Subject字段,表明请 求的语音流发送者ID、发送方媒体流序列号、语音流接收者ID、接收方媒体流序列号等参数, 请求中携带消息7中媒体服务器回复的200OK 响应消息体,s字段为“Play”代表实时点播, m 字段中媒体参数标识为“audio”表示请求语音媒体流,增加y字段描述SSRC值,f字段描述 媒体参数。

- 9:语音流发送者收到SIP服务器的Invite请求后,回复200OK 响应,携带SDP消息体,消息 体中描述了媒体流发送者发送媒体流的IP、端口、媒体格式、SSRC 字段等内容,s字段为 “Play”代表实时点播,m 字段中媒体参数标识为“audio”表示请求语音媒体流。

- 10:SIP服务器收到语音流发送者返回的200OK 响应后,向媒体服务器发送 ACK 请求,请求 中携带消息9中语音流发送者回复的200OK 响应消息体,完成与媒体服务器的Invite会话 建立过程。

- 11:SIP服务器收到语音流发送者返回的200OK 响应后,向语音流发送者发送 ACK 请求,请 求中不携带消息体,完成与语音流发送者的Invite会话建立过程。

- 12:完成三方呼叫控制后,SIP服务器通过 B2BUA 代理方式建立语音流接收者和媒体服务器 之间的媒体连接。在消息5中增加SSRC值,转发给媒体服务器。

- 13:媒体服务器收到Invite请求,回复200OK 响应,携带SDP消息体,消息体中描述了媒体服 务器发送媒体流的IP、端口、媒体格式、SSRC值等内容,s字段为“Play”代表实时点播,m 字段 中媒体参数标识为“audio”表示请求语音媒体流。

- 14:SIP服务器将消息13转发给语音流接收者。

  ```http
  SIP/2.0 200 OK
  From: <sip:xxx@192.168.0.107:5060>;tag=xxx
  To: <sip:xxx@192.168.0.111:5060>;tag=xxx
  Call-ID: xxx
  CSeq: 1 INVITE
  User-Agent: General SIP UAS V1.0
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=xxx
  Contact: <sip:xxx@192.168.0.107:5060>
  Content-Type: application/SDP
  Content-Length: 182
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.107
  s=Play
  c=IN IP4 192.168.0.107
  t=0 0
  m=audio 40000 RTP/AVP 8
  a=rtpmap:8 PCMA/8000
  a=sendonly
  y=0022371912
  
  ```

  

- 15:语音流接收者收到200OK 响应后,回复 ACK 消息,完成与SIP服务器的Invite会话建立 过程。

  ```http
  SIP/2.0 200 OK
  From: <sip:xxx@192.168.0.107:5060>;tag=xxx
  To: <sip:xxx@192.168.0.111:5060>;tag=xxx
  Call-ID: xxx
  CSeq: 1 INVITE
  User-Agent: General SIP UAS V1.0
  Via: SIP/2.0/UDP 192.168.0.111:5060;rport;branch=xxx
  Contact: <sip:xxx@192.168.0.107:5060>
  Content-Type: application/SDP
  Content-Length: 182
  
  v=0
  o=xxx 0 0 IN IP4 192.168.0.107
  s=Play
  c=IN IP4 192.168.0.107
  t=0 0
  m=audio 40000 RTP/AVP 8
  a=rtpmap:8 PCMA/8000
  a=sendonly
  y=0022371912
  
  ```

  

- 16:SIP服务器将消息15转发给媒体服务器,完成与媒体服务器的Invite会话建立过程。

- 17:SIP服务器向语音流接收者发送 BYE消息,断开消息5、14、15建立的Invite会话。

  ```http
  BYE sip:xxx@xxx SIP/2.0
  From: <sip:xxx@xxx>;tag=xxx
  To: <sip:xxx@xxx>;tag=xxx
  CSeq: 2 BYE
  Call-ID: xxx
  Via: SIP/2.0/UDP 192.168.0.111:5060;branch=xxx
  Max-Forwards: 70
  Content-Length: 0
  
  ```

  

- 18:语音流接收者收到 BYE消息后回复200OK 响应,会话断开。

- 19:SIP服务器向媒体服务器发送 BYE 消息,断开消息 12、13、16 建立的同媒体服务器的 Invite会话。

- 20:媒体服务器收到 BYE消息后回复200OK 响应,会话断开。

- 21:SIP服务器向媒体服务器发送 BYE消息,断开消息6、7、10建立的同媒体服务器的Invite 会话

- 22:媒体服务器收到 BYE消息后回复200OK 响应,会话断开。

- 23:SIP服务器向语音流发送者发送 BYE 消息,断开消息8、9、11建立的同语音流发送者的 Invite会话。

- 24:语音流发送者收到 BYE消息后回复200OK 响应,会话断开。



### 语音对讲

​		语音对讲功能实现中心用户与前端用户之间的一对一语音对讲功能。 

​		语音对讲功能由下述两个独立的流程组合实现:

- 通过实时视音频点播功能，中心用户获得前端设备的实时视音频媒体流
- 通过语音广播功能，中心用户向前端对讲设备发送实时音频媒体流
  - 语音流的封装格式见C.2.4音频流的 RTP封装定义



#### 语音对讲流程



```http
INVITE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107;tag=xxx
To: sip:xxx@192.168.0.111
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 18467 INVITE
Allow: SUBSCRIBE, NOTIFY, INVITE, ACK, BYE, CANCEL, UPDATE
Supported: 
Subject: xxx:xx,xxx:xx
Content-Type: application/sdp
Content-Length:   169

v=0
o=xxx 0 0 IN IP4 192.168.0.107
<!--请求媒体流的操作类型:Talk-->
s=Talk
c=IN IP4 192.168.0.107
t=0 0
<!--媒体类型:音频(audio) 传输端口:38000 传输方式:UDP 负载类型:8(PCMA)-->
m=audio 38000 RTP/AVP 8
a=sendrecv
<!--负载类型:8(PCMA) 时钟频率:8000-->
a=rtpmap:8 PCMA/8000
<!--"f=v/编码格式/分辨率/帧率/码率/码率大小a/编码格式/码率大小/采样率"-->
f=v/a/1/8/1
y=0100000001

```

```http
SIP/2.0 200 OK
Call-ID: xxx
Contact: <sip:xxx@192.168.0.111:5060>
Content-Length: 270
Content-Type: application/sdp
CSeq: 18467 INVITE
From: <sip:xxx@192.168.0.107>;tag=xxx
To: <sip:xxx@192.168.0.111>;tag=xxx
User-Agent: SIP UAS V.2016.xxxx
Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx

v=0
o=xxx 0 0 IN IP4 192.168.0.111
s=Talk
i=VCam Talk Session
c=IN IP4 192.168.0.111
t=0 0
m=audio 9712 RTP/AVP 8
a=recvonly
a=rtpmap:8 PCMA/8000/1
m=audio 9712 RTP/AVP 8
a=sendonly
a=rtpmap:8 PCMA/8000/1
y=0100000001
f=v/0/0/0/0/0a/0/0/0

```

```http
ACK sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107;tag=xxx
To: sip:xxx@192.168.0.111;tag=xxx
Call-ID: xxx
CSeq: 18467 ACK
Content-Length:  0

```



可以总结为：**音频数据的双向传输。**

```c++
static std::string CreateSDPForAudio(const GB28181MediaContext& mediaContext)
{
    char str[500] = { 0 };
    pj_ansi_snprintf(str, 500,
                     "v=0\n"
                     "o=%s 0 0 IN IP4 %s\n"
                     "s=Talk\n"
                     "c=IN IP4 %s\n"
                     "t=0 0\n"
                     "m=audio %d RTP/AVP 8\n"
                     "a=sendrecv\n"
                     "a=rtpmap:8 PCMA/8000\n"
                     "f=v/a/1/8/1\n"
                     "y=0100000001\n",
                     mediaContext.GetDeviceId().c_str(),
                     mediaContext.GetRecvAddress().c_str(),
                     mediaContext.GetRecvAddress().c_str(),
                     mediaContext.GetRecvPort()
                    );
    return str;
}

```



### 校时

- 联网内设备支持基于SIP方式或NTP方式的网络校时功能，标准时间为北京时间；
- 系统运行时可根据配置使用具体校时方式；
- SIP校时在注册过程中完成，流程同注册和注销流程；
- 在注册成功情况下，注册流程的最后一个SIP应答消息200 OK中的Date头域中携带时间信息，格式为"Date:yyyy-MM-ddTHH:mm:ss.SSS";
- 当SIP代理通过注册方式校时，其注册过期时间宜设置为小于SIP代理与SIP服务器之间出现1s误差所经过的运行时间；

```sip
# 1.请求注册
REGISTER sip:xxx@192.168.0.107:5060 SIP/2.0
...

# 2.返回401未认证
SIP/2.0 401 Unauthorized
...

# 3.携带认证信息再次请求注册
REGISTER sip:xxx@192.168.0.107:5060 SIP/2.0
Authorization: Digest username="xxx", realm="xxx", nonce="xx", uri="sip:xxx@192.168.0.107:5060", response="xxx", algorithm=MD5, opaque="xx"
...

# 返回注册成功 携带“Date: 2023-10-19T10:23:03”
SIP/2.0 200 OK
...
Date: 2023-10-19T10:23:03 
Content-Length:  0

```

```c++
void CMySipContext::Response(pjsip_rx_data* rdata, int st_code, int headType)
{
	pjsip_tx_data* tdata = nullptr;
	pj_status_t status = pjsip_endpt_create_response(m_endPoint, rdata, st_code, nullptr, &tdata);
	if (PJ_SUCCESS != status)
		return;
	
	std::string date = "";
	pj_str_t c;
	pj_str_t key;
	pjsip_hdr* hdr = nullptr;
	std::stringstream ss;
	time_t t;
	switch (headType)
	{
	case DateHead:
		key = pj_str((char*)"Date");
		t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ss << std::put_time(std::localtime(&t), "%FT%T");
		date = ss.str();
		hdr = reinterpret_cast<pjsip_hdr*>(pjsip_date_hdr_create(m_pool, &key, pj_cstr(&c, date.c_str())));
		pjsip_msg_add_hdr(tdata->msg, hdr);
		break;
     // ... 
     default:
		break;
    }
    
    // ...
}
```





### 订阅与取订

- 订阅应使用**SUBSCRIBE**方法；

#### 事件订阅

​		订阅后，事件发生时，向事件订阅者发送通知。

- 事件源：
  - 联网系统
  - SIP服务器
  - 报警设备
  - 移动设备
  - 被集成的卡口系统等
- 事件观察者
  - 联网系统
  - SIP服务器
  - 客户端
- 事件：
  - 报警事件
  - 移动设备位置通知事件
  - PTZ精准位置变化事件等



#### 目录订阅

​		订阅目录后，目录发生变化向订阅者发送通知。

- 目录拥有者
  - 联网系统
  - 有子设备的设备
  - 代理设备网关
- 目录接收者
  - 联网系统
  - 有子设备的设备
  - 代理设备网关



![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/7db0abef947ead03510c2a782ebe940e.png)



#### 订阅格式

​		订阅

```http
SUBSCRIBE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107;tag=xxx
To: sip:xxx@192.168.0.111
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 18467 SUBSCRIBE
Event: presence;id=xxx
Expires: 300
Supported: 100rel
Accept: application/pidf+xml, application/xpidf+xml
Allow-Events: presence
Subject: xxx:xxx,xxx:xxx
Content-Type: application/MANSCDP+xml
Content-Length:   327

<?xml version="1.0" encoding="UTF-8"?>
<Query>
<CmdType>Alarm</CmdType>
<SN>130</SN>
<DeviceID>xxx</DeviceID>
<StartAlarmPrority>0</StartAlarmPrority>
<EndAlarmPrority>0</EndAlarmPrority>
<AlarmMethod>0</AlarmMethod>
<StartTime>2023-10-24T00:00:00</StartTime>
<EndTime>2023-10-24T23:00:00</EndTime>
</Query>

```

​		订阅结果

```http
SIP/2.0 200 OK
Call-ID: xxx
Contact: <sip:xxx@192.168.0.111:5060>
Content-Length: 0
CSeq: 18467 SUBSCRIBE
Event: presence;id=xxx
Expires: 300
From: <sip:xxx@192.168.0.107>;tag=xxx
To: <sip:xxx@192.168.0.111>;tag=xxx
User-Agent: SIP UAS V.2016.xxxx
Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx

```

​		取消订阅

```http
SUBSCRIBE sip:xxx@192.168.0.111:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.0.107:5060;rport;branch=xxx
Max-Forwards: 70
From: sip:xxx@192.168.0.107;tag=xxx
To: sip:xxx@192.168.0.111
Contact: <sip:xxx@192.168.0.107:5060>
Call-ID: xxx
CSeq: 26500 SUBSCRIBE
Event: presence;id=xxx
Expires: 0
Supported: 100rel
Accept: application/pidf+xml, application/xpidf+xml
Allow-Events: presence
Subject: xxx:xx,xxx:xx
Content-Type: application/MANSCDP+xml
Content-Length:   327

<?xml version="1.0" encoding="UTF-8"?>
<Query>
<CmdType>Alarm</CmdType>
<SN>130</SN>
<DeviceID>xxx</DeviceID>
<StartAlarmPrority>0</StartAlarmPrority>
<EndAlarmPrority>0</EndAlarmPrority>
<AlarmMethod>0</AlarmMethod>
<StartTime>2023-10-24T00:00:00</StartTime>
<EndTime>2023-10-24T23:00:00</EndTime>
</Query>

```

取消订阅结果

```http
SIP/2.0 200 OK
Call-ID: xxx
Contact: <sip:xxx@192.168.0.111:5060>
Content-Length: 0
CSeq: 26500 SUBSCRIBE
Event: presence;id=xxx
Expires: 0
From: <sip:xxx@192.168.0.107>;tag=xxx
To: <sip:xxx@192.168.0.111>;tag=xxx
User-Agent: SIP UAS V.2016.xxxx
Via: SIP/2.0/UDP 192.168.0.107:5060;rport=5060;branch=xxx
```







#### 订阅与取订代码

```c++
// init

pj_init();
pjlib_util_init();
pj_caching_pool_init(&m_cachingPool, &pj_pool_factory_default_policy, 0);
pjsip_endpt_create(&m_cachingPool.factory, nullptr, &m_endPoint);
pjsip_tsx_layer_init_module(m_endPoint);
pjsip_ua_init_module(m_endPoint, nullptr);

// 订阅相关
pjsip_evsub_init_module(m_endPoint);
pjsip_pres_init_module(m_endPoint, pjsip_evsub_instance());
```

```c++
// 订阅请求
std::string CMySipMedia::CreateAlarmXmlText_(const std::string& eventName, const GBSubscribeContext& subContext)
{
	char szAlarmInfo[500] = { 0 };
	pj_ansi_snprintf(szAlarmInfo, 500,
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<Query>\n"
		"<CmdType>%s</CmdType>\n"
		"<SN>130</SN>\n"
		"<DeviceID>%s</DeviceID>\n"
		"<StartAlarmPrority>0</StartAlarmPrority>\n"
		"<EndAlarmPrority>0</EndAlarmPrority>\n"
		"<AlarmMethod>0</AlarmMethod>\n"
		"<StartTime>%s</StartTime>\n"
		"<EndTime>%s</EndTime>\n"
		"</Query>\n", eventName.c_str()
		, subContext.GetDeviceId().c_str()
		, subContext.GetSubStartTime().c_str()
		, subContext.GetSubEndTime().c_str()
	);

	return szAlarmInfo;
}

```

```c++
// 发送订阅请求
static pjsip_evsub_user pres_user = {
    &pres_on_evsub_state,
    &pres_on_evsub_tsx_state,
    &pres_on_evsub_rx_refresh,
    &pres_on_evsub_rx_notify,
    &pres_on_evsub_client_refresh,
    &pres_on_evsub_server_timeout,
};

// 接收NOTIFY消息
static void pres_on_evsub_rx_notify(pjsip_evsub* sub,
	pjsip_rx_data* rdata,
	int* p_st_code,
	pj_str_t** p_st_text,
	pjsip_hdr* res_hdr,
	pjsip_msg_body** p_body)
{
}

bool CMySipContext::Subscribe(pjsip_dialog* dlg, pjsip_evsub_user* pres_user, const std::string& eventName, const std::string& xmlText, const GBSubscribeContext& subContext)
{
    // 创建uac
	pjsip_evsub* sub = nullptr;
	static const pj_str_t STR_PRESENCE = { (char*)"presence", 8 };
	pj_status_t status = pjsip_evsub_create_uac(dlg, pres_user, &STR_PRESENCE, 0, &sub);
	if (PJ_SUCCESS != status)
		return false;

    // 初始化
	pjsip_tx_data* tdata = nullptr;
	status = pjsip_evsub_initiate(sub, nullptr, subContext.GetExpires(), &tdata);
	if (PJ_SUCCESS != status)
		return false;

    // 构造Body
	pjsip_media_type type;
	type.type = pj_str((char*)"application");
	type.subtype = pj_str((char*)"MANSCDP+xml");
	auto text = pj_str(const_cast<char*>(xmlText.c_str()));
	tdata->msg->body = pjsip_msg_body_create(m_pool, &type.type, &type.subtype, &text);

	auto hName = pj_str((char*)"Subject");
	auto subjectUrl = subContext.GetDeviceId() + ":" + SiralNum + "," + GetCode() + ":" + SiralNum;
	auto hValue = pj_str(const_cast<char*>(subjectUrl.c_str()));
	auto hdr = pjsip_generic_string_hdr_create(m_pool, &hName, &hValue);
	pjsip_msg_add_hdr(tdata->msg, reinterpret_cast<pjsip_hdr*>(hdr));

    // 消息发送
	status = pjsip_evsub_send_request(sub, tdata);
	if (PJ_SUCCESS != status)
		return false;

	return true;
}

```





### 报警事件通知

- 发生报警事件时，源设备将报警信息发送给SIP服务器；

- 报警事件通知和分发使用MESSAGE方法；
- 源设备包括：
  - SIP设备
  - 网关
  - SIP客户端
  - 联网系统
  - 综合接处警系统以及卡口系统
- 目标设备包括：
  - 具有接警功能的SIP客户端
  - 联网系统
  - 综合接处警系统以及卡口系统

- 事件源接受事件订阅后，在事件触发后应立即通知事件观察者，事件观察者应向事件源发送事件收到的确认消息；
- 事件通知使用NOTIFY方法；

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/e8c829872809cfbc2e06f95c2f6b5eaf.png)

> 1-4： 报警上告
>
> 5-8：报警通知

#### 报警格式

```xml
// 省略头

<?xml version="1.0" encoding="UTF-8" standalone="true"?>
<Notify>
	<CmdType>Alarm</CmdType>
	<SN>2</SN>
	<DeviceID>xxx</DeviceID>
	<AlarmPriority>1</AlarmPriority>         			报警级别
	<AlarmMethod>5</AlarmMethod>						报警方式
	<AlarmTime>2023-10-20T09:19:12</AlarmTime>			报警时间
	<AlarmDescription>视频动检</AlarmDescription>
	<AlarmInfo>11</AlarmInfo>
	<Info>
		<AlarmType>2</AlarmType>					报警类型
		<AlarmTypeParam>							报警类型参数
			<EventType>1</EventType>
		</AlarmTypeParam>
	</Info>
</Notify>

经度（Longitude）；
纬度（Latitude）；
```

```xml
SIP/2.0 200 OK
Via: SIP/2.0/UDP 192.168.0.111:5060;rport=5060;received=192.168.0.111;branch=xxx
Call-ID: xxx
From: <sip:xxx@192.168.0.111>;tag=xxx
To: <sip:xxx@192.168.0.107>;tag=xxx
CSeq: 2 MESSAGE
Content-Type: application/MANSCDP+xml
Content-Length:   160

<?xml version="1.0" encoding="UTF-8"?>
<Response>
	<CmdType>Alarm</CmdType>
	<SN>2</SN>
	<DeviceID>xxx</DeviceID>
	<Result>OK</Result>
</Response>

```



##### 报警流程

1. 设备端主动发送报警
2. 服务端返回200 OK.
3. 服务端返回Result为OK
4. 设备端返回200 OK



### 图像抓拍

​		图像抓拍是设备控制功能的一个应用，使用设备控制的流程，增加图像抓拍的功能字段。

- 图像格式宜使用JPEG；
- 图像分辨率宜采用与主码流相同的分辨率；
- **图像传输方式宜使用http；**
- 图像抓拍传输完成中的SessionID应与图像抓拍配置中的SessionID一致；
- 抓拍图像文件命令宜遵循如下格式：

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/c7e521901031b7f7917b003d2744d230.jpeg)

#### 抓拍流程

![在这里插入图片描述](https://raw.githubusercontent.com/Mocearan/picgo-server/main/4360df2686d6000829e580963277ea75.jpeg)

- SIP服务器（mysipsdk.dll）向设备端发送图像抓拍配置命令；

  ```xml
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<Control>"
      "<CmdType>DeviceConfig</CmdType>"
      "<SN>%s</SN>"
      "<DeviceID>%s</DeviceID>"
      "<SnapShotConfig>"
          "<SnapNum>%d</SnapNum>"
          "<Interval>%d</Interval>"
          "<UploadURL>%s</UploadURL>"
          "<SessionID>%s</SessionID>"
      "</SnapShotConfig>"
  "</Control>"
  
  SnapNum:连拍张数,最多10张,当为手动抓拍时取值为1
  Interval:单张抓拍间隔时间,单位:秒,最短为1秒
  UploadURL:抓拍图像上传路径,例如:http://192.168.1.121/uploadsnapshot?token=xxx
  SessionID:会话ID,由平台生成,用于关联抓拍的图像和平台请求,SessionID由大小写英文字母、数字、短划线组成，长度32~128字节
  ```

  ```c++
  int CGBDeviceConfig::SnapShotConfig_(CMyGBDevice* device, const std::string& deviceID, const std::string& request)
  {
  	if (!device || deviceID.empty() || request.empty())
  		return -1;
  
  	Json::Value value;
  	Json::Reader reader;
  	if (!reader.parse(request.c_str(), value))
  		return -1;
  
  	std::string sn = GetConfigSN();
  	std::string uploadURL = value["uploadURL"].asString();
  	std::string sessionID = value["sessionID"].asString();
  	int snapNum = value["snapNum"].asInt();
  	int interval = value["interval"].asInt();
  
  	char snapShotInfo[500] = { 0 };
  	snprintf(snapShotInfo, 500,
  		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
  		"<Control>\n"
  		"<CmdType>DeviceConfig</CmdType>\n"
  		"<SN>%s</SN>\n"
  		"<DeviceID>%s</DeviceID>\n"
  		"<SnapShotConfig>\n"
  		"<SnapNum>%d</SnapNum>\n"
  		"<Interval>%d</Interval>\n"
  		"<UploadURL>%s</UploadURL>\n"
  		"<SessionID>%s</SessionID>\n"
  		"</SnapShotConfig>\n"
  		"</Control>\n"
  		, sn.c_str()
  		, deviceID.c_str()
  		, snapNum
  		, interval
  		, uploadURL.c_str()
  		, sessionID.c_str()
  	);
  
  	return CMySipContext::GetInstance().SendSipMessage(device, snapShotInfo);
  }
  ```

- 设备端返回200 OK；

- 设备端返回图像抓拍配置响应；

- SIP服务器（mysipsdk.dll）返回200 OK；

- 设备端将图像数据传输至图片存储服务器；

- 设备端发送图像抓拍传输完成消息；

- SIP服务器（mysipsdk.dll）返回200 OK；

  ```c++
  bool CDeviceConfigMethod::OnRecvMsg(pjsip_rx_data* rdata)
  {
  	if (PJSIP_OTHER_METHOD == rdata->msg_info.cseq->method.id)
  	{
  		CMyXmlParser xmlParser(CMySipContext::GetInstance().GetMessageBody(rdata));
  		CMyDynamicStruct dynamicStruct;
  		dynamicStruct.Set(xmlParser.GetXml());
  
  		auto cmd = xmlParser.GetXml()->RootElement()->Value();
  		auto cmdType = dynamicStruct.Get("CmdType");
  		if ("UploadSnapShotFinished" == cmdType)
  		{
              /*
              CmdType:固定为字符串"UploadSnapShotFinished"
              SessionID:会话ID,由平台生成,用于关联抓拍的图像和平台请求
              SnapShotList:抓拍图像标识列表
              SnapShotFileID:抓拍图像唯一标识,由前端抓拍设备生成
              */
  			CSnapShotFinishInfo snapShotInfo;
  			snapShotInfo.deviceID = dynamicStruct.Get("DeviceID");
  			snapShotInfo.sessionID = dynamicStruct.Get("SessionID");
  			std::string snapShotList = dynamicStruct.Get("SnapShotList");
  
  			Json::Reader reader;
  			Json::Value value;
  			reader.parse(snapShotList, value);
  			int size = value.size();
  			for (int i = 0; i < size; i++)
  			{
  				Json::Value val = value[i];
  				std::string snapShotfileID = val["SnapShotFileID"].asString();
  				snapShotInfo.snapShotFileIDs.push_back(snapShotfileID);
  			}
  
              // 回调至应用层,需注册
  			if (m_dataCB)
  				m_dataCB(m_handleType, m_user, &snapShotInfo);
  		}
  		else
  		{
  			return false;
  		}
  
  		Response(rdata, PJSIP_SC_OK, NoHead);
  		return true;
  	}
  
  	return false;
  }
  ```

  

