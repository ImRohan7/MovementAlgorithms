#include "ofApp.h"

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
}

//--------------------------------------------------------------
void ofApp::setup(){
	p1.mPosition = ofVec2f(100, sHeight);
	p1.mOrientation = 0.0f; // radians
	s1.mAngular = 0.0;
	s1.mLinear = ofVec2f(0.1f,0.0f);

	A = ofVec2f(0, -50);
	B = ofVec2f(0, 50);
	C = ofVec2f(100, 0);
	
//	p1.mVelocity = ofVec2f(2, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
	p1.update(s1, ofGetElapsedTimef());
	sRotation++;

	kinematicMotion();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofTranslate(p1.mPosition.x, p1.mPosition.y);
	ofRotateZRad(p1.mOrientation); // rotate
	ofSetColor(150, 200, 0);
	ofDrawTriangle(A, B, C);
	ofSetColor(225, 0, 0);
	ofDrawCircle(0,0, 50);

}

void ofApp::kinematicMotion()
{
	switch (sState)
	{
	case 1:
		if (p1.mPosition.x >= sWidth)
		{
			sState++;
			p1.mVelocity = ofVec2f(0, 0);
			s1.mLinear = ofVec2f(0, -0.1f); // vel
			p1.mOrientation = p1.getNewOrientation(ofVec2f(-1, 0), 0); // rotaion
		}
		break;
	case 2:
		if (p1.mPosition.y <= 50)
		{
			sState++;
			p1.mVelocity = ofVec2f(0, 0);
			s1.mLinear = ofVec2f(-0.1f,0); // vel
			p1.mOrientation = p1.getNewOrientation(ofVec2f(0, -1), p1.mOrientation); // rotaion
		}
		break;
	case 3:
		if (p1.mPosition.x <= 50)
		{
			sState++;
			p1.mVelocity = ofVec2f(0, 0);
			s1.mLinear = ofVec2f(0,0.1f); // vel
			p1.mOrientation = p1.getNewOrientation(ofVec2f(1, 0), p1.mOrientation); // rotaion
		}
		break;
	case 4:
		if (p1.mPosition.y >= sHeight)
		{
			p1.mVelocity = ofVec2f(0, 0);
			p1.mOrientation = p1.getNewOrientation(ofVec2f(0, 1), p1.mOrientation); // rotaion
		}
	default:
		break;
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
