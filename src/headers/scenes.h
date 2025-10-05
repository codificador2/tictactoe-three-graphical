#pragma once

#include "./util.h"

void renderMainMenuScene(appState* state);

void renderGameBoardScene(appState* state);

void renderGameActionScene(appState* state);

void renderActionScene(appState* state);

SDL_AppResult handleMainMenuEvent(appState* state, SDL_Event* event);

SDL_AppResult handleGameBoardEvent(appState* state, SDL_Event* event);

SDL_AppResult handleGameActionEvent(appState* state, SDL_Event* event);

SDL_AppResult handleActionEvent(appState* state, SDL_Event* event);
