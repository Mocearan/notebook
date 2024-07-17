# tcpdump

[tcpdump抓包技巧_tcp抓包-CSDN博客](https://blog.csdn.net/locahuang/article/details/135106427)

[tcpdump 抓取指定多个端口的包_百度搜索 (baidu.com)](https://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&tn=68018901_3_dg&wd=tcpdump 抓取指定多个端口的包&oq=tcpdump%20%E6%8A%93%E5%8F%96%E6%8C%87%E5%AE%9A%E7%AB%AF%E5%8F%A3%E7%9A%84%E5%8C%85&rsv_pq=95ee6dc900016e1b&rsv_t=327e80%2FmHVPA7qMXTZo2wlqRSdNoueeEB18Cd89GZvXiKOWxaNjnYBP55Ms64h4hCfWJEA&rqlang=cn&rsv_dl=tb&rsv_enter=1&rsv_sug3=8&rsv_sug1=4&rsv_sug7=100&rsv_sug2=0&rsv_btype=t&inputT=702&rsv_sug4=2444)

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

