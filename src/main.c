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
/**
 * Main entry point for a 'breakout' style game.
 *
 * @author Carl Stika
 * @date October 10, 2017
 *
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>

#include "breakout.h"
#include "gfx\sprites_gfx.h"

int ExitGame = 0;
void pollKeyboard();

/**
 * Main routine
 */
void main(void) {
    uint8_t loop = 0;

    /* Seed the random numbers */
    srand(rtc_Time());

    BreakoutInit();

    /* Start the graphics routines */
    gfx_Begin(gfx_8bpp);
    gfx_SetDrawBuffer();

    gfx_SetPalette(sprites_gfx_pal, sizeof(sprites_gfx_pal), 0);
    gfx_SetMonospaceFont(8);

    /* Loop until a key is pressed */
    //os_GetCSC()
    while (!BreakoutIsGameExiting() && !ExitGame) {
        pollKeyboard();

        BreakoutUpdate();

        BreakoutRender();

        /* Swap the buffer with the screen */
        gfx_SwapDraw();
    }

    /* End the graphics */
    gfx_End();
}

void pollKeyboard() {
    int key;
    int keyPress = 0;
    /* Update kb_Data */
    kb_Scan();

    /* Load group 7 registers */
    key = kb_Data[7];
    switch (key) {
        case kb_Down:
            keyPress = KEY_ARROW_DOWN;
            break;
        case kb_Right:
            keyPress = KEY_ARROW_RIGHT;
            break;
        case kb_Up:
            keyPress = KEY_ARROW_UP;
            break;
        case kb_Left:
            keyPress = KEY_ARROW_LEFT;
            break;
        default:
            break;
    }
    if( kb_Data[6]&kb_Clear) {
          ExitGame = 1;
    }
    if( kb_Data[1]&kb_2nd ) {
        keyPress = KEY_ESC;
    }
    if(kb_Data[5]&kb_6) {
        keyPress=KEY_ARROW_RIGHT;
    }
    if(kb_Data[3]&kb_4) {
        keyPress=KEY_ARROW_LEFT;
    }
    if(kb_Data[6]&&kb_Enter) {
        keyPress= KEY_ENTER;
    }

    BreakoutKeyHandler(keyPress);
}

/* vim: set et ts=4 sw=4 sts=4 ft=c:expandtab:ai:cindent */
