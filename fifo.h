#pragma once

typedef struct {
	void **objects;
	long size;
} FifoQueue;

FifoQueue *mkQueue();
void *addToQueue(FifoQueue *, void *, size_t);
void *popFromQueue(FifoQueue *); 
void destroyQueue(FifoQueue *);
