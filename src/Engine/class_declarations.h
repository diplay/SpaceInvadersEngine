#define ENGINE_BEGIN namespace Engine {
#define ENGINE_END }

#ifndef CLASS_DECLARATION_H

#define CLASS_DECLARATION_H

ENGINE_BEGIN

enum ResourceType
{
	RESOURCE_OTHER = 0,
	RESOURCE_SPRITE = 1,
	RESOURCE_TEXTURE = 2,
	RESOURCE_FRAME_SEQUENCE = 3,
	RESOURCE_FONT = 4,
	RESOURCE_SOUND = 5
};

enum TextureParameters
{
	TEX_DEFAULT = 0,
	TEX_LINEAR_FILTER = 1,
	TEX_NO_FILTER = 2,
	TEX_USAGE_RENDER_TARGET = 4
};

enum Keys
{
	KEY_ESC = 27,
	KEY_RIGHT = 262,
	KEY_LEFT = 263,
	KEY_DOWN = 264,
	KEY_UP = 265,
	KEY_RETURN = 13,
	KEY_DELETE = 127,
	KEY_BACKSPACE = 8
	//TODO: write all keys here
};

enum MouseButtons
{
	BUTTON_LEFT = 0,
	BUTTON_RIGHT = 1,
	BUTTON_MIDDLE = 2
};

//Resources and resource management
class ResourceManager;
class Resource;
class Texture;
class Sprite;
class FrameSequence;

//Utilities and fonts
class Font;
class BasicFont;
class CustomFont;
class Utils;
class TimerListener;
class Timer;
class TimerManager;
class Config;

struct Rectangle
{
	float left;
	float top;
	float right;
	float bottom;
};

struct Point
{
	float x;
	float y;

	Point(): x(0.0f), y(0.0f){}
	Point(float _x, float _y): x(_x), y(_y){}
};

class Vector;

//IO
class Keyboard;
class KeyboardListener;
class Mouse;
class MouseListener;

//Engine core
class Game;
class GameState;
struct InitParameters;

//Graphical Engine
class Drawable;
class Entity;
class EntityListener;
class Layer;
class Scene;
class Cursor;
class DefaultCursor;
class SpriteCursor;

//Graphical user interface
class Control;
/*
class Focusable;
class Textual;
class Label;
class TextListener;
class Image;
class Button;
class TextBox;
class Interface;
class FocusListener;
*/

//Audio Engine
class Audio;

//Graphics library
class Render;
ENGINE_END

#endif
