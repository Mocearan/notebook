# ps

​		process status

----

​		列出运行`ps`命令时刻的进程快照，是`top`的静态版本。

​		使用`ps`命令查看程序运行的状态、结束状态、占用资源等。

> linux中有五种进程状态：
>
> - R：运行（正在运行或在运行队列中等待）
> - S：中断（休眠、受阻，等待某个条件的形成或接收到信号）
> - D：不可中断（收到信号不唤醒和不可运行，进程必须等待直到有中断发生）
> - Z：僵死（进程中止，但进程资源未回收，如进程描述符，需要父进程回收后释放）
> - T：停止（进程收到`SIGSTOP/SIGSTP/SIGTIN/SIGTOU`信号后停止运行

## 参数

- `a`，显式所有进程

- `-a`，显示同一终端下的所有程序

- `-A`，显示所有进程

- `e`，显示环境变量

- `f`，显示程序间的关系

  - 常用`-ef`

- `-H`，树状结构

- `r`，显示当前终端的进程

- `T`，显示当前终端的所有程序

- `u`，指定用户的所有进程

- `-au`，显示较详细的咨询

- `-aux`，显示所有包含其他使用者的进程

  - VSZ，虚拟内存用量
  - RSS，固定内存占用
  - TTY，终端机
  - STAT，程序状态

- `-C`，列出指定命令的状况

- `-l`，显示进程信息

  - F， 进程的用户级别

  - S，程序状态

  - UID，用户ID

  - PID，进程id

  - PPID，父进程id

  - C，CPU资源占比

    

- `--help`

- `--version`