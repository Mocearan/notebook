


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



