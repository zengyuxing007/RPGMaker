#include "qspinbox.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/ClassesWindow.h"
#include "../Components/Headers/Class.h"
#include "../Components/Headers/Ability.h"
#include "../Components/Headers/Attribute.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;
using namespace projectcomponents;

ClassesWindow::ClassesWindow(QWidget *parent, Properties *properties)
	: QFrame(parent), _properties(properties) {
		_layout = new QHBoxLayout(this);

		createWindow();

		loadClasses();
}

ClassesWindow::~ClassesWindow() {}

void ClassesWindow::createWindow() {
	// Class selection
	QGroupBox *selection = new QGroupBox("Classes", this);
	QVBoxLayout *selectionLayout = new QVBoxLayout(selection);

	QLabel *selectionLabel = new QLabel("Select a class:", selection);
	_classList = new QListWidget(selection);
	QFrame *selectionButtonFrame = new QFrame(selection);
	QHBoxLayout *selectionButtonLayout = new QHBoxLayout(selectionButtonFrame);
	selectionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *newClass = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", selectionButtonFrame);
	newClass->setToolTip("Add a new class");
	newClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editClass = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", selectionButtonFrame);
	editClass->setToolTip("Edit the current class");
	editClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteClass = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", selectionButtonFrame);
	deleteClass->setToolTip("Delete the current class");
	deleteClass->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *classUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", selectionButtonFrame);
	classUpButton->setToolTip("Move selected class up");
	classUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *classDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", selectionButtonFrame);
	classDownButton->setToolTip("Move selected class down");
	classDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	selectionButtonLayout->addWidget(newClass);
	selectionButtonLayout->addWidget(editClass);
	selectionButtonLayout->addWidget(deleteClass);
	selectionButtonLayout->addSpacing(20);
	selectionButtonLayout->addWidget(classUpButton);
	selectionButtonLayout->addWidget(classDownButton);
	selectionLayout->addWidget(selectionLabel);
	selectionLayout->addWidget(_classList);
	selectionLayout->addWidget(selectionButtonFrame);

	// Description
	QGroupBox *description = new QGroupBox("Description", this);
	QVBoxLayout *descriptionLayout = new QVBoxLayout(description);

	_classDescLabel = new QLabel(description);
	_classDesc = new QPlainTextEdit(description);
	QFrame *descriptionButtonFrame = new QFrame(description);
	QHBoxLayout *descriptionButtonLayout = new QHBoxLayout(descriptionButtonFrame);
	descriptionButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *saveDescription = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", descriptionButtonFrame);
	saveDescription->setToolTip("Save the description for the current class");
	saveDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *reloadDescription = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", descriptionButtonFrame);
	reloadDescription->setToolTip("Reload the original description for the current class");
	reloadDescription->setMaximumSize(g_buttonsize, g_buttonsize);
	descriptionButtonLayout->addWidget(saveDescription);
	descriptionButtonLayout->addWidget(reloadDescription);
	descriptionLayout->addWidget(_classDescLabel);
	descriptionLayout->addWidget(_classDesc);
	descriptionLayout->addWidget(descriptionButtonFrame);

	// Attributes
	QGroupBox *attributes = new QGroupBox("Attributes", this);
	QVBoxLayout *attributesLayout = new QVBoxLayout(attributes);

	_classAttLabel = new QLabel(attributes);
	_classAttributes = new QListWidget(attributes);
	QFrame *attributesButtonFrame = new QFrame(attributes);
	QHBoxLayout *attributesButtonLayout = new QHBoxLayout(attributesButtonFrame);
	attributesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *editAttribute = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", attributesButtonFrame);
	editAttribute->setToolTip("Edit the current attribute");
	editAttribute->setMaximumSize(g_buttonsize, g_buttonsize);
	attributesButtonLayout->addWidget(editAttribute);
	attributesLayout->addWidget(_classAttLabel);
	attributesLayout->addWidget(_classAttributes);
	attributesLayout->addWidget(attributesButtonFrame);

	// Abilities
	QGroupBox *abilities = new QGroupBox("Abilities", this);
	QVBoxLayout *abilitiesLayout = new QVBoxLayout(abilities);

	_classAbLabel = new QLabel(abilities);
	_classAbilities = new QListWidget(abilities);
	QFrame *abilitiesButtonFrame = new QFrame(abilities);
	QHBoxLayout *abilitiesButtonLayout = new QHBoxLayout(abilitiesButtonFrame);
	abilitiesButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addAbility = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", abilitiesButtonFrame);
	addAbility->setToolTip("Add an ability to the current class");
	addAbility->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editAbility = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", abilitiesButtonFrame);
	editAbility->setToolTip("Edit the current ability");
	editAbility->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteAbility = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", abilitiesButtonFrame);
	deleteAbility->setToolTip("Delete the ability from the current class");
	deleteAbility->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", abilitiesButtonFrame);
	abilityUpButton->setToolTip("Move selected ability up");
	abilityUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *abilityDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", abilitiesButtonFrame);
	abilityDownButton->setToolTip("Move selected ability down");
	abilityDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	abilitiesButtonLayout->addWidget(addAbility);
	abilitiesButtonLayout->addWidget(editAbility);
	abilitiesButtonLayout->addWidget(deleteAbility);
	abilitiesButtonLayout->addSpacing(20);
	abilitiesButtonLayout->addWidget(abilityUpButton);
	abilitiesButtonLayout->addWidget(abilityDownButton);
	abilitiesLayout->addWidget(_classAbLabel);
	abilitiesLayout->addWidget(_classAbilities);
	abilitiesLayout->addWidget(abilitiesButtonFrame);

	_layout->addWidget(selection);
	_layout->addWidget(description);
	_layout->addWidget(attributes);
	_layout->addWidget(abilities);

	connect(_classList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(classSelected(const QString &)));
	connect(newClass, SIGNAL(clicked()), this, SLOT(newClass()));
	connect(editClass, SIGNAL(clicked()), this, SLOT(editClass()));
	connect(deleteClass, SIGNAL(clicked()), this, SLOT(deleteClass()));
	connect(classUpButton, SIGNAL(clicked()), this, SLOT(moveClassUp()));
	connect(classDownButton, SIGNAL(clicked()), this, SLOT(moveClassDown()));
	connect(saveDescription, SIGNAL(clicked()), this, SLOT(saveClassDescription()));
	connect(reloadDescription, SIGNAL(clicked()), this, SLOT(reloadClassDescription()));
	connect(editAttribute, SIGNAL(clicked()), this, SLOT(editAttribute()));
	connect(addAbility, SIGNAL(clicked()), this, SLOT(newAbility()));
	connect(editAbility, SIGNAL(clicked()), this, SLOT(editAbility()));
	connect(deleteAbility, SIGNAL(clicked()), this, SLOT(deleteAbility()));
	connect(abilityUpButton, SIGNAL(clicked()), this, SLOT(moveAbilityUp()));
	connect(abilityDownButton, SIGNAL(clicked()), this, SLOT(moveAbilityDown()));

}

