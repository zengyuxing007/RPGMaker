#include "qfileinfo.h"

#include "../Logger.h"
#include "../Headers/Enemy.h"
#include "../ProgramDefaults.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Enemy::Enemy(QString name)
	: _name(name), _class(0), _level(0),
	_elements(QVector<Element*>(0)),
	_abilities(QVector<Ability*>(0)),
	_drops(EnemyDrops::create()) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created enemy " + _name);
}

Enemy::~Enemy() {
	emit deleteSignal(_name);

	this->_abilities.clear();
	this->_elements.clear();
	_class = 0;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed enemy " + _name);
}

QString Enemy::getName() const {
	return this->_name;
}

QString Enemy::getDescription() const {
	return this->_description;
}

int Enemy::getLevel() const {
	return this->_level;
}

Class* Enemy::getClass() const {
	return this->_class;
}

const QVector<Element*>& Enemy::getElements() const {
	return this->_elements;
}

const QVector<Ability*>& Enemy::getAbilities() const {
	return _abilities;
}

QVector<QPair<Ability*, int>> Enemy::getAllAbilities() const {
	QVector<QPair<Ability*, int>> abilities(0);

	if (!!_class) {
		QVector<QPair<Ability*, int>> classAbilities = _class->getAbilities();
		for (int i = 0; i < classAbilities.size(); i++)
			abilities.append(classAbilities.at(i));
	}

	for (int i = 0; i < _abilities.size(); i++)
		abilities.append(QPair<Ability*, int>(_abilities.at(i), 0));

	return abilities;
}

QPair<QPixmap, QString> Enemy::getBattleImage() const {
	return _battleimage;
}

QPair<QPixmap, QString> Enemy::getEnvironmentImage() const {
	return _environmentimage;
}

EnemyDrops* Enemy::getDrops() const {
	return _drops;
}

void Enemy::setName(QString name) {
	if (name == QString())
		throw ProjectException("An enemy should not have an empty name");

	this->_name = name;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of enemy " + _name);
}

void Enemy::setDescription(QString description) {
	this->_description = description;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed description of enemy " + _name);
}

void Enemy::setLevel(int exp) {
	this->_level = exp;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed level of enemy " + _name);
}

void Enemy::setClass(Class *newClass) {
	if (!!_class)
		disconnect(_class, 0, this, 0);

	for (int i = 0; i < _abilities.size(); i++) {
		Ability *it = _abilities.at(i);
		if (!!newClass->getAbility(it->getName()).first) {
			_abilities.remove(i);
			i--;
		}
	}

	_class = newClass;
	connect(newClass, SIGNAL(deleteSignal(QString)), this, SLOT(deleteClass(QString)));

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed class of enemy " + _name);
}

void Enemy::setBattleImage(QString imagename) {
	QFileInfo imageinfo(g_battlersPath + imagename);
	if (!imageinfo.exists())
		throw ProjectException("The image " + imagename + "does not exist in the path " + g_battlersPath + ".");

	_battleimage.first.load(g_battlersPath + imagename);
	_battleimage.second = imagename;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed battler image of enemy " + _name);
}

void Enemy::setEnvironmentImage(QString imagename) {
	QFileInfo imageinfo(g_charactersPath + imagename);
	if (!imageinfo.exists())
		throw ProjectException("The image " + imagename + "does not exist in the path " + g_charactersPath + ".");

	_environmentimage.first.load(g_charactersPath + imagename);
	_environmentimage.second = imagename;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed environmental image of enemy " + _name);
}

void Enemy::setDrops(EnemyDrops *drops) {
	if (!drops)
		throw ProjectException("Cannot set null drops.");

	_drops = drops;

	Logger *logger = Logger::instance();
	logger->addMessage("Placed enemy drops for enemy " + _name);
	logger->setStatus(true);
}

void Enemy::addElement(Element *element) {
	addComponentAndLog<Element>(_elements, element, "element", "enemy " + _name);
	connect(element, SIGNAL(deleteSignal(QString)), this, SLOT(deleteElement(QString)));
}

void Enemy::addAbility(Ability *newAbility) {
	if (!!_class && !!_class->getAbility(newAbility->getName()).first)
		throw ProjectException("This ability is already registered under the class of this enemy.");

	addComponentAndLog<Ability>(_abilities, newAbility, "ability", "enemy " + _name);
	connect(newAbility, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAbility(QString)));
}

