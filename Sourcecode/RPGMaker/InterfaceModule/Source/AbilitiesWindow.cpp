#include "qpair.h"
#include "qvector.h"
#include "qspinbox.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"
#include "qradiobutton.h"

#include "../ProgramDefaults.h"
#include "../Headers/AbilitiesWindow.h"
#include "../Components/Headers/Element.h"
#include "../Components/Headers/Attribute.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;
using namespace interfacemodule;

AbilitiesWindow::AbilitiesWindow(QWidget *parent, Properties *properties)
	: QFrame(parent), _properties(properties) {
		_layout = new QHBoxLayout(this);
		createWindow();
		loadAbilities();
}

AbilitiesWindow::~AbilitiesWindow() {}

void AbilitiesWindow::createWindow() {
	// Ability selection
	QGroupBox *selection = new QGroupBox("Abilities", this);
	QVBoxLayout *selectionLayout = new QVBoxLayout(selection);

	QLabel *selectionLabel = new QLabel("Select an ability:", selection);
	_abilitiesList = new QListWidget(selection);
	QFrame *selectionButtonFrame = new QFrame(selection);
	QHBoxLayout *selectionButtonLayout = new QHBoxLayout(selectionButtonFrame);
	selectionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *newAbility = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", selectionButtonFrame);
	newAbility->setToolTip("Add a new ability");
	newAbility->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAbility = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", selectionButtonFrame);
	editAbility->setToolTip("Edit the current ability");
	editAbility->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAbility = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", selectionButtonFrame);
	deleteAbility->setToolTip("Delete the current ability");
	deleteAbility->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", selectionButtonFrame);
	abilityUpButton->setToolTip("Move selected ability up");
	abilityUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", selectionButtonFrame);
	abilityDownButton->setToolTip("Move selected ability down");
	abilityDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	selectionButtonLayout->addWidget(newAbility);
	selectionButtonLayout->addWidget(editAbility);
	selectionButtonLayout->addWidget(deleteAbility);
	selectionButtonLayout->addSpacing(20);
	selectionButtonLayout->addWidget(abilityUpButton);
	selectionButtonLayout->addWidget(abilityDownButton);
	selectionLayout->addWidget(selectionLabel);
	selectionLayout->addWidget(_abilitiesList);
	selectionLayout->addWidget(selectionButtonFrame);

	// Description
	QGroupBox *description = new QGroupBox("Description", this);
	QVBoxLayout *descriptionLayout = new QVBoxLayout(description);

	_descLabel = new QLabel(description);
	_description = new QPlainTextEdit(description);
	QFrame *descriptionButtonFrame = new QFrame(description);
	QHBoxLayout *descriptionButtonLayout = new QHBoxLayout(descriptionButtonFrame);
	descriptionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveDescription = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", descriptionButtonFrame);
	saveDescription->setToolTip("Save the description for the current ability");
	saveDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadDescription = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", descriptionButtonFrame);
	reloadDescription->setToolTip("Reload the original description for the current ability");
	reloadDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	descriptionButtonLayout->addWidget(saveDescription);
	descriptionButtonLayout->addWidget(reloadDescription);
	descriptionLayout->addWidget(_descLabel);
	descriptionLayout->addWidget(_description);
	descriptionLayout->addWidget(descriptionButtonFrame);

	// Elements
	QGroupBox *elements = new QGroupBox("Elements", this);
	QVBoxLayout *elementsLayout = new QVBoxLayout(elements);

	_elemLabel = new QLabel(elements);
	_elements = new QListWidget(elements);
	QFrame *elementsButtonFrame = new QFrame(elements);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(elementsButtonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addElement = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", elementsButtonFrame);
	addElement->setToolTip("Add an element to the current ability");
	addElement->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteElement = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", elementsButtonFrame);
	deleteElement->setToolTip("Delete the element from the current ability");
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
	elementsLayout->addWidget(_elemLabel);
	elementsLayout->addWidget(_elements);
	elementsLayout->addWidget(elementsButtonFrame);

	_layout->addWidget(selection);
	_layout->addWidget(description);
	_layout->addWidget(elements);

	connect(_abilitiesList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(abilitySelected(const QString &)));
	connect(newAbility, SIGNAL(clicked()), this, SLOT(newAbility()));
	connect(editAbility, SIGNAL(clicked()), this, SLOT(editAbility()));
	connect(deleteAbility, SIGNAL(clicked()), this, SLOT(deleteAbility()));
	connect(abilityUpButton, SIGNAL(clicked()), this, SLOT(moveAbilityUp()));
	connect(abilityDownButton, SIGNAL(clicked()), this, SLOT(moveAbilityDown()));
	connect(saveDescription, SIGNAL(clicked()), this, SLOT(saveDescription()));
	connect(reloadDescription, SIGNAL(clicked()), this, SLOT(reloadDescription()));
	connect(addElement, SIGNAL(clicked()), this, SLOT(addElement()));
	connect(deleteElement, SIGNAL(clicked()), this, SLOT(deleteElement()));
	connect(elementUpButton, SIGNAL(clicked()), this, SLOT(moveElementUp()));
	connect(elementDownButton, SIGNAL(clicked()), this, SLOT(moveElementDown()));
}

void AbilitiesWindow::loadAbilities() {
	_abilitiesList->clear();
	_descLabel->clear();
	_description->clear();
	_elemLabel->clear();
	_elements->clear();

	QVector<Ability*> abilities = _properties->getAbilities()->getAbilities();
	for (int i = 0; i < abilities.size(); i++) {
		Ability *it = abilities.at(i);
		_abilitiesList->addItem(it->getName());
	}

	if (_abilitiesList->count() > 0)
		_abilitiesList->setCurrentRow(0);

}

 void AbilitiesWindow::updateWindow() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem()) {
		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability)
			abilitySelected(ability->getName());
	}
}

