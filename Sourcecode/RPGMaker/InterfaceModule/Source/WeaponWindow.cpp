#include "qspinbox.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/WeaponWindow.h"
#include "../Components/Headers/Class.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Attribute.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;
using namespace projectcomponents;

WeaponWindow::WeaponWindow(QWidget *parent, Properties *properties)
	: QFrame(parent), _properties(properties) {
		_layout = new QHBoxLayout(this);
		createWindow();
		loadWeapons();
}

WeaponWindow::~WeaponWindow() {}

void WeaponWindow::createWindow() {
	// Weapon Selection
	QGroupBox *selection = new QGroupBox("Weapons", this);
	QVBoxLayout *selectionLayout = new QVBoxLayout(selection);

	QLabel *selectionLabel = new QLabel("Select a weapon:", selection);
	_weaponList = new QListWidget(selection);
	QFrame *selectionButtonFrame = new QFrame(selection);
	QHBoxLayout *selectionButtonLayout = new QHBoxLayout(selectionButtonFrame);
	selectionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *newWeapon = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", selectionButtonFrame);
	newWeapon->setToolTip("Add a new weapon");
	newWeapon->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editWeapon = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", selectionButtonFrame);
	editWeapon->setToolTip("Edit the current weapon");
	editWeapon->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteWeapon = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", selectionButtonFrame);
	deleteWeapon->setToolTip("Delete the current weapon");
	deleteWeapon->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *weaponUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", selectionButtonFrame);
	weaponUpButton->setToolTip("Move selected weapon up");
	weaponUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *weaponDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", selectionButtonFrame);
	weaponDownButton->setToolTip("Move selected weapon down");
	weaponDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	selectionButtonLayout->addWidget(newWeapon);
	selectionButtonLayout->addWidget(editWeapon);
	selectionButtonLayout->addWidget(deleteWeapon);
	selectionButtonLayout->addSpacing(20);
	selectionButtonLayout->addWidget(weaponUpButton);
	selectionButtonLayout->addWidget(weaponDownButton);
	selectionLayout->addWidget(selectionLabel);
	selectionLayout->addWidget(_weaponList);
	selectionLayout->addWidget(selectionButtonFrame);

	// Description
	QGroupBox *description = new QGroupBox("Description", this);
	QVBoxLayout *descriptionLayout = new QVBoxLayout(description);

	_weaponDescLabel = new QLabel(description);
	_weaponDescription = new QPlainTextEdit(description);
	QFrame *descriptionButtonFrame = new QFrame(description);
	QHBoxLayout *descriptionButtonLayout = new QHBoxLayout(descriptionButtonFrame);
	descriptionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveDescription = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", descriptionButtonFrame);
	saveDescription->setToolTip("Save the description for the current weapon");
	saveDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadDescription = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", descriptionButtonFrame);
	reloadDescription->setToolTip("Reload the original description for the current weapon");
	reloadDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	descriptionButtonLayout->addWidget(saveDescription);
	descriptionButtonLayout->addWidget(reloadDescription);
	descriptionLayout->addWidget(_weaponDescLabel);
	descriptionLayout->addWidget(_weaponDescription);
	descriptionLayout->addWidget(descriptionButtonFrame);

	// Attributes
	QGroupBox *attributes = new QGroupBox("Attributes", this);
	QVBoxLayout *attributesLayout = new QVBoxLayout(attributes);

	_weaponAttLabel = new QLabel(attributes);
	_weaponAttributes = new QListWidget(attributes);
	QFrame *attributesButtonFrame = new QFrame(attributes);
	QHBoxLayout *attributesButtonLayout = new QHBoxLayout(attributesButtonFrame);
	attributesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addAttribute = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", attributesButtonFrame);
	addAttribute->setToolTip("Add a new attribute to the current weapon");
	addAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAttribute = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", attributesButtonFrame);
	editAttribute->setToolTip("Edit the current attribute");
	editAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAttribute = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", attributesButtonFrame);
	deleteAttribute->setToolTip("Delete the attribute from current weapon");
	deleteAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *attributeUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", attributesButtonFrame);
	attributeUpButton->setToolTip("Move selected attribute up");
	attributeUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *attributeDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", attributesButtonFrame);
	attributeDownButton->setToolTip("Move selected attribute down");
	attributeDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	attributesButtonLayout->addWidget(addAttribute);
	attributesButtonLayout->addWidget(editAttribute);
	attributesButtonLayout->addWidget(deleteAttribute);
	attributesButtonLayout->addSpacing(20);
	attributesButtonLayout->addWidget(attributeUpButton);
	attributesButtonLayout->addWidget(attributeDownButton);
	attributesLayout->addWidget(_weaponAttLabel);
	attributesLayout->addWidget(_weaponAttributes);
	attributesLayout->addWidget(attributesButtonFrame);

	// Classes
	QGroupBox *classes = new QGroupBox("Classes", this);
	QVBoxLayout *classesLayout = new QVBoxLayout(classes);

	_weaponClassLabel = new QLabel(classes);
	_weaponClasses = new QListWidget(classes);
	QFrame *classesButtonFrame = new QFrame(attributes);
	QHBoxLayout *classesButtonLayout = new QHBoxLayout(classesButtonFrame);
	classesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addClass = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", classesButtonFrame);
	addClass->setToolTip("Add a new class to the current weapon");
	addClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editClass = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", classesButtonFrame);
	editClass->setToolTip("Edit the current class");
	editClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteClass = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", classesButtonFrame);
	deleteClass->setToolTip("Delete the class from current weapon");
	deleteClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *classUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", classesButtonFrame);
	classUpButton->setToolTip("Move selected class up");
	classUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *classDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", classesButtonFrame);
	classDownButton->setToolTip("Move selected class down");
	classDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	classesButtonLayout->addWidget(addClass);
	classesButtonLayout->addWidget(editClass);
	classesButtonLayout->addWidget(deleteClass);
	classesButtonLayout->addSpacing(20);
	classesButtonLayout->addWidget(classUpButton);
	classesButtonLayout->addWidget(classDownButton);
	classesLayout->addWidget(_weaponClassLabel);
	classesLayout->addWidget(_weaponClasses);
	classesLayout->addWidget(classesButtonFrame);

	// Elements
	QGroupBox *elements = new QGroupBox("Elements", this);
	QVBoxLayout *elementsLayout = new QVBoxLayout(elements);

	_weaponElemLabel = new QLabel(elements);
	_weaponElements = new QListWidget(elements);
	QFrame *elementsButtonFrame = new QFrame(elements);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(elementsButtonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addElement = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", elementsButtonFrame);
	addElement->setToolTip("Add an element to the current weapon");
	addElement->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteElement = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", elementsButtonFrame);
	deleteElement->setToolTip("Delete the element from the current weapon");
	deleteElement->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", elementsButtonFrame);
	elementUpButton->setToolTip("Move selected element up");
	elementUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", elementsButtonFrame);
	elementDownButton->setToolTip("Move selected element down");
	elementDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	elementsButtonLayout->addWidget(addElement);
	elementsButtonLayout->addWidget(deleteElement);
	elementsButtonLayout->addSpacing(20);
	elementsButtonLayout->addWidget(elementUpButton);
	elementsButtonLayout->addWidget(elementDownButton);
	elementsLayout->addWidget(_weaponElemLabel);
	elementsLayout->addWidget(_weaponElements);
	elementsLayout->addWidget(elementsButtonFrame);

	_layout->addWidget(selection);
	_layout->addWidget(description);
	_layout->addWidget(attributes);
	_layout->addWidget(classes);
	_layout->addWidget(elements);

	connect(_weaponList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(weaponSelected(const QString &)));
	connect(newWeapon, SIGNAL(clicked()), this, SLOT(newWeapon()));
	connect(editWeapon, SIGNAL(clicked()), this, SLOT(editWeapon()));
	connect(deleteWeapon, SIGNAL(clicked()), this, SLOT(deleteWeapon()));
	connect(weaponUpButton, SIGNAL(clicked()), this, SLOT(moveWeaponUp()));
	connect(weaponDownButton, SIGNAL(clicked()), this, SLOT(moveWeaponDown()));
	connect(saveDescription, SIGNAL(clicked()), this, SLOT(saveDescription()));
	connect(reloadDescription, SIGNAL(clicked()), this, SLOT(reloadDescription()));
	connect(addAttribute, SIGNAL(clicked()), this, SLOT(addAttribute()));
	connect(editAttribute, SIGNAL(clicked()), this, SLOT(editAttribute()));
	connect(deleteAttribute, SIGNAL(clicked()), this, SLOT(deleteAttribute()));
	connect(attributeUpButton, SIGNAL(clicked()), this, SLOT(moveAttributeUp()));
	connect(attributeDownButton, SIGNAL(clicked()), this, SLOT(moveAttributeDown()));
	connect(addClass, SIGNAL(clicked()), this, SLOT(addClass()));
	connect(editClass, SIGNAL(clicked()), this, SLOT(editClass()));
	connect(deleteClass, SIGNAL(clicked()), this, SLOT(deleteClass()));
	connect(classUpButton, SIGNAL(clicked()), this, SLOT(moveClassUp()));
	connect(classDownButton, SIGNAL(clicked()), this, SLOT(moveClassDown()));
	connect(addElement, SIGNAL(clicked()), this, SLOT(addElement()));
	connect(deleteElement, SIGNAL(clicked()), this, SLOT(deleteElement()));
	connect(elementUpButton, SIGNAL(clicked()), this, SLOT(moveElementUp()));
	connect(elementDownButton, SIGNAL(clicked()), this, SLOT(moveElementDown()));
}

void WeaponWindow::loadWeapons() {
	_weaponList->clear();
	QVector<Weapon*> weapons = _properties->getEquipment()->getWeapons();
	for (int i = 0; i < weapons.size(); i++) {
		Weapon *weapon = weapons.at(i);
		_weaponList->addItem(weapon->getName());
	}

	if (_weaponList->count() > 0)
		_weaponList->setCurrentRow(0);
}

void WeaponWindow::updateWindow() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon)
			weaponSelected(weapon->getName());
	}
}

