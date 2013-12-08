#include "StateGameover.h"
#include "StateMenu.h"

class StateLoading : public Engine::GameState
{
public:
	void OnStateSet()
	{
		Engine::ResourceManager* rm = Engine::ResourceManager::GetInstance();
		Engine::CustomFont* fnt = new Engine::CustomFont("invaders.fnt");
		fnt->SetPixelSize(0.003f);
		rm->AddResource("Font", (Engine::Resource*)fnt);
		rm->AddResource("InvaderSprite", "invSprite.spr", Engine::RESOURCE_SPRITE);
		rm->AddResource("LogoSprite", "logoSprite.spr", Engine::RESOURCE_SPRITE);
		rm->AddResource("Invader10Anim", "fsInv10.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("Invader20Anim", "fsInv20.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("Invader40Anim", "fsInv40.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("MissileAnim", "fsMissile.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("InvaderRedAnim", "fsInvRed.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("InvMissileAnim", "fsInvMissile.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("DeathAnim", "fsBoom.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("ShipAnim", "fsShip.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("ObstacleParts", "fsObstacle.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("ObstacleMasks", "fsMask.txt", Engine::RESOURCE_FRAME_SEQUENCE);
		rm->AddResource("RedInvaderDeath", "ufo_lowpitch.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("RedInvaderSound", "ufo_highpitch.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("InvaderSound1", "fastinvader1.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("InvaderSound2", "fastinvader2.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("InvaderSound3", "fastinvader3.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("InvaderSound4", "fastinvader4.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("InvaderDeath", "invaderkilled.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("MissileLaunch", "shoot.wav", Engine::RESOURCE_SOUND);
		rm->AddResource("ShipDeath", "explosion.wav", Engine::RESOURCE_SOUND);
		Engine::Timer* t = new Engine::Timer("FPSTimer", 1000, NULL);
		t->Start();
		StateMenu* st = new StateMenu();
		Engine::StateManager::GetInstance()->AddState("StatePlaying", new StatePlaying());
		Engine::StateManager::GetInstance()->AddState("StateHighScore", new StateHighScore());
		Engine::StateManager::GetInstance()->AddState("StateGameover", new StateGameover());
		Engine::StateManager::GetInstance()->AddState("StateMenu", st);
		Engine::Game::GetInstance()->SetGameState(st);
	}

	void OnStateStop(){}
};

int main(int argc, char** argv)
{
	Engine::Game* game = Engine::Game::GetInstance();
	Engine::InitParameters params;
	params.width = 512;
	params.height = 512;
	params.windowCaption = "Engine Test";
	params.adaptResolution = true;
	params.windowed = true;
	params.vsync = 0;
	game->InitGame(&argc, argv, params, new StateLoading());
	return 0;
}