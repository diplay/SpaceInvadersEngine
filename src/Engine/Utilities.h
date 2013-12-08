#include "class_declarations.h"
#include <string>
#include <cmath>
#include <list>
#include <map>

#ifndef UTILITIES_H
#define UTILITIES_H

ENGINE_BEGIN

class BasicFont : public Font
{
private:
	void* font;
	void DrawString(std::string str, float x, float y);
public:

	enum BasicFontType
	{
		FONT_TIMES_NEW_ROMAN_10 = 0,
		FONT_HELVETICA_10 = 1,
		FONT_TIMES_NEW_ROMAN_24 = 2,
		FONT_HELVETICA_12 = 3,
		FONT_HELVETICA_18 = 4
	};

	BasicFont();
	BasicFont(BasicFontType t);
};

class CustomFont : public Font
{
private:
	struct Glyph
	{
		int x, y;
		int width, height;
		float xOffset, yOffset;
		float xAdvance;

		Glyph():x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0){}
		Glyph(int _x, int _y, int _w, int _h, int _xo, int _yo, int _xa):x(_x), y(_y), width(_w), height(_h), xOffset((float)_xo), yOffset((float)_yo), xAdvance((float)_xa){}
	};

	Texture* tex;
	Glyph glyphs[256];
	float pxSize, height, base;

	void DrawString(std::string str, float x, float y);
public:
	CustomFont(std::string filename);
	~CustomFont();
	void SetPixelSize(float s);
};

class Vector
{
public:
	float x;
	float y;

	Vector(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector operator +(const Vector& b) const
	{
		Vector result = *this;
		result.x += b.x;
		result.y += b.y;
		return result;
	}

	Vector operator -(const Vector& b) const
	{
		Vector result = *this;
		result.x -= b.x;
		result.y -= b.y;
		return result;
	}

	float operator *(const Vector& b) const
	{
		return x*b.x + y*b.y;
	}

	Vector operator *(float k) const
	{
		return Vector(x*k, y*k);
	}

	float Length() const
	{
		return sqrt(x*x + y*y);
	}

};

class Utils
{
public:
	static int RandInt(int min, int max);
	static bool RandBool();
	static void Randomize();
	static bool IsPointInRect(struct Rectangle r, Point p);
	static bool CheckRectIntersection(struct Rectangle r1, struct Rectangle r2);
};

class TimerListener
{
public:
	virtual void OnTimerTick(Timer* t) = 0;
};

class TimerManager
{
private:
	std::list<Timer*> timers;
	std::list<Timer*> delTimers;
	bool iter;

	TimerManager operator =(TimerManager const& tm);
	TimerManager();
	TimerManager(TimerManager const& tm);
	int nextId;

	void TimerFunc(int id);
	void AddTimer(Timer* t);

	friend Timer;
	friend void GlobalTimerfunc(int id);
public:
	static TimerManager* GetInstance();
	Timer* GetTimerById(std::string id);
	void RemoveTimer(Timer* t);
	void RemoveTimer(std::string id);
};

class Timer
{
private:
	friend TimerManager;

	bool active, iterListeners, waitCallback;
	int interval;
	int innerId;
	std::string id;
	std::list<TimerListener*> listeners;
	std::list<TimerListener*> toDelete;

	void Timerfunc(int id);
public:
	Timer(std::string id, int ms, TimerListener* tl);
	void Start();
	void Stop();
	int GetInterval();
	void SetInterval(int ms);
	void AddListener(TimerListener* tl);
	void RemoveListener(TimerListener* tl);
	bool IsActive();
	void Release();
};

class Config
{
private:
	std::map<std::string, std::string> conf;
public:
	void LoadFromFile(const char* filename);

	Config(){}

	Config(std::string filename)
	{
		LoadFromFile(filename.c_str());
	}

	std::string& operator[](std::string param)
	{
		return conf[param];
	}

	void SetParameter(std::string param, std::string value)
	{
		conf[param] = value;
	}

	std::string GetParameter(std::string param)
	{
		return conf[param];
	}

};

ENGINE_END

#endif
