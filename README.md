# Wheel-Legged-STM32

这是一个轮腿机器人控制与建模项目仓库，内容覆盖了从 Matlab/Simulink 建模、LQR/VMC 参数拟合，到基于 `STM32H723VGTx` 的嵌入式实机控制工程。当前仓库里同时保留了不同阶段的控制器实现与建模文件，适合做算法推导、参数回溯、工程移植和实机部署。

## 仓库内容

这个仓库目前主要由 3 个部分组成：

- `leg/`
  - Matlab / Simulink 建模与控制参数生成目录
  - 包含五连杆腿机构建模、腿长与虚拟腿角推导、LQR 参数拟合、VMC 验证等文件

- `wheel_leg2/`
  - 较完整的 STM32 轮腿控制工程
  - 包含 `STM32CubeMX + Keil MDK + FreeRTOS` 工程框架
  - 已接入 IMU、CAN 电机控制、LQR/VMC 控制链路
  - 含 README、CHANGELOG 和部分实验性控制模块

- `wheel_leg2_SJ/`
  - 当前更贴近实机部署的一版 STM32 主控工程
  - 继续沿用 `STM32H723VGTx` 平台
  - 保留 BMI088 姿态解算、遥控输入、轮毂/关节电机控制、双腿联合 LQR 与 VMC 控制
  - 代码结构更聚焦当前硬件平台和部署逻辑

## 项目目标

这个项目不是单一的“板级固件”，而是一套完整的轮腿机器人控制开发资产，目标包括：

- 五连杆腿机构运动学建模
- 基于腿长变化的 LQR 增益拟合
- VMC 到关节力矩的映射验证
- `BMI088` 姿态获取与机体状态估计
- `FDCAN` 轮毂与关节电机控制
- 基于 `FreeRTOS` 的 STM32 实时控制任务调度
- 从仿真参数到实机主控代码的落地

## 目录结构

```text
Wheel-Legged-STM32/
├─ README.md                  仓库总览说明
├─ leg/                       Matlab / Simulink 建模、参数拟合、仿真验证
│  ├─ *.m / *.prj             控制参数生成与验证脚本
│  ├─ *.slx / *.slxc          Simulink 模型
│  ├─ codegen/                代码生成相关产物
│  └─ slprj/                  Simulink 缓存与构建中间文件
├─ wheel_leg2/                较完整的 STM32 轮腿控制工程
│  ├─ Core/                   CubeMX 生成的启动与外设初始化
│  ├─ application/            控制逻辑、LQR/VMC、底盘任务
│  ├─ Algorithm/              Mahony、Kalman 等算法模块
│  ├─ device/                 BMI088、CAN、UART、WS2812、DWT 等驱动封装
│  ├─ Drivers/                STM32 HAL / CMSIS 官方驱动
│  ├─ Middlewares/            FreeRTOS 等中间件
│  ├─ MDK-ARM/                Keil 工程与构建输出
│  ├─ wheel_leg.ioc           STM32CubeMX 工程文件
│  ├─ README.md               子工程说明
│  └─ CHANGELOG.md            子工程变更记录
└─ wheel_leg2_SJ/             当前实机部署方向的 STM32 工程
   ├─ Core/                   CubeMX 生成代码
   ├─ application/            底盘控制、腿部控制、LQR/VMC
   ├─ Algorithm/              姿态解算与滤波算法
   ├─ device/                 外设与总线驱动
   ├─ Drivers/                STM32 HAL / CMSIS
   ├─ Middlewares/            FreeRTOS
   ├─ MDK-ARM/                Keil 工程
   ├─ wheel_leg.ioc           STM32CubeMX 工程文件
   └─ README.md               子工程说明
```

## 主要技术栈

- MCU: `STM32H723VGTx`
- 工程工具: `STM32CubeMX`, `Keil MDK`
- RTOS: `FreeRTOS`
- IMU: `BMI088`
- 通信: `FDCAN`, `UART5 + DMA`, `SPI`
- 控制方法: `LQR`, `VMC`
- 建模工具: `Matlab`, `Simulink`

## 嵌入式工程功能概览

`wheel_leg2/` 和 `wheel_leg2_SJ/` 都围绕轮腿底盘主控展开，核心能力大致包括：

- IMU 数据采集与姿态解算
- 遥控器输入解析
- 轮毂电机与腿部关节电机 CAN 控制
- 五连杆腿机构几何求解
- 基于腿长变化的在线 LQR 增益使用
- VMC 输出到关节力矩映射
- 弱电、下蹲、正常运行等底盘模式
- FreeRTOS 多任务调度

## 建模目录说明

`leg/` 目录主要服务于控制器设计和参数生成，而不是简单附件，里面的内容通常用于：

- 推导腿长、虚拟腿角与角速度关系
- 计算或拟合 LQR 增益矩阵
- 验证 VMC 力和关节力矩映射
- 通过 Simulink 进行控制闭环验证

如果后续要重新整定控制器，`leg/` 中的脚本和模型通常需要和 STM32 工程配合使用。

## 开发建议

如果你准备继续维护这个项目，建议按下面的方式理解和使用仓库：

1. 先阅读 `wheel_leg2/README.md` 和 `wheel_leg2_SJ/README.md`，确认当前主用工程版本。
2. 若需要改控制算法，优先联动查看 `leg/` 中的建模脚本。
3. 若需要编译下载，使用各子工程中的 `wheel_leg.ioc` 与 `MDK-ARM/wheel_leg.uvprojx`。
4. 若需要做仓库瘦身，优先清理 `MDK-ARM` 构建产物、`leg/slprj` 和部分 `codegen` 缓存。

## 注意事项

- 仓库中包含较多 Keil 构建产物和 Simulink 缓存文件，体积可能较大。
- 当前不同子目录保留了不同阶段的工程状态，提交到 GitHub 前建议先确认“主发布版本”是哪一个。
- 项目依赖具体硬件接线、CAN ID、零位标定和机械参数，直接移植前需要重新核对。
- 根目录 README 用于说明整个仓库；更细的控制逻辑请分别查看两个子工程内的 README 和源码。