void WeaponWindow::weaponSelected(const QString &name) {
	if (name != QString()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(name);
		if (!!weapon) {
			_weaponDescLabel->clear();
			_weaponDescLabel->setText("Description of " + weapon->getName());
			_weaponDescription->clear();
			_weaponDescription->setPlainText(weapon->getDescription());

			_weaponAttLabel->clear();
			_weaponAttLabel->setText("Attributes of " + weapon->getName());
			_weaponAttributes->clear();
			QVector<QPair<Attribute*, int>> attributes = weapon->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				QPair<Attribute*, int> it = attributes.at(i);
				_weaponAttributes->addItem(it.first->getName() + " - +" + QString::number(it.second));
			}

			_weaponClassLabel->clear();
			_weaponClassLabel->setText("Classes of " + weapon->getName());
			_weaponClasses->clear();
			QVector<QPair<Class*, int>> classes = weapon->getClasses();
			for (int i = 0; i < classes.size(); i++) {
				QPair<Class*, int> it = classes.at(i);
				_weaponClasses->addItem(it.first->getName() + " - level " + QString::number(it.second));
			}

			_weaponElemLabel->clear();
			_weaponElemLabel->setText("Elements of " + weapon->getName());
			_weaponElements->clear();
			QVector<Element*> elements = weapon->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				_weaponElements->addItem(it->getName());
			}
		}
	}
}

