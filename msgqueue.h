#ifndef __MSGQUEUE_H__
#define __MSGQUEUE_H__

#include <queue>
#include <mutex>

using namespace std;

template <class T>
class MsgQueue
{
public:
	MsgQueue();
	~MsgQueue();
	
	int receive(T&);	/* blocking */
	int poll(T&);		/* non-blocking */
	int send(T);
	
private:
	mutex mtx;
	queue<T> msq;
	condition_variable notifier;
};

/**** Implementation ****/

template <class T>
MsgQueue<T>::MsgQueue() :
	mtx(),
	msq(),
	notifier()
{
}

template <class T>
MsgQueue<T>::~MsgQueue()
{
}

template <class T>
int MsgQueue<T>::receive(T &msg)
{
	unique_lock<mutex> lock(mtx);
	
	while (msq.empty()) {		// check if msq is not empty
		notifier.wait(lock);	// at every mutex notification
	}
	
	msg = msq.front();
	msq.pop();
	
	return 0;
}

template <class T>
int MsgQueue<T>::poll(T &msg)
{
	unique_lock<mutex> lock(mtx);
	
	if (!msq.empty()) {
		msg = msq.front();
		msq.pop();
		return 0;
	} else {
		return -1;
	}
}

template <class T>
int MsgQueue<T>::send(T msg)
{
	lock_guard<mutex> lock(mtx);
	msq.push(msg);
	notifier.notify_one();
	
	return 0;
}

#endif /* __MSGQUEUE_H__ */
