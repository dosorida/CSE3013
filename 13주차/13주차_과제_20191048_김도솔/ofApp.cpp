/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = false;
	isBFS = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		freeMemory();
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen) {
			BFS();
			bShowInfo = bChecked;
		}
		else {
			cout << "You must open a file first." << endl;
		}

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {
	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here

	double x = 0, y = 0;

	int rectLong = 30;
	int rectShort = 3;

	for (i = 0; i < HEIGHT; i++) {
		y = rectLong * (double)(i / 2) + rectShort * (double)(i - (i / 2));
		for (j = 0; j < WIDTH; j++) {
			x = rectLong * (double)(j / 2) + rectShort * (double)(j - (j / 2));
			if (maze[i][j] == '+') {
				ofDrawRectangle(x, y, rectShort, rectShort);
			}
			else if (maze[i][j] == '|') {
				ofDrawRectangle(x, y, rectShort, rectLong);
			}
			else if (maze[i][j] == '-') {
				ofDrawRectangle(x, y, rectLong, rectShort);
			}
		}
	}

	if (isDFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			DFS();
			dfsdraw();
		}	
		else
			cout << "You must open file first" << endl;
	}

	if (isBFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			BFS();
			bfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName.c_str());
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			// 미로 가로, 세로 구하기
			ifstream fp;
			fp.open(filePath);

			string line;

			while (getline(fp, line)) {
				WIDTH = line.length();
				HEIGHT++;
			}

			fp.close();

			// 미로 데이터 저장
			fp.open(filePath);

			maze.resize(HEIGHT, vector<char>(WIDTH));

			while (getline(fp, line)) {
				for (int i = 0; i < WIDTH; i++) {
					maze[idx][i] = line[i];
				}
				idx++;
			}

			fp.close();
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}

void ofApp::freeMemory() {
	maze.clear();
	visited.clear();
	result.clear();
	dist.clear();
	bfspath.clear();
	while (!path.empty()) path.pop();
}

bool ofApp::DFS() {
	// 초기화
	pair<int, int> start = make_pair(1, 1);
	pair<int, int> destination = make_pair(HEIGHT - 2, WIDTH - 2);
	visited.resize(HEIGHT, vector<bool>(WIDTH, false));
	path.push(start);

	while (!path.empty()) {
		int y = path.top().first;
		int x = path.top().second;
		
		if (y == destination.first && x == destination.second) {
			isDFS = true;
			return true;  // 탈출 경로를 찾았을 경우 종료
		}

		// 왼쪽 > 위쪽 > 오른쪽 > 아래쪽
		if (y - 1 >= 0 && maze[y - 1][x] == ' ' && !visited[y - 1][x]) {
			visited[y - 1][x] = true;
			path.push(make_pair(y - 1, x));
			continue;
		}
		if (x + 1 <= WIDTH && maze[y][x + 1] == ' ' && !visited[y][x + 1]) {
			visited[y][x + 1] = true;
			path.push(make_pair(y, x + 1));
			continue;
		}
		if (y + 1 < HEIGHT && maze[y + 1][x] == ' ' && !visited[y + 1][x]) {
			visited[y + 1][x] = true;
			path.push(make_pair(y + 1, x));
			continue;
		}
		if (x - 1 >= 0 && maze[y][x - 1] == ' ' && !visited[y][x - 1]) {
			visited[y][x - 1] = true;
			path.push(make_pair(y, x - 1));
			continue;
		}

		path.pop();
	}
	return false;  // 탈출 경로를 찾지 못한 경우 종료
}

void ofApp::dfsdraw() {
	// maze 벡터를 기반으로 DFS 탐색 경로를 그려줍니다.
	// Print the contents of the 'path' stack
	int i, j, x, y;
	int rectLong = 30;
	int rectShort = 3;
	ofSetColor(200);
	for (i = 0; i < HEIGHT; i++) {
		y = rectLong * (double)(i / 2) + rectShort * (double)(i - (i / 2));
		y += 15;
		for (j = 0; j < WIDTH; j++) {
			x = rectLong * (double)(j / 2) + rectShort * (double)(j - (j / 2));
			x += 15;
			if (visited[i][j] == true) {
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, rectShort, rectShort);
				}
				else if (i % 2) {
					ofDrawRectangle(x - rectLong + rectShort, y, rectLong, rectShort);
				}
				else {
					ofDrawRectangle(x, y - rectLong + rectShort, rectShort, rectLong);
				}
			}
		}
	}
	
	result.resize(HEIGHT, vector<bool>(WIDTH, false));
	ofSetColor(255, 0, 0);
	while (!path.empty()) {
		int py = path.top().first;
		int px = path.top().second;
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (py == i && px == j) {
					result[i][j] = true;
				}
			}
		}
		path.pop();
	}

	for (i = 0; i < HEIGHT; i++) {
		y = rectLong * (double)(i / 2) + rectShort * (double)(i - (i / 2));
		y += 15;
		for (j = 0; j < WIDTH; j++) {
			x = rectLong * (double)(j / 2) + rectShort * (double)(j - (j / 2));
			x += 15;
			if (result[i][j] == true) {
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, rectShort, rectShort);
				}
				else if (i % 2) {
					ofDrawRectangle(x - rectLong + rectShort, y, rectLong, rectShort);
				}
				else {
					ofDrawRectangle(x, y - rectLong + rectShort, rectShort, rectLong);
				}
			}
		}
	}
}

