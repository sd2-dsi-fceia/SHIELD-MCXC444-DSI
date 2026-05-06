/*=============================================================================
 *  SD2_SHIELD - Hardware Abstraction Layer (HAL) para Shield de prácticas
 *-----------------------------------------------------------------------------
 *  Institución : DSI - FCEIA - UNR
 *  Autor       : Walter S. Wagner
 *  Año         : 2026
 *
 *  Descripción:
 *  ------------
 *  Este módulo implementa una capa de abstracción de hardware (HAL) para el
 *  manejo de periféricos básicos del shield didáctico utilizado en la cátedra.
 *  Facilita una interfaz de alto nivel para:
 *      - LEDs (salidas digitales)
 *      - Pulsadores (entradas digitales)
 *      - Salidas digitales adicionales: Relé y Buzzer
 *      - Pines de control y acceso SPI para Display OLED gráfico
 *
 *  El diseño desacopla la lógica de aplicación del hardware subyacente,
 *  permitiendo independencia de plataforma, facilidad de mantenimiento y de escalabilidad.
 *============================================================================*/

/*=============================================================================
 *  Licencia BSD-3 Clause
 *============================================================================*/
#ifndef SD2_SHIELD_H_
#define SD2_SHIELD_H_

#include <stdbool.h>
#include "PERI_GPIO.h"
#include "PERI_PORT.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 *  Definiciones de tipos públicos
 *============================================================================*/

/**
 * @brief Identificadores lógicos de LEDs disponibles en el shield
 *
 * NOTA:
 * - La numeración comienza en 0 para facilitar indexación en arreglos.
 * - SHIELD_LED_ID_TOTAL permite dimensionar estructuras de forma segura.
 */
typedef enum
{
    SHIELD_LED_ID_1 = 0,
    SHIELD_LED_ID_2,
    SHIELD_LED_ID_3,

    SHIELD_LED_ID_TOTAL   /**< Cantidad total de LEDs */
} shield_ledId_enum;

/**
 * @brief Identificadores lógicos de pulsadores disponibles en el shield
 */
typedef enum
{
    SHIELD_SW_ID_2 = 0,
    SHIELD_SW_ID_3,

    SHIELD_SW_ID_TOTAL    /**< Cantidad total de switches */
} shield_swId_enum;

/**
 * @brief Identificadores lógicos de salidas digitales adicionales
 *
 * Este enum agrupa periféricos de salida digitales genéricos distintos de los LEDs,
 * tales como relé, buzzer y similares, facilitando su manejo cohesivo
 * y futura expansión (alarma, motor, ventilador, etc).
 *
 * NOTA:
 * - La numeración comienza en 0 para indexar arreglos.
 * - SHIELD_DOUT_ID_TOTAL indica la cantidad de salidas adicionales.
 */
typedef enum
{
    SHIELD_DOUT_ID_RELAY = 0,     /**< Control de Relé (Puerto E0) */
    SHIELD_DOUT_ID_BUZZER,        /**< Control de Buzzer (Puerto E30) */

    SHIELD_DOUT_ID_TOTAL          /**< Cantidad total de salidas digitales adicionales */
} shield_doutId_enum;

/**
 * @brief Mensajes de control para LEDs y salidas DOUT
 *
 * Los valores siguen la lógica de salida digital:
 *   - OFF: Estado inactivo (por defecto al iniciar)
 *   - ON:  Estado activo
 *   - TOGGLE (solo para LEDs): Invierte el estado actual
 *
 * NOTA:
 * - No todos los DOUT soportan TOGGLE (consultar documentación de cada función).
 */
typedef enum
{
    SHIELD_MSG_OFF = 0,      /**< Fuerza nivel lógico inactivo */
    SHIELD_MSG_ON,           /**< Fuerza nivel lógico activo */
    SHIELD_MSG_TOGGLE        /**< Invierte el estado actual (sólo LEDs) */
} shield_msg_enum;

/**
 * @brief Identificadores de pines de control y acceso SPI para display OLED gráfico
 *
 * Estos IDs permiten controlar el reset, el pin de datos/comando, y el chip select
 * del display OLED gráfico, además del acceso vía SPI.
 */
typedef enum
{
    OLED_RST_PIN = 0,      /**< Pin de reset del OLED */
    OLED_DATA_CMD_PIN,     /**< Pin de datos/comando del OLED */
    OLED_CS,               /**< Chip Select manual del OLED */
    OLED_TOTAL             /**< Cantidad total de pines de control del OLED */
} shield_oledPin_enum;

/**
 * @brief Estructura descriptiva de un GPIO físico del shield
 *
 * Encapsula la información del periférico físico:
 *  - Base del periférico PORT
 *  - Base del periférico GPIO
 *  - Número de pin dentro del puerto
 *
 * Favorece el desacople entre API y hardware físico concreto.
 */
typedef struct
{
    PORT_Type *port;
    GPIO_Type *gpio;
    uint32_t   pin;
} shield_gpioInfo_type;

/*=============================================================================
 *  API pública
 *============================================================================*/

/**
 * @brief Inicializa el hardware del shield.
 *
 * Configura:
 *  - Pines de LEDs y salidas DOUT como salida
 *  - Pines de pulsadores como entrada
 *  - Pines de control OLED como salida
 *  - Estados iniciales seguros
 *
 * Debe ejecutarse una única vez al inicio del sistema.
 */
void shield_init(void);

/**
 * @brief Controla el estado de un LED
 *
 * @param id  Identificador lógico del LED
 * @param msg Acción a realizar sobre el LED (ON, OFF, TOGGLE)
 *
 * @note
 * La función abstrae completamente el acceso a hardware.
 */
void shield_setLed(shield_ledId_enum id, shield_msg_enum msg);

/**
 * @brief Controla el estado de una salida digital adicional (relé, buzzer, etc)
 *
 * @param id  Identificador lógico de la salida DOUT
 * @param msg Acción a realizar (ON, OFF)
 *
 * @note
 * TOGGLE no es soportado a menos que se especifique para algún periférico DOUT.
 * La semántica "activo" depende de la topología eléctrica del hardware.
 */
void shield_setDout(shield_doutId_enum id, shield_msg_enum msg);

/**
 * @brief Lee el estado de un pulsador
 *
 * @param id Identificador lógico del pulsador
 * @return true  Pulsador presionado
 * @return false Pulsador no presionado
 *
 * @note
 * La semántica "presionado" depende del hardware (pull-up/pull-down).
 */
bool shield_getSw(shield_swId_enum id);

/**
 * @brief Inicializa y configura el periférico SPI0 para comunicación con display OLED.
 *
 * Configura los pines, clocks y parámetros de SPI para el acceso al display.
 * Debe ejecutarse antes de cualquier transferencia SPI con el OLED.
 */
void shield_configSPI0(void);

/**
 * @brief Envía un buffer de datos por SPI al display OLED.
 *
 * Selecciona el OLED (CS bajo), transfiere los datos, libera el OLED (CS alto).
 *
 * @param buf  Puntero a buffer de datos a enviar
 * @param len  Cantidad de bytes a transferir
 */
void shield_SPISend(uint8_t* buf, size_t len);

/**
 * @brief Setea el valor lógico del pin de control de OLED.
 *
 * Permite controlar RST, D/C, CS, etc. de forma manual.
 *
 * @param oledPin  Identificador lógico según shield_oledPin_enum
 * @param state    0 = nivel bajo, 1 = nivel alto
 */
void shield_setOledPin(shield_oledPin_enum oledPin, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* SD2_SHIELD_H_ */
/*==================[end of file]============================================*/
