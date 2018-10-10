/*
 * File: pid_reg.h
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

#ifndef RTW_HEADER_pid_reg_h_
#define RTW_HEADER_pid_reg_h_
#include <stddef.h>
#ifndef pid_reg_COMMON_INCLUDES_
# define pid_reg_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* pid_reg_COMMON_INCLUDES_ */

#include "pid_reg_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Real-time Model Data Structure */
struct tag_RTM_pid_reg_T {
  const char_T *errorStatus;
};

/* Model entry point functions */
extern void pid_reg_initialize(void);
extern void pid_reg_step(void);
extern void pid_reg_terminate(void);

/* Real-time Model object */
extern RT_MODEL_pid_reg_T *const pid_reg_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Gain' : Unused code path elimination
 * Block '<Root>/Gain1' : Unused code path elimination
 * Block '<S1>/Derivative Gain' : Unused code path elimination
 * Block '<S1>/Filter' : Unused code path elimination
 * Block '<S1>/Filter Coefficient' : Unused code path elimination
 * Block '<S1>/Integral Gain' : Unused code path elimination
 * Block '<S1>/Integrator' : Unused code path elimination
 * Block '<S1>/Proportional Gain' : Unused code path elimination
 * Block '<S1>/Saturate' : Unused code path elimination
 * Block '<S1>/Sum' : Unused code path elimination
 * Block '<S1>/SumD' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'pid_reg'
 * '<S1>'   : 'pid_reg/PID Controller'
 */
#endif                                 /* RTW_HEADER_pid_reg_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