void WeaponWindow::newWeapon() {
	// Setup window
	QDialog weaponWindow(0);
	weaponWindow.setWindowTitle("Add a new weapon");
	QVBoxLayout *layout = new QVBoxLayout(&weaponWindow);

	QLabel *label = new QLabel("Enter a name and select a type:", &weaponWindow);

	QFrame *setupFrame = new QFrame(&weaponWindow);
	QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
	QLineEdit *nameFrame = new QLineEdit(setupFrame);
	QComboBox *typeSelection = new QComboBox(setupFrame);
	setupLayout->addWidget(nameFrame);
	setupLayout->addSpacing(20);
	setupLayout->addWidget(typeSelection);

	QFrame *buttonframe = new QFrame(&weaponWindow);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
	QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
	buttonlayout->addWidget(acceptbutton);
	buttonlayout->addWidget(cancelbutton);

	layout->addWidget(label);
	layout->addWidget(setupFrame);
	layout->addWidget(buttonframe);

	connect(acceptbutton, SIGNAL(clicked()), &weaponWindow, SLOT(accept()));
	connect(cancelbutton, SIGNAL(clicked()), &weaponWindow, SLOT(reject()));

	QStringList types;
	types << "Mainhand" << "Offhand" << "Twohanded";
	typeSelection->addItems(types);

	if (weaponWindow.exec()) {
		QString name = nameFrame->text();
		QString typeString = typeSelection->currentText();

		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newWeapon();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("A weapon should not have an empty name."));
			newWeapon();
			return;
		}

		if (!!_properties->getEquipment()->getWeapon(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("A weapon with the same name already exists."));
			newWeapon();
			return;
		}

		Weapon::Subtype type;
		if (typeString == QString("Mainhand"))
			type = Weapon::MAINHAND;
		else if (typeString == QString("Offhand"))
			type = Weapon::OFFHAND;
		else if (typeString == QString("Twohanded"))
			type = Weapon::TWOHANDED;
		else {
			QMessageBox::critical(this, tr("Unexpected error occurred"), tr("The selected type could not be matched."));
			return;
		}

		Weapon *weapon = Weapon::create(name, type);
		try {
			_properties->getEquipment()->addWeapon(weapon);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete weapon;
			return;
		}

		_weaponList->addItem(weapon->getName());
		_weaponList->setCurrentRow(_weaponList->count()-1);
		emit updateSignal();
	}
}

