#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "qvector.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qgraphicssceneevent.h"

#include "Party.h"
#include "SoundItem.h"
#include "MenuMainScreen.h"
#include "MenuProfileScreen.h"
#include "MenuAbilitiesScreen.h"
#include "MenuInventoryScreen.h"
#include "MenuEquipmentScreen.h"

namespace playmodule {

	class MenuScene : public QGraphicsScene {

		Q_OBJECT

	public:
		enum State {MAIN_MENU, CHARACTER_PROFILE, CHARACTER_EQUIPMENT, CHARACTER_ABILITIES, INVENTORY};

		MenuScene(QObject *parent, SoundItem *sound);
		virtual ~MenuScene();
		void initialize(Party *party);
		void finalize();

	protected:
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:
		State _state;
		Party *_party;
		SoundItem *_sound;
		MenuMainScreen *_mainMenu;
		MenuProfileScreen *_characterProfile;
		MenuEquipmentScreen *_equipmentScreen;
		MenuAbilitiesScreen *_abilitiesScreen;
		MenuInventoryScreen *_inventoryScreen;

	private slots:
		void closeMainMenu();

		void viewCharacterProfile(BattleCharacterItem *character);
		void closeCharacterProfile();

		void viewCharacterEquipment(BattleCharacterItem *character);
		void closeCharacterEquipment();

		void viewCharacterAbilities(BattleCharacterItem *character);
		void closeCharacterAbilities();

		void viewInventory();
		void closeInventory();

	signals:
		void closeMenu();
	};
}

#endif