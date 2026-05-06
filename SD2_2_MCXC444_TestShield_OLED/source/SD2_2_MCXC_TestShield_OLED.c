/*=============================================================================
 *  SD2_OLED_example - Ejemplo de uso del display OLED con el SD2 Shield
 *-----------------------------------------------------------------------------
 *  Institución : DSI - FCEIA - UNR
 *  Autor       : Walter S. Wagner
 *  Año         : 2026
 *
 *  Descripción:
 *  ------------
 *  Ejemplo didáctico de inicialización y uso del display OLED gráfico del
 *  shield de prácticas. Demuestra cómo alternar entre dos pantallas usando
 *  primitivas gráficas del driver oled (rectángulos, texto, círculos).
 *============================================================================*/

/*==================[inclusions]=============================================*/

#include "SD2_board.h"
#include "SD2_shield.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "oled.h"

/*==================[macros and definitions]=================================*/

#define TIEMPO_OLED 2000

typedef enum
{
    OLED_DISPLAY_1 = 0,
    OLED_DISPLAY_2,
} estDisplay_enum;

/*==================[internal data declaration]==============================*/

static int32_t Timer_OLED; // Temporizador en ms
static estDisplay_enum Display_Act = OLED_DISPLAY_1; // Estado de la pantalla a mostrar

/*==================[internal functions declaration]=========================*/

/**
 * @brief Dibuja la primer pantalla en el display OLED
 *
 * La pantalla contiene un rectángulo blanco con fondo negro,
 * texto y un círculo blanco.
 */
void Oled_Display_1(void)
{
    oled_clearScreen(OLED_COLOR_BLACK);
    oled_fillRect(32, 16, 32+64, 16+32, OLED_COLOR_WHITE);
    oled_fillRect(32+8, 16+8, 32+64-8, 16+32-8, OLED_COLOR_BLACK);
    oled_putString(41, 29, (uint8_t *)"MCXC-444", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_circle(64, 32, 31, OLED_COLOR_WHITE);
}

/**
 * @brief Dibuja la segunda pantalla en el display OLED
 *
 * La pantalla contiene un rectángulo negro con fondo blanco,
 * texto y un círculo negro.
 */
void Oled_Display_2(void)
{
    oled_clearScreen(OLED_COLOR_WHITE);
    oled_fillRect(32, 16, 32+64, 16+32, OLED_COLOR_BLACK);
    oled_fillRect(32+8, 16+8, 32+64-8, 16+32-8, OLED_COLOR_WHITE);
    oled_putString(41, 29, (uint8_t *)" SHIELD", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_circle(64, 32, 31, OLED_COLOR_BLACK);
}

/*==================[main]===================================================*/

int main(void)
{
    /* Inicialización de hardware base y periféricos */
    board_init();
    shield_init();              // Inicialización del hardware del shield
    shield_configSPI0();        // Inicialización del SPI0 para el display OLED

    SysTick_Config(SystemCoreClock / 1000U);
    Timer_OLED = TIEMPO_OLED;

    oled_init();                // Inicializa el driver y la pantalla OLED
    oled_setContrast(16);       // Configura el contraste del display
    Oled_Display_2();           // Presenta pantalla inicial

    while(1) {
        /* Cambia de pantalla cada TIEMPO_OLED ms */
        if (Timer_OLED == 0)
        {
            Timer_OLED = TIEMPO_OLED;
            switch (Display_Act)
            {
                case OLED_DISPLAY_1:
                    Oled_Display_1();
                    Display_Act = OLED_DISPLAY_2;
                    break;

                case OLED_DISPLAY_2:
                    Oled_Display_2();
                    Display_Act = OLED_DISPLAY_1;
                    break;

                default:
                    Display_Act = OLED_DISPLAY_1;
                    break;
            }
        }
    }

    return 0;
}

/*==================[ISR SysTick]============================================*/

/**
 * @brief Interrupción SysTick, decrementa el temporizador para cambio de pantalla
 */
void SysTick_Handler(void)
{
    if (Timer_OLED > 0) {
        Timer_OLED--;
    }
}

/*==================[end of file]============================================*/
