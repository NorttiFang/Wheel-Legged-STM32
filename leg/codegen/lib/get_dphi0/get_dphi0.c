/*
 * File: get_dphi0.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 27-Dec-2025 11:55:37
 */

/* Include Files */
#include "get_dphi0.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double phi_1
 *                double phi_4
 *                double d_phi1
 *                double d_phi4
 * Return Type  : double
 */
double get_dphi0(double phi_1, double phi_4, double d_phi1, double d_phi4)
{
  double a;
  double a_tmp;
  double a_tmp_tmp;
  double b_a;
  double b_a_tmp;
  double b_a_tmp_tmp;
  double b_d_phi0_tmp;
  double b_d_phi0_tmp_tmp;
  double c_a;
  double c_a_tmp;
  double c_a_tmp_tmp;
  double c_d_phi0_tmp;
  double c_d_phi0_tmp_tmp;
  double d_a;
  double d_a_tmp;
  double d_a_tmp_tmp;
  double d_d_phi0_tmp;
  double d_d_phi0_tmp_tmp;
  double d_phi0_tmp;
  double d_phi0_tmp_tmp;
  double e_a;
  double e_a_tmp;
  double f_a;
  double f_a_tmp;
  double g_a;
  double h_a;
  double i_a;
  d_phi0_tmp_tmp = cos(phi_1);
  b_d_phi0_tmp_tmp = sin(phi_1);
  c_d_phi0_tmp_tmp = cos(phi_4);
  d_d_phi0_tmp_tmp = sin(phi_4);
  a_tmp_tmp = 0.15 * b_d_phi0_tmp_tmp;
  a_tmp = a_tmp_tmp - 0.15 * d_d_phi0_tmp_tmp;
  b_a_tmp_tmp = 0.15 * d_phi0_tmp_tmp;
  b_a_tmp = (0.15 - b_a_tmp_tmp) + 0.15 * c_d_phi0_tmp_tmp;
  c_a_tmp_tmp = a_tmp * a_tmp;
  d_a_tmp_tmp = b_a_tmp * b_a_tmp;
  c_a_tmp = ((c_a_tmp_tmp + d_a_tmp_tmp) + 0.0729) - 0.0729;
  d_a_tmp = c_a_tmp + 0.54 * b_a_tmp;
  e_a_tmp = 0.54 * a_tmp;
  f_a_tmp = 0.2916 * c_a_tmp_tmp;
  c_a_tmp_tmp = 0.2916 * d_a_tmp_tmp;
  d_a_tmp_tmp = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  b_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  c_a =
      0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) +
      a_tmp_tmp;
  d_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  e_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  f_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  g_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  h_a =
      0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) +
      a_tmp_tmp;
  i_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  d_phi0_tmp = 0.08748 * d_phi0_tmp_tmp * a_tmp;
  b_d_phi0_tmp = 0.08748 * b_d_phi0_tmp_tmp * b_a_tmp;
  c_d_phi0_tmp = 0.08748 * c_d_phi0_tmp_tmp * a_tmp;
  d_d_phi0_tmp = 0.08748 * d_d_phi0_tmp_tmp * b_a_tmp;
  return d_phi1 *
             ((0.15 * d_phi0_tmp_tmp +
               0.54 *
                   cos(2.0 *
                       atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) *
                   ((((d_phi0_tmp - 2.0 *
                                        (0.3 * d_phi0_tmp_tmp * a_tmp +
                                         0.3 * b_d_phi0_tmp_tmp * b_a_tmp) *
                                        c_a_tmp) +
                      b_d_phi0_tmp) /
                         (2.0 *
                          sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp)) -
                     0.081 * d_phi0_tmp_tmp) /
                        d_a_tmp -
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                     e_a_tmp) *
                        ((0.081 * b_d_phi0_tmp_tmp +
                          0.3 * cos(phi_1) *
                              (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                         0.3 * sin(phi_1) *
                             ((0.15 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4))) /
                        (d_a_tmp * d_a_tmp)) /
                   (d_a_tmp_tmp * d_a_tmp_tmp / (d_a_tmp * d_a_tmp) + 1.0)) /
                  ((0.27 * cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                c_a_tmp_tmp) -
                                           e_a_tmp) /
                                          d_a_tmp)) -
                    0.075) +
                   b_a_tmp_tmp) +
              (0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                           c_a_tmp_tmp) -
                                      e_a_tmp) /
                                     d_a_tmp)) +
               a_tmp_tmp) *
                  (a_tmp_tmp +
                   0.54 *
                       sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                            c_a_tmp_tmp) -
                                       e_a_tmp) /
                                      d_a_tmp)) *
                       ((((d_phi0_tmp -
                           2.0 *
                               (0.3 * cos(phi_1) *
                                    (0.15 * sin(phi_1) - 0.15 * sin(phi_4)) +
                                0.3 * sin(phi_1) *
                                    ((0.15 - 0.15 * cos(phi_1)) +
                                     0.15 * cos(phi_4))) *
                               (((a_tmp * a_tmp + b_a_tmp * b_a_tmp) + 0.0729) -
                                0.0729)) +
                          b_d_phi0_tmp) /
                             (2.0 * sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                         c_a_tmp_tmp)) -
                         0.081 * cos(phi_1)) /
                            d_a_tmp -
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                         e_a_tmp) *
                            ((0.081 * sin(phi_1) +
                              0.3 * cos(phi_1) *
                                  (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                             0.3 * sin(phi_1) *
                                 ((0.15 - 0.15 * cos(phi_1)) +
                                  0.15 * cos(phi_4))) /
                            (d_a_tmp * d_a_tmp)) /
                       (a * a / (d_a_tmp * d_a_tmp) + 1.0)) /
                  (b_a * b_a)) /
             (c_a * c_a / (d_a * d_a) + 1.0) -
         d_phi4 *
             (0.54 *
                  cos(2.0 *
                      atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                            e_a_tmp) /
                           d_a_tmp)) *
                  ((((c_d_phi0_tmp - 2.0 *
                                         (0.3 * c_d_phi0_tmp_tmp * a_tmp +
                                          0.3 * d_d_phi0_tmp_tmp * b_a_tmp) *
                                         c_a_tmp) +
                     d_d_phi0_tmp) /
                        (2.0 *
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp)) -
                    0.081 * c_d_phi0_tmp_tmp) /
                       d_a_tmp -
                   (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                    e_a_tmp) *
                       ((0.081 * d_d_phi0_tmp_tmp +
                         0.3 * cos(phi_4) *
                             (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                        0.3 * sin(phi_4) *
                            ((0.15 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4))) /
                       (d_a_tmp * d_a_tmp)) /
                  ((e_a * e_a / (d_a_tmp * d_a_tmp) + 1.0) *
                   ((0.27 * cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 c_a_tmp_tmp) -
                                            e_a_tmp) /
                                           d_a_tmp)) -
                     0.075) +
                    b_a_tmp_tmp)) +
              0.54 *
                  sin(2.0 *
                      atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                            e_a_tmp) /
                           d_a_tmp)) *
                  (0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                               c_a_tmp_tmp) -
                                          e_a_tmp) /
                                         d_a_tmp)) +
                   a_tmp_tmp) *
                  ((((c_d_phi0_tmp -
                      2.0 *
                          (0.3 * cos(phi_4) *
                               (0.15 * sin(phi_1) - 0.15 * sin(phi_4)) +
                           0.3 * sin(phi_4) *
                               ((0.15 - 0.15 * cos(phi_1)) +
                                0.15 * cos(phi_4))) *
                          (((a_tmp * a_tmp + b_a_tmp * b_a_tmp) + 0.0729) -
                           0.0729)) +
                     d_d_phi0_tmp) /
                        (2.0 *
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp)) -
                    0.081 * cos(phi_4)) /
                       d_a_tmp -
                   (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                    e_a_tmp) *
                       ((0.081 * sin(phi_4) +
                         0.3 * cos(phi_4) *
                             (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                        0.3 * sin(phi_4) *
                            ((0.15 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4))) /
                       (d_a_tmp * d_a_tmp)) /
                  ((f_a * f_a / (d_a_tmp * d_a_tmp) + 1.0) * (g_a * g_a))) /
             (h_a * h_a / (i_a * i_a) + 1.0);
}

/*
 * File trailer for get_dphi0.c
 *
 * [EOF]
 */
