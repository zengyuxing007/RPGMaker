#include "../Logger.h"
#include "../Headers/BattlegroupSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

BattlegroupSystem::BattlegroupSystem()
	: _battlegroups(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an battlegroup system");
		logger->setStatus(true);
}

BattlegroupSystem::~BattlegroupSystem() {
	for (int i = 0; i < _battlegroups.size(); i++) {
		Battlegroup *it = _battlegroups.at(i);

		if (!!it)
			delete it;
	}

	_battlegroups.clear();
}

const QVector<Battlegroup*>& BattlegroupSystem::getBattlegroups() const {
	return _battlegroups;
}

Battlegroup* BattlegroupSystem::getBattlegroup(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for a battlegroup with an empty name.");

	return getComponent<Battlegroup>(_battlegroups, name);
}

void BattlegroupSystem::addBattlegroup(Battlegroup *battlegroup) {
	if (!battlegroup)
		throw ProjectException("Cannot add a null battlegroup.");

	_battlegroups.append(battlegroup);

	Logger *logger = Logger::instance();
	logger->addMessage("Added battlegroup " + battlegroup->getName() + " to the project");
	logger->setStatus(true);
}

void BattlegroupSystem::removeBattlegroup(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an battlegroup with an empty name.");

	Battlegroup *battlegroup = removeComponentAndLog(_battlegroups, name, "battlegroup", "project");

	if (!!battlegroup)
		delete battlegroup;
}

BattlegroupSystem* BattlegroupSystem::create() {
	return new BattlegroupSystem;
}

BattlegroupSystem* BattlegroupSystem::translateFromXML(QDomElement &element) {
	BattlegroupSystem *system = create();

	QDomNodeList battlegroupElements = element.childNodes();

	for (int i = 0; i < battlegroupElements.size(); i++) {
		QDomElement battlegroupElement = battlegroupElements.at(i).toElement();
		system->addBattlegroup(Battlegroup::translateFromXML(battlegroupElement));
	}

	return system;
}

void BattlegroupSystem::translateToXML(BattlegroupSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("battlegroups");

	const QVector<Battlegroup*> battlegroups = system->getBattlegroups();
	for (int i = 0; i < battlegroups.size(); i++) {
		Battlegroup::translateToXML(battlegroups.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}