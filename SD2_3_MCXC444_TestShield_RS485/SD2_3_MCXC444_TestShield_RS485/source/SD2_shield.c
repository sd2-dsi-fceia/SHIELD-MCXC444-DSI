/*=============================================================================
 *  SD2_SHIELD - HAL para Shield de prácticas
 *=============================================================================
 *  Institución : DSI - FCEIA - UNR
 *  Autor       : Walter S. Wagner
 *  Año         : 2026
 *============================================================================*/

/*==================[inclusions]=============================================*/
#include "SD2_shield.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

/*=============================================================================
 *  Mapeo hardware (HW Binding Layer)
 *-----------------------------------------------------------------------------
 *  Vincula IDs lógicos con recursos físicos reales del microcontrolador,
 *  permitiendo modificar el hardware sin impacto en la API pública.
 *============================================================================*/

/**
 * @brief Tabla de mapeo para LEDs
 * NOTA:
 * - Lógica activa en nivel bajo (active-low)
 * - outputLogic = 1 → LED apagado al iniciar
 */
static const shield_gpioInfo_type shield_gpioLeds[] =
{
    {PORTE, GPIOE, 20},     /* LED1 */
    {PORTE, GPIOE, 21},     /* LED2 */
    {PORTE, GPIOE, 22}      /* LED3 */
};

/**
 * @brief Tabla de mapeo para pulsadores
 * NOTA:
 * - Configurados con pull-up interno
 * - Lectura activa en bajo (pressed = 0 lógico)
 */
static const shield_gpioInfo_type shield_gpioSw[] =
{
    {PORTB, GPIOB, 17},     /* SW2 */
    {PORTB, GPIOB, 16}      /* SW3 */
};

/**
 * @brief Tabla de mapeo para salidas digitales adicionales (DOUT)
 *
 * NOTA:
 * - Lógica activa en alto (active-high)
 * - outputLogic = 0 → salida inactiva al iniciar
 * - El orden debe coincidir con el enum shield_doutId_enum
 */
static const shield_gpioInfo_type shield_gpioDout[] =
{
    {PORTE, GPIOE, 0},      /* RELÉ */
    {PORTE, GPIOE, 30}      /* BUZZER */
};

/*=============================================================================
 *  Inicialización de hardware
 *============================================================================*/
void shield_init(void)
{
    int32_t i;

    /*---------------------------------------------------------------------
     * Configuración GPIO para LEDs (activo-bajo)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_led_config =
    {
        .outputLogic  = 1U,                    /* Estado inicial: OFF (active-low) */
        .pinDirection = kGPIO_DigitalOutput,
    };

    /*---------------------------------------------------------------------
     * Configuración GPIO para salidas DOUT (activo-alto)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_dout_config =
    {
        .outputLogic  = 0U,                    /* Estado inicial: OFF (active-high) */
        .pinDirection = kGPIO_DigitalOutput,
    };

    /*---------------------------------------------------------------------
     * Configuración GPIO para pulsadores (entrada)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_in_config =
    {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U                     /* No relevante en entrada */
    };

    /*---------------------------------------------------------------------
     * Configuración PORT para salidas (LEDs y DOUT)
     *--------------------------------------------------------------------*/
    const port_pin_config_t port_out_config =
    {
        .pullSelect          = kPORT_PullDisable,
        .slewRate            = kPORT_SlowSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .driveStrength       = kPORT_LowDriveStrength,
        .mux                 = kPORT_MuxAsGpio,
    };

    /*---------------------------------------------------------------------
     * Configuración PORT para pulsadores
     *--------------------------------------------------------------------*/
    const port_pin_config_t port_sw_config =
    {
        .pullSelect          = kPORT_PullUp,    /* Define lógica activa en bajo */
        .slewRate            = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .driveStrength       = kPORT_LowDriveStrength,
        .mux                 = kPORT_MuxAsGpio,
    };

    /*---------------------------------------------------------------------
     * Habilitación de clocks de puertos
     *--------------------------------------------------------------------*/
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);

    /*---------------------------------------------------------------------
     * Inicialización de LEDs
     *--------------------------------------------------------------------*/
    for (i = 0; i < SHIELD_LED_ID_TOTAL; i++)
    {
        PORT_SetPinConfig(shield_gpioLeds[i].port,
                          shield_gpioLeds[i].pin,
                          &port_out_config);

        GPIO_PinInit(shield_gpioLeds[i].gpio,
                     shield_gpioLeds[i].pin,
                     &gpio_led_config);
    }

    /*---------------------------------------------------------------------
     * Inicialización de salidas digitales adicionales (DOUT)
     *--------------------------------------------------------------------*/
    for (i = 0; i < SHIELD_DOUT_ID_TOTAL; i++)
    {
        PORT_SetPinConfig(shield_gpioDout[i].port,
                          shield_gpioDout[i].pin,
                          &port_out_config);

        GPIO_PinInit(shield_gpioDout[i].gpio,
                     shield_gpioDout[i].pin,
                     &gpio_dout_config);
    }

    /*---------------------------------------------------------------------
     * Inicialización de pulsadores
     *--------------------------------------------------------------------*/
    for (i = 0; i < SHIELD_SW_ID_TOTAL; i++)
    {
        PORT_SetPinConfig(shield_gpioSw[i].port,
                          shield_gpioSw[i].pin,
                          &port_sw_config);

        GPIO_PinInit(shield_gpioSw[i].gpio,
                     shield_gpioSw[i].pin,
                     &gpio_in_config);
    }
}

/*=============================================================================
 *  Control de LEDs
 *============================================================================*/
void shield_setLed(shield_ledId_enum id, shield_msg_enum msg)
{
    uint32_t mask = (1U << shield_gpioLeds[id].pin);

    switch (msg)
    {
        case SHIELD_MSG_OFF:
            /* Active-low → escribir 1 apaga */
            GPIO_PortSet(shield_gpioLeds[id].gpio, mask);
            break;

        case SHIELD_MSG_ON:
            /* Active-low → escribir 0 enciende */
            GPIO_PortClear(shield_gpioLeds[id].gpio, mask);
            break;

        case SHIELD_MSG_TOGGLE:
            GPIO_PortToggle(shield_gpioLeds[id].gpio, mask);
            break;

        default:
            /* Comportamiento seguro: no hacer nada */
            break;
    }
}

/*=============================================================================
 *  Control de salidas digitales adicionales (DOUT)
 *============================================================================*/
void shield_setDout(shield_doutId_enum id, shield_msg_enum msg)
{
    uint32_t mask = (1U << shield_gpioDout[id].pin);

    /* Lógica activa en alto (active-high): Set=ON, Clear=OFF */
    switch (msg)
    {
        case SHIELD_MSG_OFF:
            GPIO_PortClear(shield_gpioDout[id].gpio, mask);
            break;

        case SHIELD_MSG_ON:
            GPIO_PortSet(shield_gpioDout[id].gpio, mask);
            break;

        case SHIELD_MSG_TOGGLE:
            // (No implementado)
            break;

        default:
            break;
    }
}

/*=============================================================================
 *  Lectura de pulsadores
 *============================================================================*/
bool shield_getSw(shield_swId_enum id)
{
    /* Active-low → se invierte la lectura */
    return !GPIO_PinRead(shield_gpioSw[id].gpio,
                         shield_gpioSw[id].pin);
}

/*==================[end of file]============================================*/
