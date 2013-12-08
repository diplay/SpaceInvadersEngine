#include "engine.h"
#include "GL/freeglut.h"

ENGINE_BEGIN

void DefaultCursor::OnCursorSet()
{
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
}

void DefaultCursor::OnCursorStop()
{
	glutSetCursor(GLUT_CURSOR_NONE);
}

ENGINE_END