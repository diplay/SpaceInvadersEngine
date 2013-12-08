#include "engine.h"
#include "GL/freeglut.h"
#include <list>

ENGINE_BEGIN

TimerManager* tm = TimerManager::GetInstance();

TimerManager::TimerManager()
{
	nextId = 0;
}

TimerManager* TimerManager::GetInstance()
{
	static TimerManager t;
	return &t;
}

void TimerManager::TimerFunc(int id)
{
	iter = true;
	int sz = timers.size();
	std::list<Timer*>::iterator i;
	for(i = timers.begin(); i != timers.end(); i++)
		if((*i)->innerId == id)
			(*i)->Timerfunc(id);
	if(delTimers.size() != 0)
	{
		for(i = delTimers.begin(); i != delTimers.end(); i++)
		{
			delete (*i);
			timers.remove(*i);
		}
		delTimers.clear();
	}
	iter = false;
}

void TimerManager::AddTimer(Timer* t)
{
	timers.push_back(t);
	t->innerId = nextId;
	nextId++;
	if(nextId == -1)
		nextId++;
}

Timer* TimerManager::GetTimerById(std::string id)
{
	std::list<Timer*>::iterator i;
	for(i = timers.begin(); i != timers.end(); i++)
		if((*i)->id == id)
			return (*i);
	return NULL;
}

void TimerManager::RemoveTimer(Timer* t)
{
	t->Stop();
	if(iter)
		delTimers.push_back(t);
	else
		timers.remove(t);
}

void TimerManager::RemoveTimer(std::string id)
{
	RemoveTimer(GetTimerById(id));
}

void GlobalTimerfunc(int id)
{
	tm->TimerFunc(id);
}

Timer::Timer(std::string id, int ms, TimerListener* tl)
{
	if(tl)
		listeners.push_back(tl);
	interval = ms;
	this->id = id;
	iterListeners = false;
	active = false;
	waitCallback = false;
	tm->AddTimer(this);
}

void Timer::Start()
{
	if(!active)
	{
		active = true;
		if(!waitCallback)
			glutTimerFunc(interval, GlobalTimerfunc, innerId);
		waitCallback = true;
	}
}

void Timer::Stop()
{
	active = false;
}

bool Timer::IsActive()
{
	return active;
}

void Timer::Timerfunc(int id)
{
	waitCallback = false;
	if(active)
	{
		iterListeners = true;
		std::list<TimerListener*>::iterator i;
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnTimerTick(this);
		glutTimerFunc(interval, GlobalTimerfunc, id);
		waitCallback = true;
		iterListeners = false;
		if(!toDelete.empty())
		{
			for(i = toDelete.begin(); i != toDelete.end(); i++)
				listeners.remove(*i);
			toDelete.clear();
		}
	}
}

int Timer::GetInterval()
{
	return interval;
}

void Timer::SetInterval(int ms)
{
	interval = ms;
}

void Timer::AddListener(TimerListener* tl)
{
	if(tl)
		listeners.push_back(tl);
}

void Timer::RemoveListener(TimerListener* tl)
{
	if(!iterListeners)
		listeners.remove(tl);
	else
		toDelete.push_back(tl);
}

void Timer::Release()
{
	tm->RemoveTimer(this);
}

ENGINE_END