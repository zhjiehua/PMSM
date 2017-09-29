/**
  ******************************************************************************
  * @file    STM32F30x_PFCInit.c
  * @author  STMicroelectronics - GCSA - IPD Motion Control Team - Shenzhen
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file contains interface of PFC Module specific for STM32F3
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MC_type.h"
#include "PWMnCurrFdbkClass.h"
#include "PIRegulatorClass.h"
#include "StateMachineClass.h"
#include "MCTuningClass.h"
#include "MCInterfaceClass.h"

#include "STM32F30x_PFCApplication.h"
#include "Parameters conversion.h"

#if defined(PFC_ENABLED)

#include "PFCParameters.h"
#include "PFCInit.h"

#define TIMCK_NS ((1.0/TIM_CLK)*1000000000.0) /*!< duration of TIM clock period, ns*/

#undef NONE
#undef EXT_MODE
#undef INT_MODE
#define NONE		0x00
#define EXT_MODE 	0x01
#define INT_MODE 	0x02

#if (PFC_USE_INTERNAL_OPAMP == ENABLE)
  #define PFC_OPAMP_PARAMS &PFC_OpAmpParams
#else
  #define PFC_OPAMP_PARAMS MC_NULL
  #warning "PFC EXT OPAMP"
#endif

#if (PFC_BKIN_MODE == INT_MODE)
  #define OCP_PFC1_PARAMS  &OCP_PFC_params1
#else
  #define OCP_PFC1_PARAMS MC_NULL
  #warning "PFC EXT COMP or NONE"
#endif

#undef NONE
#undef EXT_MODE
#undef INT_MODE
#define NONE		((uint8_t)(0x00))
#define EXT_MODE 	((uint8_t)(0x01))
#define INT_MODE 	((uint8_t)(0x02))

/**
  * @brief  Internal COMP parameters PFC 1 - F30x - OCP
  */
