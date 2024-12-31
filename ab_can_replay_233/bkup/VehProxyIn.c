/*
 * File: VehProxyIn.c
 *
 * Code generated for Simulink model 'VehProxyIn'.
 *
 * Model version                  : 1.48
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Thu Jul 18 10:29:06 2024
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: Infineon->TriCore
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "VehProxyIn.h"
#include "rtwtypes.h"
#include "WhlSpdDirct.h"
#include "Rte_Type.h"
#include "VehProxyIn_types.h"
#include "VehProxyIn_private.h"
#include "Rte_Intern.h"
#include "EgoMotionInfo.h"
#include "dds_main.h"

const BodyInfoMsg VehProxyIn_rtZBodyInfoMsg = {
  PwrMdSts_OFF,                        /* TrnOnSts */
  DrvMd_Normal,                        /* DrvSts */
  DrvLcatn_Left,                       /* Drvlocatn */
  LgtSts_Off,                          /* TrnLgtLftSts */
  LgtSts_Off,                          /* TrnLgtRgtSts */
  LgtSts_Off,                          /* HarzardLgtSts */
  LgtSts_Off,                          /* BeamLgtHightSts */
  LgtSts_Off,                          /* BeamLgtlowSts */
  WiprSts_Off,                         /* FrntWiprSts */
  WiprSts_Off,                         /* RearWiprSts */
  LgtSts_Off,                          /* FrntFogLgtSts */
  LgtSts_Off,                          /* RearFogLgtSts */
  DrSts_Open,                          /* DrFrntLeft */
  DrSts_Open,                          /* DrFrntRgt */
  DrSts_Open,                          /* DrRearLeft */
  DrSts_Open,                          /* DrRearRgt */
  DrSts_Open,                          /* FrntHd */
  DrSts_Open,                          /* RearTrnk */
  DrSts_Open,                          /* DrvSeatBelt */
  DrSts_Open,                          /* PassengrSeatBelt */
  RearMirrorSts_Fold                   /* RearMirrorSts */
};                                     /* BodyInfoMsg ground */

/* Exported block states */
MotionInfoMsg MotionInfoMsgData;/* Simulink.Signal object 'MotionInfoMsgData' */
BrakingInfoMsg BrakingInfoMsgData;
                               /* Simulink.Signal object 'BrakingInfoMsgData' */
SteeringInfoMsg SteeringInfoMsgData;
                              /* Simulink.Signal object 'SteeringInfoMsgData' */
PropulsionInfoMsg PropulsionInfoMsgData;
                            /* Simulink.Signal object 'PropulsionInfoMsgData' */
ParamaterSettingMsg ParamaterSettingMsgData;
                          /* Simulink.Signal object 'ParamaterSettingMsgData' */
SwitchSettingMsg SwitchSettingMsgData;
                             /* Simulink.Signal object 'SwitchSettingMsgData' */

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
BodyInfoMsg BodyInfoMsgData;

/* PublicStructure Variables for Internal Data */
ARID_DEF_VehProxyIn_T VehProxyIn_ARID_DEF;/* '<S52>/Unit Delay' */
boolean pedal_L_override_flag;
uint8 ldw_switch_status; //define LDWSwitchStatus for SWK ET6 demo.
uint8 ldp_switch_status; //define LKASwitchStatus for SWK ET6 demo.
uint8 ldw_switch_status_value = 2;
uint8 ldp_switch_status_value = 2;


uint8 ACC_MainSwitch_pre = 0;
uint8 ACC_MainSwitch_HoldCount = 0;
uint8 ACC_MainSwitchStatus = 0;
uint8 ACC_v_cancel_status = 0;
extern float32 angle_offset;
float32 DriverInputTrqValue = 2.5F;
uint16 VehProxyInRunningTime = 0u;

// extern EgoVehicleInfo ReadEgoVehInfo;
proj233_msgs_msg_EgoMotionInfo EgoMotionInfo;

extern int DDS_publish_VehicleData(proj233_msgs_msg_EgoMotionInfo *info);
// extern uint64 EthTsyn_time_get(void);

uint32 Float_data_receive(uint8 id)
{
   uint32 Rte_InternalBuffer =0;
  //  float32 sss = 0.0;

    if (Com_ReceiveSignal(id, &Rte_InternalBuffer) == E_OK)
    {
      //Rte_ComHook_SGCounterIn_257R_SigRx(&Rte_InternalBuffer);
      // sss = (float32)Rte_InternalBuffer/100;
      //Rte_SetEvent_DRE_E1F727A1A2934C26670316F43552DB25();
    }
   return Rte_InternalBuffer;
}

void EgoMotionInfoFill(void)
{ 
  static sint8 DDS_publish_VehicleData_status = 0;

  uint64 Tsyn_TimeStamp = 0u;
  // Tsyn_TimeStamp = EthTsyn_time_get();
  EgoMotionInfo.header.stamp.sec = (uint32)(Tsyn_TimeStamp / 1000U);
  EgoMotionInfo.header.stamp.nanosec = (uint32)((Tsyn_TimeStamp % 1000U)*1000);
  EgoMotionInfo.longitudinal_acceleration = MotionInfoMsgData.longitudAccl;
  EgoMotionInfo.lateral_acceleration = MotionInfoMsgData.LateralAccl;
  EgoMotionInfo.vehicle_speed = MotionInfoMsgData.SpdESC;
  EgoMotionInfo.yaw_rate = MotionInfoMsgData.YawRat;
  EgoMotionInfo.steering_wheel_angle = SteeringInfoMsgData.Steering_angle * 0.0174F;
  EgoMotionInfo.brake_pedal_state = BrakingInfoMsgData.BrkPedalRatio;
  // EgoMotionInfo.wipers = Rte_IRead_VehProxyIn_Runnable_FrontWiperStatus_FrontWiperStatus();
  EgoMotionInfo.wipers = (uint8)Float_data_receive(ComConf_ComSignal_SGFrontWiperStatus_261R); 
  DDS_publish_VehicleData_status = DDS_publish_VehicleData(&EgoMotionInfo);
}

