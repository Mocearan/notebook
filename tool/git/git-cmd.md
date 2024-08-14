# git-cmd

---

[Git 分支操作全解析：创建、切换、合并、删除及冲突解决 (qq.com)](https://mp.weixin.qq.com/s/uIA_APfF8oBjAOPM1lgeLQ)



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

- 配置项结构

```ini
[http]
    postBuffer = 2M
[user]
    name = runoob
    email = test@runoob.com
```

#### 查看配置

- 查看配置列表 `git config --list`

  > ​		默认查看所有配置项
  >
  > ​		重复的变量名，那就说明它们来自不同的配置文件（比如 /etc/gitconfig 和 ~/.gitconfig），不过最终 Git 实际采用的是最后一个。
  >
  > ​		可以直接查看对应的文件

- 查看配置项

  `git config user.name`

- 编辑配置

  `git config --global -e`

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



## repo cmd

### 创建本地仓库 init

​		执行了`git init`初始化后，会在当前目录下生成`.git`文件夹

- 将当前目录初始化为本地Git仓库

  `git init`

- 新建一个仓库目录

  `git init [rname]`

### 关联远程仓库 remote

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

```ruby
  # 下载远程仓库的所有变动
  $ git fetch [remote]
  # 显示所有远程仓库
  $ git remote -v
  # 显示某个远程仓库的信息
  $ git remote show [remote]
  # 增加一个新的远程仓库，并命名
  $ git remote add [shortname] [url]
  # 取回远程仓库的变化，并与本地分支合并
  $ git pull [remote] [branch]
  # 上传本地指定分支到远程仓库
  $ git push [remote] [branch]
  # 强行推送当前分支到远程仓库，即使有冲突
  $ git push [remote] --force
  # 推送所有分支到远程仓库
  $ git push [remote] --all
```

### 克隆仓库 clone

- 克隆一个仓库到当前目录

  `git clone [path/url] [dir]`

  > - path 是一个本地的即存仓库路径。
  > - url 是一个远端地址，可以是 `username@host:path`
  > - dir 可以指定本地的目录，并可以自定义项目名
  >
  > `git clone git://github.com/schacon/grit.git ../mygrit`
  >
  > > 克隆出的仓库是与源仓库关联的。



## branch cmd

### 分支 branch

```shell
# 查看分支
git branch

# 查看所有分支
git branch -a

# 创建分支
git branch [bname]

# 删除分支 
git branch -d [bname]
```

- `HEAD` 指向的 `branch` 不能删除

  - 如果要删除 `HEAD` 指向的 `branch`，需要先用 `checkout` 把 `HEAD` 指向其他地方。

- 删除 `branch` 是删掉引用，不删除任何的 `commit``

  - `branch`只是一个引用。

  - 如果`branch`的删除会造成`commit`孤悬，那么在一定时间后，`commit`会被 Git 的回收机制删除掉。

    > 孤悬：
    >
    > ​		`commit` 不在任何一个 `branch` 的「路径」上，或者换句话说，如果没有任何一个 `branch` 路径可以回溯到这条 `commit`

- 未被合并到 `master` 过的 `branch` 在删除时会失败

  - 防止误删「未完成」的 `branch`
  - 如果确认是要删除这个 `branch` ，可以把 `-d` 改成 `-D`，小写换成大写，就能删除了
    - 例如某个未完成的功能被团队确认永久毙掉了，不再做了





### 切换 checkout

​		切换分支实际上是将给定分支引用的提交记录签出工作区，并将``HEAD``引用到该分支引用上。

```shell
# 切换分支
git checkout [bname]

# 如果`bname`分支不存在，则创建并切换到该分支
git checkout -b [bname]
```



### 获取 fetch

​		用于从关联仓库获取指定的`branch`到当前`HEAD`的工作区。

```shell
git fetch [host_alias] [bname_list]
```

- 缺省形式`git fetch`为拉取远程仓库所有分支上比本地更新的`commit`
- 缺省形式`git fetch`为拉取远程仓库所有分支上比本地更新的`commit`





### 合并 merge

​		合并其他分支上的提交记录到当前分支上。

```shell
git merge [alias]/[bname]
```

- `merge` 操作自动地填写简要的提交信息。在提交信息修改完成后退出，`merge` 就算完成了。
- `bname`引用串上的`commit`会按照时间排序，插入到`HEAD`所引用分支上的`commit`串里。形成一个按时间排序的新`commit`串。
- 合并只是将该分支的提交记录同步到当前分支上，但该分支依然存在。可以选择在合并后，删除该分支。

> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2aad5a0279~tplv-t2oaga2asx-watermark.awebp)
>
> `git merge branch1`，会把 `5` 和 `6` 这两个 `commit` 的内容一并应用到 `4` 上，然后生成一个新的提交，并跳转到提交信息填写的界面

#### 同枝合并

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
  >
  > ​		`git merge --no-ff`会强制创建一个合并提交，即使在合并时可以通过快进（fast-forward）方式完成。
  >
  > 1. **保持历史记录清晰**：通过创建合并提交，可以明确标识出分支合并的地方，帮助维护项目的历史记录。
  > 2. **更好地跟踪特性**：合并提交可以承载更多的信息，例如合并的上下文，方便将来回溯和理解代码历史。
  > 3. **避免历史冗长**：在更改数量多且有复杂性的分支中，快进合并可能会导致历史非常线性和难以追踪的状态



#### 合并多个分支

​		`git merge origin/master hotfix-2275 hotfix-2276 hotfix-2290`

#### 冲突 conflict

​		如果在合并时，两个分支上的`commit`之间存在对某一内容的竞争性修改。那么需要人工参与对内容的合并。合并完成之后手动提交来产生`commit`。

​		Git会报告发生冲突的文件，并在文件中发生冲突的位置，使用符号标注分支之间导致冲突的不同内容。

- 如果冲突，需要手工合并。合并完成后，提交暂存并重新提交。

  - 也就是合并冲突后，会将冲突内容添加到当前工作区中

  > 1. 运行 `git status` 命令查看哪些文件包含冲突。
  > 2. 编辑有冲突的文件，手动解决文件中的冲突。
  >    1. 编辑有冲突的文件，删除特殊符号，决定要使用的内容
  >       特殊符号：`<<<<<<< HEAD 当前分支的代码 ======= 合并过来的代码 >>>>>>> hot-fix`
  > 3. 对编辑后的文件进行 `git add`，标记为已解决冲突的文件。
  > 4. 使用 `git commit` 提交更改，Git 会自动生成一个合并提交，其中包含各自分支中的更改。
  >
  > 在解决冲突前，最好先备份当前的代码状态，以免不小心破坏代码库。另外，在处理冲突之前，可以通过运行 `git diff` 命令来查看冲突的源代码，以便更好地理解要解决的问题。
  >
  > ​		![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fddc2af5b06ef6~tplv-t2oaga2asx-watermark.awebp)
  >
  > ​		手工修改时，应该将冲突标注内容全部删除后，做内容上的修改。保存退出之后重新`git add`并``commit``。此时的`commit`依然会带有自动合并时的提交信息。
  >
  > ​		也可以使用更方便的merge工具。可以在网上搜索。

- 如果发生冲突时，可以放弃合并

  - 将添加到`HEAD`工作区上的其他`branch`上的内容放弃
  - `git merge --abort`即可



### 拉取 pull

​		从关联仓库拉取最新提交记录，并合并本地的版本

```shell
git pull <remote_name> <remote_bname>:<local_bname>

#git pull origin master:brantest
```

- `git pull` 其实就是 `git fetch`和 `git merge FETCH_HEAD` 两步的简写
- `remote_name`通常是 `origin`，这是 Git 默认的远程仓库名称。
- `remote_bname`通常是 `main` 或 `master`，也可以是其他分支。
  - `<remote_bname>`:`<local_bname>`, 缺省默认都为`master`
    - `git pull` / `git pull origin`
  - 如果远程分支与当前分支合并，则冒号后面的部分可以省略
    - `git pull origin master`
- 使用`--rebase`来保持远程分支线性
  - `git pull --rebase`
    - `pull = fetch + merge`
    - `pull --rebase = fetch + rebase`
  - [直接使用git pull拉取代码，被同事狠狠地diss了！ (qq.com)](https://mp.weixin.qq.com/s/n1KbNaT46SwVPCBxpW31ow)
- 可能产生冲突
  - 当 `pull` 操作发现不仅远端仓库包含本地没有的 `commit`
  - 而且本地仓库也包含远端没有的 `commit` 时
  - 它就会把远端和本地的独有 `commit`进行合并，自动生成一个新的 `commit`。

>   ​		和手动的 `commit` 不同，这种 `commit` 会自动填入一个默认的提交信息，简单说明了这条 `commit` 的来由。你可以直接退出界面来使用这个自动填写的提交信息，也可以修改它来填入自己提交信息。

### 推送 push

​		上传本地的分支版本到关联仓库合并。

​		把当前`branch`引用的`commit`路径上的所有未上传的`commit`上传到关联仓库。

```shell
git push <remote_name> <remote_bname>:<local_bname>
```

- 如果本地分支名与远程分支名相同，则可以省略远程分支名

  - `git push <remote_name> <local_bname>`
    - `git push origin master`

- `git push`是针对分支的，多个分支需要分别推送或切换分支推送。

  ```shell
  git chekcout feature1
  git push origin feature1
  ```

- （Git 2.0起）如果`push`的是，从关联仓库`clone/pull`时没有的分支，就必须指明主机名和分支名。

  - 这个行为可以通过`git config push.default`来改变。

- `push`只会将分支的引用上传，而不会将本地的`HEAD`信息上传。

  - 所以远程仓库的`HEAD`不会被更新，只会永远引用`master`分支。

- push并不一定能够成功

  ​		Git 的`push` 其实是用本地仓库的 `commits `记录去覆盖并更新远端仓库`branch`上的 `commits `记录，如果在远端仓库含有本地没有的 `commits` 的时候，`push` （如果成功）将会导致远端的 commits 被擦掉。这种结果当然是不可行的，因此 Git 会在 `push` 的时候进行检查，如果出现这样的情况，push 就会失败。

  

  >   ```shell
  >   ! [rejected]        master -> master (fetch first)
  >   error: failed to push some refs to 'https://gitee.com/masstsing/gpra.git'
  >   hint: Updates were rejected because the remote contains work that you do
  >   hint: not have locally. This is usually caused by another repository pushing
  >   hint: to the same ref. You may want to first integrate the remote changes
  >   hint: (e.g., 'git pull ...') before pushing again.
  >   hint: See the 'Note about fast-forwards' in 'git push --help' for details.
  >   ```
  >
  >   需要先`pull`把关联仓库的内容取回本地，进行冲突合并后再推送。




>   ![本地仓库超前于远程仓库](https://gitee.com/masstsing/picgo-picserver/raw/master/20211106182413.webp)
>
>   >   上传5,6两个`commit`

```shell
# 将本地master分支推送到远程仓库的master分支
git push origin master

# 将当前分支推送到关联仓库的关联分支上
# 没有关联分支将失败
git push

# 推送分支到仓库并设置远程分支 
git push [-u] origin [local_branch]

# 覆盖远程的内容
# --force（或简写为 -f）
git push --force origin master

# 删除主机的分支可以使用 --delete 参数
git push origin --delete master
```



## commit cmd



### 提交 commit

​		将暂存区中缓存的所有工作内容作为一次记录，提交到仓库。

>   暂存区的目录树写到对象库中，分支会做相应的更新。即分支目录树就是提交时暂存区的目录树。

```shell
git commit {file_list} -ma"message"
```

- `HEAD`，将指向这次提交记录
- `{file_list}`，提交暂存区的指定文件到提交记录，文件列表缺省默认提交暂存区中所有文件

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

​		查看历史提交记录。[Git - git-log Documentation (git-scm.com)](http://git-scm.com/docs/git-log)

```shell
git log	
```

-   `--oneline`， 单行输出

-   `--graph`， 查看历史分支结构图

-   `--reverse`，逆时序查看

-   `--author=NAME`，查看指定作者的提交记录

-   `--decorate`, 查看带标签的提交记录

-   `--since`

-   `--before`

-   `--until`

-   `--after`

-   `--patch / -p` 查看每个`commit`的变动细节，包括内容。可以用于代码`review`

-   `--stat`， 查看简化的变动内容，每个文件的变动行数。

```shell
git log 
	--oneline 
	--before={3.weeks.ago} 
	--after={2010-04-18} 
	--no-merges
```



#### blame

​		`git blame [fname]`， 查看指定文件历史修改记录。查出某个文件的每一行内容到底是由哪位大神所写。

```shell
git blame 文件名
```

如果只查文件中某一部分由谁所写：
	`git blame 文件名 | grep "查找词"`
或者：
	`git blame 文件名 -L a,b`

- -L 参数表示后面接的是行号(Line)， a,b代表查询文件的第a行到第b行之间的文件内容情况。
- a, 则代表从第a行到文件结尾
- ,b则代表从文件开头到第b行。

```shell
git blame webpack.mix.js | grep "Mix Asset"
git blame webpack.mix.js -L 5,5
```

​		https://git-scm.com/docs/git-blame

#### diff

​		`git diff --staged`， 查看暂存区和上一条`commit`的差异。

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
  >  new
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

​		`reflog` 是 "reference log" 的缩写，使用它可以查看 `Git `仓库中的引用的移动记录。

> ​		`reflog`是一个本地结构，它记录了HEAD和分支引用在过去指向的位置。`reflog`信息没法与其他任何人共享，每个人都是自己特有的reflog。重要的一点是，它不是永久保存的，有一个可配置的过期时间，reflog中过期的信息会被自动删除。

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



### 精选 cherry-pick

​		 `cherry-pick` cherry-pick 是一种将某个分支上的一个或多个提交应用到另一个分支的操作，而无需将整个分支合并过来。

> 创建一个新的提交，这个新的提交与原提交的 SHA 值不同，但是提交的内容是相同的。

```shell
git cherry-pick <commit-id>
```

​		使用 cherry-pick 操作，我们可以复制指定的提交，然后将其应用到当前分支上，这个提交就成为了当前分支上的一个新的提交。cherry-pick 操作可以方便地将某个分支上的某个功能或修复应用到另一个分支上，而无需将整个分支合并过来。

​		使用 cherry-pick 操作时，可能会出现冲突，这时候需要手动解决冲突，并提交一个新的提交来解决冲突。因此，在使用 cherry-pick 操作之前，我们需要仔细考虑哪些提交需要复制，以及是否会产生冲突等问题。



### 签出 checkout

​		将`commit`记录的文件更改状态应用到当前的工作区。

> 可以将签出简单的理解为，拉到台面上来。

- 把某个 `commit` 作为当前 `commit`，把 `HEAD` 移动过去
- 并把工作目录的文件内容替换成这个 `commit` 所记录的内容。

```shell
git checkout [commit/bname/HEAD]

# cases
git checkout HEAD^^
git checkout master~5
git checkout 78a4bc
git checkout 78a4bc^
```

​		`checkout` 和 `reset` 都可以切换 `HEAD` 的位置

- `reset` 在移动 `HEAD` 时会带着它所指向的 `branch` 一起移动
-  `checkout` 不会带着它所指向的 `branch` 一起移动
  - `checkout` 有一个专门用来只让 `HEAD` 和 `branch` 脱离而不移动 `HEAD` 的用法：
    	`git checkout --detach`

![git checkout --detach](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/30/1600acce7b90b009~tplv-t2oaga2asx-watermark.awebp)

```shell
# 签出分支引用的commit到工作区
git checkout [bname]

# 签出分支到工作区，不存在则创建
git checkout -b [bname]

# 基于远程 master 分支签出一个新分支
git checkout -b new_feature origin/master

# 以仓库中HEAD commit的文件内容替换当前工作区中对应文件的修改，即放弃未暂存更改
# `.`，当前提交记录全部
# 已经添加到暂存区的文件，或者新创建未追踪文件不会受到影响。
git checkout -- [fname]

# 以仓库中HEAD commit的文件内容替换当前工作区中对应文件的修改，即从仓库中恢复文件
# 可以用于丢弃当前工作区、暂存区中对应文件的改动
git checkout HEAD [fname]


# 丢弃本地工作区中所有改动和提交记录
# 获取关联仓库的最新版本，并改动主分支指向
git fetch origin
git reset --hard origin/master

# 从远程分支创建并签出分支
git checkout -b [bname] origin/[bname]
```

> ​		`git clone + 远程仓库地址`将项目下载下来之后，倘若远程仓库有多个分支，我们会发现，使用`git branch`查看本地分支时，只有一个master分支
>
> ​		使用`git branch -a`查看远程分支,那些以`remotes`开头且为红色的都是远程分支，或者理解为**这些分支是隐藏的.**
>
> ​		此命令将远程分支与本地分支联系起来，或者使用`-t`参数在本地建立一个和远程分支名字一样的分支
>
> - `git checkout -b feature origin/feature`
> - `git checkout -t origin/feature`



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

    ```ruby
    # 新建一个tag在当前commit
    $ git tag [tag]
    # 新建一个tag在指定commit
    $ git tag [tag] [commit]
    # 查看tag信息
    $ git show [tag]
    # 提交指定tag
    $ git push [remote] [tag]
    # 提交所有tag
    $ git push [remote] --tags
    # 新建一个分支，指向某个tag
    $ git checkout -b [branch] [tag]
    ```

### 重置 reset

​		重置（移动）HEAD，及它指向的分支。

````shell
git reset --[soft/mixed/hard/keep] [HEAD]/[master]/[commit](^/~N)
````

- 模式

  - `hard`：工作区和暂存区都重置到与指定提交记录一致
  - `mixed`：重置暂存区，工作区内容保持不变。
    - 暂存区的目录树会被重写，被 `master `分支指向的目录树所替换，但是工作区不受影响。
  - `soft`：工作区和暂存区都重置，并把内容修改放进暂存区
  - `keep`：保持暂存区和工作区不变
  - 缺省默认为`mixed`

- `^/~N`

  - `^`代表父节点

    - 可叠加表示若干层

      - `^`个数为`0`时`HEAD`表示仍旧保持在当前提交记录

        > 那功能就成了 只操作工作区和暂存区。此时依据`hard/soft`而有不同

  - `~N`：指定回退父节点的层数，可以为`0`，等同于`HEAD`

- `[fname]` 回退指定文件版本

```shell
# HEAD不移动，重置暂存区
git reset [HEAD] 

# 撤消上次提交并返回到上一个提交，不会删除工作区最新的更改
git reset HEAD~1

# 撤消上次提交并返回到上一个提交，删除工作区最新的更改
git reset --hard HEAD~1

# 回退到上个版本, 工作区和暂存区也都随着commit迁移。	
git reset --hard HEAD^ 	

# 回退到指定版本
# 先使用`git log`查看历史记录，找到要回滚的某个commit id。
git reset --hard commit_id/bname~N/HEAD~N 
	
# 回退到关联仓库版本
git reset --hard origin/[bname] 

# 重置当前HEAD为指定commit，但保持暂存区和工作区不变
git reset --keep [commit]
```

​		![](https://gitee.com/masstsing/picgo-picserver/raw/master/15fe19c8a3235853~tplv-t2oaga2asx-watermark.awebp)



### 变基 rebase

​		将一个分支的整体迁移到另一个分支上。以`commit`的角度看，是重新设置分支的`commit`序列的基础点（父`commit`）。

> ​		`merge`之后`commit`历史就会出现分支叉再汇合的结构，如果不希望`commit`历史出现分支叉，可以用`rebase`代替`merge`

```shell
git rebase [<options>] [<upstream> [<branch>]] 
```

- 将指定`branch`所在的分支上的所有`upstream`串，重新提交到指定的`upstream`后
  - `upstream`: 目标分支（即你想要将当前分支的提交移至其之上；可以是分支名、commit ID等）。
  - `branch`: 需要进行rebase的分支（默认为当前分支，如果不指定）
  - `option`：
    - `--onto <newbase>`: 指定新的基底，对于复杂的rebase情况很有用
    - `-i`, `--interactive`: 交互式rebase，可以选择、重排或修改提交
    - `--continue`: 在解决冲突后继续正在进行的rebase操作
    - `--abort`: 取消当前的rebase操作，返回到rebase之前的状态
    - `--skip`: 跳过当前提交，继续rebase操作
    - `-p`, `--preserve-merges`: 保留合并提交（自Git 2.34起已经被弃用，推荐使用`--rebase-merges`）
    - `--rebase-merges`: 保留合并提交的结构
    - `-v`, `--verbose`: 在rebase过程中显示详细信息
- `git rebase`也可能造成冲突

#### merge vs. rebase

- `git merge branch1`

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fdea7b6646a1f3~tplv-t2oaga2asx-watermark.awebp)

- `git rebase master`

  > - ``git checkout branch1`
  > - `git rebase master`

![img](https://gitee.com/masstsing/picgo-picserver/raw/master/1600abd620a8e28c~tplv-t2oaga2asx-watermark.awebp)

> `git checkout master`
>
> `git merge branch1`
>
> > 移动master引用。
>
> ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/160149e054fe485c~tplv-t2oaga2asx-watermark.awebp)
>
> > ​		假设master上的记录都已经提交并推送到了关联仓库，那么如果在master上向分支`rebase`，因为`master`跟随变基引用到`rebase`后分支上的最新`commit`，导致原来`master`上自分叉点开始的提交都被剔除（没有引用指向了）。因为这些提交在关联仓库存在，从而导致无法`push`
>>
> > ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/16014b5a6919c0b7~tplv-t2oaga2asx-watermark.awebp)
>>
> > 为了避免这种情况发生，一般是在其他分支上向`master`主支`rebase`。
>> 
> > 其他的两个分支之间进行`rebase`就不必如此。

