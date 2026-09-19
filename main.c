/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define PLAYER_SPD 10;


typedef struct Paddle {
	Vector2 pos;
	float speed;
	Vector2 bbox;
	bool just_scored;
	double last_scored;
	double next_score;
	int score;
}Paddle;

typedef struct Ball {
	Vector2 pos;
	float speed;
	Vector2 movec;
	int diameter;
	int ball_state;
} Ball;

void IncreaseScore(Paddle* paddle);
void UpdatePaddles(Paddle* paddle1, Paddle* paddle2, int screenHeight, int screenWidth);
void UpdateBall(Ball *ball, Paddle* paddle1, Paddle* paddle2);
int main()
{
	const int screenWidth = 900;
	const int screenHeight = 600;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Pong");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	//paddle shit
	Paddle P1Paddle = { 0 };
	P1Paddle.pos = (Vector2){ 75, screenHeight / 2 - 60 };
	P1Paddle.bbox = (Vector2){ 15, 120 };
	P1Paddle.just_scored = 0;
	P1Paddle.last_scored = 0;
	P1Paddle.next_score = 0.0;

	Paddle P2Paddle = { 0 };
	P2Paddle.pos = (Vector2){ screenWidth-105, screenHeight/2-60 };
	P2Paddle.bbox = (Vector2){ 15, 120 };
	P1Paddle.just_scored = 0;
	P1Paddle.last_scored = 0;
	P1Paddle.next_score = 0.0;
	
	Ball ball = { 0 };
	ball.pos = (Vector2){ screenWidth / 2, screenHeight / 2 };
	ball.movec = (Vector2){ 5,0 };
	ball.speed = 5;
	ball.diameter = 16;
	ball.ball_state = 0;
	// Load a texture from the resources directory
	//Texture wabbit = LoadTexture("wabbit_alpha.png");

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		UpdatePaddles(&P1Paddle, &P2Paddle, screenHeight, screenWidth);

		UpdateBall(&ball, &P1Paddle, &P2Paddle);
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Pong", 200, 200, 20, WHITE);

		DrawRectangle(P1Paddle.pos.x, P1Paddle.pos.y, P1Paddle.bbox.x, P1Paddle.bbox.y, WHITE);

		DrawRectangle(P2Paddle.pos.x, P2Paddle.pos.y, P2Paddle.bbox.x, P2Paddle.bbox.y, WHITE);

		DrawCircle(ball.pos.x, ball.pos.y, ball.diameter, WHITE);

		//DrawText(TextFormat("Pos: %.2f,%.2f", ball.pos.x, ball.pos.y), 0, 0, 20, WHITE);
		//DrawText(TextFormat("Movec: %.2f,%.2f", ball.movec.x, ball.movec.y), 200, 0, 20, WHITE);
		//DrawText(TextFormat("Ball State: %i", ball.ball_state), 400, 0, 20, WHITE);
		
		DrawText(TextFormat("%i", P1Paddle.score), 150, 0, 75, WHITE);
		DrawText(TextFormat("%i", P2Paddle.score), screenWidth-150, 0, 75, WHITE);
		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	//UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void IncreaseScore(Paddle* paddle) {
	if (GetTime() > paddle->next_score) {
		paddle->score += 1;
		paddle->next_score = GetTime() + 2.5;
	}
}

void UpdatePaddles(Paddle* paddle1, Paddle* paddle2, int screenHeight, int screenWidth) {
	//player 1
	if (IsKeyDown(KEY_W)) paddle1->pos.y -= PLAYER_SPD;
	if (IsKeyDown(KEY_S)) paddle1->pos.y += PLAYER_SPD;
	
	if (paddle1->pos.y < 5) paddle1->pos.y = 5;
	if (paddle1->pos.y > screenHeight - 5 - paddle1->bbox.y) paddle1->pos.y = screenHeight - 5 - paddle1->bbox.y;

	if (IsKeyDown(KEY_UP)) paddle2->pos.y -= PLAYER_SPD;
	if (IsKeyDown(KEY_DOWN)) paddle2->pos.y += PLAYER_SPD;

	if (paddle2->pos.y < 5) paddle2->pos.y = 5;
	if (paddle2->pos.y > screenHeight - 5 - paddle2->bbox.y) paddle2->pos.y = screenHeight - 5 - paddle2->bbox.y;
};

void UpdateBall(Ball* ball, Paddle* paddle1, Paddle* paddle2) {
	Rectangle paddle1rect = { paddle1->pos.x, paddle1->pos.y, paddle1->bbox.x,paddle1->bbox.y };
	Rectangle paddle2rect = { paddle2->pos.x, paddle2->pos.y, paddle2->bbox.x,paddle2->bbox.y };

	if (ball->pos.x + (ball->diameter) > GetScreenWidth()) {
		ball->movec.x -= 2.5f;
		ball->movec.y -= 0.35f * GetRandomValue(0, 1);

		IncreaseScore(paddle1);
	}
	if (ball->pos.x - (ball->diameter) < 0) {
		ball->movec.x += 2.5f;
		ball->movec.y += 0.25f * GetRandomValue(0, 1);

		IncreaseScore(paddle2);
	}

	if (ball->pos.y + (ball->diameter) < GetScreenHeight()) {
		ball->movec.y += 2.5f;
	}

	if (ball->pos.y - (ball->diameter) > 0) {
		ball->movec.y -= 2.5f;
	}

	if (ball->pos.x > paddle1rect.x && ball->pos.x < paddle1rect.x + paddle1rect.width) {
		if (ball->pos.y > paddle1rect.y && ball->pos.y < paddle1rect.y + paddle1rect.height) {
			ball->movec.x *= -1;
		}
	}

	if (ball->pos.x > paddle2rect.x && ball->pos.x < paddle2rect.x + paddle2rect.width) {
		if (ball->pos.y > paddle2rect.y && ball->pos.y < paddle2rect.y + paddle2rect.height) {
			ball->movec.x *= -1;
		}
	}
	if (ball->movec.x > 10.00f) ball->movec.x = 10.00f;
	if (ball->movec.x < -10.00f) ball->movec.x = -10.00f;
	if (ball->movec.y > 10.00f) ball->movec.y = 10.00f;
	if (ball->movec.y < -10.00f) ball->movec.y = -10.00f;
	ball->pos.x += ball->movec.x;
	ball->pos.y += ball->movec.y;

};