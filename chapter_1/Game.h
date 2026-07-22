#pragma once
#include <SDL.h>

struct Vector2
{
    float x;
    float y;
};

struct Ball
{
    Vector2 position;
    Vector2 velocity;
};

class Game
{
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    
    bool mIsRunning;

    int thickness = 15;
    int paddleHeight = 100;
    int paddleLength = 15;
    float paddleSpeed = 300.0f;

    int screenWidth = 1024;
    int screenHeight = 768;

    Vector2 mPaddlePosLeft { 10.0f, screenHeight / 2.0f };
    Vector2 mPaddlePosRight { screenWidth - (10.0f + paddleLength ), screenHeight / 2.0f };
    Vector2 mBallPos { screenWidth / 2.0f, screenHeight / 2.0f };
    Vector2 mBallVelocity { 200.0f, 235.0f };
};