/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */
/*
 * reference follow link for freeModbus use in STM8S.
 * <http://bbs.ednchina.com/BLOG_ARTICLE_3016783.HTM>
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm8s.h"

/* ----------------------- static functions ---------------------------------*/
#if 0
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );
#endif

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
  if(xRxEnable == TRUE)
  {
//    UART1->CR2 |= (1<<5);       // UART1_ITConfig(UART1_IT_RXNE, ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  }
  else
  {
//    UART1->CR2 &= ~(1<<5);       // UART1_ITConfig(UART1_IT_RXNE, DISABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
  }
  if(xTxEnable == TRUE)
  {
//    UART1->CR2 |= (1<<6);        // UART1_ITConfig(UART1_IT_TXE, ENABLE);
    UART1_ITConfig(UART1_IT_TXE, ENABLE);
  }
  else
  {
//    UART1->CR2 &= ~(1<<6);       //UART1_ITConfig(UART1_IT_TXE, DISABLE);
    UART1_ITConfig(UART1_IT_TXE, DISABLE);
  }
}

/* Serial initialization */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
#if 0
  uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;
  /* set 8/9 data bits */
  if(ucDataBits==8)
  {
    UART1->CR1 &= (uint8_t)(~UART1_CR1_M); // 8 data bits 
  }
  else
  {
    UART1->CR1 |= (uint8_t)(UART1_CR1_M); // 9 data bits
  }
 
  /* Clear the STOP bits, default stopbit is 1bit.
  UART1_STOPBITS_1   = (uint8_t)0x00, -One stop bit is  transmitted at the end of frame 
  UART1_STOPBITS_0_5 = (uint8_t)0x10, -Half stop bits is transmitted at the end of frame
  UART1_STOPBITS_2   = (uint8_t)0x20, -Two stop bits are  transmitted at the end of frame
  UART1_STOPBITS_1_5 = (uint8_t)0x30, -One and half stop bits
  */
  UART1->CR3 &= (uint8_t)(~UART1_CR3_STOP);  
  /* Set the STOP bits number according to UART1_StopBits value  */
  UART1->CR3 |= (uint8_t)0x00;  //  UART1_STOPBITS_1
  
  /*
  UART1_PARITY_NO     = (uint8_t)0x00, -No Parity
  UART1_PARITY_EVEN   = (uint8_t)0x04, -Even Parity
  UART1_PARITY_ODD    = (uint8_t)0x06, -Odd Parity
  */
  
  /* Clear the Parity Control bit */  
  UART1->CR1 &= (uint8_t)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));  
  /* Set the Parity Control bit to UART1_Parity value */
//  UART1->CR1 |= (uint8_t)Parity;
  switch(eParity)
  {
  case MB_PAR_NONE:
   UART1->CR1 |= (uint8_t)(0x00);
   break;
  case MB_PAR_ODD:
   UART1->CR1 |= (uint8_t)(0x06);
   break;
  case MB_PAR_EVEN:
   UART1->CR1 |= (uint8_t)(0x04);
   break;
  default:
   break;
  }
  
  /* Clear the LSB mantissa of UART1DIV  */
  UART1->BRR1 &= (uint8_t)(~UART1_BRR1_DIVM);  
  /* Clear the MSB mantissa of UART1DIV  */
  UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVM);  
  /* Clear the Fraction bits of UART1DIV */
  UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVF); 
  
  /* Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value */
  BaudRate_Mantissa    = ((uint32_t)CLK_GetClockFreq() / (ulBaudRate << 4));
  BaudRate_Mantissa100 = (((uint32_t)CLK_GetClockFreq() * 100) / (ulBaudRate << 4));
  /* Set the fraction of UART1DIV  */
  UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
  /* Set the MSB mantissa of UART1DIV  */
  UART1->BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0); 
  /* Set the LSB mantissa of UART1DIV  */
  UART1->BRR1 |= (uint8_t)BaudRate_Mantissa;   

 
  /* Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits */
  UART1->CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN); 
  /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART1->CR3 &= (uint8_t)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL); 
  /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART1->CR3 |= (uint8_t)((uint8_t)0x80 & (uint8_t)(UART1_CR3_CPOL | 
                                                        UART1_CR3_CPHA | UART1_CR3_LBCL));  
  /* Set the Transmitter Enable bit */
  UART1->CR2 |= (uint8_t)UART1_CR2_TEN;     
  /* Set the Receiver Enable bit */
  UART1->CR2 |= (uint8_t)UART1_CR2_REN;  
  
  /* Clear the Clock Enable bit */
  UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN); 
  
  /* Enable the UART Receive interrupt: this interrupt is generated when the UART
    receive data register is not empty */
  UART1->CR2 |= UART1_CR2_RIEN;
//  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  /* Enable the UART Transmit complete interrupt: this interrupt is generated 
     when the UART transmit Shift Register is empty */
  UART1->CR2 |= UART1_CR2_TCIEN;
//  UART1_ITConfig(UART1_IT_TXE, ENABLE);

  /* Enable UART */
  UART1_Cmd(ENABLE);
  
    /* Enable general interrupts */
  enableInterrupts();    
#else
  UART1_WordLength_TypeDef WordLength=UART1_WORDLENGTH_8D;
  UART1_Parity_TypeDef Parity=UART1_PARITY_NO;
  
  if(ucDataBits!=8)WordLength=UART1_WORDLENGTH_9D;
  
  switch(eParity)
  {
  case MB_PAR_NONE:
    Parity = UART1_PARITY_NO;
    break;
  case MB_PAR_ODD:
    Parity = UART1_PARITY_ODD;
    break;
  case MB_PAR_EVEN:
    Parity = UART1_PARITY_EVEN;
    break;
  default:
    Parity = UART1_PARITY_NO;
    break;
  }
  
  UART1_Init((uint32_t)ulBaudRate, WordLength,UART1_STOPBITS_1, Parity,
                   UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  /* Enable the UART Receive interrupt: this interrupt is generated when the UART
    receive data register is not empty */
//  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  /* Enable the UART Transmit complete interrupt: this interrupt is generated 
     when the UART transmit Shift Register is empty */
//  UART1_ITConfig(UART1_IT_TXE, ENABLE);

  /* Enable UART */
  UART1_Cmd(ENABLE);
  
    /* Enable general interrupts */
  enableInterrupts();
#endif

  return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */  
//  while((UART1->SR & 0x80)==0x00)
    UART1->DR = ucByte;
//  UART1_SendData8(TxBuffer[TxCounter++]);
  return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
  *pucByte = UART1->DR;
  return TRUE;
}

#if 0
/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 * 
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
#endif
