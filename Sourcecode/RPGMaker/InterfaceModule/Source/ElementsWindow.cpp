#include "qgroupbox.h"
#include "qpushbutton.h"
#include "qstringlist.h"
#include "qmessagebox.h"
#include "qinputdialog.h"
#include "qerrormessage.h"
#include "../Headers/ElementsWindow.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using projectcomponents::ProjectException;
using namespace interfacemodule;

ElementsWindow::ElementsWindow(QWidget *parent, Properties *properties)
	: QFrame(parent), _properties(properties) {

		_layout = new QHBoxLayout(this);

		createElementsBox();
		createVulnerableBox();
		createResistantBox();

		loadElements();

		connect(_elementsList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(elementSelected(const QString &)));
}

ElementsWindow::~ElementsWindow() {}

void ElementsWindow::createElementsBox() {
	QGroupBox *elementsBox = new QGroupBox("Element", this);
	QLabel *elementsLabel = new QLabel("Select an element:", elementsBox);
	_elementsList = new QListWidget(elementsBox);

	QFrame *buttonFrame = new QFrame(elementsBox);
	QHBoxLayout *elementsButtonLayout = new QHBoxLayout(buttonFrame);
	elementsButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *elementsNewButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonFrame);
	elementsNewButton->setToolTip("Add a new element");
	elementsNewButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementsEditButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonFrame);
	elementsEditButton->setToolTip("Edit the selected element");
	elementsEditButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementsDeleteButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
	elementsDeleteButton->setToolTip("Delete the selected element");
	elementsDeleteButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementsUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", buttonFrame);
	elementsUpButton->setToolTip("Move selected element up");
	elementsUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *elementsDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", buttonFrame);
	elementsDownButton->setToolTip("Move selected element down");
	elementsDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	elementsButtonLayout->addWidget(elementsNewButton);
	elementsButtonLayout->addWidget(elementsEditButton);
	elementsButtonLayout->addWidget(elementsDeleteButton);
	elementsButtonLayout->addSpacing(20);
	elementsButtonLayout->addWidget(elementsUpButton);
	elementsButtonLayout->addWidget(elementsDownButton);

	QVBoxLayout *elementsLayout = new QVBoxLayout(elementsBox);
	elementsLayout->addWidget(elementsLabel);
	elementsLayout->addWidget(_elementsList);
	elementsLayout->addWidget(buttonFrame);

	_layout->addWidget(elementsBox);

	connect(elementsNewButton, SIGNAL(clicked()), this, SLOT(newElement()));
	connect(elementsEditButton, SIGNAL(clicked()), this, SLOT(editElement()));
	connect(elementsDeleteButton, SIGNAL(clicked()), this, SLOT(deleteElement()));
	connect(elementsUpButton, SIGNAL(clicked()), this, SLOT(moveElementUp()));
	connect(elementsDownButton, SIGNAL(clicked()), this, SLOT(moveElementDown()));
}

void ElementsWindow::createVulnerableBox() {
	QGroupBox *vulnerableBox = new QGroupBox("Vulnerable", this);
	_vulnerableLabel = new QLabel("No element selected", vulnerableBox);
	_vulnerableList = new QListWidget(vulnerableBox);

	QPushButton *vulnerableAddButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", vulnerableBox);
	vulnerableAddButton->setToolTip("Add an element to the list");
	vulnerableAddButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *vulnerableDeleteButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", vulnerableBox);
	vulnerableDeleteButton->setToolTip("Delete the selected element from the list");
	vulnerableDeleteButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *vulnerableUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", vulnerableBox);
	vulnerableUpButton->setToolTip("Move selected element up");
	vulnerableUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *vulnerableDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", vulnerableBox);
	vulnerableDownButton->setToolTip("Move selected element down");
	vulnerableDownButton->setMaximumSize(g_buttonsize, g_buttonsize);

	QFrame *buttonFrame = new QFrame(vulnerableBox);
	QHBoxLayout *vulnerableButtonLayout = new QHBoxLayout(buttonFrame);
	vulnerableButtonLayout->setAlignment(Qt::AlignCenter);
	vulnerableButtonLayout->addWidget(vulnerableAddButton);
	vulnerableButtonLayout->addWidget(vulnerableDeleteButton);
	vulnerableButtonLayout->addSpacing(20);
	vulnerableButtonLayout->addWidget(vulnerableUpButton);
	vulnerableButtonLayout->addWidget(vulnerableDownButton);

	QVBoxLayout *vulnerableLayout = new QVBoxLayout(vulnerableBox);
	vulnerableLayout->addWidget(_vulnerableLabel);
	vulnerableLayout->addWidget(_vulnerableList);
	vulnerableLayout->addWidget(buttonFrame);

	_layout->addWidget(vulnerableBox);

	connect(vulnerableAddButton, SIGNAL(clicked()), this, SLOT(addVulnerableElement()));
	connect(vulnerableDeleteButton, SIGNAL(clicked()), this, SLOT(deleteVulnerableElement()));
	connect(vulnerableUpButton, SIGNAL(clicked()), this, SLOT(moveVulnerableElementUp()));
	connect(vulnerableDownButton, SIGNAL(clicked()), this, SLOT(moveVulnerableElementDown()));
}

