/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 *
 * File: AWDControl.c
 *
 * Code generated for Simulink model 'AWDControl'.
 *
 * Model version                  : 1.166
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Wed Apr 15 17:26:45 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "AWDControl.h"

/* Named constants for Chart: '<S3>/Chart' */
#define CALL_EVENT                     (-1)
#define IN_ERRORS_DETECTED             ((uint8_T)1U)
#define IN_FAILURE                     ((uint8_T)1U)
#define IN_NORMAL                      ((uint8_T)2U)
#define IN_NO_ACTIVE_CHILD             ((uint8_T)0U)
#define IN_NO_ERRORS                   ((uint8_T)2U)
#define event_sensorFailed             (0)

/* Named constants for Chart: '<Root>/Vehicle Dynamic Controller' */
#define IN_AUTO_MODE                   ((uint8_T)1U)
#define IN_CHECK_STATE                 ((uint8_T)1U)
#define IN_DYNAMIC_CONTROL             ((uint8_T)2U)
#define IN_DYNAMIC_CONTROLLER          ((uint8_T)1U)
#define IN_ERROR                       ((uint8_T)1U)
#define IN_ERRORS                      ((uint8_T)1U)
#define IN_MANUAL_MODE                 ((uint8_T)2U)
#define IN_MANUAL_OFF                  ((uint8_T)1U)
#define IN_MANUAL_ON                   ((uint8_T)2U)
#define IN_NORMAL_OPERATION            ((uint8_T)2U)
#define IN_OFF_MODE                    ((uint8_T)3U)
#define IN_OVERRIDE_CONTROL            ((uint8_T)2U)
#define IN_TRACTION_CONTROL            ((uint8_T)3U)
#define event_motorFailed              (0)

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
static real_T look1_binlag(real_T u0, const real_T bp0[], const real_T table[],
  uint32_T maxIndex);

/* Forward declaration for local functions */
static void STEERING_CHECK(int32_T *sfEvent_e);
static boolean_T isManualModeRequest(boolean_T frontLeftMotorOverride, boolean_T
  frontRightmotorOverride);
static boolean_T isCarTurning(real_T steeringAngle);
static boolean_T isVehicleSlipping(real_T rearSR);
static void overrideMotorRequest(boolean_T motorOverride1, boolean_T
  motorOverride2, real_T *motor1, real_T *motor2);
static void OFF_MODE(const CONTROL_MODE *DataTypeConversion);
static void NORMAL_OPERATION(const CONTROL_MODE *DataTypeConversion, const
  int32_T *sfEvent);
