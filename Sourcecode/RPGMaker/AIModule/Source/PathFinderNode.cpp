#include "../Headers/PathFinderNode.h"

using namespace aimodule;

/**
*	Constructor. <i>location</i> is the location of the search node in the environment,
*	<i>route</i> is the route from the Pathfinder AI's starting location to this node's location,
*	<i>value</i> is the expected distance until the destination is reached from this search node.
*/
PathFinderNode::PathFinderNode(QPoint location, QVector<QPoint> route, int value)
	: _location(location), _route(route), _value(value) {}

/**
*	Standard destructor.
*/
PathFinderNode::~PathFinderNode() {}

/**
*	Get the location of the search node in the environment.
*/
const QPoint& PathFinderNode::getLocation() const {
	return _location;
}

/**
*	Get the route from the Pathfinder AI's starinting location to the
*	location of this search node's location in the environment.
*/
const QVector<QPoint>& PathFinderNode::getRoute() const {
	return _route;
}

/**
*	Get the expected distance until the destination is reached from this search node.
*/
int PathFinderNode::getValue() const {
	return _value;
}