void AbilitiesWindow::abilitySelected(const QString &name) {
	_descLabel->clear();
	_description->clear();
	_elemLabel->clear();
	_elements->clear();

	if (name != QString()) {
		Ability *ability = _properties->getAbilities()->getAbility(name);
		if (!!ability) {
			_descLabel->setText("Description of " + ability->getName());
			_description->setPlainText(ability->getDescription());

			_elemLabel->setText("Elements of " + ability->getName());
			QVector<Element*> elements = ability->getElements();
			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				_elements->addItem(it->getName());
			}
		}
	}
}

void AbilitiesWindow::newAbility() {
	// Setup dialog
	QDialog abilityWindow(0);
	abilityWindow.setWindowTitle("Add a new ability");
	QVBoxLayout *layout = new QVBoxLayout(&abilityWindow);

	QLabel *label1 = new QLabel("Enter a name for the ability:", &abilityWindow);
	QLineEdit *nameField = new QLineEdit(&abilityWindow);

	QLabel *label2 = new QLabel("Set the cost to use this ability:", &abilityWindow);
	QFrame *costFrame = new QFrame(&abilityWindow);
	QHBoxLayout *costLayout = new QHBoxLayout(costFrame);
	QLabel *costLabel = new QLabel(_properties->getAttributes()->getEnergySource()->getName() + ":");
	QSpinBox *costSelection = new QSpinBox(costFrame);
	costSelection->setMinimum(1);
	costSelection->setMaximum(9999);
	costSelection->setSingleStep(1);
	costSelection->setValue(1);
	costLayout->addWidget(costLabel);
	costLayout->addSpacing(20);
	costLayout->addWidget(costSelection);

	QLabel *label3 = new QLabel("Select an attribute and a multiplier:", &abilityWindow);
	QFrame *attributeFrame = new QFrame(&abilityWindow);
	QHBoxLayout *attributeLayout = new QHBoxLayout(attributeFrame);
	QLabel *attributeLabel1 = new QLabel("Attribute:", attributeFrame);
	QComboBox *attributeSelection = new QComboBox(attributeFrame);
	attributeSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
	QLabel *attributeLabel2 = new QLabel(" x", attributeFrame);
	QDoubleSpinBox *multiplierSelection = new QDoubleSpinBox(attributeFrame);
	multiplierSelection->setMinimum(0);
	multiplierSelection->setMaximum(10);
	multiplierSelection->setSingleStep(0.01);
	multiplierSelection->setDecimals(2);
	attributeLayout->addWidget(attributeLabel1);
	attributeLayout->addWidget(attributeSelection);
	attributeLayout->addWidget(attributeLabel2);
	attributeLayout->addWidget(multiplierSelection);

	QLabel *label4 = new QLabel("Select a type for the ability:", &abilityWindow);
	QFrame *typeFrame = new QFrame(&abilityWindow);
	QHBoxLayout *typeLayout = new QHBoxLayout(typeFrame);
	QRadioButton *offensiveButton = new QRadioButton("Offensive", typeFrame);
	QRadioButton *defensiveButton = new QRadioButton("Defensive", typeFrame);	
	typeLayout->addWidget(offensiveButton);
	typeLayout->addWidget(defensiveButton);

	QFrame *buttonframe = new QFrame(&abilityWindow);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
	QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
	buttonlayout->addWidget(acceptbutton);
	buttonlayout->addWidget(cancelbutton);

	layout->addWidget(label1);
	layout->addWidget(nameField);
	layout->addWidget(label2);
	layout->addWidget(costFrame);
	layout->addWidget(label3);
	layout->addWidget(attributeFrame);
	layout->addWidget(label4);
	layout->addWidget(typeFrame);
	layout->addWidget(buttonframe);

	connect(acceptbutton, SIGNAL(clicked()), &abilityWindow, SLOT(accept()));
	connect(cancelbutton, SIGNAL(clicked()), &abilityWindow, SLOT(reject()));

	QVector<Attribute*> attributes = _properties->getAttributes()->getOffensiveAttributes();
	attributeSelection->addItem(QString());
	for (int i = 0; i < attributes.size(); i++)
		attributeSelection->addItem(attributes.at(i)->getName());

	offensiveButton->setChecked(true);

	// Execute dialog
	if (abilityWindow.exec()) {
		QString name = nameField->text();

		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newAbility();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An ability should not have an empty name."));
			newAbility();
			return;
		}

		if (_properties->getAbilities()->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An ability with the same name already exists."));
			newAbility();
			return;
		}

		Ability *ability;
		if (offensiveButton->isChecked())
			ability = Ability::create(name, Ability::OFFENSIVE);
		else
			ability = Ability::create(name, Ability::DEFENSIVE);

		try {
			_properties->getAbilities()->addAbility(ability);

			if (attributeSelection->currentText() != QString())
				ability->setAttribute(_properties->getAttributes()->getAttribute(attributeSelection->currentText()), multiplierSelection->value());

			ability->setCost(costSelection->value());

		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete ability;
			return;
		}

		_abilitiesList->addItem(ability->getName());
		_abilitiesList->setCurrentRow(_abilitiesList->count()-1);
		emit updateSignal();
	}
}

