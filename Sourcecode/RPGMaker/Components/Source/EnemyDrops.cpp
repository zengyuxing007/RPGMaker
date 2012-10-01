#include "../Logger.h"
#include "../Headers/EnemyDrops.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

EnemyDrops::EnemyDrops()
	: _experience(0),
	_currency(QPair<int, int>(0, 0)),
	_drops(QVector<QPair<EquipmentPiece*, int>>(0)) {}

EnemyDrops::~EnemyDrops() {}

int EnemyDrops::getExperience() const {
	return _experience;
}

QPair<int, int> EnemyDrops::getCurrency() const {
	return _currency;
}

const QVector<QPair<EquipmentPiece*, int>>& EnemyDrops::getItems() const {
	return _drops;
}

void EnemyDrops::setExperience(int experience) {
	if (experience < 0)
		throw ProjectException("The experience should be greater than 0.");

	_experience = experience;

	Logger *logger = Logger::instance();
	logger->addMessage("Set experience of enemy drops");
	logger->setStatus(true);
}

void EnemyDrops::setCurrency(QPair<int, int> currency) {
	if ((currency.first < 0) || (currency.second < 0))
		throw ProjectException("The amount of currency should be more than 0.");

	if (currency.first > currency.second)
		throw ProjectException("The minimum currency should be smaller than the maximum currency.");

	_currency = currency;

	Logger *logger = Logger::instance();
	logger->addMessage("Set currency of enemy drops");
	logger->setStatus(true);
}

void EnemyDrops::addItem(EquipmentPiece *item, int chance) {
	if (!item)
		throw ProjectException("A null item cannot be placed as a drop.");

	if ((chance < 0) || (chance > 100))
		throw ProjectException("An item should have a drop chance between 0 and 100.");

	_drops.append(QPair<EquipmentPiece*, int>(item, chance));
	connect(item, SIGNAL(deleteSignal(QString)), this, SLOT(deleteItem(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Added " + item->getName() + " to drops");
	logger->setStatus(true);
}

void EnemyDrops::editItem(int position, int chance) {
	if ((position < 0) || (position >= _drops.size()))
		throw ProjectException("The index is out of bounds.");

	if ((chance < 0) || (chance > 100))
		throw ProjectException("An item should have a drop chance between 0 and 100.");

	_drops.replace(position, QPair<EquipmentPiece*, int>(_drops.at(position).first, chance));

	Logger *logger = Logger::instance();
	logger->addMessage("Changed drop chance of drop");
	logger->setStatus(true);
}

void EnemyDrops::removeItem(int position) {
	if ((position < 0) || (position >= _drops.size()))
		throw ProjectException("The index is out of bounds.");

	QPair<EquipmentPiece*, int> drop = _drops.at(position);
	
	int amount = 0;
	for (int i = 0; i < _drops.size(); i++) {
		QPair<EquipmentPiece*, int> it = _drops.at(i);
		if (it.first == drop.first)
			amount++;
	}

	if (amount == 1)
		disconnect(drop.first, 0, this, 0);

	_drops.remove(position);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed drop from drops");
	logger->setStatus(true);
}

EnemyDrops* EnemyDrops::create() {
	return new EnemyDrops();
}

EnemyDrops* EnemyDrops::translateFromXML(QDomElement &element) {
	EnemyDrops *drops = EnemyDrops::create();
	drops->setExperience(element.firstChildElement("experience").attributeNode("value").value().toInt());
	QPair<int, int> currency;
	currency.first = element.firstChildElement("currency").attributeNode("minValue").value().toInt();
	currency.second = element.firstChildElement("currency").attributeNode("maxValue").value().toInt();
	drops->setCurrency(currency);

	return drops;
}

void EnemyDrops::translateToXML(EnemyDrops *drops, QDomDocument &document, QDomElement &parent) {
	QDomElement experienceElement = document.createElement("experience");
	experienceElement.setAttribute("value", drops->_experience);
	parent.appendChild(experienceElement);

	QDomElement currencyElement = document.createElement("currency");
	currencyElement.setAttribute("minValue", drops->_currency.first);
	currencyElement.setAttribute("maxValue", drops->_currency.second);
	parent.appendChild(currencyElement);

	QDomElement equipmentElement = document.createElement("equipment");
	for (int i = 0; i < drops->_drops.size(); i++) {
		QPair<EquipmentPiece*, int> it = drops->_drops.at(i);
		QDomElement dropElement;

		switch (it.first->getType()) {
		case EquipmentPiece::WEAPON:
			dropElement = document.createElement("weapon");
			break;
		case EquipmentPiece::ARMOR:
			dropElement = document.createElement("armor");
			break;
		case EquipmentPiece::ACCESSORY:
			dropElement = document.createElement("accessory");
			break;
		}

		dropElement.setAttribute("name", it.first->getName());
		dropElement.setAttribute("chance", it.second);
		equipmentElement.appendChild(dropElement);
	}

	parent.appendChild(equipmentElement);
}

void EnemyDrops::deleteItem(QString name) {
	for (int i = 0; i < _drops.size();) {
		if (_drops.at(i).first->getName() == name)
			_drops.remove(i);
		else
			i++;
	}
}