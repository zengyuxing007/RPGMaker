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
#include "../Headers/PortalEventWindow.h"
#include "../Headers/EventConfigWindow.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;

PortalEventWindow::PortalEventWindow(QWidget *parent, Project *project)
	: QFrame(parent), _project(project) {

		createWindow();
		loadEvents();

		if (_eventsList->count() > 0)
			_eventsList->setCurrentRow(0);
}

PortalEventWindow::~PortalEventWindow() {}

void PortalEventWindow::createWindow() {
	QHBoxLayout *layout = new QHBoxLayout(this);

	{	// Portal Event list
		
		QGroupBox *eventsFrame = new QGroupBox("Portal Events", this);
		QVBoxLayout *eventsLayout = new QVBoxLayout(eventsFrame);
		layout->addWidget(eventsFrame);

		QLabel *label = new QLabel("Select a portal event:", eventsFrame);
		_eventsList = new QListWidget(eventsFrame);

		QFrame *buttonFrame = new QFrame(eventsFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignCenter);
		QPushButton *newEventButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonFrame);
		newEventButton->setToolTip("Create a new portal event");
		newEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *editEventButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonFrame);
		editEventButton->setToolTip("Edit the current portal event");
		editEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *deleteEventButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		deleteEventButton->setToolTip("Delete the current portal event");
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

		QLabel *triggerLabel = new QLabel("Set a trigger for this portal event:", eventsFrame);
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
		connect (newEventButton, SIGNAL(clicked()), this, SLOT(newPortalEvent()));
		connect (editEventButton, SIGNAL(clicked()), this, SLOT(editPortalEvent()));
		connect (deleteEventButton, SIGNAL(clicked()), this, SLOT(deletePortalEvent()));
		connect(eventUpButton, SIGNAL(clicked()), this, SLOT(movePortalEventUp()));
		connect(eventDownButton, SIGNAL(clicked()), this, SLOT(movePortalEventDown()));
		connect (_objectList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(objectSelected(const QString &)));
	}

	{
		QGroupBox *environmentFrame = new QGroupBox("Destination", this);
		QVBoxLayout *environmentLayout = new QVBoxLayout(environmentFrame);
		layout->addWidget(environmentFrame);

		QFrame *environmentSelectionFrame = new QFrame(environmentFrame);
		QHBoxLayout *environmentSelectionLayout = new QHBoxLayout(environmentSelectionFrame);

		QLabel *label = new QLabel("Select an environment and click on a cell to set the portal's destination:", environmentSelectionFrame);
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

void PortalEventWindow::loadEvents() {
	_eventsList->clear();
	_objectList->clear();
	_environmentList->clear();

	_environment->clearContents();
	_environment->setColumnCount(0);
	_environment->setRowCount(0);

	QVector<PortalEvent*> events = _project->getEvents()->getPortalEvents();
	for (int i = 0; i < events.size(); i++) {
		PortalEvent *it = events.at(i);
		if (!!it)
			_eventsList->addItem(it->getName());
	}

	if (_eventsList->count() > 0) {
		_objectList->addItem(QString());
		QVector<Object*> objects = _project->getObjects()->getObjects();
		for (int i = 0; i < objects.size(); i++) {
			_objectList->addItem(objects.at(i)->getName());
		}
	}
}

void PortalEventWindow::paintEnvironment(Environment *environment) {
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

void PortalEventWindow::paintTile(Environment *environment, QPoint location) {
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

void PortalEventWindow::markTile(Environment *environment, QPoint location) {
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
	painter.fillRect(compositiontile.rect(), Qt::green);
	painter.setOpacity(1);
	painter.end();

	// Place the new contents of the cell
	QLabel *label = new QLabel();
	label->setPixmap(compositiontile);
	_environment->setCellWidget(location.y(), location.x(), label);
}

void PortalEventWindow::updateWindow() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		int index = _eventsList->currentRow();
		loadEvents();
		_eventsList->setCurrentRow(index);
	}
}

void PortalEventWindow::eventSelected(const QString &name) {
	_environmentList->clear();
	_currentObject->clear();
	_environment->clear();
	_environment->setColumnCount(0);
	_environment->setRowCount(0);

	if ((_eventsList->count() > 0) && (name != QString())) {
		PortalEvent *event = _project->getEvents()->getPortalEvent(name);
		if (!!event) {

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

			QVector<Environment*> environments = _project->getEnvironments()->getEnvironments();
			for (int i = 0; i < environments.size(); i++) {
				Environment *it = environments.at(i);
				if (!!it)
					_environmentList->addItem(it->getName());
			}

			if (!!event->getEnvironment()) {
				QString envName = event->getEnvironment()->getName();
				for (int i = 0; i < environments.size(); i++) {
					Environment *it = environments.at(i);
					if (event->getEnvironment()->getName() == it->getName()) {
						_environmentList->setCurrentIndex(i);
						break;
					}
				}
			}

		}
	}
}

void PortalEventWindow::newPortalEvent() {
	EventConfigWindow configDialog(0, _project, Event::PORTAL);
	if (configDialog.exec()) {

		PortalEvent *event = 0;
		try {
			event = PortalEvent::create(configDialog.getName(), Event::NONE);
			event->setLimit(configDialog.getExecutionLimit());
			event->setRequirements(configDialog.getRequirementEvents());
			if (!!configDialog.getFollowupEvent())
				event->setFollowup(configDialog.getFollowupEvent());

			_project->getEvents()->addPortalEvent(event);

		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			return;
		}

		_eventsList->currentItem()->setText(event->getName());
	}

	loadEvents();
	_eventsList->setCurrentRow(_eventsList->count() -1);
}

void PortalEventWindow::editPortalEvent() {
	if ((_eventsList->count() > 0) && (!!_eventsList->currentItem())) {
		PortalEvent *event = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
		if (!!event) {

			EventConfigWindow configDialog(0, _project, Event::PORTAL);
			configDialog.setEvent(event);
			if (configDialog.exec()) {

				try {
					event->setName(configDialog.getName());
					event->setLimit(configDialog.getExecutionLimit());
					event->setRequirements(configDialog.getRequirementEvents());

					if (!!configDialog.getFollowupEvent())
						event->setFollowup(configDialog.getFollowupEvent());

				} catch (ProjectException &e) {
					QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
					return;
				}

				_eventsList->currentItem()->setText(event->getName());
			}
		}
	}
}

void PortalEventWindow::deletePortalEvent() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {

		QString currentEvent = _eventsList->currentItem()->text();
		PortalEvent *event = _project->getEvents()->getPortalEvent(currentEvent);

		if (!!event) {

			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete portal event",
				"Are you sure you want to delete the event?\nDeleting an event will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_project->getEvents()->removePortalEvent(currentEvent);

				loadEvents();
				if (_eventsList->count() > 0)
					_eventsList->setCurrentRow(0);
			}
		}
	}
}

