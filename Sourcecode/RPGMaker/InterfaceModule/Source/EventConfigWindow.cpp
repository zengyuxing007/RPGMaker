#include "qstringlist.h"
#include "qgridlayout.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/EventConfigWindow.h"

using namespace interfacemodule;

EventConfigWindow::EventConfigWindow(QWidget *parent, Project *project, Event::Type type)
	: _project(project) {

		setWindowTitle("Event configuration");
		setWindowModality(Qt::ApplicationModal);

		createWindow(type);
		loadWindow(type);
}

EventConfigWindow::~EventConfigWindow() {}

QString EventConfigWindow::getName() const {
	return _nameField->text();
}

int EventConfigWindow::getExecutionLimit() const {
	return _executionLimitSelection->value();
}

Event* EventConfigWindow::getFollowupEvent() const {
	Event *event = 0;
	QVector<Event*> events = _project->getEvents()->getEvents();
	if (_followpupEventSelection->currentIndex() > 0) {
		for (int i = 0; i < events.size(); i++) {
			if (events.at(i)->getName() == _followpupEventSelection->currentText()) {
				event = events.at(i);
				break;
			}
		}
	}

	return event;
}

int EventConfigWindow::getBattleEventActivation() const {
	return _battleEventActivationSelection->value();
}

QVector<Event*> EventConfigWindow::getRequirementEvents() const {
	QVector<Event*> requirements(0);
	for (int i = 0; i < _requirementEventSelection->count(); i++) {

		QVector<Event*> events = _project->getEvents()->getEvents();
		for (int j = 0; j < events.size(); j++) {
			if (events.at(j)->getName() == _requirementEventSelection->item(i)->text()) {
				requirements.append(events.at(j));
				break;
			}
		}
	}

	return requirements;
}

void EventConfigWindow::setEvent(Event *event) {
	_event = event;
	_oldName = event->getName();
	_followpupEventSelection->clear();
	_requirementEventSelection->clear();
	_nameField->clear();

	_nameField->setText(event->getName());
	_executionLimitSelection->setValue(event->getLimit());
	if (event->getType() == Event::BATTLE)
		_battleEventActivationSelection->setValue(dynamic_cast<BattleEvent*>(event)->getActivation());

	QVector<Event*> events = _project->getEvents()->getEvents();
	events.remove(events.indexOf(event));
	QVector<Event*> requirements = event->getRequirements();
	for (int i = 0; i < requirements.size(); i++) {
		events.remove(events.indexOf(requirements.at(i)));
		_requirementEventSelection->addItem(requirements.at(i)->getName());
	}

	_followpupEventSelection->addItem(QString());
	for (int i = 0; i < events.size(); i++)
		_followpupEventSelection->addItem(events.at(i)->getName());

	if (!!_event->getFollowup()) {
		_followpupEventSelection->setCurrentIndex(events.indexOf(_event->getFollowup()) +1);
	} else {
		_followpupEventSelection->setCurrentIndex(0);
	}
}

