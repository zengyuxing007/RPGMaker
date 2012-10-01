#include "../Logger.h"
#include "../Headers/AttributeSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

AttributeSystem::AttributeSystem()
	: _lifeSource(Attribute::create("Health", Attribute::LIFE)),
	_energySource(Attribute::create("Mana", Attribute::ENERGY)),
	_agilityForce(Attribute::create("Agility", Attribute::AGILITY)),
	_evasionForce(Attribute::create("Evasion", Attribute::EVASION)),
	_offensiveAttributes(QVector<Attribute*>(0)),
	_defensiveAttributes(QVector<Attribute*>(0)) {}

AttributeSystem::~AttributeSystem() {
	_offensiveAttributes.clear();
	_defensiveAttributes.clear();
}

Attribute* AttributeSystem::getAttribute(QString name) const {
	if (!!_lifeSource && (_lifeSource->getName() == name))
		return _lifeSource;

	if (!!_energySource && (_energySource->getName() == name))
		return _energySource;

	if (!!_agilityForce && (_agilityForce->getName() == name))
		return _agilityForce;

	if (!!_evasionForce && (_evasionForce->getName() == name))
		return _evasionForce;

	for (int i = 0; i < _offensiveAttributes.size(); i++) {
		Attribute *it = _offensiveAttributes.at(i);
		if (it->getName() == name)
			return it;
	}

	for (int i = 0; i < _defensiveAttributes.size(); i++) {
		Attribute *it = _defensiveAttributes.at(i);
		if (it->getName() == name)
			return it;
	}

	return 0;
}

Attribute* AttributeSystem::getLifeSource() const {
	return _lifeSource;
}

Attribute* AttributeSystem::getEnergySource() const {
	return _energySource;
}

Attribute* AttributeSystem::getAgilityForce() const {
	return _agilityForce;
}

Attribute* AttributeSystem::getEvasionForce() const {
	return _evasionForce;
}

Attribute* AttributeSystem::getOffensiveAttribute(QString name) const {
	return getComponent<Attribute>(_offensiveAttributes, name);
}

Attribute* AttributeSystem::getDefensiveAttribute(QString name) const {
	return getComponent<Attribute>(_defensiveAttributes, name);
}

QVector<Attribute*> AttributeSystem::getAttributes() const {
	QVector<Attribute*> attributes;

	if (!!_lifeSource)
		attributes.append(_lifeSource);

	if (!!_energySource)
		attributes.append(_energySource);

	if (!!_agilityForce)
		attributes.append(_agilityForce);

	if (!!_evasionForce)
		attributes.append(_evasionForce);

	attributes += _offensiveAttributes;
	attributes += _defensiveAttributes;

	return attributes;
}

const QVector<Attribute*>& AttributeSystem::getOffensiveAttributes() const {
	return _offensiveAttributes;
}

const QVector<Attribute*>& AttributeSystem::getDefensiveAttributes() const {
	return _defensiveAttributes;
}

QVector<QPair<Attribute*, Attribute*>> AttributeSystem::getLinks() const {
	QVector<QPair<Attribute*, Attribute*>> links(0);

	for (int i = 0; i < _offensiveAttributes.size(); i++) {
		Attribute *offensive = _offensiveAttributes.at(i);
		if (!!offensive->getLink())
			links.append(QPair<Attribute*, Attribute*>(offensive, offensive->getLink()));
	}

	return links;
}

void AttributeSystem::addOffensiveAttribute(Attribute *attribute) {
	if (!attribute)
		throw ProjectException("A null-attribute cannot be set.");

	if (isRegistered(attribute))
		throw ProjectException("An attribute with the same name already exists.");

	_offensiveAttributes.append(attribute);

	Logger *logger = Logger::instance();
	logger->addMessage("Added new offensive attribute " + attribute->getName());
	logger->setStatus(true);

	emit addedAttribute(attribute);
}

void AttributeSystem::addDefensiveAttribute(Attribute *attribute) {
	if (!attribute)
		throw ProjectException("A null-attribute cannot be set.");

	if (isRegistered(attribute))
		throw ProjectException("An attribute with the same name already exists.");
	
	_defensiveAttributes.append(attribute);

	Logger *logger = Logger::instance();
	logger->addMessage("Added new defensive attribute " + attribute->getName());
	logger->setStatus(true);

	emit addedAttribute(attribute);
}

void AttributeSystem::addLink(Attribute *offensive, Attribute *defensive) {
	if (!offensive || !defensive)
		throw ProjectException("Cannot establish a link if one attribute is null.");

	offensive->unsetLink();
	defensive->unsetLink();

	offensive->setLink(defensive);
	defensive->setLink(offensive);
}

