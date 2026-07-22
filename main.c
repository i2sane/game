#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <string.h>
#include "fifo.h"

#define LEVELWALLSLIMIT 10000 
#define ENEMIESLIMIT 1024 

const int winWidth = 1024, winHeight = 768;
const int playerSpeed = 4;

/* open for suggestions on other events to add! */
typedef enum {
	SPARKINGROOF,
	ROPEBREAKING,
	TIMEMODULEFAILURE
} elevatorEvent;

typedef enum {
	NOTHING
} inventoryItem;

struct enemy {
	Rectangle hitbox;
	FifoQueue *enqueuedEvents;
};

struct playerRPG {
	Rectangle hitbox;
	FifoQueue *enqueuedEvents;
};

typedef struct {
	Rectangle *attachedHitbox;
	Camera2D camera;
} rpgCamera;

typedef struct {
	inventoryItem inventory[50];
	
	/* ELEVATOR SPECIFIC BULLSHITTO */
	bool isInElevator;
	Texture elevatorImg; 
	elevatorEvent currentElevatorEvent;
	
	/* RPG RELATED BULLSHITTO */
	Rectangle *levelWalls[LEVELWALLSLIMIT];
	struct enemy *enemies[ENEMIESLIMIT];
	Texture levelImg;
	struct playerRPG rpgPlayer;
	rpgCamera camera;
} gameState;

void attachCameraToHitbox(rpgCamera *camera, Rectangle *hitbox) {
	camera->camera.target = (Vector2){
		hitbox->x + 20,
		hitbox->y + 20
	};
	camera->camera.offset = (Vector2){
		winWidth / 2.0f,
		winHeight / 2.0f
	};
}

void initgameState(gameState *state) {
	memset(state, 0, sizeof (gameState));
	state->isInElevator = true;
	state->elevatorImg = LoadTexture("images/elevator1.png");
	state->rpgPlayer.hitbox.height = 10;
	state->rpgPlayer.hitbox.width = 10;
	state->camera.camera.zoom = 1.0f;
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

void updategameState(gameState *state) {
	if (state->isInElevator) {
		
	} else {
		struct playerRPG *player = &state->rpgPlayer;
		attachCameraToHitbox(&state->camera, &player->hitbox);
		if (IsKeyDown(KEY_UP))
			player->hitbox.y -= playerSpeed;
		if (IsKeyDown(KEY_DOWN))
			player->hitbox.y += playerSpeed;
		if (IsKeyDown(KEY_LEFT))
			player->hitbox.x -= playerSpeed;
		if (IsKeyDown(KEY_RIGHT))
			player->hitbox.x += playerSpeed;
	}
}

void rpgDrawLevelWalls(Rectangle **walls, int size) {
	for (int i = 0; i < size && walls[i] != NULL; i++) {
		DrawRectangleRec(*walls[i], BLACK);
	}
}

void rpgDrawPlayer(struct playerRPG *player) {
	DrawRectangleRec(player->hitbox, BLACK);
}

void drawgameState(gameState *state) {
		ClearBackground(WHITE);
		if (state->isInElevator) {
			DrawTexture(state->elevatorImg, 0, 0, WHITE);
		} else {
			BeginMode2D(state->camera.camera);
				rpgDrawLevelWalls(&state->levelWalls[0], LEVELWALLSLIMIT);
				rpgDrawPlayer(&state->rpgPlayer);
			EndMode2D();
		}
}

int main(int argc, char **argv) {
	puts("UETG!");
	InitWindow(winWidth, winHeight, "UTTG");
	gameState state;
	initgameState(&state);
	state.isInElevator = false;
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
		updategameState(&state);
		BeginDrawing();
			drawgameState(&state);
		EndDrawing();
	}
	
	destroygameState(&state);
	
	return 0;
}
