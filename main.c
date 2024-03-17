/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "trisradio/ge/ge.h"
#include "trisradio/renderer/renderer.h"
#include "trisradio/players/player_joystick.h"
#include "trisradio/players/player_remote.h"

/* DEFINITION OF THE ADC's */

#define BTN1_PORT     GPIOB
#define BTN1_PIN      5U
#define BTN1_LINE     PAL_LINE( BTN1_PORT, BTN1_PIN )

#define BTN2_PORT     GPIOA
#define BTN2_PIN      10U
#define BTN2_LINE     PAL_LINE( BTN2_PORT, BTN2_PIN )

static ADCConversionGroup adcgrpcfg1 = {
  .circular     = false,
  .num_channels = 2,
  .end_cb       = NULL,
  .error_cb     = NULL, //NULL se non prevista
  .cfgr         = ADC_CFGR_CONT,
  .cfgr2        = 0U,
  .tr1          = ADC_TR_DISABLED,
  .tr2          = ADC_TR_DISABLED,
  .tr3          = ADC_TR_DISABLED,
  .awd2cr       = 0U,
  .awd3cr       = 0U,
  .smpr         = {
    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5) | //clock del circuito Sample & Hold
    ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_247P5),
    0U
  },
  .sqr = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2),
    0U,
    0U,
    0U
  }
};

static ADCConversionGroup adcgrpcfg2 =
{
  .circular     = false,
  .num_channels = 2,
  .end_cb       = NULL,
  .error_cb     = NULL, //NULL se non prevista
  .cfgr         = ADC_CFGR_CONT,
  .cfgr2        = 0U,
  .tr1          = ADC_TR_DISABLED,
  .tr2          = ADC_TR_DISABLED,
  .tr3          = ADC_TR_DISABLED,
  .awd2cr       = 0U,
  .awd3cr       = 0U,
  .smpr         = {
    ADC_SMPR2_SMP_AN17(ADC_SMPR_SMP_247P5) | //clock del circuito Sample & Hold
    ADC_SMPR1_SMP_AN5(ADC_SMPR_SMP_247P5),
    0U
  },
  .sqr = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN17) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN5),
    0U,
    0U,
    0U
  }
};

/* END OF ADC's DEFS */

/* DEFINITION OF THE RF */

#define  NRF24L01_LINE_CE                     PAL_LINE( GPIOA, 9U)
#define  NRF24L01_SPI_CS                      PAL_LINE( GPIOB, 6U)
#define  NRF24L01_SPI_SCK                     PAL_LINE( GPIOA, 5U)
#define  NRF24L01_SPI_MISO                    PAL_LINE( GPIOA, 6U)
#define  NRF24L01_SPI_MOSI                    PAL_LINE( GPIOA, 7U)

#define  NRF24L01_LINE_IRQ                    PAL_LINE( GPIOC, 7U)
#define  FRAME_LEN                            5

static SPIConfig std_spi_cfg = {
  .circular=FALSE,
  .slave=FALSE,
  .data_cb=NULL,
  .error_cb=NULL,
  .ssport=GPIOB,
  .sspad=6U,                                       /* Line of CS. */
  SPI_CR1_BR_1 | SPI_CR1_BR_0,                    /* CR1 register */
  0                                               /* CR2 register */
};

static RFConfig nrf24l01_cfg = {
  NRF24L01_LINE_CE,
  NRF24L01_LINE_IRQ,
  &SPID1,
  &std_spi_cfg,
  NRF24L01_ARC_15_times,     /* auto_retr_count */
  NRF24L01_ARD_4000us,       /* auto_retr_delay */
  NRF24L01_AW_5_bytes,       /* address_width */
  42,                       /* channel_freq 2.4 + 0.13 GHz */
  NRF24L01_ADR_2Mbps,        /* data_rate */
  NRF24L01_PWR_0dBm,         /* out_pwr */
  NRF24L01_LNA_disabled,     /* lna */
  NRF24L01_DPL_enabled ,     /* en_dpl */
  NRF24L01_ACK_PAY_disabled, /* en_ack_pay */
  NRF24L01_DYN_ACK_disabled  /* en_dyn_ack */
};

/* END OF RF DEFS */

/* MAIN DEFS */

#define LOCKED_APPLYMOVE(mtx, game, action) do                                  \
                                            {                                   \
                                              chMtxLock((mtx));                 \
                                              ge_applyAction((game), (action)); \
                                              chMtxUnlock((mtx));               \
                                            }                                   \
                                            while(0)

