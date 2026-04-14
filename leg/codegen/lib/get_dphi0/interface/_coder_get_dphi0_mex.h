/*
 * File: _coder_get_dphi0_mex.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 27-Dec-2025 11:55:37
 */

#ifndef _CODER_GET_DPHI0_MEX_H
#define _CODER_GET_DPHI0_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_get_dphi0_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                  const mxArray *prhs[4]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_get_dphi0_mex.h
 *
 * [EOF]
 */
