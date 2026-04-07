/*
 * File: _coder_get_dphi0_l0_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Apr-2024 13:23:52
 */

#ifndef _CODER_GET_DPHI0_L0_API_H
#define _CODER_GET_DPHI0_L0_API_H

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
void get_dphi0_l0(real_T phi_1, real_T phi_4, real_T d_phi1, real_T d_phi4,
                  real_T *d_phi0, real_T *d_l0, real_T *dd_phi0, real_T *dd_l0);

void get_dphi0_l0_api(const mxArray *const prhs[4], int32_T nlhs,
                      const mxArray *plhs[4]);

void get_dphi0_l0_atexit(void);

void get_dphi0_l0_initialize(void);

void get_dphi0_l0_terminate(void);

void get_dphi0_l0_xil_shutdown(void);

void get_dphi0_l0_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_get_dphi0_l0_api.h
 *
 * [EOF]
 */
