/**
  ******************************************************************************
  * @file    STM32F30x_PFCApplication.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F30X__PFC_APPLICATION_H
#define __STM32F30X__PFC_APPLICATION_H

/* Includes ------------------------------------------------------------------*/
/** @addtogroup STM32F10x_PMSM_MC_Library
  * @{
  */

/** @addtogroup Active Digital PFC
  * @{
  */
  
/** @defgroup PFC_Module_exported_types PFC Module exported types
* @{
*/

typedef const struct
{
  uint32_t wSelection;         /*!< Internal comparator used for protection. 
				                            It must be
                                    COMP_Selection_COMPx x = 1,2,3,4,5,6,7.*/
  uint8_t bInvertingInput_MODE;/*!< COMPx inverting input mode. It must be either 
                                    equal to EXT_MODE or INT_MODE. */
  uint32_t wInvertingInput;    /*!< COMPx inverting input pin. It must be one of
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
  GPIO_TypeDef* hInvertingInput_GPIO_PORT;
                               /*!< COMPx inverting input GPIO port as defined in 
                                    wInvertingInput (Just if 
                                    bInvertingInput_MODE is EXT_MODE).
                                    It must be GPIOx x= A, B, ...*/
  uint16_t hInvertingInput_GPIO_PIN;
                               /*!< COMPx inverting input GPIO pin as defined in 
                                    wInvertingInput (Just if 
                                    bInvertingInput_MODE is EXT_MODE). 
                                    It must be GPIO_Pin_x x= 0, 1, ...*/
  uint32_t wNonInvertingInput;
                               /*!< COMPx non inverting input. It must be one of
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
  GPIO_TypeDef* hNonInvertingInput_GPIO_PORT;
                               /*!< COMPx non inverting input GPIO port as 
                                    defined in wNonInvertingInput.
                                    It must be GPIOx x= A, B, ...*/
  uint16_t hNonInvertingInput_GPIO_PIN;
                               /*!< COMPx non inverting input GPIO pin as defined
                                    in wNonInvertingInput. 
                                    It must be GPIO_Pin_x x= 0, 1, ...*/
  uint8_t bOutput_MODE;
                               /*!< COMPx output. It must be either 
                                    equal to EXT_MODE or INT_MODE. */
  uint32_t wOutput;            /*!< COMPx output selection. It must be one of
                                    the following:
                                    COMP_Output_TIM1BKIN,
                                    COMP_Output_TIM1BKIN2,
                                    COMP_Output_TIM8BKIN,
                                    COMP_Output_TIM8BKIN2,
                                    COMP_Output_TIM1BKIN2_TIM8BKIN2.*/
  GPIO_TypeDef* hOutput_GPIO_PORT;
                               /*!< COMPx output GPIO port.
                                    It must be GPIOx x= A, B, ...*/
  uint16_t hOutput_GPIO_PIN;
                               /*!< COMPx output GPIO pin as defined. 
                                    It must be GPIO_Pin_x x= 0, 1, ...*/
  uint8_t bOutput_GPIO_AF;     /*!< COMPx output alternate functions setting.
                                    It must be one of the GPIO_AF_x (x=0,1, ...) 
                                    according to the defined GPIO port and pin.*/
  uint32_t wOutputPol;
                               /*!< COMPx output polarity. It must be one of
                                    the following:
                                    COMP_OutputPol_NonInverted,
                                    COMP_OutputPol_Inverted.*/
  uint32_t wMode;              /*!< COMPx mode it is used to define both
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
} PFC_F30XCOMPParams_t, *PFC_pF30XCOMPParams_t;

typedef const struct
{
/* First OPAMP settings ------------------------------------------------------*/
  uint32_t wOPAMP_Selection;          /*!< First OPAMP selection. It must be 
                                           either equal to 
                                           OPAMP_Selection_OPAMP1 or
                                           OPAMP_Selection_OPAMP3.*/
  uint8_t bOPAMP_InvertingInput_MODE; /*!< First OPAMP inverting input mode.
                                           It must be either equal to EXT_MODE
                                           or INT_MODE.*/
  uint32_t wOPAMP_InvertingInput;     /*!< First OPAMP inverting input pin.     
                                           
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
  GPIO_TypeDef* hOPAMP_InvertingInput_GPIO_PORT; 
                                      /*!< First OPAMP inverting input GPIO port
                                           as defined in wOPAMP_InvertingInput.
                                           It must be GPIOx x= A, B, ... if 
                                           bOPAMP_InvertingInput_MODE is 
                                           EXT_MODE, otherwise can be dummy.*/
  uint16_t hOPAMP_InvertingInput_GPIO_PIN;
                                      /*!< First OPAMP inverting input GPIO pin
                                           as defined in wOPAMP_InvertingInput.
                                           It must be GPIO_Pin_x x= 0, 1, ... if 
                                           bOPAMP_InvertingInput_MODE is 
                                           EXT_MODE, otherwise can be dummy.*/
  uint32_t wOPAMP_NonInvertingInput; 
                                      /*!< First OPAMP non inverting input first
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
  GPIO_TypeDef* hOPAMP_NonInvertingInput_GPIO_PORT; 
                                      /*!< First OPAMP non inverting input GPIO 
                                           port as defined in 
                                           wOPAMP_NonInvertingInput_PHA. 
                                           It must be GPIOx x= A, B, ...*/
  uint16_t hOPAMP_NonInvertingInput_GPIO_PIN;
                                      /*!< First OPAMP non inverting input GPIO
                                           pin as defined in 
                                           wOPAMP_NonInvertingInput_PHA. 
                                           It must be GPIO_Pin_x x= 0, 1, ...*/

  GPIO_TypeDef* hOPAMP_Output_GPIO_PORT; 
                                      /*!< First OPAMP output GPIO port.
                                           It must be GPIOx x= A, B, ...
                                           Note: Output pin is PA2 for OPAMP1,
                                           PB1 for OPAMP3.*/
  uint16_t hOPAMP_Output_GPIO_PIN;
                                      /*!< First OPAMP output GPIO pin.
                                           It must be GPIO_Pin_x x= 0, 1, ...
                                           Note: Output pin is PA2 for OPAMP1,
                                           PB1 for OPAMP3.*/
/* Common settings -----------------------------------------------------------*/
  uint32_t wOPAMP_PGAGain;            /*!< It defines the OPAMP PGA gains.
					                                 It must be one of the following:
                                           OPAMP_OPAMP_PGAGain_2,
                                           OPAMP_OPAMP_PGAGain_4,
                                           OPAMP_OPAMP_PGAGain_8,
                                           OPAMP_OPAMP_PGAGain_16.
                                           This value is taken in account
                                           just if wOPAMPx_InvertingInput is
                                           equal to OPAMPx_InvertingInput_PGA*/
  uint32_t OPAMP_PGAConnect;          /*!< It defines the OPAMP connection
                                           with an external filter when PGA
                                           is enabled.
                                           It must be one of the following:
                                           OPAMP_PGAConnect_No,
                                           OPAMP_PGAConnect_IO1,
                                           OPAMP_PGAConnect_IO2.
                                           See reference manual RM0316.
                                           This value is taken in account
                                           just if wOPAMPx_InvertingInput is
                                           equal to OPAMPx_InvertingInput_PGA*/
} PFC_R3_4_F30XOPAMPParams_t, *PFC_pR3_4_F30XOPAMPParams_t;

