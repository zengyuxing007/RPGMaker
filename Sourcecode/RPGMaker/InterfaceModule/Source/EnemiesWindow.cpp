#include "qdir.h"
#include "qvector.h"
#include "qspinbox.h"
#include "qgroupbox.h"
#include "qstringlist.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qgridlayout.h"
#include "qinputdialog.h"

#include "../Headers/EnemiesWindow.h"
#include "../Headers/EnemyConfigWindow.h"
#include "../Headers/BattlegroupsDialog.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Ability.h"
#include "../Components/Headers/Character.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;
using namespace projectcomponents;

EnemiesWindow::EnemiesWindow(QWidget *parent, Project *project)
	: _project(project) {

		createWindow();
		loadEnemies();

		if (_enemySelection->count() > 0)
			_enemySelection->setCurrentRow(0);
}

EnemiesWindow::~EnemiesWindow() {}

void EnemiesWindow::createWindow() {
	QGridLayout *layout = new QGridLayout(this);

	// Enemy frame
	QGroupBox *enemyFrame = new QGroupBox("Enemies", this);
	QVBoxLayout *enemyLayout = new QVBoxLayout(enemyFrame);
	enemyLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(enemyFrame, 0, 0, 3, 1);

	// Biography frame
	QGroupBox *descriptionFrame = new QGroupBox("Description", this);
	QVBoxLayout *descriptionLayout = new QVBoxLayout(descriptionFrame);
	descriptionLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(descriptionFrame, 0, 1);

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

	// Drops frame
	QGroupBox *dropsFrame = new QGroupBox("Drops", this);
	QVBoxLayout *dropsLayout = new QVBoxLayout(dropsFrame);
	dropsLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(dropsFrame, 0, 2);

	// Images frame
	QGroupBox *imagesFrame = new QGroupBox("Images", this);
	QHBoxLayout *imagesLayout = new QHBoxLayout(imagesFrame);
	imagesLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(imagesFrame, 2, 1, 1, 2);

	// Enemy field
	QLabel *label1 = new QLabel("Select an enemy:", enemyFrame);
	enemyLayout->addWidget(label1);

	_enemySelection = new QListWidget(enemyFrame);
	enemyLayout->addWidget(_enemySelection);

	QFrame *enemyButtonFrame = new QFrame(enemyFrame);
	enemyLayout->addWidget(enemyButtonFrame);
	QHBoxLayout *enemyButtonLayout = new QHBoxLayout(enemyButtonFrame);
	enemyButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *enemyNewButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", enemyButtonFrame);
	enemyNewButton->setToolTip(tr("Add a new enemy to the project"));
	enemyNewButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *enemyEditButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", enemyButtonFrame);
	enemyEditButton->setToolTip(tr("Edit the current enemy"));
	enemyEditButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *enemyDeleteButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", enemyButtonFrame);
	enemyDeleteButton->setToolTip(tr("Delete the current enemy"));
	enemyDeleteButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *battlegroupButton = new QPushButton(QIcon(g_iconPath + "users_icon&24.png"), "", enemyButtonFrame);
	battlegroupButton->setToolTip("Manage battlegroups");
	battlegroupButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *enemyUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", enemyButtonFrame);
	enemyUpButton->setToolTip("Move selected enemy up");
	enemyUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *enemyDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", enemyButtonFrame);
	enemyDownButton->setToolTip("Move selected enemy down");
	enemyDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	enemyButtonLayout->addWidget(enemyNewButton);
	enemyButtonLayout->addWidget(enemyEditButton);
	enemyButtonLayout->addWidget(enemyDeleteButton);
	enemyButtonLayout->addSpacing(20);
	enemyButtonLayout->addWidget(battlegroupButton);
	enemyButtonLayout->addSpacing(20);
	enemyButtonLayout->addWidget(enemyUpButton);
	enemyButtonLayout->addWidget(enemyDownButton);

	// Description field
	_descriptionLabel = new QLabel(descriptionFrame);
	_descriptionField = new QPlainTextEdit(descriptionFrame);
	QFrame *descriptionButtonFrame = new QFrame(descriptionFrame);
	QHBoxLayout *descriptionButtonLayout = new QHBoxLayout(descriptionButtonFrame);
	descriptionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveDescriptionButton = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", descriptionButtonFrame);
	saveDescriptionButton->setToolTip("Save the description for the current enemy");
	saveDescriptionButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadDescriptionButton = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", descriptionButtonFrame);
	reloadDescriptionButton->setToolTip("Reload the description of the current enemy");
	reloadDescriptionButton->setMaximumSize(g_buttonsize, g_buttonsize);
	descriptionButtonLayout->addWidget(saveDescriptionButton);
	descriptionButtonLayout->addWidget(reloadDescriptionButton);
	descriptionLayout->addWidget(_descriptionLabel);
	descriptionLayout->addWidget(_descriptionField);
	descriptionLayout->addWidget(descriptionButtonFrame);

	// Elements field
	_elementsLabel = new QLabel(elementsFrame);
	_elementsList = new QListWidget(elementsFrame);
	QFrame *elementsButtonFrame = new QFrame(elementsFrame);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(elementsButtonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addElementButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", elementsButtonFrame);
	addElementButton->setToolTip("Add an element to the current enemy");
	addElementButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteElementButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", elementsButtonFrame);
	deleteElementButton->setToolTip("Delete the element from the current enemy");
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
	addAbilityButton->setToolTip("Add an ability to the current enemy");
	addAbilityButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAbilityButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", abilitiesButtonFrame);
	editAbilityButton->setToolTip("Edit the current ability for this enemy");
	editAbilityButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAbilityButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", abilitiesButtonFrame);
	deleteAbilityButton->setToolTip("Delete the ability from the current enemy");
	deleteAbilityButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", abilitiesButtonFrame);
	abilityUpButton->setToolTip("Move selected ability up");
	abilityUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", abilitiesButtonFrame);
	abilityDownButton->setToolTip("Move selected ability down");
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

	// Drops field
	_dropsLabel = new QLabel(dropsFrame);
	_dropList = new QListWidget(dropsFrame);
	QFrame *dropsButtonFrame = new QFrame(dropsFrame);
	QHBoxLayout *dropsButtonLayout = new QHBoxLayout(dropsButtonFrame);
	dropsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addDropButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", dropsButtonFrame);
	addDropButton->setToolTip("Add a drop to the current enemy");
	addDropButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editDropButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", dropsButtonFrame);
	editDropButton->setToolTip(tr("Edit the current drop"));
	editDropButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteDropButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", dropsButtonFrame);
	deleteDropButton->setToolTip("Delete the drop from the current enemy");
	deleteDropButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *dropUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", dropsButtonFrame);
	dropUpButton->setToolTip("Move selected drop up");
	dropUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *dropDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", dropsButtonFrame);
	dropDownButton->setToolTip("Move selected drop down");
	dropDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	dropsButtonLayout->addWidget(addDropButton);
	dropsButtonLayout->addWidget(editDropButton);
	dropsButtonLayout->addWidget(deleteDropButton);
	dropsButtonLayout->addSpacing(20);
	dropsButtonLayout->addWidget(dropUpButton);
	dropsButtonLayout->addWidget(dropDownButton);
	dropsLayout->addWidget(_dropsLabel);
	dropsLayout->addWidget(_dropList);
	dropsLayout->addWidget(dropsButtonFrame);

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

	connect(_enemySelection, SIGNAL(currentTextChanged(const QString &)), this, SLOT(enemySelected(const QString &)));
	connect(enemyNewButton, SIGNAL(clicked()), this, SLOT(newEnemy()));
	connect(enemyEditButton, SIGNAL(clicked()), this, SLOT(editEnemy()));
	connect(enemyDeleteButton, SIGNAL(clicked()), this, SLOT(deleteEnemy()));
	connect(battlegroupButton, SIGNAL(clicked()), this, SLOT(battlegroupDialog()));
	connect(enemyUpButton, SIGNAL(clicked()), this, SLOT(moveEnemyUp()));
	connect(enemyDownButton, SIGNAL(clicked()), this, SLOT(moveEnemyDown()));
	connect(saveDescriptionButton, SIGNAL(clicked()), this, SLOT(saveDescription()));
	connect(reloadDescriptionButton, SIGNAL(clicked()), this, SLOT(reloadDescription()));
	connect(addElementButton, SIGNAL(clicked()), this, SLOT(addElement()));
	connect(deleteElementButton, SIGNAL(clicked()), this, SLOT(deleteElement()));
	connect(elementUpButton, SIGNAL(clicked()), this, SLOT(moveElementUp()));
	connect(elementDownButton, SIGNAL(clicked()), this, SLOT(moveElementDown()));
	connect(addAbilityButton, SIGNAL(clicked()), this, SLOT(addAbility()));
	connect(deleteAbilityButton, SIGNAL(clicked()), this, SLOT(deleteAbility()));
	connect(abilityUpButton, SIGNAL(clicked()), this, SLOT(moveAbilityUp()));
	connect(abilityDownButton, SIGNAL(clicked()), this, SLOT(moveAbilityDown()));
	connect(addDropButton, SIGNAL(clicked()), this, SLOT(newDrop()));
	connect(editDropButton, SIGNAL(clicked()), this, SLOT(editDrop()));
	connect(deleteDropButton, SIGNAL(clicked()), this, SLOT(deleteDrop()));
	connect(dropUpButton, SIGNAL(clicked()), this, SLOT(moveDropUp()));
	connect(dropDownButton, SIGNAL(clicked()), this, SLOT(moveDropDown()));
}

void EnemiesWindow::loadEnemies() {
	_enemySelection->clear();
	QVector<Enemy*> enemies = _project->getEnemies()->getEnemies();

	for (int i = 0; i < enemies.size(); i++) {
		_enemySelection->addItem(enemies.at(i)->getName());
	}
}

void EnemiesWindow::enemySelected(const QString &name) {
	_descriptionLabel->clear();
	_descriptionField->clear();
	_elementsLabel->clear();
	_elementsList->clear();
	_abilitiesLabel->clear();
	_abilitiesList->clear();
	_dropsLabel->clear();
	_dropList->clear();
	_envImageLabel->clear();
	_battleImageLabel->clear();

	if ((_enemySelection->count() > 0) && (name != QString())) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			// Description
			_descriptionLabel->setText("Description of " + enemy->getName());
			_descriptionField->setPlainText(enemy->getDescription());

			// Elements
			_elementsLabel->setText("Elements of " + enemy->getName());
			QVector<Element*> elements = enemy->getElements();
			for (int i = 0; i < elements.size(); i++) {
				_elementsList->addItem(elements.at(i)->getName());
			}

			// Abilities
			_abilitiesLabel->setText("Abilities of " + enemy->getName());
			QVector<Ability*> abilities = enemy->getAbilities();
			for (int i = 0; i < abilities.size(); i++) {
				_abilitiesList->addItem(abilities.at(i)->getName());
			}

			// Drops
			_dropsLabel->setText("Drops of " + enemy->getName());
			QVector<QPair<EquipmentPiece*, int>> drops = enemy->getDrops()->getItems();
			for (int i =0; i < drops.size(); i++) {
				QPair<EquipmentPiece*, int> it = drops.at(i);
				_dropList->addItem(it.first->getName() + " - chance " + QString::number(it.second) + "%");
			}

			// Images
			QPair<QPixmap, QString> environmentImage = enemy->getEnvironmentImage();
			QPair<QPixmap, QString> battleImage = enemy->getBattleImage();

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

void EnemiesWindow::newEnemy() {
	EnemyConfigWindow configDiag(0, _project);
	if (configDiag.exec()) {
		Enemy *enemy = 0;
		try {
			enemy = Enemy::create(configDiag.getName());
			enemy->setLevel(configDiag.getLevel());

			if (configDiag.getClass() != QString())
				enemy->setClass(_project->getProperties()->getClasses()->getClass(configDiag.getClass()));

			enemy->getDrops()->setExperience(configDiag.getExperience());
			enemy->getDrops()->setCurrency(configDiag.getCurrency());
			enemy->setEnvironmentImage(configDiag.getEnvironmentImage());
			enemy->setBattleImage(configDiag.getBattleImage());
			_project->getEnemies()->addEnemy(enemy);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete enemy;
			return;
		}

		_enemySelection->addItem(configDiag.getName());

		if (_enemySelection->count() > 0)
			_enemySelection->setCurrentRow(_enemySelection->count() -1);
	}
}

void EnemiesWindow::editEnemy() {
	if (_enemySelection->count() > 0) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			EnemyConfigWindow configDiag(0, _project);
			configDiag.setName(enemy->getName());
			configDiag.setLevel(enemy->getLevel());
			configDiag.setExperience(enemy->getDrops()->getExperience());
			configDiag.setCurrency(enemy->getDrops()->getCurrency());
			configDiag.setEnvironmentImage(enemy->getEnvironmentImage().second);
			configDiag.setBattleImage(enemy->getBattleImage().second);

			if (!!enemy->getClass())
				configDiag.setClass(enemy->getClass()->getName());
			else
				configDiag.setClass(QString());

			if (configDiag.exec()) {
				try {
					enemy->setName(configDiag.getName());
					enemy->setLevel(configDiag.getLevel());
					enemy->getDrops()->setExperience(configDiag.getExperience());
					enemy->getDrops()->setCurrency(configDiag.getCurrency());
					enemy->setEnvironmentImage(configDiag.getEnvironmentImage());
					enemy->setBattleImage(configDiag.getBattleImage());

					if (configDiag.getClass() != QString())
						enemy->setClass(_project->getProperties()->getClasses()->getClass(configDiag.getClass()));
					else
						enemy->unsetClass();

				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_enemySelection->item(_enemySelection->currentRow())->setText(enemy->getName());
				enemySelected(enemy->getName());
			}
		}
	}
}

void EnemiesWindow::deleteEnemy() {
	if (_enemySelection->count() > 0) {
		QMessageBox::StandardButton response = QMessageBox::warning(this, tr("Delete enemy"),
			tr("Are you sure you want to delete this enemy?\nDeleting an enemy will delete all references to it and cannot be undone."),
			QMessageBox::Ok | QMessageBox::Cancel);

		if (response == QMessageBox::Ok) {
			_project->getEnemies()->removeEnemy(_enemySelection->currentItem()->text());

			loadEnemies();
			if (_enemySelection->count() > 0)
				_enemySelection->setCurrentRow(0);
		}
	}
}

void EnemiesWindow::battlegroupDialog() {
	BattlegroupsDialog dialog(this, _project);
	dialog.exec();
}

void EnemiesWindow::moveEnemyUp() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_enemySelection->currentRow() != 0)) {

		Enemy *currentEnemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!currentEnemy) {
			const QVector<Enemy*>& enemies = _project->getEnemies()->getEnemies();
			int index = enemies.indexOf(currentEnemy);

			try {
				switchPlaces(const_cast<QVector<Enemy*>&>(enemies), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEnemies();
			_enemySelection->setCurrentRow(index-1);
		}
	}
}

