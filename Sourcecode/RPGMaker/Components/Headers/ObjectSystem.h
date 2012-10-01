#ifndef OBJECTSYSTEM_H
#define OBJECTSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Object.h"

namespace projectcomponents {

	/**
	*	The ObjectSystem is a management system for Objects.
	*/
	class ObjectSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~ObjectSystem();

		const QVector<Object*>& getObjects() const;
		Object* getObject(QString name) const throw (ProjectException);

		void addObject(Object *object) throw (ProjectException);

		void removeObject(QString name) throw (ProjectException);

		static ObjectSystem* create();
		static ObjectSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(ObjectSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		ObjectSystem();

		QVector<Object*> _objects;
	};
}

#endif