#include "ofApp.h"

#include "Kinematic.h"
#include "../KinemSeek.h"
#include <vector>

namespace {
	int sHeight = 1080/2; // Y
	int sWidth = 1920/2; // X
	// Kinematic
	physics::Kinematic p1;
	AI::KinemSeek seek;
	physics::SteeringOutput steer;
	ofVec2f target(600, 600);
	int sState = 1;
	std::vector<ofVec2f> breadCrumbs;
	float sPassedTime;


	void leaveCrumbs();
}

//--------------------------------------------------------------
void ofApp::setup(){
	//kinematicSetup();

	p1.mPosition = ofVec2f(200, 200);
	p1.mVelocity = ofVec2f(0.2, 2);
	seek = AI::KinemSeek(p1, target, 4.0f);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	//kinematicUpdate();
	steer = seek.getSteering();
	seek.mCharacter.update(steer, ofGetLastFrameTime()); // update

	if (ofGetMousePressed())
	{
		seek.mTarget = ofVec2f(ofGetMouseX(), ofGetMouseY());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	//kinemtaicDraw();
	ofSetColor(200, 0, 150);
	ofDrawCircle(seek.mCharacter.mPosition, 20);
	ofDrawCircle(seek.mTarget, 15);
}


namespace {
	void leaveCrumbs()
	{
		sPassedTime = 0;
		breadCrumbs.push_back(p1.mPosition);
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
			steer.mLinear = ofVec2f(0, -vel); // vel
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, 0); // rotaion
		}
		break;
	case 2:
		if (p1.mPosition.y <= 50)
		{
			sState++;
			p1.mVelocity = ofVec2f(-1, 0);
			steer.mLinear = ofVec2f(-vel,0); // vel
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, p1.mOrientation); // rotaion
		}
		break;
	case 3:
		if (p1.mPosition.x <= 50)
		{
			sState++;
			p1.mVelocity = ofVec2f(0, 1);
			steer.mLinear = ofVec2f(0,vel); // vel
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, p1.mOrientation); // rotaion
		}
		break;
	case 4:
		if (p1.mPosition.y >= sHeight)
		{
			p1.mVelocity = ofVec2f(1, 0);
			p1.mOrientation = p1.getNewOrientation(p1.mVelocity, p1.mOrientation); // rotaion
			p1.mVelocity = ofVec2f(0, 0);
		}
	default:
		break;
	}
}


void ofApp::kinematicSetup()
{
	p1.mPosition = ofVec2f(100, sHeight);
	p1.mVelocity = ofVec2f(5, 0);
	p1.mOrientation = 0.0f; // radians
	steer.mAngular = 0.0;
	steer.mLinear = ofVec2f(2.0f, 0.0f);

	leaveCrumbs();
}


void ofApp::kinemtaicDraw()
{
	ofTranslate(p1.mPosition.x, p1.mPosition.y);
	ofRotateZRad(p1.mOrientation); // rotate
	ofSetColor(150, 200, 0);
	ofDrawTriangle(ofVec2f(0, -50), ofVec2f(0, 50), ofVec2f(100, 0));
	ofSetColor(225, 0, 0);
	ofDrawCircle(0, 0, 50);

	ofRotateZRad(-p1.mOrientation); // rotate
	ofTranslate(-p1.mPosition.x, -p1.mPosition.y); //reset

	for (ofVec2f v : breadCrumbs)
	{
		ofDrawCircle(v, 20);
	}
}

void ofApp::kinematicUpdate()
{
	p1.update(steer, ofGetLastFrameTime());
	kinematicMotion();

	sPassedTime += ofGetLastFrameTime();
	if (sPassedTime > 0.2)
		leaveCrumbs();
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
