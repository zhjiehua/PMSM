/**
  ******************************************************************************
  * @file    STM32F10x_PFCInit.c
  * @author  STMicroelectronics - GCSA - IPD Motion Control Team - Shenzhen
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file contains interface of PFC Module specific for STM32F1
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
#include "PIRegulatorClass.h"
#include "StateMachineClass.h"
#include "MCTuningClass.h"
#include "MCInterfaceClass.h"

#include "STM32F10x_PFCApplication.h"
#include "Parameters conversion.h"

#if defined(PFC_ENABLED)

#include "PFCParameters.h"
#include "PFCInit.h"

#define TIMCK_NS ((1.0/TIM_CLK)*1000000000.0) /*!< duration of TIM clock period, ns*/
#define MAINSFREQHYS 50.0
#define MAINSFREQHITHR ((MAINSFREQ*10.0) + MAINSFREQHYS)
#define MAINSFREQLOWTHR ((MAINSFREQ*10.0) - MAINSFREQHYS)
#if TIMREMAPPING ==  GPIO_PartialRemap
#undef TIMREMAPPING
#define TIMREMAPPING GPIO_PartialRemap_TIM3
#elif TIMREMAPPING ==  GPIO_FullRemap
#undef TIMREMAPPING
#define TIMREMAPPING GPIO_PartialRemap_TIM3
#endif
#define SWOVERCURRENTTH		20000    /*!< Software OverCurrent threshold, expressed in s16A */
#define NOMINALCURRENTS16A 30000

#if (SYNCFILTER <= 1)
#define SYNCFILTER_IC   0
#elif (SYNCFILTER <= 2)
#define SYNCFILTER_IC   1
#elif (SYNCFILTER <= 6)
#define SYNCFILTER_IC   2
#elif (SYNCFILTER <= 10)
#define SYNCFILTER_IC   3
#elif (SYNCFILTER <= 14)
#define SYNCFILTER_IC   4
#elif (SYNCFILTER <= 20)
#define SYNCFILTER_IC   5
#elif (SYNCFILTER <= 28)
#define SYNCFILTER_IC   6
#elif (SYNCFILTER <= 40)
#define SYNCFILTER_IC   7
#elif (SYNCFILTER <= 56)
#define SYNCFILTER_IC   8
#elif (SYNCFILTER <= 72)
#define SYNCFILTER_IC   9
#elif (SYNCFILTER <= 88)
#define SYNCFILTER_IC   10
#elif (SYNCFILTER <= 112)
#define SYNCFILTER_IC   11
#elif (SYNCFILTER <= 144)
#define SYNCFILTER_IC   12
#elif (SYNCFILTER <= 176)
#define SYNCFILTER_IC   13
#elif (SYNCFILTER <= 224)
#define SYNCFILTER_IC   14
#else
#define SYNCFILTER_IC   15
#endif

#if (ETRFILTER <= 1)
#define ETRFILTER_IC   0
#elif (ETRFILTER <= 2)
#define ETRFILTER_IC   1
#elif (ETRFILTER <= 6)
#define ETRFILTER_IC   2
#elif (ETRFILTER <= 10)
#define ETRFILTER_IC   3
#elif (ETRFILTER <= 14)
#define ETRFILTER_IC   4
#elif (ETRFILTER <= 20)
#define ETRFILTER_IC   5
#elif (ETRFILTER <= 28)
#define ETRFILTER_IC   6
#elif (ETRFILTER <= 40)
#define ETRFILTER_IC   7
#elif (ETRFILTER <= 56)
#define ETRFILTER_IC   8
#elif (ETRFILTER <= 72)
#define ETRFILTER_IC   9
#elif (ETRFILTER <= 88)
#define ETRFILTER_IC   10
#elif (ETRFILTER <= 112)
#define ETRFILTER_IC   11
#elif (ETRFILTER <= 144)
#define ETRFILTER_IC   12
#elif (ETRFILTER <= 176)
#define ETRFILTER_IC   13
#elif (ETRFILTER <= 224)
#define ETRFILTER_IC   14
#else
#define ETRFILTER_IC   15
#endif