void AbilitiesWindow::editAbility() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem()) {
		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {

			// Setup dialog
			QDialog abilityWindow(0);
			abilityWindow.setWindowTitle("Edit ability");
			QVBoxLayout *layout = new QVBoxLayout(&abilityWindow);

			QLabel *label1 = new QLabel("Enter a name for the ability:", &abilityWindow);
			QLineEdit *nameField = new QLineEdit(&abilityWindow);

			QLabel *label2 = new QLabel("Set the cost to use this ability:", &abilityWindow);
			QFrame *costFrame = new QFrame(&abilityWindow);
			QHBoxLayout *costLayout = new QHBoxLayout(costFrame);
			QLabel *costLabel = new QLabel(_properties->getAttributes()->getEnergySource()->getName() + ":");
			QSpinBox *costSelection = new QSpinBox(costFrame);
			costSelection->setMinimum(1);
			costSelection->setMaximum(9999);
			costSelection->setSingleStep(1);
			costSelection->setValue(1);
			costLayout->addWidget(costLabel);
			costLayout->addSpacing(20);
			costLayout->addWidget(costSelection);

			QLabel *label3 = new QLabel("Select an attribute and a multiplier:", &abilityWindow);
			QFrame *attributeFrame = new QFrame(&abilityWindow);
			QHBoxLayout *attributeLayout = new QHBoxLayout(attributeFrame);
			QLabel *attributeLabel1 = new QLabel("Attribute:", attributeFrame);
			QComboBox *attributeSelection = new QComboBox(attributeFrame);
			attributeSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QLabel *attributeLabel2 = new QLabel(" x", attributeFrame);
			QDoubleSpinBox *multiplierSelection = new QDoubleSpinBox(attributeFrame);
			multiplierSelection->setMinimum(0);
			multiplierSelection->setMaximum(10);
			multiplierSelection->setSingleStep(0.01);
			multiplierSelection->setDecimals(2);
			attributeLayout->addWidget(attributeLabel1);
			attributeLayout->addWidget(attributeSelection);
			attributeLayout->addWidget(attributeLabel2);
			attributeLayout->addWidget(multiplierSelection);

			QLabel *label4 = new QLabel("Select a type for the ability:", &abilityWindow);
			QFrame *typeFrame = new QFrame(&abilityWindow);
			QHBoxLayout *typeLayout = new QHBoxLayout(typeFrame);
			QRadioButton *offensiveButton = new QRadioButton("Offensive", typeFrame);
			QRadioButton *defensiveButton = new QRadioButton("Defensive", typeFrame);	
			typeLayout->addWidget(offensiveButton);
			typeLayout->addWidget(defensiveButton);

			QFrame *buttonframe = new QFrame(&abilityWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label1);
			layout->addWidget(nameField);
			layout->addWidget(label2);
			layout->addWidget(costFrame);
			layout->addWidget(label3);
			layout->addWidget(attributeFrame);
			layout->addWidget(label4);
			layout->addWidget(typeFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &abilityWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &abilityWindow, SLOT(reject()));

			// Set attribute, cost, multiplier and type
			int index = 0;
			QVector<Attribute*> attributes = _properties->getAttributes()->getOffensiveAttributes();
			attributeSelection->addItem(QString());
			for (int i = 0; i < attributes.size(); i++) {
				attributeSelection->addItem(attributes.at(i)->getName());
				if (!!ability->getAttribute().first && (attributes.at(i)->getName() == ability->getAttribute().first->getName())) {
					index = i+1;
					multiplierSelection->setValue(ability->getAttribute().second);
				}
			}
			attributeSelection->setCurrentIndex(index);
			costSelection->setValue(ability->getCost());

			switch (ability->getType()) {
			case Ability::OFFENSIVE:
				offensiveButton->setChecked(true);
				break;
			case Ability::DEFENSIVE:
				defensiveButton->setChecked(true);
				break;
			}


			nameField->setText(ability->getName());
			switch (ability->getType()) {
			case Ability::OFFENSIVE:
				offensiveButton->setChecked(true);
				break;
			case Ability::DEFENSIVE:
				defensiveButton->setCheckable(true);
				break;
			}

			// Execute dialog
			if (abilityWindow.exec()) {
				QString name = nameField->text();

				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editAbility();
					return;
				}

				// If name is empty...
				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("An ability should not have an empty name."));
					editAbility();
					return;
				}

				// If the new name is not the same as the previous one and the name is already in use...
				if ((name != ability->getName()) && _properties->getAbilities()->isRegistered(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("An ability with the same name already exists."));
					editAbility();
					return;
				}

				// Set new name and edit attribute
				try {
					ability->setName(name);
					if (attributeSelection->currentText() != QString())
						ability->setAttribute(_properties->getAttributes()->getOffensiveAttribute(attributeSelection->currentText()), multiplierSelection->value());
					else 
						ability->unsetAttribute();

					ability->setCost(costSelection->value());

					if (offensiveButton->isChecked())
						ability->setType(Ability::OFFENSIVE);
					else
						ability->setType(Ability::DEFENSIVE);

				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				// Load abilities again
				int abilityIndex = _abilitiesList->currentRow();
				loadAbilities();
				
				if (_abilitiesList->count() > 0)
					_abilitiesList->setCurrentRow(abilityIndex);

				emit updateSignal();
			}
		}
	}
}

