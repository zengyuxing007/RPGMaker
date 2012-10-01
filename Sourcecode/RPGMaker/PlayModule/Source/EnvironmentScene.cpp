#include "qsize.h"
#include "qevent.h"
#include "qvector.h"
#include "qpainter.h"
#include "qgraphicsview.h"

#include "../ProgramDefaults.h"
#include "../Headers/EnvironmentScene.h"
#include "../AIModule/Headers/PathFinderAI.h"

using namespace playmodule;
using aimodule::PathFinderAI;

EnvironmentScene::EnvironmentScene(QObject *parent, SoundItem *sound)
	: QGraphicsScene(parent), _activeEvent(0), _environment(0), _textEventHandler(0), 
	_character(0), _floor(0), _ceiling(0), _objects(0), _sound(sound) {
		setBackgroundBrush(QBrush(Qt::black));
}

EnvironmentScene::~EnvironmentScene() {}

void EnvironmentScene::initialize(QVector<GameEvent*> events, Environment *environment, Character *character, QPoint location) {
	_events = events;
	_floor = new EnvironmentFloorItem(0);
	_ceiling = new EnvironmentCeilingItem(0);
	_character = new EnvironmentCharacterItem(_floor);
	addItem(_floor);
	addItem(_ceiling);

	setEnvironment(environment);
	setCharacter(character);
	setCharacterLocation(location);
	setCharacterDestination(location);

	_character->setActive(true);

	if (_environment->getBackgroundMusic() != QString())
		_sound->setBackgroundSource(MediaSource(g_backgroundMusicPath + environment->getBackgroundMusic()));
	else
		_sound->stop();

	connect(_character, SIGNAL(eventTriggered(Event*)), this, SLOT(executeEvent(Event*)));
	connect(_character, SIGNAL(characterMoved()), this, SIGNAL(focusSignal()));
	connect(_character, SIGNAL(characterMoved()), this, SLOT(shuffleObjects()));
}

void EnvironmentScene::finalize() {
	clear();
	_activeEvent = 0;
	_environment = 0;
	_events.clear();
}

QGraphicsItem* EnvironmentScene::getFocusItem() const {
	return _character;
}

void EnvironmentScene::setEnvironment(Environment *currentEnvironment) {
	_environment = currentEnvironment;
	_floor->setEnvironment(currentEnvironment);
	_ceiling->setEnvironment(currentEnvironment);
	_character->setEnvironment(currentEnvironment);
	setSceneRect(_floor->pixmap().rect());

	setObjects(currentEnvironment);
}

void EnvironmentScene::setObjects(Environment *currentEnvironment) {
	QSize dimensions = currentEnvironment->getDimensions();

	for (int y = 0; y < dimensions.height(); y++) {
		for (int x = 0; x < dimensions.width(); x++) {
			Event *event = currentEnvironment->getEvent(QPoint(x, y));
			if (!!event && !!event->getObject()) {
				EnvironmentItem *object = new EnvironmentItem(_floor);
				object->setObject(event->getObject());
				object->setEnvironment(currentEnvironment);
				object->setLocation(translateCellCoordinates(QPoint(x, y)));
			}
		}
	}
	
}

void EnvironmentScene::setCharacter(Character *currentCharacter) {
	_character->setCharacter(currentCharacter);
}

void EnvironmentScene::setCharacterLocation(QPoint location) {
	_character->setLocation(location);
	emit focusSignal();
}

void EnvironmentScene::setCharacterDestination(QPoint destination) {
	_character->setDestination(destination);
}

void EnvironmentScene::setCharacterRoute(QVector<QPoint> route) {
	_character->setRoute(route);
}

QPoint EnvironmentScene::translateCellCoordinates(QPoint point) {
	QPoint mapCoordinates;
	mapCoordinates.setX(point.x()*g_tilesize + g_tilesize/2);
	mapCoordinates.setY(point.y()*g_tilesize + g_tilesize/2);
	return mapCoordinates;
}

QPoint EnvironmentScene::translateMapCoordinates(QPoint point) {
	QPoint cellCoordinates;
	cellCoordinates.setX(point.x()/g_tilesize);
	cellCoordinates.setY(point.y()/g_tilesize);
	return cellCoordinates;
}

