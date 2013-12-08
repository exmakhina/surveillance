#ifndef _MOTIONAPP_H_
#define _MOTIONAPP_H_

#include "capture.h"
#include "motiondetector.h"
#include "action.h"

class MotionApp {
public:
	MotionApp();
	~MotionApp();
	
	void start();

private:
	MotionDetector * motionDetector;
	Capture * camera;
	Action * fileAction;
};

#endif  /* _MOTIONAPP_H_ */
