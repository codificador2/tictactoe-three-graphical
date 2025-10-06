#include "headers/scenes.h"

#include "headers/actions.h"
#include "headers/game.h"
#include "headers/gameUtil.h"
#include "headers/items.h"
#include "headers/textures.h"

void renderMainMenuScene(appState* state)
{
	SDL_FRect rect;

	SDL_Rect mainMenu_viewport;
	mainMenu_viewport.x = state->wInfo.windowWidth / 4;
	mainMenu_viewport.w = state->wInfo.windowWidth / 2;
	mainMenu_viewport.y = state->wInfo.windowHeight / 6;
	mainMenu_viewport.h = state->wInfo.windowHeight / 1.5;
	SDL_SetRenderViewport(state->renderer, &mainMenu_viewport);

	// SDL_FRect view = { .x=0, .y=0, .w=mainMenu_viewport.w, .h=mainMenu_viewport.h };
	// SDL_SetRenderDrawColor(state->renderer, 0, 0, 255, 1);
	// SDL_RenderFillRect(state->renderer, &view);

	rect.w = mainMenu_viewport.w / 3.0;
	rect.h = rect.w / 2.0;
	rect.x = (mainMenu_viewport.w / 2.0) - (rect.w / 2.0);
	rect.y = (mainMenu_viewport.h / 2.0) - (rect.h / 2.0);
	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			state->selectedZone = ZONE_PLAY_BTN;
		state->updateZone = false;
	}
	SDL_RenderTexture(
			state->renderer,
			(state->selectedZone == ZONE_PLAY_BTN) ? play_btn_selected_sprite->texture : play_btn_sprite->texture,
			NULL,
			&rect
			);

	SDL_RenderPresent(state->renderer);
}

void renderGameBoardScene(appState* state)
{
	renderBoard(state);
	renderSelector(state);

	SDL_SetRenderViewport(state->renderer, NULL);

	SDL_FRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = state->wInfo.windowWidth / 6.0f;
	rect.h = rect.w / 2.0f;
	SDL_RenderTexture(state->renderer, turn_text_sprite->texture, NULL, &rect);

	rect.x = rect.w + 10.0f;
	rect.w = rect.h;
	SDL_RenderTexture(state->renderer, (state->game.currentTurn == 'x') ? x_sprite->texture : o_sprite->texture, NULL, &rect);

	if (state->game.moveMade)
	{
		rect.x = 0;
		rect.y = state->wInfo.windowWidth / 12.0f;
		rect.w = state->wInfo.windowWidth / 10.0f;
		rect.h = rect.w;
		SDL_RenderTexture(state->renderer, undo_sprite->texture, NULL, &rect);
		
		if (state->updateZone)
		{
			if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			{
				state->selectedZone = ZONE_UNDO;
			}
		}
	}
	if (state->game.showNext)
		renderNextButton(state);

	player* currentPlayer = (state->game.currentTurn == 'x') ? &state->game.xPlayer : &state->game.oPlayer;

	if (state->game.dollarTime >= 0)
	{
		float textRatio = (float)state->tInfo.dollarText.w / (float)state->tInfo.dollarText.h;
		rect.h = state->wInfo.windowWidth / 20.0f;
		rect.w = rect.h * textRatio;
		const int maxWidth = (state->wInfo.windowWidth * 0.9f) - (state->wInfo.windowWidth / 12.0f) - (state->wInfo.windowWidth / 9.0f) - 15;
		if (rect.w > maxWidth)
		{
			rect.w = maxWidth;
			rect.h = rect.w / textRatio;
		}
		rect.x = state->wInfo.windowWidth / 10.0f;
		rect.y = state->wInfo.windowHeight * 0.95 - rect.h / 2.0f;
		SDL_RenderTexture(state->renderer, state->tInfo.dollarText.texture, NULL, &rect);
	}

	if (currentPlayer->alertId || currentPlayer->alertCreditCard || currentPlayer->hasHalfDebuff)
	{
		rect.x = state->wInfo.windowWidth / 10.0f;
		float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
		rect.h = state->wInfo.windowWidth / 20.0f;
		rect.w = rect.h * textRatio;
		const int maxWidth = (state->wInfo.windowWidth * 0.9f) - (state->wInfo.windowWidth / 12.0f) - (state->wInfo.windowWidth / 9.0f) - 15;
		if (rect.w > maxWidth)
		{
			rect.w = maxWidth;
			rect.h = rect.w / textRatio;
		}
		rect.y = state->wInfo.windowHeight * 0.95 - rect.h / 2.0f;
		if (state->game.dollarTime >= 0)
			rect.y -= rect.h + 15;
		SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);
	}


	SDL_RenderPresent(state->renderer);
}

