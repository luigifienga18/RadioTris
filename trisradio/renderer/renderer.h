#ifndef TRISRADIO_RENDERER_H
#define TRISRADIO_RENDERER_H

#include "hal.h"
#include "../ge/ge.h"

void renderStart(void);
void renderGame(gameStatus_t *game, bool text_visible);
void renderMenu(gameStatus_t *game, bool text_visible);
void renderLogo(void);
void renderPlay(gameStatus_t *game, bool text_visible);
void renderX0(gameStatus_t *game, int i, int j);
void renderLineWin(gameStatus_t *game, bool text_visible);
void renderCursor(gameStatus_t *game, bool text_visible);
void renderMenuMultiplayer(gameStatus_t *game, bool text_visible);

#endif