void WeaponWindow::editWeapon() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Edit weapon"),
				tr("Enter a new name for the weapon:"), QLineEdit::Normal, weapon->getName(), &accepted);

			if (accepted && (name != weapon->getName())) {
				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editWeapon();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("A weapon should not have an empty name"));
					editWeapon();
					return;
				}

				if (!!_properties->getEquipment()->getWeapon(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("A weapon with the same name already exists."));
					editWeapon();
					return;
				}

				weapon->setName(name);
				_weaponList->currentItem()->setText(weapon->getName());
				emit updateSignal();
			}
		}
	}
}

void WeaponWindow::deleteWeapon() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		QString name = _weaponList->currentItem()->text();
		Weapon *weapon = _properties->getEquipment()->getWeapon(name);
		if (!!weapon) {
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete weapon",
				"Are you sure you want to delete the weapon?\nDeleting a weapon will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {
				_properties->getEquipment()->removeWeapon(name);
				loadWeapons();

				if (_weaponList->count() > 0)
					_weaponList->setCurrentRow(0);

				emit updateSignal();
			}
		}
	}
}

void WeaponWindow::moveWeaponUp() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponList->currentRow() != 0)) {

		Weapon *currentWeapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!currentWeapon) {
			const QVector<Weapon*>& weapons = _properties->getEquipment()->getWeapons();
			int index = weapons.indexOf(currentWeapon);

			try {
				switchPlaces(const_cast<QVector<Weapon*>&>(weapons), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadWeapons();
			_weaponList->setCurrentRow(index-1);
		}
	}
}

void WeaponWindow::moveWeaponDown() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponList->currentRow() != _weaponList->count()-1)) {

		Weapon *currentWeapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!currentWeapon) {
			const QVector<Weapon*>& weapons = _properties->getEquipment()->getWeapons();
			int index = weapons.indexOf(currentWeapon);

			try {
				switchPlaces(const_cast<QVector<Weapon*>&>(weapons), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadWeapons();
			_weaponList->setCurrentRow(index+1);
		}
	}
}

void WeaponWindow::saveDescription() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			weapon->setDescription(_weaponDescription->toPlainText());
		}
	}
}

void WeaponWindow::reloadDescription() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			_weaponDescription->setPlainText(weapon->getDescription());
		}
	}
}

