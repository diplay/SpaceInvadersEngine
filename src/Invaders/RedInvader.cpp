#include "GameClasses.h"
#include <string>

RedInvader::RedInvader(Engine::Layer* l, int* score)
{
	layer = l;
	ent = new Engine::Entity();
	ent->SetSprite((Engine::Sprite*)Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite"));
	ent->SetFrameSequence((Engine::FrameSequence*)Engine::ResourceManager::GetInstance()->GetResource("InvaderRedAnim"));
	ent->SetWidth(0.18f);
	ent->SetHeight(0.08f);
	l->AddDrawable(ent);
	sc = score;
	lowpitch = (Engine::Sound*)(Engine::ResourceManager::GetInstance()->GetResource("RedInvaderDeath"));
	highpitch = (Engine::Sound*)(Engine::ResourceManager::GetInstance()->GetResource("RedInvaderSound"));
	soundTimer = new Engine::Timer("SoundTimer", highpitch->GetLength() + 2, this);
}

void RedInvader::Init()
{
	ent->SetSprite((Engine::Sprite*)Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite"));
	ent->SetFrameSequence((Engine::FrameSequence*)Engine::ResourceManager::GetInstance()->GetResource("InvaderRedAnim"));
	ent->MoveStop();
	ent->SetCoord(Engine::Point(3.0f, 0.0f));
}

void RedInvader::Go()
{
	if(!ent->IsMoving())
	{
		static Engine::Point target = Engine::Point(2.0f, 0.73f);
		ent->SetCoord(target);
		target.x *= -1.0f;
		ent->MoveStart(target, 0.01f, Engine::TimerManager::GetInstance()->GetTimerById("ShipMove"));
		soundTimer->Start();
	}
}

void RedInvader::OnMove(Engine::Entity* e)
{
	if(ent->IsMoving() && Engine::Utils::IsPointInRect(ent->GetRect(), e->GetCoord()))
	{
		e->SetCoord(Engine::Point(-2.0f, 0.0f));
		e->MoveStop();
		int lucky = 50 * Engine::Utils::RandInt(2, 6);
		(*sc) += lucky;
		ent->MoveStop();
		Engine::TimerManager::GetInstance()->GetTimerById("FPSTimer")->AddListener(this);
		ent->SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
		ent->SetText(std::to_string(lucky));
		lowpitch->Play();
		soundTimer->Stop();
	}
}

void RedInvader::OnTimerTick(Engine::Timer* t)
{
	if(t == soundTimer)
	{
		highpitch->Play();
		if(!ent->IsMoving())
		{
			t->Stop();
		}
	}
	else
	{
		Engine::TimerManager::GetInstance()->GetTimerById("InvadersMove")->RemoveListener(this);
		Engine::TimerManager::GetInstance()->GetTimerById("FPSTimer")->RemoveListener(this);
		Init();
	}
}

RedInvader::~RedInvader()
{
	layer->RemoveDrawable(ent);
	Engine::TimerManager::GetInstance()->GetTimerById("FPSTimer")->RemoveListener(this);
	delete ent;
}
