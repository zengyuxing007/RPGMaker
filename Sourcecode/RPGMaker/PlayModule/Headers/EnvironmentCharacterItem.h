#ifndef ENVIRONMENTCHARACTERITEM_H
#define ENVIRONMENTCHARACTERITEM_H

#include "qpoint.h"
#include "qtimer.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "../Components/Headers/Character.h"
#include "../Components/Headers/Environment.h"

using projectcomponents::Character;
using projectcomponents::Environment;

namespace playmodule {

	class EnvironmentCharacterItem : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:

		enum Direction {NORTH, EAST, SOUTH, WEST};

		EnvironmentCharacterItem(QGraphicsItem *parent);
		virtual ~EnvironmentCharacterItem();

		Direction getDirection() const;
		Character* getCharacter() const;
		Environment* getEnvironment() const;
		QPoint getLocation() const;
		QPoint getDestination() const;
		QVector<QPoint> getRoute() const;

		void setCharacter(Character *character);
		void setEnvironment(Environment *environment);
		void setLocation(QPoint location);
		void setDestination(QPoint destination);
		void setRoute(QVector<QPoint> route);

		void initializeMovement();
		void finalizeMovement();

	private:
		void checkForTouchTrigger();
		void setCharacterFrame(QPoint frame);
		QPoint translateCellCoordinates(QPoint point);
		QPoint translateMapCoordinates(QPoint point);

		Direction _direction;

		QTimer *_animationTimer;
		Character *_character;
		Environment *_environment;

		QPoint _location;
		QPoint _destination;
		QVector<QPoint> _route;

	private slots:
		void animation();

	signals:
		void characterMoved();
		void eventTriggered(Event *event);
	};
}

#endif