#include "Includes.h"
#include "GraphSimConst.h"

void createObject(SDL_Event*, Vec2 mousePos);
void deleteObject(Vec2 mousePos, SDL_Event* event, Uint8 button);
void parseKey(SDL_Event*);
bool dragNode(SDL_Event*, Uint8 button, Vec2 mousePos, bool pan);
void openNodeMenu(SDL_Event*);
void openEdgeMenu(SDL_Event*);
void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec);
void parseMenuClick(Vec2 mousePos, SDL_Event * event); 
void newFile();

void addChip(SDL_Event*);
void removeChip(SDL_Event*);
void signSwitch(SDL_Event*);

void fire(SDL_Event*);
void inverseFire(SDL_Event*);


void parseEvent(SDL_Event* event) {
	Vec2 mousePos = getMousePos();
	switch (event->type) {
	case SDL_QUIT:
		running = false;
		saveFile();
		break;
	case SDL_MOUSEMOTION:

		ghost->setPos(mousePos);

		if (sidebar->isTouched(mousePos)) {
			updateHoverStatus(mousePos, icons);
		}

		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button) {
		case SDL_BUTTON_LEFT:

			if (sidebar->isTouched(mousePos)) {
				parseMenuClick(mousePos, event);
			}
			else {
				switch (selectedInd) {
				case 0:
				case 1:
					//Node and Edge tools, create graph
					SetCursor(LoadCursor(NULL, IDC_CROSS));
					createObject(event, mousePos);
					break;
				case 2:
					//editing tools, move and erase nodes/edges
					switch (selectedEditTool) {
					case 0:
						deleteObject(mousePos, event, SDL_BUTTON_LEFT);
						break;
					case 1:
						SetCursor(LoadCursor(NULL, IDC_SIZEALL));
						dragNode(event, SDL_BUTTON_LEFT, mousePos, false);
						break;
					}
					break;
				case 3:
					//chip and sign tools, chip adding/subtracting, edge/vertex switching
					switch (selectedChipTool) {
					case 0:
						addChip(event);
						break;
					case 1:
						removeChip(event);
						break;
					case 2:
						signSwitch(event);
						break;
					}
					break;
				case 4:
					//Firing tools, Fire and inverse fire
					switch (selectedFireTool) {
					case 0:
						fire(event);
						break;
					case 1:
						inverseFire(event);
						break;
					}
					break;
				}
			}
			break;

		case SDL_BUTTON_RIGHT:
			switch (selectedInd) {
			case 0:
			case 1:
			case 2:
				deleteObject(mousePos, event, SDL_BUTTON_RIGHT);
				break;
			case 3:
				//do inverse of current op (chip add <-> chip subtract, vertex switching stays the same)
				switch (selectedChipTool) {
				case 0:
					removeChip(event);
					break;
				case 1:
					addChip(event);
					break;
				case 2:
					deleteObject(mousePos, event, SDL_BUTTON_RIGHT);
					break;
				}
				break;
			case 4:
				//do inverse of current op (fire <-> inverse fire)
				switch (selectedFireTool) {
				case 0:
					inverseFire(event);
					break;
				case 1:
					fire(event);
					break;
				}
				break;
			}
			break;
		case SDL_BUTTON_MIDDLE:
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			dragNode(event, SDL_BUTTON_MIDDLE, mousePos, true);
			break;
		}
		break;
	case SDL_KEYDOWN:
		parseKey(event);
		break;

	case SDL_WINDOWEVENT:
		SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		bool resize = false;
		if (SCREEN_HEIGHT < 680) {
			SCREEN_HEIGHT = 680;
			resize = true;
		}
		if (SCREEN_WIDTH < 200) {
			SCREEN_WIDTH = 200;
			resize = true;
		}

		if (resize) SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		updateUIOnMove();
		break; 
	}
}


void addChip(SDL_Event *event) {
	Vec2 mousepos = getMousePos();
	for (GraphNode* v : nodes) {
		if (v->containsPoint(mousepos)) {
			v->changeChips(1);
			return;
		}
	}
}

void removeChip(SDL_Event *event) {
	Vec2 mousepos = getMousePos();
	for (GraphNode* v : nodes) {
		if (v->containsPoint(mousepos)) {
			v->changeChips(-1);
			return;
		}
	}
}

