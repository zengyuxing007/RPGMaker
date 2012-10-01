#include "../Logger.h"
#include "../Headers/Ability.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Ability::Ability(QString name, Ability::Type type)
	:_cost(0), _name(name), _type(type),
	_elements(QVector<Element*>(0)),
	_attribute(0, 0) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created ability " + _name);
}

Ability::~Ability() {
	emit deleteSignal(_name);
	this->_elements.clear();

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed ability " + _name);
}

int Ability::getCost() const {
	return _cost;
}

Ability::Type Ability::getType() const {
	return _type;
}

QString Ability::getName() const {
	return this->_name;
}

QString Ability::getDescription() const {
	return this->_description;
}

Element* Ability::getElement(QString name) const {
	return getComponent<Element>(_elements, name);
}

const QVector<Element*>& Ability::getElements() const {
	return this->_elements;
}

QPair<Attribute*, double> Ability::getAttribute() const {
	return _attribute;
}

void Ability::setCost(int cost) {
	if (cost < 0)
		throw ProjectException("The cost of an ability should be a non-negative value.");

	_cost = cost;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed cost of ability " + _name);
	logger->setStatus(true);
}

void Ability::setName(QString name) {
	if (name == QString()) 
		throw ProjectException("An ability should not have an empty name");

	this->_name = name;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of ability to " + _name);
}

void Ability::setType(Ability::Type type) {
	_type = type;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed type of ability " + _name);
	logger->setStatus(true);
}

void Ability::setDescription(QString newDescription) {
	this->_description = newDescription;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed description of ability " + this->_name);
}

void Ability::setAttribute(Attribute *attribute, double multiplier) {
	if (!attribute)
		throw ProjectException("Cannot set a null attribute.");

	if (attribute->getType() != Attribute::OFFENSIVE)
		throw ProjectException("An ability should be linked to an offensive attribute.");

	_attribute = QPair<Attribute*, double>(attribute, multiplier);
	connect(attribute, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAttribute(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Added attribute " + attribute->getName() + " to ability " + _name);
	logger->setStatus(true);
}

void Ability::addElement(Element *newElement) {
	addComponentAndLog<Element>(_elements, newElement, "element", "ability " + _name);
	connect(newElement, SIGNAL(deleteSignal(QString)), this, SLOT(deleteElement(QString)));
}

void Ability::editAttribute(double multiplier) {
	if (!_attribute.first)
		throw ProjectException("Cannot edit multiplier because there is no attribute.");

	_attribute.second = multiplier;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed multiplier of attribute " + _attribute.first->getName() + " in ability " + _name);
	logger->setStatus(true);
}

void Ability::removeElement(Element *oldElement) {
	this->removeElement(oldElement->getName());
}

void Ability::removeElement(QString oldElement) {
	Element *element = removeComponentAndLog<Element>(_elements, oldElement, "element", "ability " + _name);
	if (!!element)
		disconnect(element, 0, this, 0);
}

void Ability::unsetAttribute() {
	if (!!_attribute.first)
		disconnect(_attribute.first, 0, this, 0);

	_attribute = QPair<Attribute*, double>(0, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed attribute from ability " + _name);
	logger->setStatus(true);
}

Ability* Ability::create(QString name, Ability::Type type) {
	if (name == QString())
		throw ProjectException("An ability should not have an empty name.");

	Ability *ability = new Ability(name, type);
	return ability;
}

Ability* Ability::translateFromXML(QDomElement &abilityElement) {
	Ability::Type type;
	if (abilityElement.parentNode().toElement().tagName() == QString("offensive"))
		type = Ability::OFFENSIVE;
	else if (abilityElement.parentNode().toElement().tagName() == QString("defensive"))
		type = Ability::DEFENSIVE;

	QString name = abilityElement.attributeNode("name").value();
	Ability *ability = Ability::create(name, type);

	// Cost
	QDomElement costElement = abilityElement.firstChildElement("cost");
	ability->setCost(costElement.attributeNode("value").value().toInt());

	// Description
	ability->setDescription(abilityElement.firstChildElement("description").text());
	return ability;
}

void Ability::translateToXML(Ability *ability, QDomDocument &document, QDomElement &parent) {
	QDomElement abilityElement = document.createElement("ability");
	abilityElement.setAttribute("name", ability->_name);

	// Cost
	QDomElement cost = document.createElement("cost");
	cost.setAttribute("value", ability->getCost());
	abilityElement.appendChild(cost);

	// Description
	QDomElement description = document.createElement("description");
	QDomText text = document.createTextNode(ability->_description);
	description.appendChild(text);
	abilityElement.appendChild(description);

	// Elements
	QDomElement elements = document.createElement("elements");
	for (int i = 0; i < ability->_elements.size(); i++) {
		Element *it = ability->_elements.at(i);
		QDomElement element = document.createElement("element");
		element.setAttribute("name", it->getName());
		elements.appendChild(element);
	}
	abilityElement.appendChild(elements);

	// Attribute
	if (!!ability->getAttribute().first) {
		QDomElement attributeElement = document.createElement("attribute");
		QPair<Attribute*, double> attribute = ability->getAttribute();
		attributeElement.setAttribute("name", attribute.first->getName());
		attributeElement.setAttribute("multiplier", attribute.second);
		abilityElement.appendChild(attributeElement);
	}

	parent.appendChild(abilityElement);
}

void Ability::deleteElement(QString name) {
	try {
		removeElement(name);
	} catch (ProjectException &e) {}
}

void Ability::deleteAttribute(QString name) {
	try {
		unsetAttribute();
	} catch (ProjectException &e) {}
}