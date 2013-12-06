#ifndef _MOTIONAPP_H_
#define _MOTIONAPP_H_

#include "capture.h"
#include "motiondetector.h"

class MotionApp {
public:
	MotionApp();
	~MotionApp();
	
	void start();

private:
	MotionDetector * motionDetector;
	Capture * camera;	
};

#endif  /* _MOTIONAPP_H_ */
