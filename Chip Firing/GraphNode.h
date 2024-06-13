#pragma once
#include"GraphSimConst.h"
#include"NodeText.h"
#include"GraphEdge.h"


class GraphNode
{
private:
	Vec2 pos;
	NodeType type;
	bool ghost;
	int chips;
	int degree;
	NodeText* text;
	std::vector<GraphEdge*> adjacent;

	void renderNode();
	void renderSink();

public:
	GraphNode(int x, int y, NodeType type);
	GraphNode(int x, int y, int chips, NodeType type);
	GraphNode(Vec2 pos, NodeType type);
	~GraphNode();

	GraphNode* copy();

	NodeText* getText();

	void updateTextColor();

	void setPos(int x, int y);
	void setPos(Vec2 position);

	void translateBy(Vec2 vec);

	Vec2 getPos();
	int getX();
	int* getXaddr();
	int getY();
	int* getYaddr();

	int getChips();
	void setChips(int chips);
	void changeChips(int diff);

	NodeType getType();
	void setType(NodeType type);

	void toggleGhost();
	void render();

	void addEdge(GraphEdge* edge);
	void removeEdge(GraphEdge* edge);
	std::vector<GraphEdge*> getEdges();

	bool containsPoint(Vec2 position);
	bool containsPoint(int x, int y);

	void fire();
	void inverseFire();
};
