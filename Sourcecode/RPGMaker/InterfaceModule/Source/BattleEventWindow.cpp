#include "qvector.h"
#include "qpainter.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qheaderview.h"
#include "qradiobutton.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/BattleEventWindow.h"
#include "../Headers/EventConfigWindow.h"
#include "../Components/Headers/Battlegroup.h"
#include "../Components/Headers/BattleEvent.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;

BattleEventWindow::BattleEventWindow(QWidget *parent, Project *project)
	: QFrame(parent), _project(project) {

		createWindow();
		loadEvents();

		if (_eventsList->count() > 0)
			_eventsList->setCurrentRow(0);
}

BattleEventWindow::~BattleEventWindow() {}

void BattleEventWindow::createWindow() {
	QHBoxLayout *layout = new QHBoxLayout(this);

	{	// Battle Event list
		QGroupBox *eventsFrame = new QGroupBox("Battle Events", this);
		QVBoxLayout *eventsLayout = new QVBoxLayout(eventsFrame);
		layout->addWidget(eventsFrame);

		QLabel *label = new QLabel("Select a battle event:", eventsFrame);
		_eventsList = new QListWidget(eventsFrame);

		QFrame *buttonFrame = new QFrame(eventsFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignCenter);
		QPushButton *newEventButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonFrame);
		newEventButton->setToolTip("Create a new battle event");
		newEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *editEventButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonFrame);
		editEventButton->setToolTip("Edit the current battle event");
		editEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *deleteEventButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		deleteEventButton->setToolTip("Delete the current battle event");
		deleteEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *eventUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", buttonFrame);
		eventUpButton->setToolTip("Move selected event up");
		eventUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *eventDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", buttonFrame);
		eventDownButton->setToolTip("Move selected event down");
		eventDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonLayout->addWidget(newEventButton);
		buttonLayout->addWidget(editEventButton);
		buttonLayout->addWidget(deleteEventButton);
		buttonLayout->addSpacing(20);
		buttonLayout->addWidget(eventUpButton);
		buttonLayout->addWidget(eventDownButton);

		QLabel *triggerLabel = new QLabel("Set a trigger for this battle event:", eventsFrame);
		QFrame *triggerFrame = new QFrame(eventsFrame);
		QHBoxLayout *triggerLayout = new QHBoxLayout(triggerFrame);
		_triggerButtons = new QButtonGroup(eventsFrame);
		_triggerButtons->setExclusive(true);
		QRadioButton *touchTriggerButton = new QRadioButton("Touch", triggerFrame);
		QRadioButton *interactTriggerButton = new QRadioButton("Interact", triggerFrame);
		QRadioButton *noneTriggerButton = new QRadioButton("None", triggerFrame);
		_triggerButtons->addButton(touchTriggerButton, 0);
		_triggerButtons->addButton(interactTriggerButton, 1);
		_triggerButtons->addButton(noneTriggerButton, 2);
		triggerLayout->addWidget(triggerLabel);
		triggerLayout->addSpacing(20);
		triggerLayout->addWidget(touchTriggerButton);
		triggerLayout->addWidget(interactTriggerButton);
		triggerLayout->addWidget(noneTriggerButton);

		QGroupBox *objectFrame = new QGroupBox("Objects" ,eventsFrame);
		QVBoxLayout *objectsLayout = new QVBoxLayout(objectFrame);
		QLabel *objectLabel = new QLabel("Select an object to associate with this event:", objectFrame);
		_objectList = new QComboBox(objectFrame);
		_objectList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		_objectList->setInsertPolicy(QComboBox::InsertAlphabetically);
		_currentObject = new QLabel(objectFrame);
		objectsLayout->addWidget(objectLabel);
		objectsLayout->addWidget(_objectList);
		objectsLayout->addWidget(_currentObject);

		eventsLayout->addWidget(label);
		eventsLayout->addWidget(triggerFrame);
		eventsLayout->addWidget(_eventsList);
		eventsLayout->addWidget(buttonFrame);
		eventsLayout->addWidget(objectFrame);

		connect (_eventsList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(eventSelected(const QString &)));
		connect (_triggerButtons, SIGNAL(buttonPressed(int)), this, SLOT(triggerChanged(int)));
		connect (newEventButton, SIGNAL(clicked()), this, SLOT(newBattleEvent()));
		connect (editEventButton, SIGNAL(clicked()), this, SLOT(editBattleEvent()));
		connect (deleteEventButton, SIGNAL(clicked()), this, SLOT(deleteBattleEvent()));
		connect(eventUpButton, SIGNAL(clicked()), this, SLOT(moveBattleEventUp()));
		connect(eventDownButton, SIGNAL(clicked()), this, SLOT(moveBattleEventDown()));
		connect (_objectList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(objectSelected(const QString &)));
	}

	{	// List of battlegroups
		QGroupBox *battlegroupListFrame = new QGroupBox("Battlegroups", this);
		QVBoxLayout *battlegroupListLayout = new QVBoxLayout(battlegroupListFrame);
		layout->addWidget(battlegroupListFrame);

		QLabel *label = new QLabel("Select a battlegroup:", battlegroupListFrame);

		QFrame *buttonFrame = new QFrame(battlegroupListFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignCenter);
		QPushButton *addBattlegroupButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", buttonFrame);
		addBattlegroupButton->setToolTip("Add a battlegroup");
		addBattlegroupButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *removeBattlegroupButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		removeBattlegroupButton->setToolTip("Remove the current battlegroup from the current event");
		removeBattlegroupButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *battlegroupUpButton = new QPushButton(QIcon(g_iconPath + "arrow_top_icon&24.png"), "", buttonFrame);
		battlegroupUpButton->setToolTip("Move selected battlegroup up");
		battlegroupUpButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *battlegroupDownButton = new QPushButton(QIcon(g_iconPath + "arrow_bottom_icon&24.png"), "", buttonFrame);
		battlegroupDownButton->setToolTip("Move selected battlegroup down");
		battlegroupDownButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonLayout->addWidget(addBattlegroupButton);
		buttonLayout->addWidget(removeBattlegroupButton);
		buttonLayout->addSpacing(20);
		buttonLayout->addWidget(battlegroupUpButton);
		buttonLayout->addWidget(battlegroupDownButton);

		_groupsList = new QListWidget(battlegroupListFrame);

		battlegroupListLayout->addWidget(label);
		battlegroupListLayout->addWidget(_groupsList);
		battlegroupListLayout->addWidget(buttonFrame);

		connect (addBattlegroupButton, SIGNAL(clicked()), this, SLOT(addBattlegroup()));
		connect (removeBattlegroupButton, SIGNAL(clicked()), this, SLOT(removeBattlegroup()));
		connect(battlegroupUpButton, SIGNAL(clicked()), this, SLOT(moveBattlegroupUp()));
		connect(battlegroupDownButton, SIGNAL(clicked()), this, SLOT(moveBattlegroupDown()));
	}

	{	// Battle event placement
		QGroupBox *environmentFrame = new QGroupBox("Locations", this);
		QVBoxLayout *environmentLayout = new QVBoxLayout(environmentFrame);
		layout->addWidget(environmentFrame);

		QFrame *environmentSelectionFrame = new QFrame(environmentFrame);
		QHBoxLayout *environmentSelectionLayout = new QHBoxLayout(environmentSelectionFrame);

		QLabel *label = new QLabel("Select the locations where this battle event should occur:", environmentSelectionFrame);
		_environmentList = new QComboBox(environmentSelectionFrame);
		_environmentList->setInsertPolicy(QComboBox::InsertAlphabetically);
		_environment = new QTableWidget(environmentFrame);
		_environment->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_environment->setSelectionBehavior(QAbstractItemView::SelectItems);
		_environment->setSelectionMode(QAbstractItemView::SingleSelection);
		_environment->horizontalHeader()->setDefaultSectionSize(g_tilesize);
		_environment->verticalHeader()->setDefaultSectionSize(g_tilesize);
		_environment->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
		_environment->verticalHeader()->setResizeMode(QHeaderView::Fixed);
		_environment->horizontalHeader()->hide();
		_environment->verticalHeader()->hide();
		environmentSelectionLayout->addWidget(label);
		environmentSelectionLayout->addSpacing(20);
		environmentSelectionLayout->addWidget(_environmentList);

		environmentLayout->addWidget(environmentSelectionFrame);
		environmentLayout->addWidget(_environment);

		connect(_environmentList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(environmentSelected(const QString &)));
		connect(_environment, SIGNAL(cellClicked(int, int)), this, SLOT(environmentCellClicked(int , int)));
	}
}

