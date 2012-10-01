#include "qdir.h"
#include "qvector.h"
#include "qspinbox.h"
#include "qgroupbox.h"
#include "qstringlist.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qheaderview.h"
#include "qgridlayout.h"
#include "qinputdialog.h"

#include "../Headers/CharactersWindow.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Ability.h"
#include "../Components/Headers/Character.h"
#include "../Headers/CharacterConfigWindow.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;
using namespace projectcomponents;

CharactersWindow::CharactersWindow(QWidget *parent, Project *project)
	: _project(project) {

		createWindow();
		loadCharacters();

		if (_characterSelection->count() > 0)
			_characterSelection->setCurrentRow(0);
}

CharactersWindow::~CharactersWindow() {}

void CharactersWindow::createWindow() {
	QGridLayout *layout = new QGridLayout(this);

	// Character frame
	QGroupBox *characterFrame = new QGroupBox("Characters", this);
	QVBoxLayout *characterLayout = new QVBoxLayout(characterFrame);
	characterLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(characterFrame, 0, 0, 3, 1);

	// Biography frame
	QGroupBox *biographyFrame = new QGroupBox("Biography", this);
	QVBoxLayout *biographyLayout = new QVBoxLayout(biographyFrame);
	biographyLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(biographyFrame, 0, 1);

	// Elements frame
	QGroupBox *elementsFrame = new QGroupBox("Elements", this);
	QVBoxLayout *elementsLayout = new QVBoxLayout(elementsFrame);
	elementsLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(elementsFrame, 1, 2);

	// Abilities frame
	QGroupBox *abilitiesFrame = new QGroupBox("Abilities", this);
	QVBoxLayout *abilitiesLayout = new QVBoxLayout(abilitiesFrame);
	abilitiesLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(abilitiesFrame, 1, 1);

	// Equipment frame
	QGroupBox *equipmentFrame = new QGroupBox("Equipment", this);
	QVBoxLayout *equipmentLayout = new QVBoxLayout(equipmentFrame);
	equipmentLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(equipmentFrame, 0, 2);

	// Images frame
	QGroupBox *imagesFrame = new QGroupBox("Images", this);
	QHBoxLayout *imagesLayout = new QHBoxLayout(imagesFrame);
	imagesLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(imagesFrame, 2, 1, 1, 2);

	// Character field
	QLabel *label1 = new QLabel("Select a character:", characterFrame);
	characterLayout->addWidget(label1);

	_characterSelection = new QListWidget(characterFrame);
	characterLayout->addWidget(_characterSelection);

	QFrame *characterButtonFrame = new QFrame(characterFrame);
	characterLayout->addWidget(characterButtonFrame);
	QHBoxLayout *characterButtonLayout = new QHBoxLayout(characterButtonFrame);
	characterButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *characterNewButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", characterButtonFrame);
	characterNewButton->setToolTip(tr("Add a new character to the project"));
	characterNewButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *characterEditButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", characterButtonFrame);
	characterEditButton->setToolTip(tr("Edit the current character"));
	characterEditButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *characterDeleteButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", characterButtonFrame);
	characterDeleteButton->setToolTip(tr("Delete the current character"));
	characterDeleteButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *characterUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", characterButtonFrame);
	characterUpButton->setToolTip("Move selected character up");
	characterUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *characterDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", characterButtonFrame);
	characterDownButton->setToolTip("Move selected character down");
	characterDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	characterButtonLayout->addWidget(characterNewButton);
	characterButtonLayout->addWidget(characterEditButton);
	characterButtonLayout->addWidget(characterDeleteButton);
	characterButtonLayout->addSpacing(20);
	characterButtonLayout->addWidget(characterUpButton);
	characterButtonLayout->addWidget(characterDownButton);

	// Biography field
	_biographyLabel = new QLabel(biographyFrame);
	_biographyField = new QPlainTextEdit(biographyFrame);
	QFrame *biographyButtonFrame = new QFrame(biographyFrame);
	QHBoxLayout *biographyButtonLayout = new QHBoxLayout(biographyButtonFrame);
	biographyButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveBiographyButton = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", biographyButtonFrame);
	saveBiographyButton->setToolTip("Save the biography for the current character");
	saveBiographyButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadBiographyButton = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", biographyButtonFrame);
	reloadBiographyButton->setToolTip("Reload the biography of the current character");
	reloadBiographyButton->setMaximumSize(g_buttonsize, g_buttonsize);
	biographyButtonLayout->addWidget(saveBiographyButton);
	biographyButtonLayout->addWidget(reloadBiographyButton);
	biographyLayout->addWidget(_biographyLabel);
	biographyLayout->addWidget(_biographyField);
	biographyLayout->addWidget(biographyButtonFrame);

	// Elements field
	_elementsLabel = new QLabel(elementsFrame);
	_elementsList = new QListWidget(elementsFrame);
	QFrame *elementsButtonFrame = new QFrame(elementsFrame);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(elementsButtonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addElementButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", elementsButtonFrame);
	addElementButton->setToolTip("Add an element to the current character");
	addElementButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteElementButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", elementsButtonFrame);
	deleteElementButton->setToolTip("Delete the element from the current character");
	deleteElementButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", elementsButtonFrame);
	elementUpButton->setToolTip("Move selected element up");
	elementUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", elementsButtonFrame);
	elementDownButton->setToolTip("Move selected element down");
	elementDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	elementsButtonLayout->addWidget(addElementButton);
	elementsButtonLayout->addWidget(deleteElementButton);
	elementsButtonLayout->addSpacing(20);
	elementsButtonLayout->addWidget(elementUpButton);
	elementsButtonLayout->addWidget(elementDownButton);
	elementsLayout->addWidget(_elementsLabel);
	elementsLayout->addWidget(_elementsList);
	elementsLayout->addWidget(elementsButtonFrame);

	// Abilities field
	_abilitiesLabel = new QLabel(abilitiesFrame);
	_abilitiesList = new QListWidget(abilitiesFrame);
	QFrame *abilitiesButtonFrame = new QFrame(abilitiesFrame);
	QHBoxLayout *abilitiesButtonLayout = new QHBoxLayout(abilitiesButtonFrame);
	abilitiesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addAbilityButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", abilitiesButtonFrame);
	addAbilityButton->setToolTip("Add an ability to the current character");
	addAbilityButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAbilityButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", abilitiesButtonFrame);
	editAbilityButton->setToolTip("Edit the current ability for this character");
	editAbilityButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAbilityButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", abilitiesButtonFrame);
	deleteAbilityButton->setToolTip("Delete the ability from the current character");
	deleteAbilityButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", abilitiesButtonFrame);
	abilityUpButton->setToolTip("Move selected class up");
	abilityUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", abilitiesButtonFrame);
	abilityDownButton->setToolTip("Move selected class down");
	abilityDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	abilitiesButtonLayout->addWidget(addAbilityButton);
	abilitiesButtonLayout->addWidget(editAbilityButton);
	abilitiesButtonLayout->addWidget(deleteAbilityButton);
	abilitiesButtonLayout->addSpacing(20);
	abilitiesButtonLayout->addWidget(abilityUpButton);
	abilitiesButtonLayout->addWidget(abilityDownButton);
	abilitiesLayout->addWidget(_abilitiesLabel);
	abilitiesLayout->addWidget(_abilitiesList);
	abilitiesLayout->addWidget(abilitiesButtonFrame);

	// Equipment field
	_equipmentLabel = new QLabel(equipmentFrame);
	_equipmentList = new QTableWidget(equipmentFrame);
	_equipmentList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_equipmentList->setSelectionBehavior(QAbstractItemView::SelectItems);
	_equipmentList->setSelectionMode(QAbstractItemView::SingleSelection);
	_equipmentList->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_equipmentList->horizontalHeader()->hide();
	_equipmentList->setRowCount(10);
	_equipmentList->setColumnCount(1);
	QStringList headers;
	headers << "Weapon: Mainhand" << "Weapon: Offhand" << "Armor: Helmet" << "Armor: Torso" << "Armor: Leggings" << "Armor: Gloves" << "Armor: Boots" << "Accessory: Earring" << "Accessory: Necklace" << "Accessory: Ring";
	_equipmentList->setVerticalHeaderLabels(headers);
	QFrame *equipmentButtonFrame = new QFrame(equipmentFrame);
	QHBoxLayout *equipmentButtonLayout = new QHBoxLayout(equipmentButtonFrame);
	equipmentButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *editEquipmentButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", equipmentButtonFrame);
	editEquipmentButton->setToolTip("Edit the current equipmentslot for this character");
	editEquipmentButton->setMaximumSize(g_buttonsize, g_buttonsize);
	equipmentButtonLayout->addWidget(editEquipmentButton);
	QPushButton *deleteEquipmentButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", equipmentButtonFrame);
	deleteEquipmentButton->setToolTip("Unset the current equipmentslot for this character");
	deleteEquipmentButton->setMaximumSize(g_buttonsize, g_buttonsize);
	equipmentButtonLayout->addWidget(deleteEquipmentButton);
	equipmentLayout->addWidget(_equipmentLabel);
	equipmentLayout->addWidget(_equipmentList);
	equipmentLayout->addWidget(equipmentButtonFrame);

	// Images field - Environment image
	QFrame *envImageFrame = new QFrame(imagesFrame);
	QVBoxLayout *envImageLayout = new QVBoxLayout(envImageFrame);
	envImageLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	QLabel *label3 = new QLabel("Environmental image:", envImageFrame);
	_envImageLabel = new QLabel(envImageFrame);
	envImageLayout->addWidget(label3);
	envImageLayout->addWidget(_envImageLabel);
	imagesLayout->addWidget(envImageFrame);
	
	// Images field - Battle image
	QFrame *battleImageFrame = new QFrame(imagesFrame);
	QVBoxLayout *battleImageLayout = new QVBoxLayout(battleImageFrame);
	battleImageLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	QLabel *label4 = new QLabel("Battler image:", battleImageFrame);
	_battleImageLabel = new QLabel(battleImageFrame);
	battleImageLayout->addWidget(label4);
	battleImageLayout->addWidget(_battleImageLabel);
	imagesLayout->addWidget(battleImageFrame);

	connect(_characterSelection, SIGNAL(currentTextChanged(const QString &)), this, SLOT(characterSelected(const QString &)));
	connect(characterNewButton, SIGNAL(clicked()), this, SLOT(newCharacter()));
	connect(characterEditButton, SIGNAL(clicked()), this, SLOT(editCharacter()));
	connect(characterDeleteButton, SIGNAL(clicked()), this, SLOT(deleteCharacter()));
	connect(characterUpButton, SIGNAL(clicked()), this, SLOT(moveCharacterUp()));
	connect(characterDownButton, SIGNAL(clicked()), this, SLOT(moveCharacterDown()));
	connect(saveBiographyButton, SIGNAL(clicked()), this, SLOT(saveBiography()));
	connect(reloadBiographyButton, SIGNAL(clicked()), this, SLOT(reloadBiography()));
	connect(addElementButton, SIGNAL(clicked()), this, SLOT(addElement()));
	connect(deleteElementButton, SIGNAL(clicked()), this, SLOT(deleteElement()));
	connect(elementUpButton, SIGNAL(clicked()), this, SLOT(moveElementUp()));
	connect(elementDownButton, SIGNAL(clicked()), this, SLOT(moveElementDown()));
	connect(addAbilityButton, SIGNAL(clicked()), this, SLOT(addAbility()));
	connect(editAbilityButton, SIGNAL(clicked()), this, SLOT(editAbility()));
	connect(deleteAbilityButton, SIGNAL(clicked()), this, SLOT(deleteAbility()));
	connect(abilityUpButton, SIGNAL(clicked()), this, SLOT(moveAbilityUp()));
	connect(abilityDownButton, SIGNAL(clicked()), this, SLOT(moveAbilityDown()));
	connect(editEquipmentButton, SIGNAL(clicked()), this, SLOT(editEquipment()));
	connect(deleteEquipmentButton, SIGNAL(clicked()), this, SLOT(deleteEquipment()));
}

void CharactersWindow::loadCharacters() {
	_characterSelection->clear();
	QVector<Character*> characters = _project->getCharacters()->getCharacters();

	for (int i = 0; i < characters.size(); i++) {
		_characterSelection->addItem(characters.at(i)->getName());
	}
}

void CharactersWindow::characterSelected(const QString &name) {
	_biographyLabel->clear();
	_biographyField->clear();
	_elementsLabel->clear();
	_elementsList->clear();
	_abilitiesLabel->clear();
	_abilitiesList->clear();
	_equipmentLabel->clear();
	_equipmentList->clearContents();
	_envImageLabel->clear();
	_battleImageLabel->clear();

	if ((_characterSelection->count() > 0) && (name != QString())) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			// Biography
			_biographyLabel->setText("Biography of " + character->getName());
			_biographyField->setPlainText(character->getBiography());

			// Elements
			_elementsLabel->setText("Elements of " + character->getName());
			QVector<Element*> elements = character->getElements();
			for (int i = 0; i < elements.size(); i++) {
				_elementsList->addItem(elements.at(i)->getName());
			}

			// Abilities
			_abilitiesLabel->setText("Abilities of " + character->getName());
			QVector<QPair<Ability*, int>> abilities = character->getAbilities();
			for (int i = 0; i < abilities.size(); i++) {
				QString item;
				item += abilities.at(i).first->getName();
				item += " - level " + QString::number(abilities.at(i).second);
				_abilitiesList->addItem(item);
			}

			// Equipment
			_equipmentLabel->setText("Equipment of " + character->getName());
			QPair<Weapon*, Weapon*> weapons = character->getEquipment()->getWeapons();
			if (!!weapons.first)
				_equipmentList->setItem(0, 0, new QTableWidgetItem(weapons.first->getName()));
			if (!!weapons.second)
				_equipmentList->setItem(1, 0, new QTableWidgetItem(weapons.second->getName()));

			QVector<Armor*> armor = character->getEquipment()->getArmor();
			for (int i = 0; i < armor.size(); i++) {
				if (!!armor.at(i))
					_equipmentList->setItem(i + 2, 0, new QTableWidgetItem(armor.at(i)->getName()));
			}

			QVector<Accessory*> accessories = character->getEquipment()->getAccessories();
			for (int i = 0; i < accessories.size(); i++) {
				if (!!accessories.at(i))
					_equipmentList->setItem(i + 7, 0, new QTableWidgetItem(accessories.at(i)->getName()));
			}
			

			// Images
			QPair<QPixmap, QString> environmentImage = character->getEnvironmentImage();
			QPair<QPixmap, QString> battleImage = character->getBattleImage();

			if (environmentImage.second != QString())
				_envImageLabel->setPixmap(environmentImage.first);
			else
				_envImageLabel->setText("No image selected");

			if (battleImage.second != QString())
				_battleImageLabel->setPixmap(battleImage.first);
			else
				_battleImageLabel->setText("No image selected");
		}
	}
}

