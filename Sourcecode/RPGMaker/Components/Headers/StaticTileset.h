#ifndef STATICTILESET_H
#define STATICTILESET_H

#include "qvector.h"

#include "Tileset.h"
#include "ProjectException.h"

using projectcomponents::Tileset;
using projectcomponents::ProjectException;

namespace projectcomponents {

	/**
	*	A StaticTileset contains a static tileset image.
	*	It is mainly used to place large objects in environments.
	*	StaticTileset is a subclass of <i>Tileset</i>.
	*/
	class StaticTileset : public Tileset {
	public:
		virtual ~StaticTileset();

		QPixmap getTile(QPoint location) const throw (ProjectException);
		QVector<QPixmap> getTiles() const;

		static StaticTileset* create(QString name, QString imagename) throw (ProjectException);
		static StaticTileset* translateFromXML(QDomElement &tileset) throw (ProjectException);
		static void translateToXML(StaticTileset *tileset, QDomDocument &document, QDomElement &parent);

	private:
		StaticTileset(QString name, QString imagename);
	};

}
#endif