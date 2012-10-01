#include "../ProgramDefaults.h"
#include "../Headers/EnvironmentCharacterItem.h"

using namespace playmodule;

EnvironmentCharacterItem::EnvironmentCharacterItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent),
	_direction(SOUTH),
	_animationTimer(new QTimer(this)),
	_character(0),
	_environment(0),
	_location(0, 0),
	_destination(0, 0),
	_route(0) {

		_animationTimer->setInterval(50);
		_animationTimer->setSingleShot(true);

		connect(_animationTimer, SIGNAL(timeout()), this, SLOT(animation()));
}

EnvironmentCharacterItem::~EnvironmentCharacterItem() {}

EnvironmentCharacterItem::Direction EnvironmentCharacterItem::getDirection() const {
	return _direction;
}

Character* EnvironmentCharacterItem::getCharacter() const {
	return _character;
}

Environment* EnvironmentCharacterItem::getEnvironment() const {
	return _environment;
}

QPoint EnvironmentCharacterItem::getLocation() const {
	return _location;
}

QPoint EnvironmentCharacterItem::getDestination() const {
	return _destination;
}

QVector<QPoint> EnvironmentCharacterItem::getRoute() const {
	return _route;
}

void EnvironmentCharacterItem::setCharacter(Character *character) {
	_character = character;

	switch (_direction) {
	case NORTH:
		setCharacterFrame(QPoint(0, 3));
		break;
	case EAST:
		setCharacterFrame(QPoint(0, 2));
		break;
	case SOUTH:
		setCharacterFrame(QPoint(0, 0));
		break;
	case WEST:
		setCharacterFrame(QPoint(0, 1));
		break;
	}
}

void EnvironmentCharacterItem::setEnvironment(Environment *environment) {
	_environment = environment;
}

void EnvironmentCharacterItem::setLocation(QPoint location) {
	_location = location;

	QPoint newCharacterPosition;
	QSize characterDimensions = pixmap().size();

	newCharacterPosition.setX(location.x() - characterDimensions.width()/2);
	newCharacterPosition.setY(location.y() - characterDimensions.height() + g_tilesize/2);

	setPos(newCharacterPosition);
	emit characterMoved();
}

void EnvironmentCharacterItem::setDestination(QPoint destination) {
	_destination = destination;
}

void EnvironmentCharacterItem::setRoute(QVector<QPoint> route) {
	_route = route;
}

void EnvironmentCharacterItem::initializeMovement() {
	if (_route.size() > 0) {
		QPoint nextDestination = _route.front();

		int x = nextDestination.x() - _location.x();
		int y = nextDestination.y() - _location.y();

		if (x == 0) {
			if (y < 0) {
				_direction = NORTH;
				setCharacterFrame(QPoint(0, 3));
			} else {
				_direction = SOUTH;
				setCharacterFrame(QPoint(0, 0));
			}
		} else {
			if (x < 0) {
				_direction = WEST;
				setCharacterFrame(QPoint(0, 1));
			} else {
				_direction = EAST;
				setCharacterFrame(QPoint(0, 2));
			}
		}

		if (_environment->getAccess(translateMapCoordinates(nextDestination)) != Tileset::NONE) {
			_destination = nextDestination;
			_animationTimer->start();
		} else {
			_route.pop_front();
		}
	}
}

void EnvironmentCharacterItem::finalizeMovement() {
	checkForTouchTrigger();

	if (_route.size() > 0)
		_route.remove(0);
}

void EnvironmentCharacterItem::checkForTouchTrigger() {
	QPoint cellLocation = translateMapCoordinates(_location);
	Event *event = _environment->getEvent(cellLocation);

	if (!!event && (event->getTrigger() == Event::TOUCH))
		emit eventTriggered(event);
}

void EnvironmentCharacterItem::setCharacterFrame(QPoint frame) {
	QPixmap characterFrame = _character->getEnvironmentImage().first;
	QSize frameDimension(characterFrame.width()/4, characterFrame.height()/4);

	setPixmap(characterFrame.copy(frame.x() * frameDimension.width(), frame.y() * frameDimension.height(), frameDimension.width(), frameDimension.height()));
}

QPoint EnvironmentCharacterItem::translateCellCoordinates(QPoint point) {
	QPoint mapCoordinates;
	mapCoordinates.setX(point.x()*g_tilesize + g_tilesize/2);
	mapCoordinates.setY(point.y()*g_tilesize + g_tilesize/2);
	return mapCoordinates;
}

QPoint EnvironmentCharacterItem::translateMapCoordinates(QPoint point) {
	QPoint cellCoordinates;
	cellCoordinates.setX(point.x()/g_tilesize);
	cellCoordinates.setY(point.y()/g_tilesize);
	return cellCoordinates;
}

void EnvironmentCharacterItem::animation() {
	int x = 0, y = 0, frame = 0;

	QPoint characterFrame(0, 0);
	switch (_direction) {
	case NORTH:
		characterFrame.setY(3);
		y = - (g_tilesize/4);
		frame = qAbs(_destination.y() - _location.y());
		break;
	case SOUTH:
		characterFrame.setY(0);
		y = g_tilesize/4;
		frame = qAbs(_destination.y() - _location.y());
		break;
	case WEST:
		characterFrame.setY(1);
		x = - (g_tilesize/4);
		frame = qAbs(_destination.x() - _location.x());
		break;
	case EAST:
		characterFrame.setY(2);
		x = g_tilesize/4;
		frame = qAbs(_destination.x() - _location.x());
		break;
	}

	switch (frame) {
	case g_tilesize:
		characterFrame.setX(1);
		break;
	case g_tilesize*3/4:
		characterFrame.setX(2);
		break;
	case g_tilesize/2:
		characterFrame.setX(3);
		break;
	case g_tilesize/4:
		characterFrame.setX(0);
		break;
	}

	QPoint newLocation(_location.x() + x, _location.y() + y);
	setLocation(newLocation);
	setCharacterFrame(characterFrame);

	if (_location != _destination) {
		_animationTimer->start();
	} else {
		finalizeMovement();
		initializeMovement();
	}
}