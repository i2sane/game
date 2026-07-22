#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <string.h>
#include "fifo.h"

const int winWidth = 1024, winHeight = 768;

/* open for suggestions on other events to add! */
typedef enum {
	SPARKINGROOF,
	ELEVATORSHAKING
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
	Rectangle *levelWalls[10000];
	struct enemy *enemies[1024];
	Texture levelImg;
	struct playerRPG rpgPlayer;
} gameState;

void initgameState(gameState *state) {
	memset(state, 0, sizeof (gameState));
	state->isInElevator = true;
	state->elevatorImg = LoadTexture("images/elevator1.png");
}

void *getLastFreePtrArrayItem(void **array, int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		if (array[i] == NULL)
			return array[i];
	}
	return NULL;
}

int main(int argc, char **argv) {
	puts("UTTG!");
	gameState state;
	initgameState(&state);
	InitWindow(winWidth, winHeight, "UTTG");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(WHITE);
			if (state.isInElevator) {
				DrawTexture(state.elevatorImg, 0, 0, WHITE);
			} else {
				// nothing yet
			}
		EndDrawing();
	}
	
	return 0;
}
