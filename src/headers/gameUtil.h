#pragma once

#include "./util.h"

bool canPieceChange(Piece oldPiece, Piece newPiece, bool requireHalfMove);

bool isHalfMove(Piece oldPiece, Piece newPiece);

void initGame(gameState* game);

void renderNextButton(appState* state);

void setupAlerts(appState* state);

void updateInvNums(appState* state);

void toNextPlayer(appState* state);

void getDollarText(appState* state);

void renderStaticBoard(appState* state, Piece board[9], SDL_FRect* rect);

void renderSingleItem(appState* state, sprite* texture, int index, SDL_FRect* rect, bool inventory);

Piece getWinningPiece(appState* state);