#define LOCKED_GE_INIT(mtx, game) do                    \
                                  {                     \
                                    chMtxLock((mtx));   \
                                    ge_init((game));    \
                                    chMtxUnlock((mtx)); \
                                  }                     \
                                  while(0)

#define SLAVE_GET_GAMESTATE(rPlayer, game) do                                               \
                                            {                                               \
                                              playerRemote_getGameState((rPlayer), (game)); \
                                              if((game)->g_status == IN_GAME_ONLINE_MASTER) \
                                                (game)->g_status = IN_GAME_ONLINE_SLAVE;    \
                                            }                                               \
                                            while(0)

#define LOCKED_SWAP_GAMESTATE(mtx, dst, src) do                           \
                                            {                             \
                                              chMtxLock((mtx));           \
                                              ge_swapStates((dst),(src)); \
                                              chMtxUnlock((mtx));         \
                                            }                             \
                                            while(0)

static gameStatus_t game;
static mutex_t game_mutex;

static THD_WORKING_AREA(renderWA, 1024);
static THD_FUNCTION(renderTh, arg) {

  chRegSetThreadName("Render Thread");
  gameStatus_t *gamep = (gameStatus_t *)arg;

  // Inizializzazione del display
  renderStart();
  // Renderizzi il logo per 4 secondi
  renderLogo();
  chThdSleepMilliseconds(4000);
  bool text_visible = true;

  // ciclo while che renderizza il game
  while(true)
  {
    // rendergame
    chMtxLock(&game_mutex);

    renderGame(gamep,text_visible);

    chMtxUnlock(&game_mutex);
  // attesa di mezzo secondo
    text_visible=!text_visible;

    chThdSleepMilliseconds(500);
  }

}

/*
 * Application entry point.
 */