void signSwitch(SDL_Event* event) {
	//first, check if node is touched
	Vec2 mousePos = getMousePos();
	for (GraphNode* v : nodes) {
		if (v->containsPoint(mousePos)) {
			for (GraphEdge* e : v->getEdges()) {
				if (e->getNode1()->getType() != Sink && e->getNode2()->getType() != Sink) {
				e->invertSign();
				}
			}
			return;
		}
	}

	for (GraphEdge* e : edges) {
		if (e->isTouched(mousePos)) {
			if (e->getNode1()->getType() != Sink && e->getNode2()->getType() != Sink) {
				e->invertSign();
			}
		}
	}
}

void fire(SDL_Event* event) {
	Vec2 mousepos = getMousePos();
	for (GraphNode* v : nodes) {
		if (v->containsPoint(mousepos)) {
			v->fire();
			return;
		}
	}
}
void inverseFire(SDL_Event* event) {
	Vec2 mousepos = getMousePos();
	for (GraphNode* v : nodes) {
		if (v->containsPoint(mousepos)) {
			v->inverseFire();
			return;
		}
	}
}



void newFile()
{
	saveFile();

	//delete all things
	for (GraphNode* node : nodes) {
		delete node;
	}
	nodes.clear();
	for (GraphEdge* edge : edges) {
		delete edge;
	}
	nodes.clear();
	edges.clear();
	currentFilepath = "None";
}


void openNodeMenu(SDL_Event* event) {
	
	SDL_Rect menuBG = { 0, 0, (int)(63 * (int)nodeTypeTotal), 63 };
	SDL_Color menuColor = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, menuColor);
	for (int i = 0; i < nodeIcons.size(); i++) {
		nodeIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if(SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, nodeIcons);
			}

			//rendering plus the extra menu
			render(false);
			drawFilledRectangle(menuBG, menuColor);
			for (int i = 0; i < nodeIcons.size(); i++) {
				nodeIcons[i]->render();
			}
			SDL_RenderPresent(renderer);
			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, menuColor);
			for (int i = 0; i < nodeIcons.size(); i++) {
				nodeIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < nodeIcons.size() && !nodeIcons[i]->containsPoint(mousePos); i++);
	if (i < nodeIcons.size()) {
		ghost->setType((NodeType)i);
		updateIcons();
	}

}
void openEdgeMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63, (int)(63 * (int)edgeIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < edgeIcons.size(); i++) {
		edgeIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)){
				updateHoverStatus(mousePos, edgeIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < edgeIcons.size(); i++) {
				edgeIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < edgeIcons.size(); i++) {
				edgeIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < edgeIcons.size() && !edgeIcons[i]->containsPoint(mousePos); i++);
	if (i < edgeIcons.size()) {
		edgeType = (EdgeType)i;
		updateIcons();
	}

}
void openEditMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63*2, (int)(63 * (int)editIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < editIcons.size(); i++) {
		editIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, editIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < editIcons.size(); i++) {
				editIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < editIcons.size(); i++) {
				editIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < editIcons.size() && !editIcons[i]->containsPoint(mousePos); i++);
	if (i < editIcons.size()) {
		selectedEditTool = i;
		updateIcons();
	}

}

void openChipMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63 * 3, (int)(63 * (int)chipIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < chipIcons.size(); i++) {
		chipIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, chipIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < chipIcons.size(); i++) {
				chipIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < chipIcons.size(); i++) {
				chipIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < chipIcons.size() && !chipIcons[i]->containsPoint(mousePos); i++);
	if (i < chipIcons.size()) {
		selectedChipTool = i;
		updateIcons();
	}

}

