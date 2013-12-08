#include "class_declarations.h"
#include "Utilities.h"
#include <list>

#ifndef GRAPHICAL_H
#define GRAPHICAL_H

ENGINE_BEGIN

class Drawable
{
protected:
	friend Layer;
	virtual void Draw(float xOffset = 0, float yOffset = 0) = 0;
};

class Entity : public Drawable, public TimerListener
{
private:
	std::string text;
	Font* fnt;
	//
	Sprite* sp;
	float width, height;
	Point pt;
	Rectangle rect;
	//
	bool moving;
	float dx, dy;
	float destX, destY;
	bool dest;
	Timer* moveTimer;
	//
	FrameSequence* fseq;
	int curFrameIndex, curFrame;
	Timer* animTimer;
	bool animation;
	bool visible;
	//
	std::list<EntityListener*> listeners;
	//
	friend Layer;
	virtual void Draw(float xOffset = 0, float yOffset = 0);
public:
	Entity();
	~Entity();
	virtual void OnTimerTick(Timer* t);
	void SetVisible(bool v);
	bool IsVisible();
	void SetCoord(Point c);
	Point GetCoord();
	void MoveStart(Point dest, float speed,  Timer* t);
	void MoveStart(Point dest, int time, Timer* t);
	void MoveStart(Vector speed, Timer* t);
	void MoveStop();
	bool IsMoving();
	void SetFrame(int f);
	int GetFrame();
	void SetFrameIndex(int f);
	int GetFrameIndex();
	void SetFrameSequence(FrameSequence* fs);
	void AnimationStart(Timer* t);
	void AnimationStop();
	void AddListener(EntityListener* el);
	void RemoveListener(EntityListener* el);
	Rectangle GetRect();
	void SetSprite(Sprite* s);
	void SetText(std::string txt);
	void SetFont(Font* f);
	void SetWidth(float w);
	void SetHeight(float h);
	float GetWidth();
	float GetHeight();
};

class EntityListener
{
public:
	virtual void OnMovementStart(Entity* e){}
	virtual void OnMovementStop(Entity* e){}
	virtual void OnAnimationStart(Entity* e){}
	virtual void OnAnimationStop(Entity* e){}
	virtual void OnMove(Entity* e){}
	virtual void OnAnimation(Entity* e){}
};

class Layer : public Drawable
{
protected:
	float bR, bG, bB, bA;
	Point offset;
	Point totalOffset;
	std::list<Drawable*> objects;

	friend Scene;
	virtual void Draw(float xOffset = 0, float yOffset = 0);
public:
	Layer();
	void AddDrawable(Drawable* d);
	void RemoveDrawable(Drawable* d);
	void SetOffset(Point o);
	void AddOffset(Point o);
	Point GetOffset();
	Point GetTotalOffset();
	void SetBackgroundColor(float R, float G, float B, float A);
};

class Scene
{
private:
	std::list<Layer*> layers;

	friend GameState;
	virtual void DrawScene();
public:
	void AddLayer(Layer* l);
	void RemoveLayer(Layer* l);
	void Clear();
};

class Cursor
{
protected:
	friend GameState;
	virtual void OnCursorSet() = 0;
	virtual void OnCursorStop() = 0;
	virtual void DrawCursor(int x, int y) = 0;
	virtual void DrawCursor(Point pt) = 0;
};

class DefaultCursor : public Cursor
{
private:
	virtual void OnCursorSet();
	virtual void OnCursorStop();
	virtual void DrawCursor(int x, int y){}
	virtual void DrawCursor(Point pt){}
};

ENGINE_END

#endif