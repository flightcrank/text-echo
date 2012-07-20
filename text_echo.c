
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"

#define CWIDTH 11
#define CHEIGHT 19

static SDL_Surface *screen;
static SDL_Surface *charmap;
static SDL_Surface *scroll_buff;

void draw_bg() {
		
	SDL_Rect src;
	
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	int r = SDL_FillRect(screen,&src,0);
	
	if (r != 0) {
		
		printf("fill rectangle faliled ");
	}
}

int scroll_up(int x, int y) {

	SDL_Rect src, dest;
	
	//copy screen to buffer
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	dest.x = x;
	dest.y = y;
	dest.w = screen->w;
	dest.h = screen->h;

	SDL_BlitSurface(screen, &src, scroll_buff, &dest);

	//paint screen black
	draw_bg();

	//move buffer back to screen scrolled up one line higher
	src.x = 0;
	src.y = CHEIGHT;
	src.w = screen->w;
	src.h = screen->h - CHEIGHT;
	
	dest.x = x;
	dest.y = y;
	dest.w = screen->w;
	dest.h = screen->h - CHEIGHT;

	SDL_BlitSurface(scroll_buff, &src, screen, &dest);

	return 0;
}

int printc(char c, int x, int y) {

	SDL_Rect src, dest;
	char *cmap[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 
			"abcdefghijklmnopqrstuvwxyz", 
			"0123456789 {}<>[],.:;'\"|~`",
			"/?!@#$%^&*()_+-="};
	int i,j;

	src.x = 0;
	src.y = 0;
	src.w = CWIDTH;
	src.h = CHEIGHT;
	
	dest.x = x;
	dest.y = y;
	dest.w = CWIDTH;
	dest.h = CHEIGHT;

	for (i = 0; i < 4; i++) {
	
		for(j = 0; j < strlen(cmap[i]); j++) {
			
			if (c == '\n') {
			
				return 1;
			}

			if(c == cmap[i][j]) {

				SDL_BlitSurface(charmap, &src, screen, &dest);
				return 2;
			}
			
			src.x += CWIDTH;
		}
		
		src.y += CHEIGHT;
		src.x = 0;
	}

	
	return 0;
}

int main() {

	/* Initialize SDL’s video system and check for errors */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {

		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	/* Make sure SDL_Quit gets called when the program exits! */
	atexit(SDL_Quit);
	
	/* Attempt to set a 640x480 24 bit color video mode */
	screen = SDL_SetVideoMode(640, 480, 24, SDL_DOUBLEBUF );
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	//load charmap to a surface
	charmap = SDL_LoadBMP("charmap.bmp");

	if (charmap == NULL) {
	
		printf("Error loading image: %s\n", SDL_GetError());
		return 1;
	}

	//image buffer to handle vertical scrolling
	scroll_buff = SDL_CreateRGBSurface(0,640,480,24,0,0,0,0);

	if (scroll_buff == NULL) {
	
		printf("Error creating surface: %s\n", SDL_GetError());
		return 1;
	}


	//use colour key on the image (magenta value)	
	Uint32 colorkey = SDL_MapRGB(charmap->format, 255, 0, 255);

	//draw charmapwith a colour key enabled
	SDL_SetColorKey(charmap, SDL_SRCCOLORKEY, colorkey);


	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	SDL_Event event;

	//open file
	FILE *fp = fopen("hack.c","r");

	if (fp == NULL) {
		
		puts("error opening file");
		return 1;
	}
	
	//get first char of file
	char c = fgetc(fp);
	int x = 10;
	int y = 0;

	//draw background
	draw_bg();

	/* program loop */
	while (quit == 0) {
		
		//porcess events
		while (SDL_PollEvent(&event)) {
		
			int r;

			switch(event.type) {
				
				case SDL_KEYDOWN:
					
					//exit out of game loop if escape is pressed
					switch( event.key.keysym.sym ){
					
						case SDLK_ESCAPE:
							
							quit = 1;
						break;

						default:
							
							//at the end of file stop processing file
							if (c != EOF) {
								
								r = printc(c,x,y);
								c = fgetc(fp);
								
								//newline
								if (r == 1) {
							
									y += CHEIGHT;
									x = 10;
									
									//last line, move screen up one line
									if (y > 480 - CHEIGHT) {
										
										y = 480 - CHEIGHT;
										scroll_up(0,0);
									}
				
								} else if(r == 2) {
						
									x += CWIDTH;
								}
							} else {
							
								puts("END OF LINE");
							}
						break;
					}

				break;
			}
		}

		/* Ask SDL to update the entire screen. */
		SDL_Flip(screen);

		next_game_tick += 1000 / 30;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {

            		SDL_Delay(sleep);
        	}
	}

	//close file
	fclose(fp);

	//free charmap surface
	SDL_FreeSurface(charmap);
	
	return 0;
}
