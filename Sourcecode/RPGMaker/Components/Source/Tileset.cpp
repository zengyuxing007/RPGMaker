#include "qrect.h"

#include "../Logger.h"
#include "../Headers/Tileset.h"
#include "../ProgramDefaults.h"

using namespace projectcomponents;

/**
*	Construct a tileset with name <i>name</i>, type <i>type</i> and load the tileset image <i>imagename</i>.
*/
Tileset::Tileset(QString name, Type type, QString imagename)
	: _type(type),
	_name(name),
	_imagename(imagename) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created tileset " + _name);
}

/**
*	Standard destructor.
*/
Tileset::~Tileset() {
	emit deleteSignal(_name);

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed tileset " + _name);
}

/**
*	Returns the type of the tileset.
*/
Tileset::Type Tileset::getType() const {
	return _type;
}

/**
*	Returns the name of the tileset.
*/
QString Tileset::getName() const {
	return _name;
}

/**
*	Returns the name of the tileset image.
*/
QString Tileset::getImagename() const {
	return _imagename;
}

/**
*	Returns the dimensions of the tileset in the number of tiles per row and column.
*/
QSize Tileset::getDimensions() const {
	return _dimensions;
}

/**
*	Returns the tileset image.
*/
QPixmap Tileset::getImage() const {
	return _image;
}

/**
*	Returns the access mapping of each tile in the tileset.
*/
QVector<Tileset::Access> Tileset::getAccess() const {
	return _access;
}

/**
*	Returns the access mapping of the tile specified at location <i>location</i>.
*	The location has to be valid, a <i>ProjectException</i> is thrown otherwise.
*/
Tileset::Access Tileset::getAccess(QPoint location) const {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The row index " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The column index " + QString::number(location.x()) + " does not exist.");

	return _access.at(_dimensions.width() * location.y() + location.x());
}

/**
*	Set the name of the tileset.
*/
void Tileset::setName(QString name) {
	if (name == QString())
		throw ProjectException("A tileset cannot have an empty name.");

	_name = name;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of tileset to " + _name);
}

/**
*	Set the access level of the tile at location <i>location</i>.
*	The location has to be valid, a <i>ProjectException</i> is throw otherwise.
*/
void Tileset::setAccess(QPoint location, Access access) {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The row index " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The column index " + QString::number(location.x()) + " does not exist.");

	_access.replace(_dimensions.width() * location.y() + location.x(), access);

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed access of a tile in tileset " + _name);
}

/**
*	Get the next type of access level for the tile at location <i>location</i>.
*	The location has to be valid, a <i>ProjectException</i> is throw otherwise.
*/
Tileset::Access Tileset::scrollAccess(QPoint location) {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The row index " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The column index " + QString::number(location.x()) + " does not exist.");

	switch (getAccess(location)) {
	case Tileset::OVER:
		setAccess(location, Tileset::UNDER);
		return getAccess(location);
		break;
	case Tileset::UNDER:
		setAccess(location, Tileset::NONE);
		return getAccess(location);
		break;
	case  Tileset::NONE:
		setAccess(location, Tileset::OVER);
		return getAccess(location);
		break;
	}
}