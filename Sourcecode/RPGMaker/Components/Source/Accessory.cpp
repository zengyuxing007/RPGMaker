#include "../Logger.h"
#include "../Headers/Accessory.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Accessory::Accessory(QString name, Subtype type)
	: EquipmentPiece(name, EquipmentPiece::ACCESSORY), _subtype(type) {}

Accessory::~Accessory() {}

Accessory::Subtype Accessory::getSubtype() const {
	return _subtype;
}

void Accessory::setSubtype(Accessory::Subtype type) {
	_subtype = type;
}

Accessory* Accessory::create(QString name, Subtype type) {
	if (name == QString())
		throw ProjectException("An accessory should not have an empty name.");

	Accessory *accessory = new Accessory(name, type);
	return accessory;
}

Accessory* Accessory::translateFromXML(QDomElement &accessoryElement) {
	QString name = accessoryElement.attributeNode("name").value();

	Accessory::Subtype type;
	QString typeString = accessoryElement.attributeNode("type").value();
	if (typeString == "earring")
		type = Accessory::EARRING;
	else if (typeString == "necklace")
		type = Accessory::NECKLACE;
	else if (typeString == "ring")
		type = Accessory::RING;
	else
		throw ProjectException("No type " + typeString + " exists for accessory.");

	Accessory *accessory = Accessory::create(name, type);
	accessory->setDescription(accessoryElement.firstChildElement("description").text());
	return accessory;
}

void Accessory::translateToXML(Accessory *accessory, QDomDocument &document, QDomElement &parent) {
	QDomElement accessoryElement = document.createElement("accessory");
	accessoryElement.setAttribute("name", accessory->_name);
	switch (accessory->_subtype) {
	case Accessory::EARRING:
		accessoryElement.setAttribute("type", "earring");
		break;
	case Accessory::NECKLACE:
		accessoryElement.setAttribute("type", "necklace");
		break;
	case Accessory::RING:
		accessoryElement.setAttribute("type", "ring");
		break;
	}

	QDomElement description = document.createElement("description");
	QDomText text = document.createTextNode(accessory->_description);
	description.appendChild(text);
	accessoryElement.appendChild(description);

	QDomElement elements = document.createElement("elements");
	for (int i = 0; i < accessory->_elements.size(); i++) {
		Element *it = accessory->_elements.at(i);
		QDomElement element = document.createElement("element");
		element.setAttribute("name", it->getName());
		elements.appendChild(element);
	}
	accessoryElement.appendChild(elements);

	QDomElement attributes = document.createElement("attributes");
	for (int i = 0; i < accessory->_attributes.size(); i++) {
		QPair<Attribute*, int> it = accessory->_attributes.at(i);
		QDomElement attribute = document.createElement("attribute");
		attribute.setAttribute("name", it.first->getName());
		attribute.setAttribute("value", it.second);
		attributes.appendChild(attribute);
	}
	accessoryElement.appendChild(attributes);

	QDomElement classes = document.createElement("classes");
	for (int i = 0; i < accessory->_classes.size(); i++) {
		QPair<Class*, int> it = accessory->_classes.at(i);
		QDomElement cat = document.createElement("class");
		cat.setAttribute("name", it.first->getName());
		cat.setAttribute("level", it.second);
		classes.appendChild(cat);
	}
	accessoryElement.appendChild(classes);

	parent.appendChild(accessoryElement);
}