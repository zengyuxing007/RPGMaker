#ifndef STATICTILE_H 
#define STATICTILE_H

#include "Tile.h"

using projectcomponents::Tile;

namespace projectcomponents {

	/**
	*	A StaticTile contains the static building block for environments.
	*	A StaticTile does not take its surrounding tiles into account and remains fixed in form.
	*	StaticTile is a subclass of <i>Tile<i/>.
	*/
	class StaticTile : public Tile{
	public:
		virtual ~StaticTile();

		QPixmap getImage() const;

		static StaticTile* create(Tileset *tileset, QPoint location) throw (ProjectException);

	private:
		StaticTile(Tileset *tileset, QPoint location);
	};

}

#endif