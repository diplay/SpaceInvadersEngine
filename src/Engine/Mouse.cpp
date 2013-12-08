#include "engine.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#pragma comment(lib, "glew32")

ENGINE_BEGIN

Mouse* mouse = Mouse::GetInstance();
int width, height;

Mouse* Mouse::GetInstance()
{
	static Mouse m;
	return &m;
}

void MouseCallback(int button, int state, int x, int y)
{
	mouse->SetCoords(((float)x * 2.0f / width) - 1.0f, (- (float)y * 2.0f / height) + 1.0f);
	mouse->PressButton((MouseButtons)button, (state + 1) % 2);
}

void MouseMoveCallback(int x, int y)
{
	mouse->SetCoords(((float)x * 2.0f / width) - 1.0f, (- (float)y * 2.0f / height) + 1.0f);
	std::list<MouseListener*>::iterator i;
	for(i = mouse->listeners.begin(); i != mouse->listeners.end(); i++)
		(*i)->OnDrag(NULL, mouse);
}

void MousePassiveMoveCallback(int x, int y)
{
	mouse->SetCoords(((float)x * 2.0f / width) - 1.0f, (- (float)y * 2.0f / height) + 1.0f);
	std::list<MouseListener*>::iterator i;
	for(i = mouse->listeners.begin(); i != mouse->listeners.end(); i++)
		(*i)->OnMove(NULL, mouse);
}

void Mouse::InitMouse()
{
	glutMouseFunc(MouseCallback);
	glutMotionFunc(MouseMoveCallback);
	glutPassiveMotionFunc(MousePassiveMoveCallback);
	width = Render::GetInstance()->GetScreenWidth();
	height = Render::GetInstance()->GetScreenHeight();
}

void Mouse::PressButton(MouseButtons b, bool state)
{
	SetButtonState(b, state);
	std::list<MouseListener*>::iterator i;
	if(state)
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnButtonPressed(NULL, this, b);
	else
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnButtonReleased(NULL, this, b);
}

Point Mouse::GetCoords()
{
	return curCoords;
}

void Mouse::SetCoords(Point p)
{
	curCoords = p;
}

void Mouse::SetCoords(float x, float y)
{
	curCoords.x = x;
	curCoords.y = y;
}

void Mouse::SetButtonState(MouseButtons b, bool state)
{
	buttonsStates[b] = state;
}

bool Mouse::GetButtonState(MouseButtons b)
{
	return buttonsStates[b];
}

void Mouse::AddListener(MouseListener* ml)
{
	if(ml)
		listeners.push_back(ml);
}

void Mouse::RemoveListener(MouseListener* ml)
{
	listeners.remove(ml);
}

ENGINE_END