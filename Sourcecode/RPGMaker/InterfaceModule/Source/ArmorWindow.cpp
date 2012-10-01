#include "qspinbox.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/ArmorWindow.h"
#include "../Components/Headers/Class.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Attribute.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;
using namespace projectcomponents;

ArmorWindow::ArmorWindow(QWidget *parent, Properties *properties)
	: QFrame(parent), _properties(properties) {
		_layout = new QHBoxLayout(this);
		createWindow();
		loadArmor();
}

ArmorWindow::~ArmorWindow() {}

void ArmorWindow::createWindow() {
	// Armor Selection
	QGroupBox *selection = new QGroupBox("Armors", this);
	QVBoxLayout *selectionLayout = new QVBoxLayout(selection);

	QLabel *selectionLabel = new QLabel("Select a armor:", selection);
	_armorList = new QListWidget(selection);
	QFrame *selectionButtonFrame = new QFrame(selection);
	QHBoxLayout *selectionButtonLayout = new QHBoxLayout(selectionButtonFrame);
	selectionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *newArmor = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", selectionButtonFrame);
	newArmor->setToolTip("Add a new armor");
	newArmor->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editArmor = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", selectionButtonFrame);
	editArmor->setToolTip("Edit the current armor");
	editArmor->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteArmor = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", selectionButtonFrame);
	deleteArmor->setToolTip("Delete the current armor");
	deleteArmor->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *armorUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", selectionButtonFrame);
	armorUpButton->setToolTip("Move selected armor up");
	armorUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *armorDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", selectionButtonFrame);
	armorDownButton->setToolTip("Move selected armor down");
	armorDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	selectionButtonLayout->addWidget(newArmor);
	selectionButtonLayout->addWidget(editArmor);
	selectionButtonLayout->addWidget(deleteArmor);
	selectionButtonLayout->addSpacing(20);
	selectionButtonLayout->addWidget(armorUpButton);
	selectionButtonLayout->addWidget(armorDownButton);
	selectionLayout->addWidget(selectionLabel);
	selectionLayout->addWidget(_armorList);
	selectionLayout->addWidget(selectionButtonFrame);

	// Description
	QGroupBox *description = new QGroupBox("Description", this);
	QVBoxLayout *descriptionLayout = new QVBoxLayout(description);

	_armorDescLabel = new QLabel(description);
	_armorDescription = new QPlainTextEdit(description);
	QFrame *descriptionButtonFrame = new QFrame(description);
	QHBoxLayout *descriptionButtonLayout = new QHBoxLayout(descriptionButtonFrame);
	descriptionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveDescription = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", descriptionButtonFrame);
	saveDescription->setToolTip("Save the description for the current armor");
	saveDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadDescription = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", descriptionButtonFrame);
	reloadDescription->setToolTip("Reload the original description for the current armor");
	reloadDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	descriptionButtonLayout->addWidget(saveDescription);
	descriptionButtonLayout->addWidget(reloadDescription);
	descriptionLayout->addWidget(_armorDescLabel);
	descriptionLayout->addWidget(_armorDescription);
	descriptionLayout->addWidget(descriptionButtonFrame);

	// Attributes
	QGroupBox *attributes = new QGroupBox("Attributes", this);
	QVBoxLayout *attributesLayout = new QVBoxLayout(attributes);

	_armorAttLabel = new QLabel(attributes);
	_armorAttributes = new QListWidget(attributes);
	QFrame *attributesButtonFrame = new QFrame(attributes);
	QHBoxLayout *attributesButtonLayout = new QHBoxLayout(attributesButtonFrame);
	attributesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addAttribute = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", attributesButtonFrame);
	addAttribute->setToolTip("Add a new attribute to the current armor");
	addAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAttribute = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", attributesButtonFrame);
	editAttribute->setToolTip("Edit the current attribute");
	editAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAttribute = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", attributesButtonFrame);
	deleteAttribute->setToolTip("Delete the attribute from current armor");
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
	attributesLayout->addWidget(_armorAttLabel);
	attributesLayout->addWidget(_armorAttributes);
	attributesLayout->addWidget(attributesButtonFrame);

	// Classes
	QGroupBox *classes = new QGroupBox("Classes", this);
	QVBoxLayout *classesLayout = new QVBoxLayout(classes);

	_armorClassLabel = new QLabel(classes);
	_armorClasses = new QListWidget(classes);
	QFrame *classesButtonFrame = new QFrame(attributes);
	QHBoxLayout *classesButtonLayout = new QHBoxLayout(classesButtonFrame);
	classesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addClass = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", classesButtonFrame);
	addClass->setToolTip("Add a new class to the current armor");
	addClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editClass = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", classesButtonFrame);
	editClass->setToolTip("Edit the current class");
	editClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteClass = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", classesButtonFrame);
	deleteClass->setToolTip("Delete the class from current armor");
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
	classesLayout->addWidget(_armorClassLabel);
	classesLayout->addWidget(_armorClasses);
	classesLayout->addWidget(classesButtonFrame);

	// Elements
	QGroupBox *elements = new QGroupBox("Elements", this);
	QVBoxLayout *elementsLayout = new QVBoxLayout(elements);

	_armorElemLabel = new QLabel(elements);
	_armorElements = new QListWidget(elements);
	QFrame *elementsButtonFrame = new QFrame(elements);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(elementsButtonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addElement = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", elementsButtonFrame);
	addElement->setToolTip("Add an element to the current armor");
	addElement->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteElement = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", elementsButtonFrame);
	deleteElement->setToolTip("Delete the element from the current armor");
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
	elementsLayout->addWidget(_armorElemLabel);
	elementsLayout->addWidget(_armorElements);
	elementsLayout->addWidget(elementsButtonFrame);

	_layout->addWidget(selection);
	_layout->addWidget(description);
	_layout->addWidget(attributes);
	_layout->addWidget(classes);
	_layout->addWidget(elements);

	connect(_armorList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(armorSelected(const QString &)));
	connect(newArmor, SIGNAL(clicked()), this, SLOT(newArmor()));
	connect(editArmor, SIGNAL(clicked()), this, SLOT(editArmor()));
	connect(deleteArmor, SIGNAL(clicked()), this, SLOT(deleteArmor()));
	connect(armorUpButton, SIGNAL(clicked()), this, SLOT(moveArmorUp()));
	connect(armorDownButton, SIGNAL(clicked()), this, SLOT(moveArmorDown()));
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

void ArmorWindow::loadArmor() {
	_armorList->clear();
	QVector<Armor*> armors = _properties->getEquipment()->getArmor();
	for (int i = 0; i < armors.size(); i++) {
		Armor *armor = armors.at(i);
		_armorList->addItem(armor->getName());
	}

	if (_armorList->count() > 0)
		_armorList->setCurrentRow(0);
}

void ArmorWindow::updateWindow() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor)
			armorSelected(armor->getName());
	}
}

