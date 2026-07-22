#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <string.h>
#include "fifo.h"

const int winWidth = 1024, winHeight = 768;
#define LEVELWALLSLIMIT 10000 
#define ENEMIESLIMIT 1024 

/* open for suggestions on other events to add! */
typedef enum {
	SPARKINGROOF,
	ROPEBREAKING,
	TIMEMODULEFAILURE
} elevatorEvent;

struct enemy {
	Rectangle hitbox;
	FifoQueue *enqueuedEvents;
};

struct playerRPG {
	Rectangle hitbox;
	FifoQueue *enqueuedEvents;
};

typedef struct {
	/* ELEVATOR SPECIFIC BULLSHITTO */
	bool isInElevator;
	Texture elevatorImg; /* THIS NEEDS TO BE PANNED AROUND. or maybe not i still haven't decided. might not be for my sake :) */
	elevatorEvent currentElevatorEvent;
	
	/* RPG RELATED BULLSHITTO */
	Rectangle *levelWalls[LEVELWALLSLIMIT];
	struct enemy *enemies[ENEMIESLIMIT];
	Texture levelImg;
	struct playerRPG rpgPlayer;
} gameState;

void initgameState(gameState *state) {
	memset(state, 0, sizeof (gameState));
	state->isInElevator = true;
	state->elevatorImg = LoadTexture("images/elevator1.png");
}

void destroygameState(gameState *state) {
	UnloadTexture(state->elevatorImg);
}

void *getLastFreePtrArrayItem(void **array, int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		if (array[i] == NULL)
			return array[i];
	}
	return NULL;
}

void rpgDrawLevelWalls(Rectangle **walls, int size) {
	for (int i = 0; i < size && walls[i] != NULL; i++) {
		DrawRectangleRec(*walls[i], BLACK);
	}
}

void rpgDrawPlayer(struct playerRPG *player) {
	DrawRectangleRec(player->hitbox, BLACK);
}

int main(int argc, char **argv) {
	puts("UTTG!");
	InitWindow(winWidth, winHeight, "UTTG");
	gameState state;
	initgameState(&state);
	state.isInElevator = false;
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(WHITE);
			if (state.isInElevator) {
				DrawTexture(state.elevatorImg, 0, 0, WHITE);
			} else {
				rpgDrawLevelWalls(&state.levelWalls[0], LEVELWALLSLIMIT);
				rpgDrawPlayer(&state.rpgPlayer);
			}
		EndDrawing();
	}
	
	destroygameState(&state);
	
	return 0;
}
