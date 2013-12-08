#include "GameClasses.h"

class StatePlaying : public Engine::GameState, public Engine::KeyboardListener, public Engine::TimerListener
{
private:
	Engine::Scene s;
	Engine::Layer baseLayer, invLayer;
	Engine::Entity entFPS, entScore;
	Engine::Timer* invMove, *shipMove, *misAnim;
	Engine::Entity* entShip, *entMissile;
	Spaceship* ship;
	Invaders* inv;
	Missiles* missiles;
	Obstacle* obst[4];
	int score, hp;
	bool active;
public:
	StatePlaying();
	~StatePlaying();
	void OnStateSet();
	void OnStateStop();
	void InitNewGame();
	void OnKeyDown(Engine::Control* sender, Engine::Keys key);
	void OnTimerTick(Engine::Timer* t);
	int GetScore();
};