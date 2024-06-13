#include "GraphSimConst.h"
#pragma once
class Textbox
{
private:
	Vec2 pos;
	std::string text;
	SDL_Texture* texture;
	SDL_Rect box; //height encoded here
	SDL_Color color;
	bool beingEdited;
public:
	Textbox(std::string text, Vec2 pos, int height, SDL_Color color);
	Textbox(std::string text, int x, int y, int height, SDL_Color color);
	~Textbox();

	void setText(std::string text);
	std::string getText();

	void setColor(SDL_Color color);
	SDL_Color getColor();

	void setEditState(bool state);
	bool getEditState();

	void setPos(Vec2 pos);
	void setPos(int x, int y);
	void setCornerPos(Vec2 pos);
	Vec2 getPos();

	void translateBy(Vec2 pos);

	void setHeight(int height);
	int getHeight();

	bool containsPoint(Vec2 point);
	void renderBoundingBox();
	void render();
};

