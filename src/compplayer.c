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

/***************************************************************************
 * compplayer.c
 *
 *  @author  Carl Stika
 *  @date October 10, 2017
 *
 *  Functions for computer playing the breakout game.
 *  This is used in attract mode.
 *
 **************************************************************************/
#include <stdlib.h>
#include <tice.h>

#include "compplayer.h"
static int initsrand = 0;

int CompPlayer_getComputerMove(struct GameInfo *gameInfo) {
    int compMove = MOVE_NOTHING;
    int value = 0;

    if(initsrand==0) {
        initsrand = 1;
        srand(rtc_Time());
    }

    if(gameInfo->isBallStuckToPaddle) {
        //the ball is on the paddle.
        //Just random move and then release ball.
        value = rand()%20;
        if(value==0) {
            compMove = MOVE_RELEASE_BALL;
        }
        else {
            if(value<10) {
                compMove = MOVE_PADDLE_RIGHT;
            }
            else {
                compMove = MOVE_PADDLE_LEFT;
            }
        }
    }
    else {
        //Calculate where the ball may go.
        //Calculate the point the paddle intersects the balls line. Not taking in account the bounce.
        float calcX = ((((float)gameInfo->paddleY - (float)gameInfo->ballY)*gameInfo->ballVelX)/gameInfo->ballVelY) + (float)gameInfo->ballX;

        if((int)calcX<gameInfo->paddleX) {
            compMove = MOVE_PADDLE_LEFT;
        }
        else {
            compMove = MOVE_PADDLE_RIGHT;
        }
    }

    return compMove;
}