void ElementsWindow::createResistantBox() {
	QGroupBox *resistantBox = new QGroupBox("Resistant", this);
	_resistantLabel = new QLabel("No element selected", resistantBox);
	_resistantList = new QListWidget(resistantBox);

	QPushButton *resistantAddButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", resistantBox);
	resistantAddButton->setToolTip("Add an element to the list");
	resistantAddButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *resistantDeleteButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", resistantBox);
	resistantDeleteButton->setToolTip("Delete the selected element from the list");
	resistantDeleteButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *resistantUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", resistantBox);
	resistantUpButton->setToolTip("Move selected element up");
	resistantUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *resistantDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", resistantBox);
	resistantDownButton->setToolTip("Move selected element down");
	resistantDownButton->setMaximumSize(g_buttonsize, g_buttonsize);

	QFrame *buttonFrame = new QFrame(resistantBox);
	QHBoxLayout *resistantButtonLayout = new QHBoxLayout(buttonFrame);
	resistantButtonLayout->setAlignment(Qt::AlignCenter);
	resistantButtonLayout->addWidget(resistantAddButton);
	resistantButtonLayout->addWidget(resistantDeleteButton);
	resistantButtonLayout->addSpacing(20);
	resistantButtonLayout->addWidget(resistantUpButton);
	resistantButtonLayout->addWidget(resistantDownButton);

	QVBoxLayout *resistantLayout = new QVBoxLayout(resistantBox);
	resistantLayout->addWidget(_resistantLabel);
	resistantLayout->addWidget(_resistantList);
	resistantLayout->addWidget(buttonFrame);

	_layout->addWidget(resistantBox);

	connect(resistantAddButton, SIGNAL(clicked()), this, SLOT(addResistantElement()));
	connect(resistantDeleteButton, SIGNAL(clicked()), this, SLOT(deleteResistantElement()));
	connect(resistantUpButton, SIGNAL(clicked()), this, SLOT(moveResistantElementUp()));
	connect(resistantDownButton, SIGNAL(clicked()), this, SLOT(moveResistantElementDown()));
}

void ElementsWindow::loadElements() {
	_elementsList->clear();
	QVector<Element*> elements = _properties->getElements()->getElements();
	for (int i = 0; i < elements.size(); i++) {
		Element *it = elements.at(i);
		_elementsList->addItem(it->getName());
	}
}

void ElementsWindow::elementSelected(const QString &name) {
	if (name != QString()) {
		_vulnerableList->clear();
		_resistantList->clear();
		_vulnerableLabel->clear();
		_resistantLabel->clear();

		Element *element = _properties->getElements()->getElement(name);

		if (!!element) {
			_vulnerableLabel->setText(name + " is vulnerable to");
			_resistantLabel->setText(name + " is resistant to");

			QVector<Element*> vulnerables = element->getVulnerableElements();
			QVector<Element*> resistants = element->getResistantElements();

			for (int i = 0; i < vulnerables.size(); i++) {
				Element *it = vulnerables.at(i);
				_vulnerableList->addItem(it->getName());
			}

			for (int i = 0; i < resistants.size(); i++) {
				Element *it = resistants.at(i);
				_resistantList->addItem(it->getName());
			}
		}
	} 
}

