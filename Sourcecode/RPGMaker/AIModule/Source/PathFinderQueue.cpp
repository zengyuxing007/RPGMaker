#include "../Headers/PathFinderQueue.h"

using namespace aimodule;

/**
*	Standard constructor.
*/
PathFinderQueue::PathFinderQueue()
	: _queue() {}

/**
*	 Standard destructor.
*/
PathFinderQueue::~PathFinderQueue() {}

/**
*	Returns the status of this queue.
*	Returns true if the queue is empty.
*	Returns false otherwise.
*/
bool PathFinderQueue::isEmpty() {
	return _queue.isEmpty();
}

/**
*	Adds a Pathfinder search node at the back of the queue.
*/
void PathFinderQueue::push(PathFinderNode node) {
	if (_queue.isEmpty())
		_queue.append(node);
	else {
		QLinkedList<PathFinderNode>::iterator it;
		for (it = _queue.begin(); it != _queue.end(); it++) {
			if ((*it).getValue() > node.getValue()) {
				_queue.insert(it, node);
				return;
			}
		}

		_queue.insert(it, node);
	}
}

/**
*	removes the first Pathfinder node from the list and returns it to the caller.
*/
PathFinderNode PathFinderQueue::pop() {
	return _queue.takeFirst();
}