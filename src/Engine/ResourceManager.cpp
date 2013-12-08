#include "engine.h"
#include <fstream>

ENGINE_BEGIN

ResourceManager* ResourceManager::GetInstance()
{
	static ResourceManager rm;
	return &rm;
}

void ResourceManager::AddResource(std::string id, Resource* r)
{
	if(resources[id] != NULL)
		return;
	resources[id] = r;
}

void ResourceManager::AddResource(std::string id, std::string filename, ResourceType type)
{
	if((type != RESOURCE_OTHER) && (resources[id] != NULL))
		return;
	switch(type)
	{
	case RESOURCE_SPRITE:
		resources[id] = (Resource*)(new Sprite(filename));
		break;
	case RESOURCE_TEXTURE:
		resources[id] = (Resource*)(new Texture(filename));
		break;
	case RESOURCE_FRAME_SEQUENCE:
		resources[id] = (Resource*)(new FrameSequence(filename));
		break;
	case RESOURCE_FONT:
		resources[id] = (Resource*)(new CustomFont(filename));
		break;
	case RESOURCE_SOUND:
		resources[id] = (Resource*)(new Sound(filename));
		break;
	default:
		break;
	}
}

void ResourceManager::AddResourceList(std::string filename)
{
	//TODO
	std::ifstream f(filename.c_str());
	std::string id, file, type;
	ResourceType rt;
	while (!f.eof())
	{
		f >> id;
		f >> file;
		f >> type;
		if(type.compare("RESOURCE_SPRITE") == 0 || type.compare("1") == 0)
			rt = RESOURCE_SPRITE;
		else if (type.compare("RESOURCE_TEXTURE") == 0 || type.compare("2") == 0)
			rt = RESOURCE_TEXTURE;
		else if (type.compare("RESOURCE_FRAME_SEQUENCE") == 0 || type.compare("3") == 0)
			rt = RESOURCE_FRAME_SEQUENCE;
		else
			rt = RESOURCE_OTHER;
		AddResource(id, file, rt);
	}
	f.close();
}

Resource* ResourceManager::FindResourceByFile(std::string file)
{
	std::map<std::string, Resource*>::iterator i;
	for(i = resources.begin(); i != resources.end(); i++)
	{
		if(i->second && file == i->second->GetFileName())
			return i->second;
	}
	return NULL;
}

void ResourceManager::UnloadResource(std::string id)
{
	Resource* tmp = resources[id];
	if (tmp)
	{
		delete tmp;
		resources.erase(id);
	}
}

void ResourceManager::UnloadAll()
{
	std::map<std::string, Resource*>::iterator i;
	for(i = resources.begin(); i != resources.end(); i++)
		delete i->second;
	resources.clear();
}

Resource* ResourceManager::GetResource(std::string id)
{
	return resources[id];
}

ENGINE_END