void WeaponWindow::addElement() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			// Setup window
			QDialog elementWindow(0);
			elementWindow.setWindowTitle("Add element to " + weapon->getName());
			QVBoxLayout *layout = new QVBoxLayout(&elementWindow);

			QLabel *label = new QLabel("Select an element:", &elementWindow);
			QComboBox *elementSelection = new QComboBox(&elementWindow);
			elementSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QFrame *buttonFrame = new QFrame(&elementWindow);
			QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
			QPushButton *acceptButton = new QPushButton("Accept", buttonFrame);
			QPushButton *cancelButton = new QPushButton("Cancel", buttonFrame);
			buttonLayout->addWidget(acceptButton);
			buttonLayout->addWidget(cancelButton);

			layout->addWidget(label);
			layout->addWidget(elementSelection);
			layout->addWidget(buttonFrame);

			connect(acceptButton, SIGNAL(clicked()), &elementWindow, SLOT(accept()));
			connect(cancelButton, SIGNAL(clicked()), &elementWindow, SLOT(reject()));

			QVector<Element*> weaponElements = weapon->getElements();
			QVector<Element*> elements = _properties->getElements()->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				if (!weaponElements.contains(it))
					elementSelection->addItem(it->getName());
			}

			if (elementSelection->count() == 0) {
				QMessageBox::information(this, tr("No elements available"), tr("There are no more elements available to choose from."));
				return;
			}

			if (elementWindow.exec()) {
				Element *element = _properties->getElements()->getElement(elementSelection->currentText());
				if (!!element) {
					try {
						weapon->addElement(element);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_weaponElements->addItem(element->getName());
				}
			}
		}
	}
}

void WeaponWindow::deleteElement() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponElements->count() > 0) && (!!_weaponElements->currentItem())) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			weapon->removeElement(_weaponElements->currentItem()->text());
			_weaponElements->takeItem(_weaponElements->currentRow());
		}
	}
}

void WeaponWindow::moveElementUp() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponElements->count() > 0) && !!_weaponElements->currentItem() && (_weaponElements->currentRow() != 0)) {

		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			Element *currentElement = weapon->getElements().at(_weaponElements->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = weapon->getElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int weaponIndex = _weaponList->currentRow();
				loadWeapons();
				_weaponList->setCurrentRow(weaponIndex);
				_weaponElements->setCurrentRow(index-1);
			}
		}
	}
}

void WeaponWindow::moveElementDown() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponElements->count() > 0) && !!_weaponElements->currentItem() && (_weaponElements->currentRow() != _weaponElements->count()-1)) {

		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			Element *currentElement = weapon->getElements().at(_weaponElements->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = weapon->getElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int weaponIndex = _weaponList->currentRow();
				loadWeapons();
				_weaponList->setCurrentRow(weaponIndex);
				_weaponElements->setCurrentRow(index+1);
			}
		}
	}
}

void WeaponWindow::addAttribute() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Add attribute to " + weapon->getName());
			QVBoxLayout *layout = new QVBoxLayout(&attributeWindow);

			QLabel *label = new QLabel("Select an attribute and set a value:", &attributeWindow);

			QFrame *setupFrame = new QFrame(&attributeWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QComboBox *attributeSelection = new QComboBox(setupFrame);
			attributeSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QLabel *attributeLabel = new QLabel(" + ", setupFrame);
			QSpinBox *valueSelection = new QSpinBox(setupFrame);
			valueSelection->setMinimum(0);
			valueSelection->setMaximum(255);
			valueSelection->setSingleStep(1);
			valueSelection->setValue(1);
			setupLayout->addWidget(attributeSelection);
			setupLayout->addWidget(attributeLabel);
			setupLayout->addWidget(valueSelection);

			QFrame *buttonframe = new QFrame(&attributeWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &attributeWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &attributeWindow, SLOT(reject()));

			QVector<QPair<Attribute*, int>> weaponAttributes = weapon->getAttributes();
			QVector<Attribute*> attributes = _properties->getAttributes()->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				Attribute *it = attributes.at(i);
				for (int j = 0; j < weaponAttributes.size(); j++) {
					if (weaponAttributes.at(j).first == it)
						goto skip;
				}

				attributeSelection->addItem(it->getName());

				skip:;
			}

			if (attributeSelection->count() == 0) {
				QMessageBox::information(this, tr("No attributes available"), tr("There are no more attributes available to choose from."));
				return;
			}

			if (attributeWindow.exec()) {
				Attribute *attribute = _properties->getAttributes()->getAttribute(attributeSelection->currentText());
				int value = valueSelection->value();

				if (!!attribute) {
					try {
						weapon->addAttribute(attribute, value);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_weaponAttributes->addItem(attribute->getName() + " - +" + QString::number(value));
				} 
			}
		}
	}
}

