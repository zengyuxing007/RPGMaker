#ifndef WEAPON_H
#define WEAPON_H

#include "qobject.h"
#include "qstring.h"
#include "qvector.h"

#include "EquipmentPiece.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	A Weapon represents a piece of equipment that bootst a <i>Character</i>'s physical power.
	*/
	class Weapon : public EquipmentPiece {

	public:
		enum Subtype {MAINHAND = 0, OFFHAND = 1, TWOHANDED = 2};

		virtual ~Weapon();

		Subtype getSubtype() const;
		void setSubtype(Subtype type);

		static Weapon* create(QString name, Subtype type) throw (ProjectException);
		static Weapon* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(Weapon *weapon, QDomDocument &document, QDomElement &parent);

	private:
		Weapon(QString name, Subtype type);

		Subtype _subtype;
	};

}

#endif