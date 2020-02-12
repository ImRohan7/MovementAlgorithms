#include "ofApp.h"

#include "Kinematic.h"
#include "../KinemSeek.h"
#include <vector>
#include "../AISystem.h"

namespace {
	int sHeight = 1080/2; // Y
	int sWidth = 1920/2; // X
	// Kinematic
	physics::Kinematic lead; // leader
	physics::Kinematic targt;
	AI::KinemSeek seek;
	physics::SteeringOutput steer;
	std::vector<physics::Kinematic> followers; // for flocking
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
		if (lead.mPosition.x >= sWidth)
		{
			sState++;
			lead.mVelocity = ofVec2f(0, -1); // up
			steer.mLinear = ofVec2f(0, -vel); // vel
			lead.updateOrientation(steer); // rotaion
		}
		break;
	case 2:
		if (lead.mPosition.y <= 50)
		{
			sState++;
			lead.mVelocity = ofVec2f(-1, 0);
			steer.mLinear = ofVec2f(-vel,0); // vel
			lead.updateOrientation(steer); // rotaion
			
		}
		break;
	case 3:
		if (lead.mPosition.x <= 50)
		{
			sState++;
			lead.mVelocity = ofVec2f(0, 1);
			steer.mLinear = ofVec2f(0,vel); // vel
			lead.updateOrientation(steer); // rotaion

		}
		break;
	case 4:
		if (lead.mPosition.y >= sHeight)
		{
			lead.mVelocity = ofVec2f(1, 0);
			lead.updateOrientation(steer); // rotaion

			lead.mVelocity = ofVec2f(0, 0);
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


		lead.mPosition = ofVec2f(100, sHeight);
		lead.mVelocity = ofVec2f(5, 0);
		lead.mOrientation = 0.0f; // radians
		steer.mAngular = 0.0;
		steer.mLinear = ofVec2f(2.0f, 0.0f);
		break;

	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:
		lead.mPosition = ofVec2f(200, 200);
		lead.mVelocity = ofVec2f(0.5, 3);
		targt.mPosition = ofVec2f(600, 600);
		targt.mVelocity = ofVec2f(0, 0);
		seek = AI::KinemSeek(lead, targt, 8.0f);
		seek.mMaxAccel = 10;
		seek.mSlowRadArrive = 250;
		seek.mTargetRadArrive = 25;
		//seek.mMaxRotat = 20;
		//seek.mMaxSpeed = 8;
		seek.mTimeTotargetArrive = 0.2f;
		break;
	case AISystem::Algo::WanderSteering:
		lead.mPosition = ofVec2f(200, 200);
		lead.mVelocity = ofVec2f(0.5, 0.4);
		targt.mPosition = ofVec2f(600, 600);
		targt.mVelocity = ofVec2f(0.8, 0);
		seek = AI::KinemSeek(lead, targt, 8.0f);
		seek.mMaxAccel = 10;
		seek.mMaxRotat = 20;
		seek.mMaxSpeed = 8;
		break;
	case AISystem::Algo::Flocking:
		lead.mPosition = ofVec2f(400, 400);
		lead.mSepRadius = 200;
		lead.mVelocity = ofVec2f(2, 0);
		lead.mWeight = 200;
		// followers
		followers.push_back(physics::Kinematic(ofVec2f(50,300)));
		followers.push_back(physics::Kinematic(ofVec2f(50,400)));
		followers.push_back(physics::Kinematic(ofVec2f(50,500)));
		followers.push_back(physics::Kinematic(ofVec2f(200,300)));
		followers.push_back(physics::Kinematic(ofVec2f(200,400)));
		followers.push_back(physics::Kinematic(ofVec2f(200,500)));
	
		for (int i=0; i<followers.size(); i++)
		{
			followers[i].mSepRadius = 100;
			followers[i].mWeight = 20;
			
		}

		break;

	default:
		std::cout << "Unknown case passed";
		break;
	}
	

	RecordCrumbs();
}

