/**
  @page Speed monitor

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    Speed monitor/readme.txt 
  * @author  Motor Control Team - System Lab 
  * @version 4.3.0
  * @date    22-Sep-2016 15:29
  * @brief   How to generate a PWM signal according with the electrical motor speed
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

This example describes how to generate a PWM signal in order to provide the information 
of Electrical Speed of the motor. This signal is generated in RUN mode only.

The PWM timer is configured in Toggle and Up counting mode with a prescaler set for 
very low frequency generation (2MHz). The Autoreload preload function has been defined in 
order to provide the new value before each PWM period.
The ARR register is modified according with the electrical angle calculated through API 
(SPD_GetAvrgMecSpeed01Hz) and number of poles.
 
MC-Lib API used:

- SPD_GetAvrgMecSpeed01Hz()

@par Directory contents 

  - Speed monitor/Speed monitor_F1.c     Example configuration file

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
