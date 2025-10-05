#include "headers/gameUtil.h"
#include "headers/textures.h"

bool canPieceChange(Piece oldPiece, Piece newPiece, bool requireHalfMove)
{
	if (requireHalfMove)
	{
		switch (oldPiece)
		{
			case PIECE_NONE:
				return newPiece == PIECE_HALF_X || newPiece == PIECE_HALF_O;
			case PIECE_HALF_O:
				return newPiece == PIECE_NONE || newPiece == PIECE_O;
			case PIECE_O:
				return newPiece == PIECE_HALF_O;
			case PIECE_HALF_X:
				return newPiece == PIECE_NONE || newPiece == PIECE_X;
			case PIECE_X:
				return newPiece == PIECE_HALF_X;
			case PIECE_DOLLAR:
				return false;
		}
	}
	switch (oldPiece)
	{
		case PIECE_NONE:
			return newPiece != PIECE_NONE;
		case PIECE_HALF_O:
			return newPiece != PIECE_X;
		case PIECE_O:
			return newPiece == PIECE_HALF_O || newPiece == PIECE_NONE;
		case PIECE_HALF_X:
			return newPiece != PIECE_O;
		case PIECE_X:
			return newPiece == PIECE_HALF_X || newPiece == PIECE_NONE;
		case PIECE_DOLLAR:
			return false;
	}
}

static void initPlayer(player* p)
{
	p->alertCreditCard = false;
	p->alertId = false;
	p->hasHalfDebuff = false;
	p->hasOthersID = false;
	p->hasOwnID = true;
	p->idCooldown = -1;
	p->money = 1000;
	memset(p->inventory, 0, 6 * sizeof(Uint8));
	for (int i = 0; i < 4; i++)
		p->creditCardNumber[i] = SDL_rand(10) + '0';
}

void initGame(gameState* game)
{
	game->selectedInSelector = 0;
	game->nextPiece = PIECE_NONE;
	game->selectedTile = -1;
	game->selectedAction = -1;
	memset(game->board, PIECE_NONE, 9 * sizeof(Piece));
	memcpy(game->oldBoard, game->board, 9 * sizeof(Piece));
	game->moveMade = false;
	game->currentTurn = 'x';
	game->blockPosition = -1;
	game->dollarTime = -1;
	game->mustHalfMove = false;
	initPlayer(&game->xPlayer);
	initPlayer(&game->oPlayer);
	game->showNext = false;
	game->action = ACTION_SKIP;
	game->lastHalfMove = -1;
}

bool isHalfMove(Piece oldPiece, Piece newPiece)
{
	switch (oldPiece)
	{
		case PIECE_DOLLAR:
			return false;
		case PIECE_NONE:
			return newPiece == PIECE_HALF_X || newPiece == PIECE_HALF_O;
		case PIECE_HALF_X:
			return newPiece == PIECE_X || newPiece == PIECE_NONE;
		case PIECE_HALF_O:
			return newPiece == PIECE_O || newPiece == PIECE_NONE;
		case PIECE_X:
			return newPiece == PIECE_HALF_X;
		case PIECE_O:
			return newPiece == PIECE_HALF_O;
	}
}

void renderNextButton(appState* state)
{
	SDL_FRect rect;
	rect.w = state->wInfo.windowWidth / 10.0f;
	rect.h = rect.w;
	rect.x = 0;
	rect.y = state->wInfo.windowHeight - rect.h;
	SDL_RenderTexture(state->renderer, next_sprite->texture, NULL, &rect);

	if (state->updateZone)
	{
		if (isPosInRect(state, state->wInfo.mouseX, state->wInfo.mouseY, &rect))
			state->selectedZone = ZONE_NEXT;
	}
}

void setupAlerts(appState* state)
{
	player* currentPlayer = (state->game.currentTurn == 'x') ? &state->game.xPlayer : &state->game.oPlayer;
	if (currentPlayer->alertId)
		getText(state, &state->tInfo.text, "The other player attemped but failed to steal your ID", (SDL_Color){255,0,0,255});
	else if (currentPlayer->alertCreditCard)
		getText(state, &state->tInfo.text, "The other player used your credit card ", (SDL_Color){255,0,0,255});
	else if (currentPlayer->hasHalfDebuff)
		getText(state, &state->tInfo.text, "You have been shot", (SDL_Color){255,0,0,255});
}

void updateInvNums(appState* state)
{
	char str[2] = "\0\0";
	SDL_Color color = { 255, 255, 255, 255 };

	str[0] = state->game.xPlayer.inventory[ITEM_DOLLAR] + '0';
	getText(state, &state->tInfo.invNums.x_dollar, str, color);

	str[0] = state->game.xPlayer.inventory[ITEM_SETTER] + '0';
	getText(state, &state->tInfo.invNums.x_setter, str, color);

	str[0] = state->game.xPlayer.inventory[ITEM_RANDOM] + '0';
	getText(state, &state->tInfo.invNums.x_random, str, color);

	str[0] = state->game.xPlayer.inventory[ITEM_GUN] + '0';
	getText(state, &state->tInfo.invNums.x_gun, str, color);

	str[0] = state->game.xPlayer.inventory[ITEM_DIVIDER] + '0';
	getText(state, &state->tInfo.invNums.x_divider, str, color);

	str[0] = state->game.xPlayer.inventory[ITEM_BAG] + '0';
	getText(state, &state->tInfo.invNums.x_bag, str, color);


	str[0] = state->game.oPlayer.inventory[ITEM_DOLLAR] + '0';
	getText(state, &state->tInfo.invNums.o_dollar, str, color);

	str[0] = state->game.oPlayer.inventory[ITEM_SETTER] + '0';
	getText(state, &state->tInfo.invNums.o_setter, str, color);

	str[0] = state->game.oPlayer.inventory[ITEM_RANDOM] + '0';
	getText(state, &state->tInfo.invNums.o_random, str, color);

	str[0] = state->game.oPlayer.inventory[ITEM_GUN] + '0';
	getText(state, &state->tInfo.invNums.o_gun, str, color);

	str[0] = state->game.oPlayer.inventory[ITEM_DIVIDER] + '0';
	getText(state, &state->tInfo.invNums.o_divider, str, color);

	str[0] = state->game.oPlayer.inventory[ITEM_BAG] + '0';
	getText(state, &state->tInfo.invNums.o_bag, str, color);
}

void toNextPlayer(appState* state)
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
	if (state->game.dollarTime >= 0)
		--state->game.dollarTime;
	if (state->game.dollarTime == -1)
	{
		for (int i = 0; i < 9; i++)
		{
			if (state->game.board[i] == PIECE_DOLLAR)
				state->game.board[i] = PIECE_NONE;
		}
	}
	getDollarText(state);
	updateInvNums(state);
}

void getDollarText(appState* state)
{
	if (state->game.dollarTime == 0) {
		getText(state, &state->tInfo.dollarText, "Dollar leaves next turn", (SDL_Color){0,255,0,255});
		return;
	}

	char* str;
	SDL_asprintf(&str, "Dollar leaves in %d turns", state->game.dollarTime + 1);
	getText(state, &state->tInfo.dollarText, str, (SDL_Color){0,255,0,255});
	SDL_free(str);
}
