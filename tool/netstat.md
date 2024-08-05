# netstat

---





## 参数

- `-a (all)`：显示所有选项，默认不显示LISTEN相关
- `-t (tcp)`：仅显示tcp相关选项
- `-u (udp)`：仅显示udp相关选项
- `-n` ：以数字形式显示地址信息。拒绝显示别名，能显示数字的全部转化成数字。
- `-l` ：仅列出有在 Listen (监听) 的服務状态

> 提示：`LISTEN`和`LISTENING`的状态只有用-a或者-l才能看到

- `-p` ：显示建立相关链接的程序名
- `-r` ：显示路由信息，路由表
- `-e `：显示扩展信息，例如uid等
- `-s` ：按各个协议进行统计
- `-c` ：每隔一个固定时间，执行该netstat命令。



## 状态说明

- `LISTEN`： 侦听来自远方的TCP端口的连接请求
- `SYN-SENT`： 再发送连接请求后等待匹配的连接请求
- `SYN-RECEIVED`：再收到和发送一个连接请求后等待对方对连接请求的确认
- `ESTABLISHED`： 代表一个打开的连接
- `FIN-WAIT-1`： 等待远程TCP连接中断请求，或先前的连接中断请求的确认
- `FIN-WAIT-2`： 从远程TCP等待连接中断请求
- `CLOSE-WAIT`： 等待从本地用户发来的连接中断请求
- `CLOSING`： 等待远程TCP对连接中断的确认
- `LAST-ACK`： 等待原来的发向远程TCP的连接中断请求的确认
- `TIME-WAIT`： 等待足够的时间以确保远程TCP接收到连接中断请求的确认
- `CLOSED`： 没有任何连接状态
- `SYN_RECV`：表示正在等待处理的请求数；
- `ESTABLISHED`：表示正常数据传输状态；
- `TIME_WAIT`：表示处理完毕，等待超时结束的请求数。



## cases

- 查看哪些IP连接本机

  ```shell
  netstat -anp
  ```

- 统计端口连接数

  ```shell
  netstat -nat | grep -i "80" | wc -l
  ```

- 统计httpd协议连接数

  ```shell
  ps -ef | grep httpd | wc -l
  ```

- 统计已连接上的，状态为`established`

  ```shell
  netstat -anp | grep ESTABLISHED | grep 'tcp' |wc -l
  ```

- 查出哪个IP地址连接最多，将其封了

  ```shell
  netstat -anp | grep ESTABLISHED | awk {print $5}|awk -F: {print $1} | sort | uniq -c | sort -r +0n
  netstat -anp | grep SYN | awk {print $5}|awk -F: {print $1} | sort | uniq -c | sort -r +0n
  
  # awk '/^tcp/ {print $5}'     :筛选以tcp开头的行，并打印5th filed.
  # awk -F: '{print $1}'        :以冒号为字段分隔符，打印第一个字段(-F fs 指定行中分隔数据字段的字段分隔符)
  # uniq -c                     :去除重复项目，-c 进行计数
  # sort -rn                    :进行排序，-r 反向排序 -n 使用纯数字进行排序
  ```

- 统计每个ip的连接数

  ```shell
  netstat -n | awk '/^tcp/ {print $5}' | awk -F: '{print $1}' | sort | uniq -c| sort -rn
  ss -n | awk '/^tcp/ {print $5}' | awk -F: '{print $1}' | sort | uniq -c| sort -rn
  ```

- 看服务器的TCP连接状态并汇总

  ```shell
  netstat -ant | awk 'NR>=3 {++State[$6]} END {for (key in State) print key,State[key]}'
  ss -ant | awk 'NR>=2 {++State[$1]} END {for (key in State) print key,State[key]}'
  netstat -an|awk '/^tcp/{++S[$NF]}END{for (a in S)print a,S[a]}'
  
  # /^tcp/：滤出tcp开头的记录，屏蔽udp, socket等无关记录。
  # S[]：相当于定义了一个名叫S的数组
  # NF：The number of fields in the current input record.(表示输入数据的字段总数，如上所示的记录，NF等于6)
  # $NF：表示某个字段的值，如上所示的记录，$NF也就是$6，表示第6个字段的值，也就是TIME_WAIT
  # S[$NF]：表示数组元素的值，如上所示的记录，就是state[TIME_WAIT]状态的连接数
  # ++S[$NF]：表示把某个数加一，如上所示的记录，就是把state[TIME_WAIT]状态的连接数加一
  # END：表示在最后阶段要执行的命令
  # for(key in S)：遍历数组
  ```

- 查看系统连接状态统计

  ```shell
  netstat  -ant | fgrep ":"| cut -b 70-90 | sort | uniq -c
  ```

- 性能测试并发请求数及其TCP连接状态

  ```shell
  netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
  ```

  