void VehProxyIn_printf(void)
{
  uart_printf("ddslongitudinal %0.3f", EgoMotionInfo.longitudinal_acceleration);
  uart_printf("ddslateral %0.3f", EgoMotionInfo.lateral_acceleration);
  uart_printf("ddsvehicle_speed %0.2f", EgoMotionInfo.vehicle_speed);
  uart_printf("ddsyaw_rate %0.3f", EgoMotionInfo.yaw_rate);
  uart_printf("ddssteering_wheel_angle %0.3f", EgoMotionInfo.steering_wheel_angle);
  uart_printf("ddsbrake_pedal_state %0.3f", EgoMotionInfo.brake_pedal_state);
  uart_printf("ddswipers: %d\n", EgoMotionInfo.wipers);

  uart_printf("TrnOnSts: %d", BodyInfoMsgData.TrnOnSts);
  uart_printf("ACC_MainSwitchStatus: %d", ACC_MainSwitchStatus);
  uart_printf("ACC_v_set_status: %d", SwitchSettingMsgData.ACC_v_set_status);
  uart_printf("ACC_v_resume_status: %d", SwitchSettingMsgData.ACC_v_resume_status);
  uart_printf("ACC_v_plus_status: %d", ParamaterSettingMsgData.ACC_v_plus_status);
  uart_printf("ACC_v_minus_status: %d\n", ParamaterSettingMsgData.ACC_v_minus_status);

  if(VehProxyInRunningTime % 1000u == 0u)
  {
    uart_printf("VehProxyInRunningTime %d", VehProxyInRunningTime);
  }
}

