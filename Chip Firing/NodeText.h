#pragma once
#include "GraphSimConst.h"
#include "GraphNode.h"
class NodeText
{
private:
	GraphNode* node;
	std::string text;
	SDL_Texture* texture;
	SDL_Rect box;
	SDL_Color color;
	bool editing;
public:
	NodeText(GraphNode* node, std::string text, SDL_Color color);
	~NodeText();

	GraphNode* getNode();

	bool isTouched(Vec2 pos);
	void setText(std::string text);
	std::string getText();

	void setState(bool state);
	bool getState();

	void updatePos();
	void setColor(SDL_Color color);
	void render();
};