void CharactersWindow::newCharacter() {
	CharacterConfigWindow configDiag(0, _project);
	if (configDiag.exec()) {
		Character *character = 0;
		try {
			character = Character::create(configDiag.getName());
			character->setLevel(configDiag.getLevel());

			if (configDiag.getClass() != QString())
				character->setClass(_project->getProperties()->getClasses()->getClass(configDiag.getClass()));

			character->setEnvironmentImage(configDiag.getEnvironmentImage());
			character->setBattleImage(configDiag.getBattleImage());
			_project->getCharacters()->addCharacter(character);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete character;
			return;
		}

		_characterSelection->addItem(configDiag.getName());

		if (_characterSelection->count() > 0)
			_characterSelection->setCurrentRow(_characterSelection->count() -1);
	}
}

void CharactersWindow::editCharacter() {
	if (_characterSelection->count() > 0) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {
			CharacterConfigWindow configDiag(0, _project);
			configDiag.setName(character->getName());
			configDiag.setLevel(character->getLevel());
			configDiag.setEnvironmentImage(character->getEnvironmentImage().second);
			configDiag.setBattleImage(character->getBattleImage().second);

			if (!!character->getClass())
				configDiag.setClass(character->getClass()->getName());
			else
				configDiag.setClass(QString());

			if (configDiag.exec()) {
				try {
					character->setName(configDiag.getName());
					character->setLevel(configDiag.getLevel());
					character->setEnvironmentImage(configDiag.getEnvironmentImage());
					character->setBattleImage(configDiag.getBattleImage());

					if (configDiag.getClass() != QString())
						character->setClass(_project->getProperties()->getClasses()->getClass(configDiag.getClass()));
					else
						character->unsetClass();

				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_characterSelection->item(_characterSelection->currentRow())->setText(character->getName());
				characterSelected(character->getName());
			}
		}
	}
}

