#ifndef ELEMENT_H
#define ELEMENT_H

#include "qdom.h"
#include "qstring.h"
#include "qvector.h"
#include "qobject.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	The Element class represents the a natural element in the gameworld.
	*	It can be linked to other elements to create an elemental circle.
	*/
	class Element : public QObject {

		Q_OBJECT

	public:
		~Element();

		QString getName() const;
		void setName(QString name) throw (ProjectException);

		const QVector<Element*>& getResistantElements() const;
		const QVector<Element*>& getVulnerableElements() const;

		bool isResistant(Element *element) const;
		bool isResistant(QString elementName) const;
		bool isVulnerable(Element *element) const;
		bool isVulnerable(QString elementName) const;

		void addResistantElement(Element *newElement) throw (ProjectException);
		void addVulnerableElement(Element *newElement) throw (ProjectException);

		void removeResistantElement(Element *oldElement) throw (ProjectException);
		void removeResistantElement(QString oldElement) throw (ProjectException);
		void removeVulnerableElement(Element *oldElement) throw (ProjectException);
		void removeVulnerableElement(QString oldElement) throw (ProjectException);

		static Element* create(QString name) throw (ProjectException);
		static Element* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(Element *element, QDomDocument &document, QDomElement &parent);

	private:
		Element(QString name);

		QString _name;
		QVector<Element*> _resistant;
		QVector<Element*> _vulnerable;

	signals:
		void deleteSignal(QString name);

	private slots:
		void deleteResistant(QString name);
		void deleteVulnerable(QString name);
	};

}


#endif