void ClassesWindow::loadClasses() {
	_classList->clear();
	QVector<Class*> classes = _properties->getClasses()->getClasses();
	for (int i = 0; i < classes.size(); i++) {
		Class *it = classes.at(i);
		_classList->addItem(it->getName());
	}

	if (_classList->count() > 0)
		_classList->setCurrentRow(0);
}

void ClassesWindow::updateWindow() {
	if ((_classList->count() > 0) && !!_classList->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat)
			classSelected(cat->getName());
	}
}

void ClassesWindow::classSelected(const QString &name) {
	if (name != QString()) {
		Class *cat = _properties->getClasses()->getClass(name);
		if (!!cat) {
			_classDescLabel->clear();
			_classDescLabel->setText("Description of " + cat->getName());
			_classDesc->clear();
			_classDesc->setPlainText(cat->getDescription());

			_classAttLabel->clear();
			_classAttLabel->setText("Attributes of " + cat->getName());
			_classAttributes->clear();
			QVector<QPair<Attribute*, QPair<int, double>>> attributes = cat->getAttributes();
			for (int i = 0; i < attributes.size(); i++) {
				QPair<Attribute*, QPair<int, double>> it = attributes.at(i);
				_classAttributes->addItem(it.first->getName() + " - initial value: " + QString::number(it.second.first) + " - multiplier: " + QString::number(it.second.second));
			}

			_classAbLabel->clear();
			_classAbLabel->setText("Abilities of " + cat->getName());
			_classAbilities->clear();
			QVector<QPair<Ability*, int>> abilities = cat->getAbilities();
			for (int i = 0; i < abilities.size(); i++) {
				QPair<Ability*, int> it = abilities.at(i);
				_classAbilities->addItem(it.first->getName() + " - level " + QString::number(it.second));
			}
		}
	}
}

