#include "engine.h"

ENGINE_BEGIN

GameState::GameState()
{
	curScene = NULL;
	curCursor = new DefaultCursor();
}

void GameState::OnStateSet()
{
	if(curCursor)
		curCursor->OnCursorSet();
}

void GameState::OnStateStop()
{
	if(curCursor)
		curCursor->OnCursorStop();
}

void GameState::SetScene(Scene* s)
{
	curScene = s;
}

void GameState::SetCursor(Cursor* c)
{
	if(curCursor)
		curCursor->OnCursorStop();
	curCursor = c;
	if(curCursor)
		curCursor->OnCursorSet();
}

Cursor* GameState::GetCursor()
{
	return curCursor;
}

void GameState::Draw()
{
	static Mouse* m = Mouse::GetInstance();
	if(curScene)
		curScene->DrawScene();
	if(curCursor)
		curCursor->DrawCursor(m->GetCoords());
}

ENGINE_END