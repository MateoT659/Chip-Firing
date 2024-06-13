#include "NodeText.h"

NodeText::NodeText(GraphNode* node, std::string text, SDL_Color color)
{
	this->node = node;
	this->editing = false;

	if (text == "") text = " ";
	if (text.size() > 2) text.resize(2);
	this->text = text;
	if (text.size() == 1) {
		box.h = 10 * RADIUS / sqrt34;
		box.w = 6 * RADIUS / sqrt34;
	}
	else {
		box.h = 10 * RADIUS / sqrt61;
		box.w = 12 * RADIUS / sqrt61;
	}
	box.x = node->getX() - box.w / 2;
	box.y = node->getY() - box.h / 2;

	this->color = color;
	this->texture = NULL;


	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);

	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
		SDL_FreeSurface(textSurface);
		return;
	}
	this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture == NULL)
	{
		std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	}
	SDL_FreeSurface(textSurface);
}

NodeText::~NodeText()
{
	SDL_DestroyTexture(this->texture);
}

GraphNode* NodeText::getNode()
{
	return node;
}

bool NodeText::isTouched(Vec2 pos)
{
	return node->containsPoint(pos);
}

void NodeText::setText(std::string text)
{
	if (text == "") text = " ";
	if (text.size() > 2) text.resize(2);
	this->text = text;
	if (text.size() == 1) {
		box.h = 10 * RADIUS / sqrt34;
		box.w = 6 * RADIUS / sqrt34;
	}
	else {
		box.h = 10 * RADIUS / sqrt61;
		box.w = 12 * RADIUS / sqrt61;
	}
	box.x = node->getX() - box.w / 2;
	box.y = node->getY() - box.h / 2;

	SDL_DestroyTexture(this->texture);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
		SDL_FreeSurface(textSurface);
		return;
	}
	this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture == NULL)
	{
		std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	}
	SDL_FreeSurface(textSurface);
}

std::string NodeText::getText()
{
	return text;
}

void NodeText::setState(bool state)
{
	this->editing = state;
}

bool NodeText::getState()
{
	return editing;
}

void NodeText::updatePos()
{
	box.x = node->getX() - box.w / 2;
	box.y = node->getY() - box.h / 2;
}

void NodeText::setColor(SDL_Color color)
{
	this->color = color;

	SDL_DestroyTexture(this->texture);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
		SDL_FreeSurface(textSurface);
		return;
	}
	this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture == NULL)
	{
		std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	}
	SDL_FreeSurface(textSurface);
}

void NodeText::render()
{
	SDL_RenderSetViewport(renderer, &box);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderSetViewport(renderer, nullptr);
}