void ElementsWindow::newElement() {
	bool accepted;
	QString name = QInputDialog::getText(this, tr("New element"),
		tr("Enter a name for the new element:"), QLineEdit::Normal, QString(""), &accepted);

	if (accepted) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newElement();
			return;
		}

		if (name == QString()) {
			QErrorMessage *errorDialog = QErrorMessage::qtHandler();
			errorDialog->showMessage("An element should not have an empty name.");
			newElement();
			return;
		}

		if (!!_properties->getElements()->getElement(name)) {
			QErrorMessage *errorDialog = QErrorMessage::qtHandler();
			errorDialog->showMessage("An element with the same name already exists.");
			newElement();
			return;
		}

		Element *newElement = Element::create(name);

		try {
			_properties->getElements()->addElement(newElement);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete newElement;
			return;
		}

		_elementsList->addItem(name);
		_elementsList->setCurrentRow(_elementsList->count() -1);
		emit updateSignal();
	}
}

void ElementsWindow::editElement() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem()) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			bool accepted;
			QString newName = QInputDialog::getText(this, tr("Edit element"),
				tr("Enter a new name for the element:"), QLineEdit::Normal, currentElement->getName(), &accepted);

			if (accepted && (newName != currentElement->getName())) {
				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(newName, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editElement();
					return;
				}

				if (newName == QString()) {
					QMessageBox::critical(this, "No empty name allowed", "An element should not have an empty name.");
					editElement();
					return;
				}

				if (!!_properties->getElements()->getElement(newName)) {
					QMessageBox::critical(this, "Name already in use", "An element with the same name already exists.");
					editElement();
					return;
				}

				try {
					currentElement->setName(newName);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int currentIndex = _elementsList->currentRow();
				loadElements();
				
				for (int i = 0; i < _elementsList->count(); i++) {
					if (_elementsList->item(i)->text() == newName) {
						_elementsList->setCurrentRow(i);
						break;
					}
				}

				emit updateSignal();
			}
		}
	}
}

void ElementsWindow::deleteElement() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem()) {

		QString currentElement = _elementsList->currentItem()->text();
		Element *element = _properties->getElements()->getElement(currentElement);
		if (!!element) {

			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete element",
				"Are you sure you want to delete the element?\nDeleting an element will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_properties->getElements()->removeElement(currentElement);
				loadElements();

				if (_elementsList->count() > 0)
					_elementsList->setCurrentRow(0);

				emit updateSignal();
			}
		}
	}
}

void ElementsWindow::moveElementUp() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_elementsList->currentRow() != 0)) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			const QVector<Element*>& elements = _properties->getElements()->getElements();
			int index = elements.indexOf(currentElement);

			try {
				switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadElements();
			_elementsList->setCurrentRow(index-1);
		}
	}
}

void ElementsWindow::moveElementDown() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_elementsList->currentRow() != _elementsList->count()-1)) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			const QVector<Element*>& elements = _properties->getElements()->getElements();
			int index = elements.indexOf(currentElement);

			try {
				switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadElements();
			_elementsList->setCurrentRow(index+1);
		}
	}
}

void ElementsWindow::addVulnerableElement() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem()) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			QVector<Element*> elements = _properties->getElements()->getElements();
			QVector<Element*> resistantElements = currentElement->getResistantElements();
			QVector<Element*> vulnerableElements = currentElement->getVulnerableElements();

			removeComponent<Element>(elements, currentElement->getName());
			for(int i = 0; i < resistantElements.size(); i++) {
				removeComponent<Element>(elements, resistantElements.at(i)->getName());
			}
			for (int i = 0; i < vulnerableElements.size(); i++) {
				removeComponent<Element>(elements, vulnerableElements.at(i)->getName());
			}

			QStringList availableElements;
			for (int i = 0; i < elements.size(); i++) {
				availableElements.append(elements.at(i)->getName());
			}

			if (availableElements.size() == 0) {
				QMessageBox::information(0, "No elements available", "There are no more possible elements to select.");
				return;
			}

			bool accepted;
			QString name = QInputDialog::getItem(this, "Add a vulnerable element to " + currentElement->getName(), "Select an element:", availableElements, 0, false, &accepted);

			if (accepted) {
				if (name == QString()) {
					QErrorMessage *errorDialog = QErrorMessage::qtHandler();
					errorDialog->showMessage("Selected an unknown element.");
					return;
				}

				Element *element = _properties->getElements()->getElement(name);
				try {
					currentElement->addVulnerableElement(element);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_vulnerableList->addItem(name);
				_vulnerableList->setCurrentRow(_vulnerableList->count() -1);
			}
		}
	}
}

void ElementsWindow::deleteVulnerableElement() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_vulnerableList->count() > 0) && !!_vulnerableList->currentItem()) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			try {
				currentElement->removeVulnerableElement(_vulnerableList->currentItem()->text());
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			_vulnerableList->takeItem(_vulnerableList->currentRow());
				
			if (_vulnerableList->count() > 0)
				_vulnerableList->setCurrentRow(0);
		}
	}
}

