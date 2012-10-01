#ifndef BATTLEEVENT_H
#define BATTLEEVENT_H

#include "qstring.h"
#include "qvector.h"

#include "Event.h"
#include "Battlegroup.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	A BattleEvent represents the start sign to initiate battle in the game.
	*	It contains a list of battlegroups which can be put against the party of the player.
	*	BattleEvent is a subclass of <i>Event</i>.
	*/
	class BattleEvent : public Event {

		Q_OBJECT

	public:
		virtual ~BattleEvent();

		void execute();

		int getActivation() const;
		const QVector<QPair<Battlegroup*, int>>& getBattlegroups() const;
		QPair<Battlegroup*, int> getBattlegroup(QString name) const throw (ProjectException);

		void addBattlegroup(Battlegroup *battlegroup, int chance) throw (ProjectException);
		void setActivation(int activation) throw (ProjectException);
		void removeBattlegroup(QString name) throw (ProjectException);

		static BattleEvent* create(QString ID, Event::Trigger trigger) throw (ProjectException);
		static BattleEvent* translateFromXML(QDomElement element) throw (ProjectException);
		static void translateToXML(BattleEvent *event, QDomDocument &document, QDomElement &parent);

	private:
		BattleEvent(QString ID, Event::Trigger trigger);

		int _activation;
		QVector<QPair<Battlegroup*, int>> _groups;

	private slots:
		void battlegroupDeleted(QString name);
	};
}

#endif