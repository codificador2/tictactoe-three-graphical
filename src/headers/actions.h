#pragma once

#include "./util.h"

/*
 * Returns:
 * 0: Can perform action
 * 1: Cannot perform action
 * 2: Player succeeded in action
 */
int performAction(appState* state);

void renderStealAction(appState* state);
SDL_AppResult handleStealActionEvent(appState* state, SDL_Event* event);

void renderCheckIdAction(appState* state);
SDL_AppResult handleCheckIdActionEvent(appState* state, SDL_Event* event);

void renderCheckCardAction(appState* state);
SDL_AppResult handleCheckCardActionEvent(appState* state, SDL_Event* event);

void renderLookAction(appState* state);
SDL_AppResult handleLookActionEvent(appState* state, SDL_Event* event);

void renderInventoryAction(appState* state);
SDL_AppResult handleInventoryActionEvent(appState* state, SDL_Event* event);

void renderShopAction(appState* state);
SDL_AppResult handleShopActionEvent(appState* state, SDL_Event* event);

void renderBlockAction(appState* state);
SDL_AppResult handleBlockActionEvent(appState* state, SDL_Event* event);

