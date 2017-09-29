/**
  ******************************************************************************
  * @file    STM32F10x_PFCApplication.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10X_PFC_APPLICATION_H
#define __STM32F10X_PFC_APPLICATION_H

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STM32_PFC_Library
  * @{
  */

/** @addtogroup STM32F10x_PFCApplication
  * @{
  */
  
/** @defgroup PFC_Module_exported_types PFC Module exported types
* @{
*/

/** 
  * @brief  PFC Module parameters definition  
  */
typedef const struct
{
  /* Current reading A/D Conversions initialization --------------------------*/
  uint32_t wADC_Clock_Divider;      /*!< APB2 clock prescaling factor for 
                                         ADC peripheral. It must be RCC_PCLK2_DivX
                                         x = 2, 4, 6, 8 */   
  uint8_t hIChannel;			          /*!< ADC channel used for conversion of 
                                         current. It must be equal to  
                                         ADC_Channel_x x= 0, ..., 15*/
  GPIO_TypeDef* hIPort;           	/*!< GPIO port used by hIChannel. It must 
                                         be equal to GPIOx x= A, B, ...*/
  uint16_t hIPin;                 	/*!< GPIO pin used by hIChannel. It must be 
                                         equal to GPIO_Pin_x x= 0, 1, ...*/ 
  uint8_t b_ISamplingTime;        	/*!< Sampling time used to convert hIChannel. 
                                         It must be equal to ADC_SampleTime_xCycles5 
                                         x= 1, 7, ...*/ 
  uint8_t hVMChannel;             	/*!< ADC channel used for conversion of 
                                         Mains Voltage. It must be equal to  
                                         ADC_Channel_x x= 0, ..., 15*/
  GPIO_TypeDef* hVMPort;          	/*!< GPIO port used by hVMChannel. It must 
                                         be equal to GPIOx x= A, B, ...*/
  uint16_t hVMPin;                	/*!< GPIO pin used by hVMChannel. It must be 
                                         equal to GPIO_Pin_x x= 0, 1, ...*/ 
  uint8_t b_VMSamplingTime;       	/*!< Sampling time used to convert hVMChannel. 
                                         It must be equal to ADC_SampleTime_xCycles5 
                                         x= 1, 7, ...*/ 
  
  /* PWM generation parameters --------------------------------------------------*/
  uint32_t wCPUFreq;                /*!< CPU and TIM frequency, MHz*/ 
  uint32_t wPWMFreq;                /*!< PWM frequency expressed in Hertz*/
  uint16_t hPWMARR;                 /*!< Timer ARR related to PFC PWM frequency*/
  uint32_t wTIMRemapping;           /*!< it remaps TIMx I/Os: GPIO_PartialRemap_TIM3,
                                         GPIO_FullRemap_TIM3,'0u' no remap*/
  uint16_t hPWMPolarity;            /*!< PMW output polarity, it must be 
                                         TIM_OCPolarity_High or TIM_OCPolarity_Low
					                               according to power device driver */
  GPIO_TypeDef* hPWMPort;           /*!< PWM output port (as after re-mapping). 
                                         It must be GPIOx x= A, B, ...
					                               It must be associated to TIM3 CH1*/
  uint16_t hPWMPin;                 /*!< PWM output pin (as after re-mapping).
                                         It must be GPIO_Pin_x x= 0, 1, ...
					                               It must be associated to TIM3 CH1*/ 
  FunctionalState ETRFault;         /*!< It enable/disable the management of 
                                         emergency input instantaneously through ETR, 
                                         stopping PWM generation. It must be 
                                         either equal to ENABLE or DISABLE.
                                         When disabled, functionality is to be
                                         configured via EXTI line: in that case,
                                         parameters hEXTIPinSource, hEXTIPortSource
                                         must be defined*/  
  uint16_t hETRPolarity;            /*!< ETR input polarity, according to 
                                         hardware protection polarity:
                                         TIM_ExtTRGPolarity_Inverted,
                                         TIM_ExtTRGPolarity_NonInverted*/
  EXTITrigger_TypeDef EXTIPolarity; /*!< EXTI input polarity, according to 
                                         hardware protection polarity:
                                         EXTI_Trigger_Falling,
                                         EXTI_Trigger_Rising*/
  uint8_t bFaultPolarity;           /*!< 1 if fault is on rising edge, 0
                                         if fault is on falling edge*/
  GPIO_TypeDef* hFaultPort;         /*!< Fault GPIO input 
                                         port (ETR/EXTI). 
                                         It must be GPIOx x= A, B, ...*/
  uint16_t hFaultPin;               /*!< Fault GPIO input pin 
                                         (ETR/EXTI). It must be 
                                         GPIO_Pin_x x= 0, 1, ...*/
  uint8_t hEXTIPortSource;          /*!< Fault GPIO input port, EXTI configuration:
                                         GPIO_PortSourceGPIOx, x=A,B,...*/
  uint8_t hEXTIPinSource;           /*!< Fault GPIO input pin, EXTI configuration:
                                         GPIO_PinSourcex, x=0,1,...*/
  uint32_t wEXTILine;               /*!< EXTI Line*/
  uint8_t bEXTIIRQn;                /*!< EXTI interrupt no. */
  uint16_t hETRFilter;              /*!< Time filter applied to validate ETR.
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
  uint16_t hSYNCPolarity;           /*!< Syncronization circuit polarity, it must be 
                                         TIM_ICPolarity_Rising or TIM_ICPolarity_Falling */
  GPIO_TypeDef* hSYNCPort;          /*!< SYNC input port (as after re-mapping). 
                                         It must be GPIOx x= A, B, ...
										                     It must be associated to TIMx CH2*/
  uint16_t hSYNCPin;                /*!< SYNC input pin (as after re-mapping).
                                         It must be GPIO_Pin_x x= 0, 1, ...
										                     It must be associated to TIMx CH2*/ 
  uint16_t hSYNCFilter;             /*!< Time filter applied to validate SYNC.
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
  uint8_t bCurrentLoopRate;         /*!< Ratio PWM frequency to Current
  				                               control frequency */
  uint8_t bVoltageLoopRate;	        /*!< Ratio PFC_Scheduler()
                                         frequency to Voltage control frequency */
  int16_t hNominalCurrent;          /*!< Output nominal current in s16A */
  PIParams_t CurrentLoopPIParams;   /*!< It contains the parameters of the PI
                                         object necessary for PLL 
                                         implementation */
  PIParams_t VoltageLoopPIParams;   /*!< It contains the parameters of the PI
                                         object necessary for PLL 
                                         implementation */
  int16_t hStartUpDuration;         /*!< Default duration of PFC startup 
                                         phase, expressed in voltage loops */
  int16_t hVoltageReference;        /*!< Default Boost Voltage reference,
                                         expressed in Volts */
  uint16_t hMainsFreqLowTh;         /*!< Min Mains frequency allowed for PFC
                                         operations, expressed in tenth of Hz */
  uint16_t hMainsFreqHiTh;          /*!< Max Mains frequency allowed for PFC
                                         operations, expressed in tenth of Hz */
  int16_t VMainsAmplLowTh;          /*!< Min Mains voltage amplitude allowed for
                                         PFC operations, expressed in Volts */
  int16_t VMainsAmplHiTh;           /*!< Max Mains voltage amplitude allowed for
                                         PFC operations, expressed in Volts */
  int16_t OutputPowerActivation;    /*!< Output power for PFC activation,
                                         expressed in Watts */
  int16_t OutputPowerDeActivation;  /*!< Output power for PFC deactivation,
                                         expressed in Watts */
  int16_t SWOverVoltageTh;          /*!< Software OverVoltage threshold,
                                         expressed in Volts */
  int16_t SWOverCurrentTh;          /*!< Software OverCurrent threshold,
                                         expressed in s16A */
  uint16_t hPropDelayOnTimCk;       /*!< Latency at PFC switch turn-on, TIM clock cycles*/
  uint16_t hPropDelayOffTimCk;      /*!< Latency at PFC switch turn-off, TIM clock cycles*/
  uint16_t hADCSamplingTimeTimCk;   /*!< ADC sampling time, TIM clock cycles*/
  uint16_t hADCConversionTimeTimCk; /*!< ADC conversion time, TIM clock cycles*/
  uint16_t hADCLatencyTimeTimCk;    /*!< ADC latency time, TIM clock cycles*/  
  uint16_t hMainsConversionFactor;  /*!< It is used to convert AC MAINS voltage from 
                                         u16Volts into real Volts (V). 
                                         1 u16Volt = 65536/hConversionFactor Volts
                                         For real sensors hConversionFactor it's 
                                         equal to the product between the expected MCU
                                         voltage and the voltage sensing network 
                                         attenuation */
  uint16_t hCurrentConversionFactor;/*!< it's used to convert AC mains current from
                                         u16Amps into real Amperes (A)*/
}PFCParams_t, *pPFCParams_t;
  
/**
* @}
*/

/** @defgroup PFC_Module_exported_methods PFC Module exported methods
  * @{
  */
/**
  * @brief  It initializes the PFC module. It must be called once only after 
  *         Motor Control Application initialization (MC Boot)
  * @param  pPFCParams parameters of the PFC drive
  * @param  oMCT CMCT object, motor drive coordinated with PFC
  * @param  oMCT CMCT object, motor drive coordinated with PFC
  * @param  pBusVoltage pointer, if PFC doesn't receive coordinated MC objects
  * @retval none
  */
void PFC_Init(pPFCParams_t pPFCParams, CMCT oMCT1, CMCT oMCT2, int16_t *pBusVoltage);


  
/**
  * @}
  */

/**
  * @}
  */

#endif /* __PFC_APPLICATION_H */
/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
