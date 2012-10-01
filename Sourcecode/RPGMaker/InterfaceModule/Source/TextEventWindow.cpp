#include "qvector.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qradiobutton.h"
#include "qinputdialog.h"

#include "../ProgramDefaults.h"
#include "../Headers/TextEventWindow.h"
#include "../Headers/EventConfigWindow.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace interfacemodule;

TextEventWindow::TextEventWindow(QWidget *parent, Project *project)
	: QFrame(parent), _project(project) {

		createWindow();
		loadEvents();

		if (_eventsList->count() > 0)
			_eventsList->setCurrentRow(0);
}

TextEventWindow::~TextEventWindow() {}

void TextEventWindow::createWindow() {
	QHBoxLayout *layout = new QHBoxLayout(this);

	{	// Text Event list
		QGroupBox *eventsFrame = new QGroupBox("Text Events", this);
		QVBoxLayout *eventsLayout = new QVBoxLayout(eventsFrame);
		layout->addWidget(eventsFrame);

		QLabel *label = new QLabel("Select a text event:", eventsFrame);
		_eventsList = new QListWidget(eventsFrame);

		QFrame *buttonFrame = new QFrame(eventsFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignCenter);
		QPushButton *newEventButton = new QPushButton(QIcon(g_iconPath + "sq_plus_icon&24.png"), "", buttonFrame);
		newEventButton->setToolTip("Create a new text event");
		newEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *editEventButton = new QPushButton(QIcon(g_iconPath + "pencil_icon&24.png"), "", buttonFrame);
		editEventButton->setToolTip("Edit the current text event");
		editEventButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *deleteEventButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		deleteEventButton->setToolTip("Delete the current text event");
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

		QLabel *triggerLabel = new QLabel("Set a trigger for this text event:", eventsFrame);
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
		connect (newEventButton, SIGNAL(clicked()), this, SLOT(newTextEvent()));
		connect (editEventButton, SIGNAL(clicked()), this, SLOT(editTextEvent()));
		connect (deleteEventButton, SIGNAL(clicked()), this, SLOT(deleteTextEvent()));
		connect (_objectList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(objectSelected(const QString &)));
	}

	{	// List of messages
		QGroupBox *textListFrame = new QGroupBox("Messages", this);
		QVBoxLayout *textListLayout = new QVBoxLayout(textListFrame);
		layout->addWidget(textListFrame);

		QLabel *label = new QLabel("Select a messageblock:", textListFrame);

		QFrame *buttonFrame = new QFrame(textListFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignCenter);
		QPushButton *newTextButton = new QPushButton(QIcon(g_iconPath + "star_icon&24.png"), "", buttonFrame);
		newTextButton->setToolTip("Create a new messageblock");
		newTextButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *deletetextButton = new QPushButton(QIcon(g_iconPath + "delete_icon&24.png"), "", buttonFrame);
		deletetextButton->setToolTip("Delete the current messageblock");
		deletetextButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonLayout->addWidget(newTextButton);
		buttonLayout->addWidget(deletetextButton);
		buttonLayout->addSpacing(20);

		_textList = new QListWidget(textListFrame);

		textListLayout->addWidget(label);
		textListLayout->addWidget(_textList);
		textListLayout->addWidget(buttonFrame);
		connect (_textList, SIGNAL(currentRowChanged(int)), this, SLOT(messageSelected(int)));
		connect (newTextButton, SIGNAL(clicked()), this, SLOT(newMessage()));
		connect (deletetextButton, SIGNAL(clicked()), this, SLOT(deleteMessage()));
	}
	
	{	// Message field
		QGroupBox *textFrame = new QGroupBox("Message", this);
		QVBoxLayout *textLayout = new QVBoxLayout(textFrame);
		layout->addWidget(textFrame);

		QLabel *label = new QLabel("Write or edit the message:", textFrame);

		QFrame *buttonFrame = new QFrame(textFrame);
		QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
		buttonLayout->setAlignment(Qt::AlignCenter);
		QPushButton *saveMessageButton = new QPushButton(QIcon(g_iconPath + "save_icon&24.png"), "", buttonFrame);
		saveMessageButton->setToolTip("Save the current message");
		saveMessageButton->setMaximumSize(g_buttonsize, g_buttonsize);
		QPushButton *reloadMessageButton = new QPushButton(QIcon(g_iconPath + "playback_reload_icon&24.png"), "", buttonFrame);
		reloadMessageButton->setToolTip("Reload the original message");
		reloadMessageButton->setMaximumSize(g_buttonsize, g_buttonsize);
		buttonLayout->addWidget(saveMessageButton);
		buttonLayout->addWidget(reloadMessageButton);
		buttonLayout->addSpacing(20);

		_textField = new QPlainTextEdit(textFrame);

		textLayout->addWidget(label);
		textLayout->addWidget(_textField);
		textLayout->addWidget(buttonFrame);

		connect(saveMessageButton, SIGNAL(clicked()), this, SLOT(saveMessage()));
		connect(reloadMessageButton, SIGNAL(clicked()), this, SLOT(reloadMessage()));
	}
	
}

