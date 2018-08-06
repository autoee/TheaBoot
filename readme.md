#TheaBoot


# 起源
	
本项目起源于我对曾经对pix飞控源代码的一次修改，作为一个超轻量化bootloader程序，原版的功能无疑已经将:程序加载，信息读取等必要功能实现了，但对于产品来说其仍然缺少一些功能，如密码保护，SN序列号信息等。故我根据实际工程需要改写了这个版本的bootloader，为其起名TheaBoot，一次来纪念我的前女友Thea，虽然终归殊途，但和她在一起的日子真的很开心。

# 功能规划表

## 通信接口：

- [x] USART 串口
- [x] USB CDCACM 虚拟串口

## 基础功能:

- [x] 条件触发Bootloader中断引导APP
- [x] 读取 stm32 UDID 序列号
- [x] 读取 设备存留APP区域容量
- [x] 读取Bootloader版本
- [x] 读取设备型号
- [x] 读取设备序列号
- [x] 读取设备版本
- [x] 读取设备FLash结构(参考自STM32 DFU DEMO)
- [x] 读取设备备注描述信息
- [x] 擦除APP区域数据
- [x] 烧写固件
- [x] 固件CRC校验
- [x] 引导并启动APP
- [ ] Bootloader核心功能密码保护