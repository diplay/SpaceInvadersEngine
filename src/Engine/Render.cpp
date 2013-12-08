#include "engine.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <cstdio>
#pragma comment(lib, "glew32")

ENGINE_BEGIN

bool adaptResolution;
int screenWidth, screenHeight, window, frames;
GLuint texShaders, colorRectShaders;
GLuint texVAO;
GLuint targetLoc, srcLoc, texLoc, colLoc, aspLoc;
GLuint curTex = 0;

Render* Render::GetInstance()
{
	static Render r;
	return &r;
}

void Render::SetBackgroundColor(float R, float G, float B, float A)
{
	glClearColor(R, G, B, A);
}

void RenderFrame()
{
	static Render* r = Render::GetInstance();
	r->RenderFrame();
}

void Render::RenderFrame()
{
	static Game* g = Game::GetInstance();
	glClear(GL_COLOR_BUFFER_BIT);
	if(g->curState)
		g->curState->Draw();
	glutSwapBuffers();
	if(g->nextState)
		g->CheckState();
	glutPostRedisplay();
	frames++;
}

void Reshape(int w, int h)
{
	if(adaptResolution)
	{
		glViewport(0, 0, w, h);
		screenWidth = w;
		screenHeight = h;
		glUniform1f(aspLoc, (float)screenHeight / screenWidth);
	}
}

void FPSTimerFunc(int id)
{
	static Game* g = Game::GetInstance();
	g->fps = frames;
	glutTimerFunc(1000, FPSTimerFunc, id);
	frames = 0;
}

GLuint MakeAProgram(const GLchar* vshader, const GLchar* fshader)
{
	GLuint vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vshader, NULL);
	glShaderSource(fs, 1, &fshader, NULL);

	char err[1000];
	int l;
	glCompileShader(vs);
	glGetShaderInfoLog(vs, 1000, &l, err);
	printf("%s\n", err);
	glCompileShader(fs);
	glGetShaderInfoLog(vs, 1000, &l, err);
	printf("%s\n", err);

	GLuint result = glCreateProgram();
	glAttachShader(result, vs);
	glAttachShader(result, fs);
	glLinkProgram(result);
	glGetProgramInfoLog(result, 1000, &l, err);
	printf("%s\n", err);
	return result;
}

void InitShaders()
{
	const GLchar* vshader = "#version 130\n" \
		"in vec2 position;\n" \
		"uniform vec4 target;\n" \
		"uniform float aspect;\n" \
		"out vec2 texCoord;\n" \
		"void main(){\n" \
		"vec2 pos = position;\n" \
		"texCoord = position;\n" \
		"pos.x *= target.z * aspect;\n" \
		"pos.y *= target.w;\n" \
		"pos.x += (target.x * aspect);\n" \
		"pos.y += target.y;\n" \
		"gl_Position = vec4(pos, 0.0f, 1.0f);\n" \
		"}";
	const GLchar* fshader = "#version 130\n" \
		"in vec2 texCoord;\n" \
		"uniform sampler2D tex;\n" \
		"uniform vec4 src;\n" \
		"out vec4 color;\n" \
		"void main(){\n" \
		"vec2 tc = texCoord;\n" \
		"tc.x *= src.z;\n" \
		"tc.y *= src.w;\n" \
		"tc.x += src.x;\n" \
		"tc.y += src.y;\n" \
		"color = texture(tex, tc);\n" \
		"}";

	texShaders = MakeAProgram(vshader, fshader);
	glUseProgram(texShaders);

	texLoc = glGetUniformLocation(texShaders, "tex");
	printf("texLoc = %d\n", texLoc);
	targetLoc = glGetUniformLocation(texShaders, "target");
	printf("targetLoc = %d\n", targetLoc);
	srcLoc = glGetUniformLocation(texShaders, "src");
	printf("srcLoc = %d\n", srcLoc);
	glUniform1i(texLoc, 0);
	aspLoc = glGetUniformLocation(texShaders, "aspect");
	printf("aspLoc = %d\n", aspLoc);
	glUniform1f(aspLoc, (float)screenHeight / screenWidth);
	printf("aspect = %f\n", (float)screenHeight / screenWidth);

	vshader = "#version 130\n" \
		"in vec2 position;\n" \
		"uniform vec4 target;\n" \
		"void main(){\n" \
		"vec2 pos = position;\n" \
		"pos.x *= target.z;\n" \
		"pos.y *= target.w;\n" \
		"pos.x += target.x;\n" \
		"pos.y += target.y;\n" \
		"gl_Position = vec4(pos, 0.0f, 1.0f);\n" \
		"}";

	fshader = "#version 130\n" \
		"uniform vec4 col;\n" \
		"out vec4 color;\n" \
		"void main(){\n" \
		"color = col;\n" \
		"}";

	colorRectShaders = MakeAProgram(vshader, fshader);
	glUseProgram(colorRectShaders);

	colLoc = glGetUniformLocation(colorRectShaders, "col");
	printf("colLoc = %d\n", colLoc);

	glUseProgram(texShaders);
}

