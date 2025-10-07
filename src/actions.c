#include "headers/actions.h"
#include "headers/game.h"
#include "headers/gameUtil.h"
#include "headers/items.h"
#include "headers/textures.h"

int performAction(appState* state)
{
	player* currentPlayer = (state->game.currentTurn == 'x') ? &state->game.xPlayer : &state->game.oPlayer;
	player* otherPlayer = (state->game.currentTurn == 'o') ? &state->game.xPlayer : &state->game.oPlayer;
	switch (state->game.action)
	{
		case ACTION_STEAL:
			if (currentPlayer->idCooldown > 0)
				return 1;

			if (SDL_rand(6) == 0) {
				getText(state, &state->tInfo.text, "You successfully stole the other player's ID", (SDL_Color){255,255,0,255});
				currentPlayer->hasOthersID = true;
				otherPlayer->hasOwnID = false;
				return 2;
			}
			getText(state, &state->tInfo.text, "The other player caught you trying to steal their ID", (SDL_Color){255,255,255,255});
			currentPlayer->idCooldown = 2;
			otherPlayer->alertId = true;
			return 0;
		case ACTION_CHECK_ID:
			if (currentPlayer->hasOwnID) {
				getText(state, &state->tInfo.text, "You still have your ID", (SDL_Color){255,255,255,255});
				return 0;
			}
			getText(state, &state->tInfo.text, "You were able to retrive your stolen ID", (SDL_Color){255,255,0,255});
			currentPlayer->hasOwnID = true;
			otherPlayer->hasOthersID = false;
			return 2;
		case ACTION_CHECK_CARD:
			randomizeCard(currentPlayer);
			getText(state, &state->tInfo.text, currentPlayer->creditCardNumber, (SDL_Color){255,255,255,255});
			return 2;
		case ACTION_LOOK:
			getText(state, &state->tInfo.text, "Opponents credit card:", (SDL_Color){255,255,255,255});
			char filler[4] = "____";
			int digit = SDL_rand(4);
			filler[digit] = otherPlayer->creditCardNumber[digit];
			getText(state, &state->tInfo.secondaryText, filler, (SDL_Color){255,255,255,255});
			return 2;
		case ACTION_INVENTORY:
			getText(state, &state->tInfo.text, "Inventory", (SDL_Color){255,255,255,255});
			return 2;
		case ACTION_SHOP:
			getText(state, &state->tInfo.text, "Enter credit card number:", (SDL_Color){255,255,255,255});
			SDL_strlcpy(state->game.typedcardNumber, "____", 5);
			getText(state, &state->tInfo.creditCardInputText, state->game.typedcardNumber, (SDL_Color){255,255,255,255});
			char* str;
			SDL_asprintf(&str, "Tries remaining: %d", 3 - state->game.creditCardTries);
			getText(state, &state->tInfo.secondaryText, str, (SDL_Color){255,255,255,255});
			SDL_free(str);
			return 2;
		case ACTION_BLOCK:
			return 2;
		case ACTION_SKIP:
			return 2;


	}
	return 0;
}

void renderStealAction(appState* state)
{
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_FRect rect;
	if (state->wInfo.windowWidth > state->wInfo.windowHeight) {
		rect.h = state->wInfo.windowHeight / 2.0f;
		rect.w = rect.h * (4.0f/3.0f);
	} else {
		rect.w = state->wInfo.windowWidth / 2.0f;
		rect.h = rect.w * 0.75f;
	}
	rect.x = (state->wInfo.windowWidth / 2.0) - (rect.w / 2.0);
	rect.y = (state->wInfo.windowHeight / 2.0) - (rect.h / 2.0);

	if (state->game.actionSuccess)
	{
		SDL_RenderTexture(
				state->renderer,
				(state->game.currentTurn == 'x') ? o_id_sprite->texture : x_id_sprite->texture,
				NULL,
				&rect
				);

		float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
		rect.h = state->wInfo.windowHeight * 0.125f;
		rect.w = rect.h * textRatio;
		rect.x = state->wInfo.windowWidth / 2.0f - rect.w / 2.0f;

		if (rect.w > state->wInfo.windowWidth * 0.8f) {
			rect.w = state->wInfo.windowWidth * 0.8f;
			rect.h = rect.w / textRatio;
			rect.x = state->wInfo.windowWidth / 10.0f;
		}
		rect.y -= rect.h + 10;
		SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);
	}
	else
	{
		float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
		rect.h = state->wInfo.windowHeight * 0.125f;
		rect.w = rect.h * textRatio;
		rect.x = state->wInfo.windowWidth / 2.0f - rect.w / 2.0f;

		if (rect.w > state->wInfo.windowWidth * 0.8f) {
			rect.w = state->wInfo.windowWidth * 0.8f;
			rect.h = rect.w / textRatio;
			rect.x = state->wInfo.windowWidth / 10.0f;
		}
		rect.y = state->wInfo.windowHeight / 2.0f - rect.h / 2.0f;
		SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);
	}
	renderNextButton(state);
}

