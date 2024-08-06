#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* node1, GraphNode* node2, SDL_Color color, EdgeType type) {
	this->node1 = node1;
	this->node2 = node2;
	this->color = color;
	this->type = type;

	isSwitched = false;
	
	Vec2 line = node2->getPos() - node1->getPos();

	double mag = line.mag();

	this->unitX = line.x / mag;
	this->unitY = line.y / mag;

	double fromX = node1->getX();
	double fromY = node1->getY();
	fromX += unitX * RADIUS;
	fromY += unitY * RADIUS;
	this->from.x = (int)fromX;
	this->from.y = (int)fromY;

	double toX = node2->getX();
	double toY = node2->getY();
	toX -= unitX * RADIUS;
	toY -= unitY * RADIUS;
	this->to.x = (int)toX;
	this->to.y = (int)toY;

	//prevents division by zero

	ymin = min(node1->getY(), node2->getY());
	ymax = max(node1->getY(), node2->getY());
	xmin = min(node1->getX(), node2->getX());
	xmax = max(node1->getX(), node2->getX());

	slope = (double)(to.y - from.y) / (to.x - from.x + (to.x == from.x ? 1 : 0));

	b = (int) (from.y - slope * from.x);
}

void GraphEdge::toggleSwitch()
{
	isSwitched = !isSwitched;
}

void GraphEdge::render() {

	//calculate to and from vectors in update and constructor, then use them in specific renders
	switch (type) {
	case Positive:
		setRenderColor(WHITE);
		renderPos();
		break;
	case Negative:
		setRenderColor(RED);
		renderNeg();
		break;
	}
}

void GraphEdge::renderNeg()
{
	Vec2 v((int)(unitX*5), (int)(unitY*5));
	//adjust to change size;

	Vec2 line = (to - from)/2 - v*4;
	drawLine(from, from + line);
	drawLine(to, to - line);
	Vec2 sqStart = from + line;

	Vec2 perp(-(int)(unitY*10), (int)(unitX*10));
	drawLine(sqStart, sqStart + perp + v);
	drawLine(sqStart + perp + v, sqStart - perp + v * 3);
	drawLine(sqStart - perp + v * 3, sqStart + perp + v*5);
	drawLine(sqStart + perp + v * 5, sqStart - perp + v * 7);
	drawLine(sqStart - perp + v * 7, to - line);
}

void GraphEdge::renderPos()
{
	drawLine(from, to);
}

bool GraphEdge::isTouched(int x, int y) {
	
	if (slope > 0) {
		return  ((y >= ymin - 10 && y <= ymax + 10) && (x >= xmin - 10 && x <= xmax + 10)) && ((y <= slope * (x + 12) + b + 12 && y >= slope * (x - 12) + b - 12));
	}
	else {
		return  ((y >= ymin - 10 && y <= ymax + 10) && (x >= xmin - 10 && x <= xmax + 10)) && ((y <= slope * (x - 12) + b + 12 && y >= slope * (x + 12) + b - 12));
	}
}

bool GraphEdge::isTouched(Vec2 pos) {
	return isTouched(pos.x, pos.y);
}

void GraphEdge::invertSign()
{
	this->type = (EdgeType)(1 - (int)type);
}

GraphNode* GraphEdge::getNode1() {
	return node1;
}

GraphNode* GraphEdge::getNode2() {
	return node2;
}
SDL_Color GraphEdge::getColor() {
	return color;
}
void GraphEdge::setColor(SDL_Color color) {
	this->color = color;
}

EdgeType GraphEdge::getType()
{
	return type;
}

bool GraphEdge::containsNode(GraphNode* node) {
	return node1 == node || node2 == node;
}

bool GraphEdge::isFrom(GraphNode* node)
{
	return node == node1;
}

bool GraphEdge::isTo(GraphNode* node) {
	return node == node2;
}


bool GraphEdge::isSwitchedOn()
{
	return isSwitched;
}

bool GraphEdge::isSwitchTouched(int x, int y)
{
	Vec2 mid = (from + to) / 2;
	return (mid.x - x) * (mid.y - y) <= 30 * 30;
}

bool GraphEdge::isSwitchTouched(Vec2 pos)
{
	Vec2 mid = (from + to) / 2;
	return (mid - pos).mag2() <= 30 * 30;
}

void GraphEdge::update() {
	//updates information about node1 and node2 after any movement.

	Vec2 line = node2->getPos() - node1->getPos();

	double mag = line.mag();

	this->unitX = line.x / mag;
	this->unitY = line.y / mag;


	ymin = min(node1->getY(), node2->getY());
	ymax = max(node1->getY(), node2->getY());
	xmin = min(node1->getX(), node2->getX());
	xmax = max(node1->getX(), node2->getX());
	
	double fromX = node1->getX(), fromY = node1->getY();
	fromX += unitX * RADIUS;
	fromY += unitY * RADIUS;
	this->from.x = (int)fromX;
	this->from.y = (int)fromY;
	double toX = node2->getX();
	double toY = node2->getY();
	toX -= unitX * RADIUS;
	toY -= unitY * RADIUS;
	this->to.x = (int)toX;
	this->to.y = (int)toY;

	slope = (double)(to.y - from.y) / (to.x - from.x + (to.x == from.x ? 1 : 0));
	b = (int)(from.y - slope * from.x);
}