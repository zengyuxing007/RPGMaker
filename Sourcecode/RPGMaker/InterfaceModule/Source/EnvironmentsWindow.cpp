#include "qpixmap.h"
#include "qpainter.h"
#include "qgroupbox.h"
#include "qtabwidget.h"
#include "qpushbutton.h"
#include "qstringlist.h"
#include "qheaderview.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qscrollarea.h"
#include "qinputdialog.h"

#include "../Headers/TileAccessWindow.h"
#include "../Headers/EnvironmentsWindow.h"
#include "../Components/Headers/Tileset.h"
#include "../Components/Headers/AutoTile.h"
#include "../Headers/TilesetConfigWindow.h"
#include "../Components/Headers/StaticTile.h"
#include "../Components/Headers/Environment.h"
#include "../Components/Headers/AutoTileset.h"
#include "../Headers/EnvironmentConfigWindow.h"
#include "../Components/Headers/StaticTileset.h"

using namespace interfacemodule;
using namespace projectcomponents;

EnvironmentsWindow::EnvironmentsWindow(QWidget *parent, Project *project) 
	: QFrame(parent), _project(project) {

		setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
		setFrameShape(QFrame::Panel);

		_layout = new QHBoxLayout(this);

		createTilesetBox();
		createEnvironmentBox();

		_layergroup->button(9)->click();

		loadStaticTilesets();
		loadAutoTilesets();
		loadEnvironments();

		if (_statictilesetselection->count() > 0)
			_statictilesetselection->setCurrentIndex(0);

		if (_environmentselection->count() > 0)
			_environmentselection->setCurrentIndex(0);
}

EnvironmentsWindow::~EnvironmentsWindow() {}

