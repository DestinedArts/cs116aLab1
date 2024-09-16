#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxGui.h"
#include <glm/gtx/intersect.hpp>

class ofApp : public ofBaseApp
{

	vector<glm::vec3> verts;
	vector<glm::ivec3> tInd;

	public:
		void setup();
		void update();
		void draw();

		void loadMesh();
		float calcDet3x3(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofEasyCam cam;

		bool bHide = false;
		//ofxFloatSlider point, direction;
		ofxFloatSlider px, py, pz, dx, dy, dz;
		ofxPanel gui;
};