void BattleEventWindow::loadEvents() {
	_eventsList->clear();
	_groupsList->clear();
	_objectList->clear();
	_currentObject->clear();

	_environmentList->clear();
	_environment->clear();
	_environment->setRowCount(0);
	_environment->setColumnCount(0);

	QVector<BattleEvent*> events = _project->getEvents()->getBattleEvents();
	for (int i = 0; i < events.size(); i++) {
		BattleEvent *it = events.at(i);
		if (!!it)
			_eventsList->addItem(it->getName());
	}

	if (_eventsList->count() > 0) {
		_objectList->addItem(QString());
		const QVector<Object*> objects = _project->getObjects()->getObjects();
		for (int i = 0; i < objects.size(); i++) {
			_objectList->addItem(objects.at(i)->getName());
		}
	}
}

void BattleEventWindow::loadBattlegroups() {
	_groupsList->clear();

	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());

		if (!!event) {
			QVector<QPair<Battlegroup*, int>> groups = event->getBattlegroups();
			for (int i = 0; i < groups.size(); i++) {
				Battlegroup *it = groups.at(i).first;
				if (!!it)
					_groupsList->addItem(it->getName());
			}
		}
	}
}

void BattleEventWindow::paintEnvironment(Environment *environment) {
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

void BattleEventWindow::paintTile(Environment *environment, QPoint location) {
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

void BattleEventWindow::markTile(Environment *environment, QPoint location) {
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

void BattleEventWindow::updateWindow() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		int index = _eventsList->currentRow();
		loadEvents();
		_eventsList->setCurrentRow(index);
	}
}

void BattleEventWindow::eventSelected(const QString &name) {
	_groupsList->clear();
	_environmentList->clear();
	_environment->clear();
	_environment->setRowCount(0);
	_environment->setColumnCount(0);

	if ((_eventsList->count() > 0) && (name != QString())) {

		QVector<Environment*> environments = _project->getEnvironments()->getEnvironments();
		for (int i = 0; i < environments.size(); i++) {
			_environmentList->addItem(environments.at(i)->getName());
		}

		BattleEvent *event = _project->getEvents()->getBattleEvent(name);
		if (!!event) {
			loadBattlegroups();
			if (_groupsList->count() > 0)
				_groupsList->setCurrentRow(0);

			if (!!event->getObject()) {
				for (int i = 0; i < _objectList->count(); i++) {
					if (_objectList->itemText(i) == event->getObject()->getName()) {
						_objectList->setCurrentIndex(i);
						break;
					}
				}
			}

			if (!!event->getObject()) {
				QList<QAbstractButton*> buttons = _triggerButtons->buttons();
				for (int i = 0; i < buttons.size(); i++) {
					buttons.at(i)->setEnabled(false);
				}
			} else {
				QList<QAbstractButton*> buttons = _triggerButtons->buttons();
				for (int i = 0; i < buttons.size(); i++) {
					buttons.at(i)->setEnabled(true);
				}

				switch (event->getTrigger()) {
				case Event::TOUCH:
					_triggerButtons->button(0)->setChecked(true);
					break;
				case Event::INTERACT:
					_triggerButtons->button(1)->setChecked(true);
					break;
				case Event::NONE:
					_triggerButtons->button(2)->setChecked(true);
					break;
				}
			}
		}
	}
}

void BattleEventWindow::newBattleEvent() {
	EventConfigWindow configDialog(0, _project, Event::BATTLE);
	if (configDialog.exec()) {

		BattleEvent *event = 0;
		try {
			event = BattleEvent::create(configDialog.getName(), Event::NONE);
			event->setLimit(configDialog.getExecutionLimit());
			event->setRequirements(configDialog.getRequirementEvents());
			if (!!configDialog.getFollowupEvent())
				event->setFollowup(configDialog.getFollowupEvent());

			event->setActivation(configDialog.getBattleEventActivation());

			_project->getEvents()->addBattleEvent(event);

		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			return;
		}

		_eventsList->currentItem()->setText(event->getName());
	}

	loadEvents();
	_eventsList->setCurrentRow(_eventsList->count() -1);
}

void BattleEventWindow::editBattleEvent() {
	if ((_eventsList->count() > 0) && (!!_eventsList->currentItem())) {
		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {

			EventConfigWindow configDialog(0, _project, Event::BATTLE);
			configDialog.setEvent(event);
			if (configDialog.exec()) {

				try {
					event->setName(configDialog.getName());
					event->setLimit(configDialog.getExecutionLimit());
					event->setRequirements(configDialog.getRequirementEvents());

					if (!!configDialog.getFollowupEvent())
						event->setFollowup(configDialog.getFollowupEvent());

					event->setActivation(configDialog.getBattleEventActivation());

				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_eventsList->currentItem()->setText(event->getName());
			}
		}
	}
}

void BattleEventWindow::deleteBattleEvent() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {

		QString currentEvent = _eventsList->currentItem()->text();
		BattleEvent *event = _project->getEvents()->getBattleEvent(currentEvent);

		if (!!event) {

			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete battle event",
				"Are you sure you want to delete the event?\nDeleting an event will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_project->getEvents()->removeBattleEvent(currentEvent);

				loadEvents();
				if (_eventsList->count() > 0)
					_eventsList->setCurrentRow(0);
			}
		}
	}
}

