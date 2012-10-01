#ifndef BATTLEGROUPSYSTEM_H
#define BATTLEGROUPSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Battlegroup.h"

namespace projectcomponents {

	/**
	*	The BattlegroupSystem is a management system for battlegroups.
	*/
	class BattlegroupSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~BattlegroupSystem();

		const QVector<Battlegroup*>& getBattlegroups() const;
		Battlegroup* getBattlegroup(QString name) const throw (ProjectException);

		void addBattlegroup(Battlegroup *battlegroup) throw (ProjectException);

		void removeBattlegroup(QString name) throw (ProjectException);

		static BattlegroupSystem* create();
		static BattlegroupSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(BattlegroupSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		BattlegroupSystem();

		QVector<Battlegroup*> _battlegroups;
	};
}

#endif