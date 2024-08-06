

# 分支模型

---



## 基本工作流

Git 的基本工作流程如下：

1. 在开始编写代码之前，首先需要创建一个 Git 仓库（repository），用于存储代码和版本历史记录。
2. 在编写代码时，可以通过 `git add` 命令将更改的文件添加到 Git 的暂存区（staging area）中。
3. 通过 `git commit` 命令将暂存区中的更改提交到 Git 仓库中，并生成一个新的版本号（commit hash）。
4. 如果需要撤销某个提交，可以使用 `git revert` 命令来创建一个新的提交，该提交将会抵消先前的提交效果。
5. 如果需要合并不同分支的代码，可以使用 `git merge` 命令进行合并。
6. 如果需要查看代码的历史提交记录，可以使用 `git log` 命令来获取详细信息。
7. 如果需要将代码推送到远程仓库，可以使用 `git push` 命令将本地代码推送到远程仓库。
8. 如果需要从远程仓库中获取代码，可以使用 `git pull` 命令将远程代码拉取到本地。

![img](https://pic3.zhimg.com/80/v2-73153e41672062a0ec7f3bd095c43b06_720w.webp)



## 协作

Git 是一个优秀的多人协作工具，以下是 Git 多人协作的一些最佳实践：

1. 使用分支：使用分支可以帮助团队成员在不影响主分支的情况下进行开发和测试，避免代码冲突和错误。建议采用主分支、开发分支、特性分支、发布分支、热修复分支等分支管理策略。
2. 提交规范：每次提交代码时应该附加有意义的提交信息，描述本次提交的更改内容和目的。建议采用语义化版本号和提交信息模板等规范，以便更好地记录和追踪代码变更历史。
3. 定期合并：团队成员应该定期将自己的分支合并回主分支或者开发分支。这可以避免较大的代码冲突和错误，并且保持代码库的整洁和可维护性。
4. 代码审查：通过代码审查可以确保代码的质量和一致性，并且可以识别和纠正潜在的问题和错误。建议采用 pull request 和 code review 等工具和流程，以便团队成员对彼此的代码进行审查和反馈。
5. 团队协作：团队成员之间应该保持及时和有效的沟通，共享技术和经验，并尽可能避免个人行为和偏见对项目和团队产生不良影响。

通过采用上述最佳实践，可以帮助团队高效协作、保证代码质量和稳定性，并提高团队的生产力和创造力。



## 分支管理策略

​		在 Git 中，常见的分支管理策略包括以下几个方面：

1. 主分支：主分支通常是最稳定的分支，用于发布生产版本。在 Git 中，主分支通常是 master 分支或者 main 分支。
2. 开发分支：开发分支通常从主分支派生而来，在其上进行新功能或修复错误的开发。在 Git 中，通常使用 develop 分支作为开发分支。
3. 特性分支：特性分支是为了开发单独的功能而创建的分支。这些分支通常从开发分支派生而来，并在实现目标后被合并回开发分支。在 Git 中，通常使用 feature/ 分支命名约定来表示特性分支。
4. 发布分支：发布分支是用于准备发布版本的分支，通常从主分支派生而来。这些分支应该包含与发布相关的所有更改，并且应该经过全面测试和审核后再合并回主分支。在 Git 中，通常使用 release/ 分支命名约定来表示发布分支。
5. 热修复分支：热修复分支通常用于快速修复紧急问题，例如安全漏洞或崩溃。这些分支通常从主分支派生而来，并且只包含必要的更改。在 Git 中，通常使用 hotfix/ 分支命名约定来表示热修复分支。

## **bug分支**

在 Git 中，通常使用 bug 分支用于修复代码中的错误或缺陷。当发现 bug 时，可以从当前开发分支（如 develop 分支）创建一个 bug 分支，在该分支上进行错误修复。修复完成后，可以将更改提交到 bug 分支，并将其合并回开发分支和主分支。

以下是一个典型的使用 Git bug 分支的流程：

1. 从当前开发分支（如 develop 分支）创建一个新的 bug 分支：

```text
git checkout -b bug/fix-xxx
```

1. 在 bug 分支上进行错误修复，包括必要的测试和代码审查。
2. 提交更改并推送到远程仓库：

```text
git add .
git commit -m "Fix xxx bug"
git push origin bug/fix-xxx
```

1. 将 bug 分支合并回开发分支（如 develop 分支）：

```text
git checkout develop
git merge --no-ff bug/fix-xxx
```

1. 测试修复是否正确，如果一切正常，则将开发分支合并回主分支（如 master 分支）：

```text
git checkout master
git merge --no-ff develop
```

​		通过使用 Git bug 分支，可以帮助团队更好地管理和修复代码中的错误和缺陷，同时保持代码库的稳定性和可靠性。





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

- 代码分享
- 多任务

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



