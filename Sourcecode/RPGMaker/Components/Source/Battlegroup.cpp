#include "qfile.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/Battlegroup.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Battlegroup::Battlegroup(QString name)
	: _name(name), _enemies(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created battlegroup " + _name);
		logger->setStatus(true);
}

Battlegroup::~Battlegroup() {
	emit deleteSignal(_name);

	Logger *logger = Logger::instance();
	logger->addMessage("Destroyed logger " + _name);
	logger->setStatus(true);
}

QString Battlegroup::getName() const {
	return _name;
}

QString Battlegroup::getVictoryMusic() const {
	return _victoryMusic;
}

QString Battlegroup::getBackgroundMusic() const {
	return _backgroundMusic;
}

QVector<Enemy*> Battlegroup::getEnemies() const {
	return _enemies;
}

Enemy* Battlegroup::getEnemy(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an enemy with an empty name.");

	return getComponent<Enemy>(_enemies, name);
}

Enemy* Battlegroup::getEnemy(int position) const {
	if ((position < 0) || (position >= _enemies.size()))
		throw ProjectException("The index " + QString::number(position) + " does not exist.");

	return _enemies.at(position);
}

void Battlegroup::addEnemy(Enemy *enemy) {
	if (!enemy)
		throw ProjectException("Cannot add a null enemy.");

	if (_enemies.size() == g_maxBattlegroupsize)
		throw ProjectException("Cannot add another enemy. The battlegroup is full.");

	_enemies.append(enemy);
	connect(enemy, SIGNAL(deleteSignal(QString)), this, SLOT(enemyDeleted(QString)), Qt::UniqueConnection);

	Logger *logger = Logger::instance();
	logger->addMessage("Added " + enemy->getName() + " to battlegroup " + _name);
	logger->setStatus(true);
}

void Battlegroup::setName(QString name) {
	if (name == QString())
		throw ProjectException("A battlegroup should not have an empty name.");

	_name = name;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed name of battlegroup to " + _name);
	logger->setStatus(true);
}

void Battlegroup::setVictoryMusic(QString name) {
	if (name == QString())
		_victoryMusic = QString();
	else {
		if (!QFile::exists(g_backgroundMusicPath + name))
			throw ProjectException("There is no music file called " + name + " in folder " + g_backgroundMusicPath + ".");
		else
			_victoryMusic = name;
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed victory music of battlegroup " + _name);
	logger->setStatus(true);
}

void Battlegroup::setBackgroundMusic(QString name) {
	if (name == QString())
		_backgroundMusic = QString();
	else {
		if (!QFile::exists(g_backgroundMusicPath + name))
			throw ProjectException("There is no music file called " + name + " in folder " + g_backgroundMusicPath + ".");
		else
			_backgroundMusic = name;
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed battle music of battlegroup " + _name);
	logger->setStatus(true);
}

void Battlegroup::removeEnemy(QString name) {
	if (!getEnemy(name))
		throw ProjectException("Cannot remove enemy " + name + " because it is not a member of the battlegroup.");

	int i = 0;
	while (i < _enemies.size()) {
		Enemy *it = _enemies.at(i);
		if (!!it && (it->getName() == name)) {
			_enemies.remove(i);

			if (!getEnemy(name))
				disconnect(it, 0, this, 0);

		} else
			i++;
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Removed enemy " + name + " from battlegroup " + _name);
	logger->setStatus(true);
}

void Battlegroup::removeEnemy(int position) {
	if ((position < 0) || (position >= _enemies.size()))
		throw ProjectException("The index " + QString::number(position) + " does not exist.");

	Enemy *enemy = _enemies.at(position);
	_enemies.remove(position);

	if (!getEnemy(enemy->getName()))
		disconnect(enemy, 0, this, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed enemy " + enemy->getName() + " from battlegroup " + _name);
	logger->setStatus(true);
}

Battlegroup* Battlegroup::create(QString name) {
	if (name == QString())
		throw ProjectException("Cannot create a battlegroup with an empty name.");

	return new Battlegroup(name);
}

Battlegroup* Battlegroup::translateFromXML(QDomElement element) {
	QString name = element.attributeNode("name").value();
	Battlegroup *group = Battlegroup::create(name);

	QDomElement soundElement = element.firstChildElement("sound");
	group->setBackgroundMusic(soundElement.firstChildElement("backgroundmusic").attributeNode("title").value());
	group->setVictoryMusic(soundElement.firstChildElement("victorymusic").attributeNode("title").value());

	return group;
}

void Battlegroup::translateToXML(Battlegroup *group, QDomDocument &document, QDomElement &parent) {
	QDomElement groupElement = document.createElement("battlegroup");
	groupElement.setAttribute("name", group->getName());

	// Sound
	QDomElement soundElement = document.createElement("sound");
	QDomElement backgroundmusicElement = document.createElement("backgroundmusic");
	backgroundmusicElement.setAttribute("title", group->getBackgroundMusic());
	soundElement.appendChild(backgroundmusicElement);
	QDomElement victorymusicElement = document.createElement("victorymusic");
	victorymusicElement.setAttribute("title", group->getVictoryMusic());
	soundElement.appendChild(victorymusicElement);
	groupElement.appendChild(soundElement);

	// Enemies
	QVector<Enemy*> enemies = group->getEnemies();
	for (int i = 0; i < enemies.size(); i++) {
		Enemy *it = enemies.at(i);
		QDomElement enemyElement = document.createElement("enemy");
		enemyElement.setAttribute("name", it->getName());
		groupElement.appendChild(enemyElement);
	}

	parent.appendChild(groupElement);
}

void Battlegroup::enemyDeleted(QString name) {
	if (!!getEnemy(name)) {
		removeEnemy(name);
	}
}