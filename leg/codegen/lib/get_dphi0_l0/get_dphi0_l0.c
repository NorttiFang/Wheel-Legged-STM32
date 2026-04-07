/*
 * File: get_dphi0_l0.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Apr-2024 13:23:52
 */

/* Include Files */
#include "get_dphi0_l0.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);

static double rt_powd_snf(double u0, double u1);

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
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Arguments    : double phi_1
 *                double phi_4
 *                double d_phi1
 *                double d_phi4
 *                double *d_phi0
 *                double *d_l0
 *                double *dd_phi0
 *                double *dd_l0
 * Return Type  : void
 */
void get_dphi0_l0(double phi_1, double phi_4, double d_phi1, double d_phi4,
                  double *d_phi0, double *d_l0, double *dd_phi0, double *dd_l0)
{
  double a;
  double a_tmp;
  double a_tmp_tmp;
  double ab_a;
  double b_a;
  double b_a_tmp;
  double b_a_tmp_tmp;
  double b_d_phi0_tmp;
  double b_d_phi0_tmp_tmp;
  double b_d_phi0_tmp_tmp_tmp;
  double b_dd_phi0_tmp;
  double b_dd_phi0_tmp_tmp;
  double bb_a;
  double c_a;
  double c_a_tmp;
  double c_a_tmp_tmp;
  double c_d_phi0_tmp;
  double c_d_phi0_tmp_tmp;
  double c_d_phi0_tmp_tmp_tmp;
  double c_dd_phi0_tmp;
  double cb_a;
  double d_a;
  double d_a_tmp;
  double d_a_tmp_tmp;
  double d_d_phi0_tmp_tmp;
  double d_d_phi0_tmp_tmp_tmp;
  double d_dd_phi0_tmp;
  double d_phi0_tmp;
  double d_phi0_tmp_tmp;
  double d_phi0_tmp_tmp_tmp;
  double db_a;
  double dd_phi0_tmp;
  double dd_phi0_tmp_tmp;
  double e_a;
  double e_a_tmp;
  double e_a_tmp_tmp;
  double e_d_phi0_tmp_tmp_tmp;
  double e_dd_phi0_tmp;
  double eb_a;
  double f_a;
  double f_a_tmp;
  double f_dd_phi0_tmp;
  double fb_a;
  double g_a;
  double g_a_tmp;
  double g_dd_phi0_tmp;
  double gb_a;
  double h_a;
  double h_a_tmp;
  double hb_a;
  double i_a;
  double i_a_tmp;
  double ib_a;
  double j_a;
  double jb_a;
  double k_a;
  double kb_a;
  double l_a;
  double lb_a;
  double m_a;
  double mb_a;
  double n_a;
  double nb_a;
  double o_a;
  double ob_a;
  double p_a;
  double pb_a;
  double q_a;
  double qb_a;
  double r_a;
  double rb_a;
  double s_a;
  double sb_a;
  double t_a;
  double tb_a;
  double u_a;
  double ub_a;
  double v_a;
  double vb_a;
  double w_a;
  double wb_a;
  double x_a;
  double xb_a;
  double y_a;
  double yb_a;
  d_phi0_tmp_tmp_tmp = cos(phi_1);
  b_d_phi0_tmp_tmp_tmp = cos(phi_4);
  c_d_phi0_tmp_tmp_tmp = sin(phi_1);
  d_d_phi0_tmp_tmp_tmp = sin(phi_4);
  a_tmp_tmp = 0.15 * c_d_phi0_tmp_tmp_tmp;
  a_tmp = a_tmp_tmp - 0.15 * d_d_phi0_tmp_tmp_tmp;
  b_a_tmp_tmp = 0.15 * d_phi0_tmp_tmp_tmp;
  b_a_tmp = (0.108 - b_a_tmp_tmp) + 0.15 * b_d_phi0_tmp_tmp_tmp;
  c_a_tmp_tmp = a_tmp * a_tmp;
  d_a_tmp_tmp = b_a_tmp * b_a_tmp;
  c_a_tmp = ((c_a_tmp_tmp + d_a_tmp_tmp) + 0.0625) - 0.0625;
  e_a_tmp_tmp = -0.25 * a_tmp;
  d_a_tmp = ((2.0 * (0.25 * b_a_tmp) + c_a_tmp_tmp) + d_a_tmp_tmp) +
            e_a_tmp_tmp * 0.0;
  e_a_tmp = e_a_tmp_tmp * 2.0;
  f_a_tmp = 0.25 * c_a_tmp_tmp;
  g_a_tmp = 0.25 * d_a_tmp_tmp;
  a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  b_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  c_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  d_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  h_a_tmp = 0.5 * a_tmp;
  i_a_tmp = c_a_tmp + 0.5 * b_a_tmp;
  e_a = (((0.0 - b_a_tmp_tmp) + 0.054) -
         0.25 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
        sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
            0.0;
  f_a = ((0.054 - b_a_tmp_tmp) -
         0.25 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
        sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
            0.0;
  g_a = ((0.054 - b_a_tmp_tmp) -
         0.25 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
        sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
            0.0;
  h_a = (a_tmp_tmp +
         0.0 * cos(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp))) +
        sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
            0.25;
  d_phi0_tmp_tmp = d_phi1 * 0.15 * d_phi0_tmp_tmp_tmp;
  b_d_phi0_tmp_tmp = d_phi0_tmp_tmp - d_phi4 * 0.15 * b_d_phi0_tmp_tmp_tmp;
  e_d_phi0_tmp_tmp_tmp = d_phi1 * 0.15 * c_d_phi0_tmp_tmp_tmp;
  c_d_phi0_tmp_tmp =
      e_d_phi0_tmp_tmp_tmp - d_phi4 * 0.15 * d_d_phi0_tmp_tmp_tmp;
  d_phi0_tmp = -0.25 * b_d_phi0_tmp_tmp;
  e_a_tmp_tmp = 0.5 * c_d_phi0_tmp_tmp * b_a_tmp;
  b_d_phi0_tmp = d_phi0_tmp * 2.0;
  d_phi0_tmp = ((2.0 * (c_d_phi0_tmp_tmp * b_a_tmp) + d_phi0_tmp * 0.0) +
                2.0 * (0.25 * c_d_phi0_tmp_tmp)) +
               2.0 * (b_d_phi0_tmp_tmp * a_tmp);
  d_d_phi0_tmp_tmp =
      2.0 * b_d_phi0_tmp_tmp * (0.15 * sin(phi_1) - 0.15 * sin(phi_4)) +
      2.0 * c_d_phi0_tmp_tmp *
          ((0.108 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4));
  c_d_phi0_tmp =
      (0.5 * b_d_phi0_tmp_tmp * (0.15 * sin(phi_1) - 0.15 * sin(phi_4)) -
       2.0 * d_d_phi0_tmp_tmp *
           (((a_tmp * a_tmp + b_a_tmp * b_a_tmp) + 0.0625) - 0.0625)) +
      e_a_tmp_tmp;
  *d_phi0 =
      -((((d_phi0_tmp_tmp +
           0.5 *
               cos(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
               ((((0.5 *
                       (d_phi0_tmp_tmp - d_phi4 * 0.15 * b_d_phi0_tmp_tmp_tmp) *
                       a_tmp -
                   2.0 *
                       (2.0 * b_d_phi0_tmp_tmp * a_tmp +
                        2.0 *
                            (d_phi1 * 0.15 * c_d_phi0_tmp_tmp_tmp -
                             d_phi4 * 0.15 * d_d_phi0_tmp_tmp_tmp) *
                            b_a_tmp) *
                       c_a_tmp) +
                  e_a_tmp_tmp) /
                     sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                 b_d_phi0_tmp) /
                    d_a_tmp -
                d_phi0_tmp *
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                    (d_a_tmp * d_a_tmp)) *
               (d_a_tmp * d_a_tmp) / (a * a + d_a_tmp * d_a_tmp)) -
          2.0 *
              sin(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
              0.0 *
              ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                    2.0 +
                b_d_phi0_tmp) /
                   d_a_tmp -
               d_phi0_tmp *
                   (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                   (d_a_tmp * d_a_tmp)) *
              (d_a_tmp * d_a_tmp) / (b_a * b_a + d_a_tmp * d_a_tmp)) /
             (((0.054 - b_a_tmp_tmp) -
               0.25 *
                   cos(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp))) +
              sin(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  0.0) -
         ((e_d_phi0_tmp_tmp_tmp +
           2.0 *
               sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
               0.25 *
               ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                     2.0 +
                 b_d_phi0_tmp) /
                    d_a_tmp -
                d_phi0_tmp *
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                    (d_a_tmp * d_a_tmp)) *
               (d_a_tmp * d_a_tmp) / (c_a * c_a + d_a_tmp * d_a_tmp)) +
          0.0 *
              cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
              ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                    2.0 +
                b_d_phi0_tmp) /
                   d_a_tmp -
               d_phi0_tmp *
                   (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                   (d_a_tmp * d_a_tmp)) *
              (d_a_tmp * d_a_tmp) / (d_a * d_a + d_a_tmp * d_a_tmp)) *
             ((a_tmp_tmp +
               0.0 *
                   cos(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp))) +
              sin(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  0.25) /
             (e_a * e_a)) *
        (f_a * f_a)) /
      (g_a * g_a + h_a * h_a);
  a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  b_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  c_a = a_tmp_tmp +
        0.25 * sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp));
  d_a = (b_a_tmp_tmp - 0.054) +
        0.25 * cos(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp));
  *d_l0 =
      -(2.0 *
            (e_d_phi0_tmp_tmp_tmp +
             0.5 *
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (a * a + d_a_tmp * d_a_tmp)) *
            ((b_a_tmp_tmp - 0.054) +
             0.25 * cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp))) -
        2.0 *
            (d_phi0_tmp_tmp +
             0.5 *
                 cos(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (b_a * b_a + d_a_tmp * d_a_tmp)) *
            (a_tmp_tmp +
             0.25 * sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp)))) /
      (2.0 * sqrt(c_a * c_a + d_a * d_a));
  a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  b_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  c_a = (c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
         b_d_phi0_tmp) /
            d_a_tmp -
        d_phi0_tmp * (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
            (d_a_tmp * d_a_tmp);
  d_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  d_a = d_a * d_a + d_a_tmp * d_a_tmp;
  e_a = (c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
         b_d_phi0_tmp) /
            d_a_tmp -
        d_phi0_tmp * (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
            (d_a_tmp * d_a_tmp);
  f_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  f_a = f_a * f_a + d_a_tmp * d_a_tmp;
  g_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  h_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  i_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  i_a = i_a * i_a + d_a_tmp * d_a_tmp;
  j_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  j_a = j_a * j_a + d_a_tmp * d_a_tmp;
  k_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  l_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  m_a = (c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
         b_d_phi0_tmp) /
            d_a_tmp -
        d_phi0_tmp * (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
            (d_a_tmp * d_a_tmp);
  n_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  n_a = n_a * n_a + d_a_tmp * d_a_tmp;
  o_a = (c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
         b_d_phi0_tmp) /
            d_a_tmp -
        d_phi0_tmp * (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
            (d_a_tmp * d_a_tmp);
  p_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  p_a = p_a * p_a + d_a_tmp * d_a_tmp;
  q_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  r_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  s_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  s_a = s_a * s_a + d_a_tmp * d_a_tmp;
  t_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  t_a = t_a * t_a + d_a_tmp * d_a_tmp;
  u_a = (((0.0 - b_a_tmp_tmp) + 0.054) -
         0.25 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
        sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
            0.0;
  v_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  w_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  x_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  y_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  ab_a =
      (((0.0 - b_a_tmp_tmp) + 0.054) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  bb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  cb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  bb_a =
      (e_d_phi0_tmp_tmp_tmp +
       2.0 *
           sin(2.0 *
               rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                 h_a_tmp,
                             i_a_tmp)) *
           0.25 *
           ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                 2.0 +
             b_d_phi0_tmp) /
                d_a_tmp -
            d_phi0_tmp *
                (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                (d_a_tmp * d_a_tmp)) *
           (d_a_tmp * d_a_tmp) / (bb_a * bb_a + d_a_tmp * d_a_tmp)) +
      0.0 *
          cos(2.0 * rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
            b_d_phi0_tmp) /
               d_a_tmp -
           d_phi0_tmp *
               (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
               (d_a_tmp * d_a_tmp)) *
          (d_a_tmp * d_a_tmp) / (cb_a * cb_a + d_a_tmp * d_a_tmp);
  cb_a =
      ((0.054 - b_a_tmp_tmp) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  db_a =
      ((0.054 - b_a_tmp_tmp) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  eb_a = (a_tmp_tmp +
          0.0 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
         sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
             0.25;
  fb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  gb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  hb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  ib_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  jb_a =
      (((0.0 - b_a_tmp_tmp) + 0.054) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  kb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  lb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  mb_a =
      ((0.054 - b_a_tmp_tmp) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  nb_a = (a_tmp_tmp +
          0.0 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
         sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
             0.25;
  ob_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  pb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  qb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  rb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  sb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  tb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  ub_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  vb_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  wb_a =
      (((0.0 - b_a_tmp_tmp) + 0.054) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  xb_a =
      ((0.054 - b_a_tmp_tmp) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  yb_a = (a_tmp_tmp +
          0.0 * cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp))) +
         sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
             0.25;
  e_a_tmp_tmp =
      (((0.0 - b_a_tmp_tmp) + 0.054) -
       0.25 * cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
      sin(2.0 *
          rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          0.0;
  yb_a = yb_a * yb_a + e_a_tmp_tmp * e_a_tmp_tmp;
  dd_phi0_tmp = d_phi1 * d_phi1 * 0.15;
  b_dd_phi0_tmp = d_phi4 * d_phi4 * 0.15;
  dd_phi0_tmp_tmp = dd_phi0_tmp * c_d_phi0_tmp_tmp_tmp;
  c_dd_phi0_tmp = dd_phi0_tmp_tmp - b_dd_phi0_tmp * d_d_phi0_tmp_tmp_tmp;
  b_dd_phi0_tmp_tmp = dd_phi0_tmp * d_phi0_tmp_tmp_tmp;
  dd_phi0_tmp = b_dd_phi0_tmp_tmp - b_dd_phi0_tmp * b_d_phi0_tmp_tmp_tmp;
  b_dd_phi0_tmp = 0.25 * c_dd_phi0_tmp;
  e_a_tmp_tmp = 0.5 * c_dd_phi0_tmp * a_tmp;
  c_a_tmp_tmp = 0.5 * dd_phi0_tmp * b_a_tmp;
  d_dd_phi0_tmp = b_dd_phi0_tmp * 2.0;
  e_dd_phi0_tmp = b_d_phi0_tmp_tmp * b_d_phi0_tmp_tmp;
  f_dd_phi0_tmp = c_d_phi0_tmp_tmp * c_d_phi0_tmp_tmp;
  g_dd_phi0_tmp = 2.0 * e_dd_phi0_tmp;
  d_a_tmp_tmp = 2.0 * f_dd_phi0_tmp;
  e_dd_phi0_tmp =
      ((((0.5 * f_dd_phi0_tmp - 2.0 * (d_d_phi0_tmp_tmp * d_d_phi0_tmp_tmp)) +
         0.5 * e_dd_phi0_tmp) -
        2.0 *
            (((2.0 * dd_phi0_tmp * b_a_tmp - 2.0 * c_dd_phi0_tmp * a_tmp) +
              g_dd_phi0_tmp) +
             d_a_tmp_tmp) *
            c_a_tmp) -
       e_a_tmp_tmp) +
      c_a_tmp_tmp;
  dd_phi0_tmp = ((((g_dd_phi0_tmp + d_a_tmp_tmp) + 2.0 * (0.25 * dd_phi0_tmp)) +
                  b_dd_phi0_tmp * 0.0) -
                 2.0 * (c_dd_phi0_tmp * a_tmp)) +
                2.0 * (dd_phi0_tmp * b_a_tmp);
  b_dd_phi0_tmp = 2.0 * (d_phi0_tmp * d_phi0_tmp);
  c_dd_phi0_tmp = rt_powd_snf(d_a_tmp, 3.0);
  f_dd_phi0_tmp = rt_powd_snf(d_a_tmp, 4.0);
  g_dd_phi0_tmp = 2.0 * d_phi0_tmp * d_a_tmp;
  *dd_phi0 =
      ((((((((((((dd_phi0_tmp_tmp -
                  0.5 *
                      cos(2.0 *
                          rt_atan2d_snf(
                              sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                  h_a_tmp,
                              i_a_tmp)) *
                      (((((d_dd_phi0_tmp +
                           e_dd_phi0_tmp /
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                               2.0) -
                          c_d_phi0_tmp * c_d_phi0_tmp /
                              rt_powd_snf((f_a_tmp - c_a_tmp * c_a_tmp) +
                                              g_a_tmp,
                                          1.5) /
                              4.0) /
                             d_a_tmp -
                         (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                          e_a_tmp) *
                             dd_phi0_tmp / (d_a_tmp * d_a_tmp)) +
                        b_dd_phi0_tmp *
                            (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                             e_a_tmp) /
                            c_dd_phi0_tmp) -
                       2.0 *
                           (c_d_phi0_tmp /
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                                2.0 +
                            b_d_phi0_tmp) *
                           d_phi0_tmp / (d_a_tmp * d_a_tmp)) *
                      (d_a_tmp * d_a_tmp) / (a * a + d_a_tmp * d_a_tmp)) +
                 2.0 *
                     sin(2.0 *
                         rt_atan2d_snf(
                             sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                 h_a_tmp,
                             i_a_tmp)) *
                     0.0 *
                     (((((d_dd_phi0_tmp +
                          e_dd_phi0_tmp /
                              sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                              2.0) -
                         c_d_phi0_tmp * c_d_phi0_tmp /
                             rt_powd_snf(
                                 (f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp, 1.5) /
                             4.0) /
                            d_a_tmp -
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp) *
                            dd_phi0_tmp / (d_a_tmp * d_a_tmp)) +
                       b_dd_phi0_tmp *
                           (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                            e_a_tmp) /
                           c_dd_phi0_tmp) -
                      2.0 *
                          (c_d_phi0_tmp /
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                               2.0 +
                           b_d_phi0_tmp) *
                          d_phi0_tmp / (d_a_tmp * d_a_tmp)) *
                     (d_a_tmp * d_a_tmp) / (b_a * b_a + d_a_tmp * d_a_tmp)) +
                0.0 *
                    cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp)) *
                    (c_a * c_a) * f_dd_phi0_tmp / (d_a * d_a)) +
               4.0 *
                   sin(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   0.25 * (e_a * e_a) * f_dd_phi0_tmp / (f_a * f_a)) -
              cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  ((c_d_phi0_tmp /
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                    b_d_phi0_tmp) /
                       d_a_tmp -
                   d_phi0_tmp *
                       (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                        e_a_tmp) /
                       (d_a_tmp * d_a_tmp)) *
                  d_phi0_tmp * d_a_tmp / (g_a * g_a + d_a_tmp * d_a_tmp)) +
             4.0 *
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 0.0 *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 d_phi0_tmp * d_a_tmp / (h_a * h_a + d_a_tmp * d_a_tmp)) +
            0.5 *
                cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                (g_dd_phi0_tmp +
                 2.0 *
                     (c_d_phi0_tmp /
                          sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                      b_d_phi0_tmp) *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                      e_a_tmp)) *
                ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                      2.0 +
                  b_d_phi0_tmp) /
                     d_a_tmp -
                 d_phi0_tmp *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                     (d_a_tmp * d_a_tmp)) *
                (d_a_tmp * d_a_tmp) / (i_a * i_a)) -
           2.0 *
               sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
               0.0 *
               (g_dd_phi0_tmp +
                2.0 *
                    (c_d_phi0_tmp /
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                     b_d_phi0_tmp) *
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp)) *
               ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                     2.0 +
                 b_d_phi0_tmp) /
                    d_a_tmp -
                d_phi0_tmp *
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                    (d_a_tmp * d_a_tmp)) *
               (d_a_tmp * d_a_tmp) / (j_a * j_a)) /
              (((0.054 - b_a_tmp_tmp) -
                0.25 *
                    cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp))) +
               sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   0.0) +
          ((a_tmp_tmp +
            0.0 * cos(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
           sin(2.0 *
               rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                 h_a_tmp,
                             i_a_tmp)) *
               0.25) *
              ((((((((b_dd_phi0_tmp_tmp +
                      0.0 *
                          cos(2.0 *
                              rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp)) *
                          (((((d_dd_phi0_tmp +
                               (((((0.5 *
                                        (c_d_phi0_tmp_tmp * c_d_phi0_tmp_tmp) -
                                    2.0 *
                                        (d_d_phi0_tmp_tmp * d_d_phi0_tmp_tmp)) +
                                   0.5 *
                                       (b_d_phi0_tmp_tmp * b_d_phi0_tmp_tmp)) -
                                  2.0 *
                                      (((2.0 *
                                             (d_phi1 * d_phi1 * 0.15 *
                                                  d_phi0_tmp_tmp_tmp -
                                              d_phi4 * d_phi4 * 0.15 *
                                                  b_d_phi0_tmp_tmp_tmp) *
                                             b_a_tmp -
                                         2.0 *
                                             (d_phi1 * d_phi1 * 0.15 *
                                                  c_d_phi0_tmp_tmp_tmp -
                                              d_phi4 * d_phi4 * 0.15 *
                                                  d_d_phi0_tmp_tmp_tmp) *
                                             a_tmp) +
                                        2.0 * (b_d_phi0_tmp_tmp *
                                               b_d_phi0_tmp_tmp)) +
                                       2.0 * (c_d_phi0_tmp_tmp *
                                              c_d_phi0_tmp_tmp)) *
                                      (((a_tmp * a_tmp + b_a_tmp * b_a_tmp) +
                                        0.0625) -
                                       0.0625)) -
                                 e_a_tmp_tmp) +
                                c_a_tmp_tmp) /
                                   sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                        g_a_tmp) /
                                   2.0) -
                              c_d_phi0_tmp * c_d_phi0_tmp /
                                  rt_powd_snf((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                  g_a_tmp,
                                              1.5) /
                                  4.0) /
                                 d_a_tmp -
                             (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                              e_a_tmp) *
                                 dd_phi0_tmp / (d_a_tmp * d_a_tmp)) +
                            b_dd_phi0_tmp *
                                (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                                 e_a_tmp) /
                                c_dd_phi0_tmp) -
                           2.0 *
                               (c_d_phi0_tmp /
                                    sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                         g_a_tmp) /
                                    2.0 +
                                b_d_phi0_tmp) *
                               d_phi0_tmp / (d_a_tmp * d_a_tmp)) *
                          (d_a_tmp * d_a_tmp) /
                          (k_a * k_a + d_a_tmp * d_a_tmp)) +
                     2.0 *
                         sin(2.0 *
                             rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                         0.25 *
                         (((((d_dd_phi0_tmp +
                              e_dd_phi0_tmp /
                                  sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                       g_a_tmp) /
                                  2.0) -
                             c_d_phi0_tmp * c_d_phi0_tmp /
                                 rt_powd_snf((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp,
                                             1.5) /
                                 4.0) /
                                d_a_tmp -
                            (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                             e_a_tmp) *
                                dd_phi0_tmp / (d_a_tmp * d_a_tmp)) +
                           b_dd_phi0_tmp *
                               (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                                e_a_tmp) /
                               c_dd_phi0_tmp) -
                          2.0 *
                              (c_d_phi0_tmp /
                                   sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                        g_a_tmp) /
                                   2.0 +
                               b_d_phi0_tmp) *
                              d_phi0_tmp / (d_a_tmp * d_a_tmp)) *
                         (d_a_tmp * d_a_tmp) /
                         (l_a * l_a + d_a_tmp * d_a_tmp)) +
                    cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp)) *
                        (m_a * m_a) * f_dd_phi0_tmp / (n_a * n_a)) -
                   4.0 *
                       sin(2.0 *
                           rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                       0.0 * (o_a * o_a) * f_dd_phi0_tmp / (p_a * p_a)) +
                  0.0 *
                      cos(2.0 *
                          rt_atan2d_snf(
                              sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                  h_a_tmp,
                              i_a_tmp)) *
                      ((c_d_phi0_tmp /
                            sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                            2.0 +
                        b_d_phi0_tmp) /
                           d_a_tmp -
                       d_phi0_tmp *
                           (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                            e_a_tmp) /
                           (d_a_tmp * d_a_tmp)) *
                      d_phi0_tmp * d_a_tmp / (q_a * q_a + d_a_tmp * d_a_tmp)) +
                 4.0 *
                     sin(2.0 *
                         rt_atan2d_snf(
                             sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                 h_a_tmp,
                             i_a_tmp)) *
                     0.25 *
                     ((c_d_phi0_tmp /
                           sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                       b_d_phi0_tmp) /
                          d_a_tmp -
                      d_phi0_tmp *
                          (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                           e_a_tmp) /
                          (d_a_tmp * d_a_tmp)) *
                     d_phi0_tmp * d_a_tmp / (r_a * r_a + d_a_tmp * d_a_tmp)) -
                0.0 *
                    cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp)) *
                    (g_dd_phi0_tmp +
                     2.0 *
                         (c_d_phi0_tmp /
                              sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                              2.0 +
                          b_d_phi0_tmp) *
                         (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                          e_a_tmp)) *
                    ((c_d_phi0_tmp /
                          sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                      b_d_phi0_tmp) /
                         d_a_tmp -
                     d_phi0_tmp *
                         (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                          e_a_tmp) /
                         (d_a_tmp * d_a_tmp)) *
                    (d_a_tmp * d_a_tmp) / (s_a * s_a)) -
               2.0 *
                   sin(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   0.25 *
                   (g_dd_phi0_tmp +
                    2.0 *
                        (c_d_phi0_tmp /
                             sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                             2.0 +
                         b_d_phi0_tmp) *
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp)) *
                   ((c_d_phi0_tmp /
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                     b_d_phi0_tmp) /
                        d_a_tmp -
                    d_phi0_tmp *
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp) /
                        (d_a_tmp * d_a_tmp)) *
                   (d_a_tmp * d_a_tmp) / (t_a * t_a)) /
              (u_a * u_a)) +
         2.0 *
             ((e_d_phi0_tmp_tmp_tmp +
               2.0 *
                   sin(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   0.25 *
                   ((c_d_phi0_tmp /
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                     b_d_phi0_tmp) /
                        d_a_tmp -
                    d_phi0_tmp *
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp) /
                        (d_a_tmp * d_a_tmp)) *
                   (d_a_tmp * d_a_tmp) / (v_a * v_a + d_a_tmp * d_a_tmp)) +
              0.0 *
                  cos(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  ((c_d_phi0_tmp /
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                    b_d_phi0_tmp) /
                       d_a_tmp -
                   d_phi0_tmp *
                       (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                        e_a_tmp) /
                       (d_a_tmp * d_a_tmp)) *
                  (d_a_tmp * d_a_tmp) / (w_a * w_a + d_a_tmp * d_a_tmp)) *
             ((d_phi0_tmp_tmp +
               0.5 *
                   cos(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   ((c_d_phi0_tmp /
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                     b_d_phi0_tmp) /
                        d_a_tmp -
                    d_phi0_tmp *
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp) /
                        (d_a_tmp * d_a_tmp)) *
                   (d_a_tmp * d_a_tmp) / (x_a * x_a + d_a_tmp * d_a_tmp)) -
              2.0 *
                  sin(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  0.0 *
                  ((c_d_phi0_tmp /
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                    b_d_phi0_tmp) /
                       d_a_tmp -
                   d_phi0_tmp *
                       (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                        e_a_tmp) /
                       (d_a_tmp * d_a_tmp)) *
                  (d_a_tmp * d_a_tmp) / (y_a * y_a + d_a_tmp * d_a_tmp)) /
             (ab_a * ab_a)) -
        2.0 * (bb_a * bb_a) *
            ((a_tmp_tmp +
              0.0 * cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp))) +
             sin(2.0 *
                 rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 0.25) /
            rt_powd_snf(
                (((0.0 - b_a_tmp_tmp) + 0.054) -
                 0.25 * cos(2.0 *
                            rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
                    sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp)) *
                        0.0,
                3.0)) *
           (cb_a * cb_a) / (db_a * db_a + eb_a * eb_a) -
       2.0 *
           (((d_phi0_tmp_tmp +
              0.5 *
                  cos(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  ((c_d_phi0_tmp /
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                    b_d_phi0_tmp) /
                       d_a_tmp -
                   d_phi0_tmp *
                       (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                        e_a_tmp) /
                       (d_a_tmp * d_a_tmp)) *
                  (d_a_tmp * d_a_tmp) / (fb_a * fb_a + d_a_tmp * d_a_tmp)) -
             2.0 *
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 0.0 *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (gb_a * gb_a + d_a_tmp * d_a_tmp)) /
                (((0.054 - b_a_tmp_tmp) -
                  0.25 * cos(2.0 *
                             rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp))) +
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                     0.0) -
            ((e_d_phi0_tmp_tmp_tmp +
              2.0 *
                  sin(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  0.25 *
                  ((c_d_phi0_tmp /
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                    b_d_phi0_tmp) /
                       d_a_tmp -
                   d_phi0_tmp *
                       (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                        e_a_tmp) /
                       (d_a_tmp * d_a_tmp)) *
                  (d_a_tmp * d_a_tmp) / (hb_a * hb_a + d_a_tmp * d_a_tmp)) +
             0.0 *
                 cos(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (ib_a * ib_a + d_a_tmp * d_a_tmp)) *
                ((a_tmp_tmp +
                  0.0 * cos(2.0 *
                            rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                     0.25) /
                (jb_a * jb_a)) *
           ((e_d_phi0_tmp_tmp_tmp +
             2.0 *
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 0.25 *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (kb_a * kb_a + d_a_tmp * d_a_tmp)) +
            0.0 *
                cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                      2.0 +
                  b_d_phi0_tmp) /
                     d_a_tmp -
                 d_phi0_tmp *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                     (d_a_tmp * d_a_tmp)) *
                (d_a_tmp * d_a_tmp) / (lb_a * lb_a + d_a_tmp * d_a_tmp)) *
           (((0.054 - b_a_tmp_tmp) -
             0.25 * cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp))) +
            sin(2.0 *
                rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                  h_a_tmp,
                              i_a_tmp)) *
                0.0) /
           (mb_a * mb_a + nb_a * nb_a)) +
      (2.0 *
           ((d_phi0_tmp_tmp +
             0.5 *
                 cos(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (ob_a * ob_a + d_a_tmp * d_a_tmp)) -
            2.0 *
                sin(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                0.0 *
                ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                      2.0 +
                  b_d_phi0_tmp) /
                     d_a_tmp -
                 d_phi0_tmp *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                     (d_a_tmp * d_a_tmp)) *
                (d_a_tmp * d_a_tmp) / (pb_a * pb_a + d_a_tmp * d_a_tmp)) *
           ((a_tmp_tmp +
             0.0 * cos(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp))) +
            sin(2.0 *
                rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                  h_a_tmp,
                              i_a_tmp)) *
                0.25) +
       2.0 *
           ((e_d_phi0_tmp_tmp_tmp +
             2.0 *
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 0.25 *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (qb_a * qb_a + d_a_tmp * d_a_tmp)) +
            0.0 *
                cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                      2.0 +
                  b_d_phi0_tmp) /
                     d_a_tmp -
                 d_phi0_tmp *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                     (d_a_tmp * d_a_tmp)) *
                (d_a_tmp * d_a_tmp) / (rb_a * rb_a + d_a_tmp * d_a_tmp)) *
           (((0.054 - b_a_tmp_tmp) -
             0.25 * cos(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp))) +
            sin(2.0 *
                rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                  h_a_tmp,
                              i_a_tmp)) *
                0.0)) *
          (((d_phi0_tmp_tmp +
             0.5 *
                 cos(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (sb_a * sb_a + d_a_tmp * d_a_tmp)) -
            2.0 *
                sin(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                0.0 *
                ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                      2.0 +
                  b_d_phi0_tmp) /
                     d_a_tmp -
                 d_phi0_tmp *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                     (d_a_tmp * d_a_tmp)) *
                (d_a_tmp * d_a_tmp) / (tb_a * tb_a + d_a_tmp * d_a_tmp)) /
               (((0.054 - b_a_tmp_tmp) -
                 0.25 * cos(2.0 *
                            rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) +
                sin(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                    0.0) -
           ((e_d_phi0_tmp_tmp_tmp +
             2.0 *
                 sin(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 0.25 *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (ub_a * ub_a + d_a_tmp * d_a_tmp)) +
            0.0 *
                cos(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                      2.0 +
                  b_d_phi0_tmp) /
                     d_a_tmp -
                 d_phi0_tmp *
                     (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                     (d_a_tmp * d_a_tmp)) *
                (d_a_tmp * d_a_tmp) / (vb_a * vb_a + d_a_tmp * d_a_tmp)) *
               ((a_tmp_tmp +
                 0.0 * cos(2.0 *
                           rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp))) +
                sin(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                    0.25) /
               (wb_a * wb_a)) *
          (xb_a * xb_a) / (yb_a * yb_a);
  a = (c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
       b_d_phi0_tmp) /
          d_a_tmp -
      d_phi0_tmp * (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
          (d_a_tmp * d_a_tmp);
  b_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  b_a = b_a * b_a + d_a_tmp * d_a_tmp;
  c_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  d_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  e_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  e_a = e_a * e_a + d_a_tmp * d_a_tmp;
  f_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  f_a =
      d_phi0_tmp_tmp +
      0.5 *
          cos(2.0 * rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
            b_d_phi0_tmp) /
               d_a_tmp -
           d_phi0_tmp *
               (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
               (d_a_tmp * d_a_tmp)) *
          (d_a_tmp * d_a_tmp) / (f_a * f_a + d_a_tmp * d_a_tmp);
  g_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  g_a =
      e_d_phi0_tmp_tmp_tmp +
      0.5 *
          sin(2.0 * rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
          ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
            b_d_phi0_tmp) /
               d_a_tmp -
           d_phi0_tmp *
               (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
               (d_a_tmp * d_a_tmp)) *
          (d_a_tmp * d_a_tmp) / (g_a * g_a + d_a_tmp * d_a_tmp);
  h_a = (c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
         b_d_phi0_tmp) /
            d_a_tmp -
        d_phi0_tmp * (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
            (d_a_tmp * d_a_tmp);
  i_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  i_a = i_a * i_a + d_a_tmp * d_a_tmp;
  j_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  k_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  l_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  l_a = l_a * l_a + d_a_tmp * d_a_tmp;
  m_a = a_tmp_tmp +
        0.25 * sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp));
  n_a = (b_a_tmp_tmp - 0.054) +
        0.25 * cos(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp));
  o_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  p_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp;
  o_a =
      2.0 *
          (e_d_phi0_tmp_tmp_tmp +
           0.5 *
               sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
               ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                     2.0 +
                 b_d_phi0_tmp) /
                    d_a_tmp -
                d_phi0_tmp *
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                    (d_a_tmp * d_a_tmp)) *
               (d_a_tmp * d_a_tmp) / (o_a * o_a + d_a_tmp * d_a_tmp)) *
          ((b_a_tmp_tmp - 0.054) +
           0.25 * cos(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp))) -
      2.0 *
          (d_phi0_tmp_tmp +
           0.5 *
               cos(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
               ((c_d_phi0_tmp / sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                     2.0 +
                 b_d_phi0_tmp) /
                    d_a_tmp -
                d_phi0_tmp *
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) + e_a_tmp) /
                    (d_a_tmp * d_a_tmp)) *
               (d_a_tmp * d_a_tmp) / (p_a * p_a + d_a_tmp * d_a_tmp)) *
          (a_tmp_tmp +
           0.25 * sin(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)));
  p_a = a_tmp_tmp +
        0.25 * sin(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp));
  q_a = (b_a_tmp_tmp - 0.054) +
        0.25 * cos(2.0 *
                   rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp));
  *dd_l0 =
      -(((2.0 *
              ((b_a_tmp_tmp - 0.054) +
               0.25 *
                   cos(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp))) *
              ((((b_dd_phi0_tmp_tmp +
                  cos(2.0 * rt_atan2d_snf(
                                sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                      (a * a) * f_dd_phi0_tmp / (b_a * b_a)) +
                 0.5 *
                     sin(2.0 *
                         rt_atan2d_snf(
                             sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                 h_a_tmp,
                             i_a_tmp)) *
                     (((((d_dd_phi0_tmp +
                          e_dd_phi0_tmp /
                              sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                              2.0) -
                         c_d_phi0_tmp * c_d_phi0_tmp /
                             rt_powd_snf(
                                 (f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp, 1.5) /
                             4.0) /
                            d_a_tmp -
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp) *
                            dd_phi0_tmp / (d_a_tmp * d_a_tmp)) +
                       b_dd_phi0_tmp *
                           (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                            e_a_tmp) /
                           c_dd_phi0_tmp) -
                      2.0 *
                          (c_d_phi0_tmp /
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                               2.0 +
                           b_d_phi0_tmp) *
                          d_phi0_tmp / (d_a_tmp * d_a_tmp)) *
                     (d_a_tmp * d_a_tmp) / (c_a * c_a + d_a_tmp * d_a_tmp)) +
                sin(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                    ((c_d_phi0_tmp /
                          sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                      b_d_phi0_tmp) /
                         d_a_tmp -
                     d_phi0_tmp *
                         (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                          e_a_tmp) /
                         (d_a_tmp * d_a_tmp)) *
                    d_phi0_tmp * d_a_tmp / (d_a * d_a + d_a_tmp * d_a_tmp)) -
               0.5 *
                   sin(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   (g_dd_phi0_tmp +
                    2.0 *
                        (c_d_phi0_tmp /
                             sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                             2.0 +
                         b_d_phi0_tmp) *
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp)) *
                   ((c_d_phi0_tmp /
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                     b_d_phi0_tmp) /
                        d_a_tmp -
                    d_phi0_tmp *
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                         e_a_tmp) /
                        (d_a_tmp * d_a_tmp)) *
                   (d_a_tmp * d_a_tmp) / (e_a * e_a)) -
          2.0 * (f_a * f_a)) -
         2.0 * (g_a * g_a)) +
        2.0 *
            (a_tmp_tmp +
             0.25 * sin(2.0 * rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 g_a_tmp) -
                                                h_a_tmp,
                                            i_a_tmp))) *
            ((((dd_phi0_tmp_tmp +
                sin(2.0 *
                    rt_atan2d_snf(
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) - h_a_tmp,
                        i_a_tmp)) *
                    (h_a * h_a) * f_dd_phi0_tmp / (i_a * i_a)) -
               0.5 *
                   cos(2.0 * rt_atan2d_snf(
                                 sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                     h_a_tmp,
                                 i_a_tmp)) *
                   (((((d_dd_phi0_tmp +
                        e_dd_phi0_tmp /
                            sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                            2.0) -
                       c_d_phi0_tmp * c_d_phi0_tmp /
                           rt_powd_snf((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp,
                                       1.5) /
                           4.0) /
                          d_a_tmp -
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) *
                          dd_phi0_tmp / (d_a_tmp * d_a_tmp)) +
                     b_dd_phi0_tmp *
                         (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                          e_a_tmp) /
                         c_dd_phi0_tmp) -
                    2.0 *
                        (c_d_phi0_tmp /
                             sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) /
                             2.0 +
                         b_d_phi0_tmp) *
                        d_phi0_tmp / (d_a_tmp * d_a_tmp)) *
                   (d_a_tmp * d_a_tmp) / (j_a * j_a + d_a_tmp * d_a_tmp)) -
              cos(2.0 *
                  rt_atan2d_snf(sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                    h_a_tmp,
                                i_a_tmp)) *
                  ((c_d_phi0_tmp /
                        sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                    b_d_phi0_tmp) /
                       d_a_tmp -
                   d_phi0_tmp *
                       (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                        e_a_tmp) /
                       (d_a_tmp * d_a_tmp)) *
                  d_phi0_tmp * d_a_tmp / (k_a * k_a + d_a_tmp * d_a_tmp)) +
             0.5 *
                 cos(2.0 * rt_atan2d_snf(
                               sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) -
                                   h_a_tmp,
                               i_a_tmp)) *
                 (g_dd_phi0_tmp +
                  2.0 *
                      (c_d_phi0_tmp /
                           sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                       b_d_phi0_tmp) *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp)) *
                 ((c_d_phi0_tmp /
                       sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) / 2.0 +
                   b_d_phi0_tmp) /
                      d_a_tmp -
                  d_phi0_tmp *
                      (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + g_a_tmp) +
                       e_a_tmp) /
                      (d_a_tmp * d_a_tmp)) *
                 (d_a_tmp * d_a_tmp) / (l_a * l_a))) /
          (2.0 * sqrt(m_a * m_a + n_a * n_a)) -
      o_a * o_a / (4.0 * rt_powd_snf(p_a * p_a + q_a * q_a, 1.5));
}

/*
 * File trailer for get_dphi0_l0.c
 *
 * [EOF]
 */
