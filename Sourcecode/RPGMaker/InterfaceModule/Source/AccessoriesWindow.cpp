#include "qspinbox.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/AccessoriesWindow.h"
#include "../Components/Headers/Class.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Attribute.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;
using namespace projectcomponents;

AccessoriesWindow::AccessoriesWindow(QWidget *parent, Properties *properties)
	: QFrame(parent), _properties(properties) {
		_layout = new QHBoxLayout(this);
		createWindow();
		loadAccessories();
}

AccessoriesWindow::~AccessoriesWindow() {}

void AccessoriesWindow::createWindow() {
	// Accessory Selection
	QGroupBox *selection = new QGroupBox("Accessories", this);
	QVBoxLayout *selectionLayout = new QVBoxLayout(selection);

	QLabel *selectionLabel = new QLabel("Select a accessory:", selection);
	_accessoryList = new QListWidget(selection);
	QFrame *selectionButtonFrame = new QFrame(selection);
	QHBoxLayout *selectionButtonLayout = new QHBoxLayout(selectionButtonFrame);
	selectionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *newAccessory = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", selectionButtonFrame);
	newAccessory->setToolTip("Add a new accessory");
	newAccessory->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAccessory = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", selectionButtonFrame);
	editAccessory->setToolTip("Edit the current accessory");
	editAccessory->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAccessory = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", selectionButtonFrame);
	deleteAccessory->setToolTip("Delete the current accessory");
	deleteAccessory->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *accessoryUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", selectionButtonFrame);
	accessoryUpButton->setToolTip("Move selected accessory up");
	accessoryUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *accessoryDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", selectionButtonFrame);
	accessoryDownButton->setToolTip("Move selected accessory down");
	accessoryDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	selectionButtonLayout->addWidget(newAccessory);
	selectionButtonLayout->addWidget(editAccessory);
	selectionButtonLayout->addWidget(deleteAccessory);
	selectionButtonLayout->addSpacing(20);
	selectionButtonLayout->addWidget(accessoryUpButton);
	selectionButtonLayout->addWidget(accessoryDownButton);
	selectionLayout->addWidget(selectionLabel);
	selectionLayout->addWidget(_accessoryList);
	selectionLayout->addWidget(selectionButtonFrame);

	// Description
	QGroupBox *description = new QGroupBox("Description", this);
	QVBoxLayout *descriptionLayout = new QVBoxLayout(description);

	_accessoryDescLabel = new QLabel(description);
	_accessoryDescription = new QPlainTextEdit(description);
	QFrame *descriptionButtonFrame = new QFrame(description);
	QHBoxLayout *descriptionButtonLayout = new QHBoxLayout(descriptionButtonFrame);
	descriptionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveDescription = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", descriptionButtonFrame);
	saveDescription->setToolTip("Save the description for the current accessory");
	saveDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadDescription = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", descriptionButtonFrame);
	reloadDescription->setToolTip("Reload the original description for the current accessory");
	reloadDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	descriptionButtonLayout->addWidget(saveDescription);
	descriptionButtonLayout->addWidget(reloadDescription);
	descriptionLayout->addWidget(_accessoryDescLabel);
	descriptionLayout->addWidget(_accessoryDescription);
	descriptionLayout->addWidget(descriptionButtonFrame);

	// Attributes
	QGroupBox *attributes = new QGroupBox("Attributes", this);
	QVBoxLayout *attributesLayout = new QVBoxLayout(attributes);

	_accessoryAttLabel = new QLabel(attributes);
	_accessoryAttributes = new QListWidget(attributes);
	QFrame *attributesButtonFrame = new QFrame(attributes);
	QHBoxLayout *attributesButtonLayout = new QHBoxLayout(attributesButtonFrame);
	attributesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addAttribute = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", attributesButtonFrame);
	addAttribute->setToolTip("Add a new attribute to the current accessory");
	addAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAttribute = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", attributesButtonFrame);
	editAttribute->setToolTip("Edit the current attribute");
	editAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAttribute = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", attributesButtonFrame);
	deleteAttribute->setToolTip("Delete the attribute from current accessory");
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
	attributesLayout->addWidget(_accessoryAttLabel);
	attributesLayout->addWidget(_accessoryAttributes);
	attributesLayout->addWidget(attributesButtonFrame);

	// Classes
	QGroupBox *classes = new QGroupBox("Classes", this);
	QVBoxLayout *classesLayout = new QVBoxLayout(classes);

	_accessoryClassLabel = new QLabel(classes);
	_accessoryClasses = new QListWidget(classes);
	QFrame *classesButtonFrame = new QFrame(attributes);
	QHBoxLayout *classesButtonLayout = new QHBoxLayout(classesButtonFrame);
	classesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addClass = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", classesButtonFrame);
	addClass->setToolTip("Add a new class to the current accessory");
	addClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editClass = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", classesButtonFrame);
	editClass->setToolTip("Edit the current class");
	editClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteClass = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", classesButtonFrame);
	deleteClass->setToolTip("Delete the class from current accessory");
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
	classesLayout->addWidget(_accessoryClassLabel);
	classesLayout->addWidget(_accessoryClasses);
	classesLayout->addWidget(classesButtonFrame);

	// Elements
	QGroupBox *elements = new QGroupBox("Elements", this);
	QVBoxLayout *elementsLayout = new QVBoxLayout(elements);

	_accessoryElemLabel = new QLabel(elements);
	_accessoryElements = new QListWidget(elements);
	QFrame *elementsButtonFrame = new QFrame(elements);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(elementsButtonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addElement = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", elementsButtonFrame);
	addElement->setToolTip("Add an element to the current accessory");
	addElement->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteElement = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", elementsButtonFrame);
	deleteElement->setToolTip("Delete the element from the current accessory");
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
	elementsLayout->addWidget(_accessoryElemLabel);
	elementsLayout->addWidget(_accessoryElements);
	elementsLayout->addWidget(elementsButtonFrame);

	_layout->addWidget(selection);
	_layout->addWidget(description);
	_layout->addWidget(attributes);
	_layout->addWidget(classes);
	_layout->addWidget(elements);

	connect(_accessoryList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(accessorySelected(const QString &)));
	connect(newAccessory, SIGNAL(clicked()), this, SLOT(newAccessory()));
	connect(editAccessory, SIGNAL(clicked()), this, SLOT(editAccessory()));
	connect(deleteAccessory, SIGNAL(clicked()), this, SLOT(deleteAccessory()));
	connect(accessoryUpButton, SIGNAL(clicked()), this, SLOT(moveAccessoryUp()));
	connect(accessoryDownButton, SIGNAL(clicked()), this, SLOT(moveAccessoryDown()));
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

void AccessoriesWindow::loadAccessories() {
	_accessoryList->clear();
	QVector<Accessory*> accessories = _properties->getEquipment()->getAccessories();
	for (int i = 0; i < accessories.size(); i++) {
		Accessory *accessory = accessories.at(i);
		_accessoryList->addItem(accessory->getName());
	}

	if (_accessoryList->count() > 0)
		_accessoryList->setCurrentRow(0);
}

void AccessoriesWindow::updateWindow() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory)
			accessorySelected(accessory->getName());
	}
}

