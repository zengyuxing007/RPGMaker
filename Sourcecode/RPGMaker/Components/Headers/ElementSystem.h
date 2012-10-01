#ifndef ELEMENTSYSTEM_H
#define ELEMENTSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Element.h"

namespace projectcomponents {

	/**
	*	The ElementSystem class is a management system for <i>Element</i>.
	*/
	class ElementSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~ElementSystem();

		const QVector<Element*>& getElements() const;
		Element* getElement(QString name) const throw (ProjectException);

		void addElement(Element *element) throw (ProjectException);

		void removeElement(QString name) throw (ProjectException);

		static ElementSystem* create();
		static ElementSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(ElementSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		ElementSystem();

		QVector<Element*> _elements;
	};
}

#endif