void EnemiesWindow::moveEnemyDown() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_enemySelection->currentRow() != _enemySelection->count()-1)) {

		Enemy *currentEnemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!currentEnemy) {
			const QVector<Enemy*>& enemies = _project->getEnemies()->getEnemies();
			int index = enemies.indexOf(currentEnemy);

			try {
				switchPlaces(const_cast<QVector<Enemy*>&>(enemies), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEnemies();
			_enemySelection->setCurrentRow(index+1);
		}
	}
}

void EnemiesWindow::saveDescription() {
	if (_enemySelection->count() > 0) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			enemy->setDescription(_descriptionField->toPlainText());
		}
	}
}

void EnemiesWindow::reloadDescription() {
	if (_enemySelection->count() > 0) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			_descriptionField->setPlainText(enemy->getDescription());
		}
	}
}

void EnemiesWindow::addElement() {
	if (_enemySelection->count() > 0) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			// Setup window
			QDialog elementwindow(0);
			elementwindow.setWindowTitle("Add element to " + enemy->getName());
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

			QVector<Element*> enemyElements = enemy->getElements();
			QVector<Element*> elements = _project->getProperties()->getElements()->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				if (!enemyElements.contains(it))
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
						enemy->addElement(element);
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

void EnemiesWindow::deleteElement() {
	if ((_enemySelection->count() > 0) && (_elementsList->count() > 0) && (!!_elementsList->currentItem())) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			QString elementname = _elementsList->currentItem()->text();
			enemy->removeElement(elementname);
			_elementsList->takeItem(_elementsList->currentRow());
		}
	}
}

