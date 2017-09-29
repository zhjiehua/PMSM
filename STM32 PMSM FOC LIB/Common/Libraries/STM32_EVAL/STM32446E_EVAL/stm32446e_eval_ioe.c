/**
  ******************************************************************************
  * @file    stm324xg_eval_ioe.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   This file provides a set of functions needed to manage the STMPE811
  *          IO Expander devices mounted on STM324xG-EVAL evaluation board.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

  /* File Info : ---------------------------------------------------------------
  
    Note:
    -----
    - This driver uses the DMA method for sending and receiving data on I2C bus
      which allow higher efficiency and reliability of the communication.  
  
    SUPPORTED FEATURES:
      - IO Read/write : Set/Reset and Read (Polling/Interrupt)
      - Joystick: config and Read (Polling/Interrupt)
      - Touch Screen Features: Single point mode (Polling/Interrupt)
      - TempSensor Feature: accuracy not determined (Polling).

    UNSUPPORTED FEATURES:
      - Row ADC Feature is not supported (not implemented on STM324xG_EVAL board)
  ----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32446e_eval_ioe.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324xG_EVAL
  * @{
  */
    
/** @defgroup STM324xG_EVAL_IOE 
  * @brief  This file includes the IO Expander driver for STMPE811 IO Expander 
  *         devices.
  * @{
  */ 
  

/** @defgroup STM324xG_EVAL_IOE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_IOE_Private_Defines
  * @{
  */ 
