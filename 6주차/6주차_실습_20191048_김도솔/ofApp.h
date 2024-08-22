#pragma once

#include "ofMain.h"

struct LineSegment {
	int x1;
	int x2;
	int y1;
	int y2;
};

struct Dot {
	int x1;
	int y1;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	LineSegment* lineseg;
	Dot* dot;

	/* WaterFall-related member variables Regions */

	// flag variables
	int draw_flag;
	int load_flag;
	int selection_dot;
	int currentIndex;

	// Line segment and dot related variables
	int num_of_line;
	int num_of_dot;


	/* WaterFall-related member functions */
	ofPoint target_dot;
	float water_radius;
	float dot_diameter;


	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void initializeWaterLines(); // 2nd week portion.

};