void renderGameActionScene(appState* state)
{
	renderBoard(state);
	renderActions(state);
	SDL_SetRenderViewport(state->renderer, NULL);

	SDL_FRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = state->wInfo.windowWidth / 6.0f;
	rect.h = rect.w / 2.0f;
	SDL_RenderTexture(state->renderer, turn_text_sprite->texture, NULL, &rect);

	rect.x = rect.w + 10.0f;
	rect.w = rect.h;
	SDL_RenderTexture(state->renderer, (state->game.currentTurn == 'x') ? x_sprite->texture : o_sprite->texture, NULL, &rect);

	rect.x = 0;
	rect.y = state->wInfo.windowWidth / 12.0f;
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

	SDL_RenderPresent(state->renderer);
}

void renderActionScene(appState* state)
{
	switch (state->game.action)
	{
		case ACTION_STEAL:
			renderStealAction(state);
			break;
		case ACTION_CHECK_ID:
			renderCheckIdAction(state);
			break;
		case ACTION_CHECK_CARD:
			renderCheckCardAction(state);
			break;
		case ACTION_LOOK:
			renderLookAction(state);
			break;
		case ACTION_INVENTORY:
			renderInventoryAction(state);
			break;
	}

	SDL_RenderPresent(state->renderer);
}

void renderItemScene(appState* state)
{
	switch (state->game.selectedItem)
	{
		case ITEM_DOLLAR:
			renderDollarItem(state);
			break;
		case ITEM_SETTER:
			renderSetterItem(state);
			break;
		case ITEM_RANDOM:
			renderRandomizerItem(state);
			break;
		case ITEM_GUN:
			renderGunItem(state);
			break;
	}
	SDL_RenderPresent(state->renderer);
}