void EnemiesWindow::moveElementUp() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_elementsList->count() > 0) && !!_elementsList->currentItem() && (_elementsList->currentRow() != 0)) {

		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			Element *element = enemy->getElements().at(_elementsList->currentRow());
			if (!!element) {
				const QVector<Element*>& elements = enemy->getElements();
				int index = elements.indexOf(element);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int enemyIndex = _enemySelection->currentRow();
				loadEnemies();
				_enemySelection->setCurrentRow(enemyIndex);
				_elementsList->setCurrentRow(index-1);
			}
		}
	}
}

void EnemiesWindow::moveElementDown() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_elementsList->count() > 0) && !!_elementsList->currentItem() && (_elementsList->currentRow() != _elementsList->count()-1)) {

		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			Element *element = enemy->getElements().at(_elementsList->currentRow());
			if (!!element) {
				const QVector<Element*>& elements = enemy->getElements();
				int index = elements.indexOf(element);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int enemyIndex = _enemySelection->currentRow();
				loadEnemies();
				_enemySelection->setCurrentRow(enemyIndex);
				_elementsList->setCurrentRow(index+1);
			}
		}
	}
}

void EnemiesWindow::addAbility() {
	if (_enemySelection->count() > 0) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			// Setup dialog
			QDialog abilitywindow(0);
			abilitywindow.setWindowTitle("Add ability to " + enemy->getName());
			QVBoxLayout *layout = new QVBoxLayout(&abilitywindow);

			QLabel *label1 = new QLabel("Select an ability:", &abilitywindow);
			QComboBox *abilityselection = new QComboBox(&abilitywindow);
			abilityselection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QFrame *buttonframe = new QFrame(&abilitywindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", &abilitywindow);
			QPushButton *cancelbutton = new QPushButton("Cancel", &abilitywindow);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label1);
			layout->addWidget(abilityselection);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &abilitywindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &abilitywindow, SLOT(reject()));

			QVector<Ability*> abilities = _project->getProperties()->getAbilities()->getAbilities();
			QVector<Ability*> enemyAbilities = enemy->getAbilities();
			for (int i = 0; i < abilities.size(); i++) {
				Ability *it = abilities.at(i);
				if (!enemyAbilities.contains(it))
					abilityselection->addItem(it->getName());
			}

			if (abilityselection->count() == 0) {
				QMessageBox::information(this, tr("No abilities available"), tr("There are no more abilities available to choose from."));
				return;
			}

			if (abilitywindow.exec()) {
				Ability *ability = _project->getProperties()->getAbilities()->getAbility(abilityselection->currentText());

				if (!!ability) {
					try {
						enemy->addAbility(ability);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_abilitiesList->addItem(ability->getName());
				}
			}

		}
	}
}

