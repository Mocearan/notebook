# tcpdump



## 用法

### 使用Wireshark实时浏览

```bash
ssh root@remote_ip'sudo tcpdump -i any 'port 5066' -XX -nn -vv' | .\Wireshark.exe -k -i -
```

### 按端口范围抓取

```bash
sudo tcpdump -i any '(port 20000) or (port 21000)' -XX -nn -vv
sudo tcpdump -i any port 5060 or portrange 20000-21000 -XX -nn -vv
```

### 指定原地址

```bash
sudo tcpdump -i any src host 192.168.1.1
```

### 指定目的地址

```bash
sudo tcpdump -i any dst host 192.168.1.1
```

### 指定源端口

```bash
sudo tcpdump -i any src port 8080
```

### 指定目的端口

```bash
sudo tcpdump -i any dst port 8080
```

### 按网络过滤

```bash
sudo tcpdump -i any net 192.168
sudo tcpdump -i any src net 192.168
sudo tcpdump -i any dst net 192.168
```

### 按协议过滤

```bash
sudo tcpdump -i any arp
sudo tcpdump -i any ip
sudo tcpdump -i any tcp
sudo tcpdump -i any udp
```

