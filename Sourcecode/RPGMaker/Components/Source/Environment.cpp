#include "qfile.h"
#include "qpainter.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/AutoTile.h"
#include "../Headers/StaticTile.h"
#include "../Headers/Environment.h"

using namespace projectcomponents;

Environment::Environment(QString name, QSize dimensions)
	: _name(name),
	_dimensions(dimensions),
	_backgroundMusic(QString()),
	_battlebackName(QString()),
	_battlebackImage(QPixmap()),
	_events(_dimensions.width() * _dimensions.height(), 0),
	_layers(QVector<QVector<Tile*>>
	(g_layers, QVector<Tile*>(_dimensions.width() * _dimensions.height(), 0))) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created environment " + _name);
}

Environment::~Environment() {
	emit deleteSignal(_name);

	for (int i = 0; i < _layers.size(); i++) {
		QVector<Tile*> layer = _layers.at(i);
		for (int j = 0; j < layer.size(); j++) {
			Tile *it = layer.at(j);
			if (!!it)
				delete it;
		}
	}
	_layers.clear();

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed environment " + _name);
}

QString Environment::getName() const {
	return _name;
}

QSize Environment::getDimensions() const {
	return _dimensions;
}

QVector<Event*> Environment::getEvents() const {
	return _events;
}

QString Environment::getBackgroundMusic() const {
	return _backgroundMusic;
}

QString Environment::getBattlebackName() const {
	return _battlebackName;
}

QPixmap Environment::getBattlebackImage() const {
	return _battlebackImage;
}

QVector<Tile*> Environment::getLayer(int layer) const {
	if (layer < 0 || layer >= _layers.size())
		throw ProjectException("The index of layer " + QString::number(layer) + " does not exist.");

	return _layers.at(layer);
}

Event* Environment::getEvent(QPoint location) const {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The index of row " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The index of column " + QString::number(location.x()) + " does not exist.");

	return _events.at(_dimensions.width() * location.y() + location.x());
}

Tile* Environment::getTile(int layer, QPoint location) const {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The index of row " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The index of column " + QString::number(location.x()) + " does not exist.");

	if (layer < 0 || layer >= _layers.size())
		throw ProjectException("The index of layer " + QString::number(layer) + " does not exist.");

	return _layers.at(layer).at(_dimensions.width() * location.y() + location.x());
}

Tileset::Access Environment::getAccess(QPoint location) const {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The index of row " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The index of column " + QString::number(location.x()) + " does not exist.");

	Event *event = getEvent(location);
	if (!!event && !!event->getObject())
		return Tileset::NONE;

	Tileset::Access access = Tileset::NONE;

	for (int i = 0; i < _layers.size(); i++) {
		Tile *tile = getTile(i, location);

		if (!!tile) {
			if (tile->getAccess() == Tileset::UNDER)
				return access;
			else
				access = tile->getAccess();
		}
	}

	return access;
}

void Environment::setName(QString name) {
	if (name == QString())
		throw ProjectException("An environment should not have an empty name.");

	_name = name;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of environment to " + _name);
}

