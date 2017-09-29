/**
  ******************************************************************************
  * @file    Speed monitor.c
  * @author  STMicroelectronics - System Lab - MC Team
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file implementes user functions definition
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
/* Pre-compiler coherency check */
#include "MC.h"
#include "Timebase.h"

#include "Parameters conversion.h"
#ifdef DUALDRIVE
#include "Parameters conversion motor 2.h"
#endif

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/*Create the CMCT local reference: CMCT oMCT*/
static CMCT oMCT;
/*Create the CSTM local reference*/
static CSTM oCSTM;
bool flag_INIT_TIM5 = TRUE;

void ARR_TIM5_update(CSPD xCSPD);
void speedmonitor_start(void);

uint8_t number_poles = POLE_PAIR_NUM;

uint16_t Speed_RPM = 0;
uint16_t ARR_TIM5=0;

void speedmonitor_start()
{
 /* Get reference of MCT*/ 
 oMCT = GetMCT(M1);
 
 oCSTM = MCT_GetStateMachine(oMCT);
 
 /*Get the CSPD instance of SpeednPosFdbk Class */
 CSPD xCSPD = MCT_GetSpeednPosSensorMain(oMCT);
   
 /* It controls the status of State Machine*/ 
 switch (STM_GetState(oCSTM))
 {
 case RUN: 
 {   
  if(flag_INIT_TIM5 == TRUE)
  {
  flag_INIT_TIM5 = FALSE;
  
  /* TIM5 clock enabled */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, (FunctionalState)ENABLE);
  
//  GPIO_InitTypeDef GPIO_InitStructure;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65530;
  TIM_TimeBaseStructure.TIM_Prescaler = 17; //2MHz clock
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM5, &TIM_OCInitStructure);

  TIM_ARRPreloadConfig(TIM5, (FunctionalState)ENABLE);
  
  /* TIM3 enable counter */
  TIM_Cmd(TIM5, (FunctionalState)ENABLE); 
  }
 }
 break; 
 case IDLE:
 {
     GPIO_InitTypeDef GPIO_InitStructure;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
   
     /* TIM5 disable counter */
     TIM_Cmd(TIM5, (FunctionalState)DISABLE); 
     flag_INIT_TIM5 = TRUE;
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
     TIM_OC4Init(TIM5, &TIM_OCInitStructure);
 } 
 break;
 } 
}

void ARR_TIM5_update(CSPD xCSPD)        
{
 /* Calcualte the Autoreload value according with the electrical motor speed */  
 Speed_RPM = (uint16_t)(SPD_GetAvrgMecSpeed01Hz(xCSPD)*((float)number_poles/10)); 
 ARR_TIM5 = (u16)(2000000/Speed_RPM); 
 TIM5->ARR = ARR_TIM5;
}

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
