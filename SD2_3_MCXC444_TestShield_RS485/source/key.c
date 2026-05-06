/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * Copyright 2023, Guido Cicconi
 * Copyright 2026, Walter S. Wagner
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * (ver detalles en el archivo .h)
 */

#include "key.h"

/*==================[macros and definitions]=================================*/
typedef enum
{
    ESPERANDO_ACTIVACION = 0,
    ESPERANDO_DESACTIVACION
} estPul_enum;

#define MAX_SW_BOARD    (BOARD_SW_ID_TOTAL)
#define MAX_SW_SHIELD   (SHIELD_SW_ID_TOTAL)

/* Estructuras duplicadas para board y para shield ------------------------- */
static estPul_enum estSW_board[MAX_SW_BOARD];
static int32_t timerSW_board[MAX_SW_BOARD];
static bool pressSW_board[MAX_SW_BOARD];
static bool releaseSW_board[MAX_SW_BOARD];

static estPul_enum estSW_shield[MAX_SW_SHIELD];
static int32_t timerSW_shield[MAX_SW_SHIELD];
static bool pressSW_shield[MAX_SW_SHIELD];
static bool releaseSW_shield[MAX_SW_SHIELD];

/*==================[internal functions definition]==========================*/

/* Helper macros para acceso indirecto segun origen (board/shield) */
#define _ESTSW(src)      ((src)==KEY_SRC_BOARD ? estSW_board : estSW_shield)
#define _TIMERSW(src)    ((src)==KEY_SRC_BOARD ? timerSW_board : timerSW_shield)
#define _PRESSSW(src)    ((src)==KEY_SRC_BOARD ? pressSW_board : pressSW_shield)
#define _RELEASESW(src)  ((src)==KEY_SRC_BOARD ? releaseSW_board : releaseSW_shield)

#define _MAX_SW(src)     ((src)==KEY_SRC_BOARD ? MAX_SW_BOARD : MAX_SW_SHIELD)

/* Llamadas a getSw para cada origen */
static inline bool _getSw(key_src_enum src, int i)
{
    if (src == KEY_SRC_BOARD)
        return board_getSw(i);
    else
        return shield_getSw(i);
}

/*==================[API definition]=========================================*/

void key_init(key_src_enum origen)
{
    int32_t max = _MAX_SW(origen);
    estPul_enum *est = _ESTSW(origen);
    bool *press = _PRESSSW(origen);
    bool *release = _RELEASESW(origen);
    int32_t* timer = _TIMERSW(origen);

    for (int32_t i = 0 ; i < max ; i++)
    {
        est[i] = ESPERANDO_ACTIVACION;
        press[i] = 0;
        release[i] = 0;
        timer[i] = 0;
    }
}

void key_periodicTask1ms(key_src_enum origen)
{
    int32_t max = _MAX_SW(origen);
    estPul_enum *est = _ESTSW(origen);
    bool *press = _PRESSSW(origen);
    bool *release = _RELEASESW(origen);
    int32_t* timer = _TIMERSW(origen);

    for (int32_t i = 0; i < max; i++) {
        switch (est[i]) {
        case ESPERANDO_ACTIVACION:
            if (_getSw(origen, i)) {
                press[i] = 1;
                est[i] = ESPERANDO_DESACTIVACION;
            }
            break;
        case ESPERANDO_DESACTIVACION:
            timer[i]++;
            if (!_getSw(origen, i)) {
                release[i] = 1;
                est[i] = ESPERANDO_ACTIVACION;
                timer[i]=0;
            }
            break;
        default:
            est[i] = ESPERANDO_ACTIVACION;
            break;
        }
    }
}

bool key_getPressEv(key_src_enum origen, int32_t id)
{
    bool *press = _PRESSSW(origen);
    bool ret = false;
    if (press[id])
    {
        press[id] = 0;
        ret = true;
    }
    return ret;
}

bool key_getReleaseEv(key_src_enum origen, int32_t id)
{
    bool *release = _RELEASESW(origen);
    bool ret = false;

    if (release[id])
    {
        release[id] = 0;
        ret = true;
    }
    return ret;
}

bool key_getPressed(key_src_enum origen, int32_t id, int32_t timeMs)
{
    int32_t* timer = _TIMERSW(origen);
    if (timer[id] >= timeMs) return true;
    else return false;
}

void key_clearFlags(key_src_enum origen, int32_t id)
{
    bool *press = _PRESSSW(origen);
    bool *release = _RELEASESW(origen);
    press[id] = 0;
    release[id] = 0;
}

/*==================[end of file]============================================*/