void PortalEventWindow::movePortalEventUp() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_eventsList->currentRow() != 0)) {

		PortalEvent *currentEvent = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
		if (!!currentEvent) {
			const QVector<PortalEvent*>& events = _project->getEvents()->getPortalEvents();
			int index = events.indexOf(currentEvent);

			try {
				switchPlaces(const_cast<QVector<PortalEvent*>&>(events), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEvents();
			_eventsList->setCurrentRow(index-1);
		}
	}
}

void PortalEventWindow::movePortalEventDown() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_eventsList->currentRow() != _eventsList->count()-1)) {

		PortalEvent *currentEvent = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
		if (!!currentEvent) {
			const QVector<PortalEvent*>& events = _project->getEvents()->getPortalEvents();
			int index = events.indexOf(currentEvent);

			try {
				switchPlaces(const_cast<QVector<PortalEvent*>&>(events), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEvents();
			_eventsList->setCurrentRow(index+1);
		}
	}
}

void PortalEventWindow::triggerChanged(int id) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		PortalEvent *event = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
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

void PortalEventWindow::objectSelected(const QString &name) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_objectList->count() > 0)) {

		PortalEvent *event = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
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

void PortalEventWindow::environmentSelected(const QString &name) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_environmentList->count() > 0)) {
		PortalEvent *event = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
		if (!!event) {

			Environment *environment = _project->getEnvironments()->getEnvironment(name);
			if (!!environment) {
				paintEnvironment(environment);

				if (!!event->getEnvironment() && (event->getEnvironment()->getName() == name)) {
					markTile(environment, event->getLocation());
				}
			}
		}
	}
}

void PortalEventWindow::environmentCellClicked(int row, int column) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_environmentList->count() > 0)) {
		PortalEvent *event = _project->getEvents()->getPortalEvent(_eventsList->currentItem()->text());
		if (!!event) {

			Environment *environment = _project->getEnvironments()->getEnvironment(_environmentList->currentText());
			if (!!environment) {

				if (!!event->getEnvironment() && (event->getEnvironment()->getName() == environment->getName()))
					paintTile(environment, event->getLocation());

				event->setDestination(environment, QPoint(column, row));
				markTile(environment, QPoint(column, row));
			}
		}
	}
}