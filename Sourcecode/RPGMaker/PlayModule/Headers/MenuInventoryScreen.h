#ifndef MENUINVENTORYSCREEN_H
#define MENUINVENTORYSCREEN_H

#include "qevent.h"
#include "qobject.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "Inventory.h"
#include "../Components/Headers/EquipmentPiece.h"

namespace playmodule {

	class MenuInventoryScreen : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:

		MenuInventoryScreen(QGraphicsItem *parent);
		virtual ~MenuInventoryScreen();

		void initialize(Inventory *inventory);
		void finalize();

		void keyPressEvent(QKeyEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:

		void createWindow();
		void createLabels();

		void nextItem();
		void previousItem();
		void setCurrentItem(QGraphicsSimpleTextItem *item);

		Inventory *_inventory;

		// Item list
		QGraphicsPixmapItem *_itemsPanel;

		int _currentItem;
		QGraphicsSimpleTextItem *_itemsLabel;
		QGraphicsSimpleTextItem *_currentItemLabel;
		QVector<QGraphicsSimpleTextItem*> _activeItemLabels;

		// Description panel
		QGraphicsPixmapItem *_descriptionPanel;

		QGraphicsSimpleTextItem *_nameLabel;
		QGraphicsSimpleTextItem *_typeLabel;
		QGraphicsSimpleTextItem *_descriptionLabel;
		QGraphicsTextItem *_description;

		// Classes panel
		QGraphicsPixmapItem *_classPanel;

		QGraphicsSimpleTextItem *_classLabel;
		QVector<QGraphicsSimpleTextItem*> _classLabels;

		// Elements panel
		QGraphicsPixmapItem *_elementPanel;

		QGraphicsSimpleTextItem *_elementLabel;
		QVector<QGraphicsSimpleTextItem*> _elementLabels;

		// Attributes panel
		QGraphicsPixmapItem *_attributePanel;

		QGraphicsSimpleTextItem *_attributeLabel;
		QVector<QGraphicsSimpleTextItem*> _attributeLabels;
		
	signals:
		void closeInventory();
	};
}

#endif