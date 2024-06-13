#include "GraphNode.h"

//radius == 15

GraphNode::GraphNode(int x, int y, NodeType type) {
	this->pos.set(x, y);
	this->chips = 0;
	this->type = type;
	ghost = false;
	std::string str = "q";
	if (type == Node) {
		str = "0";
	}
	text = new NodeText(this,str, WHITE);
	degree = 0;
}

GraphNode::GraphNode(int x, int y, int chips, NodeType type)
{
	this->pos.set(x, y);
	this->chips = chips;
	this->type = type;
	ghost = false;
	std::string str = "q";
	if (type == Node) {
		str = std::to_string(chips);
	}
	text = new NodeText(this, str, WHITE);
	degree = 0;
	
}

GraphNode::GraphNode(Vec2 pos, NodeType type)
{
	this->pos = pos;
	this->type = type;
	this->chips = 0;
	this->ghost = false;
	this->text = new NodeText(this, type == Node ? "0" : "q", type == Node? WHITE: BLACK);
	this->degree = 0;
}
GraphNode::~GraphNode()
{
	delete text;
}
GraphNode* GraphNode::copy()
{
	return new GraphNode(pos, type);
}

NodeText* GraphNode::getText()
{
	return text;
}

void GraphNode::updateTextColor()
{
	SDL_Color textcolor = BLACK;
	if (type == Node) {
		if (chips >= degree) {
			textcolor = GREEN;
		}
		else if (chips >= 0) {
			textcolor = WHITE;
		}
		else {
			textcolor = RED;
		}
	}

	text->setColor(textcolor);
}


void GraphNode::setPos(int x, int y) {
	pos.set(x, y);
	text->updatePos();
}

void GraphNode::setPos(Vec2 pos)
{
	this->pos = pos;
	text->updatePos();
}

void GraphNode::translateBy(Vec2 vec)
{
	pos += vec;
	text->updatePos();
}

NodeType GraphNode::getType()
{
	return type;
}

void GraphNode::setType(NodeType type)
{
	this->type = type;
	text->setText(type == Node ? "0" : "q");

	SDL_Color color = type == Node ? WHITE : BLACK;
	if (ghost) {
		color.a = 100;
	}
	text->setColor(color);

}

void GraphNode::toggleGhost()
{
	ghost = !ghost;

	SDL_Color color = type == Node ? WHITE : BLACK;
	if (ghost) {
		color.a = 100;
	}
	text->setColor(color);

}

bool GraphNode::containsPoint(Vec2 point)
{
	return (pos - point).mag2() <= RADIUS * RADIUS;
}

bool GraphNode::containsPoint(int xPos, int yPos) {

	return containsPoint(Vec2(xPos, yPos));
}

void GraphNode::fire()
{
	chips -= degree;
	GraphNode* other;
	for (GraphEdge* e : adjacent) {
		other = e->getNode1() == this ? e->getNode2() : e->getNode1();
		
		if (e->getType() == Positive) {
			other->changeChips(1);
		}
		else {
			other->changeChips(-1);
		}
	}
	if (type == Node) {
		text->setText(std::to_string(chips));

		if (chips >= degree) {
			text->setColor(GREEN);
		}
		else if (chips >= 0) {
			text->setColor(WHITE);
		}
		else {
			text->setColor(RED);
		}
	}
}

void GraphNode::inverseFire()
{
	chips += degree;
	GraphNode* other;
	for (GraphEdge* e : adjacent) {
		other = e->getNode1() == this ? e->getNode2() : e->getNode1();

		if (e->getType() == Positive) {
			other->changeChips(-1);
		}
		else {
			other->changeChips(+1);
		}
	}
	if (type == Node) {
		text->setText(std::to_string(chips));

		if (chips >= degree) {
			text->setColor(GREEN);
		}
		else if (chips >= 0) {
			text->setColor(WHITE);
		}
		else {
			text->setColor(RED);
		}
	}
}

Vec2 GraphNode::getPos()
{
	return pos;
}



void GraphNode::renderSink()
{
	//line solution (MUAH beautiful)
	setRenderColor(AMETHYST, ghost ? 100 : 255);

	int x = pos.x, y = pos.y;
	int dx = RADIUS, dy = 0, prevdx = -1;

	while (dx >= dy) {
		drawLine(x + dx, y + dy, x - dx, y + dy);
		if (dy != 0)
			drawLine(x + dx, y - dy, x - dx, y - dy);

		if (prevdx != dx && dy>0 && dy != dx) {
			drawLine(x + dy, y + dx, x - dy, y + dx);
			drawLine(x + dy, y - dx, x - dy, y - dx);
			prevdx = dx;
		}
		dy++;
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}
	text->render();
}

void GraphNode::renderNode() {
	setRenderColor(WHITE, ghost ? 100 : 255);

	int x = pos.x, y = pos.y;
	int dx = RADIUS, dy = 0;

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
		while (RADIUS * RADIUS < dx * dx + dy * dy) {
			dx--;
		}
	}

	text->render();
}

void GraphNode::render() {
	switch (type) {
	case Node:
		renderNode();
		break;
	case Sink:
		renderSink();
		break;
	}


}

void GraphNode::addEdge(GraphEdge* edge)
{
	adjacent.push_back(edge);
	degree = adjacent.size();
	if (type == Node) {
		if (chips >= degree) {
			text->setColor(GREEN);
		}
		else if (chips >= 0) {
			text->setColor(WHITE);
		}
		else {
			text->setColor(RED);
		}
	}
	return;

}

void GraphNode::removeEdge(GraphEdge* edge)
{
	for (int i = 0; i < adjacent.size();) {
		if (adjacent[i]==edge) {
			adjacent.erase(adjacent.begin() + i);
		}
		else {
			i++;
		}
	}
	degree = adjacent.size();
	if (type == Node) {
		if (chips >= degree) {
			text->setColor(GREEN);
		}
		else if (chips >= 0) {
			text->setColor(WHITE);
		}
		else {
			text->setColor(RED);
		}
	}
}

std::vector<GraphEdge*> GraphNode::getEdges()
{
	return adjacent;
}

int GraphNode::getX() {
	return pos.x;
}

int* GraphNode::getXaddr(){
	return (int*) & (pos.x);
}

int GraphNode::getY() {
	return pos.y;
}

int* GraphNode::getYaddr() {
	return (int*) &(pos.y);
}

int GraphNode::getChips()
{
	return this->chips;
}

void GraphNode::setChips(int chips)
{
	this->chips = chips;

	if (type == Node) {
		text->setText(std::to_string(chips));

		if (chips >= degree) {
			text->setColor(GREEN);
		}
		else if (chips >= 0) {
			text->setColor(WHITE);
		}
		else {
			text->setColor(RED);
		}
	}
}

void GraphNode::changeChips(int diff)
{
	this->chips += diff;

	if (type == Node) {
		text->setText(std::to_string(chips));

		if (chips >= degree) {
			text->setColor(GREEN);
		}
		else if (chips >= 0) {
			text->setColor(WHITE);
		}
		else {
			text->setColor(RED);
		}
	}
}
