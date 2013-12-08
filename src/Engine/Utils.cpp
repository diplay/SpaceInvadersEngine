#include "engine.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <fstream>

ENGINE_BEGIN

void Utils::Randomize()
{
	srand(time(NULL));
}

bool Utils::RandBool()
{
	return rand() % 2;
}

int Utils::RandInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

bool Utils::IsPointInRect(struct Rectangle r, Point p)
{
	return p.x >= r.left && p.x <= r.right && p.y >= r.bottom && p.y <= r.top;
}

bool Utils::CheckRectIntersection(struct Rectangle r1, struct Rectangle r2)
{
	//TODO
	return false;
}

void Config::LoadFromFile(const char* filename)
{
	std::ifstream f(filename);
	std::string k, v;
	while(!f.eof())
	{
		f >> k;
		f >> v;
		conf[k] = v;
	}
	f.close();
}

ENGINE_END