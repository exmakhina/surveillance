#ifndef _MOTIONDETECTOR_H_
#define _MOTIONDETECTOR_H_

#include <opencv2/opencv.hpp>
#include <thread>
#include "capture.h"
#include "settings.h"


class MotionDetector {
public:
	MotionDetector(Capture *);
	~MotionDetector();
	
private:
	Capture * camera;
	
	static void launcher(void *);
	std::thread * motionDetectionThread;
	void motionDetection();
	void  update_mhi(const cv::Mat &, int);
	
    int thresholdLimit;
    int last;
    bool abort;

    // ring image buffer
    cv::Mat* buf[MAX_FRAMES];
    
    // temporary images
    cv::Mat* mhi; // MHI
};

#endif  /* _MOTIONDETECTOR_H_ */
