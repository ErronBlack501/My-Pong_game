#include "raylib.h"
#include <stdlib.h>
//----------------------------------------------------------------------------------
// Global Variables Definition
const int screenWidth = 800;
const int screenHeight = 600;
Vector2 ballPosition;
Vector2 ballSpeed;
float ballRadius;
Rectangle leftPaddle;
Rectangle rightPaddle;
float paddleSpeed;
Color DarkGreen = (Color){ 46, 170, 122, 255 };
Color LightGreen = (Color){ 1, 170, 122, 255 };
Color MyWhiteColor = (Color){ 255, 255, 255, 80 };
int leftScore = 0;
int rightScore = 0;
int winner = 0, framesCounter = 0;
bool isWinning = false, isPaused = false;

//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void DrawBall(void);
void DrawPaddles(void);
void UpdateDrawFrame(void); // Update and Draw one frame
void Initialization(void);
//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main(void)
{
    Initialization();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void DrawBall(void)
{
    DrawCircleV(ballPosition, ballRadius, YELLOW);
}

void UpdateDrawFrame(void)
{
   if (!isWinning)
   {
       if (IsKeyPressed(KEY_SPACE)) isPaused = !isPaused;
       if (!isPaused)
       {
           // Update
           //----------------------------------------------------------------------------------
           // TODO: We update our variables here

           ballPosition.x += ballSpeed.x * GetFrameTime();
           ballPosition.y += ballSpeed.y * GetFrameTime();

           //----------------------------------------------------------------------------------

           //Game logics
           //----------------------------------------------------------------------------------

           //Ball's collisions detections for top and bottom screen
           if (ballPosition.y <= ballRadius)
           {
               ballPosition.y = ballRadius;
               ballSpeed.y *= -1;
           }
           if ((ballPosition.y >= (GetScreenHeight() - ballRadius)))
           {
               ballPosition.y = GetScreenHeight() - ballRadius;
               ballSpeed.y *= -1;
           }
           //Collisions detections between ball and paddles
           if (CheckCollisionCircleRec(ballPosition, ballRadius, leftPaddle))
           {
              if (ballSpeed.x < 0)
              {
                   ballSpeed.x *= -1.1f;
                   ballSpeed.y = (ballPosition.y - leftPaddle.y) / (leftPaddle.height / 2) * ballSpeed.x;
              }
           }
           if (CheckCollisionCircleRec(ballPosition, ballRadius, rightPaddle))
           {
               if (ballSpeed.x > 0)
               {
                    ballSpeed.x *= -1.1f;
                    ballSpeed.y = (ballPosition.y - rightPaddle.y) / (rightPaddle.height / 2) * -ballSpeed.x;
               }
           }
           //------------------------------------------------------

           //Player's paddle movement
           if (IsKeyDown(KEY_W)) leftPaddle.y -= GetFrameTime() * paddleSpeed;
           if (IsKeyDown(KEY_S)) leftPaddle.y += GetFrameTime() * paddleSpeed;
           //CPU's paddle movement
           if (rightPaddle.y + rightPaddle.height > ballPosition.y)
               rightPaddle.y -= GetFrameTime() * paddleSpeed;
           if (rightPaddle.y + rightPaddle.height <= ballPosition.y)
               rightPaddle.y += GetFrameTime() * paddleSpeed;

           // Check walls collision for "paddles"
           if (leftPaddle.y < 0) leftPaddle.y = 0;
           if (leftPaddle.y + leftPaddle.height >= (float)screenHeight) leftPaddle.y = (float)screenHeight - leftPaddle.height;
           if (rightPaddle.y < 0) rightPaddle.y = 0;
           if (rightPaddle.y + rightPaddle.height >= (float)screenHeight) rightPaddle.y = (float)screenHeight - rightPaddle.height;

           if (ballPosition.x < -20*ballRadius)
           {
               ballPosition.x = GetScreenWidth() / 2;
               ballPosition.y = GetRandomValue((int)ballRadius, (int)(GetScreenHeight() - ballRadius));
               ballSpeed.x = -GetRandomValue(200, 300);
               ballSpeed.y = GetRandomValue(200, 300);
               rightScore++;
           }
           if (ballPosition.x > GetScreenWidth() + 20*ballRadius)
           {
               ballPosition.x = GetScreenWidth() / 2;
               ballPosition.y = GetRandomValue((int)ballRadius, (int)(GetScreenHeight() - ballRadius));
               ballSpeed.x = GetRandomValue(200, 300);
               ballSpeed.y = GetRandomValue(200, 300);
               leftScore++;
           }

           //Checking for a winner
           if(leftScore >= 11 || rightScore >= 11)
           {
               if(abs(leftScore - rightScore) >= 2)
               {
                   winner = (leftScore > rightScore) ? 1 : 2;
                   isWinning = true;
               }
           }

       }
       else
           framesCounter++;
   }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        if (!isWinning)
        {
            ClearBackground(LightGreen);
            DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, DarkGreen);
            DrawCircle(screenWidth / 2, screenHeight / 2, 80, MyWhiteColor);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            DrawBall();
            DrawPaddles();
            DrawText(TextFormat("%i", leftScore), screenWidth / 4 - 20, 20, 70, RAYWHITE);
            DrawText(TextFormat("%i", rightScore), 3 * screenWidth / 4 - 20, 20, 70, RAYWHITE);
            if (isPaused && ((framesCounter/30)%2)) DrawText("PAUSED", screenWidth / 2 - 60, screenHeight / 2 - 18, 30, RAYWHITE);
        }

        if (isWinning)
        {
            ClearBackground(LightGreen);
            DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, DarkGreen);
            DrawCircle(screenWidth / 2, screenHeight / 2, 80, MyWhiteColor);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            DrawText(TextFormat("%i", leftScore), screenWidth / 4 - 20, 20, 70, RAYWHITE);
            DrawText(TextFormat("%i", rightScore), 3 * screenWidth / 4 - 20, 20, 70, RAYWHITE);
            if (winner == 1)
            {
                 DrawText("Left side is the winner", GetScreenWidth() / 2 - 154, GetScreenHeight() / 2 - 10, 30, RED);

            }

            if (winner == 2)
            {
                 DrawText("Right side is the winner", GetScreenWidth() / 2 - 166, GetScreenHeight() / 2 - 10, 30, BLUE);
            }

            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                leftScore = rightScore = 0;
                winner = 0;
                leftPaddle = (Rectangle) { 15, GetScreenHeight() / 2.0f - 40, 13, 70 };
                rightPaddle = (Rectangle) { GetScreenWidth() - 25, GetScreenHeight() / 2.0f - 40, 13, 70 };
                isWinning = false;
            }
        }
        DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void Initialization(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);
    ballPosition = (Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    ballSpeed = (Vector2) { 150.0f, 150.0f };
    ballRadius = 6.0f;
    leftPaddle = (Rectangle) { 15, GetScreenHeight() / 2.0f - 40, 13, 70 };
    rightPaddle = (Rectangle) { GetScreenWidth() - 25, GetScreenHeight() / 2.0f - 40, 13, 70 };
    paddleSpeed = 400.0f;
    SetTargetFPS(144);   // Set our game to run at 60 frames-per-second minimum
    //--------------------------------------------------------------------------------------
}
void DrawPaddles(void)
{
    DrawRectangleRounded(leftPaddle, 0.8, 0, RED);
    DrawRectangleRounded(rightPaddle, 0.8, 0, BLUE);
}
