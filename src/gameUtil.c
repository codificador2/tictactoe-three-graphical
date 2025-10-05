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
	p->inv.bagAmount = 0;
	p->inv.dollarAmount = 0;
	p->inv.gunAmount = 0;
	p->inv.randomizeAmount = 0;
	p->inv.halfOtherAmount = 0;
	p->inv.setToOwnAmount = 0;
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
}
