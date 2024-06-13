#include "Includes.h"

bool error;


OPENFILENAMEA ofn;       // common dialog box structure
char szFile[260];       // buffer for file name
HWND hwnd = { 0 };              // owner window
CHOOSECOLOR cc;                 // common dialog box structure 
COLORREF acrCustClr[16]; // array of custom colors 
HBRUSH hbrush;       // brush handle
DWORD rgbCurrent;        // initial color selection
std::string currentFilepath;


void initFiles() {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "GraphC File\0 * .gphc\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = "gphc";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	currentFilepath = "None";
	
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
}


std::string getOpenPath() {
	// Display the Open dialog box. 
	std::string filename = "ERROR";

	if (GetOpenFileNameA(&ofn)) {
		filename = ofn.lpstrFile;
	}

	return filename;
}
std::string getSavePath() {
	std::string filename = "ERROR";
	if(GetSaveFileNameA(&ofn)){
		filename = ofn.lpstrFile;
	}
	return filename;
}

void openFile() {
	//use ifstream to upload status from file
	error = false;
	
	std::ifstream inStream;
	std::string filename, line, word;
	int num;

	filename = getOpenPath();
	if (filename == "ERROR") {
		return;
	}


	std::vector<GraphNode*> fileNodes;
	std::vector<GraphEdge*> fileEdges;

	inStream.open(filename);

	std::vector<std::string> wordVec;

	////nodes
	int x, y, chips, type;
	while (!error && getline(inStream, line) && line.size() != 0) {
		std::stringstream strStream(line);

		while (strStream >> word) wordVec.push_back(word);

		if (wordVec.size() != 4) {
			return;
		}

		try {
			x = stoi(wordVec[0]);
			y = stoi(wordVec[1]);
			chips = stoi(wordVec[2]);
			type = stoi(wordVec[3]);
		}
		catch (...) {
			return;
		}
		if (type < 0 || type >= nodeTypeTotal) return;

		fileNodes.push_back(new GraphNode(x, y, chips, (NodeType)type));

		wordVec.clear();
	}




	//edges
	int ind1, ind2, etype;
	while (!error && getline(inStream, line) && line.size() != 0) {
		std::stringstream strStream(line);

		while (strStream >> word) wordVec.push_back(word);

		if (wordVec.size() != 3) return;

		try {
			ind1 = stoi(wordVec[0]);
			ind2 = stoi(wordVec[1]);
			etype = stoi(wordVec[2]);
		}
		catch (...) {
			return;
		}
		if (ind1 < 0) return;
		if (ind2 < 0) return;
		if (etype < 0 || etype >= edgeTypeTotal) return;
		try {
			fileEdges.push_back(new GraphEdge(fileNodes[ind1], fileNodes[ind2], WHITE, (EdgeType)etype));
			fileNodes[ind1]->addEdge(fileEdges[fileEdges.size() - 1]);
			fileNodes[ind2]->addEdge(fileEdges[fileEdges.size() - 1]);
		}
		catch (...) {
			return;
		}

		wordVec.clear();
	}


	inStream.close();
	if (error) return;

	for (GraphNode *node : nodes) {
		delete node;
	}
	nodes.clear();
	for (GraphEdge* edge : edges) {
		delete edge;
	}
	edges.clear();
	nodes = fileNodes;
	edges = fileEdges;

	for (GraphNode* n : nodes) {
		n->updateTextColor();
	}

	currentFilepath = filename;

	updateIcons();
}

void saveTo(std::string filename) {
	std::ofstream outStream;
	std::string line, word;
	outStream.open(filename);

	std::unordered_map<GraphNode*, int> indexMap;

	//input all nodes on seperate lines: x, y, radius, color, type, 
	for (int i = 0; i < nodes.size(); i++) {
		outStream << nodes[i]->getX() << " "
			<< nodes[i]->getY() << " "
			<< nodes[i]->getChips() << " "
			<< nodes[i]->getType() << "\n";
		indexMap[nodes[i]] = i;
	}

	outStream << "\n";


	//input all edges on seperate lines: ind1, ind2, color, type

	for (int i = 0; i < edges.size(); i++) {
		outStream << indexMap[edges[i]->getNode1()] << " "
			<< indexMap[edges[i]->getNode2()] << " "
			<< (int)edges[i]->getType() << "\n";
	}


	currentFilepath = filename;

	outStream.close();
}

void saveAsFile() {
	//use ofstream to save current status to file
	
	std::string filename = getSavePath();
	if (filename == "ERROR") {
		return;
	}
	saveTo(filename);
}
void saveFile() {
	//use ofstream to save current status to file
	
	std::string filename;
	if (currentFilepath == "None") {
		filename = getSavePath();
		if (filename == "ERROR") {
			return;
		}
	}
	else {
		filename = currentFilepath;
	}
	saveTo(filename);
}

