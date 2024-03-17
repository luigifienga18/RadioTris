#include "material/logo.h"
#include "renderer.h"
#include "ssd1306.h"
#include "chprintf.h"

/*
 *
 * Definition of all data that we've need
 * */
#define BUFF_SIZE   20
char buff[BUFF_SIZE];

static const I2CConfig i2ccfg = {
  // I2C_TIMINGR address offset
  .timingr = 0x10,
  .cr1 = 0,
  .cr2 = 1,
};

static const SSD1306Config ssd1306cfg = {
  &I2CD1,
  &i2ccfg,
  SSD1306_SAD_0X78,
};

static SSD1306Driver SSD1306D1;

//Renderitation of logo on display
void renderLogo(void )
{
  int x = 0;

  int y = 0;

  int c = 0;

  ssd1306FillScreen(&SSD1306D1, 0x00);

  for(y = 0; y<SSD1306_HEIGHT; y++){

    for(x = 0; x<SSD1306_WIDTH; x++){

      ssd1306DrawPixel(&SSD1306D1, x, y, image_data[c]);

      c++;

      }

    ssd1306UpdateScreen(&SSD1306D1);

  }

  ssd1306UpdateScreen(&SSD1306D1);

}

/*Initialitation of
 * GPIOB8 (D15)
 * AND
 * GPIOB9 (D14)
 * pin
 * For the render of display
 * */
