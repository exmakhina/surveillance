#ifndef _MOTIONAPP_H_
#define _MOTIONAPP_H_

#include "appobject.h"
#include "capture.h"
#include "motiondetector.h"
#include "action.h"

class MotionApp : public AppObject {
public:
	MotionApp();
	~MotionApp();

	void start();
	void stop();

private:
	MotionDetector * motionDetector;
	Capture * camera;
	Action * fileAction;
};

#endif  /* _MOTIONAPP_H_ */