void Environment::setDimensions(QSize dimensions) {
	if (!dimensions.isValid())
		throw ProjectException("The dimensions of an environment should be strictly greater than 0.");

	if (dimensions == _dimensions)
		return;

	int rows = qMin<int>(dimensions.height(), _dimensions.height());
	int columns = qMin<int>(dimensions.width(), _dimensions.width());

	// Tiles
	for (int i = 0; i < _layers.size(); i++) {
		// Move tiles from old layers to new layers
		QVector<Tile*> tileReplacement(dimensions.width()*dimensions.height(), 0);
		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				tileReplacement.replace(dimensions.width() * row + column, _layers.at(i).at(_dimensions.width() * row + column));

				// The autotiles in the last column need to know they have no surrounding tiles to their right
				if (column == (columns-1)) {
					Tile *tile = getTile(i, QPoint(column, row));
					if (!!tile && (tile->getType() == Tile::AUTO)) {
						AutoTile *autotile = dynamic_cast<AutoTile*>(tile);
						autotile->setSurroundingTile(QPoint(2, 0), 0);
						autotile->setSurroundingTile(QPoint(2, 1), 0);
						autotile->setSurroundingTile(QPoint(2, 2), 0);
					}
				}

				// The autotiles in the last row need to know they have no surrounding tiles below them
				if (row == (rows-1)) {
					Tile *tile = getTile(i, QPoint(column, row));
					if (!!tile && (tile->getType() == Tile::AUTO)) {
						AutoTile *autotile = dynamic_cast<AutoTile*>(tile);
						autotile->setSurroundingTile(QPoint(0, 2), 0);
						autotile->setSurroundingTile(QPoint(1, 2), 0);
						autotile->setSurroundingTile(QPoint(2, 2), 0);
					}
				}
			}
		}

		// Delete excess tiles, if any
		QVector<Tile*> currentlayer = _layers.at(i);
		for (int row = rows; row < _dimensions.height(); row++) {
			for (int column = columns; column < _dimensions.width(); column++) {
				Tile *tile = getTile(i, QPoint(column, row));
				if (!!tile)
					delete tile;
			}
		}

		_layers.replace(i, tileReplacement);
	}

	// Events
	QVector<Event*> eventReplacement(dimensions.width() * dimensions.height(), 0);
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			eventReplacement.replace(dimensions.width() * row + column, _events.at(_dimensions.width() * row + column));
		}
	}
	_events = eventReplacement;

	_dimensions = dimensions;
	emit resizeSignal(_name);

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed dimensions of environment " + _name);
}