void EnvironmentsWindow::createTilesetBox() {
	QGroupBox *tilesetframe = new QGroupBox("Tilesets", this);
	tilesetframe->setMaximumWidth(376);
	QVBoxLayout *tilesetlayout = new QVBoxLayout(tilesetframe);
	tilesetlayout->setAlignment(Qt::AlignLeft);
	_layout->addWidget(tilesetframe);

	QFrame *buttonframe = new QFrame(tilesetframe);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	buttonlayout->setAlignment(Qt::AlignLeft);
	tilesetlayout->addWidget(buttonframe);

	QTabWidget *tilesettabframe = new QTabWidget(tilesetframe);
	tilesetlayout->addWidget(tilesettabframe);

	QPushButton *tilesetnewbutton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonframe);
	tilesetnewbutton->setToolTip("Add a new tileset to the project");
	tilesetnewbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *tileseteditbutton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonframe);
	tileseteditbutton->setToolTip("Edit the current tileset");
	tileseteditbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *tilesetdeletebutton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonframe);
	tilesetdeletebutton->setMaximumSize(g_buttonsize, g_buttonsize);
	tilesetdeletebutton->setToolTip("Delete the current tileset");
	QPushButton *accessControlButton = new QPushButton(QIcon(g_iconPath + "layers_2_icon&24.png"), "", buttonframe);
	accessControlButton->setMaximumSize(g_buttonsize, g_buttonsize);
	accessControlButton->setToolTip("Edit the access levels of tiles");
	buttonlayout->addWidget(tilesetnewbutton);
	buttonlayout->addWidget(tileseteditbutton);
	buttonlayout->addWidget(tilesetdeletebutton);
	buttonlayout->addSpacing(20);
	buttonlayout->addWidget(accessControlButton);
	buttonlayout->addSpacing(20);

	QLabel *currenttile = new QLabel("Current tile:", buttonframe);
	_currenttile.first = new QLabel(buttonframe);
	_currenttile.first->setMaximumSize(g_tilesize, g_tilesize);
	_currenttile.second = 0;
	buttonlayout->addWidget(currenttile);
	buttonlayout->addWidget(_currenttile.first);

	QFrame *statictilsetframe = new QFrame();
	tilesettabframe->addTab(statictilsetframe, "Static tilesets");
	QVBoxLayout *statictilesetlayout = new QVBoxLayout(statictilsetframe);

	QFrame *staticselectionframe = new QFrame(statictilsetframe);
	QHBoxLayout *staticselectionlayout = new QHBoxLayout(staticselectionframe);
	staticselectionlayout->setAlignment(Qt::AlignLeft);
	statictilesetlayout->addWidget(staticselectionframe);

	QLabel *currenttilesetlabel = new QLabel("Current tileset:");
	_statictilesetselection = new QComboBox(statictilsetframe);
	_statictilesetselection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	_statictilesetselection->setInsertPolicy(QComboBox::InsertAlphabetically);
	staticselectionlayout->addWidget(currenttilesetlabel);
	staticselectionlayout->addWidget(_statictilesetselection);

	_statictileset = new QTableWidget(statictilsetframe);
	_currenttab = 0;
	_statictileset->clear();
	_statictileset->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_statictileset->setSelectionBehavior(QAbstractItemView::SelectItems);
	_statictileset->setSelectionMode(QAbstractItemView::SingleSelection);
	_statictileset->horizontalHeader()->setDefaultSectionSize(g_tilesize);
	_statictileset->verticalHeader()->setDefaultSectionSize(g_tilesize);
	_statictileset->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_statictileset->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	statictilesetlayout->addWidget(_statictileset);

	QFrame *autotilesetframe = new QFrame();
	tilesettabframe->addTab(autotilesetframe, "Auto tilesets");
	QVBoxLayout *autotilesetlayout = new QVBoxLayout(autotilesetframe);

	_autotileset = new QTableWidget(autotilesetframe);
	_autotileset->clear();
	_autotileset->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_autotileset->setSelectionBehavior(QAbstractItemView::SelectItems);
	_autotileset->setSelectionMode(QAbstractItemView::SingleSelection);
	_autotileset->horizontalHeader()->setDefaultSectionSize(g_tilesize);
	_autotileset->verticalHeader()->setDefaultSectionSize(g_tilesize);
	_autotileset->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_autotileset->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	autotilesetlayout->addWidget(_autotileset);

	connect(tilesetnewbutton, SIGNAL(clicked()), this, SLOT(newTileset()));
	connect(tileseteditbutton, SIGNAL(clicked()), this, SLOT(editTileset()));
	connect(tilesetdeletebutton, SIGNAL(clicked()), this, SLOT(deleteTileset()));
	connect(accessControlButton, SIGNAL(clicked()), this, SLOT(openAccessControl()));
	connect(tilesettabframe, SIGNAL(currentChanged(int)), this, SLOT(tilesetTabClicked(int)));
	connect(_statictilesetselection, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(tilesetChanged(const QString &)));
	connect(_statictileset, SIGNAL(cellClicked(int, int)), this, SLOT(tilesetCellClicked(int, int)));
	connect(_autotileset, SIGNAL(cellClicked(int, int)), this, SLOT(tilesetCellClicked(int, int)));
}

