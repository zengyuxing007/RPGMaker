#ifndef EVENT_H
#define EVENT_H

#include "qdom.h"
#include "qpoint.h"
#include "qobject.h"
#include "qstring.h"

#include "Object.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	An Event represents an abstract event that may take place in an environment.
	*	The Event class is a base class and cannot be instantiated.
	*	Look for the subclasses <i>TextEvent</i>, <i>PortalEvent</i> and <i>BattleEvent</i>.
	*/
	class Event: public QObject {

		Q_OBJECT

	public:
		enum Type {PORTAL, TEXT, BATTLE};
		enum Trigger {TOUCH, INTERACT, NONE};

		Event();
		virtual ~Event();

		virtual void execute() =0;

		Type getType() const;
		int getLimit() const;
		QString getName() const;
		Object* getObject() const;
		Trigger getTrigger() const;
		Event* getFollowup() const;
		Event* getRequirement(QString name) const;
		QVector<Event*> getRequirements() const;

		void setName(QString ID) throw (ProjectException);
		void setTrigger(Trigger trigger);
		void setLimit(int limit) throw (ProjectException);
		void setObject(Object *object) throw (ProjectException);
		void setFollowup(Event *event) throw (ProjectException);
		void setRequirements(QVector<Event*> requirements);

		void addRequirement(Event *event) throw (ProjectException);

		void unsetObject();
		void unsetFollowup();

		void removeRequirement(QString name);

	protected:
		Event(QString name, Type type, Trigger trigger);

		static void Event::translateFromXML(QDomElement element, Event *event);
		static void Event::translateToXML(Event *event, QDomDocument &document, QDomElement &parent);

		Type _type;
		int _limit;
		QString _name;
		Object *_object;
		Trigger _trigger;
		Event *_followup;
		QVector<Event*> _requirements;

	private slots:
		void objectDeleted(QString name);
		void eventDeleted(QString name);

	signals:
		void deleteSignal(QString ID);
	};
}

#endif