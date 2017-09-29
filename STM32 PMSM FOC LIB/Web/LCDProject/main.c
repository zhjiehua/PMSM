/**
  ******************************************************************************
  * @file    main.c 
  * @author  STMicroelectronics - System Lab - MC Team
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   User interface main program body
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
#include "LCD_Interface.h"
#include "LCDIExportedFunctions.h"

// Setup the exported functions see LCDIExportedFunctions.h enum.
void* const exportedFunctions[EF_LCDI_NUMBERS] =
{
  (void*)(&LCDI_Init),
  (void*)(&LCDI_UpdateMeasured),
  (void*)(&LCDI_UpdateAll),
  (void*)(&LCDI_Polling)
};

#define HardFault_Handler 0xABCDEF12

extern void main_x(void);

int main (void)
{
  /* Don't run application from here. Just flash the code and use MC project. */
	main_x();
	return (0);
}

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
