#ifndef MENUMAINCHARACTERITEM_H
#define MENUMAINCHARACTERITEM_H

#include "qtimer.h"
#include "qgraphicsitem.h"

#include "BattleCharacterItem.h"

namespace playmodule {

	class MenuMainCharacterItem : public QGraphicsPixmapItem {
	public:
		MenuMainCharacterItem(QGraphicsItem *parent);
		virtual ~MenuMainCharacterItem();

		void initialize(BattleCharacterItem *character);
		void finalize();

		void markCharacter();
		void unmarkCharacter();
		void nextCharacterFrame();

		BattleCharacterItem* getCharacter() const;

	private:

		void drawBorder();
		
		int _currentFrame;
		QPixmap _characterFrames;
		BattleCharacterItem *_character;
		QGraphicsPixmapItem *_characterModel;
		QGraphicsPixmapItem *_selectionBorder;

		QGraphicsSimpleTextItem *_nameItem;
		QGraphicsSimpleTextItem *_levelItem;
		QGraphicsSimpleTextItem *_healthItem;
		QGraphicsSimpleTextItem *_energyItem;
	};
}

#endif