# git

​		Git 是一个开源的分布式版本控制系统，用于管理项目开发。

---

​		它是由 Linux 之父 Linus Torvalds 开发的，并已经成为了现代软件开发领域中最流行的版本控制系统之一。

## 参考

### 指南和手册

- [Git 社区参考书](http://book.git-scm.com/)
- [专业 Git](http://progit.org/book/)
- [像 git 那样思考](http://think-like-a-git.net/)
- [GitHub 帮助](http://help.github.com/)
- [图解 Git](http://marklodato.github.io/visual-git-guide/index-zh-cn.html)

Git 完整命令手册地址：http://git-scm.com/docs

PDF 版命令手册：[github-git-cheat-sheet.pdf](https://www.runoob.com/manual/github-git-cheat-sheet.pdf)

- 1、[Git 五分钟教程](https://www.runoob.com/w3cnote/git-five-minutes-tutorial.html)
- 2、[Git GUI使用方法](https://www.runoob.com/w3cnote/git-gui-window.html)
- 3、[Github 简明教程](https://www.runoob.com/w3cnote/git-guide.html)
- 5、[互联网组织的未来：剖析GitHub员工的任性之源](https://www.runoob.com/w3cnote/internet-organization-github.html)



[手写 git hooks 脚本（pre-commit、commit-msg） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/391221822)

[十分钟理解 Git 原理 | 微信开放社区 (qq.com)](https://developers.weixin.qq.com/community/develop/article/doc/000c8e6e6c01800076481a8f251413)

[图解Git操作，一篇就够 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/263050507)

[Git 原理入门 - 阮一峰的网络日志 (ruanyifeng.com)](https://www.ruanyifeng.com/blog/2018/10/git-internals.html)

[Git 原理入门 - 阮一峰的网络日志 (ruanyifeng.com)](https://www.ruanyifeng.com/blog/2018/10/git-internals.html)

[(256条消息) git基本原理详解_git原理_乜都得的博客-CSDN博客](https://blog.csdn.net/xiaoputao0903/article/details/23912561)

[这才是真正的Git——Git内部原理揭秘！ - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/96631135)

[从基本Git指令到背后原理，带你手把手实现一个简单的Git - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/340369222)

[(256条消息) 编程实现简单的git版本管理系统_git版本管理实现_STcyclone的博客-CSDN博客](https://blog.csdn.net/STcyclone/article/details/112395164)

[从基本Git指令到背后原理，带你手把手实现一个简单的Git_git 实现_linuxguitu的博客-CSDN博客](https://blog.csdn.net/linuxguitu/article/details/111884001#:~:text=从基本Git指令到背后原理，带你手把手实现一个简单的Git 1 1. init 在学习 git 原理之前，我们先忘掉平时用的 commit，branch，tag,4 4.commit object 虽然我们已经可以用一个 tree object 来表示整个项目的版本信息了，但是似乎还是有些不足的地方： )

[万字长文肝Git----从私有Git仓库的搭建到命令的使用再到分支管理，全流程全套服务包您满意【建议收藏】-云社区-华为云 (huaweicloud.com)](https://bbs.huaweicloud.com/blogs/281902)

https://zhuanlan.zhihu.com/p/182553920

[别乱提交代码了，看下大厂 Git 提交规范是怎么做的！ - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/100773495)



[Git全套详细知识讲解（安装、常用指令、分支管理、远程仓库）-云社区-华为云 (huaweicloud.com)](https://bbs.huaweicloud.com/blogs/395581)

[Git分支开发工作流原来这么简单的【云驻计划】-云社区-华为云 (huaweicloud.com)](https://bbs.huaweicloud.com/blogs/335534)

[一文彻底搞懂Git分支原理【云驻计划】-云社区-华为云 (huaweicloud.com)](https://bbs.huaweicloud.com/blogs/335530)

[带你彻底搞懂Git上的远程分支与变基【云驻计划】-云社区-华为云 (huaweicloud.com)](https://bbs.huaweicloud.com/blogs/335536)

[阅读 git 最初版源码总结_第一版git源代码-CSDN博客](https://blog.csdn.net/m0_47696151/article/details/119305064)



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

-  SVN 中，所有开发者共享同一个中央代码库，并且需要有网络连接才能进行版本控制操作。
- SVN 中，分支比较重量级（即创建和合并分支需要花费相对更多的时间和资源），因此往往只用于重要的版本分支。
- SVN 采用递增的数字版本号来标识每个提交。
- SVN 是一种集中式版本控制系统，因此所有数据都存储在中央代码库中。如果中央代码库损坏或丢失，可能会导致数据丢失或无法恢复。
- SVN 则需要检查文件的元数据（如时间戳和文件大小）来确定是否修改。

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
    
    
    
    

- Git 中，每个开发者都拥有本地代码库的完整副本，可以离线工作并在不同的工作流程之间自由转换。
- Git 在分支管理方面比 SVN 更加强大和灵活。Git 的分支非常轻量级，创建和合并分支也很容易，因此可以轻松实现多人协作和并行开发。
- Git 使用 SHA-1 哈希值来标识每个提交
- Git 是一种分布式版本控制系统，每个开发者都拥有完整的代码库副本，保证了代码的整体性和可靠性。
- Git 使用基于内容的哈希算法来检测文件是否修改



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

https://mp.weixin.qq.com/s/UGCeUrrOR3c70nc0XdAOeA

## 仓库结构

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/git-command.jpg)

​		仓库目录下会生成一个``.git``目录，即本地仓库。包含了所有工作内容的元数据，和文件索引。当前文件夹的文件都将置于git管理工具的管理与跟踪当中。

​		本地仓库包括一个缓存（暂存区）和一个提交记录存储。

- `hooks`：存放一些shell脚本
- `info`：包含git仓库的一些信息
- `logs`：保存所有更新的引用记录。logs文件夹有refs文件夹和HEAD文件
  - `HEAD`文件保存的是所有的操作记录，使用`git reflog`查询的结果就是从这个文件来的
  - `refs`文件夹中一般会有两个文件夹：
    - **heads：** 存储所有本地分支的对象，每个本地分支名对应一个文件名称。这些分支文件中存储的是对应本地分支下的操作记录。使用git branch查看本地所有分支时，查询出的分支就是heads文件夹下所有文件的名称。
    - **remotes：**存储所有远程分支的对象，每个远程分支对应一个文件名称。这些分支文件中存储的是对应远程分支下的操作记录。
- `objects`：存放所有 `git `对象，哈希值一共40位，前 2 位作为文件夹名称，后 38 位作为对象文件名
- `refs`：引用，一般有三个文件夹：
  - `heads`：存储所有本地分支的对象，每个本地分支名对应一个文件名称，文件中存储了分支最近一次提交commit对应的id（是一个哈希值）。
  - `remotes`：远程仓库信息，其中\refs\remotes\origin\HEAD记录了当前分支指向的远程分支，即当前分支提交到的远程分支
  - `tags`： 发布重要版本时，用于标记此里程碑。
- `COMMIT_EDITMSG`：存储着最近一次的提交信息,`Git`系统不会用到这个文件，只是给用户一个参考
- `config`：存储当前仓库的配置信息
- `description`：描述信息
- `HEAD`：**HEAD**指针，它指向了当前分支，这个文件记录了当前分支是哪个分支。
- `index`：暂存区（stage），是一个二进制文件



## 体系

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/git-process.png)

​		git维护本地仓库的三棵树：工作区（changes）、暂存区（Index、stage）、提交记录（repo）。

- 工作区是当前签出的提交记录的最新更改

  - 签出是指从仓库中，将某个提交记录记录的版本提出到文件系统中展示

- 暂存区是仓库的缓存区，工作环境的保存可以暂存在缓存中

  - **中间状态管理**：暂存区允许你在提交之前选择哪些更改要包含在下一次提交中。这使得你可以分批处理和组织更改，而不需要一次性提交所有修改。

  - **准备提交**：将文件的更改添加到暂存区后，Git会记录这些更改的信息。这可以让你在真正提交之前检查和确认要提交的内容。

  - **分步提交**：通过暂存区，你可以将不同的文件或更改按逻辑分组，这样可以更清晰地记录和理解项目的历史。

  - **避免错误提交**：当只想提交部分更改时，暂存区可以帮助你避免将不相关的文件或修改一起提交，从而减少错误。

  - **多次提交一次更改**：例如，你可以在暂存区中准备多个文件的更改，分开提交不同的部分，这样有助于保持版本历史的清晰。

    > ​		也就是说，每次提交，都是将当前添加到暂存区的内容提交。工作区中的更改，可以分批添加到暂存区中来清晰的分为几次提交。
    >
    > ​		所以stage翻译为暂存区并不能体现出其实际的作用，当做**提交准备区**更为实际。

- 确认提交到仓库时工作内容会作为一个提交记录被仓库记录，以HEAD标识。

  - 版本库包含Git存储库的所有历史记录和元数据。
  - 工作区和提交准备区是`git`

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/trees.png)

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/1352126739_7909.jpg)

​		`objects`，是存储Git元信息的存储库。Git并非按照文件组织，而是按照对象进行存储，通过索引的方式，来组成仓库中的版本。





### 工作区

​		`.git`，同级目录下的文件结构称为工作区中的工作目录，保存了从仓库中签出（checkout）的内容。

>   ​		工作区相当于简单的系统目录，是Git中某版本的内容副本。对于工作区的操作就是普通的操作系统文件结构操作，不需要特定的命令来操作工作区的内容。

#### 未跟踪

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

#### 已修改未暂存

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

#### .gitignore

​		`.gitignore`，用以指定不被git追踪的文件。

> ​		在 GitHub 上创建仓库，你可以在创建仓库的界面中就通过选项来让 GitHub 帮你创建好一个符合项目类型的 `.gitignore` 文件，你就不用再自己麻烦去写一大堆的配置了。

​		![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe3f2100f33d2a~tplv-t2oaga2asx-watermark.awebp)

> **规则：**
>
> ​		[Git - gitignore Documentation (git-scm.com)](https://git-scm.com/docs/gitignore)

​		有时候`git`推送时，`.gitignore`文件中指明忽略的文件在`push`之后还是会显示。是因为文件在早先已经被`git`追踪，再添加到`.gitignore`中是无效的，添加之后，需要清楚一下对应的`git`缓存。`git -rm -r --cached file_name`，然后再`commit/push`



### 暂存区

​		暂存区是`.git`下一个叫``index``的文件。它是一个索引，用于记录被添加到暂存区中的文件的对象存储。

​		stage 这个词在 Git 里，是「集中收集改动以待提交」的意思；而 staging area ，就是一个「汇集待提交的文件改动的地方」。简称「暂存」和「暂存区」。至于 staged 表示「已暂存」。或称提交准备区。

> ```shell
> [root@instance-m7qyn6sr gpra]# git status
> On branch master
> Your branch is up to date with 'origin/master'.
> 
> Changes to be committed:
>   (use "git restore --staged <file>..." to unstage)
>         new file:   shoping.txt
> ```
>
> -   `On branch master`
>
> -   `Your branch is up to date with 'origin/master'.`
>
>     当前本地master分支已经更新到origin的master.
>
>     （没有落后）
>
> -   `Changes to be committed:
>     (use "git restore --staged <file>..." to unstage)
>           new file:   shoping.txt`
>
>     工作区中内容已被添加到暂存区中，成为`staged`状态。可以使用`git restore --staged`来从暂存区溢出。

​		暂存区是所有分支共享的，因此，只要未提交(commit)的内容都会在所有分支上呈现。

​		有时，我们还未完成当前分支的功能开发，但是又亟需在另一个分支上修改内容(如：修复一个 BUG)：

- 一种方式是通过“git commit”(提交)，解决这个问题。

  ​		但在正常使用中，我们应该在完成某个功能的开发后才提交一个版本，而不是频繁的提交。

- 使用`git stash`来储藏暂存区索引上的内容，并使用`HEAD`的内容覆盖当前工作区。



### 提交记录（仓库）

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

>  		字符串可以是
>
> -  `commit` 的 SHA-1 码（例：`c08de9a4d8771144cd23986f9f76c4ed729e69b0`）
> -  `branch`（例：`ref: refs/heads/feature3`）

​		Git 中的 `HEAD` 和每一个 `branch` 以及其他的引用，都是以文本文件的形式存储在本地仓库 `.git` 目录中，而 Git 在工作的时候，就是通过这些文本文件的内容来判断这些所谓的「引用」是指向谁的。

#### HEAD

​		`HEAD` 是引用中最特殊的一个：引用当前工作目录所对应的 `commit`。

- `HEAD`是仓库唯一的，表示当初在仓库中的位置。
  - 当前 `commit` 在哪里，`HEAD` 就在哪里
  - 这是一个永远自动指向当前 `commit` 的引用
  - 所以永远可以用 `HEAD` 来操作当前 `commit`。
- `HEAD` 除了可以指向 `commit`，还可以指向一个 `branch`
  -  `branch` 也是一个引用，指向一个`commit`，因此也是间接地指向某个 `commit`
- 每次 `commit` ，`HEAD` 指向最新的 `commit`
  - 工作目录自动与最新的 `commit` 对应
  -  `branch` 也会跟随`HEAD`的指向
-  `checkout`、`reset` 手动改变当前 `commit` 的时候，`HEAD` 也会同步更改
- 远程仓库的 `HEAD` 永远指向默认`master`分支，并会随着默认分支的移动而移动的。
  - `HEAD`只是一个仓库的当前位置游标，无论远程仓库还是本地仓库 
  - 所以，分支`push`时，远程仓库的 `HEAD` 不会像本地仓库的`HEAD`一样在分支之间切换。
  - 所以，`push` 的时候只会上传当前的 `branch` 的指向，并不会把本地的 `HEAD` 的指向也一起上传到远程仓库。



> [Git HEAD detached from XXX (git HEAD 游离) 解决办法 - 程序员大本营 (pianshen.com)](https://www.pianshen.com/article/54381757523/)

#### 分支  branch

​		`branch`是一个引用，是对一组提交记录路径的引用。

​		`branch`引用名类似于一个有向图上的指针，是从起点到当前。当``branch``名在不同提交记录之间移动时，就像数组头指针的移动。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779f983c81e7~tplv-t2oaga2asx-watermark.awebp)

- ``branch``， 从它引用的`commit`开始，向后包含所有的`commit`
  - `branch` 可以认为是从初始 `commit` 到 `branch` 所指向的 `commit` 之间的所有 `commit`s 的一个「串」
- 与其他`branch`共有的`commit`是整个仓库中所共同认可的基石
- `branch` 包含了从初始 `commit` 到它的所有路径，而不是一条路径。并且，这些路径之间也是彼此平等的。
  - 保留路径的原因是为了版本的回溯
    - 比如可以回到合并之前，重新选择，组织内容。
- `HEAD`可以直接指向`commit`，也可以通过引用某个`branch`间接引用到某个`commit`
  - `HEAD`在该分支上的移动操作，`branch`会跟随移动

> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779fa5e6970d~tplv-t2oaga2asx-watermark.awebp)
>
> `master` 的本质是一个指向 `3` 的引用，但你也可以把 `master` 理解为是 `1` `2` `3` 三个 `commit` 的「串」，它的起点是 `1`，终点是 `3`。

> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fe3354a1d3cd26~tplv-t2oaga2asx-watermark.awebp)
>
> master` 在合并了 `branch1` 之后，从初始 `commit` 到 `master` 有了两条路径： `1` `2` `3` `4` `7` 和 `1` `2` `5` `6` `7` 。而且，这两条路径是平等的，`1` `2` `3` `4` `7` 这条路径并不会因为它是「原生路径」而拥有任何的特别之处。

> ​	本地的新提交会使得本地的`HEAD`和`branch`引用更新，从而与关联仓库的引用指向不同的提交记录：
>
> ```shell
> commit 9b6d2c9488110e0ba8834c6bf202f9be8180003a (HEAD -> master)
> Author: mass <mazengrong12211@163.com>
> Date:   Wed Nov 3 21:25:37 2021 +0800
> 
>    new
> 
> commit 7c71d3b861454ecd715551d84e751446532f4b55 (origin/master, origin/HEAD)
> Merge: 1ea72f9 707ae91
> Author: mass <mazengrong12211@163.com>
> Date:   Tue Nov 2 16:37:13 2021 +0800
> ```
>
> `HEAD->master` 说明`HEAD`引用`master`，并一同引用最新的提交记录。
>
> `origin/master, origin/HEAD`， 表示关联仓库的分支依旧停留在原先的位置。



##### master分支

​		Git仓库默认拥有`master`分支，即主分支。

- 新建的仓库没有任何`commit`，但是当创建第一个`commit`时，`master会指向它`，并且`HEAD`会指向`master`
  - 这和所有的`branch`行为一样
- 从关联仓库`git clone`时，会从关联仓库下载`.git`到工作目录中，还会签出`master`。

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779f5c66ac9e~tplv-t2oaga2asx-watermark.awebp)

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fd779f5c191a3f~tplv-t2oaga2asx-watermark.awebp)



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
