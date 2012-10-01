#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "Class.h"
#include "qobject.h"
#include "qstring.h"
#include "qvector.h"

#include "EquipmentPiece.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The Accessory class represents a piece of equipment that grants a <i>Character</i> special properties.
	*/
	class Accessory : public EquipmentPiece {

	public:
		enum Subtype {EARRING = 7, NECKLACE = 8, RING = 9};

		virtual ~Accessory();

		Subtype getSubtype() const;
		void setSubtype(Subtype type);

		static Accessory* create(QString name, Subtype type) throw (ProjectException);
		static Accessory* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(Accessory *accessory, QDomDocument &document, QDomElement &parent);

	private:
		Accessory(QString name, Subtype type);

		Subtype _subtype;
	};

}

#endif