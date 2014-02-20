#include "capture.h"
#include "settings.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace cv;
using namespace std;

Capture::Capture(int cam):
	abort(true),
	pause(false),
	capture(0),
	readIndex(0),
	writeIndex(0),
	device(cam)
{
}

void Capture::start()
{
	Mat* newImage;

	if (abort)
	{
		abort = false;

		capture = new VideoCapture(device);
		if ( !capture->isOpened() )
		{
			cerr << "No webcam\n";
			return;
		}

		for (int i=0; i<Settings::instance().getMaxFrames(); i++) {
			newImage = new Mat(640, 480, CV_8UC3);
			image.push_back(newImage);
		}

		captureThread = new thread(launcher, this);
	}
}

void Capture::stop()
{
	if (!abort)
	{
		abort = true;
		captureThread->join();
		delete captureThread;

		for (int i=0; i<Settings::instance().getMaxFrames(); i++) {
			delete image[i];
		}
		image.clear();

		readIndex = 0;
		writeIndex = 0;
	}
}

int Capture::getImage(Mat & imageRef)
{
	if ((readIndex != writeIndex) || pause){
		lock_guard<mutex> lock(mtx);	// lock mtx for the if {} statement
		
		imageRef = *image[readIndex++];
		if (readIndex >= Settings::instance().getMaxFrames()) readIndex = 0;
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
			if (writeIndex >= Settings::instance().getMaxFrames()) writeIndex = 0;
			if (writeIndex == readIndex) pause = true;		// overrun condition, pause until reader process the images
			// a new image has been capture, notify the waiting thread (MotiobDetector)
			syncCV.notify_one();
		}
	}
	
	if (capture->isOpened())
		capture->release();

	cout << "Capture thread stopped...\n";
}
