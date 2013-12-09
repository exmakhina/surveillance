#ifndef _FILEACTION_H_
#define _FILEACTION_H_

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "action.h"

class FileAction : public Action {
public:
	FileAction();
	~FileAction();
	
	void handler(const cv::Mat&);

private:
	// Image FIFO
	std::list<cv::Mat*> imageFIFO;
	
	// Thread stuff
	std::thread* actionThread;
	static void launcher(void*);
	void run();
	bool abort;
	
	// Synchronization stuff
	std::mutex mtx;
	std::condition_variable syncCV;
};


#endif  /* _FILEACTION_H_ */