void CharactersWindow::deleteCharacter() {
	if (_characterSelection->count() > 0) {
		QMessageBox::StandardButton response = QMessageBox::warning(this, tr("Delete character"),
			tr("Are you sure you want to delete this character?\nDeleting a character will delete all references to it and cannot be undone."),
			QMessageBox::Ok | QMessageBox::Cancel);

		if (response == QMessageBox::Ok) {
			_project->getCharacters()->removeCharacter(_characterSelection->currentItem()->text());

			loadCharacters();
			if (_characterSelection->count() > 0)
				_characterSelection->setCurrentRow(0);
		}
	}
}

void CharactersWindow::moveCharacterUp() {
	if ((_characterSelection->count() > 0) && !!_characterSelection->currentItem() && (_characterSelection->currentRow() != 0)) {

		Character *currentCharacter = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!currentCharacter) {
			const QVector<Character*>& characters = _project->getCharacters()->getCharacters();
			int index = characters.indexOf(currentCharacter);

			try {
				switchPlaces(const_cast<QVector<Character*>&>(characters), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadCharacters();
			_characterSelection->setCurrentRow(index-1);
		}
	}
}

void CharactersWindow::moveCharacterDown() {
	if ((_characterSelection->count() > 0) && !!_characterSelection->currentItem() && (_characterSelection->currentRow() != _characterSelection->count()-1)) {

		Character *currentCharacter = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!currentCharacter) {
			const QVector<Character*>& characters = _project->getCharacters()->getCharacters();
			int index = characters.indexOf(currentCharacter);

			try {
				switchPlaces(const_cast<QVector<Character*>&>(characters), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadCharacters();
			_characterSelection->setCurrentRow(index+1);
		}
	}
}

void CharactersWindow::saveBiography() {
	if (_characterSelection->count() > 0) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {
			character->setBiography(_biographyField->toPlainText());
		}
	}
}

void CharactersWindow::reloadBiography() {
	if (_characterSelection->count() > 0) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {
			_biographyField->setPlainText(character->getBiography());
		}
	}
}