void AccessoriesWindow::accessorySelected(const QString &name) {
	if (name != QString()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(name);
		if (!!accessory) {
			_accessoryDescLabel->clear();
			_accessoryDescLabel->setText("Description of " + accessory->getName());
			_accessoryDescription->clear();
			_accessoryDescription->setPlainText(accessory->getDescription());

			_accessoryAttLabel->clear();
			_accessoryAttLabel->setText("Attributes of " + accessory->getName());
			_accessoryAttributes->clear();
			QVector<QPair<Attribute*, int>> attributes = accessory->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				QPair<Attribute*, int> it = attributes.at(i);
				_accessoryAttributes->addItem(it.first->getName() + " - +" + QString::number(it.second));
			}

			_accessoryClassLabel->clear();
			_accessoryClassLabel->setText("Classes of " + accessory->getName());
			_accessoryClasses->clear();
			QVector<QPair<Class*, int>> classes = accessory->getClasses();
			for (int i = 0; i < classes.size(); i++) {
				QPair<Class*, int> it = classes.at(i);
				_accessoryClasses->addItem(it.first->getName() + " - level " + QString::number(it.second));
			}

			_accessoryElemLabel->clear();
			_accessoryElemLabel->setText("Elements of " + accessory->getName());
			_accessoryElements->clear();
			QVector<Element*> elements = accessory->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				_accessoryElements->addItem(it->getName());
			}
		}
	}
}

