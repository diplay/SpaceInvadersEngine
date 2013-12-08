#include "engine.h"

ENGINE_BEGIN

void StateManager::AddState(std::string name, GameState* state)
{
	if(states[name] == NULL)
		states[name] = state;
}

GameState* StateManager::GetState(std::string stateName)
{
	return states[stateName];
}

void StateManager::RemoveState(std::string name)
{
	states[name] = NULL;
}

StateManager* StateManager::GetInstance()
{
	static StateManager sm;
	return &sm;
}

ENGINE_END