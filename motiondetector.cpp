#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "opencv2/gpu/gpu.hpp"
#include "settings.h"
#include "motiondetector.h"
#include "capture.h"
#include "action.h"


using namespace std;
using namespace cv;

// various tracking parameters (in seconds)
const double MHI_DURATION = 1;

MotionDetector::MotionDetector(Capture * captureInstance):
	camera(captureInstance),
	abort(false),
	last(0),
    mhi(NULL),
    thresholdLimit(THRESHOLD)
{
	motionDetectionThread = new thread(launcher, this);
}

MotionDetector::~MotionDetector()
{
	list<Action*>::const_iterator it; 
	
	abort = true;
	motionDetectionThread->join();
	delete motionDetectionThread;
	
	for (int i=0; i<MAX_FRAMES; i++ ) {
		delete buf[i];
	}
	
	delete mhi;
	
	for (it = actionList.begin(); it != actionList.end(); ++it) {
		delete *it;
	}
}

void MotionDetector::launcher(void * instance)
{
	static_cast<MotionDetector *>(instance)->motionDetection();
}

void MotionDetector::motionDetection()
{
	Mat image;
	
	cout << "MotionDetector thread started...\n";
	
	while (!abort) {
		{
			unique_lock<mutex> lock(syncMutex);
			camera->syncCV.wait(lock);
		}
			
		if (camera->getImage(image) == 0) {
			update_mhi(image, thresholdLimit);
		} 
    }
    
    cout << "MotionDetector thread stopped...\n";
}

// parameters:
//  img - input video frame
//  diff_threshold - threshold to avoid motion detection because of noise in the video
void MotionDetector::update_mhi(const Mat & img, int diff_threshold )
{
    double timestamp = (double)clock()/CLOCKS_PER_SEC; // get current time in seconds
    Size size = img.size();		// get current frame size
    int i, idx1 = last, idx2;
    Mat silh;
    Rect comp_rect(Point(0, 0), size);
    double count;
    list<Action*>::const_iterator action;
    gpu::GpuMat GpuSilh;
    gpu::GpuMat GpuSilhROI;
    
    // allocate images at the beginning or
    // reallocate them if the frame size is changed
    if( !mhi || (mhi->size().width != size.width) || (mhi->size().height != size.height) ) {
		for( i = 0; i < MAX_FRAMES; i++ ) {
			buf[i] = new Mat(Mat::zeros(size, CV_8UC1));
		}
		
		mhi = new Mat(Mat::zeros(size, CV_32FC1));
    }

    cvtColor( img, *buf[last], CV_BGR2GRAY ); // convert frame to grayscale

    idx2 = (last + 1) % MAX_FRAMES; // index of (last - (N-1))th frame
    last = idx2;

    silh = abs(*buf[idx1] - *buf[idx2]); 	// get difference between frames
    GpuSilh.upload(silh);
    gpu::threshold( GpuSilh, GpuSilh, diff_threshold, 1, CV_THRESH_BINARY ); // and threshold it
    updateMotionHistory( silh, *mhi, timestamp, MHI_DURATION ); // update MHI

    // select silhouette ROI
	Mat silhROI = silh(comp_rect);
	GpuSilhROI.upload(silhROI);
	count = gpu::norm( GpuSilhROI, CV_L1); 	// calculate number of points within silhouette ROI
	
	// check for the case of little motion
	if( count >= comp_rect.size().width*comp_rect.size().height * 0.05 ) {
		// got motion
		for (action = actionList.begin(); action != actionList.end(); ++action) {
			(*action)->handler(img);
		}
	} else {
		// got nothing
	}
}

void MotionDetector::registerAction(Action * action)
{
	if (action) {
		actionList.push_back(action);
	}
}