void CharactersWindow::addElement() {
	if (_characterSelection->count() > 0) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			// Setup window
			QDialog elementwindow(0);
			elementwindow.setWindowTitle("Add element to " + character->getName());
			QVBoxLayout *layout = new QVBoxLayout(&elementwindow);

			QLabel *label = new QLabel("Select an element:", &elementwindow);
			QComboBox *elementselection = new QComboBox(&elementwindow);
			elementselection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QFrame *buttonframe = new QFrame(&elementwindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(elementselection);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &elementwindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &elementwindow, SLOT(reject()));

			QVector<Element*> characterElements = character->getElements();
			QVector<Element*> elements = _project->getProperties()->getElements()->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				if (!characterElements.contains(it))
					elementselection->addItem(it->getName());
			}

			if (elementselection->count() == 0) {
				QMessageBox::information(this, tr("No elements available"), tr("There are no more elements available to choose from."));
				return;
			}

			if (elementwindow.exec()) {
				Element *element = _project->getProperties()->getElements()->getElement(elementselection->currentText());
				if (!!element) {
					try {
						character->addElement(element);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_elementsList->addItem(element->getName());
				}
			}
		}
	}
}

void CharactersWindow::deleteElement() {
	if ((_characterSelection->count() > 0) && (_elementsList->count() > 0) && (!!_elementsList->currentItem())) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {
			character->removeElement(_elementsList->currentItem()->text());
			_elementsList->takeItem(_elementsList->currentRow());
		}
	}
}

