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

#ifndef BREAKOUT_H
#define BREAKOUT_h

//Max blocks on playfield
#define MAX_BLOCKS_WIDTH 15
#define MAX_BLOCKS_HEIGHT 8

//Game objects
#define BLOCK_WIDTH 18
#define BLOCK_HEIGHT 10
#define BLOCK_SPACE_WIDTH 2
#define BLOCK_SPACE_HEIGHT 1


#define PADDLE_NORMAL_SIZE_WIDTH 	30
#define PADDLE_NORMAL_SIZE_HEIGHT 	6
#define PADDLE_NORMAL_COLOR gfx_yellow
#define DIST_PADDLE_FROM_BOTTOM 20

//Playfield
#define BLOCK_MAX_WIDTH_PIXELS (BLOCK_WIDTH*MAX_BLOCKS_WIDTH+BLOCK_SPACE_WIDTH*(MAX_BLOCKS_WIDTH-1))

#define BLOCK_START_X ((320- BLOCK_MAX_WIDTH_PIXELS)/2)
#define BLOCK_START_Y 20

#define PLAYFIELD_BORDER_X (BLOCK_START_X - 5)
#define PLAYFIELD_BORDER_Y 15
#define PLAYFIELD_BORDER_WIDTH BLOCK_MAX_WIDTH_PIXELS+8
#define PLAYFIELD_BORDER_HEIGHT (240-4-PLAYFIELD_BORDER_Y)

#define BALL_RADIUS 2

//Dist the border lines from the side of the screen.
#define BORDER_SIZE 8

enum GameState {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER,
    GAME_STATE_EXITING
};


//#define MAX_BALL_DIRECIONS 16

/* Game Variables */
extern char brickPlayField[MAX_BLOCKS_HEIGHT][MAX_BLOCKS_WIDTH];
extern int numBricksOnPlayfield;

void NextLevel();

//KEYS
enum KeyCodes {
    KEY_NONE,
    KEY_ARROW_UP,
    KEY_ARROW_RIGHT,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ENTER,
    KEY_ESC
};

void BreakoutInit();
void BreakoutUpdate();
void BreakoutRender();
int BreakoutIsGameExiting();
void BreakoutKeyHandler(int key);
void AddPlayerScore(int score);
void SetGameOver();
void DecPlayerBall();



#endif
