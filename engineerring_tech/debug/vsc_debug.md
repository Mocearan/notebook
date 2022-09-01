# VSC Debug

---



- F5进行调试
- 配置平台
- 配置编译器



## lauch.json

​		程序启动配置。

```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "sms", 									// `name` 启动项名字
            "type": "cppdbg",
            "request": "launch",
            "program": "sms",								// `program`，要启动的二进制程序路径
            "args": [],										// `args`，二进制程序的命令行参数
            "stopAtEntry": false,
            "cwd": "run",									//`cwd`，cammond work directory 命令工作目录，执行`program`的目录
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",								// `MIMode`，调试工具, linux下允许为 gdb, lldb
            "setupCommands": [								// 调试器的命令行参数
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build",						// `preLauchTask`，启动前执行的任务
            											  // - 可以执行一个``task.json``,  以`task.json`的`label`标识
            "miDebuggerPath": "/usr/bin/gdb"				// `miDebuggerPath`，调试器路径
        }
    ]
}
```



## task.json

​		任务配置。

```json
{
    "tasks": [
        {
            "type": "shell",							// 任务类型, shell脚本任务或进行proc
            "label": "build",							// 任务名
            "command": "script/build.sh",				 // 任务使用的命令
            "args": [								   // 传入命令的命令行参数
                "build",
                "Debug"
            ],
            "options": {							// 选项
                "cwd": "script"						// 命令工作目录
            },
             "group": {
                 "kind": "build",
                 "isDefault": true
             },
            "detail": "调试器生成的任务。"
        }
    ],
    "version": "2.0.0"
}
```



## vsc 启动调试

​		以直接调试的方式运行程序。

​		通过配置`launch.json`，添加一个`request`为`launch`的`configurations`子项。



## vsc 附加进程调试

​		以附加到已经运行的进程上调试的方式运行程序。

​		通过配置`launch.json`，添加一个`request`为`attach`的`configurations`子项。在调试插件里，选择启动方式为附加进程启动。



## vsc多线程调试

​		多线程调试主要是在多线程的栈帧之间切换，查看调用栈和调用栈上的变量值。



## 条件断点

​		添加断点后，右键断点，选择表达式，在表达式结果为真时中断。



## gdb命令

​		在调试控制台中，通过`-exec <command>`的方式来使用gdb命令来辅助调试。

`-exec print n`

- vsc中需要使用gdb命令来查看内存布局





