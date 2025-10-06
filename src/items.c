#include "headers/items.h"
#include "headers/game.h"
#include "headers/gameUtil.h"
#include "headers/textures.h"

int useItem(appState* state)
{
	player* currentPlayer = (state->game.currentTurn == 'x') ? &state->game.xPlayer : &state->game.oPlayer;
	player* otherPlayer = (state->game.currentTurn == 'o') ? &state->game.xPlayer : &state->game.oPlayer;
	switch (state->game.selectedItem)
	{
		case ITEM_DOLLAR:
			if (state->game.dollarTime == -1 && currentPlayer->inventory[ITEM_DOLLAR] > 0)
			{
				state->game.showNext = false;
				SDL_memcpy(state->game.preItemBoard, state->game.board, 9 * sizeof(Piece));
				return 2;
			}
			return 1;
		case ITEM_SETTER:
			if (currentPlayer->inventory[ITEM_SETTER] > 0)
			{
				state->game.showNext = false;
				SDL_memcpy(state->game.preItemBoard, state->game.board, 9 * sizeof(Piece));
				return 2;
			}
			return 1;
		case ITEM_RANDOM:
			if (currentPlayer->inventory[ITEM_RANDOM] > 0)
			{
				state->game.showNext = false;
				return 2;
			}
			return 1;
		case ITEM_GUN:
			if (currentPlayer->inventory[ITEM_GUN] > 0)
			{
				otherPlayer->hasHalfDebuff = true;
				--currentPlayer->inventory[ITEM_GUN];
				return 3;
			}
			return 1;
		case ITEM_DIVIDER:
			if (currentPlayer->inventory[ITEM_DIVIDER] > 0)
			{
				SDL_memcpy(state->game.preItemBoard, state->game.board, 9 * sizeof(Piece));
				Piece full = (state->game.currentTurn == 'o') ? PIECE_X : PIECE_O;
				Piece half = (state->game.currentTurn == 'o') ? PIECE_HALF_X : PIECE_HALF_O;
				for (int i = 0; i < 9; i++)
				{
					if (state->game.board[i] == full)
						state->game.board[i] = half;
					else if (state->game.board[i] == half)
						state->game.board[i] = PIECE_NONE;
				}
				return 2;
			}
			return 1;
	}
	return 0;
}

void renderDollarItem(appState* state)
{
	state->game.nextPiece = PIECE_DOLLAR;
	renderBoard(state);
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_FRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;

	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);

	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
		{
			state->selectedZone = ZONE_BACK;
		}
	}

	rect.y = rect.h;

	if (state->game.showNext)
	{
		renderNextButton(state);
		SDL_RenderTexture(state->renderer, undo_sprite->texture, NULL, &rect);

		if (state->updateZone)
		{
			if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			{
				state->selectedZone = ZONE_UNDO;
			}
		}
	}

}

void renderSetterItem(appState* state)
{
	state->game.nextPiece = (state->game.currentTurn == 'x') ? PIECE_HALF_X : PIECE_HALF_O;
	renderBoard(state);
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_FRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;

	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);

	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
		{
			state->selectedZone = ZONE_BACK;
		}
	}

	rect.y = rect.h;

	if (state->game.showNext)
	{
		renderNextButton(state);
		SDL_RenderTexture(state->renderer, undo_sprite->texture, NULL, &rect);

		if (state->updateZone)
		{
			if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			{
				state->selectedZone = ZONE_UNDO;
			}
		}
	}

}

void renderRandomizerItem(appState* state)
{
	renderBoard(state);
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_FRect rect;

	if (state->game.showNext)
	{
		renderNextButton(state);
	}
	else
	{
		rect.x = 0;
		rect.y = 0;
		rect.w = state->wInfo.windowWidth / 10.0f;
		rect.h = rect.w;

		SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);

		if (state->updateZone)
		{
			if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			{
				state->selectedZone = ZONE_BACK;
			}
		}
	}

}

void renderGunItem(appState* state)
{

}

