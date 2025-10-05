#include "headers/util.h"
#include "headers/textures.h"

#include <SDL3_image/SDL_image.h>

int getAppState(void* appstate, appState** newState)
{
	if (appstate != NULL)
	{
		*newState = (appState*)appstate;
		return 0;
	}
	*newState = NULL;
	return 1;
}

int getTexture(appState* state, char* name)
{
	char* fullPath = NULL;
	SDL_asprintf(&fullPath, "%s../src/assets/%s", SDL_GetBasePath(), name);

	SDL_Surface* surf = IMG_Load(fullPath);
	if (surf == NULL)
		return 1;
	SDL_free(fullPath);

	state->textures[state->textureCount].w = surf->w;
	state->textures[state->textureCount].h = surf->h;
	state->textures[state->textureCount].texture = SDL_CreateTextureFromSurface(state->renderer, surf);
	if (state->textures[state->textureCount].texture == NULL)
		return 2;

	SDL_DestroySurface(surf);

	++state->textureCount;

	return 0;
}

bool isPosInRect(appState* state, float x, float y, SDL_FRect* rect)
{
	SDL_Rect viewport;
	SDL_GetRenderViewport(state->renderer, &viewport);
	if (
			x >= rect->x + viewport.x && 
			x <= rect->x + viewport.x + rect->w &&
			y >= rect->y + viewport.y &&
			y <= rect->y + viewport.y + rect->h
			)
		return true;
	return false;
}

SDL_AppResult getAllTextures(appState* state)
{
	if (getTexture(state, "buttons/play_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	play_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/play_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	play_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "board.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	board_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "x_piece.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	x_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "o_piece.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	o_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "half_x_piece.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	half_x_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "half_o_piece.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	half_o_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "dollar_piece.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	dollar_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "selector.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	selector_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "undo.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	undo_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "turn_text.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	turn_text_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "win_text.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	win_text_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/steal_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	steal_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/steal_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	steal_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/check_id_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	check_id_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/check_id_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	check_id_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/check_card_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	check_card_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/check_card_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	check_card_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/look_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	look_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/look_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	look_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/inventory_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	inventory_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/inventory_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	inventory_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/shop_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	shop_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/shop_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	shop_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/block_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	block_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/block_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	block_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/skip_btn.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	skip_btn_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "buttons/skip_btn_selected.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	skip_btn_selected_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "back.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	back_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "next.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	next_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "x_id.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	x_id_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "o_id.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	o_id_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "card.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	card_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "items/dollar_item.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	dollar_item_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "items/setter_item.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	setter_item_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "items/randomizer_item.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	randomizer_item_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "items/gun_item.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	gun_item_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "items/divider_item.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	divider_item_sprite = &state->textures[state->textureCount - 1];

	if (getTexture(state, "items/bag_item.png") != 0)
	{
		SDL_Log("Failed to load texture. %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	bag_item_sprite = &state->textures[state->textureCount - 1];

	return SDL_APP_CONTINUE;
}

int getFont(appState* state, char* name, float size)
{
	char* fullPath = NULL;
	SDL_asprintf(&fullPath, "%s../src/assets/%s", SDL_GetBasePath(), name);

	state->tInfo.font = TTF_OpenFont(fullPath, size);

	SDL_free(fullPath);

	return 0;
}

void getText(appState* state, sprite* dest, char* text, SDL_Color color)
{
	SDL_Surface* surf = TTF_RenderText_Blended(state->tInfo.font, text, 0, color);
	dest->w = surf->w;
	dest->h = surf->h;
	dest->texture = SDL_CreateTextureFromSurface(state->renderer, surf);
	SDL_DestroySurface(surf);
}