void EnemiesWindow::deleteAbility() {
	if ((_enemySelection->count() > 0) && (_abilitiesList->count() > 0) && (!!_abilitiesList->currentItem())) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			QString abilityname = _abilitiesList->currentItem()->text().left(_abilitiesList->currentItem()->text().lastIndexOf(QString(" - ")));
			enemy->removeAbility(abilityname);
			_abilitiesList->takeItem(_abilitiesList->currentRow());
		}
	}
}

void EnemiesWindow::moveAbilityUp() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_abilitiesList->currentRow() != 0)) {

		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			Ability* ability = enemy->getAbilities().at(_abilitiesList->currentRow());
			if (!!ability) {
				const QVector<Ability*>& abilities = enemy->getAbilities();
				int index = abilities.indexOf(ability);

				try {
					switchPlaces(const_cast<QVector<Ability*>&>(abilities), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int enemyIndex = _enemySelection->currentRow();
				loadEnemies();
				_enemySelection->setCurrentRow(enemyIndex);
				_abilitiesList->setCurrentRow(index-1);
			}
		}
	}
}

void EnemiesWindow::moveAbilityDown() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_abilitiesList->currentRow() != _abilitiesList->count()-1)) {

		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			Ability* ability = enemy->getAbilities().at(_abilitiesList->currentRow());
			if (!!ability) {
				const QVector<Ability*>& abilities = enemy->getAbilities();
				int index = abilities.indexOf(ability);

				try {
					switchPlaces(const_cast<QVector<Ability*>&>(abilities), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int enemyIndex = _enemySelection->currentRow();
				loadEnemies();
				_enemySelection->setCurrentRow(enemyIndex);
				_abilitiesList->setCurrentRow(index+1);
			}
		}
	}
}

