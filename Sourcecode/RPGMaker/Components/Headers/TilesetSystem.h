#ifndef TILESETSYSTEM_H
#define TILESETSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Tileset.h"

namespace projectcomponents {

	/**
	*	The TilesetSystem class is a management system for Tilesets.
	*/
	class TilesetSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~TilesetSystem();

		const QVector<Tileset*>& getTilesets() const;
		Tileset* getTileset(QString name) const throw (ProjectException);

		void addTileset(Tileset *tileset) throw (ProjectException);

		void removeTileset(QString name) throw (ProjectException);

		static TilesetSystem* create();
		static TilesetSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(TilesetSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		TilesetSystem();

		QVector<Tileset*> _tilesets;
	};
}

#endif