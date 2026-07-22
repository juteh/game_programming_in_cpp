#include "Game.h"

Game::Game()
    :mWindow(nullptr)
    ,mRenderer(nullptr)
    ,mIsRunning(true)
{
}

Uint32 mTicksCount = 0;
int mPaddleDir = 0;

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
    
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
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
    
    // paddle
    if (mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * paddleSpeed * deltaTime;
        
        if (mPaddlePos.y < (paddleHeight/2.0f + thickness))
        {
            mPaddlePos.y = paddleHeight/2.0f + thickness;
        }
        else if (mPaddlePos.y > 768.0f - paddleHeight/2.0f - thickness)
        {
            mPaddlePos.y = screenHeight - paddleHeight/2.0f - thickness;
        }
    }
    
    // ball collision
    
    // collide top
    if ((mBallPos.y <= thickness)  && mBallVelocity.y < 0.0f)
    {
        mBallVelocity.y *= -1.0f;
    }
    
    // collide bottom
    if ((mBallPos.y >= screenHeight - thickness)  && mBallVelocity.y > 0.0f)
    {
        mBallVelocity.y *= -1.0f;
    }
    
    // collide right
    if (mBallPos.x >= screenWidth - thickness)
    {
        mBallVelocity.x *= -1.0f;
    }
    
    // collide left (paddle)
    float diff = mPaddlePos.y - mBallPos.y;
    diff = (diff > 0.0f) ? diff : -diff;

    if (mBallVelocity.x < 0.0f &&
        diff <= paddleHeight / 2.0f &&
        mBallPos.x <= mPaddlePos.x + paddleLength &&
        mBallPos.x >= mPaddlePos.x)
    {
        mBallVelocity.x *= -1.0f;
    }
    
    mBallPos.x += mBallVelocity.x * deltaTime;
    mBallPos.y += mBallVelocity.y * deltaTime;
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        0, 0, 255, 255
    );
    SDL_RenderClear(mRenderer);
    
    SDL_SetRenderDrawColor(mRenderer,255,255,255,255);
    
    // draw walls
    
    const SDL_Rect wall_top { 0,0,screenWidth,thickness };
    const SDL_Rect wall_right { screenWidth - thickness,0,thickness,screenHeight };
    const SDL_Rect wall_bottom { 0,screenHeight - thickness,screenWidth,thickness };
    
    SDL_RenderFillRect(mRenderer, &wall_top);
    SDL_RenderFillRect(mRenderer, &wall_right);
    SDL_RenderFillRect(mRenderer, &wall_bottom);
    
    // draw ball
    
    const SDL_Rect ball{
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };

    SDL_RenderFillRect(mRenderer, &ball);
    
    // draw paddle
    
    const SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleHeight/2),
        paddleLength,
        paddleHeight
    };
    
    SDL_RenderFillRect(mRenderer, &paddle);
    
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}