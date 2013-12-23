#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "settings.h"

class Capture {
public:
	Capture(int);	// int is the camera ID
	~Capture();
	
	int getImage(cv::Mat &);	// return 0 if valid frame, -1 otherwise
	
	//std::mutex syncMutex;
	std::condition_variable syncCV;
	
private:
	std::mutex mtx;
	bool pause;
	std::vector<cv::Mat*> image;	// Circular frame buffer
    cv::VideoCapture* capture;
    int readIndex;
    int writeIndex;
    
	static void launcher(void *);
	std::thread * captureThread;
	void run();
	bool abort;
};

#endif
