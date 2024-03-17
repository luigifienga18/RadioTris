#include "ge.h"

#define P2S(playerId) ((playerId) == PLAYER_1_MASTER ? X : C)

void ge_init(gameStatus_t *game)
{
  game->g_status = IN_MAIN_MENU;
  game->g_game = NG;
  game->h_elem = MENU_FIRSTLINE;
  game->counter = 0;
  game->playing_player.id = PLAYER_1_MASTER;
}

void ge_deInit(gameStatus_t *game)
{
  // TDB
  (void)game;
}

void ge_swapStates(gameStatus_t *dst, gameStatus_t *src)
{
  dst->counter        = src->counter;
  dst->g_game         = src->g_game;
  dst->g_status       = src->g_status;
  dst->h_elem         = src->h_elem;
  dst->playing_player = src->playing_player;
  dst->win_situation  = src->win_situation;
  dst->winner_player  = src->winner_player;
  for(unsigned char i=0; i<3; i++)
    for(unsigned char j=0; j<3; j++)
        dst->tris_matrix[i][j] = src->tris_matrix[i][j];
}


static void ge_applyMove(gameStatus_t *game, action_t *action)
{
  switch(action->typeOfAction)
  {

    case JOYSTICK_UP:
    {
      if (game->h_elem == IN_GAME_31)
        game->h_elem = IN_GAME_21;
      else if (game->h_elem == IN_GAME_21)
        game->h_elem = IN_GAME_11;
      else if (game->h_elem == IN_GAME_32)
       game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_12;
      else if (game->h_elem == IN_GAME_33)
        game->h_elem = IN_GAME_23;
      else if (game->h_elem == IN_GAME_23)
        game->h_elem = IN_GAME_13;
    }break;
    case JOYSTICK_DOWN:
    {
      if (game->h_elem == IN_GAME_11)
        game->h_elem = IN_GAME_21;
      else if (game->h_elem == IN_GAME_21)
        game->h_elem = IN_GAME_31;
      else if (game->h_elem == IN_GAME_12)
        game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_32;
      else if (game->h_elem == IN_GAME_13)
        game->h_elem = IN_GAME_23;
      else if (game->h_elem == IN_GAME_23)
        game->h_elem = IN_GAME_33;
    }break;
    case JOYSTICK_LEFT:
    {
      if (game->h_elem == IN_GAME_13)
        game->h_elem = IN_GAME_12;
      else if (game->h_elem == IN_GAME_12)
        game->h_elem = IN_GAME_11;
      else if (game->h_elem == IN_GAME_23)
        game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_21;
      else if (game->h_elem == IN_GAME_33)
        game->h_elem = IN_GAME_32;
      else if (game->h_elem == IN_GAME_32)
        game->h_elem = IN_GAME_31;
    }break;
    case JOYSTICK_RIGHT:
    {
      if (game->h_elem == IN_GAME_11)
        game->h_elem = IN_GAME_12;
      else if (game->h_elem == IN_GAME_12)
         game->h_elem = IN_GAME_13;
      else if (game->h_elem == IN_GAME_21)
         game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_23;
      else if (game->h_elem == IN_GAME_31)
        game->h_elem = IN_GAME_32;
      else if (game->h_elem == IN_GAME_32)
        game->h_elem = IN_GAME_33;
    }break;
    default:
    {

    }break;
  }
}

static void ge_applyDiagonalMove(gameStatus_t *game, action_t *action)
{
  switch(action->typeOfAction)
  {

    case JOYSTICK_UP_RIGHT:
    {
      if (game->h_elem == IN_GAME_31)
        game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_13;
      else if (game->h_elem == IN_GAME_32)
        game->h_elem = IN_GAME_23;
      else if (game->h_elem == IN_GAME_21)
        game->h_elem = IN_GAME_12;
    }break;
    case JOYSTICK_DOWN_RIGHT:
    {
      if (game->h_elem == IN_GAME_11)
        game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_33;
      else if (game->h_elem == IN_GAME_12)
        game->h_elem = IN_GAME_23;
      else if (game->h_elem == IN_GAME_21)
        game->h_elem = IN_GAME_32;
    }break;
    case JOYSTICK_DOWN_LEFT:
    {
      if (game->h_elem == IN_GAME_13)
        game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_31;
      else if (game->h_elem == IN_GAME_12)
        game->h_elem = IN_GAME_21;
      else if (game->h_elem == IN_GAME_23)
        game->h_elem = IN_GAME_32;
    }break;
    case JOYSTICK_UP_LEFT:
    {
      if (game->h_elem == IN_GAME_33)
        game->h_elem = IN_GAME_22;
      else if (game->h_elem == IN_GAME_22)
        game->h_elem = IN_GAME_11;
      else if (game->h_elem == IN_GAME_23)
        game->h_elem = IN_GAME_12;
      else if (game->h_elem == IN_GAME_32)
        game->h_elem = IN_GAME_21;
    }break;
    default:
    {

    }break;
  }
}