PFCParams_t PFCParams =
{
  /* Current reading A/D Conversions initialization --------------------------*/
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
  (uint32_t)TIM_CLK,              /*!< CPU and TIM frequency, MHz*/
  (uint32_t)PWMFREQ,              /*!< PWM frequency expressed in Hertz*/
  (uint16_t)(TIM_CLK/PWMFREQ),		/*!< Timer ARR related to PFC PWM frequency*/
  (uint32_t)TIMREMAPPING,			    /*!< it remaps TIMx I/Os*/
  (uint16_t)PWMPOLARITY,          /*!< PMW output polarity, it must be
                                       TIM_OCPolarity_High or TIM_OCPolarity_Low
										                   according to power device driver */
  (GPIO_TypeDef*)PWMPORT,         /*!< PWM output port (as after re-mapping).
                                       It must be GPIOx x= A, B, ...
										                   It must be associated to TIMx CH1*/
  (uint16_t)PWMPIN,               /*!< PWM output pin (as after re-mapping).
                                       It must be GPIO_Pin_x x= 0, 1, ...
										                   It must be associated to TIMx CH1*/

  (FunctionalState)ETRFAULT,      /*!< It enable/disable the management of
                                       an emergency input instantaneously
                                       stopping PWM generation. It must be
                                       either equal to ENABLE or DISABLE */
  (uint16_t)ETRPOLARITY,          /*!< ETR input polarity, according to
                                       hardware protection polarity:
                                       TIM_ExtTRGPolarity_Inverted,
                                       TIM_ExtTRGPolarity_NonInverted*/
  (EXTITrigger_TypeDef)EXTIPOLARITY, /*!< EXTI input polarity, according to
                                       hardware protection polarity:
                                       EXTI_Trigger_Falling,
                                       EXTI_Trigger_Rising*/
  (uint8_t)FAULTPOLARITY,         /*!< 1 if fault is on rising edge, 0
                                       if fault is on falling edge*/
  (GPIO_TypeDef*)FAULTPORT,       /*!< Fault GPIO input
                                       port (if used, after re-mapping).
                                       It must be GPIOx x= A, B, ...*/
  (uint16_t)FAULTPIN,             /*!< Fault GPIO input pin
                                       (if used, after re-mapping). It must be
                                       GPIO_Pin_x x= 0, 1, ...*/
  (uint8_t)EXTIPORTSOURCE,        /*!< Fault GPIO input port, EXTI configuration*/
  (uint8_t)EXTIPINSOURCE,         /*!< Fault GPIO input pin, EXTI configuration*/
  (uint32_t)EXTILINE,             /*!< EXTI Line*/
  (uint8_t)EXTIIRQN,              /*!< EXTI interrupt no. */
  (uint16_t)ETRFILTER_IC,         /*!< Time filter applied to validate ETR.
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
  (uint16_t)SYNCPOLARITY,         /*!< Syncronization circuit polarity, it must be
                                       TIM_ICPolarity_Rising or TIM_ICPolarity_Falling */
  (GPIO_TypeDef*)SYNCPORT,        /*!< SYNC input port (as after re-mapping).
                                       It must be GPIOx x= A, B, ...
										                   It must be associated to TIMx CH2*/
  (uint16_t)SYNCPIN,              /*!< SYNC input pin (as after re-mapping).
                                       It must be GPIO_Pin_x x= 0, 1, ...
										                   It must be associated to TIMx CH2*/
  (uint16_t)SYNCFILTER_IC,        /*!< Time filter applied to validate SYNC.
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
  (uint8_t)(PWMFREQ/CURRCTRLFREQUENCY),	/*!< Ratio PWM frequency to Current
  				                             control frequency */
  (uint8_t)(SYS_TICK_FREQUENCY/VOLTCTRLFREQUENCY), /*!< Ratio PFC_Scheduler()
                                       frequency to Voltage control frequency */
  (int16_t)(NOMINALCURRENTS16A),  /*!< Output nominal current in s16A */
	{
  		(int16_t)PID_CURR_KP_DEFAULT, /*!< Default Kp gain, used to initialize Kp gain
                                       software variable*/
  		(int16_t)PID_CURR_KI_DEFAULT, /*!< Default Ki gain, used to initialize Ki gain
                                       software variable*/
  		(uint16_t)CURR_KPDIV,       /*!< Kp gain divisor, used in conjuction with
                                       Kp gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  		(uint16_t)CURR_KIDIV,       /*!< Ki gain divisor, used in conjuction with
                                       Ki gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  		(int32_t)S16_MAX * CURR_KIDIV,/*!< Upper limit used to saturate the integral
                                       term given by Ki / Ki divisor * integral of
                                       process variable error */
  		(int32_t)(-S16_MAX) * CURR_KIDIV,/*!< Lower limit used to saturate the integral
                                       term given by Ki / Ki divisor * integral of
                                       process variable error */
  		S16_MAX,                    /*!< Upper limit used to saturate the PI output*/
  		0,                          /*!< Lower limit used to saturate the PI output*/
  (uint16_t)LOG2(CURR_KPDIV),     /*!< Kp gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 because 2^9 = 512 */
  (uint16_t)LOG2(CURR_KIDIV)      /*!< Ki gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 because 2^9 = 512 */
	},
	{
  		(int16_t)PID_VOLT_KP_DEFAULT,/*!< Default Kp gain, used to initialize Kp gain
                                       software variable*/
  		(int16_t)PID_VOLT_KI_DEFAULT,/*!< Default Ki gain, used to initialize Ki gain
                                       software variable*/
  		(uint16_t)VOLT_KPDIV,       /*!< Kp gain divisor, used in conjuction with
                                       Kp gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  		(uint16_t)VOLT_KIDIV,       /*!< Ki gain divisor, used in conjuction with
                                       Ki gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  		(int32_t)NOMINALCURRENTS16A * VOLT_KIDIV,/*!< Upper limit used to saturate the integral
                                       term given by Ki / Ki divisor * integral of
                                       process variable error */
  		(int32_t)(-NOMINALCURRENTS16A) * VOLT_KIDIV,/*!< Lower limit used to saturate the integral
                                       term given by Ki / Ki divisor * integral of
                                       process variable error */
  		(int16_t)NOMINALCURRENTS16A,/*!< Upper limit used to saturate the PI output */
  		(int16_t)(-NOMINALCURRENTS16A),/*!< Lower limit used to saturate the PI output */
  (uint16_t)LOG2(VOLT_KPDIV),         /*!< Kp gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 because 2^9 = 512 */
  (uint16_t)LOG2(VOLT_KIDIV)      /*!< Ki gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 because 2^9 = 512 */
	},
  (int16_t)((STARTUPDURATION*0.001)*(VOLTCTRLFREQUENCY*1.0)), /*!< Default duration of PFC startup
                                       phase, expressed in voltage loops */
  (int16_t)VOLTAGEREFERENCE,      /*!< Default Boost Voltage reference,
                                       expressed in Volts */
  (uint16_t)MAINSFREQLOWTHR,      /*!< Min Mains frequency allowed for PFC
                                       operations, expressed in tenth of Hz */
  (uint16_t)MAINSFREQHITHR,       /*!< Max Mains frequency allowed for PFC
                                       operations, expressed in tenth of Hz */
  (int16_t)VMAINSAMPLLOWTH,       /*!< Min Mains voltage amplitude allowed for
                                       PFC operations, expressed in Volts */
  (int16_t)VMAINSAMPLHITH,        /*!< Max Mains voltage amplitude allowed for
                                       PFC operations, expressed in Volts */
  (int16_t)OUTPUTPOWERACTIVATION, /*!< Output power for PFC activation,
                                       expressed in Watts */
  (int16_t)OUTPUTPOWERDEACTIVATION,/*!< Output power for PFC deactivation,
                                       expressed in Watts */
  (int16_t)(SWOVERVOLTAGETH*10),  /*!< Software OverVoltage threshold,
                                       expressed in Volts */
  (int16_t)SWOVERCURRENTTH,       /*!< Software OverCurrent threshold,
                                       expressed in s16A */
  (uint16_t)(PROPDELAYON/TIMCK_NS),/*!< Latency at PFC switch turn-on, TIM clock cycles*/
  (uint16_t)(PROPDELAYOFF/TIMCK_NS),/*!< Latency at PFC switch turn-off, TIM clock cycles*/
  (uint16_t)(TIM_CLK/(ADC_CLK_MHz*1000000.0)*ISAMPLINGTIMEREAL),   /*!< ADC sampling time, TIM clock cycles*/
  (uint16_t)(TIM_CLK/(ADC_CLK_MHz*1000000.0)*12), /*!< ADC conversion time, TIM clock cycles*/
  (uint16_t)(TIM_CLK/(ADC_CLK_MHz*1000000.0)*3),    /*!< ADC latency time, TIM clock cycles*/
  (uint16_t)(32767.0/(3.3 * VMAINS_PARTITIONING_FACTOR)),  /*!< It is used to convert bus voltage from
                                       u16Volts into real Volts (V).
                                       1 u16Volt = 65536/hConversionFactor Volts
                                       For real sensors hConversionFactor it's
                                       equal to the product between the expected MCU
                                       voltage and the partioning network
                                       attenuation*/
  (uint16_t)((PFCSHUNTRESISTOR*PFCAMPLGAIN*65536.0)/3.3) /*!< it's used to convert AC mains current from
                                       u16Amps into real Amperes (A)*/
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