bool ofApp::BFS() {
	// 초기화
	pair<int, int> start = make_pair(1, 1);
	pair<int, int> destination = make_pair(HEIGHT - 2, WIDTH - 2);
	visited.resize(HEIGHT, vector<bool>(WIDTH, false));
	dist.resize(HEIGHT, vector<int>(WIDTH, -1));

	queue<pair<int, int>> q;
	q.push(start);
	visited[start.first][start.second] = true;
	dist[start.first][start.second] = 0;

	while (!q.empty()) {
		int y = q.front().first;
		int x = q.front().second;
		q.pop();

		if (y == destination.first && x == destination.second) {
			isBFS = true;
			return true;  // 탈출 경로를 찾았을 경우 종료
		}

		// 왼쪽 > 위쪽 > 오른쪽 > 아래쪽
		if (y - 1 >= 0 && maze[y - 1][x] == ' ' && !visited[y - 1][x]) {
			visited[y - 1][x] = true;
			dist[y - 1][x] = dist[y][x] + 1;
			q.push(make_pair(y - 1, x));
		}
		if (x + 1 <= WIDTH && maze[y][x + 1] == ' ' && !visited[y][x + 1]) {
			visited[y][x + 1] = true;
			dist[y][x + 1] = dist[y][x] + 1;
			q.push(make_pair(y, x + 1));
		}
		if (y + 1 < HEIGHT && maze[y + 1][x] == ' ' && !visited[y + 1][x]) {
			visited[y + 1][x] = true;
			dist[y + 1][x] = dist[y][x] + 1;
			q.push(make_pair(y + 1, x));
		}
		if (x - 1 >= 0 && maze[y][x - 1] == ' ' && !visited[y][x - 1]) {
			visited[y][x - 1] = true;
			dist[y][x - 1] = dist[y][x] + 1;
			q.push(make_pair(y, x - 1));
		}
	}

	return false;  // 탈출 경로를 찾지 못한 경우 종료
}

void ofApp::bfsdraw() {
	// maze 벡터를 기반으로 DFS 탐색 경로를 그려줍니다.
	// Print the contents of the 'path' stack
	int i, j, x, y;
	int rectLong = 30;
	int rectShort = 3;
	ofSetColor(200);
	for (i = 0; i < HEIGHT; i++) {
		y = rectLong * (double)(i / 2) + rectShort * (double)(i - (i / 2));
		y += 15;
		for (j = 0; j < WIDTH; j++) {
			x = rectLong * (double)(j / 2) + rectShort * (double)(j - (j / 2));
			x += 15;
			if (visited[i][j] == true) {
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, rectShort, rectShort);
				}
				else if (i % 2) {
					ofDrawRectangle(x - rectLong + rectShort, y, rectLong, rectShort);
				}
				else {
					ofDrawRectangle(x, y - rectLong + rectShort, rectShort, rectLong);
				}
			}
		}
	}

	// BFS 탐색 경로를 찾았을 때
	if (isBFS) {
		pair<int, int> start = make_pair(1, 1);
		pair<int, int> destination = make_pair(HEIGHT - 2, WIDTH - 2);
		pair<int, int> current = destination;
		int pathLength = dist[current.first][current.second];
		bfspath.resize(HEIGHT, vector<bool>(WIDTH, false));

		while (pathLength > 0) {
			bfspath[current.first][current.second] = true;

			// 이동 경로 선택
			if (current.first - 1 >= 0 && dist[current.first - 1][current.second] == pathLength - 1) {
				current.first -= 1;
			}
			else if (current.second + 1 < WIDTH && dist[current.first][current.second + 1] == pathLength - 1) {
				current.second += 1;
			}
			else if (current.first + 1 < HEIGHT && dist[current.first + 1][current.second] == pathLength - 1) {
				current.first += 1;
			}
			else if (current.second - 1 >= 0 && dist[current.first][current.second - 1] == pathLength - 1) {
				current.second -= 1;
			}

			pathLength--;
		}

		// 출발 지점 표시
		bfspath[start.first][start.second] = true;
	}

	ofSetColor(0, 0, 255);
	for (i = 0; i < HEIGHT; i++) {
		y = rectLong * (double)(i / 2) + rectShort * (double)(i - (i / 2));
		y += 15;
		for (j = 0; j < WIDTH; j++) {
			x = rectLong * (double)(j / 2) + rectShort * (double)(j - (j / 2));
			x += 15;
			if (bfspath[i][j] == true) {
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, rectShort, rectShort);
				}
				else if (i % 2) {
					ofDrawRectangle(x - rectLong + rectShort, y, rectLong, rectShort);
				}
				else {
					ofDrawRectangle(x, y - rectLong + rectShort, rectShort, rectLong);
				}
			}
		}
	}
}