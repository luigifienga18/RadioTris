#ifndef TRIS_PLAYER_JOYSTICK_H
#define TRIS_PLAYER_JOYSTICK_H

#include "../ge/ge.h"
#include "hal.h"
#include "player.h"

#define ADC_GRP_BUF_DEPTH 16
#define VOLTAGE_RES 3.3/4096

typedef struct playerJoystick_t
{
  player_t player;
  ADCDriver *joystick_adcDriver;
  ADCConversionGroup *joystick_adcCG;
  ioline_t joystick_button_line;
}playerJoystick_t;

void playerJoystick_init(playerJoystick_t *, playerId, ADCDriver *, ADCConversionGroup *, ioline_t );
void playerJoystick_deInit(playerJoystick_t *);
void playerJoystick_getAction(playerJoystick_t *, action_t *);

#endif
