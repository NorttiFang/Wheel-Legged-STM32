/*
 * File: VM_cal.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 27-Dec-2025 11:52:09
 */

/* Include Files */
#include "VM_cal.h"
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
 * syms T1 T2 F0 Tp phi1 phi4 XE l1 l2 l3 l4
 *
 * Arguments    : double F0
 *                double Tp
 *                double phi1
 *                double phi4
 *                double *Tp1
 *                double *Tp2
 * Return Type  : void
 */
void VM_cal(double F0, double Tp, double phi1, double phi4, double *Tp1,
            double *Tp2)
{
  double A0;
  double B0;
  double C0;
  double XB_tmp;
  double XD;
  double YB_tmp;
  double YD;
  double lBD;
  double phi2;
  double phi3;
  YD = 0.15 * sin(phi4);
  YB_tmp = 0.15 * sin(phi1);
  XD = 0.15 * cos(phi4) + 0.15;
  XB_tmp = 0.15 * cos(phi1);
  C0 = XD - XB_tmp;
  B0 = YD - YB_tmp;
  lBD = sqrt(C0 * C0 + B0 * B0);
  A0 = 0.54 * C0;
  B0 *= 0.54;
  C0 = (lBD * lBD + 0.0729) - 0.0729;
  phi2 = 2.0 * rt_atan2d_snf(B0 + sqrt((A0 * A0 + B0 * B0) - C0 * C0), A0 + C0);
  C0 = 0.27 * cos(phi2);
  lBD = 0.27 * sin(phi2);
  phi3 = rt_atan2d_snf((YB_tmp - YD) + lBD, (XB_tmp - XD) + C0);
  B0 = XB_tmp + C0;
  C0 = YB_tmp + lBD;
  XD = sqrt((B0 - 0.075) * (B0 - 0.075) + C0 * C0);
  C0 = rt_atan2d_snf(C0, B0 - 0.075);
  lBD = sin(phi1 - phi2);
  A0 = sin(phi3 - phi2);
  YD = sin(phi3 - phi4);
  B0 = C0 - phi3;
  C0 -= phi2;
  *Tp1 = 0.15 * sin(B0) * lBD / A0 * F0 + 0.15 * cos(B0) * lBD / XD * A0 * Tp;
  *Tp2 = 0.15 * sin(C0) * YD / A0 * F0 + 0.15 * cos(C0) * YD / XD * A0 * Tp;
}

/*
 * File trailer for VM_cal.c
 *
 * [EOF]
 */
