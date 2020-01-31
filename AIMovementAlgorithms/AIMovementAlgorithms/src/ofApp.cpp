#include "ofApp.h"
#include <vector>

namespace {
	int sHeight = 1080/2; // Y
	int sWidth = 1920/2; // X
	float sRotation = 0.0f;
	ofVec2f offset(50,0);
	
	physics::Kinematic p1;
	physics::SteeringOutput s1;
	ofVec2f A;
	ofVec2f B;
	ofVec2f C;


	int sState = 1;
	std::vector<ofVec2f> breadCrumbs;

	void leaveCrumbs();
	float sPassedTime;
}

//--------------------------------------------------------------
void ofApp::setup(){
	p1.mPosition = ofVec2f(100, sHeight);
	p1.mVelocity = ofVec2f(5,0);
	p1.mOrientation = 0.0f; // radians
	s1.mAngular = 0.0;
	s1.mLinear = ofVec2f(2.0f,0.0f);

	A = ofVec2f(0, -50);
	B = ofVec2f(0, 50);
	C = ofVec2f(100, 0);
	
	leaveCrumbs();

//	p1.mVelocity = ofVec2f(2, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
	p1.update(s1, ofGetLastFrameTime());
	
	sRotation++;

	kinematicMotion();

	sPassedTime += ofGetLastFrameTime();
	if (sPassedTime > 0.2)
		leaveCrumbs();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofTranslate(p1.mPosition.x, p1.mPosition.y);
	ofRotateZRad(p1.mOrientation); // rotate
	ofSetColor(150, 200, 0);
	ofDrawTriangle(A, B, C);
	ofSetColor(225, 0, 0);
	ofDrawCircle(0,0, 50);

	
	ofRotateZRad(-p1.mOrientation); // rotate
	ofTranslate(-p1.mPosition.x, -p1.mPosition.y); //reset

	for (ofVec2f v : breadCrumbs)
	{
		ofDrawCircle(v, 20);
	}
	
}

void ofApp::kinematicMotion()
{
	float vel = 2.0f;
	switch (sState)
	{
	case 1:
		if (p1.mPosition.x >= sWidth)
		{
			sState++;
			p1.mVelocity = ofVec2f(0, -1); // up
			s1.mLinear = ofVec2f(0, -vel); // vel
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, 0); // rotaion
		}
		break;
	case 2:
		if (p1.mPosition.y <= 50)
		{
			sState++;
			p1.mVelocity = ofVec2f(-1, 0);
			s1.mLinear = ofVec2f(-vel,0); // vel
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, p1.mOrientation); // rotaion
		}
		break;
	case 3:
		if (p1.mPosition.x <= 50)
		{
			sState++;
			p1.mVelocity = ofVec2f(0, 1);
			s1.mLinear = ofVec2f(0,vel); // vel
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, p1.mOrientation); // rotaion
		}
		break;
	case 4:
		if (p1.mPosition.y >= sHeight)
		{
			p1.mVelocity = ofVec2f(1, 0);
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, p1.mOrientation); // rotaion
			p1.mVelocity = ofVec2f(0, 0);
			cout << breadCrumbs.size();
		}
	default:
		break;
	}
}

namespace {
	void leaveCrumbs()
	{
		sPassedTime = 0;
		breadCrumbs.push_back(p1.mPosition);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
