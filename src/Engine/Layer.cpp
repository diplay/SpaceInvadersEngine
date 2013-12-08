#include "engine.h"

ENGINE_BEGIN

Layer::Layer()
{
	SetBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Layer::AddDrawable(Drawable* d)
{
	if(d)
		objects.push_back(d);
}

Point Layer::GetOffset()
{
	return offset;
}

void Layer::SetOffset(Point o)
{
	offset = o;
}

void Layer::Draw(float xOffset, float yOffset)
{
	totalOffset.x = xOffset + offset.x;
	totalOffset.y = yOffset + offset.y;
	static Render* r = Render::GetInstance();
	static Rectangle scrRect = {-1.0f, -1.0f, 1.0f, 1.0f};
	if(bB != 0 || bR != 0 || bG != 0 || bA != 0)
		r->RenderColorRect(bR, bG, bB, bA, scrRect);
	std::list<Drawable*>::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
		(*i)->Draw(totalOffset.x, totalOffset.y);
}

void Layer::RemoveDrawable(Drawable* d)
{
	objects.remove(d);
}

void Layer::AddOffset(Point o)
{
	offset.x += o.x;
	offset.y += o.y;
}

Point Layer::GetTotalOffset()
{
	return totalOffset;
}

void Layer::SetBackgroundColor(float R, float G, float B, float A)
{
	bR = R;
	bG = G;
	bB = B;
	bA = A;
}

ENGINE_END