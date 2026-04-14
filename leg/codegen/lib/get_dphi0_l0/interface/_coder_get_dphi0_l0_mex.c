/*
 * File: _coder_get_dphi0_l0_mex.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Apr-2024 13:23:52
 */

/* Include Files */
#include "_coder_get_dphi0_l0_mex.h"
#include "_coder_get_dphi0_l0_api.h"

/* Function Definitions */
/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[]
 *                int32_T nrhs
 *                const mxArray *prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&get_dphi0_l0_atexit);
  /* Module initialization. */
  get_dphi0_l0_initialize();
  /* Dispatch the entry-point. */
  unsafe_get_dphi0_l0_mexFunction(nlhs, plhs, nrhs, prhs);
  /* Module termination. */
  get_dphi0_l0_terminate();
}

/*
 * Arguments    : void
 * Return Type  : emlrtCTX
 */
emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "GBK", true);
  return emlrtRootTLSGlobal;
}

/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[4]
 *                int32_T nrhs
 *                const mxArray *prhs[4]
 * Return Type  : void
 */
void unsafe_get_dphi0_l0_mexFunction(int32_T nlhs, mxArray *plhs[4],
                                     int32_T nrhs, const mxArray *prhs[4])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *outputs[4];
  int32_T i;
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 4) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 4, 4,
                        12, "get_dphi0_l0");
  }
  if (nlhs > 4) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 12,
                        "get_dphi0_l0");
  }
  /* Call the function. */
  get_dphi0_l0_api(prhs, nlhs, outputs);
  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    i = 1;
  } else {
    i = nlhs;
  }
  emlrtReturnArrays(i, &plhs[0], &outputs[0]);
}

/*
 * File trailer for _coder_get_dphi0_l0_mex.c
 *
 * [EOF]
 */
