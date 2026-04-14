/*
 * File: _coder_get_dphi0_l0_api.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Apr-2024 13:23:52
 */

/* Include Files */
#include "_coder_get_dphi0_l0_api.h"
#include "_coder_get_dphi0_l0_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131627U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "get_dphi0_l0",                                       /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *phi_1,
                               const char_T *identifier);

static const mxArray *emlrt_marshallOut(const real_T u);

/* Function Definitions */
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims = 0;
  real_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 0U,
                          (const void *)&dims);
  ret = *(real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *phi_1
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *phi_1,
                               const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(phi_1), &thisId);
  emlrtDestroyArray(&phi_1);
  return y;
}

/*
 * Arguments    : const real_T u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const mxArray * const prhs[4]
 *                int32_T nlhs
 *                const mxArray *plhs[4]
 * Return Type  : void
 */
void get_dphi0_l0_api(const mxArray *const prhs[4], int32_T nlhs,
                      const mxArray *plhs[4])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  real_T d_l0;
  real_T d_phi0;
  real_T d_phi1;
  real_T d_phi4;
  real_T dd_l0;
  real_T dd_phi0;
  real_T phi_1;
  real_T phi_4;
  st.tls = emlrtRootTLSGlobal;
  /* Marshall function inputs */
  phi_1 = emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "phi_1");
  phi_4 = emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "phi_4");
  d_phi1 = emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "d_phi1");
  d_phi4 = emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "d_phi4");
  /* Invoke the target function */
  get_dphi0_l0(phi_1, phi_4, d_phi1, d_phi4, &d_phi0, &d_l0, &dd_phi0, &dd_l0);
  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(d_phi0);
  if (nlhs > 1) {
    plhs[1] = emlrt_marshallOut(d_l0);
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(dd_phi0);
  }
  if (nlhs > 3) {
    plhs[3] = emlrt_marshallOut(dd_l0);
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void get_dphi0_l0_atexit(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  get_dphi0_l0_xil_terminate();
  get_dphi0_l0_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void get_dphi0_l0_initialize(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, NULL);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void get_dphi0_l0_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_get_dphi0_l0_api.c
 *
 * [EOF]
 */