void ArmorWindow::armorSelected(const QString &name) {
	if (name != QString()) {
		Armor *armor = _properties->getEquipment()->getArmor(name);
		if (!!armor) {
			_armorDescLabel->clear();
			_armorDescLabel->setText("Description of " + armor->getName());
			_armorDescription->clear();
			_armorDescription->setPlainText(armor->getDescription());

			_armorAttLabel->clear();
			_armorAttLabel->setText("Attributes of " + armor->getName());
			_armorAttributes->clear();
			QVector<QPair<Attribute*, int>> attributes = armor->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				QPair<Attribute*, int> it = attributes.at(i);
				_armorAttributes->addItem(it.first->getName() + " - +" + QString::number(it.second));
			}

			_armorClassLabel->clear();
			_armorClassLabel->setText("Classes of " + armor->getName());
			_armorClasses->clear();
			QVector<QPair<Class*, int>> classes = armor->getClasses();
			for (int i = 0; i < classes.size(); i++) {
				QPair<Class*, int> it = classes.at(i);
				_armorClasses->addItem(it.first->getName() + " - level " + QString::number(it.second));
			}

			_armorElemLabel->clear();
			_armorElemLabel->setText("Elements of " + armor->getName());
			_armorElements->clear();
			QVector<Element*> elements = armor->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				_armorElements->addItem(it->getName());
			}
		}
	}
}

