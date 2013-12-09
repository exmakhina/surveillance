#include "capture.h"
#include "settings.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace cv;
using namespace std;

Capture::Capture():
	abort(false),
	pause(false),
	capture(0),
	readIndex(0),
	writeIndex(0)
{
	capture = new VideoCapture(CAM_ID);
    if ( !capture->isOpened() )
    {
		cerr << "No webcam\n";
		return;
    }
    
    for (int i=0; i<MAX_FRAMES; i++) {
    	image[i] = new Mat(640, 480, CV_8UC3);
    }
    
    captureThread = new thread(launcher, this);
}

Capture::~Capture()
{
	abort = true;
	captureThread->join();
	delete captureThread;
	
	for (int i=0; i<MAX_FRAMES; i++) {
    	delete image[i];
    }
}

int Capture::getImage(Mat & imageRef)
{
	if ((readIndex != writeIndex) || pause){
		imageRef = *image[readIndex++];
		if (readIndex >= MAX_FRAMES) readIndex = 0;
		//TODO: add mutex here (for pause)
		if (pause) pause = false;		// unlock overrun condition
		return 0;
	} else {
		return -1;   	// readIndex == writeIndex means no new frames have been fetch from the camera
						// excepted for overrun condition.
	}
}

/*
 * Private stuff
 */
void Capture::launcher(void * instance){
	static_cast<Capture *>(instance)->run();
}

void Capture::run()
{
	cout << "Capture thread started...\n";
	
	while (!abort) {
		if ( !capture->isOpened() )
		{
			cerr << "No webcam\n";
			return;
		}
		
		if (!pause) {
			lock_guard<mutex> lock(mtx);	// lock mtx for the whole if {} statement
			// update image
			capture->read(*image[writeIndex++]);
			if (writeIndex >= MAX_FRAMES) writeIndex = 0;
			if (writeIndex == readIndex) pause = true;		// overrun condition, pause until reader process the images
			// a new image has been capture, notify the waiting thread (MotiobDetector)
			syncCV.notify_one();
		}
	}
	
	cout << "Capture thread stopped...\n";
}
