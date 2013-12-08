#ifndef _FILEACTION_H_
#define _FILEACTION_H_

#include <list>
#include <thread>
#include <mutex>
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
	std::mutex	FIFOlock;
	
	// Thread stuff
	std::thread* actionThread;
	static void launcher(void*);
	void run();
	bool abort;
};


#endif  /* _FILEACTION_H_ */