void AttributeSystem::removeOffensiveAttribute(QString name) {
	Attribute *attribute = removeComponentAndLog<Attribute>(_offensiveAttributes, name, "offensive attribute", "attributes");

	if (!!attribute) 
		delete attribute;
}

void AttributeSystem::removeDefensiveAttribute(QString name) {
	Attribute *attribute = removeComponentAndLog<Attribute>(_defensiveAttributes, name, "defensive attribute", "attributes");

	if (!!attribute)
		delete attribute;
}

void AttributeSystem::removeLink(QString offensive, QString defensive) {
	if ((offensive == QString()) || (defensive == QString()))
		throw ProjectException("Cannot search for an attribute with an empty name.");

	if (!getOffensiveAttribute(offensive))
		throw ProjectException("The offensive attribute does not exist.");

	if (!getDefensiveAttribute(defensive))
		throw ProjectException("The defensive attribute does not exist.");

	getOffensiveAttribute(offensive)->unsetLink();
	getDefensiveAttribute(defensive)->unsetLink();
}

AttributeSystem* AttributeSystem::create() {
	return new AttributeSystem();
}

AttributeSystem* AttributeSystem::translateFromXML(QDomElement &element) {
	AttributeSystem *attributeSystem = AttributeSystem::create();

	QDomElement lifesourceElement = element.firstChildElement("lifesource");
	if (!lifesourceElement.isNull()) {
		attributeSystem->getLifeSource()->setName(lifesourceElement.attributeNode("name").value());
	}

	QDomElement energysourceElement = element.firstChildElement("energysource");
	if (!energysourceElement.isNull()) {
		attributeSystem->getEnergySource()->setName(energysourceElement.attributeNode("name").value());
	}

	QDomElement agilityforceElement = element.firstChildElement("agilityforce");
	if (!agilityforceElement.isNull()) {
		attributeSystem->getAgilityForce()->setName(agilityforceElement.attributeNode("name").value());
	}

	QDomElement evasionforceElement = element.firstChildElement("evasionforce");
	if (!evasionforceElement.isNull()) {
		attributeSystem->getEvasionForce()->setName(evasionforceElement.attributeNode("name").value());
	}

	QDomNodeList offensiveElements = element.elementsByTagName("offensive");
	for (int i = 0; i < offensiveElements.size(); i++) {
		QDomElement it = offensiveElements.at(i).toElement();
		Attribute *newAttribute = Attribute::create(it.attributeNode("name").value(), Attribute::OFFENSIVE);
		attributeSystem->addOffensiveAttribute(newAttribute);
	}

	QDomNodeList defensiveElements = element.elementsByTagName("defensive");
	for (int i = 0; i < defensiveElements.size(); i++) {
		QDomElement it = defensiveElements.at(i).toElement();
		Attribute *newAttribute = Attribute::create(it.attributeNode("name").value(), Attribute::DEFENSIVE);
		attributeSystem->addDefensiveAttribute(newAttribute);
	}

	return attributeSystem;
}

void AttributeSystem::translateToXML(AttributeSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement attributes = document.createElement("attributes");

	Attribute::translateToXML(system->getLifeSource(), document, attributes);
	Attribute::translateToXML(system->getEnergySource(), document, attributes);
	Attribute::translateToXML(system->getAgilityForce(), document, attributes);
	Attribute::translateToXML(system->getEvasionForce(), document, attributes);

	const QVector<Attribute*> offensive = system->getOffensiveAttributes();
	for (int i = 0; i < offensive.size(); i++)
		Attribute::translateToXML(offensive.at(i), document, attributes);

	const QVector<Attribute*> defensive = system->getDefensiveAttributes();
	for (int i = 0; i < defensive.size(); i++)
		Attribute::translateToXML(defensive.at(i), document, attributes);

	parent.appendChild(attributes);
}

bool AttributeSystem::isRegistered(Attribute *attribute) {
	return isRegistered(attribute->getName());
}

bool AttributeSystem::isRegistered(QString name) {
	if (!!_lifeSource && (_lifeSource->getName() == name))
		return true;

	if (!!_energySource && (_energySource->getName() == name))
		return true;

	if (!!_agilityForce && (_agilityForce->getName() == name))
		return true;

	if (!!_evasionForce && (_evasionForce->getName() == name))
		return true;

	for (int i = 0; i < _offensiveAttributes.size(); i++) {
		Attribute *it = _offensiveAttributes.at(i);
		if (it->getName() == name)
			return true;
	}

	for (int i = 0; i < _defensiveAttributes.size(); i++) {
		Attribute *it = _defensiveAttributes.at(i);
		if (it->getName() == name)
			return true;
	}

	return false;
}