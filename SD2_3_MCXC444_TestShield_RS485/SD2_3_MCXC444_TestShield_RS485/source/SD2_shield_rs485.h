/*=============================================================================
 *  SD2_shield_rs485.h - Driver de comunicación RS485 para el Shield
 *-----------------------------------------------------------------------------
 *  ----------------------
 *  Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *  Copyright 2017, Diego Alegrechi
 *  Copyright 2017, Gustavo Muro
 *  Copyright 2026, Walter S. Wagner
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived from this
 *       software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Descripción:
 *  ------------
 *  Prototipos para inicialización y control de la interfaz RS485 a través
 *  del shield didáctico. Implementa comunicación half-duplex por UART2 y
 *  opera el pin de dirección (RE/DE) necesario para el bus RS485.
 *============================================================================*/

#ifndef SD2_SHIELD_RS485_H_
#define SD2_SHIELD_RS485_H_

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdbool.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[API pública]============================================*/

/**
 * @brief Inicializa la interfaz RS485 del shield (UART y control de dirección)
 */
void shield_rs485_init(void);

/**
 * @brief Envía un byte por el bus RS485
 * @param dato   Byte a transmitir
 */
void shield_rs485_sendByte(uint8_t dato);

/**
 * @brief Indica si hay un dato recibido pendiente por RS485
 * @return true si hay un byte recibido, false si no
 */
bool shield_rs485_isDataAvailable(void);

/**
 * @brief Devuelve el último byte recibido por RS485
 * @return  último byte recibido
 */
uint8_t shield_rs485_readByte(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* SD2_SHIELD_RS485_H_ */
