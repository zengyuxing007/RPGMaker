#ifndef AUTOTILE_H
#define AUTOTILE_H

#include "qstring.h"
#include "qvector.h"

#include "Tile.h"
#include "ProjectException.h"

using projectcomponents::Tile;
using projectcomponents::ProjectException;

namespace projectcomponents {

	/**
	*	An AutoTile is dynamic tile.
	*	It keeps a list of its surrounding tiles and will dynamically change its form.
	*	An AutoTile usually represents a pattern such as a road or a river.
	*/
	class AutoTile : public Tile {
	public:
		virtual ~AutoTile();

		QPixmap getImage() const;
		QVector<Tile*> getSurroundingtiles() const;

		void setSurroundingTile(QPoint location, Tile *tile) throw (ProjectException);
		void setSurroundingTiles(QVector<Tile*> tiles) throw (ProjectException);

		static AutoTile* create(Tileset *tileset, QPoint location) throw (ProjectException);
	private:
		AutoTile(Tileset *tileset, QPoint location);

		QVector<Tile*> _surroundingTiles;
	};
}

#endif