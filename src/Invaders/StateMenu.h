#include "engine.h"

class StateMenu : public Engine::GameState, public Engine::KeyboardListener
{
private:
	Engine::Scene sc;
	Engine::Layer l;
	Engine::Entity entNewGame, entExit, entHigh, entLogo;
	enum Selection
	{
		SELECTED_NEW_GAME = 0,
		SELECTED_HIGH_SCORES = 1,
		SELECTED_EXIT = 2
	} sel;
	void CheckSelected();
public:
	void OnStateSet();
	void OnStateStop();
	void OnKeyDown(Engine::Control* sender, Engine::Keys k);
	StateMenu();
};