#include "GameClasses.h"

#define SHIP_WIDTH 0.13f

Spaceship::Spaceship(Engine::Entity* eShip, Engine::Entity* eMissile, Engine::Layer* baseLayer, int* shipHp)
{
	hp = shipHp;
	entShip = eShip;
	entMissile = eMissile;
	timer = Engine::TimerManager::GetInstance()->GetTimerById("ShipMove");
	baseL = baseLayer;
	timer->AddListener((Engine::TimerListener*)this);
	entShip->SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite")));
	entShip->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("ShipAnim")));
	entShip->SetWidth(SHIP_WIDTH);
	entShip->SetHeight(0.08f);
	entMissile->SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite")));
	entMissile->SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("MissileAnim")));
	entMissile->SetWidth(0.01f);
	entMissile->SetHeight(0.05f);
	entMissile->SetCoord(Engine::Point(0.0f, -2.0f));
	respawn = false;
}

void Spaceship::InitSpaceShip()
{
	entShip->SetCoord(Engine::Point(0.0f, -0.95f));
	*hp = 3;
}

void Spaceship::OnTimerTick(Engine::Timer* t)
{
	static Engine::Keyboard* keyboard = Engine::Keyboard::GetInstance();
	if(t == timer)
	{
		int l = keyboard->GetKeyState((Engine::Keys)Engine::KEY_LEFT);
		int r = keyboard->GetKeyState((Engine::Keys)Engine::KEY_RIGHT);
		Engine::Point c = entShip->GetCoord();
		if(l && c.x > -1.0f)
			c.x -= 0.025f;
		if(r && c.x < (1.0f - SHIP_WIDTH))
			c.x += 0.025f;
		entShip->SetCoord(c);
	}
	else
	{
		static int cnt = 0;
		entShip->SetVisible((bool)(cnt % 2));
		cnt++;
		if(cnt == 10)
		{
			cnt = 0;
			t->RemoveListener(this);
			respawn = false;
		}
	}
}

void Spaceship::OnMove(Engine::Entity* e)
{
	if(Engine::Utils::IsPointInRect(entShip->GetRect(), e->GetCoord()) && !respawn)
	{
		static Engine::Sound* s = (Engine::Sound*)(Engine::ResourceManager::GetInstance()->GetResource("ShipDeath"));
		s->Play();
		e->MoveStop();
		(*hp)--;
		if(*hp == 0)
			Engine::Game::GetInstance()->SetGameState(Engine::StateManager::GetInstance()->GetState("StateGameover"));
		respawn = true;
		Engine::TimerManager::GetInstance()->GetTimerById("MissileAnim")->AddListener(this);
	}
}

void Spaceship::OnMovementStop(Engine::Entity* e)
{
	baseL->RemoveDrawable(e);
}

void Spaceship::Fire()
{
	if(!entMissile->IsMoving())
	{
		static Engine::Sound* s = (Engine::Sound*)(Engine::ResourceManager::GetInstance()->GetResource("MissileLaunch"));
		Engine::Point pt = entShip->GetCoord();
		pt.x += SHIP_WIDTH / 2.0f;
		entMissile->SetCoord(pt);
		pt.y = 1.1f;
		entMissile->MoveStart(pt, 0.07f, timer);
		s->Play();
	}
}

Spaceship::~Spaceship()
{
	timer->RemoveListener((Engine::TimerListener*)this);
	Engine::TimerManager::GetInstance()->GetTimerById("MissileAnim")->RemoveListener(this);
	entMissile->MoveStop();
}
