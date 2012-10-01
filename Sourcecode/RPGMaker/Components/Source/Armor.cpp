#include "../Logger.h"
#include "../Headers/Armor.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Armor::Armor(QString name, Subtype type)
	: EquipmentPiece(name, EquipmentPiece::ARMOR), _subtype(type) {}

Armor::~Armor() {}

Armor::Subtype Armor::getSubtype() const {
	return _subtype;
}

void Armor::setSubtype(Armor::Subtype type) {
	_subtype = type;
}

Armor* Armor::create(QString name, Subtype type) {
	if (name == QString())
		throw ProjectException("Armor should not have an empty name.");

	return new Armor(name, type);
}

Armor* Armor::translateFromXML(QDomElement &armorElement) {
	QString name = armorElement.attributeNode("name").value();
	
	Armor::Subtype type;
	QString typeString = armorElement.attributeNode("type").value();
	if (typeString == "helmet")
		type = Armor::HELMET;
	else if (typeString == "torso")
		type = Armor::TORSO;
	else if (typeString == "leggings")
		type = Armor::LEGGINGS;
	else if (typeString == "gloves")
		type = Armor::GLOVES;
	else if (typeString == "boots")
		type = Armor::BOOTS;
	else
		throw ProjectException("No type " + typeString + " exist for armor.");

	Armor *armor = Armor::create(name, type);
	armor->setDescription(armorElement.firstChildElement("description").text());
	return armor;
}

void Armor::translateToXML(Armor *armor, QDomDocument &document, QDomElement &parent) {
	QDomElement armorElement = document.createElement("armor");
	armorElement.setAttribute("name", armor->_name);
	switch (armor->_subtype) {
	case Armor::HELMET:
		armorElement.setAttribute("type", "helmet");
		break;
	case Armor::TORSO:
		armorElement.setAttribute("type", "torso");
		break;
	case Armor::LEGGINGS:
		armorElement.setAttribute("type", "leggings");
		break;
	case Armor::GLOVES:
		armorElement.setAttribute("type", "gloves");
		break;
	case Armor::BOOTS:
		armorElement.setAttribute("type", "boots");
		break;
	}

	QDomElement description = document.createElement("description");
	QDomText text = document.createTextNode(armor->_description);
	description.appendChild(text);
	armorElement.appendChild(description);

	QDomElement elements = document.createElement("elements");
	for (int i = 0; i < armor->_elements.size(); i++) {
		Element *it = armor->_elements.at(i);
		QDomElement element = document.createElement("element");
		element.setAttribute("name", it->getName());
		elements.appendChild(element);
	}
	armorElement.appendChild(elements);

	QDomElement attributes = document.createElement("attributes");
	for (int i = 0; i < armor->_attributes.size(); i++) {
		QPair<Attribute*, int> it = armor->_attributes.at(i);
		QDomElement attribute = document.createElement("attribute");
		attribute.setAttribute("name", it.first->getName());
		attribute.setAttribute("value", it.second);
		attributes.appendChild(attribute);
	}
	armorElement.appendChild(attributes);

	QDomElement classes = document.createElement("classes");
	for (int i = 0; i < armor->_classes.size(); i++) {
		QPair<Class*, int> it = armor->_classes.at(i);
		QDomElement cat = document.createElement("class");
		cat.setAttribute("name", it.first->getName());
		cat.setAttribute("level", it.second);
		classes.appendChild(cat);
	}
	armorElement.appendChild(classes);

	parent.appendChild(armorElement);
}