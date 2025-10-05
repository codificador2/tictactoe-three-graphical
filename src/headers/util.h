#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define MAX_TEXTURES (33)

typedef struct
{
	SDL_Texture* texture;
	int w;
	int h;
} sprite;

typedef enum
{
	ZONE_NONE,
	ZONE_PLAY_BTN,
	ZONE_SELECTOR,
	ZONE_UNDO,
	ZONE_BACK,
	ZONE_NEXT,
	ZONE_ACTION_SELECTOR
} Zone;

typedef struct
{
	int windowWidth;
	int windowHeight;
	float mouseX;
	float mouseY;
	float lastMouseDownX;
	float lastMouseDownY;
} windowInfo;

typedef enum
{
	SCENE_MAIN_MENU,
	SCENE_GAME_BOARD,
	SCENE_GAME_ACTION,
	SCENE_ACTION
} Scene;

typedef enum
{
	ACTION_STEAL,
	ACTION_CHECK_ID,
	ACTION_CHECK_CARD,
	ACTION_LOOK,
	ACTION_INVENTORY,
	ACTION_SHOP,
	ACTION_BLOCK,
	ACTION_SKIP
} Action;

typedef enum
{
	PIECE_NONE,
	PIECE_X,
	PIECE_O,
	PIECE_HALF_X,
	PIECE_HALF_O,
	PIECE_DOLLAR
} Piece;

typedef struct
{
	Uint8 setToOwnAmount: 4;
	Uint8 dollarAmount: 4;
	Uint8 randomizeAmount: 4;
	Uint8 gunAmount: 4;
	Uint8 halfOtherAmount: 4;
	Uint8 bagAmount: 4;
} inventory;

typedef struct
{
	bool hasOthersID: 1;
	bool hasOwnID: 1;
	Sint8 idCooldown: 3;
	bool alertId: 1;
	bool alertCreditCard: 1;
	Uint32 money: 32;
	char creditCardNumber[4];
	bool hasHalfDebuff: 1;
	inventory inv;
} player;

typedef struct
{
	Piece oldBoard[9];
	Piece board[9];
	Sint8 selectedTile;
	Sint8 blockPosition;
	Sint8 dollarTime;

	char currentTurn;
	Piece nextPiece;

	Uint8 selectedInSelector;

	Sint8 selectedAction;
	Action action;
	bool actionSuccess;

	player xPlayer;
	player oPlayer;

	bool moveMade;
	bool mustHalfMove;
	Sint8 lastHalfMove;

	bool showNext;
} gameState;

typedef struct
{
	sprite text;
	sprite secondaryText;
	TTF_Font* font;
} textInfo;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	sprite textures[MAX_TEXTURES];
	Uint64 textureCount;
	windowInfo wInfo;
	Zone selectedZone;
	bool updateZone;
	Scene scene;
	gameState game;
	textInfo tInfo;
	SDL_FColor backgroundColor;
} appState;

int getAppState(void* appstate, appState** newState);

int getTexture(appState* state, char* name);

int getFont(appState* state, char* name, float size);

bool isPosInRect(appState* state, float x, float y, SDL_FRect* rect);

SDL_AppResult getAllTextures(appState* state);

void getText(appState* state, sprite* dest, char* text, SDL_Color color);