void ElementsWindow::moveVulnerableElementUp() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_vulnerableList->count() > 0) && !!_vulnerableList->currentItem() && (_vulnerableList->currentRow() != 0)) {

		Element *element = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!element) {

			Element *currentElement = element->getVulnerableElements().at(_vulnerableList->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = element->getVulnerableElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int elementIndex = _elementsList->currentRow();
				loadElements();
				_elementsList->setCurrentRow(elementIndex);
				_vulnerableList->setCurrentRow(index-1);
			}
		}
	}
}

void ElementsWindow::moveVulnerableElementDown() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_vulnerableList->count() > 0) && !!_vulnerableList->currentItem() && (_vulnerableList->currentRow() != _vulnerableList->count()-1)) {

		Element *element = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!element) {

			Element *currentElement = element->getVulnerableElements().at(_vulnerableList->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = element->getVulnerableElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int elementIndex = _elementsList->currentRow();
				loadElements();
				_elementsList->setCurrentRow(elementIndex);
				_vulnerableList->setCurrentRow(index+1);
			}
		}
	}
}

void ElementsWindow::addResistantElement() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem()) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			QVector<Element*> elements = _properties->getElements()->getElements();
			QVector<Element*> resistantElements = currentElement->getResistantElements();
			QVector<Element*> vulnerableElements = currentElement->getVulnerableElements();

			removeComponent<Element>(elements, currentElement->getName());
			for(int i = 0; i < resistantElements.size(); i++) {
				removeComponent<Element>(elements, resistantElements.at(i)->getName());
			}
			for (int i = 0; i < vulnerableElements.size(); i++) {
				removeComponent<Element>(elements, vulnerableElements.at(i)->getName());
			}

			QStringList availableElements;
			for (int i = 0; i < elements.size(); i++) {
				availableElements.append(elements.at(i)->getName());
			}

			if (availableElements.size() == 0) {
				QMessageBox::information(0, "No elements available", "There are no more possible elements to select.");
				return;
			}

			bool accepted;
			QString name = QInputDialog::getItem(this, "Add a resistant element to " + currentElement->getName(), "Select an element:", availableElements, 0, false, &accepted);

			if (accepted) {
				if (name == QString()) {
					QErrorMessage *errorDialog = QErrorMessage::qtHandler();
					errorDialog->showMessage("Selected an unknown element.");
					return;
				}

				Element *element = _properties->getElements()->getElement(name);
				try {
					currentElement->addResistantElement(element);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_resistantList->addItem(name);
				_resistantList->setCurrentRow(_resistantList->count()-1);
			}
		}
	}
}

void ElementsWindow::deleteResistantElement() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_resistantList->count() > 0) && !!_resistantList->currentItem()) {

		Element *currentElement = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!currentElement) {
			try {
				currentElement->removeResistantElement(_resistantList->currentItem()->text());
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			_resistantList->takeItem(_resistantList->currentRow());

			if (_resistantList->count() > 0)
				_resistantList->setCurrentRow(0);
		}
	}
}

void ElementsWindow::moveResistantElementUp() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_resistantList->count() > 0) && !!_resistantList->currentItem() && (_resistantList->currentRow() != 0)) {

		Element *element = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!element) {

			Element *currentElement = element->getResistantElements().at(_resistantList->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = element->getResistantElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int elementIndex = _elementsList->currentRow();
				loadElements();
				_elementsList->setCurrentRow(elementIndex);
				_resistantList->setCurrentRow(index-1);
			}
		}
	}
}

void ElementsWindow::moveResistantElementDown() {
	if ((_elementsList->count() > 0) && !!_elementsList->currentItem() && (_resistantList->count() > 0) && !!_resistantList->currentItem() && (_resistantList->currentRow() != _vulnerableList->count()-1)) {

		Element *element = _properties->getElements()->getElement(_elementsList->currentItem()->text());
		if (!!element) {

			Element *currentElement = element->getResistantElements().at(_resistantList->currentRow());
			if (!!currentElement) {
				const QVector<Element*>& elements = element->getResistantElements();
				int index = elements.indexOf(currentElement);

				try {
					switchPlaces(const_cast<QVector<Element*>&>(elements), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int elementIndex = _elementsList->currentRow();
				loadElements();
				_elementsList->setCurrentRow(elementIndex);
				_resistantList->setCurrentRow(index+1);
			}
		}
	}
}