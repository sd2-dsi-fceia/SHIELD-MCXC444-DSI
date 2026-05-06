/* Copyright 2018, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2018, Gustavo Muro
 * Copyright 2023, Guido Cicconi
 * Copyright 2026, Walter S. Wagner
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

#ifndef KEY_H_
#define KEY_H_

/*==================[inclusions]=============================================*/
#include "SD2_board.h"
#include "SD2_shield.h"
#include <stdbool.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros y definiciones]==================================*/

/**
 * @brief Origen lógico de los recursos: BOARD o SHIELD
 *
 * Permite seleccionar con qué set de switches/leds se opera.
 */
typedef enum {
    KEY_SRC_BOARD = 0,    /**< Usa recursos del board/base */
    KEY_SRC_SHIELD,       /**< Usa recursos del shield didáctico */
    KEY_SRC_TOTAL
} key_src_enum;

/*==================[external functions definition]==========================*/

/**
 * @brief Inicializa módulo de gestión de switches
 *
 * @param origen   KEY_SRC_BOARD o KEY_SRC_SHIELD
 *
 * Inicializa todos los recursos para el origen seleccionado.
 */
void key_init(key_src_enum origen);

/**
 * @brief Periodic task para actualización de estados (debe llamarse cada 1ms)
 *
 * @param origen   KEY_SRC_BOARD o KEY_SRC_SHIELD
 */
void key_periodicTask1ms(key_src_enum origen);

/**
 * @brief Devuelve evento de presionado para el switch indicado
 *
 * @param origen   KEY_SRC_BOARD o KEY_SRC_SHIELD
 * @param id       Identificador del switch (según origen)
 * @return true si el evento ocurrió (flag se resetea tras lectura)
 */
bool key_getPressEv(key_src_enum origen, int32_t id);

/**
 * @brief Devuelve evento de liberación para el switch indicado
 *
 * @param origen   KEY_SRC_BOARD o KEY_SRC_SHIELD
 * @param id       Identificador del switch (según origen)
 * @return true si el evento ocurrió (flag se resetea tras lectura)
 */
bool key_getReleaseEv(key_src_enum origen, int32_t id);

/**
 * @brief Devuelve si el switch está presionado por lo menos timeMs milisegundos
 *
 * @param origen   KEY_SRC_BOARD o KEY_SRC_SHIELD
 * @param id       Identificador del switch (según origen)
 * @param timeMs   Tiempo en milisegundos requerido
 * @return true si cumple la condición, false si no
 */
bool key_getPressed(key_src_enum origen, int32_t id, int32_t timeMs);

/**
 * @brief Limpia los flags de evento de presionado/liberado para el switch
 *
 * @param origen   KEY_SRC_BOARD o KEY_SRC_SHIELD
 * @param id       Identificador del switch (según origen)
 */
void key_clearFlags(key_src_enum origen, int32_t id);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* KEY_H_ */