void openFireMenu(SDL_Event* event) {
	SDL_Rect menuBG = { 0, 63 * 4, (int)(63 * (int)fireIcons.size()), 63 };
	SDL_Color color = { 80, 80, 80, 255 };
	Vec2 mousePos = getMousePos();

	render(false);
	drawFilledRectangle(menuBG, color);
	for (int i = 0; i < fireIcons.size(); i++) {
		fireIcons[i]->render();
	}
	SDL_RenderPresent(renderer);

	while (!SDL_PollEvent(event));
	bool toolTipRendered = false;
	long lastRenderMilli = 0;
	while (!(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)) {
		if (SDL_PollEvent(event)) {
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			mousePos = getMousePos();
			if (rectIsTouched(menuBG, mousePos)) {
				updateHoverStatus(mousePos, fireIcons);
			}

			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < fireIcons.size(); i++) {
			 fireIcons[i]->render();
			}
			SDL_RenderPresent(renderer);

			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli > 500) {
			render(false);
			drawFilledRectangle(menuBG, color);
			for (int i = 0; i < fireIcons.size(); i++) {
				fireIcons[i]->render();
			}
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	mousePos = getMousePos();
	int i;
	for (i = 0; i < fireIcons.size() && !fireIcons[i]->containsPoint(mousePos); i++);
	if (i < fireIcons.size()) {
		selectedFireTool = i;
		updateIcons();
	}

}



void createObject(SDL_Event* event, Vec2 mousePos) {
	// if initial click is not touching a node, creates another node
	// else, waits for the left button to release. if the release is also on a node,
	// creates an edge between those two nodes 
	int n1 = -1, n2 = -1;

	for (int i = 0; i < nodes.size() && n1 == -1; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			n1 = i;
		}
	}

	if (n1 != -1) {
		renderU(false);
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
			SDL_PollEvent(event);
		}

		mousePos = getMousePos();
		ghost->setPos(mousePos);

		for (int i = 0; i < nodes.size() && n2 == -1; i++) {
			if (nodes[i]->containsPoint(mousePos)) {
				n2 = i;
			}
		}

		if (n2 != -1 && n2 != n1) {
			GraphEdge* toAdd = new GraphEdge(nodes[n1], nodes[n2], WHITE, edgeType);
			edges.push_back(toAdd);
			nodes[n1]->addEdge(toAdd);
			nodes[n2]->addEdge(toAdd);
			
		}
	}
	else {
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
			SDL_PollEvent(event);
		}
		Vec2 mousepos2 = getMousePos();
		
		if (ghost->containsPoint(mousepos2)){ 
			nodes.push_back(ghost->copy());
		}
		ghost->setPos(mousepos2);
	}
}
void deleteObject(Vec2 mousePos, SDL_Event* event, Uint8 button) {
	// deletes any nodes or edges touched
	while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
		if (SDL_PollEvent(event)) {
			mousePos = getMousePos();
			for (int i = 0; i < nodes.size();) {
				if (nodes[i]->containsPoint(mousePos)) {					
					for (int j = 0; j < edges.size();) {
						if (edges[j]->containsNode(nodes[i])) {
							GraphEdge* toDelete = edges[j];
							toDelete->getNode1()->removeEdge(toDelete);
							toDelete->getNode2()->removeEdge(toDelete);
							edges.erase((edges.begin() + j));
							delete toDelete;
						}
						else {
							j++;
						}
					}
					GraphNode* toDelete = nodes[i];
					nodes.erase(nodes.begin() + i);
					delete toDelete;
					
				}
				else {
					i++;
				}
			}

			for (int i = 0; i < edges.size();) {
				if (edges[i]->isTouched(mousePos)) {
					GraphEdge* toDelete = edges[i];
					toDelete->getNode1()->removeEdge(toDelete);
					toDelete->getNode2()->removeEdge(toDelete);
					edges.erase(edges.begin() + i);
					delete toDelete;
				}
				else {
					i++;
				}
			}
			renderU(false);
		}
	}
	ghost->setPos(mousePos);
}

