/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 *
 * File: AWDControl_data.c
 *
 * Code generated for Simulink model 'AWDControl'.
 *
 * Model version                  : 1.157
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Wed Apr  8 20:52:49 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "AWDControl.h"

/* Constant parameters (default storage) */
const ConstP rtConstP = {
  /* Expression: tanh([-5:5]) + 1
   * Referenced by: '<S7>/1-D Lookup Table'
   */
  { 9.0795737404891774E-5, 0.000670700260932966, 0.0049452463132695357,
    0.0359724199241831, 0.23840584404423515, 1.0, 1.7615941559557649,
    1.9640275800758169, 1.9950547536867305, 1.999329299739067, 1.999909204262595
  },

  /* Pooled Parameter (Expression: [0:10])
   * Referenced by:
   *   '<S7>/1-D Lookup Table'
   *   '<S7>/1-D Lookup Table1'
   */
  { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 },

  /* Expression: -1*tanh([-5:5])
   * Referenced by: '<S7>/1-D Lookup Table1'
   */
  { 0.99990920426259511, 0.999329299739067, 0.99505475368673046,
    0.9640275800758169, 0.76159415595576485, -0.0, -0.76159415595576485,
    -0.9640275800758169, -0.99505475368673046, -0.999329299739067,
    -0.99990920426259511 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
