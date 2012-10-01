#include "../Logger.h"
#include "../Headers/Properties.h"
#include "../Headers/AutoTileset.h"
#include "../Headers/StaticTileset.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Standard constructor.
*/
Properties::Properties()
	: _elements(0),
	_attributes(0),
	_abilities(0),
	_classes(0),
	_equipment(0),
	_tilesets(0) {

		setElements(ElementSystem::create());
		setAttributes(AttributeSystem::create());
		setAbilities(AbilitySystem::create());
		setClasses(ClassSystem::create());
		setEquipment(EquipmentSystem::create());
		setTilesets(TilesetSystem::create());
}

/**
*	Standard destructor.
*/
Properties::~Properties() {

	delete _tilesets;
	delete _equipment;
	delete _classes;
	delete _abilities;
	delete _attributes;
	delete _elements;
}

/**
*	Return the class management system.
*/
ClassSystem* Properties::getClasses() const {
	return _classes;
}

/**
*	Return the element management system.
*/
ElementSystem* Properties::getElements() const {
	return _elements;
}

/**
*	Return the tileset management system.
*/
TilesetSystem* Properties::getTilesets() const {
	return _tilesets;
}

/**
*	Return the ability management system.
*/
AbilitySystem* Properties::getAbilities() const {
	return _abilities;
}

/**
*	Return the equipment management system.
*/
EquipmentSystem* Properties::getEquipment() const {
	return _equipment;
}

/**
*	Return the attribute management system.
*/
AttributeSystem* Properties::getAttributes() const {
	return _attributes;
}

/**
*	Set the class management system.
*/
void Properties::setClasses(ClassSystem *classSystem) {
	if (!classSystem)
		throw ProjectException("Cannot set a null class system.");

	if (!!_classes) {
		disconnect(_classes, 0, this, 0);
		delete _classes;
	}

	_classes = classSystem;
	connect(classSystem, SIGNAL(addedClass(Class*)), this, SLOT(addedClass(Class*)));
}

/**
*	Set the element management system.
*/
void Properties::setElements(ElementSystem *elementSystem) {
	if (!elementSystem)
		throw ProjectException("Cannot set a null element system.");

	if (!!_elements)
		delete _elements;

	_elements = elementSystem;
}

/**
*	Set the tileset management system.
*/
void Properties::setTilesets(TilesetSystem *tilesetSystem) {
	if (!tilesetSystem)
		throw ProjectException("Cannot set a null tileset system.");

	if (!!_tilesets)
		delete _tilesets;

	_tilesets = tilesetSystem;
}

/**
*	Set the ability management system.
*/
void Properties::setAbilities(AbilitySystem *abilitySystem) {
	if (!abilitySystem)
		throw ProjectException("Cannot set a null ability system.");

	if (!!_attributes)
		delete _abilities;

	_abilities = abilitySystem;
}

/**
*	Set the equipment management system.
*/
void Properties::setEquipment(EquipmentSystem *equipmentSystem) {
	if (!equipmentSystem)
		throw ProjectException("Cannot set a null equipment system.");

	if (!!_equipment)
		delete _equipment;

	_equipment = equipmentSystem;
}

/**
*	Set the attribute management system.
*/
void Properties::setAttributes(AttributeSystem *attributeSystem) {
	if (!attributeSystem)
		throw ProjectException("Cannot set a null attribute system.");

	if (!!_attributes) {
		disconnect(_attributes, 0, this, 0);
		delete _attributes;
	}

	_attributes = attributeSystem;
	connect(attributeSystem, SIGNAL(addedAttribute(Attribute*)), this, SLOT(addedAttribute(Attribute*)));
}

/**
*	Translate project properties from XML and return a pointer to an instance of Properties.
*/
Properties* Properties::translateFromXML(QDomElement &propertiesElement) {
	Properties *properties = new Properties();

	properties->setAttributes(AttributeSystem::translateFromXML(propertiesElement.firstChildElement("attributes")));
	properties->setElements(ElementSystem::translateFromXML(propertiesElement.firstChildElement("elements")));
	properties->setAbilities(AbilitySystem::translateFromXML(propertiesElement.firstChildElement("abilities")));
	properties->setClasses(ClassSystem::translateFromXML(propertiesElement.firstChildElement("classes")));
	properties->setEquipment(EquipmentSystem::translateFromXML(propertiesElement.firstChildElement("equipment")));
	properties->setTilesets(TilesetSystem::translateFromXML(propertiesElement.firstChildElement("tilesets")));

	return properties;
}

/**
*	Translate the project properties to XML and append it to the XML-document.
*/
void Properties::translateToXML(Properties *properties, QDomDocument &document, QDomElement &parent) {
	QDomElement propertiesElement = document.createElement("properties");

	// Attributes
	AttributeSystem::translateToXML(properties->getAttributes(), document, propertiesElement);

	// Elements
	ElementSystem::translateToXML(properties->getElements(), document, propertiesElement);
	
	// Abilities
	AbilitySystem::translateToXML(properties->getAbilities(), document, propertiesElement);

	// Classes
	ClassSystem::translateToXML(properties->getClasses(), document, propertiesElement);

	// Equipment
	EquipmentSystem::translateToXML(properties->getEquipment(), document, propertiesElement);

	// Items
	QDomElement items = document.createElement("items");
	propertiesElement.appendChild(items);

	// Tilesets
	TilesetSystem::translateToXML(properties->getTilesets(), document, propertiesElement);

	parent.appendChild(propertiesElement);
}

/**
*	This is called when a class is added to the class management system.
*/
void Properties::addedClass(Class *cat) {
	QVector<Attribute*> attributes = _attributes->getAttributes();
	for (int i = 0; i < attributes.size(); i++)
		cat->addAttribute(attributes.at(i), 1, 1.0);
}

/**
*	This is called when an attribute is added to the attribute management system.
*/
void Properties::addedAttribute(Attribute *attribute) {
	QVector<Class*> classes = _classes->getClasses();
	for (int i = 0; i < classes.size(); i++)
		classes.at(i)->addAttribute(attribute, 1, 1.0);
}