bool dragNode(SDL_Event* event, Uint8 button, Vec2 mousePos, bool pan) {
	GraphNode* toMove = nullptr;
	Vec2 mousePos2;
	//finds node to be moved
	for (int i = 0; i < nodes.size() && toMove == nullptr; i++) {
		if (nodes[i]->containsPoint(mousePos)) {
			toMove = nodes[i];
		}
	}

	if (toMove == nullptr) {
		if (pan == true) {
			while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
				if (SDL_PollEvent(event)) {
					mousePos2 = getMousePos();
					for (int i = 0; i < nodes.size(); i++) {
						nodes[i]->translateBy(mousePos2 - mousePos);
					}
					for (int i = 0; i < edges.size(); i++) {
						edges[i]->update();
					}
					
					mousePos = getMousePos();
					renderU(false);
					SDL_RenderPresent(renderer);
				}
			}
			return true;
		}
		SDL_Rect selectBox = { mousePos.x, mousePos.y, 0, 0 };
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
			if (SDL_PollEvent(event)) {
				mousePos2 = getMousePos();
				selectBox.w = mousePos2.x - mousePos.x;
				selectBox.h = mousePos2.y - mousePos.y;
				renderObjects(false);
				drawFilledRectangle(selectBox, { 137, 207, 240 , 150 });
				renderInterface();
				SDL_RenderPresent(renderer);
			}
		}
			//next: include free edges in the box selection
		std::unordered_set<GraphNode*> touched;
		std::unordered_set<NodeText*> textTouched;

		for (int i = 0; i < nodes.size(); i++) {
			if (rectIsTouched(selectBox, nodes[i]->getPos())) {
				touched.insert(nodes[i]);
			}
		}
		if (touched.size() == 0) {
			return false;
		}
		while (event->type != SDL_MOUSEBUTTONDOWN) {
			SDL_PollEvent(event);
		}
		mousePos = getMousePos();
		bool isNodeTouched = false;
		for (GraphNode* node : touched) {
			isNodeTouched = isNodeTouched || node->containsPoint(mousePos);
		}
		if (!isNodeTouched) {
			return false;
		}
					mousePos2 = getMousePos();
		switch (event->button.button) {
		case SDL_BUTTON_LEFT:
			mousePos2 = getMousePos();
		
			while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)) {
						if (SDL_PollEvent(event)) {
							mousePos2 = getMousePos();
							for (GraphNode* node : touched) {
								node->setPos(node->getPos() + mousePos2 - mousePos);
							}
							for (NodeText* nt : textTouched) {
								nt->updatePos();
							}
							for (GraphEdge* edge : edges) {
								edge->update();
							}
							renderU(false);
							mousePos = getMousePos();
						}
					}
					break;

				case SDL_BUTTON_RIGHT:
					//delete all nodes in the square
					for (int i = 0; i < nodes.size();) {
						if (touched.find(nodes[i]) != touched.end()) {
							for (int j = 0; j < edges.size();) {
								if (edges[j]->containsNode(nodes[i])) {
									GraphEdge* toDelete = edges[j];
									toDelete->getNode1()->removeEdge(toDelete);
									toDelete->getNode2()->removeEdge(toDelete);
									edges.erase((edges.begin() + j));
									delete toDelete;
								}
								else {
									j++;
								}
							}
							GraphNode* toDelete = nodes[i];
							nodes.erase(nodes.begin() + i);
							delete toDelete;
						}
						else {
							i++;
						}
					}
					break;
				}
			
		

		//if you press right click (ON A NODE), delete all nodes in the box and connected edges.
		//if you press left click (ON A NODE), move nodes and connected edges
		//then, find the edges connected to those nodes(or i could update all of the edges every time might be better)

	}
	else {
		//move the node
		std::vector<GraphEdge*> toUpdate;
		NodeText* updateT = toMove->getText();
		// finds connected edges
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i]->containsNode(toMove)) {
				toUpdate.push_back(edges[i]);
			}
		}

		// waits for click to release
		while (!(event->type == SDL_MOUSEBUTTONUP && event->button.button == button)) {
			if (SDL_PollEvent(event)) {
				SDL_GetMouseState(toMove->getXaddr(), toMove->getYaddr());
				for (int i = 0; i < toUpdate.size(); i++) {
					toUpdate[i]->update();
				}
				if (updateT != nullptr) updateT->updatePos();
				renderU(false);
			}
		}

		// updates any connected edges
		for (int i = 0; i < toUpdate.size(); i++) {
			toUpdate[i]->update();
		}
	}
	ghost->setPos(getMousePos());
	return true;
}

void parseKey(SDL_Event* event) {

	switch (event->key.keysym.sym) {
	case SDLK_ESCAPE:
		running = false;
		saveFile();
		break;

	case SDLK_e:
		if (selectedInd <= 1) {
			edgeType = (EdgeType)(((int)edgeType + 1) % edgeTypeTotal);
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 1;
			icons[1]->toggleSelected();
		}
		break;
	case SDLK_v:
		if (selectedInd <= 1) {
			ghost->setType((NodeType)(((int)ghost->getType() + 1) % nodeTypeTotal));
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 1;
			icons[1]->toggleSelected();
		}
		break;
	case SDLK_t:
		if (selectedInd == 2) {
			selectedEditTool = (selectedEditTool + 1) % editIcons.size();
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 2;
			icons[2]->toggleSelected();
		}
		break;
	case SDLK_c:
		if (selectedInd == 3) {
			selectedChipTool = (selectedChipTool + 1) % chipIcons.size();
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 3;
			icons[3]->toggleSelected();
		}
		break;
	case SDLK_f:
		if (selectedInd == 4) {
			selectedFireTool = (selectedFireTool + 1) % fireIcons.size();
			updateIcons();
		}
		else {
			icons[selectedInd]->toggleSelected();
			selectedInd = 4;
			icons[4]->toggleSelected();
		}
		break;
	case SDLK_RIGHT:
		SCREEN_WIDTH += 100;
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case SDLK_LEFT:
		SCREEN_WIDTH -= 100;
		if (SCREEN_WIDTH < 200) {
			SCREEN_WIDTH = 200;
		}
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case SDLK_UP:
		SCREEN_HEIGHT -= 100;
		if (SCREEN_HEIGHT < 680) {
			SCREEN_HEIGHT = 680;
		}
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case SDLK_DOWN:
		SCREEN_HEIGHT += 100;
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	}
}

