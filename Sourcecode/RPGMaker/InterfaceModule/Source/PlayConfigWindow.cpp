#include "qframe.h"
#include "qlabel.h"
#include "qpainter.h"
#include "qboxlayout.h"
#include "qheaderview.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/PlayConfigWindow.h"

using namespace interfacemodule;

PlayConfigWindow::PlayConfigWindow(QWidget *parent, Project *project)
	: QDialog(parent), _project(project) {
		
		_tabs = new QTabWidget(this);

		QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addWidget(_tabs);

		createLocationTab();
		createPartyTab();
		createInventoryTab();

		setModal(true);
		setFixedSize(800, 600);

		loadLocationTab();
		loadPartyTab();
		loadInventoryTab();

		QFrame *buttonFrame = new QFrame(this);
		QPushButton *acceptButton = new QPushButton("Play", buttonFrame);
		acceptButton->setDefault(true);
		QPushButton *cancelButton = new QPushButton("Close", buttonFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignRight);
		buttonLayout->addWidget(acceptButton);
		buttonLayout->addWidget(cancelButton);
		layout->addWidget(buttonFrame);

		connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

PlayConfigWindow::~PlayConfigWindow() {}

void PlayConfigWindow::createLocationTab() {
	QFrame *locationTab = new QFrame();
	_tabs->addTab(locationTab, "Location");
	QVBoxLayout *layout = new QVBoxLayout(locationTab);

	QFrame *selectionFrame = new QFrame(_tabs);
	QVBoxLayout *selectionLayout = new QVBoxLayout(selectionFrame);

	QLabel *label1 = new QLabel("Select an environment and click on a tile to set the starting location:", selectionFrame);
	_environmentSelection = new QComboBox(selectionFrame);
	_environmentSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	selectionLayout->addWidget(label1);
	selectionLayout->addSpacing(20);
	selectionLayout->addWidget(_environmentSelection);
	layout->addWidget(selectionFrame);

	_environment = new QTableWidget(_tabs);
	_environment->clear();
	_environment->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_environment->setSelectionBehavior(QAbstractItemView::SelectItems);
	_environment->setSelectionMode(QAbstractItemView::SingleSelection);
	_environment->horizontalHeader()->setDefaultSectionSize(g_tilesize);
	_environment->verticalHeader()->setDefaultSectionSize(g_tilesize);
	_environment->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_environment->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_environment->horizontalHeader()->hide();
	_environment->verticalHeader()->hide();
	layout->addWidget(_environment);

	connect(_environmentSelection, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(environmentChanged(const QString &)));
	connect(_environment, SIGNAL(cellClicked(int, int)), this, SLOT(environmentCellClicked(int, int)));
}

void PlayConfigWindow::createPartyTab() {
	QFrame *partyTab = new QFrame();
	_tabs->addTab(partyTab, "Party");
	QVBoxLayout *layout = new QVBoxLayout(partyTab);

	QFrame *selectionFrame = new QFrame(_tabs);
	QHBoxLayout *selectionLayout = new QHBoxLayout(selectionFrame);

	QLabel *label1 = new QLabel("Add the characters to the starting party.\nThe first character will serve as the party leader.", selectionFrame);
	QPushButton *addCharacter = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", selectionFrame);
	addCharacter->setToolTip("Add a new character to the party");
	addCharacter->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *removeCharacter = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", selectionFrame);
	removeCharacter->setMaximumSize(g_buttonsize, g_buttonsize);
	removeCharacter->setToolTip("Remove the character from the current slot");
	selectionLayout->addWidget(label1);
	selectionLayout->addSpacing(20);
	selectionLayout->addWidget(addCharacter);
	selectionLayout->addWidget(removeCharacter);
	layout->addWidget(selectionFrame);

	_party = new QTableWidget(partyTab);
	_party->clear();
	_party->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_party->setSelectionBehavior(QAbstractItemView::SelectItems);
	_party->setSelectionMode(QAbstractItemView::SingleSelection);
	_party->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_party->horizontalHeader()->hide();
	_party->setColumnCount(1);
	_party->setRowCount(g_maxPartysize);
	QStringList headers;
	for (int i = 0; i < g_maxPartysize; i++) {
		QString header("Slot ");
		header += QString::number(i);
		headers << header;
	}
	_party->setVerticalHeaderLabels(headers);
	layout->addWidget(_party);

	connect(addCharacter, SIGNAL(clicked()), this, SLOT(addCharacter()));
	connect(removeCharacter, SIGNAL(clicked()), this, SLOT(removeCharacter()));
}

void PlayConfigWindow::createInventoryTab() {
}

void PlayConfigWindow::loadLocationTab() {
	_environmentSelection->clear();
	_environment->clear();
	_environment->setRowCount(0);
	_environment->setColumnCount(0);

	QVector<Environment*> environments = _project->getEnvironments()->getEnvironments();
	StartEvent *start = _project->getStartEvent();
	int index = 0;

	for (int i = 0; i < environments.size(); i++) {
		Environment *it = environments.at(i);
		_environmentSelection->addItem(it->getName());

		if (!!start->getLocation().first && (start->getLocation().first->getName() == it->getName()))
			index = i;
	}

	_environmentSelection->setCurrentIndex(index);
}

void PlayConfigWindow::loadPartyTab() {
	_party->clear();
	QVector<Character*> members = _project->getStartEvent()->getParty();
	for (int i = 0; i < members.size(); i++) {
		Character *it = members.at(i);
		_party->setItem(i, 0, new QTableWidgetItem(it->getName()));
	}
}

void PlayConfigWindow::loadInventoryTab() {

}

void PlayConfigWindow::paintEnvironment(Environment *environment) {
	QSize dimensions = environment->getDimensions();

	_environment->clear();
	_environment->setColumnCount(dimensions.width());
	_environment->setRowCount(dimensions.height());

	for (int y = 0; y < dimensions.height(); y++) {
		for (int x = 0; x < dimensions.width(); x++) {
			paintTile(environment, QPoint(x, y));
		}
	}
}

void PlayConfigWindow::paintTile(Environment *environment, QPoint location) {
	// Delete the current contents of the cell
	QTableWidgetItem *currentlabel = _environment->takeItem(location.y(), location.x());
	if (!!currentlabel)
		delete currentlabel;

	// Compose draw the new contents
	QPixmap compositiontile(g_tilesize, g_tilesize);
	compositiontile.fill(Qt::transparent);
	QPainter painter(&compositiontile);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	for (int i = 0; i < g_layers; i++) {
		Tile *tile = environment->getTile(i, location);
		if (!!tile)
			painter.drawPixmap(0, 0, tile->getImage());
	}

	painter.end();

	// Place the new contents of the cell
	QLabel *label = new QLabel();
	label->setPixmap(compositiontile);
	_environment->setCellWidget(location.y(), location.x(), label);
}

void PlayConfigWindow::markTile(Environment *environment, QPoint location) {
	// Delete the current contents of the cell
	QTableWidgetItem *currentlabel = _environment->takeItem(location.y(), location.x());
	if (!!currentlabel)
		delete currentlabel;

	// Compose draw the new contents
	QPixmap compositiontile(g_tilesize, g_tilesize);
	compositiontile.fill(Qt::transparent);
	QPainter painter(&compositiontile);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	for (int i = 0; i < g_layers; i++) {
		Tile *tile = environment->getTile(i, location);
		if (!!tile)
			painter.drawPixmap(0, 0, tile->getImage());
	}

	painter.setOpacity(0.5);
	painter.fillRect(compositiontile.rect(), Qt::yellow);
	painter.setOpacity(1);
	painter.end();

	// Place the new contents of the cell
	QLabel *label = new QLabel();
	label->setPixmap(compositiontile);
	_environment->setCellWidget(location.y(), location.x(), label);
}

void PlayConfigWindow::environmentChanged(const QString &name) {
	if (_environmentSelection->count() > 0) {
		Environment *environment = _project->getEnvironments()->getEnvironment(name);
		if (!!environment) {
			
			paintEnvironment(environment);

			StartEvent *start = _project->getStartEvent();
			if (!!start->getLocation().first && (start->getLocation().first->getName() == environment->getName()))
				markTile(environment, start->getLocation().second);
		}
	}
}

void PlayConfigWindow::environmentCellClicked(int row, int column) {
	if (_environmentSelection->count() > 0) {
		StartEvent *event = _project->getStartEvent();
		Environment *currentEnvironment = _project->getEnvironments()->getEnvironment(_environmentSelection->currentText());
		if (!!currentEnvironment) {

			if (currentEnvironment->getAccess(QPoint(column, row)) == Tileset::NONE) {
				QMessageBox::information(this, tr("Location not accessible"), tr("The start location for the game is located on a tile which has no access for characters. Please choose another location."));
				return;
			}

			if (!!event->getLocation().first && (event->getLocation().first->getName() == currentEnvironment->getName()))
				paintTile(currentEnvironment, event->getLocation().second);

			event->setLocation(currentEnvironment, QPoint(column, row));
			markTile(currentEnvironment, QPoint(column, row));
		}
	}
}

void PlayConfigWindow::addCharacter() {
	QVector<Character*> inParty = _project->getStartEvent()->getParty();
	if (inParty.size() == g_maxPartysize) {
		QMessageBox::information(this, "Party is full", "All available slots for the party are taken.");
		return;
	}

	QVector<Character*> characters = _project->getCharacters()->getCharacters();
	QStringList items;
	for (int i = 0; i < characters.size(); i++) {
		Character *it = characters.at(i);
		if (!inParty.contains(it))
			items << it->getName();
	}

	if (items.size() == 0) {
		QMessageBox::information(this, "No more characters available", "There are no more characters to choose from.");
		return;
	}

	bool accepted;
	QString item = QInputDialog::getItem(this, "Select character", "Select a character to add to the party.", items, 0, false, &accepted);
	if (accepted) {
		Character *newMember = _project->getCharacters()->getCharacter(item);
		if (!!newMember) {
			try {
				_project->getStartEvent()->addCharacter(newMember);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, "An unexpected error has occurred", e.what());
				return;
			}
			loadPartyTab();
		}
	}
}

void PlayConfigWindow::removeCharacter() {
	if (!!_party->currentItem() && (_party->currentItem()->text() != QString())) {
		try {
			_project->getStartEvent()->removeCharacter(_party->currentRow());
		} catch (ProjectException &e) {
			QMessageBox::critical(this, "An unexpected error has occurred", e.what());
			return;
		}

		loadPartyTab();
	}
}