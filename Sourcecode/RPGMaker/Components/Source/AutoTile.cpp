#include "../Headers/AutoTile.h"
#include "../Headers/AutoTileset.h"

using namespace projectcomponents;

AutoTile::AutoTile(Tileset *tileset, QPoint location) 
	: Tile(AUTO, tileset, location),
	_surroundingTiles(QVector<Tile*>(9, 0)) {}

AutoTile::~AutoTile() {
	_surroundingTiles.clear();
}

QPixmap AutoTile::getImage() const {
	AutoTileset *tileset = dynamic_cast<AutoTileset*>(_tileset);
	if (!tileset)
		throw ProjectException("The tileset this tile belongs to is not of the right type.");

	return tileset->getTile(_location, _surroundingTiles);
}

QVector<Tile*> AutoTile::getSurroundingtiles() const {
	return _surroundingTiles;
}

void AutoTile::setSurroundingTile(QPoint location, Tile *tile) {
	if ((location.y() < 0) || (location.y() > 2))
		throw ProjectException("The row index " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() > 2))
		throw ProjectException("The column index " + QString::number(location.x()) + " does not exist.");

	if ((location.x() == 1) && (location.y() == 1))
		throw ProjectException("The current tile cannot surround its self.");

	_surroundingTiles.replace((location.y() * 3) + location.x(), tile);
}

void AutoTile::setSurroundingTiles(QVector<Tile*> tiles) {
	if (tiles.size() != 9)
		throw ProjectException("The surrounding tiles must have a length of 9 tiles.");

	if (!!tiles.at(4))
		throw ProjectException("The tile at index 4 should be 0.");

	_surroundingTiles = tiles;
}

AutoTile* AutoTile::create(Tileset *tileset, QPoint location) {
	if (tileset->getType() != Tileset::AUTO)
		throw ProjectException("No suitable tileset is given to build an auto tile.");

	QSize dimensions = tileset->getDimensions();
	if ((location.x() < 0) || (location.x() >= dimensions.width()))
		throw ProjectException("The column index " + QString::number(location.x()) + " is not valid.");

	if ((location.y() < 0) || (location.y() >= dimensions.height()))
		throw ProjectException("The row index " + QString::number(location.y()) + " is not valid.");


	AutoTile *tile = new AutoTile(tileset, location);
	return tile;
}