static real_T look1_binlag(real_T u0, const real_T bp0[], const real_T table[],
  uint32_T maxIndex)
{
  real_T y;
  real_T frac;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'on'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'on'
   */
  if (u0 < bp0[maxIndex]) {
    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex;
    frac = 0.0;
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'wrapping'
   */
  if (iLeft == maxIndex) {
    y = table[iLeft];
  } else {
    y = (table[iLeft + 1U] - table[iLeft]) * frac + table[iLeft];
  }

  return y;
}

/* Function for Chart: '<S3>/Chart' */
static void STEERING_CHECK(int32_T *sfEvent_e)
{
  int32_T b_previousEvent;

  /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
  switch (rtDW.is_STEERING_CHECK) {
   case IN_FAILURE:
    /* The engine speed data is no longer available. */
    rtDW.BusConversion_InsertedFor_Vehic.steeringAngle = 0.0;
    b_previousEvent = *sfEvent_e;
    *sfEvent_e = event_sensorFailed;
    if (rtDW.is_active_ERROR_STATE != 0U) {
      switch (rtDW.is_ERROR_STATE) {
       case IN_ERRORS_DETECTED:
        rtDW.sensorError = true;
        break;

       case IN_NO_ERRORS:
        if (*sfEvent_e == event_sensorFailed) {
          rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
          rtDW.sensorError = true;
        }
        break;
      }
    }

    *sfEvent_e = b_previousEvent;
    break;

   case IN_NORMAL:
    /* Inport: '<Root>/m_steeringAngle' */
    /* The speed sensor data accurately represents the engine speed. */
    if ((rtU.m_steeringAngle < -180.0) || (rtU.m_steeringAngle > 180.0)) {
      /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
      rtDW.is_STEERING_CHECK = IN_FAILURE;
    } else {
      rtDW.BusConversion_InsertedFor_Vehic.steeringAngle = rtU.m_steeringAngle;
    }

    /* End of Inport: '<Root>/m_steeringAngle' */
    break;
  }
}

/* Function for Chart: '<Root>/Vehicle Dynamic Controller' */
static boolean_T isManualModeRequest(boolean_T frontLeftMotorOverride, boolean_T
  frontRightmotorOverride)
{
  return frontLeftMotorOverride || frontRightmotorOverride;
}

/* Function for Chart: '<Root>/Vehicle Dynamic Controller' */
static boolean_T isCarTurning(real_T steeringAngle)
{
  return fabs(steeringAngle) >= 0.2;
}

/* Function for Chart: '<Root>/Vehicle Dynamic Controller' */
static boolean_T isVehicleSlipping(real_T rearSR)
{
  return rearSR >= 0.2;
}

/* Function for Chart: '<Root>/Vehicle Dynamic Controller' */
static void overrideMotorRequest(boolean_T motorOverride1, boolean_T
  motorOverride2, real_T *motor1, real_T *motor2)
{
  if (motorOverride1 && motorOverride2) {
    *motor1 = 1.0;
    *motor2 = 1.0;
  } else if (motorOverride1 && (!motorOverride2)) {
    *motor1 = 1.0;
    *motor2 = 0.0;
  } else if ((!motorOverride1) && motorOverride2) {
    *motor1 = 0.0;
    *motor2 = 1.0;
  } else {
    *motor1 = 0.0;
    *motor2 = 0.0;
  }
}

/* Function for Chart: '<Root>/Vehicle Dynamic Controller' */
static void OFF_MODE(const CONTROL_MODE *DataTypeConversion)
{
  boolean_T hoisted_cond;
  switch (*DataTypeConversion) {
   case CONTROL_MODE_AUTO:
    rtDW.is_NORMAL_OPERATION = IN_AUTO_MODE;
    rtDW.is_AUTO_MODE = IN_DYNAMIC_CONTROLLER;
    rtDW.is_DYNAMIC_CONTROLLER = IN_CHECK_STATE;
    break;

   case CONTROL_MODE_MANUAL:
    rtDW.is_NORMAL_OPERATION = IN_MANUAL_MODE;
    rtDW.is_MANUAL_MODE = IN_MANUAL_OFF;
    break;

   default:
    /* Outport: '<Root>/c_frontLeftMotorActuate' */
    rtY.c_frontLeftMotorActuate = 0.0;

    /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
     *  Outport: '<Root>/c_frontLeftMotorActuate'
     */
    hoisted_cond = ((!rtU.m_frontLeftSwitch) && (rtY.c_frontLeftMotorActuate ==
      0.0));
    if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
      rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
    }

    rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

    /* Outport: '<Root>/c_frontRightMotorActuate' */
    rtY.c_frontRightMotorActuate = 0.0;

    /* Inport: '<Root>/m_frontRightSwitch' incorporates:
     *  Outport: '<Root>/c_frontRightMotorActuate'
     */
    hoisted_cond = ((!rtU.m_frontRightSwitch) && (rtY.c_frontRightMotorActuate ==
      0.0));
    if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
      rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
    }

    rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
    break;
  }
}

