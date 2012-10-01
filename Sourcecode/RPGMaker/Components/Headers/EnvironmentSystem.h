#ifndef ENVIRONMENTSYSTEM_H
#define ENVIRONMENTSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Environment.h"

namespace projectcomponents {

	/**
	*	The EnvironmentSystem class is a management system for <i>Environment</i>.
	*/
	class EnvironmentSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~EnvironmentSystem();

		const QVector<Environment*>& getEnvironments() const;
		Environment* getEnvironment(QString name) const throw (ProjectException);

		void addEnvironment(Environment *environment) throw (ProjectException);

		void removeEnvironment(QString name) throw (ProjectException);

		static EnvironmentSystem* create();
		static EnvironmentSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(EnvironmentSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		EnvironmentSystem();

		QVector<Environment*> _environments;
	};
}

#endif