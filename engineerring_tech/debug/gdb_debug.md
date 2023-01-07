# gdb Debug

---

​		

## 断点

### 设置断点

![image-20220612164348880](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612164348880.png)

​		对于重载的同名函数，如不指定参数，则所有重载都会被打上断点。

​		`watch`并非纯粹的观察，本质上也是一个断点，并且会将断点前的值和断点触发时的值都打印出来。



### 查看断点

![image-20220612165155905](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612165155905.png)

### 删除断点

![image-20220612165212964](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612165212964.png)



### 保存断点

![image-20220612170023413](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612170023413.png)





## 执行

### 启动

![image-20220612233529724](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612233529724.png)

### 停止

![image-20220612233604852](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612233604852.png)

### 执行

![image-20220612233546118](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220612233546118.png)



## 查看

### 查看栈

![image-20220613001134906](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220613001134906.png)



### 查看源码

![image-20220613001152076](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220613001152076.png)



### 查看变量

![image-20220613002416006](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220613002416006.png)



## 远程调试

### 直接调试

- gdbserver 启动监听端口到要调试的进程

  ```shell
  gdbserver ip:port proccess_name
  ```

- gdbclient 进入gdb-cmd，然后连接到gdbserver

  ```shell
  gdb
  (gdb) target remote ip:port
  ```

- gdbclient连接后，不能使用`run`来运行进程，因为进程已经被gdbserver启动并挂起，使用`continue`使得进程继续运行

### 附加进程调试

- 启动监听

  ```shell
  gdbserver ip:port --attach pid
  ```







### vs远程调试linux

​		附加进程调试，ssh



## release版本调试

​		在发布release版本时，同时生成一个带``-g``调试信息表的版本，用作符号源的提供者。

`release_bin` / `release_g_bin`

### 抽取调试信息，给release_bin版本使用

- 抽取

  ```shell
  objcopy --only-keep-debug release_g_bin bin.symbol
  ```

- 使用调试信息文件进行调试

  ```shell
  gdb --symbol bin.symbol --exec release_bin
  ```

​	可能还是会有信息的缺失



### 直接使用release_g_bin进行调试

```shell
gdb --symbol release_g_bin --exec release_bin
```



## coredump调试分析

​		

###  死循环

- `gdb attach pid`
- `(gdb)gcore pid.core`
- `detach`
- `gdb process pid.core`
  - 如果需要，可以使用带`-g`的进行调试



### 死锁

- `gdb attach pid`
- `(gdb)gcore pid.core`
- `detach`
- `gdb process pid.core`
  - 如果需要，可以使用带`-g`的进行调试
- `i thread`
  - 查看是否`lock_wait`



### 段错误

- `ulimit -c unlimited`
- `process`
  - 自动生成`pid.core`
- `gdb process pid.core`
- `(gdb)bt`



