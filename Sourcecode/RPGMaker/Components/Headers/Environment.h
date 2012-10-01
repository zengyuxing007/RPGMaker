#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "qdom.h"
#include "qpair.h"
#include "qsize.h"
#include "qpoint.h"
#include "qstring.h"
#include "qobject.h"
#include "qvector.h"

#include "Tile.h"
#include "Event.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The Environment class contains all the data of an environment.
	*	An environment contains several layers of tiles as well as objects and events.
	*/
	class Environment : public QObject {

		Q_OBJECT

	public:
		virtual ~Environment();

		QString getName() const;
		QSize getDimensions() const;
		QVector<Event*> getEvents() const;
		QString getBackgroundMusic() const;
		QString getBattlebackName() const;
		QPixmap getBattlebackImage() const;
		QVector<Tile*> getLayer(int layer) const throw (ProjectException);
		Event* getEvent(QPoint location) const throw (ProjectException);
		Tile* getTile(int layer, QPoint location) const throw (ProjectException);
		Tileset::Access getAccess(QPoint location) const throw (ProjectException);

		void setName(QString name) throw (ProjectException);
		void setDimensions(QSize dimensions) throw (ProjectException);
		void setEvent(Event *event, QPoint location) throw (ProjectException);
		void setBackgroundMusic(QString name) throw (ProjectException);
		void setBattleback(QString name) throw (ProjectException);
		QVector<QPoint> setTile(Tile *tile, int layer, QPoint location) throw (ProjectException);
		QVector<QPoint> floodLayer(Tile *tile, int layer, QPoint location) throw (ProjectException);

		void removeEvent(QPoint location) throw (ProjectException);

		QVector<QPixmap> generateRender() const;
		QVector<QPixmap> generateFloor() const;
		QVector<QPixmap> generateCeiling() const;
		QVector<Tileset::Access> generateAccessGrid() const;

		static Environment* create(QString name, QSize dimensions) throw (ProjectException);
		static Environment* translateFromXML(QDomElement &environment) throw (ProjectException);
		static void translateToXML(Environment *environment, QDomDocument &document, QDomElement &parent);

	private:
		Environment(QString name, QSize dimensions);

		QString _name;
		QSize _dimensions;
		QString _backgroundMusic;
		QString _battlebackName;
		QPixmap _battlebackImage;
		QVector<Event*> _events;
		QVector<QVector<Tile*>> _layers;

	signals:
		void deleteSignal(QString name);
		void resizeSignal(QString name);

	private slots:
		void tilesetDeleted(QString name);
		void eventDeleted(QString ID);
	};
	
}

#endif