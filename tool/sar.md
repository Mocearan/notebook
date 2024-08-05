# sar

---



## cases

- 查看socket队列信息

  ```shell
  $ sar -n SOCK 5
  
  Linux 3.10.0-1160.76.1.el7.x86_64 (instance-u7c2pyc5) 	08/05/2024 	_x86_64_	(2 CPU)
  
  04:12:18 PM    totsck    tcpsck    udpsck    rawsck   ip-frag    tcp-tw
  04:12:23 PM      1679       555         6         0         0        61
  ```

  