void renderStart(void )
{
  /*Setting Pin*/
  palSetLineMode(PAL_LINE(GPIOB, 8U), PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(PAL_LINE(GPIOB, 9U), PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_PUPDR_PULLUP);

  ssd1306ObjectInit(&SSD1306D1);

  ssd1306Start(&SSD1306D1, &ssd1306cfg);

  ssd1306FillScreen(&SSD1306D1, 0x00);

}

void renderGame(gameStatus_t *game,bool text_visible)
{
  if(game->g_status==IN_MAIN_MENU)
  {
    renderMenu(game,text_visible);
  }
  else if(game->g_status==IN_GAME_OFFLINE || game->g_status==IN_GAME_ONLINE_MASTER || game->g_status==IN_GAME_ONLINE_SLAVE)
  {
    renderPlay(game,text_visible);
  }
  else if(game->g_status==IN_ONLINE_MULTIPLAYER_MENU)
  {
    renderMenuMultiplayer(game, text_visible);
  }


}

/*Render of FirstMenu*/
void renderMenu(gameStatus_t *game, bool text_visible)
{


/*
 *  CENTRAL VIEW
 *
    ssd1306GotoXy(&SSD1306D1, 40, 1);
    chsnprintf(buff, BUFF_SIZE, "TRIS");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 20, 25);

    chsnprintf(buff, BUFF_SIZE, "-Multiplayer");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 40, 35);
    chsnprintf(buff, BUFF_SIZE, "Locale");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 20, 50);
    chsnprintf(buff, BUFF_SIZE, "-Multiplayer");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
*/

    /*LEFT VIEW*/
    ssd1306FillScreen(&SSD1306D1, 0x00);

    ssd1306GotoXy(&SSD1306D1, 40, 1);
    chsnprintf(buff, BUFF_SIZE, "TRIS");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 1, 25);
    chsnprintf(buff, BUFF_SIZE, "-Local");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 8, 35);
    chsnprintf(buff, BUFF_SIZE, "Multiplayer");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);


    ssd1306GotoXy(&SSD1306D1, 1, 50);
    chsnprintf(buff, BUFF_SIZE, "-Multiplayer");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);


      if(text_visible && (game->h_elem==MENU_FIRSTLINE))
            {
              ssd1306GotoXy(&SSD1306D1, 1, 25);
              chsnprintf(buff, BUFF_SIZE, "-Local");
              ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);

              ssd1306GotoXy(&SSD1306D1, 8, 35);
              chsnprintf(buff, BUFF_SIZE, "Multiplayer");
              ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);

             }
           else if (!text_visible && game->h_elem==MENU_FIRSTLINE)
           {
             ssd1306GotoXy(&SSD1306D1, 1, 25);
                           chsnprintf(buff, BUFF_SIZE, "-Local");
                           ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);

                           ssd1306GotoXy(&SSD1306D1, 8, 35);
                           chsnprintf(buff, BUFF_SIZE, "Multiplayer");
                           ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);

            }

      if(text_visible && (game->h_elem==MENU_SECONDLINE))
      {
        ssd1306GotoXy(&SSD1306D1, 1, 50);
        chsnprintf(buff, BUFF_SIZE, "-Multiplayer");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      }
      else if (!text_visible && game->h_elem==MENU_SECONDLINE)
      {
        ssd1306GotoXy(&SSD1306D1, 1, 50);
                chsnprintf(buff, BUFF_SIZE, "-Multiplayer");
                ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);
      }

     ssd1306UpdateScreen(&SSD1306D1);

}
/*Render of Cursor for playing*/
void renderCursor(gameStatus_t *game,bool text_visible)
{

  if(text_visible && (game->h_elem==IN_GAME_11))
    {
     ssd1306DrawCircle(&SSD1306D1,11,11,1,SSD1306_COLOR_WHITE);  //1,1
    }
  else if (!text_visible && game->h_elem==IN_GAME_11)
    {
     ssd1306DrawCircle(&SSD1306D1,11,11,1,SSD1306_COLOR_BLACK);  //1,1
    }

  if(text_visible && (game->h_elem==IN_GAME_12))
    {
    ssd1306DrawCircle(&SSD1306D1,32,11,1,SSD1306_COLOR_WHITE);  //1,2
    }
  else if (!text_visible && game->h_elem==IN_GAME_12)
    {
    ssd1306DrawCircle(&SSD1306D1,32,11,1,SSD1306_COLOR_BLACK);  //1,2
    }

  if(text_visible && (game->h_elem==IN_GAME_13))
    {
    ssd1306DrawCircle(&SSD1306D1,53,11,1,SSD1306_COLOR_WHITE);  //1,3
    }
  else if (!text_visible && game->h_elem==IN_GAME_13)
    {
    ssd1306DrawCircle(&SSD1306D1,53,11,1,SSD1306_COLOR_BLACK);  //1,3
    }

  if(text_visible && (game->h_elem==IN_GAME_21))
    {
    ssd1306DrawCircle(&SSD1306D1,11,32,1,SSD1306_COLOR_WHITE);  //2,1
    }
  else if (!text_visible && game->h_elem==IN_GAME_21)
    {
    ssd1306DrawCircle(&SSD1306D1,11,32,1,SSD1306_COLOR_BLACK);  //2,1
    }

  if(text_visible && (game->h_elem==IN_GAME_22))
    {
    ssd1306DrawCircle(&SSD1306D1,32,32,1,SSD1306_COLOR_WHITE);  //2,2
    }
  else if (!text_visible && game->h_elem==IN_GAME_22)
    {
    ssd1306DrawCircle(&SSD1306D1,32,32,1,SSD1306_COLOR_BLACK);  //2,2
    }

  if(text_visible && (game->h_elem==IN_GAME_23))
    {
    ssd1306DrawCircle(&SSD1306D1,53,32,1,SSD1306_COLOR_WHITE);  //2,3
    }
  else if (!text_visible && game->h_elem==IN_GAME_23)
    {
    ssd1306DrawCircle(&SSD1306D1,53,32,1,SSD1306_COLOR_BLACK);  //2,3
    }

  if(text_visible && (game->h_elem==IN_GAME_31))
    {
    ssd1306DrawCircle(&SSD1306D1,11,53,1,SSD1306_COLOR_WHITE);  //3,1
    }
  else if (!text_visible && game->h_elem==IN_GAME_31)
    {
    ssd1306DrawCircle(&SSD1306D1,11,53,1,SSD1306_COLOR_BLACK);  //3,1
    }

  if(text_visible && (game->h_elem==IN_GAME_32))
    {
    ssd1306DrawCircle(&SSD1306D1,32,53,1,SSD1306_COLOR_WHITE);  //3,2
    }
  else if (!text_visible && game->h_elem==IN_GAME_32)
    {
    ssd1306DrawCircle(&SSD1306D1,32,53,1,SSD1306_COLOR_BLACK);  //3,2
    }

  if(text_visible && (game->h_elem==IN_GAME_33))
    {
    ssd1306DrawCircle(&SSD1306D1,53,53,1,SSD1306_COLOR_WHITE);  //3,3
    }
  else if (!text_visible && game->h_elem==IN_GAME_33)
    {
    ssd1306DrawCircle(&SSD1306D1,53,53,1,SSD1306_COLOR_BLACK);  //3,3
    }
}
/*Render of move*/
void renderX0(gameStatus_t *game,int i,int j)
{
  if(i==0 && j==0)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,5,5,17,17,SSD1306_COLOR_WHITE);   //Da sinistra a destra   //1,1
      ssd1306DrawLine(&SSD1306D1,17,5,5,17,SSD1306_COLOR_WHITE);   //Da destra a sinistra
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,11,11,7,SSD1306_COLOR_WHITE);  //1,1
    }

  }
  else if(i==0 && j==1)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,26,5,38,17,SSD1306_COLOR_WHITE);                           //1,2
      ssd1306DrawLine(&SSD1306D1,38,5,26,17,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,32,11,7,SSD1306_COLOR_WHITE);  //1,2
    }
  }
  else if(i==0 && j==2)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,47,5,59,17,SSD1306_COLOR_WHITE);                           //1,3
      ssd1306DrawLine(&SSD1306D1,59,5,47,17,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,53,11,7,SSD1306_COLOR_WHITE);  //1,3
    }
  }
  else if(i==1 && j==0)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,5,26,17,38,SSD1306_COLOR_WHITE);                           //2,1
      ssd1306DrawLine(&SSD1306D1,17,26,5,38,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,11,32,7,SSD1306_COLOR_WHITE);  //2,1
    }
  }
  else if(i==1 && j==1)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,26,26,38,38,SSD1306_COLOR_WHITE);                          //2,2
      ssd1306DrawLine(&SSD1306D1,38,26,26,38,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,32,32,7,SSD1306_COLOR_WHITE);  //2,2
    }
  }
  else if(i==1 && j==2)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,47,26,59,38,SSD1306_COLOR_WHITE);                          //2,3
      ssd1306DrawLine(&SSD1306D1,59,26,47,38,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,53,32,7,SSD1306_COLOR_WHITE);  //2,3
    }
  }
  else if(i==2 && j==0)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,5,47,17,59,SSD1306_COLOR_WHITE);                           //3,1
      ssd1306DrawLine(&SSD1306D1,17,47,5,59,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,11,53,7,SSD1306_COLOR_WHITE);  //3,1
    }
  }
  else if(i==2 && j==1)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,26,47,38,59,SSD1306_COLOR_WHITE);                          //3,2
      ssd1306DrawLine(&SSD1306D1,38,47,26,59,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,32,53,7,SSD1306_COLOR_WHITE);  //3,2
    }
  }
  else if(i==2 && j==2)
  {
    if(game->tris_matrix[i][j]==X)
    {
      ssd1306DrawLine(&SSD1306D1,47,47,59,59,SSD1306_COLOR_WHITE);                          //3,3
      ssd1306DrawLine(&SSD1306D1,59,47,47,59,SSD1306_COLOR_WHITE);
    }
    else if(game->tris_matrix[i][j]==C)
    {
      ssd1306DrawCircle(&SSD1306D1,53,53,7,SSD1306_COLOR_WHITE);  //3,3*/
    }
  }
}
/*Render of possible line of victory*/
void renderWin(gameStatus_t *game,bool text_visible)
{
  if(text_visible && (game->win_situation==TRIS_123))
    {
    ssd1306DrawLine(&SSD1306D1,3,11,61,11,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_123))
    {
    ssd1306DrawLine(&SSD1306D1,3,11,61,11,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_159))
    {
    ssd1306DrawLine(&SSD1306D1,3,3,61,61,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_159))
    {
    ssd1306DrawLine(&SSD1306D1,3,3,61,61,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_147))
    {
    ssd1306DrawLine(&SSD1306D1,11,3,11,61,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_147))
    {
    ssd1306DrawLine(&SSD1306D1,11,3,11,61,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_258))
    {
    ssd1306DrawLine(&SSD1306D1,32,3,32,61,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_258))
    {
    ssd1306DrawLine(&SSD1306D1,32,3,32,61,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_357))
    {
    ssd1306DrawLine(&SSD1306D1,61,3,3,61,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_357))
    {
    ssd1306DrawLine(&SSD1306D1,61,3,3,61,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_123))
    {
    ssd1306DrawLine(&SSD1306D1,3,11,61,11,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_123))
    {
    ssd1306DrawLine(&SSD1306D1,3,11,61,11,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_369))
    {
    ssd1306DrawLine(&SSD1306D1,53,3,53,61,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_369))
    {
    ssd1306DrawLine(&SSD1306D1,53,3,53,61,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_456))
    {
    ssd1306DrawLine(&SSD1306D1,3,32,61,32,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_456))
    {
    ssd1306DrawLine(&SSD1306D1,3,32,61,32,SSD1306_COLOR_BLACK);
    }

  if(text_visible && (game->win_situation==TRIS_789))
    {
    ssd1306DrawLine(&SSD1306D1,3,53,61,53,SSD1306_COLOR_WHITE);
    }
  else if (!text_visible && (game->win_situation==TRIS_789))
    {
    ssd1306DrawLine(&SSD1306D1,3,53,61,53,SSD1306_COLOR_BLACK);
    }
}
/*Render the match*/
void renderPlay(gameStatus_t *game,bool text_visible)
{
  ssd1306FillScreen(&SSD1306D1, 0x00);
//  ssd1306UpdateScreen(&SSD1306D1);

  /*TRIS SCHEMA*/
  ssd1306DrawLine(&SSD1306D1,0,0,63,0,SSD1306_COLOR_WHITE);           //First row
  ssd1306DrawLine(&SSD1306D1,0,21,63,21,SSD1306_COLOR_WHITE);         //Second row
  ssd1306DrawLine(&SSD1306D1,0,42,63,42,SSD1306_COLOR_WHITE);         //Third row
  ssd1306DrawLine(&SSD1306D1,0,63,63,63,SSD1306_COLOR_WHITE);         //Last row
  ssd1306DrawLine(&SSD1306D1,0,0,0,63,SSD1306_COLOR_WHITE);           //First column
  ssd1306DrawLine(&SSD1306D1,21,0,21,63,SSD1306_COLOR_WHITE);         //Second column
  ssd1306DrawLine(&SSD1306D1,42,0,42,63,SSD1306_COLOR_WHITE);         //Third column
  ssd1306DrawLine(&SSD1306D1,63,0,63,63,SSD1306_COLOR_WHITE);         //Last column

  if(game->g_game==IN_GAME && game->g_status==IN_GAME_OFFLINE)
   {
    renderCursor(game,text_visible);
   }
  else if(game->g_game==IN_GAME && (game->g_status==IN_GAME_ONLINE_MASTER) && game->playing_player.id == PLAYER_1_MASTER)
  {
      renderCursor(game,text_visible);
  }
  else if(game->g_game==IN_GAME && (game->g_status==IN_GAME_ONLINE_SLAVE) && game->playing_player.id == PLAYER_2_SLAVE)
  {
      renderCursor(game,text_visible);
  }

  if(game->playing_player.id == PLAYER_1_MASTER && game->g_game==IN_GAME)
  {
    if(game->g_status==IN_GAME_OFFLINE)
    {
      ssd1306GotoXy(&SSD1306D1, 90, 1);
      chsnprintf(buff, BUFF_SIZE, "P1");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
      ssd1306GotoXy(&SSD1306D1, 80, 32);
      chsnprintf(buff, BUFF_SIZE, "TURN");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }
    else if(game->g_status==IN_GAME_ONLINE_MASTER)
    {
          ssd1306GotoXy(&SSD1306D1, 80, 1);
          chsnprintf(buff, BUFF_SIZE, "YOUR");
          ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
          ssd1306GotoXy(&SSD1306D1, 80, 32);
          chsnprintf(buff, BUFF_SIZE, "TURN");
          ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }
    else if(game->g_status==IN_GAME_ONLINE_SLAVE)
    {
      ssd1306GotoXy(&SSD1306D1, 80, 1);
      chsnprintf(buff, BUFF_SIZE, "OPP");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
      ssd1306GotoXy(&SSD1306D1, 80, 32);
      chsnprintf(buff, BUFF_SIZE, "TURN");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }

  }
  else if(game->playing_player.id == PLAYER_2_SLAVE && game->g_game==IN_GAME)
  {
    if(game->g_status==IN_GAME_OFFLINE)
    {
      ssd1306GotoXy(&SSD1306D1, 90, 1);
      chsnprintf(buff, BUFF_SIZE, "P2");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
      ssd1306GotoXy(&SSD1306D1, 80, 32);
      chsnprintf(buff, BUFF_SIZE, "TURN");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }
    else if(game->g_status==IN_GAME_ONLINE_SLAVE)
    {
      ssd1306GotoXy(&SSD1306D1, 80, 1);
      chsnprintf(buff, BUFF_SIZE, "YOUR");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
      ssd1306GotoXy(&SSD1306D1, 80, 32);
      chsnprintf(buff, BUFF_SIZE, "TURN");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }
    else if(game->g_status==IN_GAME_ONLINE_MASTER)
    {
          ssd1306GotoXy(&SSD1306D1, 80, 1);
          chsnprintf(buff, BUFF_SIZE, "OPP");
          ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
          ssd1306GotoXy(&SSD1306D1, 80, 32);
          chsnprintf(buff, BUFF_SIZE, "TURN");
          ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }
  }


  /*PRINT PLAY*/
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      renderX0(game,i,j);
    }
  }


  if(game->g_game==END_GAME && game->win_situation!=DRAW)
  {
    if(game->winner_player.id==PLAYER_1_MASTER)
    {
      if(text_visible && game->g_status==IN_GAME_OFFLINE)
               {
             ssd1306GotoXy(&SSD1306D1, 90, 1);
                       chsnprintf(buff, BUFF_SIZE, "P1");
                       ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                       ssd1306GotoXy(&SSD1306D1, 84, 32);
                       chsnprintf(buff, BUFF_SIZE, "WIN");
                       ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
               }
             else if (!text_visible && game->g_status==IN_GAME_OFFLINE)
               {
               ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
               }

           if(text_visible && (game->g_status==IN_GAME_ONLINE_MASTER))
           {
             ssd1306GotoXy(&SSD1306D1, 84, 1);
                 chsnprintf(buff, BUFF_SIZE, "YOU");
                 ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                 ssd1306GotoXy(&SSD1306D1, 84, 32);
                 chsnprintf(buff, BUFF_SIZE, "WIN");
                 ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
           }
           else if(!text_visible && (game->g_status==IN_GAME_ONLINE_MASTER))
           {
             ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
           }

           if(text_visible && ( game->g_status==IN_GAME_ONLINE_SLAVE))
           {
             ssd1306GotoXy(&SSD1306D1, 84, 1);
                 chsnprintf(buff, BUFF_SIZE, "YOU");
                 ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                 ssd1306GotoXy(&SSD1306D1, 80, 32);
                 chsnprintf(buff, BUFF_SIZE, "LOSE");
                 ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
           }
           else if(!text_visible && (game->g_status==IN_GAME_ONLINE_SLAVE))
           {
             ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
           }
    }
    else
    {
      if(text_visible && game->g_status==IN_GAME_OFFLINE)
                   {
                 ssd1306GotoXy(&SSD1306D1, 90, 1);
                           chsnprintf(buff, BUFF_SIZE, "P2");
                           ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                           ssd1306GotoXy(&SSD1306D1, 84, 32);
                           chsnprintf(buff, BUFF_SIZE, "WIN");
                           ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                   }
                 else if (!text_visible && game->g_status==IN_GAME_OFFLINE)
                   {
                   ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
                   }

          if(text_visible && (game->g_status==IN_GAME_ONLINE_MASTER))
          {
            ssd1306GotoXy(&SSD1306D1, 84, 1);
                chsnprintf(buff, BUFF_SIZE, "YOU");
                ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                ssd1306GotoXy(&SSD1306D1, 80, 32);
                chsnprintf(buff, BUFF_SIZE, "LOSE");
                ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
          }
          else if(!text_visible && (game->g_status==IN_GAME_ONLINE_MASTER))
          {
            ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
          }

          if(text_visible && (game->g_status==IN_GAME_ONLINE_SLAVE))
           {
            ssd1306GotoXy(&SSD1306D1, 84, 1);
                chsnprintf(buff, BUFF_SIZE, "YOU");
                ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
                ssd1306GotoXy(&SSD1306D1, 84, 32);
                chsnprintf(buff, BUFF_SIZE, "WIN");
                ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
           }
           else if(!text_visible && (game->g_status==IN_GAME_ONLINE_SLAVE))
           {
             ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
           }
    }
    renderWin(game,text_visible);

  }
  else if(game->g_game==END_GAME && game->win_situation==DRAW)
  {


    if(text_visible)
    {
           ssd1306GotoXy(&SSD1306D1, 80, 25);
           chsnprintf(buff, BUFF_SIZE, "DRAW");
           ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    }
    else
    {
      ssd1306DrawRectangleFill(&SSD1306D1,79,0,50,60,SSD1306_COLOR_BLACK);
    }
  }

  ssd1306UpdateScreen(&SSD1306D1);

}
/*Render the second menu, for the Multiplayer*/
void renderMenuMultiplayer(gameStatus_t *game, bool text_visible)
{
  ssd1306FillScreen(&SSD1306D1, 0x00);

  ssd1306GotoXy(&SSD1306D1, 32, 1);
  chsnprintf(buff, BUFF_SIZE, "Online");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);

  ssd1306GotoXy(&SSD1306D1, 1, 25);
  chsnprintf(buff, BUFF_SIZE, "-Host game");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);


  ssd1306GotoXy(&SSD1306D1, 1, 50);
  chsnprintf(buff, BUFF_SIZE, "-Join Game");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);


    if(text_visible && (game->h_elem==MENU_FIRSTLINE))
          {
            ssd1306GotoXy(&SSD1306D1, 1, 25);
            chsnprintf(buff, BUFF_SIZE, "-Host game");
            ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);

           }
         else if (!text_visible && game->h_elem==MENU_FIRSTLINE)
         {
           ssd1306GotoXy(&SSD1306D1, 1, 25);
           chsnprintf(buff, BUFF_SIZE, "-Host game");
           ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);
          }

    if(text_visible && (game->h_elem==MENU_SECONDLINE))
    {
      ssd1306GotoXy(&SSD1306D1, 1, 50);
      chsnprintf(buff, BUFF_SIZE, "-Join Game");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
    }
    else if (!text_visible && game->h_elem==MENU_SECONDLINE)
    {
      ssd1306GotoXy(&SSD1306D1, 1, 50);
      chsnprintf(buff, BUFF_SIZE, "-Join Game");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);
    }

   ssd1306UpdateScreen(&SSD1306D1);


}