void ofApp::AIUpdate()
{
	float A, B;

	RecordCrumbs();
	
	switch (sAlgo)
	{
	case AISystem::Algo::Basic_Kinematic:
		lead.update(steer, ofGetLastFrameTime());
		kinematicMotion();

		break;
	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:
		
		steer = seek.getSteeringForArrival();
		steer.mAngular = AISystem::getSteeringFor_Align(
			seek.mTarget.mOrientation, seek.mCharacter.mOrientation,
			3.5, 0.15f, 0.5).mAngular;

		seek.mCharacter.update(steer, ofGetLastFrameTime()); // update
		seek.mTarget.update(physics::SteeringOutput(), ofGetLastFrameTime()); // update

		A = ofRadToDeg(seek.mCharacter.mOrientation);
		B = ofRadToDeg(seek.mTarget.mOrientation);

		if (ofGetMousePressed())
		{
			seek.mTarget.mPosition = ofVec2f(ofGetMouseX(), ofGetMouseY());
			int deg = rand() % 300;
			seek.mTarget.mOrientation = ofDegToRad(deg);
			breadCrumbs.clear();
		}

	//	seek.mCharacter.updateOrientation(steer); // rotaion

		break;
	case AISystem::Algo::WanderSteering:
		steer = seek.getSteeringForWandering();
	
		seek.mCharacter.update(steer, ofGetLastFrameTime()); // update

		break;
	case AISystem::Algo::Flocking:
		lead.update(physics::SteeringOutput(), ofGetLastFrameTime());
		AISystem::calcCom(lead, followers);
		AISystem::calcCovel(lead, followers);
		for (int i = 0; i < followers.size(); i++)
		{
			auto st = AISystem::getSteeringForFlocking(lead, followers, i);
			float cOr = followers[i].mVelocity.length() > 0 ? atan2(followers[i].mVelocity.y, followers[i].mVelocity.x) : followers[i].mOrientation;

			st.mAngular = AISystem::getSteeringFor_Align(lead.mOrientation, cOr, 4.5, 0.1,4.5).mAngular;
			followers[i].update(st, ofGetLastFrameTime());
			followers[i].updateOrientation(steer);
		}

		// back to screen
		if (lead.mPosition.x > sWidth)
		{
			lead.mPosition.x -= sWidth;
		}
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
		ofTranslate(lead.mPosition.x, lead.mPosition.y);
		ofRotateZRad(lead.mOrientation); // rotate
		ofSetColor(150, 200, 0);
		ofDrawTriangle(ofVec2f(0, -50), ofVec2f(0, 50), ofVec2f(100, 0));
		ofSetColor(225, 0, 0);
		ofDrawCircle(0, 0, 50);

		ofRotateZRad(-lead.mOrientation); // reset rotate
		ofTranslate(-lead.mPosition.x, -lead.mPosition.y); //reset
		break;

	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:
		ofSetColor(250, 0, 150);
		ofNoFill();
		ofDrawCircle(seek.mTarget.mPosition, seek.mSlowRadArrive); // slow rad
		ofDrawCircle(seek.mTarget.mPosition, seek.mTargetRadArrive); // target rad
		ofFill();
		drawBoid(seek.mTarget.mPosition, seek.mTarget.mOrientation);
		ofDrawLine(seek.mCharacter.mPosition, seek.mCharacter.mPosition + 10 * steer.mLinear);
		
		drawBoid(seek.mCharacter.mPosition, seek.mCharacter.mOrientation);
		break;

	case AISystem::Algo::WanderSteering:
		ofSetColor(250, 0, 150);
		ofDrawLine(seek.mCharacter.mPosition, seek.mCharacter.mPosition + 10 * steer.mLinear);

		ofTranslate(seek.mCharacter.mPosition);
		ofRotateZRad(seek.mCharacter.mOrientation); // rotate
		ofDrawCircle(ofVec2f(0, 0), 20);
		ofSetColor(150, 200, 0);
		ofDrawTriangle(ofVec2f(10, -20), ofVec2f(10, 20), ofVec2f(30, 0));
		break;
	case AISystem::Algo::Flocking:
		ofSetColor(250, 0, 150);
		ofDrawCircle(lead.mPosition, 20);
		for (auto f : followers)
		{
			ofTranslate(f.mPosition);
			ofRotateZRad(f.mOrientation); // rotate
			ofDrawCircle(ofVec2f(0,0), 15);
			ofSetColor(150, 200, 0);
			ofDrawTriangle(ofVec2f(10, -20), ofVec2f(10, 20), ofVec2f(30, 0));
			ofRotateZRad(-f.mOrientation); // rotate
			ofTranslate(-f.mPosition);

		}
		break;

	default:
		std::cout << "Unknown case passed";
		break;
	}
	

	////////
	
}

// rotat and draw BOid with triangle
void ofApp::drawBoid(ofVec2f pos, float ori)
{
	// translate
	ofTranslate(pos);
	// rotate
	ofRotateZRad(ori); // rotate
	// draw
	ofSetColor(250, 0, 150);
	ofDrawCircle(ofVec2f(0, 0), 20);
	ofSetColor(150, 200, 0);
	ofDrawTriangle(ofVec2f(10, -20), ofVec2f(10, 20), ofVec2f(30, 0));
	// reverse rotate
	ofRotateZRad(-ori); // rotate
	// translate back
	ofTranslate(-pos);

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
