#ifndef INTERFACEMODULE_H
#define INTERFACEMODULE_H

#include "qmainwindow.h"
#include "../Headers/ProjectWindow.h"

namespace interfacemodule {

	class InterfaceModule : public QMainWindow {

		Q_OBJECT

	public:
		InterfaceModule();
		virtual ~InterfaceModule();

	private slots:
		void newProjectSlot();
		void openSlot();
		void saveSlot();
		void saveAsSlot();
		void aboutSlot();

	private:
		void createActions();
		void createMenus();
		void createToolbars();
		void createStatusbar();
		void connectProjectActions();
		void disconnectProjectActions();
		bool closeProject();
		bool maybeSave();

		Project *_project;
		ProjectWindow *_projectWindow;

		QMenu *_programMenu;
		QMenu *_projectMenu;
		QMenu *_helpMenu;
		
		QToolBar *_programToolbar;
		QToolBar *_projectToolbar;
		
		QAction *_newAction;
		QAction *_openAction;
		QAction *_saveAction;
		QAction *_saveAsAction;
		QAction *_exitAction;
		QAction *_renameAction;
		QAction *_propertiesAction;
		QAction *_charactersAndEnemiesAction;
		QAction *_objectsAndEventsAction;
		QAction *_tilesetsAndEnvironmentsAction;
		QAction *_playAction;
		QAction *_aboutAction;
	};

}

#endif