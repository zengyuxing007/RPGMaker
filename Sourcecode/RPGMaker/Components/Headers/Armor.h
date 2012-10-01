#ifndef ARMOR_H
#define ARMOR_H

#include "Class.h"
#include "qobject.h"
#include "qstring.h"
#include "qvector.h"

#include "EquipmentPiece.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The Armor class represents a piece of equipment that protects a <i>Character</i>.
	*/
	class Armor : public EquipmentPiece {

	public:
		enum Subtype {HELMET = 2, TORSO = 3, LEGGINGS = 4, GLOVES = 5, BOOTS = 6};

		virtual ~Armor();

		Subtype getSubtype() const;
		void setSubtype(Subtype type);

		static Armor* create(QString name, Subtype type) throw (ProjectException);
		static Armor* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(Armor *armor, QDomDocument &document, QDomElement &parent);

	private:
		Armor(QString name, Subtype type);

		Subtype _subtype;
	};

}

#endif