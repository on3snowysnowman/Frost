#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// ASCII characters to render
const std::string asciiChars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

// Function to create an SDL_Texture with the full ASCII string
SDL_Texture* CreateFontAtlas(SDL_Renderer* renderer, TTF_Font* font, int& glyphWidth, int& glyphHeight) {
    // Get the size of one glyph (monospaced, so all are the same)
    if (TTF_SizeText(font, "A", &glyphWidth, &glyphHeight)) {
        std::cerr << "TTF_SizeText failed: " << TTF_GetError() << "\n";
        return nullptr;
    }

    // Render the entire ASCII string onto a surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, asciiChars.c_str(), {255, 255, 255, 255});
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Blended failed: " << TTF_GetError() << "\n";
        return nullptr;
    }

    // Convert the surface to an SDL_Texture
    SDL_Texture* fontAtlas = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!fontAtlas) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << "\n";
    }

    SDL_FreeSurface(textSurface);
    return fontAtlas;
}

// Function to render a specific character from the texture
void RenderCharacter(SDL_Renderer* renderer, SDL_Texture* fontAtlas, char character, int glyphWidth, int glyphHeight, int xPos, int yPos) {
    int index = asciiChars.find(character);
    if (index == std::string::npos) return; // Skip unknown characters

    // Calculate the position of the character in the texture
    SDL_Rect srcRect = {index * glyphWidth, 0, glyphWidth, glyphHeight};
    SDL_Rect destRect = {xPos, yPos, glyphWidth, glyphHeight};

    SDL_RenderCopy(renderer, fontAtlas, &srcRect, &destRect);
}

int main(int argc, char* argv[]) {
    // Initialize SDL and SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1) {
        std::cerr << "Initialization failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Font Atlas Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load font
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/Hack-Regular.ttf", 32);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
        return 1;
    }

    int glyphWidth, glyphHeight;
    SDL_Texture* fontAtlas = CreateFontAtlas(renderer, font, glyphWidth, glyphHeight);
    if (!fontAtlas) return 1;

    // Main loop
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render the entire texture at the top of the screen (for testing)
        SDL_Rect fullAtlasRect = {0, 0, glyphWidth * asciiChars.size(), glyphHeight};
        SDL_Rect screenRect = {50, 50, glyphWidth * asciiChars.size(), glyphHeight};
        SDL_RenderCopy(renderer, fontAtlas, &fullAtlasRect, &screenRect);

        // Render individual characters below
        const std::string testString = "Hello, World!";
        for (size_t i = 0; i < testString.size(); ++i) {
            RenderCharacter(renderer, fontAtlas, testString[i], glyphWidth, glyphHeight, 50 + i * glyphWidth, 150);
        }

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(fontAtlas);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