/* Function for Chart: '<Root>/Vehicle Dynamic Controller' */
static void NORMAL_OPERATION(const CONTROL_MODE *DataTypeConversion, const
  int32_T *sfEvent)
{
  boolean_T hoisted_cond;
  real_T rtb_uDLookupTable1;
  real_T rtb_Multiply1;
  real_T rtb_Multiply;
  if (rtDW.sensorError || (*sfEvent == event_motorFailed)) {
    if (rtDW.is_NORMAL_OPERATION == IN_AUTO_MODE) {
      rtDW.is_DYNAMIC_CONTROLLER = IN_NO_ACTIVE_CHILD;
      rtDW.is_AUTO_MODE = IN_NO_ACTIVE_CHILD;
      rtDW.is_NORMAL_OPERATION = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_MANUAL_MODE = IN_NO_ACTIVE_CHILD;
      rtDW.is_NORMAL_OPERATION = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_AWD_CONTROL = IN_ERROR;

    /* Outport: '<Root>/c_frontLeftMalfunction' */
    rtY.c_frontLeftMalfunction = true;

    /* Outport: '<Root>/c_frontRightMalfunction' */
    rtY.c_frontRightMalfunction = true;

    /* Outport: '<Root>/c_frontLeftMotorActuate' */
    rtY.c_frontLeftMotorActuate = 0.0;

    /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
     *  Outport: '<Root>/c_frontLeftMotorActuate'
     */
    hoisted_cond = ((!rtU.m_frontLeftSwitch) && (rtY.c_frontLeftMotorActuate ==
      0.0));
    if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
      rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
    }

    rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

    /* Outport: '<Root>/c_frontRightMotorActuate' */
    rtY.c_frontRightMotorActuate = 0.0;

    /* Inport: '<Root>/m_frontRightSwitch' incorporates:
     *  Outport: '<Root>/c_frontRightMotorActuate'
     */
    hoisted_cond = ((!rtU.m_frontRightSwitch) && (rtY.c_frontRightMotorActuate ==
      0.0));
    if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
      rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
    }

    rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
  } else {
    switch (rtDW.is_NORMAL_OPERATION) {
     case IN_AUTO_MODE:
      switch (*DataTypeConversion) {
       case CONTROL_MODE_OFF:
        rtDW.is_DYNAMIC_CONTROLLER = IN_NO_ACTIVE_CHILD;
        rtDW.is_AUTO_MODE = IN_NO_ACTIVE_CHILD;
        rtDW.is_NORMAL_OPERATION = IN_OFF_MODE;
        break;

       case CONTROL_MODE_MANUAL:
        rtDW.is_DYNAMIC_CONTROLLER = IN_NO_ACTIVE_CHILD;
        rtDW.is_AUTO_MODE = IN_NO_ACTIVE_CHILD;
        rtDW.is_NORMAL_OPERATION = IN_MANUAL_MODE;
        rtDW.is_MANUAL_MODE = IN_MANUAL_OFF;
        break;

       default:
        switch (rtDW.is_AUTO_MODE) {
         case IN_DYNAMIC_CONTROLLER:
          /* Inport: '<Root>/m_activateLeft' incorporates:
           *  Inport: '<Root>/m_activateRight'
           */
          if (isManualModeRequest(rtU.m_activateLeft, rtU.m_activateRight)) {
            rtDW.is_DYNAMIC_CONTROLLER = IN_NO_ACTIVE_CHILD;
            rtDW.is_AUTO_MODE = IN_OVERRIDE_CONTROL;
          } else {
            switch (rtDW.is_DYNAMIC_CONTROLLER) {
             case IN_CHECK_STATE:
              if (isCarTurning
                  (rtDW.BusConversion_InsertedFor_Vehic.steeringAngle)) {
                rtDW.is_DYNAMIC_CONTROLLER = IN_DYNAMIC_CONTROL;
              } else {
                if (isVehicleSlipping
                    (rtDW.BusConversion_InsertedFor_Vehic.slipRatioRear)) {
                  rtDW.is_DYNAMIC_CONTROLLER = IN_TRACTION_CONTROL;
                }
              }
              break;

             case IN_DYNAMIC_CONTROL:
              if (!isCarTurning
                  (rtDW.BusConversion_InsertedFor_Vehic.steeringAngle)) {
                rtDW.is_DYNAMIC_CONTROLLER = IN_CHECK_STATE;
              } else {
                /* Outputs for Function Call SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.torqueVectoring' */
                /* Lookup_n-D: '<S7>/1-D Lookup Table' incorporates:
                 *  Constant: '<S7>/Wheelbase'
                 *  Gain: '<S7>/Vehicle Mass3'
                 *  Gain: '<S7>/height of CM1'
                 *  Product: '<S7>/Divide3'
                 *  Sum: '<S7>/Add'
                 */
                rtb_Multiply1 = look1_binlag(225.0 *
                  rtDW.BusConversion_InsertedFor_Vehic.accelerationX * 0.5 /
                  1.484 + 1147.77, rtConstP.uDLookupTable_bp01Data,
                  rtConstP.pooled5, 1U);

                /* Lookup_n-D: '<S7>/1-D Lookup Table1' incorporates:
                 *  Constant: '<S7>/TrackWidth'
                 *  Gain: '<S7>/Front roll stiffness'
                 *  Gain: '<S7>/Vehicle Mass2'
                 *  Gain: '<S7>/height of CM'
                 *  Product: '<S7>/Divide1'
                 */
                rtb_uDLookupTable1 = look1_binlag(6.9301788608938839 *
                  rtDW.BusConversion_InsertedFor_Vehic.accelerationY * 225.0 *
                  0.5 / 1.6002032004064011, rtConstP.uDLookupTable1_bp01Data,
                  rtConstP.pooled5, 1U);

                /* Product: '<S7>/Multiply' */
                rtb_Multiply = rtb_uDLookupTable1 * rtb_Multiply1;

                /* Product: '<S7>/Multiply1' incorporates:
                 *  Constant: '<S7>/Constant3'
                 *  Sum: '<S7>/Subtract'
                 */
                rtb_Multiply1 *= 1.0 - rtb_uDLookupTable1;

                /* Saturate: '<S7>/Saturation' */
                if (rtb_Multiply > 1.0) {
                  rtY.c_frontLeftMotorActuate = 1.0;
                } else if (rtb_Multiply < 0.0) {
                  rtY.c_frontLeftMotorActuate = 0.0;
                } else {
                  rtY.c_frontLeftMotorActuate = rtb_Multiply;
                }

                /* End of Saturate: '<S7>/Saturation' */
                /* End of Outputs for SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.torqueVectoring' */

                /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
                 *  Outport: '<Root>/c_frontLeftMotorActuate'
                 */
                hoisted_cond = ((!rtU.m_frontLeftSwitch) &&
                                (rtY.c_frontLeftMotorActuate == 0.0));
                if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
                  rtDW.durationLastReferenceTick_1 =
                    rtDW.chartAbsoluteTimeCounter;
                }

                rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

                /* Outputs for Function Call SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.torqueVectoring' */
                /* Saturate: '<S7>/Saturation1' */
                if (rtb_Multiply1 > 1.0) {
                  rtY.c_frontRightMotorActuate = 1.0;
                } else if (rtb_Multiply1 < 0.0) {
                  rtY.c_frontRightMotorActuate = 0.0;
                } else {
                  rtY.c_frontRightMotorActuate = rtb_Multiply1;
                }

                /* End of Saturate: '<S7>/Saturation1' */
                /* End of Outputs for SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.torqueVectoring' */

                /* Inport: '<Root>/m_frontRightSwitch' incorporates:
                 *  Outport: '<Root>/c_frontRightMotorActuate'
                 */
                hoisted_cond = ((!rtU.m_frontRightSwitch) &&
                                (rtY.c_frontRightMotorActuate == 0.0));
                if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
                  rtDW.durationLastReferenceTick_2 =
                    rtDW.chartAbsoluteTimeCounter;
                }

                rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
              }
              break;

             case IN_TRACTION_CONTROL:
              if (!isVehicleSlipping
                  (rtDW.BusConversion_InsertedFor_Vehic.slipRatioRear)) {
                rtDW.is_DYNAMIC_CONTROLLER = IN_CHECK_STATE;
              } else if (isCarTurning
                         (rtDW.BusConversion_InsertedFor_Vehic.steeringAngle)) {
                rtDW.is_DYNAMIC_CONTROLLER = IN_DYNAMIC_CONTROL;
              } else {
                /* Outputs for Function Call SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.tractionControl' */
                /* Outport: '<Root>/c_frontLeftMotorActuate' incorporates:
                 *  Constant: '<S8>/Desired slip ratio'
                 *  Sum: '<S8>/Subtract'
                 */
                rtY.c_frontLeftMotorActuate =
                  rtDW.BusConversion_InsertedFor_Vehic.slipRatioRear - 0.4;

                /* End of Outputs for SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.tractionControl' */

                /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
                 *  Outport: '<Root>/c_frontLeftMotorActuate'
                 */
                hoisted_cond = ((!rtU.m_frontLeftSwitch) &&
                                (rtY.c_frontLeftMotorActuate == 0.0));
                if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
                  rtDW.durationLastReferenceTick_1 =
                    rtDW.chartAbsoluteTimeCounter;
                }

                rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

                /* Outputs for Function Call SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.tractionControl' */
                /* Outport: '<Root>/c_frontRightMotorActuate' incorporates:
                 *  Constant: '<S8>/Desired slip ratio'
                 *  Sum: '<S8>/Subtract'
                 */
                rtY.c_frontRightMotorActuate =
                  rtDW.BusConversion_InsertedFor_Vehic.slipRatioRear - 0.4;

                /* End of Outputs for SubSystem: '<S4>/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.tractionControl' */

                /* Inport: '<Root>/m_frontRightSwitch' incorporates:
                 *  Outport: '<Root>/c_frontRightMotorActuate'
                 */
                hoisted_cond = ((!rtU.m_frontRightSwitch) &&
                                (rtY.c_frontRightMotorActuate == 0.0));
                if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
                  rtDW.durationLastReferenceTick_2 =
                    rtDW.chartAbsoluteTimeCounter;
                }

                rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
              }
              break;
            }
          }
          break;

         case IN_OVERRIDE_CONTROL:
          /* Inport: '<Root>/m_activateLeft' incorporates:
           *  Inport: '<Root>/m_activateRight'
           */
          if (!isManualModeRequest(rtU.m_activateLeft, rtU.m_activateRight)) {
            rtDW.is_AUTO_MODE = IN_DYNAMIC_CONTROLLER;
            rtDW.is_DYNAMIC_CONTROLLER = IN_CHECK_STATE;
          } else {
            /* Outport: '<Root>/c_frontRightMotorActuate' incorporates:
             *  Outport: '<Root>/c_frontLeftMotorActuate'
             */
            overrideMotorRequest(rtU.m_activateLeft, rtU.m_activateRight,
                                 &rtY.c_frontLeftMotorActuate,
                                 &rtY.c_frontRightMotorActuate);

            /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
             *  Outport: '<Root>/c_frontLeftMotorActuate'
             */
            hoisted_cond = ((!rtU.m_frontLeftSwitch) &&
                            (rtY.c_frontLeftMotorActuate == 0.0));
            if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
              rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
            }

            rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

            /* Inport: '<Root>/m_frontRightSwitch' incorporates:
             *  Outport: '<Root>/c_frontRightMotorActuate'
             */
            hoisted_cond = ((!rtU.m_frontRightSwitch) &&
                            (rtY.c_frontRightMotorActuate == 0.0));
            if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
              rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
            }

            rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
          }
          break;
        }
        break;
      }
      break;

     case IN_MANUAL_MODE:
      switch (*DataTypeConversion) {
       case CONTROL_MODE_AUTO:
        rtDW.is_MANUAL_MODE = IN_NO_ACTIVE_CHILD;
        rtDW.is_NORMAL_OPERATION = IN_AUTO_MODE;
        rtDW.is_AUTO_MODE = IN_DYNAMIC_CONTROLLER;
        rtDW.is_DYNAMIC_CONTROLLER = IN_CHECK_STATE;
        break;

       case CONTROL_MODE_OFF:
        rtDW.is_MANUAL_MODE = IN_NO_ACTIVE_CHILD;
        rtDW.is_NORMAL_OPERATION = IN_OFF_MODE;
        break;

       default:
        switch (rtDW.is_MANUAL_MODE) {
         case IN_MANUAL_OFF:
          /* Inport: '<Root>/m_activateLeft' incorporates:
           *  Inport: '<Root>/m_activateRight'
           */
          if (isManualModeRequest(rtU.m_activateLeft, rtU.m_activateRight)) {
            rtDW.is_MANUAL_MODE = IN_MANUAL_ON;
          } else {
            /* Outport: '<Root>/c_frontLeftMotorActuate' */
            rtY.c_frontLeftMotorActuate = 0.0;

            /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
             *  Outport: '<Root>/c_frontLeftMotorActuate'
             */
            hoisted_cond = ((!rtU.m_frontLeftSwitch) &&
                            (rtY.c_frontLeftMotorActuate == 0.0));
            if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
              rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
            }

            rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

            /* Outport: '<Root>/c_frontRightMotorActuate' */
            rtY.c_frontRightMotorActuate = 0.0;

            /* Inport: '<Root>/m_frontRightSwitch' incorporates:
             *  Outport: '<Root>/c_frontRightMotorActuate'
             */
            hoisted_cond = ((!rtU.m_frontRightSwitch) &&
                            (rtY.c_frontRightMotorActuate == 0.0));
            if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
              rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
            }

            rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
          }
          break;

         case IN_MANUAL_ON:
          /* Inport: '<Root>/m_activateLeft' incorporates:
           *  Inport: '<Root>/m_activateRight'
           */
          if (!isManualModeRequest(rtU.m_activateLeft, rtU.m_activateRight)) {
            rtDW.is_MANUAL_MODE = IN_MANUAL_OFF;
          } else {
            /* Outport: '<Root>/c_frontRightMotorActuate' incorporates:
             *  Outport: '<Root>/c_frontLeftMotorActuate'
             */
            overrideMotorRequest(rtU.m_activateLeft, rtU.m_activateRight,
                                 &rtY.c_frontLeftMotorActuate,
                                 &rtY.c_frontRightMotorActuate);

            /* Inport: '<Root>/m_frontLeftSwitch' incorporates:
             *  Outport: '<Root>/c_frontLeftMotorActuate'
             */
            hoisted_cond = ((!rtU.m_frontLeftSwitch) &&
                            (rtY.c_frontLeftMotorActuate == 0.0));
            if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
              rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
            }

            rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;

            /* Inport: '<Root>/m_frontRightSwitch' incorporates:
             *  Outport: '<Root>/c_frontRightMotorActuate'
             */
            hoisted_cond = ((!rtU.m_frontRightSwitch) &&
                            (rtY.c_frontRightMotorActuate == 0.0));
            if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
              rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
            }

            rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
          }
          break;
        }
        break;
      }
      break;

     case IN_OFF_MODE:
      OFF_MODE(DataTypeConversion);
      break;
    }
  }
}

