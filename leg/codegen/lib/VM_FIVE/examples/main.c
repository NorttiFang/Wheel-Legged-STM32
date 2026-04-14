/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 20-Dec-2023 12:46:47
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
#include "VM_FIVE.h"
#include "VM_FIVE_terminate.h"
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
  main_VM_FIVE();
  /* Terminate the application.
You do not need to do this more than one time. */
  VM_FIVE_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_VM_FIVE(void)
{
  double F0_tmp;
  double L0;
  double Tp1;
  double Tp2;
  double phi0;
  /* Initialize function 'VM_FIVE' input arguments. */
  F0_tmp = argInit_real_T();
  /* Call the entry-point 'VM_FIVE'. */
  VM_FIVE(F0_tmp, F0_tmp, F0_tmp, F0_tmp, &Tp1, &Tp2, &L0, &phi0);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