void EnvironmentScene::keyPressEvent(QKeyEvent *event) {
	if (_character->getRoute().size() == 0) {
		if (_activeEvent == 0) {
			if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
				checkForInteractionTrigger();
				event->accept();
				return;
			} else if ((event->key() == Qt::Key_M) || (event->key() == Qt::Key_I)) {
				emit openMenu();
				event->accept();
				return;
			} else if (checkMovementTrigger(event)) {
				bool accepted = false;
				int x = 0, y = 0;

				switch (event->key()) {
				case Qt::Key_Up:
					if (_character->getLocation().y() - g_tilesize  > 0) {
						y = -g_tilesize;
						accepted = true;
					}
					break;
				case Qt::Key_Down:
					if (_character->getLocation().y() + g_tilesize < _floor->pixmap().height()) {
						y = g_tilesize;
						accepted = true;
					}
					break;
				case Qt::Key_Left:
					if (_character->getLocation().x() - g_tilesize > 0) {
						x = - g_tilesize;
						accepted = true;
					}
					break;
				case Qt::Key_Right:
					if (_character->getLocation().x() + g_tilesize < _floor->pixmap().width()) {
						x = g_tilesize;
						accepted = true;
					}
					break;
				}

				if (accepted) {
					QPoint destination(_character->getLocation().x() + x, _character->getLocation().y() + y);
					_character->setRoute(QVector<QPoint>(1, destination));
					_character->initializeMovement();
				}
			}

		} else {
			if (!!_activeEvent && (_activeEvent->getType() == Event::TEXT)) {
				_textEventHandler->keyPressEvent(event);
			}
		}
	}
}

void EnvironmentScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if (_activeEvent == 0) {
		QPoint cellLocation = translateMapCoordinates(QPoint(event->scenePos().x(), event->scenePos().y()));

		// If the location is not valid, then return.
		QSize dimensions = _environment->getDimensions();
		if ((cellLocation.x() < 0) || (cellLocation.x() >= dimensions.width()) || (cellLocation.y() < 0) || (cellLocation.y() >= dimensions.height()))
			return;

		// Trim down the current route of the character
		QVector<QPoint> route = _character->getRoute();

		PathFinderAI pathfinder;
		pathfinder.setEnvironment(_environment);
		pathfinder.setLocation(translateMapCoordinates(_character->getDestination()));
		pathfinder.setDestination(cellLocation);

		QVector<QPoint> cellLocations = pathfinder.calculateRoute();
		QVector<QPoint> mapLocations(0);
		for (int i = 0; i < cellLocations.size(); i++) {
			mapLocations.append(translateCellCoordinates(cellLocations.at(i)));
		}

		if (!route.isEmpty())
			mapLocations.prepend(route.first());

		_character->setRoute(mapLocations);
		if (_character->getLocation() == _character->getDestination())
			_character->initializeMovement();
	}
}

bool EnvironmentScene::checkMovementTrigger(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Up:
	case Qt::Key_Down:
	case Qt::Key_Right:
	case Qt::Key_Left:
		return true;
	}

	return false;
}

void EnvironmentScene::checkForInteractionTrigger() {
	QPoint cellLocation = translateMapCoordinates(_character->getLocation());
	QSize dimensions = _environment->getDimensions();

	switch (_character->getDirection()) {
	case EnvironmentCharacterItem::NORTH:
		cellLocation.setY(cellLocation.y() -1);
		break;
	case EnvironmentCharacterItem::EAST:
		cellLocation.setX(cellLocation.x() +1);
		break;
	case EnvironmentCharacterItem::SOUTH:
		cellLocation.setY(cellLocation.y() +1);
		break;
	case EnvironmentCharacterItem::WEST:
		cellLocation.setX(cellLocation.x() -1);
		break;
	}

	if ((cellLocation.x() >= 0) && (cellLocation.x() < dimensions.width()) && (cellLocation.y() >= 0) && (cellLocation.y() < dimensions.height())) {
		Event *event = _environment->getEvent(cellLocation);

		if (!!event && (event->getTrigger() == Event::INTERACT))
			executeEvent(event);
	}
}

void EnvironmentScene::currentEventFinished() {
	switch (_activeEvent->getType()) {
	case Event::TEXT:
		removeItem(_textEventHandler);
		delete _textEventHandler;
		_textEventHandler = 0;
		break;
	case Event::BATTLE:
		if (_environment->getBackgroundMusic() != QString())
			_sound->setBackgroundSource(MediaSource(g_backgroundMusicPath + _environment->getBackgroundMusic()));
		else
			_sound->stop();
		break;
	}

	_activeEvent->execute();
	if (!!_activeEvent->getFollowupEvent())
		executeEvent(_activeEvent->getFollowupEvent()->getEvent());
	else
		_activeEvent = 0;
}

