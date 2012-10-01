#ifndef ABILITY_H
#define ABILITY_H

#include "qpair.h"
#include "qstring.h"
#include "qvector.h"
#include "Element.h"
#include "qobject.h"
#include "Attribute.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	An ability is a special action that a <i>Character</i> or <i>Enemy</i> can do in battle.
	*	It can be linked with several Elements and takes an Attribute of the Character or
	*	Enemy and multiplies it with a certain value.
	*/
	class Ability : public QObject{

		Q_OBJECT

	public:

		enum Type {OFFENSIVE, DEFENSIVE};

		~Ability();

		int getCost() const;
		Type getType() const;
		QString getName() const;
		QString getDescription() const;
		Element* getElement(QString name) const;
		const QVector<Element*>& getElements() const;
		QPair<Attribute*, double> getAttribute() const;

		void setCost(int cost) throw (ProjectException);
		void setName(QString name) throw (ProjectException);
		void setType(Ability::Type type);
		void setDescription(QString newDescription);
		void setAttribute(Attribute *attribute, double multiplier) throw (ProjectException);
		
		void addElement(Element *newElement) throw (ProjectException);
		void editAttribute(double multiplier) throw (ProjectException);

		void removeElement(Element *oldElement) throw (ProjectException);
		void removeElement(QString oldElement) throw (ProjectException);
		void unsetAttribute();

		static Ability* create(QString name, Type type) throw (ProjectException);
		static Ability* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(Ability *ability, QDomDocument &document, QDomElement &parent);

	private:
		Ability(QString name, Type type);

		int _cost;
		Type _type;
		QString _name;
		QString _description;
		QVector<Element*> _elements;
		QPair<Attribute*, double> _attribute;

	signals:
		void deleteSignal(QString name);

	private slots:
		void deleteElement(QString name);
		void deleteAttribute(QString name);

	};

}

#endif