#include "../Logger.h"
#include "../Headers/Attribute.h"

using namespace projectcomponents;

Attribute::Attribute(QString name, Attribute::Type type)
	: _name(name), _type(type), _link(0) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created attribute " + _name);
}

Attribute::~Attribute() {
	emit deleteSignal(_name);

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed attribute " + _name);
}

Attribute::Type Attribute::getType() const {
	return _type;
}

QString Attribute::getName() const {
	return this->_name;
}

Attribute* Attribute::getLink() const {
	return _link;
}

void Attribute::setName(QString name) {
	if (name == QString())
		throw ProjectException("An attribute should not have an empty name");

	this->_name = name;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of attribute to " + _name);
}

void Attribute::setLink(Attribute *attribute) {
	if (!attribute)
		throw ProjectException("Cannot set a null link to an attribute.");

	switch (attribute->getType()) {
	case Attribute::LIFE:
	case Attribute::ENERGY:
	case Attribute::EVASION:
	case Attribute::AGILITY:
		throw ProjectException("A standard attribute cannot be linked.");
		break;
	}

	switch (getType()) {
	case Attribute::LIFE:
	case Attribute::ENERGY:
	case Attribute::EVASION:
	case Attribute::AGILITY:
		throw ProjectException("This attribute cannot be linked to one of the standard attributes.");
		break;
	}

	if (_type == attribute->getType())
		throw ProjectException("A link can only be established between an offensive and a defensive attribute.");

	if (!!_link)
		disconnect(_link, 0, this, 0);

	_link = attribute;
	connect(_link, SIGNAL(deleteSignal(QString)), this, SLOT(attributeDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Linked an attribute to attribute " + _name);
	logger->setStatus(true);
}

void Attribute::unsetLink() {
	if (!!_link)
		disconnect(_link, 0, this, 0);

	_link = 0;
	Logger *logger = Logger::instance();
	logger->addMessage("Removed attribute link from attribute " + _name);
	logger->setStatus(true);
}

Attribute* Attribute::create(QString name, Attribute::Type type) {
	if (name == QString())
		throw ProjectException("An attribute should not have an empty name.");

	Attribute *attribute = new Attribute(name, type);
	return attribute;
}

Attribute* Attribute::translateFromXML(QDomElement &attributeElement) {
	Attribute::Type type;
	if (attributeElement.tagName() == QString("lifesource"))
		type = Attribute::LIFE;
	else if (attributeElement.tagName() == QString("energysource"))
		type = Attribute::ENERGY;
	else if (attributeElement.tagName() == QString("agilityforce"))
		type = Attribute::AGILITY;
	else if (attributeElement.tagName() == QString("evasionforce"))
		type = Attribute::EVASION;
	else if (attributeElement.tagName() == QString("offensive"))
		type = Attribute::OFFENSIVE;
	else if (attributeElement.tagName() == QString("defensive"))
		type = Attribute::DEFENSIVE;

	QString name = attributeElement.attributeNode("name").value();
	Attribute *attribute = Attribute::create(name, type);
	return attribute;
}

void Attribute::translateToXML(Attribute *attribute, QDomDocument &document, QDomElement &parent) {
	QDomElement attributeElement;
	switch (attribute->getType()) {
	case Attribute::LIFE:
		attributeElement = document.createElement("lifesource");
		break;
	case Attribute::ENERGY:
		attributeElement = document.createElement("energysource");
		break;
	case Attribute::AGILITY:
		attributeElement = document.createElement("agilityforce");
		break;
	case Attribute::EVASION:
		attributeElement = document.createElement("evasionforce");
		break;
	case Attribute::OFFENSIVE:
		attributeElement = document.createElement("offensive");
		if (!!attribute->getLink()) {
			QDomElement linkElement = document.createElement("link");
			linkElement.setAttribute("name", attribute->getLink()->getName());
			attributeElement.appendChild(linkElement);
		}
		break;
	case Attribute::DEFENSIVE:
		attributeElement = document.createElement("defensive");
		if (!!attribute->getLink()) {
			QDomElement linkElement = document.createElement("link");
			linkElement.setAttribute("name", attribute->getLink()->getName());
			attributeElement.appendChild(linkElement);
		}
		break;
	}

	attributeElement.setAttribute("name", attribute->getName());

	parent.appendChild(attributeElement);
}

void Attribute::attributeDeleted(QString name) {
	if (!!_link && (_link->getName() == name))
		unsetLink();
}