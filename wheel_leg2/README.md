# wheel_leg

基于 `STM32H723VGTx` 的轮腿机器人主控工程，使用 `STM32CubeMX + Keil MDK + FreeRTOS` 搭建，包含 IMU 姿态解算、遥控输入解析、轮毂/关节电机 CAN 控制，以及五连杆腿机构的 LQR + VMC 控制。

## 项目概览

这个仓库对应的是一套轮腿平衡底盘控制程序，而不是纯双轮平衡车程序。代码里已经实现了以下主链路：

- `BMI088` IMU 采集与姿态解算
- 基于 `Mahony` 的四元数姿态更新
- `UART5 + DMA` 遥控器数据接收与解析
- `FDCAN1/FDCAN2` 控制 4 个腿部关节电机
- `FDCAN3` 控制 2 个轮毂电机
- 基于腿长变化的 `LQR` 增益拟合
- `VMC` 虚拟模型控制到关节力矩映射
- 预留 `MPC` 约束整形模块，用于后续限制轮毂/摆杆输出变化率
- `FreeRTOS` 多任务调度
- `WS2812` 状态灯指示

从当前代码状态看，工程目标是完成一台带五连杆腿机构的轮腿机器人底盘控制器，支持弱电模式、下蹲模式和正常站立/行走控制。

## 硬件与外设

### 主控

- MCU: `STM32H723VGTx`
- 开发方式: `STM32CubeMX` 生成底层初始化，`Keil MDK` 进行编译下载

### 传感器

- IMU: `BMI088`
- 接口: `SPI`
- 用途: 提供角速度、加速度、姿态角、机体运动加速度

### 执行器

- 轮毂电机: 2 路，使用 `FDCAN3`
- 腿部关节电机: 4 路，使用 `FDCAN1` 与 `FDCAN2`
- 代码中轮毂电机 ID:
  - 左轮: `0x202`
  - 右轮: `0x201`
- 代码中腿部电机 ID:
  - 左腿: `0x001`、`0x002`
  - 右腿: `0x003`、`0x004`

### 遥控与指示

- 遥控接收: `UART5 + DMA`
- 串口参数: `115200 8N1`
- 特殊配置: 使能了 `RX Pin Invert`，适配反相输入
- 状态灯: `WS2812`

## 软件架构

### 启动流程

上电后主程序大致按下面顺序执行：

1. 初始化 GPIO、DMA、UART5、SPI、FDCAN、TIM 等外设
2. 开启 `UART5 DMA` 接收遥控数据
3. 初始化 CAN 过滤器与总线
4. 点亮 `WS2812`
5. 初始化 `DWT` 和延时模块
6. 初始化 `BMI088`
7. 创建并启动 `FreeRTOS` 任务

入口位于：

- `Core/Src/main.c`
- `Core/Src/freertos.c`

### FreeRTOS 任务

当前主要启用了两个实时任务：

- `ins_task`
  - 文件: `application/imu_task.c`
  - 职责: 读取 BMI088、陀螺零偏估计、Mahony 姿态解算、计算机体加速度与累计 yaw
- `Chassis_task`
  - 文件: `application/Chassis_Control.c`
  - 职责: 底盘状态更新、模式切换、LQR/VMC 计算、电机控制输出

### 控制链路

控制主链路可以概括为：

`IMU/电机反馈 -> 状态估计 -> 腿部运动学 -> LQR -> VMC -> 轮毂/关节电机输出`

其中关键模块包括：

- `application/imu_task.c`
  - 读取 `BMI088`
  - 姿态四元数更新
  - 计算欧拉角 `yaw/pitch/roll`
  - 计算机体系和导航系下的运动加速度

- `application/Chassis_Control.c`
  - 读取遥控器输入
  - 更新左右腿几何状态
  - 估计车体速度/加速度
  - 根据腿长调用 `lqr_k()` 拟合反馈增益
  - 根据 `VM_cal()` 将腿部推力/摆杆力矩映射成关节电机力矩
  - 向轮毂电机和关节电机发 CAN 指令

- `application/MPC.c`
  - 提供轻量的一步式约束优化接口
  - 可对 `T/Tp` 做幅值、变化率和轮侧饱和保护约束
  - 当前已经随工程入库，但默认还没有接入主控制闭环

- `application/LQR_VMC.c`
  - `lqr_k()`: 按腿长拟合 12 维 LQR 增益
  - `get_legparam()`: 由五连杆关节角求腿长和虚拟腿角
  - `get_dphi0()`: 计算虚拟腿角速度
  - `VM_cal()`: 虚拟模型控制到两关节力矩映射

- `Algorithm/MahonyAHRS.c`
  - Mahony 姿态融合

- `Algorithm/kalman_filter.c`
  - 通用卡尔曼滤波器

## 运行模式

从现有代码看，底盘支持以下模式：

