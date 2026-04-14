#include "MPC.h"
#include <math.h>
#include <string.h>


// 一个简单的clamp函数
static float clampf_local(float x, float xmin, float xmax)
{
    if (x < xmin) return xmin;
    if (x > xmax) return xmax;
    return x;
}

/* 一步解析解：
   min  q*(u-u_ref)^2 + r*(u-u_prev)^2
   ->   u* = (q*u_ref + r*u_prev)/(q+r)
*/
//单步“伪 MPC”解析解，求解代价函数的最小值
static float solve_one_step(float u_ref, float u_prev, float q, float r)
{
    const float eps = 1e-6f;
    return (q * u_ref + r * u_prev) / (q + r + eps);
}

/* 当轮扭矩快打满时，压缩 Tp，减轻轮侧需求 */
static float calc_tp_scale(float T_cmd_abs, const LQR_MPC_Param_t *p)
{
    float T_limit = fmaxf(fabsf(p->T_min), fabsf(p->T_max));
    if (T_limit < 1e-6f) return 1.0f;

    float ratio = T_cmd_abs / T_limit;
    if (ratio <= p->wheel_guard_ratio)
        return 1.0f;

    float den = 1.0f - p->wheel_guard_ratio;
    if (den < 1e-6f) den = 1e-6f;

    float excess = (ratio - p->wheel_guard_ratio) / den;
    float scale  = 1.0f - p->tp_guard_gain * excess;

    if (scale < p->tp_min_scale) scale = p->tp_min_scale;
    if (scale > 1.0f) scale = 1.0f;
    return scale;
}

static void store_leg_state(LQR_MPC_State_t *state, const Leg_Posture *leg)
{
    state->x_prev[0] = (float)leg->theata;
    state->x_prev[1] = (float)leg->dtheata;
    state->x_prev[2] = (float)leg->x;
    state->x_prev[3] = (float)leg->dx;
    state->x_prev[4] = (float)leg->phi;
    state->x_prev[5] = (float)leg->dphi;
}

void LQR_MPC_Init(LQR_MPC_State_t *state)
{
    memset(state, 0, sizeof(LQR_MPC_State_t));
}

void LQR_MPC_Refine(Leg_Posture *leg,
                    const LQR_MPC_Param_t *param,
                    LQR_MPC_State_t *state,
                    float dt,
                    bool bypass)
{
    float T_ref, Tp_ref;
    float T_star, Tp_star;
    float T_cmd, Tp_cmd;
    float dT_lim, dTp_lim;
    float tp_scale;

    if (dt < 1e-5f || dt > 0.05f)
        dt = 0.001f;

    T_ref  = (float)leg->T;
    Tp_ref = (float)leg->Tp;

    /* 首次进入，防止一上电就突跳 */
    if (!state->inited)
    {
        state->T_prev  = T_ref;
        state->Tp_prev = Tp_ref;
        store_leg_state(state, leg);
        state->inited = 1;
    }

    /* 旁路：弱电、离地、恢复、跳跃等情况不要用 */
    if ((!param->enabled) || bypass)
    {
        state->T_prev      = T_ref;
        state->Tp_prev     = Tp_ref;
        state->T_ref_dbg   = T_ref;
        state->Tp_ref_dbg  = Tp_ref;
        state->T_out_dbg   = T_ref;
        state->Tp_out_dbg  = Tp_ref;
        state->dT_dbg      = 0.0f;
        state->dTp_dbg     = 0.0f;
        state->tp_scale_dbg = 1.0f;
        state->sat_flag    = 0;
        store_leg_state(state, leg);
        return;
    }

    /* ---------- 第1步：先求 T ---------- */
    T_star = solve_one_step(T_ref, state->T_prev, param->q_T, param->r_dT);

    dT_lim = param->dT_max_per_s * dt;
    if (dT_lim < 1e-4f) dT_lim = 1e-4f;

    T_cmd = clampf_local(T_star,
                         state->T_prev - dT_lim,
                         state->T_prev + dT_lim);
    T_cmd = clampf_local(T_cmd, param->T_min, param->T_max);

    /* ---------- 第2步：根据 T 的占用率压缩 Tp ---------- */
    tp_scale = calc_tp_scale(fabsf(T_cmd), param);

    Tp_ref *= tp_scale;

    /* 对 Tp 的上下限也同步缩放，避免轮侧快饱和时关节还在猛打 */
    {
        float Tp_min_scaled = param->Tp_min * tp_scale;
        float Tp_max_scaled = param->Tp_max * tp_scale;

        /* ---------- 第3步：再求 Tp ---------- */
        Tp_star = solve_one_step(Tp_ref, state->Tp_prev, param->q_Tp, param->r_dTp);

        dTp_lim = param->dTp_max_per_s * dt;
        if (dTp_lim < 1e-4f) dTp_lim = 1e-4f;

        Tp_cmd = clampf_local(Tp_star,
                              state->Tp_prev - dTp_lim,
                              state->Tp_prev + dTp_lim);
        Tp_cmd = clampf_local(Tp_cmd, Tp_min_scaled, Tp_max_scaled);
    }

    /* 写回 */
    leg->T  = T_cmd;
    leg->Tp = Tp_cmd;

    /* 更新状态 */
    state->dT_dbg       = T_cmd - state->T_prev;
    state->dTp_dbg      = Tp_cmd - state->Tp_prev;
    state->T_prev       = T_cmd;
    state->Tp_prev      = Tp_cmd;
    state->T_ref_dbg    = T_ref;
    state->Tp_ref_dbg   = (float)leg->Tp;   /* 注意：这里 leg->Tp 已经是输出值 */
    state->T_out_dbg    = T_cmd;
    state->Tp_out_dbg   = Tp_cmd;
    state->tp_scale_dbg = tp_scale;

    state->sat_flag = 0;
    if (fabsf(T_cmd - T_star) > 1e-5f)  state->sat_flag = 1;
    if (fabsf(Tp_cmd - Tp_star) > 1e-5f) state->sat_flag = 1;

    store_leg_state(state, leg);
}