# Verdure.UniversalController
讲述一个万能控制器的复刻过程，以及代码的整理。

## 项目介绍

本项目是讲述复刻一个B站up主黑人黑科技制作的万能的遥控器的过程，主要包括硬件的焊接，以及实物展示，和代码的整理，由于作者直接打包的代码，所以本人经过整理将代码整理到了GitHub，如果有侵权的问题，那可以通知本人，本人可以及时的删掉。


本人B站首页:
https://space.bilibili.com/25228512


原作者的嘉立创开源广场的地址：
https://oshwhub.com/bukaiyuan/ESP32-hang-mu-yao-kong-qi

原作者的B站首页地址：
https://space.bilibili.com/14368720

2023/11/1 更新日志修复右摇杆左右不工作 由于作者源代码引脚配置错误所以读取不到数值修复如图所示
![{8A2A25EC-EAC2-4beb-A618-6B27955EA742}](/Images/code.png)



成品效果图如下：
![pic](/Images/pic.jpg)

![content](https://github.com/maker-community/Verdure.UniversalController/assets/24240675/55d67c78-786c-4e57-bb5e-eb6434aa8670)

主控板的资料地址：
https://github.com/Xinyuan-LilyGO/T-Display-S3-AMOLED

![pic](/Images/T-Display-S3-AMOLED.jpg)


backup目录为作者采用VS加Arduino插件构建的项目，本人将代码重新整理了，可以直接用Arduino IDE编译的版本放到了src目录。

并且采用英文命名。

src=> ControllerSystem目录的代码为手柄正式的代码，包含系统框架和按键测试，以及遥控器发射端的代码，已经编译烧录测试。

src=> ReceiverDemo目录的代码为通讯控制舵机测试代码，用Arduino Nano进行通讯测试。

![pic](/Images/pic1.jpg)


src=> libraries 包含项目用到的依赖库 重要通知，由于这个23017在库里下载不到，所以我把源文件上传了，大家其他的库都可以下载的到。


接收器PCB板子地址
https://oshwhub.com/greenshade/arduino_nano_receive

接收器板子实物图
![实物图](/Images/receive.jpeg)





