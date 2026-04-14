#ifndef __MPC_H
#define __MPC_H

#include "Leg_Task.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    /* 代价函数权重
       J = q*(u-u_ref)^2 + r*(u-u_prev)^2
       这里分别对 T / Tp 独立求解 */
    float q_T;
    float q_Tp;
    float r_dT;
    float r_dTp;

    /* 幅值约束 */
    float T_min;
    float T_max;
    float Tp_min;
    float Tp_max;

    /* 变化率约束，单位：每秒最大变化量 */
    float dT_max_per_s;
    float dTp_max_per_s;

    /* 轮扭矩保护：当 |T| 接近极限时，自动压缩 Tp */
    float wheel_guard_ratio;   // 例如 0.75f，超过 75% 开始压缩 Tp
    float tp_guard_gain;       // 压缩强度，0~1
    float tp_min_scale;        // Tp 最小保留比例，例如 0.35f

    uint8_t enabled;
} LQR_MPC_Param_t;

typedef struct
{
    float T_prev;
    float Tp_prev;

    /* 预留给后面“真·增量 MPC”用的上一步状态 */
    float x_prev[6];

    uint8_t inited;
    uint8_t sat_flag;

    /* 调试量 */
    float T_ref_dbg;
    float Tp_ref_dbg;
    float T_out_dbg;
    float Tp_out_dbg;
    float dT_dbg;
    float dTp_dbg;
    float tp_scale_dbg;
} LQR_MPC_State_t;

void LQR_MPC_Init(LQR_MPC_State_t *state);

/* 在最终 T/Tp 生成后调用。bypass=true 时直接透传并同步内部状态 */
void LQR_MPC_Refine(Leg_Posture *leg,
                    const LQR_MPC_Param_t *param,
                    LQR_MPC_State_t *state,
                    float dt,
                    bool bypass);

#endif