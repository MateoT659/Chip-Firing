#include "Includes.h"

void setRenderColor(SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
void setRenderColor(SDL_Color color, int a) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, a);
}
void setRenderColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void clearScreen(SDL_Color color) {
	setRenderColor(color);
	SDL_RenderClear(renderer);
}

void drawPoint(int x, int y) {
	SDL_RenderDrawPoint(renderer, x, y);
}
void drawPoint(Vec2 pos) {
	SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void drawLine(Vec2 point1, Vec2 point2) {
	SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
}

void drawOpenRectangle(int x, int y, int w, int h, SDL_Color color) {
	drawOpenRectangle({ x, y, w, h }, color);
}
void drawOpenRectangle(int x, int y, int w, int h) {
	drawOpenRectangle({ x, y, w, h });
}
void drawOpenRectangle(SDL_Rect rect, SDL_Color color) {
	setRenderColor(color);
	drawOpenRectangle(rect);
}
void drawOpenRectangle(SDL_Rect rect) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}
	SDL_RenderDrawRect(renderer, &rect);
}

void drawFilledRectangle(SDL_Rect rect, SDL_Color color) {
	setRenderColor(color);
	drawFilledRectangle(rect);
}
void drawFilledRectangle(SDL_Rect rect) {

	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}

	while ((rect.w > 0 && rect.h > 0)) {
		SDL_RenderDrawRect(renderer, &rect);
		rect.x++;
		rect.y++;
		rect.h -= 2;
		rect.w -= 2;
	}
}
void drawFilledRectangle(int x, int y, int w, int h, SDL_Color color) {
	drawFilledRectangle({ x, y, w, h }, color);
}
void drawFilledRectangle(int x, int y, int w, int h) {
	drawFilledRectangle({ x, y, w, h });
}

void drawArrow(int x1, int y1, int x2, int y2, SDL_Color color) {
	setRenderColor(color);

	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

	double dx = x2 - x1;
	double dy = y2 - y1;
	double tx = x2, ty = y2;
	double mag = sqrt(dx * dx + dy * dy);
	dx /= mag; dy /= mag;

	for (int i = 10; i > 0; i--) {
		SDL_RenderDrawLine(renderer, (int)(tx - i * dx - i * dy),
			(int)(ty - i * dy + i * dx),
			(int)(tx - i * dx + i * dy),
			(int)(ty - i * dy - i * dx));
	}
}

void drawCircle(Vec2 pos, int radius) {
	drawCircle(pos.x, pos.y, radius);
}

void drawOpenCircle(Vec2 pos, int radius) {
	int x = pos.x, y = pos.y;
	int dx = radius, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x - dx, y + dy);
		drawPoint(x - dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		drawPoint(x - dx, y - dy);
		drawPoint(x - dy, y - dx);
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}
	}
}

void drawCircle(int cx, int cy, int radius) {
	
	int x = cx, y = cy;
	int dx = radius, dy = 0, prevdx = -1;

	while (dx >= dy) {
		drawLine(x + dx, y + dy, x - dx, y + dy);
		if (dy != 0)
			drawLine(x + dx, y - dy, x - dx, y - dy);

		if (prevdx != dx && dy > 0 && dy != dx) {
			drawLine(x + dy, y + dx, x - dy, y + dx);
			drawLine(x + dy, y - dx, x - dy, y - dx);
			prevdx = dx;
		}
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}

	}
}

void updateIcons() {
	icons[0]->setIcon(nodeIcons[(int)ghost->getType()]->getIcon());
	icons[1]->setIcon(edgeIcons[(int)edgeType]->getIcon());
	icons[2]->setIcon(editIcons[selectedEditTool]->getIcon());
	icons[3]->setIcon(chipIcons[selectedChipTool]->getIcon());
	icons[4]->setIcon(fireIcons[selectedFireTool]->getIcon());
	
}

SDL_Texture* loadTexture(std::string filepath) {
	//returns nullptr on error;
	SDL_Texture* texture;
	SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());

	if (loadedSurface == NULL)
	{
		std::cout << "Error: Couldn't load image \"" << filepath  << "\"!\n";
		return nullptr;
	}

	//Create texture from surface pixels
	texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (texture == NULL)
	{
		std::cout << "Error: Couldn't make texture from image!\n";
		return nullptr;
	}

	SDL_FreeSurface(loadedSurface);
	return texture;
}

void render(bool showGhost) {
	renderObjects(showGhost);

	renderInterface();
}

void renderObjects(bool showGhost) {
	clearScreen(bgColor);


	for (int i = 0; i < edges.size(); i++) {
		edges[i]->render();
	}


	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->render();
	}



	if (selectedInd <=1 && showGhost)
		ghost->render();

}

void updateUIOnMove() {
	sidebar->setSize(sidebar->getWidth(), SCREEN_HEIGHT);

	icons[icons.size()-4]->setPos(0, SCREEN_HEIGHT - 70 - 63 * 3);
	icons[icons.size()-3]->setPos(0, SCREEN_HEIGHT - 70 - 63 * 2);
	icons[icons.size()-2]->setPos(0, SCREEN_HEIGHT - 70 - 63);
	icons[icons.size()-1]->setPos(0, SCREEN_HEIGHT - 70);
}

void renderInterface() {
	sidebar->render();

	for (int i = 0; i < icons.size(); i++) {
		icons[i]->render();
	}
}

void renderU(bool showGhost) {
	render(showGhost);
	SDL_RenderPresent(renderer);
}

bool colorEquals(SDL_Color c1, SDL_Color c2) {
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}