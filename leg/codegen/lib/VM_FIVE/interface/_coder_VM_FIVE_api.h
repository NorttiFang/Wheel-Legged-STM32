/*
 * File: _coder_VM_FIVE_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 20-Dec-2023 12:46:47
 */

#ifndef _CODER_VM_FIVE_API_H
#define _CODER_VM_FIVE_API_H

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
void VM_FIVE(real_T F0, real_T Tp, real_T phi1, real_T phi4, real_T *Tp1,
             real_T *Tp2, real_T *L0, real_T *phi0);

void VM_FIVE_api(const mxArray *const prhs[4], int32_T nlhs,
                 const mxArray *plhs[4]);

void VM_FIVE_atexit(void);

void VM_FIVE_initialize(void);

void VM_FIVE_terminate(void);

void VM_FIVE_xil_shutdown(void);

void VM_FIVE_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_VM_FIVE_api.h
 *
 * [EOF]
 */
