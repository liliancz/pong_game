#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")


/*
void set_Pixel(unsigned char* buffer, int width, int x, int y, int r, int g, int b, int a)
{
	int first_byte_of_the_pixel = y * width * 3 + x * 3;
	buffer[first_byte_of_the_pixel] = r;
	buffer[first_byte_of_the_pixel + 1] = g;
	buffer[first_byte_of_the_pixel + 2] = b;
	buffer[first_byte_of_the_pixel + 3] = a;
}
*/
//Alternative example

struct Pixel
{
	unsigned char r, g, b, a;
};

struct Rect
{
	Pixel color;
	float x, y, w, h;
};

void set_Pixel_Alternative(Pixel *pixel, Pixel color)
{
	Pixel *p = (Pixel*)pixel;
	p->r = color.r;
	p->g = color.g;
	p->b = color.b;
	p->a = color.a;
}

// fill_Rectangle(my_own_buffer, screen_width, screen_height, 50, 50, 50, 200, 0, 0, 255, 255);

void fill_Rectangle(Pixel *buffer, int buffer_width, int buffer_height, Rect rect)
{
	for (int i = rect.y; i < rect.y + rect.h; i++)
	{
		for (int j = rect.x; j < rect.x + rect.w; j++)
		{
			set_Pixel_Alternative(&buffer[i * buffer_width + j], rect.color);
		}
	}
}

//move_rectangle(paddle1.x, paddle1.y, paddle1.w, paddle1.h);

void move_rectangle(Rect *rect, float speedx, float speedy)
{
	rect->x += speedx;
	rect->y += speedy;
}

int main(int argc, char **argv)
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;

	unsigned char prev_key_state[256];
	unsigned char *keys = NULL;
	SDL_Window *window = NULL;

	SDL_Init(SDL_INIT_VIDEO);

	prev_key_state[256];
	keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	window = SDL_CreateWindow("WONDERFUL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_Surface *your_draw_buffer = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *screen = SDL_GetWindowSurface(window);

	Pixel *my_own_buffer = (Pixel*)malloc(sizeof(Pixel)*screen_width*screen_height);

	// Declaration of variables
	Rect ball;
	ball.x = (screen_width / 2)-5;
	ball.y = (screen_height / 2)-5;
	ball.w = 10;
	ball.h = 10;
	ball.color.a = 255;
	ball.color.b = 255;
	ball.color.r = 255;
	ball.color.g = 255;

	float ballspeed_x = -1.0;
	float ballspeed_y = 1.0;


	Rect paddle1;
	paddle1.x = 2;
	paddle1.y = (screen_height / 2) -50 ;
	paddle1.w = 10;
	paddle1.h = 100;
	paddle1.color.a = 255;
	paddle1.color.b = 255;
	paddle1.color.r = 255;
	paddle1.color.g = 255; 

	float paddle1speed = 0.0;

	Rect paddle2;
	paddle2.x = screen_width - 12;
	paddle2.y = (screen_height / 2)-50;
	paddle2.w = 10;
	paddle2.h = 100;
	paddle2.color.a = 255;
	paddle2.color.b = 255;
	paddle2.color.r = 255;
	paddle2.color.g = 255;

	float paddle2speed = 0.0;


	for (;;)
	{
		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}


		// PONG GAME

		//update
		move_rectangle(&ball, ballspeed_x, ballspeed_y);
		// Collision top
		if (ball.y < 0)
		{
			ball.y = 0;
			ballspeed_y *= -1;
		}
		// colosion down
		else if (ball.y + ball.h >= screen_height)
		{
			ball.y = screen_height - ball.h - 1;
			ballspeed_y *= -1;
		}
		//collision right
		if (ball.x < 0)
		{
			ball.x = 0;
			ballspeed_x *= -1;
		}
		// collision left
		else if (ball.x + ball.w >= screen_width)
		{
			ball.x = screen_width - ball.w - 1;
			ballspeed_x *= -1;
		}
		
		
		
		
		// 1s. follow the ball
		// 2. paddle cant go off screen
		//	3. ball change direction when hits the paddle
		//if (paddle1.y > 0 + ballspeed_y && paddle1.y + paddle1.h < screen_height + ballspeed_y) paddle1.y = ball.y - (paddle1.h / 2) + (ball.h / 2);
		
		if (paddle1.y > 0 && ball.x < screen_width*0.6)
		{
			paddle1.y = ball.y - paddle1.h / 2 ;

			if (paddle1.y > screen_height - paddle1.h)
			{
				paddle1.y = screen_height - paddle1.h - 1;
			}

			if (paddle1.y < paddle1.h/3 && ball.y < paddle1.h)
			{
				paddle1.y = 1;
			}
		}

		if (paddle2.y > 0 && ball.x > screen_width*0.3)
		{
			paddle2.y = ball.y - paddle2.h / 2;

			if (paddle2.y > screen_height - paddle2.h)
			{
				paddle2.y = screen_height - paddle2.h - 1;
			}

			if (paddle2.y < paddle2.h / 3 && ball.y < paddle2.h)
			{
				paddle2.y = 1;
			}
		}


		//draw
		for (int i = 0; i < screen_width*screen_height; i++)
		{
			my_own_buffer[i].r = 0;
			my_own_buffer[i].g = 0; //rand ()%256;
			my_own_buffer[i].b = 0;//rand() % 256;
			my_own_buffer[i].a = 255;//rand() % 256;
		}

		fill_Rectangle(my_own_buffer, screen_width, screen_height, paddle1);

		fill_Rectangle(my_own_buffer, screen_width, screen_height, paddle2);


		

		fill_Rectangle(my_own_buffer, screen_width, screen_height, ball);

		memcpy(your_draw_buffer->pixels, my_own_buffer, sizeof(Pixel)*screen_width*screen_height);

		//BLIT BUFFER TO SCREEN
		SDL_BlitScaled(your_draw_buffer, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
	}

	return 0;
}