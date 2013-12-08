#include "engine.h"

class Spaceship : public Engine::TimerListener, public Engine::EntityListener
{
private:
	int* hp;
	bool respawn;
	Engine::Entity* entShip, *entMissile;
	Engine::Timer* timer;
	Engine::Layer* baseL;
public:
	Spaceship(Engine::Entity* eShip, Engine::Entity* eMissile, Engine::Layer* baseLayer, int* shipHp);
	~Spaceship();
	void Fire();
	void InitSpaceShip();
	void OnTimerTick(Engine::Timer* t);
	void OnMove(Engine::Entity* e);
	void OnMovementStop(Engine::Entity* e);
};

class Missile
{
public:
	Engine::Entity* mis;
	Engine::Timer* timer, *animTimer;
	Engine::Layer* l;

	Missile(Engine::EntityListener* el,  Engine::Layer* layer)
	{
		animTimer = Engine::TimerManager::GetInstance()->GetTimerById("MissileAnim");
		l = layer;
		timer = Engine::TimerManager::GetInstance()->GetTimerById("ShipMove");
		mis = new Engine::Entity();
		mis->SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite")));
		mis->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("InvMissileAnim")));
		mis->SetWidth(0.04f);
		mis->SetHeight(0.07f);
		mis->AddListener(el);
	}

	void Fire(float x, float y)
	{
		l->AddDrawable(mis);
		mis->SetCoord(Engine::Point(x, y));
		mis->MoveStart(Engine::Point(x, -1.0f), 0.03f, timer);
		mis->AnimationStart(animTimer);
	}

	~Missile()
	{
		l->RemoveDrawable(mis);
		delete mis;
	}
};

class Invader : public Engine::TimerListener
{
public:
	Engine::Entity* inv;
	Engine::Layer* layer;
	int score;

	Invader(int i, int j, Engine::Layer*l)
	{
		layer = l;
		inv = new Engine::Entity();
		inv->SetCoord(Engine::Point(0.1333f * i, 0.15f * j));
		inv->SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite")));
		if(j == 0 || j == 1)
		{
			inv->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("Invader10Anim")));
			score = 10;
			inv->SetWidth(0.1f);
			inv->SetHeight(0.067f);
		}
		else if (j == 2 || j == 3)
		{
			inv->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("Invader20Anim")));
			score = 20;
			inv->SetWidth(0.1f);
			inv->SetHeight(0.073f);
		}
		else
		{
			inv->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("Invader40Anim")));
			score = 40;
			inv->SetWidth(0.1f);
			inv->SetHeight(0.1f);
		}
		inv->AnimationStart(Engine::TimerManager::GetInstance()->GetTimerById("InvadersMove"));
		layer->AddDrawable(inv);
	}

	void OnTimerTick(Engine::Timer* t)
	{
		t->RemoveListener(this);
		layer->RemoveDrawable(inv);
		delete this;
	}

	~Invader()
	{
		delete inv;
	}
};

class Missiles
{
private:
	std::list<Missile*> missiles;
	Engine::Layer* l;
	Engine::EntityListener* el;
	Engine::Timer* timer, *animTimer;
	Engine::EntityListener** obstacles;
public:
	Missiles(Engine::Layer* layer, Engine::EntityListener* ship, Engine::EntityListener** obst);
	~Missiles();
	void Fire(float x, float y);
};

class RedInvader : public Engine::EntityListener, public Engine::TimerListener
{
private:
	Engine::Entity* ent;
	Engine::Layer* layer;
	int* sc;
	Engine::Sound* lowpitch, *highpitch;
	Engine::Timer* soundTimer;
public:
	RedInvader(Engine::Layer* l, int* score);
	~RedInvader();
	void Go();
	void Init();
	void OnMove(Engine::Entity* e);
	void OnTimerTick(Engine::Timer* t);
};

class Invaders : public Engine::TimerListener, public Engine::EntityListener
{
private:
	RedInvader* red;
	Missiles* mis;
	Engine::Point dir;
	Engine::Layer* l;
	std::list<Invader*> invaders;
	Engine::Timer* timer;
	int* score, *hp;
	float interval;
	int stage, curSound;
	float direction, lowest;
	Engine::Sound* moveSounds[5];
public:
	Invaders(Engine::Layer* baseLayer, Engine::Layer* layer, Missiles* m, Engine::Entity* shipMissile, int* pScore, int* shipHp);
	~Invaders();
	void OnTimerTick(Engine::Timer* t);
	void OnMove(Engine::Entity* e);
	void InitInvaders();
	void SetFormation();
};

class Obstacle : public Engine::EntityListener
{
private:
	Engine::Entity parts[10];
	Engine::Entity masks[10];
	Engine::Layer* baseL;
public:
	Obstacle(float x, float y, Engine::Layer* baseLayer);
	~Obstacle();
	void OnMove(Engine::Entity* e);
	void InitObstacle();
};