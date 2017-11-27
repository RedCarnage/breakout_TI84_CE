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
/******************************************************************************
 * ball.c
 *
 * @author  Carl Stika
 * @date October 10, 2017
 *
 *****************************************************************************/
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
#include "ball.h"

static enum BallState ballState = BALL_STATE_UNKNOWN;
static float ballx = 0; // Current center position of ball
static float bally = 0; // Current center position of ball

static float old_ballx = 0; // Current center position of ball
static float old_bally = 0; // Current center position of ball

static float ballVelx = 0;
static float ballVely = 0;

/**
 *
 */
float  getBallX() {
    return ballx;
}

float  getBallY() {
    return bally;
}

float  getBallVelX() {
    return ballVelx;
}
float  getBallVelY() {
    return ballVely;
}

/**********************************************************************
 * Check if ball hit a brick.
 **********************************************************************/

/**
 * calculate reflection of ball off the brick.
 * @param x            ball pos x
 * @param y            ball pos y
 * @param cx           center x of paddle
 * @param cy           top y pos of paddle
 * @param widthDivisor ratio of width over hieght of the paddle/
 */
static void reflectBall(int x, int y, int cx, int cy, float widthDivisor) {
   float nx, ny, len, d;

   //find the normal direction of the hit
   nx = (float)(x - cx)/widthDivisor;  //since the width is greater then the height
   ny = (float)(y - cy);

   //is there are equal then maybe we should do a 45 degree normal
   if(abs(nx)>abs(ny)) {
       //xdir is the mazimun
       if(nx>0) {
           nx = 1.0f;
           ny = 0.0f;
       }
       else {
           nx = -1.0f;
           ny = 0.0f;
       }
   }
   else {
       //y is the maximun
       if(ny>0) {
           nx = 0.0f;
           ny = 1.0f;
       }
       else {
           nx = 0.0f;
           ny = -1.0f;
       }
   }

   len = (float)sqrt(nx*nx+ny*ny);
   nx/=len;
   ny/=len;

   d = 2.0f * ((nx*ballVelx)+(ny*ballVely));

   ballVelx = ballVelx - d*nx;
   ballVely = ballVely - d*ny;
}

/**
 * didBallHitPaddle
 */
static int didBallHitPaddle(int x, int y, int paddleX, int paddleY) {
  int result = COLLIDE_NONE;

  if(x>=(paddleX-(PADDLE_NORMAL_SIZE_WIDTH/2)-BALL_RADIUS) && x<=(paddleX+(PADDLE_NORMAL_SIZE_WIDTH/2)+BALL_RADIUS) &&
      y>=(paddleY-BALL_RADIUS) && y<=(paddleY+PADDLE_NORMAL_SIZE_HEIGHT) ) {
        result |= COLLIDE_FROM_TOP;

        reflectBall(x, y, paddleX, paddleY+(PADDLE_NORMAL_SIZE_HEIGHT/2), 4.0f);
  }

  return result;

}

/**
 * checks if ball hit the block
 *
 * @param  x      x position of the ball
 * @param  y      y position of the ball
 * @param  radius radius of the ball
 * @return        returns flags from enum CollisionDirection. The flags are or'ed together.
 */
static int didBallHitBlock(int x, int y, int radius) {
  int result = COLLIDE_NONE;
  int i,j;
  int bx=BLOCK_START_X;
  int by=BLOCK_START_Y;

  for(j=0;j<MAX_BLOCKS_HEIGHT && result==COLLIDE_NONE;j++) {
      bx = BLOCK_START_X;
      for(i=0;i<MAX_BLOCKS_WIDTH && result==COLLIDE_NONE;i++) {
          if(brickPlayField[j][i]>0) {
            if( x>=(bx-BALL_RADIUS) && (x<=(bx+BLOCK_WIDTH+BALL_RADIUS)) &&
                y>=(by-BALL_RADIUS) && (y<=(by+BLOCK_HEIGHT+BALL_RADIUS)) ) {

                  //collide with the brick
                  brickPlayField[j][i] = 0;
                  numBricksOnPlayfield--;
                  if(y>=(by+BLOCK_HEIGHT-BALL_RADIUS)) {
                    result |= COLLIDE_FROM_TOP;
                  }
                  else {
                    if(y<(by+BALL_RADIUS)) {
                      result |= COLLIDE_FROM_BELOW;
                    }
                  }

                  if(x>(bx+BLOCK_WIDTH-BALL_RADIUS)) {
                    result |= COLLIDE_FROM_LEFT;
                  }
                  else {
                    if(x<(bx+BALL_RADIUS)) {
                      result |= COLLIDE_FROM_RIGHT;
                    }
                  }

                  if(result!=COLLIDE_NONE) {
                    reflectBall(x, y, (bx + BLOCK_WIDTH/2),(by + BLOCK_HEIGHT/2), 2.0f);
                    AddPlayerScore(10);
                  }

            }
          }
          bx+=BLOCK_WIDTH + BLOCK_SPACE_WIDTH;
      }
      by+=BLOCK_HEIGHT + BLOCK_SPACE_HEIGHT;
  }

  return result;
}

