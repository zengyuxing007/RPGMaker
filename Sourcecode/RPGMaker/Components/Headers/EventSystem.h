#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "qvector.h"

#include "Event.h"
#include "TextEvent.h"
#include "PortalEvent.h"
#include "BattleEvent.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	The EventSystem class is a management system for Events.
	*/
	class EventSystem : public QObject{

		Q_OBJECT

	public:
		virtual ~EventSystem();

		QVector<Event*> getEvents() const;
		const QVector<TextEvent*>& getTextEvents() const;
		const QVector<PortalEvent*>& getPortalEvents() const;
		const QVector<BattleEvent*>& getBattleEvents() const;

		Event* getEvent(QString ID) const throw (ProjectException);
		TextEvent* getTextEvent(QString ID) const throw (ProjectException);
		PortalEvent* getPortalEvent(QString ID) const throw (ProjectException);
		BattleEvent* getBattleEvent(QString ID) const throw (ProjectException);

		void addEvent(Event *event) throw (ProjectException);
		void addTextEvent(TextEvent *event) throw (ProjectException);
		void addPortalEvent(PortalEvent *event) throw (ProjectException);
		void addBattleEvent(BattleEvent *event) throw (ProjectException);

		void removeEvent(QString name) throw (ProjectException);
		void removeTextEvent(QString name) throw (ProjectException);
		void removePortalEvent(QString name) throw (ProjectException);
		void removeBattleEvent(QString name) throw (ProjectException);

		static EventSystem* create() throw (ProjectException);
		static EventSystem* translateFromXML(QDomElement element) throw (ProjectException);
		static void translateToXML(EventSystem *system, QDomDocument &document, QDomElement &parent);
	private:
		EventSystem();

		QVector<TextEvent*> _textEvents;
		QVector<PortalEvent*> _portalEvents;
		QVector<BattleEvent*> _battleEvents;

	private slots:
		void textEventDeleted(QString ID);
		void portalEventDeleted(QString ID);
		void battleEventDeleted(QString ID);
	};
}

#endif