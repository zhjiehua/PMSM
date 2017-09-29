/**
  ******************************************************************************
  * @file    PFCApplication.h
  * @author  STMicroelectronics - GCSA - IPD Motion Control Team - Shenzhen
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file contains interface of PFC Module
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
#ifndef __PFC_APPLICATION_H
#define __PFC_APPLICATION_H

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STM32_PFC_Library
  * @{
  */

/** @addtogroup PFCApplication
  * @{
  */

/** @defgroup PFC_Module_exported_types PFC Module exported types
* @{
*/



/**
  * @brief  PFC medium/low frequency task scheduler; to be called with fixed
  *         periodicity, equal to PFC medium frequency task rate (as set in
  *         PFCParameters.h)
  * @param  none
  * @retval none
  */
void PFC_Scheduler(void);

/**
  * @brief  It returns the State Machine object.
  * \n\link StateMachine_class_exported_methodsT Methods listed here can be applied \endlink
  * @param  none
  * @retval CSTM, instance of StateMachine Class
  */
CSTM PFC_GetStateMachine(void);

/**
  * @brief  It returns the PFC Enable/disable status.
  * \n\link StateMachine_class_exported_methodsT Methods listed here can be applied \endlink
  * @param  none
  * @retval bool, TRUE PFC is enabled, FALSE PFC is disabled
  */
bool PFC_GetState(void);

/**
  * @brief  This is a user command used to indicate that the user has seen the
  *         error condition. If is possible, the command is executed
  *         instantaneously otherwise the command is discarded. User must take
  *         care of this possibility by checking the return value.
  * @param  none
  * @retval bool It returns TRUE if the command is successfully executed
  *         otherwise it return FALSE.
  */
bool PFC_FaultAcknowledged(void);

/**
  * @brief  This is a user command to be used to arrange PFC enabling; activation
  *         is automatically done when conditions occur. Actual state (PFC
  *         running e.g.) is to be checked through related state machine
  * @param  none
  * @retval bool It returns TRUE if the command is successfully executed
  *         otherwise it return FALSE.
  */
bool PFC_Enable(void);

/**
  * @brief  This is a user command to be used to arrange PFC disabling / deactivation
  * @param  none
  * @retval bool It returns TRUE if the command is successfully executed
  *         otherwise it return FALSE.
  */
bool PFC_Disable(void);

/**
  * @brief  Set the DC boost voltage reference to be maintained by PFC
  * @param  hVoltRef DC bus voltage, expressed in Volts (>0)
  * @retval none
  */
void PFC_SetBoostVoltageReference(int16_t hVoltRef);

/**
  * @brief  Set the duration of the PFC startup phase; during this time, DC bus
  *         voltage is boosted up to reference
  * @param  hStartUpDuration PFC Startup duration, expressed in milliseconds (>0)
  * @retval none
  */
void PFC_SetStartUpDuration(int16_t hStartUpDuration);

/**
  * @brief  It returns the current control loop PI object
  * \n\link PI_Regulator_Class_Exported_MethodsT Methods listed here can be applied \endlink
  * @param  none
  * @retval CPI, instance of PIRegulator Class
  */
CPI PFC_GetCurrentLoopPI(void);

/**
  * @brief  It returns the voltage control loop PI object
  * \n\link PI_Regulator_Class_Exported_MethodsT Methods listed here can be applied \endlink
  * @param  none
  * @retval CPI, instance of PIRegulator Class
  */
CPI PFC_GetVoltageLoopPI(void);

/**
  * @brief  It returns latest measured Mains Voltage
  * @param  none
  * @retval uint16_t Latest measured Mains Voltage expressed in Volt (0-to-pk)
  */
int16_t PFC_GetMainsVoltage(void);

/**
  * @brief  It returns latest measured Mains frequency
  * @param  none
  * @retval uint16_t Latest measured Mains frequency expressed in tenth of Hz
  */
int16_t PFC_GetMainsFrequency(void);

/**
  * @brief  Get the DC boost voltage reference to be maintained by PFC
  * @param  none
  * @retval int16_t DC bus voltage, expressed in Volts
  */
int16_t PFC_GetBoostVoltageReference(void);

/**
  * @brief  Get the duration of the PFC startup phase; during this time, DC bus
  *         voltage is boosted up to reference
  * @param  none
  * @retval hStartUpDuration PFC Startup duration, expressed in milliseconds
  */
int16_t PFC_GetStartUpDuration(void);

/**
  * @brief  It returns latest averaged Vbus measurement expressed in Volts
  * @param  none
  * @retval uint16_t Latest averaged Vbus measurement in Volts
  */
int16_t PFC_GetAvBusVoltage_V(void);

/**
  * @brief  It is called to move the state macchine to the HW fault state.
  * @param  none
  * @retval none
  */
void PFC_OCP_Processing(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __PFC_APPLICATION_H */
/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