void CharactersWindow::moveElementUp() {
	if ((_characterSelection->count() > 0) && !!_characterSelection->currentItem() && (_elementsList->count() > 0) && !!_elementsList->currentItem() && (_elementsList->currentRow() != 0)) {

		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			Element *element = character->getElements().at(_elementsList->currentRow());
			if (!!element) {
				const QVector<Element*>& elements = character->getElements();
				int index = elements.indexOf(element);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int characterIndex = _characterSelection->currentRow();
				loadCharacters();
				_characterSelection->setCurrentRow(characterIndex);
				_elementsList->setCurrentRow(index-1);
			}
		}
	}
}

void CharactersWindow::moveElementDown() {
	if ((_characterSelection->count() > 0) && !!_characterSelection->currentItem() && (_elementsList->count() > 0) && !!_elementsList->currentItem() && (_elementsList->currentRow() != _elementsList->count()-1)) {

		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			Element *element = character->getElements().at(_elementsList->currentRow());
			if (!!element) {
				const QVector<Element*>& elements = character->getElements();
				int index = elements.indexOf(element);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int characterIndex = _characterSelection->currentRow();
				loadCharacters();
				_characterSelection->setCurrentRow(characterIndex);
				_elementsList->setCurrentRow(index+1);
			}
		}
	}
}

