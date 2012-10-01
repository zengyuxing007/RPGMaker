#include "../Logger.h"
#include "../Headers/Element.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Element::Element(QString name)
	: _name(name),
	_resistant(QVector<Element*>(0)),
	_vulnerable(QVector<Element*>(0)) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created element " + _name);
}

Element::~Element() {
	emit deleteSignal(_name);

	this->_resistant.clear();
	this->_vulnerable.clear();

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed element " + _name);
}

QString Element::getName() const {
	return this->_name;
}

void Element::setName(QString name) {
	if (name == QString())
		throw ProjectException("An element should not have an empty name.");

	_name = name;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of element to " + _name);
}

const QVector<Element*>& Element::getResistantElements() const {
	return this->_resistant;
}

const QVector<Element*>& Element::getVulnerableElements() const {
	return this->_vulnerable;
}

bool Element::isResistant(Element *element) const {
	return this->isResistant(element->getName());
}

bool Element::isResistant(QString elementName) const{
	if (getComponent<Element>(this->_resistant, elementName))
		return true;
	else
		return false;
}

bool Element::isVulnerable(Element *element) const {
	return this->isVulnerable(element->getName());
}

bool Element::isVulnerable(QString elementName) const {
	if (getComponent<Element>(this->_vulnerable, elementName))
		return true;
	else
		return false;
}

void Element::addResistantElement(Element *newElement) {
	if (!!isVulnerable(newElement))
		throw ProjectException("An element is not allowed to be resistant and vulnerable to the same element at the same time.");

	addComponentAndLog<Element>(_resistant, newElement, "resistant element", "element " + _name);
	connect(newElement, SIGNAL(deleteSignal(QString)), this, SLOT(deleteResistant(QString)));
}

void Element::addVulnerableElement(Element *newElement) {
	if (isResistant(newElement))
		throw ProjectException("An element is not allowed to be resistant and vulnerable to the same element at the same time.");

	addComponentAndLog<Element>(_vulnerable, newElement, "vulnerable element", "element " + _name);
	connect(newElement, SIGNAL(deleteSignal(QString)), this, SLOT(deleteVulnerable(QString)));
}

void Element::removeResistantElement(Element *oldElement) {
	this->removeResistantElement(oldElement->getName());
}

void Element::removeResistantElement(QString oldElement) {
	Element *element = removeComponentAndLog<Element>(_resistant, oldElement, "resistant element", "element " + _name);
	if (!!element)
		disconnect(element, 0, this, 0);
}

void Element::removeVulnerableElement(Element *oldElement) {
	this->removeVulnerableElement(oldElement->getName());
}

void Element::removeVulnerableElement(QString oldElement) {
	Element *element = removeComponentAndLog<Element>(_vulnerable, oldElement, "vulnerable element", "element " + _name);
	if (!!element)
		disconnect(element, 0, this, 0);
}

Element* Element::create(QString name) {
	if (name == QString())
		throw ProjectException("An element should not have an empty name.");

	Element *element = new Element(name);
	return element;
}

Element* Element::translateFromXML(QDomElement &elementElement) {
	QString name = elementElement.attributeNode("name").value();
	Element *element = Element::create(name);
	return element;
}

void Element::translateToXML(Element *element, QDomDocument &document, QDomElement &parent) {
	QDomElement elementElement = document.createElement("element");
	elementElement.setAttribute("name", element->_name);

	QDomElement resistantElement = document.createElement("resistant");
	for (int i = 0; i < element->_resistant.size(); i++) {
		Element *it = element->_resistant.at(i);
		QDomElement resistant = document.createElement("element");
		resistant.setAttribute("name", it->getName());
		resistantElement.appendChild(resistant);
	}
	elementElement.appendChild(resistantElement);

	QDomElement vulnerableElement = document.createElement("vulnerable");
	for (int i = 0; i < element->_vulnerable.size(); i++) {
		Element *it = element->_vulnerable.at(i);
		QDomElement vulnerable = document.createElement("element");
		vulnerable.setAttribute("name", it->getName());
		vulnerableElement.appendChild(vulnerable);
	}
	elementElement.appendChild(vulnerableElement);

	parent.appendChild(elementElement);
}

void Element::deleteResistant(QString name) {
	try {
		removeResistantElement(name);
	} catch (ProjectException &e) {}
}

void Element::deleteVulnerable(QString name) {
	try {
		removeVulnerableElement(name);
	} catch (ProjectException &e) {}
}