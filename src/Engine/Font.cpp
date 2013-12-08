#include "engine.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#pragma comment(lib, "glew32")

ENGINE_BEGIN

Font::Font(std::string filename) : Resource(RESOURCE_FONT, filename)
{
}

BasicFont::BasicFont() : Font("")
{
	font = GLUT_BITMAP_TIMES_ROMAN_10;
}

BasicFont::BasicFont(BasicFontType t) : Font("")
{
	switch(t)
	{
	case FONT_HELVETICA_10:
		font = GLUT_BITMAP_HELVETICA_10;
		break;
	case FONT_HELVETICA_12:
		font = GLUT_BITMAP_HELVETICA_12;
		break;
	case FONT_HELVETICA_18:
		font = GLUT_BITMAP_HELVETICA_18;
		break;
	case FONT_TIMES_NEW_ROMAN_10:
		font = GLUT_BITMAP_TIMES_ROMAN_10;
		break;
	case FONT_TIMES_NEW_ROMAN_24:
		font = GLUT_BITMAP_TIMES_ROMAN_24;
		break;
	}
}

void BasicFont::DrawString(std::string str, float x, float y)
{
	static Render* r = Render::GetInstance();
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)str.c_str());
}

ENGINE_END