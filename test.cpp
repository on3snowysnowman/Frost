#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define OPEN_SANS "data/fonts/Hack-Regular.ttf"


void default_code()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow("TTF Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *rend = NULL;
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    rend = SDL_CreateRenderer(win, -1, render_flags);


    TTF_Font *fonts[8];
    fonts[0] = TTF_OpenFont(OPEN_SANS, 12);
    fonts[1] = TTF_OpenFont(OPEN_SANS, 14);
    fonts[2] = TTF_OpenFont(OPEN_SANS, 16); 
    fonts[3] = TTF_OpenFont(OPEN_SANS, 20);
    fonts[4] = TTF_OpenFont(OPEN_SANS, 24);
    fonts[5] = TTF_OpenFont(OPEN_SANS, 30);
    fonts[6] = TTF_OpenFont(OPEN_SANS, 36);
    fonts[7] = TTF_OpenFont(OPEN_SANS, 42);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color grey = {40, 40, 40, 255};

    SDL_Texture* font_textures[8];

    for (int i=0; i<8; i++) {
        SDL_Surface *surface = TTF_RenderUTF8_Shaded(fonts[i], "I look meh.", white, grey);
        font_textures[i] = SDL_CreateTextureFromSurface(rend, surface);

        SDL_FreeSurface(surface);

        // SDL_QueryTexture(texture, NULL, NULL, &(txt_box.w), &(txt_box.h));
        // SDL_RenderCopy(rend, texture, NULL, &txt_box);
        // txt_box.y += txt_box.h + 5;
    }

    int quit = 0;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) goto QUIT;
        }

        SDL_SetRenderDrawColor(rend, grey.r, grey.g, grey.b, 255);
        SDL_RenderClear(rend);

        SDL_Rect txt_box = {10, 10, 0, 0};
        for (int i=0; i<8; i++) {
            // SDL_Surface *surface = TTF_RenderUTF8_Shaded(fonts[i], "I look meh.", white, grey);
            // SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surface);
            SDL_QueryTexture(font_textures[i], NULL, NULL, &(txt_box.w), &(txt_box.h));
            SDL_RenderCopy(rend, font_textures[i], NULL, &txt_box);
            txt_box.y += txt_box.h + 5;
        }

        SDL_RenderPresent(rend);
        SDL_Delay(16);
    }

    QUIT:

    SDL_Quit();
}


int main()
{
    
}