void EnemiesWindow::newDrop() {
	if (_enemySelection->count() > 0) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			QStringList dropTypes;
			dropTypes << "Weapon" << "Armor" << "Accessory";

			bool accepted;
			QString type = QInputDialog::getItem(0, "Type of drop", "Select the type of item this enemy should drop:", dropTypes, 0, false, &accepted);
			if (accepted) {

				// Setup dialog
				QDialog dropWindow(0);
				dropWindow.setWindowTitle("Add a drop to " + enemy->getName());
				QVBoxLayout *layout = new QVBoxLayout(&dropWindow);

				QFrame *frame = new QFrame(&dropWindow);
				QHBoxLayout *frameLayout = new QHBoxLayout(frame);
				QLabel *label1 = new QLabel("Select an item and a chance to drop:", frame);
				QComboBox *itemSelection = new QComboBox(frame);
				itemSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
				QSpinBox *chanceSelection = new QSpinBox(frame);
				chanceSelection->setMinimum(1);
				chanceSelection->setMaximum(100);
				frameLayout->addWidget(itemSelection);
				frameLayout->addWidget(chanceSelection);
				QFrame *buttonframe = new QFrame(&dropWindow);
				QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
				QPushButton *acceptbutton = new QPushButton("Accept", &dropWindow);
				QPushButton *cancelbutton = new QPushButton("Cancel", &dropWindow);
				buttonlayout->addWidget(acceptbutton);
				buttonlayout->addWidget(cancelbutton);

				layout->addWidget(label1);
				layout->addWidget(frame);
				layout->addWidget(buttonframe);

				connect(acceptbutton, SIGNAL(clicked()), &dropWindow, SLOT(accept()));
				connect(cancelbutton, SIGNAL(clicked()), &dropWindow, SLOT(reject()));

				if (type == "Weapon") {
					QVector<Weapon*> weapons = _project->getProperties()->getEquipment()->getWeapons();
					for (int i = 0; i < weapons.size(); i++) {
						itemSelection->addItem(weapons.at(i)->getName());
					}
				}
				else if (type == "Armor") {
					QVector<Armor*> armor = _project->getProperties()->getEquipment()->getArmor();
					for (int i = 0; i < armor.size(); i++) {
						itemSelection->addItem(armor.at(i)->getName());
					}
				}
				else if (type == "Accessory") {
					QVector<Accessory*> accessories = _project->getProperties()->getEquipment()->getAccessories();
					for (int i = 0; i < accessories.size(); i++) {
						itemSelection->addItem(accessories.at(i)->getName());
					}
				} else {
					return;
				}

				if (itemSelection->count() == 0) {
					QMessageBox::information(this, tr("No items available"), tr("There are no items available to choose from."));
					return;
				}

				if (dropWindow.exec()) {
					EquipmentPiece *item;
					QString dropString;
					if (type == "Weapon") {
						item = _project->getProperties()->getEquipment()->getWeapon(itemSelection->currentText());
						dropString = QString("Weapon: ");
					} else if (type == "Armor") {
						item = _project->getProperties()->getEquipment()->getArmor(itemSelection->currentText());
						dropString = QString("Armor: ");
					} else if (type == "Accessory") {
						item = _project->getProperties()->getEquipment()->getAccessory(itemSelection->currentText());
						dropString = QString("Accessory: ");
					}

					try {
						enemy->getDrops()->addItem(item, chanceSelection->value());
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_dropList->clear();
					QVector<QPair<EquipmentPiece*, int>> drops = enemy->getDrops()->getItems();
					for (int i =0; i < drops.size(); i++) {
						QPair<EquipmentPiece*, int> it = drops.at(i);
						_dropList->addItem(it.first->getName() + " - chance " + QString::number(it.second) + "%");
					}
				}
			}
		}
	}
}

