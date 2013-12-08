#include "class_declarations.h"
#include <string>
//#include <stack>

#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

ENGINE_BEGIN

class GameState
{
private:
	Scene* curScene;
	//Interface* curInterface;
	Cursor* curCursor;

	friend Game;
	friend Render;
	void Draw();
public:
	virtual void OnStateSet();
	virtual void OnStateStop();

	GameState();

	void SetScene(Scene* s);
	//void SetInterface(Interface* i);
	void SetCursor(Cursor* c);
	Cursor* GetCursor();
	void SetGlobalMouseListener(MouseListener* ml);
	void SetGlobalKeyboardListener(KeyboardListener* kl);
};

struct InitParameters
{
	int width;
	int height;
	int colorDepth;
	bool windowed;
	bool adaptResolution;
	int vsync;
	std::string windowCaption;
};

class StateManager
{
private:
	std::map<std::string, GameState*> states;
	StateManager(){};
	StateManager(StateManager const&);
	StateManager operator =(StateManager const&);
public:
	void AddState(std::string name, GameState* st);
	GameState* GetState(std::string stateName);
	void RemoveState(std::string name);
	static StateManager* GetInstance();
};

class Game
{
private:
	int fps;
	friend Render;
	friend void FPSTimerFunc(int);
	void CheckState();
	GameState* curState, *nextState;
	Game();
	Game(Game const&);
	Game operator =(Game const&);
public:
	void InitGame(int* argc, char** argv, InitParameters inp, GameState* firstState);
	void SetGameState(GameState* gs);
	GameState* GetCurrentState();
	void ExitGame();
	int GetFPS();
	void ToggleFullScreen();
	static Game* GetInstance();
};

ENGINE_END

#endif