/**
  ******************************************************************************
  * @file    PFCInit.h
  * @author  STMicroelectronics - GCSA - IPD Motion Control Team - Shenzhen
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file serves the purpose to initialize the PFC Application
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
#ifndef __PFC_INIT_H
#define __PFC_INIT_H

/* Includes ------------------------------------------------------------------*/


/** @addtogroup STM32_PFC_Library
  * @{
  */

/** @addtogroup PFCInit
  * @{
  */

/** @defgroup PFC_Module_exported_types PFC Module exported types
* @{
*/

/**
  * @brief  It initializes the PFC application. It must be called only after
  *         Motor Control Application initialization (MC_Boot)
  * @param  oMCT CMCT object, motor drive coordinated with PFC
  * @param  oMCT CMCT object, motor drive coordinated with PFC
  * @param  pBusVoltage pointer, if PFC doesn't receive coordinated MC objects
  * @retval none
  */
void PFC_Boot(CMCT oMCT1, CMCT oMCT2, int16_t *pBusVoltage);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __PFC_INIT_H */
/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
