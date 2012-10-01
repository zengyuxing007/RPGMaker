#ifndef ABILITYSYSTEM_H
#define ABILITYSYSTEM_H

#include "qdom.h"
#include "qvector.h"
#include "qobject.h"
#include "qstring.h"

#include "Ability.h"
#include "ProjectException.h"

using projectcomponents::Ability;
using projectcomponents::ProjectException;

namespace projectcomponents {

	/**
	*	The AbilitySystem class is a management system for <i>Ability</i>.
	*/
	class AbilitySystem : public QObject {

		Q_OBJECT

	public:
		virtual ~AbilitySystem();

		const QVector<Ability*>& getAbilities() const;
		Ability* getAbility(QString name) const;

		void addAbility(Ability *ability) throw (ProjectException);

		void removeAbility(QString name);

		bool isRegistered(Ability *ability);
		bool isRegistered(QString name);

		static AbilitySystem* create() throw (ProjectException);
		static AbilitySystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(AbilitySystem *abilitySystem, QDomDocument &document, QDomElement &parent) throw (ProjectException);

	private:
		AbilitySystem();
		QVector<Ability*> _abilities;
	};

}


#endif