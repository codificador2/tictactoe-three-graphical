#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define MAX_TEXTURES (40)

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
	ZONE_ACTION_SELECTOR,
	ZONE_INVENTORY
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
	SCENE_ACTION,
	SCENE_ITEM
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
	PIECE_NONE = 0,
	PIECE_X = 1,
	PIECE_O = 2,
	PIECE_HALF_X = 3,
	PIECE_HALF_O = 4,
	PIECE_DOLLAR = 5
} Piece;

typedef enum
{
	ITEM_DOLLAR = 0,
	ITEM_SETTER = 1,
	ITEM_RANDOM = 2,
	ITEM_GUN = 3,
	ITEM_DIVIDER = 4,
	ITEM_BAG = 5
} Item;

typedef struct
{
	bool hasOthersID: 1;
	bool hasOwnID: 1;
	Sint8 idCooldown: 3;
	bool alertId: 1;
	bool alertCreditCard: 1;
	Uint32 money: 32;
	char creditCardNumber[5];
	bool hasHalfDebuff: 1;
	Uint8 inventory[6];
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

	int selectedInInventory;
	Item selectedItem;
	Piece preItemBoard[9];
} gameState;

typedef struct
{
	sprite x_dollar;
	sprite x_setter;
	sprite x_random;
	sprite x_gun;
	sprite x_divider;
	sprite x_bag;

	sprite o_dollar;
	sprite o_setter;
	sprite o_random;
	sprite o_gun;
	sprite o_divider;
	sprite o_bag;
} inventoryNums;

typedef struct
{
	sprite text;
	sprite secondaryText;
	inventoryNums invNums;
	TTF_Font* font;
	sprite dollarText;
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
