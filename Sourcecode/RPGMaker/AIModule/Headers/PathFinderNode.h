#ifndef PATHFINDERNODE_H
#define PATHFINDERNODE_H

#include "qpoint.h"
#include "qvector.h"

namespace aimodule {

	/**
	*	A pathfinder node contains the information about a point in the search route.
	*	It contains the location in the environment, the route to get to this location and an expected value
	*	of the route that is still to come before the destination is reached.
	*/
	class PathFinderNode {
	public:
		PathFinderNode(QPoint location, QVector<QPoint> route, int value);
		virtual ~PathFinderNode();

		const QPoint& getLocation() const;
		const QVector<QPoint>& getRoute() const;
		int getValue() const;

	private:
		QPoint _location;
		QVector<QPoint> _route;
		int _value;
	};
}

#endif