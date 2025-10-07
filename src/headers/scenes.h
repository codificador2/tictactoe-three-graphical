#pragma once

#include "./util.h"

void renderMainMenuScene(appState* state);

void renderGameBoardScene(appState* state);

void renderGameActionScene(appState* state);

void renderActionScene(appState* state);

void renderItemScene(appState* state);

void renderShopScene(appState* state);

void renderWinScene(appState* state);

SDL_AppResult handleMainMenuEvent(appState* state, SDL_Event* event);

SDL_AppResult handleGameBoardEvent(appState* state, SDL_Event* event);

SDL_AppResult handleGameActionEvent(appState* state, SDL_Event* event);

SDL_AppResult handleActionEvent(appState* state, SDL_Event* event);

SDL_AppResult handleItemEvent(appState* state, SDL_Event* event);

SDL_AppResult handleShopEvent(appState* state, SDL_Event* event);

SDL_AppResult handleWinEvent(appState* state, SDL_Event* event);
