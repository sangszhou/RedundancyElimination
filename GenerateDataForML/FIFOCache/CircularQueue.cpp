#include "CircularQueue.h"
#include <iostream>
using namespace std;

CircularQueue::CircularQueue(unsigned int MaxQueueSize)  {
	MaxSize = MaxQueueSize + 1;
	queue = new unsigned int[MaxSize];
	front = rear = 0;
}

CircularQueue& CircularQueue::Add(const unsigned int &x)  {
	front = (front+1)%MaxSize;
	queue[front] = x;
	//printf("add %d\n", x);
	return *this;
}

CircularQueue& CircularQueue::Delete(unsigned int &x)  {
	rear = (rear+1)%MaxSize;
	x = queue[rear];
	//printf("delete %d\n", x);
	return *this;
}