void EnvironmentsWindow::createEnvironmentBox() {
	QGroupBox *environmentframe = new QGroupBox("Environments", this);
	QVBoxLayout *environmentlayout = new QVBoxLayout(environmentframe);
	environmentlayout->setAlignment(Qt::AlignLeft);
	_layout->addWidget(environmentframe);

	QFrame *buttonframe = new QFrame(environmentframe);
	QHBoxLayout *buttonlayout = new QHBoxLayout(buttonframe);
	buttonlayout->setAlignment(Qt::AlignLeft);
	environmentlayout->addWidget(buttonframe);

	QFrame *environmentselectionframe = new QFrame(environmentframe);
	QHBoxLayout *environmentselectionlayout = new QHBoxLayout(environmentselectionframe);
	environmentselectionlayout->setAlignment(Qt::AlignLeft);
	environmentlayout->addWidget(environmentselectionframe);

	QPushButton *environmentnewbutton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonframe);
	environmentnewbutton->setToolTip("Add a new tileset to the project");
	environmentnewbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *environmenteditbutton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonframe);
	environmenteditbutton->setToolTip("Edit the current tileset");
	environmenteditbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *environmentdeletebutton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonframe);
	environmentdeletebutton->setMaximumSize(g_buttonsize, g_buttonsize);
	environmentdeletebutton->setToolTip("Delete the current tileset");
	buttonlayout->addWidget(environmentnewbutton);
	buttonlayout->addWidget(environmenteditbutton);
	buttonlayout->addWidget(environmentdeletebutton);
	buttonlayout->addSpacing(20);

	_layergroup = new QButtonGroup(buttonframe);
	QPushButton *layer1button = new QPushButton("1", buttonframe);
	layer1button->setToolTip("Place tiles in layer 1");
	layer1button->setMaximumSize(g_buttonsize, g_buttonsize);
	layer1button->setCheckable(true);
	_layergroup->addButton(layer1button, 0);
	QPushButton *layer2button = new QPushButton("2", buttonframe);
	layer2button->setToolTip("Place tiles in layer 2");
	layer2button->setMaximumSize(g_buttonsize, g_buttonsize);
	layer2button->setCheckable(true);
	_layergroup->addButton(layer2button, 1);
	QPushButton *layer3button = new QPushButton("3", buttonframe);
	layer3button->setToolTip("Place tiles in layer 3");
	layer3button->setMaximumSize(g_buttonsize, g_buttonsize);
	layer3button->setCheckable(true);
	_layergroup->addButton(layer3button, 2);
	QPushButton *layer4button = new QPushButton("4", buttonframe);
	layer4button->setToolTip("Place tiles in layer 4");
	layer4button->setMaximumSize(g_buttonsize, g_buttonsize);
	layer4button->setCheckable(true);
	_layergroup->addButton(layer4button, 3);
	QPushButton *eventbutton = new QPushButton(QIcon(g_iconPath + "box_icon&24.png"), "", buttonframe);
	eventbutton->setToolTip("Place events and objects");
	eventbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	eventbutton->setCheckable(true);
	_layergroup->addButton(eventbutton, 9);
	buttonlayout->addWidget(layer1button);
	buttonlayout->addWidget(layer2button);
	buttonlayout->addWidget(layer3button);
	buttonlayout->addWidget(layer4button);
	buttonlayout->addWidget(eventbutton);
	buttonlayout->addSpacing(20);

	_modegroup = new QButtonGroup(buttonframe);
	_modegroup->setExclusive(false);
	QPushButton *floodbutton = new QPushButton(QIcon(g_iconPath + "fill_icon&24.png"), "", buttonframe);
	floodbutton->setToolTip("Flood a uniform area");
	floodbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	floodbutton->setCheckable(true);
	_modegroup->addButton(floodbutton, 0);
	QPushButton *erasebutton = new QPushButton(QIcon(g_iconPath + "checkbox_unchecked_icon&24.png"), "", buttonframe);
	erasebutton->setToolTip("Erase a tile");
	erasebutton->setMaximumSize(g_buttonsize, g_buttonsize);
	erasebutton->setCheckable(true);
	_modegroup->addButton(erasebutton, 1);
	buttonlayout->addWidget(floodbutton);
	buttonlayout->addWidget(erasebutton);
	buttonlayout->addSpacing(20);

	QPushButton *previewbutton = new QPushButton(QIcon(g_iconPath + "eye_icon&24.png"), "", buttonframe);
	previewbutton->setToolTip("Show a full render of the selected environment");
	previewbutton->setMaximumSize(g_buttonsize, g_buttonsize);
	buttonlayout->addWidget(previewbutton);

	QLabel *currentenvironmentlabel = new QLabel("Current environment:", environmentselectionframe);
	_environmentselection = new QComboBox(environmentselectionframe);
	_environmentselection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	_environmentselection->setInsertPolicy(QComboBox::InsertAlphabetically);
	environmentselectionlayout->addWidget(currentenvironmentlabel);
	environmentselectionlayout->addWidget(_environmentselection);

	_environment = new QTableWidget(environmentframe);
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
	environmentlayout->addWidget(_environment);

	connect(environmentnewbutton, SIGNAL(clicked()), this, SLOT(newEnvironment()));
	connect(environmenteditbutton, SIGNAL(clicked()), this, SLOT(editEnvironment()));
	connect(environmentdeletebutton, SIGNAL(clicked()), this, SLOT(deleteEnvironment()));
	connect(previewbutton, SIGNAL(clicked()), this, SLOT(displayPreview()));
	connect(_layergroup, SIGNAL(buttonClicked(int)), this, SLOT(layerSelected(int)));
	connect(_environmentselection, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(environmentChanged(const QString &)));
	connect(_environment, SIGNAL(cellClicked(int, int)), this, SLOT(environmentCellClicked(int, int)));
}