void ArmorWindow::newArmor() {
	// Setup window
	QDialog armorWindow(0);
	armorWindow.setWindowTitle("Add a new armor");
	QVBoxLayout *layout = new QVBoxLayout(&armorWindow);

	QLabel *label = new QLabel("Enter a name and select a type:", &armorWindow);

	QFrame *setupFrame = new QFrame(&armorWindow);
	QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
	QLineEdit *nameFrame = new QLineEdit(setupFrame);
	QComboBox *typeSelection = new QComboBox(setupFrame);
	setupLayout->addWidget(nameFrame);
	setupLayout->addSpacing(20);
	setupLayout->addWidget(typeSelection);

	QFrame *buttonframe = new QFrame(&armorWindow);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
	QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
	buttonlayout->addWidget(acceptbutton);
	buttonlayout->addWidget(cancelbutton);

	layout->addWidget(label);
	layout->addWidget(setupFrame);
	layout->addWidget(buttonframe);

	connect(acceptbutton, SIGNAL(clicked()), &armorWindow, SLOT(accept()));
	connect(cancelbutton, SIGNAL(clicked()), &armorWindow, SLOT(reject()));

	QStringList types;
	types << "Helmet" << "Torso" << "Leggings" << "Gloves" << "Boots";
	typeSelection->addItems(types);

	if (armorWindow.exec()) {
		QString name = nameFrame->text();
		QString typeString = typeSelection->currentText();

		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newArmor();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("A armor should not have an empty name."));
			newArmor();
			return;
		}

		if (!!_properties->getEquipment()->getArmor(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("A armor with the same name already exists."));
			newArmor();
			return;
		}

		Armor::Subtype type;
		if (typeString == QString("Helmet"))
			type = Armor::HELMET;
		else if (typeString == QString("Torso"))
			type = Armor::TORSO;
		else if (typeString == QString("Leggings"))
			type = Armor::LEGGINGS;
		else if (typeString == QString("Gloves"))
			type = Armor::GLOVES;
		else if (typeString == QString("Boots"))
			type = Armor::BOOTS;
		else {
			QMessageBox::critical(this, tr("Unexpected error occurred"), tr("The selected type could not be matched."));
			return;
		}

		Armor *armor = Armor::create(name, type);
		try {
			_properties->getEquipment()->addArmor(armor);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete armor;
			return;
		}

		_armorList->addItem(armor->getName());
		_armorList->setCurrentRow(_armorList->count()-1);
		emit updateSignal();
	}
}

void ArmorWindow::editArmor() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Edit armor"),
				tr("Enter a new name for the armor:"), QLineEdit::Normal, armor->getName(), &accepted);

			if (accepted && (name != armor->getName())) {
				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editArmor();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("An armor should not have an empty name"));
					editArmor();
					return;
				}

				if (!!_properties->getEquipment()->getArmor(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("An armor with the same name already exists."));
					editArmor();
					return;
				}

				armor->setName(name);
				_armorList->currentItem()->setText(armor->getName());
				emit updateSignal();
			}
		}
	}
}

void ArmorWindow::deleteArmor() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		QString name = _armorList->currentItem()->text();
		Armor *armor = _properties->getEquipment()->getArmor(name);
		if (!!armor) {
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete armor",
				"Are you sure you want to delete the armor?\nDeleting an armor will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {
				_properties->getEquipment()->removeArmor(name);
				loadArmor();

				if (_armorList->count() > 0)
					_armorList->setCurrentRow(0);

				emit updateSignal();
			}
		}
	}
}

void ArmorWindow::moveArmorUp() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorList->currentRow() != 0)) {

		Armor *currentArmor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!currentArmor) {
			const QVector<Armor*>& armor = _properties->getEquipment()->getArmor();
			int index = armor.indexOf(currentArmor);

			try {
				switchPlaces(const_cast<QVector<Armor*>&>(armor), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadArmor();
			_armorList->setCurrentRow(index-1);
		}
	}
}

