#include "../Headers/StaticTile.h"
#include "../Headers/StaticTileset.h"

using namespace projectcomponents;

/**
*	Construct a static tile with a reference to tileset <i>tileset</i> at location <i>location</i> on this tileset.
*/
StaticTile::StaticTile(Tileset *tileset, QPoint location)
	: Tile(STATIC, tileset, location) {}

/**
*	Standard destructor.
*/
StaticTile::~StaticTile() {}

/**
*	Returns the image this tile is mapped to.
*/
QPixmap StaticTile::getImage() const {
	StaticTileset *tileset = dynamic_cast<StaticTileset*>(_tileset);
	if (!tileset)
		throw ProjectException("The tileset this tile belongs to is not of the right type.");

	return tileset->getTile(_location);
}

/**
*	Creates a static tile with a reference to tileset<i>tileset</i> at location<i>location</i> on this tileset and return a pointer to this tile.
*	The type of the tilset given must be of the STATIC type and the location must be valid, otherwise a <i>ProjectException</i> is thrown.
*/
StaticTile* StaticTile::create(Tileset *tileset, QPoint location) {
	if (tileset->getType() != Tileset::STATIC)
		throw ProjectException("No suitable tileset is given to build a static tile.");

	QSize dimensions = tileset->getDimensions();
	if ((location.x() < 0) || (location.x() >= dimensions.width()))
		throw ProjectException("The column index " + QString::number(location.x()) + " is not valid.");

	if ((location.y() < 0) || (location.y() >= dimensions.height()))
		throw ProjectException("The row index " + QString::number(location.y()) + " is not valid.");

	StaticTile *tile = new StaticTile(tileset, location);
	return tile;
}