F30XCOMPParams_t OCP_PFC_params1 =
{
  OCP_PFC_SELECTION,          /*!< Internal comparator used for protection.
                                   It must be
                                   COMP_Selection_COMPx x = 1,2,3,4,5,6,7.*/
  OCP_PFC_INVERTINGINPUT_MODE,/*!< COMPx inverting input mode. It must be either
                                   equal to EXT_MODE or INT_MODE. */
  OCP_PFC_INVERTINGINPUT,     /*!< COMPx inverting input pin. It must be one of
                                   the following:
                                   COMP1_InvertingInput_PA0,
                                   COMP2_InvertingInput_PA2,
                                   COMP3_InvertingInput_PD15,
                                   COMP3_InvertingInput_PB12,
                                   COMP4_InvertingInput_PE8,
                                   COMP4_InvertingInput_PB2,
                                   COMP5_InvertingInput_PD13,
                                   COMP5_InvertingInput_PB10,
                                   COMP6_InvertingInput_PD10,
                                   COMP6_InvertingInput_PB15 if
                                   bInvertingInput_MODE is EXT_MODE or
                                   COMPX_InvertingInput_DAC1,
                                   COMPX_InvertingInput_DAC2,
                                   COMPX_InvertingInput_VREF,
                                   COMPX_InvertingInput_VREF_1_4,
                                   COMPX_InvertingInput_VREF_1_2,
                                   COMPX_InvertingInput_VREF_3_4 if
                                   bInvertingInput_MODE is INT_MODE.
                                   If bInvertingInput_MODE is EXT_MODE, the
                                   only available options are related to the
                                   selected COMP in wSelection.*/
  OCP_PFC_INVERTINGINPUT_GPIO_PORT,/*!< COMPx inverting input GPIO port as defined in
                                   wInvertingInput (Just if
                                   bInvertingInput_MODE is EXT_MODE).
                                   It must be GPIOx x= A, B, ...*/
  OCP_PFC_INVERTINGINPUT_GPIO_PIN,/*!< COMPx inverting input GPIO pin as defined in
                                   wInvertingInput (Just if
                                   bInvertingInput_MODE is EXT_MODE).
                                   It must be GPIO_Pin_x x= 0, 1, ...*/
  OCP_PFC_NONINVERTINGINPUT,  /*!< COMPx non inverting input. It must be one of
                                   the following:
                                   COMP1_NonInvertingInput_PA1,
                                   COMP2_NonInvertingInput_PA3,
                                   COMP2_NonInvertingInput_PA7,
                                   COMP3_NonInvertingInput_PB14,
                                   COMP3_NonInvertingInput_PD14,
                                   COMP4_NonInvertingInput_PB0,
                                   COMP4_NonInvertingInput_PE7,
                                   COMP5_NonInvertingInput_PB13,
                                   COMP5_NonInvertingInput_PD12,
                                   COMP6_NonInvertingInput_PB11,
                                   COMP6_NonInvertingInput_PD11,
                                   COMP7_NonInvertingInput_PC1,
                                   COMP7_NonInvertingInput_PA0.
                                   The only available options are related to the
                                   selected COMP in wSelection.*/
  OCP_PFC_NONINVERTINGINPUT_GPIO_PORT, /*!< COMPx non inverting input GPIO port as
                                   defined in wNonInvertingInput.
                                   It must be GPIOx x= A, B, ...*/
  OCP_PFC_NONINVERTINGINPUT_GPIO_PIN,  /*!< COMPx non inverting input GPIO pin as defined
                                   in wNonInvertingInput.
                                   It must be GPIO_Pin_x x= 0, 1, ...*/
  OCP_PFC_OUTPUT_MODE,        /*!< COMPx output. It must be either
                                   equal to EXT_MODE or INT_MODE. */
  OCP_PFC_OUTPUT,             /*!< COMPx output selection. It must be one of
                                   the following:
                                   COMP_Output_TIM1BKIN,
                                   COMP_Output_TIM1BKIN2,
                                   COMP_Output_TIM8BKIN,
                                   COMP_Output_TIM8BKIN2,
                                   COMP_Output_TIM1BKIN2_TIM8BKIN2.*/
  OCP_PFC_OUTPUT_GPIO_PORT,   /*!< COMPx output GPIO port.
                                   It must be GPIOx x= A, B, ...*/
  OCP_PFC_OUTPUT_GPIO_PIN,    /*!< COMPx output GPIO pin as defined.
                                   It must be GPIO_Pin_x x= 0, 1, ...*/
  OCP_PFC_OUTPUT_GPIO_AF,     /*!< COMPx output alternate functions setting.
                                   It must be one of the GPIO_AF_x (x=0,1, ...)
                                   according to the defined GPIO port and pin.*/
  OCP_PFC_OUTPUTPOL,          /*!< COMPx output polarity. It must be one of
                                   the following:
                                   COMP_OutputPol_NonInverted,
                                   COMP_OutputPol_Inverted.*/
  OCP_PFC_FILTER,             /*!< COMPx mode it is used to define both
                                   the speed (analog filter) and
                                   the power consumption of the internal
                                   comparator. It must be one of the
                                   following:
                                   COMP_Mode_HighSpeed,
                                   COMP_Mode_MediumSpeed,
                                   COMP_Mode_LowPower,
                                   COMP_Mode_UltraLowPower.
                                   More speed means less filter and more
                                   consumption.*/

};

