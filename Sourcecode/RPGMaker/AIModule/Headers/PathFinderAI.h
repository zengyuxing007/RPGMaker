#ifndef PATHFINDERAI_H
#define PATHFINDERAI_H

#include "qpoint.h"

#include "../Components/Headers/Environment.h"

namespace aimodule {

	/**
	*	The Pathfinder AI will search in a given environment, from a given location, to a given destination
	*	the optimal path from the location to the destination, if any. The Pathfinder AI uses A* Search and
	*	the Manhattan distance heuristic to find its goal.
	*/
	class PathFinderAI {
	public:
		PathFinderAI();
		virtual ~PathFinderAI();

		Environment* getEnvironment() const;
		QPoint getLocation() const;
		QPoint getDestination() const;

		void setEnvironment(Environment *environment);
		void setLocation(QPoint location);
		void setDestination(QPoint destination);

		QVector<QPoint> calculateRoute() const;

	private:

		bool isGoalState(QPoint location) const;
		int heuristic(QPoint location) const;
		QVector<QPoint> generateSuccessors(QPoint location) const;

		Environment *_environment;
		QPoint _location;
		QPoint _destination;

	};
}

#endif