#include "../Logger.h"
#include "../Headers/Class.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Class::Class(QString name)
	: _name(name), _primaryAttribute(0),
	_attributes(QVector<QPair<Attribute*, QPair<int, double>>>(0)),
	_abilities(QVector<QPair<Ability*, int>>(0)) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created class " + _name);
}

Class::~Class() {
	emit deleteSignal(_name);

	this->_attributes.clear();
	this->_abilities.clear();

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed class " + _name);
}

QString Class::getName() const {
	return this->_name;
}

QString Class::getDescription() const {
	return this->_description;
}

Attribute* Class::getPrimaryAttribute() const {
	return _primaryAttribute;
}

const QVector<QPair<Attribute*, QPair<int, double>>>& Class::getAttributes() const {
	return this->_attributes;
}

const QVector<QPair<Ability*, int>>& Class::getAbilities() const {
	return this->_abilities;
}

QPair<Attribute*, QPair<int, double>> Class::getAttribute(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search with an empty name.");

	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, QPair<int, double>> it = _attributes.at(i);
		if (it.first->getName() == name)
			return it;
	}

	return QPair<Attribute*, QPair<int, double>>(0, QPair<int, double>(0, 0));
}

QPair<Ability*, int> Class::getAbility(QString name) const {
	return getPair<Ability, int>(_abilities, name);
}

void Class::setName(QString name) {
	if (name == QString())
		throw ProjectException("A class should not have an empty name");

	this->_name = name;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of class to " + _name);
}

void Class::setDescription(QString description) {
	this->_description = description;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed description of class " + _name);
}

void Class::setPrimaryAttribute(Attribute *attribute) {
	if (!attribute)
		throw ProjectException("Cannot set a null primary attribute.");

	if (attribute->getType() != Attribute::OFFENSIVE)
		throw ProjectException("Only an offensive attribute can be set as a primary attribute.");

	_primaryAttribute = attribute;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed primary attribute of class " + _name);
	logger->setStatus(true);
}

void Class::addAttribute(Attribute *newAttribute, int initial, double value) {
	QPair<Attribute*, QPair<int, double>> attribute(newAttribute, QPair<int, double>(initial, value));
	this->addAttribute(attribute);
}

void Class::addAttribute(QPair<Attribute*, QPair<int, double>> newAttribute) {
	_attributes.append(newAttribute);
	connect(newAttribute.first, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAttribute(QString)));
}

void Class::addAbility(Ability *newAbility, int acquire) {
	QPair<Ability*, int> ability(newAbility, acquire);
	this->addAbility(ability);
}

void Class::addAbility(QPair<Ability*, int> newAbility) {
	addPairAndLog<Ability, int>(_abilities, newAbility, "ability", "class " + _name);
	connect(newAbility.first, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAbility(QString)));
}

void Class::editAttribute(QString name, int initial, double multiplier) {
	if (name == QString())
		throw ProjectException("The attribute with an empty name does not exist.");

	for (int i = 0; i < _attributes.size(); i++) {
		if (_attributes.at(i).first->getName() == name)
			_attributes.replace(i, QPair<Attribute*, QPair<int, double>>(_attributes.at(i).first, QPair<int, double>(initial, multiplier)));
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed multiplier of attribute " + name + " in class " + _name);
	logger->setStatus(true);
}

void Class::editAbility(QString name, int acquire) {
	if (name == QString())
		throw ProjectException("The ability with an empty name does not exist.");

	for (int i = 0; i < _abilities.size(); i++) {
		if (_abilities.at(i).first->getName() == name)
			_abilities.replace(i, QPair<Ability*, int>(_abilities.at(i).first, acquire));
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed acquisition of ability " + name + " in class " + _name);
	logger->setStatus(true);
}

void Class::removeAttribute(Attribute *oldAttribute) {
	this->removeAttribute(oldAttribute->getName());
}

void Class::removeAttribute(QString oldAttribute) {
	if (oldAttribute == QString())
		throw ProjectException("Cannot search for an attribute with an empty name.");

	if (_attributes.empty())
		throw ProjectException("Could not remove attribute " + oldAttribute + " because the list is empty");

	QPair<Attribute*, QPair<int, double>> attribute;
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, QPair<int, double>> it = _attributes.at(i);
		if (oldAttribute == it.first->getName()) {
			attribute = it;
			_attributes.remove(i);
			break;
		}
	}



	if (!!attribute.first)
		disconnect(attribute.first, 0, this, 0);
}

void Class::removeAbility(Ability *oldAbility) {
	this->removeAbility(oldAbility->getName());
}

void Class::removeAbility(QString oldAbility) {
	QPair<Ability*, int> ability = removePairAndLog<Ability, int>(_abilities, oldAbility, "ability", "class " + _name);
	if (!!ability.first)
		disconnect(ability.first, 0, this, 0);
}

void Class::unsetPrimaryAttribute() {
	_primaryAttribute = 0;

	Logger *logger = Logger::instance();
	logger->addMessage("Removed primary attribute from class " + _name);
	logger->setStatus(true);
}

Class* Class::create(QString name) {
	if (name == QString())
		throw ProjectException("A class should not have an empty name.");

	Class *cat = new Class(name);
	return cat;
}

Class* Class::translateFromXML(QDomElement &catElement) {
	QString name = catElement.attributeNode("name").value();
	Class *cat = Class::create(name);
	cat->setDescription(catElement.firstChildElement("description").text());
	return cat;
}

void Class::translateToXML(Class *cat, QDomDocument &document, QDomElement &parent) {
	QDomElement catElement = document.createElement("class");
	catElement.setAttribute("name", cat->_name);

	QDomElement description = document.createElement("description");
	QDomText text = document.createTextNode(cat->_description);
	description.appendChild(text);
	catElement.appendChild(description);

	QDomElement attributes = document.createElement("attributes");
	for (int i = 0; i < cat->_attributes.size(); i++) {
		QPair<Attribute*, QPair<int, double>> it = cat->_attributes.at(i);
		QDomElement attribute = document.createElement("attribute");
		attribute.setAttribute("name", it.first->getName());
		attribute.setAttribute("initial", it.second.first);
		attribute.setAttribute("multiplier", it.second.second);
		attributes.appendChild(attribute);
	}
	catElement.appendChild(attributes);

	if (!!cat->getPrimaryAttribute()) {
		QDomElement primaryAttributeElement = document.createElement("primaryAttribute");
		primaryAttributeElement.setAttribute("name", cat->getPrimaryAttribute()->getName());
		catElement.appendChild(primaryAttributeElement);
	}

	QDomElement abilities = document.createElement("abilities");
	for (int i = 0; i < cat->_abilities.size(); i++) {
		QPair<Ability*, int> it = cat->_abilities.at(i);
		QDomElement ability = document.createElement("ability");
		ability.setAttribute("name", it.first->getName());
		ability.setAttribute("acquire", it.second);
		abilities.appendChild(ability);
	}
	catElement.appendChild(abilities);

	parent.appendChild(catElement);
}

void Class::deleteAttribute(QString name) {
	if (!!_primaryAttribute && (_primaryAttribute->getName() == name))
		unsetPrimaryAttribute();

	try {
		removeAttribute(name);
	} catch (ProjectException &e) {}
}

void Class::deleteAbility(QString name) {
	try {
		removeAbility(name);
	} catch (ProjectException &e) {}
}