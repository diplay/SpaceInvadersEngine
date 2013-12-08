#include "engine.h"

ENGINE_BEGIN

void Scene::AddLayer(Layer* l)
{
	if(l)
		layers.push_back(l);
}

void Scene::RemoveLayer(Layer* l)
{
	layers.remove(l);
}

void Scene::Clear()
{
	layers.clear();
}

void Scene::DrawScene()
{
	std::list<Layer*>::iterator i;
	for(i = layers.begin(); i != layers.end(); i++)
		(*i)->Draw();
}

ENGINE_END