void renderDividerItem(appState* state)
{
	SDL_FRect rect;
	rect.w = state->wInfo.windowWidth / 3.0f;
	rect.h = rect.w;
	rect.x = state->wInfo.windowWidth / 9.0f;
	rect.y = state->wInfo.windowHeight / 2.0f - rect.h / 2.0f;

	renderStaticBoard(state, state->game.preItemBoard, &rect);

	rect.x *= 5;

	renderStaticBoard(state, state->game.board, &rect);

	SDL_SetRenderViewport(state->renderer, NULL);
	
	rect.w = 0 - (state->wInfo.windowWidth / 6.0f);
	rect.h = rect.w;
	rect.x = state->wInfo.windowWidth / 2.0f - rect.w / 2.0f;
	rect.y = state->wInfo.windowHeight / 2.0f - rect.h / 2.0f;
	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);


	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.x = state->wInfo.windowWidth - rect.w;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, next_sprite->texture, NULL, &rect);
	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
		{
			state->selectedZone = ZONE_NEXT;
		}
	}

	rect.x = 0;
	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);
	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
		{
			state->selectedZone = ZONE_BACK;
		}
	}

}

SDL_AppResult handleDollarItemEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->game.selectedTile != -1)
		{
			state->game.board[state->game.selectedTile] = PIECE_DOLLAR;
			state->game.showNext = true;
		}
		else if (state->selectedZone == ZONE_UNDO)
		{
			SDL_memcpy(state->game.board, state->game.preItemBoard, 9 * sizeof(Piece));
			state->game.showNext = false;
		}
		else if (state->selectedZone == ZONE_NEXT)
		{
			state->game.dollarTime = 3;
			if (state->game.currentTurn == 'x')
				--state->game.xPlayer.inventory[ITEM_DOLLAR];
			else
				--state->game.oPlayer.inventory[ITEM_DOLLAR];
			toNextPlayer(state);
		}
		else if (state->selectedZone == ZONE_BACK)
		{
			SDL_memcpy(state->game.board, state->game.preItemBoard, 9 * sizeof(Piece));
			state->game.showNext = false;
			state->scene = SCENE_ACTION;
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleSetterItemEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->game.selectedTile != -1)
		{
			state->game.board[state->game.selectedTile] = state->game.nextPiece;
			state->game.showNext = true;
		}
		else if (state->selectedZone == ZONE_UNDO)
		{
			SDL_memcpy(state->game.board, state->game.preItemBoard, 9 * sizeof(Piece));
			state->game.showNext = false;
		}
		else if (state->selectedZone == ZONE_NEXT)
		{
			if (state->game.currentTurn == 'x')
				--state->game.xPlayer.inventory[ITEM_SETTER];
			else
				--state->game.oPlayer.inventory[ITEM_SETTER];
			toNextPlayer(state);
		}
		else if (state->selectedZone == ZONE_BACK)
		{
			SDL_memcpy(state->game.board, state->game.preItemBoard, 9 * sizeof(Piece));
			state->game.showNext = false;
			state->scene = SCENE_ACTION;
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleRandomizerItemEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->game.selectedTile != -1)
		{
			state->game.board[state->game.selectedTile] = SDL_rand(5);
			state->game.showNext = true;
		}
		else if (state->selectedZone == ZONE_NEXT)
		{
			if (state->game.currentTurn == 'x')
				--state->game.xPlayer.inventory[ITEM_RANDOM];
			else
				--state->game.oPlayer.inventory[ITEM_RANDOM];
			toNextPlayer(state);
		}
		else if (state->selectedZone == ZONE_BACK)
		{
			state->scene = SCENE_ACTION;
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleGunItemEvent(appState* state, SDL_Event* event)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleDividerItemEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->selectedZone == ZONE_BACK)
		{
			SDL_memcpy(state->game.board, state->game.preItemBoard, 9 * sizeof(Piece));
			state->scene = SCENE_ACTION;
		}
		else if (state->selectedZone == ZONE_NEXT)
		{
			
			if (state->game.currentTurn == 'x')
				--state->game.xPlayer.inventory[ITEM_DIVIDER];
			else
				--state->game.oPlayer.inventory[ITEM_DIVIDER];
			toNextPlayer(state);
		}
	}
	return SDL_APP_CONTINUE;
}
