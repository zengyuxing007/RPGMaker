#ifndef MENUCHARACTERPROFILE_H
#define MENUCHARACTERPROFILE_H

#include "qtimer.h"
#include "qevent.h"
#include "qobject.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "BattleCharacterItem.h"

namespace playmodule {

	class MenuProfileScreen : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:

		MenuProfileScreen(QGraphicsItem *parent);
		virtual ~MenuProfileScreen();

		void initialize(BattleCharacterItem *character);
		void finalize();

		BattleCharacterItem* getCharacter() const;

		void keyPressEvent(QKeyEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:

		void createWindow();
		void createImagePanel();
		void createProfilePanel();
		void createGeneralAttributesPanel();
		void createOffensiveAttributesPanel();
		void createDefensiveAttributesPanel();
		
		QTimer *_timer;
		BattleCharacterItem *_character;

		// Images
		QGraphicsPixmapItem *_imagesPanel;
		int _currentFrame;
		QPixmap _environmentalFrames;
		QGraphicsPixmapItem *_battleModel;
		QGraphicsPixmapItem *_environmentalModel;

		// General info
		QGraphicsPixmapItem *_generalInfoPanel;
		QGraphicsSimpleTextItem *_nameItem;
		QGraphicsSimpleTextItem *_levelItem;
		QGraphicsSimpleTextItem *_experienceItem;
		QGraphicsSimpleTextItem *_classItem;
		QGraphicsTextItem *_biographyItem;

		// General attributes
		QGraphicsPixmapItem *_generalAttributesPanel;
		QGraphicsSimpleTextItem *_generalItem;
		QGraphicsSimpleTextItem *_healthItem;
		QGraphicsSimpleTextItem *_energyItem;
		QGraphicsSimpleTextItem *_agilityItem;
		QGraphicsSimpleTextItem *_evasionItem;

		// Offensive attributes
		QGraphicsPixmapItem *_offensiveAttributesPanel;
		QGraphicsSimpleTextItem *_offensiveItem;
		QVector<QGraphicsSimpleTextItem*> _offensiveItems;

		// Defensive attributes
		QGraphicsPixmapItem *_defensiveAttributesPanel;
		QGraphicsSimpleTextItem *_defensiveItem;
		QVector<QGraphicsSimpleTextItem*> _defensiveItems;

	private slots:
		void timeout();

	signals:
		void closeCharacterProfile();
	};
}

#endif