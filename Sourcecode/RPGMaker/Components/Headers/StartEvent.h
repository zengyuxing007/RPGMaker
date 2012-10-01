#ifndef STARTEVENT_H
#define STARTEVENT_H

#include "qobject"
#include "qvector"

#include "Event.h"
#include "Armor.h"
#include "Weapon.h"
#include "Accessory.h"
#include "Character.h"
#include "Environment.h"

namespace projectcomponents {

	/**
	*	A StartEvent contains all starting information of a game such as
	*	the starting location and the initial party.
	*/
	class StartEvent : public QObject {

		Q_OBJECT

	public:
		virtual ~StartEvent();

		QPair<Environment*, QPoint> getLocation() const;
		int getCurrency() const;
		Character* getCharacter(QString name) const throw (ProjectException);
		Character* getCharacter(int position) const throw (ProjectException);
		QVector<Character*> getParty() const;

		void setLocation(Environment *environment, QPoint location) throw (ProjectException);
		void setCurrency(int currency) throw (ProjectException);
		void addCharacter(Character *character) throw (ProjectException);

		void removeCharacter(QString name) throw (ProjectException);
		void removeCharacter(int position) throw (ProjectException);
		void removeLocation();

		static StartEvent* create() throw (ProjectException);
		static StartEvent* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(StartEvent *event, QDomDocument &document, QDomElement &parent);

	private:
		StartEvent();

		int _currency;
		QVector<Character*> _characters;
		QPair<Environment*, QPoint> _location;

	private slots:
		void characterDeleted(QString name);
	};
}

#endif