#include "class_declarations.h"
#include <string>
#include <list>

#ifndef INPUT_H
#define INPUT_H

ENGINE_BEGIN

class Keyboard
{
private:
	bool keyStates[300];
	std::list<KeyboardListener*> listeners;
	//friend KeyboardListener;
	//
	Keyboard(){};
	Keyboard(const Keyboard& k);
	Keyboard operator =(const Keyboard& k);
public:
	void InitKeyboard();
	bool GetKeyState(Keys k);
	void SetKeyState(Keys k, bool state);
	void PressKey(Keys k, bool state);
	void AddListener(KeyboardListener* k);
	void RemoveListener(KeyboardListener* k);
	static Keyboard* GetInstance();
};

class KeyboardListener
{
public:
	virtual void OnKeyDown(Control* sender, Keys key){};
	virtual void OnKeyUp(Control* sender, Keys key){};
	virtual void OnCharKeyDown(Control* sender, Keys key){};
};

#define MOUSE_BUTTONS_COUNT 3

class Mouse
{
private:
	Mouse(){};
	Mouse(const Mouse& k);
	Mouse operator =(const Mouse& k);
	//
	Point curCoords;
	bool buttonsStates[MOUSE_BUTTONS_COUNT];
	std::list<MouseListener*> listeners;
	//
	friend void MouseMoveCallback(int x, int y);
	friend void MousePassiveMoveCallback(int x, int y);	
public:
	void InitMouse();
	Point GetCoords();
	void SetCoords(Point p);
	void SetCoords(float x, float y);
	void SetButtonState(MouseButtons b, bool state);
	bool GetButtonState(MouseButtons b);
	void PressButton(MouseButtons b, bool state);
	static Mouse* GetInstance();
	void AddListener(MouseListener* ml);
	void RemoveListener(MouseListener* ml);
};

class MouseListener
{
public:
	virtual void OnMove(Control* sender, Mouse* m){};
	virtual void OnButtonPressed(Control* sender, Mouse* m, MouseButtons b){};
	virtual void OnButtonReleased(Control* sender, Mouse* m, MouseButtons b){};
	virtual void OnDrag(Control* sender, Mouse* m){};
};

ENGINE_END

#endif