void WeaponWindow::editAttribute() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponAttributes->count() > 0) && !!_weaponAttributes->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			QPair<Attribute*, int> attribute = weapon->getAttribute(_weaponAttributes->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Edit attribute of " + weapon->getName());
			QVBoxLayout *layout = new QVBoxLayout(&attributeWindow);

			QLabel *label = new QLabel("Edit the value for attribute " + attribute.first->getName() + ":", &attributeWindow);

			QFrame *setupFrame = new QFrame(&attributeWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QLabel *attributeLabel = new QLabel(attribute.first->getName() + " + ", setupFrame);
			QSpinBox *valueSelection = new QSpinBox(setupFrame);
			valueSelection->setMinimum(0);
			valueSelection->setMaximum(255);
			valueSelection->setSingleStep(1);
			valueSelection->setValue(attribute.second);
			setupLayout->addWidget(attributeLabel);
			setupLayout->addWidget(valueSelection);

			QFrame *buttonframe = new QFrame(&attributeWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &attributeWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &attributeWindow, SLOT(reject()));

			if (attributeWindow.exec()) {
				weapon->editAttribute(attribute.first->getName(), valueSelection->value());
				_weaponAttributes->currentItem()->setText(attribute.first->getName() + " - +" + QString::number(valueSelection->value()));
			}
		}
	}
}

void WeaponWindow::deleteAttribute() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponAttributes->count() > 0) && !!_weaponAttributes->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			weapon->removeAttribute(_weaponAttributes->currentItem()->text().split(" - ").at(0));
			_weaponAttributes->takeItem(_weaponAttributes->currentRow());
		}
	}
}

void WeaponWindow::moveAttributeUp() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponAttributes->count() > 0) && !!_weaponAttributes->currentItem() && (_weaponAttributes->currentRow() != 0)) {

		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			QPair<Attribute*, int> attribute = weapon->getAttributes().at(_weaponAttributes->currentRow());
			if (!!attribute.first) {
				const QVector<QPair<Attribute*, int>>& attributes = weapon->getAttributes();
				int index = attributes.indexOf(attribute);

				try {
					switchPairs(const_cast<QVector<QPair<Attribute*, int>>&>(attributes), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int weaponIndex = _weaponList->currentRow();
				loadWeapons();
				_weaponList->setCurrentRow(weaponIndex);
				_weaponAttributes->setCurrentRow(index-1);
			}
		}
	}
}

void WeaponWindow::moveAttributeDown() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponAttributes->count() > 0) && !!_weaponAttributes->currentItem() && (_weaponAttributes->currentRow() != _weaponAttributes->count()-1)) {

		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			QPair<Attribute*, int> attribute = weapon->getAttributes().at(_weaponAttributes->currentRow());
			if (!!attribute.first) {
				const QVector<QPair<Attribute*, int>>& attributes = weapon->getAttributes();
				int index = attributes.indexOf(attribute);

				try {
					switchPairs(const_cast<QVector<QPair<Attribute*, int>>&>(attributes), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int weaponIndex = _weaponList->currentRow();
				loadWeapons();
				_weaponList->setCurrentRow(weaponIndex);
				_weaponAttributes->setCurrentRow(index+1);
			}
		}
	}
}

void WeaponWindow::addClass() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			// Setup window
			QDialog classWindow(0);
			classWindow.setWindowTitle("Add class to " + weapon->getName());
			QVBoxLayout *layout = new QVBoxLayout(&classWindow);

			QLabel *label = new QLabel("Select a class and set a level requirement:", &classWindow);

			QFrame *setupFrame = new QFrame(&classWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QComboBox *classSelection = new QComboBox(setupFrame);
			classSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QLabel *classLabel = new QLabel(" at level ", setupFrame);
			QSpinBox *levelSelection = new QSpinBox(setupFrame);
			levelSelection->setMinimum(0);
			levelSelection->setMaximum(99);
			levelSelection->setSingleStep(1);
			levelSelection->setValue(1);
			setupLayout->addWidget(classSelection);
			setupLayout->addWidget(classLabel);
			setupLayout->addWidget(levelSelection);

			QFrame *buttonframe = new QFrame(&classWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &classWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &classWindow, SLOT(reject()));

			QVector<QPair<Class*, int>> weaponClasses = weapon->getClasses();
			QVector<Class*> classes = _properties->getClasses()->getClasses();
			for (int i = 0; i < classes.size(); i++) {
				Class *it = classes.at(i);
				for (int j = 0; j < weaponClasses.size(); j++) {
					if (weaponClasses.at(j).first == it)
						goto skip;
				}

				classSelection->addItem(it->getName());

			skip:;
			}

			if (classSelection->count() == 0) {
				QMessageBox::information(this, tr("No classes available"), tr("There are no more classes available to choose from."));
				return;
			}

			if (classWindow.exec()) {
				Class *cat = _properties->getClasses()->getClass(classSelection->currentText());
				int level = levelSelection->value();

				if (!!cat) {
					try {
						weapon->addClass(cat, level);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_weaponClasses->addItem(cat->getName() + " - level " + QString::number(level));
				} 
			}
		}
	}
}

