/*=============================================================================
 *  SD2_SHIELD_RS485_KEY_example.c
 *-----------------------------------------------------------------------------
 *  Institución : DSI - FCEIA - UNR
 *  Autor       : Walter S. Wagner
 *  Año         : 2026
 *
 *  Licencia:
 *  ----------
 *  Copyright 2026, Walter S. Wagner
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Descripción del ejemplo:
 *  -------------------------
 *  Este ejemplo demuestra el uso combinado de las teclas del shield y la comunicación RS485.
 *    - Cuando se presiona SW2 del shield, se envía el carácter 'W' a través del bus RS485.
 *    - Cuando se presiona SW3 del shield, se envía el carácter 'D' por RS485.
 *    - Si se recibe el carácter 'E' por RS485, el LED1 del shield se enciende.
 *    - Si se recibe el carácter 'A', el LED1 del shield se apaga.
 *
 *  Prueba con PC:
 *  --------------
 *  Para probar la comunicación desde la PC:
 *     - Conectar un conversor RS485 a USB entre el bus RS485 y la PC.
 *     - Abrir una consola serie en el puerto COM asignado
 *     - Configurar la terminal serie a 115200 baudios
 *     - Presionar 'E' o 'A' desde la terminal para controlar el LED1 del shield.
 *     - Al presionar los botones del shield, se verán los caracteres 'W' o 'D' llegar a la PC.
 *
 *============================================================================*/

/*==================[inclusions]=============================================*/

// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Project Included Files
#include "SD2_board.h"
#include "SD2_shield.h"
#include "key.h"
#include "SD2_shield_rs485.h"
#include "clock_config.h"

/*==================[external functions definition]==========================*/

int main(void)
{
    BOARD_BootClockRUN();

    /* Inicialización de hardware de la base y del shield */
    board_init();
    shield_init();

    /* Inicialización de gestión de teclas del shield */
    key_init(KEY_SRC_SHIELD);

    /* Inicialización del módulo RS485 del shield */
    shield_rs485_init();

    /* Inicialización de LEDs del shield en estado apagado */
    shield_setLed(SHIELD_LED_ID_1, SHIELD_MSG_OFF);
    shield_setLed(SHIELD_LED_ID_2, SHIELD_MSG_OFF);
    shield_setLed(SHIELD_LED_ID_3, SHIELD_MSG_OFF);

    /* Inicialización de SysTick para temporización cada 1 ms */
    SysTick_Config(SystemCoreClock / 1000U);

    while (1)
    {
        /* ---- Envío de datos por RS485 al presionar teclas del shield ---- */
        if (key_getPressEv(KEY_SRC_SHIELD, SHIELD_SW_ID_2))
            shield_rs485_sendByte('W');

        if (key_getPressEv(KEY_SRC_SHIELD, SHIELD_SW_ID_3))
            shield_rs485_sendByte('D');

        /* ---- Recepción de datos por RS485 desde la PC/consola ---- */
        if (shield_rs485_isDataAvailable())
        {
            uint8_t dataRec = shield_rs485_readByte();

            /* Si llega 'E', enciende LED1 del shield */
            if (dataRec == 'E')
                shield_setLed(SHIELD_LED_ID_1, SHIELD_MSG_ON);

            /* Si llega 'A', apaga LED1 del shield */
            if (dataRec == 'A')
                shield_setLed(SHIELD_LED_ID_1, SHIELD_MSG_OFF);
        }
    }
}

/*==================[ISR Systick]============================================*/

/**
 * @brief Interrupción SysTick: actualiza gestión de teclas
 */
void SysTick_Handler(void)
{
    /* Actualiza el estado de teclas del shield cada 1 ms */
    key_periodicTask1ms(KEY_SRC_SHIELD);
}

/*==================[end of file]============================================*/
