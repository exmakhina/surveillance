#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include <thread>
#include <opencv2/opencv.hpp>
#include "settings.h"

class Capture {
public:
	Capture();
	~Capture();
	
	int getImage(cv::Mat &);	// return 0 if valid frame, -1 otherwise
	
private:
	bool abort;
	bool pause;
	cv::Mat image[MAX_FRAMES];	// Circular frame buffer
    cv::VideoCapture * capture;
    int readIndex;
    int writeIndex;
    
	static void launcher(void *);
	std::thread * captureThread;
	    
	void run();
};

#endif
