# vsc配置体系

---

​		vsc的最小配置单元是文件夹，可以给不同的文件夹配置不同的外部编译器、不同的快捷命令任务、快速debug和其他插件的拓展功能等。



## vsc的配置结构

​		vsc的编译器配置文件为`settings.json`，用以配置编辑器和相关插件。由`user settings / workspace settings / folder settings`进行不同维度和粒度的定制化配置。

​		一个文件夹下可以通过配置`.vscode`文件夹使得其成为一个vsc项目，`.vscode`中可以通过`settings.json`来定制该文件夹粒度的配置。通过`tasks.json`配置编译任务，`launch.json`来配置运行。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/24563956-3a02d30da47b6b49.png)





## settings.json

​		全局`user`设置通过`ctrl+shift+p`输入`settings.json`打开，或者在设置的`用户设置`。

​		更局部的设置会遮蔽更全局的设置。

​		一般在更全局的设置中进行更普适的配置，在更局部的设置中进行更定制化的设置。

​		比如可以在`用户设置`中进行字体等设置，`工作区设置`中进行适合当前机器等操作习惯和方便的配置，在`文件夹配置`中配置项目相关的环境。



## tasks.json

​		`tasks.json`用以支持一些命令的编辑，类似于脚本。

​		有些拓展插件会有封装好的task可以直接执行,我们也可以自定义各种各样的task,例如实现“编译当前文件”,“删除多余文件”等等操作。

​		`tasks.json`只能在当前的`folder space`起作用，即包含`.vscode`的目录下，即一个项目。



## `launch.json`

​		对项目的启动等进行配置，不同的语言拓展插件会提供不同的模板。

​		`F5`可以选择模板并生成对应的`launch.json/tasks.json`