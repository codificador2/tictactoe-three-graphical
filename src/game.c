#include "headers/game.h"
#include "headers/gameUtil.h"
#include "headers/textures.h"
#include <SDL3/SDL_render.h>

SDL_AppResult commonEventLogic(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_Q && event->key.mod == (SDL_KMOD_LCTRL | SDL_KMOD_LSHIFT)))
		return SDL_APP_SUCCESS;

	if (event->type == SDL_EVENT_WINDOW_RESIZED)
	{
		SDL_GetWindowSizeInPixels(state->window, &state->wInfo.windowWidth, &state->wInfo.windowHeight);
		if (state->wInfo.windowWidth < MIN_WINDOW_WIDTH)
		{
			state->wInfo.windowWidth = MIN_WINDOW_WIDTH;
			SDL_SetWindowSize(state->window, state->wInfo.windowWidth, state->wInfo.windowHeight);
		}
		if (state->wInfo.windowHeight < MIN_WINDOW_HEIGHT)
		{
			state->wInfo.windowHeight = MIN_WINDOW_HEIGHT;
			SDL_SetWindowSize(state->window, state->wInfo.windowWidth, state->wInfo.windowHeight);
		}
	}

	if (event->type == SDL_EVENT_MOUSE_MOTION)
	{
		state->wInfo.mouseX = event->motion.x;
		state->wInfo.mouseY = event->motion.y;
		state->updateZone = true;
	}

	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		state->wInfo.lastMouseDownX = event->button.x;
		state->wInfo.lastMouseDownY = event->button.y;
	}

	return SDL_APP_CONTINUE;
}

