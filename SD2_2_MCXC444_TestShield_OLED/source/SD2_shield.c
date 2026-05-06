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
#include "fsl_spi.h"

/*==================[macros and definitions]=================================*/

#define SPI_MASTER              SPI0
#define SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)

/*=============================================================================
 *  Mapeo hardware (HW Binding Layer)
 *-----------------------------------------------------------------------------
 *  Vincula IDs lógicos con recursos físicos reales del microcontrolador,
 *  permitiendo modificar el hardware sin impacto en la API pública.
 *============================================================================*/

/**
 * @brief Tabla de mapeo para LEDs
 */
static const shield_gpioInfo_type shield_gpioLeds[] = {
    {PORTE, GPIOE, 20},     /* LED1 */
    {PORTE, GPIOE, 21},     /* LED2 */
    {PORTE, GPIOE, 22}      /* LED3 */
};

/**
 * @brief Tabla de mapeo para pulsadores
 */
static const shield_gpioInfo_type shield_gpioSw[] = {
    {PORTB, GPIOB, 17},     /* SW2 */
    {PORTB, GPIOB, 16}      /* SW3 */
};

/**
 * @brief Tabla de mapeo para salidas digitales adicionales (DOUT)
 */
static const shield_gpioInfo_type shield_gpioDout[] = {
    {PORTE, GPIOE, 0},      /* RELÉ */
    {PORTE, GPIOE, 30}      /* BUZZER */
};

/**
 * @brief Tabla de mapeo para pines de control de OLED (RST, DC, CS)
 */
static const shield_gpioInfo_type shield_gpioOled[] = {
    {PORTB, GPIOB, 3},      /* RST */
    {PORTB, GPIOB, 2},      /* DATA/CMD */
    {PORTD, GPIOD, 2},      /* CS (manual) */
};

/*==================[internal data definition]===============================*/

static spi_master_handle_t handle;
static volatile bool masterFinished = false;


/*=============================================================================
 *  Inicialización de hardware
 *============================================================================*/
void shield_init(void)
{
    int32_t i;

    /*---------------------------------------------------------------------
     * Configuración GPIO para LEDs (activo-bajo)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_led_config = {
        .outputLogic  = 1U,                    /* Estado inicial: OFF (active-low) */
        .pinDirection = kGPIO_DigitalOutput,
    };

    /*---------------------------------------------------------------------
     * Configuración GPIO para salidas DOUT (activo-alto)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_dout_config = {
        .outputLogic  = 0U,                    /* Estado inicial: OFF (active-high) */
        .pinDirection = kGPIO_DigitalOutput,
    };

    /*---------------------------------------------------------------------
     * Configuración GPIO para SPI OLED DISPLAY (RST - DC - CS)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_oled_config = {
        .outputLogic = 0, // RST y DC, nivel bajo al arrancar (OFF)
        .pinDirection = kGPIO_DigitalOutput,
    };

    gpio_pin_config_t gpio_oled_cs_config = {
        .outputLogic = 1, // CS idle en alto
        .pinDirection = kGPIO_DigitalOutput,
    };

    /*---------------------------------------------------------------------
     * Configuración GPIO para pulsadores (entrada)
     *--------------------------------------------------------------------*/
    gpio_pin_config_t gpio_in_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U
    };

    /*---------------------------------------------------------------------
     * Configuración PORT para salidas (LEDs, DOUT, OLED)
     *--------------------------------------------------------------------*/
    const port_pin_config_t port_out_config = {
        .pullSelect          = kPORT_PullDisable,
        .slewRate            = kPORT_SlowSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .driveStrength       = kPORT_LowDriveStrength,
        .mux                 = kPORT_MuxAsGpio,
    };

    const port_pin_config_t port_oled_config = {
        .pullSelect          = kPORT_PullDisable,
        .slewRate            = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .driveStrength       = kPORT_LowDriveStrength,
        .mux                 = kPORT_MuxAsGpio,
    };

    /*---------------------------------------------------------------------
     * Configuración PORT para pulsadores
     *--------------------------------------------------------------------*/
    const port_pin_config_t port_sw_config = {
        .pullSelect          = kPORT_PullUp,
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
    for (i = 0; i < SHIELD_LED_ID_TOTAL; i++) {
        PORT_SetPinConfig(shield_gpioLeds[i].port, shield_gpioLeds[i].pin, &port_out_config);
        GPIO_PinInit(shield_gpioLeds[i].gpio, shield_gpioLeds[i].pin, &gpio_led_config);
    }

    /*---------------------------------------------------------------------
     * Inicialización de salidas digitales adicionales (DOUT)
     *--------------------------------------------------------------------*/
    for (i = 0; i < SHIELD_DOUT_ID_TOTAL; i++) {
        PORT_SetPinConfig(shield_gpioDout[i].port, shield_gpioDout[i].pin, &port_out_config);
        GPIO_PinInit(shield_gpioDout[i].gpio, shield_gpioDout[i].pin, &gpio_dout_config);
    }

    /*---------------------------------------------------------------------
     * Inicialización de pulsadores
     *--------------------------------------------------------------------*/
    for (i = 0; i < SHIELD_SW_ID_TOTAL; i++) {
        PORT_SetPinConfig(shield_gpioSw[i].port, shield_gpioSw[i].pin, &port_sw_config);
        GPIO_PinInit(shield_gpioSw[i].gpio, shield_gpioSw[i].pin, &gpio_in_config);
    }

    /*---------------------------------------------------------------------
     * Inicialización pines OLED DISPLAY (RST, DC, CS)
     *--------------------------------------------------------------------*/
    for (i = 0; i < OLED_TOTAL; i++) {
        PORT_SetPinConfig(shield_gpioOled[i].port, shield_gpioOled[i].pin, &port_oled_config);
        if(i == OLED_CS) {
            GPIO_PinInit(shield_gpioOled[i].gpio, shield_gpioOled[i].pin, &gpio_oled_cs_config);
        } else {
            GPIO_PinInit(shield_gpioOled[i].gpio, shield_gpioOled[i].pin, &gpio_oled_config);
        }
    }
}

