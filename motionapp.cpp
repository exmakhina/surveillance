#include "appobject.h"
#include "motionapp.h"
#include "motiondetector.h"
#include "capture.h"
#include "action.h"
#include "fileaction.h"
#include "settings.h"

using namespace std;

MotionApp::MotionApp():
		camera(NULL),
		motionDetector(NULL),
		fileAction(NULL),
		AppObject("MotionApp")
{
}

int MotionApp::start()
{
	if (camera == NULL)
		camera = new Capture(Settings::instance().getCamID());
	else
		return -1;

	if (motionDetector == NULL)
		motionDetector = new MotionDetector(camera);
	else
		return -1;

	fileAction = new FileAction();	// deleted by motionDetector
	
	motionDetector->registerAction(fileAction);

	return 0;
}

int MotionApp::stop()
{
	delete motionDetector;
	delete camera;

	camera = NULL;
	motionDetector = NULL;

	return 0;
}