void AccessoriesWindow::newAccessory() {
	// Setup window
	QDialog accessoryWindow(0);
	accessoryWindow.setWindowTitle("Add a new accessory");
	QVBoxLayout *layout = new QVBoxLayout(&accessoryWindow);

	QLabel *label = new QLabel("Enter a name and select a type:", &accessoryWindow);

	QFrame *setupFrame = new QFrame(&accessoryWindow);
	QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
	QLineEdit *nameFrame = new QLineEdit(setupFrame);
	QComboBox *typeSelection = new QComboBox(setupFrame);
	setupLayout->addWidget(nameFrame);
	setupLayout->addSpacing(20);
	setupLayout->addWidget(typeSelection);

	QFrame *buttonframe = new QFrame(&accessoryWindow);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
	QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
	buttonlayout->addWidget(acceptbutton);
	buttonlayout->addWidget(cancelbutton);

	layout->addWidget(label);
	layout->addWidget(setupFrame);
	layout->addWidget(buttonframe);

	connect(acceptbutton, SIGNAL(clicked()), &accessoryWindow, SLOT(accept()));
	connect(cancelbutton, SIGNAL(clicked()), &accessoryWindow, SLOT(reject()));

	QStringList types;
	types << "Earring" << "Necklace" << "Ring";
	typeSelection->addItems(types);

	if (accessoryWindow.exec()) {
		QString name = nameFrame->text();
		QString typeString = typeSelection->currentText();

		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newAccessory();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("A accessory should not have an empty name."));
			newAccessory();
			return;
		}

		if (!!_properties->getEquipment()->getAccessory(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("A accessory with the same name already exists."));
			newAccessory();
			return;
		}

		Accessory::Subtype type;
		if (typeString == QString("Earring"))
			type = Accessory::EARRING;
		else if (typeString == QString("Necklace"))
			type = Accessory::NECKLACE;
		else if (typeString == QString("Ring"))
			type = Accessory::RING;
		else {
			QMessageBox::critical(this, tr("Unexpected error occurred"), tr("The selected type could not be matched."));
			return;
		}

		Accessory *accessory = Accessory::create(name, type);
		try {
			_properties->getEquipment()->addAccessory(accessory);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete accessory;
			return;
		}

		_accessoryList->addItem(accessory->getName());
		_accessoryList->setCurrentRow(_accessoryList->count()-1);
		emit updateSignal();
	}
}

void AccessoriesWindow::editAccessory() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Edit accessory"),
				tr("Enter a new name for the accessory:"), QLineEdit::Normal, accessory->getName(), &accepted);

			if (accepted && (name != accessory->getName())) {

				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editAccessory();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("An accessory should not have an empty name"));
					editAccessory();
					return;
				}

				if (!!_properties->getEquipment()->getAccessory(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("An accessory with the same name already exists."));
					editAccessory();
					return;
				}

				accessory->setName(name);
				_accessoryList->currentItem()->setText(accessory->getName());
				emit updateSignal();
			}
		}
	}
}

void AccessoriesWindow::deleteAccessory() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		QString name = _accessoryList->currentItem()->text();
		Accessory *accessory = _properties->getEquipment()->getAccessory(name);
		if (!!accessory) {
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete accessory",
				"Are you sure you want to delete the accessory?\nDeleting an accessory will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {
				_properties->getEquipment()->removeAccessory(name);
				loadAccessories();

				if (_accessoryList->count() > 0)
					_accessoryList->setCurrentRow(0);

				emit updateSignal();
			}
		}
	}
}

void AccessoriesWindow::moveAccessoryUp() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryList->currentRow() != 0)) {

		Accessory *currentAccessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!currentAccessory) {
			const QVector<Accessory*>& accessories = _properties->getEquipment()->getAccessories();
			int index = accessories.indexOf(currentAccessory);

			try {
				switchPlaces(const_cast<QVector<Accessory*>&>(accessories), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAccessories();
			_accessoryList->setCurrentRow(index-1);
		}
	}
}

