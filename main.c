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
	Image elevatorImg; /* THIS NEEDS TO BE PANNED AROUND. or maybe not i still haven't decided. might not be for my sake :) */
	elevatorEvent currentElevatorEvent;
	
	/* RPG RELATED BULLSHITTO */
	Rectangle *levelWalls[10000];
	struct enemy *enemies[1024];
	struct playerRPG rpgPlayer;
} gameState;

void initgameState(gameState *state) {
	memset(state, 0, sizeof (gameState));
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
		EndDrawing();
	}
	
	return 0;
}
