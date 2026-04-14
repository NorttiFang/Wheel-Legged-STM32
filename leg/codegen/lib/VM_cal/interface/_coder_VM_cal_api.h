/*
 * File: _coder_VM_cal_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 27-Dec-2025 11:52:09
 */

#ifndef _CODER_VM_CAL_API_H
#define _CODER_VM_CAL_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void VM_cal(real_T F0, real_T Tp, real_T phi1, real_T phi4, real_T *Tp1,
            real_T *Tp2);

void VM_cal_api(const mxArray *const prhs[4], int32_T nlhs,
                const mxArray *plhs[2]);

void VM_cal_atexit(void);

void VM_cal_initialize(void);

void VM_cal_terminate(void);

void VM_cal_xil_shutdown(void);

void VM_cal_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_VM_cal_api.h
 *
 * [EOF]
 */