void AccessoriesWindow::moveAccessoryDown() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryList->currentRow() != _accessoryList->count()-1)) {

		Accessory *currentAccessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!currentAccessory) {
			const QVector<Accessory*>& accessories = _properties->getEquipment()->getAccessories();
			int index = accessories.indexOf(currentAccessory);

			try {
				switchPlaces(const_cast<QVector<Accessory*>&>(accessories), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAccessories();
			_accessoryList->setCurrentRow(index+1);
		}
	}
}

void AccessoriesWindow::saveDescription() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			accessory->setDescription(_accessoryDescription->toPlainText());
		}
	}
}

void AccessoriesWindow::reloadDescription() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			_accessoryDescription->setPlainText(accessory->getDescription());
		}
	}
}

void AccessoriesWindow::addElement() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			// Setup window
			QDialog elementWindow(0);
			elementWindow.setWindowTitle("Add element to " + accessory->getName());
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

			QVector<Element*> accessoryElements = accessory->getElements();
			QVector<Element*> elements = _properties->getElements()->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				if (!accessoryElements.contains(it))
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
						accessory->addElement(element);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_accessoryElements->addItem(element->getName());
				}
			}
		}
	}
}

void AccessoriesWindow::deleteElement() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryElements->count() > 0) && (!!_accessoryElements->currentItem())) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			accessory->removeElement(_accessoryElements->currentItem()->text());
			_accessoryElements->takeItem(_accessoryElements->currentRow());
		}
	}
}

void AccessoriesWindow::moveElementUp() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryElements->count() > 0) && !!_accessoryElements->currentItem() && (_accessoryElements->currentRow() != 0)) {

		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			Element *currentElement = accessory->getElements().at(_accessoryElements->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = accessory->getElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int accessoryIndex = _accessoryList->currentRow();
				loadAccessories();
				_accessoryList->setCurrentRow(accessoryIndex);
				_accessoryElements->setCurrentRow(index-1);
			}
		}
	}
}

void AccessoriesWindow::moveElementDown() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryElements->count() > 0) && !!_accessoryElements->currentItem() && (_accessoryElements->currentRow() != _accessoryElements->count()-1)) {

		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			Element *currentElement = accessory->getElements().at(_accessoryElements->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = accessory->getElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int accessoryIndex = _accessoryList->currentRow();
				loadAccessories();
				_accessoryList->setCurrentRow(accessoryIndex);
				_accessoryElements->setCurrentRow(index+1);
			}
		}
	}
}

void AccessoriesWindow::addAttribute() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Add attribute to " + accessory->getName());
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

			QVector<QPair<Attribute*, int>> accessoryAttributes = accessory->getAttributes();
			QVector<Attribute*> attributes = _properties->getAttributes()->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				Attribute *it = attributes.at(i);
				for (int j = 0; j < accessoryAttributes.size(); j++) {
					if (accessoryAttributes.at(j).first == it)
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
						accessory->addAttribute(attribute, value);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_accessoryAttributes->addItem(attribute->getName() + " - +" + QString::number(value));
				} 
			}
		}
	}
}

void AccessoriesWindow::editAttribute() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryAttributes->count() > 0) && !!_accessoryAttributes->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			QPair<Attribute*, int> attribute = accessory->getAttribute(_accessoryAttributes->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Edit attribute of " + accessory->getName());
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
				accessory->editAttribute(attribute.first->getName(), valueSelection->value());
				_accessoryAttributes->currentItem()->setText(attribute.first->getName() + " - +" + QString::number(valueSelection->value()));
			}
		}
	}
}

void AccessoriesWindow::deleteAttribute() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryAttributes->count() > 0) && !!_accessoryAttributes->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			accessory->removeAttribute(_accessoryAttributes->currentItem()->text().split(" - ").at(0));
			_accessoryAttributes->takeItem(_accessoryAttributes->currentRow());
		}
	}
}

void AccessoriesWindow::moveAttributeUp() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryAttributes->count() > 0) && !!_accessoryAttributes->currentItem() && (_accessoryAttributes->currentRow() != 0)) {

		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			QPair<Attribute*, int> attribute = accessory->getAttributes().at(_accessoryAttributes->currentRow());
			if (!!attribute.first) {
				const QVector<QPair<Attribute*, int>>& attributes = accessory->getAttributes();
				int index = attributes.indexOf(attribute);

				try {
					switchPairs(const_cast<QVector<QPair<Attribute*, int>>&>(attributes), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int accessoryIndex = _accessoryList->currentRow();
				loadAccessories();
				_accessoryList->setCurrentRow(accessoryIndex);
				_accessoryAttributes->setCurrentRow(index-1);
			}
		}
	}
}

