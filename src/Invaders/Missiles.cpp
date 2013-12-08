#include "GameClasses.h"

Missiles::Missiles(Engine::Layer* layer, Engine::EntityListener* ship, Engine::EntityListener** obst)
{
	l = layer;
	el = ship;
	timer = Engine::TimerManager::GetInstance()->GetTimerById("ShipMove");
	animTimer = Engine::TimerManager::GetInstance()->GetTimerById("MissileAnim");
	obstacles = obst;
}

void Missiles::Fire(float x, float y)
{
	std::list<Missile*>::iterator i;
	for(i = missiles.begin(); i != missiles.end(); i++)
	{
		if(!(*i)->mis->IsMoving())
		{
			(*i)->Fire(x, y);
			return;
		}
	}
	Missile* m = new Missile(el, l);
	missiles.push_back(m);
	m->Fire(x, y);
	m->mis->AddListener(obstacles[0]);
	m->mis->AddListener(obstacles[1]);
	m->mis->AddListener(obstacles[2]);
	m->mis->AddListener(obstacles[3]);
}

Missiles::~Missiles()
{
	std::list<Missile*>::iterator i;
	for(i = missiles.begin(); i != missiles.end(); i++)
	{
		delete (*i);
	}
}