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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm8s.h"

/* ----------------------- static functions ---------------------------------*/
#if 0
static void prvvTIMERExpiredISR( void );
#endif

/* ----------------------- Start implementation -----------------------------*/
/* 定时器初始化 */
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
/* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
                                                           --> 16us
  - In this example we need to generate a time base equal to 50us
   so TIM4_PERIOD = (0.00005 * 125000 - 1) = 5 */
    /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, (5*usTim1Timerout50us));
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);  
  /* enable interrupts */
 // enableInterrupts();
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
  return TRUE;
}

#include "led.h"
inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
     /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  TIM4_SetCounter(0x00);
     /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
  TIM4_Cmd(DISABLE);
  TIM4_SetCounter(0x00);
  TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
#if 0
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}
#endif

