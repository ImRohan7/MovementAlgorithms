#include "ofApp.h"

#include "Kinematic.h"
#include "../KinemSeek.h"
#include <vector>

namespace {
	int sHeight = 1080/2; // Y
	int sWidth = 1920/2; // X
	// Kinematic
	physics::Kinematic p1;
	physics::Kinematic tar;
	AI::KinemSeek seek;
	physics::SteeringOutput steer;
	
	int sState = 1;
	std::vector<ofVec2f> breadCrumbs;
	float sPassedTime;
	float sCrumbTime = 0.3f;

	void leaveCrumbs();
	void DrawCrumbs();
}

//--------------------------------------------------------------
void ofApp::setup(){
	//kinematicSetup();

	p1.mPosition = ofVec2f(200, 200);
	p1.mVelocity = ofVec2f(0.2, 2);
	tar.mPosition = ofVec2f(600, 600);
	tar.mVelocity = ofVec2f(1, 1);
	seek = AI::KinemSeek(p1, tar, 8.0f);
	seek.mMaxAccel = 10;
	seek.mSlowRad = 200;
	seek.mTargetRad = 40;
	seek.mTimeTotarget = 0.4f;


}

//--------------------------------------------------------------
void ofApp::update(){
	//kinematicUpdate();
	steer = seek.getSteeringForArrival();
	seek.mCharacter.update(steer, ofGetLastFrameTime()); // update
	seek.mTarget.update(physics::SteeringOutput() , ofGetLastFrameTime()); // update

	if (ofGetMousePressed())
	{
		seek.mTarget.mPosition = ofVec2f(ofGetMouseX(), ofGetMouseY());
		breadCrumbs.clear();
	}

	leaveCrumbs();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//kinemtaicDraw();
	ofSetColor(250, 0, 150);
	ofNoFill();
	ofDrawCircle(seek.mTarget.mPosition, seek.mSlowRad); // slow rad
	ofDrawCircle(seek.mTarget.mPosition, seek.mTargetRad); // target rad

	ofFill();
	ofDrawCircle(seek.mCharacter.mPosition, 20);
	ofDrawCircle(seek.mTarget.mPosition, 15);

	DrawCrumbs();
}


namespace {
	
	void leaveCrumbs()
	{
		sPassedTime += ofGetLastFrameTime();
		if (sPassedTime > sCrumbTime)
		{
			sPassedTime = 0;
			breadCrumbs.push_back(seek.mCharacter.mPosition);
		}
	}


	void DrawCrumbs()
	{
		ofSetColor(250, 200, 150);
		for (ofVec2f v : breadCrumbs)
		{
			ofDrawCircle(v, 13);
		}

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
