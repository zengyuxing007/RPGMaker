#ifndef PATHFINDERQUEUE_H
#define PATHFINDERQUEUE_H

#include "qlinkedlist.h"

#include "PathFinderNode.h"

namespace aimodule {

	/**
	*	A simple First In, First Out queue because Qt did not provide one.
	*/
	class PathFinderQueue {
	public:

		PathFinderQueue();
		virtual ~PathFinderQueue();

		bool isEmpty();

		void push(PathFinderNode node);
		PathFinderNode pop();

	private:
		QLinkedList<PathFinderNode> _queue;
	};
}

#endif