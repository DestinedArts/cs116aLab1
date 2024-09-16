#include <limits>
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//set camera parameter
	cam.setDistance(10.0);
	cam.setNearClip(.1);
	cam.setTarget(glm::vec3(0, 0, 0));
	ofNoFill();

	// create sliders for user to select location of point and direction of the ray
	gui.setup(); // most of the time you don't need a name
	gui.add(px.setup("px",  0.0, -5.0, 5.0));
	gui.add(py.setup("py", -2.0, -5.0, 5.0));
	gui.add(pz.setup("pz",  0.0, -5.0, 5.0));
	gui.add(dx.setup("dx",  0.0, 0.0, 1.0));
	gui.add(dy.setup("dy",  1.0, 0.0, 1.0));
	gui.add(dz.setup("dz",  0.0, 0.0, 1.0));

	loadMesh();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
	// get values out of slider for the ray's point and direction
	glm::vec3 p = glm::vec3((float)px, (float)py, (float)pz);	// operator overloading, extract value as var
	glm::vec3 d = glm::vec3((float)dx, (float)dy, (float)dz);	// operator overloading, extract value as var

	// parameters for finding the intersection of the ray with closest surface of the pyramid
	float tBest = numeric_limits<float>::max();
	float betaBest = 0;	// beta for best ray triangle intersection
	float gammaBest = 0; // gamma for best ray triangle intersection
	size_t iTriBest = 0; // index of tInd for best triangle

	cam.begin();
	ofDrawSphere(p, 0.05); // draw the ray as a small point
	ofDrawLine(p, p + d);  // and a line for the direction

	// for each of the triangles in the mesh
	for (size_t i = 0; i < tInd.size(); i++)
	{
		// draw the triangle
		glm::vec3 v0 = verts[tInd[i][0]];
		glm::vec3 v1 = verts[tInd[i][1]];
		glm::vec3 v2 = verts[tInd[i][2]];
		ofDrawTriangle(v0, v1, v2);

		// determine if the ray intersects the triangle
		glm::vec3 c0 = v0 - v1;
		glm::vec3 c1 = v0 - v2;
		glm::vec3 c2 = d;
		glm::vec3 c3 = v0 - p;
		
		// use Cramer's Rule to solve for the intersection
		float dt = calcDet3x3(c0, c1, c2);
		if (dt == 0)
			continue; // no solution to intersection so skip this triangle
		float dx = calcDet3x3(c3, c1, c2);
		float dy = calcDet3x3(c0, c3, c2);
		float dz = calcDet3x3(c0, c1, c3);

		float beta = dx / dt;
		float gamma = dy / dt;
		float t = dz / dt;

		// Check for intersection inside triangle
		if (beta < 0 || gamma < 0 || beta + gamma > 1)
			continue;	// intersection outside triangle so skip

		// if the intersection is closer than the previous best, then save it
		if (t < tBest)
		{
			tBest = t;
			betaBest = beta;
			gammaBest = gamma;
			iTriBest = i;
		}
	}

	// if there is an intersection between ray and mesh
	if (tBest < numeric_limits<float>::max())
	{
		// calculate position of intersection
		glm::vec3 v0 = verts[tInd[iTriBest][0]];
		glm::vec3 v1 = verts[tInd[iTriBest][1]];
		glm::vec3 v2 = verts[tInd[iTriBest][2]];
		glm::vec3 q = v0 + betaBest * (v1 - v0) + gammaBest * (v2 - v0);
		ofDrawSphere(q, 0.1); // then draw the intersection as a large point
	}

	cam.end();

	if (!bHide)	// draw after camera
		gui.draw();

}

void ofApp::loadMesh()
{
	// Create vertices
	verts.push_back(glm::vec3(-1, 0, 1));
	verts.push_back(glm::vec3(1, 0, 1));
	verts.push_back(glm::vec3(1, 0, -1));
	verts.push_back(glm::vec3(-1, 0, -1));
	verts.push_back(glm::vec3(0, 3, 0));

	// Create index triangles
	// base
	tInd.push_back(glm::ivec3(0, 1, 2));
	tInd.push_back(glm::ivec3(0, 2, 3));
	// sides
	tInd.push_back(glm::ivec3(0, 1, 4));
	tInd.push_back(glm::ivec3(1, 2, 4));
	tInd.push_back(glm::ivec3(2, 3, 4));
	tInd.push_back(glm::ivec3(3, 0, 4));
}

//--------------------------------------------------------------

// calculate determinant of 3x3 matrix
// v0, v1, v2 are column vectors
float ofApp::calcDet3x3(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
	return 
		v0[0] * (v1[1] * v2[2] - v2[1] * v1[2]) +
		v1[0] * (v2[1] * v0[2] - v0[1] * v2[2]) +
		v2[0] * (v0[1] * v1[2] - v1[1] * v0[2]);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
	case 'f':	// fullscreen
		ofToggleFullscreen();
		break;
	case 'h':
		// hide var
		bHide = !bHide;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	glm::vec3 baryPos;
	float t;

	//	bool intersect = glm::intersectRayTriangle(glm::vec3(0, 0, 20),
	//		glm::vec3(0, 0, -1), verts[0], verts[1], verts[4], baryPos, t);

	// fire array down z axis
	glm::vec2 baryPoint;
	bool inter = glm::intersectRayTriangle(glm::vec3(0, 0, 20),
		glm::vec3(0, 0, -1), verts[0], verts[1], verts[4], baryPoint, t);

	if (inter)
	{
		cout << "hit" << endl;
		cout << "t: " << t;
	}
	else
		cout << "missed" << endl;
	
	cout << "mouse point in world: " << cam.screenToWorld(glm::vec3(x, y, 0)) << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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