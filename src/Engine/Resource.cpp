#include "engine.h"

ENGINE_BEGIN

Resource::Resource(ResourceType type, std::string filename)
{
	rt = type;
	file = filename;
}

Resource::~Resource()
{

}

std::string Resource::GetFileName()
{
	return file;
}

ENGINE_END