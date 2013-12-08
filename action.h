#ifndef _ACTION_H_
#define _ACTION_H_

#include <opencv2/opencv.hpp>

class Action {
public:
	Action() {}
	virtual ~Action() {}
	
	virtual void handler(const cv::Mat&) = 0;
};

#endif  /* _ACTION_H_ */
