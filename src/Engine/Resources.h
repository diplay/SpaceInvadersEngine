#include <string>
#include <map>
#include "class_declarations.h"
#include "Graphics.h"

#ifndef RESOURCES_H
#define RESOURCES_H

ENGINE_BEGIN

class ResourceManager
{
private:
	std::map<std::string, Resource*> resources;
	ResourceManager(){}
	ResourceManager(ResourceManager const & rm);
	ResourceManager operator =(ResourceManager const & rm);
public:
	void AddResource(std::string id, std::string filename, ResourceType type);
	void AddResource(std::string id, Resource* r);
	void AddResourceList(std::string filename);
	void UnloadResource(std::string id); 
	void UnloadAll();
	Resource* FindResourceByFile(std::string file);
	Resource* GetResource(std::string id);
	static ResourceManager* GetInstance();
};

class Resource
{
protected:
	ResourceType rt;
	std::string file;
public:
	Resource(ResourceType type, std::string filename);
	ResourceType GetType();
	std::string GetFileName();
	virtual ~Resource();
};

class Sound : public Resource
{
private:
	void* data;
public:
	Sound(std::string filename);
	void Play();
	int GetLength();
};

class Texture : public Resource
{
private:
	friend Render;

	int w, h, par;
	unsigned int data;

	void LoadFromFile(std::string filename);
	void LoadTGA(std::string filename);
	void Free();
public:
	Texture(std::string filename, TextureParameters params = TEX_LINEAR_FILTER);
	Texture(TextureParameters params = TEX_LINEAR_FILTER);
	virtual ~Texture();

	int GetWidth();
	int GetHeight();
	void Draw(float x, float y, float targetWidth, float targetHeight, int xOffset, int yOffset, int srcWidth, int srcHeight);
};

class Font : public Resource
{
protected:
	friend Entity;
	virtual void DrawString(std::string str, float x, float y) = 0;
public:
	Font(std::string filename);
};

struct Frame
{
	float x, y;
	float width, height;

	Frame():x(0.0f),y(0.0f),width(0.0f),height(0.0f){}
	Frame(float _x, float _y, float _w, float _h):x(_x),y(_y),width(_w),height(_h){}
};

class Sprite : public Resource
{
private:
	Texture* tex;
	Frame* frames;
	int framesCnt;
public:
	Sprite(std::string filename);
	Sprite(Texture* t, int width, int height);
	void Draw(float x, float y, float width, float height, int frame);
	int GetFramesCount();
	~Sprite();
};



class FrameSequence : Resource
{
private:
	int* frames;
	int framesCount;
public:
	FrameSequence(std::string filename);
	int GetFrame(unsigned int num);
	~FrameSequence();
};

ENGINE_END

#endif