PFC_R3_4_F30XOPAMPParams_t PFC_OpAmpParams =
{
  /* Internal OPAMP1 settings --------------------------------------------------*/
  PFC_OPAMP_SELECTION,		            /*!< First OPAMP selection. It must be
                                           either equal to
                                           OPAMP_Selection_OPAMP1 or
                                           OPAMP_Selection_OPAMP3.*/
  PFC_OPAMP_INVERTINGINPUT_MODE,      /*!< First OPAMP inverting input mode.
                                           It must be either equal to EXT_MODE
                                           or INT_MODE.*/
  PFC_OPAMP_INVERTINGINPUT,           /*!< First OPAMP inverting input pin.

                                           If wOPAMP_Selection is
                                           OPAMP_Selection_OPAMP1 it
                                           must be one of the following:
                                           OPAMP1_InvertingInput_PC5 or
                                           OPAMP1_InvertingInput_PA3 if the
                                           bOPAMP_InvertingInput_MODE is
                                           EXT_MODE or
                                           OPAMP1_InvertingInput_PGA or
                                           OPAMP1_InvertingInput_FOLLOWER if the
                                           bOPAMP_InvertingInput_MODE is
                                           INT_MODE.

                                           If wOPAMP_Selection is
                                           OPAMP_Selection_OPAMP3 it
                                           must be one of the following:
                                           OPAMP3_InvertingInput_PB10 or
                                           OPAMP3_InvertingInput_PB2 if the
                                           bOPAMP_InvertingInput_MODE is
                                           EXT_MODE or
                                           OPAMP3_InvertingInput_PGA or
                                           OPAMP3_InvertingInput_FOLLOWER if the
                                           bOPAMP_InvertingInput_MODE is
                                           INT_MODE.*/
  PFC_OPAMP_INVERTINGINPUT_GPIO_PORT, /*!< First OPAMP inverting input GPIO port
                                           as defined in wOPAMP_InvertingInput.
                                           It must be GPIOx x= A, B, ... if
                                           bOPAMP_InvertingInput_MODE is
                                           EXT_MODE, otherwise can be dummy.*/
  PFC_OPAMP_INVERTINGINPUT_GPIO_PIN,  /*!< First OPAMP inverting input GPIO pin
                                           as defined in wOPAMP_InvertingInput.
                                           It must be GPIO_Pin_x x= 0, 1, ... if
                                           bOPAMP_InvertingInput_MODE is
                                           EXT_MODE, otherwise can be dummy.*/
  PFC_OPAMP_NONINVERTINGINPUT,        /*!< First OPAMP non inverting input first
                                           selection.

                                           If wOPAMP_Selection is
                                           OPAMP_Selection_OPAMP1 it
                                           must be one of the following:
                                           OPAMP1_NonInvertingInput_PA7,
                                           OPAMP1_NonInvertingInput_PA5,
                                           OPAMP1_NonInvertingInput_PA3,
                                           OPAMP1_NonInvertingInput_PA1.

                                           If wOPAMP_Selection is
                                           OPAMP_Selection_OPAMP3 it
                                           must be one of the following:
                                           OPAMP3_NonInvertingInput_PB13,
                                           OPAMP3_NonInvertingInput_PA5,
                                           OPAMP3_NonInvertingInput_PA1,
                                           OPAMP3_NonInvertingInput_PB0.*/
  PFC_OPAMP_NONINVERTINGINPUT_GPIO_PORT,
                                      /*!< First OPAMP non inverting input GPIO
                                           port as defined in
                                           wOPAMP_NonInvertingInput_PHA.
                                           It must be GPIOx x= A, B, ...*/
  PFC_OPAMP_NONINVERTINGINPUT_GPIO_PIN,
                                      /*!< First OPAMP non inverting input GPIO
                                           pin as defined in
                                           wOPAMP_NonInvertingInput_PHA.
                                           It must be GPIO_Pin_x x= 0, 1, ...*/

  PFC_OPAMP_OUT_GPIO_PORT,
                                      /*!< First OPAMP output GPIO port.
                                           It must be GPIOx x= A, B, ...
                                           Note: Output pin is PA2 for OPAMP1,
                                           PB1 for OPAMP3.*/
  PFC_OPAMP_OUT_GPIO_PIN,
                                      /*!< First OPAMP output GPIO pin.
                                           It must be GPIO_Pin_x x= 0, 1, ...
                                           Note: Output pin is PA2 for OPAMP1,
                                           PB1 for OPAMP3.*/
/* Common settings -----------------------------------------------------------*/
  PFC_OPAMP_PGAGAIN,                  /*!< It defines the OPAMP PGA gains.
	                                         It must be one of the following:
                                           OPAMP_OPAMP_PGAGain_2,
                                           OPAMP_OPAMP_PGAGain_4,
                                           OPAMP_OPAMP_PGAGain_8,
                                           OPAMP_OPAMP_PGAGain_16.
                                           This value is taken in account
                                           just if wOPAMPx_InvertingInput is
                                           equal to OPAMP2_InvertingInput_PGA*/
  PFC_OPAMP_PGACONNECT                    /*!< It defines the OPAMP connection
                                           with an external filter when PGA
                                           is enabled.
                                           It must be one of the following:
                                           OPAMP_PGAConnect_No,
                                           OPAMP_PGAConnect_IO1,
                                           OPAMP_PGAConnect_IO2.
                                           See reference manual RM0316.
                                           This value is taken in account
                                           just if wOPAMPx_InvertingInput is
                                           equal to OPAMP2_InvertingInput_PGA*/

};