void ClassesWindow::newClass() {
	// Setup Dialog
	QDialog classWindow(0);
	classWindow.setWindowTitle("Add a new class");
	QVBoxLayout *layout = new QVBoxLayout(&classWindow);

	QLabel *label1 = new QLabel("Enter a name for the class:", &classWindow);
	QLineEdit *nameField = new QLineEdit(&classWindow);

	QLabel *label2 = new QLabel("Select a primary attribute:", &classWindow);
	QComboBox *attributeSelection = new QComboBox(&classWindow);
	attributeSelection->setInsertPolicy(QComboBox::InsertAlphabetically);

	QFrame *buttonframe = new QFrame(&classWindow);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
	QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
	buttonlayout->addWidget(acceptbutton);
	buttonlayout->addWidget(cancelbutton);

	layout->addWidget(label1);
	layout->addWidget(nameField);
	layout->addWidget(label2);
	layout->addWidget(attributeSelection);
	layout->addWidget(buttonframe);

	connect(acceptbutton, SIGNAL(clicked()), &classWindow, SLOT(accept()));
	connect(cancelbutton, SIGNAL(clicked()), &classWindow, SLOT(reject()));

	attributeSelection->addItem(QString());
	QVector<Attribute*> attributes = _properties->getAttributes()->getOffensiveAttributes();
	for (int i = 0; i < attributes.size(); i++)
		attributeSelection->addItem(attributes.at(i)->getName());

	// Execute dialog
	if (classWindow.exec()) {
		QString name = nameField->text();

		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newClass();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("A class should not have an empty name."));
			newClass();
			return;
		}

		if (!!_properties->getClasses()->getClass(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("A class with the same name already exists."));
			newClass();
			return;
		}

		Class *cat = Class::create(name);
		try {
			_properties->getClasses()->addClass(cat);

			if (attributeSelection->currentIndex() == 0)
				cat->unsetPrimaryAttribute();
			else
				cat->setPrimaryAttribute(_properties->getAttributes()->getOffensiveAttribute(attributeSelection->currentText()));

		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete cat;
			return;
		}

		_classList->addItem(cat->getName());
		_classList->setCurrentRow(_classList->count()-1);
		emit updateSignal();
	}
}