void EnemiesWindow::editDrop() {
	if ((_enemySelection->count() > 0) && (_dropList->count() > 0) && !!_dropList->currentItem()) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			int currentIndex = _dropList->currentRow();
			const QVector<QPair<EquipmentPiece*, int>>& constDrops = enemy->getDrops()->getItems();
			QPair<EquipmentPiece*, int> drop = constDrops.at(currentIndex);

			bool accepted;
			int newValue = QInputDialog::getInt(0, "Edit drop", "Edit the dropchance of the item " + drop.first->getName() + ":", drop.second, 1, 100, drop.second, &accepted);

			if (accepted) {

				enemy->getDrops()->editItem(currentIndex, newValue);

				_dropList->clear();
				QVector<QPair<EquipmentPiece*, int>> drops = enemy->getDrops()->getItems();
				for (int i =0; i < drops.size(); i++) {
					QPair<EquipmentPiece*, int> it = drops.at(i);
					_dropList->addItem(it.first->getName() + " - chance " + QString::number(it.second) + "%");
				}
			}
		}
	}
}

void EnemiesWindow::deleteDrop() {
	if ((_enemySelection->count() > 0) && (_dropList->count() > 0) && !!_dropList->currentItem()) {
		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {
			int currentIndex = _dropList->currentRow();
			enemy->getDrops()->removeItem(currentIndex);
			_dropList->takeItem(currentIndex);
		}
	}
}