void ArmorWindow::moveArmorDown() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorList->currentRow() != _armorList->count()-1)) {

		Armor *currentArmor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!currentArmor) {
			const QVector<Armor*>& armor = _properties->getEquipment()->getArmor();
			int index = armor.indexOf(currentArmor);

			try {
				switchPlaces(const_cast<QVector<Armor*>&>(armor), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadArmor();
			_armorList->setCurrentRow(index+1);
		}
	}
}

void ArmorWindow::saveDescription() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			armor->setDescription(_armorDescription->toPlainText());
		}
	}
}

void ArmorWindow::reloadDescription() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			_armorDescription->setPlainText(armor->getDescription());
		}
	}
}

void ArmorWindow::addElement() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			// Setup window
			QDialog elementWindow(0);
			elementWindow.setWindowTitle("Add element to " + armor->getName());
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

			QVector<Element*> armorElements = armor->getElements();
			QVector<Element*> elements = _properties->getElements()->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				if (!armorElements.contains(it))
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
						armor->addElement(element);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_armorElements->addItem(element->getName());
				}
			}
		}
	}
}

void ArmorWindow::deleteElement() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorElements->count() > 0) && (!!_armorElements->currentItem())) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			armor->removeElement(_armorElements->currentItem()->text());
			_armorElements->takeItem(_armorElements->currentRow());
		}
	}
}

void ArmorWindow::moveElementUp() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorElements->count() > 0) && !!_armorElements->currentItem() && (_armorElements->currentRow() != 0)) {

		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			Element *currentElement = armor->getElements().at(_armorElements->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = armor->getElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int armorIndex = _armorList->currentRow();
				loadArmor();
				_armorList->setCurrentRow(armorIndex);
				_armorElements->setCurrentRow(index-1);
			}
		}
	}
}

void ArmorWindow::moveElementDown() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorElements->count() > 0) && !!_armorElements->currentItem() && (_armorElements->currentRow() != _armorElements->count()-1)) {

		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			Element *currentElement = armor->getElements().at(_armorElements->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = armor->getElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int armorIndex = _armorList->currentRow();
				loadArmor();
				_armorList->setCurrentRow(armorIndex);
				_armorElements->setCurrentRow(index+1);
			}
		}
	}
}

void ArmorWindow::addAttribute() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Add attribute to " + armor->getName());
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

			QVector<QPair<Attribute*, int>> armorAttributes = armor->getAttributes();
			QVector<Attribute*> attributes = _properties->getAttributes()->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				Attribute *it = attributes.at(i);
				for (int j = 0; j < armorAttributes.size(); j++) {
					if (armorAttributes.at(j).first == it)
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
						armor->addAttribute(attribute, value);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_armorAttributes->addItem(attribute->getName() + " - +" + QString::number(value));
				} 
			}
		}
	}
}

void ArmorWindow::editAttribute() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorAttributes->count() > 0) && !!_armorAttributes->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			QPair<Attribute*, int> attribute = armor->getAttribute(_armorAttributes->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Edit attribute of " + armor->getName());
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
				armor->editAttribute(attribute.first->getName(), valueSelection->value());
				_armorAttributes->currentItem()->setText(attribute.first->getName() + " - +" + QString::number(valueSelection->value()));
			}
		}
	}
}

void ArmorWindow::deleteAttribute() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorAttributes->count() > 0) && !!_armorAttributes->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			armor->removeAttribute(_armorAttributes->currentItem()->text().split(" - ").at(0));
			_armorAttributes->takeItem(_armorAttributes->currentRow());
		}
	}
}

void ArmorWindow::moveAttributeUp() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorAttributes->count() > 0) && !!_armorAttributes->currentItem() && (_armorAttributes->currentRow() != 0)) {

		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			QPair<Attribute*, int> attribute = armor->getAttributes().at(_armorAttributes->currentRow());
			if (!!attribute.first) {
				const QVector<QPair<Attribute*, int>>& attributes = armor->getAttributes();
				int index = attributes.indexOf(attribute);

				try {
					switchPairs(const_cast<QVector<QPair<Attribute*, int>>&>(attributes), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int armorIndex = _armorList->currentRow();
				loadArmor();
				_armorList->setCurrentRow(armorIndex);
				_armorAttributes->setCurrentRow(index-1);
			}
		}
	}
}