void renderCheckIdAction(appState* state)
{
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_FRect rect;

	float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
	rect.h = state->wInfo.windowHeight * 0.125f;
	rect.w = rect.h * textRatio;
	rect.x = state->wInfo.windowWidth / 2.0f - rect.w / 2.0f;

	if (rect.w > state->wInfo.windowWidth * 0.8f) {
		rect.w = state->wInfo.windowWidth * 0.8f;
		rect.h = rect.w / textRatio;
		rect.x = state->wInfo.windowWidth / 10.0f;
	}
	rect.y = (state->wInfo.windowHeight / 2.0) - (rect.h / 2.0);
	SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);

	renderNextButton(state);
}

void renderCheckCardAction(appState* state)
{
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_FRect rect;
	if (state->wInfo.windowWidth > state->wInfo.windowHeight) {
		rect.h = state->wInfo.windowHeight / 2.0f;
		rect.w = rect.h * (4.0f/3.0f);
	} else {
		rect.w = state->wInfo.windowWidth / 2.0f;
		rect.h = rect.w * 0.75f;
	}
	rect.x = (state->wInfo.windowWidth / 2.0) - (rect.w / 2.0);
	rect.y = (state->wInfo.windowHeight / 2.0) - (rect.h / 2.0);

	SDL_RenderTexture(state->renderer, card_sprite->texture, NULL, &rect);

	float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
	rect.x += rect.w * 0.55f;
	rect.y += rect.h * 0.4f;
	rect.w /= 6.0f;
	rect.h = rect.w / textRatio;
	SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);
	renderNextButton(state);
}

void renderLookAction(appState* state)
{
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_Rect viewport;
	if (state->wInfo.windowWidth > state->wInfo.windowHeight) {
		viewport.h = state->wInfo.windowHeight / 2.0f;
		viewport.w = viewport.h * (4.0f/3.0f);
	} else {
		viewport.w = state->wInfo.windowWidth / 2.0f;
		viewport.h = viewport.w * 0.75f;
	}
	viewport.x = (state->wInfo.windowWidth / 2.0) - (viewport.w / 2.0);
	viewport.y = (state->wInfo.windowHeight / 2.0) - (viewport.h / 2.0);

	SDL_SetRenderViewport(state->renderer, &viewport);

	SDL_FRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = viewport.w;
	rect.h = viewport.h;

	SDL_RenderTexture(state->renderer, card_sprite->texture, NULL, &rect);

	float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
	rect.h = state->wInfo.windowHeight * 0.125f;
	rect.w = rect.h * textRatio;
	rect.x = state->wInfo.windowWidth / 2.0f - rect.w / 2.0f;

	if (rect.w > state->wInfo.windowWidth * 0.8f) {
		rect.w = state->wInfo.windowWidth * 0.8f;
		rect.h = rect.w / textRatio;
		rect.x = state->wInfo.windowWidth / 10.0f;
	}
	rect.y -= rect.h + 10;
	SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);

	float text2Ratio = (float)state->tInfo.secondaryText.w / (float)state->tInfo.secondaryText.h;
	rect.x = viewport.w * 0.55f;
	rect.y = viewport.h * 0.4f;
	rect.w = viewport.w / 6.0f;
	rect.h = rect.w / text2Ratio;
	// SDL_Log("x: %f, y: %f", rect.x, rect.y);
	// SDL_RenderFillRect(state->renderer, &rect);
	SDL_RenderTexture(state->renderer, state->tInfo.secondaryText.texture, NULL, &rect);

	SDL_SetRenderViewport(state->renderer, NULL);
	renderNextButton(state);
}