- `git rebase HEAD`

  `rebase`到当前所在分支，称为原地`rebase`。相当于空操作。4

  > `git rebase HEAD~2`
  >
  > ​		把`HEAD`引用的`commit`，`rebase`到`HEAD`前两个`commit`。实际上操作完成之后，没有变化。

  ![img](https://gitee.com/masstsing/picgo-picserver/raw/master/15fdf5fd00522381~tplv-t2oaga2asx-watermark.awebp)

#### `-i`

​		交互式`rebase`

```shell
git rebase -i 
git rebase --interavtive
```

- 在`rebase`前，可以指定要`rebase`的`commit`串中的每一个`commit`是否进一步修改
  - 可以据此来进行一次原地`rebase`，来修正此前提交中中的内容而不添加新的`commit`
  - `git commit --amend`



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
>  - 其他操作列在上图`Commands`中
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

#### `--onto`

​		`--onto`， 指定`rebase`时重新提交的起点。

- `rebase`时起点默认是自动判定的

  - 起点是当前`commit`和目标`commit`交叉点
  - 起点`commit`在`rebase`时不包含在内，从其子`commit`开始

- `--onto`可以指定`rebase`时目标`commit`、起点`commit`、终点`commit`

  > ```shell
  > git rebase --onto commit3 commit4 branch1
  > ```
  >
  > ![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe24400d7d73d0~tplv-t2oaga2asx-watermark.awebp)
  >
  > - commit3  指定目标
  > - commit4 指定起点（从子`commit`开始）
  > - `branch1`，终点commit

- 可以使用原地`rebase --onto`来撤销`commit`：

> ```shell
>  git rebase --onto HEAD~2 HEAD~1 branch1
> ```
>
> ​		以倒数第二个 `commit` 为起点（起点不包含在 `rebase` 序列里哟），`branch1` 为终点，`rebase` 到倒数第三个 `commit` 上。
>
> ![img](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2017/11/22/15fe243fce5804fd~tplv-t2oaga2asx-watermark.awebp)

​	



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
> >  				如果出错内容在 `master`：不要强制 `push`，而要用 `revert` 把写错的 `commit` 撤销。
> >			
> >  		​		如果出错内容在私有 `branch`：在本地把内容修正后，强制 `push` (`push -f`）一次就可以解决。
> >			
> >  		> ​		本地对已有的 `commit` 做了修改，这时你再 `push` 就会失败，因为中央仓库包含本地没有的 `commit`s。但这个和前面讲过的情况不同，这次的冲突不是因为同事 `push` 了新的提交，而是因为你刻意修改了一些内容，这个冲突是你预料到的，你本来就希望用本地的内容覆盖掉中央仓库的内容。那么这时就不要乖乖听话，按照提示去先 `pull` 一下再 `push` 了，而是要选择「强行」`push`：
> >  		>
> >  		> `git push origin branch1 -f`

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



### 定位 bisect

​		使用二分搜索算法来查找提交历史中的哪一次提交首次引入了错误。

​		整个定位过程几乎是机械的操作，只需要无脑地：验证、标记、验证，即可定位到bug所在的提交

> ​		你只需要告诉这个命令一个包含该bug的坏`commit ID`和一个引入该bug之前的好`commit ID`，这个命令会用二分法在这两个提交之间选择一个中间的`commit ID`，切换到那个`commit ID`的代码，然后询问你这是好的`commit ID`还是坏的`commit ID`，你告诉它是好还是坏，然后它会不断缩小范围，直到找到那次引入bug的凶手`commit ID`。
>
> ​		这样我们就只需要分析那一次提交的代码，就能快速定位和解决这个bug（具体定位的时间取决于该次提交的代码量和你的经验），所以我们**提交代码时一定要养成小批量提交的习惯，每次只提交一个小的独立功能**，这样出问题了，定位起来会非常快。

使用git bisect二分法定位问题的基本步骤：

1. `git bisect start `[最近的出错的commitid] [较远的正确的commitid]

   ```shell
   git bisect start 5d14c34b d577ce4
   or
   git bisect start HEAD d577ce4
   ```

   执行完启动`bisect`之后，马上就切到中间的一次提交啦，以下是打印结果：

   ```shell
   $ git bisect start 5d14c34b d577ce4
   Bisecting: 11 revisions left to test after this (roughly 4 steps)
   [1cfafaaa58e03850e0c9ddc4246ae40d18b03d71] fix: read-tip icon样式泄露 (#54)
   ```

2. 测试相应的功能

   - `git bisect good `标记正确
     - 标记万`good`，马上又通过二分法，切到了一次新的提交
   - 出现问题则 标记错误 `git bisect bad`
     - 标记下`bad`，再一次切到中间的提交

3. 不断地验证、标记、验证、标记…最终会提示我们那一次提交导致了这次的bug

   ```shell
   c0c4cc1a is the first bad commit
   ```

   

### 发布 archive

```ruby
# 生成一个可供发布的压缩包
$ git archive
```



## stage cmd

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
> (use "git restore --staged <file>..." to unstage)
> 	modified:   new.txt
> ```

```shell
vim new.txt # write
git status
```

> ```shell
> Changes to be committed:
> (use "git restore --staged <file>..." to unstage)
> 	modified:   new.txt
> 
> Changes not staged for commit:
> (use "git add <file>..." to update what will be committed)
> (use "git restore <file>..." to discard changes in working directory)
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

​		将文件同时从暂存区和工作区上删除。

```shell
git rm [--cached] [file_list]
```

```shell
# 文件从暂存区和工作区中删除
git rm [file_list]

# 放弃工作区修改，并从暂存区和工作区中删除
git rm -f [file_list]
# git rm -f *

# 从暂存区域移除，保留工作区
git rm --cached [file_list]
```

>   从暂存区移除后在实际上提交时，才会在仓库中移除了。

### 移动 mv

​		用于移动或重命名文件树。

`git mv [-f] {file_list} [dest_dir]`

-   重命名

    `git mv -f [file1] [file2]`

    >   如果目标文件名已存在，则-f
    >
    >   可以视为文件间的移动



### 状态 status

​		`git status `，查看当前工作状。

​		基于当前正在做的修改来展示状态，因此是当前工作区和暂存区的状态。

- `git status -s`, 获得简短的输出结果



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

