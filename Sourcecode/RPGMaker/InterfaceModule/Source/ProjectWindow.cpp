#include "qtabwidget.h"
#include "qmessagebox.h"
#include "qlayoutitem.h"
#include "qinputdialog.h"
#include "qerrormessage.h"

#include "../Headers/EventsWindow.h"
#include "../Headers/ProjectWindow.h"
#include "../Headers/PlayConfigWindow.h"
#include "../Headers/PropertiesWindow.h"
#include "../Headers/EnvironmentsWindow.h"
#include "../Headers/CharactersAndEnemiesWindow.h"

#include "../PlayModule/Headers/PlayModule.h"
#include "../PlayModule/Headers/PlayException.h"

using playmodule::PlayModule;
using playmodule::PlayException;
using namespace interfacemodule;

ProjectWindow::ProjectWindow(QWidget *parent, Project *project)
	: QFrame(parent), _project(project) {

		_layout = new QStackedLayout(this);

		_currentFrame = new EnvironmentsWindow(0, _project);
		_layout->addWidget(_currentFrame);
}

ProjectWindow::~ProjectWindow() {
	if (!!_currentFrame)
		delete _currentFrame;
}

void ProjectWindow::renameSlot() {
	bool accepted;
	QString title = QInputDialog::getText(this, tr("Name your project"),
		tr("Enter a new title for your project:"), QLineEdit::Normal, _project->getTitle(), &accepted);

	if (accepted) {
		try {
			_project->setTitle(title);
		} catch (ProjectException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			return;
		}
	}
}

void ProjectWindow::propertiesSlot() {
	if (!!_currentFrame) {
		QLayoutItem *item = _layout->takeAt(0);
		delete item;

		delete _currentFrame;
		_currentFrame = new PropertiesWindow(0, _project->getProperties());
		_layout->addWidget(_currentFrame);
	}
}

void ProjectWindow::charactersAndEnemiesSlot() {
	if (!!_currentFrame) {
		QLayoutItem *item = _layout->takeAt(0);
		delete item;

		delete _currentFrame;
		_currentFrame = new CharactersAndEnemiesWindow(0, _project);
		_layout->addWidget(_currentFrame);
	}
}

void ProjectWindow::objectsAndEventsSlot() {
	if (!!_currentFrame) {
		QLayoutItem *item = _layout->takeAt(0);
		delete item;

		delete _currentFrame;
		_currentFrame = new EventsWindow(0, _project);
		_layout->addWidget(_currentFrame);
	}
}

void ProjectWindow::tilesetsAndEnvironmentsSlot() {
	if (!!_currentFrame) {
		QLayoutItem *item = _layout->takeAt(0);
		delete item;

		delete _currentFrame;
		_currentFrame = new EnvironmentsWindow(0, _project);
		_layout->addWidget(_currentFrame);
	}
}

void ProjectWindow::playSlot() {

	PlayConfigWindow playConfig(0, _project);
	
	if (playConfig.exec()) {
		PlayModule *playModule = 0;
		try {
			playModule = PlayModule::create(this, _project);
		} catch (PlayException &e) {
			QMessageBox::critical(this, tr("An unexpected error occurred"), e.what());
			return;
		}

		playModule->showNormal();
		playModule->play();
	}
}