void EnvironmentsWindow::loadStaticTilesets() {
	_statictilesetselection->clear();
	_statictileset->clearContents();
	_statictileset->setRowCount(0);
	_statictileset->setColumnCount(0);

	QVector<Tileset*> tilesets = _project->getProperties()->getTilesets()->getTilesets();
	for (int i = 0; i < tilesets.size(); i++) {
		Tileset *it = tilesets.at(i);
		if (it->getType() == Tileset::STATIC) {
			_statictilesetselection->addItem(it->getName());
		}
	}
}

void EnvironmentsWindow::loadAutoTilesets() {
	_autotileset->clearContents();
	_autotileset->setRowCount(0);
	_autotileset->setColumnCount(3);

	QStringList verticalheaders;
	QVector<Tileset*> tilesets = _project->getProperties()->getTilesets()->getTilesets();
	for (int i = 0; i < tilesets.size(); i++) {
		Tileset *it = tilesets.at(i);
		if (it->getType() == Tileset::AUTO) {
			verticalheaders << it->getName();
			_autotileset->setRowCount(_autotileset->rowCount() + 1);
			
			QVector<QPixmap> tileimages = it->getTiles();
			for (int j = 0; j < tileimages.size(); j++) {
				QLabel *label = new QLabel();
				label->setPixmap(tileimages.at(j));
				_autotileset->setCellWidget(_autotileset->rowCount() -1, j, label);
			}
		}
	}

	_autotileset->setVerticalHeaderLabels(verticalheaders);
}

void EnvironmentsWindow::loadEnvironments() {
	_environmentselection->clear();
	_environment->clearContents();
	_environment->setRowCount(0);
	_environment->setColumnCount(0);

	QVector<Environment*> environments = _project->getEnvironments()->getEnvironments();
	for (int i = 0; i < environments.size(); i++) {
		Environment *it = environments.at(i);
		_environmentselection->addItem(it->getName());
	}
}