void CharactersWindow::addAbility() {
	if (_characterSelection->count() > 0) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			// Setup dialog
			QDialog abilitywindow(0);
			abilitywindow.setWindowTitle("Add ability to " + character->getName());
			QVBoxLayout *layout = new QVBoxLayout(&abilitywindow);

			QLabel *label1 = new QLabel("Select an ability:", &abilitywindow);
			QFrame *frame1 = new QFrame(&abilitywindow);
			QHBoxLayout *layout1 = new QHBoxLayout(frame1);
			QComboBox *abilityselection = new QComboBox(&abilitywindow);
			abilityselection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QLabel *label2 = new QLabel(" is learned at level ");
			QSpinBox *levelselection = new QSpinBox(&abilitywindow);
			levelselection->setMinimum(0);
			levelselection->setMaximum(99);
			layout1->addWidget(abilityselection);
			layout1->addWidget(label2);
			layout1->addWidget(levelselection);
			QFrame *buttonframe = new QFrame(&abilitywindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", &abilitywindow);
			QPushButton *cancelbutton = new QPushButton("Cancel", &abilitywindow);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label1);
			layout->addWidget(frame1);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &abilitywindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &abilitywindow, SLOT(reject()));

			QVector<Ability*> abilities = _project->getProperties()->getAbilities()->getAbilities();
			QVector<QPair<Ability*, int>> characterabilities = character->getAbilities();
			for (int i = 0; i < abilities.size(); i++) {
				Ability *it = abilities.at(i);
				for (int j = 0; j < characterabilities.size(); j++) {
					if (characterabilities.at(j).first == it)
						goto skip;
				}

				abilityselection->addItem(it->getName());

				skip:;
			}

			if (abilityselection->count() == 0) {
				QMessageBox::information(this, tr("No abilities available"), tr("There are no more abilities available to choose from."));
				return;
			}

			if (abilitywindow.exec()) {
				Ability *ability = _project->getProperties()->getAbilities()->getAbility(abilityselection->currentText());
				int level = levelselection->value();

				if (!!ability) {
					try {
						character->addAbility(ability, level);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					QString item;
					item = ability->getName() + " - level " + QString::number(level);
					_abilitiesList->addItem(item);
				}
			}

		}
	}
}

void CharactersWindow::editAbility() {
	if ((_characterSelection->count() > 0) && (_abilitiesList->count() > 0) && (!!_abilitiesList->currentItem())) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {
			QString abilityname = _abilitiesList->currentItem()->text().left(_abilitiesList->currentItem()->text().lastIndexOf(QString(" - ")));
			QPair<Ability*, int> ability = character->getAbility(abilityname);

			// Setup window
			QDialog abilitywindow(0);
			abilitywindow.setWindowTitle("Edit ability of " + character->getName());
			QVBoxLayout *layout = new QVBoxLayout(&abilitywindow);

			QLabel *label1 = new QLabel("Change the level:", &abilitywindow);
			QFrame *frame1 = new QFrame(&abilitywindow);
			QHBoxLayout *layout1 = new QHBoxLayout(frame1);
			QLabel *label2 = new QLabel(ability.first->getName() + " is learned at level ");
			QSpinBox *levelselection = new QSpinBox(frame1);
			levelselection->setMaximum(99);
			levelselection->setMinimum(0);
			levelselection->setValue(ability.second);
			layout1->addWidget(label2);
			layout1->addWidget(levelselection);
			QFrame *buttonframe = new QFrame(&abilitywindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", &abilitywindow);
			QPushButton *cancelbutton = new QPushButton("Cancel", &abilitywindow);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label1);
			layout->addWidget(frame1);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &abilitywindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &abilitywindow, SLOT(reject()));

			if (abilitywindow.exec()) {

				try {
					character->editAbility(ability.first->getName(), levelselection->value());
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_abilitiesList->currentItem()->setText(ability.first->getName() + "- level " + QString::number(levelselection->value()));
			}
		}
	}
}

void CharactersWindow::deleteAbility() {
	if ((_characterSelection->count() > 0) && (_abilitiesList->count() > 0) && (!!_abilitiesList->currentItem())) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {
			QString abilityname = _abilitiesList->currentItem()->text().left(_abilitiesList->currentItem()->text().lastIndexOf(QString(" - ")));
			character->removeAbility(abilityname);
			_abilitiesList->takeItem(_abilitiesList->currentRow());
		}
	}
}

