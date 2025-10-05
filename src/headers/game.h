#pragma once

#include "./util.h"

#define MIN_WINDOW_WIDTH 480
#define MIN_WINDOW_HEIGHT 360

SDL_AppResult commonEventLogic(appState* state, SDL_Event* event);

void renderBoard(appState* state);

void renderSelector(appState* state);

void renderActions(appState* state);
