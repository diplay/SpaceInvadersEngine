#include "GameClasses.h"

#define PART_SIZE 0.07f

Obstacle::Obstacle(float x, float y, Engine::Layer* baseLayer)
{
	parts[0].SetCoord(Engine::Point(x, y + PART_SIZE * 2));
	parts[1].SetCoord(Engine::Point(x, y + PART_SIZE));
	parts[2].SetCoord(Engine::Point(x, y));
	//
	parts[3].SetCoord(Engine::Point(x + PART_SIZE, y + PART_SIZE * 2));
	parts[4].SetCoord(Engine::Point(x + PART_SIZE, y + PART_SIZE));
	//
	parts[5].SetCoord(Engine::Point(x + PART_SIZE * 2, y + PART_SIZE * 2));
	parts[6].SetCoord(Engine::Point(x + PART_SIZE * 2, y + PART_SIZE));
	//
	parts[7].SetCoord(Engine::Point(x + PART_SIZE * 3, y + PART_SIZE * 2));
	parts[8].SetCoord(Engine::Point(x + PART_SIZE * 3, y + PART_SIZE));
	parts[9].SetCoord(Engine::Point(x + PART_SIZE * 3, y));

	for(int i = 0; i < 10; i ++)
	{
		parts[i].SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite")));
		masks[i].SetSprite((Engine::Sprite*)(Engine::ResourceManager::GetInstance()->GetResource("InvaderSprite")));
		parts[i].SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("ObstacleParts")));
		masks[i].SetFrameSequence((Engine::FrameSequence*)(Engine::ResourceManager::GetInstance()->GetResource("ObstacleMasks")));
		masks[i].SetCoord(parts[i].GetCoord());
		masks[i].SetVisible(false);
		masks[i].SetWidth(PART_SIZE);
		masks[i].SetHeight(PART_SIZE);
		parts[i].SetWidth(PART_SIZE);
		parts[i].SetHeight(PART_SIZE);
		parts[i].SetFrameIndex(i);
		baseLayer->AddDrawable(&parts[i]);
		baseLayer->AddDrawable(&masks[i]);
	}
	baseL = baseLayer;
}

void Obstacle::InitObstacle()
{
	for(int i = 0; i < 10; i ++)
	{
		masks[i].SetVisible(false);
		masks[i].SetFrameIndex(0);
	}
}

void Obstacle::OnMove(Engine::Entity* e)
{
	Engine::Point pt = e->GetCoord(), ptRight = pt;
	ptRight.x += e->GetWidth();
	for(int i = 0; i < 10; i++)
	{
		if(parts[i].IsVisible() && (Engine::Utils::IsPointInRect(parts[i].GetRect(), pt) || Engine::Utils::IsPointInRect(parts[i].GetRect(), ptRight)))
		{
			if(masks[i].IsVisible())
				masks[i].SetFrameIndex(masks[i].GetFrameIndex() + 1);
			else
				masks[i].SetVisible(true);

			if(masks[i].GetFrameIndex() == 3)
			{
				masks[i].SetVisible(false);
				parts[i].SetVisible(false);
			}
			e->MoveStop();
			e->SetCoord(Engine::Point(0.0f, 2.0f));
			break;
		}
	}
}

Obstacle::~Obstacle()
{
	for(int i = 0; i < 10; i ++)
	{
		baseL->RemoveDrawable(&masks[i]);
		baseL->RemoveDrawable(&parts[i]);
	}
}