/** 
  * @brief  PFC Module parameters definition  
  */
typedef const struct
{
  /* Current reading A/D Conversions initialization --------------------------*/
  ADC_TypeDef* ADCx;                /*!< ADC peripheral to be used*/
  bool ADCInitialized;              /*!< TRUE if ADC2 has been already
                                         initialized (3shunt or ICS configurations),
                                         FALSE otherwise (1shunt configuration)*/
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
  uint16_t hPWMPolarity;            /*!< PMW output polarity, it must be 
                                         TIM_OCPolarity_High or TIM_OCPolarity_Low
					                               according to power device driver */
  GPIO_TypeDef* hPWMPort;           /*!< PWM output port (as after re-mapping). 
                                         It must be GPIOx x= A, B, ...
					                               It must be associated to TIM3 CH1*/
  uint16_t hPWMPin;                 /*!< PWM output pin (as after re-mapping).
                                         It must be GPIO_Pin_x x= 0, 1, ...
					                               It must be associated to TIM3 CH1*/
  uint8_t bPWMAF;                   /*!< PFC PWM alternate function output port. 
                                         It must be GPIO_AF_x x= 0, 1, .. according to the GPIO port and Pin */
  FunctionalState ETRFault;         /*!< It enable/disable the management of 
                                         emergency input instantaneously through ETR, 
                                         stopping PWM generation. It must be 
                                         either equal to ENABLE or DISABLE.
                                         When disabled, functionality is to be
                                         configured via EXTI line: in that case,
                                         parameters hEXTIPinSource, hEXTIPortSource
                                         must be defined*/  
  uint16_t bBKINFilter;             /*!< ETR input polarity, according to 
                                         hardware protection polarity:
                                         TIM_ExtTRGPolarity_Inverted,
                                         TIM_ExtTRGPolarity_NonInverted*/
  uint8_t hBKINPolarity;            /*!< 1 if fault is on rising edge, 0
                                         if fault is on falling edge*/
  GPIO_TypeDef* hBKINPort;          /*!< Fault GPIO input 
                                         port (ETR/EXTI). 
                                         It must be GPIOx x= A, B, ...*/
  uint16_t hBKINPin;                /*!< Fault GPIO input pin 
                                         (ETR/EXTI). It must be 
                                         GPIO_Pin_x x= 0, 1, ...*/
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
  uint8_t bSYNCAF;                  /*!< SYNC input port (as after re-mapping). 
                                         It must be GPIO_AF_x x= 0, 1, ...
										                     It must be associated to TIM4 CH2*/
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
  uint16_t hNominalCurrent;          /*!< Output nominal current in u16A */
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
  
  PFC_pF30XCOMPParams_t pOCP_PFC_COMPParams;
  uint16_t hDAC_OCP_Threshold;
  PFC_pR3_4_F30XOPAMPParams_t pOPAMPParams;
  
} PFCParams_t, *pPFCParams_t;
  
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

/**
  * @}
  */

#endif 
/*__STM32F30X__PFC_APPLICATION_H*/
/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
