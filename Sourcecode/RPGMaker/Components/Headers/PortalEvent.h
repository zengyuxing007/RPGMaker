#ifndef PORTALEVENT_H
#define PORTALEVENT_H

#include "qpoint.h"

#include "Event.h"
#include "Environment.h"
#include "ProjectException.h"

namespace projectcomponents {
	
	/**
	*	A PortalEvent represents a portal event in an environment.
	*	It allows a player to move between environments.
	*	It contains a link to an environment and a location in that
	*	environment which is the destination of this portal event.
	*	PortalEvent is a subclass of <i>Event</i>.
	*/
	class PortalEvent: public Event {

		Q_OBJECT

	public:
		virtual ~PortalEvent();

		void execute();

		QPoint getLocation() const;
		Environment *getEnvironment() const;

		void setDestination(Environment *environment, QPoint location) throw (ProjectException);
		void unsetDestination();

		static PortalEvent* create(QString ID, Event::Trigger trigger) throw (ProjectException);
		static PortalEvent* translateFromXML(QDomElement element) throw (ProjectException);
		static void translateToXML(PortalEvent *event, QDomDocument &document, QDomElement &parent);
		
	private:
		PortalEvent(QString ID, Event::Trigger trigger);

		QPoint _location;
		Environment *_environment;

	private slots:
		void environmentDeleted(QString name);
		void environmentResized(QString name);
	};
}

#endif