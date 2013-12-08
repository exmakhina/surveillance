#include <thread>
#include <list>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <mutex>
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
	Mat* newImage = new Mat();
	
	FIFOlock.lock();
	image.copyTo(*newImage);		// save the image locally
	imageFIFO.push_back(newImage);	// save its address in the local FIFO
	FIFOlock.unlock();
}

// Private stuff
void FileAction::launcher(void * instance)
{
	static_cast<FileAction*>(instance)->run();
}

void FileAction::run()
{
	chrono::milliseconds frameTime(1000/FPS);
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
		while(!imageFIFO.empty()) {
			tt = chrono::system_clock::to_time_t ( chrono::system_clock::now() );
			dateAndTime = ctime(&tt);
			
			if (fileID >= FPS) fileID = 1;  // Cannot have more than "FPS" image in the same second...
			filename = 	genericName + 
						dateAndTime.substr(0, dateAndTime.length()-1) + 
						"_" + 
						to_string(fileID++) + 
						extension;
			
			cout <<"saving file: " <<filename <<endl;
			
			FIFOlock.lock();
			currentPicture = imageFIFO.front();
			imwrite(filename, *currentPicture, jpgParams);
			delete currentPicture;
			imageFIFO.pop_front();
			FIFOlock.unlock();
		} 
		
		// If no image to save, wait at least the time to capture a new one...
		this_thread::sleep_for( frameTime );
	}
	
	cout << "FileAction thread stopped...\n";
}