void AbilitiesWindow::deleteAbility() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem()) {
		QString name = _abilitiesList->currentItem()->text();
		Ability *ability = _properties->getAbilities()->getAbility(name);
		if (!!ability) {
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete ability",
				"Are you sure you want to delete the offensive ability?\nDeleting an ability will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_properties->getAbilities()->removeAbility(name);

				_abilitiesList->takeItem(_abilitiesList->currentRow());
				emit updateSignal();
			}
		}
	}
}

void AbilitiesWindow::moveAbilityUp() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_abilitiesList->currentRow() != 0)) {

		Ability *currentAbility = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!currentAbility) {
			const QVector<Ability*>& abilities = _properties->getAbilities()->getAbilities();
			int index = abilities.indexOf(currentAbility);

			try {
				switchPlaces(const_cast<QVector<Ability*>&>(abilities), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAbilities();
			_abilitiesList->setCurrentRow(index-1);
		}
	}
}

void AbilitiesWindow::moveAbilityDown() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_abilitiesList->currentRow() != _abilitiesList->count()-1)) {

		Ability *currentAbility = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!currentAbility) {
			const QVector<Ability*>& abilities = _properties->getAbilities()->getAbilities();
			int index = abilities.indexOf(currentAbility);

			try {
				switchPlaces(const_cast<QVector<Ability*>&>(abilities), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAbilities();
			_abilitiesList->setCurrentRow(index+1);
		}
	}
}

