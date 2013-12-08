#include "engine.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

ENGINE_BEGIN
Keyboard* keyboard = Keyboard::GetInstance();

void KeyboardCallback(unsigned char key, int x, int y)
{
	keyboard->PressKey((Keys)key, true);
}

void KeyboardCallbackUp(unsigned char key, int x, int y)
{
	keyboard->PressKey((Keys)key, false);
}

void KeyboardSpecCallback(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		keyboard->PressKey(KEY_UP, true);
		break;
	case GLUT_KEY_DOWN:
		keyboard->PressKey(KEY_DOWN, true);
		break;
	case GLUT_KEY_RIGHT:
		keyboard->PressKey(KEY_RIGHT, true);
		break;
	case GLUT_KEY_LEFT:
		keyboard->PressKey(KEY_LEFT, true);
		break;
	}
}

void KeyboardSpecCallbackUp(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		keyboard->PressKey(KEY_UP, false);
		break;
	case GLUT_KEY_DOWN:
		keyboard->PressKey(KEY_DOWN, false);
		break;
	case GLUT_KEY_RIGHT:
		keyboard->PressKey(KEY_RIGHT, false);
		break;
	case GLUT_KEY_LEFT:
		keyboard->PressKey(KEY_LEFT, false);
		break;
	}
}

Keyboard* Keyboard::GetInstance()
{
	static Keyboard k;
	return &k;
}

void Keyboard::InitKeyboard()
{
	glutKeyboardFunc(KeyboardCallback);
	glutKeyboardUpFunc(KeyboardCallbackUp);
	glutSpecialFunc(KeyboardSpecCallback);
	glutSpecialUpFunc(KeyboardSpecCallbackUp);
}

void Keyboard::PressKey(Keys key, bool state)
{
	SetKeyState(key, state);
	std::list<KeyboardListener*>::iterator i;
	if(state)
		for(i = listeners.begin(); i != listeners.end(); i++)
		{
			(*i)->OnKeyDown(NULL, key);
			if(key >= 32 && key < 127)
				(*i)->OnCharKeyDown(NULL, key);
		}
	else
		for(i = listeners.begin(); i != listeners.end(); i++)
			(*i)->OnKeyUp(NULL, key);
}

bool Keyboard::GetKeyState(Keys k)
{
	return keyStates[k];
}

void Keyboard::SetKeyState(Keys k, bool state)
{
	keyStates[k] = state;
}

void Keyboard::AddListener(KeyboardListener* kl)
{
	if(kl)
		listeners.push_back(kl);
}

void Keyboard::RemoveListener(KeyboardListener* kl)
{
	listeners.remove(kl);
}

ENGINE_END