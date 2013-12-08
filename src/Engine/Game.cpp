#include "engine.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#pragma comment(lib, "glew32")

ENGINE_BEGIN

GameState* Game::GetCurrentState()
{
	return curState;
}

void Game::InitGame(int* argc, char** argv, InitParameters inp, GameState* firstState)
{
	glutInit(argc, argv);
	Render::GetInstance()->InitGraphics(inp);
	Mouse::GetInstance()->InitMouse();
	Keyboard::GetInstance()->InitKeyboard();
	Audio::GetInstance()->InitAudio();
	Utils::Randomize();
	SetGameState(firstState);
	glutMainLoop();
}

Game::Game()
{
	fps = 0;
	curState = NULL;
	nextState = NULL;
}

Game* Game::GetInstance()
{
	static Game instance;
	return &instance;
}

void Game::CheckState()
{
	if(curState != NULL)
		curState->OnStateStop();
	curState = nextState;
	nextState = NULL;
	curState->OnStateSet();
}

void Game::SetGameState(GameState* gs)
{
	if(gs == NULL)
		return;
	if(curState)
		nextState = gs;
	else
	{
		curState = gs;
		curState->OnStateSet();
	}
}

void Game::ExitGame()
{
	glutExit();
}

int Game::GetFPS()
{
	return fps;
}

void Game::ToggleFullScreen()
{
	Render::GetInstance()->FullScreenWindow();
}

ENGINE_END