void EventConfigWindow::createWindow(Event::Type type) {
	QGridLayout *layout = new QGridLayout(this);

	// Build the left frame
	QFrame *leftFrame = new QFrame(this);
	QVBoxLayout *leftLayout = new QVBoxLayout(leftFrame);
	layout->addWidget(leftFrame, 0, 0);

	QLabel *label1 = new QLabel("Enter a name for this event:", leftFrame);
	leftLayout->addWidget(label1);

	_nameField = new QLineEdit(leftFrame);
	leftLayout->addWidget(_nameField);

	QLabel *label2 = new QLabel("Select an execution limit for this event.\nA limit set to -1 is unlimited.");
	leftLayout->addWidget(label2);

	_executionLimitSelection = new QSpinBox(leftFrame);
	_executionLimitSelection->setMaximum(9999);
	_executionLimitSelection->setMinimum(-1);
	_executionLimitSelection->setSingleStep(1);
	leftLayout->addWidget(_executionLimitSelection);

	QLabel *label3 = new QLabel("Select a follow up event:", leftFrame);
	leftLayout->addWidget(label3);

	_followpupEventSelection = new QComboBox(leftFrame);
	_followpupEventSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	leftLayout->addWidget(_followpupEventSelection);

	if (type == Event::BATTLE) {
		QLabel *label4 = new QLabel("Select an activation value:", leftFrame);
		leftLayout->addWidget(label4);

		_battleEventActivationSelection = new QSpinBox(leftFrame);
		_battleEventActivationSelection->setMinimum(1);
		_battleEventActivationSelection->setMaximum(100);
		_battleEventActivationSelection->setSingleStep(1);
		leftLayout->addWidget(_battleEventActivationSelection);
	}

	QFrame *leftButtonFrame = new QFrame(leftFrame);
	QHBoxLayout *leftButtonLayout = new QHBoxLayout(leftButtonFrame);
	QPushButton *acceptButton = new QPushButton("Accept", leftButtonFrame);
	QPushButton *cancelButton = new QPushButton("Cancel", leftButtonFrame);
	leftButtonLayout->addWidget(acceptButton);
	leftButtonLayout->addWidget(cancelButton);
	leftLayout->addWidget(leftButtonFrame);

	connect(acceptButton, SIGNAL(clicked()), this, SLOT(checkInput()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	// Build the right frame
	QFrame *rightFrame = new QFrame(this);
	QVBoxLayout *rightLayout = new QVBoxLayout(rightFrame);
	layout->addWidget(rightFrame, 0, 1);

	QLabel *label5 = new QLabel("Add or remove requirement events:", rightFrame);
	rightLayout->addWidget(label5);

	_requirementEventSelection = new QListWidget(rightFrame);
	rightLayout->addWidget(_requirementEventSelection);

	QFrame *rightButtonFrame = new QFrame(leftFrame);
	QHBoxLayout *rightButtonLayout = new QHBoxLayout(rightButtonFrame);
	rightButtonLayout->setAlignment(Qt::AlignCenter);
	QPushButton *addRequirementButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", rightButtonFrame);
	addRequirementButton->setToolTip("Add a requirement event for this event");
	addRequirementButton->setMaximumSize(g_buttonsize, g_buttonsize);
	QPushButton *removeRequirementButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", rightButtonFrame);
	removeRequirementButton->setToolTip("Remove a requirement event for this event");
	removeRequirementButton->setMaximumSize(g_buttonsize, g_buttonsize);
	rightButtonLayout->addWidget(addRequirementButton);
	rightButtonLayout->addWidget(removeRequirementButton);
	rightLayout->addWidget(rightButtonFrame);

	connect(addRequirementButton, SIGNAL(clicked()), this, SLOT(addRequirementEvent()));
	connect(removeRequirementButton, SIGNAL(clicked()), this, SLOT(removeRequirementEvent()));
}

void EventConfigWindow::loadWindow(Event::Type type) {
	_executionLimitSelection->setValue(-1);

	QVector<Event*> events = _project->getEvents()->getEvents();
	_followpupEventSelection->addItem(QString());
	for (int i = 0; i < events.size(); i++)
		_followpupEventSelection->addItem(events.at(i)->getName());

	_requirementEventSelection->clear();
}

void EventConfigWindow::checkInput() {
	if (_nameField->text() == QString()) {
		QMessageBox::critical(this, tr("No empty name allowed"), tr("An event should not have an empty name.\nEnter a name for your event before proceeding."));
		return;
	}

	if ((_oldName != _nameField->text()) && !!_project->getCharacters()->getCharacter(_nameField->text())) {
		QMessageBox::critical(this, tr("Name already in use"), tr("An event with the same name already exists.\nEnter a new name for your event before proceeding."));
		return;
	}

	accept();
}

void EventConfigWindow::addRequirementEvent() {
	QStringList requirementOptions;
	QVector<Event*> events = _project->getEvents()->getEvents();
	for (int i = 0; i < events.size(); i++)
		requirementOptions.append(events.at(i)->getName());

	for (int i = 0; i < _requirementEventSelection->count(); i++)
		requirementOptions.removeAt(requirementOptions.indexOf(_requirementEventSelection->item(i)->text()));

	requirementOptions.removeAt(requirementOptions.indexOf(_nameField->text()));
	requirementOptions.removeAt(requirementOptions.indexOf(_followpupEventSelection->currentText()));

	if (requirementOptions.size() == 0) {
		QMessageBox::information(this, "No events available", "There are no more events available to choose from.");
		return;
	}

	bool accepted;
	QString selectedRequirement = QInputDialog::getItem(this, "Requirement event", "Select a requirement event:", requirementOptions, 0, false, &accepted);
	if (accepted) {
		_requirementEventSelection->addItem(selectedRequirement);

		for (int i = 0; i < _followpupEventSelection->count(); i++) {
			if (_followpupEventSelection->itemText(i) == selectedRequirement) {
				_followpupEventSelection->removeItem(i);
				break;
			}
		}
	}
	
}

void EventConfigWindow::removeRequirementEvent() {
	if ((_requirementEventSelection->count() > 0) && (!!_requirementEventSelection->currentItem())) {
		delete _requirementEventSelection->takeItem(_requirementEventSelection->currentRow());

		// Rebuild the follow up list
		QString currentFollowup = _followpupEventSelection->currentText();
		_followpupEventSelection->clear();

		QStringList followupList;
		QVector<Event*> events = _project->getEvents()->getEvents();
		for (int i = 0; i < events.size(); i++)
			followupList.append(events.at(i)->getName());

		if (!!_event)
			followupList.removeAt(followupList.indexOf(_oldName));

		for (int i = 0; i < _requirementEventSelection->count(); i++)
			followupList.removeAt(followupList.indexOf(_requirementEventSelection->item(i)->text()));

		// Insert items in combobox
		followupList.prepend(QString());
		for (int i = 0; i < followupList.size(); i++) {
			_followpupEventSelection->addItem(followupList.at(i));

			if (followupList.at(i) == currentFollowup)
				_followpupEventSelection->setCurrentIndex(i);
		}
	}
}