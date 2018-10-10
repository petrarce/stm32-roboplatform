/*
 * File: pid_reg.c
 *
 * Code generated for Simulink model 'pid_reg'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Tue Sep 11 12:51:19 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "pid_reg.h"
#include "pid_reg_private.h"

/* Real-time model */
RT_MODEL_pid_reg_T pid_reg_M_;
RT_MODEL_pid_reg_T *const pid_reg_M = &pid_reg_M_;

/* Model step function */
void pid_reg_step(void)
{
  /* (no output/update code required) */
}

/* Model initialize function */
void pid_reg_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(pid_reg_M, (NULL));
}

/* Model terminate function */
void pid_reg_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
