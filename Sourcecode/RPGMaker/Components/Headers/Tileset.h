#ifndef TILESET_H
#define TILESET_H

#include "qdom.h"
#include "qsize.h"
#include "qpoint.h"
#include "qobject.h"
#include "qstring.h"
#include "qvector.h"
#include "qpixmap.h"

#include "ProjectException.h"

using projectcomponents::ProjectException;

namespace projectcomponents {

	/**
	*	A Tileset contains the building blocks for environments.
	*	It holds the image and the accesslevels for each tile.
	*	This class is a base class and cannot be instantiated.
	*	Look for the subclasses <i>StaticTileset</i> or <i>AutoTileset</i>.
	*/
	class Tileset : public QObject {

		Q_OBJECT

	public:
		enum Type {STATIC, AUTO};
		enum Access {OVER, UNDER, NONE};

		Tileset(QString name, Type type, QString imagename);
		virtual ~Tileset();

		Type getType() const;
		QString getName() const;
		QString getImagename() const;
		QSize getDimensions() const;
		QPixmap getImage() const;
		QVector<Access> getAccess() const;
		Access getAccess(QPoint location) const throw (ProjectException);

		virtual QVector<QPixmap> getTiles() const =0;

		void setName(QString name);
		void setAccess(QPoint location, Access access) throw (ProjectException);
		Access scrollAccess(QPoint location) throw (ProjectException);

	protected:
		Type _type;
		QString _name;
		QString _imagename;
		QSize _dimensions;
		QPixmap _image;
		QVector<Access> _access;

	signals:
		void deleteSignal(QString name);
	};

}

#endif