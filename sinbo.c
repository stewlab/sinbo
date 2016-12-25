#include "gamedef.h"

double time_scale = 0;

void DrawGraphics()
{
	SDL_Rect src, dest;
	
	src.x = 0;
        src.y = 0;
	src.w = background->w;
	src.h = background->h;
	dest = src;
	SDL_BlitSurface(background, &src, screen, &dest);

	src.x = 0;
	src.y = 0;
	src.w = paddle->w;
	src.h = paddle->h;
	dest.x = p[0].x;
	dest.y = 460;
	dest.w = paddle->w;
	dest.h = paddle->h;
	SDL_BlitSurface(paddle, &src, screen, &dest);
	
	src.x = 0;
	src.y = 0;
	src.w = ball->w;
	src.h = ball->h;
	dest.x = b[0].x - ball->w/2;
	dest.y = b[0].y - ball->h/2;
	dest.w = ball->w;
	dest.h = ball->h;
	SDL_BlitSurface(ball, &src, screen, &dest);
}


void InitBall()
{
	b[0].x = rand() % screen->w;
	b[0].y = rand() % screen->h;
	b[0].dx = (rand() % (MAX_BALL_SPEED * 2)) - MAX_BALL_SPEED;
	b[0].dy = (rand() % (MAX_BALL_SPEED * 2)) - MAX_BALL_SPEED;
}


int LoadGameData()
{
	SDL_Surface *temp;
	char string[50];
	
	sprintf(string, "pixmaps/bg_640x480.png");
	temp = IMG_Load(string);
	background = SDL_DisplayFormat(temp);
	if (background == NULL) {
		printf("Unable to load background: %s\n", SDL_GetError() );
		return 1;
	}
	SDL_FreeSurface(temp);
	
	sprintf(string, "pixmaps/paddle_75x15.png");
	temp = IMG_Load(string);
	paddle = SDL_DisplayFormat(temp);
	if (paddle == NULL) {
		printf("Unable to load the paddle: %s\n", SDL_GetError() );
		return 1;
	}
	SDL_FreeSurface(temp);
	
	sprintf(string, "pixmaps/ball_15x15.png");
	temp = IMG_Load(string);
	ball = SDL_DisplayFormat(temp);
	if (ball == NULL) {
		printf("Unable to load the ball: %s\n", SDL_GetError() );
		return 1;
	}
	SDL_FreeSurface(temp);
	
	SDL_SetColorKey(ball, SDL_SRCCOLORKEY,
				SDL_MapRGB(ball->format, 0, 0, 0));
}


void UnloadGameData(void)
{
	if (background != NULL) {
		SDL_FreeSurface(background);
		background = NULL;
	}
	if (paddle != NULL) {
		SDL_FreeSurface(paddle);
		paddle = NULL;
	}
	if (ball != NULL) {
		SDL_FreeSurface(ball);
		ball = NULL;
	}
}


/*#######################################################
	MAIN GAME LOOP
########################################################*/
void GameLoop()
{
	int quit = 0;
	int x, y;
	
	int prev_ticks = 0, cur_ticks = 0;
	int start_time, end_time;
	int frames_drawn = 0;
	
	prev_ticks = SDL_GetTicks();
	
	start_time = time(NULL);
	SDL_WarpMouse(x, y);
	while (quit == 0) {
		
		/* i think i need help here.  i want teh game to run
		at 30 frames per second */
		prev_ticks = cur_ticks;
		cur_ticks = SDL_GetTicks();
		time_scale = (double)(cur_ticks-prev_ticks)/30.0;
		
		SDL_Event event;
		SDL_PollEvent(&event);
		
		if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)
			quit = 1;

			if (event.key.keysym.sym == SDLK_LEFT) {
				p[0].direction = 1;
				p[0].dx = 20;
				p[0].x -= p[0].dx;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				p[0].direction = 2;
				p[0].dx = 20;
				p[0].x += p[0].dx;
			}
			else if (event.type == SDL_MOUSEMOTION) {
				p[0].x = event.motion.x;
			}
		
		
		DrawGraphics(screen, &paddle);
		InitBall();
		PaddleLimits();
		BallPhysics();
		
		SDL_Flip(screen);
		
		frames_drawn++;
	}
	
	end_time = time(NULL);
	if (start_time == end_time) end_time++;
	
	/* show framerate and stuff */
	printf("Drew %i frames in %i seconds, for a framerate of %.2f fps.\n",
	   frames_drawn,
	   end_time-start_time,
	   (float)frames_drawn/(float)(end_time-start_time));
}



int main(int argc, char *argv[])

{
	int i;
	int fullscreen = 0;
	int hwsurface = 0;
	int doublebuf = 0;
	
	if (argc < 1)
		return 1;
	
	for ( i=0; i<argc; i++) {
		if (!strcmp(argv[i], "--hwsurface") ) {
			hwsurface = 1;
		} else if (!strcmp(argv[i], "--doublebuf") ) {
			doublebuf = 1;
		} else if (!strcmp(argv[i], "--fullscreen") ) {
			fullscreen = 1;
		} else if (!strcmp(argv[i], "--help") ) {
				printf( "Sinbo, written by stateq2\n");
				printf( "Usage: sinbo [ option(s) ]\n");
				printf("  DISPLAY OPTIONS, for tweaking and experimenting:\n");
				printf("    --fullscreen\n");
				printf("    --hwsurface  (use an SDL hardware surface if possible)\n");
				printf("    --doublebuf  (use SDL double buffering if possible)\n");
				printf("  The display options default to a windowed, software buffer.\n\n");
				printf("  OTHER OPTIONS.\n");
				printf("    --help  (displays this text)\n");
				
				exit(0);
		}
	}
	
	
	/* initialize SDL video */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	atexit(SDL_Quit);
	
	/* set up 32-bit  double buffered video mode */
	if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		(hwsurface ? SDL_HWSURFACE : SDL_SWSURFACE) |
		(doublebuf ? SDL_DOUBLEBUF : 0) |
		(fullscreen ? SDL_FULLSCREEN : 0) ) == NULL) {
			printf( "Unable to set video mode: %s\n", SDL_GetError() );
			return 1;
		}
		
	screen = SDL_GetVideoSurface();
	
	SDL_WM_SetCaption("Sinbo", "Sinbo");
	SDL_ShowCursor(0);
	
	LoadGameData();
	GameLoop();
	SDL_ShowCursor(1);
	UnloadGameData();
	
	return 0;
}