SDL_AppResult handleMainMenuEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT && state->selectedZone == ZONE_PLAY_BTN) {
		setupAlerts(state);
		state->scene = SCENE_GAME_BOARD;
		state->backgroundColor.g = 0.01f;
		state->backgroundColor.r = 0.08f * (state->game.currentTurn == 'o');
		state->backgroundColor.b = 0.08f * (state->game.currentTurn == 'x');
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult handleGameBoardEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE && event->key.mod == 0)
		state->scene = SCENE_MAIN_MENU;
	
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->game.selectedTile != -1 && !state->game.showNext && !(state->game.selectedTile == state->game.lastHalfMove && state->game.oldBoard[state->game.selectedTile] == state->game.nextPiece))
		{
			state->game.moveMade = true;
			const bool wasHalfMove = isHalfMove(state->game.board[state->game.selectedTile], state->game.nextPiece);
			state->game.board[state->game.selectedTile] = state->game.nextPiece;
			if (wasHalfMove)
			{
				if (state->game.mustHalfMove || ((state->game.currentTurn == 'x') ? state->game.xPlayer.hasHalfDebuff : state->game.oPlayer.hasHalfDebuff))
				{
					// state->game.currentTurn = (state->game.currentTurn == 'x') ? 'o' : 'x';
					state->game.showNext = true;
				}
				else
				{
					state->game.mustHalfMove = true;
					state->game.lastHalfMove = state->game.selectedTile;
				}
			}
			else
			{
				// state->game.currentTurn = (state->game.currentTurn == 'x') ? 'o' : 'x';
				state->game.showNext = true;
			}
		}
		else if (event->button.x > state->wInfo.windowWidth * (8.0f/9.0f) - state->wInfo.windowWidth / 12.0f && event->button.x < state->wInfo.windowWidth * (11.0f/12.0f))
		{
			const int oneTileSize = state->wInfo.windowWidth / 9;
			const int selectedStartY = state->wInfo.windowHeight / 2 - oneTileSize / 2;
			const int startY = selectedStartY - state->game.selectedInSelector * oneTileSize;
			const int mouseY = (event->button.y > startY) ? event->button.y : event->button.y + oneTileSize * 8;
			state->game.selectedInSelector = (int)(((mouseY - startY) - ((mouseY - startY) % oneTileSize)) / oneTileSize) % 8;
			switch (state->game.selectedInSelector)
			{
				case 0:
				case 4:
					state->game.nextPiece = PIECE_NONE;
					break;
				case 1:
				case 3:
					state->game.nextPiece = PIECE_HALF_O;
					break;
				case 2:
					state->game.nextPiece = PIECE_O;
					break;
				case 5:
				case 7:
					state->game.nextPiece = PIECE_HALF_X;
					break;
				case 6:
					state->game.nextPiece = PIECE_X;
					break;
			}
		}
		else if (state->selectedZone == ZONE_UNDO)
		{
			SDL_memcpy(state->game.board, state->game.oldBoard, 9 * sizeof(Piece));
			state->game.moveMade = false;
			state->game.mustHalfMove = false;
			state->game.showNext = false;
			state->game.lastHalfMove = -1;
		}
		else if (state->selectedZone == ZONE_NEXT)
		{
			state->scene = SCENE_GAME_ACTION;
		}
	}

	if (event->type == SDL_EVENT_MOUSE_WHEEL)
	{
		if (event->wheel.y > 0)
		{
			if (state->game.selectedInSelector == 0)
				state->game.selectedInSelector = 7;
			else
				--state->game.selectedInSelector;
		}
		else if (event->wheel.y < 0)
		{
			if (state->game.selectedInSelector == 7)
				state->game.selectedInSelector = 0;
			else
				++state->game.selectedInSelector;
		}
		switch (state->game.selectedInSelector)
		{
			case 0:
			case 4:
				state->game.nextPiece = PIECE_NONE;
				break;
			case 1:
			case 3:
				state->game.nextPiece = PIECE_HALF_O;
				break;
			case 2:
				state->game.nextPiece = PIECE_O;
				break;
			case 5:
			case 7:
				state->game.nextPiece = PIECE_HALF_X;
				break;
			case 6:
				state->game.nextPiece = PIECE_X;
				break;
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult handleGameActionEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->selectedZone == ZONE_BACK)
		{
			setupAlerts(state);
			state->scene = SCENE_GAME_BOARD;
		}
		else if (state->game.selectedAction != -1)
		{
			state->game.action = state->game.selectedAction;
			int res = performAction(state);
			if (res != 1)
			{
				state->scene = SCENE_ACTION;
				state->game.actionSuccess = (res == 2);
			}
		}
		// else
		// {
		// 	state->game.moveMade = false;
		// 	state->mustHalfMove = false;
		// 	SDL_memcpy(state->game.oldBoard, state->game.board, 9 * sizeof(Piece));
		// 	state->game.selectedInSelector = 0;
		// 	state->game.nextPiece = PIECE_NONE;
		// }
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleActionEvent(appState* state, SDL_Event* event)
{
	switch (state->game.action)
	{
		case ACTION_STEAL:
			return handleStealActionEvent(state, event);
		case ACTION_CHECK_ID:
			return handleCheckIdActionEvent(state, event);
		case ACTION_CHECK_CARD:
			return handleCheckCardActionEvent(state, event);
		case ACTION_LOOK:
			return handleLookActionEvent(state, event);
		case ACTION_INVENTORY:
			handleInventoryActionEvent(state, event);
		default:
			return SDL_APP_CONTINUE;
	}
}

SDL_AppResult handleItemEvent(appState* state, SDL_Event* event)
{
	switch (state->game.selectedItem)
	{
		case ITEM_DOLLAR:
			return handleDollarItemEvent(state, event);
		case ITEM_SETTER:
			return handleSetterItemEvent(state, event);
		case ITEM_RANDOM:
			return handleRandomizerItemEvent(state, event);
		case ITEM_GUN:
			return handleGunItemEvent(state, event);
		default:
			return SDL_APP_CONTINUE;
	}
}
