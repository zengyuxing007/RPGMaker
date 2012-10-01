#ifndef CLASSSYSTEM_H
#define CLASSSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Class.h"
#include "AttributeSystem.h"

namespace projectcomponents {

	/**
	*	The ClassSystem class is a management system for <i>Class</i>.
	*/
	class ClassSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~ClassSystem();

		const QVector<Class*>& getClasses() const;
		Class* getClass(QString name) const throw (ProjectException);

		void addClass(Class *cat) throw (ProjectException);

		void removeClass(QString name) throw (ProjectException);

		static ClassSystem* create();
		static ClassSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(ClassSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		ClassSystem();

		QVector<Class*> _classes;
		AttributeSystem *_attributes;

	private slots:
		void addedAttribute(Attribute *attribute);

	signals:
		void addedClass(Class *cat);
	};
}

#endif