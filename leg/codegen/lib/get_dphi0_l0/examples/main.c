/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Apr-2024 13:23:52
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "get_dphi0_l0.h"
#include "get_dphi0_l0_terminate.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static double argInit_real_T(void);

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : int argc
 *                char **argv
 * Return Type  : int
 */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* The initialize function is being called automatically from your entry-point
   * function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
  main_get_dphi0_l0();
  /* Terminate the application.
You do not need to do this more than one time. */
  get_dphi0_l0_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_get_dphi0_l0(void)
{
  double d_l0;
  double d_phi0;
  double dd_l0;
  double dd_phi0;
  double phi_1_tmp;
  /* Initialize function 'get_dphi0_l0' input arguments. */
  phi_1_tmp = argInit_real_T();
  /* Call the entry-point 'get_dphi0_l0'. */
  get_dphi0_l0(phi_1_tmp, phi_1_tmp, phi_1_tmp, phi_1_tmp, &d_phi0, &d_l0,
               &dd_phi0, &dd_l0);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
