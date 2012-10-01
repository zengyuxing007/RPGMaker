#include "../Logger.h"
#include "../Headers/Weapon.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Weapon::Weapon(QString name, Subtype type)
	: EquipmentPiece(name, EquipmentPiece::WEAPON), _subtype(type) {}

Weapon::~Weapon() {}

Weapon::Subtype Weapon::getSubtype() const {
	return _subtype;
}

void Weapon::setSubtype(Weapon::Subtype type) {
	_subtype = type;
}

Weapon* Weapon::create(QString name, Subtype type) {
	if (name == QString())
		throw ProjectException("A weapon should not have an empty name.");

	Weapon *weapon = new Weapon(name, type);
	return weapon;
}

Weapon* Weapon::translateFromXML(QDomElement &weaponElement) {
	QString name = weaponElement.attributeNode("name").value();
	
	Weapon::Subtype type;
	QString typeString = weaponElement.attributeNode("type").value();
	if (typeString == "mainHand")
		type = Weapon::MAINHAND;
	else if (typeString == "offHand")
		type = Weapon::OFFHAND;
	else if (typeString == "twoHanded")
		type = Weapon::TWOHANDED;
	else
		throw ProjectException("No type " + typeString + " exists for weapon.");

	Weapon *weapon = Weapon::create(name, type);
	weapon->setDescription(weaponElement.firstChildElement("description").text());
	return weapon;
}

void Weapon::translateToXML(Weapon *weapon, QDomDocument &document, QDomElement &parent) {
	QDomElement weaponElement = document.createElement("weapon");
	weaponElement.setAttribute("name", weapon->_name);
	switch (weapon->_subtype) {
	case Weapon::MAINHAND:
		weaponElement.setAttribute("type", "mainHand");
		break;
	case Weapon::OFFHAND:
		weaponElement.setAttribute("type", "offHand");
		break;
	case Weapon::TWOHANDED:
		weaponElement.setAttribute("type", "twoHanded");
		break;
	}

	QDomElement description = document.createElement("description");
	QDomText text = document.createTextNode(weapon->_description);
	description.appendChild(text);
	weaponElement.appendChild(description);

	QDomElement elements = document.createElement("elements");
	for (int i = 0; i < weapon->_elements.size(); i++) {
		Element *it = weapon->_elements.at(i);
		QDomElement element = document.createElement("element");
		element.setAttribute("name", it->getName());
		elements.appendChild(element);
	}
	weaponElement.appendChild(elements);

	QDomElement attributes = document.createElement("attributes");
	for (int i = 0; i < weapon->_attributes.size(); i++) {
		QPair<Attribute*, int> it = weapon->_attributes.at(i);
		QDomElement attribute = document.createElement("attribute");
		attribute.setAttribute("name", it.first->getName());
		attribute.setAttribute("value", it.second);
		attributes.appendChild(attribute);
	}
	weaponElement.appendChild(attributes);

	QDomElement classes = document.createElement("classes");
	for (int i = 0; i < weapon->_classes.size(); i++) {
		QPair<Class*, int> it = weapon->_classes.at(i);
		QDomElement cat = document.createElement("class");
		cat.setAttribute("name", it.first->getName());
		cat.setAttribute("level", it.second);
		classes.appendChild(cat);
	}
	weaponElement.appendChild(classes);

	parent.appendChild(weaponElement);
}