void TextEventWindow::loadEvents() {
	_eventsList->clear();
	_textList->clear();
	_textField->clear();
	_objectList->clear();

	QVector<TextEvent*> events = _project->getEvents()->getTextEvents();
	for (int i = 0; i < events.size(); i++) {
		TextEvent *it = events.at(i);
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

void TextEventWindow::loadMessages() {
	_textList->clear();
	_textField->clear();

	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {
			QVector<QString> messages = event->getMessages();
			for (int i = 0; i < messages.size(); i++) {
				_textList->addItem("Message " + QString::number(i));
			}
		}
	}
}

void TextEventWindow::updateWindow() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		int index = _eventsList->currentRow();
		loadEvents();
		_eventsList->setCurrentRow(index);
	}
}

void TextEventWindow::eventSelected(const QString &name) {
	_textList->clear();
	_textField->clear();
	_currentObject->clear();

	if ((_eventsList->count() > 0) && (name != QString())) {

		TextEvent *event = _project->getEvents()->getTextEvent(name);
		if (!!event) {
			loadMessages();
			if (_textList->count() > 0)
				messageSelected(0);

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

void TextEventWindow::messageSelected(int index) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_textList->count() > 0) && !!_textList->currentItem()) {

		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {
			_textField->clear();
			_textField->setPlainText(event->getMessage(index));
		}
	}
}

void TextEventWindow::newTextEvent() {

	EventConfigWindow configDialog(0, _project, Event::TEXT);
	if (configDialog.exec()) {

		TextEvent *event = 0;
		try {
			event = TextEvent::create(configDialog.getName(), Event::NONE);
			event->setLimit(configDialog.getExecutionLimit());
			event->setRequirements(configDialog.getRequirementEvents());
			if (!!configDialog.getFollowupEvent())
				event->setFollowup(configDialog.getFollowupEvent());

			_project->getEvents()->addTextEvent(event);

		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			return;
		}

		_eventsList->currentItem()->setText(event->getName());
	}

	loadEvents();
	_eventsList->setCurrentRow(_eventsList->count() -1);
}

void TextEventWindow::editTextEvent() {
	if ((_eventsList->count() > 0) && (!!_eventsList->currentItem())) {
		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {

			EventConfigWindow configDialog(0, _project, Event::TEXT);
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

void TextEventWindow::deleteTextEvent() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {

		QString currentEvent = _eventsList->currentItem()->text();
		TextEvent *event = _project->getEvents()->getTextEvent(currentEvent);

		if (!!event) {

			QMessageBox::StandardButton response = QMessageBox::warning(this,"Delete text event",
				"Are you sure you want to delete the event?\nDeleting an event will delete all references to it, and cannot be undone.",
				QMessageBox::Ok | QMessageBox::Cancel);

			if (response == QMessageBox::Ok) {

				_project->getEvents()->removeTextEvent(currentEvent);

				loadEvents();
				if (_eventsList->count() > 0)
					_eventsList->setCurrentRow(0);
			}
		}
	}
}

void TextEventWindow::moveTextEventUp() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_eventsList->currentRow() != 0)) {

		TextEvent *currentEvent = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!currentEvent) {
			const QVector<TextEvent*>& events = _project->getEvents()->getTextEvents();
			int index = events.indexOf(currentEvent);

			try {
				switchPlaces(const_cast<QVector<TextEvent*>&>(events), index, index-1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEvents();
			_eventsList->setCurrentRow(index-1);
		}
	}
}

void TextEventWindow::moveTextEventDown() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_eventsList->currentRow() != _eventsList->count()-1)) {

		TextEvent *currentEvent = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!currentEvent) {
			const QVector<TextEvent*>& events = _project->getEvents()->getTextEvents();
			int index = events.indexOf(currentEvent);

			try {
				switchPlaces(const_cast<QVector<TextEvent*>&>(events), index, index+1);
			} catch (ProjectException &e) {
				QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
				return;
			}

			loadEvents();
			_eventsList->setCurrentRow(index+1);
		}
	}
}

void TextEventWindow::triggerChanged(int id) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
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

void TextEventWindow::objectSelected(const QString &name) {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_objectList->count() > 0)) {

		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
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

void TextEventWindow::newMessage() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem()) {
		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {
			event->addMessage(QString());
			
			loadMessages();
			_textList->setCurrentRow(_textList->count() -1);
		}
	}
}

void TextEventWindow::deleteMessage() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_textList->count() > 0) && !!_textList->currentItem()) {

		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {

			event->removeMessage(_textList->currentRow());
			loadMessages();
			if (_textList->count() > 0)
				_textList->setCurrentRow(0);
		}
	}
}

void TextEventWindow::saveMessage() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_textList->count() > 0) && !!_textList->currentItem()) {
		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {
			event->editMessage(_textField->toPlainText(), _textList->currentRow());
		}
	}
}

void TextEventWindow::reloadMessage() {
	if ((_eventsList->count() > 0) && !!_eventsList->currentItem() && (_textList->count() > 0) && !!_textList->currentItem()) {
		TextEvent *event = _project->getEvents()->getTextEvent(_eventsList->currentItem()->text());
		if (!!event) {
			_textField->setPlainText(event->getMessage(_textList->currentRow()));
		}
	}
}