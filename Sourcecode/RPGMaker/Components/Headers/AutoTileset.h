#ifndef AUTOTILESET_H
#define AUTOTILESET_H

#include "qvector.h"

#include "Tile.h"
#include "Tileset.h"
#include "ProjectException.h"

using projectcomponents::Tile;
using projectcomponents::Tileset;
using projectcomponents::ProjectException;

namespace projectcomponents {

	/**
	*	An AutoTileset contains a dynamic tileset image.
	*	It is mainly used to make fluid patters such as roads and rivers.
	*	AutoTileset is a subclass of <i>Tileset</i>.
	*/
	class AutoTileset : public Tileset {
	public:
		virtual ~AutoTileset();

		QPixmap getTile(QPoint location, QVector<Tile*> surroundingTiles) const throw (ProjectException);
		QVector<QPixmap> getTiles() const;

		static AutoTileset* create(QString name, QString imagename) throw (ProjectException);
		static AutoTileset* translateFromXML(QDomElement &tileset) throw (ProjectException);
		static void translateToXML(AutoTileset* tileset, QDomDocument &document, QDomElement &parent);

	private:
		AutoTileset(QString name, QString imagename);

		bool isAutoTile(Tile *tile) const;
	};

}

#endif