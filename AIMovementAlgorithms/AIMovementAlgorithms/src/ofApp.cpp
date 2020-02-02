#include "ofApp.h"

#include "Kinematic.h"
#include "../KinemSeek.h"
#include <vector>
#include "../AISystem.h"

namespace {
	int sHeight = 1080/2; // Y
	int sWidth = 1920/2; // X
	// Kinematic
	physics::Kinematic charct;
	physics::Kinematic targt;
	AI::KinemSeek seek;
	physics::SteeringOutput steer;
	
	int sState = 1;
	std::vector<ofVec2f> breadCrumbs;
	float sPassedTime = 0.0f;
	float sCrumbTime = 0.05f;

	AISystem::Algo sAlgo; // which algo are we running

	void RecordCrumbs();
	void DrawCrumbs();
}

//--------------------------------------------------------------
void ofApp::setup(){
	sAlgo = AISystem::Algo::SeekArrive;
	AISetup();

}

//--------------------------------------------------------------
void ofApp::update(){
	
	AIUpdate();

}

//--------------------------------------------------------------
void ofApp::draw(){

	AIDraw();
	
}


namespace {
	
	void RecordCrumbs()
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
			ofDrawCircle(v, 10);
		}

	}
}

void ofApp::kinematicMotion()
{
	float vel = 2.0f;
	switch (sState)
	{
	case 1:
		if (charct.mPosition.x >= sWidth)
		{
			sState++;
			charct.mVelocity = ofVec2f(0, -1); // up
			steer.mLinear = ofVec2f(0, -vel); // vel
			charct.updateOrientation(); // rotaion
		}
		break;
	case 2:
		if (charct.mPosition.y <= 50)
		{
			sState++;
			charct.mVelocity = ofVec2f(-1, 0);
			steer.mLinear = ofVec2f(-vel,0); // vel
			charct.updateOrientation(); // rotaion
			
		}
		break;
	case 3:
		if (charct.mPosition.x <= 50)
		{
			sState++;
			charct.mVelocity = ofVec2f(0, 1);
			steer.mLinear = ofVec2f(0,vel); // vel
			charct.updateOrientation(); // rotaion

		}
		break;
	case 4:
		if (charct.mPosition.y >= sHeight)
		{
			charct.mVelocity = ofVec2f(1, 0);
			charct.updateOrientation(); // rotaion

			charct.mVelocity = ofVec2f(0, 0);
		}
	default:
		break;
	}
}


void ofApp::AISetup()
{
	switch (sAlgo)
	{
	case AISystem::Algo::Basic_Kinematic:


		charct.mPosition = ofVec2f(100, sHeight);
		charct.mVelocity = ofVec2f(5, 0);
		charct.mOrientation = 0.0f; // radians
		steer.mAngular = 0.0;
		steer.mLinear = ofVec2f(2.0f, 0.0f);
		break;

	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:
		charct.mPosition = ofVec2f(200, 200);
		charct.mVelocity = ofVec2f(0.5, 0.4);
		targt.mPosition = ofVec2f(600, 600);
		targt.mVelocity = ofVec2f(0.8, 0);
		seek = AI::KinemSeek(charct, targt, 8.0f);
		seek.mMaxAccel = 10;
		seek.mSlowRad = 250;
		seek.mTargetRad = 25;
		seek.mMaxRotat = 0.25;
		seek.mMaxSpeed = 1;
		seek.mTimeTotarget = 0.2f;
		break;
	case AISystem::Algo::WanderSteering:
		
		break;
	case AISystem::Algo::Flocking:

		break;

	default:
		std::cout << "Unknown case passed";
		break;
	}
	

	RecordCrumbs();
}

void ofApp::AIUpdate()
{
	RecordCrumbs();
	
	switch (sAlgo)
	{
	case AISystem::Algo::Basic_Kinematic:
		charct.update(steer, ofGetLastFrameTime());
		kinematicMotion();

		break;
	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:
		
	//	steer = seek.getSteeringForArrival();
		steer = seek.getSteeringForWandering();
		seek.mCharacter.update(steer, ofGetLastFrameTime()); // update
		seek.mTarget.update(physics::SteeringOutput(), ofGetLastFrameTime()); // update

		if (ofGetMousePressed())
		{
			seek.mTarget.mPosition = ofVec2f(ofGetMouseX(), ofGetMouseY());
			breadCrumbs.clear();
		}

		//seek.mCharacter.updateOrientation(); // rotaion

		break;
	case AISystem::Algo::WanderSteering:

		break;
	case AISystem::Algo::Flocking:

		break;

	default:
		std::cout << "Unknown case passed";
		break;
	}
	
}

void ofApp::AIDraw()
{
		DrawCrumbs();
	switch (sAlgo)
	{
	case AISystem::Algo::Basic_Kinematic:
		ofTranslate(charct.mPosition.x, charct.mPosition.y);
		ofRotateZRad(charct.mOrientation); // rotate
		ofSetColor(150, 200, 0);
		ofDrawTriangle(ofVec2f(0, -50), ofVec2f(0, 50), ofVec2f(100, 0));
		ofSetColor(225, 0, 0);
		ofDrawCircle(0, 0, 50);

		ofRotateZRad(-charct.mOrientation); // reset rotate
		ofTranslate(-charct.mPosition.x, -charct.mPosition.y); //reset
		break;

	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:
		ofSetColor(250, 0, 150);
		ofNoFill();
		ofDrawCircle(seek.mTarget.mPosition, seek.mSlowRad); // slow rad
		ofDrawCircle(seek.mTarget.mPosition, seek.mTargetRad); // target rad
		ofFill();
		ofDrawCircle(seek.mTarget.mPosition, 15);
		ofDrawLine(seek.mCharacter.mPosition, seek.mCharacter.mPosition + 10 * steer.mLinear);
		
		ofTranslate(seek.mCharacter.mPosition);
		ofRotateZRad(seek.mCharacter.mOrientation); // rotate
		ofDrawCircle(ofVec2f(0,0),20);
		ofSetColor(150, 200, 0);
		ofDrawTriangle(ofVec2f(10, -20), ofVec2f(10, 20), ofVec2f(30, 0));
		break;

	case AISystem::Algo::WanderSteering:

		break;
	case AISystem::Algo::Flocking:

		break;

	default:
		std::cout << "Unknown case passed";
		break;
	}
	

	////////
	
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
