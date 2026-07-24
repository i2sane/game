#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "timer.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define LEVELWALLSLIMIT 10000 
#define NPCLIMIT 1024 
#define FRAMERATE 60
#define INVENTORYSIZE 10
#define FLOORS 6

const int winWidth = 1024, winHeight = 768;
const int playerSpeed = 4;
Sound hum, ding; 

/* open for suggestions on other events to add! */
typedef enum {
	ONETIMEDELAY
} elevatorEvent;

typedef enum {
	NOTHING,
	LETHAL_WEAPON,
	GUM /* you swallow this to regain 10 health. lololo */
} inventoryItem;

struct npcRPG {
	Rectangle hitbox;
	bool aggressive;
	FifoQueue *enqueuedEvents;
};

struct playerRPG {
	Rectangle hitbox;
	FifoQueue *enqueuedEvents;
};

typedef struct {
	Vector2 beginningPoint;
	Vector2 endPoint;
	int wallThickness;
} collidableWall;

typedef struct {
	Rectangle *attachedHitbox;
	Camera2D camera;
} rpgCamera;

typedef struct {
	bool gameHasStarted;
	inventoryItem inventory[INVENTORYSIZE];
	bool isPanicked;
	char panicText[10000];
	int floor;
	
	/* ELEVATOR SPECIFIC BULLSHITTO */
	bool elevatorCycling;
	Timer timeToHalt;
	bool isInElevator;
	bool gettingOffElevator;
	bool oneTimeDelayDone;
	Texture elevatorImg; 
	bool rpgTransitionDue;
	
	/* RPG RELATED BULLSHITTO */
	collidableWall *levels[FLOORS][LEVELWALLSLIMIT];
	struct npcRPG *npcs[NPCLIMIT];
	Texture levelImg;
	struct playerRPG rpgPlayer;
	rpgCamera camera;
	bool rpgLevelTransitionDue;
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
	collidableWall *lvl = state->levels[4][0];
	SetRandomSeed(time(NULL));
	memset(state, 0, sizeof (gameState));
	state->isInElevator = true;
	state->elevatorImg = LoadTexture("assets/images/elevator1.png");
	state->rpgPlayer.hitbox.height = 10;
	state->rpgPlayer.hitbox.width = 10;
	state->camera.camera.zoom = 1.0f;
	state->floor = 5;
	hum = LoadSound("assets/sounds/hum.wav");
	ding = LoadSound("assets/sounds/ding.wav");
	lvl[0] = (collidableWall){
		{40, 40},
		10
	};
}

void destroygameState(gameState *state) {
	UnloadTexture(state->elevatorImg);
	UnloadSound(hum);
	UnloadSound(ding);
}

bool isTitleScreenStillShowing(gameState *state) {
	if (!state->gameHasStarted) {
		if (IsKeyDown(KEY_SPACE)) {
			state->gameHasStarted = true;
			return false;
		}
		return true;
	}
	return false;
}

void updateElevatorState(gameState *state) {
	if (IsSoundPlaying(hum) || IsSoundPlaying(ding))
			return;
	if (!isTimerInvalid(state->timeToHalt)) {
		if (!isTimerExpired(state->timeToHalt)) 
			return;
		else
			invalidateTimer(&state->timeToHalt);
	}
	if (!state->oneTimeDelayDone) {
		state->timeToHalt = createTimer(2.0);
		state->oneTimeDelayDone = true;
		return;
	}
	if (state->gettingOffElevator) {
		state->isInElevator = false;
		state->gettingOffElevator = false;
		return;
	}
	if (state->elevatorCycling) {
		state->elevatorCycling = false;
		state->floor--;
		PlaySound(ding);
		state->timeToHalt = createTimer(2.5);
		state->gettingOffElevator = true;
		return;
	}
	if (state->floor > 0) {
		state->elevatorCycling = true;
		PlaySound(hum);
	} 
}

void handleRpgPlayer(gameState *state) {
	struct playerRPG *player = &state->rpgPlayer;
	struct npcRPG **npcs = &state->npcs[0];
	collidableWall **walls = &state->levels[state->floor - 1][0];
	Rectangle oldPlrHitbox = state->rpgPlayer.hitbox;
	attachCameraToHitbox(&state->camera, &player->hitbox);
	/* THIS INFURATES ME. */
	if (IsKeyDown(KEY_UP))
		player->hitbox.y -= playerSpeed;
	if (IsKeyDown(KEY_DOWN))
		player->hitbox.y += playerSpeed;
	if (IsKeyDown(KEY_LEFT))
		player->hitbox.x -= playerSpeed;
	if (IsKeyDown(KEY_RIGHT))
		player->hitbox.x += playerSpeed;
	for (int i = 0; walls[i] != NULL && i < LEVELWALLSLIMIT; i++) {
		/*if (CheckCollisionPointRec(walls[i]->wall, player->hitbox))
			player->hitbox = oldPlrHitbox;*/
	}
}

void updategameState(gameState *state) {
	if (isTitleScreenStillShowing(state))
		return;
	if (state->isInElevator) {
		updateElevatorState(state);
	} else {
		handleRpgPlayer(state);
		//handleRpgNpcs(state);
	}
}

void rpgDrawLevelWalls(collidableWall **walls, int size) {
	for (int i = 0; i < size && walls[i] != NULL; i++) {
		DrawLineEx(walls[i]->beginningPoint, walls[i]->endPoint, walls[i]->wallThickness, BLACK);
	}
}

void rpgDrawPlayer(struct playerRPG *player) {
	DrawRectangleRec(player->hitbox, BLACK);
}

void drawgameState(gameState *state) {
		char buf[1024] = {0};
		if (!state->gameHasStarted) {
			ClearBackground(BLACK);
			/* this does apparently center the hitbox of the text but the text itself remains uncentered. 
			 * i'll get around to fixing this eventually. */
			char *text = "UETG :D\n\nPress space to start the game.";
			Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 24, 0);
			DrawText(text, 
					 winWidth / 2 - (int)(textSize.x / 2), 
					 winHeight / 2 - (int)(textSize.y / 2), 
					 24, 
					 WHITE);
			return;
		}
		ClearBackground(WHITE);
		if (state->isInElevator) {
			DrawTexture(state->elevatorImg, 0, 0, WHITE);
			if (state->floor != 0) {
				snprintf(buf, 1023, "%d", state->floor);
				DrawText(buf, 299, 291, 12, WHITE);
			} else {
				DrawText("you win", 299, 291, 12, WHITE);
			}
		} else {
			BeginMode2D(state->camera.camera);
#if UETG_DEBUG
				struct playerRPG *player = &state->rpgPlayer;
				snprintf(buf, 1023, "%f, %f", player->hitbox.x, player->hitbox.y);
				DrawText(buf, player->hitbox.x + 10, player->hitbox.y + 10, 14, BLACK);
#endif
				rpgDrawLevelWalls(&state->levels[state->floor - 1][0], LEVELWALLSLIMIT);
				rpgDrawPlayer(&state->rpgPlayer);
			EndMode2D();
		}
}

gameState state;

void drawFrame() {
	updategameState(&state);
	BeginDrawing();
		drawgameState(&state);
#if UETG_DEBUG
		printf("%d, %d\n", GetMouseX(), GetMouseY());
#endif
	EndDrawing();
}

int main(int argc, char **argv) {
	puts("UETG!");
	InitWindow(winWidth, winHeight, "UETG");
	InitAudioDevice();
	initgameState(&state);
	SetTargetFPS(FRAMERATE);
	
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(drawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		drawFrame();
	}
#endif
	
	destroygameState(&state);
	
	return 0;
}