void renderInventoryAction(appState* state)
{
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_Rect viewport;
	viewport.h = state->wInfo.windowHeight * 0.6;
	viewport.w = state->wInfo.windowWidth * 0.8;
	viewport.x = (state->wInfo.windowWidth / 2.0) - (viewport.w / 2.0);
	viewport.y = (state->wInfo.windowHeight / 2.0) - (viewport.h / 2.0);

	SDL_SetRenderViewport(state->renderer, &viewport);

	SDL_FRect rect;

	float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
	rect.h = state->wInfo.windowHeight * 0.125f;
	rect.w = rect.h * textRatio;

	if (rect.w > viewport.w) {
		rect.w = viewport.w;
		rect.h = rect.w / textRatio;
	}
	rect.x = viewport.w / 2.0f - rect.w / 2.0f;
	rect.y = 0;
	SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);

	rect.x = 0;
	rect.w = viewport.w / 6.0f - 10;
	rect.h = rect.w;
	rect.y = rect.h + 20;
	renderSingleItem(state, dollar_item_sprite, ITEM_DOLLAR, &rect, true);
	renderSingleItem(state, setter_item_sprite, ITEM_SETTER, &rect, true);
	renderSingleItem(state, randomizer_item_sprite, ITEM_RANDOM, &rect, true);
	renderSingleItem(state, gun_item_sprite, ITEM_GUN, &rect, true);
	renderSingleItem(state, divider_item_sprite, ITEM_DIVIDER, &rect, true);
	renderSingleItem(state, bag_item_sprite, ITEM_BAG, &rect, true);

	SDL_SetRenderViewport(state->renderer, NULL);

	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.x = 0;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);

	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			state->selectedZone = ZONE_BACK;
	}
}

void renderShopAction(appState* state)
{
	SDL_SetRenderViewport(state->renderer, NULL);
	SDL_Rect viewport;
	if (state->wInfo.windowWidth > state->wInfo.windowHeight) {
		viewport.h = state->wInfo.windowHeight / 2.0f;
		viewport.w = viewport.h * (4.0f/3.0f);
	} else {
		viewport.w = state->wInfo.windowWidth / 2.0f;
		viewport.h = viewport.w * 0.75f;
	}
	viewport.x = (state->wInfo.windowWidth / 2.0) - (viewport.w / 2.0);
	viewport.y = (state->wInfo.windowHeight / 2.0) - (viewport.h / 2.0);

	SDL_SetRenderViewport(state->renderer, &viewport);

	SDL_FRect rect;
	rect.w = viewport.w / 2.0f;
	rect.h = rect.w / ((float)state->tInfo.creditCardInputText.w/(float)state->tInfo.creditCardInputText.h);
	rect.y = viewport.h / 2.0f - rect.h / 2.0f;
	rect.x = viewport.w / 2.0f - rect.w / 2.0f;

	SDL_RenderTexture(state->renderer, state->tInfo.creditCardInputText.texture, NULL, &rect);

	float textRatio = (float)state->tInfo.text.w / (float)state->tInfo.text.h;
	rect.h = state->wInfo.windowHeight * 0.125f;
	rect.w = rect.h * textRatio;
	rect.x = state->wInfo.windowWidth / 2.0f - rect.w / 2.0f;

	if (rect.w > viewport.w * 0.8f) {
		rect.w = viewport.w * 0.8f;
		rect.h = rect.w / textRatio;
		rect.x = viewport.w / 10.0f;
	}
	rect.y -= rect.h + 10;
	SDL_RenderTexture(state->renderer, state->tInfo.text.texture, NULL, &rect);

	SDL_SetRenderViewport(state->renderer, NULL);

	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.x = 0;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);
	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			state->selectedZone = ZONE_BACK;
	}

	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.x = state->wInfo.windowWidth * 0.9f;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, next_sprite->texture, NULL, &rect);
	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			state->selectedZone = ZONE_NEXT;
	}

	rect.w = state->wInfo.windowWidth / 2.0f;
	rect.h = rect.w / ((float)state->tInfo.secondaryText.w/(float)state->tInfo.secondaryText.h);
	rect.x = state->wInfo.windowWidth / 4.0f;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, state->tInfo.secondaryText.texture, NULL, &rect);
}

