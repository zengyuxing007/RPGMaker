#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "qdom.h"
#include "qstring.h"
#include "qvector.h"
#include "qobject.h"
#include "ProjectException.h"

namespace projectcomponents {

	/**
	*	The Attribute class represents a physical property of a <i>Character</i>, <i>Enemy</i> or <i>EquipmentPiece</i>.
	*/
	class Attribute : public QObject {

		Q_OBJECT

	public:
		enum Type {LIFE, ENERGY, AGILITY, EVASION, OFFENSIVE, DEFENSIVE};

		~Attribute();

		Type getType() const;
		QString getName() const;
		Attribute* getLink() const;

		void setName(QString name) throw (ProjectException);
		void setLink(Attribute *attribute) throw (ProjectException);

		void unsetLink();

		static Attribute* create(QString name, Type type) throw (ProjectException);
		static Attribute* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(Attribute *attribute, QDomDocument &document, QDomElement &parent);

	private:
		Attribute(QString name, Type type);

		Type _type;
		QString _name;
		Attribute *_link;

	signals:
		void deleteSignal(QString name);

	private slots:
		void attributeDeleted(QString name);
	};

}

#endif