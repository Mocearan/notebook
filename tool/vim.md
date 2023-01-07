#	Vim All In One

Vim: Vi Improved.

---
## Overview

​		Vim 是一个编辑器。

- 完全跨平台
- 高度可定制、可扩展
- 有着良好生态环境 

​		Vim最初是个针对字符界面的编辑器，后来支持了主流的图形界面，包括Windows的图形界面、linux的GTK，macOS的cocoa和carbon。

### 主要内容：

- Vim的安装
- Vim的模式和命令
- Vim的配置
- Vim的使用技巧
- Vim里的重要插件
- Vim集成到工作流

### 推荐书籍
- Vim使用技巧
- the vim primer
- modern vim
- Vim8文本处理实战

> window上装一个纯英文的输入法（美国键盘）用来写程序，而不是在中文输入法中用Shift来切换，这不太靠谱。


---
## Vim的安装

### Linux
#### Redhat & Centos
​		默认安装的Vim可能是一个最小功能的版本。
> yum list installed | grep vim 查看已经安装的Vim版本
> - vim-minimal 最小版本
> - vim-enhanced 增强版本
> - vim-X11 图形界面版本

​		使用图形界面的linux推荐使用图形界面支持的vim，否则尽量使用增强版的vim。

#### Debian & Ubuntu
> 版本
> - vim
> - vim-Athena  适用于较老的X-window界面
> - vim-gnome
> - vim-gtk		适用于KDE环境
> - vim-gtk3
> - vim-nox		较全功能的文本界面
> - vim-tiny  最小功能

- 如果使用标准的gnome面环境，安装vim-gtk3或vim-gnome。
- 使用KDE桌面安装vim-gtk
- 只用文本界面安装vim-nox

`apt list --installed | grep vim`
查看已经安装的Vim版本
- 先`sudo apt update`确保更新环境
- 然后`sudo apt install vim-gtk3`安装gtk3版本的Vim（或其他版本）
- 图形版本的Vim可以纯文本启动
- Ubuntu上，vim和gVim是同一个可执行程序的符号链接，且gVim的执行效果和vim -g相同

#### 源码编译
​		linux发行版的版本较老，则其包管理器不能更新到最新的软件。所以需要通过源码进行手工的编译。
​		Vim有很多的编译配置选项，有些缺省是不打开的。比如说想添加上Python支持和图形界面：

```shell
./configure --enable-pythoninter \
			--enable-python3interp \
			--enable-gui=auto
make -j
make install
```

​		Vim 将被安装到/usr/local下。可以用`which vim`来检查系统是否会自动优先选择你的Vim。
​		如果不是的话，可能需要调整一下PATH的顺序，或者设置别名来启动/usr/local/bin/vim.

​		`vim --version`用于输出vim的版本信息。
​		如上安装过程可以找到啊如下信息：

```shell
Huge version with ... GUI
+python/dyn
+python3/dyn
```
​		目前python2已经停止支持，所以醉倒保证还有相应python3（很多vim插件已经开始要求python3，且不再支持python2了）

### macOS下的安装
​		macOS中一般已经内置了vim，并提供了除图形界面外的较完整的功能。
​		如果希望使用图形界面，则需要自行安装MacVim，一个跟现代macOS融合较好的独立Vim版本。有两种安装方式：

- HomeBrew。推荐，升级容易。
- MacVim独立安装包。没有使用HomeBrew或不方便使用其网络环境时，可以使用这种方式。

​		使用HomeBrew足够狗简单且升级方便，没有必要自己去编译MacVim。

#### HomeBrew安装MacVim

#### MacVim安装包


### Windows下的安装
​		windows默认不带有Vim，可以从Vim官网下载对应安装包：
​		https://www.vim.org/download.php#pc

#### 原生Window本地应用

> 在Linux和macOS上，64bit应用已经成为主流，而在64bit的windows操作系统上，32bit应用仍然很常见。
> 默认的Vim8安装包仍然是一个32bit，能够满足一般的需求，除非想要编辑2GB以上的文件。

#### Cygwin/MSYS2

#### 远程使用Vim
​		这大概是一种最常用的环境，即通过mintty、PuTTY、SecureCRT、xshell等终端模拟软件进行远程连接。
​		这种情况下如要注意的是，远程终端软件的远程字符集（如PuTTY中的`Windows>Translation>Remote character set`应当设置成UTF-8。
​		具体跟软件和版本有关，查阅所使用的远程终端软件的设定和相关文档。