void renderBoard(appState* state)
{
	SDL_Rect board_viewport;
	board_viewport.x = state->wInfo.windowWidth / 6;
	board_viewport.w = state->wInfo.windowWidth / 1.5;
	board_viewport.y = state->wInfo.windowHeight / 6;
	board_viewport.h = state->wInfo.windowHeight / 1.5;
	SDL_SetRenderViewport(state->renderer, &board_viewport);

	SDL_FRect rect;
	if (board_viewport.w > board_viewport.h) {
		rect.h = board_viewport.h;
		rect.w = rect.h;
	} else {
		rect.w = board_viewport.w;
		rect.h = rect.w;
	}
	rect.x = (board_viewport.w / 2.0) - (rect.w / 2.0);
	rect.y = (board_viewport.h / 2.0) - (rect.h / 2.0);

	SDL_RenderTexture(state->renderer, board_sprite->texture, NULL, &rect);

	rect.w /= 3;
	rect.h /= 3;

	sprite* piece;

	state->game.selectedTile = -1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			switch (state->game.board[j + (3 * i)])
			{
				case PIECE_NONE:
					break;
				case PIECE_X:
					piece = x_sprite;
					break;
				case PIECE_O:
					piece = o_sprite;
					break;
				case PIECE_HALF_X:
					piece = half_x_sprite;
					break;
				case PIECE_HALF_O:
					piece = half_o_sprite;
					break;
				case PIECE_DOLLAR:
					piece = dollar_sprite;
					break;
			}
			if (!isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			{
				if (state->game.board[j + (3 * i)] != PIECE_NONE)
					SDL_RenderTexture(state->renderer, piece->texture, NULL, &rect);
			}
			else
			{
				if (
						(
							!state->game.showNext &&
							state->game.blockPosition != j + (3 * i) &&
							(state->scene == SCENE_GAME_BOARD) &&
							canPieceChange(state->game.board[j + (3 * i)], state->game.nextPiece, state->game.mustHalfMove || ((state->game.currentTurn == 'x') ? state->game.xPlayer.hasHalfDebuff : state->game.oPlayer.hasHalfDebuff)) &&
							!(
								j + (3 * i) == state->game.lastHalfMove &&
								state->game.oldBoard[j + (3 * i)] == state->game.nextPiece
								)
						) ||
						(
						 state->scene == SCENE_ITEM &&
						 (state->game.selectedItem == ITEM_DOLLAR || state->game.selectedItem == ITEM_SETTER || state->game.selectedItem == ITEM_RANDOM) &&
						 !state->game.showNext
						) ||
						(
						 state->scene == SCENE_ACTION &&
						 state->game.action == ACTION_BLOCK
						)
				   )
				{
					if (state->scene == SCENE_ITEM)
					{
						if (state->game.selectedItem == ITEM_RANDOM)
							piece = random_piece_sprite;
						else if (state->game.selectedItem == ITEM_DOLLAR)
							piece = dollar_sprite;
						else if (state->game.selectedItem == ITEM_SETTER)
							piece = (state->game.currentTurn == 'x') ? half_x_sprite : half_o_sprite;
					}
					else if (state->scene == SCENE_ACTION)
					{
						if (state->game.board[j + (3 * i)] != PIECE_NONE)
							SDL_RenderTexture(state->renderer, piece->texture, NULL, &rect);
						// SDL_SetRenderDrawColorFloat(state->renderer, 1.0f, 1.0f, 0.0f, 0.5f);
						// SDL_RenderFillRect(state->renderer, &rect);
						SDL_SetRenderDrawColorFloat(state->renderer, state->backgroundColor.r, state->backgroundColor.g, state->backgroundColor.b, 1.0f);
						SDL_RenderRect(state->renderer, &rect);
					}
					else
					{
						switch (state->game.nextPiece)
						{
							case PIECE_X:
								piece = x_sprite;
								break;
							case PIECE_O:
								piece = o_sprite;
								break;
							case PIECE_HALF_X:
								piece = half_x_sprite;
								break;
							case PIECE_HALF_O:
								piece = half_o_sprite;
								break;
							case PIECE_DOLLAR:
								piece = dollar_sprite;
								break;
							case PIECE_NONE:
								break;
						}
					}
					if (state->scene != SCENE_ACTION)
					{
						SDL_SetTextureAlphaModFloat(piece->texture, 0.5f);
						SDL_RenderTexture(state->renderer, piece->texture, NULL, &rect);
						SDL_SetTextureAlphaModFloat(piece->texture, 1.0f);
					}
					state->game.selectedTile = j + (3 * i);
				}
				else
				{
					if (state->game.board[j + (3 * i)] != PIECE_NONE)
						SDL_RenderTexture(state->renderer, piece->texture, NULL, &rect);
				}
			}
			if (state->game.blockPosition == j + (3 * i))
			{
				SDL_SetRenderDrawColorFloat(state->renderer, state->backgroundColor.r, state->backgroundColor.g, state->backgroundColor.b, 1.0f);
				SDL_RenderRect(state->renderer, &rect);
				// SDL_SetRenderDrawColorFloat(state->renderer, 1.0f, 1.0f, 0.0f, 0.5f);
				// SDL_RenderFillRect(state->renderer, &rect);
			}
			rect.x += rect.w;
		}
		rect.x -= 3 * rect.w;
		rect.y += rect.h;
	}
}

static void renderSelectorGradients(appState* state, SDL_Rect* viewport)
{
	SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);
	SDL_Vertex vertices[4];
	const int indices[12] = {
		0, 1, 2,
		1, 2, 3,
	};
	SDL_zeroa(vertices);
	vertices[0].color.a = 1.0f;
	vertices[0].position.x = 0;
	vertices[0].position.y = 0;
	vertices[0].color.r = state->backgroundColor.r;
	vertices[0].color.g = state->backgroundColor.g;
	vertices[0].color.b = state->backgroundColor.b;

	vertices[1].color.a = 1.0f;
	vertices[1].position.x = viewport->w;
	vertices[1].position.y = 0;
	vertices[1].color.r = state->backgroundColor.r;
	vertices[1].color.g = state->backgroundColor.g;
	vertices[1].color.b = state->backgroundColor.b;

	vertices[2].position.x = 0;
	vertices[2].position.y = viewport->h / 5.0f;
	vertices[2].color.r = state->backgroundColor.r;
	vertices[2].color.g = state->backgroundColor.g;
	vertices[2].color.b = state->backgroundColor.b;

	vertices[3].position.x = viewport->w;
	vertices[3].position.y = viewport->h / 5.0f;
	vertices[3].color.r = state->backgroundColor.r;
	vertices[3].color.g = state->backgroundColor.g;
	vertices[3].color.b = state->backgroundColor.b;
	SDL_RenderGeometry(state->renderer, NULL, vertices, 4, indices, 6);
	vertices[0].color.a = 0.0f;
	vertices[0].position.y = viewport->h - viewport->h / 5.0f;

	vertices[1].color.a = 0.0f;
	vertices[1].position.y = viewport->h - viewport->h / 5.0f;

	vertices[2].color.a = 1.0f;
	vertices[2].position.y = viewport->h;

	vertices[3].color.a = 1.0f;
	vertices[3].position.y = viewport->h;
	SDL_RenderGeometry(state->renderer, NULL, vertices, 4, indices, 6);
}

