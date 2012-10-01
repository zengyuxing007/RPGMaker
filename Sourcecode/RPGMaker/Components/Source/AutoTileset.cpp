#include "qrect.h"
#include "qpainter.h"

#include "qfileinfo.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/AutoTileset.h"

using namespace projectcomponents;

const int autoTilesetWidth = 3;
const int autoTilesetHeight = 1;

AutoTileset::AutoTileset(QString name, QString imagename) 
	: Tileset(name, Tileset::AUTO, imagename) {
		_image.load(g_autoTilesetPath + imagename);
		_dimensions = QSize(autoTilesetWidth, autoTilesetHeight);
		_access = QVector<Tileset::Access>(autoTilesetWidth * autoTilesetHeight, Tileset::NONE);
}

AutoTileset::~AutoTileset() {}

QPixmap AutoTileset::getTile(QPoint location, QVector<Tile*> surroundingTiles) const {
	if (location.y() != 0)
		throw ProjectException("The row index " + QString::number(location.y()) + " does not exist.");

	if ((location.x() < 0) || (location.x() > 2))
		throw ProjectException("The column index " + QString::number(location.x()) + " does not exist.");

	if (!!surroundingTiles.at(4))
		throw ProjectException("The tile at index 4 should be 0.");

	switch (location.x()) {
	case 0:
		{
			QPixmap composedTile(g_tilesize, g_tilesize);
			QPainter painter(&composedTile);
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.fillRect(composedTile.rect(), Qt::transparent);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			painter.drawPixmap(composedTile.rect(), _image, QRect(g_tilesize, g_tilesize*2, g_tilesize, g_tilesize));

			Tile *NWTile = surroundingTiles.at(0);
			Tile *northTile = surroundingTiles.at(1);
			Tile *NETile = surroundingTiles.at(2);
			Tile *westTile = surroundingTiles.at(3);
			Tile *eastTile = surroundingTiles.at(5);
			Tile *SWTile = surroundingTiles.at(6);
			Tile *southTile = surroundingTiles.at(7);
			Tile *SETile = surroundingTiles.at(8);

			// Corners
			 if (isAutoTile(northTile) && isAutoTile(westTile) && !isAutoTile(NWTile))
				painter.drawPixmap(QRect(0, 0, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2, 0, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(northTile) && !isAutoTile(westTile))
				painter.drawPixmap(QRect(0, 0, g_tilesize/2, g_tilesize/2), _image, QRect(0, 0, g_tilesize/2, g_tilesize/2));

			 if (isAutoTile(northTile) && isAutoTile(eastTile) && !isAutoTile(NETile))
				painter.drawPixmap(QRect(g_tilesize/2, 0, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2.5, 0, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(northTile) && !isAutoTile(eastTile))
				painter.drawPixmap(QRect(g_tilesize/2, 0, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize/2, 0, g_tilesize/2, g_tilesize/2));

			 if (isAutoTile(southTile) && isAutoTile(westTile) && !isAutoTile(SWTile))
				painter.drawPixmap(QRect(0, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2, g_tilesize/2, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(southTile) && !isAutoTile(westTile))
				painter.drawPixmap(QRect(0, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(0, g_tilesize/2, g_tilesize/2, g_tilesize/2));

			 if (isAutoTile(southTile) && isAutoTile(eastTile) && !isAutoTile(SETile))
				painter.drawPixmap(QRect(g_tilesize/2, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2.5, g_tilesize/2, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(southTile) && !isAutoTile(eastTile))
				painter.drawPixmap(QRect(g_tilesize/2, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize/2, g_tilesize/2, g_tilesize/2, g_tilesize/2));

			 // Edges
			 if (!isAutoTile(northTile) && isAutoTile(westTile) && isAutoTile(eastTile))
				painter.drawPixmap(QRect(0, 0, g_tilesize, g_tilesize/2), _image, QRect(g_tilesize, g_tilesize, g_tilesize, g_tilesize/2));
			 else if (!isAutoTile(northTile) && !isAutoTile(westTile) && isAutoTile(eastTile))
				painter.drawPixmap(QRect(g_tilesize/2, 0, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize/2, g_tilesize, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(northTile) && !isAutoTile(eastTile) && isAutoTile(westTile))
				painter.drawPixmap(QRect(0, 0, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2, g_tilesize, g_tilesize/2, g_tilesize/2));

			 if (!isAutoTile(southTile) && isAutoTile(westTile) && isAutoTile(eastTile))
				painter.drawPixmap(QRect(0, g_tilesize/2, g_tilesize, g_tilesize/2), _image, QRect(g_tilesize, g_tilesize*3.5, g_tilesize, g_tilesize/2));
			 else if (!isAutoTile(southTile) && !isAutoTile(eastTile) && isAutoTile(westTile))
				painter.drawPixmap(QRect(0, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2, g_tilesize*3.5, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(southTile) && !isAutoTile(westTile) && isAutoTile(eastTile))
				painter.drawPixmap(QRect(g_tilesize/2, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize/2, g_tilesize*3.5, g_tilesize/2, g_tilesize/2));

			 if (!isAutoTile(westTile) && isAutoTile(northTile) && isAutoTile(southTile))
				 painter.drawPixmap(QRect(0, 0, g_tilesize/2, g_tilesize), _image, QRect(0, g_tilesize*2, g_tilesize/2, g_tilesize));
			 else if (!isAutoTile(westTile) && !isAutoTile(southTile) && isAutoTile(northTile))
				 painter.drawPixmap(QRect(0, 0, g_tilesize/2, g_tilesize/2), _image, QRect(0, g_tilesize*3, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(westTile) && !isAutoTile(northTile) && isAutoTile(southTile))
				 painter.drawPixmap(QRect(0, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(0, g_tilesize*1.5, g_tilesize/2, g_tilesize/2));

			 if (!isAutoTile(eastTile) && isAutoTile(northTile) && isAutoTile(southTile))
				 painter.drawPixmap(QRect(g_tilesize/2, 0, g_tilesize/2, g_tilesize), _image, QRect(g_tilesize*2.5, g_tilesize*2, g_tilesize/2, g_tilesize));
			 else if (!isAutoTile(eastTile) && !isAutoTile(northTile) && isAutoTile(southTile))
				 painter.drawPixmap(QRect(g_tilesize/2, g_tilesize/2, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2.5, g_tilesize*1.5, g_tilesize/2, g_tilesize/2));
			 else if (!isAutoTile(eastTile) && !isAutoTile(southTile) && isAutoTile(northTile))
				 painter.drawPixmap(QRect(g_tilesize/2, 0, g_tilesize/2, g_tilesize/2), _image, QRect(g_tilesize*2.5, g_tilesize*3, g_tilesize/2, g_tilesize/2));

			painter.end();
			return composedTile;
		}

		break;
	case 1:
		return _image.copy(g_tilesize, g_tilesize*2, g_tilesize, g_tilesize);
		break;
	case 2:
		return _image.copy(g_tilesize, 0, g_tilesize, g_tilesize);
		break;
	}

	return QPixmap();
}

QVector<QPixmap> AutoTileset::getTiles() const {
	QVector<QPixmap> tiles(0);
	tiles.append(_image.copy(0, 0, g_tilesize, g_tilesize));
	tiles.append(_image.copy(g_tilesize, g_tilesize*2, g_tilesize, g_tilesize));
	tiles.append(_image.copy(g_tilesize, 0, g_tilesize, g_tilesize));

	return tiles;
}

bool AutoTileset::isAutoTile(Tile *tile) const {
	if (!!tile) {
		QPoint location = tile->getLocation();
		if ((tile->getTileset()->getName() == _name) && (location.y() == 0) && ((location.x() == 0) || (location.x() == 1)))
			return true;
		else
			return false;
	}

	return false;
}

AutoTileset* AutoTileset::create(QString name, QString imagename) {
	if (name == QString())
		throw ProjectException("A tileset should not have an empty name.");

	QFileInfo imageinfo(g_autoTilesetPath + imagename);
	if (!imageinfo.exists())
		throw ProjectException("The image " + imagename + " does not exist in the path " + g_autoTilesetPath + ".");

	AutoTileset *tileset = new AutoTileset(name, imagename);
	return tileset;
}

AutoTileset* AutoTileset::translateFromXML(QDomElement &tilesetElement) {
	QString name = tilesetElement.attributeNode("name").value();
	QString imagename = tilesetElement.firstChildElement("image").attributeNode("name").value();
	AutoTileset *tileset = AutoTileset::create(name, imagename);

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

void AutoTileset::translateToXML(AutoTileset* tileset, QDomDocument &document, QDomElement &parent) {
	QDomElement tilesetElement = document.createElement("tileset");
	tilesetElement.setAttribute("name", tileset->getName());
	
	QDomElement typeElement = document.createElement("type");
	typeElement.setAttribute("value", "AUTO");
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