#include "engine.h"
#include <string>

struct HighScore
{
	std::string player;
	int score;
	Engine::Point coor;
	Engine::Entity entScore, entPlayer;

	HighScore(): score(0){}

	HighScore(std::string p, int s, Engine::Point pt): player(p), score(s)
	{
		SetCoor(pt);
		entScore.SetFont((Engine::Font*)(Engine::ResourceManager::GetInstance()->GetResource("Font")));
		entPlayer.SetFont((Engine::Font*)(Engine::ResourceManager::GetInstance()->GetResource("Font")));
	}

	void SetCoor(Engine::Point pt)
	{
		coor = pt;
		pt.x = 0.5f;
		entScore.SetCoord(pt);
		entPlayer.SetCoord(coor);
		entPlayer.SetText(player);
		entScore.SetText(std::to_string(score));
	}
};

#define SCORE_CNT 10

class StateHighScore : public Engine::GameState, public Engine::KeyboardListener
{
private:
	HighScore scores[SCORE_CNT];
	Engine::Scene sc;
	Engine::Layer l;
	Engine::Entity entPressReturn;
public:
	StateHighScore();
	void LoadHighScores();
	void OnStateSet();
	void OnStateStop();
	void OnKeyUp(Engine::Control* sender, Engine::Keys k);
	int GetLowScore();
	void AddHighScore(std::string name, int score);
};
