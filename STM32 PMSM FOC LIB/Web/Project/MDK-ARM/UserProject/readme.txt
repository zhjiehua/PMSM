/**
  @page mdkarm MDK-ARM Project Template
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version 4.1.0
  * @date    26-May-2015 14:11
  * @brief   This sub-directory contains all the user-modifiable files needed 
  *          to create a new project linked with the STM32F10x Standard Peripheral  
  *          Library and working with RealView MDK-ARM toolchain (Version 4.10 and later).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
  @endverbatim
 
 @par Directory contents
 
 - Project.Uv2/.Opt: A pre-configured project file with the provided library structure
                     that produces an executable image with MDK-ARM
              
@note the @subpage note file contains the needed steps to follow when using the 
default startup file provided by MDK-ARM when creating new projects.
      
 @par How to use it ?
 
 - Open the Project.uvproj project
 - In the build toolbar select the project config:
     - STM32100B-EVAL: to configure the project for STM32 Medium-density Value 
       line devices
     @note The needed define symbols for this config are already declared in the
           preprocessor section: USE_STDPERIPH_DRIVER, STM32F10X_MD_VL, USE_STM32100B_EVAL

     - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
     @note The needed define symbols for this config are already declared in the
           preprocessor section: USE_STDPERIPH_DRIVER, STM32F10X_CL, USE_STM3210C_EVAL

     - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
     @note The needed define symbols for this config are already declared in the
           preprocessor section: USE_STDPERIPH_DRIVER, STM32F10X_MD, USE_STM3210B_EVAL

     - STM3210E-EVAL: to configure the project for STM32 High-density devices
     @note The needed define symbols for this config are already declared in the
           preprocessor section: USE_STDPERIPH_DRIVER, STM32F10X_HD, USE_STM3210E_EVAL

     - STM32F10X_XL: to configure the project for STM32 XL-density devices
     @note The needed define symbols for this config are already declared in the
           preprocessor section: USE_STDPERIPH_DRIVER, STM32F10X_XL, USE_STM3210E_EVAL

 - Rebuild all files: Project->Rebuild all target files
 - Load project image: Debug->Start/Stop Debug Session
 - Run program: Debug->Run (F5)

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
    
 * <h3><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h3>
 */
