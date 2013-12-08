#include "StateHighScore.h"
#include <fstream>
#include <algorithm>

void StateHighScore::LoadHighScores()
{
	std::ifstream f("highscores.txt");
	std::string name;
	int score;
	Engine::Point pt(-0.6f, 0.8f);
	for(int i = 0; i < SCORE_CNT; i++)
	{
		f >> name >> score;
		scores[i] = HighScore(name, score, pt);
		pt.y -= 0.1f;
	}
	f.close();
}

StateHighScore::StateHighScore()
{
	LoadHighScores();
	sc.AddLayer(&l);
	entPressReturn.SetFont((Engine::Font*)(Engine::ResourceManager::GetInstance()->GetResource("Font")));
	entPressReturn.SetText("Press ESC to return to main menu");
	entPressReturn.SetCoord(Engine::Point(-0.7f, -0.8f));
	l.AddDrawable(&entPressReturn);
	for(int i = 0; i < SCORE_CNT; i++)
	{
		l.AddDrawable(&(scores[i].entPlayer));
		l.AddDrawable(&(scores[i].entScore));
	}
	SetScene(&sc);
	SetCursor(NULL);
}

void StateHighScore::OnStateSet()
{
	GameState::OnStateSet();
	Engine::Keyboard::GetInstance()->AddListener(this);
}

void StateHighScore::OnStateStop()
{
	GameState::OnStateStop();
	Engine::Keyboard::GetInstance()->RemoveListener(this);
	std::ofstream f("highscores.txt");
	for(int i = 0; i < SCORE_CNT; i++)
		f << scores[i].player << ' ' << scores[i].score << '\n';
	f.close();
}

void StateHighScore::OnKeyUp(Engine::Control* sender, Engine::Keys k)
{
	if(k == Engine::KEY_ESC)
		Engine::Game::GetInstance()->SetGameState(Engine::StateManager::GetInstance()->GetState("StateMenu"));
}

int StateHighScore::GetLowScore()
{
	return scores[SCORE_CNT - 1].score;
}

bool CompScores(HighScore const& left, HighScore const& right)
{
	return left.score > right.score;
}

void StateHighScore::AddHighScore(std::string name, int score)
{
	if(scores[SCORE_CNT - 1].score < score)
	{
		scores[SCORE_CNT - 1].player = name;
		scores[SCORE_CNT - 1].score = score;
	}
	std::sort(scores, &scores[SCORE_CNT], CompScores);
	Engine::Point pt(-0.6f, 0.8f);
	for(int i = 0; i < SCORE_CNT; i++)
	{
		scores[i].SetCoor(pt);
		pt.y -= 0.1f;
	}
}