void EnvironmentsWindow::paintEnvironment(Environment *environment) {
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

void EnvironmentsWindow::markEnvironment(Environment *environment) {
	QSize dimensions = environment->getDimensions();

	_environment->clear();
	_environment->setColumnCount(dimensions.width());
	_environment->setRowCount(dimensions.height());

	for (int y = 0; y < dimensions.height(); y++) {
		for (int x = 0; x < dimensions.width(); x++) {
			markTile(environment, QPoint(x, y));
		}
	}
}

void EnvironmentsWindow::paintTile(Environment *environment, QPoint location) {

	// Delete the current contents of the cell
	QTableWidgetItem *currentlabel = _environment->takeItem(location.y(), location.x());
	if (!!currentlabel)
		delete currentlabel;

	// Compose draw the new contents
	QPixmap compositiontile(g_tilesize, g_tilesize);
	compositiontile.fill(Qt::transparent);
	QPainter painter(&compositiontile);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	for (int i = 0; i < _currentlayer; i++) {
		Tile *tile = environment->getTile(i, location);
		if (!!tile)
			painter.drawPixmap(0, 0, tile->getImage());
	}

	painter.setOpacity(0.5);
	painter.fillRect(compositiontile.rect(), Qt::lightGray);
	painter.setOpacity(1);

	Tile *tile = environment->getTile(_currentlayer, location);
	if (!!tile)
		painter.drawPixmap(0, 0, tile->getImage());

	painter.end();

	// Place the new contents of the cell
	QLabel *label = new QLabel();
	label->setPixmap(compositiontile);
	_environment->setCellWidget(location.y(), location.x(), label);
}

void EnvironmentsWindow::markTile(Environment *environment, QPoint location) {
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

	// Mark the tile with a color if an event is present
	Event *event = environment->getEvent(location);
	if (!!event) {
		painter.setOpacity(0.5);
		switch (event->getType()) {
		case Event::TEXT:
			painter.fillRect(compositiontile.rect(), Qt::blue);
			break;
		case Event::PORTAL:
			painter.fillRect(compositiontile.rect(), Qt::green);
			break;
		case Event::BATTLE:
			painter.fillRect(compositiontile.rect(), Qt::red);
			break;
		}
		painter.setOpacity(1);
	}

	painter.end();

	// Place the new contents of the cell
	QLabel *label = new QLabel();
	label->setPixmap(compositiontile);
	_environment->setCellWidget(location.y(), location.x(), label);
}

void EnvironmentsWindow::placeTile(QPoint location) {
	Tile *tile = 0;
	// If there exists a current tile and the erase mode is not checked, then...
	if (!!_currenttile.second && !_modegroup->button(1)->isChecked()) {
		switch (_currenttile.second->getType()) {
		case Tile::STATIC:
			tile = StaticTile::create(_currenttile.second->getTileset(), _currenttile.second->getLocation());
			break;
		case Tile::AUTO:
			tile  = AutoTile::create(_currenttile.second->getTileset(), _currenttile.second->getLocation());
			break;
		}
	}

	QVector<QPoint> alteredtiles(0);
	Environment *environment = _project->getEnvironments()->getEnvironment(_environmentselection->currentText());

	// If flood fill is selected then...
	if (_modegroup->button(0)->isChecked()) {
		alteredtiles = environment->floodLayer(tile, _currentlayer, QPoint(location.x(), location.y()));
	} else {
		alteredtiles = environment->setTile(tile, _currentlayer, QPoint(location.x(), location.y()));
	}

	for (int i = 0; i < alteredtiles.size(); i++) {
		paintTile(environment, alteredtiles.at(i));
	}
}

void EnvironmentsWindow::placeEvent(QPoint location) {
	Event *event = 0;
	Environment *environment = _project->getEnvironments()->getEnvironment(_environmentselection->currentText());

	if (!_modegroup->button(1)->isChecked()) {
		
		QVector<Event*> events = _project->getEvents()->getEvents();
		QStringList eventList;
		for (int i = 0; i < events.size(); i++) {
			Event *it = events.at(i);
			if (!!it)
				eventList << it->getName();
		}

		if (eventList.count() == 0) {
			QMessageBox::information(this, tr("No events available"), tr("There are no events available to choose from."));
			return;
		}

		bool accepted;
		QString ID = QInputDialog::getItem(this, tr("Select an event"), tr("Select an event for the selected location:"), eventList, 0, false, &accepted);
		if (accepted) {
			Event *event = _project->getEvents()->getEvent(ID);
			if (!!event) {
				try {
					environment->setEvent(event, location);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				markTile(environment, location);
			}
		}

	} else {
		try {
			environment->removeEvent(location);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			return;
		}
		markTile(environment, location);
	}
}

void EnvironmentsWindow::newTileset() {
	TilesetConfigWindow configdaig(0, TilesetConfigWindow::NEW, _project);
	configdaig.setupNewTileset();
	if (configdaig.exec()) {
		Tileset *tileset;
		switch (configdaig.getType()) {
		case Tileset::STATIC:
			tileset = StaticTileset::create(configdaig.getName(), configdaig.getImagename());
			break;
		case Tileset::AUTO:
			tileset = AutoTileset::create(configdaig.getName(), configdaig.getImagename());
			break;
		}

		try {
			_project->getProperties()->getTilesets()->addTileset(tileset);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete tileset;
			return;
		}

		switch (tileset->getType()) {
		case Tileset::STATIC:
			loadStaticTilesets();
			_statictilesetselection->setCurrentIndex(_statictilesetselection->count()-1);
			break;
		case Tileset::AUTO:
			loadAutoTilesets();
			break;
		}
	}
}

void EnvironmentsWindow::editTileset() {
	Tileset *tileset;
	if ((_currenttab == 0) && (_statictilesetselection->count() > 0))
		tileset = _project->getProperties()->getTilesets()->getTileset(_statictilesetselection->currentText());
	else if ((_currenttab == 1) && (!!_currenttile.second))
		tileset = _project->getProperties()->getTilesets()->getTileset(_autotileset->verticalHeaderItem(_autotileset->currentRow())->text());
	else
		return;

	if (!!tileset) {
		TilesetConfigWindow configdiag(0, TilesetConfigWindow::EDIT, _project);
		configdiag.setupEditTileset(tileset->getName(), tileset->getType(), tileset->getImagename(), tileset->getImage());
		if (configdiag.exec()) {
			tileset->setName(configdiag.getName());
			
			switch (tileset->getType()) {
			case Tileset::STATIC:
				_statictilesetselection->setItemText(_statictilesetselection->currentIndex(), tileset->getName());
				break;
			case Tileset::AUTO:
				_autotileset->verticalHeaderItem(_autotileset->currentRow())->setText(tileset->getName());
				break;
			}
		}
	}
}

void EnvironmentsWindow::deleteTileset() {
	Tileset *tileset = 0;
	if ((_currenttab == 0) && (_statictilesetselection->count() > 0))
		tileset = _project->getProperties()->getTilesets()->getTileset(_statictilesetselection->currentText());
	else if ((_currenttab == 1) && (!!_currenttile.second))
		tileset = _project->getProperties()->getTilesets()->getTileset(_autotileset->verticalHeaderItem(_autotileset->currentRow())->text());
	else
		return;

	if (!!tileset) {
		QMessageBox::StandardButton response = QMessageBox::warning(this, tr("Delete tileset"),
			tr("Are you sure you want to delete this tileset?\nDeleting a tileset will delete all references to it and cannot be undone."),
			QMessageBox::Ok | QMessageBox::Cancel);

		if (response == QMessageBox::Ok) {

			if (!!_currenttile.second && (_currenttile.second->getTileset()->getName() == tileset->getName())) {
				_currenttile.first->clear();
				delete _currenttile.second;
				_currenttile.second = 0;
			}

			Tileset::Type type = tileset->getType();
			_project->getProperties()->getTilesets()->removeTileset(tileset->getName());
			switch (type) {
			case Tileset::STATIC:
				loadStaticTilesets();

				if (_statictilesetselection->count() > 0)
					_statictilesetselection->setCurrentIndex(0);

				break;
			case Tileset::AUTO:
				loadAutoTilesets();
				break;
			}

			if (_environmentselection->count() > 0)
				environmentChanged(_environmentselection->currentText());
		}
	}
}

void EnvironmentsWindow::tilesetTabClicked(int index) {
	_currenttile.first->clear();
	if (!!_currenttile.second) {
		delete _currenttile.second;
		_currenttile.second = 0;
	}

	_currenttab = index;
	if ((_currenttab == 0) && (_statictilesetselection->count() > 0))
		_statictilesetselection->setCurrentIndex(0);
}

void EnvironmentsWindow::tilesetChanged(const QString &name) {
	if ((_currenttab == 0) && (_statictilesetselection->count() > 0)) {
		Tileset *tileset = _project->getProperties()->getTilesets()->getTileset(name);
		if (!!tileset) {
			QSize dimensions = tileset->getDimensions();
			_statictileset->clear();
			_statictileset->setColumnCount(dimensions.width());
			_statictileset->setRowCount(dimensions.height());

			QVector<QPixmap> tileimages = tileset->getTiles();
			for (int y = 0; y < dimensions.height(); y++) {
				for (int x = 0; x < dimensions.width(); x++) {
					QLabel *label = new QLabel();
					label->setPixmap(tileimages.at(y * dimensions.width() + x));
					_statictileset->setCellWidget(y, x, label);
				}
			}
		}
	}
}

void EnvironmentsWindow::tilesetCellClicked(int row, int column) {
	_currenttile.first->clear();
	if (!!_currenttile.second) {
		delete _currenttile.second;
		_currenttile.second = 0;
	}

	switch (_currenttab) {
	case 0:
		{
			QString name = _statictilesetselection->currentText();
			Tileset *tileset = _project->getProperties()->getTilesets()->getTileset(name);
			if (!!tileset) {
				_currenttile.second = StaticTile::create(tileset, QPoint(column, row));
				_currenttile.first->setPixmap(_currenttile.second->getImage());
			}
		}
		break;
	case 1:
		{
			QString name = _autotileset->verticalHeaderItem(row)->text();
			Tileset *tileset = _project->getProperties()->getTilesets()->getTileset(name);
			if (!!tileset) {
				_currenttile.second = AutoTile::create(tileset, QPoint(column, 0));
				_currenttile.first->setPixmap(_currenttile.second->getImage());
			}
		}
		break;
	}
}

void EnvironmentsWindow::openAccessControl() {
	switch (_currenttab) {
	case 0:
		{
			if ((_statictilesetselection->count() > 0) && (_statictilesetselection->currentText() != QString())) {
				QString name = _statictilesetselection->currentText();
				Tileset *tileset = _project->getProperties()->getTilesets()->getTileset(name);
				if (!!tileset) {
					TileAccessWindow accessWindow(0, tileset);
					accessWindow.exec();
				}
			}
		}
		break;
	case 1:
		{
			if ((_autotileset->rowCount() > 0) && !!_autotileset->verticalHeaderItem(_autotileset->currentRow())) {
				QString name = _autotileset->verticalHeaderItem(_autotileset->currentRow())->text();
				Tileset *tileset = _project->getProperties()->getTilesets()->getTileset(name);
				if (!!tileset) {
					TileAccessWindow accessWindow(0, tileset);
					accessWindow.exec();
				}
			}
		}
		break;
	}
}

void EnvironmentsWindow::newEnvironment() {
	EnvironmentConfigWindow configdiag(0, _project);
	if (configdiag.exec()) {
		QSize dimensions(configdiag.getWidth(), configdiag.getHeight());
		Environment *environment = Environment::create(configdiag.getName(), dimensions);

		try {
			_project->getEnvironments()->addEnvironment(environment);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			delete environment;
			return;
		}

		loadEnvironments();
		_environmentselection->setCurrentIndex(_environmentselection->count() -1);
	}
}

void EnvironmentsWindow::editEnvironment() {
	if (_environmentselection->count() > 0) {
		Environment *environment = _project->getEnvironments()->getEnvironment(_environmentselection->currentText());
		if (!!environment) {
			QSize dimensions = environment->getDimensions();

			EnvironmentConfigWindow configdiag(0, _project);
			configdiag.setName(environment->getName());
			configdiag.setWidth(dimensions.width());
			configdiag.setHeight(dimensions.height());
			configdiag.setBackgroundMusic(environment->getBackgroundMusic());
			configdiag.setBattleback(environment->getBattlebackName());

			if (configdiag.exec()) {
				environment->setName(configdiag.getName());
				dimensions.setHeight(configdiag.getHeight());
				dimensions.setWidth(configdiag.getWidth());
				environment->setDimensions(dimensions);
				environment->setBackgroundMusic(configdiag.getBackgroundMusic());

				if (environment->getBattlebackName() != configdiag.getBattleback())
					environment->setBattleback(configdiag.getBattleback());

				_environmentselection->setItemText(_environmentselection->currentIndex(), environment->getName());
				environmentChanged(_environmentselection->currentText());
			}
		}
	}
}

void EnvironmentsWindow::deleteEnvironment() {
	if (_environmentselection->count() > 0) {
		QMessageBox::StandardButton response = QMessageBox::warning(this, tr("Delete environment"),
			tr("Are you sure you want to delete this environment?\nDeleting an environment will delete all references to it and cannot be undone."),
			QMessageBox::Ok | QMessageBox::Cancel);

		if (response == QMessageBox::Ok) {
			_project->getEnvironments()->removeEnvironment(_environmentselection->currentText());

			loadEnvironments();
			if (_environmentselection->count() > 0)
				_environmentselection->setCurrentIndex(0);
		}
	}
}

void EnvironmentsWindow::environmentChanged(const QString &name) {
	if (_environmentselection->count() > 0) {
		Environment *environment = _project->getEnvironments()->getEnvironment(name);
		if (!!environment) {
			if (_currentlayer < g_layers)
				paintEnvironment(environment);
			else
				markEnvironment(environment);
		}
	}
}

void EnvironmentsWindow::environmentCellClicked(int row, int column) {
	if (_currentlayer < g_layers)
		placeTile(QPoint(column, row));
	else
		placeEvent(QPoint(column, row));
}

void EnvironmentsWindow::layerSelected(int layer) {
	_currentlayer = layer;

	if (_environmentselection->count() > 0) {
		Environment *environment = _project->getEnvironments()->getEnvironment(_environmentselection->currentText());
		if (!!environment) {
			if (_currentlayer < g_layers) {
				paintEnvironment(environment);
				_modegroup->button(0)->setEnabled(true);
			} else {
				markEnvironment(environment);
				_modegroup->button(0)->setEnabled(false);
			}
		}
	}
}

void EnvironmentsWindow::displayPreview() {
	if (_environmentselection->count() > 0) {
		Environment *environment = _project->getEnvironments()->getEnvironment(_environmentselection->currentText());
		if (!!environment) {

			// Draw environment
			QSize dimensions = environment->getDimensions();
			QVector<QPixmap> renderedTiles = environment->generateRender();

			QPixmap image(dimensions.width() * g_tilesize, dimensions.height() * g_tilesize);
			QPainter painter(&image);
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.fillRect(0, 0, dimensions.width() * g_tilesize, dimensions.height() * g_tilesize, Qt::white);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

			for (int y = 0; y < dimensions.height(); y++) {
				for (int x = 0; x < dimensions.width(); x++) {
					painter.drawPixmap(x *g_tilesize, y * g_tilesize, renderedTiles.at(dimensions.width() *y + x));
				}
			}

			painter.end();

			// Generate dialog window
			QDialog previewWindow(0);
			previewWindow.setWindowTitle(environment->getName());

			QScrollArea *imageframe = new QScrollArea(&previewWindow);
			QLabel *imagelabel = new QLabel(imageframe);
			imagelabel->setPixmap(image);
			imageframe->setWidget(imagelabel);

			previewWindow.setMinimumSize(qMin<int>(image.width(), 800), qMin<int>(image.height(), 600));

			QVBoxLayout *layout = new QVBoxLayout(&previewWindow);
			layout->addWidget(imageframe);

			previewWindow.exec();
		}
	}
}