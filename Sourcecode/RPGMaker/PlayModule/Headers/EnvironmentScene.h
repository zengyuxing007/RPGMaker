#ifndef ENVIRONMENTSCENE_H
#define ENVIRONMENTSCENE_H

#include "qpoint.h"
#include "qevent.h"
#include "qtimer.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qgraphicssceneevent.h"

#include "phonon/mediaobject.h"
#include "phonon/audiooutput.h"

#include "SoundItem.h"
#include "GameEvent.h"
#include "EnvironmentItem.h"
#include "TextEventHandler.h"
#include "EnvironmentFloorItem.h"
#include "EnvironmentCeilingItem.h"
#include "EnvironmentCharacterItem.h"
#include "../Components/Headers/Character.h"
#include "../Components/Headers/TextEvent.h"
#include "../Components/Headers/Environment.h"
#include "../Components/Headers/PortalEvent.h"
#include "../Components/Headers/BattleEvent.h"

using Phonon::MediaObject;
using Phonon::AudioOutput;
using projectcomponents::Environment;
using projectcomponents::Character;

namespace playmodule {

	class EnvironmentScene : public QGraphicsScene {

		Q_OBJECT

	public:
		enum Direction {NORTH, EAST, SOUTH, WEST};

		EnvironmentScene(QObject *parent, SoundItem *sound);
		virtual ~EnvironmentScene();
		void initialize(QVector<GameEvent*> events, Environment *environment, Character *character, QPoint location);
		void finalize();

		QGraphicsItem* getFocusItem() const;

		void setEnvironment(Environment *currentEnvironment);
		void setObjects(Environment *currentEnvironment);
		void setCharacter(Character *currentCharacter);
		void setCharacterLocation(QPoint location);
		void setCharacterDestination(QPoint destination);
		void setCharacterRoute(QVector<QPoint> route);

		QPoint translateCellCoordinates(QPoint point);
		QPoint translateMapCoordinates(QPoint point);

	protected:
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:
		bool checkMovementTrigger(QKeyEvent *event);
		void checkForInteractionTrigger();

		// State
		GameEvent *_activeEvent;
		Environment *_environment;
		QVector<GameEvent*> _events;
		TextEventHandler *_textEventHandler;

		// Rendering
		EnvironmentCharacterItem *_character;
		EnvironmentFloorItem *_floor;
		EnvironmentCeilingItem *_ceiling;
		QVector<EnvironmentItem*> _objects;

		// Sound
		SoundItem *_sound;

	public slots:
		void currentEventFinished();

	private slots:
		void executeEvent(Event *event);
		void executeTextEvent(TextEvent *event);
		void executePortalEvent(PortalEvent *event);
		void executeBattleEvent(BattleEvent *event);
		void shuffleObjects();

	signals:
		void focusSignal();
		void openMenu();
		void startBattle(const QPixmap &background, Battlegroup *enemies);
	};

}

#endif