void BattleEventWindow::moveBattleEventUp() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_eventsList->currentRow() != 0)) {

		BattleEvent *currentEvent = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!currentEvent) {
			const QVector<BattleEvent*>& events = _project->getEvents()->getBattleEvents();
			int index = events.indexOf(currentEvent);

			try {
				switchPlaces(const_cast<QVector<BattleEvent*>&>(events), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEvents();
			_eventsList->setCurrentRow(index-1);
		}
	}
}

void BattleEventWindow::moveBattleEventDown() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_eventsList->currentRow() != _eventsList->count()-1)) {

		BattleEvent *currentEvent = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!currentEvent) {
			const QVector<BattleEvent*>& events = _project->getEvents()->getBattleEvents();
			int index = events.indexOf(currentEvent);

			try {
				switchPlaces(const_cast<QVector<BattleEvent*>&>(events), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEvents();
			_eventsList->setCurrentRow(index+1);
		}
	}
}

void BattleEventWindow::triggerChanged(int id) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {
			switch (id) {
			case 0:
				event->setTrigger(Event::TOUCH);
				break;
			case 1:
				event->setTrigger(Event::INTERACT);
				break;
			case 2:
				event->setTrigger(Event::NONE);
				break;
			}
		}
	}
}

void BattleEventWindow::playMusic() {

}

