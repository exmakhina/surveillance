#include <iostream>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "settings.h"
#include "motiondetector.h"
#include "capture.h"

using namespace std;
using namespace cv;

MotionDetector::MotionDetector(Capture * captureInstance):
	camera(captureInstance),
	abort(false),
	last(0),
    buf(0),
    mhi(0),
    orient(0),
    mask(0),
    segmask(0),
    threshold(THRESHOLD)
{
	motionDetectionThread = new thread(launcher, this);
}

MotionDetector::~MotionDetector()
{
	motionDetectionThread.join();
	delete motionDetectionThread;
}

void MotionDetector::launcher(void * instance)
{
	static_cast<MotionDetector *>(instance)->motionDetection();
}

void MotionDetector::motionDetection()
{
	Mat image;
	chrono::milliseconds frameTime( 1000/FPS );
	
	cout << "MotionDetector thread started..." << endl;
	
	while (!abort) {
		if (camera->getImage(&image) == 0) {
			update_mhi(&image, threshold);
		} else {
			this_thread::sleep_for( frameTime );	// sleep the time to capture at least one new frame
		}
    }
}

// parameters:
//  img - input video frame
//  diff_threshold - threshold to avoid motion detection because of noise in the video
void MotionDetector::update_mhi(const Mat & img, int diff_threshold )
{
    double timestamp = (double)clock()/CLOCKS_PER_SEC; // get current time in seconds
    Size size = img.size()		// get current frame size
    int i, idx1 = last, idx2;
    Mat silh;
    Rect comp_rect(0, 0, size.width(), size.height());
    double count;
    
    // allocate images at the beginning or
    // reallocate them if the frame size is changed
    if( !mhi || (mhi->size().width != size.width) || (mhi->size().height != size.height) ) {
		for( i = 0; i < MAX_FRAMES; i++ ) {
			buf[i] = new Mat(size, CV_8UC1);
		}
		
		/*delete mhi;
		delete orient;
		delete segmask;
		delete mask;*/
		
		mhi = new Mat(size, CV_32FC1);
		orient = new Mat(size, CV_32FC1);
		segmask = new Mat(size, CV_32FC1);
		mask = new Mat(size, CV_8UC1);
    }

    cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale

    idx2 = (last + 1) % MAX_FRAMES; // index of (last - (N-1))th frame
    last = idx2;

    silh = abs(buf[idx1] - buf[idx2]); 	// get difference between frames
    cvThreshold( silh, silh, diff_threshold, 1, CV_THRESH_BINARY ); // and threshold it
    cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI

    // select silhouette ROI
	Mat silhROI = silh(comp_rect);
	count = cvNorm( silhROI, 0, CV_L1, 0 ); // calculate number of points within silhouette ROI
	
	// check for the case of little motion
	if( count >= comp_rect.size().width*comp_rect.size().height * 0.05 ) {
		// got motion
		cout << "Got Motion !!" << endl;
	} else {
		// got nothing
	}
}
