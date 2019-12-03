# Beyond Compare 4.2.10(64位)注册机

#### 介绍（如果对你有用，请给个star！本程序仅用于学习交流）
master分支支持破解Beyond Compare 4.2.10(64位)(当前最新版)和Beyond Compare 4.2.9(64位), 但破解速度比较慢,
crack4.2.9分支只支持破解Beyond Compare 4.2.9(64位)，但破解速度快，使用方法：把BCompareCrack.exe拷贝到Beyond Compare安装目录，点击运行就可以

以下Beyond Compare 4.2.9反汇编分析：

call bcompare.BFB730是计算exe是否被修改过和是否超出试用期限
![输入图片说明](https://images.gitee.com/uploads/images/2019/0804/010609_a608f578_1650820.png "权限判断函数.png")

经过第一个call后，在执行到call bcompare.853E00前，在地址ecx+0x600储存权限校验结果数据，后面很多判断都是基于这里的校验结果(bcompare.853E00直接赋值给eax就是用作后面的判断，应该弹出哪个提示框以及是否可以继续使用)
这里我们直接改写bcompare.853E00，给ecx+0x600赋值0x500(为什么是0x500?, 因为经过多次比较值为0x500可以使用软件，0x0E02不可使用)
![输入图片说明](https://images.gitee.com/uploads/images/2019/0807/110921_5d62b09d_1650820.png "QQ图片20190807110600.png")

本程序实际上就是修改PE以下红框位置的数据(只修改这些数据软件还会弹出一个提示框，但不影响使用)，
![输入图片说明](https://images.gitee.com/uploads/images/2019/0804/010641_06c07a05_1650820.png "exe文件修改位置.png")

#### 软件架构
软件架构说明


#### 安装教程

不需要安装，只需把BCompareCrack.exe拷贝到Beyond Compare安装目录，点击运行就可以

#### 使用说明

将\Release\BCompareCrack.exe拷贝到Beyond Compare 4目录下然后运行即可，也可以自已编译