void AbilitiesWindow::addElement() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem()) {
		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {

			// Setup window
			QDialog elementwindow(0);
			elementwindow.setWindowTitle("Add element to " + ability->getName());
			QVBoxLayout *layout = new QVBoxLayout(&elementwindow);

			QLabel *label = new QLabel("Select an element:", &elementwindow);
			QComboBox *elementselection = new QComboBox(&elementwindow);
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

			QVector<Element*> abilityElements = ability->getElements();
			QVector<Element*> elements = _properties->getElements()->getElements();

			for (int i = 0; i < elements.size(); i++) {
				Element *it = elements.at(i);
				if (!abilityElements.contains(it))
					elementselection->addItem(it->getName());
			}

			if (elementselection->count() == 0) {
				QMessageBox::information(this, tr("No elements available"), tr("There are no more elements available to choose from."));
				return;
			}

			if (elementwindow.exec()) {
				Element *element = _properties->getElements()->getElement(elementselection->currentText());
				if (!!element) {
					try {
						ability->addElement(element);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_elements->addItem(element->getName());
				}
			}
		}
	}
}

void AbilitiesWindow::deleteElement() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_elements->count() > 0) && (!!_elements->currentItem())) {
		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {
			ability->removeElement(_elements->currentItem()->text());
			_elements->takeItem(_elements->currentRow());
		}
	}
}

void AbilitiesWindow::moveElementUp() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_elements->count() > 0) && !!_elements->currentItem() && (_elements->currentRow() != 0)) {

		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {

			Element *element = ability->getElements().at(_elements->currentRow());
			if (!!element) {
				const QVector<Element*>& elements = ability->getElements();
				int index = elements.indexOf(element);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int abilityIndex = _abilitiesList->currentRow();
				loadAbilities();
				_abilitiesList->setCurrentRow(abilityIndex);
				_elements->setCurrentRow(index-1);
			}
		}
	}
}

void AbilitiesWindow::moveElementDown() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem() && (_elements->count() > 0) && !!_elements->currentItem() && (_elements->currentRow() != _elements->count()-1)) {

		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {

			Element *element = ability->getElements().at(_elements->currentRow());
			if (!!element) {
				const QVector<Element*>& elements = ability->getElements();
				int index = elements.indexOf(element);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int abilityIndex = _abilitiesList->currentRow();
				loadAbilities();
				_abilitiesList->setCurrentRow(abilityIndex);
				_elements->setCurrentRow(index+1);
			}
		}
	}
}

void AbilitiesWindow::saveDescription() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem()) {
		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {
			ability->setDescription(_description->toPlainText());
		}
	}
}

void AbilitiesWindow::reloadDescription() {
	if ((_abilitiesList->count() > 0) && !!_abilitiesList->currentItem()) {
		Ability *ability = _properties->getAbilities()->getAbility(_abilitiesList->currentItem()->text());
		if (!!ability) {
			_description->setPlainText(ability->getDescription());
		}
	}
}