PFCParams_t PFCParams =
{
  /* Current reading A/D Conversions initialization --------------------------*/
  PFC_ADC,                        /*!< ADC peripheral to be used*/
  (bool)ADCINITIALIZED,           /*!< TRUE if ADC2 has been already
                                       initialized (3shunt or ICS configurations),
                                       FALSE otherwise (1shunt configuration)*/
  ADC_CLOCK_DIVIDER,              /*!< APB2 clock prescaling factor for
                                       ADC peripheral. It must be RCC_PCLK2_DivX
                                       x = 2, 4, 6, 8 */
  (uint8_t)ICHANNEL,              /*!< ADC channel used for conversion of
                                       current. It must be equal to
                                       ADC_Channel_x x= 0, ..., 15*/
  (GPIO_TypeDef*)IPORT,           /*!< GPIO port used by hIChannel. It must
                                       be equal to GPIOx x= A, B, ...*/
  (uint16_t)IPIN,                 /*!< GPIO pin used by hIChannel. It must be
                                       equal to GPIO_Pin_x x= 0, 1, ...*/
  (uint8_t)ISAMPLINGTIME,         /*!< Sampling time used to convert hIChannel.
                                       It must be equal to ADC_SampleTime_xCycles5
                                       x= 1, 7, ...*/

  (uint8_t)VMCHANNEL,             /*!< ADC channel used for conversion of
                                       Mains Voltage. It must be equal to
                                       ADC_Channel_x x= 0, ..., 15*/
  (GPIO_TypeDef*)VMPORT,          /*!< GPIO port used by hVMChannel. It must
                                       be equal to GPIOx x= A, B, ...*/
  (uint16_t)VMPIN,                /*!< GPIO pin used by hVMChannel. It must be
                                       equal to GPIO_Pin_x x= 0, 1, ...*/
  (uint8_t)VMSAMPLINGTIME,        /*!< Sampling time used to convert hVMChannel.
                                       It must be equal to ADC_SampleTime_xCycles5
                                       x= 1, 7, ...*/

  /* PWM generation parameters --------------------------------------------------*/
  (uint32_t)TIM_CLK,                  /*!< CPU and TIM frequency, MHz*/
  (uint32_t)PWMFREQ,                  /*!< PWM frequency expressed in Hertz*/
  (uint16_t)(TIM_CLK/PWMFREQ),		    /*!< Timer ARR related to PFC PWM frequency*/
  (uint16_t)PWMPOLARITY,              /*!< PMW output polarity, it must be
                                           TIM_OCPolarity_High or TIM_OCPolarity_Low
										                       according to power device driver */
  (GPIO_TypeDef*)PWMPORT,             /*!< PWM output port (as after re-mapping).
                                           It must be GPIOx x= A, B, ...
										                       It must be associated to TIMx CH1*/
  (uint16_t)PWMPIN,                   /*!< PWM output pin (as after re-mapping).
                                           It must be GPIO_Pin_x x= 0, 1, ...
										                       It must be associated to TIMx CH1*/

  (uint8_t)PWM_AF,                    /*!< PFC PWM alternate function output port.
                                           It must be GPIO_AF_x x= 0, 1, .. according to the GPIO port and Pin */
  (FunctionalState)ETRFAULT,          /*!< It enable/disable the management of
                                           an emergency input instantaneously
                                           stopping PWM generation. It must be
                                           either equal to ENABLE or DISABLE */
  (uint16_t)BKINFILTER,               /*!< ETR input polarity, according to
                                           hardware protection polarity:
                                           TIM_ExtTRGPolarity_Inverted,
                                           TIM_ExtTRGPolarity_NonInverted*/
  (uint8_t)BKINPOLARITY,              /*!< 1 if fault is on rising edge, 0
                                           if fault is on falling edge*/
  (GPIO_TypeDef*)BKINPORT,            /*!< Fault GPIO input
                                           port (if used, after re-mapping).
                                           It must be GPIOx x= A, B, ...*/
  (uint16_t)BKINPIN,                  /*!< Fault GPIO input pin
                                           (if used, after re-mapping). It must be
                                           GPIO_Pin_x x= 0, 1, ...*/
  (uint16_t)ETRFILTER,                /*!< Time filter applied to validate ETR.
                                           This value defines the frequency used to sample
                                           ETR input and the length of the digital
                                           filter applied. The digital filter is made of an
                                           event counter in which N events are needed to
                                           validate a transition on the input.
                                           0000: No filter, sampling is done at fCK_INT.
                                           0001: fSAMPLING=fCK_INT , N=2.
                                           0010: fSAMPLING=fCK_INT , N=4.
                                           0011: fSAMPLING=fCK_INT , N=8.
                                           0100: fSAMPLING=fCK_INT/2, N=6.
                                           0101: fSAMPLING=fCK_INT/2, N=8.
                                           0110: fSAMPLING=fCK_INT/4, N=6.
                                           0111: fSAMPLING=fCK_INT/4, N=8.
                                           1000: fSAMPLING=fCK_INT/8, N=6.
                                           1001: fSAMPLING=fCK_INT/8, N=8.
                                           1010: fSAMPLING=fCK_INT/16, N=5.
                                           1011: fSAMPLING=fCK_INT/16, N=6.
                                           1100: fSAMPLING=fCK_INT/16, N=8.
                                           1101: fSAMPLING=fCK_INT/32, N=5.
                                           1110: fSAMPLING=fCK_INT/32, N=6.
                                           1111: fSAMPLING=fCK_INT/32, N=8 */

  /* VMAINS syncronization parameters ----------------------------------------*/
  (uint16_t)SYNCPOLARITY,             /*!< Syncronization circuit polarity, it must be
                                           TIM_ICPolarity_Rising or TIM_ICPolarity_Falling */
  (GPIO_TypeDef*)SYNCPORT,            /*!< SYNC input port (as after re-mapping).
                                           It must be GPIOx x= A, B, ...
										                       It must be associated to TIMx CH2*/
  (uint16_t)SYNCPIN,                  /*!< SYNC input pin (as after re-mapping).
                                           It must be GPIO_Pin_x x= 0, 1, ...
										                       It must be associated to TIMx CH2*/
  (uint8_t)SYNC_AF,                   /*!< SYNC input port (as after re-mapping).
                                           It must be GPIO_AF_x x= 0, 1, ...
										                       It must be associated to TIM4 CH2*/
  (uint16_t)SYNCFILTER,               /*!< Time filter applied to validate SYNC.
                                           This value defines the frequency used to sample
                                           SYNC input and the length of the digital
                                           filter applied. The digital filter is made of an
                                           event counter in which N events are needed to
                                           validate a transition on the input.
                                           0000: No filter, sampling is done at fCK_INT.
                                           0001: fSAMPLING=fCK_INT , N=2.
                                           0010: fSAMPLING=fCK_INT , N=4.
                                           0011: fSAMPLING=fCK_INT , N=8.
                                           0100: fSAMPLING=fCK_INT/2, N=6.
                                           0101: fSAMPLING=fCK_INT/2, N=8.
                                           0110: fSAMPLING=fCK_INT/4, N=6.
                                           0111: fSAMPLING=fCK_INT/4, N=8.
                                           1000: fSAMPLING=fCK_INT/8, N=6.
                                           1001: fSAMPLING=fCK_INT/8, N=8.
                                           1010: fSAMPLING=fCK_INT/16, N=5.
                                           1011: fSAMPLING=fCK_INT/16, N=6.
                                           1100: fSAMPLING=fCK_INT/16, N=8.
                                           1101: fSAMPLING=fCK_INT/32, N=5.
                                           1110: fSAMPLING=fCK_INT/32, N=6.
                                           1111: fSAMPLING=fCK_INT/32, N=8 */

  /* PFC Control Parameters --------------------------------------------------*/
  (uint8_t)(PWMFREQ/CURRCTRLFREQUENCY),/*!< Ratio PWM frequency to Current
  				                                 control frequency */
  (uint8_t)(SYS_TICK_FREQUENCY/VOLTCTRLFREQUENCY), /*!< Ratio PFC_Scheduler()
                                           frequency to Voltage control frequency */
  (uint16_t)(NOMINALCURRENT),           /*!< Output nominal current in u16A */
	{
  		(int16_t)PID_CURR_KP_DEFAULT,    /*!< Default Kp gain, used to initialize Kp gain
                                            software variable*/
  		(int16_t)PID_CURR_KI_DEFAULT,    /*!< Default Ki gain, used to initialize Ki gain
                                            software variable*/
  		(uint16_t)CURR_KPDIV,            /*!< Kp gain divisor, used in conjuction with
                                            Kp gain allows obtaining fractional values.
                                            If FULL_MISRA_C_COMPLIANCY is not defined
                                            the divisor is implemented through
                                            algebrical right shifts to speed up PI
                                            execution. Only in this case this parameter
                                            specifies the number of right shifts to be
                                            executed */
  		(uint16_t)CURR_KIDIV,            /*!< Ki gain divisor, used in conjuction with
                                            Ki gain allows obtaining fractional values.
                                            If FULL_MISRA_C_COMPLIANCY is not defined
                                            the divisor is implemented through
                                            algebrical right shifts to speed up PI
                                            execution. Only in this case this parameter
                                            specifies the number of right shifts to be
                                            executed */
  		(int32_t)S16_MAX * CURR_KIDIV,   /*!< Upper limit used to saturate the integral
                                            term given by Ki / Ki divisor * integral of
                                            process variable error */
  		(int32_t)(-S16_MAX) * CURR_KIDIV,/*!< Lower limit used to saturate the integral
                                            term given by Ki / Ki divisor * integral of
                                            process variable error */
  		S16_MAX,                         /*!< Upper limit used to saturate the PI output */
  		0,                               /*!< Lower limit used to saturate the PI output */
      (uint16_t)LOG2(CURR_KPDIV),      /*!< Kp gain divisor expressed as power of 2.
                                            E.g. if gain divisor is 512 the value
                                            must be 9 because 2^9 = 512 */
      (uint16_t)LOG2(CURR_KIDIV)       /*!< Ki gain divisor expressed as power of 2.
                                            E.g. if gain divisor is 512 the value
                                            must be 9 because 2^9 = 512 */
	},
	{
  		(int16_t)PID_VOLT_KP_DEFAULT,    /*!< Default Kp gain, used to initialize Kp gain
                                            software variable*/
  		(int16_t)PID_VOLT_KI_DEFAULT,    /*!< Default Ki gain, used to initialize Ki gain
                                            software variable*/
  		(uint16_t)VOLT_KPDIV,            /*!< Kp gain divisor, used in conjuction with
                                            Kp gain allows obtaining fractional values.
                                            If FULL_MISRA_C_COMPLIANCY is not defined
                                            the divisor is implemented through
                                            algebrical right shifts to speed up PI
                                            execution. Only in this case this parameter
                                            specifies the number of right shifts to be
                                            executed */
  		(uint16_t)VOLT_KIDIV,            /*!< Ki gain divisor, used in conjuction with
                                            Ki gain allows obtaining fractional values.
                                            If FULL_MISRA_C_COMPLIANCY is not defined
                                            the divisor is implemented through
                                            algebrical right shifts to speed up PI
                                            execution. Only in this case this parameter
                                            specifies the number of right shifts to be
                                            executed */
  		(int32_t)(NOMINALCURRENT/2) * VOLT_KIDIV,/*!< Upper limit used to saturate the integral
                                            term given by Ki / Ki divisor * integral of
                                            process variable error */
  		(int32_t)(-(NOMINALCURRENT/2)) * VOLT_KIDIV,/*!< Lower limit used to saturate the integral
                                            term given by Ki / Ki divisor * integral of
                                            process variable error */
  		(int16_t)(NOMINALCURRENT/2),         /*!< Upper limit used to saturate the PI output */
  		(int16_t)(-(NOMINALCURRENT/2)),      /*!< Lower limit used to saturate the PI output */
      (uint16_t)LOG2(VOLT_KPDIV),      /*!< Kp gain divisor expressed as power of 2.
                                            E.g. if gain divisor is 512 the value
                                            must be 9 because 2^9 = 512 */
  (uint16_t)LOG2(VOLT_KIDIV)           /*!< Ki gain divisor expressed as power of 2.
                                            E.g. if gain divisor is 512 the value
                                            must be 9 because 2^9 = 512 */
	},
  (int16_t)((STARTUPDURATION*0.001)*(VOLTCTRLFREQUENCY*1.0)), /*!< Default duration of PFC startup
                                           phase, expressed in voltage loops */
  (int16_t)VOLTAGEREFERENCE,          /*!< Default Boost Voltage reference,
                                           expressed in Volts */
  (uint16_t)MAINSFREQLOWTH,           /*!< Min Mains frequency allowed for PFC
                                           operations, expressed in tenth of Hz */
  (uint16_t)MAINSFREQHITH,            /*!< Max Mains frequency allowed for PFC
                                           operations, expressed in tenth of Hz */
  (int16_t)VMAINSAMPLLOWTH,           /*!< Min Mains voltage amplitude allowed for
                                           PFC operations, expressed in Volts */
  (int16_t)VMAINSAMPLHITH,            /*!< Max Mains voltage amplitude allowed for
                                           PFC operations, expressed in Volts */
  (int16_t)OUTPUTPOWERACTIVATION,     /*!< Output power for PFC activation,
                                           expressed in Watts */
  (int16_t)OUTPUTPOWERDEACTIVATION,   /*!< Output power for PFC deactivation,
                                           expressed in Watts */
  (int16_t)SWOVERVOLTAGETH,           /*!< Software OverVoltage threshold,
                                           expressed in Volts */
  (int16_t)SWOVERCURRENTTH,           /*!< Software OverCurrent threshold,
                                           expressed in s16A */
  (uint16_t)(PROPDELAYON/TIMCK_NS),   /*!< Latency at PFC switch turn-on, TIM clock cycles*/
  (uint16_t)(PROPDELAYOFF/TIMCK_NS),  /*!< Latency at PFC switch turn-off, TIM clock cycles*/
  (uint16_t)(TIM_CLK/(ADC_CLK_MHz*1000000.0)*ISAMPLINGTIMEREAL),   /*!< ADC sampling time, TIM clock cycles*/
  (uint16_t)(TIM_CLK/(ADC_CLK_MHz*1000000.0)*12), /*!< ADC conversion time, TIM clock cycles*/
  (uint16_t)(TIM_CLK/(ADC_CLK_MHz*1000000.0)*3),    /*!< ADC latency time, TIM clock cycles*/
  (uint16_t)(65535.0/(3.3 * VMAINS_PARTITIONING_FACTOR)), /*!< It is used to convert bus voltage from
                                           u16Volts into real Volts (V).
                                           1 u16Volt = 65536/hConversionFactor Volts
                                           For real sensors hConversionFactor it's
                                           equal to the product between the expected MCU
                                           voltage and the partioning network
                                           attenuation*/
  (uint16_t)((PFCSHUNTRESISTOR*PFCAMPLGAIN*65536.0)/3.3), /*!< it's used to convert AC mains current from
                                           u16Amps into real Amperes (A)*/
  (PFC_pF30XCOMPParams_t) (OCP_PFC1_PARAMS),
  (uint16_t) (OCP_PFC_REF),
  (PFC_pR3_4_F30XOPAMPParams_t) (PFC_OPAMP_PARAMS)
};


/**
  * @brief  It initializes the PFC application. It must be called only after
  *         Motor Control Application initialization (MC_Boot)
  * @param  oMCT CMCT object, motor drive coordinated with PFC
  * @param  oMCT CMCT object, motor drive coordinated with PFC
  * @param  pBusVoltage pointer, if PFC doesn't receive coordinated MC objects
  * @retval none
  */
void PFC_Boot(CMCT oMCT1, CMCT oMCT2, int16_t *pBusVoltage)
{
  PFC_Init(&PFCParams,oMCT1,oMCT2,pBusVoltage);
}

#endif

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
