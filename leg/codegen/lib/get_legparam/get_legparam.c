/*
 * File: get_legparam.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Dec-2025 12:23:04
 */

/* Include Files */
#include "get_legparam.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int i;
    int i1;
    if (u0 > 0.0) {
      i = 1;
    } else {
      i = -1;
    }
    if (u1 > 0.0) {
      i1 = 1;
    } else {
      i1 = -1;
    }
    y = atan2(i, i1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }
  return y;
}

/*
 * Arguments    : double phi1
 *                double phi4
 *                double *L0
 *                double *phi0
 * Return Type  : void
 */
void get_legparam(double phi1, double phi4, double *L0, double *phi0)
{
  double A0;
  double B0;
  double C0;
  double XB_tmp;
  double YB_tmp;
  double lBD;
  YB_tmp = 0.15 * sin(phi1);
  XB_tmp = 0.15 * cos(phi1);
  C0 = (0.15 * cos(phi4) + 0.15) - XB_tmp;
  B0 = 0.15 * sin(phi4) - YB_tmp;
  lBD = sqrt(C0 * C0 + B0 * B0);
  A0 = 0.54 * C0;
  B0 *= 0.54;
  C0 = (lBD * lBD + 0.0729) - 0.0729;
  C0 = 2.0 * rt_atan2d_snf(B0 + sqrt((A0 * A0 + B0 * B0) - C0 * C0), A0 + C0);
  B0 = XB_tmp + 0.27 * cos(C0);
  C0 = YB_tmp + 0.27 * sin(C0);
  *L0 = sqrt((B0 - 0.075) * (B0 - 0.075) + C0 * C0);
  *phi0 = rt_atan2d_snf(C0, B0 - 0.075);
}

/*
 * File trailer for get_legparam.c
 *
 * [EOF]
 */
