#include "qicon.h"
#include "qregexp.h"
#include "qaction.h"
#include "qtoolbar.h"
#include "qmenubar.h"
#include "qvalidator.h"
#include "qstatusbar.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include "qerrormessage.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/InterfaceModule.h"
#include "../XMLModule/Headers/XMLMobule.h"

using xmlmodule::XMLModule;
using namespace interfacemodule;

InterfaceModule::InterfaceModule() 
	: _project(0),
	_projectWindow(0) { 

	createActions();
	createMenus();
	createToolbars();
	createStatusbar();
}

InterfaceModule::~InterfaceModule() {
}

void InterfaceModule::newProjectSlot() {
	// If a project is already open, ask if it should be closed.
	if (!!_project) {
		if (!closeProject())
			return;

		if (!!_projectWindow)
			delete _projectWindow;
		
		_projectWindow = 0;
	}

	// Show dialogbox asking the title of the new project.
	bool accepted;
	QString title = QInputDialog::getText(this, tr("Name your project"),
				tr("Enter a title for your project:"), QLineEdit::Normal, QString(""), &accepted);

	QRegExpValidator validator(g_nameRegex);
	int pos = 0;

	if (accepted && (validator.validate(title, pos) != QValidator::Acceptable)) {
		QMessageBox::critical(this, "Not a valid name", "A name should contain a maximum of 25 alphanumeric characters and spaces.");
		newProjectSlot();
		return;
	}

	// If accepted, try to create the project.
	if (accepted) {
		try {
			_project = Project::create(title);
		} catch (ProjectException &e) {
			// Show error and clear logger and unset _project
			QErrorMessage *errorDialog = QErrorMessage::qtHandler();
			errorDialog->showMessage(e.what());
			_project = 0;
			Logger::instance()->clearmessages();
			return;
		}

		_projectWindow = new ProjectWindow(this, _project);
		connectProjectActions();
		setCentralWidget(_projectWindow);
	}
}

void InterfaceModule::openSlot() {
	// If a project is already open, ask if it should be closed.
	if (!!_project) {
		if (!closeProject())
			return;

		if (!!_projectWindow)
			delete _projectWindow;

		_projectWindow = 0;
	}

	// Show dialogbox asking the path to the project to be opened.
	QString projectpath = QString();
	projectpath = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("RPGMaker files (*.rpgm)"));

	// If accepted, try to open the project.
	if (projectpath != QString()) {
		XMLModule xmlmodule;
		try {
			_project = xmlmodule.openProject(projectpath);
		} catch (XMLException &e) {
			// Show error and clear logger and unset _project
			QErrorMessage *errormsg = QErrorMessage::qtHandler();
			errormsg->showMessage(e.what());
			_project = 0;
			Logger::instance()->clearmessages();
			return;
		}

		Logger::instance()->clearmessages();
		Logger::instance()->setStatus(false);
		Project::currentProjectPath = projectpath;

		_projectWindow = new ProjectWindow(this, _project);
		connectProjectActions();
		setCentralWidget(_projectWindow);
	}
}

void InterfaceModule::saveSlot() {
	if (_project) {
		if (Project::currentProjectPath == QString()) {
			QString projectPath = QString();
			projectPath = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("RPGMaker files (*.rpgm)"));

			if (projectPath != QString()) {
				Project::currentProjectPath = projectPath;
			} else {
				return;
			}
		}

		XMLModule xmlmodule;
		try {
			xmlmodule.writeProject(_project, Project::currentProjectPath);
		} catch (XMLException &e) {
			QErrorMessage *errorDialog = QErrorMessage::qtHandler();
			errorDialog->showMessage(e.what());
		}

		Logger::instance()->flush();
	}
}

void InterfaceModule::saveAsSlot() {
	if (_project) {
		QString savePath = QString();
		savePath = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("RPGMaker files (*.rpgm)"));

		if (savePath != QString()) {
			XMLModule xmlmodule;
			try {
				xmlmodule.writeProject(_project, savePath);
			} catch (XMLException &e) {
				QErrorMessage *errorDialog = QErrorMessage::qtHandler();
				errorDialog->showMessage(e.what());
			}
		}

		Logger::instance()->flush();
	}
}

void InterfaceModule::aboutSlot() {
	QMessageBox::about(this, tr("About RPGMaker"),
		tr("<b>RPGMaker</b> is a program to create your own role playing games. "
			"It is created as the bachelor thesis by Jan Discart."));
}

