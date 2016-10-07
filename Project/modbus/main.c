/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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
#include "mb.h"
#include "mbport.h"

/* note: this header is end of FreeModbus header */
#include "stm8s.h"
#include "led.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Config(void);
static void CLK_Config(void);
void Delay(uint32_t nCount);

/* Private functions ---------------------------------------------------------*/
const uint8_t *FW_Pro = "Angle alarm";
const uint8_t *FW_Ver = "0.1.0";

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000+1
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

void main(void)
{
  eMBErrorCode    eStatus;

  
  /* Clock configuration -----------------------------------------*/
  CLK_Config();  

  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();

  /* SPI configuration -----------------------------------------*/
//  SPI_Config();

  /*
  eMBErrorCode
  eMBInit( eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
  
  eMode - MB_RTU, MB_ASCII, MB_TCP
  ucSlaveAddress - 0, 1-247
  ucPort - 
  ulBaudRate - 
  eParity - 
  
  */
  eStatus = eMBInit( MB_RTU, 1, 1, 9600, MB_PAR_NONE );

  /* Enable the Modbus Protocol Stack. */
  eStatus = eMBEnable(  );
 
  /* Infinite loop */
  while (1)
  {
    ( void )eMBPoll(  );

    /* Here we simply count the number of poll cycles. */
//    usRegInputBuf[0]++;
    usRegInputBuf[0]++;
    usRegInputBuf[1]=1;
    usRegInputBuf[2]=2;
    usRegInputBuf[3]=3;
  
  }
}

/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/**
  * @brief  Configure LEDs available on the evaluation board
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* Initialize LEDs mounted on the Eval board */
//  STM_EVAL_LEDInit(LED1);
//  STM_EVAL_LEDInit(LED2);
//  STM_EVAL_LEDInit(LED3);
//  STM_EVAL_LEDInit(LED4);
    //beep_init();
}

/* 该函数为 0x04的数据填写回调函数，该函数由mbfuncinput.c中的
 * eMBFuncReadInputRegister()函数调用。
 * 该函数将指定开始地址usAddress的usNRegs指定数量寄存器内容添加到
 * pucRegBuffer缓存中 */
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    // 输入的地址信息检测
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}


/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
