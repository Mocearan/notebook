# top

---

​		top是一个性能分析工具，能够实时显示系统中各个进程的资源占用情况，用来分析进程运行过程中的性能问题。

```shell
top - 16:56:20 up 425 days, 5:32,  0 users,       load average: 0.00, 0.01, 0.05
  # 系统当前时间  系统运行时间     当前用户登录数      系统负载    1min 5min 15min 均值归一值
Tasks: 124 total,   1 running, 123 sleeping,   0 stopped,   0 zombie
# 		进程数	   活跃进程数    休眠进程数      停止进程数    僵尸进程数   
%Cpu(s):  0.4 us,  0.3 sy,     0.0 ni,          
#CPU占比  用户空间  系统空间   用户进程空间中改变过优先级的进程
			99.3 id,  0.0 wa,        0.0 hi,       0.0 si,         0.0 st
	#      空闲进程占比 等待I/O占比  处理硬中断占比   处理软中断占比   有虚拟cpu的情况
	#    （反映系统忙闲）
KiB Mem :  8008632 total,   748744 free,       1099364 used,  6160524 buff/cache
# 内存数据     物理内存总量   空间内存总量    使用的物理内存量     用作内核缓存的内存量 
KiB Swap:        0 total,        0 free,        0 used.      6594016 avail Mem 
# 交换区数据    交换区总量    交换区空闲量     使用的交换区量   缓冲的交换区总量

  PID   USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND         
#进程ID 运行用户 优先级   虚拟内存用量   共享内存用量 CPU/内存占比i   累计CPU占用时间
#				任务nice值     物理内存用量  进程状态   				运行命令
 1173 root      20   0   50088    960    624 S   1.3  0.0   8463:46 rshim                             
23913 root      20   0  696980  37424  16120 S   1.3  0.5   0:07.16 node                             
28220 root      20   0 1010048  65156  17236 S   1.0  0.8 304:42.06 YDService                         
25220 root      20   0  161980   2208   1568 R   0.3  0.0   0:00.01 top                               
26198 mysql     20   0 1759724 172932   2840 S   0.3  2.2  56:01.83 mysqld                           
26747 root      20   0  688996  16488   3312 S   0.3  0.2 332:32.87 barad_agent                      
```

- `buffers` 是指块设备的读写缓冲区
- `cached`是文件系统本身的页缓存

`htop`是`top`的增强版本。