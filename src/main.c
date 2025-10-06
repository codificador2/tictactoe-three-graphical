#include <stdlib.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "headers/util.h"
#include "headers/gameUtil.h"
#include "headers/scenes.h"
#include "headers/game.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	appState* state = calloc(1, sizeof(appState));
	if (state == NULL)
	{
		return SDL_APP_FAILURE;
	}
	state->window = NULL;
	state->renderer = NULL;

	if (!TTF_Init())
	{
		SDL_Log("Couldn't initialize SDL_ttf. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to initialize video. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetAppMetadata("thing 3", "1.0", "net.codificador2.thing3");
	if (!SDL_CreateWindowAndRenderer("thing 3", 640, 480, SDL_WINDOW_RESIZABLE, &state->window, &state->renderer))
	{
		SDL_Log("Failed to initialize window or renderer. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderVSync(state->renderer, 1);

	*appstate = state;

	getFont(state, "font.otf", 64.0f);

	SDL_AppResult res = getAllTextures(state);
	if (res != SDL_APP_CONTINUE)
		return res;

	SDL_GetWindowSizeInPixels(state->window, &state->wInfo.windowWidth, &state->wInfo.windowHeight);
	SDL_GetMouseState(&state->wInfo.mouseX, &state->wInfo.mouseY);

	state->selectedZone = ZONE_NONE;
	state->scene = SCENE_MAIN_MENU;
	state->updateZone = true;

	state->backgroundColor.r = 0.0f;
	state->backgroundColor.g = 0.0f;
	state->backgroundColor.b = 0.0f;
	state->backgroundColor.a = 1.0f;

	initGame(&state->game);

	state->game.xPlayer.inventory[ITEM_GUN] = 1;

	updateInvNums(state);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	appState* state;
	if (getAppState(appstate, &state) != 0) return SDL_APP_FAILURE;

	SDL_AppResult res = commonEventLogic(state, event);
	if (res != SDL_APP_CONTINUE)
		return res;

	switch (state->scene)
	{
		case SCENE_MAIN_MENU:
			return handleMainMenuEvent(state, event);
		case SCENE_GAME_BOARD:
			return handleGameBoardEvent(state, event);
		case SCENE_GAME_ACTION:
			return handleGameActionEvent(state, event);
		case SCENE_ACTION:
			return handleActionEvent(state, event);
		case SCENE_ITEM:
			return handleItemEvent(state, event);
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	appState* state;
	if (getAppState(appstate, &state) != 0) return SDL_APP_FAILURE;

	if (state->updateZone)
		state->selectedZone = ZONE_NONE;

	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_SetRenderDrawColorFloat(state->renderer, state->backgroundColor.r, state->backgroundColor.g, state->backgroundColor.b, 1.0f);
	SDL_RenderClear(state->renderer);

	switch (state->scene)
	{
		case SCENE_MAIN_MENU:
			renderMainMenuScene(state);
			break;
		case SCENE_GAME_BOARD:
			renderGameBoardScene(state);
			break;
		case SCENE_GAME_ACTION:
			renderGameActionScene(state);
			break;
		case SCENE_ACTION:
			renderActionScene(state);
			break;
		case SCENE_ITEM:
			renderItemScene(state);
			break;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult res)
{
	if (appstate != NULL)
	{
		appState* state = (appState*)appstate;

		TTF_CloseFont(state->tInfo.font);
		TTF_Quit();

		for (int i = 0; i < state->textureCount; i++)
		{
			SDL_DestroyTexture(state->textures[i].texture);
		}

		SDL_DestroyRenderer(state->renderer);
		SDL_DestroyWindow(state->window);
		SDL_free(state);
	}
}

