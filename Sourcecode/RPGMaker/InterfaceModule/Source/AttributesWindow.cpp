#include "qdialog.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qstringlist.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/AttributesWindow.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;

AttributesWindow::AttributesWindow(QWidget *parent, AttributeSystem *attributes)
	: QFrame(parent), _attributes(attributes) {

		_layout = new QHBoxLayout(this);

		createWindow();

		loadAttributes();
}

AttributesWindow::~AttributesWindow() {}

void AttributesWindow::createWindow() {

	// Standard attributes
	QFrame *standardAttributesFrame = new QFrame(this);
	QVBoxLayout *standardAttributesLayout = new QVBoxLayout(standardAttributesFrame);
	standardAttributesLayout->setAlignment(Qt::AlignTop);
	_layout->addWidget(standardAttributesFrame);

	QLabel *standardLabel = new QLabel("Standard attributes:", standardAttributesFrame);
	standardAttributesLayout->addWidget(standardLabel);

	QFrame *lifesourceFrame = new QFrame(standardAttributesFrame);
	QHBoxLayout *lifesourceLayout = new QHBoxLayout(lifesourceFrame);
	_lifesourceLabel = new QLabel("", lifesourceFrame);
	QPushButton *editLifesource = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", lifesourceFrame);
	editLifesource->setToolTip("Edit the lifesource attribute");
	editLifesource->setMaximumSize(g_buttonsize, g_buttonsize);
	lifesourceLayout->addWidget(_lifesourceLabel);
	lifesourceLayout->addWidget(editLifesource);
	standardAttributesLayout->addWidget(lifesourceFrame);

	QFrame *energysourceFrame = new QFrame(standardAttributesFrame);
	QHBoxLayout *energysourceLayout = new QHBoxLayout(energysourceFrame);
	_energysourceLabel = new QLabel("", energysourceFrame);
	QPushButton *editEnergysource = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", energysourceFrame);
	editEnergysource->setToolTip("Edit the energysource attribute");
	editEnergysource->setMaximumSize(g_buttonsize, g_buttonsize);
	energysourceLayout->addWidget(_energysourceLabel);
	energysourceLayout->addWidget(editEnergysource);
	standardAttributesLayout->addWidget(energysourceFrame);

	QFrame *agilityforceFrame = new QFrame(standardAttributesFrame);
	QHBoxLayout *agilityforceLayout = new QHBoxLayout(agilityforceFrame);
	_agilityforceLabel = new QLabel("", agilityforceFrame);
	QPushButton *editAgilityforce = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", agilityforceFrame);
	editAgilityforce->setToolTip("Edit the agilityforce attribute");
	editAgilityforce->setMaximumSize(g_buttonsize, g_buttonsize);
	agilityforceLayout->addWidget(_agilityforceLabel);
	agilityforceLayout->addWidget(editAgilityforce);
	standardAttributesLayout->addWidget(agilityforceFrame);

	QFrame *evasionforceFrame = new QFrame(standardAttributesFrame);
	QHBoxLayout *evasionforceLayout = new QHBoxLayout(evasionforceFrame);
	_evasionforceLabel = new QLabel("", evasionforceFrame);
	QPushButton *editEvasionforce = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", evasionforceFrame);
	editEvasionforce->setToolTip("Edit the evasionforce attribute");
	editEvasionforce->setMaximumSize(g_buttonsize, g_buttonsize);
	evasionforceLayout->addWidget(_evasionforceLabel);
	evasionforceLayout->addWidget(editEvasionforce);
	standardAttributesLayout->addWidget(evasionforceFrame);

	// Offensive attributes
	QFrame *offensiveFrame = new QFrame(this);
	QVBoxLayout *offensiveLayout = new QVBoxLayout(offensiveFrame);
	QLabel *offensiveLabel = new QLabel("Offensive attributes:", offensiveFrame);
	_offensiveList = new QListWidget(offensiveFrame);
	QFrame *offensiveButtonframe = new QFrame(offensiveFrame);
	QHBoxLayout *offensiveButtonlayout = new QHBoxLayout(offensiveButtonframe);
	offensiveButtonlayout->setAlignment(Qt::AlignCenter);
	QPushButton *newOffensive = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", offensiveButtonframe);
	newOffensive->setToolTip("Add a new offensive attribute");
	newOffensive->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editOffensive = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", offensiveButtonframe);
	editOffensive->setToolTip("Edit the current offensive attribute");
	editOffensive->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteOffensive = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", offensiveButtonframe);
	deleteOffensive->setToolTip("Delete the current offensive attribute");
	deleteOffensive->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *offensiveUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", offensiveButtonframe);
	offensiveUpButton->setToolTip("Move selected attribute up");
	offensiveUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *offensiveDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", offensiveButtonframe);
	offensiveDownButton->setToolTip("Move selected attribute down");
	offensiveDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	offensiveButtonlayout->addWidget(newOffensive);
	offensiveButtonlayout->addWidget(editOffensive);
	offensiveButtonlayout->addWidget(deleteOffensive);
	offensiveButtonlayout->addSpacing(20);
	offensiveButtonlayout->addWidget(offensiveUpButton);
	offensiveButtonlayout->addWidget(offensiveDownButton);
	offensiveLayout->addWidget(offensiveLabel);
	offensiveLayout->addWidget(_offensiveList);
	offensiveLayout->addWidget(offensiveButtonframe);
	_layout->addWidget(offensiveFrame);

	// Defensive attributes
	QFrame *defensiveFrame = new QFrame(this);
	QVBoxLayout *defensiveLayout = new QVBoxLayout(defensiveFrame);
	QLabel *defensiveLabel = new QLabel("Defensive attributes:", defensiveFrame);
	_defensiveList = new QListWidget(defensiveFrame);
	QFrame *defensiveButtonframe = new QFrame(defensiveFrame);
	QHBoxLayout *defensiveButtonlayout = new QHBoxLayout(defensiveButtonframe);
	defensiveButtonlayout->setAlignment(Qt::AlignCenter);
	QPushButton *newDefensive = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", defensiveButtonframe);
	newDefensive->setToolTip("Add a new defensive attribute");
	newDefensive->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *editDefensive = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", defensiveButtonframe);
	editDefensive->setToolTip("Edit the current defensive attribute");
	editDefensive->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteDefensive = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", defensiveButtonframe);
	deleteDefensive->setToolTip("Delete the current defensive attribute");
	deleteDefensive->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *defensiveUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", defensiveButtonframe);
	defensiveUpButton->setToolTip("Move selected attribute up");
	defensiveUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *defensiveDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", defensiveButtonframe);
	defensiveDownButton->setToolTip("Move selected attribute down");
	defensiveDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
	defensiveButtonlayout->addWidget(newDefensive);
	defensiveButtonlayout->addWidget(editDefensive);
	defensiveButtonlayout->addWidget(deleteDefensive);
	defensiveButtonlayout->addSpacing(20);
	defensiveButtonlayout->addWidget(defensiveUpButton);
	defensiveButtonlayout->addWidget(defensiveDownButton);
	defensiveLayout->addWidget(defensiveLabel);
	defensiveLayout->addWidget(_defensiveList);
	defensiveLayout->addWidget(defensiveButtonframe);
	_layout->addWidget(defensiveFrame);

	// Link table
	QFrame *linking = new QFrame(this);
	QVBoxLayout *linkingLayout = new QVBoxLayout(linking);
	_layout->addWidget(linking);
	QLabel *linkingLabel = new QLabel("Linked attributes:", linking);
	linkingLayout->addWidget(linkingLabel);
	_linkTable = new QTableWidget(linking);
	_linkTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_linkTable->setSelectionBehavior(QAbstractItemView::SelectItems);
	_linkTable->setSelectionMode(QAbstractItemView::SingleSelection);
	linkingLayout->addWidget(_linkTable);
	QFrame *linkingButtonFrame = new QFrame(linking);
	QHBoxLayout *linkingButtonLayout = new QHBoxLayout(linkingButtonFrame);
	linkingButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *newLink = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", linkingButtonFrame);
	newLink->setToolTip("Add a new link");
	newLink->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *deleteLink = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", linkingButtonFrame);
	deleteLink->setToolTip("Delete the current link");
	deleteLink->setMaximumSize(g_buttonsize, g_buttonsize);
	linkingButtonLayout->addWidget(newLink);
	linkingButtonLayout->addWidget(deleteLink);
	linkingLayout->addWidget(linkingButtonFrame);

	connect(editLifesource, SIGNAL(clicked()), this, SLOT(editLifesource()));
	connect(editEnergysource, SIGNAL(clicked()), this, SLOT(editEnergysource()));
	connect(editAgilityforce, SIGNAL(clicked()), this, SLOT(editAgilityforce()));
	connect(editEvasionforce, SIGNAL(clicked()), this, SLOT(editEvasionforce()));
	connect(newOffensive, SIGNAL(clicked()), this, SLOT(newOffensive()));
	connect(editOffensive, SIGNAL(clicked()), this, SLOT(editOffensive()));
	connect(deleteOffensive, SIGNAL(clicked()), this, SLOT(deleteOffensive()));
	connect(offensiveUpButton, SIGNAL(clicked()), this, SLOT(moveOffensiveUp()));
	connect(offensiveDownButton, SIGNAL(clicked()), this, SLOT(moveOffensiveDown()));
	connect(newDefensive, SIGNAL(clicked()), this, SLOT(newDefensive()));
	connect(editDefensive, SIGNAL(clicked()), this, SLOT(editDefensive()));
	connect(deleteDefensive, SIGNAL(clicked()), this, SLOT(deleteDefensive()));
	connect(defensiveUpButton, SIGNAL(clicked()), this, SLOT(moveDefensiveUp()));
	connect(defensiveDownButton, SIGNAL(clicked()), this, SLOT(moveDefensiveDown()));
	connect(newLink, SIGNAL(clicked()), this, SLOT(addLink()));
	connect(deleteLink, SIGNAL(clicked()), this, SLOT(deleteLink()));
}

