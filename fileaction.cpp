#include <thread>
#include <list>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "fileaction.h"
#include "settings.h"

using namespace std;
using namespace cv;

FileAction::FileAction():
	abort(false)
{
	actionThread = new thread(launcher, this);
}

FileAction::~FileAction()
{
	list<Mat*>::const_iterator it;
	
	abort = true;
	actionThread->join();
	delete actionThread;
	
	for (it = imageFIFO.begin(); it != imageFIFO.end(); ++it) {
		delete *it;
	}
}

void FileAction::handler(const Mat& image)
{
	lock_guard<mutex> lock(mtx);	// hold the mutex during the execution of handler() {}
	
	// Limit the number of images in the file writer waiting list
	if (imageFIFO.size() >= MAX_FRAMES) {
		cout << "Warning: Image Fifo overrun, skipping frame.\n";
		return;
	} 
	
	Mat* newImage = new Mat();
	
	image.copyTo(*newImage);		// save the image locally
	imageFIFO.push_back(newImage);	// save its address in the local FIFO
	
	syncCV.notify_one();				// notify the run() thread a new image is ready
}

// Private stuff
void FileAction::launcher(void * instance)
{
	static_cast<FileAction*>(instance)->run();
}

void FileAction::run()
{
	list<Mat*>::const_iterator pict;
	Mat* currentPicture;
	time_t tt;
	vector<int> jpgParams;
    jpgParams.push_back(CV_IMWRITE_JPEG_QUALITY);
    jpgParams.push_back(90);
    string genericName = "capture_";
    string extension = ".jpg";
    unsigned fileID = 1;
    string dateAndTime;
	string filename;
	
	cout << "FileAction thread started...\n";
	
	while (!abort) {
		{
			unique_lock<mutex> lock(mtx);
			syncCV.wait(lock);		// wait until handler() call notify_one()
		}
		while(!imageFIFO.empty()) {
			tt = chrono::system_clock::to_time_t ( chrono::system_clock::now() );
			dateAndTime = ctime(&tt);
			
			if (fileID >= FPS) fileID = 1;  // Cannot have more than "FPS" image in the same second...
			filename = 	genericName + 
						dateAndTime.substr(0, dateAndTime.length()-1) + 
						"_" + 
						to_string(fileID++) + 
						extension;
			{
				lock_guard<mutex> fifoLock(mtx);	// lock the mutex for the current {} section (FIFO access)
				
				currentPicture = imageFIFO.front();
				imwrite(filename, *currentPicture, jpgParams);
				delete currentPicture;
				imageFIFO.pop_front();
			}
		} 
	}
	
	cout << "FileAction thread stopped...\n";
}
