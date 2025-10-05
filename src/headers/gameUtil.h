#pragma once

#include "./util.h"

bool canPieceChange(Piece oldPiece, Piece newPiece, bool requireHalfMove);

bool isHalfMove(Piece oldPiece, Piece newPiece);

void initGame(gameState* game);

void renderNextButton(appState* state);

void setupAlerts(appState* state);
