# git

​		Git 是一个开源的分布式版本控制系统，用于管理项目开发。

---

​		Git：版本控制系统，内容管理系统，工作管理系统。

> ### 指南和手册
>
> - [Git 社区参考书](http://book.git-scm.com/)
> - [专业 Git](http://progit.org/book/)
> - [像 git 那样思考](http://think-like-a-git.net/)
> - [GitHub 帮助](http://help.github.com/)
> - [图解 Git](http://marklodato.github.io/visual-git-guide/index-zh-cn.html)
>
> Git 完整命令手册地址：http://git-scm.com/docs
>
> PDF 版命令手册：[github-git-cheat-sheet.pdf](https://www.runoob.com/manual/github-git-cheat-sheet.pdf)
>
> - 1、[Git 五分钟教程](https://www.runoob.com/w3cnote/git-five-minutes-tutorial.html)
> - 2、[Git GUI使用方法](https://www.runoob.com/w3cnote/git-gui-window.html)
> - 3、[Github 简明教程](https://www.runoob.com/w3cnote/git-guide.html)
> - 5、[互联网组织的未来：剖析GitHub员工的任性之源](https://www.runoob.com/w3cnote/internet-organization-github.html)



## 版本控制历史

​		版本控制系统， Version Control System - VCS

### 最小版本控制

​		最小的版本控制就是编辑器的Undo/Redo功能（撤销和重做）。

​		而脱离内存的可保存撤销和重做，就是版本控制的本质。

​		为达到可保存的目的，内存中的自动记忆保存的模式就变得更为复杂。于是通过文件系统来实现主动提交改动的机制。

### 多目录版本控制

​		vcs软件出现之前，通过手工维护多个目录来实现版本的迭代管理。

​		但是多人合作的时候，多目录的手工管理在面对资源竞争的时候显得很脆弱，不能有效及时的合并冲突。

### 集中式版本控制

​		SVN，CVS

​		**版本控制**、**主动提交**、**中央仓库**这三个要素，共同构成了版本控制系统（VCS）的核心：开发团队中的每个人向中央仓库主动提交自己的改动和同步别人的改动，并在需要的时候查看和操作历史版本，这就是版本控制系统。

​		![中央式 VCS](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/30/1600a9978ea4bec3~tplv-t2oaga2asx-watermark.awebp)

### 分布式版本控制

​		分布式版本控制系统，Distributed Version Control System - DVCS

​		Git

![分布式 VCS](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/30/1600a9a4a20c2e6e~tplv-t2oaga2asx-watermark.awebp)

​		分布式版本控制系统将提交和上传分拆，提交是提交在本地仓库。在有必要时，将本地仓库上传到远端仓库。

分布式 VCS 的优点：

1.  大多数的操作可以在本地进行，所以速度更快，而且由于无需联网，所以即使不在公司甚至没有在联网，你也可以提交代码、查看历史，从而极大地减小了开发者的网络条件和物理位置的限制（例如，你可以在飞机上提交代码、切换分支等等）；
2.  由于可以提交到本地，所以你可以分步提交代码，把代码提交做得更细，而不是一个提交包含很多代码，难以 review 也难以回溯。

分布式 VCS 的缺点：

1.  由于每一个机器都有完整的本地仓库，所以初次获取项目（Git 术语：clone）的时候会比较耗时；

2.  由于每个机器都有完整的本地仓库，所以本地占用的存储比中央式 VCS 要高。

    ![image-20211102120148191](https://gitee.com/masstsing/picgo-picserver/raw/master/20211102140151.png)

## 安装Git

​		Git 各平台安装包下载地址为：http://git-scm.com/downloads

### linux

#### 软件包按照

`yum install git`

#### 源码安装

```shell
$ tar -zxf git-1.7.2.2.tar.gz
$ cd git-1.7.2.2
$ make prefix=/usr/local all
$ sudo make prefix=/usr/local install
```





## 图形客户端

- [GitX (L) (OSX, 开源软件)](http://gitx.laullon.com/)
- [Tower (OSX)](http://www.git-tower.com/)
- [Source Tree (OSX, 免费)](http://www.sourcetreeapp.com/)
- [GitHub for Mac (OSX, 免费)](http://mac.github.com/)
- [GitBox (OSX, App Store)](https://itunes.apple.com/gb/app/gitbox/id403388357?mt=12)



## 配置

​	`git config`，配置或读取相应的工作环境变量

### 配置级别

- `--system`

  `/etc/gitconfig`，配置作用于系统中所有用户

  >  Git 装在什么目录，就以此作为根目录来定位

- `--global`

  `~/.gitconfig`，配置作用于当前用户

- `--local`

  `.git/config`，配置作用于当前仓库



> - 每一个级别的配置都会覆盖上层的配置的同名变量。
>
> - 缺省默认`--local`



### 配置项

#### 配置项结构

```ini
[http]
    postBuffer = 2M
[user]
    name = runoob
    email = test@runoob.com
```

#### 查看配置

​	`git config --list`

> ​		默认查看所有配置项
>
> ​		重复的变量名，那就说明它们来自不同的配置文件（比如 /etc/gitconfig 和 ~/.gitconfig），不过最终 Git 实际采用的是最后一个。
>
> ​		可以直接查看对应的文件

- 查看配置项

  `git config user.name`

#### 编辑配置

​		`git config --global -e`

#### 个人信息配置

- 用户名 `user.name`

  `git config --global user.name "mass"`

- 邮箱 `user.email`

  `git config --global user.email "mass@gmail.com"`

#### 核心配置

- 编辑器 `core.editor`

  `git config --global core.editor vim`

#### 合并

- 差异比较工具 `merge.tool`

  `git config --global merge.tool vimdiff`

  > ​		Git 可以理解 kdiff3，tkdiff，meld，xxdiff，emerge，vimdiff，gvimdiff，ecmerge，和 opendiff 等合并工具的前端集成。
  >
  > ​		也可以指定使用自己开发的工具



- 颜色配置

  `git config color.ui true`

- 单行输出

  `git config format.pretty oneline`

#### 认证

-   记住密码

    `git config credential.helper store`

    >   安全方式：https://link.juejin.cn/?target=https%3A%2F%2Fhelp.github.com%2Farticles%2Fcaching-your-github-password-in-git%2F

#### 推送

-   默认推送行为

    `git config push.default current`

    >   -   `current`，所有分支都可以用`git push`来直接`push`，目标自动质量`origin`仓库的同名分支。
    >
    >   https://link.juejin.cn/?target=https%3A%2F%2Fgit-scm.com%2Fdocs%2Fgit-config%23git-config-pushdefault



## 仓库

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/git-command.jpg)

​		仓库目录下会生成一个``.git``目录，即本地仓库。包含了所有工作内容的元数据，和文件索引。

​		本地仓库包括一个缓存（暂存区）和一个提交记录存储。

### 创建仓库 init

- 将当前目录初始化为本地Git仓库

  `git init`

- 新建一个仓库目录

  `git init [rname]`

### 关联仓库 remote

- 关联仓库

  `git remote add [host_alias] [url]`

  >   -   `host_alias`
  >
  >       -   `origin`，默认远程地址别名
  >
  >           `git remote add origin git@github.com:tianqixin/runoob-git-test.git`
  >
  >       >   `git remote add github git@github.com:tianqixin/runoob-git-test.git`



- 显示所有关联仓库

    `git remote -v`

    >   `-v` 缺省表示查看当前的关联仓库
    >
    >   -v可以看到每个别名的实际链接地址

- 显示某个远程仓库信息

    `git remote show [remote_url]`

- 取消关联仓库

    `git remote rm [name]`

- 修改关联仓库名

    `git remote rename [oldname] [newname]`

### 克隆仓库 clone

- 克隆一个仓库到当前目录

  `git clone [path/url] [dir]`

  > path 是一个本地的即存仓库路径。
  >
  > url 是一个远端地址，可以是 `username@host:path`
  >
  > dir 可以指定本地的目录，并可以自定义项目名
  >
  > `git clone git://github.com/schacon/grit.git ../mygrit`
  >
  > > 克隆出的仓库是与源仓库关联的。





## 工作流

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/git-process.png)

​		git维护本地仓库的三棵树：工作区、暂存区（Index、stage）、提交记录。

​		工作目录是当前的工作环境，暂存区是仓库的缓存区，HEAD指向仓库中最近一次提交记录。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/trees.png)

> ​		工作环境的保存可以暂存在缓存中，确认提交到仓库时工作内容会作为一个提交记录被仓库记录，以HEAD标识。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/1352126739_7909.jpg)

> - 工作区
>
>   是仓库中`.git`目录外的工作内容副本文件
>
> - 暂存区
>
>   `.git/index`，记录缓存的工作内容
>
> - 提交记录（版本库）
>
>   `.git`，仓库中存储的已提交的工作内容

> ​		工作区、暂存区是针对于某一个提交记录的工作内容。也就是说，不同的提交记录实际上代表着有差异的版本，某个版本都有自己对应的暂存区和工作区。而HEAD标识着这个当前的提交记录。

> ​		`objects`，是存储Git元信息的存储库。Git并非按照文件组织，而是按照对象进行存储，通过索引的方式，来组成仓库中的版本。





## 工作区

​		`.git`，同级目录下的文件结构称为工作区中的工作目录，保存了从仓库中签出（checkout）的内容。

>   ​		工作区相当于简单的系统目录，是Git中某版本的内容副本。对于工作区的操作就是普通的操作系统文件结构操作，不需要特定的命令来操作工作区的内容。

### 未跟踪

​		工作区中新增的文件并未委托给Git仓库管理。此时新增文件的状态为：

```shell
[root@instance-m7qyn6sr gpra]# git status
On branch master
Your branch is up to date with 'origin/master'.

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        shoping.txt

nothing added to commit but untracked files present (use "git add" to track)
```

>   -   `On branch master`
>
>   -   `Your branch is up to date with 'origin/master'.`
>
>       当前本地master分支已经更新到origin的master.
>
>       （没有落后）
>
>   -   `Untracked files:
>         (use "git add <file>..." to include in what will be committed)
>               shoping.txt`
>
>       未追踪的文件shoping.txt。需要使用`git add`来将工作目录中未追踪的文件添加到Git仓库管理。、
>
>   -   `nothing added to commit but untracked files present (use "git add" to track)`
>
>       没有已添加但未提交的内容，但是存在未追踪的文件。

### 已修改未暂存

​		对于存在但不同于提交记录中的工作内容，称为已修改但未暂存的内容。

```shell
[root@instance-m7qyn6sr gpra]# git status
On branch master
Your branch is ahead of 'origin/master' by 1 commit.
  (use "git push" to publish your local commits)

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   shoping.txt

```

>   `Changes not staged for commit:
>     (use "git add <file>..." to update what will be committed)
>     (use "git restore <file>..." to discard changes in working directory)
>           modified:   shoping.txt`
>
>   -   git add 来更新暂存区
>   -   git restore file 丢弃工作目录中的修改

### .gitignore

​		`.gitignore`，用以指定不被git追踪的文件。

> ​		在 GitHub 上创建仓库，你可以在创建仓库的界面中就通过选项来让 GitHub 帮你创建好一个符合项目类型的 `.gitignore` 文件，你就不用再自己麻烦去写一大堆的配置了。

​		![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe3f2100f33d2a~tplv-t2oaga2asx-watermark.awebp)

> **规则：**
>
> ​		[Git - gitignore Documentation (git-scm.com)](https://git-scm.com/docs/gitignore)

## 暂存区

>   ​		Git 中的 stage 取自这个词的另一个意思：组织和准备某个事件。而 "staging area" 的意思也并不是「舞台区域」，而是「用来汇集物品或材料以备使用的区域」的意思。 
>
>   ​		stage 这个词在 Git 里，是「集中收集改动以待提交」的意思；而 staging area ，就是一个「汇集待提交的文件改动的地方」。简称「暂存」和「暂存区」。至于 staged 表示「已暂存」

​		暂存区是`.git`下一个叫``index``的文件。它是一个索引，用于记录被添加到暂存区中的文件的对象存储。

```shell
[root@instance-m7qyn6sr gpra]# git status
On branch master
Your branch is up to date with 'origin/master'.

Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        new file:   shoping.txt
```

>   -   `On branch master`
>
>   -   `Your branch is up to date with 'origin/master'.`
>
>       当前本地master分支已经更新到origin的master.
>
>       （没有落后）
>
>   -   `Changes to be committed:
>         (use "git restore --staged <file>..." to unstage)
>               new file:   shoping.txt`
>
>       工作区中内容已被添加到暂存区中，成为`staged`状态。可以使用`git restore --staged`来从暂存区溢出。

​		暂存区是所有分支共享的，因此，只要未提交(commit)的内容都会在所有分支上呈现。

> ​		有时，我们还未完成当前分支的功能开发，但是又亟需在另一个分支上修改内容(如：修复一个 BUG)：
>
> - 一种方式是通过“git commit”(提交)，解决这个问题。
>
>   ​		但在正常使用中，我们应该在完成某个功能的开发后才提交一个版本，而不是频繁的提交。
>
> - 使用`git stash`来储藏暂存区索引上的内容，并使用`HEAD`的内容覆盖当前工作区。

### 新增 add

​		将指定工作内容加入版本控制。

> ​		更新暂存区的目录树，工作区修改（或新增）的工作内容写入到对象库中的一个新的对象中，对象ID记录在暂存区的文件索引中。

#### detail

​		`git add`添加的是文件改动，而不是文件名

```shell
vim new.txt # write
git add .
git status
```

> ```shell
> Changes to be committed:
>   (use "git restore --staged <file>..." to unstage)
> 	modified:   new.txt
> ```

```shell
vim new.txt # write
git status
```

> ```shell
> Changes to be committed:
>   (use "git restore --staged <file>..." to unstage)
> 	modified:   new.txt
> 
> Changes not staged for commit:
>   (use "git add <file>..." to update what will be committed)
>   (use "git restore <file>..." to discard changes in working directory)
> 	modified:   new.txt
> ```

> - Changes to be commited
> - Changes not staged for commit
>
> 因为通过add加入暂存区的不是文件名，而是文件的改动。不同的改动之间在提交和未提交上是有区别的。

#### 操作

- 将工作区的工作内容加入暂存区缓存

    `git add {file_list}`

    > file_list，同linux文件列表操作，可以指定路径，可以使用通配。
    >
    > `git add .`
    >
    > `git add *.html`

- 交互式添加

    `git add -i`



### 删除 rm

​		将文件同时从git树和工作区上删除。

`git rm [--cached] [file_list]`

-   文件从暂存区和工作区中删除：

    `git rm [file_list]`

-   放弃工作区修改，并从暂存区和工作区中删除

    `git rm -f [file_list]`

    >   `git rm -f *`

-   从暂存区域移除，保留工作区

    `git rm --cached [file_list]`

    >   ​		实际上，从提交记录的角度考虑，从暂存区移除，实际上提交时，就等于让内容从版本管理中移除了。

### 移动 mv

​		用于移动或重命名文件树。

`git mv [-f] {file_list} [dest_dir]`

-   重命名

    `git mv -f [file1] [file2]`

    >   如果目标文件名已存在，则-f
    >
    >   可以视为文件间的移动



### 状态 status

​		查看工作目录当前状态。

​		`git status `

> 通常我们使用 **-s** 参数来获得简短的输出结果：
>
> ` git status -s`



### 差异 diff

​		比较暂存区和工作区中文件的差异

​		`git diff [fname]`

> `git diff`，可以查看`git add`所有修改文件时向暂存区添加的内容。

- 查看暂存区和当前提交记录的差异

`--cached /--staged`，同上

> - 可以看到如果此时此刻`git commit`，将会提交什么
>
>   ```shell
>   [root@instance-m7qyn6sr gpra]# git diff --staged
>   diff --git a/new.txt b/new.txt
>   index c586658..5fe2c31 100644
>   --- a/new.txt
>   +++ b/new.txt
>   @@ -1 +1,5 @@
>   -aaa`:`
>   +a
>   +aaaa
>   +aa`:
>   ```
>
> - `--cached`，等价`--staged`

- 查看工作区和指定commit`的差异

`git diff [commit]`

> `git diff HEAD` 
>
> 可以查看`git add`所有修改文件后`git commit`，将会提交的内容。
>
> > 没有被 add 过 的文件，untracked files 并不会显示出来

- 查看分支差异

`git diff <source_branch> <target_branch>`



## 提交记录

​		提交记录树对外表现的只是某一个提交记录对象对应的版本。这个版本由HEAD指向来决定。

```shell
commit 9a800d55da3286cad058bbaf83e2d7e54557a8a4 (HEAD -> master, origin/master, origin/HEAD)
Author: masstsing <mazengrong1211@163.com>
Date:   Tue Nov 2 06:27:42 2021 +0000

    Initial commit
```

>   -   `commit` 
>
>       表示一个提交记录
>
>   -   `9a800d55da3286cad058bbaf83e2d7e54557a8a4`
>
>       提交记录的SHA-1校验和， 可以使用前若干位，只要能唯一标识仓库中该条提交记录即可。
>
>   -   `HEAD -> master, origin/master, origin/HEAD`
>
>   -   `Author: masstsing <mazengrong1211@163.com>`
>
>       提交记录的作者信息
>
>   -   `Date:   Tue Nov 2 06:27:42 2021 +0000`
>
>       提交记录的提交时间日期信息
>
>   -   `Initial commit`
>
>       提交信息

> ​		使用提交记录SHA-1校验和唯一标识一条提交记录也是很不方便，不利于管理者阅读和操作的。所以 Git 提供了「引用」的机制：使用固定的字符串作为引用，指向某个 `commit`，作为操作 `commit` 时的快捷方式。

### 引用

​		Git 提供了「引用」的机制：使用固定的字符串作为引用，指向某个 `commit`，作为操作 `commit` 时的快捷方式。

​		这个字符串可以是一个 `commit` 的 SHA-1 码（例：`c08de9a4d8771144cd23986f9f76c4ed729e69b0`），也可以是一个 `branch`（例：`ref: refs/heads/feature3`）。

> ​		Git 中的 `HEAD` 和每一个 `branch` 以及其他的引用，都是以文本文件的形式存储在本地仓库 `.git` 目录中，而 Git 在工作的时候，就是通过这些文本文件的内容来判断这些所谓的「引用」是指向谁的。

#### HEAD

​		`HEAD` 是引用中最特殊的一个：引用当前工作目录所对应的 `commit`。

>   ​		每次当有新的 `commit` 的时候，工作目录自动与最新的 `commit` 对应；而与此同时，`HEAD` 也会转而指向最新的 `commit`。
>
>   ​		当使用 `checkout`、`reset` 等指令手动指定改变当前 `commit` 的时候，`HEAD` 也会一起跟过去。

-   当前 `commit` 在哪里，`HEAD` 就在哪里，这是一个永远自动指向当前 `commit` 的引用，所以你永远可以用 `HEAD` 来操作当前 `commit`。

>   HEAD是仓库唯一的。

-   `HEAD` 除了可以指向 `commit`，还可以指向一个 `branch`，当它指向某个 `branch` 的时候，会通过这个 `branch` 来间接地指向某个 `commit`。

-   当 `HEAD` 在提交时自动向前移动的时候，它会像一个拖钩一样带着它所指向的 `branch` 一起移动。

    >   `HEAD`引用哪个分支，提交时就会移动到哪个分支索引。在不同的分支索引上提交，会让不同的分支发展：
    >
    >   ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fe3354ab0861a7~tplv-t2oaga2asx-watermark.awebp)

-   远程仓库的 `HEAD` 是永远指向它的默认`master`分支，并会随着默认分支的移动而移动的。

    >   ​		 在分支`push`时，远程仓库的 `HEAD` 不会像本地仓库的`HEAD`一样在分支之间切换。
    >
    >   ​		`push` 的时候只会上传当前的 `branch` 的指向，并不会把本地的 `HEAD` 的指向也一起上传到远程仓库。

##### HEAD游离

> [Git HEAD detached from XXX (git HEAD 游离) 解决办法 - 程序员大本营 (pianshen.com)](https://www.pianshen.com/article/54381757523/)

#### 分支 branch

​		`branch`是一个引用，是对一组具有同一属性的提交记录的引用。

> ​		这里说的同一属性，是指在仓库管理的角度上，这一组提交记录共同描述一个发展中的特性内容。还不足以融合进整个仓库统一内容中去。
>
> ​		分支隔离与其他内容相比具有某个不同之处的特性内容。
>
> ​		`branch`引用名类似于一个有向图上的指针，是从起点到当前。当``branch``名在不同提交记录之间移动时，就像数组头指针的移动。
>
> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779f983c81e7~tplv-t2oaga2asx-watermark.awebp)

-   ``branch``， 从它引用的`commit`开始，向后包含所有的`commit`

    与其他`branch`共有的`commit`是整个仓库中所共同认可的基石

    >   -    `branch` 可以认为是从初始 `commit` 到 `branch` 所指向的 `commit` 之间的所有 `commit`s 的一个「串」：
    >
    >       ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779fa5e6970d~tplv-t2oaga2asx-watermark.awebp)
    >
    >       ​		`master` 的本质是一个指向 `3` 的引用，但你也可以把 `master` 理解为是 `1` `2` `3` 三个 `commit` 的「串」，它的起点是 `1`，终点是 `3`。
    >
    >   -   `branch` 包含了从初始 `commit` 到它的所有路径，而不是一条路径。并且，这些路径之间也是彼此平等的。
    >
    >       ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fe3354a1d3cd26~tplv-t2oaga2asx-watermark.awebp)
    >
    >       >   ​	`master` 在合并了 `branch1` 之后，从初始 `commit` 到 `master` 有了两条路径： `1` `2` `3` `4` `7` 和 `1` `2` `5` `6` `7` 。而且，这两条路径是平等的，`1` `2` `3` `4` `7` 这条路径并不会因为它是「原生路径」而拥有任何的特别之处。
    >       >
    >       >   >   ​		比如说系列小说的内容发展，基于某一个叙事发展而来，但最后融合了其他的叙事，最终发展成现在的样子。对此而言，前面的若干叙事都是有意义不分主次的。比如《银河帝国》，比如《复联》。
    >
    >       保留路径的原因是为了版本的回溯。
    >
    >       >   比如可以回到合并之前，重新选择，组织内容。

-   `HEAD`可以直接指向`commit`，也可以通过引用某个`branch`间接引用到某个`commit`。

-   `HEAD`在该分支上的移动操作，会带着被它引用的`branch`移动。

    >   ​		本地的新提交会使得本地的`HEAD`和`branch`引用更新，从而与关联仓库的引用指向不同的提交记录：
    >
    >   ```shell
    >   commit 9b6d2c9488110e0ba8834c6bf202f9be8180003a (HEAD -> master)
    >   Author: mass <mazengrong12211@163.com>
    >   Date:   Wed Nov 3 21:25:37 2021 +0800
    >   
    >       new
    >   
    >   commit 7c71d3b861454ecd715551d84e751446532f4b55 (origin/master, origin/HEAD)
    >   Merge: 1ea72f9 707ae91
    >   Author: mass <mazengrong12211@163.com>
    >   Date:   Tue Nov 2 16:37:13 2021 +0800
    >   ```
    >
    >   > `HEAD->master` 说明`HEAD`引用`master`，并一同引用最新的提交记录。
    >   >
    >   > `origin/master, origin/HEAD`， 表示关联仓库的分支依旧停留在原先的位置。

##### master分支

​		Git仓库默认拥有`master`分支，即主分支。

​		新建的仓库没有任何`commit`，但是当创建第一个`commit`时，`master会指向它`，并且`HEAD`会指向`master`。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779f5c66ac9e~tplv-t2oaga2asx-watermark.awebp)

​		从关联仓库`git clone`时，会从关联仓库下载`.git`到工作目录中，还会签出`master`。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779f5c191a3f~tplv-t2oaga2asx-watermark.awebp)

> ​		大多数团队会规定以`master`为核心，所有分支的开发围绕着`master`来进行。
>
> ​		所有的 `branch` 之间都是平等的。

##### 操作

-   查看分支

    `git branch`

    > `git branch -a`查看所有分支

-   创建分支
    `git branch [bname]`

-   删除分支
    `git branch -d [bname]`

    > - `HEAD` 指向的 `branch` 不能删除
    >
    >   如果要删除 `HEAD` 指向的 `branch`，需要先用 `checkout` 把 `HEAD` 指向其他地方。
    >
    > - 删除 `branch` 是删掉引用，不删除任何的 `commit`
    >
    >   `branch`只是一个引用。
    >
    >   > ​		如果`branch`的删除会造成`commit`孤悬，那么在一定时间后，它会被 Git 的回收机制删除掉。
    >   >
    >   > > 孤悬：`commit` 不在任何一个 `branch` 的「路径」上，或者换句话说，如果没有任何一个 `branch` 可以回溯到这条 `commit`
    >
    > - 未被合并到 `master` 过的 `branch` 在删除时会失败
    >
    >   防止误删「未完成」的 `branch`。
    >
    >   > ​		如果确认是要删除这个 `branch` ，可以把 `-d` 改成 `-D`，小写换成大写，就能删除了。
    >   >
    >   > > （例如某个未完成的功能被团队确认永久毙掉了，不再做了）
    
-   切换分支

    `git checkout [bname]`

    将要切换到的分支上的最近提交记录签出。实际上是将``HEAD``引用到该分支引用上。

    >   `git checkout -b [bname]`
    >
    >   ​	如果`bname`分支不存在，则创建并切换到该分支。

-   合并分支

    `git merge [bname]`

    将`bname`分支合并到当前分支上。

    >   合并只是将该分支的提交记录同步到当前分支上，但该分支依然存在。可以选择在合并后，删除该分支。

    >   合并可能会导致冲突，如果冲突，需要手工合并。合并完成后，add并重新提交。

#### 引用偏移

​		Git中有两个偏移符号，`^`,`~`。用以通过给定的引用来回溯若干`commit`

- `^`，偏移单计数

  每个`^`代表偏移一次。

  > `master^`
  >
  > `HEAD^^`

- `~`，偏移批量计数

  `~N`代表偏移N次

  > `HEAD~5`

### 提交 commit

​		将暂存区内容添加到提交记录中

>   ​		暂存区的目录树写到对象库中，分支会做相应的更新。即分支目录树就是提交时暂存区的目录树。

`git commit {file_list} -ma"message"`

​		将暂存区中缓存的所有工作内容作为一次记录，提交到仓库。

> HEAD将指向这次提交记录。
>
> 会弹出一个文件要求提供提交信息。
>
> `{file_list}`，提交暂存区的指定文件到提交记录，文件列表缺省默认提交暂存区中所有文件

- `-m`，带信息的提交

  `git commit -m"{commit message}"`

  > commit message 作为提交信息提供，不再弹出文件编辑。
  >
  > Linux 系统中，commit 信息使用单引号
  >
  > Windows 系统，commit 信息使用双引号

- `-a`，组合`git add`

  `git commit -a`

  修改文件后不需要执行 git add 命令，直接来提交。文件必须已经加入Git的管理。

- `--amend`，修正`commit`

  `git commit --amend`

  将`HEAD`指向的`commit`和暂存区的内容合并成一个新的`commit`，用新的`commit`替换当前的`commit`。

  > `--amend`只能修改当前`commit`的内容，
  >
  > ​		注意是合并暂存区，所以修改的内容，需要手动`git add`到暂存区。

### 查看提交记录

> 可以gitk来优化查看体验。

#### log

`git log`	查看历史提交记录。

> ```shell
> commit 9b6d2c9488110e0ba8834c6bf202f9be8180003a (HEAD -> master)
> Author: mass <mazengrong12211@163.com>
> Date:   Wed Nov 3 21:25:37 2021 +0800
>     new
> 
> commit 7c71d3b861454ecd715551d84e751446532f4b55 (origin/master, origin/HEAD)
> Merge: 1ea72f9 707ae91
> Author: mass <mazengrong12211@163.com>
> Date:   Tue Nov 2 16:37:13 2021 +0800
>     merge store
>     Merge branch 'master' of https://gitee.com/masstsing/gpra
> 
> commit 1ea72f9a1e58a4752531ce949dd7d4ded01b53bb
> Author: mass <mazengrong12211@163.com>
> Date:   Tue Nov 2 16:35:11 2021 +0800
>     shop
> ```
>
> 
>
> 

>   -   `--oneline`， 单行输出
>
>   -   `--graph`， 查看历史分支结构图
>
>   -   `--reverse`，逆时序查看
>
>   -   `--author=NAME`，查看指定作者的提交记录
>
>   -   `--decorate`, 查看带标签的提交记录
>
>   -   `--since`
>
>   -   `--before`
>
>   -   `--until`
>
>   -   `--after`
>
>   -   `--patch / -p` 查看每个`commit`的变动细节，包括内容。可以用于代码`review`
>
>   -   `--stat`， 查看简化的变动内容，每个文件的变动行数。
>
>       
>
>   >   [Git - git-log Documentation (git-scm.com)](http://git-scm.com/docs/git-log)
>
>   >   EX:
>   >
>   >   ```shell
>   >    git log 
>   >    	--oneline 
>   >    	--before={3.weeks.ago} 
>   >    	--after={2010-04-18} 
>   >    	--no-merges
>   >   ```

#### blame

`git blame [fname]`， 查看指定文件历史修改记录

#### diff

>  详见 暂存区/ 差异 diff

`git diff --staged`， 查看暂存区和上一条`commit`的差异。

#### show

`git show [commit/HEAD/bname]`， 查看`commit`的改动内容。

> - `git show`， 查看`HEAD`引用的`commit`的改动内容
>
>   ```shell
>   Author: mass <mazengrong12211@163.com>
>   Date:   Wed Nov 3 21:25:37 2021 +0800
>               
>       new
>               
>   diff --git a/new.txt b/new.txt
>   new file mode 100644
>   index 0000000..c586658
>   --- /dev/null
>   +++ b/new.txt
>   @@ -0,0 +1 @@
>   +aaa`:
>   ```

- 查看`commit`版本中的指定文件

  `git show [commit] [fname]`

  > ```shell
  > # git show new.txt 
  > commit 9b6d2c9488110e0ba8834c6bf202f9be8180003a (HEAD -> master)
  > Author: mass <mazengrong12211@163.com>
  > Date:   Wed Nov 3 21:25:37 2021 +0800
  >     new
  > 
  > diff --git a/new.txt b/new.txt
  > new file mode 100644
  > index 0000000..c586658
  > --- /dev/null
  > +++ b/new.txt
  > @@ -0,0 +1 @@
  > +aaa`:`
  > ```

#### reflog

​		`reflog` 是 "reference log" 的缩写，使用它可以查看 Git 仓库中的引用的移动记录。

`git reflog [HEAD/bname]`

> `git reflog master`

- 缺省显示 `HEAD` 的移动记录

- 恢复分支引用

  > ​	假如误删了 `branch1` 这个 `branch`，那么你可以查看一下 `HEAD` 的移动历史：
  >
  > `git reflog`
  >
  > ![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe3de05468c613~tplv-t2oaga2asx-watermark.awebp)
  >
  > > ​		`HEAD` 的最后一次移动行为是「从 `branch1` 移动到 `master`」。而在这之后，`branch1` 就被删除了。所以它之前的那个 `commit` 就是 `branch1` 被删除之前的位置了，也就是第二行的 `c08de9a`。
  >
  > `git checkout c08de9a` ，移动`HEAD`到该`commit`
  >
  > `git checkout -b branch1`，在该`commit`上创建分支引用
  >
  > 删除的 `branch1` 就找回来了
  >
  > > ​		不再被引用直接或间接指向的 `commit` 会在一定时间后被 Git 回收，所以使用 `reflog` 来找回删除的 `branch` 的操作一定要及时，不然有可能会由于 `commit` 被回收而再也找不回来。

  

#### 查看改动内容的方法

![image-20211108224814868](https://gitee.com/masstsing/picgo-picserver/raw/master/image-20211108224814868.png)

### 推送 push

​		上传本地的分支版本到关联仓库合并。

>   ​		把当前`branch`引用的`commit`路径上的所有未上传的`commit`上传到关联仓库。

```shell
[root@instance-m7qyn6sr gpra]# git status
On branch master
Your branch is ahead of 'origin/master' by 2 commits.
  (use "git push" to publish your local commits)
```

>   `Your branch is ahead of 'origin/master' by 2 commits.
>    (use "git push" to publish your local commits)`
>
>   -   本地仓库该分支上的提交记录已经领先于关联仓库
>   -   可以使用git push来向远端发布本地的提交记录
>       -   会提交本地领先远端的若干提交记录
>
>   ![本地仓库超前于远程仓库](https://gitee.com/masstsing/picgo-picserver/raw/master/20211106182413.webp)
>
>   >   上传5,6两个`commit`

-   `git push`是针对分支的，多个分支需要切换分支来进行推送。

    ```shell
    git chekcout feature1
    git push origin feature1
    ```

    >   `origin feature`，用于指定推送到的关联仓库和分支。
    >
    >   （Git 2.0起）如果`push`的是，从关联仓库`clone/pull`时没有的分支，就必须指明主机名和分支名。
    >
    >   这个行为可以通过`git config push.default`来改变。

    `push`只会将当前分支的引用上传，而不会将本地的`HEAD`信息上传。

    >   所以远程仓库的`HEAD`不会被更新，只会永远引用`master`分支。

-   push并不一定能够成功

    ​		Git 的`push` 其实是用本地仓库的 commits 记录去覆盖并更新远端仓库`branch`上的 commits 记录，如果在远端仓库含有本地没有的 commits 的时候，`push` （如果成功）将会导致远端的 commits 被擦掉。这种结果当然是不可行的，因此 Git 会在 `push` 的时候进行检查，如果出现这样的情况，push 就会失败。

    >   ```shell
    >   ! [rejected]        master -> master (fetch first)
    >   error: failed to push some refs to 'https://gitee.com/masstsing/gpra.git'
    >   hint: Updates were rejected because the remote contains work that you do
    >   hint: not have locally. This is usually caused by another repository pushing
    >   hint: to the same ref. You may want to first integrate the remote changes
    >   hint: (e.g., 'git pull ...') before pushing again.
    >   hint: See the 'Note about fast-forwards' in 'git push --help' for details.
    >   ```

    需要先`pull`把关联仓库的内容取回本地，进行冲突合并后再推送。

>   ​		合并冲突时，当 `pull` 操作发现不仅远端仓库包含本地没有的 `commit`s，而且本地仓库也包含远端没有的 `commit`s 时，它就会把远端和本地的独有 `commit`s 进行合并，自动生成一个新的 `commit`。
>
>   >   ​		和手动的 `commit` 不同，这种 `commit` 会自动填入一个默认的提交信息，简单说明了这条 `commit` 的来由。你可以直接退出界面来使用这个自动填写的提交信息，也可以修改它来填入自己提交信息
>
>   ​		这种「把不同的内容进行合并，生成新的提交」的操作，叫做合并（呵呵呵哈哈），它所对应的 Git 指令是 `merge`。事实上，`git pull` 这个指令的内部实现就是把远程仓库使用 `git fetch` 取下来以后再进行 `merge` 操作的。

#### 操作

`git push <remote_name> <remote_bname>:<local_bname>`

>   如果本地分支名与远程分支名相同，则可以省略冒号：
>
>   ​	`git push <remote_name> <local_bname>`
>
>   >   ` git push origin master:master`

- 将所有提交记录推送到关联仓库

  `git push`

- 推送分支到关联仓库

  `git push [-u] origin [branchs]`
  
  >   `git push origin master`
  
  `git push --force origin master`
  
  > 本地版本与远程版本有差异，强制推送使用 --force 参数：
  
  
  
- 删除主机的分支可以使用 --delete 参数

    `git push origin --delete master`

- `-f`, `--force`，忽略冲突，强制 `push`

### 获取 fetch

​		用于从关联仓库获取指定的`branch`到当前`HEAD`的工作区。

`git fetch [host_alias] [bname_list]`

>    命令执行完后需要执行 git merge 远程分支到你所在的分支。
>
>    缺省形式`git fetch`为拉取远程仓库所有分支上比本地更新的`commit`



### 合并  merge

​		指定一个`commit`，把它与当前`commit`的工作区合并（`HEAD`工作区)。

> ​		理想情况下，git能自动的在工作区合并，添加暂存并提交记录，如果出现合并冲突，则需要手工缝合冲突（conflict）。这些手工缝合做为改动，留驻在工作区。需要重新手工添加暂存并提交。

#### 合并分支

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2aad5a0279~tplv-t2oaga2asx-watermark.awebp)

- 合并其他分支上的提交记录到当前分支上

    `git merge [alias]/[bname]`
    
    > `git merge branch1`
    >
    > ​		会把 `5` 和 `6` 这两个 `commit` 的内容一并应用到 `4` 上，然后生成一个新的提交，并跳转到提交信息填写的界面
    >
    > ​		`merge` 操作自动地填写简要的提交信息。在提交信息修改完成后退出，`merge` 就算完成了。
    >
    > ​		`bname`引用串上的`commit`会按照时间排序，插入到`HEAD`所引用分支上的`commit`串里。形成一个按时间排序的新`commit`串。
    
- 合并多个分支

    `git merge origin/master hotfix-2275 hotfix-2276 hotfix-2290`

##### 冲突 conflict

​		如果在合并时，两个分支上的`commit`之间存在对某一内容的竞争性修改。那么需要人工参与对内容的合并。合并完成之后手动提交来产生`commit`。

​		Git会报告发生冲突的文件，并在文件中发生冲突的位置，使用符号标注分支之间导致冲突的不同内容。

> ​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2af5b06ef6~tplv-t2oaga2asx-watermark.awebp)
>
> ​		手工修改时，应该将冲突标注内容全部删除后，做内容上的修改。保存退出之后重新`git add`并``commit``。此时的`commit`依然会带有自动合并时的提交信息。
>
> ​		也可以使用更方便的merge工具。可以在网上搜索。

​		如果发生冲突时，想放弃合并。则需要将添加到`HEAD`工作区上的其他`branch`上的内容放弃。此时：`git merge --abort`即可。

#### 同支合并

​		`merge` 时的目标 `commit` 和 `HEAD` 引用的 `commit` 并不存在分叉，而在同一`branch`的引用串上。

> 比如说已经合并过的分支。

- `HEAD`领先于目标`commit`

  ​	在这种情况下， Git 什么也不会做，`merge` 是一个空操作。

  ​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2b2357b9d9~tplv-t2oaga2asx-watermark.awebp)

  > ​		`merge` 没必要再创建一个新的 `commit` 来进行合并操作，因为并没有什么需要合并的。`HEAD`引用的`commit`本就是目标`commit`修改后的版本提交记录。

- `HEAD`落后于目标`commit`

  ​	Git 会直接把 `HEAD`（以及它所指向的 `branch`，如果有的话）移动到目标 `commit`。即`fast-forward`（快速前移）

  ​	![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2b2486758a~tplv-t2oaga2asx-watermark.awebp)

  > ​		一般情况下，创建新的 `branch` 都是会和原 `branch` （例如上图中的 `master` ）并行开发的，不然没必要开 `branch` ，直接在原 `branch` 上开发就好。
  >
  > ​		但事实上，上图中的情形其实很常见，因为这其实是 `pull` 操作的一种经典情形：本地的 `master` 没有新提交，而远端仓库中有来自于别处到 `master`的新提交
  >
  > ​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2b46c69d46~tplv-t2oaga2asx-watermark.awebp)
  >
  > > ​		`origin/master` 和 `origin/HEAD` 是是对远端仓库的 `master` 和 `HEAD` 的本地镜像。它们是一直存在于本地仓库中的。
  > >
  > > ​		在 `git fetch` 下载远端仓库内容时，这两个镜像引用得到了更新：`origin/master` 和 `origin/HEAD` 移动到了最新的 `commit`。
  > >
  > > ​		所以这里其实是：`git merge origin/HEAD`.
  > >
  > > ​		`HEAD` 会带着 `master` 指向最新 `commit`

### 拉取 pull

​		从关联仓库拉取最新提交记录，并合并本地的版本

>   ​		`git pull` 其实就是 `git fetch`和 `git merge FETCH_HEAD` 的简写
>
>   ​		在工作目录中 获取（fetch） 并 合并（merge 远端的改动。

`git pull <remote_name> <remote_bname>:<local_bname>`

>   `git pull origin master:brantest`

-   <remote_bname>:<local_bname>, 缺省默认都为master

    `git pull` / `git pull origin`

-   如果远程分支与当前分支合并，则冒号后面的部分可以省略。

    `git pull origin master`



### 签出(检出) checkout

签出指定的commit。

> ​		签出是指在仓库的角度，将提交记录的工作内容，拉出到工作区。
>
> ​		`checkout` 的意思就是把某个 `commit` 作为当前 `commit`，把 `HEAD` 移动过去，并把工作目录的文件内容替换成这个 `commit` 所对应的内容）。
>
> > 可以将签出简单的理解为，拉到台面上来。
>
> ​		`checkout` 和 `reset` 都可以切换 `HEAD` 的位置，`reset` 在移动 `HEAD` 时会带着它所指向的 `branch` 一起移动，而 `checkout` 不会。
>
> > ​		`checkout` 有一个专门用来只让 `HEAD` 和 `branch` 脱离而不移动 `HEAD` 的用法：
> > ​		`git checkout --detach`
> >
> > ![git checkout --detach](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/30/1600acce7b90b009~tplv-t2oaga2asx-watermark.awebp)

`git checkout [commit/bname/HEAD]`

- 签出分支引用的commit到工作区

  `git checkout [bname]`

  > - `git checkout -b [bname]`, 签出分支到工作区，不存在则创建
  >
  >   > `git checkout -b new_feature origin/master`
  >   >
  >   > 基于远程 master 分支切出一个新分支
  >
  > - `git checkout HEAD^^`
  >
  > - `git checkout master~5`
  >
  > - `git checkout 78a4bc`
  >
  > - `git checkout 78a4bc^`

  

- 签出文件到工作区，替换工作区中对应的文件

  `git checkout -- [fname]`

  > - `.`，当前提交记录全部
  >
  > - 可以用于丢弃当前工作区中对应文件的改动。
  > - 是用``HEAD``对应`commit`中的文件，替换工作区中对应的文件。
  > - 已经添加到暂存区的文件，或者新创建未追踪文件不会受到影响。
  >
  > >​		如果想丢弃本地工作区中所有改动和提交记录，则获取关联仓库的最新版本，并改动主分支指向：
  > >
  > >`git fetch origin`
  > >
  > >`git reset --hard origin/master`

- 签出文件到工作区和暂存区

  `git checkout HEAD [fname]`

  > `.`， HEAD 指向的提交记录全部
  >
  > 可以用于丢弃当前工作区、暂存区中对应文件的改动。

- 从远程分支创建并签出分支

  `git checkout -b [bname] origin/[bname]`

  > ​	`git clone + 远程仓库地址`将项目下载下来之后，倘若远程仓库有多个分支，我们会发现，使用`git branch`查看本地分支时，只有一个master分支
  >
  > ​	使用`git branch -a`查看远程分支,那些以rmotes开头且为红色的都是远程分支，或者理解为**这些分支是隐藏的.**
  >
  > ​	
  >
  > ​	此命令将远程分支与本地分支联系起来，或者使用`-t`参数在本地建立一个和远程分支名字一样的分支：
  >
  > ​	`git checkout -b feature origin/feature`
  >
  > ​	`git checkout -t origin/feature`

### 标签 tag

​		标签可用于记录版本或其他工作节点的记录。

`git tag -a <tagname> -m "runoob.com标签"`

> -   commit_id 可以是提交记录的前若干位哈希值，足以在仓库中唯一标识提交记录。
>
> -   缺省commit_id 默认为当前HEAD指向的提交记录

-   `-a`，创建一个带注解的标签

    `git tag -a tag_message `

    >   缺省`-m`会弹出编辑器，编辑一句标签注解。

-   `-s`，PGP签名标签

-   查看所有标签

    `git tag`

### 重置 reset

​		重置（移动）HEAD，及它指向的分支。

`git reset --soft/mixed/hard [HEAD]/[master]/[commit]`

>   -   `hard`
>
>       工作区和暂存区都重置到与指定提交记录一致
>
>   -   `mixed`
>
>       重置暂存区，工作区内容保持不变。
>
>       >   ​		暂存区的目录树会被重写，被 master 分支指向的目录树所替换，但是工作区不受影响。
>
>   -   `soft`
>
>       工作区和暂存区都重置，并把内容修改放进暂存区
>
>   缺省默认为`mixed`

>   -   `^`代表父节点
>
>       可叠加表示若干层
>
>       `^`个数为0时`HEAD`表示仍旧保持在当前提交记录，那功能就成了，只操作工作区和暂存区。此时依据`hard/soft`而有不同。
>
>   -   `~N`
>
>       指定回退父节点的层数，可以为0，等同于`HEAD`

>   -   `[fname]`
>
>        回退指定文件版本

-   HEAD不移动，重置暂存区

    `git reset [HEAD]`

-   回退

    -   回退到上个版本

        > `git reset --hard HEAD^`
        >
        > ​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fe19c8a3235853~tplv-t2oaga2asx-watermark.awebp)
        >
        > 工作区和暂存区也都随着`commit`迁移。
        >
        > > ​		被撤销的那条提交并没有消失，只是你不再用到它了。如果你在撤销它之前记下了它的 `SHA-1` 码，那么你还可以通过 `SHA-1` 来找到他它。

    -   回退到指定版本

        `git reset --hard commit_id/bname~N/HEAD~N`

-   回退到关联仓库版本

    `git reset --hard origin/[bname] `

### 变基 rebase

​		重新设置分支的`commit`序列的基础点（父`commit`）。

`git rebase [HEAD/bname/commit]`

> ​		将指定`commit`所在的分支上的所有`commit`串，重新提交到指定的`commit`后。
>
> ​		`merge`之后`commit`历史就会出现分支叉再汇合的结构，如果不希望`commit`历史出现分支叉，可以用`rebase`代替`merge`。
>
> `git merger branch1`
>
> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fdea7b6646a1f3~tplv-t2oaga2asx-watermark.awebp)
>



> `git checkout branch1`
>
> `git rebase master`
>
> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/1600abd620a8e28c~tplv-t2oaga2asx-watermark.awebp)
>
> `git checkout master`
>
> `git merge branch1`
>
> > 移动master引用。
>
> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/160149e054fe485c~tplv-t2oaga2asx-watermark.awebp)
>
> > ​		假设master上的记录都已经提交并推送到了关联仓库，那么如果在master上向分支`rebase`，因为`master`跟随变基引用到`rebase`后分支上的最新`commit`，导致原来`master`上自分叉点开始的提交都被剔除（没有引用指向了）。因为这些提交在关联仓库存在，从而导致无法`push`
> >
> > ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/16014b5a6919c0b7~tplv-t2oaga2asx-watermark.awebp)
> >
> > > 为了避免这种情况发生，一般是在其他分支上向`master`主支`rebase`。
> > >
> > > 其他的两个分支之间进行`rebase`就不必如此。

- `git rebase HEAD`

  `rebase`到当前所在分支，称为原地`rebase`。相当于空操作。4

  > `git rebase HEAD~2`
  >
  > ​		把`HEAD`引用的`commit`，`rebase`到`HEAD`前两个`commit`。实际上操作完成之后，没有变化。

  ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fdf5fd00522381~tplv-t2oaga2asx-watermark.awebp)

- `-i`， 交互式`base`

  `git rebase -i `/ `git rebase --interavtive`

  > ​		在`rebase`前，可以指定要`rebase`的`commit`串中的每一个`commit`是否进一步修改。可以据此来进行一次原地`rebase`，来修正此前提交中中的内容而不添加新的`commit`。
  >
  > > `git commit --amend`
  >
  > `git rebase -i HEAD^^`， 可以在原地`commit`加上`-i`，进入交互界面
  >
  > ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fdf5fd04f46d6e~tplv-t2oaga2asx-watermark.awebp)
  >
  > - 顶部列出将要被`rebase`的所有`commit`
  >
  >   最老的`commit`在最上
  >
  > - `pick`，操作`commit`的方式，`pick`为直接应用
  >
  >   - `edit`， 将`commit`操作方式改为，应用`commit`，然后停下来等待继续修正。
  >   - 其他操作列在上图`Commands`中
  >   - 删除该条操作记录，可以从分支串中移除该`commit`
  >
  > 退出界面后，命令提示`rebase`停止在操作中断的位置，`edit`则可以修改`commit`了。
  >
  > ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fdf5fd007159fa~tplv-t2oaga2asx-watermark.awebp)
  >
  > `git rebase --continue`可以完成当前`rebase`中断位置，继续到下一个操作终端的地方。
  >
  > 在`edit`操作中断的地方，可以使用`git commit --amend`进行修正。
  >
  > `git rebase --continue`，修改每个操作中断的`rebase`位置，最终会提示`rebase`成功。
  
- `--onto`， 指定`rebase`时重新提交的起点

​		默认的，`rebase`时起点是自动判定的，起点是当前`commit`和目标`commit`交叉点。

> 起点`commit`在`rebase`时不包含在内，从其子`commit`开始。

​		`--onto`可以指定`rebase`时目标`commit`、起点`commit`、终点`commit`

> `git rebase --onto commit3 commit4 branch1`
>
> ![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe24400d7d73d0~tplv-t2oaga2asx-watermark.awebp)
>
> - commit3  指定目标
> - commit4 指定起点（从子`commit`开始）
> - `branch1`，终点commit

​		可以使用原地`rebase --onto`来撤销提交：

> `git rebase --onto HEAD~2 HEAD~1 branch1 `
>
> > ​		以倒数第二个 `commit` 为起点（起点不包含在 `rebase` 序列里哟），`branch1` 为终点，`rebase` 到倒数第三个 `commit` 上。
>
> ![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe243fce5804fd~tplv-t2oaga2asx-watermark.awebp)



### 还原 revert

​		将指定`commit`的修改内容还原，并生成一个新的`commit`。

> ​		还原并非直接删除一个提交记录，而是新建一个提交记录，撤销指定`commit`中做出的修改。
>
> ​		将需要revert的版本的内容再反向修改回去，版本会递增，不影响之前提交的内容

`git revert [commit]/[HEAD]/[bname]`

> `git revert HEAD`，撤销前一次 commit
>
> `git revert HEAD^`，撤销前前一次 commit
>
> `git revert commit` 撤销指定的版本
>
> > ​		对于关联仓库，`revert` 完成之后，把新的 `commit` 再 `push` 上去，这个 `commit` 的内容就被撤销了。
>
> >  		如果出错内容在 `master`：不要强制 `push`，而要用 `revert` 把写错的 `commit` 撤销。
> >
> > ​		如果出错内容在私有 `branch`：在本地把内容修正后，强制 `push` (`push -f`）一次就可以解决。
> >
> > > ​		本地对已有的 `commit` 做了修改，这时你再 `push` 就会失败，因为中央仓库包含本地没有的 `commit`s。但这个和前面讲过的情况不同，这次的冲突不是因为同事 `push` 了新的提交，而是因为你刻意修改了一些内容，这个冲突是你预料到的，你本来就希望用本地的内容覆盖掉中央仓库的内容。那么这时就不要乖乖听话，按照提示去先 `pull` 一下再 `push` 了，而是要选择「强行」`push`：
> > >
> > > `git push origin branch1 -f`

### 储藏 stash

​		用于暂存工作现场。

> ​		git的暂存区是所有分支共享的，因此，只要未提交(commit)的内容都会在所有分支上呈现。有时，我们还未完成当前分支的功能开发，但是又亟需在另一个分支上修改内容。可以使用“git stash”(储藏)来解决这个问题。

- `git stash push [-m|--message <message>]`

  ​		将当前添加到索引的内容储藏起来，并使用 HEAD 的内容对当前工作区进行覆盖。

  > ​		当你手头有一件临时工作要做，需要把工作目录暂时清理干净，那么你可以直接：`git stash`。
  >
  > ​		工作目录的改动就被清空了，所有改动都被存了起来。
  >
  > 然后你就可以从你当前的工作分支切到 其他分支。回到当前分支后，可以`git stash pop`将存储的内容取回工作区。
  >
  > > ​		没有被 track 的文件（即从来没有被 add 过的文件不会被 stash 起来，因为 Git 会忽略它们。如果想把这些文件也一起 stash，可以加上 `-u` 参数，它是 `--include-untracked` 的简写。就像这样：
  > >
  > > ​		`git statsh -u`

- `git stash list`

  查看储藏记录列表

  > <储藏记录名称>: On <分支名称>: <备注信息>

- `git stash apply [--index]`

  从储藏版本恢复，并保留记录，若不指定储藏版本，则默认为最近的储藏版本。

- `git stash drop <stash>`

  删除指定的储藏版本记录。

  > stash 储藏记录名称，可以通过“git stash list”查看。

- `git stash pop [--index]`

  从储藏版本恢复，并删除记录.

  >  若不指定储藏版本，则默认为最近的储藏版本。

- `git stash clear`

  清空所有的储藏记录，并不可恢复。非必要情况下，不建议使用。

- `git stash branch <branchname> [<stash>]`

  从指定储藏记录创建分支会包含指定的储藏内容.

  > ​		我们知道在当前分支创建新的分支会克隆当前分支已提交的内容，但是不会包含未提交的内容。
  >
  > ​		这条命令可以从指定储藏记录创建分支，使得从未提交的内容中创建分支。

## github

​		github是一个基于Git的代码托管平台，作为第三方的远端关联仓库使用。

### ssh加密传输

​		本地的Git仓库与github仓库之间的传输是通过ssh加密的，所以需要配置验证信息。

-   生成公钥私钥

​		`ssh-keygen -t rsa -C "youremail@example.com"`

>    **your_email@youremail.com** 改为你在 Github 上注册的邮箱

-   github配置公钥



## gitee 

​		gitee可以视作github的平替，国内速度快。

​		同一个本地仓库可以同时关联github和gitee，等多个远端仓库。

​		用多个远程库时，我们要注意，git 给远程库起的默认名称是 origin，如果有多个远程库，我们需要用不同的名称来标识不同的远程库。

>   `git remote add github git@github.com:tianqixin/runoob-git-test.git`
>
>   `git remote add gitee git@gitee.com:imnoob/runoob-test.git`

![img](https://www.runoob.com/wp-content/uploads/2020/03/gitee8.png)

如果要推送到 GitHub，使用命令：

```shell
git push github master
```

如果要推送到 Gitee，使用命令：

```shell
git push gitee master
```



## 搭建私有Git服务器

-   安装Git

    ```shell
    $ yum install curl-devel expat-devel gettext-devel openssl-devel zlib-devel perl-devel
    $ yum install git
    ```

-   创建Git用户组

    ```shell
    $ groupadd git
    $ useradd git -g git
    ```

-   创建登录证书

    ​		收集所有需要登录的用户的公钥，公钥位于id_rsa.pub文件中.

    ​		把我们的公钥导入到``/home/git/.ssh/authorized_keys``文件里，一行一个。

    ```shell
    $ cd /home/git/
    $ mkdir .ssh
    $ chmod 755 .ssh
    $ touch .ssh/authorized_keys
    $ chmod 644 .ssh/authorized_keys
    ```

-   初始化Git仓库

    选定一个目录作为Git仓库，假定是/home/gitrepo/runoob.git，在/home/gitrepo目录下输入命令：

    ```shell
    $ cd /home
    $ mkdir gitrepo
    $ chown git:git gitrepo/
    $ cd gitrepo
    
    $ git init --bare runoob.git
    Initialized empty Git repository in /home/gitrepo/runoob.git/
    ```

    把仓库所属用户改为git：

    ```shell
    $ chown -R git:git runoob.git
    ```

-   克隆仓库使用

    ```shell
    $ git clone git@192.168.45.4:/home/gitrepo/runoob.git
    Cloning into 'runoob'...
    warning: You appear to have cloned an empty repository.
    Checking connectivity... done.
    ```

    >   192.168.45.4 为 Git 所在服务器 ip ，你需要将其修改为你自己的 Git 服务 ip。
    >
    >   这样我们的 Git 服务器安装就完成。




## 分支模型

### 瀑布模型

​		所有人都工作在`master`上，写完了的`commit`通过`push`发送到仓库。`pull`来获取别人的最新`commits`。

​		解决了多人并行开发和版本管理。

​		但是每个人的代码在被大家看到的时候，就已经进入了正式的发布版本库，所有人的工作都会被直接`push`到`master`。导致每个人的代码在正式启用前无法通过这个管理系统来`review`。

​		不能很好的进行团队协作：

> ​		现在的商业团队，开发项目多是采用「边开发边发布、边开发边更新、边开发边修复」的持续开发策略

### 发布/开发模型

​		如果所有的开发都放到一个新的branch做，那当你在新的feature还没有开发完，没办法merge回去，但是却在里面写了一个通用的小模块，想要给同事用应该怎么办.

> ​		如果新的feature是以小模块为单位，写好一个模块就commit一次，那么就可以实现只合并该分支的某一部分commits.
>
> ​		可以将这个通用小模块提交到当前分支，这样在其他分支可以cherry-pick本次commit

### 特性分支模型

- 任何新的功能（feature）或 bug 修复全都新建一个 `branch` 来写；
- `branch` 写完后，合并到 `master`，然后删掉这个 `branch`。

![Feature Branching](https://gitee.com/masstsing/picgo-picserver/raw/master/15fde6edbfe362c4~tplv-t2oaga2asx-watermark.awebp)

 两个好处：

#### 代码分享

​		不同的任务被隔离在分支中，这就使得即便`push`到中央仓库

```shell
git push origin books
```

​		![](https://gitee.com/masstsing/picgo-picserver/raw/master/16007bb30da619c5~tplv-t2oaga2asx-watermark.awebp)

​		对其他的任务也是无害的。其他人就可以从中央仓库`pull`，来`review`这个任务的内容。

```shell
git pull
git chekcout books
```

​		`review`完成后，就可以将分支合并到主支上去。

```shell
git checkout master
git pull # merge 之前 pull 一下，让 master 更新到和远程仓库同步
git merge books
```

> ​		可以先用当前分支去合并master，解决完冲突，这样master再合并当前分支的时候，就可以执行自动化发布了。
>
> > ​		开发时通常将其他分支合并到 master（而不是将 master 合并到其他分支上）来保证 master上拥有全部代码。这样的好处就是当我们产品需要打包上线时在 master 上可以随时获得全部代码。
>
> ```shell
> git pull # merge 之前 pull 一下，让 master 更新到和远程仓库同步
> git merge master # 假设HEAD在books分支上
> # 在分支books上解决冲突
> git checkout master
> git merge books
> ```

![把 books 合并到 master](https://gitee.com/masstsing/picgo-picserver/raw/master/160087ca5a7d901a~tplv-t2oaga2asx-watermark.awebp)

​		之后可以把合并结果推送到中央仓库，并将本地和中央仓库的分支都删除掉

```shell
git push
git branch -d books
git push origin -d books # 用 -d 参数把远程仓库的 branch 也删了
```

![把 master push 上去，并删除本地和远程的 books](https://gitee.com/masstsing/picgo-picserver/raw/master/1600877abc63a4d9~tplv-t2oaga2asx-watermark.awebp)

##### review challenge

​		在`review`中对任务中的内容提出质疑，进行讨论，从而改进内容。

​		在此过程中，需要不断的`push`和`pull`代码。

> review有专门的工具gerrit

##### RP：pull Request

​		PR不是Git的内容，而是各种Git仓库的提供商提供的便捷功能。可以让团队方便讨论一个`branch`，在讨论和修改完成后，一键合并`branch`到`master`。

- `push` 分支到中央仓库

- 仓库提供商在中央仓库处创建一个`Pull Request`

  ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fde6edbe5190a4~tplv-t2oaga2asx-watermark.awebp)

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fde6edbeda949d~tplv-t2oaga2asx-watermark.awebp)

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fde6edc112a514~tplv-t2oaga2asx-watermark.awebp)

​		其他人可以在`PR`上查看分支的`commit`，可以发表意见。可以根据意见修改内容，推送新的`commit`到`PR`上。

​		最终达成一致时，可以在`PR`上一键合并，中央仓库真正的将分支合并到`master`.`PR`

​	!["Merge pull request" 按钮自动在中央仓库 merge](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/30/1600884104e5bb94~tplv-t2oaga2asx-watermark.awebp)

> ​		如果 pull request 分支和 master 分支存在冲突，GitHub 还提供修改的冲突的地方。修改完了自动生成多一个 commit

#### 多任务

​		是在独立的 `branch` 上做事，切换任务是很简单的。你只要稍微把目前未提交的代码简单收尾一下，然后做一个带有「未完成」标记的提交（例如，在提交信息里标上「TODO」），然后回到 `master` 去创建一个新的 `branch` 就好了。或者使用`git stash`来保存现场。

```shell
git checkout master
git checkout -b new_feature

######## 上述命令可以简化为
git checkout -b new_feature origin/master 
#基于远程 master 分支切出一个新分支。
```

