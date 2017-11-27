/**
 * Copyright (c) 2017, Carl Stika
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 * Breakout
 *
 * @author Carl Stika
 * @date October 10, 2017
 *
 *
 **************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>

#include "breakout.h"
#include "titlesplash.h"
#include "breakouttitle.h"
#include "ball.h"
#include "compplayer.h"

#include "blockplayfields.h"


static int paddleX = 0; //Current center position of paddle
static int paddleY = 0; //Current center position of paddle

static int old_paddleX = 0; //Current center position of paddle
static int old_paddleY = 0; //Current center position of paddle
static int paddleSpeed = 10;


static int curLevel = 0;

static int menuSelection = 0;

enum GameState gameState;

int numBricksOnPlayfield = 0;
char brickPlayField[MAX_BLOCKS_HEIGHT][MAX_BLOCKS_WIDTH];


int highScore = 100;
int numPlayerBalls = 3;
int curPlayerScore = 0;

#define MAX_MAIN_MENU_ITEM 2
char *mainTitleMenu[] = {
    "New Game",
    "Exit"
};

#define MAX_PAUSE_MENU_ITEM 3
char *pauseMenu[] = {
    "Cont Game",
    "New Game",
    "Exit"
};

/* forward declarations*/
void InitLevels();
void InitBall();
void InitPaddle();
void UpdateTitleMenu();
void UpdateGamePlay();
void UpdatePauseMenu();
void UpdateGameOverScreen();
void RenderGameBoard();
void RenderTitleMenu();
void RenderPauseMenu();
void RenderGameOverScreen();

/**
 * Initializes the game.
 * Should be called once.
 *
 * Starts the game at Title screen and in a attract mode.
 */
void BreakoutInit() {
    gameState = GAME_STATE_TITLE;
    InitLevels();
    numPlayerBalls = 3;
    curPlayerScore = 0;
    InitBall();
    InitPaddle();
    SplashTitleInit(BreakoutTitle, BreakoutTitleLength);
}

/**
 * Start a new game.
 */
void BreakoutStartNewGame() {
    InitLevels();
    numPlayerBalls = 3;
    curPlayerScore = 0;
    InitBall();
    InitPaddle();
}

int BreakoutIsGameExiting() {
    return gameState==GAME_STATE_EXITING;
}

static int lastKeyPress = 0;

void movePaddleLeft() {
    paddleX -= paddleSpeed;

    if(paddleX-(PADDLE_NORMAL_SIZE_WIDTH/2) <= PLAYFIELD_BORDER_X) {
        paddleX = PLAYFIELD_BORDER_X + (PADDLE_NORMAL_SIZE_WIDTH/2) + 1;
    }
}

void movePaddleRight() {
    paddleX += paddleSpeed;

    if(paddleX+(PADDLE_NORMAL_SIZE_WIDTH/2) >= (PLAYFIELD_BORDER_X+PLAYFIELD_BORDER_WIDTH)) {
        paddleX = (PLAYFIELD_BORDER_X+PLAYFIELD_BORDER_WIDTH) - (PADDLE_NORMAL_SIZE_WIDTH/2) - 1;
    }
}

void BreakoutKeyHandler(int key) {
    if(gameState == GAME_STATE_PLAYING) {
        if(key==KEY_ARROW_LEFT) {
            movePaddleLeft();
        }
        if(key==KEY_ARROW_RIGHT) {
            movePaddleRight();
        }
        if(key==KEY_ARROW_UP) {
            if(ballStuckToPaddle()) {
                ballReleaseFromPaddle();
            }
        }
        if(key==KEY_ESC) {
            gameState = GAME_STATE_PAUSED;
        }
    }
    else {
        if( gameState == GAME_STATE_TITLE ||
            gameState == GAME_STATE_GAME_OVER) {
            if(key!=lastKeyPress) { //we do not want repeated keys
                if(key==KEY_ARROW_UP) {
                    menuSelection--;
                    if(menuSelection<0) {
                        menuSelection+= MAX_MAIN_MENU_ITEM;
                    }
                }
                if(key==KEY_ARROW_DOWN) {
                    menuSelection++;
                    menuSelection%= MAX_MAIN_MENU_ITEM;
                }
                if(key==KEY_ENTER) {
                    if(menuSelection==0) {
                        gameState = GAME_STATE_PLAYING;
                        BreakoutStartNewGame();
                    }
                    else {
                        gameState = GAME_STATE_EXITING;
                    }
                }
            }
        }
        else {
            if(gameState == GAME_STATE_PAUSED) {
                if(key!=lastKeyPress) { //we do not want repeated keys
                    if(key==KEY_ARROW_UP) {
                        menuSelection--;
                        if(menuSelection<0) {
                            menuSelection+= MAX_PAUSE_MENU_ITEM;
                        }
                    }
                    if(key==KEY_ARROW_DOWN) {
                        menuSelection++;
                        menuSelection%=MAX_PAUSE_MENU_ITEM;
                    }
                    if(key==KEY_ENTER) {
                        switch(menuSelection) {
                            case 0:
                                gameState = GAME_STATE_PLAYING;
                                break;
                            case 1:
                                gameState = GAME_STATE_PLAYING;
                                BreakoutStartNewGame();
                                break;
                            case 2:
                                gameState = GAME_STATE_EXITING;
                                break;
                        }
                    }
                }
            }

        }

    }
    lastKeyPress = key;

}

