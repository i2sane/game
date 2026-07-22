#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "fifo.h"

#define LEVELWALLSLIMIT 10000 
#define NPCLIMIT 1024 

const int winWidth = 1024, winHeight = 768;
const int playerSpeed = 4;
bool debug = true;
Sound hum, ding; 

/* open for suggestions on other events to add! */
typedef enum {
	SPARKINGROOF,
	ROPEBREAKING,
	DINGNOISE,
	HUMNOISE
} elevatorEvent;

typedef enum {
	NOTHING,
	ADVERTISER_FRIENDLY_WEAPON,
	GUM /* you swallow this to regain 10 health. lololo */
} inventoryItem;

struct npc {
	Rectangle hitbox;
	bool aggressive;
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
	bool gameHasStarted;
	inventoryItem inventory[10];
	
	/* ELEVATOR SPECIFIC BULLSHITTO */
	int floor;
	int cyclesTillNextFloor;
	bool isInElevator;
	Texture elevatorImg; 
	FifoQueue enqueuedElevatorEvents;
	
	/* RPG RELATED BULLSHITTO */
	Rectangle *levelWalls[LEVELWALLSLIMIT];
	struct npc *npc[NPCLIMIT];
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
	SetRandomSeed(time(NULL));
	memset(state, 0, sizeof (gameState));
	state->isInElevator = true;
	state->elevatorImg = LoadTexture("images/elevator1.png");
	state->rpgPlayer.hitbox.height = 10;
	state->rpgPlayer.hitbox.width = 10;
	state->camera.camera.zoom = 1.0f;
	state->floor = 5;
	hum = LoadSound("sounds/hum.ogg");
	ding = LoadSound("sounds/ding.ogg");
	Rectangle **levelWalls = &state->levelWalls[0];
}

void destroygameState(gameState *state) {
	UnloadTexture(state->elevatorImg);
	UnloadSound(hum);
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
		elevatorEvent *queuedEvent = (elevatorEvent *)popFromQueue(&state->enqueuedElevatorEvents);
		if (queuedEvent != NULL) {
			switch (*queuedEvent) {
				case DINGNOISE:
					PlaySound(ding);
					usleep(1000 * 700);
					break;
				case HUMNOISE:
					PlaySound(hum);
					usleep(1000 * 1000);
					break;
			}
			free(queuedEvent);
		}
		if (state->cyclesTillNextFloor <= 0) {
			elevatorEvent newEvent = DINGNOISE;
			state->cyclesTillNextFloor = GetRandomValue(3, 6);
			if (state->floor != 5)
				state->floor--;
			else
				addToQueue(&state->enqueuedElevatorEvents, &newEvent, sizeof (newEvent));
		} else {
			elevatorEvent newEvent = HUMNOISE;
			addToQueue(&state->enqueuedElevatorEvents, &newEvent, sizeof (newEvent));
			state->cyclesTillNextFloor--;
		}
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
				if (debug) {
					char buf[1024] = {0};
					struct playerRPG *player = &state->rpgPlayer;
					snprintf(buf, 1023, "%f, %f", player->hitbox.x, player->hitbox.y);
					DrawText(buf, player->hitbox.x + 10, player->hitbox.y + 10, 14, BLACK);
				}
				rpgDrawLevelWalls(&state->levelWalls[0], LEVELWALLSLIMIT);
				rpgDrawPlayer(&state->rpgPlayer);
			EndMode2D();
		}
}

int main(int argc, char **argv) {
	puts("UETG!");
	InitWindow(winWidth, winHeight, "UETG");
	InitAudioDevice();
	gameState state;
	initgameState(&state);
//	state.isInElevator = false;
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