void ArmorWindow::moveAttributeDown() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorAttributes->count() > 0) && !!_armorAttributes->currentItem() && (_armorAttributes->currentRow() != _armorAttributes->count()-1)) {

		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			QPair<Attribute*, int> attribute = armor->getAttributes().at(_armorAttributes->currentRow());
			if (!!attribute.first) {
				const QVector<QPair<Attribute*, int>>& attributes = armor->getAttributes();
				int index = attributes.indexOf(attribute);

				try {
					switchPairs(const_cast<QVector<QPair<Attribute*, int>>&>(attributes), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int armorIndex = _armorList->currentRow();
				loadArmor();
				_armorList->setCurrentRow(armorIndex);
				_armorAttributes->setCurrentRow(index+1);
			}
		}
	}
}

void ArmorWindow::addClass() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			// Setup window
			QDialog classWindow(0);
			classWindow.setWindowTitle("Add class to " + armor->getName());
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

			QVector<QPair<Class*, int>> armorClasses = armor->getClasses();
			QVector<Class*> classes = _properties->getClasses()->getClasses();
			for (int i = 0; i < classes.size(); i++) {
				Class *it = classes.at(i);
				for (int j = 0; j < armorClasses.size(); j++) {
					if (armorClasses.at(j).first == it)
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
						armor->addClass(cat, level);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_armorClasses->addItem(cat->getName() + " - level " + QString::number(level));
				} 
			}
		}
	}
}

void ArmorWindow::editClass() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorClasses->count() > 0) && !!_armorClasses->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			QPair<Class*, int> cat = armor->getClass(_armorClasses->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog classWindow(0);
			classWindow.setWindowTitle("Edit class of " + armor->getName());
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
				armor->editClass(cat.first->getName(), levelSelection->value());
				_armorClasses->currentItem()->setText(cat.first->getName() + " - level " + QString::number(levelSelection->value()));
			}
		}
	}
}

void ArmorWindow::deleteClass() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorClasses->count() > 0) && !!_armorClasses->currentItem()) {
		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {
			armor->removeClass(_armorClasses->currentItem()->text().split(" - ").at(0));
			_armorClasses->takeItem(_armorClasses->currentRow());
		}
	}
}

void ArmorWindow::moveClassUp() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorClasses->count() > 0) && !!_armorClasses->currentItem() && (_armorClasses->currentRow() != 0)) {

		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			QPair<Class*, int> cat = armor->getClasses().at(_armorClasses->currentRow());
			if (!!cat.first) {
				const QVector<QPair<Class*, int>>& classes = armor->getClasses();
				int index = classes.indexOf(cat);

				try {
					switchPairs(const_cast<QVector<QPair<Class*, int>>&>(classes), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int armorIndex = _armorList->currentRow();
				loadArmor();
				_armorList->setCurrentRow(armorIndex);
				_armorClasses->setCurrentRow(index-1);
			}
		}
	}
}

void ArmorWindow::moveClassDown() {
	if ((_armorList->count() > 0) && !!_armorList->currentItem() && (_armorClasses->count() > 0) && !!_armorClasses->currentItem() && (_armorClasses->currentRow() != _armorClasses->count()-1)) {

		Armor *armor = _properties->getEquipment()->getArmor(_armorList->currentItem()->text());
		if (!!armor) {

			QPair<Class*, int> cat = armor->getClasses().at(_armorClasses->currentRow());
			if (!!cat.first) {
				const QVector<QPair<Class*, int>>& classes = armor->getClasses();
				int index = classes.indexOf(cat);

				try {
					switchPairs(const_cast<QVector<QPair<Class*, int>>&>(classes), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int armorIndex = _armorList->currentRow();
				loadArmor();
				_armorList->setCurrentRow(armorIndex);
				_armorClasses->setCurrentRow(index+1);
			}
		}
	}
}