/* Model step function */
void AWDControl_step(void)
{
  boolean_T hoisted_cond;
  real_T rtb_Divide;
  CONTROL_MODE DataTypeConversion;
  int32_T sfEvent_e;
  boolean_T hoisted_cond_tmp;
  boolean_T hoisted_cond_tmp_0;

  /* Chart: '<S3>/Chart' incorporates:
   *  Inport: '<Root>/m_GPSvehicleSpeed'
   *  Inport: '<Root>/m_accelerationLateral'
   *  Inport: '<Root>/m_accelerationLongitudinal'
   *  Inport: '<Root>/m_frontLeftRPM'
   *  Inport: '<Root>/m_frontRightRPM'
   *  Inport: '<Root>/m_rearAxleRPM'
   *  Inport: '<Root>/m_yawRate'
   */
  sfEvent_e = CALL_EVENT;
  if (rtDW.is_active_c2_AWDControl == 0U) {
    rtDW.is_active_c2_AWDControl = 1U;
    rtDW.is_active_STEERING_CHECK = 1U;
    rtDW.is_STEERING_CHECK = IN_NORMAL;
    rtDW.is_active_REAR_AXEL = 1U;
    rtDW.is_REAR_AXEL = IN_NORMAL;
    rtDW.is_active_FRONT_LEFT_WHEEL = 1U;
    rtDW.is_FRONT_LEFT_WHEEL = IN_NORMAL;
    rtDW.is_active_FRONT_RIGHT_WHEEL = 1U;
    rtDW.is_FRONT_RIGHT_WHEEL = IN_NORMAL;
    rtDW.is_active_YAW_RATE_CHECK = 1U;
    rtDW.is_YAW_RATE_CHECK = IN_NORMAL;
    rtDW.is_active_ACCELEROMETER_LONG_CH = 1U;
    rtDW.is_ACCELEROMETER_LONG_CHECK = IN_NORMAL;
    rtDW.is_active_ACCELEROMETER_LATERAL = 1U;
    rtDW.is_ACCELEROMETER_LATERAL_CHECK = IN_NORMAL;
    rtDW.is_active_GPS_SPEED_CHECK = 1U;
    rtDW.is_GPS_SPEED_CHECK = IN_NORMAL;
    rtDW.is_active_ERROR_STATE = 1U;
    rtDW.is_ERROR_STATE = IN_NO_ERRORS;
  } else {
    if (rtDW.is_active_STEERING_CHECK != 0U) {
      STEERING_CHECK(&sfEvent_e);
    }

    if (rtDW.is_active_REAR_AXEL != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_REAR_AXEL) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.rearAxleRPMOut = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_rearAxleRPM < -6538.361853420437) || (rtU.m_rearAxleRPM >
             6538.361853420437)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_REAR_AXEL = IN_FAILURE;
        } else {
          rtDW.rearAxleRPMOut = rtU.m_rearAxleRPM;
        }
        break;
      }
    }

    if (rtDW.is_active_FRONT_LEFT_WHEEL != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_FRONT_LEFT_WHEEL) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.frontLeftRPMOut = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_frontLeftRPM < -6538.361853420437) || (rtU.m_frontLeftRPM >
             6538.361853420437)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_FRONT_LEFT_WHEEL = IN_FAILURE;
        } else {
          rtDW.frontLeftRPMOut = rtU.m_frontLeftRPM;
        }
        break;
      }
    }

    if (rtDW.is_active_FRONT_RIGHT_WHEEL != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_FRONT_RIGHT_WHEEL) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.frontRightRPMOut = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_frontRightRPM < -6538.361853420437) || (rtU.m_frontRightRPM >
             6538.361853420437)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_FRONT_RIGHT_WHEEL = IN_FAILURE;
        } else {
          rtDW.frontRightRPMOut = rtU.m_frontRightRPM;
        }
        break;
      }
    }

    if (rtDW.is_active_YAW_RATE_CHECK != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_YAW_RATE_CHECK) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.BusConversion_InsertedFor_Vehic.yawRate = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_yawRate < -1000.0) || (rtU.m_yawRate > 1000.0)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_YAW_RATE_CHECK = IN_FAILURE;
        } else {
          rtDW.BusConversion_InsertedFor_Vehic.yawRate = rtU.m_yawRate;
        }
        break;
      }
    }

    if (rtDW.is_active_ACCELEROMETER_LONG_CH != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_ACCELEROMETER_LONG_CHECK) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.BusConversion_InsertedFor_Vehic.accelerationX = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_accelerationLongitudinal < -10.0) ||
            (rtU.m_accelerationLongitudinal > 10.0)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_ACCELEROMETER_LONG_CHECK = IN_FAILURE;
        } else {
          rtDW.BusConversion_InsertedFor_Vehic.accelerationX =
            rtU.m_accelerationLongitudinal;
        }
        break;
      }
    }

    if (rtDW.is_active_ACCELEROMETER_LATERAL != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_ACCELEROMETER_LATERAL_CHECK) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.BusConversion_InsertedFor_Vehic.accelerationY = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_accelerationLateral < -50.0) || (rtU.m_accelerationLateral >
             50.0)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_ACCELEROMETER_LATERAL_CHECK = IN_FAILURE;
        } else {
          rtDW.BusConversion_InsertedFor_Vehic.accelerationY =
            rtU.m_accelerationLateral;
        }
        break;
      }
    }

    if (rtDW.is_active_GPS_SPEED_CHECK != 0U) {
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (rtDW.is_GPS_SPEED_CHECK) {
       case IN_FAILURE:
        /* The engine speed data is no longer available. */
        rtDW.GPSvehicleSpeedOut = 0.0;
        if (rtDW.is_active_ERROR_STATE != 0U) {
          switch (rtDW.is_ERROR_STATE) {
           case IN_ERRORS_DETECTED:
            rtDW.sensorError = true;
            break;

           case IN_NO_ERRORS:
            rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
            rtDW.sensorError = true;
            break;
          }
        }
        break;

       case IN_NORMAL:
        /* The speed sensor data accurately represents the engine speed. */
        if ((rtU.m_GPSvehicleSpeed < 0.0) || (rtU.m_GPSvehicleSpeed > 200.0)) {
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          rtDW.is_GPS_SPEED_CHECK = IN_FAILURE;
        } else {
          rtDW.GPSvehicleSpeedOut = rtU.m_GPSvehicleSpeed;
        }
        break;
      }
    }

    if (rtDW.is_active_ERROR_STATE != 0U) {
      switch (rtDW.is_ERROR_STATE) {
       case IN_ERRORS_DETECTED:
        rtDW.sensorError = true;
        break;

       case IN_NO_ERRORS:
        if (sfEvent_e == event_sensorFailed) {
          rtDW.is_ERROR_STATE = IN_ERRORS_DETECTED;
          rtDW.sensorError = true;
        }
        break;
      }
    }
  }

  /* End of Chart: '<S3>/Chart' */

  /* Gain: '<S11>/Gain' */
  rtb_Divide = 3.7495238095238093 * rtDW.GPSvehicleSpeedOut;

  /* BusCreator: '<Root>/BusConversion_InsertedFor_Vehicle Dynamic Controller_at_inport_0' incorporates:
   *  Gain: '<S11>/Gain1'
   *  Product: '<S11>/Divide'
   *  Sum: '<S11>/Subtract'
   */
  rtDW.BusConversion_InsertedFor_Vehic.slipRatioRear = (9.5492965855137211 *
    rtDW.rearAxleRPMOut - rtb_Divide) / rtb_Divide;

  /* Gain: '<S9>/Gain' */
  rtb_Divide = 3.7495238095238093 * rtDW.GPSvehicleSpeedOut;

  /* BusCreator: '<Root>/BusConversion_InsertedFor_Vehicle Dynamic Controller_at_inport_0' incorporates:
   *  Gain: '<S9>/Gain1'
   *  Product: '<S9>/Divide'
   *  Sum: '<S9>/Subtract'
   */
  rtDW.BusConversion_InsertedFor_Vehic.slipRatioFL = (9.5492965855137211 *
    rtDW.frontLeftRPMOut - rtb_Divide) / rtb_Divide;

  /* Gain: '<S10>/Gain' */
  rtb_Divide = 3.7495238095238093 * rtDW.GPSvehicleSpeedOut;

  /* BusCreator: '<Root>/BusConversion_InsertedFor_Vehicle Dynamic Controller_at_inport_0' incorporates:
   *  Gain: '<S10>/Gain1'
   *  Product: '<S10>/Divide'
   *  Sum: '<S10>/Subtract'
   */
  rtDW.BusConversion_InsertedFor_Vehic.slipRatioFR = (9.5492965855137211 *
    rtDW.frontRightRPMOut - rtb_Divide) / rtb_Divide;
  rtDW.BusConversion_InsertedFor_Vehic.velocityX = rtDW.GPSvehicleSpeedOut;
  rtDW.BusConversion_InsertedFor_Vehic.velocityY = 0.0;

  /* DataTypeConversion: '<S1>/Data Type Conversion' incorporates:
   *  Inport: '<Root>/m_controlMode'
   */
  DataTypeConversion = (CONTROL_MODE)rtU.m_controlMode;

  /* Chart: '<Root>/Vehicle Dynamic Controller' incorporates:
   *  Inport: '<Root>/m_frontLeftSwitch'
   *  Inport: '<Root>/m_frontRightSwitch'
   *  Outport: '<Root>/c_frontLeftMotorActuate'
   *  Outport: '<Root>/c_frontRightMotorActuate'
   */
  rtDW.chartAbsoluteTimeCounter++;
  hoisted_cond_tmp = !rtU.m_frontLeftSwitch;
  hoisted_cond = (hoisted_cond_tmp && (rtY.c_frontLeftMotorActuate == 0.0));
  if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
    rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
  }

  rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;
  hoisted_cond_tmp_0 = !rtU.m_frontRightSwitch;
  hoisted_cond = (hoisted_cond_tmp_0 && (rtY.c_frontRightMotorActuate == 0.0));
  if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
    rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
  }

  rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
  sfEvent_e = CALL_EVENT;
  if (rtDW.is_active_c3_AWDControl == 0U) {
    rtDW.chartAbsoluteTimeCounter = 0;
    rtDW.is_active_c3_AWDControl = 1U;
    rtDW.is_active_CHECK_FOR_CONTROL_ERR = 1U;
    rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
    rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
    rtDW.is_CHECK_FOR_CONTROL_ERRORS = IN_NO_ERRORS;
    rtDW.condWasTrueAtLastTimeStep_1 = (hoisted_cond_tmp &&
      (rtY.c_frontLeftMotorActuate == 0.0));
    rtDW.condWasTrueAtLastTimeStep_2 = (hoisted_cond_tmp_0 &&
      (rtY.c_frontRightMotorActuate == 0.0));
    rtDW.is_active_AWD_CONTROL = 1U;
    rtDW.is_AWD_CONTROL = IN_NORMAL_OPERATION;
    rtDW.is_NORMAL_OPERATION = IN_OFF_MODE;
  } else {
    if ((rtDW.is_active_CHECK_FOR_CONTROL_ERR != 0U) &&
        (rtDW.is_CHECK_FOR_CONTROL_ERRORS == IN_NO_ERRORS)) {
      hoisted_cond = (hoisted_cond_tmp && (rtY.c_frontLeftMotorActuate == 0.0));
      if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_1)) {
        rtDW.durationLastReferenceTick_1 = rtDW.chartAbsoluteTimeCounter;
      }

      rtDW.condWasTrueAtLastTimeStep_1 = hoisted_cond;
      if (rtDW.chartAbsoluteTimeCounter - rtDW.durationLastReferenceTick_1 >=
          5000) {
        hoisted_cond = true;
      } else {
        hoisted_cond = (hoisted_cond_tmp_0 && (rtY.c_frontRightMotorActuate ==
          0.0));
        if ((!hoisted_cond) || (!rtDW.condWasTrueAtLastTimeStep_2)) {
          rtDW.durationLastReferenceTick_2 = rtDW.chartAbsoluteTimeCounter;
        }

        rtDW.condWasTrueAtLastTimeStep_2 = hoisted_cond;
        hoisted_cond = (rtDW.chartAbsoluteTimeCounter -
                        rtDW.durationLastReferenceTick_2 >= 5000);
      }

      if (hoisted_cond) {
        rtDW.is_CHECK_FOR_CONTROL_ERRORS = IN_ERRORS;
        sfEvent_e = event_motorFailed;
        if (rtDW.is_active_AWD_CONTROL != 0U) {
          switch (rtDW.is_AWD_CONTROL) {
           case IN_ERROR:
            /* Outport: '<Root>/c_frontLeftMalfunction' */
            rtY.c_frontLeftMalfunction = true;

            /* Outport: '<Root>/c_frontRightMalfunction' */
            rtY.c_frontRightMalfunction = true;
            break;

           case IN_NORMAL_OPERATION:
            NORMAL_OPERATION(&DataTypeConversion, &sfEvent_e);
            break;
          }
        }

        sfEvent_e = CALL_EVENT;
      }
    }

    if (rtDW.is_active_AWD_CONTROL != 0U) {
      switch (rtDW.is_AWD_CONTROL) {
       case IN_ERROR:
        /* Outport: '<Root>/c_frontLeftMalfunction' */
        rtY.c_frontLeftMalfunction = true;

        /* Outport: '<Root>/c_frontRightMalfunction' */
        rtY.c_frontRightMalfunction = true;
        break;

       case IN_NORMAL_OPERATION:
        NORMAL_OPERATION(&DataTypeConversion, &sfEvent_e);
        break;
      }
    }
  }

  /* End of Chart: '<Root>/Vehicle Dynamic Controller' */
}

/* Model initialize function */
void AWDControl_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
