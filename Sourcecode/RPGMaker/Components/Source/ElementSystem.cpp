#include "../Logger.h"
#include "../Headers/ElementSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

ElementSystem::ElementSystem()
	: _elements(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an element system");
		logger->setStatus(true);
}

ElementSystem::~ElementSystem() {
	for (int i = 0; i < _elements.size(); i++) {
		Element *it = _elements.at(i);

		if (!!it)
			delete it;
	}

	_elements.clear();
}

const QVector<Element*>& ElementSystem::getElements() const {
	return _elements;
}

Element* ElementSystem::getElement(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an element with an empty name.");

	return getComponent<Element>(_elements, name);
}

void ElementSystem::addElement(Element *element) {
	if (!element)
		throw ProjectException("Cannot add a null element.");

	_elements.append(element);

	Logger *logger = Logger::instance();
	logger->addMessage("Added element " + element->getName() + " to the project");
	logger->setStatus(true);
}

void ElementSystem::removeElement(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an element with an empty name.");

	Element *element = removeComponentAndLog(_elements, name, "element", "project");

	if (!!element)
		delete element;
}

ElementSystem* ElementSystem::create() {
	return new ElementSystem;
}

ElementSystem* ElementSystem::translateFromXML(QDomElement &element) {
	ElementSystem *system = create();

	QDomNodeList elementElements = element.childNodes();

	for (int i = 0; i < elementElements.size(); i++) {
		QDomElement elementElement = elementElements.at(i).toElement();
		system->addElement(Element::translateFromXML(elementElement));
	}

	return system;
}

void ElementSystem::translateToXML(ElementSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("elements");

	const QVector<Element*> elements = system->getElements();
	for (int i = 0; i < elements.size(); i++) {
		Element::translateToXML(elements.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}