void AttributesWindow::loadAttributes() {
	_lifesourceLabel->clear();
	_energysourceLabel->clear();
	_agilityforceLabel->clear();
	_evasionforceLabel->clear();
	_offensiveList->clear();
	_defensiveList->clear();
	_linkTable->clear();

	_lifesourceLabel->setText("The lifesource attribute is set to: " + _attributes->getLifeSource()->getName());
	_energysourceLabel->setText("The energysource attribute is set to: " + _attributes->getEnergySource()->getName());
	_agilityforceLabel->setText("The agilityforce attribute is set to: " + _attributes->getAgilityForce()->getName());
	_evasionforceLabel->setText("The evasionforce attribute is set to: " + _attributes->getEvasionForce()->getName());
	
	QVector<Attribute*> offensiveAttributes = _attributes->getOffensiveAttributes();
	for (int i = 0; i < offensiveAttributes.size(); i++) {
		_offensiveList->addItem(offensiveAttributes.at(i)->getName());
	}

	QVector<Attribute*> defensiveAttributes = _attributes->getDefensiveAttributes();
	for (int i = 0; i < defensiveAttributes.size(); i++) {
		_defensiveList->addItem(defensiveAttributes.at(i)->getName());
	}

	loadLinks();
}

void AttributesWindow::loadLinks() {
	_linkTable->clear();
	_linkTable->setRowCount(0);
	_linkTable->setColumnCount(0);

	QVector<QPair<Attribute*, Attribute*>> links = _attributes->getLinks();
	_linkTable->setColumnCount(2);
	_linkTable->setRowCount(links.size());
	QStringList horizontalHeaders;
	horizontalHeaders << "Offensive" << "Defensive";
	_linkTable->setHorizontalHeaderLabels(horizontalHeaders);
	for(int i = 0; i < links.size(); i++) {
		_linkTable->setItem(i, 0, new QTableWidgetItem(links.at(i).first->getName()));
		_linkTable->setItem(i, 1, new QTableWidgetItem(links.at(i).second->getName()));
	}
}

