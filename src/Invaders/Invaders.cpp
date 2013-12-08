#include "GameClasses.h"

#define min(x, y) (x < y ? x : y)

Invaders::Invaders(Engine::Layer* baseLayer, Engine::Layer* layer, Missiles* m, Engine::Entity* shipMissile, int* pScore, int* shipHp)
{
	l = layer;
	mis = m;
	timer = Engine::TimerManager::GetInstance()->GetTimerById("InvadersMove");
	timer->AddListener((Engine::TimerListener*)this);
	score = pScore;
	interval = 1000.0f;
	hp = shipHp;
	red = new RedInvader(baseLayer, pScore);
	shipMissile->AddListener(this);
	shipMissile->AddListener(red);
	stage = 0;
	Engine::ResourceManager* rm = Engine::ResourceManager::GetInstance();
	moveSounds[0] = (Engine::Sound*)rm->GetResource("InvaderSound4");
	moveSounds[1] = (Engine::Sound*)rm->GetResource("InvaderSound1");
	moveSounds[2] = (Engine::Sound*)rm->GetResource("InvaderSound2");
	moveSounds[3] = (Engine::Sound*)rm->GetResource("InvaderSound3");
	moveSounds[4] = (Engine::Sound*)rm->GetResource("InvaderDeath");
}

void Invaders::InitInvaders()
{
	stage = 0;
	SetFormation();
}

void Invaders::SetFormation()
{
	for(int i = 0; i < 11; i++)
		for(int j = 0; j < 5; j++)
			invaders.push_back(new Invader(i, j, l));
	l->SetOffset(Engine::Point(-0.7f,  - 0.05f * stage));
	stage++;
	direction = 0.03f;
	lowest = 1.0f;
	dir = Engine::Point(direction, 0.0f);
	red->Init();
	interval = 1000.0f;
	timer->SetInterval((int)interval);
	curSound = 0;
}

void Invaders::OnTimerTick(Engine::Timer* t)
{
	curSound = (curSound + 1) % 4;
	moveSounds[curSound]->Play();
	//
	l->AddOffset(Engine::Point(dir));
	Engine::Point lOffset = l->GetOffset();
	if(dir.y < 0.0f)
	{
		dir.x = direction;
		dir.y = 0.0f;
	}
	else if(((dir.x > 0.0f) && (invaders.back()->inv->GetCoord().x + lOffset.x > 0.8f)) ||
		((dir.x < 0.0f) && (invaders.front()->inv->GetCoord().x + lOffset.x <= -0.95f)))
	{
		dir.x = 0.0f;
		dir.y = -0.08f;
		direction *= -1;
	}
	//
	std::list<Invader*>::iterator i;
	for(i = invaders.begin(); i != invaders.end(); i++)
	{
		lowest = min(lOffset.y + (*i)->inv->GetCoord().y, lowest);
		if(Engine::Utils::RandInt(0, 100) == 1)
		{
			Engine::Point pt = (*i)->inv->GetCoord();
			pt.x += lOffset.x;
			pt.y += lOffset.y;
			mis->Fire(pt.x, pt.y);
		}
	}
	//
	if(lowest < -0.9f)
		Engine::Game::GetInstance()->SetGameState(Engine::StateManager::GetInstance()->GetState("StateGameover"));
	//
	if(Engine::Utils::RandInt(0, 25000/interval) == 1)
		red->Go();
}

void Invaders::OnMove(Engine::Entity* e)
{
	Engine::Point pt = e->GetCoord(), pte = e->GetCoord();
	if(pt.y > 1.0f)
		e->MoveStop();
	std::list<Invader*>::iterator i;
	pt.x -= l->GetOffset().x;
	pt.y -= l->GetOffset().y;
	pte.x -= l->GetOffset().x;
	pte.y -= l->GetOffset().y;
	pte.y += 0.1f;
	for(i = invaders.begin(); i != invaders.end(); i++)
	{
		if(Engine::Utils::IsPointInRect((*i)->inv->GetRect(), pt) || Engine::Utils::IsPointInRect((*i)->inv->GetRect(), pte))
		{
			e->SetCoord(Engine::Point(-2.0f, 0.0f));
			e->MoveStop();
			*score += (*i)->score;
			(*i)->inv->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("DeathAnim")));
			timer->AddListener(*i);
			invaders.erase(i);
			interval -= 17.6f;
			timer->SetInterval((int)interval);
			moveSounds[4]->Play();
			break;
		}
	}
	if(invaders.size() == 0)
	{
		(*hp)++;
		SetFormation();
	}
}

Invaders::~Invaders()
{
	timer->RemoveListener((Engine::TimerListener*)this);
	std::list<Invader*>::iterator i;
	for(i = invaders.begin(); i != invaders.end(); i++)
	{
		l->RemoveDrawable((*i)->inv);
		delete (*i);
	}
	invaders.clear();
	delete red;
}