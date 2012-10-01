#ifndef EQUIPMENTPIECE_H
#define EQUIPMENTPIECE_H

#include "qpair.h"
#include "qobject.h"
#include "qstring.h"
#include "qvector.h"

#include "Class.h"
#include "Element.h"
#include "Attribute.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The EquipmentPiece class is a base class for the equipment a character can wear.
	*	Look for the sublcasses <i>Weapon</i>, <i>Armor</i> and <i>Accessory</i>.
	*/
	class EquipmentPiece : public QObject {

		Q_OBJECT

	public:
		enum Type {WEAPON, ARMOR, ACCESSORY};

		virtual ~EquipmentPiece();

		QString getName() const;
		QString getDescription() const;
		Type getType() const;
		Element* getElement(QString name) const;
		QPair<Class*, int> getClass(QString name) const;
		QPair<Attribute*, int> getAttribute(QString name) const;

		const QVector<Element*>& getElements() const;
		const QVector<QPair<Class*, int>>& getClasses() const;
		const QVector<QPair<Attribute*, int>>& getAttributes() const;

		void setName(QString name) throw(ProjectException);
		void setDescription(QString newDescription);
		void setType(Type type);

		void addElement(Element *newElement) throw(ProjectException);
		void addAttribute(Attribute *newAttribute, int value) throw(ProjectException);
		void addAttribute(QPair<Attribute*, int> newAttribute) throw(ProjectException);
		void addClass(Class *cat, int level) throw (ProjectException);
		void addClass(QPair<Class*, int> cat) throw(ProjectException);

		void editAttribute(QString name, int value) throw (ProjectException);
		void editClass(QString name, int level) throw (ProjectException);

		void removeElement(Element *oldElement) throw(ProjectException);
		void removeElement(QString oldElement) throw(ProjectException);
		void removeAttribute(Attribute* oldAttribute) throw(ProjectException);
		void removeAttribute(QString oldAttribute) throw(ProjectException);
		void removeClass(Class *cat) throw(ProjectException);
		void removeClass(QString name) throw(ProjectException);

	protected:
		EquipmentPiece(QString name, Type type);

		Type _type;
		QString _name;
		QString _description;
		QVector<Element*> _elements;
		QVector<QPair<Attribute*, int>> _attributes;
		QVector<QPair<Class*, int>> _classes;

	signals:
		void deleteSignal(QString name);

	private slots:
		void deleteElement(QString name);
		void deleteAttribute(QString name);
		void deleteClass(QString name);
	};
}

#endif