void WeaponWindow::editClass() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponClasses->count() > 0) && !!_weaponClasses->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			QPair<Class*, int> cat = weapon->getClass(_weaponClasses->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog classWindow(0);
			classWindow.setWindowTitle("Edit class of " + weapon->getName());
			QVBoxLayout *layout = new QVBoxLayout(&classWindow);

			QLabel *label = new QLabel("Edit the level requirement for class " + cat.first->getName(), &classWindow);

			QFrame *setupFrame = new QFrame(&classWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QLabel *classLabel = new QLabel(cat.first->getName() + " at level ", setupFrame);
			QSpinBox *levelSelection = new QSpinBox(setupFrame);
			levelSelection->setMinimum(0);
			levelSelection->setMaximum(99);
			levelSelection->setSingleStep(1);
			levelSelection->setValue(cat.second);
			setupLayout->addWidget(classLabel);
			setupLayout->addWidget(levelSelection);

			QFrame *buttonframe = new QFrame(&classWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &classWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &classWindow, SLOT(reject()));

			if (classWindow.exec()) {
				weapon->editClass(cat.first->getName(), levelSelection->value());
				_weaponClasses->currentItem()->setText(cat.first->getName() + " - level " + QString::number(levelSelection->value()));
			}
		}
	}
}

void WeaponWindow::deleteClass() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponClasses->count() > 0) && !!_weaponClasses->currentItem()) {
		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {
			weapon->removeClass(_weaponClasses->currentItem()->text().split(" - ").at(0));
			_weaponClasses->takeItem(_weaponClasses->currentRow());
		}
	}
}

void WeaponWindow::moveClassUp() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponClasses->count() > 0) && !!_weaponClasses->currentItem() && (_weaponClasses->currentRow() != 0)) {

		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			QPair<Class*, int> cat = weapon->getClasses().at(_weaponClasses->currentRow());
			if (!!cat.first) {
				const QVector<QPair<Class*, int>>& classes = weapon->getClasses();
				int index = classes.indexOf(cat);

				try {
					switchPairs(const_cast<QVector<QPair<Class*, int>>&>(classes), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int weaponIndex = _weaponList->currentRow();
				loadWeapons();
				_weaponList->setCurrentRow(weaponIndex);
				_weaponClasses->setCurrentRow(index-1);
			}
		}
	}
}

void WeaponWindow::moveClassDown() {
	if ((_weaponList->count() > 0) && !!_weaponList->currentItem() && (_weaponClasses->count() > 0) && !!_weaponClasses->currentItem() && (_weaponClasses->currentRow() != _weaponClasses->count()-1)) {

		Weapon *weapon = _properties->getEquipment()->getWeapon(_weaponList->currentItem()->text());
		if (!!weapon) {

			QPair<Class*, int> cat = weapon->getClasses().at(_weaponClasses->currentRow());
			if (!!cat.first) {
				const QVector<QPair<Class*, int>>& classes = weapon->getClasses();
				int index = classes.indexOf(cat);

				try {
					switchPairs(const_cast<QVector<QPair<Class*, int>>&>(classes), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int weaponIndex = _weaponList->currentRow();
				loadWeapons();
				_weaponList->setCurrentRow(weaponIndex);
				_weaponClasses->setCurrentRow(index+1);
			}
		}
	}
}