#include "Includes.h"

GraphNode* ghost;
EdgeType edgeType;
std::vector<GraphNode*> nodes;
std::vector<GraphEdge*> edges;
bool running;
SDL_Window* window;
SDL_Surface* windowSurface;
Image *sidebar;
SDL_Renderer* renderer;
TTF_Font* font;
SDL_Texture *hoveredTexture;
SDL_Texture* selectedTexture;
std::vector<Icon*> icons;
int selectedInd;
std::vector<Icon*> nodeIcons;
std::vector<Icon*> edgeIcons;
std::vector<Icon*> editIcons;
std::vector<Icon*> chipIcons;
std::vector<Icon*> fireIcons;
int selectedEditTool;
int selectedChipTool;
int selectedFireTool;
std::string currentToolTip;
Textbox* tip;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

/*
* 
* NEXT : ADD FIRING, CHIP ADDING, INVERTING.
* 
*/

void renderToolTip() {
	
	tip->setText(currentToolTip);
	Vec2 mousepos = getMousePos();
	if (mousepos.y + 30 >= SCREEN_HEIGHT) {
		tip->setCornerPos(mousepos - Vec2(0, 20));
	}
	else {
		tip->setCornerPos(mousepos + Vec2(10, 10));
	}
	tip->renderBoundingBox();
	tip->render();
}

int main( int argc, char* args[] )
{
	SDL_Event event;
	running = true;
	
	initSDL();
	initTextures();
	initFiles();

	ghost = new GraphNode(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Node);
	ghost->toggleGhost();

	tip = new Textbox(" ", getMousePos(), 20, { 100, 100, 100, 255 });
	bool toolTipRendered = false;
	long lastRenderMilli = 0;

	while (running) {
		//input section
		while (SDL_PollEvent(&event)){
			currentToolTip = "";
			if (toolTipRendered)
				toolTipRendered = false;
			parseEvent(&event);

			renderU(true);
			lastRenderMilli = SDL_GetTicks64();
		}
		if (!toolTipRendered && currentToolTip != "" && SDL_GetTicks64() - lastRenderMilli>500) {
			render(false);
			renderToolTip();
			SDL_RenderPresent(renderer);
			toolTipRendered = true;
		}
	}

	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}

//inits

void initSDL() {
	//initiates SDL, and creates/launches the window


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL_Init error: " << SDL_GetError << std::endl;
		exit(1);
	}
	SCREEN_WIDTH = 1350;
	SCREEN_HEIGHT = 900;
	window = SDL_CreateWindow("Graph Diagram Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image init error: " << IMG_GetError() << "\n";
		exit(1);
	}

	if (TTF_Init() == -1) {
		std::cout << "SDL_font init error: " << TTF_GetError() << "\n";
		exit(1);
	}

	currentToolTip = "";

	 windowSurface = SDL_GetWindowSurface(window);

	edgeType = Positive;


	clearScreen(BLACK);

	SDL_SetWindowIcon(window, IMG_Load("Assets/GraphIcon.png"));

}

void initTextures() {
	font = TTF_OpenFont("Assets/font.ttf", 255);
	if (font == NULL)
	{
		std::cout << "Failed to load font! Error: " << TTF_GetError() << "\n";
	}

	sidebar = new Image(0, 0, 61, SCREEN_HEIGHT, "Assets/Sidebar.png");

	//icon textures
	hoveredTexture = loadTexture("Assets/IconHover.png");
	selectedTexture = loadTexture("Assets/IconSelected.png");

	//node type icons
	nodeIcons.push_back(new Icon(0, 0, "Assets/IconOpenCircle.png", "Vertex"));
	nodeIcons.push_back(new Icon(63, 0, "Assets/IconSink.png", "Sink Vertex"));

	//edge type icons
	edgeIcons.push_back(new Icon(0, 63, "Assets/IconEdge.png", "Positive Edge"));
	edgeIcons.push_back(new Icon(63 * 1, 63, "Assets/IconResistEdge.png", "Negative Edge"));
	
	selectedEditTool = 0;
	editIcons.push_back(new Icon(0, 63 * 2, "Assets/IconEraser.png", "Eraser Tool (D)"));
	editIcons.push_back(new Icon(63, 63 * 2, "Assets/IconMove.png", "Node Movement Tool (M)"));

	selectedChipTool = 0;
	chipIcons.push_back(new Icon(0, 63 * 3, "Assets/IconAddChip.png", "Add Chips"));
	chipIcons.push_back(new Icon(63, 63 * 3, "Assets/IconSubChip.png", "Subtract Chips"));
	chipIcons.push_back(new Icon(63 * 2, 63 * 3, "Assets/IconSignSwitch.png", "Switch Sign"));


	selectedFireTool = 0;
	fireIcons.push_back(new Icon(0, 63 * 4, "Assets/IconFire.png", "Fire Vertex"));
	fireIcons.push_back(new Icon(63, 63 * 4, "Assets/IconInvFire.png", "Inverse Fire Vertex"));

	selectedInd = 0;

	//sidebar icons
	icons.push_back(new Icon(0, 0, "Assets/IconOpenCircle.png", "Vertex Placement Tools (V)"));
	icons[0]->toggleSelected();
	icons.push_back(new Icon(0, 63, "Assets/IconEdge.png", "Edge Connection Tools (E)"));
	icons[1]->linkTo(icons[0]);
	icons.push_back(new Icon(0, 63 * 2, "Assets/IconEraser.png", "Editing Tools (T)"));
	icons.push_back(new Icon(0, 63 * 3, "Assets/IconAddChip.png", "Chip Editing Tools (C)"));
	icons.push_back(new Icon(0, 63 * 4, "Assets/IconFire.png", "Firing Tools (F)"));

	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120 - 63 * 3, "Assets/IconNewFile.png", "New File"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120 - 63 * 2, "Assets/IconSave.png", "Save Current File"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120 - 63, "Assets/IconSaveAs.png", "Save As"));
	icons.push_back(new Icon(0, SCREEN_HEIGHT - 120, "Assets/IconOpen.png", "Open File"));
}