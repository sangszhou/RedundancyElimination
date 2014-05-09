#ifndef _CIRCULARQUEUE_H_
#define _CIRCULARQUEUE_H_

class CircularQueue  {
public:
	CircularQueue(unsigned int MaxQueueSize = 10);
	~CircularQueue()  {
		delete []queue;
	}

	bool IsEmpty() const  {
		return (front == rear);
	}

	bool IsFull() const  {
		return ((front+1)%MaxSize == rear);
	}
	
	// Add & Delete do not do full/empty check
	// leave full/empty check to upper layer

	CircularQueue& Add(const unsigned int &x);
	CircularQueue& Delete(unsigned int &x);

private:
	unsigned int front;
	unsigned int rear;
	unsigned int MaxSize;
	unsigned int *queue;
};

#endif