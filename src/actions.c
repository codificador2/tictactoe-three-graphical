#include "headers/actions.h"
#include "headers/gameUtil.h"
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

			if (SDL_rand(4) == 0) {
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
			getText(state, &state->tInfo.text, currentPlayer->creditCardNumber, (SDL_Color){255,255,255,255});
			return 2;
		case ACTION_LOOK:
			getText(state, &state->tInfo.text, "Opponents credit card:", (SDL_Color){255,255,255,255});
			char filler[4] = "____";
			int digit = SDL_rand(4);
			filler[digit] = otherPlayer->creditCardNumber[digit];
			getText(state, &state->tInfo.secondaryText, filler, (SDL_Color){255,255,255,255});
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

static void toNextPlayer(appState* state)
{

	player* currentPlayer = (state->game.currentTurn == 'x') ? &state->game.xPlayer : &state->game.oPlayer;
	// player* otherPlayer = (state->game.currentTurn == 'o') ? &state->game.xPlayer : &state->game.oPlayer;
	currentPlayer->alertCreditCard = false;
	currentPlayer->alertId = false;
	currentPlayer->money += 50;
	state->game.moveMade = false;
	state->game.mustHalfMove = false;
	SDL_memcpy(state->game.oldBoard, state->game.board, 9 * sizeof(Piece));
	state->game.selectedInSelector = 0;
	state->game.nextPiece = PIECE_NONE;
	state->scene = SCENE_GAME_BOARD;
	state->game.currentTurn = (state->game.currentTurn == 'x') ? 'o' : 'x';
	state->game.showNext = false;
	setupAlerts(state);
	state->backgroundColor.g = 0.01f;
	state->backgroundColor.r = 0.08f * (state->game.currentTurn == 'o');
	state->backgroundColor.b = 0.08f * (state->game.currentTurn == 'x');
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