void BattleEventWindow::stopMusic() {

}

void BattleEventWindow::objectSelected(const QString &name) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_objectList->count() > 0)) {

		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {
			if (name == QString()) {
				QList<QAbstractButton*> buttons = _triggerButtons->buttons();
				for (int i = 0; i < buttons.size(); i++) {
					buttons.at(i)->setEnabled(true);
				}

				event->unsetObject();
				_triggerButtons->button(2)->click();

				_currentObject->clear();
				_currentObject->setText("No object selected");

			} else {
				Object *object = _project->getObjects()->getObject(name);
				if (!!object) {
					event->setObject(object);
					_triggerButtons->button(1)->click();

					_currentObject->clear();
					_currentObject->setPixmap(object->getImage());

					QList<QAbstractButton*> buttons = _triggerButtons->buttons();
					for (int i = 0; i < buttons.size(); i++) {
						buttons.at(i)->setEnabled(false);
					}
				}
			}
		}
	}
}

void BattleEventWindow::addBattlegroup() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {
			QVector<QPair<Battlegroup*, int>> registeredGroups = event->getBattlegroups();
			QVector<Battlegroup*> groups = _project->getBattlegroups()->getBattlegroups();

			QStringList choices;
			for (int i = 0; i < groups.size(); i++) {
				Battlegroup *it = groups.at(i);

				for (int j = 0; j < registeredGroups.size(); j++) {
					if (registeredGroups.at(j).first->getName() == it->getName())
						goto skip;
				}

				choices << it->getName();

				skip:;
			}

			if (choices.size() == 0) {
				QMessageBox::information(this, tr("No more battlegroups available"), tr("There are no more battlegroups available to choose from."));
				return;
			}

			bool accepted;
			QString choice = QInputDialog::getItem(this, tr("Select battlegroup"), tr("Select a battlegroup to add to this battle event:"), choices, 0, false, &accepted);
			if (accepted) {
				Battlegroup *group = _project->getBattlegroups()->getBattlegroup(choice);
				if (!!group) {
					try {
						event->addBattlegroup(group, 1);
					} catch (ProjectException &e) {
						QMessageBox::critical(this, "An unexpected error has occurred", e.what());
						return;
					}

					loadBattlegroups();
				}
			}
		}
	}
}

