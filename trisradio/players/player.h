#ifndef TRIS_PLAYER_H
#define TRIS_PLAYER_H

typedef enum playerId
{
  PLAYER_1_MASTER,
  PLAYER_2_SLAVE
}playerId;

// NOTA: Aggiornare le firme delle funzioni che prendono in ingresso id
typedef struct player_t
{
  playerId id;
}player_t;

#endif