void CharactersWindow::moveAbilityUp() {
	if ((_characterSelection->count() > 0) && !!_characterSelection->currentItem() && (_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_abilitiesList->currentRow() != 0)) {

		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			QPair<Ability*, int> ability = character->getAbilities().at(_abilitiesList->currentRow());
			if (!!ability.first) {
				const QVector<QPair<Ability*, int>>& abilities = character->getAbilities();
				int index = abilities.indexOf(ability);

				try {
					switchPairs(const_cast<QVector<QPair<Ability*, int>>&>(abilities), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int characterIndex = _characterSelection->currentRow();
				loadCharacters();
				_characterSelection->setCurrentRow(characterIndex);
				_abilitiesList->setCurrentRow(index-1);
			}
		}
	}
}

void CharactersWindow::moveAbilityDown() {
	if ((_characterSelection->count() > 0) && !!_characterSelection->currentItem() && (_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_abilitiesList->currentRow() != _abilitiesList->count()-1)) {

		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			QPair<Ability*, int> ability = character->getAbilities().at(_abilitiesList->currentRow());
			if (!!ability.first) {
				const QVector<QPair<Ability*, int>>& abilities = character->getAbilities();
				int index = abilities.indexOf(ability);

				try {
					switchPairs(const_cast<QVector<QPair<Ability*, int>>&>(abilities), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int characterIndex = _characterSelection->currentRow();
				loadCharacters();
				_characterSelection->setCurrentRow(characterIndex);
				_abilitiesList->setCurrentRow(index+1);
			}
		}
	}
}

void CharactersWindow::editEquipment() {
	if ((_characterSelection->count() > 0) && (_equipmentList->currentRow() >= 0)) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			QStringList equipmentItems;

			int currentRow = _equipmentList->currentRow();
			if (currentRow < 2) {
				Weapon::Subtype filter;
				if (currentRow == 0)
					filter = Weapon::MAINHAND;
				else if (currentRow == 1)
					filter = Weapon::OFFHAND;

				QVector<Weapon*> weapons = _project->getProperties()->getEquipment()->getWeapons();
				for (int i = 0; i < weapons.size(); i++) {
					Weapon *it = weapons.at(i);
					if (((it->getSubtype() == filter) || (it->getSubtype() == Weapon::TWOHANDED)) && (!!it->getClass(character->getClass()->getName()).first))
						equipmentItems << it->getName();
				}
			} else if (currentRow < 7) {
				Armor::Subtype filter;
				if (currentRow == 2)
					filter = Armor::HELMET;
				else if (currentRow == 3)
					filter = Armor::TORSO;
				else if (currentRow == 4)
					filter = Armor::LEGGINGS;
				else if (currentRow == 5)
					filter = Armor::GLOVES;
				else if (currentRow == 6)
					filter = Armor::BOOTS;

				QVector<Armor*> armor = _project->getProperties()->getEquipment()->getArmor();
				for (int i = 0; i < armor.size(); i++) {
					Armor *it = armor.at(i);
					if ((it->getSubtype() == filter) && (!!it->getClass(character->getClass()->getName()).first))
						equipmentItems << it->getName();
				}
			} else if (currentRow < 10) {
				Accessory::Subtype filter;
				if (currentRow == 7)
					filter = Accessory::EARRING;
				else if (currentRow == 8)
					filter = Accessory::NECKLACE;
				else if (currentRow == 9)
					filter = Accessory::RING;

				QVector<Accessory*> accessories = _project->getProperties()->getEquipment()->getAccessories();
				for (int i = 0; i < accessories.size(); i++) {
					Accessory *it = accessories.at(i);
					if ((it->getSubtype() == filter) && (!!it->getClass(character->getClass()->getName()).first))
						equipmentItems << it->getName();
				}
			} else {
				return;
			}

			if (equipmentItems.count() == 0) {
				QMessageBox::information(this, tr("No equipment available"), tr("There is no equipment available to place in that slot"));
				return;
			}

			bool accepted;
			QString equipmentName = QInputDialog::getItem(this, "Place equipment", "Place equipment in slot " + _equipmentList->verticalHeaderItem(currentRow)->text() + ":", equipmentItems, 0, false, &accepted);

			if (accepted) {
				if (currentRow < 2) {
					Weapon *weapon = _project->getProperties()->getEquipment()->getWeapon(equipmentName);
					if (!!weapon) {
						switch (weapon->getSubtype()) {
						case Weapon::MAINHAND:
							character->getEquipment()->setMainhand(weapon);
							break;
						case Weapon::OFFHAND:
							character->getEquipment()->setOffhand(weapon);
							break;
						case  Weapon::TWOHANDED:
							character->getEquipment()->setTwohanded(weapon);
							break;
						}

						_equipmentList->takeItem(0, 0);
						_equipmentList->takeItem(1, 0);

						if (!!character->getEquipment()->getMainhand())
							_equipmentList->setItem(0, 0, new QTableWidgetItem(character->getEquipment()->getMainhand()->getName()));

						if (!!character->getEquipment()->getOffhand())
							_equipmentList->setItem(1, 0, new QTableWidgetItem(character->getEquipment()->getOffhand()->getName()));

						if (!!character->getEquipment()->getTwohanded()) {
							_equipmentList->setItem(0, 0, new QTableWidgetItem(character->getEquipment()->getTwohanded()->getName()));
							_equipmentList->setItem(1, 0, new QTableWidgetItem(character->getEquipment()->getTwohanded()->getName()));
						}
					}
				} else if (currentRow < 7) {
					Armor *armor = _project->getProperties()->getEquipment()->getArmor(equipmentName);
					if (!!armor) {
						switch (armor->getSubtype()) {
						case Armor::HELMET:
							character->getEquipment()->setHelmet(armor);
							break;
						case Armor::TORSO:
							character->getEquipment()->setTorso(armor);
							break;
						case Armor::LEGGINGS:
							character->getEquipment()->setLeggings(armor);
							break;
						case Armor::GLOVES:
							character->getEquipment()->setGloves(armor);
							break;
						case Armor::BOOTS:
							character->getEquipment()->setBoots(armor);
							break;
						}
						_equipmentList->setItem(currentRow, 0, new QTableWidgetItem(equipmentName));
					}
				} else if (currentRow < 10) {
					Accessory *accessory = _project->getProperties()->getEquipment()->getAccessory(equipmentName);
					if (!!accessory) {
						switch (accessory->getSubtype()) {
						case Accessory::EARRING:
							character->getEquipment()->setEarring(accessory);
							break;
						case Accessory::NECKLACE:
							character->getEquipment()->setNecklace(accessory);
							break;
						case Accessory::RING:
							character->getEquipment()->setRing(accessory);
							break;
						}
						_equipmentList->setItem(currentRow, 0, new QTableWidgetItem(equipmentName));
					}
				}
			}
		}
	}
}

