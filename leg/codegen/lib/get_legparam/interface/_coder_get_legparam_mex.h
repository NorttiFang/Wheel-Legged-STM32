/*
 * File: _coder_get_legparam_mex.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Dec-2025 12:23:04
 */

#ifndef _CODER_GET_LEGPARAM_MEX_H
#define _CODER_GET_LEGPARAM_MEX_H

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

void unsafe_get_legparam_mexFunction(int32_T nlhs, mxArray *plhs[2],
                                     int32_T nrhs, const mxArray *prhs[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_get_legparam_mex.h
 *
 * [EOF]
 */