void AttributesWindow::editLifesource() {
	Attribute *lifesource = _attributes->getLifeSource();
	bool accepted;
	QString name = QInputDialog::getText(this, tr("Edit lifesource attribute"),
		tr("Enter a name for the lifesource attribute:"), QLineEdit::Normal, _attributes->getLifeSource()->getName(), &accepted);

	if (accepted && (name != lifesource->getName())) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			editLifesource();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
			editLifesource();
			return;
		}

		
		if (_attributes->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
			editLifesource();
			return;
		}
		
		lifesource->setName(name);

		_lifesourceLabel->clear();
		_lifesourceLabel->setText("The lifesource attribute is set to: " + _attributes->getLifeSource()->getName());
		emit updateSignal();
	}
}

void AttributesWindow::editEnergysource() {
	Attribute *energysource = _attributes->getEnergySource();
	bool accepted;
	QString name = QInputDialog::getText(this, tr("Edit energysource attribute"),
		tr("Enter a name for the energysource attribute:"),
		QLineEdit::Normal, _attributes->getEnergySource()->getName(), &accepted);

	if (accepted && (name != energysource->getName())) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			editEnergysource();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
			editEnergysource();
			return;
		}

		if (_attributes->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
			editEnergysource();
			return;
		}

		_attributes->getEnergySource()->setName(name);

		_energysourceLabel->clear();
		_energysourceLabel->setText("The energysource attribute is set to: " + _attributes->getEnergySource()->getName());
		emit updateSignal();
	}
}

