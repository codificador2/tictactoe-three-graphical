#pragma once

#include "./util.h"

int useItem(appState* state);

void renderDollarItem(appState* state);
SDL_AppResult handleDollarItemEvent(appState* state, SDL_Event* event);

void renderSetterItem(appState* state);
SDL_AppResult handleSetterItemEvent(appState* state, SDL_Event* event);

void renderRandomizerItem(appState* state);
SDL_AppResult handleRandomizerItemEvent(appState* state, SDL_Event* event);

void renderGunItem(appState* state);
SDL_AppResult handleGunItemEvent(appState* state, SDL_Event* event);

void renderDividerItem(appState* state);
SDL_AppResult handleDividerItemEvent(appState* state, SDL_Event* event);

void renderBagItem(appState* state);
SDL_AppResult handleBagItemEvent(appState* state, SDL_Event* event);