/**
 * What haqppens when the play goes off the playfield?
 * Decrement the number of player balls and change the ball state.
 */
static void ballExitedPlayfield() {
    DecPlayerBall();

    ballState =  BALL_STATE_STUCK_TO_PADDLE;
}

void setBallState(int state) {
    ballState = state;
}


/**
 * checkForBallCollisionBoundaries
 *
 * Test Collision against boundary
 * Will check the boundaries first and then the blocks/
 *
 * @return returns flags from enum CollisionDirection. The flags are or'ed together.
 */
static int checkForBallCollisionBoundaries() {
    int result = COLLIDE_NONE;

    //check for boundarys
    if(ballVelx<0 && ballx<=(PLAYFIELD_BORDER_X+BALL_RADIUS)) {
        result |= COLLIDE_FROM_LEFT;
        ballVelx = -ballVelx;
    }
    if(ballVelx>0 && ballx>=(PLAYFIELD_BORDER_X+PLAYFIELD_BORDER_WIDTH-BALL_RADIUS)) {
        result |= COLLIDE_FROM_RIGHT;

        ballVelx = -ballVelx;
    }
    if(ballVely<0 && bally<=PLAYFIELD_BORDER_Y+BALL_RADIUS) {
        result |= COLLIDE_FROM_TOP;
        ballVely = -ballVely;
    }
    if(ballVely>0 && bally>=(240-10-BALL_RADIUS)) {
        result = COLLIDE_OUT_OF_BOUNDS;
        ballVely = -ballVely;

        ballExitedPlayfield();
    }

    return result;
}

/**
 * updateBall
 *
 * @param paddleX position of paddle
 * @param paddleY position of paddle
 */
void updateBall(int paddleX, int paddleY) {
    switch(ballState) {
        case BALL_STATE_STUCK_TO_PADDLE:
            ballVelx = 0;
            ballVely = 0;
            ballx = paddleX;
            bally = paddleY - BALL_RADIUS;
            break;
        case BALL_STATE_MOVING:
            {
                //add the velocity and check for collisions.
                float newBallx = ballx + ballVelx;
                float newBally = bally + ballVely;

                int collideStatus = checkForBallCollisionBoundaries();
                if(collideStatus==COLLIDE_NONE) {
                  if(didBallHitBlock(newBallx, newBally, BALL_RADIUS)==COLLIDE_NONE) {
                    if(didBallHitPaddle(newBallx, newBally, paddleX, paddleY)==COLLIDE_NONE) {
                      ballx = newBallx;
                      bally = newBally;
                    }
                  }
                }
            }

            break;
        default:
            break;
    }
}

/**
 * checks if ball is stuck to paddle state
 * @return true is the ball is stuck
 */
bool ballStuckToPaddle() {
    return (ballState ==  BALL_STATE_STUCK_TO_PADDLE);
}

/**
 * Change the state of the ball if the ball was stuck
 *
 */
void ballReleaseFromPaddle() {
    if(ballStuckToPaddle()) {
        ballState = BALL_STATE_MOVING;

        ballVelx = 6.0f;
        ballVely = -3.0f;
    }
}

/**
 * Sets the initial state of the ball
 */
void InitBall() {

    ballState =  BALL_STATE_STUCK_TO_PADDLE;
    ballx = PLAYFIELD_BORDER_X + PLAYFIELD_BORDER_WIDTH/2;
    bally = (240 - BORDER_SIZE - DIST_PADDLE_FROM_BOTTOM) - BALL_RADIUS;

    old_ballx = ballx;
    old_bally = bally;

    ballVelx = 0.0f;
    ballVely = 0.0f;
}

/**
 *
 *  Currently not used.
 **/
static void EraseBall() {
    gfx_SetColor(0); //background
    gfx_FillCircle_NoClip(old_ballx, old_bally, BALL_RADIUS);
}

/**
 * Draw the ball at the current position.
 */
void DrawBall() {
    gfx_SetColor(11); //white
    gfx_FillCircle_NoClip(ballx, bally, BALL_RADIUS);
}