void ClassesWindow::editClass() {
	if ((_classList->count() > 0) && !!_classList->currentItem()) {

		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {
			// Setup Dialog
			QDialog classWindow(0);
			classWindow.setWindowTitle("Edit class");
			QVBoxLayout *layout = new QVBoxLayout(&classWindow);

			QLabel *label1 = new QLabel("Enter a name for the class:", &classWindow);
			QLineEdit *nameField = new QLineEdit(&classWindow);

			QLabel *label2 = new QLabel("Select a primary attribute:", &classWindow);
			QComboBox *attributeSelection = new QComboBox(&classWindow);
			attributeSelection->setInsertPolicy(QComboBox::InsertAlphabetically);

			QFrame *buttonframe = new QFrame(&classWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label1);
			layout->addWidget(nameField);
			layout->addWidget(label2);
			layout->addWidget(attributeSelection);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &classWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &classWindow, SLOT(reject()));

			attributeSelection->addItem(QString());
			QVector<Attribute*> attributes = _properties->getAttributes()->getOffensiveAttributes();
			for (int i = 0; i < attributes.size(); i++)
				attributeSelection->addItem(attributes.at(i)->getName());

			nameField->setText(cat->getName());

			if (!cat->getPrimaryAttribute())
				attributeSelection->setCurrentIndex(0);
			else {
				for (int i = 0; i < attributeSelection->count(); i++) {
					if (attributeSelection->itemText(i) == cat->getPrimaryAttribute()->getName()) {
						attributeSelection->setCurrentIndex(i);
						break;
					}
				}
			}

			if (classWindow.exec()) {
				QString name = nameField->text();

				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editClass();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("A class should not have an empty name."));
					editClass();
					return;
				}

				if ((name != cat->getName()) && !!_properties->getClasses()->getClass(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("A class with the same name already exists."));
					editClass();
					return;
				}

				cat->setName(name);
				if (attributeSelection->currentIndex() == 0)
					cat->unsetPrimaryAttribute();
				else
					cat->setPrimaryAttribute(_properties->getAttributes()->getOffensiveAttribute(attributeSelection->currentText()));

				int classIndex = _classList->currentRow();
				loadClasses();

				if (_classList->count() > 0)
					_classList->setCurrentRow(classIndex);

				emit updateSignal();
			}
		}
	}
}

void ClassesWindow::deleteClass() {
	if ((_classList->count() > 0) && !!_classList->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {
			QString currentClass = cat->getName();
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete class",
				"Are you sure you want to delete the class?\nDeleting a class will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_properties->getClasses()->removeClass(cat->getName());
				loadClasses();

				if (_classList->count() > 0)
					_classList->setCurrentRow(0);

				emit updateSignal();
			}
		}
	}
}

void ClassesWindow::moveClassUp() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classList->currentRow() != 0)) {

		Class *currentClass = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!currentClass) {
			const QVector<Class*>& classes = _properties->getClasses()->getClasses();
			int index = classes.indexOf(currentClass);

			try {
				switchPlaces(const_cast<QVector<Class*>&>(classes), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadClasses();
			_classList->setCurrentRow(index-1);
		}
	}
}

void ClassesWindow::moveClassDown() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classList->currentRow() != _classList->count()-1)) {

		Class *currentClass = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!currentClass) {
			const QVector<Class*>& classes = _properties->getClasses()->getClasses();
			int index = classes.indexOf(currentClass);

			try {
				switchPlaces(const_cast<QVector<Class*>&>(classes), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadClasses();
			_classList->setCurrentRow(index+1);
		}
	}
}

void ClassesWindow::saveClassDescription() {
	if ((_classList->count() > 0) && !!_classList->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {
			cat->setDescription(_classDesc->toPlainText());
		}
	}
}

void ClassesWindow::reloadClassDescription() {
	if ((_classList->count() > 0) && !!_classList->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {
			_classDesc->setPlainText(cat->getDescription());
		}
	}
}

