/*
 * File: _coder_VM_FIVE_mex.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 20-Dec-2023 12:46:47
 */

#ifndef _CODER_VM_FIVE_MEX_H
#define _CODER_VM_FIVE_MEX_H

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

void unsafe_VM_FIVE_mexFunction(int32_T nlhs, mxArray *plhs[4], int32_T nrhs,
                                const mxArray *prhs[4]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_VM_FIVE_mex.h
 *
 * [EOF]
 */
