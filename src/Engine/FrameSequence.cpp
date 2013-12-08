#include "engine.h"
#include <fstream>
#include <list>

ENGINE_BEGIN

FrameSequence::FrameSequence(std::string filename) : Resource(RESOURCE_FRAME_SEQUENCE, filename)
{
	std::ifstream f(filename);
	int tmp;
	std::list<int> tmplist;
	while(!f.eof())
	{
		f >> tmp;
		tmplist.push_back(tmp);
	}
	frames = new int[tmplist.size()];
	std::list<int>::iterator i;
	int j = 0;
	for(i = tmplist.begin(); i != tmplist.end(); i++)
	{
		frames[j] = *i;
		j++;
	}
	framesCount = j;
	f.close();
}

int FrameSequence::GetFrame(unsigned int num)
{
	return frames[num % framesCount];
}

FrameSequence::~FrameSequence()
{
	delete[] frames;
}

ENGINE_END