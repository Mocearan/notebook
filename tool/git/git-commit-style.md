## git 提交规范

​	规范Git提交消息的格式，使用非常精确的规则，可以使提交历史更容易阅读。

​	每个提交消息由标题、正文和页脚组成：

```shell
<header>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>

# 标题行：50个字符以内，描述主要变更内容
#
# 主体内容：更详细的说明文本，建议72个字符以内。 需要描述的信息包括:
# * 为什么这个变更是必须的? 它可能是用来修复一个bug，增加一个feature，提升性能、可靠性、稳定性等等
# * 他如何解决这个问题? 具体描述解决问题的步骤
# * 是否存在副作用、风险? 
#
# 尾部：如果需要的化可以添加一个链接到issue地址或者其它文档，或者关闭某个issue。
```

- `header`是强制要求的，并且必须符合如下规则：

  ```shell
  <type>(<scope>): <short summary>
    │       │             │	# 简短的总结，不用大写，没有句号
    │       │             └─⫸ Summary in present tense. Not capitalized. No period at the end.
    │       │					
    │       └─⫸ Commit Scope: # 作用域应该是受影响的包的名称(由读取提交消息生成的变更日志的人感知到)。
    │
    └─⫸ Commit Type: build|ci|docs|feat|fix|perf|refactor|test
  ```

  - ##### Type

    - **build**: Changes that affect the build system or external dependencies (example scopes: gulp, broccoli, npm)
    - **ci**: Changes to our CI configuration files and scripts (examples: CircleCi, SauceLabs)
    - **docs**: 仅仅修改了文档，比如 README, CHANGELOG, CONTRIBUTE等等
    - **style**: 仅仅修改了空格、格式缩进、逗号等等，不改变代码逻辑
    - **feat**: A new feature
    - **DEPRECATED**
    - **fix**: A bug fix
    - **perf**: 优化相关，比如提升性能、体验
    - **refactor**:代码重构，没有加新功能或者修复 bug
    - **test**: 测试用例，包括单元测试、集成测试等
    - **chore**: 改变构建流程、或者增加依赖库、工具等
    - **revert**: 回滚到上一个版本

  - `summary`

    ​	使用祈使句，现在时:"change"不要"changed"也不要"changes"不要首字母大写，末尾不要有点

- `body`  除了“docs”类型的提交外，所有提交的主体都是强制性的。当正文存在时，它必须至少有20个字符长

  ​	就像在总结中一样，使用祈使句的现在时:"fix"而不是"fixed"也不是"fixes"。

  ​	解释提交消息主体中更改的动机。这个提交消息应该解释为什么要进行更改。您可以将以前的行为与新行为进行比较，以说明更改的影响。

- `footer`

  ​	页脚可以包含有关突破性更改和弃用的信息，也是引用GitHub问题，Jira票据和其他pr的地方，此提交关闭或相关。例如:

  ```shell
  BREAKING CHANGE: <breaking change summary>
  <BLANK LINE>
  <breaking change description + migration instructions>
  <BLANK LINE>
  <BLANK LINE>
  Fixes #<issue number>
  
  
  DEPRECATED: <what is deprecated>
  <BLANK LINE>
  <deprecation description + recommended update path>
  <BLANK LINE>
  <BLANK LINE>
  Closes #<pr number>
  
  # Breaking Change section should start with the phrase "BREAKING CHANGE: " followed by a summary of the breaking change, a blank line, and a detailed description of the breaking change that also includes migration instructions.
  
  # Similarly, a Deprecation section should start with "DEPRECATED: " followed by a short description of what is deprecated, a blank line, and a detailed description of the deprecation that also mentions the recommended update path.
  ```



- rever commit

  ​	如果提交恢复了以前的提交，它应该以revert:开头，后跟被恢复的提交的标题。

  ​	提交消息体的内容应该包含:

  ​	要回滚的提交的SHA信息，格式如下:此回滚commit <SHA>，清楚地描述了回滚提交消息的原因。



### Git分支与版本发布规范

- 基本原则：master为保护分支，不直接在master上进行代码修改和提交。

- 开发日常需求或者项目时，从master分支上checkout一个feature分支进行开发或者bugfix分支进行bug修复，功能测试完毕并且项目发布上线后，

  ```
  将feature分支合并到主干master，并且打Tag发布，最后删除开发分支
  ```

  。分支命名规范：

  - 分支版本命名规则：分支类型 _ 分支发布时间 _ 分支功能。比如：feature_20170401_fairy_flower
  - 分支类型包括：feature、 bugfix、refactor三种类型，即新功能开发、bug修复和代码重构
  - 时间使用年月日进行命名，不足2位补0
  - 分支功能命名使用snake case命名法，即下划线命名。

- Tag包括3位版本，前缀使用v。比如v1.2.31。Tag命名规范：

  - 新功能开发使用第2位版本号，bug修复使用第3位版本号
  - 核心基础库或者Node中间价可以在大版本发布请使用灰度版本号，在版本后面加上后缀，用中划线分隔。alpha或者belta后面加上次数，即第几次alpha：
    - v2.0.0-alpha.1
    - v2.0.0-belta.1

- 版本正式发布前需要生成changelog文档，然后再发布上线。

### 如何接入？

接入参考[git-commit-style-guide](https://github.com/feflow/git-commit-style-guide)项目。具体步骤如下：

- 第一步：在工程跟目录下的package.json文件加入如下代码所示的scripts和dependencies内容，版本号为3位版本号。

```text
  {
    "name": "application-name",
    "version": "0.1.0",
    "scripts": {
      "commitmsg": "validate-commit-msg",
      "commit": "git-cz ",
      "changelog": "conventional-changelog -p angular -i CHANGELOG.md -s -r 0"
    },
    "devDependencies": {
      "commitizen": "^2.3.0",
      "validate-commit-msg": "^2.11.1",
      "conventional-changelog-cli": "^1.2.0",
      "husky": "^0.13.1"
    }
  }
```

- 第二步：在工程根目录新建.vcmrc文件，并且文件内容为

```text
{
  "helpMessage": "\nPlease fix your commit message (and consider using https://www.npmjs.com/package/commitizen)\n",
  "types": [
    "feat",
    "fix",
    "docs",
    "style",
    "refactor",
    "perf",
    "test",
    "chore",
    "revert"
  ],
  "warnOnFail": false,
  "autoFix": false
}
```

### 接入后的Git commit操作流程

- 第一步：创建一个feature分支或者bugfix分支

```text
  $ git checkout -b feature_infinite_load    # 切换到一个feature分支或者bug fix分支
```

- 第二步：将代码提交到本地Git仓库，并填写符合要求的Commit message格式

```text
  $ git add .
  $ git commit                               # 此处不要加任何参数，比如-m
```

如下图所示：

![img](http://images2015.cnblogs.com/blog/1030776/201703/1030776-20170304153136563-2126734467.png)

- 第三步：将代码同步到远程Git仓库

```text
  $ git push origin feature_infinite_load    # 将修改发布到远程仓库
```

- 第四步：自动生成changelog，并打Tag发布

```text
  $ tnpm run changelog                    # 使用npm script中的changlog命令直接从git元数据生成日志。
  $ git tag v0.1.0
  $ git push origin v0.1.0
```