void Enemy::removeElement(Element *element) {
	this->removeElement(element->getName());
}

void Enemy::removeElement(QString name) {
	Element *element = removeComponentAndLog<Element>(_elements, name, "element", "enemy " + _name);
	if (!!element)
		disconnect(element, 0, this, 0);
}

void Enemy::removeAbility(Ability *ability) {
	this->removeAbility(ability->getName());
}

void Enemy::removeAbility(QString name) {
	Ability *ability = removeComponentAndLog<Ability>(_abilities, name, "ability", "enemy " + _name);
	if (!!ability)
		disconnect(ability, 0, this, 0);
}

void Enemy::unsetClass() {
	if (!!_class)
		disconnect(_class, 0, this, 0);

	_class = 0;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Removed class from enemy " + _name);
}

Enemy* Enemy::create(QString name) {
	if (name == QString())
		throw ProjectException("An enemy should not have an empty name.");

	Enemy *enemy = new Enemy(name);
	return enemy;
}

Enemy* Enemy::translateFromXML(QDomElement &enemyElement) {
	QString name = enemyElement.attributeNode("name").value();
	Enemy *enemy = Enemy::create(name);
	enemy->setLevel(enemyElement.firstChildElement("level").attributeNode("value").value().toInt());
	enemy->setDescription(enemyElement.firstChildElement("description").text());
	enemy->setDrops(EnemyDrops::translateFromXML(enemyElement.firstChildElement("drops")));

	QDomElement imagesElement = enemyElement.firstChildElement("images");
	QDomElement environmentElement = imagesElement.firstChildElement("environment");
	QDomElement battleElement = imagesElement.firstChildElement("battle");

	QString environmentImageName = environmentElement.attributeNode("image").value();
	QString battleImageName = battleElement.attributeNode("image").value();

	if (environmentImageName != QString())
		enemy->setEnvironmentImage(environmentImageName);

	if (battleImageName != QString())
		enemy->setBattleImage(battleImageName);

	return enemy;
}

void Enemy::translateToXML(Enemy *enemy, QDomDocument &document, QDomElement &parent) {
	QDomElement enemyElement = document.createElement("enemy");
	enemyElement.setAttribute("name", enemy->_name);

	QDomElement level = document.createElement("level");
	level.setAttribute("value", enemy->getLevel());
	enemyElement.appendChild(level);

	if (enemy->_class) {
		QDomElement cat = document.createElement("class");
		cat.setAttribute("name", enemy->_class->getName());
		enemyElement.appendChild(cat);
	}

	QDomElement description = document.createElement("description");
	QDomText text = document.createTextNode(enemy->_description);
	description.appendChild(text);
	enemyElement.appendChild(description);

	QDomElement elements = document.createElement("elements");
	for (int i = 0; i < enemy->_elements.size(); i++) {
		Element *it = enemy->_elements.at(i);
		QDomElement element = document.createElement("element");
		element.setAttribute("name", it->getName());
		elements.appendChild(element);
	}
	enemyElement.appendChild(elements);

	QDomElement abilities = document.createElement("abilities");
	for (int i = 0; i < enemy->_abilities.size(); i++) {
		Ability* it = enemy->_abilities.at(i);
		QDomElement ability = document.createElement("ability");
		ability.setAttribute("name", it->getName());
		abilities.appendChild(ability);
	}
	enemyElement.appendChild(abilities);

	QDomElement drops = document.createElement("drops");
	EnemyDrops::translateToXML(enemy->_drops, document, drops);
	enemyElement.appendChild(drops);

	QDomElement images = document.createElement("images");
	QDomElement environment = document.createElement("environment");
	environment.setAttribute("image", enemy->_environmentimage.second);
	images.appendChild(environment);
	QDomElement battle = document.createElement("battle");
	battle.setAttribute("image", enemy->_battleimage.second);
	images.appendChild(battle);
	enemyElement.appendChild(images);

	parent.appendChild(enemyElement);
}

void Enemy::deleteElement(QString name) {
	try {
		removeElement(name);
	} catch(ProjectException &e) {}
}

void Enemy::deleteAbility(QString name) {
	try {
		removeAbility(name);
	} catch (ProjectException &e) {}
}

void Enemy::deleteClass(QString name) {
	try { 
		unsetClass();
	} catch (ProjectException &e) {}
}