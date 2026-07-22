#include "Game.h"

#include <vector>

Game::Game()
    :mWindow(nullptr)
    ,mRenderer(nullptr)
    ,mIsRunning(true)
{
}

Uint32 mTicksCount = 0;
int mPaddleDirLeft = 0;
int mPaddleDirRight = 0;
std::vector<Ball> mBalls;

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)",
        100,
        100,
        screenWidth,
        screenHeight,
        0
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1, // use the default graphics driver
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    
    mBalls.push_back({
        Vector2{ screenWidth / 2.0f, screenHeight / 2.0f },
        Vector2{ 200.0f, 235.0f }
    });

    mBalls.push_back({
        Vector2{ screenWidth / 2.0f, screenHeight / 2.0f },
        Vector2{ -200.0f, 150.0f }
    });

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    
    mPaddleDirLeft = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDirLeft -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDirLeft += 1;
    }
    
    mPaddleDirRight = 0;
    if (state[SDL_SCANCODE_UP])
    {
        mPaddleDirRight -= 1;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        mPaddleDirRight += 1;
    }
}

void Game::UpdateGame()
{
    // wait until 16ms are passed for limiting FPS to 60 
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;
    
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; // converted to sec
    mTicksCount = SDL_GetTicks();
    
    // clamp max deltatime
    if ( deltaTime > 0.5f )
        deltaTime = 0.5f;
    
    // paddle left
    if (mPaddleDirLeft != 0)
    {
        mPaddlePosLeft.y += mPaddleDirLeft * paddleSpeed * deltaTime;
        
        if (mPaddlePosLeft.y < (paddleHeight/2.0f + thickness))
        {
            mPaddlePosLeft.y = paddleHeight/2.0f + thickness;
        }
        else if (mPaddlePosLeft.y > screenHeight - paddleHeight/2.0f - thickness)
        {
            mPaddlePosLeft.y = screenHeight - paddleHeight/2.0f - thickness;
        }
    }
    
    // paddle right
    if (mPaddleDirRight != 0)
    {
        mPaddlePosRight.y += mPaddleDirRight * paddleSpeed * deltaTime;
        
        if (mPaddlePosRight.y < (paddleHeight/2.0f + thickness))
        {
            mPaddlePosRight.y = paddleHeight/2.0f + thickness;
        }
        else if (mPaddlePosRight.y > screenHeight - paddleHeight/2.0f - thickness)
        {
            mPaddlePosRight.y = screenHeight - paddleHeight/2.0f - thickness;
        }
    }
    
    // ball collision
    
    for (auto& ball : mBalls)
    {
        // collide top
        if ((ball.position.y <= thickness) && ball.velocity.y < 0.0f)
        {
            ball.velocity.y *= -1.0f;
        }

        // collide bottom
        if ((ball.position.y >= screenHeight - thickness) && ball.velocity.y > 0.0f)
        {
            ball.velocity.y *= -1.0f;
        }

        // collide right paddle
        float diff = mPaddlePosRight.y - ball.position.y;
        diff = (diff > 0.0f) ? diff : -diff;
        if (ball.velocity.x > 0.0f &&
            diff <= paddleHeight / 2.0f &&
            ball.position.x + thickness / 2.0f >= mPaddlePosRight.x &&
            ball.position.x <= mPaddlePosRight.x)
        {
            ball.velocity.x *= -1.0f;
        }

        // collide left paddle
        diff = mPaddlePosLeft.y - ball.position.y;
        diff = (diff > 0.0f) ? diff : -diff;
        if (ball.velocity.x < 0.0f &&
            diff <= paddleHeight / 2.0f &&
            ball.position.x - thickness / 2.0f <= mPaddlePosLeft.x + paddleLength &&
            ball.position.x >= mPaddlePosLeft.x)
        {
            ball.velocity.x *= -1.0f;
        }

        ball.position.x += ball.velocity.x * deltaTime;
        ball.position.y += ball.velocity.y * deltaTime;
    }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        0, 0, 0, 255
    );
    SDL_RenderClear(mRenderer);
    
    SDL_SetRenderDrawColor(mRenderer,255,255,255,255);
    
    // draw walls
    
    const SDL_Rect wall_top { 0,0,screenWidth,thickness };
    const SDL_Rect wall_bottom { 0,screenHeight - thickness,screenWidth,thickness };
    
    SDL_RenderFillRect(mRenderer, &wall_top);
    SDL_RenderFillRect(mRenderer, &wall_bottom);
    
    // draw paddles
    
    const SDL_Rect paddleLeft{
        static_cast<int>(mPaddlePosLeft.x),
        static_cast<int>(mPaddlePosLeft.y - paddleHeight/2),
        paddleLength,
        paddleHeight
    };
    
    SDL_RenderFillRect(mRenderer, &paddleLeft);
    
    const SDL_Rect paddleRight{
        static_cast<int>(mPaddlePosRight.x),
        static_cast<int>(mPaddlePosRight.y - paddleHeight/2),
        paddleLength,
        paddleHeight
    };
    
    SDL_RenderFillRect(mRenderer, &paddleRight);
    
    // draw balls
    
    SDL_SetRenderDrawColor(mRenderer,0,255,0,255);
    
    for (const auto& ball : mBalls)
    {
        const SDL_Rect ballRect{
            static_cast<int>(ball.position.x - thickness / 2),
            static_cast<int>(ball.position.y - thickness / 2),
            thickness,
            thickness
        };
        SDL_RenderFillRect(mRenderer, &ballRect);
    }
    
    
    
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}