void EnvironmentScene::executeEvent(Event *event) {
	if (!!event) {

		GameEvent *gameEvent = 0;
		for (int i = 0; i < _events.size(); i++) {
			if (_events.at(i)->getEvent() == event) {
				gameEvent = _events.at(i);
				break;
			}
		}
		
		if (gameEvent->executionPermission()) {
			_activeEvent = gameEvent;

			switch (_activeEvent->getEvent()->getType()) {
			case Event::PORTAL:
				executePortalEvent(dynamic_cast<PortalEvent*>(event));
				break;
			case Event::TEXT:
				executeTextEvent(dynamic_cast<TextEvent*>(event));
				break;
			case Event::BATTLE:
				executeBattleEvent(dynamic_cast<BattleEvent*>(event));
				break;
			}
		}
	}
}

void EnvironmentScene::executeTextEvent(TextEvent *event) {

	QGraphicsView *parent = dynamic_cast<QGraphicsView*>(this->parent());
	QSize parentSize = parent->size();

	_textEventHandler = new TextEventHandler(0);
	_textEventHandler->setEvent(event);
	_textEventHandler->nextMessage();

	QPoint textLocation;
	textLocation.setX(parentSize.width()/2 - _textEventHandler->boundingRect().width()/2);
	textLocation.setY(parentSize.height() - _textEventHandler->boundingRect().height() - 30);
	_textEventHandler->setPos(parent->mapToScene(textLocation));

	addItem(_textEventHandler);
	connect(_textEventHandler, SIGNAL(eventFinishedSignal()), this, SLOT(currentEventFinished()));
}

void EnvironmentScene::executePortalEvent(PortalEvent *event) {
	Environment *environment = event->getEnvironment();

	if (!!environment) {
		QPoint location = event->getLocation();

		_character->setParentItem(0);
		removeItem(_character);
		clear();

		_floor = new EnvironmentFloorItem(0);
		_ceiling = new EnvironmentCeilingItem(0);
		addItem(_floor);
		addItem(_ceiling);
		_character->setParentItem(_floor);

		setEnvironment(environment);
		setCharacterLocation(translateCellCoordinates(location));
		setCharacterDestination(translateCellCoordinates(location));
		setCharacterRoute(QVector<QPoint>(0));
		currentEventFinished();

		if (_environment->getBackgroundMusic() != QString())
			_sound->setBackgroundSource(g_backgroundMusicPath + _environment->getBackgroundMusic());
		else
			_sound->stop();
	}
}

void EnvironmentScene::executeBattleEvent(BattleEvent *event) {
	int activation = event->getActivation();
	int random = qrand() % 101;

	if (random < activation) {
		QVector<QPair<Battlegroup*, int>> groups = event->getBattlegroups();
		if (groups.size() > 0) {
			int groupSelected = qrand() % (groups.size());
			Battlegroup *group = groups.at(groupSelected).first;
			if (!!group && (group->getEnemies().size() > 0)) {
				_character->setRoute(QVector<QPoint>(0));
				emit startBattle(_environment->getBattlebackImage(), group);
			} else {
				_activeEvent = 0;
			}
		}
	} else {
		_activeEvent = 0;
	}
}

void EnvironmentScene::shuffleObjects() {
	QList<QGraphicsItem*> items = collidingItems(_character);
	for (int i = 0; i < items.size(); i++) {
		QGraphicsItem *it = items.at(i);
		if ((it != _floor) && (it != _ceiling)) {
			QPointF characterPos = _character->pos();
			QPointF objectPos = it->pos();

			int characterHeight = _character->pixmap().height();
			int objectHeight = dynamic_cast<EnvironmentItem*>(it)->pixmap().height();

			_character->setParentItem(0);
			removeItem(_character);
			it->setParentItem(0);
			removeItem(it);

			if ((characterPos.y() + characterHeight) < (objectPos.y() + objectHeight)) {
				_character->setParentItem(_floor);
				it->setParentItem(_floor);
			} else {
				it->setParentItem(_floor);
				_character->setParentItem(_floor);
			}
		}
	}
}