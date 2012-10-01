#ifndef BATTLECHARACTERSTATUS_H
#define BATTLECHARACTERSTATUS_H

#include "qobject.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "BattleCharacterItem.h"

namespace playmodule {

	class BattleCharacterStatus : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		BattleCharacterStatus(QGraphicsItem *parent);
		virtual ~BattleCharacterStatus();

		void setCharacters(const QVector<BattleCharacterItem*> &characters);

	private:
		void createWindow();

		QVector<BattleCharacterItem*> _characters;
		QVector<QGraphicsTextItem*> _info;

	private slots:
		void valueChanged(QString name);
	};
}

#endif