void AccessoriesWindow::moveAttributeDown() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryAttributes->count() > 0) && !!_accessoryAttributes->currentItem() && (_accessoryAttributes->currentRow() != _accessoryAttributes->count()-1)) {

		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			QPair<Attribute*, int> attribute = accessory->getAttributes().at(_accessoryAttributes->currentRow());
			if (!!attribute.first) {
				const QVector<QPair<Attribute*, int>>& attributes = accessory->getAttributes();
				int index = attributes.indexOf(attribute);

				try {
					switchPairs(const_cast<QVector<QPair<Attribute*, int>>&>(attributes), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int accessoryIndex = _accessoryList->currentRow();
				loadAccessories();
				_accessoryList->setCurrentRow(accessoryIndex);
				_accessoryAttributes->setCurrentRow(index+1);
			}
		}
	}
}

void AccessoriesWindow::addClass() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			// Setup window
			QDialog classWindow(0);
			classWindow.setWindowTitle("Add class to " + accessory->getName());
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

			QVector<QPair<Class*, int>> accessoryClasses = accessory->getClasses();
			QVector<Class*> classes = _properties->getClasses()->getClasses();
			for (int i = 0; i < classes.size(); i++) {
				Class *it = classes.at(i);
				for (int j = 0; j < accessoryClasses.size(); j++) {
					if (accessoryClasses.at(j).first == it)
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
						accessory->addClass(cat, level);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_accessoryClasses->addItem(cat->getName() + " - level " + QString::number(level));
				} 
			}
		}
	}
}

void AccessoriesWindow::editClass() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryClasses->count() > 0) && !!_accessoryClasses->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			QPair<Class*, int> cat = accessory->getClass(_accessoryClasses->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog classWindow(0);
			classWindow.setWindowTitle("Edit class of " + accessory->getName());
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
				accessory->editClass(cat.first->getName(), levelSelection->value());
				_accessoryClasses->currentItem()->setText(cat.first->getName() + " - level " + QString::number(levelSelection->value()));
			}
		}
	}
}

void AccessoriesWindow::deleteClass() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryClasses->count() > 0) && !!_accessoryClasses->currentItem()) {
		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {
			accessory->removeClass(_accessoryClasses->currentItem()->text().split(" - ").at(0));
			_accessoryClasses->takeItem(_accessoryClasses->currentRow());
		}
	}
}

void AccessoriesWindow::moveClassUp() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryClasses->count() > 0) && !!_accessoryClasses->currentItem() && (_accessoryClasses->currentRow() != 0)) {

		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			QPair<Class*, int> cat = accessory->getClasses().at(_accessoryClasses->currentRow());
			if (!!cat.first) {
				const QVector<QPair<Class*, int>>& classes = accessory->getClasses();
				int index = classes.indexOf(cat);

				try {
					switchPairs(const_cast<QVector<QPair<Class*, int>>&>(classes), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int accessoryIndex = _accessoryList->currentRow();
				loadAccessories();
				_accessoryList->setCurrentRow(accessoryIndex);
				_accessoryClasses->setCurrentRow(index-1);
			}
		}
	}
}

void AccessoriesWindow::moveClassDown() {
	if ((_accessoryList->count() > 0) && !!_accessoryList->currentItem() && (_accessoryClasses->count() > 0) && !!_accessoryClasses->currentItem() && (_accessoryClasses->currentRow() != _accessoryClasses->count()-1)) {

		Accessory *accessory = _properties->getEquipment()->getAccessory(_accessoryList->currentItem()->text());
		if (!!accessory) {

			QPair<Class*, int> cat = accessory->getClasses().at(_accessoryClasses->currentRow());
			if (!!cat.first) {
				const QVector<QPair<Class*, int>>& classes = accessory->getClasses();
				int index = classes.indexOf(cat);

				try {
					switchPairs(const_cast<QVector<QPair<Class*, int>>&>(classes), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int accessoryIndex = _accessoryList->currentRow();
				loadAccessories();
				_accessoryList->setCurrentRow(accessoryIndex);
				_accessoryClasses->setCurrentRow(index+1);
			}
		}
	}
}