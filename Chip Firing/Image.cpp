#include "Image.h"

Image::Image(int x, int y, int width, int height, std::string filename)
{
	

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;


	picture = loadTexture(filename);
}

Image::Image(int x, int y, int width, int height, SDL_Texture* texture) {

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	picture = texture;
}

SDL_Texture* Image::getTexture()
{
	return picture;
}

Image::Image(Vec2 pos, int width, int height, std::string filename)
{
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = width;
	rect.h = height;


	picture = loadTexture(filename);
}

void Image::setPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Image::setPos(Vec2 pos)
{
	rect.x = pos.x;
	rect.y = pos.y;
}

Vec2 Image::getPos()
{
	return Vec2(rect.x, rect.y);
}

void Image::setSize(int width, int height)
{
	rect.w = width;
	rect.h = height;
}

int Image::getWidth()
{
	return rect.w;
}

int Image::getHeight()
{
	return rect.h;
}

void Image::setImage(SDL_Texture *texture)
{
	picture = texture;
}

void Image::render()
{
	SDL_RenderSetViewport(renderer, &rect);
	SDL_RenderCopy(renderer, picture, NULL, NULL);
	SDL_RenderSetViewport(renderer, nullptr);

}

bool Image::isTouched(Vec2 pos)
{
	return (pos.x >= rect.x) && (pos.x <= rect.x + rect.w) && (pos.y >= rect.y) && (pos.y <= rect.y + rect.h);
}