void renderBlockAction(appState* state)
{
	SDL_FRect rect;
	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.x = 0;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, back_sprite->texture, NULL, &rect);
	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			state->selectedZone = ZONE_BACK;
	}
	renderBoard(state);
}

static SDL_AppResult handleSimpleActionEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->selectedZone == ZONE_NEXT)
			toNextPlayer(state);
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleStealActionEvent(appState* state, SDL_Event* event) { return handleSimpleActionEvent(state, event); }

SDL_AppResult handleCheckIdActionEvent(appState* state, SDL_Event* event) { return handleSimpleActionEvent(state, event); }

SDL_AppResult handleCheckCardActionEvent(appState* state, SDL_Event* event) { return handleSimpleActionEvent(state, event); }

SDL_AppResult handleLookActionEvent(appState* state, SDL_Event* event) { return handleSimpleActionEvent(state, event); }

SDL_AppResult handleInventoryActionEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->selectedZone == ZONE_BACK)
			state->scene = SCENE_GAME_ACTION;
		if (state->selectedZone == ZONE_INVENTORY)
		{
			state->game.selectedItem = state->game.selectedInInventory;
			int res = useItem(state);
			if (res == 3)
			{
				toNextPlayer(state);
				state->scene = SCENE_GAME_BOARD;
			}
			else if (res != 1)
				state->scene = SCENE_ITEM;
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleShopActionEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->selectedZone == ZONE_BACK)
			state->scene = SCENE_GAME_ACTION;
		if (state->selectedZone == ZONE_NEXT && state->game.currentCardDigit == 4)
		{
			if (SDL_strncmp(state->game.typedcardNumber, state->game.xPlayer.creditCardNumber, 4) == 0)
			{
				state->game.creditCardUsed = &state->game.xPlayer;
				state->scene = SCENE_SHOP;
				state->game.creditCardTries = 0;
				getText(state, &state->tInfo.text, "Shop:", (SDL_Color){255,255,255,255});

				char* str;
				SDL_asprintf(&str, "$%d", state->game.creditCardUsed->money);
				getText(state, &state->tInfo.secondaryText, str, (SDL_Color){0,255,0,255});
				SDL_free(str);
			}
			else if (SDL_strncmp(state->game.typedcardNumber, state->game.oPlayer.creditCardNumber, 4) == 0)
			{
				state->game.creditCardUsed = &state->game.oPlayer;
				state->scene = SCENE_SHOP;
				state->game.creditCardTries = 0;
				getText(state, &state->tInfo.text, "Shop:", (SDL_Color){255,255,255,255});
			}
			else
			{
				++state->game.creditCardTries;
				char* str;
				SDL_asprintf(&str, "Tries remaining: %d", 3 - state->game.creditCardTries);
				getText(state, &state->tInfo.secondaryText, str, (SDL_Color){255,255,255,255});
				SDL_free(str);
			}
			state->game.currentCardDigit = 0;
			SDL_strlcpy(state->game.typedcardNumber, "____", 5);
			getText(state, &state->tInfo.creditCardInputText, state->game.typedcardNumber, (SDL_Color){255,255,255,255});
		}
	}
	else if (event->type == SDL_EVENT_KEY_DOWN && state->game.creditCardTries < 3)
	{
		if (event->key.key >= SDLK_0 && event->key.key <= SDLK_9 && state->game.currentCardDigit < 4)
		{
			state->game.typedcardNumber[state->game.currentCardDigit] = event->key.key;
			++state->game.currentCardDigit;
			getText(state, &state->tInfo.creditCardInputText, state->game.typedcardNumber, (SDL_Color){255,255,255,255});
		}
		else if (event->key.key == SDLK_BACKSPACE && state->game.currentCardDigit > 0)
		{
			--state->game.currentCardDigit;
			state->game.typedcardNumber[state->game.currentCardDigit] = '_';
			getText(state, &state->tInfo.creditCardInputText, state->game.typedcardNumber, (SDL_Color){255,255,255,255});
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult handleBlockActionEvent(appState* state, SDL_Event* event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (state->game.selectedTile != -1)
		{
			state->game.blockPosition = state->game.selectedTile;
			toNextPlayer(state);
		}
		else if (state->selectedZone == ZONE_BACK)
		{
			state->scene = SCENE_GAME_ACTION;
			state->game.showNext = true;
		}
	}
	return SDL_APP_CONTINUE;
}
