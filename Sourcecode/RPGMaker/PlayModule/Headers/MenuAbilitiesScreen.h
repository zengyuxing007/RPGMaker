#ifndef MENUABILITIESSCREEN_H
#define MENUABILITIESSCREEN_H

#include "qevent.h"
#include "qobject.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "BattleCharacterItem.h"

namespace playmodule {

	class MenuAbilitiesScreen : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		MenuAbilitiesScreen(QGraphicsItem *parent);
		virtual ~MenuAbilitiesScreen();

		void initialize(BattleCharacterItem *character);
		void finalize();

		void keyPressEvent(QKeyEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:

		void createWindow();
		void createLabels();

		void nextAbility();
		void previousAbility();
		void setCurrentAbility(QGraphicsSimpleTextItem *item);

		BattleCharacterItem *_character;

		// Abilities list
		QGraphicsPixmapItem *_abilitiesPanel;

		int _currentAbility;
		QGraphicsSimpleTextItem *_abilitiesLabel;
		QGraphicsSimpleTextItem *_currentAbilityLabel;
		QVector<QGraphicsSimpleTextItem*> _activeAbilityLabels;

		// Description panel
		QGraphicsPixmapItem *_descriptionPanel;

		QGraphicsSimpleTextItem *_nameLabel;
		QGraphicsSimpleTextItem *_costLabel;
		QGraphicsSimpleTextItem *_typeLabel;
		QGraphicsSimpleTextItem *_attributeLabel;
		QGraphicsSimpleTextItem *_descriptionLabel;
		QGraphicsTextItem *_description;

		// Elements panel
		QGraphicsPixmapItem *_elementPanel;
		
		QGraphicsSimpleTextItem *_elementLabel;
		QVector<QGraphicsSimpleTextItem*> _elementLabels;

	signals:
		void closeAbilitiesScreen();
	};
}

#endif