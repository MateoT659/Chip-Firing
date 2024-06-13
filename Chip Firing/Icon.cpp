#include "Icon.h"

Icon::Icon(int x, int y, std::string icon, std::string message) {
	hovered = new Image(x, y, 61, 61, hoveredTexture);
	selected = new Image(x, y, 61, 61, selectedTexture);
	this->icon = new Image(x + 12, y + 12, 37, 37, icon);
	select = new bool(false);
	hover = false;
	this->hoverMessage = message;
}

void Icon::setIcon(SDL_Texture *texture)
{
	icon->setImage(texture);
}

std::string Icon::getMessage()
{
	return hoverMessage;
}

SDL_Texture* Icon::getIcon()
{
	return icon->getTexture();
}

bool Icon::containsPoint(Vec2 pos)
{
	Vec2 thispos = hovered->getPos();
	return (pos.x >= thispos.x+8) && (pos.x <= thispos.x + 53) && (pos.y >= thispos.y + 8) && (pos.y <= thispos.y + 53);
}

bool* Icon::getSelectedAddress()
{
	return select;
}

bool Icon::isHovered()
{
	return hover;
}

bool Icon::isSelected()
{
	return *select;
}

void Icon::setHover(bool value)
{
	hover = value;
}

void Icon::setSelected(bool value)
{
	*select = value;
}

void Icon::toggleSelected()
{
	*select = !*select;
}

void Icon::render()
{
	if (*select) {
		selected->render();
	}
	else if (hover) {
		hovered->render();
	}


	icon->render();
}

void Icon::setPos(int x, int y) {
	hovered->setPos(x, y);
	selected->setPos(x, y);
	icon->setPos(x+12, y+12);
}

void Icon::linkTo(Icon *other)
{
	select = other->getSelectedAddress();
}


