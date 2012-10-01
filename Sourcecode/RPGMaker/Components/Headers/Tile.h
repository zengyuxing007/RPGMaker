#ifndef TILE_H
#define TILE_H

#include "qpoint.h"
#include "qpixmap.h"

#include "Tileset.h"
#include "ProjectException.h"

using projectcomponents::Tileset;
using projectcomponents::ProjectException;

namespace projectcomponents {

	/**
	*	A tile is the building block of an environment.
	*	It is linked to its tileset and knows its current location in that tileset.
	*	This class is a base class and cannot be instantiated.
	*	Look for the subclasses <i>StaticTile</i> and <i>AutoTile</i>.
	*/
	class Tile {
	public:
		enum Type {STATIC, AUTO};

		Tile(Type type, Tileset *tileset, QPoint location);
		virtual ~Tile();

		Type getType() const;
		Tileset* getTileset() const;
		QPoint getLocation() const;
		Tileset::Access getAccess() const;

		virtual QPixmap getImage() const throw (ProjectException) =0;

		void setAccess(Tileset::Access access);

	protected:
		Type _type;
		QPoint _location;
		Tileset *_tileset;

	};

}

#endif