/* Model step function */
void VehProxyIn_Runnable(void)
{
  sint32 qY;
  float32 rtb_Gain4;
  float32 rtb_Gain5;
  float32 rtb_Gain6;
  float32 rtb_Gain7;
  float32 rtb_Gain8;
  uint32 tmp;
  uint16 tmp_0;
  uint8 rtb_Compare_gx_tmp;
  boolean rtb_Compare_a;
  boolean rtb_Compare_b2;
  boolean rtb_Compare_k;
  WhlSpdDirct rtb_DataTypeConversion1;

  // VehProxyIn_printf();

  if (VehProxyInRunningTime <= 65500u)
    {
      VehProxyInRunningTime = VehProxyInRunningTime + 20u;
    }
  else
    {
      VehProxyInRunningTime = 0u;
    }

  EgoMotionInfoFill();

  /* Gain: '<Root>/Gain4' incorporates:
   *  Inport: '<Root>/ESCWhlFLSpd'
   */
  rtb_Gain4 = (float32)(0.05625 * (float64)
                        Rte_IRead_VehProxyIn_Runnable_ESCWhlFLSpd_Val());

  /* DataTypeConversion: '<S39>/Data Type Conversion1' incorporates:
   *  DataTypeConversion: '<S36>/Data Type Conversion1'
   *  Inport: '<Root>/FLWheelDriveDirection'
   */
  rtb_DataTypeConversion1 = (WhlSpdDirct)
    Rte_IRead_VehProxyIn_Runnable_FLWheelDriveDirection_Val();

  /* Switch: '<S36>/Switch' incorporates:
   *  Constant: '<S40>/Constant'
   *  RelationalOperator: '<S40>/Compare'
   *  UnaryMinus: '<S36>/Unary Minus'
   */
  if (rtb_DataTypeConversion1 == Forward) {
    rtb_Gain8 = rtb_Gain4;
  } else {
    rtb_Gain8 = -rtb_Gain4;
  }

  /* BusCreator: '<S6>/Bus Creator' incorporates:
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   *  Gain: '<S36>/Gain3'
   *  Switch: '<S36>/Switch'
   */
  MotionInfoMsgData.WhlSpdFL = (float32)(0.2778 * rtb_Gain8);

  /* RelationalOperator: '<S41>/Compare' incorporates:
   *  Constant: '<S41>/Constant'
   */
  rtb_Compare_b2 = (rtb_DataTypeConversion1 == Invalid);

  /* Gain: '<Root>/Gain5' incorporates:
   *  Inport: '<Root>/ESCWhlFRSpd'
   */
  rtb_Gain5 = (float32)(0.05625 * (float64)
                        Rte_IRead_VehProxyIn_Runnable_ESCWhlFRSpd_Val());

  /* DataTypeConversion: '<S39>/Data Type Conversion1' incorporates:
   *  DataTypeConversion: '<S37>/Data Type Conversion1'
   *  Inport: '<Root>/FRWheelDriveDirection'
   */
  rtb_DataTypeConversion1 = (WhlSpdDirct)
    Rte_IRead_VehProxyIn_Runnable_FRWheelDriveDirection_Val();

  /* Switch: '<S37>/Switch' incorporates:
   *  Constant: '<S43>/Constant'
   *  RelationalOperator: '<S43>/Compare'
   *  UnaryMinus: '<S37>/Unary Minus'
   */
  if (rtb_DataTypeConversion1 == Forward) {
    rtb_Gain8 = rtb_Gain5;
  } else {
    rtb_Gain8 = -rtb_Gain5;
  }

  /* BusCreator: '<S6>/Bus Creator' incorporates:
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   *  Gain: '<S37>/Gain3'
   *  Switch: '<S37>/Switch'
   */
  MotionInfoMsgData.WhlSpdFR = (float32)(0.2778 * rtb_Gain8);

  /* RelationalOperator: '<S44>/Compare' incorporates:
   *  Constant: '<S44>/Constant'
   */
  rtb_Compare_k = (rtb_DataTypeConversion1 == Invalid);

  /* Gain: '<Root>/Gain6' incorporates:
   *  Inport: '<Root>/ESCWhlRLSpd'
   */
  rtb_Gain6 = (float32)(0.05625 * (float64)
                        Rte_IRead_VehProxyIn_Runnable_ESCWhlRLSpd_Val());

  /* DataTypeConversion: '<S39>/Data Type Conversion1' incorporates:
   *  DataTypeConversion: '<S38>/Data Type Conversion1'
   *  Inport: '<Root>/RLWheelDriveDirection'
   */
  rtb_DataTypeConversion1 = (WhlSpdDirct)
    Rte_IRead_VehProxyIn_Runnable_RLWheelDriveDirection_Val();

  /* Switch: '<S38>/Switch' incorporates:
   *  Constant: '<S46>/Constant'
   *  RelationalOperator: '<S46>/Compare'
   *  UnaryMinus: '<S38>/Unary Minus'
   */
  if (rtb_DataTypeConversion1 == Forward) {
    rtb_Gain8 = rtb_Gain6;
  } else {
    rtb_Gain8 = -rtb_Gain6;
  }

  /* BusCreator: '<S6>/Bus Creator' incorporates:
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   *  Gain: '<S38>/Gain3'
   *  Switch: '<S38>/Switch'
   */
  MotionInfoMsgData.WhlSpdRL = (float32)(0.2778 * rtb_Gain8);

  /* RelationalOperator: '<S47>/Compare' incorporates:
   *  Constant: '<S47>/Constant'
   */
  rtb_Compare_a = (rtb_DataTypeConversion1 == Invalid);

  /* Gain: '<Root>/Gain7' incorporates:
   *  Inport: '<Root>/ESCWhlRRSpd'
   */
  rtb_Gain7 = (float32)(0.05625 * (float64)
                        Rte_IRead_VehProxyIn_Runnable_ESCWhlRRSpd_Val());

  /* DataTypeConversion: '<S39>/Data Type Conversion1' incorporates:
   *  Inport: '<Root>/RRWheelDriveDirection'
   */
  rtb_DataTypeConversion1 = (WhlSpdDirct)
    Rte_IRead_VehProxyIn_Runnable_RRWheelDriveDirection_Val();

  /* Switch: '<S39>/Switch' incorporates:
   *  Constant: '<S49>/Constant'
   *  RelationalOperator: '<S49>/Compare'
   *  UnaryMinus: '<S39>/Unary Minus'
   */
  if (rtb_DataTypeConversion1 == Forward) {
    rtb_Gain8 = rtb_Gain7;
  } else {
    rtb_Gain8 = -rtb_Gain7;
  }

  /* BusCreator: '<S6>/Bus Creator' incorporates:
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   *  Gain: '<S39>/Gain3'
   *  Switch: '<S39>/Switch'
   */
  MotionInfoMsgData.WhlSpdRR = (float32)(0.2778 * rtb_Gain8);

  /* Gain: '<Root>/Gain8' incorporates:
   *  Inport: '<Root>/VehicleSpd'
   */
  rtb_Gain8 = (float32)(0.05625 * (float64)
                        Rte_IRead_VehProxyIn_Runnable_VehicleSpd_VehicleSpd());

  /* BusCreator: '<S6>/Bus Creator' incorporates:
   *  Constant: '<S42>/Constant'
   *  Constant: '<S45>/Constant'
   *  Constant: '<S48>/Constant'
   *  Constant: '<S50>/Constant'
   *  Constant: '<S51>/Constant'
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   *  DataTypeConversion: '<S36>/Data Type Conversion'
   *  DataTypeConversion: '<S37>/Data Type Conversion'
   *  DataTypeConversion: '<S38>/Data Type Conversion'
   *  DataTypeConversion: '<S39>/Data Type Conversion'
   *  Logic: '<S36>/Logical Operator'
   *  Logic: '<S37>/Logical Operator'
   *  Logic: '<S38>/Logical Operator'
   *  Logic: '<S39>/Logical Operator'
   *  RelationalOperator: '<S42>/Compare'
   *  RelationalOperator: '<S45>/Compare'
   *  RelationalOperator: '<S48>/Compare'
   *  RelationalOperator: '<S50>/Compare'
   *  RelationalOperator: '<S51>/Compare'
   */
  MotionInfoMsgData.WhlSpdFLFlag = (uint8)((!rtb_Compare_b2) && (rtb_Gain4 <=
    500.0F));
  MotionInfoMsgData.WhlSpdFRFlag = (uint8)((!rtb_Compare_k) && (rtb_Gain5 <=
    500.0F));
  MotionInfoMsgData.WhlSpdRLFlag = (uint8)((!rtb_Compare_a) && (rtb_Gain6 <=
    500.0F));
  MotionInfoMsgData.WhlSpdRRFlag = (uint8)((rtb_DataTypeConversion1 != Invalid) &&
    (rtb_Gain7 <= 500.0F));

  /* Gain: '<S6>/Gain1' incorporates:
   *  Gain: '<S6>/Gain3'
   */
  rtb_Gain4 = (float32)(0.2778 * rtb_Gain8);

  /* BusCreator: '<S6>/Bus Creator' incorporates:
   *  Constant: '<S31>/Constant'
   *  Constant: '<S6>/Constant4'
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   *  DataTypeConversion: '<S6>/Data Type Conversion4'
   *  Gain: '<Root>/Gain10'
   *  Gain: '<Root>/Gain11'
   *  Gain: '<Root>/Gain9'
   *  Gain: '<S6>/Gain1'
   *  Gain: '<S6>/Gain2'
   *  Gain: '<S6>/Gain4'
   *  Inport: '<Root>/LateralAcce'
   *  Inport: '<Root>/LongitAcce'
   *  Inport: '<Root>/VehicleSpdValid'
   *  Inport: '<Root>/YawRate'
   *  RelationalOperator: '<S31>/Compare'
   *  Sum: '<Root>/Add4'
   *  Sum: '<Root>/Add5'
   *  Sum: '<Root>/Add6'
   *  Sum: '<S35>/Add3'
   */
  MotionInfoMsgData.SpdDashboard = rtb_Gain4;
  MotionInfoMsgData.SpdESC = rtb_Gain4;
  MotionInfoMsgData.SpdESCFlg = (uint8)(VehProxyIn_ConstB.DataTypeConversion3_e
    - Rte_IRead_VehProxyIn_Runnable_VehicleSpdValid_VehicleSpdValid());
  MotionInfoMsgData.YawRat = -(float32)(((float32)(0.0625f * (float64)
    Rte_IRead_VehProxyIn_Runnable_YawRate_YawRate()) - 128.0f) * 0.0174f);
  // uart_printf("YawRate_rte: %d\n", Rte_IRead_VehProxyIn_Runnable_YawRate_YawRate());
  MotionInfoMsgData.YawRatFlg = VehProxyIn_ConstB.Add3;
  MotionInfoMsgData.longitudAccl = (float32)(0.001f * (float64)
    Rte_IRead_VehProxyIn_Runnable_LongitAcce_LongitAcce()) - 26.752F;
  MotionInfoMsgData.longitudAcclFlg = VehProxyIn_ConstB.Add3_a;
  MotionInfoMsgData.LateralAccl = (float32)((float32)(0.001 * (float64)
    Rte_IRead_VehProxyIn_Runnable_LateralAcce_LateralAcce()) - 26.752F);
  MotionInfoMsgData.LateralAcclFlg = VehProxyIn_ConstB.Add3_m;
  MotionInfoMsgData.StandStillSts = (uint8)(rtb_Gain8 != 0.0F);
  MotionInfoMsgData.StandStillStsFlg = 1U;

  /* RelationalOperator: '<S4>/Compare' incorporates:
   *  Inport: '<Root>/ACC_MainSwitch'
   *  RelationalOperator: '<S5>/Compare'
   */
  rtb_Compare_gx_tmp = Rte_IRead_VehProxyIn_Runnable_ACC_MainSwitch_Val();
  /* RelationalOperator: '<S4>/Compare' incorporates:
   *  Constant: '<S4>/Constant'
   *  Inport: '<Root>/ACC_MainSwitch'
   */
  ACC_MainSwitchStatus = rtb_Compare_gx_tmp;

    if(ACC_MainSwitch_pre != rtb_Compare_gx_tmp)
    {
      ACC_MainSwitch_HoldCount = 4u;
    }

  if( ACC_MainSwitch_HoldCount >0u)
    {
    rtb_Compare_b2 = true;
    }
    else
    {
    rtb_Compare_b2 = false;
    }

  if(ACC_MainSwitch_HoldCount > 0u)
    {
      ACC_MainSwitch_HoldCount --;
    }

  /* BusCreator: '<Root>/Bus Creator' incorporates:
   *  Constant: '<S3>/Constant'
   *  DataStoreWrite: '<Root>/ParamaterSettingMsgData'
   *  DataTypeConversion: '<Root>/Data Type Conversion10'
   *  DataTypeConversion: '<Root>/Data Type Conversion11'
   *  DataTypeConversion: '<Root>/Data Type Conversion12'
   *  DataTypeConversion: '<Root>/Data Type Conversion5'
   *  Inport: '<Root>/ACC_Minus_Set'
   *  Inport: '<Root>/ACC_Plus_Resume'
   *  Inport: '<Root>/ACC_TimeGapSet'
   *  Inport: '<Root>/ACC_time_gap_minus_status'
   *  Inport: '<Root>/ALC_dtlc_style'
   *  Inport: '<Root>/ALC_need_confirm'
   *  RelationalOperator: '<S3>/Compare'
   */
  ParamaterSettingMsgData.ACC_v_set_status = 
    (Rte_IRead_VehProxyIn_Runnable_ACC_Minus_Set_Val() != 0);
  ParamaterSettingMsgData.Spd_dashboard = rtb_Gain8;
  ParamaterSettingMsgData.ACC_v_plus_status =
    (Rte_IRead_VehProxyIn_Runnable_ACC_Plus_Resume_Val() > 0);
  ParamaterSettingMsgData.ACC_v_minus_status =
    (Rte_IRead_VehProxyIn_Runnable_ACC_Minus_Set_Val() != 0);
  ParamaterSettingMsgData.ACC_time_gap_plus_status =
    (Rte_IRead_VehProxyIn_Runnable_ACC_TimeGapSet_Val() != 0);
  ParamaterSettingMsgData.ACC_time_gap_minus_status =
    (Rte_IRead_VehProxyIn_Runnable_ACC_time_gap_minus_status_Val() != 0);
  ParamaterSettingMsgData.ALC_need_confirm =
    (Rte_IRead_VehProxyIn_Runnable_ALC_need_confirm_ALC_need_confirm() != 0);
  ParamaterSettingMsgData.ALC_dtlc_style =
    Rte_IRead_VehProxyIn_Runnable_ALC_dtlc_style_ALC_dtlc_style();

  /* BusCreator: '<Root>/Bus Creator6' incorporates:
   *  Constant: '<S5>/Constant'
   *  DataStoreWrite: '<Root>/SwitchSettingMsgData'
   *  DataTypeConversion: '<Root>/Data Type Conversion6'
   *  DataTypeConversion: '<Root>/Data Type Conversion7'
   *  DataTypeConversion: '<Root>/Data Type Conversion8'
   *  DataTypeConversion: '<Root>/Data Type Conversion9'
   *  Inport: '<Root>/ACC_limit_main_status'
   *  Inport: '<Root>/City_noa_switch_status'
   *  Inport: '<Root>/Highway_noa_switch_status'
   *  Inport: '<Root>/Parking_switch_status'
   *  RelationalOperator: '<S5>/Compare'
   */
  SwitchSettingMsgData.ACC_main_status = rtb_Compare_b2;
  SwitchSettingMsgData.ACC_limit_main_status =
    (Rte_IRead_VehProxyIn_Runnable_ACC_limit_main_status_Val() != 0);
  SwitchSettingMsgData.ACC_v_resume_status = 
    (Rte_IRead_VehProxyIn_Runnable_ACC_Plus_Resume_Val() > 0);
  ACC_v_cancel_status = 
  (uint8)Float_data_receive(ComConf_ComSignal_SGACC_Cancel_291R);
  SwitchSettingMsgData.ACC_v_set_status = 
    (Rte_IRead_VehProxyIn_Runnable_ACC_Minus_Set_Val() != 0);
  SwitchSettingMsgData.Highway_pilot_switch_status = false;
  SwitchSettingMsgData.Highway_noa_switch_status =
    (Rte_IRead_VehProxyIn_Runnable_Highway_noa_switch_status_Val() != 0);
  SwitchSettingMsgData.City_noa_switch_status =
    (Rte_IRead_VehProxyIn_Runnable_City_noa_switch_status_Val() != 0);
  SwitchSettingMsgData.Parking_switch_status =
    (Rte_IRead_VehProxyIn_Runnable_Parking_switch_status_Val() != 0);

  /* MultiPortSwitch: '<S1>/Multiport Switch' incorporates:
   *  Inport: '<Root>/POWER_MODE_PEPS'
   */
  switch (Rte_IRead_VehProxyIn_Runnable_POWER_MODE_PEPS_POWER_MODE_PEPS()) {
   case 1:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant1'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch'
     */
    BodyInfoMsgData.TrnOnSts = PwrMdSts_ACC;
    break;

   case 3:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant6'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch'
     */
    BodyInfoMsgData.TrnOnSts = PwrMdSts_ON;
    break;

   default:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant5'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch'
     */
    BodyInfoMsgData.TrnOnSts = PwrMdSts_OFF;
    break;
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch' */

  /* Switch: '<S1>/Switch1' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S9>/Constant'
   *  Inport: '<Root>/FrontWiperStatus'
   *  Inport: '<Root>/RearWiperStatus'
   *  RelationalOperator: '<S10>/Compare'
   *  RelationalOperator: '<S9>/Compare'
   *  Switch: '<S1>/Switch2'
   */
  if (Rte_IRead_VehProxyIn_Runnable_RearWiperStatus_RearWiperStatus() == 1) {
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant9'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  Switch: '<S1>/Switch1'
     */
    BodyInfoMsgData.FrntWiprSts = WiprSts_Fast;
  } else if (Rte_IRead_VehProxyIn_Runnable_FrontWiperStatus_FrontWiperStatus() ==
             1) {
    /* Switch: '<S1>/Switch2' incorporates:
     *  BusCreator: '<S1>/Bus Creator1'
     *  Constant: '<S1>/Constant10'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  Switch: '<S1>/Switch1'
     */
    BodyInfoMsgData.FrntWiprSts = WiprSts_Slow;
  } else {
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant12'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  Switch: '<S1>/Switch1'
     *  Switch: '<S1>/Switch2'
     *  Switch: '<S1>/Switch3'
     */
    BodyInfoMsgData.FrntWiprSts = WiprSts_Off;
  }

  /* End of Switch: '<S1>/Switch1' */

  /* MultiPortSwitch: '<S1>/Multiport Switch1' incorporates:
   *  Inport: '<Root>/DrvSeatBeltRSt'
   */
  switch (Rte_IRead_VehProxyIn_Runnable_DrvSeatBeltRSt_DrvSeatBeltRSt()) {
   case 1:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant16'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch1'
     */
    BodyInfoMsgData.DrvSeatBelt = DrSts_Failure;
    break;

   case 2:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant14'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch1'
     */
    BodyInfoMsgData.DrvSeatBelt = DrSts_Open;
    break;

   default:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant15'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch1'
     */
    BodyInfoMsgData.DrvSeatBelt = DrSts_Closed;
    break;
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch1' */

  /* MultiPortSwitch: '<S1>/Multiport Switch2' incorporates:
   *  Inport: '<Root>/PassengrSeatBelt'
   */
  switch (Rte_IRead_VehProxyIn_Runnable_PassengrSeatBelt_PassengrSeatBelt()) {
   case 1:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant19'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch2'
     */
    BodyInfoMsgData.PassengrSeatBelt = DrSts_Failure;
    break;

   case 2:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant17'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch2'
     */
    BodyInfoMsgData.PassengrSeatBelt = DrSts_Open;
    break;

   default:
    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant18'
     *  DataStoreWrite: '<Root>/BodyInfoMsgData'
     *  MultiPortSwitch: '<S1>/Multiport Switch2'
     */
    BodyInfoMsgData.PassengrSeatBelt = DrSts_Closed;
    break;
  }

  /* End of MultiPortSwitch: '<S1>/Multiport Switch2' */

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  Constant: '<S1>/Constant13'
   *  Constant: '<S1>/Constant7'
   *  Constant: '<S1>/Constant8'
   *  DataStoreWrite: '<Root>/BodyInfoMsgData'
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DataTypeConversion: '<S1>/Data Type Conversion10'
   *  DataTypeConversion: '<S1>/Data Type Conversion11'
   *  DataTypeConversion: '<S1>/Data Type Conversion12'
   *  DataTypeConversion: '<S1>/Data Type Conversion13'
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   *  DataTypeConversion: '<S1>/Data Type Conversion4'
   *  DataTypeConversion: '<S1>/Data Type Conversion5'
   *  DataTypeConversion: '<S1>/Data Type Conversion6'
   *  DataTypeConversion: '<S1>/Data Type Conversion7'
   *  DataTypeConversion: '<S1>/Data Type Conversion8'
   *  DataTypeConversion: '<S1>/Data Type Conversion9'
   *  Inport: '<Root>/DrFrntLeft'
   *  Inport: '<Root>/DrFrntRgt'
   *  Inport: '<Root>/DrRearLeft'
   *  Inport: '<Root>/DrRearRgt'
   *  Inport: '<Root>/FrntHd'
   *  Inport: '<Root>/Front_Fog_Status'
   *  Inport: '<Root>/HazardStatus'
   *  Inport: '<Root>/HighBeamStatus'
   *  Inport: '<Root>/Left_TurnStatus'
   *  Inport: '<Root>/LowBeamStatus'
   *  Inport: '<Root>/RearMirrorSts'
   *  Inport: '<Root>/RearTrnk'
   *  Inport: '<Root>/Rear_Fog_Status'
   *  Inport: '<Root>/Right_TurnStatus'
   *  Sum: '<S11>/Add3'
   *  Sum: '<S12>/Add3'
   *  Sum: '<S13>/Add3'
   *  Sum: '<S14>/Add3'
   *  Sum: '<S15>/Add3'
   *  Sum: '<S16>/Add3'
   *  Sum: '<S17>/Add3'
   *  Sum: '<S18>/Add3'
   *  Sum: '<S19>/Add3'
   *  Sum: '<S20>/Add3'
   *  Sum: '<S21>/Add3'
   *  Sum: '<S22>/Add3'
   *  Sum: '<S23>/Add3'
   */
  BodyInfoMsgData.DrvSts = DrvMd_Normal;
  BodyInfoMsgData.Drvlocatn = DrvLcatn_Left;
  BodyInfoMsgData.TrnLgtLftSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_h -
     Rte_IRead_VehProxyIn_Runnable_Left_TurnStatus_Left_TurnStatus());
  BodyInfoMsgData.TrnLgtRgtSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_l -
     Rte_IRead_VehProxyIn_Runnable_Right_TurnStatus_Right_TurnStatus());
  BodyInfoMsgData.HarzardLgtSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_c4 -
     Rte_IRead_VehProxyIn_Runnable_HazardStatus_HazardStatus());
  BodyInfoMsgData.BeamLgtHightSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_b -
     Rte_IRead_VehProxyIn_Runnable_HighBeamStatus_HighBeamStatus());
  BodyInfoMsgData.BeamLgtlowSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_m -
     Rte_IRead_VehProxyIn_Runnable_LowBeamStatus_LowBeamStatus());
  BodyInfoMsgData.RearWiprSts = WiprSts_Off;
  BodyInfoMsgData.FrntFogLgtSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_j -
     Rte_IRead_VehProxyIn_Runnable_Front_Fog_Status_Front_Fog_Status());
  BodyInfoMsgData.RearFogLgtSts = (LgtSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_ho -
     Rte_IRead_VehProxyIn_Runnable_Rear_Fog_Status_Rear_Fog_Status());
  BodyInfoMsgData.DrFrntLeft = (DrSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_ok -
     Rte_IRead_VehProxyIn_Runnable_DrFrntLeft_Val());
  BodyInfoMsgData.DrFrntRgt = (DrSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_g -
     Rte_IRead_VehProxyIn_Runnable_DrFrntRgt_Val());
  BodyInfoMsgData.DrRearLeft = (DrSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_p -
     Rte_IRead_VehProxyIn_Runnable_DrRearLeft_Val());
  BodyInfoMsgData.DrRearRgt = (DrSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_c -
     Rte_IRead_VehProxyIn_Runnable_DrRearRgt_Val());
  BodyInfoMsgData.FrntHd = (DrSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_i -
     Rte_IRead_VehProxyIn_Runnable_FrntHd_FrntHd());
  BodyInfoMsgData.RearTrnk = (DrSts)(uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_f -
     Rte_IRead_VehProxyIn_Runnable_RearTrnk_RearTrnk());
  BodyInfoMsgData.RearMirrorSts = (RearMirrorSts)
    Rte_IRead_VehProxyIn_Runnable_RearMirrorSts_RearMirrorSts();

  /* Gain: '<S8>/Gain2' incorporates:
   *  Gain: '<Root>/Gain12'
   *  Inport: '<Root>/VCU_AccPedlPosn'
   */
  rtb_Gain4 = 0.5F * (float32)
    Rte_IRead_VehProxyIn_Runnable_VCU_AccelPdlPos_VCU_AccelPdlPos();

  /* RelationalOperator: '<S54>/Compare' incorporates:
   *  Constant: '<S54>/Constant'
   */
  rtb_Compare_b2 = (rtb_Gain4 > 30.0F);

  /* Switch: '<S52>/Switch2' incorporates:
   *  Constant: '<S52>/Constant3'
   *  RelationalOperator: '<S57>/FixPt Relational Operator'
   *  UnitDelay: '<S52>/Unit Delay'
   *  UnitDelay: '<S57>/Delay Input1'
   *
   * Block description for '<S57>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if (rtb_Compare_b2 != VehProxyIn_ARID_DEF.DelayInput1_DSTATE) {
    tmp_0 = 0U;
    VehProxyIn_ARID_DEF.UnitDelay_DSTATE = 0U;
  } else {
    tmp_0 = VehProxyIn_ARID_DEF.UnitDelay_DSTATE;
  }

  /* Sum: '<S52>/Add' incorporates:
   *  Constant: '<S52>/Constant2'
   *  Switch: '<S52>/Switch2'
   */
  tmp = tmp_0 + 20U;
  if (VehProxyIn_ARID_DEF.UnitDelay_DSTATE + 20U > 65535U) {
    tmp = 65535U;
  }

  /* Switch: '<S52>/Switch2' incorporates:
   *  Sum: '<S52>/Add'
   */
  VehProxyIn_ARID_DEF.UnitDelay_DSTATE = (uint16)tmp;

  /* Sum: '<S52>/Add1' incorporates:
   *  Constant: '<S52>/Constant2'
   *  UnitDelay: '<S52>/Unit Delay'
   */
  qY = (sint32)(VehProxyIn_ARID_DEF.UnitDelay_DSTATE - /*MW:OvSatOk*/ 20U);
  if (VehProxyIn_ARID_DEF.UnitDelay_DSTATE - 20U >
      VehProxyIn_ARID_DEF.UnitDelay_DSTATE) {
    qY = 0;
  }

  /* Switch: '<S52>/Switch' incorporates:
   *  Constant: '<S52>/Constant'
   *  Constant: '<S52>/Constant1'
   */
  if (rtb_Compare_b2) {
    tmp_0 = 2000U;
  } else {
    tmp_0 = 100U;
  }

  /* Switch: '<S52>/Switch3' incorporates:
   *  RelationalOperator: '<S52>/Relational Operator'
   *  Sum: '<S52>/Add1'
   *  Switch: '<S52>/Switch'
   */
  if (qY >= tmp_0) {
    /* Switch: '<S52>/Switch1' incorporates:
     *  Constant: '<S52>/Constant4'
     *  Constant: '<S52>/Constant5'
     *  DataTypeConversion: '<S52>/Data Type Conversion'
     *  UnitDelay: '<S52>/Unit Delay1'
     */
    if (rtb_Compare_b2) {
      VehProxyIn_ARID_DEF.UnitDelay1_DSTATE = (uint8)PedalOvrridSts_Override;
    } else {
      VehProxyIn_ARID_DEF.UnitDelay1_DSTATE = (uint8)PedalOvrridSts_Not_Override;
    }

    /* End of Switch: '<S52>/Switch1' */
  }

  /* End of Switch: '<S52>/Switch3' */

  /* MultiPortSwitch: '<S7>/Multiport Switch' incorporates:
   *  Inport: '<Root>/VCU_ShiftLevelPosition'
   */
  switch
    (Rte_IRead_VehProxyIn_Runnable_VCU_ShiftLevelPosition_VCU_ShiftLevelPosition
     ()) {
   case 1:
    /* BusCreator: '<S7>/Bus Creator3' incorporates:
     *  Constant: '<S7>/Constant1'
     *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
     *  MultiPortSwitch: '<S7>/Multiport Switch'
     */
    PropulsionInfoMsgData.Gear_postion = GearPsnSts_Gear_R;
    break;

   case 2:
    /* BusCreator: '<S7>/Bus Creator3' incorporates:
     *  Constant: '<S7>/Constant2'
     *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
     *  MultiPortSwitch: '<S7>/Multiport Switch'
     */
    PropulsionInfoMsgData.Gear_postion = GearPsnSts_Gear_N;
    break;

   case 3:
    /* BusCreator: '<S7>/Bus Creator3' incorporates:
     *  Constant: '<S7>/Constant3'
     *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
     *  MultiPortSwitch: '<S7>/Multiport Switch'
     */
    PropulsionInfoMsgData.Gear_postion = GearPsnSts_Gear_D;
    break;

   case 5:
    /* BusCreator: '<S7>/Bus Creator3' incorporates:
     *  Constant: '<S7>/Constant'
     *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
     *  MultiPortSwitch: '<S7>/Multiport Switch'
     */
    PropulsionInfoMsgData.Gear_postion = GearPsnSts_Gear_P;
    break;

   default:
    /* BusCreator: '<S7>/Bus Creator3' incorporates:
     *  Constant: '<S7>/Constant4'
     *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
     *  MultiPortSwitch: '<S7>/Multiport Switch'
     */
    PropulsionInfoMsgData.Gear_postion = GearPsnSts_Gear_Unknown;
    break;
  }

  /* End of MultiPortSwitch: '<S7>/Multiport Switch' */

  /* BusCreator: '<S7>/Bus Creator3' incorporates:
   *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
   *  Gain: '<Root>/Gain14'
   *  Inport: '<Root>/ESCTorqReqResp'
   *  Inport: '<Root>/Gear_sys_fault_status'
   *  Inport: '<Root>/VCU_AccelPdlPosV'
   *  Inport: '<Root>/VCU_fault_statuts'
   *  Sum: '<Root>/Add8'
   *  Sum: '<S53>/Add3'
   *  UnitDelay: '<S52>/Unit Delay1'
   */
  PropulsionInfoMsgData.AccelPedal_ratio = rtb_Gain4;
  PropulsionInfoMsgData.AccelPedalRatioAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3 -
     Rte_IRead_VehProxyIn_Runnable_VCU_AccelPdlPosV_VCU_AccelPdlPosV());
  PropulsionInfoMsgData.Accel_override_flag =
    VehProxyIn_ARID_DEF.UnitDelay1_DSTATE;
  PropulsionInfoMsgData.Wheel_torque = (float32)(0.1 * (float64)
    Rte_IRead_VehProxyIn_Runnable_MCU_MotActuToq_MCU_MotActuToq()) - 2000.0F;
  PropulsionInfoMsgData.Gear_sys_fault_status =
    Rte_IRead_VehProxyIn_Runnable_Gear_sys_fault_status_Gear_sys_fault_status();
  PropulsionInfoMsgData.VCU_fault_statuts =
    Rte_IRead_VehProxyIn_Runnable_VCU_fault_statuts_VCU_fault_statuts();

  /* Logic: '<S52>/Logical Operator' incorporates:
   *  Constant: '<S55>/Constant'
   *  Constant: '<S56>/Constant'
   *  DataStoreWrite: '<S52>/pedal_L_override_flag'
   *  RelationalOperator: '<S55>/Compare'
   *  RelationalOperator: '<S56>/Compare'
   */
  pedal_L_override_flag = ((rtb_Gain4 >= 5.0F) && (rtb_Gain4 < 15.0F));

  /* BusCreator: '<S8>/Bus Creator4' incorporates:
   *  Constant: '<S8>/Constant'
   *  Constant: '<S8>/Constant1'
   *  DataStoreWrite: '<Root>/SteeringInfoMsgData'
   *  Gain: '<Root>/Gain'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain3'
   *  Gain: '<S8>/Gain1'
   *  Gain: '<S8>/Gain2'
   *  Inport: '<Root>/DriverInputTrqValue'
   *  Inport: '<Root>/DriverInput_Torque_V'
   *  Inport: '<Root>/EPSFlrSts'
   *  Inport: '<Root>/EPS_CameraCtrlStatus'
   *  Inport: '<Root>/HandDetection'
   *  Inport: '<Root>/StrgWhlAng'
   *  Inport: '<Root>/StrgWhlAngSpd'
   *  Inport: '<Root>/StrgWhlAngV'
   *  Sum: '<Root>/Add'
   *  Sum: '<Root>/Add1'
   *  Sum: '<Root>/Add3'
   *  Sum: '<S58>/Add3'
   *  Sum: '<S59>/Add3'
   */
  SteeringInfoMsgData.Steering_angle = -(float32)(0.1F * (float32)
    Rte_IRead_VehProxyIn_Runnable_StrgWhlAng_StrgWhlAng() - 720.0F);
  SteeringInfoMsgData.Steering_angle_flag =
    Rte_IRead_VehProxyIn_Runnable_StrgWhlAngV_StrgWhlAngV();
  SteeringInfoMsgData.Steering_angle_rate = (float32)((0.5F * (float32)
    Rte_IRead_VehProxyIn_Runnable_StrgWhlAngSpd_StrgWhlAngSpd() - 1016.0F) *
    0.0174);
  SteeringInfoMsgData.Steering_angle_rate_flag = 1U;
  SteeringInfoMsgData.Hand_torque = (float32)(0.01 * (float64)
    Rte_IRead_VehProxyIn_Runnable_DriverInputTrqValue_DriverInputTrqValue()) -
    15.0F;
  DriverInputTrqValue = SteeringInfoMsgData.Hand_torque;
  SteeringInfoMsgData.Hand_torque_flag = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_g3 -
     Rte_IRead_VehProxyIn_Runnable_DriverInput_Torque_V_DriverInput_Torque_V());
  SteeringInfoMsgData.Hand_on_status = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_gp -
     Rte_IRead_VehProxyIn_Runnable_HandDetection_HandDetection());
  SteeringInfoMsgData.Hand_on_status_flag = 1U;
  SteeringInfoMsgData.EPS_fault_statuts =
    Rte_IRead_VehProxyIn_Runnable_EPSFlrSts_EPSFlrSts();
  SteeringInfoMsgData.EPS_active_status =
    Rte_IRead_VehProxyIn_Runnable_EPS_CameraCtrlStatus_EPS_CameraCtrlStatus();
  /* DataTypeConversion: '<S2>/Data Type Conversion' incorporates:
   *  Inport: '<Root>/EPBSts'
   *  Switch: '<S2>/Switch'
   */
  rtb_Compare_gx_tmp = Rte_IRead_VehProxyIn_Runnable_EPBSts_EPBSts();
  /* Switch: '<S2>/Switch' incorporates:
   *  Constant: '<S24>/Constant'
   *  DataTypeConversion: '<S2>/Data Type Conversion'
   *  Inport: '<Root>/EPBSts'
   *  RelationalOperator: '<S24>/Compare'
   *  UnitDelay: '<S2>/Unit Delay'
   */
  if (rtb_Compare_gx_tmp != (sint32)EPBSts_Transition) {
    VehProxyIn_ARID_DEF.UnitDelay_DSTATE_p = rtb_Compare_gx_tmp;
  }

  /* BusCreator: '<S2>/Bus Creator5' incorporates:
   *  Constant: '<S2>/Constant'
   *  DataStoreWrite: '<Root>/BrakingInfoMsgData'
   *  DataTypeConversion: '<S2>/Data Type Conversion1'
   *  DataTypeConversion: '<S2>/Data Type Conversion2'
   *  DataTypeConversion: '<S2>/Data Type Conversion3'
   *  DataTypeConversion: '<S2>/Data Type Conversion4'
   *  Inport: '<Root>/ABAActv'
   *  Inport: '<Root>/ABAAvailable'
   *  Inport: '<Root>/ABPAvailable'
   *  Inport: '<Root>/AEBActive'
   *  Inport: '<Root>/AEBAvailable'
   *  Inport: '<Root>/AWBActive'
   *  Inport: '<Root>/AWBAvailable'
   *  Inport: '<Root>/BrkPedalRatioVld'
   *  Inport: '<Root>/ESC_ABSActive'
   *  Inport: '<Root>/ESPActive'
   *  Inport: '<Root>/ESPFailSts'
   *  Inport: '<Root>/ESPFuncOffSts'
   *  Inport: '<Root>/MastrCylinderPrssrBar'
   *  Inport: '<Root>/PrefillActive'
   *  Inport: '<Root>/VCU_BrkPdlSts'
   *  Inport: '<Root>/VCU_BrkPdlStsV'
   *  Inport: '<Root>/sOutputRodDriver'
   *  Sum: '<S25>/Add3'
   *  Sum: '<S26>/Add3'
   *  Sum: '<S27>/Add3'
   *  Sum: '<S28>/Add3'
   *  Sum: '<S29>/Add3'
   *  Sum: '<S30>/Add3'
   *  UnitDelay: '<S2>/Unit Delay'
   */
  BrakingInfoMsgData.BrkPedalSts =
    Rte_IRead_VehProxyIn_Runnable_VCU_BrkPdlSts_VCU_BrkPdlSts();
  BrakingInfoMsgData.BrkPedalStsAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_n -
     Rte_IRead_VehProxyIn_Runnable_VCU_BrkPdlStsV_VCU_BrkPdlStsV());
  BrakingInfoMsgData.BrkPedalRatio =
    Rte_IRead_VehProxyIn_Runnable_sOutputRodDriver_sOutputRodDriver() * 0.5f;
  BrakingInfoMsgData.BrkPedalRatioAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_ox -
     Rte_IRead_VehProxyIn_Runnable_BrkPedalRatioVld_BrkPedalRatioVld());
  BrakingInfoMsgData.MastrCylinderPrssrBar =
    Rte_IRead_VehProxyIn_Runnable_MastrCylinderPrssrBar_MastrCylinderPrssrBar();
  BrakingInfoMsgData.ESCFaltSts =
    Rte_IRead_VehProxyIn_Runnable_ESPFailSts_ESPFailSts();
  BrakingInfoMsgData.EPBSts = (EPBSts)VehProxyIn_ARID_DEF.UnitDelay_DSTATE_p;
  BrakingInfoMsgData.EPBFaltSts = 0U;
  BrakingInfoMsgData.ABSActv = Rte_IRead_VehProxyIn_Runnable_ABSActive_ABSActive();
  rtb_Compare_gx_tmp = Rte_IRead_VehProxyIn_Runnable_ESPActive_ESPActive();
  BrakingInfoMsgData.TCSActv = rtb_Compare_gx_tmp;
  BrakingInfoMsgData.ESPActv = rtb_Compare_gx_tmp;
  BrakingInfoMsgData.ESPSwtActv =
    (Rte_IRead_VehProxyIn_Runnable_ESPFuncOffSts_ESPFuncOffSts() != 0);
  BrakingInfoMsgData.AEBAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_fg -
     Rte_IRead_VehProxyIn_Runnable_AEBAvailable_AEBAvailable());
  BrakingInfoMsgData.AEBActv = Rte_IRead_VehProxyIn_Runnable_AEBActive_AEBActive
    ();
  BrakingInfoMsgData.ABPAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_bk -
     Rte_IRead_VehProxyIn_Runnable_ABPAvailable_ABPAvailable());
  BrakingInfoMsgData.ABPActv =
    Rte_IRead_VehProxyIn_Runnable_PrefillActive_PrefillActive();
  BrakingInfoMsgData.AWBAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_g3h -
     Rte_IRead_VehProxyIn_Runnable_AWBAvailable_AWBAvailable());
  BrakingInfoMsgData.AWBActv = Rte_IRead_VehProxyIn_Runnable_AWBActive_AWBActive
    ();
  BrakingInfoMsgData.ABAAvailable = (uint8)
    (VehProxyIn_ConstB.DataTypeConversion3_g3s -
     Rte_IRead_VehProxyIn_Runnable_ABAAvailable_ABAAvailable());
  BrakingInfoMsgData.ABAActv = Rte_IRead_VehProxyIn_Runnable_ABAActv_ABAActv();

  /* Outport: '<Root>/MotionInfoMsg' incorporates:
   *  DataStoreWrite: '<Root>/MotionInfoMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_MotionInfoMsg_Val(&MotionInfoMsgData);

  /* Outport: '<Root>/SwitchSettingMsg' incorporates:
   *  DataStoreWrite: '<Root>/SwitchSettingMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_SwitchSettingMsg_Val(&SwitchSettingMsgData);

  /* Outport: '<Root>/ParamaterSettingMsg' incorporates:
   *  DataStoreWrite: '<Root>/ParamaterSettingMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_ParamaterSettingMsg_Val
    (&ParamaterSettingMsgData);

  /* Outport: '<Root>/BodyInfoMsg' incorporates:
   *  DataStoreWrite: '<Root>/BodyInfoMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_BodyInfoMsg_Val(&BodyInfoMsgData);

  /* Outport: '<Root>/PropulsionInfoMsg' incorporates:
   *  DataStoreWrite: '<Root>/PropulsionInfoMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_PropulsionInfoMsg_Val(&PropulsionInfoMsgData);

  /* Outport: '<Root>/SteeringInfoMsg' incorporates:
   *  DataStoreWrite: '<Root>/SteeringInfoMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_SteeringInfoMsg_Val(&SteeringInfoMsgData);

  /* Outport: '<Root>/BrakingInfoMsg' incorporates:
   *  DataStoreWrite: '<Root>/BrakingInfoMsgData'
   */
  Rte_IWrite_VehProxyIn_Runnable_BrakingInfoMsg_Val(&BrakingInfoMsgData);

  /* Update for UnitDelay: '<S57>/Delay Input1'
   *
   * Block description for '<S57>/Delay Input1':
   *
   *  Store in Global RAM
   */
  VehProxyIn_ARID_DEF.DelayInput1_DSTATE = rtb_Compare_b2;
  ACC_MainSwitch_pre = Rte_IRead_VehProxyIn_Runnable_ACC_MainSwitch_Val();
}

/* Model initialize function */
void VehProxyIn_Init(void)
{
  /* Registration code */

  /* states (dwork) */

  /* custom states */
  BodyInfoMsgData = VehProxyIn_rtZBodyInfoMsg;

  /* Start for DataStoreMemory generated from: '<Root>/BodyInfoMsgData' */
  BodyInfoMsgData = VehProxyIn_rtZBodyInfoMsg;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
