#include "../Logger.h"
#include "../Headers/AbilitySystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

AbilitySystem::AbilitySystem() {}

AbilitySystem::~AbilitySystem() {}

const QVector<Ability*>& AbilitySystem::getAbilities() const {
	return _abilities;
}

Ability* AbilitySystem::getAbility(QString name) const {
	return getComponent(_abilities, name);
}

void AbilitySystem::addAbility(Ability *ability) {
	if (!ability)
		throw ProjectException("Cannot set a null ability.");

	if (!!getAbility(ability->getName()))
		throw ProjectException("An ability with the same name already exists.");

	_abilities.append(ability);

	Logger *logger = Logger::instance();
	logger->addMessage("Added new ability " + ability->getName());
	logger->setStatus(true);
}

void AbilitySystem::removeAbility(QString name) {
	if (!getAbility(name))
		return;

	Ability *ability = removeComponentAndLog<Ability>(_abilities, name, "ability", "abilities");
	if (!!ability)
		delete ability;
}

bool AbilitySystem::isRegistered(Ability *ability) {
	if (!ability)
		return false;

	return isRegistered(ability->getName());
}

bool AbilitySystem::isRegistered(QString name) {
	if (name == QString())
		return false;

	for (int i = 0; i < _abilities.size(); i++) {
		Ability *it = _abilities.at(i);
		if (it->getName() == name)
			return true;
	}

	return false;
}

AbilitySystem* AbilitySystem::create() {
	return new AbilitySystem();
}

AbilitySystem* AbilitySystem::translateFromXML(QDomElement &abilitiesElement) {
	AbilitySystem *abilitySystem = AbilitySystem::create();

	QDomNodeList offensiveElements = abilitiesElement.firstChildElement("offensive").childNodes();
	for (int i = 0; i < offensiveElements.size(); i++) {
		QDomElement it = offensiveElements.at(i).toElement();
		Ability *newAbility = Ability::translateFromXML(it);
		abilitySystem->addAbility(newAbility);
	}

	QDomNodeList defensiveElements = abilitiesElement.firstChildElement("defensive").childNodes();
	for (int i = 0; i < defensiveElements.size(); i++) {
		QDomElement it = defensiveElements.at(i).toElement();
		Ability *newAbility = Ability::translateFromXML(it);
		abilitySystem->addAbility(newAbility);
	}

	return abilitySystem;
}

void AbilitySystem::translateToXML(AbilitySystem *abilitySystem, QDomDocument &document, QDomElement &parent) {
	QDomElement abilitiesElement = document.createElement("abilities");

	QVector<Ability*> abilities = abilitySystem->getAbilities();

	QDomElement offensiveElement = document.createElement("offensive");
	for (int i = 0; i < abilities.size(); i++) {
		if (abilities.at(i)->getType() == Ability::OFFENSIVE)
			Ability::translateToXML(abilities.at(i), document, offensiveElement);
	}
	abilitiesElement.appendChild(offensiveElement);

	QDomElement defensiveElement = document.createElement("defensive");
	for (int i = 0; i < abilities.size(); i++) {
		if (abilities.at(i)->getType() == Ability::DEFENSIVE)
			Ability::translateToXML(abilities.at(i), document, defensiveElement);
	}
	abilitiesElement.appendChild(defensiveElement);

	parent.appendChild(abilitiesElement);
}