### 键盘重映射
​		有些情况下可能会依据自己的键盘使用习惯来重新映射键盘键位。比如将使用频率较低的Caps Lock重映射为Esc或Ctrl等。
​		[linux下将大写锁定重映射](https://blog.csdn.net/daerzei/article/details/89414610)
​		[macbook更改大写锁定为esc](https://blog.csdn.net/tbestcc/article/details/52287622)
​		[windows交换大写锁定与esc（注册表）](https://blog.csdn.net/P_LarT/article/details/72829425)
​		[任何系统上禁用或重新分配大写锁定](https://www.kutu66.com/Mac/article_11233)



## Vim键盘操作

​		Vim内置了完整的英文帮助文件，如果需要中文帮助文件的话，需要自行安装。在Unix和WIndows上类似，只是.vim路径在windows上为vimfiles：
```shell
cd ~/.vim
mkdir -p pack/my/start
git clone https://github.com/yianwillis/vimcdoc.git pack/my/start/vimcdoc
```
​		如果后续不需要通过Git来快速升级该中文文档，可以在https://github.com/yianwillis/vimcdoc的https://github.com/yianwillis/vimcdoc/releases
​		下载tar包，自行解压到`~/.vim/pack/my/start`目录下或windows下相应的目录中。
​		windows下有一个简单的安装程序，可以自行安装，机器上没有git和tar程序的话，这个方式最简单。https://github.com/yianwillis/vimcdoc/releases/download/v2.3.0/vimcdoc-2.3.0-setup-unicode.exe



### Vim教程

​		Vim在安装中自带了一个教程，可供快速入手使用。
​		Vim教程支持多语言，可使用命令`vimtutor`来启动，如果希望指定以特定语言启动vimtutor，可以通过环境变量LANG来指定：`LANG=zh_CN.UTF-8 vimtutor`，或者在配置文件中以别名进行绑定。



### vim的模式

Vim下有四种编辑状态：

-   正常模式（normal， 也称普通模式）

    缺省的编辑模式，一般的操作命令都是这个模式下键入。在任何其他的模式下，都可以通过`<Esc>`回到命令模式。

-   插入（insert）模式

    输入文本时使用，比如在正常模式下键入 i （insert）或a （append）即可进入插入模式

-   可视模式（visual）

    用于选定文本块。可以通过v进入字符可视模式，V进入行可视模式。

-   命令模式（command-line）

    用于执行较长、较复杂的命令。在正常模式下键入冒号即可进入该模式。

    (`/`,`?`尾行搜索大致也可算作一种命令行模式。)

-   选择模式（select）

    选择内容后再输入任何内容，将会替换选择的内容。这个模式主要是为了模仿windows编辑器的行为。在可视模式和选择模式之一选定文本块后可以使用`<Ctrl-G>`切换到另一模式。

### 操作、操作对象（动作）

​		本节讨论基于正常模式

​		许多改变文本的命令都有一个操作符、一个重复次数和一个动作构成。

​		`op number motion`

- op 操作符，是指对文本施加的操作
- number 计数，实质对motion动作重复多少次
- motion 操作符的操作对象，是指光标的移动动作

**缺省op表示只做motion，即移动光标，否则表示对从当前字符直到motion移动到的地方施加op。**

**缺省number表示motion只做一次，词法上修饰motion，有number没motion没有意义**

**单字符操作、无字符操作、组合键操作缺省motion，故number也缺省**

**多字符操作不能缺省motion（不能确定范围），number可选**

**行操作是默认以行的motion执行op，其虽然字面上像op,但实际上是motion的属性，所以前面可以加number。**



#### 操作 op

​		对文本施加的某种操作。

##### 单字符、无字符 （无motion）

 - 插入
 > - `a` 追加到当前字符后
 > - `A` 追加到当前行后，相当于`$i`
 > - `i` 在当前字符前插入
 > - `I` 在当前行第一个非空字符前插入，相当于`^i`
 > - `o` 在当前行下添加一个新行并插入
 > - `O` 在当前行上添加一个新行并插入
 > - `s` 删除当前字符并插入，相当于`cl`
 > - `S` 删除当前行并插入，相当于`cc`
- 删除
 > - `x` 删除当前字符并保留在vim剪切板
 > - `X` 删除当前字符的前一个字符并保留在vim剪切板
 > - `D` 从当前字符开始删除到行尾并保留在vim剪切板，相当于`d$`
 > - `C` 从当前字符开始删除到行尾并进入插入模式，相当于`c$`

-   撤销

>   - `u` 撤销上一次执行的操作
>   -  ` U` 撤销对整行的操作 

- 替换

>    - `r` 替换当前字符
>    - `R` 从当前字符开始连续替换

 - 粘贴

>   - `p`  将最后一次删除的内容粘贴到光标后或下一行
>   -   `P` 将最后一次删除的内容粘贴到光标前或上一行

-   搜索

>   -   `/{mode}` 从文件开始正则搜索{mode}指定的文本
>   -   `?{mode}` 从文件末尾正则搜索{mode}指定的文本

#####  多字符操作  （对光标移动选中的内容进行操作，需motion）

> - `d` 删除操作符
>
> - `y` 拷贝操作符
>
> - `c` 修改操作符
>   不进删除光标移动选中的内容，而且进入插入模式
>
> - `v` 选择操作符
>   不同于上述，会进入可视模式，单仍可加上number 和 motion进行操作。
>
> - `V` 行选择操作符
>
>   会以行模式进入可视模式，仍可加上number 和 motion移动光标，但是其实行到光标所在行的全部整行将被选中。

##### 行操作（无motion）

> - `dd` 删除行
> - `yy` 拷贝行
> - `cc` 修改整行（删除整行，并在第一列开始插入）
> - `Y` 复制行
> - `U`  撤销整行中的更改



#### 操作对象 motion 

**移动光标, motion都可以被number修饰**

字符

 - `hjkl` 左下上右

单词

 - `w` 从当前字符直到下一个单词头（单词：字母、数字、下划线组成烦人序列）
 - `W` 从当前字符直到下一个单词头（单词：空格隔开的字符序列）
 - `b` 从当前字符直到单词头(begin)（单词：字母、数字、下划线组成烦人序列）
 - `B` 从当前字符直到前一个空格（单词：空格隔开的字符序列）
 - `e` 从当前字符直到后一个空格前(end)

行

 - `$` 从当前字符直到当前行尾，`<End>`功能基本一致
 - `0` 从当前字符直到当前行首，`<Home>`功能基本一致
 - `^` 从当前字符直到当前行第一个非空白字符
 - `|` 从当前列直到指定列，缺省到当前行第一列

句

-   `s` (sentence)对句子进行操作

段

-   `p` (paragraph) 对段落进行操作

页

-   `H` 从当前字符到当前页首行第一个字符
-   `M` 从当前字符到当前页中央行第一个字符
-   `L` 从当前字符到当前页末行第一个字符

>   首行末行的定义受`set scrolloff=xx`的影响，最顶上和最底部的xx行跳不到。这时H/L相当于（xx+1）H/（xx+1）L了。
>
>   所以一般设置为`set scrolloff=1`，减少翻页对这个设置的干扰。可以设为0，但一般翻页后，应该留下一点上一页的信息用作定位。

章

 - `G` 从当前字符直到指定行，缺省到尾行第一个字符
 - `gg` 从当前字符直到首行第一个字符
 - `n` 从当前正则匹配的搜索项向搜索正向跳转
 - `N` 从当前正则匹配的搜索项向搜索反向跳转
 - `t` 对HTML/XML 标签进行操作

特殊字符

 - `%` 从当前字符开始在当前行向后查找`()/[]/{}`括号中任意一个半括号，并移动发哦到其匹配的另一半括号处（全文范围）
 - `(` 向前跳转一句
 - `)` 向后跳转一句
 - `{` 向前跳转一个段落，直到遇到空行
 - `}` 向后跳转一个段落，直到遇到空行

#### number

motion范围修饰和motion重复次数修饰可结合使用

**motion重复次数修饰**

 - 1/缺省    缺省或使用a/1代表1次
 - 2及以上   表示重复动作若干次

**motion范围修饰**

 - a    光标在单词内部时，表示从单词首字符直到下一个单词首字符前的字符，包括空格

 - i	(inner)光标在单词内部时，表示从单词首字符直到单词末字符，不包括空格

    >   对`a`和`i`适用的内部边界：
    >
    >   `w/W` 词边界（包含/不包含空格）内部
    >
    >   `s` 句边界内部
    >
    >   `p` 段边界内部
    >
    >   ```shell
    >   ', ", `  单双引号、反引号内部
    >   ```
    >
    >   `t` HTML/XML 标签内部
    >
    >   `< / [ / ( / {` 尖括号，方括号，圆括号，花括号边界内部。
    >
    >   	如`c2i{`
    >
    >   
    >
    >   缺省限定， 一般视作从当前字符到词边界后的空格

 - t    (till)直到找到其后紧邻输入的字符（输入占motion位），不包含该字符

 - T   反向~

 - f    (find)查找到其后紧邻输入的字符（输入占motion位）， 包含该字符

 - F   反向~



### 功能键及组合键操作 (Ctrl后的大写为实际输入是小写)

以下包括光标跳转的操作，与motion语境不同，不能与op合用。

> - `<Ctrl-R>` redo重做
> - `<Ctrl-G>` 显示所在行位置以及文件状态信息， 并留下一个标记

-   跳转

> - `<Ctrl-O>` 回退光标到相对于当前较老的跳转点
> - `<Ctrl-I>` 前跳光标到相对于当前较新的跳转点
> - `<Ctrl-Home>` 跳转到文件第一个字符
> - `<Ctrl-End>` 跳转到文件最后一个字符

-   滚屏

> - `<Ctrl-E>` 固定光标向下滚屏。（光标超屏则被顶下，vim中光标不能离开当前页焦点）
> - `<Ctrl-Y>` 固定光标向上滚屏。（同上）
> - `zt` 固定光标，向下滚屏一页，光标在顶（top）
> - `zz` 固定光标，滚屏到光标在当前页焦点中央行
> - `zb` 固定光标，向上滚屏一页，光标在底（bottom）

-   翻页

>   - `<Ctrl-U>` 向上跳转半页
>   - `<Ctrl-D>` 
>       - 正常模式，向下跳转半页
>       - `:`， 
>       - `:e` ，展示当前目录下e可打开的文件/目录，并可用<Tab>自动补全
>   - `<PageUp>/ <Ctrl-B>`  上翻页
>   - `<PageDown>/ <Ctrl-F>`  下翻页 

-   重复

>   -   `;`  重复最近的字符查找（`f,t`等）
>   -   `,` 重复最近的字符查找，反向
>   -   `n`， 重复最近的字符串查找（`/,?`）
>   -   `N`， 重复最近的字符串查找（`/,?`），反向
>   -   `.`， 重复执行最近的修改操作

### 命令

在一般模式下输入`:`进入命令模式

- 进入 （在shell中）
> vim {file_list}

- 退出
> :q 直接退出
> :q 强制退出
> :wq 保存并退出
> :qa 退出所有窗口
> :ZZ 存盘并退出
> :`<Ctrl-Z>` 终止进程退出

- 写文件
`:w FILENAME` 将对文件的改动保存到指定文件中，本文件的buffer改动，但文件本身没有改变
可以将`v number motion`选中的内容通过`w FILENAME`写入指定文件
- 读文件
`:r FILENAME` 将指定文件的内容读取到光标所在处。
`:r !{cmd}`可以将外部命令的输出读取到光标所在处。(`:r !ls`)
- 替换
> :s/{old}/{new} 替换当前行的第一个匹配的{old}为{new}
> :s/{old}/{new}/g 替换当前行的所有匹配的{old}为{new}
> :{n1},{n2}s/{old}/{new}/g 替换n2到n2选中行所有匹配的{old}为{new}
> :%s/{old}/{new}/g 替换全文所有匹配的{old}为{new}
> :%s/{old}/{new}/gc 查找全文所有匹配的{old}并提示是否替换为{new}

- 设置查找忽略大小写
> :set ic  (ignore case)
> :set noic (no ignore case)
> :/{mode}\c 单次查找忽略带小写

- 设置查找高亮
> :set hls(earch)  	(high light search)
> :set nohls(earch) (no high light search)

- 设置部分匹配
> :set is 	(incsearch)
> :set nois

- 查找帮助
> `<F1>`
> :help
> :help {cmd}

在Vim内执行外部命令
`:!`进入命令模式后输入`!`后接外部shell命令。

> 命令补全
> 确保Vim不是在兼容模式下运行（:set nocp），在命令模式下(`:`),按`<Ctrl-D>`可以列出所输入命令部分匹配的所有列表，按`<Tab>`可以逐个以列表中的补全命令提示。如果命令后跟参数，如果是目录文件等可以补全。



## 缓冲区、多文件、多窗口、多标签

### 单文件Vim会话

#### 图形界面的复制与粘贴

将要粘贴的内容放到系统剪贴板之后，有如下方法进行粘贴：

-   正常模式 `“+p`
-   快捷键`<S-Insert>`或Mac的`<D-V>`
-   鼠标右键粘贴
-   菜单“编辑 》 粘贴“

>   Vim中`<Ctrl-V>`不能用来阿粘贴，这个组合已经被Vim用作他用。

#### 终端的复制与粘贴

在插入模式中使用终端提供的粘贴命令，或vim快捷键`<S-Insert>`进行粘贴。



>   上述两种情况，如果Vim版本不够新，在粘贴的内容中含有缩进式，会遇到因无法分辨用户输入和粘贴，导致原文缩进和自动算进冲突。
>
>   ![image-20200829145313444](https://gitee.com/masstsing/picgo-picserver/raw/master/20200829145314.png)
>
>   此时，需要让Vim直到，到底是在输入还是在粘贴。Vim可以在粘贴之前`set paste`切换到粘贴状态，智能缩进、制表符转换等功能就不会修改内容。粘贴完成后需要`set nopaste`关闭粘贴状态。
>
>   ![image-20200829145927175](https://gitee.com/masstsing/picgo-picserver/raw/master/20200829145927.png)
>
>   如果是在Vim 8.0.0210 之前的版本，可以通过代码来使得手工设置paste选项变得不必要。
>
>   ```shell
>   
>   if !has('patch-8.0.210')
>    " 进入插入模式时启用括号粘贴模式
>    let &t_SI .= "\<Esc>[?2004h"
>    " 退出插入模式时停用括号粘贴模式
>    let &t_EI .= "\<Esc>[?2004l"
>    " 见到 <Esc>[200~ 就调用 XTermPasteBegin
>    inoremap <special> <expr> <Esc>[200~ XTermPasteBegin()
>   
>    function! XTermPasteBegin()
>      " 设置使用 <Esc>[201~ 关闭粘贴模式
>      set pastetoggle=<Esc>[201~
>      " 开启粘贴模式
>      set paste
>      return ""
>    endfunction
>   endif
>   ```

#### 使用单文件Vim会话的一个问题：已存在交换文件

对每个文件单独使用一个Vim会话来编辑，很容易出现冲突的情况，迟早会遇到“已经存在交换文件！”的错误，有两个原因：

1.  上次的编辑过程发生了意外崩溃
2.  多个Vim会话编辑同一文件

当进程ID（process ID）后面没有“STILL RUNNING”，就是①，否则是②。

![](https://gitee.com/masstsing/picgo-picserver/raw/master/20200829151016.png)

情况①，需要按r来恢复上次的编辑状态，Vim支持即时在你没有存盘的情况下仍然保存你的编辑状态，因此可以恢复上次没有存盘的内容。恢复之后Vim不会删除留下来的交换文件，需要恢复、保存、退出再进入时按d删除或者手工删除交换文件。

情况②，大部分情况应该使用q或者a放弃编辑，并照当目前已经打开的Vim窗口，从那么继续。如果只需要查看文件，可以按O只读的打开文件。可以按e进行强行直接编辑，但不推荐。

Vim 8 支持当文件已经打开式，自动切换到已经打开的Vim窗口上，这个功能在文件处于一个不活跃的标签页时很有用。（Vim会将这个标签页自动切到最前面）。这个功能需要开启：

```shell
if v:version >= 800
	packadd! editexisting
endif
```



### 多文件的Vim会话

Vim支持一次打开多个文件，你只需要在命令行上写出多个文件即可，或者使用通配符。(`vim *.cpp *.h`)

为了确保在配置较差的环境里仍然能够正常工作，vim绝不会不必要地消耗内存，包括打开不必要立即打开的文件。所以，在打开多文件时，Vim 建立了一个文件列表，并且暂时只打开其中第一个文件。

Vim支持如下多文件操作的命令：

-   `:args` 可以显示多文件的列表
-   `:args {filename}` 使用新的文件名列表替参数列表
-   `:next/:n` 打开下一个文件，如果当前文件修改（未存盘）则会报错终止，但如果命令后面加！则会放弃修改内容，其他命令也类似。
-   `:Next/:N/:previous/:prev` 打开上一个文件
-   `:first / :rewind` :回到列表中的第一个文件
-   `:last` 打开列表中的最后一个文件  

>   在多文件操作的时候，每次切换相当于要关闭先前的文件，再打开下一个。所以，及时保存修改状态是非常重要的。
>
>   可以通过`set autowrite`来设置在切换/关闭文件时自动保存。
>
>   对于批量在文件头进行粘贴：`n|normal ggP`
>
>   >   `:n`, 下一个文件
>   >
>   >   `|normal` 。命令行下执行正常模式的命令
>   >
>   >   `ggP`，具体的正常模式操作命令
>
>   shell上递归通配文件:`*.cpp *.h **/*.cpp **/*.h`
>
>   vim借用了Zsh的简化语法：`:args **/*.cpp **.*h`



### 缓冲区 buffer

Vim里会对每一个已打开或要打开的文件创建一个缓冲区，这个缓冲区就是文件在Vim中的映射。在多文件编辑的时候你也会有同样数量的缓冲区。

往往缓冲区的数据量会更高，因为使用`:e`等命令打开的文件不会改名命令行参数打开的文件列表，这个文件列表只会被命令行或`:args`命令参数修改，但同样会增加缓冲区的数量。

对于`:args`代表的文件列表，仅仅只是文件列表，而缓冲区中有更多的信息，最基本的就是记忆光标的位置。

对于打开同一个文件的多个缓冲区，是对同一个文件的映射， 在同一个Vim会话中的不同窗口或标签，对其中一个缓冲区的修改，会同步的体现在各个缓冲区窗口上。就不会发生多会话编辑的冲突和错误了。

![image-20200829163500446](https://gitee.com/masstsing/picgo-picserver/raw/master/20200829163500.png)

使用`:ls / :buffers`，可以显示缓冲区的列表，同事会显示出各个缓冲区的信息。

-   文件名前面编号
-   当前活跃文件标记`%a`, 最近活跃缓冲区标记`#`，缓冲区已经被修改 `+`
-   文件名后面有行号，表示光标在文件中的位置

缓冲区操作命令：

-   `: buffers/:ls` ，显示缓冲区的列表
-   `:buffer/:b BUFFER_NO`，跳转到编号对应的缓冲区，如果当前缓冲区已被修改但未存盘会报错终止，但如果命令后跟！则会放弃修改内容，其他命令也类似
-   `:bdelete/:bd BUFFER_NO`，删除编号对应的缓冲区，编号省略的话删除当前缓冲区
-   `:bnext/:bn`，跳转到下一个缓冲区
-   `:bNext/:bN/:bprevious/:bp` ，跳转到上一个缓冲区
-   `:bfirst/:brewind`，跳转到缓冲区列表中的第一个文件
-   `:blast`，跳转到缓冲区列表中的最后一个文件
-   `BUFFER_NO <Ctrl-^>` ，切换到指定缓冲区

使用缓冲区列表有点像打开最近使用的文件菜单（但缓冲区列表不会存盘），可以当做一种快速切换到最近使用的文件的方式。

### 窗口

可以对当前窗口进行水平或垂直的分割：

`:split/:sp FILENAME`，水平分割当前窗口并打开指定文件，缺省打开当前文件（打开的是缓冲区，原因如上述两节，Vim 会话中文件都映射到缓冲区，除了命令行参数和:args命令会修改到文件列表，不可能在修改该列表）

`:vertical/:vert split`或`:vsplit/:vs`，垂直分割当前窗口。

多窗口可以使用鼠标在分割线上拖拽来改变窗口的大小，在所有启用了鼠标支持的Vim版本中都是可用的（界面，文本，终端），同时也可以用键盘进行操作：

-   `<C-W> hjkl/方向键`，可以在窗口之间跳转
-   `<C-W>w`，往右跳转窗口，换行到首
-   `<C-W>W`，往左跳转窗口，换行到末
-   `<C-W>n/:new`，打开一个新窗口
-   `<C-W>c/:close`，关闭当前窗口，如果当前窗口已经是最后一个则无效。
-   `<C-W>q/:quit`，退出当前窗口，昂最后一个窗口退出时则退出VIm
-   `<C-W>o/:only`，只保留当前窗口，当最后一个窗口退出时则退出Vim
-   `<C-W>s/:split`，水平分割当前窗口
-   `<Ctrl-W>v/:vsplit`，垂直分割当前窗口
-   `<Ctrl-W>=`，使所有窗口大小相同
-   `NO <Ctrl-W> _`，设置窗口高度，命令前的数字表示高亮行数，默认为纵向占满（想专心编辑某个文件时很有用）
-   `NO <Ctrl-W>|`，设置窗口宽度，默认为横向占满
-   `NO <Ctrl-W>+`，增加窗口的高度，NO为增加的行数，默认为 1
-   `NO <Ctrl-W>-`，减少窗口的高度，NO为减少的行数，默认为1
-   `NO <Ctrl-W>>`，增加窗口的宽度，~
-   `NO <Ctrl-W><`，减少窗口的宽度，~
-   `<C-W>T`，将当前窗口变为一个新标签页。

由于切换窗口是一个非常常见的操作，我通常会映射一下快捷键。为了跟一般的图形界面程序一致，我们使用了``<C-Tab>``和`<C-S-Tab`进行映射。

```shell
" 切换窗口 windows ctrl + tab
if &term =~ 'xterm' " xterm 下 <C-Tab>被占用，使用功能键F1，因为习惯将F1设为NerdTree的开关
    nnoremap <F1>w      <C-W>w
    inoremap <F1>w      <C-O><C-W>w
    nnoremap <F1>qw     <C-W>W
    inoremap <F1>qw     <C-O><C-W>W
else
    nnoremap <C-Tab>   <C-W>w
    inoremap <C-Tab>   <C-O><C-W>w
    nnoremap <C-S-Tab> <C-W>W
    inoremap <C-S-Tab> <C-O><C-W>W
endif
```

>   nnoremap命令映射正常模式下的键盘
>
>   inoremap命令映射插入模式下的键盘
>
>   这里插入模式下的键映射使用了临时模式切换键`<Ctrl-O>`，（`:help i_CTRL-O`），在正常模式下执行相应的窗口命令，然后返回插入模式。使用这样的键盘映射之后，这两个键在正常模式和插入模式下都可以用了。

#### 使用双窗口的比较操作

多窗口的一个重要用法就是进行文件内容的比较，Vim对此也有特殊的支持。在shell上使用vimdiff或者gvimdiff命令，后跟两个要比较的文件名，我们就可以对这两个文件进行比较。在比较时，VIm会自动折叠相同的代码行，并加亮两边文本的不同部分。窗口的滚动也是联动的。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/20200829235019.png)

>   因为双窗口比较对窗口的使用要更宽才会适应我们阅读，所以要对Vim的整体或单个窗口进行最大化、拖拉之类的操作。这些操作一般只影响右边窗口的大小。这时可以在其后跟上一个`<Ctrl-W>=`。

也可以在Vim中打开一个文件进行比较，`:vert diffsplit FILENAME`.

### 标签

多标签和多窗口是互补的。

其基本特性和其他现代的图形化界面下的编辑器的多标签页是相似的。

-   多标签页编辑允许在编辑器里同事修改多个（未存盘）的文件
-   多标签页编辑一次只展示一个文件
-   通过选择标签页（或使用键盘）可以方便地在多个标签页中进行切换

Vim的标签页在图形界面和终端文本模式下都之上上述特性：

![image-20200830000052707](https://gitee.com/masstsing/picgo-picserver/raw/master/20200830000052.png)

Vim中较为特别的一点是，在标签页中可以包含多个窗口，而不是窗口可包含多个标签页。这就意味着**在标签页里关闭最后一个窗口就关闭了整个标签页**。

即使在终端纯文本模式下，存在多标签的情况下，也可以在有鼠标支持的情况下使用鼠标点击进行切换。双击标签栏的空白处添加新标签页，点击X来关闭标签页。

标签页使用的一些键盘命令：

-   `:tab  cmd`，在关于多窗口的命令行命令cmd前加 tab，可以在新标签页中展示命令的结果（`:tab help`,	`:tab split`)
-   `:tabs`，展示所有标签页的列表
-   `:tabnew / :tabedit`，可以打开一个空白的新标签页，后面有文件名的话则打开该文件
-   `:tabclose`，可以关闭当前标签页（如果标签页里只有一个窗口，使用窗口关闭命令`<C-W>c`应该更快）
-   `:tabnext/gt/<C-PageDown>`，可以切换到下一个标签页
-   `:tabNext/:tabprevious/gT/<C-PageUp>`，可以切换到上一个标签页
-   `:tabfirst/:tabrewind`，切换到第一个标签页
-   `:tablast`，切换到最后一个标签页

>   **单窗口多文件的适合场景是对多个文件批量进行相似操作，**
>
>   **多窗口编辑的适合场景是需要对多个文件进行对比编辑**
>
>   **除此之外的一些同事编辑多个文件的场景，就可以使用多标签页的编辑方式**‘

>   Vim会话中，不同标签页以及不同窗口编辑的同一个文件对应的是同一个缓冲区，也即这样的多开实际操作同一内存，不会产生冲突。
>
>   如果缓冲区被修改了，只要当前窗口关闭时，不是包含这个缓冲区的唯一窗口，也没有什么问题。也不会影响文件的状态。
>
>   在任何一个瞬间，任何一个窗口都指向一个缓冲区，而任何一个缓冲区都属于一个或多个窗口。（除了隐藏缓冲区）。



## Vim的搜获和替换

Vim的搜索命令`/`和替换命令`:s` ，输入的待查内容是被Vim当做正则表达式处理的。Vim支持的正则表达式是POSIX流派的BRE。（`:help pattern-overview`)

![image-20200901215857175](https://gitee.com/masstsing/picgo-picserver/raw/master/20200901215857.png)

![image-20200901220014268](https://gitee.com/masstsing/picgo-picserver/raw/master/20200901220014.png)

![image-20200901220049014](https://gitee.com/masstsing/picgo-picserver/raw/master/20200901220049.png)

### 正则表达式搜索

### 元字符

	正则表达式的元字符包括五类：特殊单字符、空白符、量词、范围和断言。

![img](https://raw.githubusercontent.com/Mocearan/picgo-server/main/ffd9cd49a79599e7efa4aba069faee52.png)

![image-20200901222340037](https://gitee.com/masstsing/picgo-picserver/raw/master/20200901222340.png)



#### 基于BRE的vim正则：

>   `.` `*`  `^`  `$`  `~`  `[]`  `\`  是有特殊含义的字符
>
>   `.`： 可以匹配除换行符外的任何字符，匹配换行符需要使用`\_.`
>
>   `*`： 将之前的匹配源重复零次或多次。
>
>   `^`： 在模式头表示匹配行首，其他位置表示字符本身
>
>   `$`： 在模式末表示匹配行尾，其他位置表示字符本身
>
>   `~`： 匹配上一次替换的字符串。
>
>   `[...]`：匹配方括号内的任意字符，方括号内首字符为`^`表示结果取反，除开头外的`-`表示范围，
>
>   `\`：含义取决于下一个字符，大部分情况下，如上述字符表示转义。否则，某些字符时由特殊含义

在Vim的替换命令(`:s`)中，对于替代命令中其分隔作用的`/`，如果模式中含有`/`那就需要转义，可以使用其他模式串中没有的符号来作为分隔符。比如替换`/image/`为`/images/`，原本应该写为：`:%s/\/image\//\/images\//g`，可以改为成：`:%s!/image/!/images/!g`.

但是这个方法在搜索时就不可用了，只能把模式中的`/`写作`\/`。![image-20200901230840369](https://gitee.com/masstsing/picgo-picserver/raw/master/20200901230840.png)

>   通过`\`开始的特殊表达式：
>
>   -   BRE对于除了`*`之外的量词和分组需要转义使用：
>       -   `\?`， 表示匹配源重复0或1次
>       -   `\+`， 表示匹配源重复一次或多次
>       -   `\{n,m}`，表示之前的匹配源重复n到m次，`{n}`重复n次，`{,n}`最多n次，`{n,}`最少n次。`{}`等价于`*`，重复零次或多次。
>       -   `\( 和 \)`， 分组，组成单个匹配源，使用`\编号`进行引用
>       -   `\&`， 邻接两个匹配源，逻辑与
>       -   `\|`， 玲姐两个匹配源，逻辑或
>   -   13个特殊模式项
>       -   `\<`， 匹配单词的开头
>       -   `\>`， 匹配单词的结尾
>       -   `\s`， 匹配空白字符`<Space>`和`<Tab>`
>       -   `\S`， 匹配非空白字符
>       -   `\d`， 匹配数字，相当于`[0-9]`
>       -   `\D`， 匹配非数字，相当于`[^0-9]`
>       -   `\x`， 匹配十六进制数字，相当于`[0-9A-Fa-f]`
>       -   `\X`， 匹配非实录禁止数字，相当于`[^0-9A-Fa-f]`
>       -   `\w`， 匹配单词字符，相当于`[0-9A-Za-z_]`
>       -   `\W`， 匹配非单词字符，相当于`[^0-9A-Za-z_]`
>       -   `\h`， 匹配单词首字符，相当于``[A-Za-z_]``
>       -   `\H`， 匹配非单词首字符，相当于`[^A-Za-z_]`
>       -   `\c`， 忽略大小写进行匹配

#### Vim搜索

使用?或者/ 进行搜索的时候，Vim进行的时候部分匹配，如果想要完全匹配，则需要在确定单词边界：`\<匹配源\>`（Vim不支持PCRE的`\b`）

被誉为最有用的Vim提示，把光标定在希望搜索的单词上，按`*`， Vim会提取光标所在的单词，自动添加`\< 和 \>`进行搜索。

```shell
/\<\(begin|end\)\> # 查找begin或end
```



#### Vim的贪婪模式和非贪婪模式

Vim的非贪婪模式就是`{-n,m}`，意义和`{n,m}`基本相同，但结果是较短而非较长的字符串。



#### Vim的替换

在替换中可能要考虑的情况：

-   可能要保留匹配源中的某些字符，而替换另外一些字符
-   可能要对匹配出的内容做大小写转换
-   可能需要“计算”替换出的结果
-   可能需要决定一行里要替换单词还是多次，是自动替换还是要一一确认。等等

>   **在替换结果中保留匹配出的字符串**
>
>   可以对匹配源进行分组`\(和\)`，然后通过`\1`到`\9`来引用，如果要在替换字符串中完整引用匹配源的话，可以使用`\0`或者`&`.而在替换串中，字符`&`要写作`\&`。
>
>   将unique_ptr 替换为 unique_ptr&（多写一个<是为了在部分匹配中，去除unique_ptrfafasf之类的项）
>
>   ![image-20200902012756725](https://gitee.com/masstsing/picgo-picserver/raw/master/20200902012756.png)
>
>   `/\<unique_ptr<`，搜索该串
>
>   `\&`，从搜索到的首字符开始
>
>   `\w\+`，选取字符
>
>   `/&\&`，替换为选取的字符+&（\0也一样）
>
>   Vim提供了专门表示匹配开始和结束的匹配源：`\zs`和`\ze`，上面的正则可以写为：
>
>   ![image-20200902013502712](https://gitee.com/masstsing/picgo-picserver/raw/master/20200902013502.png)
>
>   在搜索到的unique_ptr<中选取ptr，替换为ptr&
>
>   **大小写转换的特殊替换字符串**
>
>   -   `\U`， 把下面的字符变成大写，直到`\E`出现
>   -   `\u`， 把下一个字符变成大写
>   -   `\L`， 把下面的字符变成小写，直到`\E`出现
>   -   `\l`， 把下一个字符变成小写
>   -   `\E`， 结束大小写转换
>
>   ![image-20200902013740670](https://gitee.com/masstsing/picgo-picserver/raw/master/20200902013740.png)
>
>   Vim还能用`\=`开始一个返回字符串的表达式，用来计算出一个替换结果。
>
>   ![image-20200902013906194](https://gitee.com/masstsing/picgo-picserver/raw/master/20200902013906.png)

![image-20200902014021700](https://gitee.com/masstsing/picgo-picserver/raw/master/20200902014021.png)



去除注释//和/**/

```shell

:%s!\s*//.*$!! # 把0到多个空白符后面的//直到行尾全部删除


:%s!/\*\_.\{-}\*/!!g # 与HTML标签匹配类似，需要使用非贪婪模式，加上g表示允许一行多次替换，因为一行里可以有多个/**/注释。
```

![image-20200902014809279](https://gitee.com/masstsing/picgo-picserver/raw/master/20200902014809.png)

```shell

:%s/\<\(_*\)\([a-z]\w*\)\ze(/\1\u\2/g
```





## vim配置

Vim的工作环境时由运行支持文件来设定的。想要定制Vim，就要数值Vim由那些不同类型的运行支持文件，存放在哪里。

Vim运行支持文件在Vim的安装目录下，但是用户可以“克隆”这个目录到自己的用户配置下。

### 安装目录下的运行支持文件

Vim的运行支持在不同平台上有些相似的目录结构。

-   `Unix`：/usr/share/vim/vim82
-   `macOS`:/usr/local/opt/macvim/MacVim.app/Contents/Resources/vim/runtime
-   `Windows`:C:\program Files(x86)\Vim\vim83

在这个目录下，会有很多的子目录，分类放置Vim的支持文件，常用如下：

-   `syntax`:Vim的语法加亮文件

    -   每个文件就是一种文件类型的语法高亮支持
    -   以`.vim`为后缀，`filetype.vim`里是Vim如何识别文件类型的脚本
    -   可以通过`:setfiletype java`类似的命令来设置文件的类型，进行相应的语法加亮
    -   图形界面的Vim里，可以通过 “ Syntax > Show File Types in Menu”来展示Vim的所有文件类型
    -   建议在.vimrc里加上`let do_syntax_sel_menu = 1`以及`let do_no_lazyload_menus = 1` 这是因为历史性能原因而被延迟加载的设置项，但现代电脑不是很在乎。

-   `doc`:Vim的帮助文件

-   `colors`：Vim的配色方案

    -   图形界面：`Edit > Color Scheme`浏览配色方案

-   `plugin`：Vim的插件，即用来增强Vim功能的工具

    ![image-20200826235014310](https://gitee.com/masstsing/picgo-picserver/raw/master/20200826235015.png)



### 用户的Vim配置目录

Vim的安装目录你不应该去修改。① 你可能没有权限去修改这个目录 ② 即使有权限修改，这个目录也会在Vim升级时被文件拷贝重新覆盖，修改就会丢失。

用户自己的配置应该放在自己的目录下，就是用户家目录下的Vim配置目录 `.vim / .vimfiles`。这个目录应和Vim安装目录下的运行支持文件目录有形同的结构，但其下的子目录在需要修改Vim相关行为时的才有必要创建。

如果一个同名文件在Vim安装目录里和用户自己的Vim配置目录里都有，用户的文件优先。

>   **修改Vim行为的最简单方式：**
>
>   把系统的运行支持文件拷贝到用户自己家目录下的Vim配置目录下相同的位置，然后修改其内容。
>
>   -   可以用这种方式精调Vim的语法加亮
>
>   这种方式，如果Vim系统的运行支持文件后来被修改了，你也会继续使用自己目录下的版本（未作修改）。





### vim配置文件

Vim的功能特性很多，但大部分没有缺省启用，为了使用更多的特性，需要创建一个针对所有用户的或者某一用户的vimrc文件。

可以在全局的vimrc文件中放一些通用的配置，在用户家目录下的.vimrc中放特定的配置。在用户家目录下的.vimrc文件中将全局的vimrc导入。比如：
>  1. 开始编辑 vimrc 文件，具体命令取决于您所使用的操作系统：
>        :edit ~/.vimrc          这是 Unix 系统所使用的命令
>        :edit $VIM/_vimrc       这是 MS-Windows 系统所使用的命令
>  2. 接着读取 vimrc 示例文件的内容：
>        :r $VIMRUNTIME/vimrc_example.vim
>  3. 保存文件，命令为：
>        :write

Unix下，Vim的用户配置文件放在用户的主目录下，文件名通常是.vimrc

WIndows下，Vim的配置文件是_vimrc



### vim 配置项



```shell
set enc=utf-8 " 设置编辑文件的内码是UTF-8（非所有平台缺省，但为编辑多语言文件所必需

set nocompatible " 设置Vim不需要和vi兼容

source $VIMRUNTIME/vimrc_example.vim " 导入Vim的示例配置（会打开一些有用的选项，如语法高亮、搜索加亮、命令历史、记住上次的文件位置。等等）
```

现代Unix系统上的Vim8，实际上只需要最后一句就足够了。对于现代Windows系统上的Vim8，中间的这句也可以删除。但是为了配置可迁移（不同版本，不同平台），三句都加入会比较安全。

-   备份及撤销

>   如果在修改文件时会出现以`~`结尾的文件，文件名后面直接加`~`的，还有前面加`.`后面加`.un~`的。
>
>   因为示例配置里，Vim自动设置了：
>
>   ```shell
>   set backup " 每次编辑会保留上一次的备份文件
>   set undofile " Vim在重新打开一个文件时，仍然能够撤销之前的编辑，会产生一个保留编辑历史的undofile
>   ```
>
>   通常的做法是，不产生备份文件，但保留跨会话撤销编辑的能力。因为有了撤销文件，备份其实也就没有必要了。同时将撤销文件放在用户个人的特定目录下，既保证了安全，又免去其他目录下出现不必要的杂乱文件。
>
>   **Vim会话：**即文件通过Vim一次打开到关闭之间的过程即操作，称为Vim会话。
>
>   ```shell
>   " 无论哪个环境，都有先手动创建这个目录，可以通过Vim自动创建
>   if !isdirectory(&undodir)
>   	call mkdir(&undodir, 'p', 0700)
>   endif
>   
>   " linux/macOS
>   set nobackup
>   set undodir=~/.vim/undodir
>   " windows
>   set nobackup
>   set undodir=~\vimfiles\undodir
>   ```
>
>   &undodir 代表 undodir选项的值。

-   鼠标支持

>   remote终端下使用Vim，鼠标的选择有一定的歧义：①以Vim的可视模式选择内容，并只在vim内使用该内容②产生Vim外的操作系统级的文本选择，文本用于跟其他应用程序交互。
>
>   通常在`xterm`兼容终端下：
>
>   -   不按下修饰键时，鼠标选择产生Vim内部的可是选择
>   -   按下`<Shift>`键时，鼠标选择产生操作系统的文本选择
>
>   对于不兼容`xterm`，不支持对Shift键做这样特殊处理的终端，我们一般会采用一种“绕过”方式，让Vim在某种情况下暂时不接管鼠标事件。通常的选择是在命令行模式下不使用鼠标。
>
>   最新的Vim缺省配置文件（default.vim，vimrc_example.vim会引入），在大部分情况下已经可以自动设置合适的鼠标选项了。设置如下：
>
>   ```shell
>   if has('mouse') " 在有鼠标支持
>     if has('gui_running') || (&term =~ 'xterm' && !has('mac'))
>   	set mouse=a " 图形界面运行或不是mac且支持xterm的终端，启用完全鼠标支持, 拖拽鼠标会在Vim里以可视模式选中内容且在Vim中使用
>   	" 用户按下Shift时，界面系统接管鼠标事件，用户可以使用鼠标赋值Vim窗口里的内容供其他程序使用
>     else " 在非图形界面，且不是`xterm`终端下，就只在n(ormal)v(isual)i(nsert)模式中使用鼠标。当:进入命令模式，Vim将不会接管鼠标，用户可以使用鼠标赋值Vim窗口里的内容到其他应用程序中。
>   	 set mouse=nvi ""
>     endif
>   endif
>   ```

-   中文支持

>   如果一直在UTF-8下使用中文的话，那这一小节的内容可以跳过。对于大部分在Unix下工作的人，都应该如此。
>
>   在Windows下下，或者有需要跟别人交换GB28181、GBK、GB18030编码的文本文件，那这部分的内容还是需要了解。
>
>   -   整个世界基本上在向UTF-8编码靠拢
>   -   微软由于历史原因，内部使用UTF-16，可以跟UTF-8无损转转
>   -   GB*系列是一系列向后兼容的中文标准编码方式。GB2312编码的文件时合法的GBK文件，GBK 编码的文件是合法的GB18030文件。但除了GB18030，都不能做到跟UTF-8无损转换。目前非UTF-8的简体中文文本基本上都是用GBK/GB18030编码（繁体中文Big5居多）.鉴于GB18030是国家标准，其他两种编码也和GB18030兼容。
>
>   你好😄
>
>   UTF-8  下10字节：e4bda e5a5bd  f09f9884
>
>   GB18030下8字节：c4e3 bac3 9439fd30
>
>   GB18030 处理中文在存储效率上是有事的，但它也有缺点：
>
>   -   GBK外的Unicode字符一般需要四字节编码（非中文情况会劣化）
>   -   GBK外的Unicode字符跟Unicode码点需要查表才能转换（UTF-8则可以用非常简单的条件判断、移位、与、或操作来转换）
>   -   一旦出现文件中有单字节发生损毁，后续的所有中文字符都可能发生紊乱（而UTF-8可以在一个字符之后恢复）
>
>   GB18030不会在国际化额软件中用作内码，只会是读取/写入文件时使用的转换编码。我们要让Vim支持GB18030也同样是如此。
>
>   UTF-8编码有明显规律，并非任意文件都能成功当成UTF-8来解码，我们一般的解码顺序：
>
>   1.  检查文件是不是有Unicode的BOM（字节顺序标记）字符，有的话按照BOM字符来转换文件内容。
>   2.  其次，检查文件能不能当做utf-8来解码，如果可以，就当做UTF-8来解释。
>   3.  尝试用GB18030来解码，如果能成功，就当做GB18030来转换文件内容。
>   4.  如果上面的解码都不能成功，就按Latin1字符集来解码，由于这是单字节的编码，转换必定成功。
>
>   Vim缺省基本按照这样的顺序，第三步使用何种编码与系统配置有关。明确需要处理中文，那么配置文件中最好：
>
>   ```shell
>   set fileencodings=ucs-bom, utf-8, gb18030, latin1
>   ```



-   图形界面的字体配置

>   图形界面的Vim可以自行配置使用的字体，大部分环境中只是起到煤化作用，只有在高分辨率屏幕的Windows中是必须的：不进行配置的话，字体会小的没法看。
>
>   在windows的缺省字体里，一般而言，`Consolas`和`Courier New`比较合适。以Courier New为例，在_vimrc里可以这样配置（windows上的基本写法是字体名称加冒号、“h”加字号。用下划线取代空格，否则空格需要用“\“转义。
>
>   ```shell
>   if has('gui_running')
>   	set guifont=Courier_New:h10
>   endif
>   ```
>
>   Vim在设置选项时，空格需要用`\`进行转义。比如，如果我们要在Ubuntu下把字体设成10磅的DejaVu Sans Mono:
>
>   ```shell
>   " Linux 和 Windows 不同，不能用'_' 取代空格
>   set guifont=DejaVu\ Sans\ Mono\ 10
>   ```
>
>   宽字符（中文字符）是可以单独配置的。对于纯中文的操作系统一般是不需要配置的，但如果语言设定中中文不是第一选择，就有可能在显示中文时出现操作系统误用日文字体的情况，这时候回想要手工选择一个中文字体，比如Ubuntu下：
>
>   ```shell
>   set guifont=Noto\ Sans\ Mono\ CJK\ SC\ 11
>   ```
>
>   中英文的字号不必相同，实际上在Windows上和Linux上一般也都使用的不是相同的字号。



### 插件及插件管理

![image-20200827000230139](https://gitee.com/masstsing/picgo-picserver/raw/master/20200827000230.png)

#### Vim 插件管理

Vim的目录结构是传统Unix式的，一个功能用到的文件可能会分散在多个目录下。（如同/usr/bin, /usr/share/man, /usr/share/vim)。

>   一个Vim插件，严格意义上将叫包，通常也会分散在多个目录下：
>
>   -   插件的主题通常在plugin目录下
>   -   插件的帮助文件在doc目录下
>   -   有些插件只对某些文件有效，会有文件放在ftplugin目录下
>   -   有些插件有自己的文件类型检测规则，会有文件放在ftdetect目录下
>   -   有些插件有特殊的语法加亮，会有文件放在syntax目录下
>   -   。。。
>
>   以前的插件管理，一般是一次性安装后就不管了，安装过程就是到.vim目录（windows上是vimfiles目录下），解压缩包，然后执行`vim -c ‘helptagsdoc | q’`生成帮助文档的索引。
>
>   互联网式更新年代，这种方式就显得落伍了。尤其糟糕的地方在于，它是按照文件类型来组织目录的，而不是按相关性，这就没法用Git来管理了。

Vim后来出现了一些包管理器，基本模式是：每个包有自己的目录，然后这些目录会被加到Vim的运行时路径(runtimepath)选项里。最早的`runtimepath`较为简单，Unix缺省：

```shell
$HOME/.vim, $VIM/vimfiles, $VIMRUNTIME, $VIM/vimfiles/after, $HOME/.vim/after
```

有个包管理器之后，runtimepath会显得很复杂，每个包都会增加一个自己的目录进去，好处是包的管理变得非常方便。

>   `:echo &runtimepath` 可以查看runtimepath的值。

##### vim软件包

从Vim8开始，vim官方采用了类似的体系，Vim会在用户的配置目录（Unix的`$HOME/.vim`,windows的`$HOME/vimfiles`）下识别名字叫pack的目录，并在这个目录的子目录的`start和opt`目录下寻找包的目录

>   **一个.vim目录结构：**
>
>   ![image-20200827002111613](https://gitee.com/masstsing/picgo-picserver/raw/master/20200827002112.png)
>
>   pack下有minpa和my两个子目录，这个名字Vim不关心，每个目录下又有opt和start连个子目录，再夏绵绵是每个包自己的目录了，里面又可以有自己的一套`colors/doc/plugin`之类的子目录，这样就方便管理了。

vim 8在启动时会递归加载pack目录下所有名为`start`目录下的包，用户可以用`:packadd`命令来加载某个`opt`目录下的包。(`:packadd vimcdoc`，加载vimcdoc包，显示中文帮助信息)



##### 管理器

推荐使用一款包管理器，可以带来如下的好处：

-   根据文本的配置（一般写在vimrc配置文件里）决定要安装那些包
-   自动化安装、升级和写在，包括帮助文件的索引生成

###### minpac

minpac是一个利用Vim8功能的小巧的包管理器。

-   windows安装

    ```shell
    cd /d %USERPROFILE%
    git clone https://github.com/k-takata/minpac.git ^
        vimfiles\pack\minpac\opt\minpac
    ```

-   Unix

    ```shell
    git clone https://github.com/k-takata/minpac.git \
        ~/.vim/pack/minpac/opt/minpac
    ```

然后修改.vimrc配置文件

```shell

if exists('*minpac#init')
  " Minpac is loaded.
  call minpac#init()
  call minpac#add('k-takata/minpac', {'type': 'opt'})

  " Other plugins
endif

if has('eval')
  " Minpac commands
  command! PackUpdate packadd minpac | source $MYVIMRC | call minpac#update('', {'do': 'call minpac#status()'})
  command! PackClean  packadd minpac | source $MYVIMRC | call minpac#clean()
  command! PackStatus packadd minpac | source $MYVIMRC | call minpac#status()
endif
```

保存退出，重启Vim之后，就有了三个新的命令，用来更新、清理、检查包。

-   `:PackUpdate` 安装扩展包

    >   在`Other plugins`下面加入 `call minpac#add('tpope/vim-eunuch')`
    >
    >   ```shell
    >   if exists('*minpac#init')
    >    " Minpac is loaded.
    >    call minpac#init()
    >    call minpac#add('k-takata/minpac', {'type': 'opt'})
    >   
    >    " Other plugins
    >    call minpac#add('tpope/vim-eunuch')
    >   endif
    >   ```
    >
    >   然后使用`:PackUpdata`安装或更新插件。会在侧边开启新的窗口展示包状态：
    >
    >   ![image-20200827005930624](https://gitee.com/masstsing/picgo-picserver/raw/master/20200827005931.png)
    >
    >   可以按q退出窗口。

-   `:PackStatus` 重新打开状态窗口

-   `:PackClean` 删除插件，在此之前，要在vimrc中删除相应插件的行



#### Vim 插件



##### yegappan/mru 最近使用文件

`mru`: most recently used

>   **安装**：
>
>   使用minpac进行安装
>
>   ```shell
>   if exists('*minpac#init')
>      " Minpac is loaded
>      call minpac#init()
>      call minpac#add('k-takata/minpac', {'type':'opt'})
>   
>      " Other plugins
>      call minpac#add('yegappan/mru') " mostly recent use
>   endif
>   ```
>
>   `:PackUpdate`

安装完成后，图形界面的file菜单下就可以看到recent files选项。

在文本界面，需要加入一些相关的配置：

```shell

if !has('gui_running')
  " 设置文本菜单
  if has('wildmenu')
    set wildmenu
    set cpoptions-=<
    set wildcharm=<C-Z>
    nnoremap <F10>      :emenu <C-Z> " 正常模式用<F10> 打开文本菜单
    inoremap <F10> <C-O>:emenu <C-Z> " 插入模式使用<F10> 临时模式打开文本菜单
  endif
endif
```

>   **``<F10>``打开文本菜单后，按一次``<Tab>``进入recent files选项**
>
>   之后可以使用tab自动补全以及递进下一项，或使用方向键左右选择选项



##### preservim/NERDTree

NERDTree是一个利用多窗口，提供树形文件目录功能的插件。也称文件浏览/管理插件。

![image-20200830020115011](https://gitee.com/masstsing/picgo-picserver/raw/master/20200830020115.png)

>   **安装：**
>
>   使用minpac进行安装：
>
>   ```shell
>   if exists('*minpac#init')
>      " Minpac is loaded
>      call minpac#init()
>      call minpac#add('k-takata/minpac', {'type':'opt'})
>   
>      " Other plugins
>      call minpac#add('preservim/nerdtree')
>   endif
>   ```
>
>   `:PackUpdate`

NERDTree缺省抢占netrw使用的路径形式，所以可以使用`:e .`来打开NERDTree。更常用的方法是：`:NERDTreeToggle`，此命令可以打开，也可以进行关闭。可以将其映射到某个快捷键（`F1`)。

>   **使用方法：**
>
>   打开NERDTree之后，光标跳转到该窗口
>
>   -   在目录上回车或双击可以打开或关闭该目录
>   -   在文件上敲回车或双击，立即打开该文件，如果当前窗口已存盘则在当前窗口打开，未存盘则打开新的窗口。并且光标跳转到文件窗口中
>   -   在文件上使用`go`，预览文件，光标不会跳转到该窗口。
>   -   `i`，打开文件到一个新的水平分割的窗口
>   -   `s`，打开文件到一个新的垂直分割的窗口
>   -   `t`，打开文件到一个新的标签页中
>   -   `I`，展示NERDTree自动过滤的隐藏文件和目录
>   -   `m`，menu菜单，允许增加、删除、重命名等操作。

![image-20200830022617122](https://gitee.com/masstsing/picgo-picserver/raw/master/20200830022617.png)





## Vim版本

Vim是一个持续开发改进的应用程序。每天都有持续的补丁和功能提交和改进。

### Vim 8

Vim7.4到Vim8.2最大的新功能

-   Vim软件包支持：`:help packages`
-   异步任务支持：`:help channel, :help job, :help timers`
-   终端支持：`:help terminal`