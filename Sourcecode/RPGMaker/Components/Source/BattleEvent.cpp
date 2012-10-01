#include "qfile.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/BattleEvent.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

BattleEvent::BattleEvent(QString ID, Event::Trigger trigger)
	: Event(ID, Event::BATTLE, trigger), _activation(1), _groups(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created battle event with ID " + _name);
		logger->setStatus(true);
}

BattleEvent::~BattleEvent() {}

void BattleEvent::execute() {

}

const QVector<QPair<Battlegroup*, int>>& BattleEvent::getBattlegroups() const {
	return _groups;
}

QPair<Battlegroup*, int> BattleEvent::getBattlegroup(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for a battle group with an empty name.");

	return getPair<Battlegroup, int>(_groups, name);
}

int BattleEvent::getActivation() const {
	return _activation;
}

void BattleEvent::addBattlegroup(Battlegroup *battlegroup, int chance) {
	if (!battlegroup)
		throw ProjectException("Cannot add a null battle group.");

	if (!!getBattlegroup(battlegroup->getName()).first)
		throw ProjectException("A battle group with the same name is already registered to this event.");

	_groups.append(QPair<Battlegroup*, int>(battlegroup, chance));
	connect(battlegroup, SIGNAL(deleteSignal(QString)), this, SLOT(battlegroupDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Added a battle group to battle event " + _name);
	logger->setStatus(true);
}

void BattleEvent::setActivation(int activation) {
	if ((activation < 1) || (activation > 100))
		throw ProjectException("The activation chance of a battle event should lie between 1 and 100");

	_activation = activation;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed activation of battle event " + _name);
	logger->setStatus(true);
}

void BattleEvent::removeBattlegroup(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a battle group with an empty name.");

	QPair<Battlegroup*, int> group = removePairAndLog<Battlegroup, int>(_groups, name, "battle group", "battle event " + _name);
	if (!!group.first) {
		disconnect(group.first, 0, this, 0);

		Logger *logger = Logger::instance();
		logger->addMessage("Removed battle group from battle event " + _name);
		logger->setStatus(true);
	}
}

BattleEvent* BattleEvent::create(QString ID, Event::Trigger trigger) {
	if (ID == QString())
		throw ProjectException("Cannot create a battle event with an empty ID.");

	return new BattleEvent(ID, trigger);
}

BattleEvent* BattleEvent::translateFromXML(QDomElement element) {
	QString ID = element.attributeNode("ID").value();
	QDomElement typeElement = element.firstChildElement("type");
	QString typeString = typeElement.attributeNode("value").value();

	if (typeString != "BATTLE")
		throw ProjectException("The translation of a battle event should have type BATTLE instead of " + typeString + ".");

	Event::Trigger trigger = Event::NONE;
	QString triggerString = element.firstChildElement("trigger").attributeNode("value").value();
	if (triggerString == "TOUCH")
		trigger = Event::TOUCH;
	else if (triggerString == "INTERACT")
		trigger = Event::INTERACT;
	else if (triggerString == "NONE")
		trigger = Event::NONE;

	BattleEvent *event = create(ID, trigger);
	Event::translateFromXML(element, event);

	QDomElement activationElement = element.firstChildElement("activation");
	event->setActivation(activationElement.attributeNode("value").value().toInt());

	return event;
}

void BattleEvent::translateToXML(BattleEvent *event, QDomDocument &document, QDomElement &parent) {
	QDomElement eventElement = document.createElement("battleEvent");
	Event::translateToXML(event, document, eventElement);

	QDomElement activationElement = document.createElement("activation");
	activationElement.setAttribute("value", event->getActivation());
	eventElement.appendChild(activationElement);

	QVector<QPair<Battlegroup*, int>> groups = event->getBattlegroups();
	for (int i = 0; i < groups.size(); i++) {
		QPair<Battlegroup*, int> it = groups.at(i);
		QDomElement groupElement = document.createElement("battlegroup");
		groupElement.setAttribute("name", it.first->getName());
		groupElement.setAttribute("value", it.second);
		eventElement.appendChild(groupElement);
	}

	parent.appendChild(eventElement);
}

void BattleEvent::battlegroupDeleted(QString name) {
	if (!!getBattlegroup(name).first)
		removeBattlegroup(name);
}