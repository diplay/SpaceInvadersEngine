#include "StateGameover.h"
#include <string>

StateGameover::StateGameover()
{
	entPlayerName.SetCoord(Engine::Point(-0.5f, -0.1f));
	entPlayerName.SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
	entHighScore.SetCoord(Engine::Point(-0.5f, 0.0f));
	entHighScore.SetText("Enter your name, player:");
	entHighScore.SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
	entNewGame.SetCoord(Engine::Point(-0.5f, 0.0f));
	entNewGame.SetText("GAME OVER\nPress RETURN to play again");
	entNewGame.SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
	entFPS.SetCoord(Engine::Point(0.6f, 0.9f));
	entFPS.SetText("FPS");
	entFPS.SetFont((Engine::Font*)Engine::ResourceManager::GetInstance()->GetResource("Font"));
	sc.AddLayer(&l);
	l.AddDrawable(&entNewGame);
	l.AddDrawable(&entFPS);
	l.AddDrawable(&entHighScore);
	l.AddDrawable(&entPlayerName);
	SetScene(&sc);
	st = NULL;
	fpsTimer = Engine::TimerManager::GetInstance()->GetTimerById("FPSTimer");
	st = (StatePlaying*)Engine::StateManager::GetInstance()->GetState("StatePlaying");
	stSc = (StateHighScore*)Engine::StateManager::GetInstance()->GetState("StateHighScore");
	highscore = false;
}

void StateGameover::OnStateSet()
{
	fpsTimer->AddListener(this);
	Engine::Keyboard::GetInstance()->AddListener(this);
	highscore = st->GetScore() > stSc->GetLowScore();
	entHighScore.SetVisible(highscore);
	entNewGame.SetVisible(!highscore);
	entPlayerName.SetVisible(highscore);
	playerName = std::string();
	entPlayerName.SetText(playerName);
}

void StateGameover::OnStateStop()
{
	fpsTimer->RemoveListener(this);
	Engine::Keyboard::GetInstance()->RemoveListener(this);
}

void StateGameover::OnTimerTick(Engine::Timer* t)
{
	static std::string fps = "FPS ";
	static Engine::Game* g = Engine::Game::GetInstance();
	entFPS.SetText(fps + std::to_string(g->GetFPS()));
}

void StateGameover::OnKeyUp(Engine::Control* sender, Engine::Keys k)
{
	if((k == Engine::KEY_RETURN) && !highscore)
	{
		Engine::Game::GetInstance()->SetGameState(st);
		st->InitNewGame();
	}
	if((k == Engine::KEY_RETURN) && highscore)
	{
		stSc->AddHighScore(playerName, st->GetScore());
		Engine::Game::GetInstance()->SetGameState(stSc);
	}
	if((k == Engine::KEY_BACKSPACE) && highscore)
	{
		playerName.pop_back();
		entPlayerName.SetText(playerName);
	}
}

void StateGameover::OnCharKeyDown(Engine::Control* sender, Engine::Keys k)
{
	if(highscore && (k != ' '))
	{
		playerName += (char)k;
		entPlayerName.SetText(playerName);
	}
}