void renderSelector(appState* state)
{
	SDL_Rect selector_viewport;
	selector_viewport.w = state->wInfo.windowWidth / 9.0f;
	selector_viewport.h = state->wInfo.windowHeight;
	selector_viewport.x = state->wInfo.windowWidth - selector_viewport.w - state->wInfo.windowWidth / 12.0f;
	selector_viewport.y = 0;

	SDL_SetRenderViewport(state->renderer, &selector_viewport);

	SDL_FRect rect;
	rect.w = selector_viewport.w;
	rect.h = rect.w * ((float)selector_sprite->h / (float)selector_sprite->w);
	rect.x = 0;
	rect.y = (selector_viewport.h / 2.0f - rect.w / 2.0f) - state->game.selectedInSelector * rect.w - rect.h;
	SDL_RenderTexture(state->renderer, selector_sprite->texture, NULL, &rect);
	rect.y += rect.h;
	SDL_RenderTexture(state->renderer, selector_sprite->texture, NULL, &rect);
	rect.y += rect.h;
	SDL_RenderTexture(state->renderer, selector_sprite->texture, NULL, &rect);
	rect.y -= rect.h;

	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
		{
			state->selectedZone = ZONE_SELECTOR;
			state->updateZone = false;
		}
	}

	rect.h = rect.w;
	rect.y = selector_viewport.h / 2.0f - rect.h / 2.0f;
	SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
	SDL_RenderRect(state->renderer, &rect);

	renderSelectorGradients(state, &selector_viewport);
}

static void renderActionButton(appState* state, sprite* normalTexture, sprite* selectedTexture, int index, SDL_FRect* rect)
{
	if (state->updateZone && isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, rect))
	{
		state->selectedZone = ZONE_ACTION_SELECTOR;
		state->updateZone = false;
		state->game.selectedAction = index;
	}
	SDL_RenderTexture(state->renderer, (state->game.selectedAction == index) ? selectedTexture->texture : normalTexture->texture, NULL, rect);
	rect->y += rect->h + 10;
}

void renderActions(appState* state)
{
	SDL_Rect actions_viewport;
	actions_viewport.w = state->wInfo.windowWidth / 5.0f - 10;
	actions_viewport.h = state->wInfo.windowHeight - 20;
	actions_viewport.x = state->wInfo.windowWidth - actions_viewport.w - 10;
	actions_viewport.y = 10;
	SDL_SetRenderViewport(state->renderer, &actions_viewport);

	SDL_FRect rect;
	rect.h = (actions_viewport.h) / 8.0f - 10;
	rect.w = rect.h * 2;
	rect.x = actions_viewport.w - rect.w;
	rect.y = 0;

	if (state->updateZone)
		state->game.selectedAction = -1;
	renderActionButton(state, steal_btn_sprite, steal_btn_selected_sprite, 0, &rect);
	renderActionButton(state, check_id_btn_sprite, check_id_btn_selected_sprite, 1, &rect);
	renderActionButton(state, check_card_btn_sprite, check_card_btn_selected_sprite, 2, &rect);
	renderActionButton(state, look_btn_sprite, look_btn_selected_sprite, 3, &rect);
	renderActionButton(state, inventory_btn_sprite, inventory_btn_selected_sprite, 4, &rect);
	renderActionButton(state, shop_btn_sprite, shop_btn_selected_sprite, 5, &rect);
	renderActionButton(state, block_btn_sprite, block_btn_selected_sprite, 6, &rect);
	renderActionButton(state, skip_btn_sprite, skip_btn_selected_sprite, 7, &rect);

}
