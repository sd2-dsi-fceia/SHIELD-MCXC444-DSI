/*=============================================================================
 *  SD2_shield_rs485.c - Driver de comunicación RS485 para el Shield
 *-----------------------------------------------------------------------------

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
 *  Driver de capa baja que implementa la inicialización y manejo de RS485
 *  mediante UART2 en el shield didáctico. El control de dirección RE/DE se
 *  realiza mediante un único pin (ambos puenteados en el shield).
 *============================================================================*/

/*==================[inclusions]=============================================*/

#include "fsl_uart.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "SD2_shield.h"

/*==================[macros and definitions]=================================*/

#define RS485_UART                  UART2                 /**< Periférico UART usado para RS485 */
#define RS485_UART_IRQn             UART2_FLEXIO_IRQn     /**< IRQ correspondiente a UART2 */

/*==================[internal data declaration]==============================*/

/**
 * @brief Tabla de pines de control de dirección RS485 (RE: Receiver Enable).
 * NOTA: DE y RE están puenteados en el shield, sólo se controla uno.
 */
static const shield_gpioInfo_type shield_gpioContLine[] = {
    {PORTA, GPIOA, 12},    /* RE (usado para transmisión/recepción) */
    //{PORTA, GPIOA, 13},  /* DE (no usado, puenteado en HW) */
};

static bool dataAvailable;   /**< Indica si se recibió un byte por RS485 */
static uint8_t byteRec;      /**< Último byte recibido por RS485 */

/*==================[internal functions definition]==========================*/

/**
 * @brief Controla el pin RE (Receiver Enable) del transceiver RS485.
 * @param est  true=habilita transmisión, false=modo recepción.
 */
static void rs485_RE(bool est)
{
    if (est)
        GPIO_PortSet(shield_gpioContLine[0].gpio, 1 << shield_gpioContLine[0].pin);
    else
        GPIO_PortClear(shield_gpioContLine[0].gpio, 1 << shield_gpioContLine[0].pin);
}

/*==================[external functions definition]==========================*/

/**
 * @brief Inicializa la interfaz RS485 del shield usando UART2.
 *
 * - Configura el pin de control RE.
 * - Inicializa la UART2 a 115200 baudios, 8N1, sin paridad.
 * - Setea pines de RX y TX, controla los flags e interrupciones.
 */
void shield_rs485_init(void)
{
    gpio_pin_config_t gpio_config = {
        .outputLogic = 1,
        .pinDirection = kGPIO_DigitalOutput,
    };

    const port_pin_config_t port_config = {
        .pullSelect = kPORT_PullDisable,
        .slewRate = kPORT_SlowSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .driveStrength = kPORT_LowDriveStrength,
        .mux = kPORT_MuxAsGpio,
    };

    uart_config_t config;

    // Configuración del pin RE usando la tabla
    PORT_SetPinConfig(shield_gpioContLine[0].port, shield_gpioContLine[0].pin, &port_config);
    GPIO_PinInit(shield_gpioContLine[0].gpio, shield_gpioContLine[0].pin, &gpio_config);

    rs485_RE(false); // Inicialmente modo recepción

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(RS485_UART, &config, CLOCK_GetFreq(BUS_CLK));

    /* Configura los pines RX y TX de la UART2 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt3);   // RX
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt3);   // TX

    dataAvailable = false;

    UART_EnableInterrupts(RS485_UART, kUART_RxDataRegFullInterruptEnable);
    UART_EnableInterrupts(RS485_UART, kUART_TxDataRegEmptyInterruptEnable);
    UART_EnableInterrupts(RS485_UART, kUART_TransmissionCompleteInterruptEnable);

    NVIC_EnableIRQ(RS485_UART_IRQn);
}

/**
 * @brief Envía un byte por el bus RS485.
 *
 * Controla RE para sacar el transceiver de recepción, envía el byte,
 * y espera al finalizar la transmisión para volver a modo recepción.
 */
void shield_rs485_sendByte(uint8_t dato)
{
    rs485_RE(true); // Habilita transmisión

    UART_WriteByte(RS485_UART, dato);

    // Habilita interrupciones para transmisión completa
    UART_EnableInterrupts(RS485_UART, kUART_TxDataRegEmptyInterruptEnable);
    UART_EnableInterrupts(RS485_UART, kUART_TransmissionCompleteInterruptEnable);
}

/**
 * @brief Indica si hay un byte recibido pendiente de lectura.
 */
bool shield_rs485_isDataAvailable(void)
{
    return dataAvailable;
}

/**
 * @brief Devuelve el último byte recibido por RS485.
 * Limpia el flag de disponibilidad al leer.
 */
uint8_t shield_rs485_readByte(void)
{
    dataAvailable = false;
    return byteRec;
}

/*==================[Interrupt Service Routine]==============================*/

/**
 * @brief Handler de interrupción para la UART2/RS485.
 * Gestiona recepción, transmisión y finalización (cambio de dirección).
 */
void UART2_FLEXIO_IRQHandler(void)
{
    // RX: Byte recibido
    if ((kUART_RxDataRegFullFlag & UART_GetStatusFlags(RS485_UART)) &&
        (kUART_RxDataRegFullInterruptEnable & UART_GetEnabledInterrupts(RS485_UART)))
    {
        byteRec = UART_ReadByte(RS485_UART);
        dataAvailable = true;
        UART_ClearStatusFlags(RS485_UART, kUART_RxDataRegFullFlag);
    }

    // TX Buffer vacío
    if ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(RS485_UART)) &&
        (kUART_TxDataRegEmptyInterruptEnable & UART_GetEnabledInterrupts(RS485_UART)))
    {
        UART_DisableInterrupts(RS485_UART, kUART_TxDataRegEmptyInterruptEnable);
        UART_ClearStatusFlags(RS485_UART, kUART_TxDataRegEmptyFlag);
    }

    // TX Finalizada: vuelve a modo recepción
    if ((kUART_TransmissionCompleteFlag & UART_GetStatusFlags(RS485_UART)) &&
        (kUART_TransmissionCompleteInterruptEnable & UART_GetEnabledInterrupts(RS485_UART)))
    {
        UART_DisableInterrupts(RS485_UART, kUART_TransmissionCompleteInterruptEnable);
        UART_ClearStatusFlags(RS485_UART, kUART_TransmissionCompleteFlag);
        rs485_RE(false); // Volver a modo recepción
    }
}

/*==================[end of file]============================================*/