int main(void)
{
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  chMtxObjectInit(&game_mutex);

  /* PRE GE START INIT's */
  /* ADC inputs.*/
  palSetPadMode(GPIOA, 2U, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3U, PAL_MODE_ALTERNATE(7));

  //Joystick del player 1
  palSetPadMode(GPIOA, 0U, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA, 1U, PAL_MODE_INPUT_ANALOG);

  //Joystick del player 2
  palSetPadMode(GPIOA, 4U, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOC, 4U, PAL_MODE_INPUT_ANALOG);

  /*BUTTON inputs.*/

  //Tasto del player 1
  palSetLineMode( BTN1_LINE, PAL_MODE_INPUT_PULLUP );

  //Tasto del player 2
  palSetLineMode( BTN2_LINE, PAL_MODE_INPUT_PULLUP );

  //Inizio della conversione sui circuiti ADC (la scheda dovrebbe averne 3)
  adcStart(&ADCD1, NULL);
  adcStart(&ADCD2, NULL);

  /*
   * SPID1 I/O pins setup.(It bypasses board.h configurations)
   */
  palSetLineMode(NRF24L01_SPI_SCK,  PAL_MODE_ALTERNATE(5) |
                                    PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_SPI_MISO, PAL_MODE_ALTERNATE(5) |
                                    PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_SPI_MOSI, PAL_MODE_ALTERNATE(5) |
                                    PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_SPI_CS,   PAL_MODE_OUTPUT_PUSHPULL |
                                    PAL_STM32_OSPEED_HIGHEST);
  /*
   * CE and IRQ pins setup.
   */
  palSetLineMode(NRF24L01_LINE_CE,  PAL_MODE_OUTPUT_PUSHPULL |
                                    PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_LINE_IRQ, PAL_MODE_INPUT |
                                    PAL_STM32_OSPEED_HIGHEST);

  /* Starting Serial Driver 2 with default configurations. */
  palSetPadMode( GPIOA, 2, PAL_MODE_ALTERNATE(7) );
  palSetPadMode( GPIOA, 3, PAL_MODE_ALTERNATE(7) );

  /* RF Driver Object constructor. */
  rfInit();

  /* Starting RF driver. */
  rfStart(&RFD1, &nrf24l01_cfg);

  /* PRE GE END INIT's */

  ge_init(&game);

  playerJoystick_t jPlayer1;
  playerJoystick_init(&jPlayer1, PLAYER_1_MASTER, &ADCD1, &adcgrpcfg1, BTN1_LINE);

  action_t p_action;

  chThdCreateStatic(renderWA, sizeof(renderWA), NORMALPRIO+1, renderTh, (void *)&game);

  while(true)
  {

    switch(game.g_status)
    {
      case IN_MAIN_MENU:
      case IN_ONLINE_MULTIPLAYER_MENU:
      {
        while(game.g_status == IN_MAIN_MENU || game.g_status == IN_ONLINE_MULTIPLAYER_MENU)
        {
          playerJoystick_getAction(&jPlayer1, &p_action);
          LOCKED_APPLYMOVE(&game_mutex, &game, &p_action);
        }
      }break;

      case INVALID_STATUS:
      {
        // This should not happen
      }break;

      case IN_GAME_OFFLINE:
      {
        playerJoystick_t jPlayer2;
        playerJoystick_init(&jPlayer2, PLAYER_2_SLAVE, &ADCD2, &adcgrpcfg2, BTN2_LINE);
        while(game.g_status == IN_GAME_OFFLINE)
        {
          while( game.g_status == IN_GAME_OFFLINE && game.playing_player.id == jPlayer1.player.id)
          {
            playerJoystick_getAction(&jPlayer1, &p_action);
            LOCKED_APPLYMOVE(&game_mutex, &game, &p_action);
          }
          while(game.g_status == IN_GAME_OFFLINE && game.playing_player.id == jPlayer2.player.id)
          {
            playerJoystick_getAction(&jPlayer2, &p_action);
            LOCKED_APPLYMOVE(&game_mutex, &game, &p_action);
          }
        }
        LOCKED_GE_INIT(&game_mutex, &game);
      }break;

      case IN_GAME_ONLINE_MASTER:
      {
        playerRemote_t rPlayer;
        playerRemote_init(&rPlayer, PLAYER_2_SLAVE, &RFD1);

        while(game.g_status == IN_GAME_ONLINE_MASTER)
        {
          if(game.g_game == END_GAME)
          {
            do
            {
              playerJoystick_getAction(&jPlayer1, &p_action);
            }
            while(p_action.typeOfAction != JOYSTICK_CLICK);
            break;
          }
          while(game.g_status == IN_GAME_ONLINE_MASTER && game.playing_player.id == jPlayer1.player.id)
          {
            playerJoystick_getAction(&jPlayer1, &p_action);
            LOCKED_APPLYMOVE(&game_mutex, &game, &p_action);
            if(game.g_game == END_GAME)
              playerRemote_sendGameState(&rPlayer, &game);
          }
          while(game.g_status == IN_GAME_ONLINE_MASTER && game.g_game == IN_GAME &&
                game.playing_player.id == rPlayer.player.id)
          {
            playerRemote_sendGameState(&rPlayer, &game);
            playerRemote_getAction(&rPlayer, &p_action);
            LOCKED_APPLYMOVE(&game_mutex, &game, &p_action);
            if(game.playing_player.id != rPlayer.player.id || game.g_game == END_GAME)
              playerRemote_sendGameState(&rPlayer, &game);
          }
        }
        LOCKED_GE_INIT(&game_mutex, &game);
      }break;

      case IN_GAME_ONLINE_SLAVE:
      {
        gameStatus_t slave_gamestate;
        jPlayer1.player.id = PLAYER_2_SLAVE;
        playerRemote_t rPlayer;
        playerRemote_init(&rPlayer, PLAYER_1_MASTER, &RFD1);

        while(game.g_status == IN_GAME_ONLINE_SLAVE)
        {
          SLAVE_GET_GAMESTATE(&rPlayer, &slave_gamestate);
          LOCKED_SWAP_GAMESTATE(&game_mutex, &game, &slave_gamestate);

          if(game.g_game == END_GAME)
          {
            do
            {
              playerJoystick_getAction(&jPlayer1, &p_action);
            }
            while(p_action.typeOfAction != JOYSTICK_CLICK);
            break;
          }

          if(game.g_status == IN_GAME_ONLINE_SLAVE && game.playing_player.id == PLAYER_2_SLAVE)
          {
            playerRemote_waitForReq2SendAction(&rPlayer);
            playerJoystick_getAction(&jPlayer1, &p_action);
            playerRemote_sendAction(&rPlayer, &p_action);
          }
        }
        LOCKED_GE_INIT(&game_mutex, &game);
        jPlayer1.player.id = PLAYER_1_MASTER;
      }break;

    }

  }
}
