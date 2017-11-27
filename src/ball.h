#ifndef BALL_H
#define BALL_H
/**
 * Header file for the Ball Functions
 *
 * @author Carl Stika
 * @Date October 10, 2017
 *
 */

enum BallState {
    BALL_STATE_STUCK_TO_PADDLE,
    BALL_STATE_MOVING,
    BALL_STATE_UNKNOWN
};

/*Collision states*/
enum CollisionDirection {
    COLLIDE_NONE = 0x0,
    COLLIDE_FROM_TOP = 0x01,
    COLLIDE_FROM_BELOW = 0x02,
    COLLIDE_FROM_LEFT = 0x04,
    COLLIDE_FROM_RIGHT = 0x08,
    COLLIDE_OUT_OF_BOUNDS = 0x10
};

void updateBall(int paddleX, int paddleY);
void ballReleaseFromPaddle();
bool ballStuckToPaddle();
void setBallState(int state);
void InitBall();
void DrawBall();


float getBallX();
float getBallY();
float getBallVelX();
float getBallVelY();


#endif