void ClassesWindow::editAttribute() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classAttributes->count() > 0) && !!_classAttributes->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {

			QPair<Attribute*, QPair<int, double>> attribute = cat->getAttribute(_classAttributes->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog attributeWindow(0);
			attributeWindow.setWindowTitle("Edit attribute of " + cat->getName());
			QVBoxLayout *layout = new QVBoxLayout(&attributeWindow);

			QLabel *label1 = new QLabel("Edit the initial value of attribute" + attribute.first->getName() + ":", &attributeWindow);
			QFrame *initialFrame = new QFrame(&attributeWindow);
			QHBoxLayout *initialLayout = new QHBoxLayout(initialFrame);
			QLabel *initialLabel = new QLabel(attribute.first->getName() + " +", initialFrame);
			QSpinBox *initialSelection = new QSpinBox(initialFrame);
			initialSelection->setMinimum(1);
			initialSelection->setMaximum(100);
			initialSelection->setSingleStep(1);
			initialSelection->setValue(attribute.second.first);
			initialLayout->addWidget(initialLabel);
			initialLayout->addSpacing(20);
			initialLayout->addWidget(initialSelection);

			QLabel *label2 = new QLabel("Edit the multiplier for attribute " + attribute.first->getName() + ":", &attributeWindow);
			QFrame *setupFrame = new QFrame(&attributeWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QLabel *setupLabel = new QLabel(attribute.first->getName() + " x", setupFrame);
			QDoubleSpinBox *multiplierSelection = new QDoubleSpinBox(setupFrame);
			multiplierSelection->setMinimum(0.0);
			multiplierSelection->setMaximum(10.0);
			multiplierSelection->setSingleStep(0.1);
			multiplierSelection->setDecimals(1);
			multiplierSelection->setValue(attribute.second.second);
			setupLayout->addWidget(setupLabel);
			setupLayout->addSpacing(20);
			setupLayout->addWidget(multiplierSelection);

			QFrame *buttonFrame = new QFrame(&attributeWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonFrame);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonFrame);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonFrame);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label1);
			layout->addWidget(initialFrame);
			layout->addWidget(label2);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonFrame);

			connect(acceptbutton, SIGNAL(clicked()), &attributeWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &attributeWindow, SLOT(reject()));

			if (attributeWindow.exec()) {
				cat->editAttribute(attribute.first->getName(), initialSelection->value(), multiplierSelection->value());
				_classAttributes->currentItem()->setText(attribute.first->getName() + " - initial value: " + QString::number(initialSelection->value()) + " - multiplier: " + QString::number(multiplierSelection->value()));
			}
		}
	}
}

void ClassesWindow::newAbility() {
	if ((_classList->count() > 0) && !!_classList->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {

			// Setup window
			QDialog abilityWindow(0);
			abilityWindow.setWindowTitle("Add ability to " + cat->getName());
			QVBoxLayout *layout = new QVBoxLayout(&abilityWindow);

			QLabel *label = new QLabel("Select an ability and set the acquisition level:", &abilityWindow);

			QFrame *setupFrame = new QFrame(&abilityWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QComboBox *abilitySelection = new QComboBox(setupFrame);
			abilitySelection->setInsertPolicy(QComboBox::InsertAlphabetically);
			QSpinBox *levelSelection = new QSpinBox(setupFrame);
			levelSelection->setMinimum(0);
			levelSelection->setMaximum(99);
			levelSelection->setSingleStep(1);
			setupLayout->addWidget(abilitySelection);
			setupLayout->addSpacing(20);
			setupLayout->addWidget(levelSelection);

			QFrame *buttonframe = new QFrame(&abilityWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &abilityWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &abilityWindow, SLOT(reject()));

			QVector<QPair<Ability*, int>> classAbilities = cat->getAbilities();
			QVector<Ability*> abilities = _properties->getAbilities()->getAbilities();
			for (int i = 0; i < abilities.size(); i++) {
				Ability *it = abilities.at(i);
				for (int j = 0; j < classAbilities.size(); j++) {
					if (classAbilities.at(j).first == it)
						goto skip;
				}

				abilitySelection->addItem(it->getName());

				skip:;
			}

			if (abilitySelection->count() == 0) {
				QMessageBox::information(this, tr("No abilities available"), tr("There are no more abilities available to choose from."));
				return;
			}

			if (abilityWindow.exec()) {
				Ability *ability = _properties->getAbilities()->getAbility(abilitySelection->currentText());
				int acquire = levelSelection->value();

				if (!!ability) {
					try {
						cat->addAbility(ability, acquire);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
						return;
					}

					_classAbilities->addItem(ability->getName() + " - level " + QString::number(acquire));
				}
			}
		}
	}
}

void ClassesWindow::editAbility() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classAbilities->count() > 0) && !!_classAbilities->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {

			QPair<Ability*, int> ability = cat->getAbility(_classAbilities->currentItem()->text().split(" - ").at(0));

			// Setup window
			QDialog abilityWindow(0);
			abilityWindow.setWindowTitle("Edit ability of " + cat->getName());
			QVBoxLayout *layout = new QVBoxLayout(&abilityWindow);

			QLabel *label = new QLabel("Edit the acquisition for ability " + ability.first->getName() + ":", &abilityWindow);

			QFrame *setupFrame = new QFrame(&abilityWindow);
			QHBoxLayout *setupLayout = new QHBoxLayout(setupFrame);
			QLabel *attributeLabel = new QLabel(ability.first->getName() + " is acquired at level ", setupFrame);
			QSpinBox *levelSelection = new QSpinBox(setupFrame);
			levelSelection->setMinimum(0);
			levelSelection->setMaximum(99);
			levelSelection->setSingleStep(1);
			levelSelection->setValue(ability.second);
			setupLayout->addWidget(attributeLabel);
			setupLayout->addSpacing(20);
			setupLayout->addWidget(levelSelection);

			QFrame *buttonframe = new QFrame(&abilityWindow);
			QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
			QPushButton *acceptbutton = new QPushButton("Accept", buttonframe);
			QPushButton *cancelbutton = new QPushButton("Cancel", buttonframe);
			buttonlayout->addWidget(acceptbutton);
			buttonlayout->addWidget(cancelbutton);

			layout->addWidget(label);
			layout->addWidget(setupFrame);
			layout->addWidget(buttonframe);

			connect(acceptbutton, SIGNAL(clicked()), &abilityWindow, SLOT(accept()));
			connect(cancelbutton, SIGNAL(clicked()), &abilityWindow, SLOT(reject()));

			if (abilityWindow.exec()) {
				cat->editAbility(ability.first->getName(), levelSelection->value());
				_classAbilities->currentItem()->setText(ability.first->getName() + " - level " + QString::number(levelSelection->value()));
			}
		}
	}
}

