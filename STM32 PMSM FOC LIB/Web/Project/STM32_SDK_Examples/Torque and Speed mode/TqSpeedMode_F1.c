/**
  ******************************************************************************
  * @file    TqSpeedMode.c 
  * @author  STMicroelectronics - System Lab - MC Team
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   This file shows how to start the motor with different ramp profile
  *****************************************************************************
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
#include "SpeednPosFdbkClass.h"

#include "Parameters conversion.h"
#ifdef DUALDRIVE
#include "Parameters conversion motor 2.h"
#endif

/*********************** DEFINE for USER STATE MACHINE  ***********************/
#define US_RESET        0x00 
#define US_POSITIVE_RUN 0x01
#define US_STOP         0x02
#define US_RAMP         0x03
#define US_SPEED        0x04

#define COUNT_MAX_SEC  5
#define STOP_DURATION_SEC 10
#define COUNT_MAX (COUNT_MAX_SEC * USER_TIMEBASE_FREQUENCY_HZ)
#define STOP_DURATION  (STOP_DURATION_SEC * USER_TIMEBASE_FREQUENCY_HZ)
#define USER_TIMEBASE_FREQUENCY_HZ        10
#define USER_TIMEBASE_OCCURENCE_TICKS  (SYS_TICK_FREQUENCY/USER_TIMEBASE_FREQUENCY_HZ)-1u

void TqSpeedMode_start(void);

/* variables ---------------------------------------------------------*/
/*Create the CMCI local reference: CMCI oMCI*/
static CMCI oMCI;     
/*Create the CMCT local reference: CMCT oMCT*/
static CMCT oMCT;

static uint8_t User_State = US_RESET;
bool cmd_status = FALSE;
static uint16_t UserCnt = 0;

int16_t value_Speed_RPM = 0;
uint16_t torque_value = 1750;       //Set the first value for the speed ramp 
uint16_t torque_duration = 100;   //Set the duration for first ramp  
uint16_t speed_threshold = 4000;      //Set the second value for the speed ramp 
uint16_t speed_target = 8000;      //Set the second value for the speed ramp 
uint16_t speed_duration = 500;  //Set the duration for second ramp  

/*This is the main function to use in the main.c in order to start the current example */
void TqSpeedMode_start()
{
 /* Get reference of MCI*/ 
 oMCI = GetMCI(M1);
 /* Get reference of MCT*/ 
 oMCT = GetMCT(M1);
 /*Get the CSPD instance of SpeednPosFdbk Class */
 CSPD xCSPD = MCT_GetSpeednPosSensorMain(oMCT);
 
 if (TB_UserTimebaseHasElapsed())
 {
  /* User defined code */
  switch (User_State)
  {
   case US_RESET:
    {
          /* Next state */
          /* This command sets what will be the first torque ramp after the 
          MCI_StartMotor command. It requires as first parameter the oMCI[0], as 
          second parameter is the value of motor torque reference at the end of the ramp 
          and as third parameter the torque ramp duration in milliseconds. */
          MCI_ExecTorqueRamp(oMCI, torque_value, torque_duration);
          
          /* This is a user command used to start the motor. The speed ramp shall be
          pre programmed before the command.*/
          cmd_status = MCI_StartMotor(oMCI);
          
          /* It verifies if the command  "MCI_StartMotor" is successfully executed 
          otherwise it tries to restart the procedure */
          if(cmd_status==FALSE)    
           {
            User_State = US_RESET;                       // Command NOT executed
           }
          else User_State = US_SPEED;           // Command executed
          
          UserCnt = 0;
          
    }
    break;  
   
   case US_SPEED:
    { 
       /* The function "GetAvrgMecSpeed01Hz(..)"  returns the last computed 
       average mechanical speed, expressed in 01Hz (tenth of Hertz) */
      
       value_Speed_RPM = SPD_GetAvrgMecSpeed01Hz(xCSPD)*6;     
      
       /* The following code controls if the speed target is reached otherwise it remains in current state machine state*/
       if(value_Speed_RPM >=(speed_threshold-6))
        {
         /* The speed target has been reached */ 
         MCI_ExecSpeedRamp(oMCI, speed_target/6, speed_duration); 
             
         UserCnt = 0;
         User_State = US_STOP;
        }        
    }
    break;      
    
   case US_STOP:
    {
       /* After the time "STOP_DURATION" the motor will be restarted */
       if (UserCnt >= STOP_DURATION)
          {
            /* Next state */ 
            /* This is a user command to stop the motor */
            MCI_StopMotor(oMCI);
            
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
