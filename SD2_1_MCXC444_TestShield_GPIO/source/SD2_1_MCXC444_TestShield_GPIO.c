#include "SD2_board.h"
#include "SD2_shield.h"
#include "board.h"

/*=============================================================================
 * Ejemplo de uso para SD2_SHIELD
 *-----------------------------------------------------------------------------
 * - Encender todos los LEDs y el relé cuando se presiona SW2.
 * - Apagar todos los LEDs y el relé cuando se presiona SW3.
 * - El buzzer alterna entre encendido/apagado cada vez que SW2 es presionado.
 *============================================================================*/

int main(void)
{
    /* Inicialización de hardware básico de la placa */
    BOARD_BootClockRUN();
    board_init();
    shield_init();
    BOARD_InitDebugConsole();

    while (1)
    {


        if (shield_getSw(SHIELD_SW_ID_2))
        {
            shield_setLed(SHIELD_LED_ID_1, SHIELD_MSG_ON);
            shield_setLed(SHIELD_LED_ID_2, SHIELD_MSG_ON);
            shield_setLed(SHIELD_LED_ID_3, SHIELD_MSG_ON);
            shield_setDout(SHIELD_DOUT_ID_RELAY, SHIELD_MSG_ON);
            shield_setDout(SHIELD_DOUT_ID_BUZZER, SHIELD_MSG_ON);
        }
        if (shield_getSw(SHIELD_SW_ID_3))
        {
            shield_setLed(SHIELD_LED_ID_1, SHIELD_MSG_OFF);
            shield_setLed(SHIELD_LED_ID_2, SHIELD_MSG_OFF);
            shield_setLed(SHIELD_LED_ID_3, SHIELD_MSG_OFF);
            shield_setDout(SHIELD_DOUT_ID_RELAY, SHIELD_MSG_OFF);
            shield_setDout(SHIELD_DOUT_ID_BUZZER, SHIELD_MSG_OFF);
        }
    }
}
