/*
 * File: _coder_get_dphi0_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 27-Dec-2025 11:55:37
 */

#ifndef _CODER_GET_DPHI0_API_H
#define _CODER_GET_DPHI0_API_H

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
real_T get_dphi0(real_T phi_1, real_T phi_4, real_T d_phi1, real_T d_phi4);

void get_dphi0_api(const mxArray *const prhs[4], const mxArray **plhs);

void get_dphi0_atexit(void);

void get_dphi0_initialize(void);

void get_dphi0_terminate(void);

void get_dphi0_xil_shutdown(void);

void get_dphi0_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_get_dphi0_api.h
 *
 * [EOF]
 */
