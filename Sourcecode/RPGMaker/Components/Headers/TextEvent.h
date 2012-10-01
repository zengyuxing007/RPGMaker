#ifndef TEXTEVENT_H
#define TEXTEVENT_H

#include "qvector.h"
#include "qstring.h"

#include "Event.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	A TextEvent represents a text event in an environment.
	*	It can contain several message blocks.
	*	TextEvent is a subclass of <i>Event</i>.
	*/
	class TextEvent : public Event {

		Q_OBJECT

	public:
		virtual ~TextEvent();

		void execute();

		QVector<QString> getMessages() const;
		QString getMessage(int position) const throw (ProjectException);

		void addMessage(QString message);
		void editMessage(QString message, int position) throw (ProjectException);
		void removeMessage(int position) throw (ProjectException);
		void removeAllMessages();

		static TextEvent* create(QString ID, Event::Trigger trigger) throw (ProjectException);
		static TextEvent* translateFromXML(QDomElement element) throw (ProjectException);
		static void translateToXML(TextEvent *event, QDomDocument &document, QDomElement &parent);

	private:
		TextEvent(QString ID, Event::Trigger trigger);

		QVector<QString> _messages;
	};
}

#endif