static void SetPlayfieldArray(char **playfieldStringArray, int playfieldLength) {
  int i, j;
  int h = playfieldLength<=MAX_BLOCKS_HEIGHT?playfieldLength:MAX_BLOCKS_HEIGHT;

  numBricksOnPlayfield = 0;

  //init the playfield with 0
  memset(brickPlayField, 0, sizeof(char)*MAX_BLOCKS_HEIGHT*MAX_BLOCKS_WIDTH);

  for(j=0;j<h;j++) {
      int w = strlen(playfieldStringArray[j]);

      if(w>=MAX_BLOCKS_WIDTH) w=MAX_BLOCKS_WIDTH;

      for(i=0;i<w;i++) {
        brickPlayField[j][i] = playfieldStringArray[j][i]-'0';
        if(brickPlayField[j][i]>0) numBricksOnPlayfield++;
      }
  }

}

/**
 * InitLevels
 */
static void InitLevels() {
    curLevel = 0;
    SetPlayfieldArray(playfields[curLevel].playfield, playfields[curLevel].numRows);
}

/**
 * Advance to next level
 **/
void NextLevel() {
    curLevel++;
    if(curLevel>=MAX_PLAY_FIELDS) {
        curLevel = 0;
    }
    SetPlayfieldArray(playfields[curLevel].playfield, playfields[curLevel].numRows);
}

static void InitPaddle() {
    paddleX = 320/2;
    paddleY = 240 - BORDER_SIZE - DIST_PADDLE_FROM_BOTTOM;

    old_paddleX = paddleX;
    old_paddleY = paddleY;
}


/***********************************************************************
 * Update functions
 **********************************************************************/

/**
 * This function is used to controll the paddle if
 * the computer is playing in attract mode.
 *
 */
static void updatePaddle() {
    //if game is playing
    if(gameState==GAME_STATE_TITLE)
    {
        struct GameInfo gameInfo;
        int move;

        gameInfo.ballX = getBallX();
        gameInfo.ballY = getBallY();
        gameInfo.ballVelX = getBallVelX();
        gameInfo.ballVelY = getBallVelY();

        gameInfo.paddleX = paddleX;
        gameInfo.paddleY = paddleY;

        gameInfo.isBallStuckToPaddle = ballStuckToPaddle();

        move = CompPlayer_getComputerMove(&gameInfo);
        switch(move) {
            case MOVE_NOTHING:
                break;
            case MOVE_PADDLE_LEFT:
                movePaddleLeft();
                break;
            case MOVE_PADDLE_RIGHT:
                movePaddleRight();
                break;
            case MOVE_RELEASE_BALL:
                ballReleaseFromPaddle();
                break;
        }
    }
}

static void UpdateGamePlay() {
    updateBall(paddleX, paddleY);
    updatePaddle();

    //check if all bricks have been hit
    if(numBricksOnPlayfield<=0) {
        AddPlayerScore(100);

        setBallState(BALL_STATE_STUCK_TO_PADDLE);
        NextLevel();
    }
}

static void UpdatePauseMenu() {
}

static void UpdateGameOverScreen() {
}

void BreakoutUpdate() {
    switch(gameState) {
        case GAME_STATE_TITLE:
            SplashTitleUpdate();
            updatePaddle();
            UpdateGamePlay();
            break;
        case GAME_STATE_PLAYING:
            UpdateGamePlay();
            break;
        case GAME_STATE_PAUSED:
            UpdatePauseMenu();
            break;
        case GAME_STATE_GAME_OVER:
            UpdateGameOverScreen();
            break;
        default:
            break;
    }
}


/***********************************************************************
 * Draw functions
 **********************************************************************/

/****************************************************************
 * Draw Paddle
 *
 * Currently only have 1 paddle which is normal size.
 ***************************************************************/
static void DrawPaddle() {
/*    gfx_SetColor(255);
    gfx_FillRectangle_NoClip(old_paddleX-(PADDLE_NORMAL_SIZE_WIDTH/2),
                             old_paddleY,
                             PADDLE_NORMAL_SIZE_WIDTH,
                             PADDLE_NORMAL_SIZE_HEIGHT);
*/

    gfx_SetColor(PADDLE_NORMAL_COLOR);
    gfx_FillRectangle_NoClip(paddleX-(PADDLE_NORMAL_SIZE_WIDTH/2),
                              paddleY,
                              PADDLE_NORMAL_SIZE_WIDTH,
                              PADDLE_NORMAL_SIZE_HEIGHT);
    old_paddleX = paddleX;
    old_paddleY = paddleY;
}


