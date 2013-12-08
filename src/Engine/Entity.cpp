#include "engine.h"
#include <cmath>

ENGINE_BEGIN

float const eps = 0.001f;

Entity::Entity()
{
	fnt = NULL;
	sp = NULL;
	width = 0;
	height = 0;
	curFrame = 0;
	curFrameIndex = 0;
	pt = Point();
	rect = Rectangle();
	dx = 0.0f;
	dy = 0.0f;
	dest = false;
	destX = 0.0f;
	destY = 0.0f;
	moveTimer = NULL;
	fseq = NULL;
	animation = false;
	animTimer = NULL;
	moving = false;
	visible = true;
}

Entity::~Entity()
{
	MoveStop();
	AnimationStop();
}

void Entity::SetSprite(Sprite* s)
{
	if(s)
	{
		sp = s;
		fnt = NULL;
		curFrame = 0;
		curFrameIndex = 0;
		if(fseq)
			curFrameIndex = fseq->GetFrame(0);
		AnimationStop();
	}
}

void Entity::SetText(std::string txt)
{
	text = txt;
}

void Entity::SetFont(Font* f)
{
	if(f)
	{
		fnt = f;
		sp = NULL;
		AnimationStop();
	}
}

void Entity::SetFrameSequence(FrameSequence* fs)
{
	if(fs)
	{
		fseq = fs;
		curFrameIndex = 0;
		curFrame = fs->GetFrame(0);
		AnimationStop();
	}
}

Rectangle Entity::GetRect()
{
	return rect;
}

void Entity::SetCoord(Point p)
{
	pt = p;
	rect.left = pt.x;
	rect.bottom = pt.y;
	rect.right = rect.left + width;
	rect.top = rect.bottom + height;
}

Point Entity::GetCoord()
{
	return pt;
}

void Entity::SetFrame(int frame)
{
	curFrame = frame;
}

void Entity::SetFrameIndex(int frame)
{
	if(fseq)
	{
		curFrameIndex = frame;
		curFrame = fseq->GetFrame(frame);
	}
}

int Entity::GetFrameIndex()
{
	return curFrameIndex;
}

int Entity::GetFrame()
{
	return curFrame;
}

void Entity::MoveStart(Point dest, float speed,  Timer* t)
{
	if(moving)
		MoveStop();
	moving = true;
	this->dest = true;
	destX = dest.x;
	destY = dest.y;
	Vector spd = Vector(dest.x, dest.y) - Vector(pt.x, pt.y);
	spd = spd * (speed /spd.Length());
	dx = spd.x;
	dy = spd.y;
	t->AddListener((TimerListener*)this);
	moveTimer = t;
	std::list<EntityListener*>::iterator i;
	for(i = listeners.begin(); i != listeners.end(); i++)
		(*i)->OnMovementStart(this);
}

void Entity::MoveStart(Point dest, int time, Timer* t)
{
	if(moving)
		MoveStop();
	moving = true;
	this->dest = true;
	destX = dest.x;
	destY = dest.y;
	Vector spd = Vector(dest.x, dest.y) - Vector(pt.x, pt.y);
	spd = spd * (1.0f / time);
	dx = spd.x;
	dy = spd.y;
	t->AddListener((TimerListener*)this);
	moveTimer = t;
	std::list<EntityListener*>::iterator i;
	for(i = listeners.begin(); i != listeners.end(); i++)
		(*i)->OnMovementStart(this);
}

void Entity::MoveStart(Vector speed, Timer* t)
{
	if(moving)
		MoveStop();
	moving = true;
	this->dest = false;
	dx = speed.x;
	dy = speed.y;
	t->AddListener((TimerListener*)this);
	moveTimer = t;
	std::list<EntityListener*>::iterator i;
	for(i = listeners.begin(); i != listeners.end(); i++)
		(*i)->OnMovementStart(this);
}

bool Entity::IsMoving()
{
	return moving;
}

void Entity::MoveStop()
{
	if(moving)
	{
		moveTimer->RemoveListener((TimerListener*)this);
		moveTimer = NULL;
		moving = false;
		std::list<EntityListener*>::iterator i;
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnMovementStop(this);
	}
}

void Entity::AnimationStart(Timer* t)
{
	if(!animation)
	{
		animation = true;
		t->AddListener((TimerListener*)this);
		animTimer = t;
		std::list<EntityListener*>::iterator i;
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnAnimationStart(this);
	}
}

void Entity::AnimationStop()
{
	if(animation)
	{
		animTimer->RemoveListener((TimerListener*)this);
		animTimer = NULL;
		animation = false;
		std::list<EntityListener*>::iterator i;
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnAnimationStop(this);
	}
}

void Entity::AddListener(EntityListener* el)
{
	if(el)
		listeners.push_back(el);
}

void Entity::RemoveListener(EntityListener* el)
{
	if(el)
		listeners.remove(el);
}

void Entity::Draw(float xOffset, float yOffset)
{
	if(!visible)
		return;
	if(sp)
		sp->Draw(pt.x + xOffset, pt.y + yOffset, width, height, curFrame);
	else if(fnt)
		fnt->DrawString(text, pt.x + xOffset, pt.y + yOffset);

}

void Entity::OnTimerTick(Timer* t)
{
	if(t == animTimer)
	{
		std::list<EntityListener*>::iterator i;
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnAnimation(this);
		curFrameIndex++;
		curFrame = fseq->GetFrame(curFrameIndex);
	}
	else if(t == moveTimer)
	{
		std::list<EntityListener*>::iterator i;
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnMove(this);
		pt.x += dx;
		pt.y += dy;
		rect.left = pt.x;
		rect.right = pt.x + width;
		rect.bottom = pt.y;
		rect.top = pt.y + height;
		if(dest)
			if((fabs(pt.x - destX) <= fabs(dx)) && (fabs(pt.y - destY) <= fabs(dy)))
				MoveStop();
	}
}

void Entity::SetWidth(float w)
{
	if(w > 0)
	{
		width = w;
		rect.right = rect.left + width;
	}
}

void Entity::SetHeight(float h)
{
	if(h > 0)
	{
		height = h;
		rect.top = rect.bottom + height;
	}
}

float Entity::GetWidth()
{
	return width;
}

float Entity::GetHeight()
{
	return height;
}

void Entity::SetVisible(bool v)
{
	visible = v;
}

bool Entity::IsVisible()
{
	return visible;
}

ENGINE_END