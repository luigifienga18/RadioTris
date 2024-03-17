#ifndef TRISRADIO_GE_H
#define TRISRADIO_GE_H

#include "ch.h"
#include "../players/player.h"

/*
 * The trisradio global status.
 * This is used to determine whether we are
 * in the main menu or in game
 */
typedef enum globalStatus
{
  INVALID_STATUS,
  IN_MAIN_MENU,
  IN_ONLINE_MULTIPLAYER_MENU,
  IN_GAME_OFFLINE,
  IN_GAME_ONLINE_MASTER,
  IN_GAME_ONLINE_SLAVE,
}globalStatus;

typedef enum globalGame
{
  IN_GAME,  // Quando sei in una partita, qualsiasi essa sia
  END_GAME, // Quando sei in partita ma è terminata
  NG        // Quando non sono in gioco oppure dopo END_GAME l'utente compie l'azione di CLICK
}globalGame;

typedef enum hoverElement
{
  MENU_FIRSTLINE,
  MENU_SECONDLINE,
  IN_GAME_11,IN_GAME_12,IN_GAME_13,
  IN_GAME_21,IN_GAME_22,IN_GAME_23,
  IN_GAME_31,IN_GAME_32,IN_GAME_33
}hoverElement;

typedef enum trisValue
{
  NA, //No elem
  X,
  C
}trisValue;

typedef enum winSituation
{
  TRIS_123,
  TRIS_159,
  TRIS_147,
  TRIS_258,
  TRIS_357,
  TRIS_369,
  TRIS_456,
  TRIS_789,
  DRAW
}winSituation;

typedef enum actionType
{
  JOYSTICK_UP,
  JOYSTICK_UP_RIGHT,
  JOYSTICK_RIGHT,
  JOYSTICK_DOWN_RIGHT,
  JOYSTICK_DOWN,
  JOYSTICK_DOWN_LEFT,
  JOYSTICK_LEFT,
  JOYSTICK_UP_LEFT,
  JOYSTICK_CLICK
}actionType;

typedef struct action_t
{
  player_t player;
  actionType typeOfAction;
}action_t;

typedef struct gameStatus_t
{
  globalStatus g_status;
  globalGame g_game;
  hoverElement h_elem;
  player_t playing_player;
  player_t winner_player;
  winSituation win_situation;
  trisValue tris_matrix[3][3];
  uint8_t counter;

}gameStatus_t;

void ge_init(gameStatus_t *);
void ge_deInit(gameStatus_t *);
void ge_applyAction(gameStatus_t *, action_t *);
void ge_swapStates(gameStatus_t *, gameStatus_t *);


#endif