void CharactersWindow::deleteEquipment() {
	if ((_characterSelection->count() > 0) && (_equipmentList->currentRow() >= 0) && (!!_equipmentList->currentItem())) {
		Character *character = _project->getCharacters()->getCharacter(_characterSelection->currentItem()->text());
		if (!!character) {

			int currentRow = _equipmentList->currentRow();
			if (currentRow < 2) {
				Weapon *weapon = _project->getProperties()->getEquipment()->getWeapon(_equipmentList->currentItem()->text());
				switch (weapon->getSubtype()) {
				case Weapon::MAINHAND:
					character->getEquipment()->unsetMainhand();
					_equipmentList->takeItem(currentRow, 0);
					break;
				case Weapon::OFFHAND:
					character->getEquipment()->unsetOffhand();
					_equipmentList->takeItem(currentRow, 0);
					break;
				case Weapon::TWOHANDED:
					character->getEquipment()->unsetTwohanded();
					_equipmentList->takeItem(0, 0);
					_equipmentList->takeItem(1, 0);
					break;
				}
			} else if (currentRow < 7) {
				Armor *armor = _project->getProperties()->getEquipment()->getArmor(_equipmentList->currentItem()->text());
				switch (armor->getSubtype()) {
				case Armor::HELMET:
					character->getEquipment()->unsetHelmet();
					break;
				case Armor::TORSO:
					character->getEquipment()->unsetTorso();
					break;
				case Armor::LEGGINGS:
					character->getEquipment()->unsetLeggings();
					break;
				case Armor::GLOVES:
					character->getEquipment()->unsetGloves();
					break;
				case Armor::BOOTS:
					character->getEquipment()->unsetBoots();
					break;
				}
				_equipmentList->takeItem(currentRow, 0);
			} else if (currentRow < 10) {
				Accessory *accessory = _project->getProperties()->getEquipment()->getAccessory(_equipmentList->currentItem()->text());
				switch (accessory->getSubtype()) {
				case Accessory::EARRING:
					character->getEquipment()->unsetEarring();
					break;
				case Accessory::NECKLACE:
					character->getEquipment()->unsetNecklace();
					break;
				case Accessory::RING:
					character->getEquipment()->unsetRing();
					break;
				}
				_equipmentList->takeItem(currentRow, 0);
			}
		}
	}
}