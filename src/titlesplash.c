/************************************************************************
 * Title : splashtitle.c
 *
 * Author : Carl Stika
 *
 *
 *
 ************************************************************************/
#include <graphx.h>

static int time = 0;
static char **currentTitle=0;
static int titleArrayLength = 0;

//The times are in ticks
#define TIME_IN 10
#define TIME_WAIT   30
#define TIME_OUT 10
#define TIME_BEFORE_RESTART 5

#define SCREEN_CHAR_WIDTH 40
#define SCREEN_CHAR_HEIGHT 30

static int sy=0;
static int by=SCREEN_CHAR_HEIGHT;

void SplashTitleInit(char **inTitle, int length) {
	time = 0;
    currentTitle = inTitle;
    titleArrayLength = length;
}


int SplashTitleUpdate() {
    int max = (SCREEN_CHAR_HEIGHT/2)-(titleArrayLength/2);

    if(time>=max) {
        if(time>=(max+TIME_WAIT)) {
            //Move it out to the side.
            time = 0; //test
        }
        else {
            sy = max;
            by = SCREEN_CHAR_HEIGHT-max;
        }
    }

    time++;
    return 0;
}

void SplashTitleRender() {
    int h = titleArrayLength/2;
    int i=0;

/*    for(i=0;i<h;i++) {
        gfx_PrintStringXY(currentTitle[i], (320 - gfx_GetStringWidth(currentTitle[i])) / 2, (sy+i)*8 );
    }

    for(i=h;i<titleArrayLength;i++) {
        gfx_PrintStringXY(currentTitle[i], (320- gfx_GetStringWidth(currentTitle[i])) / 2, (by+i-h)*8 );
    }
  */
    gfx_SetTextFGColor(11); //white
    gfx_SetTextBGColor(0);
    for(i=0;i<titleArrayLength;i++) {
        gfx_PrintStringXY(currentTitle[i], 8, (8+i)*8 );
    }

 }


/* vim: set et ts=4 sw=4 sts=4 ft=c:expandtab:ai:cindent */
