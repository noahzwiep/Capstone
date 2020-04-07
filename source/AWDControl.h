/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 *
 * File: AWDControl.h
 *
 * Code generated for Simulink model 'AWDControl'.
 *
 * Model version                  : 1.149
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Tue Apr  7 15:43:56 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_AWDControl_h_
#define RTW_HEADER_AWDControl_h_
#include "rtwtypes.h"
#include <math.h>
#include <stddef.h>
#ifndef AWDControl_COMMON_INCLUDES_
# define AWDControl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AWDControl_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#define AWDControl_M                   (rtM)

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_CONTROL_MODE_
#define DEFINED_TYPEDEF_FOR_CONTROL_MODE_

/* enumeration to track the request control mode */
typedef enum {
  CONTROL_MODE_OFF = 0,                /* Default value */
  CONTROL_MODE_MANUAL,
  CONTROL_MODE_AUTO
} CONTROL_MODE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VehicleState_
#define DEFINED_TYPEDEF_FOR_VehicleState_

typedef struct {
  real_T steeringAngle;
  real_T slipRatioRear;
  real_T slipRatioFL;
  real_T slipRatioFR;
  real_T yawRate;
  real_T accelerationX;
  real_T accelerationY;
  real_T velocityX;
  real_T velocityY;
} VehicleState;

#endif

#ifndef struct_md128433f1ef248ca39112e85ac51f2a3
#define struct_md128433f1ef248ca39112e85ac51f2a3

struct md128433f1ef248ca39112e85ac51f2a3
{
  int32_T S0_isInitialized;
  real_T W0_states[38];
  real_T P0_InitialStates;
  real_T P1_Coefficients[39];
};

#endif                                 /*struct_md128433f1ef248ca39112e85ac51f2a3*/

#ifndef typedef_dsp_FIRFilter_0
#define typedef_dsp_FIRFilter_0

typedef struct md128433f1ef248ca39112e85ac51f2a3 dsp_FIRFilter_0;

#endif                                 /*typedef_dsp_FIRFilter_0*/

#ifndef struct_md9mcuhcXxpHzpZb5firpbNC
#define struct_md9mcuhcXxpHzpZb5firpbNC

struct md9mcuhcXxpHzpZb5firpbNC
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  dsp_FIRFilter_0 cSFunObject;
};

#endif                                 /*struct_md9mcuhcXxpHzpZb5firpbNC*/

#ifndef typedef_dspcodegen_FIRFilter
#define typedef_dspcodegen_FIRFilter

typedef struct md9mcuhcXxpHzpZb5firpbNC dspcodegen_FIRFilter;

#endif                                 /*typedef_dspcodegen_FIRFilter*/

#ifndef typedef_cell_wrap
#define typedef_cell_wrap

typedef struct {
  uint32_T f1[8];
} cell_wrap;

#endif                                 /*typedef_cell_wrap*/

#ifndef struct_mdyQhV8pWswhMUNQqaQbWMYF
#define struct_mdyQhV8pWswhMUNQqaQbWMYF

struct mdyQhV8pWswhMUNQqaQbWMYF
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  cell_wrap inputVarSize;
  int32_T NumChannels;
  dspcodegen_FIRFilter *FilterObj;
};

#endif                                 /*struct_mdyQhV8pWswhMUNQqaQbWMYF*/

#ifndef typedef_dsp_LowpassFilter
#define typedef_dsp_LowpassFilter

typedef struct mdyQhV8pWswhMUNQqaQbWMYF dsp_LowpassFilter;

#endif                                 /*typedef_dsp_LowpassFilter*/

