# github

---

​		GitHub 是一个用于版本控制和协作的代码托管平台。 它允许您和其他人随时随地协同处理项目。



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