void Environment::setEvent(Event *event, QPoint location) {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The index of row " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The index of column " + QString::number(location.x()) + " does not exist.");

	_events.replace(location.y() * _dimensions.width() + location.x(), event);
	connect(event, SIGNAL(deleteSignal(QString)), this, SLOT(eventDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Added event " + event->getName() + " to environment " + _name);
	logger->setStatus(true);
}

void Environment::setBackgroundMusic(QString name) {
	if (name == QString())
		_backgroundMusic = name;
	else {
		if (!QFile::exists(g_backgroundMusicPath + name))
			throw ProjectException("There is no music file called " + name + " in the folder " + g_backgroundMusicPath + ".");
		else
			_backgroundMusic = name;
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed background music of environment " + _name);
	logger->setStatus(true);
}

void Environment::setBattleback(QString name) {
	if (name == QString()) {
		_battlebackName = name;
		_battlebackImage = QPixmap();
	} else {
		if (!QFile::exists(g_battlebackPath + name))
			throw ProjectException("There is no battleback called " + name + " in the folder " + g_battlebackPath);
		else {
			_battlebackName = name;
			_battlebackImage = QPixmap(g_battlebackPath + name);
		}
	}

	Logger *logger = Logger::instance();
	logger->addMessage("Changed battleback of environment " + _name);
	logger->setStatus(true);
}

QVector<QPoint> Environment::setTile(Tile *tile, int layer, QPoint location) {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The index of row " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The index of column " + QString::number(location.x()) + " does not exist.");

	if (layer < 0 || layer >= _layers.size())
		throw ProjectException("The index of layer " + QString::number(layer) + " does not exist.");

	Tile *oldTile = _layers[layer].at(location.y() * _dimensions.width() + location.x());
	if (!!oldTile)
		delete oldTile;

	_layers[layer].replace((location.y() * _dimensions.width() + location.x()), tile);

	QVector<QPoint> alteredtiles(0);
	alteredtiles.append(location);

	QVector<Tile*> surroundingtiles(9, 0);
	for (int y = -1; y < 2; y++) {
		for (int x = -1; x < 2; x++) {
			if ((x == 0) && (y == 0))
				continue;

			try {
				surroundingtiles.replace((y+1)*3 + (x+1), getTile(layer, QPoint(location.x()+x, location.y()+y)));
			} catch (ProjectException &e) {
				surroundingtiles.replace((y+1)*3 + (x+1), 0);
			}

			try {
				Tile *surroundingtile = getTile(layer, QPoint(location.x()+x, location.y()+y));
				if (!!surroundingtile && (surroundingtile->getType() == Tile::AUTO)) {
					AutoTile *autotile = dynamic_cast<AutoTile*>(surroundingtile);
					autotile->setSurroundingTile(QPoint(qAbs(x-1), qAbs(y-1)), tile);
					alteredtiles.append(QPoint(location.x()+x, location.y()+y));
				}
			} catch (ProjectException &e) {}
			
		}
	}

	if (!!tile) {
		if (tile->getType() == Tile::AUTO) {
			AutoTile *autotile = dynamic_cast<AutoTile*>(tile);
			autotile->setSurroundingTiles(surroundingtiles);
		}

		connect(tile->getTileset(), SIGNAL(deleteSignal(QString)), this, SLOT(tilesetDeleted(QString)), Qt::UniqueConnection);
	}

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed tile of environment " + _name);

	return alteredtiles;
}

QVector<QPoint> Environment::floodLayer(Tile *tile, int layer, QPoint location) {
	if (layer < 0 || layer >= _layers.size())
		throw ProjectException("The index of layer " + QString::number(layer) + " does not exist.");

	Tile *oldTile = 0;
	if (!!getTile(layer, location)) {
		Tile *temp = getTile(layer, location);
		switch (temp->getType()) {
		case Tile::STATIC:
			oldTile = StaticTile::create(temp->getTileset(), temp->getLocation());
			break;
		case Tile::AUTO:
			oldTile = AutoTile::create(temp->getTileset(), temp->getLocation());
			break;
		}
	}

	QVector<QPair<Tile*, QPoint>> uncheckedQueue(0);
	QVector<QPoint> checkedQueue(0);
	QVector<QPoint> alteredTiles(0);

	uncheckedQueue.append(QPair<Tile*, QPoint>(oldTile, location));
	while (!uncheckedQueue.empty()) {
		QPair<Tile*, QPoint> currentPair = uncheckedQueue.first();
		uncheckedQueue.pop_front();
		checkedQueue.append(currentPair.second);

		QVector<QPoint> resultTiles;
		if (!!tile) {
			switch (tile->getType()) {
			case Tile::STATIC:
				resultTiles = setTile(StaticTile::create(tile->getTileset(), tile->getLocation()), layer, currentPair.second);
				break;
			case Tile::AUTO:
				resultTiles = setTile(AutoTile::create(tile->getTileset(), tile->getLocation()), layer, currentPair.second);
				break;
			}
		} else {
			resultTiles = setTile(0, layer, currentPair.second);
		}
		
		for (int i = 0; i < resultTiles.size(); i++) {
			if (!alteredTiles.contains(resultTiles.at(i)))
				alteredTiles.append(resultTiles.at(i));
		}

		if ((currentPair.second.x() -1) >= 0) {
			QPoint uncheckedPoint(currentPair.second.x() -1, currentPair.second.y());
			Tile *uncheckedTile = getTile(layer, uncheckedPoint);
			QPair<Tile*, QPoint> uncheckedPair(uncheckedTile, uncheckedPoint);
			if (!!oldTile && !!uncheckedTile) {
				if ((uncheckedTile->getTileset() == oldTile->getTileset()) && (uncheckedTile->getLocation() == oldTile->getLocation()) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			} else {
				if ((uncheckedTile == oldTile) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			}
		}

		if ((currentPair.second.x() +1) < _dimensions.width()) {
			QPoint uncheckedPoint(currentPair.second.x() +1, currentPair.second.y());
			Tile *uncheckedTile = getTile(layer, uncheckedPoint);
			QPair<Tile*, QPoint> uncheckedPair(uncheckedTile, uncheckedPoint);
			if (!!oldTile && !!uncheckedTile) {
				if ((uncheckedTile->getTileset() == oldTile->getTileset()) && (uncheckedTile->getLocation() == oldTile->getLocation()) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			} else {
				if ((uncheckedTile == oldTile) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			}
		}

		if ((currentPair.second.y() -1) >= 0) {
			QPoint uncheckedPoint(currentPair.second.x(), currentPair.second.y() -1);
			Tile *uncheckedTile = getTile(layer, uncheckedPoint);
			QPair<Tile*, QPoint> uncheckedPair(uncheckedTile, uncheckedPoint);
			if (!!oldTile && !!uncheckedTile) {
				if ((uncheckedTile->getTileset() == oldTile->getTileset()) && (uncheckedTile->getLocation() == oldTile->getLocation()) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			} else {
				if ((uncheckedTile == oldTile) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			}
		}

		if ((currentPair.second.y() +1) < _dimensions.height()) {
			QPoint uncheckedPoint(currentPair.second.x(), currentPair.second.y() +1);
			Tile *uncheckedTile = getTile(layer, uncheckedPoint);
			QPair<Tile*, QPoint> uncheckedPair(uncheckedTile, uncheckedPoint);
			if (!!oldTile && !!uncheckedTile) {
				if ((uncheckedTile->getTileset() == oldTile->getTileset()) && (uncheckedTile->getLocation() == oldTile->getLocation()) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			} else {
				if ((uncheckedTile == oldTile) && !uncheckedQueue.contains(uncheckedPair) && !checkedQueue.contains(uncheckedPoint)) {
					uncheckedQueue.append(uncheckedPair);
				}
			}
		}
	}

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Flooded layer of environment " + _name);

	return alteredTiles;
}

void Environment::removeEvent(QPoint location) {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The index of row " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The index of column " + QString::number(location.x()) + " does not exist.");

	_events.replace(location.y() * _dimensions.width() + location.x(), 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed event from environment " + _name);
	logger->setStatus(true);
}

QVector<QPixmap> Environment::generateRender() const {
	QVector<QPixmap> render(0);
	for (int y = 0; y < _dimensions.height(); y++) {
		for (int x = 0; x < _dimensions.width(); x++) {
			QPixmap tileImage(g_tilesize, g_tilesize);
			tileImage.fill(Qt::transparent);
			QPainter painter(&tileImage);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

			for (int i = 0; i < _layers.size(); i++) {
				Tile *tile = getTile(i, QPoint(x, y));

				if (!!tile)
					painter.drawPixmap(0, 0, tile->getImage());
			}

			painter.end();

			render.append(tileImage);
		}
	}

	return render;
}

QVector<QPixmap> Environment::generateFloor() const {
	QVector<QPixmap> floor(0);

	for (int y = 0; y < _dimensions.height(); y++) {
		for (int x = 0; x < _dimensions.width(); x++) {

			QPixmap tileImage(g_tilesize, g_tilesize);
			tileImage.fill(Qt::transparent);
			QPainter painter(&tileImage);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

			for (int i = 0; i < _layers.size(); i++) {
				Tile *tile = getTile(i, QPoint(x, y));

				if (!!tile && (tile->getAccess() == Tileset::UNDER))
					break;

				if (!!tile)
					painter.drawPixmap(0, 0, tile->getImage());
			}

			painter.end();

			floor.append(tileImage);
		}
	}

	return floor;
}

QVector<QPixmap> Environment::generateCeiling() const {
	QVector<QPixmap> ceiling(0);

	for (int y = 0; y < _dimensions.height(); y++) {
		for (int x = 0; x < _dimensions.width(); x++) {
			Tileset::Access access = Tileset::NONE;

			QPixmap tileImage(g_tilesize, g_tilesize);
			tileImage.fill(Qt::transparent);
			QPainter painter(&tileImage);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

			for (int i = 0; i < _layers.size(); i++) {
				Tile *tile = getTile(i, QPoint(x, y));

				if (!!tile) {
					if (tile->getAccess() == Tileset::UNDER)
						access = Tileset::UNDER;

					if (access == Tileset::UNDER)
						painter.drawPixmap(0, 0, tile->getImage());
				}
			}

			painter.end();

			ceiling.append(tileImage);
		}
	}

	return ceiling;
}

QVector<Tileset::Access> Environment::generateAccessGrid() const {
	QVector<Tileset::Access> accessGrid(0);

	for (int y = 0; y < _dimensions.height(); y++) {
		for (int x = 0; x < _dimensions.width(); x++) {
			Tileset::Access access = Tileset::NONE;

			for (int i = 0; i < _layers.size(); i++) {
				Tile *tile = getTile(i, QPoint(x, y));

				if (!!tile && (tile->getAccess() == Tileset::UNDER))
					break;

				if (!!tile)
					access = tile->getAccess();
			}
			accessGrid.append(access);
		}
	}

	return accessGrid;
}

Environment* Environment::create(QString name, QSize dimensions) {
	if (name == QString())
		throw ProjectException("An environment should not have an empty name.");

	if (!dimensions.isValid())
		throw ProjectException("The dimensions of an environment should be strictly greater than 0.");

	Environment *environment = new Environment(name, dimensions);
	return environment;
}

Environment* Environment::translateFromXML(QDomElement &environmentElement) {
	QString name = environmentElement.attributeNode("name").value();

	QSize dimensions;
	dimensions.setWidth(environmentElement.firstChildElement("dimensions").attributeNode("columns").value().toInt());
	dimensions.setHeight(environmentElement.firstChildElement("dimensions").attributeNode("rows").value().toInt());
	Environment *environment = Environment::create(name, dimensions);

	QDomElement soundElement = environmentElement.firstChildElement("sound");
	environment->setBackgroundMusic(soundElement.firstChildElement("backgroundmusic").attributeNode("title").value());

	QDomElement battleBackElement = environmentElement.firstChildElement("battleback");
	environment->setBattleback(battleBackElement.attributeNode("name").value());

	return environment;
}

void Environment::translateToXML(Environment *environment, QDomDocument &document, QDomElement &parent) {
	QDomElement environmentElement = document.createElement("environment");
	environmentElement.setAttribute("name", environment->_name);

	QDomElement dimensionsElement = document.createElement("dimensions");
	dimensionsElement.setAttribute("rows", QString::number(environment->_dimensions.height()));
	dimensionsElement.setAttribute("columns", QString::number(environment->_dimensions.width()));
	environmentElement.appendChild(dimensionsElement);

	QDomElement soundElement = document.createElement("sound");
	QDomElement backgroundmusicElement = document.createElement("backgroundmusic");
	backgroundmusicElement.setAttribute("title", environment->getBackgroundMusic());
	soundElement.appendChild(backgroundmusicElement);
	environmentElement.appendChild(soundElement);

	QDomElement battlebackElement = document.createElement("battleback");
	battlebackElement.setAttribute("name", environment->getBattlebackName());
	environmentElement.appendChild(battlebackElement);

	QDomElement layersElement = document.createElement("layers");
	for (int i = 0; i < environment->_layers.size(); i++) {
		QDomElement layerElement = document.createElement("layer");
		layerElement.setAttribute("nr", QString::number(i));
		QVector<Tile*> layer = environment->_layers.at(i);

		for (int y = 0; y < environment->getDimensions().height(); y++) {
			for (int x = 0; x < environment->getDimensions().width(); x++) {
				Tile *tile = environment->getTile(i, QPoint(x, y));
				if (!!tile) {
					QDomElement tileElement = document.createElement("tile");
					tileElement.setAttribute("row", QString::number(y));
					tileElement.setAttribute("column", QString::number(x));

					QDomElement tilesetElement = document.createElement("tileset");
					tilesetElement.setAttribute("name", tile->getTileset()->getName());
					tilesetElement.setAttribute("row", tile->getLocation().y());
					tilesetElement.setAttribute("column", tile->getLocation().x());
					
					tileElement.appendChild(tilesetElement);
					layerElement.appendChild(tileElement);
				}
			}
		}

		layersElement.appendChild(layerElement);
	}
	environmentElement.appendChild(layersElement);

	QDomElement eventsElement = document.createElement("events");
	QVector<Event*> events = environment->getEvents();
	for (int y = 0; y < environment->getDimensions().height(); y++) {
		for (int x = 0; x < environment->getDimensions().width(); x++) {

			Event *event = environment->getEvent(QPoint(x, y));
			if (!!event) {
				QDomElement eventElement = document.createElement("event");
				eventElement.setAttribute("ID", event->getName());
				eventElement.setAttribute("row", y);
				eventElement.setAttribute("column", x);
				eventsElement.appendChild(eventElement);
			}
		}
	}
	environmentElement.appendChild(eventsElement);

	parent.appendChild(environmentElement);
}

void Environment::tilesetDeleted(QString name) {
	for (int i = 0; i < _layers.size(); i++) {
		for (int y = 0; y < _dimensions.height(); y++) {
			for (int x = 0; x < _dimensions.width(); x++) {
				Tile *tile = getTile(i, QPoint(x, y));
				if (!!tile && (tile->getTileset()->getName() == name))
					setTile(0, i, QPoint(x, y));
			}
		}
	}
}

void Environment::eventDeleted(QString ID) {
	for (int i = 0; i < _events.size(); i++) {
		Event *it = _events.at(i);
		if (!!it && (it->getName() == ID))
			_events.replace(i, 0);
	}
}