/*=============================================================================
 *  Control de LEDs
 *============================================================================*/
void shield_setLed(shield_ledId_enum id, shield_msg_enum msg)
{
    uint32_t mask = (1U << shield_gpioLeds[id].pin);

    switch (msg) {
        case SHIELD_MSG_OFF:
            GPIO_PortSet(shield_gpioLeds[id].gpio, mask);
            break;
        case SHIELD_MSG_ON:
            GPIO_PortClear(shield_gpioLeds[id].gpio, mask);
            break;
        case SHIELD_MSG_TOGGLE:
            GPIO_PortToggle(shield_gpioLeds[id].gpio, mask);
            break;
        default:
            break;
    }
}

/*=============================================================================
 *  Control de salidas digitales adicionales (DOUT)
 *============================================================================*/
void shield_setDout(shield_doutId_enum id, shield_msg_enum msg)
{
    uint32_t mask = (1U << shield_gpioDout[id].pin);

    switch (msg) {
        case SHIELD_MSG_OFF:
            GPIO_PortClear(shield_gpioDout[id].gpio, mask);
            break;
        case SHIELD_MSG_ON:
            GPIO_PortSet(shield_gpioDout[id].gpio, mask);
            break;
        case SHIELD_MSG_TOGGLE:
            // No implementado para DOUT
            break;
        default:
            break;
    }
}

/*=============================================================================
 *  Control pines de OLED (RST, DC, CS)
 *============================================================================*/
void shield_setOledPin(shield_oledPin_enum oledPin, uint8_t state)
{
    GPIO_PinWrite(shield_gpioOled[oledPin].gpio, shield_gpioOled[oledPin].pin, state);
}

/*=============================================================================
 *  Lectura de pulsadores
 *============================================================================*/
bool shield_getSw(shield_swId_enum id)
{
    return !GPIO_PinRead(shield_gpioSw[id].gpio, shield_gpioSw[id].pin);
}

/*=============================================================================
 *  Configuración SPI0 para OLED
 *============================================================================*/
void shield_configSPI0(void)
{
    const port_pin_config_t port_spi_config = {
        .pullSelect = kPORT_PullDisable,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .driveStrength = kPORT_LowDriveStrength,
        .mux = kPORT_MuxAlt2,
    };

    // SS se maneja por GPIO, no utilizar SPI SS HW
    //PORT_SetPinConfig(PORTC, 4, &port_spi_config); //SPI0_SS

    PORT_SetPinConfig(PORTC, 5, &port_spi_config); //SPI0_SCK
    PORT_SetPinConfig(PORTC, 6, &port_spi_config); //SPI0_MOSI
    PORT_SetPinConfig(PORTC, 7, &port_spi_config); //SPI0_MISO

    CLOCK_EnableClock(kCLOCK_Spi0);

    spi_master_config_t userConfig;

    SPI_MasterGetDefaultConfig(&userConfig);

    // Configuración ajustada para OLED: modo, polaridad, fase y baudrate
    userConfig.outputMode = kSPI_SlaveSelectAsGpio;
    userConfig.polarity   = kSPI_ClockPolarityActiveLow;
    userConfig.phase      = kSPI_ClockPhaseSecondEdge;
    userConfig.baudRate_Bps = 4000000U;

    SPI_MasterInit(SPI_MASTER, &userConfig, SPI_MASTER_CLK_FREQ);
    SPI_MasterTransferCreateHandle(SPI_MASTER, &handle, NULL, NULL);
}

/*=============================================================================
 *  Envío de datos al OLED por SPI
 *============================================================================*/
void shield_SPISend(uint8_t* buf, size_t len)
{
    spi_transfer_t xfer;

    xfer.txData = buf;
    xfer.rxData = NULL;
    xfer.dataSize  = len;

    // Activa CS (selecciona el OLED)
    shield_setOledPin(OLED_CS, 0); // CS LOW (selecciona slave)

    SPI_MasterTransferNonBlocking(SPI_MASTER, &handle, &xfer);

    shield_setOledPin(OLED_CS, 1); // CS HIGH (libera slave)
    // También podés usar SPI_MasterTransferBlocking si la aplicación lo requiere
}

/*==================[end of file]============================================*/
