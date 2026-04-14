# 更新日志

## v2026.04.12.1

- 更新了 `leg/balance_leg_sl.slx`，同步最新的 Simulink 仿真模型修改。
- 更新了 `leg/HerKules_legth_K.m`，用于生成最新的 K 增益矩阵参数。

## v2026.04.12

- 新增实验性模块 `application/MPC.c` 与 `application/MPC.h`，并将 `MPC.c` 加入 Keil 工程。
- 更新了 `README.md`，补充当前控制架构、项目结构和建模流程说明。
- 新增 `.gitignore` 规则，用于过滤常见的 Keil 与 Simulink 缓存/输出文件。
- 新增以下腿部建模相关文件：
  - `leg/balance_leg_sl.slx`
  - `leg/HerKules_VOCAL_SJ_LQR_v4_with_data.m`
  - `leg/HerKules_legth_K.m`
- 保留了本地缓存类文件，不纳入本次发布范围，包括 `leg/slprj/...` 等临时输出文件。
