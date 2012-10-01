#include "../Headers/Tile.h"

using namespace projectcomponents;

/**
*	Construct a tile with type <i>type</i> from tileset <i>tileset</i> at location <i>location</i>.
*/
Tile::Tile(Type type, Tileset *tileset, QPoint location)
	: _type(type),
	_tileset(tileset),
	_location(location) {}

/**
*	Standard destructor.
*/
Tile::~Tile() {}

/**
*	Returns the type of the tile.
*/
Tile::Type Tile::getType() const {
	return _type;
}

/**
*	Returns the tileset to which this tile belongs.
*/
Tileset* Tile::getTileset() const {
	return _tileset;
}

/**
*	Returns the location of the tile in the tileset to which this tile belongs.
*/
QPoint Tile::getLocation() const {
	return _location;
}

/**
*	Get the access level associated with this tile.
*/
Tileset::Access Tile::getAccess() const {
	return _tileset->getAccess(_location);
}

/**
*	Set the access level of this tile.
*/
void Tile::setAccess(Tileset::Access access) {
	_tileset->setAccess(_location, access);
}