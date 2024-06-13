#include "Textbox.h"

Textbox::Textbox(std::string text, Vec2 pos, int height, SDL_Color color)
{
	this->pos = pos;
	this->text = text;
	this->box.h = height;
	this->box.w = (int)((double)height * 3. / 5. * (double)text.size());
	this->box.x = pos.x - box.w/2;
	this->box.y = pos.y;
	
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

Textbox::Textbox(std::string text, int x, int y, int height, SDL_Color color)
{
	if (text == "") text = " ";
	this->pos.x = x;
	this->pos.y = y;
	this->text = text; 
	this->box.h = height;
	this->box.w = (int)((double)height * 3. / 5. * (double)text.size());
	this->box.x = pos.x - box.w / 2;
	this->box.y = y;
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


Textbox::~Textbox()
{
	SDL_DestroyTexture(texture);
	text.clear();
}

void Textbox::setText(std::string text)
{
	if (text == "") text = " ";
	if (this->text != text) {
		this->text = text;
		this->box.x = pos.x - box.w / 2;
		this->box.w = (int)((double)this->box.h * 3. / 5. * (double)text.size());
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), this->color);
		if (textSurface == NULL)
		{
			std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
			SDL_FreeSurface(textSurface);
			return;
		}
		SDL_DestroyTexture(this->texture);
		this->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
		}

		SDL_FreeSurface(textSurface);
	}
}

std::string Textbox::getText()
{
	return text;
}

void Textbox::setColor(SDL_Color color)
{
	if (!colorEquals(this->color, color)) {
		this->color = color;
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
}

SDL_Color Textbox::getColor() {
	return color;
}

void Textbox::setEditState(bool state)
{
	beingEdited = state;
}

bool Textbox::getEditState() {
	return beingEdited;
}

void Textbox::setPos(Vec2 pos)
{
	this->pos = pos;
	this->box.w = (int)((double)box.h * 3. / 5. * (double)text.size());
	this->box.x = pos.x - box.w / 2;
	this->box.y = pos.y;
}

void Textbox::setPos(int x, int y)
{
	this->pos.x = x;
	this->pos.y = y;
	box.y = pos.y;
	this->box.w = (int)((double)box.h * 3. / 5. * (double)text.size());
	this->box.x = pos.x - box.w / 2;
}

void Textbox::setCornerPos(Vec2 pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->box.x = pos.x;
	this->box.y = pos.y;
	this->box.w = (int)((double)box.h * 3. / 5. * (double)text.size());
}

Vec2 Textbox::getPos() {
	return pos;
}

void Textbox::translateBy(Vec2 pos)
{
	this->pos += pos;
	box.x += pos.x;
	box.y += pos.y;
}

void Textbox::setHeight(int height)
{
	if (height < 15) height = 15;
	if (height > 300) height = 300;
	this->box.h = height;
	this->box.w = (int)((double)height * 3. / 5. * (double)text.size());
	this->box.x = pos.x - box.w / 2;
}

int Textbox::getHeight()
{
	return box.h;
}

bool Textbox::containsPoint(Vec2 point)
{
	return rectIsTouched(box, point);
}

void Textbox::renderBoundingBox()
{
	drawFilledRectangle(box, WHITE);
	drawOpenRectangle(box, { 173, 216, 230, 255 });
}

void Textbox::render()
{
	if (beingEdited) {
		drawOpenRectangle(box, color);
	}
	SDL_RenderSetViewport(renderer, &box);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderSetViewport(renderer, nullptr);
}
