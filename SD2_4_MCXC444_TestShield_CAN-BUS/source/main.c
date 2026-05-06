/* Copyright 2026, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2026, Agustin M. Zuliani (amzuliani02@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusions]=============================================*/

#include "SD2_board.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "mcp2515.h"
#include "can.h"
#include <stdint.h>

/*==================[macros and definitions]=================================*/

#define MSG_TIMEOUT		1000
#define MSG_CAN_ID		0X01

/*==================[internal data declaration]==============================*/

uint16_t msg_Timeout = MSG_TIMEOUT;

struct can_frame canMsg;
struct can_frame canMsgRead;

/*==================[internal functions declaration]=========================*/

int main(void)
{
	ERROR_t	error;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    board_init();
    board_configSPI0();
    SysTick_Config(SystemCoreClock / 1000U);

    canMsg.can_id  = MSG_CAN_ID;
    canMsg.can_dlc = 1;
    canMsg.data[0] = 0x0A;

    error = mcp2515_reset();

    error = mcp2515_setBitrate(CAN_125KBPS, MCP_8MHZ);

    error = mcp2515_setNormalMode();
//    error = mcp2515_setLoopbackMode();

    PRINTF("Ejemplo: Comunicacion CAN BUS MCXC444 SD2\r\n");

    while(1)
    {
    	if (!msg_Timeout)
    	{
//    		error = mcp2515_sendMessage(&canMsg);
//    		if(error == ERROR_OK)
//    		{
//    			PRINTF("Mensaje transmitido - ID: 0x%X, DLC: %d, Data[0]: 0x%X\r\n",
//					   canMsg.can_id,
//					   canMsg.can_dlc,
//					   canMsg.data[0]);
//			}

    		error = mcp2515_readMessage(&canMsgRead);
    		if(error == ERROR_OK)
			{
				PRINTF("Mensaje recibido - ID: 0x%X, DLC: %d, Data[0]: 0x%X\r\n",
					   canMsgRead.can_id,
					   canMsgRead.can_dlc,
					   canMsgRead.data[0]);
			}

    		msg_Timeout = MSG_TIMEOUT;
    	}
    }

    return 0;
}

void SysTick_Handler(void)
{
	if (msg_Timeout)
	{
		msg_Timeout--;
	}
}
