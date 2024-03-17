#include "player_joystick.h"

void playerJoystick_init(playerJoystick_t *p,
                         playerId id, ADCDriver * adc,
                         ADCConversionGroup *adcCG,
                         ioline_t joystick_button)
{
  p->player.id = id;
  p->joystick_adcDriver = adc;
  p->joystick_adcCG = adcCG;
  p->joystick_button_line = joystick_button;
}

void playerJoystick_deInit(playerJoystick_t *p)
{
  (void)p;
}

void playerJoystick_getAction(playerJoystick_t *p, action_t * action)
{
  action->player = p->player;
  adcsample_t samples[2*ADC_GRP_BUF_DEPTH];

  float X;
  float Y;

  while(true){

    if(palReadLine(p->joystick_button_line) == PAL_LOW){
      action->typeOfAction = JOYSTICK_CLICK;
      chThdSleepMilliseconds(200);
      return;
    }

    X = 0;
    Y = 0;

    adcConvert(p->joystick_adcDriver, p->joystick_adcCG, samples, ADC_GRP_BUF_DEPTH);

    for(int i = 0; i < 2 * ADC_GRP_BUF_DEPTH; i += 2)
    {
      X += (float) samples[i] * VOLTAGE_RES;
      Y += (float) samples[i+1] * VOLTAGE_RES;
    }

    X /= ADC_GRP_BUF_DEPTH;
    Y /= ADC_GRP_BUF_DEPTH;

    // if per calcolare la direzione del joystick (la actionType JOYSTICK_NOOP è rimasta inutilizzata)

    if(X < 1.8 && X > 1.6 && Y < 1.8 && Y > 1.6) //Posizione centrale (di riposo)
    {
      continue;
    }
    else if(X < 1.8 && X > 1.6 && Y > 3.0) //Posizione DOWN centrale
    {
      action->typeOfAction = JOYSTICK_DOWN;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X > 3.0 && Y < 1.8 && Y > 1.6) //Posizione RIGHT centrale
    {
      action->typeOfAction = JOYSTICK_RIGHT;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X < 1.8 && X > 1.6 && Y < 0.5) //Posizione UP centrale
    {
      action->typeOfAction = JOYSTICK_UP;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X < 0.5 && Y < 1.8 && Y > 1.6) //Posizione LEFT centrale
    {
      action->typeOfAction = JOYSTICK_LEFT;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X < 0.5 && Y > 3.0) //Posizione DOWN_LEFT
    {
      action->typeOfAction = JOYSTICK_DOWN_LEFT;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X > 3.0 && Y > 3.0) //Posizione DOWN_RIGHT
    {
      action->typeOfAction = JOYSTICK_DOWN_RIGHT;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X < 0.5 && Y < 0.5) //Posizione UP_LEFT
    {
      action->typeOfAction = JOYSTICK_UP_LEFT;
      chThdSleepMilliseconds(200);
      return;
    }
    else if(X > 3.0 && Y < 0.5) //Posizione UP_RIGHT
    {
      action->typeOfAction = JOYSTICK_UP_RIGHT;
      chThdSleepMilliseconds(200);
      return;
    }
  }
}