#define TIMEOUT_MAX    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */
/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_IOE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_IOE_Private_Variables
  * @{
  */ 
TS_STATE TS_State;              /*<! The global structure holding the TS state */

uint32_t IOE_TimeOut = TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */
/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_IOE_Private_FunctionPrototypes
  * @{
  */ 

static void IOE_GPIO_Config(void);
static void IOE_I2C_Config(void);
static uint8_t IOE_ReadRegister(uint8_t reg);
static void IOE_WriteRegister(uint8_t reg, uint8_t data);
static uint8_t IOE_ReadID(void);
static void IOE_Init(void);
static void IOE_Start(void);
static void IOE_ConfigPins(void);

volatile uint32_t i = 0, j = 0; /* Used for delay */

#ifndef USE_Delay
static void delay(__IO uint32_t nCount);
#endif /* USE_Delay*/
/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_IOE_Private_Functions
  * @{
  */

uint8_t IOE_ReadRegister(uint8_t reg)
{
  uint8_t retVal;
  FMPI2C_TransferHandling(IOE_I2C, 0x84, 1, FMPI2C_SoftEnd_Mode,FMPI2C_Generate_Start_Write);
  FMPI2C_SendData(IOE_I2C,reg);
  
  while (FMPI2C_GetFlagStatus(IOE_I2C, FMPI2C_FLAG_TC) == RESET);
  
  FMPI2C_TransferHandling(IOE_I2C, 0x84, 1, FMPI2C_AutoEnd_Mode,FMPI2C_Generate_Start_Read);
  
  while (FMPI2C_GetFlagStatus(IOE_I2C, FMPI2C_FLAG_RXNE) == RESET);
  
  retVal = FMPI2C_ReceiveData(IOE_I2C);
  return retVal;
}

void IOE_WriteRegister(uint8_t reg, uint8_t data)
{
  FMPI2C_TransferHandling(IOE_I2C,0x84, 1, FMPI2C_Reload_Mode, FMPI2C_Generate_Start_Write);
  
  while (FMPI2C_GetFlagStatus(IOE_I2C, FMPI2C_FLAG_TXIS) == RESET);
  
  FMPI2C_SendData(IOE_I2C, reg);
  
  while (FMPI2C_GetFlagStatus(IOE_I2C, FMPI2C_FLAG_TCR) == RESET);
  
  FMPI2C_TransferHandling(IOE_I2C,0x84, 1, FMPI2C_AutoEnd_Mode, FMPI2C_No_StartStop);
  
  FMPI2C_SendData(IOE_I2C, data);
  
  
  for (i = 0; i < 10000; i++)
  {
    j = 0;
  }
}

uint8_t IOE_ReadID(void)
{
  return IOE_ReadRegister(0x00);
}

void IOE_Init(void)
{
  IOE_ReadRegister(0x41);
  IOE_WriteRegister(0x41,0x02);
  IOE_ReadRegister(0x41);
  IOE_WriteRegister(0x41,0x03);
}

void IOE_Start()
{
  IOE_ReadRegister(0x40);
  IOE_WriteRegister(0x40, 0x09);
}

void IOE_ConfigPins(void)
{
  IOE_ReadRegister(0x48);
  IOE_WriteRegister(0x48, 0x00);
  IOE_ReadRegister(0x60);
  IOE_WriteRegister(0x60, 0x00);
  IOE_ReadRegister(0x64);
  IOE_WriteRegister(0x64, 0x3e);
  IOE_ReadRegister(0x68);
  IOE_WriteRegister(0x68, 0x3e);
}

/**
  * @brief  Initializes and Configures the two IO_Expanders Functionalities 
  *         (IOs, Touch Screen ..) and configures all STM324xG_EVAL necessary
  *         hardware (GPIOs, APB clocks ..).
  * @param  None
  * @retval IOE_OK if all initializations done correctly. Other value if error.
  */
uint8_t IOE_Config(void)
{
  /* Configure the needed pins */
  IOE_GPIO_Config(); 
  IOE_I2C_Config();
    
  IOE_ReadID();
  IOE_Init();
  IOE_Start();
  IOE_ConfigPins();

  return IOE_OK; 
}

/**
  * @brief  Returns the current Joystick status.
  * @param  None
  * @retval The code of the Joystick key pressed: 
  *   @arg  JOY_NONE
  *   @arg  JOY_SEL
  *   @arg  JOY_DOWN
  *   @arg  JOY_LEFT
  *   @arg  JOY_RIGHT
  *   @arg  JOY_UP
  */
JOYState_TypeDef
 IOE_JoyStickGetState(void)
{
  uint8_t tmp = 0;
  /* Read the status of all pins */
  tmp = IOE_ReadRegister(0x10);
   
  /* Check the pressed keys */
  if ((tmp & JOY_IO_NONE) == JOY_IO_NONE)
  {
    return (JOYState_TypeDef) JOY_NONE;
  }
  else if (!(tmp & JOY_IO_SEL))
  {
    return (JOYState_TypeDef) JOY_SEL;
  }
  else if (!(tmp & JOY_IO_DOWN))
  {
    return (JOYState_TypeDef) JOY_DOWN;
  }
  else if (!(tmp & JOY_IO_LEFT))
  {
    return (JOYState_TypeDef) JOY_LEFT;
  }
  else if (!(tmp & JOY_IO_RIGHT))
  {
    return (JOYState_TypeDef) JOY_RIGHT;
  }
  else if (!(tmp & JOY_IO_UP))
  {
    return (JOYState_TypeDef) JOY_UP;
  }
  else
  { 
    return (JOYState_TypeDef) JOY_NONE;
  }
}

/**
  * @brief  Initializes the GPIO pins used by the IO expander.
  * @param  None
  * @retval None
  */
static void IOE_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable IOE_I2C and IOE_I2C_GPIO_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(IOE_I2C_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(IOE_I2C_SCL_GPIO_CLK | IOE_I2C_SDA_GPIO_CLK |
                         IOE_IT_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Reset IOE_I2C IP */
  RCC_APB1PeriphResetCmd(IOE_I2C_CLK, ENABLE);
  
  /* Release reset signal of IOE_I2C IP */
  RCC_APB1PeriphResetCmd(IOE_I2C_CLK, DISABLE);
  
  /* IOE_I2C SCL and SDA pins configuration */
  GPIO_InitStructure.GPIO_Pin = IOE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(IOE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = IOE_I2C_SDA_PIN;
  GPIO_Init(IOE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(IOE_I2C_SCL_GPIO_PORT, IOE_I2C_SCL_SOURCE, IOE_I2C_SCL_AF);
  GPIO_PinAFConfig(IOE_I2C_SDA_GPIO_PORT, IOE_I2C_SDA_SOURCE, IOE_I2C_SDA_AF);  
  
  /* Set EXTI pin as Input PullUp - IO_Expander_INT */
  GPIO_InitStructure.GPIO_Pin = IOE_IT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(IOE_IT_GPIO_PORT, &GPIO_InitStructure);  
}

#define EVAL_I2Cx_TIMING                      ((uint32_t)0x00805252)

/**
  * @brief  Configure the I2C Peripheral used to communicate with IO_Expanders.
  * @param  None
  * @retval None
  */
static void IOE_I2C_Config(void)
{
  FMPI2C_InitTypeDef I2C_InitStructure;
  
  /* IOE_I2C configuration */
  I2C_InitStructure.FMPI2C_Timing = EVAL_I2Cx_TIMING;
  I2C_InitStructure.FMPI2C_AnalogFilter = FMPI2C_AnalogFilter_Enable;
  I2C_InitStructure.FMPI2C_DigitalFilter = 0x00;
  I2C_InitStructure.FMPI2C_Mode = FMPI2C_Mode_FMPI2C;
  I2C_InitStructure.FMPI2C_OwnAddress1 = 0x84;
  I2C_InitStructure.FMPI2C_Ack = FMPI2C_Ack_Enable;
  I2C_InitStructure.FMPI2C_AcknowledgedAddress = FMPI2C_AcknowledgedAddress_7bit;
  
  FMPI2C_Init(IOE_I2C, &I2C_InitStructure);
}

#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
#endif /* USE_Delay*/
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */      
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
