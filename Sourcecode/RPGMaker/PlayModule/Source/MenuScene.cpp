#include "../Headers/MenuScene.h"

using namespace playmodule;

MenuScene::MenuScene(QObject *parent, SoundItem *sound)
	: QGraphicsScene(parent), _sound(sound), _state(MenuScene::MAIN_MENU), _party(0) {}

MenuScene::~MenuScene() {}

void MenuScene::initialize(Party *party) {
	_state = MenuScene::MAIN_MENU;
	_party = party;

	_mainMenu = new MenuMainScreen(0);
	_mainMenu->initialize(party);
	_mainMenu->show();
	addItem(_mainMenu);
	connect(_mainMenu, SIGNAL(closeMainMenu()), this, SLOT(closeMainMenu()));
	connect(_mainMenu, SIGNAL(viewCharacterProfile(BattleCharacterItem*)), this, SLOT(viewCharacterProfile(BattleCharacterItem*)));
	connect(_mainMenu, SIGNAL(viewCharacterEquipment(BattleCharacterItem*)), this, SLOT(viewCharacterEquipment(BattleCharacterItem*)));
	connect(_mainMenu, SIGNAL(viewCharacterAbilities(BattleCharacterItem*)), this, SLOT(viewCharacterAbilities(BattleCharacterItem*)));
	connect(_mainMenu, SIGNAL(viewInventory()), this, SLOT(viewInventory()));

	_characterProfile = new MenuProfileScreen(0);
	_characterProfile->hide();
	addItem(_characterProfile);
	connect(_characterProfile, SIGNAL(closeCharacterProfile()), this, SLOT(closeCharacterProfile()));

	_equipmentScreen = new MenuEquipmentScreen(0);
	_equipmentScreen->hide();
	addItem(_equipmentScreen);
	connect(_equipmentScreen, SIGNAL(closeCharacterEquipment()), this, SLOT(closeCharacterEquipment()));

	_abilitiesScreen = new MenuAbilitiesScreen(0);
	_abilitiesScreen->hide();
	addItem(_abilitiesScreen);
	connect(_abilitiesScreen, SIGNAL(closeAbilitiesScreen()), this, SLOT(closeCharacterAbilities()));

	_inventoryScreen = new MenuInventoryScreen(0);
	_inventoryScreen->hide();
	addItem(_inventoryScreen);
	connect(_inventoryScreen, SIGNAL(closeInventory()), this, SLOT(closeInventory()));
}

void MenuScene::finalize() {
	delete _mainMenu;
	_mainMenu = 0;

	delete _characterProfile;
	_characterProfile = 0;

	delete _equipmentScreen;
	_equipmentScreen = 0;

	delete _inventoryScreen;
	_inventoryScreen = 0;

	_party = 0;

	_state = MenuScene::MAIN_MENU;
}

void MenuScene::keyPressEvent(QKeyEvent *event) {

	if (_state == MenuScene::MAIN_MENU)
		_mainMenu->keyPressEvent(event);
	else if (_state == MenuScene::CHARACTER_PROFILE)
		_characterProfile->keyPressEvent(event);
	else if (_state == MenuScene::CHARACTER_EQUIPMENT)
		_equipmentScreen->keyPressEvent(event);
	else if (_state == MenuScene::CHARACTER_ABILITIES)
		_abilitiesScreen->keyPressEvent(event);
	else if (_state == MenuScene::INVENTORY)
		_inventoryScreen->keyPressEvent(event);
}

void MenuScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MenuScene::closeMainMenu() {
	_mainMenu->finalize();
	emit closeMenu();
}

void MenuScene::viewCharacterProfile(BattleCharacterItem *character) {
	_mainMenu->hide();

	_characterProfile->finalize();
	_characterProfile->initialize(character);
	_characterProfile->show();

	_state = MenuScene::CHARACTER_PROFILE;
}

void MenuScene::closeCharacterProfile() {
	_characterProfile->hide();
	_characterProfile->finalize();

	_mainMenu->show();

	_state = MenuScene::MAIN_MENU;
}

void MenuScene::viewCharacterEquipment(BattleCharacterItem *character) {
	_mainMenu->hide();

	_equipmentScreen->finalize();
	_equipmentScreen->initialize(character, _party->getInventory());
	_equipmentScreen->show();

	_state = MenuScene::CHARACTER_EQUIPMENT;
}

void MenuScene::closeCharacterEquipment() {
	_equipmentScreen->hide();
	_equipmentScreen->finalize();

	_mainMenu->show();

	_state = MenuScene::MAIN_MENU;
}

void MenuScene::viewCharacterAbilities(BattleCharacterItem *character) {
	_mainMenu->hide();

	_abilitiesScreen->finalize();
	_abilitiesScreen->initialize(character);
	_abilitiesScreen->show();

	_state = MenuScene::CHARACTER_ABILITIES;
}

void MenuScene::closeCharacterAbilities() {
	_abilitiesScreen->hide();
	_abilitiesScreen->finalize();

	_mainMenu->show();

	_state = MenuScene::MAIN_MENU;
}

void MenuScene::viewInventory() {
	_mainMenu->hide();

	_inventoryScreen->finalize();
	_inventoryScreen->initialize(_party->getInventory());
	_inventoryScreen->show();

	_state = MenuScene::INVENTORY;
}

void MenuScene::closeInventory() {
	_inventoryScreen->hide();
	_inventoryScreen->finalize();

	_mainMenu->show();

	_state = MenuScene::MAIN_MENU;
}