static void ge_applyClick(gameStatus_t *game, action_t *action)
{

  if(game->g_game == END_GAME)
  {
    game->g_status = IN_MAIN_MENU;
    game->g_game = NG;
    game->h_elem = MENU_FIRSTLINE;
    return;
  }

  bool swap_turn = false;

  switch(game->h_elem)
  {
    case IN_GAME_11:
    {
      if(game->tris_matrix[0][0] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[0][0] = P2S(action->player.id);
        if(game->tris_matrix[1][0] == P2S(action->player.id) &&
              game->tris_matrix[2][0] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_147;
        }
        else if(game->tris_matrix[0][1] == P2S(action->player.id) &&
                  game->tris_matrix[0][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_123;
        }
        else if(game->tris_matrix[1][1] == P2S(action->player.id) &&
                  game->tris_matrix[2][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_159;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_12:
    {
      if(game->tris_matrix[0][1] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[0][1] = P2S(action->player.id);
        if(game->tris_matrix[0][0] == P2S(action->player.id) &&
            game->tris_matrix[0][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_123;
        }
        else if(game->tris_matrix[1][1] == P2S(action->player.id) &&
                  game->tris_matrix[2][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_258;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_13:
    {
      if(game->tris_matrix[0][2] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[0][2] = P2S(action->player.id);

        if(game->tris_matrix[0][0] == P2S(action->player.id) &&
            game->tris_matrix[0][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_123;
        }
        else if(game->tris_matrix[1][2] == P2S(action->player.id) &&
                  game->tris_matrix[2][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_369;
        }
        else if(game->tris_matrix[1][1] == P2S(action->player.id) &&
                  game->tris_matrix[2][0] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_357;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_21:
    {
      if(game->tris_matrix[1][0] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[1][0] = P2S(action->player.id);

        if(game->tris_matrix[1][1] == P2S(action->player.id) &&
            game->tris_matrix[1][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_456;
        }
        else if(game->tris_matrix[0][0] == P2S(action->player.id) &&
                  game->tris_matrix[2][0] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_147;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_22:
    {
      if(game->tris_matrix[1][1] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[1][1] = P2S(action->player.id);

        if(game->tris_matrix[1][0] == P2S(action->player.id) &&
            game->tris_matrix[1][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_456;
        }
        else if(game->tris_matrix[0][1] == P2S(action->player.id) &&
                  game->tris_matrix[2][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_258;
        }
        else if(game->tris_matrix[0][0] == P2S(action->player.id) &&
                  game->tris_matrix[2][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_159;
        }
        else if(game->tris_matrix[0][2] == P2S(action->player.id) &&
                          game->tris_matrix[2][0] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_357;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_23:
    {
      if(game->tris_matrix[1][2] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[1][2] = P2S(action->player.id);

        if(game->tris_matrix[1][0] == P2S(action->player.id) &&
             game->tris_matrix[1][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_456;
        }
        else if(game->tris_matrix[0][2] == P2S(action->player.id) &&
                  game->tris_matrix[2][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_369;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_31:
    {
      if(game->tris_matrix[2][0] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[2][0] = P2S(action->player.id);

        if(game->tris_matrix[2][1] == P2S(action->player.id) &&
             game->tris_matrix[2][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_789;
        }
        else if(game->tris_matrix[0][0] == P2S(action->player.id) &&
                  game->tris_matrix[1][0] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_147;
        }
        else if(game->tris_matrix[1][1] == P2S(action->player.id) &&
                  game->tris_matrix[0][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_357;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_32:
    {
      if(game->tris_matrix[2][1] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[2][1] = P2S(action->player.id);

        if(game->tris_matrix[2][0] == P2S(action->player.id) &&
             game->tris_matrix[2][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_789;
        }
        else if(game->tris_matrix[0][1] == P2S(action->player.id) &&
                  game->tris_matrix[1][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_258;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    case IN_GAME_33:
    {
      if(game->tris_matrix[2][2] == NA)
      {
        swap_turn = true;
        game->counter++;
        game->tris_matrix[2][2] = P2S(action->player.id);

        if(game->tris_matrix[2][0] == P2S(action->player.id) &&
             game->tris_matrix[2][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_789;
        }
        else if(game->tris_matrix[0][2] == P2S(action->player.id) &&
                  game->tris_matrix[1][2] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_369;
        }
        else if(game->tris_matrix[0][0] == P2S(action->player.id) &&
                  game->tris_matrix[1][1] == P2S(action->player.id))
        {
          game->g_game = END_GAME;
          game->winner_player = action->player;
          game->win_situation = TRIS_159;
        }
        else if(game->counter == 9)
        {
          game->g_game = END_GAME;
          game->win_situation = DRAW;
        }
      }
    }break;

    default:
    {

    }break;

  }

  if(swap_turn && game->g_game != END_GAME && game->playing_player.id == PLAYER_1_MASTER)
    game->playing_player.id = PLAYER_2_SLAVE;
  else if(swap_turn && game->g_game != END_GAME && game->playing_player.id == PLAYER_2_SLAVE)
    game->playing_player.id = PLAYER_1_MASTER;

}

void ge_applyAction(gameStatus_t *game, action_t *action)
{
  switch(game->g_status)
  {

    case IN_MAIN_MENU:
    {
      switch(action->typeOfAction)
      {
        case JOYSTICK_UP:
        {
          if (game->h_elem == MENU_SECONDLINE)
            game->h_elem = MENU_FIRSTLINE;
          
        }break;
        case JOYSTICK_DOWN:
        {
          if (game->h_elem == MENU_FIRSTLINE)
            game->h_elem = MENU_SECONDLINE;

        }break;
        case JOYSTICK_CLICK:
        {
          switch (game->h_elem)
          {
          case MENU_FIRSTLINE:
          {
            // Here if the user clicks while selecting the offline multiplayer game
            game->g_status = IN_GAME_OFFLINE;
            game->g_game = IN_GAME;
            game->h_elem = IN_GAME_22;
            game->playing_player = action->player;
            game->counter = 0;
            for (unsigned char i = 0; i < 3; i++)
              for (unsigned char j = 0; j < 3; j++)
                game->tris_matrix[i][j] = NA;
          }break;
          case MENU_SECONDLINE:
          {
            // Here if the user clicks while selecting the online multiplayer game
            game->g_status = IN_ONLINE_MULTIPLAYER_MENU;
            game->h_elem = MENU_FIRSTLINE;
          }break;
          default:
            break;
          }
        }break;

        case JOYSTICK_UP_RIGHT:
        case JOYSTICK_RIGHT:
        case JOYSTICK_DOWN_RIGHT:
        case JOYSTICK_DOWN_LEFT:
        case JOYSTICK_LEFT:
        case JOYSTICK_UP_LEFT:
        default:
          break;
      }
    }break;
    
    case IN_ONLINE_MULTIPLAYER_MENU:
    {
      switch(action->typeOfAction)
      {
        case JOYSTICK_UP:
        {
          if (game->h_elem == MENU_SECONDLINE)
            game->h_elem = MENU_FIRSTLINE;
          
        }break;
        case JOYSTICK_DOWN:
        {
          if (game->h_elem == MENU_FIRSTLINE)
            game->h_elem = MENU_SECONDLINE;

        }break;
        case JOYSTICK_CLICK:
        {
          switch (game->h_elem)
          {
          case MENU_FIRSTLINE:
          {
            // Here if the user clicks while selecting the create match option
            game->g_status = IN_GAME_ONLINE_MASTER;
            game->g_game = IN_GAME;
            game->h_elem = IN_GAME_22;
            game->playing_player = action->player;
            game->counter = 0;
            for (unsigned char i = 0; i < 3; i++)
              for (unsigned char j = 0; j < 3; j++)
                game->tris_matrix[i][j] = NA;
          }break;
          case MENU_SECONDLINE:
          {
            // Here if the user clicks while selecting the join match option
            game->g_status = IN_GAME_ONLINE_SLAVE;
            game->g_game = IN_GAME;
            game->h_elem = IN_GAME_22;
            game->playing_player.id = PLAYER_1_MASTER;
            game->counter = 0;
            for (unsigned char i = 0; i < 3; i++)
              for (unsigned char j = 0; j < 3; j++)
                game->tris_matrix[i][j] = NA;
          }break;
          default:
            break;
          }
        }break;
        
        case JOYSTICK_LEFT:
        {
          game->g_status = IN_MAIN_MENU;
          game->h_elem = MENU_FIRSTLINE;
        }break;
        case JOYSTICK_UP_RIGHT:
        case JOYSTICK_RIGHT:
        case JOYSTICK_DOWN_RIGHT:
        case JOYSTICK_DOWN_LEFT:
        case JOYSTICK_UP_LEFT:
        default:
          break;
      }
    }break;
    
    case IN_GAME_ONLINE_MASTER:
    case IN_GAME_OFFLINE:
    {
      switch(action->typeOfAction)
      {

        case JOYSTICK_UP:
        case JOYSTICK_RIGHT:
        case JOYSTICK_DOWN:
        case JOYSTICK_LEFT:
        {
          ge_applyMove(game, action);
        }break;
        case JOYSTICK_UP_RIGHT:
        case JOYSTICK_DOWN_RIGHT:
        case JOYSTICK_DOWN_LEFT:
        case JOYSTICK_UP_LEFT:
        {
          ge_applyDiagonalMove(game, action);

        }break;
        case JOYSTICK_CLICK:
        {
          ge_applyClick(game, action);
        }break;
        default:
        {

        }break;
      }
    }break;
    
    case INVALID_STATUS:
    {

    }break;

    case IN_GAME_ONLINE_SLAVE:
    {
      game->g_status = INVALID_STATUS;
    }break;
    
    default:
      break;
  }
}