Vec2 getMousePos() {
	Vec2 position;
	SDL_GetMouseState(&position.x, &position.y);
	return position;
}

bool lineIntersectsRect(SDL_Rect rect, Vec2 p1, Vec2 p2) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}
	Vec2 topLeft(rect.x, rect.y);
	Vec2 width(rect.w, 0);
	Vec2 height(0, rect.h);

	return (rectIsTouched(rect, p1) && rectIsTouched(rect, p2))
		|| (lineIntersectsLine(p1, p2, topLeft, topLeft + width))
		|| (lineIntersectsLine(p1, p2, topLeft, topLeft + height))
		|| (lineIntersectsLine(p1, p2, topLeft + height, topLeft + height + width))
		|| (lineIntersectsLine(p1, p2, topLeft + width, topLeft + height + width));
}


bool lineIntersectsLine(Vec2 v1, Vec2 v2, Vec2 v3, Vec2 v4) {
	double den = ((v4.y - v3.y) * (v2.x - v1.x) - (v4.x - v3.x) * (v2.y - v1.y));
	if (den == 0) return false;
	
	double uA = ((v4.x - v3.x) * (v1.y - v3.y) - (v4.y - v3.y) * (v1.x - v3.x)) / den;
	
	double uB = ((v2.x - v1.x) * (v1.y - v3.y) - (v2.y - v1.y) * (v1.x - v3.x)) / den;

	return uA > 0 && uA < 1 && uB > 0 && uB < 1;
}

bool rectIsTouched(SDL_Rect rect, int x, int y) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}

	return (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);
}
bool rectIsTouched(SDL_Rect rect, Vec2 pos) {
	return rectIsTouched(rect, pos.x, pos.y);
}

void updateHoverStatus(Vec2 mousePos, std::vector<Icon*> iconVec) {
	for (int i = 0; i < iconVec.size(); i++) {
		if (iconVec[i]->isHovered()) currentToolTip = iconVec[i]->getMessage();
		if (!iconVec[i]->containsPoint(mousePos) && iconVec[i]->isHovered()) {
			iconVec[i]->setHover(false);
			
		}
		else if (iconVec[i]->containsPoint(mousePos) && !iconVec[i]->isHovered()) {
			iconVec[i]->setHover(true);
		}
	}

}

void parseMenuClick(Vec2 mousePos, SDL_Event *event)
{
	int clickedInd = -1;

	for (int i = 0; i < icons.size() && clickedInd < 0; i++) {
		if (icons[i]->containsPoint(mousePos)) {
			clickedInd = i;
		}
	}

	if (clickedInd < icons.size() - 4 && clickedInd != selectedInd) {
		icons[selectedInd]->toggleSelected();
		icons[clickedInd]->toggleSelected();
		selectedInd = clickedInd;
	}

	switch (clickedInd) {
	case -1:
		break;
	case 0:
		openNodeMenu(event);
		break;
	case 1:
		openEdgeMenu(event);
		break;
	case 2:
		openEditMenu(event);
		break;
	case 3:
		openChipMenu(event);
		break;
	case 4:
		openFireMenu(event);
		break;
	default:
		if (clickedInd == icons.size() - 3)
			saveFile();
		else if (clickedInd == icons.size() - 2) {
			saveAsFile();
		}
		else if (clickedInd == icons.size() - 1) {
			openFile();
		}
		else if (clickedInd == icons.size() - 4) {
			newFile();
		}
		break;
	}
}

