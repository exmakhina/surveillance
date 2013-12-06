#include <iostream>
#include "motionapp.h"
#include "motiondetector.h"
#include "capture.h"

using namespace std;

MotionApp::MotionApp()
{
	camera = new Capture();
	motionDetector = new MotionDetector(camera);
}

MotionApp::~MotionApp()
{
	delete camera;
	delete motionDetector;
}

void MotionApp::start()
{
	cout << "nothing happens here... Capture and MotionDetector threads are supposed to be running at this point." << endl;
}
