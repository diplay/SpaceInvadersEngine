#include "engine.h"
#include "StatePlaying.h"
#include "StateHighScore.h"

class StateGameover : public Engine::GameState, public Engine::KeyboardListener, public Engine::TimerListener
{
private:
	Engine::Entity entNewGame, entFPS, entHighScore, entPlayerName;
	Engine::Scene sc;
	Engine::Layer l;
	bool highscore;
	StatePlaying* st;
	Engine::Timer* fpsTimer;
	StateHighScore* stSc;
	std::string playerName;
public:
	StateGameover();

	void OnStateSet();
	void OnStateStop();

	void OnTimerTick(Engine::Timer* t);

	void StartGame();
	void OnKeyUp(Engine::Control* sender, Engine::Keys k);
	void OnCharKeyDown(Engine::Control* sender, Engine::Keys k);
};