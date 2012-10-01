#include "../Logger.h"
#include "../Headers/EnemySystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

EnemySystem::EnemySystem()
	: _enemies(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an enemy system");
		logger->setStatus(true);
}

EnemySystem::~EnemySystem() {
	for (int i = 0; i < _enemies.size(); i++) {
		Enemy *it = _enemies.at(i);

		if (!!it)
			delete it;
	}

	_enemies.clear();
}

const QVector<Enemy*>& EnemySystem::getEnemies() const {
	return _enemies;
}

Enemy* EnemySystem::getEnemy(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an enemy with an empty name.");

	return getComponent<Enemy>(_enemies, name);
}

void EnemySystem::addEnemy(Enemy *enemy) {
	if (!enemy)
		throw ProjectException("Cannot add a null enemy.");

	_enemies.append(enemy);

	Logger *logger = Logger::instance();
	logger->addMessage("Added enemy " + enemy->getName() + " to the project");
	logger->setStatus(true);
}

void EnemySystem::removeEnemy(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an enemy with an empty name.");

	Enemy *enemy = removeComponentAndLog<Enemy>(_enemies, name, "enemy", "project");

	if (!!enemy)
		delete enemy;
}

EnemySystem* EnemySystem::create() {
	return new EnemySystem;
}

EnemySystem* EnemySystem::translateFromXML(QDomElement &element) {
	EnemySystem *system = create();

	QDomNodeList enemyElements = element.childNodes();

	for (int i = 0; i < enemyElements.size(); i++) {
		QDomElement enemyElement = enemyElements.at(i).toElement();
		system->addEnemy(Enemy::translateFromXML(enemyElement));
	}

	return system;
}

void EnemySystem::translateToXML(EnemySystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("enemies");

	const QVector<Enemy*> enemies = system->getEnemies();
	for (int i = 0; i < enemies.size(); i++) {
		Enemy::translateToXML(enemies.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}