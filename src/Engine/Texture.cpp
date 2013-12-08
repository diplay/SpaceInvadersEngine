#include "engine.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <exception>

ENGINE_BEGIN

Texture::Texture(std::string filename, TextureParameters params) : Resource(RESOURCE_TEXTURE, filename)
{
	par = (int)params;
	LoadFromFile(filename);
}

Texture::Texture(TextureParameters params) : Resource(RESOURCE_TEXTURE, "")
{
	data = 0;
	par = (int)params;
}

struct TGAHeader
{
    GLubyte idLength;
    GLubyte colorMapType;
    GLubyte imgType;
    GLubyte colorMapDesc[5];
    GLushort xPos;
    GLushort yPos;
    GLushort width;
    GLushort height;
    GLubyte bpp;
    GLubyte imgDesc;
};

struct RGBAPixelub
{
    GLubyte R, G, B, A;
};

int Texture::GetWidth()
{
	return w;
}

int Texture::GetHeight()
{
	return h;
}

void Texture::LoadFromFile(std::string filename)
{
	int lastpoint = filename.find_last_of('.');
	std::string ext = filename.substr(lastpoint);
	if(ext.compare(".tga") == 0)
	{
		LoadTGA(filename);
	}
}

void Texture::LoadTGA(std::string filename)
{
	FILE* fTGA;
    fTGA = fopen(filename.c_str(), "rb");
    TGAHeader head = {0};
    fread(&head, sizeof(TGAHeader), 1, fTGA);
    if (head.colorMapType)
    {
        fclose(fTGA);
        throw std::exception();
    }
    w = head.width;
    h = head.height;
	GLubyte* data = NULL;
    if (head.imgType == 2)
    {
        data = new GLubyte[head.width*head.height*4];
        RGBAPixelub* dt = (RGBAPixelub*)data;
        fseek(fTGA, head.idLength, SEEK_CUR);
        for (int i = 0; i < head.width*head.height; i++)
        {
            fread(&dt[i].B, 1, 1, fTGA);
            fread(&dt[i].G, 1, 1, fTGA);
            fread(&dt[i].R, 1, 1, fTGA);
            fread(&dt[i].A, 1, 1, fTGA);
        }
    }
    fclose(fTGA);

	if(!data)
		throw std::exception();

	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	if((par & (int)TEX_NO_FILTER) || ((par & (int)TEX_DEFAULT) && (Render::GetInstance()->GetDefaultTextureParameters() & (int)TEX_NO_FILTER)))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	delete[] data;
	this->data = tex;
}

void Texture::Draw(float x, float y, float targetWidth, float targetHeight, int xOffset, int yOffset, int srcWidth, int srcHeight)
{
	static Render* r = Render::GetInstance();
	r->RenderTexture(this, x, y, targetWidth, targetHeight, xOffset, yOffset, srcWidth, srcHeight);
}

void Texture::Free()
{
	if(data)
	{
		glDeleteTextures(1, (const GLuint*)&data);
		data = 0;
	}
}

Texture::~Texture()
{
	Free();
}

ENGINE_END
