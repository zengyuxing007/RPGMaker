#include "../Logger.h"
#include "../Headers/EquipmentPiece.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

EquipmentPiece::EquipmentPiece(QString name, Type type)
	: _name(name), _type(type) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created equipment piece " + _name);
}

EquipmentPiece::~EquipmentPiece() {
	emit deleteSignal(_name);

	_elements.clear();
	_attributes.clear();
	_classes.clear();

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed equipment piece " + _name);
}

QString EquipmentPiece::getName() const {
	return _name;
}

QString EquipmentPiece::getDescription() const {
	return _description;
}

EquipmentPiece::Type EquipmentPiece::getType() const {
	return this->_type;
}

const QVector<Element*>& EquipmentPiece::getElements() const {
	return _elements;
}

const QVector<QPair<Attribute*, int>>& EquipmentPiece::getAttributes() const {
	return _attributes;
}

const QVector<QPair<Class*, int>>& EquipmentPiece::getClasses() const {
	return _classes;
}

Element* EquipmentPiece::getElement(QString name) const {
	return getComponent<Element>(_elements, name);
}

QPair<Attribute*, int> EquipmentPiece::getAttribute(QString name) const {
	return getPair<Attribute, int>(_attributes, name);
}

QPair<Class*, int> EquipmentPiece::getClass(QString name) const {
	return getPair<Class, int>(_classes, name);
}

void EquipmentPiece::setName(QString name) {
	if (name == QString())
		throw ProjectException("An equipment piece should not have an empty name");

	_name = name;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of equipment piece to " + _name);
}

void EquipmentPiece::setDescription(QString description) {
	_description = description;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed description of equipment piece " + _name);
}

void EquipmentPiece::setType(EquipmentPiece::Type type) {
	this->_type = type;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed type of equipment piece " + this->_name);
}

void EquipmentPiece::addElement(Element *element) {
	addComponentAndLog<Element>(_elements, element, "element", "equipment piece " + _name);
	connect(element, SIGNAL(deleteSignal(QString)), this, SLOT(deleteElement(QString)));
}

void EquipmentPiece::addAttribute(Attribute *attribute, int value) {
	addAttribute(QPair<Attribute*, int>(attribute, value));
}

void EquipmentPiece::addAttribute(QPair<Attribute*, int> attribute) {
	addPairAndLog<Attribute, int>(_attributes, attribute, "attribute", "equipment piece " + _name);
	connect(attribute.first, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAttribute(QString)));
}

void EquipmentPiece::addClass(Class *cat, int level) {
	addClass(QPair<Class*, int>(cat, level));
}

void EquipmentPiece::addClass(QPair<Class*, int> cat) {
	addPairAndLog<Class, int>(_classes, cat, "class", "equipment piece " + _name);
	connect(cat.first, SIGNAL(deleteSignal(QString)), this, SLOT(deleteClass(QString)));
}

void EquipmentPiece::editAttribute(QString name, int value) {
	if (name == QString())
		throw ProjectException("The attribute with an empty name does not exist.");

	for (int i = 0; i < _attributes.size(); i++) {
		if (_attributes.at(i).first->getName() == name)
			_attributes.replace(i, QPair<Attribute*, int>(_attributes.at(i).first, value));
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed multiplier of attribute " + name + " in equipment piece " + _name);
	logger->setStatus(true);
}

void EquipmentPiece::editClass(QString name, int level) {
	if (name == QString())
		throw ProjectException("The attribute with an empty name does not exist.");

	for (int i = 0; i < _classes.size(); i++) {
		if (_classes.at(i).first->getName() == name)
			_classes.replace(i, QPair<Class*, int>(_classes.at(i).first, level));
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed level of class " + name + " in equipment piece " + _name);
	logger->setStatus(true);
}

void EquipmentPiece::removeElement(Element *element) {
	removeElement(element->getName());
}

void EquipmentPiece::removeElement(QString name) {
	Element *element = removeComponentAndLog<Element>(_elements, name, "element", "equipment piece " + _name);
	if (!!element)
		disconnect(element, 0, this, 0);
}

void EquipmentPiece::removeAttribute(Attribute *attribute) {
	removeAttribute(attribute->getName());
}

void EquipmentPiece::removeAttribute(QString name) {
	QPair<Attribute*, int> attribute = removePairAndLog<Attribute, int>(_attributes, name, "attribute", "equipment piece " + _name);
	if (!!attribute.first)
		disconnect(attribute.first, 0, this, 0);
}

void EquipmentPiece::removeClass(Class *cat) {
	removeClass(cat->getName());
}

void EquipmentPiece::removeClass(QString name) {
	QPair<Class*, int> cat = removePairAndLog<Class>(_classes, name, "class", "equipment piece " + _name);
	if (!!cat.first)
		disconnect(cat.first, 0, this, 0);
}

void EquipmentPiece::deleteElement(QString name) {
	try {
		removeElement(name);
	} catch (ProjectException &e) {}
}

void EquipmentPiece::deleteAttribute(QString name) {
	try {
		removeAttribute(name);
	} catch (ProjectException &e) {}
}

void EquipmentPiece::deleteClass(QString name) {
	try { 
		removeClass(name);
	} catch (ProjectException &e) {}
}