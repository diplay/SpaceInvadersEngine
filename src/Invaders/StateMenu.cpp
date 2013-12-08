#include "StateMenu.h"
#include "StatePlaying.h"

std::string highstr = "High scores", newstr = "New Game", exitstr = "Exit", mark = ">";

StateMenu::StateMenu()
{
	entHigh.SetFont((Engine::Font*)(Engine::ResourceManager::GetInstance()->GetResource("Font")));
	entNewGame.SetFont((Engine::Font*)(Engine::ResourceManager::GetInstance()->GetResource("Font")));
	entExit.SetFont((Engine::Font*)(Engine::ResourceManager::GetInstance()->GetResource("Font")));
	sel = (Selection)0;
	Engine::Point pt(-0.2f, 0.0f);
	entNewGame.SetCoord(pt);
	pt.y -= 0.1f;
	entHigh.SetCoord(pt);
	pt.y -= 0.1f;
	entExit.SetCoord(pt);
	entLogo.SetCoord(Engine::Point(-0.5f, 0.2f));
	entLogo.SetWidth(1.0f);
	entLogo.SetHeight(0.5f);
	entLogo.SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("LogoSprite")));
	l.AddDrawable(&entLogo);
	l.AddDrawable(&entHigh);
	l.AddDrawable(&entNewGame);
	l.AddDrawable(&entExit);
	sc.AddLayer(&l);
	CheckSelected();
	SetCursor(NULL);
	SetScene(&sc);
}

void StateMenu::OnStateSet()
{
	GameState::OnStateSet();
	Engine::Keyboard::GetInstance()->AddListener(this);
}

void StateMenu::OnStateStop()
{
	GameState::OnStateStop();
	Engine::Keyboard::GetInstance()->RemoveListener(this);
}

void StateMenu::OnKeyDown(Engine::Control* sender, Engine::Keys k)
{
	switch(k)
	{
	case Engine::KEY_DOWN:
		sel = (Selection)(((int)sel + 1) % 3);
		break;
	case Engine::KEY_UP:
		sel = (Selection)(((int)sel - 1 + 3) % 3);
		break;
	case Engine::KEY_RETURN:
		switch(sel)
		{
		case SELECTED_HIGH_SCORES:
			Engine::Game::GetInstance()->SetGameState(Engine::StateManager::GetInstance()->GetState("StateHighScore"));
			break;
		case SELECTED_NEW_GAME:
			((StatePlaying*)(Engine::StateManager::GetInstance()->GetState("StatePlaying")))->InitNewGame();
			Engine::Game::GetInstance()->SetGameState(Engine::StateManager::GetInstance()->GetState("StatePlaying"));
			break;
		case SELECTED_EXIT:
			Engine::Game::GetInstance()->ExitGame();
			break;
		}
		break;
	case Engine::KEY_ESC:
		Engine::Game::GetInstance()->ExitGame();
		break;
	}
	CheckSelected();
}

void StateMenu::CheckSelected()
{
	entHigh.SetText("High scores");
	entNewGame.SetText("New Game");
	entExit.SetText("Exit");
	switch(sel)
	{

		case SELECTED_HIGH_SCORES:
			entHigh.SetText(mark + highstr);
			break;
		case SELECTED_NEW_GAME:
			entNewGame.SetText(mark + newstr);
			break;
		case SELECTED_EXIT:
			entExit.SetText(mark + exitstr);
			break;
	}
}