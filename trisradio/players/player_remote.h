#ifndef TRIS_PLAYER_REMOTE_H
#define TRIS_PLAYER_REMOTE_H

#include "hal.h"
#include "../ge/ge.h"
#include "rf.h"
#include "player.h"

typedef struct playerRemote_t
{
  player_t player;
  RFDriver *rfp;
}playerRemote_t;

void playerRemote_init(playerRemote_t *, playerId, RFDriver *);
void playerRemote_deInit(playerRemote_t *);

// Master functions
void playerRemote_getAction(playerRemote_t *, action_t *);
void playerRemote_waitForReq2SendAction(playerRemote_t *);
void playerRemote_sendGameState(playerRemote_t *, gameStatus_t *);

// Slave functions
void playerRemote_sendAction(playerRemote_t *, action_t *);
void playerRemote_getGameState(playerRemote_t *, gameStatus_t *);

#endif
