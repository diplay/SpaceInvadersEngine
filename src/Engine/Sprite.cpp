#include "engine.h"
#include <fstream>

ENGINE_BEGIN

Sprite::Sprite(std::string filename) : Resource(RESOURCE_SPRITE, filename)
{
	std::ifstream f(filename.c_str());
	std::string texturefile;
	std::list<Frame> fr;
	float texW, texH;
	float x, y, width, height;
	int texParams;
	f >> texturefile >> texParams;
	tex = (Texture*)(ResourceManager::GetInstance()->FindResourceByFile(texturefile));
	if(tex == NULL)
	{
		tex = new Texture(texturefile, (TextureParameters)texParams);
		ResourceManager::GetInstance()->AddResource(texturefile, tex);
	}
	texW = tex->GetWidth();
	texH = tex->GetHeight();
	while(!f.eof())
	{
		f >> x >> y >> width >> height;
		fr.push_back(Frame(x / texW, y / texH, width / texW, height / texH));
	}
	f.close();
	framesCnt = fr.size();
	frames = new Frame[framesCnt];
	std::list<Frame>::iterator i;
	int j;
	for(i = fr.begin() , j = 0; i != fr.end(); i++, j++)
		frames[j] = *i;
}

Sprite::Sprite(Texture* t, int width, int height) : Resource(RESOURCE_SPRITE, "")
{
	tex = t;
	int framesInARow = tex->GetWidth() / width;
	int rows = tex->GetHeight() / height;
	framesCnt = rows*framesInARow;
	frames = new Frame[rows*framesInARow];
	for(int j = 0; j < rows; j++)
		for(int i = 0; i < framesInARow; i++)
			frames[i + j*framesInARow] = Frame((float)(i*width) / tex->GetWidth(), (float)(j*height) / tex->GetHeight(),
												(float)width / tex->GetWidth(), (float)height / tex->GetHeight());
}

void Sprite::Draw(float x, float y, float width, float height, int frame)
{
	static Render* r = Render::GetInstance();
	r->RenderTexture(tex, x, y, width, height, frames[frame].x, frames[frame].y, frames[frame].width, frames[frame].height);
}

int Sprite::GetFramesCount()
{
	return framesCnt;
}

Sprite::~Sprite()
{
	delete[] frames;
}

ENGINE_END