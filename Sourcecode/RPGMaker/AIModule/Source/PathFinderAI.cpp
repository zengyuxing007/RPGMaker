#include "../Headers/PathFinderAI.h"
#include "../Headers/PathFinderNode.h"
#include "../Headers/PathFinderQueue.h"

using namespace aimodule;

/**
*	Standard constructor.
*/
PathFinderAI::PathFinderAI()
	: _environment(0), _location(0, 0), _destination(0, 0) {}

/**
*	Standard destructor.
*/
PathFinderAI::~PathFinderAI() {}

/**
*	Get the environment the Pathfinder AI is currently set to.
*/
Environment* PathFinderAI::getEnvironment() const {
	return _environment;
}

/**
*	Get the location the Pathfinder AI from which the Pathfinder AI will start its search.
*/
QPoint PathFinderAI::getLocation() const {
	return _location;
}

/**
*	Get the destination the Pathfinder AI will try to reach.
*/
QPoint PathFinderAI::getDestination() const {
	return _destination;
}

/**
*	Set the environment in which the Pathfinder AI should search.
*/
void PathFinderAI::setEnvironment(Environment *environment) {
	_environment = environment;
}

/**
*	Set the location from which the Pathfinder AI should start its search.
*/
void PathFinderAI::setLocation(QPoint location) {
	_location = location;
}

/**
*	Set the destination the Pathfinder AI should try to reach.
*/
void PathFinderAI::setDestination(QPoint destination) {
	_destination = destination;
}

/**
*	Start to search for an optimal route in the current environment, from the current location to the desired destination.
*	The Pathfinder AI will use the A* Search algorithm with the Manhattan distance heuristic. If no route is found, it will
*	return an empty route. The Pathfinder AI will will take non-accessible and objects in the environment into account.
*/
QVector<QPoint> PathFinderAI::calculateRoute() const {
	if (_environment->getAccess(_destination) == Tileset::NONE)
		return QVector<QPoint>(0);

	PathFinderNode startNode(_location, QVector<QPoint>(0), heuristic(_location));

	QVector<QPoint> explored(0);
	PathFinderQueue frontier;
	frontier.push(startNode);

	while (true) {	
		if (frontier.isEmpty())
			return QVector<QPoint>(0);

		PathFinderNode node = frontier.pop();
		if (isGoalState(node.getLocation()))
			return node.getRoute();

		if (!explored.contains(node.getLocation())) {
			explored.append(node.getLocation());
			QVector<QPoint> successors = generateSuccessors(node.getLocation());
			for (int i = 0; i < successors.size(); i++) {
				QPoint successor = successors.at(i);

				QVector<QPoint> route(0);
				route << node.getRoute() << successor;

				frontier.push(PathFinderNode(successor, route, route.size() + heuristic(successor)));
			}
		}
	}

}

bool PathFinderAI::isGoalState(QPoint location) const {
	if (location == _destination)
		return true;
	else
		return false;
}

int PathFinderAI::heuristic(QPoint location) const {
	int value;

	value = qAbs(location.x() - _destination.x()) + qAbs(location.y() - _destination.y());

	return value;
}

QVector<QPoint> PathFinderAI::generateSuccessors(QPoint location) const {
	QVector<QPoint> successors(0);

	if ((location.x() -1) >= 0) {
		QPoint left(location.x()-1, location.y());
		if (_environment->getAccess(left) != Tileset::NONE)
			successors.append(left);
	}

	if ((location.x() +1) < _environment->getDimensions().width()) {
		QPoint right(location.x()+1, location.y());
		if (_environment->getAccess(right) != Tileset::NONE)
			successors.append(right);
	}

	if ((location.y() -1) >= 0) {
		QPoint north(location.x(), location.y()-1);
		if (_environment->getAccess(north) != Tileset::NONE)
			successors.append(north);
	}

	if ((location.y() +1) < _environment->getDimensions().height()) {
		QPoint south(location.x(), location.y()+1);
		if (_environment->getAccess(south) != Tileset::NONE)
			successors.append(south);
	}

	return successors;
}