#include "FrostEngine.h"


// Constructors / Deconstructor

FrostEngine::FrostEngine() : TARGET_MILISECONDS_PER_FRAME(1000 / TARGET_FPS) {}

FrostEngine::~FrostEngine() {}


// Public

int FrostEngine::get_screen_width() { return s_screen_width; }

int FrostEngine::get_screen_height() { return s_screen_height; }



// Protected

void FrostEngine::quit() { m_is_active = false; }


// Private

void FrostEngine::_init_SDL() {}

void FrostEngine::_start_simulation() {}

void FrostEngine::_handle_SDL_events() {}

void FrostEngine::_clear_SDL_renderer() {}

void FrostEngine::_present_SDL_renderer() {}
