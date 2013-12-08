#include "StateGameover.h"

void StatePlaying::OnTimerTick(Engine::Timer* t)
{
	static Engine::Game* g = Engine::Game::GetInstance();
	static std::string sc = "SCORE: ";
	static std::string fps = "FPS ";
	static std::string hpstr = "\nHP: ";
	if(t == invMove)
		entFPS.SetText(fps + std::to_string(Engine::Game::GetInstance()->GetFPS()));
	else 
		entScore.SetText(sc + std::to_string(score) + hpstr + std::to_string(hp));
}

StatePlaying::StatePlaying()
{
	active = false;
	Engine::TimerManager* tm = Engine::TimerManager::GetInstance();
	invMove = new Engine::Timer("InvadersMove", 1000, NULL);
	shipMove = new Engine::Timer("ShipMove", 25, NULL);
	misAnim = new Engine::Timer("MissileAnim", 125, NULL);
	s.AddLayer(&baseLayer);
	baseLayer.AddDrawable(&invLayer);
	entFPS.SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
	entFPS.SetCoord(Engine::Point(0.6f, 0.9f));
	entFPS.SetText("FPS");
	entScore.SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
	entScore.SetCoord(Engine::Point(-1.0f, 0.9f));
	entScore.SetText("SCORE: 0");
	baseLayer.AddDrawable(&entFPS);
	baseLayer.AddDrawable(&entScore);
	SetScene(&s);
	SetCursor(NULL);
	invMove->AddListener(this);
	shipMove->AddListener(this);
}

void StatePlaying::OnStateSet()
{
	GameState::OnStateSet();
	Engine::Keyboard::GetInstance()->AddListener((Engine::KeyboardListener*)this);
	misAnim->Start();
	shipMove->Start();
	invMove->Start();
}

void StatePlaying::InitNewGame()
{
	if(active)
	{
		baseLayer.RemoveDrawable(entShip);
		baseLayer.RemoveDrawable(entMissile);
		delete inv;
		delete missiles;
		delete ship;
		delete obst[0];
		delete obst[1];
		delete obst[2];
		delete obst[3];
		delete entShip;
		delete entMissile;
	}
	score = 0;
	entShip = new Engine::Entity();
	entMissile = new Engine::Entity();
	baseLayer.AddDrawable(entShip);
	baseLayer.AddDrawable(entMissile);
	for(int i = 0; i < 4; i++)
	{
		obst[i] = new Obstacle(i * (1.8f * 0.25f) - 0.8f, -0.8f, &baseLayer);
		entMissile->AddListener(obst[i]);
	}
	ship = new Spaceship(entShip, entMissile, &baseLayer, &hp);
	missiles = new Missiles(&baseLayer, ship, (Engine::EntityListener**)obst);
	inv = new Invaders(&baseLayer, &invLayer, missiles, entMissile, &score, &hp);
	inv->InitInvaders();
	ship->InitSpaceShip();
	for(int i = 0; i < 4; i++)
		obst[i]->InitObstacle();
	active = true;
	misAnim->Start();
	shipMove->Start();
	invMove->Start();
}

void StatePlaying::OnStateStop()
{
	GameState::OnStateStop();
	Engine::Keyboard::GetInstance()->RemoveListener(this);
	shipMove->Stop();
	misAnim->Stop();
	invMove->Stop();
}

int StatePlaying::GetScore()
{
	return score;
}

StatePlaying::~StatePlaying()
{
	if(active)
	{
		delete inv;
		delete missiles;
		delete ship;
		delete obst[0];
		delete obst[1];
		delete obst[2];
		delete obst[3];
		delete entShip;
		delete entMissile;
	}
	shipMove->Release();
	misAnim->Release();
	invMove->Release();
}

void StatePlaying::OnKeyDown(Engine::Control* sender, Engine::Keys key)
{
	switch(key)
	{
	case ' ':
		ship->Fire();
		break;
	case Engine::KEY_ESC:
		Engine::Game::GetInstance()->SetGameState(Engine::StateManager::GetInstance()->GetState("StateMenu"));
		break;
	}
}