void ClassesWindow::deleteAbility() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classAbilities->count() > 0) && !!_classAbilities->currentItem()) {
		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {
			cat->removeAbility(_classAbilities->currentItem()->text().split(" - ").at(0));
			_classAbilities->takeItem(_classAbilities->currentRow());
		}
	}
}

void ClassesWindow::moveAbilityUp() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classAbilities->count() > 0) && !!_classAbilities->currentItem() && (_classAbilities->currentRow() != 0)) {

		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {

			QPair<Ability*, int> ability = cat->getAbilities().at(_classAbilities->currentRow());
			if (!!ability.first) {
				const QVector<QPair<Ability*, int>>& abilities = cat->getAbilities();
				int index = abilities.indexOf(ability);

				try {
					switchPairs(const_cast<QVector<QPair<Ability*, int>>&>(abilities), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int classIndex = _classList->currentRow();
				loadClasses();
				_classList->setCurrentRow(classIndex);
				_classAbilities->setCurrentRow(index-1);
			}
		}
	}
}

void ClassesWindow::moveAbilityDown() {
	if ((_classList->count() > 0) && !!_classList->currentItem() && (_classAbilities->count() > 0) && !!_classAbilities->currentItem() && (_classAbilities->currentRow() != _classAbilities->count()-1)) {

		Class *cat = _properties->getClasses()->getClass(_classList->currentItem()->text());
		if (!!cat) {

			QPair<Ability*, int> ability = cat->getAbilities().at(_classAbilities->currentRow());
			if (!!ability.first) {
				const QVector<QPair<Ability*, int>>& abilities = cat->getAbilities();
				int index = abilities.indexOf(ability);

				try {
					switchPairs(const_cast<QVector<QPair<Ability*, int>>&>(abilities), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int classIndex = _classList->currentRow();
				loadClasses();
				_classList->setCurrentRow(classIndex);
				_classAbilities->setCurrentRow(index+1);
			}
		}
	}
}