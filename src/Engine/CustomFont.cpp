#include "engine.h"
#include <cstdio>

ENGINE_BEGIN

CustomFont::CustomFont(std::string filename) : Font(filename)
{
	//KOSTIL
	FILE* f = fopen(filename.c_str(), "r");
	char buf[256];
	fgets(buf, 255, f);
	int fntH, bs,  trash;
	fscanf(f, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d packed=%d alphaChnl=%d redChnl=%d greenChnl=%d blueChnl=%d", &fntH, 
		&bs, &trash, &trash, & trash, &trash, &trash, &trash, &trash, &trash);
	fscanf(f, "%s %s %s", buf, buf, buf);
	char* c = &buf[6];
	while(*c)
	{
		if(*c == '"')
			*c = '\0';
		c++;
	}
	base = (float)bs;
	height = (float)fntH;
	pxSize = 0.01f;
	tex = (Texture*)(ResourceManager::GetInstance()->FindResourceByFile(&buf[6]));
	if(tex == NULL)
	{
		tex = new Texture(&buf[6], TEX_DEFAULT);
		ResourceManager::GetInstance()->AddResource(&buf[6], tex);
	}
	fscanf(f, "\nchars count=%d\n", &trash);
	int id, x, y, w, h, xo, yo, xa, page, chnl;
	for(int i = 0; i < trash; i++)
	{
		fscanf(f, "char id=%d   x=%d   y=%d    width=%d     height=%d     xoffset=%d    yoffset=%d    xadvance=%d     page=%d  chnl=%d\n",
			&id, &x, &y, &w, &h, &xo, &yo, &xa, &page, &chnl);
		if(id <= 255)
			glyphs[id] = Glyph(x, y, w, h, xo, yo, xa);
	}
	fclose(f);
}

void CustomFont::DrawString(std::string str, float x, float y)
{
	static Render* r = Render::GetInstance();
	float _x = x;
	float _y = y;
	for(const char* i = str.c_str(); *i != 0; i++)
	{
		r->RenderTexture(tex, _x + glyphs[*i].xOffset * pxSize, _y + (height - glyphs[*i].height - glyphs[*i].yOffset)* pxSize,
			(float)glyphs[*i].width * pxSize, (float)glyphs[*i].height * pxSize, 
			glyphs[*i].x, glyphs[*i].y + glyphs[*i].height, glyphs[*i].width, - glyphs[*i].height);
		if(*i == '\n')
		{
			_x = x;
			_y -= height * pxSize;
		}
		else
		{
			_x += glyphs[*i].xAdvance * pxSize;
		}
	}
}

void CustomFont::SetPixelSize(float s)
{
	pxSize = s;
}

CustomFont::~CustomFont()
{
	delete tex;
}

ENGINE_END