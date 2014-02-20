#include "appobject.h"
#include "motionapp.h"
#include "motiondetector.h"
#include "capture.h"
#include "action.h"
#include "fileaction.h"
#include "settings.h"

using namespace std;

MotionApp::MotionApp():
		AppObject("MotionApp")
{
	camera = new Capture(Settings::instance().getCamID());
	motionDetector = new MotionDetector(camera);

	fileAction = new FileAction();	// deleted by motionDetector
	motionDetector->registerAction(fileAction);
}

MotionApp::~MotionApp()
{
	delete motionDetector;
	delete camera;
}

void MotionApp::start()
{
	camera->start();
}

void MotionApp::stop()
{
	camera->stop();
}
