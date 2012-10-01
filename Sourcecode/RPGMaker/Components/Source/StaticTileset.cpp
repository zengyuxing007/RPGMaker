#include "qfileinfo.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/StaticTileset.h"

using namespace projectcomponents;
/**
*	Construct a static tilset with name <i>name</i> and load the tileset image with name <i>imagename</i>.
*/
StaticTileset::StaticTileset(QString name, QString imagename)
	: Tileset(name, Tileset::STATIC, imagename) {
		_image.load(g_staticTilesetPath + imagename);
		_dimensions = QSize(_image.width()/g_tilesize, _image.height()/g_tilesize);
		_access = QVector<Tileset::Access>(_dimensions.width() * _dimensions.height(), Tileset::NONE);
}

/**
*	Standard destructor
*/
StaticTileset::~StaticTileset() {}

/**
*	Get the tile image at location <i>location</i>.
*	The location has to be valid, a <i>ProjectException</i> is throw otherwise.
*/
QPixmap StaticTileset::getTile(QPoint location) const {
	if ((location.y() < 0) || (location.y() >= _dimensions.height()))
		throw ProjectException("The row index " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() >= _dimensions.width()))
		throw ProjectException("The column index " + QString::number(location.x()) + " does not exist.");
	
	return _image.copy(location.x()*g_tilesize, location.y()*g_tilesize, g_tilesize, g_tilesize);
}

/**
*	Get a list of all the tile images.
*/
QVector<QPixmap> StaticTileset::getTiles() const {
	QVector<QPixmap> tiles(0);

	for (int y = 0; y < _dimensions.height(); y++) {
		for (int x = 0; x < _dimensions.width(); x++) {
			tiles.append(_image.copy(g_tilesize*x, g_tilesize*y, g_tilesize, g_tilesize));
		}
	}

	return tiles;
}

/**
*	Create a static tileset with name <i>name</i> and load the tileset image with name <i>imagename</i> and return a pointer to this instance.
*/
StaticTileset* StaticTileset::create(QString name, QString imagename) {
	if (name == QString())
		throw ProjectException("A tileset should not have an empty name.");

	QFileInfo imageinfo(g_staticTilesetPath + imagename);
	if (!imageinfo.exists())
		throw ProjectException("The image " + imagename + " does not exist in the path " + g_staticTilesetPath + ".");

	StaticTileset *tileset = new StaticTileset(name, imagename);
	return tileset;
}

/**
*	Translate the static tileset from XML and return a pointer to this instance.
*/
StaticTileset* StaticTileset::translateFromXML(QDomElement &tilesetElement) {
	QString name = tilesetElement.attributeNode("name").value();
	QString imagename = tilesetElement.firstChildElement("image").attributeNode("name").value();
	StaticTileset *tileset = StaticTileset::create(name, imagename);

	QDomNodeList tiles = tilesetElement.firstChildElement("tiles").childNodes();
	for (int i = 0; i < tiles.size(); i++) {
		QDomElement tileElement = tiles.at(i).toElement();
		int row = tileElement.attributeNode("row").value().toInt();
		int column = tileElement.attributeNode("column").value().toInt();
		QString accessString = tileElement.attributeNode("access").value();
		if (accessString == "OVER")
			tileset->setAccess(QPoint(column, row), Tileset::OVER);
		else if (accessString == "UNDER")
			tileset->setAccess(QPoint(column, row), Tileset::UNDER);
		else
			tileset->setAccess(QPoint(column, row), Tileset::NONE);
	}

	return tileset;
}

/**
*	Translate a static tileset to XML.
*/
void StaticTileset::translateToXML(StaticTileset *tileset, QDomDocument &document, QDomElement &parent) {
	QDomElement tilesetElement = document.createElement("tileset");
	tilesetElement.setAttribute("name", tileset->getName());

	QDomElement typeElement = document.createElement("type");
	typeElement.setAttribute("value", "STATIC");
	tilesetElement.appendChild(typeElement);

	QDomElement imageElement = document.createElement("image");
	imageElement.setAttribute("name", tileset->getImagename());
	tilesetElement.appendChild(imageElement);

	QDomElement tilesElement = document.createElement("tiles");
	QVector<Tileset::Access> access = tileset->getAccess();
	for (int y = 0; y < tileset->getDimensions().height(); y++) {
		for (int x = 0; x < tileset->getDimensions().width(); x++) {
			QDomElement tileElement = document.createElement("tile");
			tileElement.setAttribute("row", y);
			tileElement.setAttribute("column", x);
			switch (tileset->getAccess(QPoint(x, y))) {
			case Tileset::OVER:
				tileElement.setAttribute("access", "OVER");
				break;
			case Tileset::UNDER:
				tileElement.setAttribute("access", "UNDER");
				break;
			case Tileset::NONE:
				tileElement.setAttribute("access", "NONE");
				break;
			}
			tilesElement.appendChild(tileElement);
		}
	}
	tilesetElement.appendChild(tilesElement);

	parent.appendChild(tilesetElement);
}