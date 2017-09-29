/**
  @page Ramp generation 

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    Ramp/readme.txt 
  * @author  Motor Control Team - System Lab 
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   How to generate a Speed Loop ramp
  ******************************************************************************
  *
  * Licensed under ST Liberty SW License Agreement V2, (the "License");
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
  @endverbatim

@par Example Description 

This example describes how to use the API of Motor Control library to generate a speed 
ramp according with two parameters, speed_first_valueRPM and speed_firstramp_duration 
respectively 3000 RPM and 200 msec by default. These values could be changed by the user.
If the current speed target has been reached the state machine moves to set a different
speed ramp. If the speed target is below the minimum threshold the motor is stopped and 
restarted after a fixed time.

MC-Lib API used:

- MCI_ExecSpeedRamp()
- MCI_StartMotor()
- MCI_RampCompleted()
- SPD_GetAvrgMecSpeed01Hz()
- MCI_StopMotor()

@par Directory contents 

  - Ramp/ramp_F1.c     Example configuration file

@par Hardware and Software environment 

  - This example runs on P-NUCLEO-KIT.
  
  - This example can be run on different board if configured with the ST MC Workbench.

@par How to use it ? 

In order to make the program work, you must do the following:

 - Select the right example from drop box menu
 - Press F8 to batch-build the entire workspace (select a proper project configuration, 
   according to the uC part in use);
 - Read the messages window and download the firmware;
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
