/*
 * File: _coder_get_legparam_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Dec-2025 12:23:04
 */

#ifndef _CODER_GET_LEGPARAM_API_H
#define _CODER_GET_LEGPARAM_API_H

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
void get_legparam(real_T phi1, real_T phi4, real_T *L0, real_T *phi0);

void get_legparam_api(const mxArray *const prhs[2], int32_T nlhs,
                      const mxArray *plhs[2]);

void get_legparam_atexit(void);

void get_legparam_initialize(void);

void get_legparam_terminate(void);

void get_legparam_xil_shutdown(void);

void get_legparam_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_get_legparam_api.h
 *
 * [EOF]
 */
