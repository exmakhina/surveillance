#ifndef _MOTIONDETECTOR_H_
#define _MOTIONDETECTOR_H_

#include <opencv2/opencv.hpp>
#include <thread>
#include <list>
#include <vector>
#include <mutex>
#include "capture.h"
#include "settings.h"
#include "action.h"


class MotionDetector {
public:
	MotionDetector(Capture *);
	~MotionDetector();
	
	void registerAction(Action *);
	
private:
	Capture * camera;
	
	std::mutex syncMutex;
	static void launcher(void *);
	std::thread * motionDetectionThread;
	void motionDetection();
	void  update_mhi(const cv::Mat &, int);
	bool abort;
	
    int thresholdLimit;
    int last;
    
    // ring image buffer
    std::vector<cv::Mat*> buf;
    
    // temporary images
    cv::Mat* mhi; // MHI
    
    // Actions list
    std::list<Action*> actionList;
};

#endif  /* _MOTIONDETECTOR_H_ */