void AttributesWindow::editAgilityforce() {
	Attribute *agilityforce = _attributes->getAgilityForce();
	bool accepted;
	QString name = QInputDialog::getText(this, tr("Edit agilityforce attribute"),
		tr("Enter a name for the agilityforce attribute:"),
		QLineEdit::Normal, _attributes->getAgilityForce()->getName(), &accepted);

	if (accepted && (name != agilityforce->getName())) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			editAgilityforce();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
			editAgilityforce();
			return;
		}

		if (_attributes->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
			editAgilityforce();
			return;
		}
			
		_attributes->getAgilityForce()->setName(name);

		_agilityforceLabel->clear();
		_agilityforceLabel->setText("The agilityforce attribute is set to: " + _attributes->getAgilityForce()->getName());
		emit updateSignal();
	}
}

void AttributesWindow::editEvasionforce() {
	Attribute *evasionforce = _attributes->getEvasionForce();
	bool accepted;
	QString name = QInputDialog::getText(this, tr("Edit evasionforce attribute"),
		tr("Enter a name for the evasionforce attribute:"),
		QLineEdit::Normal, _attributes->getEvasionForce()->getName(), &accepted);
	
	if (accepted && (name != evasionforce->getName())) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			editEvasionforce();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
			editEvasionforce();
			return;
		}

		if (_attributes->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
			editEvasionforce();
			return;
		}

		_attributes->getEvasionForce()->setName(name);

		_evasionforceLabel->clear();
		_evasionforceLabel->setText("The evasionforce attribute is set to: " + _attributes->getEvasionForce()->getName());
		emit updateSignal();
	}
}

void AttributesWindow::newOffensive() {
	bool accepted;
	QString name = QInputDialog::getText(this, tr("New offensive attribute"),
		tr("Enter a name for the offensive attribute:"),
		QLineEdit::Normal, "" , &accepted);

	if (accepted) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newOffensive();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
			newOffensive();
			return;
		}

		if (_attributes->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
			newOffensive();
			return;
		}

		Attribute *newAttribute = Attribute::create(name, Attribute::OFFENSIVE);
		try {
			_attributes->addOffensiveAttribute(newAttribute);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete newAttribute;
			return;
		}

		_offensiveList->addItem(name);
		emit updateSignal();
	}
}

