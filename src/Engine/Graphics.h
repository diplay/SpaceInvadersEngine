#include <string>
#include "class_declarations.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

ENGINE_BEGIN

class Render
{
private:
	Render(){};
	Render(const Render& r);
	Render operator =(const Render& r);
	TextureParameters defaultParams;
public:
	static Render* GetInstance();
	void RenderFrame();
	void SetBackgroundColor(float R, float G, float B, float A);
	void InitGraphics(InitParameters ip);
	void RenderTexture(Texture* tex, float x, float y, float targetWidth, float targetHeight, int xOffset, int yOffset, int srcWidth, int srcHeight);
	void RenderTexture(Texture* tex, float x, float y, float targetWidth, float targetHeight, float xOffset, float yOffset, float srcWidth, float srcHeight);
	int GetScreenWidth();
	int GetScreenHeight();
	void FullScreenWindow();
	void RenderColorRect(float R, float G, float B, float A, Rectangle r);
	TextureParameters GetDefaultTextureParameters();
};

ENGINE_END

#endif