void InterfaceModule::createActions() {
	// Program actions
	_newAction = new QAction(QIcon(g_iconPath + "doc_new_icon&24.png"), tr("&New"), this);
	_newAction->setShortcuts(QKeySequence::New);
	_newAction->setToolTip(tr("Create a new project"));
	connect(_newAction, SIGNAL(triggered()), this, SLOT(newProjectSlot()));

	_openAction = new QAction(QIcon(g_iconPath + "folder_open_icon&24.png"), tr("&Open"), this);
	_openAction->setShortcuts(QKeySequence::Open);
	_openAction->setToolTip(tr("Open an existing project"));
	connect(_openAction, SIGNAL(triggered()), this, SLOT(openSlot()));

	_saveAction = new QAction(QIcon(g_iconPath + "save_icon&24.png"), tr("&Save"), this);
	_saveAction->setShortcuts(QKeySequence::Save);
	_saveAction->setToolTip(tr("Write changes to project"));
	connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveSlot()));

	_saveAsAction = new QAction(QIcon(g_iconPath + "save_icon&24.png"), tr("Save &As"), this);
	_saveAsAction->setShortcuts(QKeySequence::SaveAs);
	_saveAsAction->setToolTip(tr("Save project under a different name"));
	connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsSlot()));

	_exitAction = new QAction(QIcon(g_iconPath + "on-off_icon&24.png"), tr("E&xit"), this);
	_exitAction->setShortcuts(QKeySequence::Quit);
	_exitAction->setToolTip(tr("Close the application"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

	// Project actions
	_renameAction = new QAction(QIcon(g_iconPath + "text_letter_t_icon&24.png"), tr("Rename"), this);
	_renameAction->setToolTip(tr("Rename project"));
	_renameAction->setDisabled(true);
	connect(_renameAction, SIGNAL(triggered()), _projectWindow, SLOT(renameSlot()));

	_propertiesAction = new QAction(QIcon(g_iconPath + "wrench_plus_2_icon&24.png"), tr("Properties"), this);
	_propertiesAction->setToolTip(tr("Manage settings"));
	_propertiesAction->setDisabled(true);
	connect(_propertiesAction, SIGNAL(triggered()), _projectWindow, SLOT(propertiesSlot()));

	_charactersAndEnemiesAction = new QAction(QIcon(g_iconPath + "users_icon&24.png"), tr("Characters and Enemies"), this);
	_charactersAndEnemiesAction->setToolTip(tr("Manage characters and enemies"));
	_charactersAndEnemiesAction->setDisabled(true);
	connect(_charactersAndEnemiesAction, SIGNAL(triggered()), _projectWindow, SLOT(charactersAndEnemiesSlot()));

	_objectsAndEventsAction = new QAction(QIcon(g_iconPath + "box_icon&24.png"), tr("Objects and Events"), this);
	_objectsAndEventsAction->setToolTip(tr("Manage objects and events"));
	_objectsAndEventsAction->setDisabled(true);
	connect(_objectsAndEventsAction, SIGNAL(triggered()), _projectWindow, SLOT(objectsAndEventsSlot()));

	_tilesetsAndEnvironmentsAction = new QAction(QIcon(g_iconPath + "globe_2_icon&24.png"), tr("Tilesets and Environments"), this);
	_tilesetsAndEnvironmentsAction->setToolTip(tr("Manage environments"));
	_tilesetsAndEnvironmentsAction->setDisabled(true);
	connect(_tilesetsAndEnvironmentsAction, SIGNAL(triggered()), _projectWindow, SLOT(tilesetsAndEnvironmentsSlot()));

	_playAction = new QAction(QIcon(g_iconPath + "playback_play_icon&24.png"), tr("Play"), this);
	_playAction->setToolTip(tr("Play your project"));
	_playAction->setDisabled(true);
	connect(_playAction, SIGNAL(triggered()), _projectWindow, SLOT(playSlot()));

	// Help actions
	_aboutAction = new QAction(QIcon(g_iconPath + "book_side_icon&24.png"), tr("&About"), this);
	_aboutAction->setToolTip(tr("More information about RPGMaker"));
	connect(_aboutAction, SIGNAL(triggered()), this, SLOT(aboutSlot()));
}

void InterfaceModule::createMenus() {
	_programMenu = menuBar()->addMenu(tr("Program"));
	_programMenu->addAction(_newAction);
	_programMenu->addAction(_openAction);
	_programMenu->addAction(_saveAction);
	_programMenu->addAction(_saveAsAction);
	_programMenu->addSeparator();
	_programMenu->addAction(_exitAction);

	menuBar()->addSeparator();
	_projectMenu = menuBar()->addMenu(tr("Project"));
	_projectMenu->addAction(_renameAction);
	_projectMenu->addSeparator();
	_projectMenu->addAction(_propertiesAction);
	_projectMenu->addAction(_charactersAndEnemiesAction);
	_projectMenu->addAction(_tilesetsAndEnvironmentsAction);
	_projectMenu->addAction(_objectsAndEventsAction);
	_projectMenu->addSeparator();
	_projectMenu->addAction(_playAction);

	menuBar()->addSeparator();
	_helpMenu = menuBar()->addMenu(tr("&Help"));
	_helpMenu->addAction(_aboutAction);
}

void InterfaceModule::createToolbars() {
	_programToolbar = addToolBar(tr("Program"));
	_programToolbar->addAction(_newAction);
	_programToolbar->addAction(_openAction);
	_programToolbar->addAction(_saveAction);

	_projectToolbar = addToolBar(tr("Project"));
	_projectToolbar->addAction(_propertiesAction);
	_projectToolbar->addAction(_charactersAndEnemiesAction);
	_projectToolbar->addAction(_tilesetsAndEnvironmentsAction);
	_projectToolbar->addAction(_objectsAndEventsAction);
	_projectToolbar->addSeparator();
	_projectToolbar->addAction(_playAction);
}

void InterfaceModule::createStatusbar() {
	statusBar()->showMessage(tr("Welcome to RPGMaker"));
}

void InterfaceModule::connectProjectActions() {
	connect(_renameAction, SIGNAL(triggered()), _projectWindow, SLOT(renameSlot()));
	connect(_propertiesAction, SIGNAL(triggered()), _projectWindow, SLOT(propertiesSlot()));
	connect(_charactersAndEnemiesAction, SIGNAL(triggered()), _projectWindow, SLOT(charactersAndEnemiesSlot()));
	connect(_objectsAndEventsAction, SIGNAL(triggered()), _projectWindow, SLOT(objectsAndEventsSlot()));
	connect(_tilesetsAndEnvironmentsAction, SIGNAL(triggered()), _projectWindow, SLOT(tilesetsAndEnvironmentsSlot()));
	connect(_playAction, SIGNAL(triggered()), _projectWindow, SLOT(playSlot()));

	_renameAction->setEnabled(true);
	_propertiesAction->setEnabled(true);
	_charactersAndEnemiesAction->setEnabled(true);
	_objectsAndEventsAction->setEnabled(true);
	_tilesetsAndEnvironmentsAction->setEnabled(true);
	_playAction->setEnabled(true);
}

void InterfaceModule::disconnectProjectActions() {
	disconnect(_renameAction, 0, _projectWindow, 0);
	disconnect(_propertiesAction, 0, _projectWindow, 0);
	disconnect(_charactersAndEnemiesAction, 0, _projectWindow, 0);
	disconnect(_objectsAndEventsAction, 0, _projectWindow, 0);
	disconnect(_tilesetsAndEnvironmentsAction, 0, _projectWindow, 0);
	disconnect(_playAction, 0, _projectWindow, 0);

	_renameAction->setEnabled(false);
	_propertiesAction->setEnabled(false);
	_charactersAndEnemiesAction->setEnabled(false);
	_objectsAndEventsAction->setEnabled(false);
	_tilesetsAndEnvironmentsAction->setEnabled(false);
	_playAction->setEnabled(false);
}

bool InterfaceModule::closeProject() {
	if (!maybeSave())
		return false;

	disconnectProjectActions();

	if (!!_projectWindow)
		delete _projectWindow;

	if (!!_project)
		delete _project;

	_projectWindow = 0;
	_project = 0;
	Project::currentProjectPath = QString();

	return true;
}

bool InterfaceModule::maybeSave() {
	Logger *logger = Logger::instance(); 
	if (logger->getStatus()) {
		QMessageBox savebox;
		savebox.setText("The project has been modified.");
		savebox.setInformativeText("Do you want to save your changes before closing the project?");
		savebox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		savebox.setDefaultButton(QMessageBox::Save);
		savebox.setIcon(QMessageBox::Question);

		switch (savebox.exec()) {
		case QMessageBox::Save:
			saveSlot();
			return true;
			break;
		case QMessageBox::Discard:
			return true;
			break;
		case QMessageBox::Cancel:
			return false;
			break;
		}
	}

	return true;
}