void AttributesWindow::editOffensive() {
	if ((_offensiveList->count() > 0) && (!!_offensiveList->currentItem())) {
		Attribute *attribute = _attributes->getOffensiveAttribute(_offensiveList->currentItem()->text());
		if (!!attribute) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Edit offensive attribute"),
				tr("Enter a new name for the offensive attribute:"),
				QLineEdit::Normal, attribute->getName() , &accepted);

			if (accepted && (name != attribute->getName())) {
				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editOffensive();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
					editOffensive();
					return;
				}

				if (_attributes->isRegistered(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
					editOffensive();
					return;
				}

				attribute->setName(name);

				_offensiveList->currentItem()->setText(attribute->getName());
				loadLinks();
				emit updateSignal();
			}
		}
	}
}

void AttributesWindow::deleteOffensive() {
	if ((_offensiveList->count() > 0) && (!!_offensiveList->currentItem())) {
		QString name = _offensiveList->currentItem()->text();
		Attribute *attribute = _attributes->getOffensiveAttribute(name);
		if (!!attribute) {
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete attribute",
				"Are you sure you want to delete the offensive attribute?\nDeleting an attribute will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_attributes->removeOffensiveAttribute(name);

				loadAttributes();
				emit updateSignal();
			}
		}
	}
}

