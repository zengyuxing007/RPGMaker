#ifndef MENUEQUIPMENTSCREEN_H
#define MENUEQUIPMENTSCREEN_H

#include "qevent.h"
#include "qobject.h"
#include "qvector.h"
#include "qstringlist.h"
#include "qgraphicsitem.h"

#include "Party.h"
#include "Inventory.h"
#include "BattleCharacterItem.h"

namespace playmodule {

	class MenuEquipmentScreen : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		enum State {EQUIPPED_SELECTION, EQUIPMENT_SELECTION};

		MenuEquipmentScreen(QGraphicsItem *parent);
		virtual ~MenuEquipmentScreen();

		void initialize(BattleCharacterItem *character, Inventory *inventory);
		void finalize();

		BattleCharacterItem *getCharacter() const;

		virtual void keyPressEvent(QKeyEvent *event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:

		void createWindow();
		void createImagePanel();
		void createLabels();

		void nextEquippedItem();
		void previousEquippedItem();
		void equippedItemSelected();

		void nextEquipmentItem();
		void previousEquipmentItem();
		void equipmentItemSelected();

		void setCurrentEquippedItem(QGraphicsSimpleTextItem *item, EquipmentPiece *equipment);
		void setCurrentEquipmentItem(QGraphicsSimpleTextItem *item, EquipmentPiece *equipment);
		
		bool equipmentFilter(EquipmentPiece *equipment);

		State _state;
		Inventory *_inventory;

		// Current character
		BattleCharacterItem *_character;
		QGraphicsPixmapItem *_characterImage;
		
		// Current equipment
		QGraphicsPixmapItem *_equippedEquipmentPanel;
		EquipmentPiece *_currentEquippedEquipmentPiece;
		QGraphicsSimpleTextItem *_equippedEquipmentItem;
		QGraphicsSimpleTextItem *_currentEquippedEquipmentItem;
		QVector<QGraphicsSimpleTextItem*> _equippedEquipmentItems;

		// Currently selected equipment stats
		QGraphicsPixmapItem *_equipmentStatsPanel;
		QGraphicsSimpleTextItem *_equippedStatsItem;
		QVector<QGraphicsSimpleTextItem*> _equippedStatsItems;

		// Updated equipment stats
		QGraphicsPixmapItem *_updatedStatsPanel;
		QGraphicsSimpleTextItem *_updatedStatsItem;
		QVector<QGraphicsSimpleTextItem*> _updatedStatsItems;

		// Equipment list
		QGraphicsPixmapItem *_equipmentListPanel;
		QStringList _equipmentStringList;
		int _currentSelectedEquipmentString;
		QVector<EquipmentPiece*> _equipmentList;
		QGraphicsSimpleTextItem *_equipmentListItem;
		QGraphicsSimpleTextItem *_currentEquipmentListItem;
		QVector<QGraphicsSimpleTextItem*> _activeEquipmentListItems;

	signals:
		void closeCharacterEquipment();
	};
}

#endif