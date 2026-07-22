#include <string.h>
#include <stdlib.h>
#include "fifo.h"

FifoQueue *mkQueue() {
	FifoQueue *queue = malloc(sizeof (FifoQueue));
	if (!queue)
		return NULL;
	memset(queue, 0, sizeof (FifoQueue));
	return queue;
}

void *addToQueue(FifoQueue *queue, void *object, size_t objectSize) {
	if (queue->objects == NULL) {
		void **objects = malloc(sizeof (void *));
		if (!objects) 
			return NULL;
		queue->objects = objects;
		queue->size++;
	} else {
		void **objects = realloc(queue->objects, sizeof (void *) * ++queue->size);
		if (!objects)
			return NULL;
		queue->objects = objects;
	}
	void *objectBuffer = malloc(objectSize);
	if (!objectBuffer)
		return NULL;
	memcpy(objectBuffer, object, objectSize);
	queue->objects[queue->size - 1] = objectBuffer;
	return objectBuffer;
}

void *popFromQueue(FifoQueue *queue) {
	if (queue->size == 0)
		return NULL;
	else {
		/* perform a realloc, but truncate from the front */
		void *poppedObject = queue->objects[0]; /* save the element we're about to pop, we need to return this later */
		size_t newSize = sizeof (void *) * (--queue->size); /* calculate the new decreased size of the queue */
		void **newObjects = malloc(newSize);
		if (!newObjects)
			return NULL;
		memcpy(newObjects, &queue->objects[1], newSize);
		free(queue->objects);
		queue->objects = newObjects;
		return poppedObject;
	}
}

void destroyQueue(FifoQueue *queue) {
	for (long i = 0; i < queue->size; i++)
		free(queue->objects[i]);
	free(queue->objects);
	free(queue);
}