static void DrawBorder() {
   gfx_SetColor(16);
   gfx_Rectangle_NoClip(PLAYFIELD_BORDER_X,
                        PLAYFIELD_BORDER_Y,
                        PLAYFIELD_BORDER_WIDTH,
                        PLAYFIELD_BORDER_HEIGHT);
}

/**
 * Draw blocks of the current playfield
 */
static void DrawBlocks() {
    int i=0, j=0;
    int x=BLOCK_START_X;
    int y=BLOCK_START_Y;

    for(j=0;j<MAX_BLOCKS_HEIGHT;j++) {
        x = BLOCK_START_X;
        for(i=0;i<MAX_BLOCKS_WIDTH;i++) {
            if(brickPlayField[j][i]>0) {
               gfx_SetColor(brickPlayField[j][i]);
               gfx_FillRectangle_NoClip(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);
            }
            x+=BLOCK_WIDTH + BLOCK_SPACE_WIDTH;
        }
        y+=BLOCK_HEIGHT + BLOCK_SPACE_HEIGHT;
    }
}


static void DrawScore() {
    int i;
    float bx = 5;
    float by = 5;
    char string[256];

    //Show the number of balls left
    for(i=0;i<numPlayerBalls;i++) {
        gfx_SetColor(11); //white
        gfx_FillCircle_NoClip(bx, by, 2);
        bx +=6;
    }

    gfx_SetTextFGColor(11); //white
    gfx_SetTextBGColor(0);
    sprintf(string, "Score %04d     high %04d", curPlayerScore, highScore);
    gfx_PrintStringXY(string, 160-40, 2 );

}

/**
 * Current we are rendering the entire board for each frame.
 * If I need the speed I might update only what has moved.
 *
 */
static void RenderGameBoard() {
    DrawBorder();

    DrawScore();

    DrawBlocks();

    DrawBall();
    DrawPaddle();
}


static void PrintStringCentered(const char *string, int y) {
    int sizePixels = strlen(string) * 8; //approx
    int x = (320-sizePixels)/2;

    if(x<0) x = 0;
    gfx_PrintStringXY(string, x, y );
}

/**
 * Generic function to render a menu.
 * Highlights the current item that is selected.
 *
 * @param menu        A list of strings
 * @param maxItems    The number of stri ngs in the array
 * @param currentItem Curent selected line
 * @param y           Starting y pos of the menu. The lines are horizontal centered
 */
static void renderMenu(char **menu, int maxItems, int currentItem, int y) {
    int i=0;
    char menuItem[255];

    for(;i<maxItems;i++) {
        if(currentItem==i) {
            sprintf(menuItem, ">%s<", menu[i]);
        }
        else {
            sprintf(menuItem, " %s ", menu[i]);
        }

        PrintStringCentered(menuItem, y );
        y+=10;
    }
}

static void RenderTitleMenu() {
    int ty = 120;

    gfx_SetTextFGColor(11); //white
    gfx_SetTextBGColor(0);

    PrintStringCentered("BREAK OUT", ty );
    ty += 10;

    renderMenu(mainTitleMenu, MAX_MAIN_MENU_ITEM, menuSelection, ty);
}

static void RenderPauseMenu() {
    int ty = 110;

    PrintStringCentered("PAUSE", ty );
    ty += 10;

    renderMenu(pauseMenu, MAX_PAUSE_MENU_ITEM, menuSelection, ty);
}

static void RenderGameOverScreen() {
    int ty = 110;

    PrintStringCentered("GAMEOVER", ty );
    ty += 10;

    RenderTitleMenu();
}

/**
 * Called by main program to render the screen
 *
 * Currently rendering the entire frame.
 *
 */
void BreakoutRender() {
//    memset(lcd_Ram, 255, LCD_SIZE);
    gfx_FillScreen(0);

    switch(gameState) {
        case GAME_STATE_TITLE:
            RenderGameBoard();
//            SplashTitleRender();
            RenderTitleMenu();
            break;
        case GAME_STATE_PLAYING:
            RenderGameBoard();
            break;
        case GAME_STATE_PAUSED:
            RenderGameBoard();

            RenderPauseMenu();
            break;
        case GAME_STATE_GAME_OVER:
            RenderGameBoard();

            RenderGameOverScreen();
            break;
        default:
            break;
    }
}

/**
 * Update the players Score
 * @param score Value to add to the current score.
 */
void AddPlayerScore(int score) {
    curPlayerScore += score;
    if(curPlayerScore>highScore) {
        highScore = curPlayerScore;
    }
}

void SetGameOver() {
    gameState = GAME_STATE_GAME_OVER;
}

/**
 * Decrements the number of balls the player has.
 * When it gets to 0 this will change the game state.
 */
void DecPlayerBall() {
    numPlayerBalls--;
    if(numPlayerBalls==0) {
        SetGameOver();
    }
}

/* vim: set et ts=4 sw=4 sts=4 ft=c:expandtab:ai:cindent */
