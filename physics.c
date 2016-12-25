#include "gamedef.h"

void PaddleLimits()
{
	if (p[0].x >= (SCREEN_WIDTH - PADDLE_WIDTH))
		p[0].x = (SCREEN_WIDTH - PADDLE_WIDTH);
	if (p[0].x < 0)
		p[0].x = 0;
}

void BallPhysics()
{
	b[0].x += b[0].dx;
	b[0].y += b[0].dy;
	
	/* bounce?....needs help*/
	if (b[0].x < 0 || b[0].x > screen->w - 1)
		b[0].dx = -b[0].dx;
	if (b[0].y < 0 || b[0].y > screen->h - 1)
		b[0].dy = -b[0].dy;
	
	/* bounce off paddle?.....needs help */
	if (b[0].y > p[0].y)
		b[0].dy = -b[0].dy;
}
