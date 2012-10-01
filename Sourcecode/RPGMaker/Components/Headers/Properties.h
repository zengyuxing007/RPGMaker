#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "qimage.h"
#include "qobject.h"
#include "qvector.h"

#include "ClassSystem.h"
#include "ElementSystem.h"
#include "TilesetSystem.h"
#include "AbilitySystem.h"
#include "EquipmentSystem.h"
#include "AttributeSystem.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The Properties class contains all the non-player related entities of the game.
	*	It is in control of Elements, Attributes, Abilities, Classes, Equipment and Tilesets.
	*/
	class Properties : public QObject {

		Q_OBJECT

	public:
		Properties();
		virtual ~Properties();

		ClassSystem* getClasses() const;
		ElementSystem* getElements() const;
		TilesetSystem* getTilesets() const;
		AbilitySystem* getAbilities() const;
		EquipmentSystem* getEquipment() const;
		AttributeSystem* getAttributes() const;
		
		void setClasses(ClassSystem *classSystem) throw (ProjectException);
		void setElements(ElementSystem *elementSystem) throw (ProjectException);
		void setTilesets(TilesetSystem *tilesetSystem) throw (ProjectException);
		void setAbilities(AbilitySystem *abilitySystem) throw (ProjectException);
		void setEquipment(EquipmentSystem *equipmentSystem) throw (ProjectException);
		void setAttributes(AttributeSystem *attributeSystem) throw (ProjectException);

		static Properties* translateFromXML(QDomElement &properties) throw (ProjectException);
		static void translateToXML(Properties *properties, QDomDocument &document, QDomElement &parent);

	private:
		ClassSystem *_classes;
		ElementSystem *_elements;
		TilesetSystem *_tilesets;
		AbilitySystem *_abilities;
		EquipmentSystem *_equipment;
		AttributeSystem *_attributes;

	private slots:
		void addedClass(Class *cat);
		void addedAttribute(Attribute *attribute);
	};

};

#endif