void InitVAO()
{
	glGenVertexArrays(1, &texVAO);
	glBindVertexArray(texVAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLfloat verts[] = {0.0f, 0.0f,
						1.0f, 0.0f,
						0.0f, 1.0f,
						1.0f, 0.0f,
						1.0f, 1.0f,
						0.0f, 1.0f};
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	GLuint posloc;
	posloc = glGetAttribLocation(texShaders, "position");
	glEnableVertexAttribArray(posloc);
	glVertexAttribPointer(posloc, 2, GL_FLOAT, GL_FALSE, 0, (void*)NULL);
}

void Render::FullScreenWindow()
{
	if(window)
		glutFullScreenToggle();
}

void Render::InitGraphics(InitParameters ip)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	screenWidth = ip.width;
	screenHeight = ip.height;
	adaptResolution = ip.adaptResolution;
	if(ip.windowed)
	{
		glutInitWindowSize(screenWidth, screenHeight);
		window = glutCreateWindow(ip.windowCaption.c_str());
	}
	else
	{
		char gms[256];
		sprintf(gms, "%dx%d:%d@60", ip.width, ip.height, 32);
		glutGameModeString(gms);
		glutEnterGameMode();
		window = 0;
	}

	int check;
	if((check = glewInit()) != GLEW_OK)
	{
		printf("Cannot init GLEW: %s\n", (const char*)glewGetErrorString(check));
		exit(1);
	}

	glutDisplayFunc(Engine::RenderFrame);
	glutReshapeFunc(Reshape);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	InitShaders();
	printf("%d\n", glGetError());
	InitVAO();
	printf("%d\n", glGetError());
	glutTimerFunc(1000, FPSTimerFunc, -1);
	glViewport(0, 0, screenWidth, screenHeight);

	defaultParams = TEX_LINEAR_FILTER;
#ifdef WIN32
	typedef BOOL (APIENTRY * wglSwapIntervalEXT_Func)(int);
	wglSwapIntervalEXT_Func wglSwapIntervalEXT = wglSwapIntervalEXT_Func(wglGetProcAddress("wglSwapIntervalEXT"));
	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(ip.vsync);
#else
	//TODO
#endif
}

void Render::RenderTexture(Texture* tex, float x, float y, float targetWidth, float targetHeight, int xOffset, int yOffset, int srcWidth, int srcHeight)
{
	if(curTex != tex->data)
	{
		glBindTexture(GL_TEXTURE_2D, tex->data);
		curTex = tex->data;
	}
	glUniform4f(targetLoc, x, y, targetWidth, targetHeight);
	glUniform4f(srcLoc, (float)xOffset / tex->w, (float)yOffset / tex->h, (float)srcWidth / tex->w, (float)srcHeight / tex->h);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render::RenderTexture(Texture* tex, float x, float y, float targetWidth, float targetHeight, float xOffset, float yOffset, float srcWidth, float srcHeight)
{
	if(curTex != tex->data)
	{
		glBindTexture(GL_TEXTURE_2D, tex->data);
		curTex = tex->data;
	}
	glUniform4f(targetLoc, x, y, targetWidth, targetHeight);
	glUniform4f(srcLoc, xOffset, yOffset, srcWidth, srcHeight);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render::RenderColorRect(float R, float G, float B, float A, Rectangle rect)
{
	glUseProgram(colorRectShaders);
	glUniform4f(colLoc, R, G, B, A);
	glUniform4f(targetLoc, rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(texShaders);
}

int Render::GetScreenWidth()
{
	return screenWidth;
}

int Render::GetScreenHeight()
{
	return screenHeight;
}

TextureParameters Render::GetDefaultTextureParameters()
{
	return defaultParams;
}

ENGINE_END