void AttributesWindow::moveOffensiveUp() {
	if ((_offensiveList->count() > 0) && !!_offensiveList->currentItem() && (_offensiveList->currentRow() != 0)) {

		Attribute *currentAttribute = _attributes->getOffensiveAttribute(_offensiveList->currentItem()->text());
		if (!!currentAttribute) {
			const QVector<Attribute*>& attributes = _attributes->getOffensiveAttributes();
			int index = attributes.indexOf(currentAttribute);

			try {
				switchPlaces(const_cast<QVector<Attribute*>&>(attributes), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAttributes();
			_offensiveList->setCurrentRow(index-1);
		}
	}
}

void AttributesWindow::moveOffensiveDown() {
	if ((_offensiveList->count() > 0) && !!_offensiveList->currentItem() && (_offensiveList->currentRow() != _offensiveList->count()-1)) {

		Attribute *currentAttribute = _attributes->getOffensiveAttribute(_offensiveList->currentItem()->text());
		if (!!currentAttribute) {
			const QVector<Attribute*>& attributes = _attributes->getOffensiveAttributes();
			int index = attributes.indexOf(currentAttribute);

			try {
				switchPlaces(const_cast<QVector<Attribute*>&>(attributes), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAttributes();
			_offensiveList->setCurrentRow(index+1);
		}
	}
}

void AttributesWindow::newDefensive() {
	bool accepted;
	QString name = QInputDialog::getText(this, tr("New defensive attribute"),
		tr("Enter a name for the defensive attribute:"),
		QLineEdit::Normal, "" , &accepted);

	if (accepted) {
		QRegExpValidator validator(g_nameRegex);
		int pos = 0;
		if (validator.validate(name, pos) != QValidator::Acceptable) {
			QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
			newDefensive();
			return;
		}

		if (name == QString()) {
			QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
			newDefensive();
			return;
		}

		if (_attributes->isRegistered(name)) {
			QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
			newDefensive();
			return;
		}

		Attribute *newAttribute = Attribute::create(name, Attribute::DEFENSIVE);
		try {
			_attributes->addDefensiveAttribute(newAttribute);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete newAttribute;
			return;
		}

		_defensiveList->addItem(name);
		emit updateSignal();
	}
}

void AttributesWindow::editDefensive() {
	if ((_defensiveList->count() > 0) && (!!_defensiveList->currentItem())) {
		Attribute *attribute = _attributes->getDefensiveAttribute(_defensiveList->currentItem()->text());
		if (!!attribute) {
			bool accepted;
			QString name = QInputDialog::getText(this, tr("Edit defensive attribute"),
				tr("Enter a new name for the defensive attribute:"),
				QLineEdit::Normal, attribute->getName() , &accepted);

			if (accepted && (name != attribute->getName())) {
				QRegExpValidator validator(g_nameRegex);
				int pos = 0;
				if (validator.validate(name, pos) != QValidator::Acceptable) {
					QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
					editOffensive();
					return;
				}

				if (name == QString()) {
					QMessageBox::critical(this, tr("No empty name allowed"), tr("An attribute should not have an empty name."));
					editOffensive();
					return;
				}

				if (_attributes->isRegistered(name)) {
					QMessageBox::critical(this, tr("Name already exists"), tr("An attribute with the same name already exists."));
					editOffensive();
					return;
				}

				attribute->setName(name);

				_defensiveList->currentItem()->setText(attribute->getName());
				loadLinks();
				emit updateSignal();
			}
		}
	}
}

void AttributesWindow::deleteDefensive() {
	if ((_defensiveList->count() > 0) && (!!_defensiveList->currentItem())) {
		QString name = _defensiveList->currentItem()->text();
		Attribute *attribute = _attributes->getDefensiveAttribute(name);
		if (!!attribute) {
			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete attribute",
				"Are you sure you want to delete the defensive attribute?\nDeleting an attribute will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_attributes->removeDefensiveAttribute(name);

				loadAttributes();
				emit updateSignal();
			}
		}
	}
}

void AttributesWindow::moveDefensiveUp() {
	if ((_defensiveList->count() > 0) && !!_defensiveList->currentItem() && (_defensiveList->currentRow() != 0)) {

		Attribute *currentAttribute = _attributes->getDefensiveAttribute(_defensiveList->currentItem()->text());
		if (!!currentAttribute) {
			const QVector<Attribute*>& attributes = _attributes->getDefensiveAttributes();
			int index = attributes.indexOf(currentAttribute);

			try {
				switchPlaces(const_cast<QVector<Attribute*>&>(attributes), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAttributes();
			_defensiveList->setCurrentRow(index-1);
		}
	}
}

void AttributesWindow::moveDefensiveDown() {
	if ((_defensiveList->count() > 0) && !!_defensiveList->currentItem() && (_defensiveList->currentRow() != _defensiveList->count()-1)) {

		Attribute *currentAttribute = _attributes->getDefensiveAttribute(_defensiveList->currentItem()->text());
		if (!!currentAttribute) {
			const QVector<Attribute*>& attributes = _attributes->getDefensiveAttributes();
			int index = attributes.indexOf(currentAttribute);

			try {
				switchPlaces(const_cast<QVector<Attribute*>&>(attributes), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadAttributes();
			_defensiveList->setCurrentRow(index+1);
		}
	}
}

void AttributesWindow::addLink() {
	if ((_offensiveList->count() > 0) && (_defensiveList->count() > 0)) {
		// Generate dialog window
		QDialog linkWindow(0);
		linkWindow.setWindowTitle("Establish an attribute link");
		QVBoxLayout *layout = new QVBoxLayout(&linkWindow);

		QFrame *offensiveFrame = new QFrame(&linkWindow);
		QVBoxLayout *offensiveLayout = new QVBoxLayout(offensiveFrame);
		QLabel *offensiveLabel = new QLabel("Select an offensive attribute:", offensiveFrame);
		QComboBox *offensiveSelection = new QComboBox(offensiveFrame);
		offensiveLayout->addWidget(offensiveLabel);
		offensiveLayout->addWidget(offensiveSelection);
		layout->addWidget(offensiveFrame);

		QFrame *defensiveFrame = new QFrame(&linkWindow);
		QVBoxLayout *defensiveLayout = new QVBoxLayout(defensiveFrame);
		QLabel *defensiveLabel = new QLabel("Select a defensive attribute:", defensiveFrame);
		QComboBox *defensiveSelection = new QComboBox(defensiveFrame);
		defensiveLayout->addWidget(defensiveLabel);
		defensiveLayout->addWidget(defensiveSelection);
		layout->addWidget(defensiveFrame);

		QFrame *buttonFrame = new QFrame(&linkWindow);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		QPushButton *acceptButton = new QPushButton("Accept", buttonFrame);
		QPushButton *rejectButton = new QPushButton("Cancel", buttonFrame);
		buttonLayout->addWidget(acceptButton);
		buttonLayout->addWidget(rejectButton);
		layout->addWidget(buttonFrame);

		connect(acceptButton, SIGNAL(clicked()), &linkWindow, SLOT(accept()));
		connect(rejectButton, SIGNAL(clicked()), &linkWindow, SLOT(reject()));

		// Only add unlinked offensive attributes
		QVector<Attribute*> offensiveAttributes = _attributes->getOffensiveAttributes();
		for (int i = 0; i < offensiveAttributes.size(); i++) {
			Attribute *it = offensiveAttributes.at(i);
			if (!it->getLink())
				offensiveSelection->addItem(it->getName());
		}

		// Only add unlinked defensive attributes
		QVector<Attribute*> defensiveAttributes = _attributes->getDefensiveAttributes();
		for (int i = 0; i < defensiveAttributes.size(); i++) {
			Attribute *it = defensiveAttributes.at(i);
			if (!it->getLink())
				defensiveSelection->addItem(it->getName());
		}

		// If no attributes are available...
		if ((offensiveSelection->count() == 0) || (defensiveSelection->count() == 0)) {
			QMessageBox::information(this, tr("Not enough attributes available"), tr("To establish a link, at least one unlinked offensive attribute and one unlinked defensive attribute should be available."));
			return;
		}

		if (linkWindow.exec()) {

			Attribute *offensiveAttribute = _attributes->getOffensiveAttribute(offensiveSelection->currentText());
			Attribute *defensiveAttribute = _attributes->getDefensiveAttribute(defensiveSelection->currentText());
			try {
				_attributes->addLink(offensiveAttribute, defensiveAttribute);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			_linkTable->setRowCount(_linkTable->rowCount()+1);
			_linkTable->setItem(_linkTable->rowCount()-1, 0, new QTableWidgetItem(offensiveAttribute->getName()));
			_linkTable->setItem(_linkTable->rowCount()-1, 1, new QTableWidgetItem(defensiveAttribute->getName()));
		}

	} else {
		QMessageBox::information(this, tr("Not enough attributes available"), tr("To establish a link, at least one unlinked offensive attribute and one unlinked defensive attribute should be available."));
		return;
	}
}

void AttributesWindow::deleteLink() {
	if (_linkTable->rowCount() > 0) {

		// Generate dialog window
		QDialog linkWindow(0);
		linkWindow.setWindowTitle("Establish an attribute link");
		QVBoxLayout *layout = new QVBoxLayout(&linkWindow);

		QFrame *frame = new QFrame(&linkWindow);
		QVBoxLayout *framelayout = new QVBoxLayout(frame);
		QLabel *selectionLabel = new QLabel("Select an attribute link:");
		QComboBox *linkSelection = new QComboBox(frame);
		framelayout->addWidget(selectionLabel);
		framelayout->addWidget(linkSelection);
		layout->addWidget(frame);

		QFrame *buttonFrame = new QFrame(&linkWindow);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		QPushButton *acceptButton = new QPushButton("Accept", buttonFrame);
		QPushButton *rejectButton = new QPushButton("Cancel", buttonFrame);
		buttonLayout->addWidget(acceptButton);
		buttonLayout->addWidget(rejectButton);
		layout->addWidget(buttonFrame);

		connect(acceptButton, SIGNAL(clicked()), &linkWindow, SLOT(accept()));
		connect(rejectButton, SIGNAL(clicked()), &linkWindow, SLOT(reject()));

		QVector<QPair<Attribute*, Attribute*>> links = _attributes->getLinks();
		for (int i = 0; i < links.size(); i++) {
			QPair<Attribute*, Attribute*> it = links.at(i);
			linkSelection->addItem(it.first->getName() + " - " + it.second->getName());
		}

		if (linkWindow.exec()) {
			QStringList attributeNames = linkSelection->currentText().split(" - ");
			Attribute *offensiveAttribute = _attributes->getOffensiveAttribute(attributeNames.at(0));
			Attribute *defensiveAttribute = _attributes->getDefensiveAttribute(attributeNames.at(1));
			_linkTable->removeRow(linkSelection->currentIndex());
			_attributes->removeLink(offensiveAttribute->getName(), defensiveAttribute->getName());
		}

	}
}