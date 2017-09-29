/**
  ******************************************************************************
  * @file    Potentiometer.c 
  * @author  STMicroelectronics - System Lab - MC Team
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file shows how to change the motor target speed using a potentiometer
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

/*********************** DEFINE for USER STATE MACHINE  ***********************/
#define US_RESET        0x00 
#define US_POSITIVE_RUN 0x01
#define US_STOP        0x02
#define COUNT_MAX_SEC  3
#define STOP_DURATION_SEC 2
#define COUNT_MAX (COUNT_MAX_SEC * USER_TIMEBASE_FREQUENCY_HZ)
#define STOP_DURATION  (STOP_DURATION_SEC * USER_TIMEBASE_FREQUENCY_HZ)
#define USER_TIMEBASE_FREQUENCY_HZ        10
#define USER_TIMEBASE_OCCURENCE_TICKS  (SYS_TICK_FREQUENCY/USER_TIMEBASE_FREQUENCY_HZ)-1u

/* variables ---------------------------------------------------------*/
void potentiometer_start(void);

static CMCI oMCI;                                     
static uint8_t User_State = US_RESET;
bool cmd_status = FALSE;
static uint16_t UserCnt = 0;

uint16_t potentiometer_value=0;
uint16_t speed_max_valueRPM = MOTOR_MAX_SPEED_RPM;    //Maximum value for speed reference from Workbench 
uint16_t speed_min_valueRPM = 2000;            //Set the minimum value for speed reference 
uint16_t speed_firstramp_duration = 100;   //Set the duration for first ramp  

/*This is the main function to use in the main.c in order to start the current example */
void potentiometer_start()
{

 oMCI = GetMCI(M1);
 
 GPIO_InitTypeDef GPIO_InitStructure;
#if defined(P_NUCLEO_IHM001)
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
#elif defined(STEVALIFN003V1) ||  defined(P_NUCLEO_IHM001)
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif      

 if (TB_UserTimebaseHasElapsed())
 {
  /* User defined code */
  switch (User_State)
  {
   case US_RESET:
    {
          /* Next state */
          /* This command sets what will be the first speed ramp after the 
          MCI_StartMotor command. It requires as first parameter the oMCI[0], as 
          second parameter the target mechanical speed in thenth of Hz and as
          third parameter the speed ramp duration in milliseconds. */
          MCI_ExecSpeedRamp(oMCI, (speed_max_valueRPM/3)/6, speed_firstramp_duration);
          
          /* This is a user command used to start the motor. The speed ramp shall be
          pre programmed before the command.*/
          cmd_status = MCI_StartMotor(oMCI);
          
          /* It verifies if the command  "MCI_StartMotor" is successfully executed 
          otherwise it tries to restart the procedure */
          if(cmd_status==FALSE)    
           {
            User_State = US_RESET;                       // Command NOT executed
           }
          else User_State = US_POSITIVE_RUN;           // Command executed
 
          UserCnt = 0;
    }
    break;  
   case US_POSITIVE_RUN:
    {
      /*control point for regular conversion request pending*/
      if(MC_RegularConvState() == UDRC_STATE_IDLE) 
      /*define the regular channel for ADC (PC4) with a sampling time */
        
      #if defined(P_NUCLEO_IHM001)
        MC_RequestRegularConv(ADC_Channel_12,ADC_SampleTime_601Cycles5);  
      #elif defined(STEVALIFN003V1) 
        MC_RequestRegularConv(ADC_Channel_9,ADC_SampleTime_71Cycles5);          
      #else
        MC_RequestRegularConv(ADC_Channel_14,ADC_SampleTime_71Cycles5);  
      #endif             
      
      /*in case of End Of Conversion (EOC) the variable defined below contains 
      the last ADC value for PC4 (potentiometer->MCU pin) */
      else if(MC_RegularConvState() == UDRC_STATE_EOC) 
            potentiometer_value = MC_GetRegularConv();
       
      /* It counts the time for each new speed value assignment  */
      if (UserCnt < COUNT_MAX) 
          {
            UserCnt++;
          }
          else
          {
            UserCnt=0;
            /* In case of low RPM value < Minimum Speed (speed_min_value) the
            motor is forced to stop */            
            if(((potentiometer_value+1)*speed_max_valueRPM / 65535) <= speed_min_valueRPM) 
             { 
               User_State = US_STOP;
               UserCnt=0;
             }
            else
             { 
             /* It changes the motor speed reference according with the potentiometer
             value acquired by ADC on channel PC4 - the duration of the ramp is fixed
             at 100 milliseconds */         
             MCI_ExecSpeedRamp(oMCI, ((potentiometer_value+1)*speed_max_valueRPM / 65535)/6, speed_firstramp_duration);  
             }
          }   
    }
    break;
   case US_STOP:
    {
       /* This is a user command to stop the motor */
       MCI_StopMotor(oMCI);
        
       /* After the time "STOP_DURATION" the motor will be restarted */
       if (UserCnt >= STOP_DURATION)
          {
            /* Next state */ 
            
            User_State = US_RESET;
            UserCnt = 0;
          }
          else
          {
            UserCnt++;
          }
    }
    break;  
  }
  TB_SetUserTimebaseTime(USER_TIMEBASE_OCCURENCE_TICKS);
 }
}

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
