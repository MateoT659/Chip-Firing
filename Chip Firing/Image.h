#pragma once
#include "GraphSimConst.h"
class Image
{
	//wrapper for textures and rects, will include rect and will include texture. will have render function to determine rendering.
private:
	SDL_Texture* picture;
	SDL_Rect rect;

public:
	Image(int x, int y, int width, int height, std::string filename);
	Image(Vec2 pos, int width, int height, std::string filename);
	Image(int x, int y, int width, int height, SDL_Texture *texture);

	SDL_Texture* getTexture();

	void setPos(int x, int y);
	void setPos(Vec2 pos);
	Vec2 getPos();

	void setSize(int width, int height);
	int getWidth();
	int getHeight();

	void setImage(SDL_Texture *texture);

	void render();
	bool isTouched(Vec2 pos);


};