- `chassis_weak`
  - 无力模式
  - 轮毂与关节输出清零，适合上电后保护或调试

- `chassis_down`
  - 下蹲/板凳模式
  - 保持腿部目标姿态，同时允许轮毂维持一定控制

- `chassis_normal`
  - 正常模式
  - 执行腿长控制、平衡控制、转向控制与轮毂驱动

模式切换主要由遥控器开关量决定，具体逻辑在 `application/Chassis_Control.c` 中。

## 通信分配

### CAN

- `FDCAN1`
  - 左侧关节电机总线
  - GPIO: `PD0/PD1`

- `FDCAN2`
  - 右侧关节电机总线
  - GPIO: `PB5/PB6`

- `FDCAN3`
  - 轮毂电机总线
  - GPIO: `PD12/PD13`

### UART

- `UART5`
  - 遥控器输入
  - TX: `PC12`
  - RX: `PD2`
  - DMA 接收

## 目录说明

```text
wheel_leg2/
├─ Core/                STM32CubeMX 生成的核心启动与外设初始化
├─ application/         业务控制层，包含 IMU 任务、底盘控制、PID、LQR/VMC、实验性 MPC 模块
├─ Algorithm/           算法模块，如 Mahony、Kalman、自定义融合算法
├─ device/              设备驱动封装，包含 BMI088、CAN、UART、WS2812、DWT、delay
├─ Drivers/             STM32 HAL / CMSIS 官方驱动
├─ Middlewares/         FreeRTOS 等中间件
├─ MDK-ARM/             Keil 工程、编译输出与调试配置
├─ leg/                 Matlab / Simulink 建模、参数拟合与验证文件
├─ wheel_leg.ioc        CubeMX 工程文件
└─ README.md            项目说明
```

## 构建与下载

### 开发环境

- `Keil MDK` 工程文件: `MDK-ARM/wheel_leg.uvprojx`
- `STM32CubeMX` 工程文件: `wheel_leg.ioc`

### 推荐流程

1. 用 `STM32CubeMX` 打开 `wheel_leg.ioc`，确认外设和时钟配置
2. 用 `Keil uVision` 打开 `MDK-ARM/wheel_leg.uvprojx`
3. 选择目标芯片 `STM32H723VGTx`
4. 编译工程
5. 连接下载器后烧录运行

### 编译相关说明

- 工程当前配置基于 `ARM Compiler 5`
- 使用 `FreeRTOS`
- 使用 `CMSIS DSP` 库
- 代码里已经包含较多 CubeMX 自动生成文件，不建议直接手改自动生成区域之外的配置前，先备份 `.ioc`

## 遥控与启动逻辑

从代码现状看，遥控器不仅负责速度/转向输入，也参与腿部电机使能和零位流程：

- `key.d`
  - 用于电机上电使能/失能与启动流程
- `key.a`
  - 在启动阶段参与关节零位设置
- `key.b`
  - 切换目标腿长
- 右摇杆
  - 前后速度与偏航控制

这一段逻辑在 `Start_DM_Motor()` 和 `Chassis_State_Control()` 中实现。

## 数学建模文件

`leg/` 目录下保存了大量 Matlab / Simulink 文件，用于：

- 五连杆腿机构建模
- 腿长/虚拟腿角推导
- LQR 增益拟合
- VMC 参数验证

如果后续要继续优化控制器，这部分文件是重要参考，而不仅仅是历史附件。

## 当前状态判断

结合代码内容，当前工程已经完成了以下基础能力：

- 基础板级外设可运行
- FreeRTOS 任务框架可运行
- BMI088 姿态解算已接入
- CAN 收发与电机反馈解析已接入
- 轮腿控制主框架已经成形
- LQR 与 VMC 控制已接入运行链路
- `MPC` 约束模块代码已经加入仓库，便于后续继续实验

但也能看出一些仍在迭代中的部分：

- 仍有部分注释掉的旧代码和调试逻辑
- 异常检测逻辑未完全启用
- 一些状态机和参数还偏实验性质
- `MPC` 模块尚未正式接入 `Chassis_Control.c` 主输出链路

## 已知注意事项

- 旧版 `readme.md` 存在编码显示异常，建议统一使用 `UTF-8`
- `MDK-ARM/` 下包含编译产物，不全是源码
- `leg/` 目录下包含大量仿真缓存与中间文件，仓库体积会偏大
- 已补充 `.gitignore` 过滤常见的 Simulink 缓存和 Keil 输出目录
- 项目强依赖具体硬件接线、CAN ID 分配和机械零位，移植到新平台前需要先核对

## 后续建议

如果后面继续维护这个项目，建议优先补充这些内容：

- 硬件连接图与接口定义表
- 关节电机型号、减速比、零位标定流程
- 遥控器通道定义
- 启动/急停/保护状态机说明
- 各控制参数的整定依据
- 实测运行效果与日志记录方法

---