void BattleEventWindow::removeBattlegroup() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_groupsList->count() > 0) && !!_groupsList->currentItem()) {
		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {

			QPair<Battlegroup*, int> group = event->getBattlegroup(_groupsList->currentItem()->text());

			if (!!group.first) {
				event->removeBattlegroup(group.first->getName());
				loadBattlegroups();
			}
		}
	}
}

void BattleEventWindow::moveBattlegroupUp() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_groupsList->count() > 0) && !!_groupsList->currentItem() && (_groupsList->currentRow() != 0)) {

		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {

			QPair<Battlegroup*, int> group = event->getBattlegroups().at(_groupsList->currentRow());
			if (!!group.first) {
				const QVector<QPair<Battlegroup*, int>>& groups = event->getBattlegroups();
				int index = groups.indexOf(group);

				try {
					switchPairs(const_cast<QVector<QPair<Battlegroup*, int>>&>(groups), index, index-1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int eventIndex = _eventsList->currentRow();
				loadEvents();
				_eventsList->setCurrentRow(eventIndex);
				_groupsList->setCurrentRow(index-1);
			}
		}
	}
}

void BattleEventWindow::moveBattlegroupDown() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_groupsList->count() > 0) && !!_groupsList->currentItem() && (_groupsList->currentRow() != _groupsList->count()-1)) {

		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {

			QPair<Battlegroup*, int> group = event->getBattlegroups().at(_groupsList->currentRow());
			if (!!group.first) {
				const QVector<QPair<Battlegroup*, int>>& groups = event->getBattlegroups();
				int index = groups.indexOf(group);

				try {
					switchPairs(const_cast<QVector<QPair<Battlegroup*, int>>&>(groups), index, index+1);
				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				int eventIndex = _eventsList->currentRow();
				loadEvents();
				_eventsList->setCurrentRow(eventIndex);
				_groupsList->setCurrentRow(index+1);
			}
		}
	}
}

void BattleEventWindow::environmentSelected(const QString &name) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_environmentList->count() > 0)) {
		Environment *environment = _project->getEnvironments()->getEnvironment(name);
		if (!!environment) {
			paintEnvironment(environment);
		}
	}
}

void BattleEventWindow::environmentCellClicked(int row, int column) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_environmentList->count() > 0)) {
		BattleEvent *event = _project->getEvents()->getBattleEvent(_eventsList->currentItem()->text());
		if (!!event) {

			Environment *environment = _project->getEnvironments()->getEnvironment(_environmentList->currentText());
			if (!!environment) {

				QPoint location(column, row);
				if (!!environment->getEvent(location) && (environment->getEvent(location)->getType() == event->getType())) {
					environment->removeEvent(location);
					paintTile(environment, location);
				} else {
					environment->setEvent(event, location);
					markTile(environment, location);
				}
			}
		}
	}
}