void EnemiesWindow::moveDropUp() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_dropList->count() > 0) && !!_dropList->currentItem() && (_dropList->currentRow() != 0)) {

		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			QPair<EquipmentPiece*, int> drop = enemy->getDrops()->getItems().at(_dropList->currentRow());
			if (!!drop.first) {
				const QVector<QPair<EquipmentPiece*, int>>& drops = enemy->getDrops()->getItems();
				int index = _dropList->currentRow();

				try {
					switchPairs(const_cast<QVector<QPair<EquipmentPiece*, int>>&>(drops), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int enemyIndex = _enemySelection->currentRow();
				loadEnemies();
				_enemySelection->setCurrentRow(enemyIndex);
				_dropList->setCurrentRow(index-1);
			}
		}
	}
}

void EnemiesWindow::moveDropDown() {
	if ((_enemySelection->count() > 0) && !!_enemySelection->currentItem() && (_dropList->count() > 0) && !!_dropList->currentItem() && (_dropList->currentRow() != _dropList->count()-1)) {

		Enemy *enemy = _project->getEnemies()->getEnemy(_enemySelection->currentItem()->text());
		if (!!enemy) {

			QPair<EquipmentPiece*, int> drop = enemy->getDrops()->getItems().at(_dropList->currentRow());
			if (!!drop.first) {
				const QVector<QPair<EquipmentPiece*, int>>& drops = enemy->getDrops()->getItems();
				int index = _dropList->currentRow();

				try {
					switchPairs(const_cast<QVector<QPair<EquipmentPiece*, int>>&>(drops), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int enemyIndex = _enemySelection->currentRow();
				loadEnemies();
				_enemySelection->setCurrentRow(enemyIndex);
				_dropList->setCurrentRow(index+1);
			}
		}
	}
}