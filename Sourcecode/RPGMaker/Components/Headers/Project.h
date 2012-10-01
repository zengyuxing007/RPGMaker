#ifndef PROJECT_H
#define PROJECT_H

#include "qobject.h"
#include "qvector.h"
#include "qstring.h"

#include "Properties.h"
#include "StartEvent.h"
#include "EventSystem.h"
#include "EnemySystem.h"
#include "ObjectSystem.h"
#include "CharacterSystem.h"
#include "BattlegroupSystem.h"
#include "EnvironmentSystem.h"

namespace projectcomponents {

	/**
	*	The Project class contains and managaes all of the project data.
	*	It contains an instance of the <i>Properties</i> class, objects and events, characters and enemies and environments.
	*/
	class Project : public QObject {

		Q_OBJECT

	public:
		virtual ~Project();

		QString getTitle() const;
		Properties* getProperties() const;
		EventSystem* getEvents() const;
		EnemySystem* getEnemies() const;
		ObjectSystem* getObjects() const;
		CharacterSystem* getCharacters() const;
		BattlegroupSystem* getBattlegroups() const;
		EnvironmentSystem* getEnvironments() const;
		StartEvent* getStartEvent() const;

		void setTitle(QString title) throw (ProjectException);
		void setEvents(EventSystem *events) throw (ProjectException);
		void setEnemies(EnemySystem* enemies) throw (ProjectException);
		void setObjects(ObjectSystem *objects) throw (ProjectException);
		void setCharacters(CharacterSystem *characters) throw (ProjectException);
		void setBattlegroups(BattlegroupSystem *battlegroups) throw (ProjectException);
		void setEnvironments(EnvironmentSystem *environments) throw (ProjectException);
		void setStartEvent(StartEvent *event) throw (ProjectException);

		static Project* translateFromXML(QDomElement &root)  throw (ProjectException);
		static void translateToXML(Project *project, QDomDocument &document);

		static Project* create(QString title) throw (ProjectException);
		static QString currentProjectPath;

	private:
		Project(QString title);
		Project(const Project&);
		Project& operator=(const Project&);

		QString _title;
		Properties *_properties;
		EventSystem *_events;
		EnemySystem *_enemies;
		ObjectSystem *_objects;
		CharacterSystem *_characters;
		BattlegroupSystem *_battlegroups;
		EnvironmentSystem *_environments;
		StartEvent *_start;

	signals:
		void removedCharacter(QString name);
		void removedEnemy(QString name);
		void removedBattlegroup(QString name);
		void removedEnvironment(QString name);
	};

}

#endif