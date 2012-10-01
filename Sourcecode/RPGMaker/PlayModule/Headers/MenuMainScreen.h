#ifndef MENUMAINSCREEN_H
#define MENUMAINSCREEN_H

#include "qtimer.h"
#include "qevent.h"
#include "qobject.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "Party.h"
#include "BattleCharacterItem.h"
#include "MenuMainCharacterItem.h"

namespace playmodule {

	class MenuMainScreen : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		enum State {OPTION_SELECTION, CHARACTER_SELECTION};

		MenuMainScreen(QGraphicsItem *parent);
		virtual ~MenuMainScreen();

		void initialize(Party *party);
		void finalize();

		virtual void keyPressEvent(QKeyEvent *event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		
	private:
		void createPanels();
		void createCharacters();
		void createOptions();
		void createTextfield();

		void nextOption();
		void previousOption();
		void nextCharacter();
		void previousCharacter();
		void optionSelected();
		void characterSelected();
		void clearCharacterSelection();

		void setCurrentOption(QGraphicsSimpleTextItem *option);
		void setCurrentCharacter(MenuMainCharacterItem *character);

		// State
		State _state;

		// Panels
		QGraphicsPixmapItem *_partyPanel;
		QGraphicsPixmapItem *_optionsPanel;
		QGraphicsPixmapItem *_textPanel;
		QGraphicsTextItem *_textField;

		// Options
		QGraphicsSimpleTextItem *_currentOption;
		QVector<QGraphicsSimpleTextItem*> _options;

		// Characters
		int _currentCharacter;
		QVector<BattleCharacterItem*> _characters;

		MenuMainCharacterItem *_currentCharacterPanel;
		QVector<MenuMainCharacterItem*> _characterPanels;
		QVector<MenuMainCharacterItem*> _activeCharacterPanels;

		// Animation timer
		QTimer *_timer;

	private slots:
		void timeout();

	signals:
		void closeMainMenu();

		void viewCharacterProfile(BattleCharacterItem *character);
		void viewCharacterEquipment(BattleCharacterItem *character);
		void viewCharacterAbilities(BattleCharacterItem *character);
		void viewInventory();
	};
}

#endif