/* Block signals and states (default storage) for system '<S2>/Lowpass Filter1' */
typedef struct {
  dspcodegen_FIRFilter gobj_0;         /* '<S2>/Lowpass Filter1' */
  dspcodegen_FIRFilter gobj_1;         /* '<S2>/Lowpass Filter1' */
  dsp_LowpassFilter obj;               /* '<S2>/Lowpass Filter1' */
  real_T LowpassFilter1_o;             /* '<S2>/Lowpass Filter1' */
  boolean_T objisempty;                /* '<S2>/Lowpass Filter1' */
  boolean_T isInitialized;             /* '<S2>/Lowpass Filter1' */
} DW_LowpassFilter1;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_LowpassFilter1 LowpassFilter;     /* '<S2>/Lowpass Filter1' */
  DW_LowpassFilter1 LowpassFilter1_p;  /* '<S2>/Lowpass Filter1' */
  VehicleState BusConversion_InsertedFor_Vehic;
  real_T frontLeftMotorActuate;        /* '<Root>/Vehicle Dynamic Controller' */
  real_T frontRightMotorActuate;       /* '<Root>/Vehicle Dynamic Controller' */
  real_T rearSlipRatio;                /* '<Root>/Vehicle Dynamic Controller' */
  real_T accelerationLong;             /* '<Root>/Vehicle Dynamic Controller' */
  real_T accelerationLat;              /* '<Root>/Vehicle Dynamic Controller' */
  real_T uDLookupTable1;               /* '<S7>/1-D Lookup Table1' */
  real_T Subtract1;                    /* '<S7>/Subtract1' */
  real_T Gain1;                        /* '<S8>/Gain1' */
  real_T OutportBufferForc_frontRightMot;/* '<S8>/Gain1' */
  real_T rearAxleRPMOut;               /* '<S3>/Chart' */
  real_T frontLeftRPMOut;              /* '<S3>/Chart' */
  real_T frontRightRPMOut;             /* '<S3>/Chart' */
  real_T GPSvehicleSpeedOut;           /* '<S3>/Chart' */
  int32_T sfEvent;                     /* '<Root>/Vehicle Dynamic Controller' */
  int32_T chartAbsoluteTimeCounter;    /* '<Root>/Vehicle Dynamic Controller' */
  int32_T durationLastReferenceTick_1; /* '<Root>/Vehicle Dynamic Controller' */
  int32_T durationLastReferenceTick_2; /* '<Root>/Vehicle Dynamic Controller' */
  int32_T sfEvent_e;                   /* '<S3>/Chart' */
  CONTROL_MODE DataTypeConversion;     /* '<S1>/Data Type Conversion' */
  uint8_T is_active_c3_AWDControl;     /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_CHECK_FOR_CONTROL_ERRORS; /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_active_CHECK_FOR_CONTROL_ERR;/* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_AWD_CONTROL;              /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_active_AWD_CONTROL;       /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_NORMAL_OPERATION;         /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_AUTO_MODE;                /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_DYNAMIC_CONTROLLER;       /* '<Root>/Vehicle Dynamic Controller' */
  uint8_T is_active_c1_AWDControl;     /* '<S3>/Chart' */
  uint8_T is_STEERING_CHECK;           /* '<S3>/Chart' */
  uint8_T is_active_STEERING_CHECK;    /* '<S3>/Chart' */
  uint8_T is_REAR_AXEL;                /* '<S3>/Chart' */
  uint8_T is_active_REAR_AXEL;         /* '<S3>/Chart' */
  uint8_T is_FRONT_LEFT_WHEEL;         /* '<S3>/Chart' */
  uint8_T is_active_FRONT_LEFT_WHEEL;  /* '<S3>/Chart' */
  uint8_T is_FRONT_RIGHT_WHEEL;        /* '<S3>/Chart' */
  uint8_T is_active_FRONT_RIGHT_WHEEL; /* '<S3>/Chart' */
  uint8_T is_YAW_RATE_CHECK;           /* '<S3>/Chart' */
  uint8_T is_active_YAW_RATE_CHECK;    /* '<S3>/Chart' */
  uint8_T is_ACCELEROMETER_LONG_CHECK; /* '<S3>/Chart' */
  uint8_T is_active_ACCELEROMETER_LONG_CH;/* '<S3>/Chart' */
  uint8_T is_ACCELEROMETER_LATERAL_CHECK;/* '<S3>/Chart' */
  uint8_T is_active_ACCELEROMETER_LATERAL;/* '<S3>/Chart' */
  uint8_T is_GPS_SPEED_CHECK;          /* '<S3>/Chart' */
  uint8_T is_active_GPS_SPEED_CHECK;   /* '<S3>/Chart' */
  uint8_T is_ERROR_STATE;              /* '<S3>/Chart' */
  uint8_T is_active_ERROR_STATE;       /* '<S3>/Chart' */
  boolean_T sensorError;               /* '<S3>/Chart' */
  boolean_T condWasTrueAtLastTimeStep_1;/* '<Root>/Vehicle Dynamic Controller' */
  boolean_T condWasTrueAtLastTimeStep_2;/* '<Root>/Vehicle Dynamic Controller' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: tanh([-5:5]) + 1
   * Referenced by: '<S7>/1-D Lookup Table'
   */
  real_T uDLookupTable_tableData[11];

  /* Pooled Parameter (Expression: [0:10])
   * Referenced by:
   *   '<S7>/1-D Lookup Table'
   *   '<S7>/1-D Lookup Table1'
   */
  real_T pooled7[11];

  /* Expression: -1*tanh([-5:5])
   * Referenced by: '<S7>/1-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData[11];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T m_steeringAngle;              /* '<Root>/m_steeringAngle' */
  real_T m_rearAxleRPM;                /* '<Root>/m_rearAxleRPM' */
  real_T m_frontLeftRPM;               /* '<Root>/m_frontLeftRPM' */
  real_T m_frontRightRPM;              /* '<Root>/m_frontRightRPM' */
  real_T m_yawRate;                    /* '<Root>/m_yawRate' */
  real_T m_accelerationLongitudinal;   /* '<Root>/m_accelerationLongitudinal' */
  real_T m_accelerationLateral;        /* '<Root>/m_accelerationLateral' */
  real_T m_GPSvehicleSpeed;            /* '<Root>/m_GPSvehicleSpeed' */
  boolean_T m_activateLeft;            /* '<Root>/m_activateLeft' */
  boolean_T m_activateRight;           /* '<Root>/m_activateRight' */
  uint8_T m_controlMode;               /* '<Root>/m_controlMode' */
  boolean_T m_frontLeftSwitch;         /* '<Root>/m_frontLeftSwitch' */
  boolean_T m_frontRightSwitch;        /* '<Root>/m_frontRightSwitch' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T c_frontLeftMotorActuate;      /* '<Root>/c_frontLeftMotorActuate' */
  real_T c_frontRightMotorActuate;     /* '<Root>/c_frontRightMotorActuate' */
  boolean_T c_frontLeftMalfunction;    /* '<Root>/c_frontLeftMalfunction' */
  boolean_T c_frontRightMalfunction;   /* '<Root>/c_frontRightMalfunction' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T *errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void AWDControl_initialize(void);
extern void AWDControl_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S7>/Laterall_Modifier' : Eliminated nontunable gain of 1
 * Block '<S7>/Longitudinal_Modifier' : Eliminated nontunable gain of 1
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
 * '<Root>' : 'AWDControl'
 * '<S1>'   : 'AWDControl/Input Filtering'
 * '<S2>'   : 'AWDControl/Output Filtering'
 * '<S3>'   : 'AWDControl/Sensor Range Check'
 * '<S4>'   : 'AWDControl/Vehicle Dynamic Controller'
 * '<S5>'   : 'AWDControl/Vehicle State Estimator'
 * '<S6>'   : 'AWDControl/Sensor Range Check/Chart'
 * '<S7>'   : 'AWDControl/Vehicle Dynamic Controller/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.torqueVectoring'
 * '<S8>'   : 'AWDControl/Vehicle Dynamic Controller/AWD_CONTROL.NORMAL_OPERATION.AUTO_MODE.tractionControl'
 * '<S9>'   : 'AWDControl/Vehicle State Estimator/Front Left Wheel Slip Ratio Calculator'
 * '<S10>'  : 'AWDControl/Vehicle State Estimator/Front Right Wheel Slip Ratio Calculator'
 * '<S11>'  : 'AWDControl/Vehicle State Estimator/Rear Wheels Slip Ratio Calculator'
 */
#endif                                 /* RTW_HEADER_AWDControl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
